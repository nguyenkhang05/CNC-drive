/*************************************************************************
 *
 *  NSMVG4PEV01.cpp
 *
 * Copyright(c) 2016-2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

/* $Id:$ */

#include "systemc.h"
#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno

#include "Common.h"
#include "CmErrMsg.h" // for WinMsg

#include "NSMVG4PEV01.h"
#include "NSMVG4CPUV01.h"
#include "G4_Intc1Wrapper.h"
#include "NSMVREGV01.h"
#include "BACKWARD_DECODER.h"
#include "ATLTSLAVE32.h"
#include "OSCI2DCDR_WITH_ERRSIG.h"

using namespace CmErrMsg; // for WinMsg
using namespace G4IntcTyp;  // for INTC's signal

extern int           token_to_int(char*, const char*, const char*);
extern unsigned int  token_to_uint(char*, const char*, const char*);
extern unsigned long token_to_ulong(char*, const char*, const char*);
extern unsigned long long token_to_ulonglong(char*, const char*, const char*);
extern double        token_to_double(char*, const char*, const char*);
extern unsigned int  token_to_uint_default( char *, unsigned int, const char *, char * );
extern unsigned long long token_to_ulonglong_default(char*, unsigned long long , const char*, const char*);


// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream dfout;
#endif // DEBUG_MESSAGE_OUTPUT

///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<param name="config_file">the pointer of configulation file</param>
///<returns>none</returns>
NSMVG4PEV01::
NSMVG4PEV01(sc_module_name module_name, const char *config_file ) : 
  sc_module(module_name),
  CL_sys_clk(),
  VCI_clk("VCI_clk"),
  SYS_BUS_clk("SYS_BUS_clk"),
  From_clk("From_clk"),
  Cram_clk("Cram_clk"),
  sys_reset("sys_reset"),
  ish("ish"),
  CL_ish(),
  CL_isgp(),
  isgp("isgp"),
  isgc("isgc"),
  tsf("tsf"),
  tss("tss"),
  tscr("tscr"),
  PE_eiint(),
  PE_eiint_type(),
  PE_feint(),
  PE_feint_type(),
  fenmi("fenmi"),
  eiint(),
  intack(),
  INTC1(),
  GVCI_BW_DECODER(0),
  GVCI_DECODER(0),
  cpugminfo_sig(),
  mConfigFile(config_file),
  G4CPU(0),
  mPeNum(1),
  mClNum(1),
  mMapFileName(),
  GC_DUMMY_SLAVE(),
  GP_DUMMY_SLAVE(),
  HB_DUMMY_SLAVE(),
  I1_DUMMY_SLAVE(),
  eiint_sig(),
  intack_sig(),
  fenmirq_sig(),
  fenmiack_sig(),
  feintrq_sig(),
  feintack_sig()
{
  char tname[64];
  char name[64];
  unsigned int i, j, k;

  read_config_file(mConfigFile); 

  G4CPU     = new NSMVG4CPUV01("G4CPU", mConfigFile);

  // Create bus
  GVCI_BW_DECODER = new BACKWARD_DECODER<BUS_WIDTH_VCI32>( "GVCI_BW_DECODER" );
  GVCI_DECODER = new OSCI2DCDR_WITH_ERRSIG<BUS_WIDTH_VCI32>( "GVCI_DECODER",
							     mMapFileName );

  // Create sockets
  for ( i = 0; i < mClNum; i++ ) {
    // for ish
    sprintf(tname, "CL%d_ish", i);
    CL_ish[i] = new TlmInitiatorSocket<BUS_WIDTH_VCI>(tname);
    // for isgp
    sprintf(tname, "CL%d_isgp", i);
    CL_isgp[i] = new TlmInitiatorSocket<BUS_WIDTH_VCI>(tname);
    // for isl
  }

  // Create Local Peripherals
  ADDRESS_TYPE s_addr = NSMVG4PEV01_INTC1_SELF_ADDRESS;
  for( i=0; i<mPeNum; i++ ){
    // for INTC1
    sprintf( name, "INTC1_%d", i );
    INTC1[i]   = new G4_Intc1Wrapper( name, config_file, i );
    s_addr = s_addr + NSMVG4PEV01_INTC1_OFFS_ADDRESS;
    INTC1[i]->ts->setBaseAddressSize(s_addr, NSMVG4PEV01_INTC1_OFFS_ADDRESS);
  }

  // Create Ports
  for( i=0; i<CL_MAX_NUM; i++ ){
    // for PE(cluster's clock
    sprintf(tname, "CL%d_sys_clk", i);
    CL_sys_clk[i] = new sc_in<sc_dt::uint64>(tname);
    (*((G4CPU->CL_sys_clk)[i]))(*(CL_sys_clk[i]));
  }
  for( i=0; i<PE_MAX_NUM; i++ ){
    // for PE's eiint
    for ( j = 0; j < INTC1_CH_NUM; j++ ) {
      // for PE's eiint port
      sprintf( name, "PE%d_eiint%d", i, j );
      PE_eiint[i][j] = new sc_in<bool>( name );
      // for PE's eiint_type port
      sprintf( name, "PE%d_eiint_type%d", i, j );
      PE_eiint_type[i][j] = new sc_in<bool>( name );
    }
    // for PE's feint
    for ( j = 0; j < FE_INTC1_CH_NUM; j++ ) {
      // for PE's feint port
      sprintf( name, "PE%d_feint%d", i, j );
      PE_feint[i][j] = new sc_in<bool>( name );
      // for PE's feint_type port
      sprintf( name, "PE%d_feint_type%d", i, j );
      PE_feint_type[i][j] = new sc_in<bool>( name );
    }
  }

  GVCI_DECODER->is( isgc );
  GVCI_BW_DECODER->is( GVCI_DECODER->ts );
  // GVCI_DECODER->freqPort is bound in G4SS.
  // GVCI_DECODER->resperr  is bound in G4SS.
  GVCI_DECODER->resetPort( sys_reset );
  for( i=0; i<CL_MAX_NUM; i++ ){
    if ( i < mClNum ) {
      G4CPU->CL_isgc[i]->bind( GVCI_BW_DECODER->ts );
      G4CPU->CL_ish[i]->bind( *(CL_ish[i]) );
      G4CPU->CL_isgp[i]->bind( *(CL_isgp[i]) );
    }
    else {
      sprintf( name, "GC_DUMMY_SLAVE_%d", i );
      GC_DUMMY_SLAVE[i] = new DummySlave<BUS_WIDTH_VCI32>( name );
      G4CPU->CL_isgc[i]->bind( GC_DUMMY_SLAVE[i]->ts );
      sprintf( name, "GP_DUMMY_SLAVE_%d", i );
      GP_DUMMY_SLAVE[i] = new DummySlave<BUS_WIDTH_VCI>( name );
      G4CPU->CL_isgp[i]->bind( GP_DUMMY_SLAVE[i]->ts );
      sprintf( name, "HB_DUMMY_SLAVE_%d", i );
      HB_DUMMY_SLAVE[i] = new DummySlave<BUS_WIDTH_VCI>( name );
      G4CPU->CL_ish[i]->bind( HB_DUMMY_SLAVE[i]->ts );
    }
  }

  for( i=0; i<PE_MAX_NUM; i++ ){
    if ( i < mPeNum ) {
      G4CPU->PE_isi[i]->bind( *(INTC1[i]->ts) );
    }
    else{
      sprintf( name, "I1_DUMMY_SLAVE_%d", i );
      I1_DUMMY_SLAVE[i] = new DummySlave<BUS_WIDTH_VCI32>( name );
      G4CPU->PE_isi[i]->bind( I1_DUMMY_SLAVE[i]->ts );
    }
  }

  for ( j = 0; j < INTC2_MAX_NUM; j++ ) {
    for ( i = 0; i < PE_MAX_NUM; i++ ) {
      sprintf( name, "i2%d_g%d_eiint", j, i );
      eiint[j][i] = new sc_in<uint64>( name );
    }
  }

  for ( j = mPeNum; j < PE_MAX_NUM; j++ ) {
    sprintf( name, "i1_p%d_intack", j );
    intack[j] = new sc_out<unsigned int>( name );
  }

  // Create sc_signals
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    // for INTC1-CPU connection
    sprintf( name, "PE%d_eiint_sig", j );
    eiint_sig[j]    = new sc_signal<uint64>( name );
    sprintf( name, "PE%d_intack_sig", j );
    intack_sig[j]   = new sc_signal<unsigned int>( name );
    sprintf( name, "PE%d_feintrq_sig", j );
    feintrq_sig[j]  = new sc_signal<unsigned int>( name );
    sprintf( name, "PE%d_feintack_sig", j );
    feintack_sig[j] = new sc_signal<unsigned int>( name );
    sprintf( name, "PE%d_fenmirq_sig", j );
    fenmirq_sig[j]  = new sc_signal<bool>( name );
    sprintf( name, "PE%d_fenmiack_sig", j );
    fenmiack_sig[j] = new sc_signal<bool>( name );
    sprintf( name, "PE%d_cpugminfo_sig", j );
    cpugminfo_sig[j] = new SC_SIGNAL(unsigned int)( name );
  }

  G4CPU->ish(ish);
  G4CPU->isgp(isgp);

  tsf(G4CPU->tsf);
  tss(G4CPU->tss);
  tscr(G4CPU->tscr);

  G4CPU->VCI_clk(VCI_clk);
  G4CPU->Sys_bus_clk(SYS_BUS_clk);
  G4CPU->From_clk(From_clk);
  G4CPU->Cram_clk(Cram_clk);
  G4CPU->sys_reset(sys_reset);

  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    G4CPU->PE_FenmiReq[j]->bind( *fenmirq_sig[j] );
    G4CPU->PE_FenmiAck[j]->bind( *fenmiack_sig[j] );
    G4CPU->PE_EiintReq[j]->bind( *eiint_sig[j] );
    G4CPU->PE_EiintAck[j]->bind( *intack_sig[j] );
    G4CPU->PE_FeintReq[j]->bind( *feintrq_sig[j] );
    G4CPU->PE_FeintAck[j]->bind( *feintack_sig[j] );
    G4CPU->PE_CpuGmInfo[j]->bind( *cpugminfo_sig[j] );
  }

  for ( k = 0; k < mPeNum; k++ ) {
    // connect INTC1-System
    INTC1[k]->resetz( sys_reset );
    INTC1[k]->clk( *CL_sys_clk[k/2] );

    // connect INTC1-External
    for ( i = 0; i < INTC1_CH_NUM; i++ ) {
      INTC1[k]->eiintl[i]->bind( *PE_eiint[k][i] );
      INTC1[k]->eiint_type[i]->bind( *PE_eiint_type[k][i] );
    }
    INTC1[k]->fenmi( fenmi );
    for ( i = 0; i < FE_INTC1_CH_NUM; i++ ) {
      (*INTC1[k]->feint[i])( (*PE_feint[k][i]) );
      (*INTC1[k]->feint_type[i])( (*PE_feint_type[k][i]) );
    }
    // connect INTC1-CPU
    INTC1[k]->fenmirq_1l( *fenmirq_sig[k] );
    INTC1[k]->fenmiack_1l( *fenmiack_sig[k] );
    INTC1[k]->t_eiint_1l( *eiint_sig[k] );
    INTC1[k]->t_intack_1l( *intack_sig[k] );
    INTC1[k]->t_feintrq_1l( *feintrq_sig[k] );
    INTC1[k]->t_feintack_1l( *feintack_sig[k] );
    INTC1[k]->cpu_gm_info( *cpugminfo_sig[k] );
    
    // connect INTC1-INTC2
    intack[k] = INTC1[k]->i1_t_intack;
    for ( j = 0; j < INTC2_MAX_NUM; j++ ) {
      INTC1[k]->i2_g_eiint->bind( *eiint[j][k] );
    }
  }
}


///<summary>Destructor</summary>
///<returns>none</returns>
NSMVG4PEV01::
~NSMVG4PEV01(void){

  delete G4CPU;

  for ( unsigned int i = 0; i < PE_MAX_NUM; i++ ) {

    if( i < mPeNum ){
      delete INTC1[i];
    }

    for ( int j = 0; j < FE_INTC1_CH_NUM; j++ ) {
      delete PE_feint[i][j];
      delete PE_feint_type[i][j];
    }

    for ( int j = 0; j < INTC1_CH_NUM; j++ ) {
      delete PE_eiint[i][j];
      delete PE_eiint_type[i][j];
    }

    if( i >= mPeNum )
      delete intack[i];
    delete cpugminfo_sig[i];
    delete eiint_sig[i];
    delete intack_sig[i];
    delete feintrq_sig[i];
    delete feintack_sig[i];
    delete fenmirq_sig[i];
    delete fenmiack_sig[i];

  }

  for ( unsigned int i = 0; i < CL_MAX_NUM; i++ ) {
    delete CL_sys_clk[i];
    if( 1 < mClNum ){
      delete CL_ish[i];
      delete CL_isgp[i];
    }
    else{
      delete GC_DUMMY_SLAVE[i];
      delete GP_DUMMY_SLAVE[i];
      delete HB_DUMMY_SLAVE[i];
    }
  }

  for ( int j = 0; j < INTC2_MAX_NUM; j++ ) {
    for ( int i = 0; i < PE_MAX_NUM; i++ ) {
      delete eiint[j][i];
    }
  }
}


/**
 * for reading the configulation file and initialization
 */
///<summary>for reading the configulation file and initialization</summary>
///<param name="filename">the pointer of configulation file</param>
///<returns>none</returns>
void NSMVG4PEV01::read_config_file( const char *filename )
{
  char    word[512];
  char    seps[] = " ,=();\t\n\r";
  char    *token;
  bool    error_detected = false;

  ifstream  config_file(filename);
  // read the config file
  while(1) {
    config_file.getline(word, 512, '\n');
    token = strtok(word, seps);

    // ----------------------------------------
    // For EOF
    if (config_file.eof()) {
      break;
    }

    // ----------------------------------------
    // For comment
    if ((token == NULL) || (strncmp(token, "//", 2) == 0)) {
      continue;
    }

    // ----------------------------------------
    // detect end marker
    // NOTICE:You have to check whether token is NULL first.
    if (strcmp(token, "[END]") == 0) {
      break;
    }

    // ----------------------------------------
    // get token
    // ----------------------------------------
    // For miscellaneous
    if ( 0 == strcmp(token, "[PE_NUM]") ) {
      token = strtok( 0, seps );
      if (token != NULL) {
        mPeNum = token_to_uint( token,
                                "name()",
                                "[PE_NUM]" );
        mClNum = (mPeNum / 2) + (mPeNum % 2);
#if defined(PLTFRM_TST)
        cout << this->name() << "->mPeNum = " << mPeNum << endl;
        cout << this->name() << "->mClNum = " << mClNum << endl;
#endif //PLTFRM_TST
        if(( mPeNum > PE_MAX_NUM ) || ((mPeNum!=1) && ((mPeNum%2)!=0)) || (mPeNum==0)){
          char msg[1024];
          // for WinMsg
          sprintf( msg,
                   "[PE_NUM] must be 1, 2, 4, 6, or 8, but specified [%d].",
                   mPeNum);
          printErrorMessage( name(), "read_config_file", msg );
          exit(1);
        }
      }
      continue;
    }
    
    // bus map file name
    if( strcmp( token, "[BUS_MAP_FILE]" ) == 0 ) {
      token = strtok( 0, seps );
      const char *fname = (!token) ? "" : token;
      strcpy( mMapFileName, fname );
      continue;
    }

  }  // end of configuration file read loop(infinite)
}  // end of read_config_file()


///<summary>get the pointer of NSMVG4CPUV01 module</summary>
///<returns>the pointer of NSMVG4CPUV01 module</returns>
NSMVG4CPUV01* NSMVG4PEV01::getG4cpuPtr( void )
{
  return(G4CPU);
}


///<summary>for generate VCD file</summary>
///<param name="tf">the pointer of sc_trace_file</param>
///<returns>none</returns>
void
NSMVG4PEV01::setTraceEnable( sc_trace_file* tf )
{
  unsigned int i, j;

  for ( i = 0; i < mPeNum; i++ ) {
    INTC1[i]->setTraceEnable( tf );
  }

  for ( j = 0; j < INTC2_MAX_NUM; j++ ) {
    for ( i = 0; i < PE_MAX_NUM; i++ ) {
      sc_trace( tf, *eiint[j][i], eiint[j][i]->name() );
    }
  }

  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    sc_trace( tf, *intack[j], intack[j]->name() );
  }
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
