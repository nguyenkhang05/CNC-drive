/*************************************************************************
 *
 *  NSMVG4SSV01.cpp
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

#include "systemc.h"
#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno

#include "CmErrMsg.h" // for WinMsg
#include "Common.h"

#include "NSMVG4SSV01.h"
#include "NSMVG4PEV01.h"
#include "NSMVG4CPUV01.h"
#include "G4_Intc2Wrapper.h"
#include "I_CONNECTOR.h"
#include "G4_IPIR.h"
#include "G4_OR_IPIR.h"
#include "NSMVMEVV02.h"
#include "NSMVBARRV01.h"
#include "NSMVTPTMV01.h"
#include "ATLTSLAVE32.h"
#include "OSCI2DCDR_WITH_ERRSIG.h"
#include "APISS.h"
#include "AXISS_E2.h"
#include "AXI2VCI.h"
#include "VCI2AXI.h"
#include "VCI2APB.h"
#include "BACKWARD_DECODER.h"
#include "IBG.h"
#include "IBG_U2.h"
#include "INTC2G.h"
#include "INTC2G_U2.h"

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT

using namespace CmErrMsg; // for WinMsg

extern int           token_to_int(char*, const char*, const char*);
extern unsigned int  token_to_uint(char*, const char*, const char*);
extern unsigned long token_to_ulong(char*, const char*, const char*);
extern unsigned long long token_to_ulonglong(char*, const char*, const char*);
extern double        token_to_double(char*, const char*, const char*);
extern unsigned int  token_to_uint_default( char *, unsigned int, const char *, char * );
extern unsigned long long token_to_ulonglong_default(char*, unsigned long long , const char*, const char*);

#define IBG_NUM 4
#define GUARD_BARR_CH_NUM 17
#define GUARD_IPIR_CH_NUM 5
#define GUARD_MEV_CH_NUM  8
#define GUARD_TPTM_CH_NUM 2
#define GUARD_INTC2_U2_CH_NUM INTC2_CH_MAX_NUM+2

/// <summary>constructor</summary>
/// <param name="module_name">module mame</param>
/// <param name="config_file">configration file name</param>
NSMVG4SSV01::
NSMVG4SSV01(sc_module_name module_name, const char *config_file ):
  sc_module(module_name),
  mConfigFile(config_file),
  H_BUS( 0 ),
  P_BUS( 0 ),
  BW_DECODER( 0 ),
  GAPB_PFSS( 0 ),
  GAPB_SG1( 0 ),
  GAPB_SG2_0( 0 ),
  GAPB_SG2_1( 0 ),
  GAPB_SG3( 0 ),
  GAPB_SG4( 0 ),
  GAPB_SG5( 0 ),
  GAPB_SG6( 0 ),
  GAPB_SG7( 0 ),
  GAPB_SG8( 0 ),
  GAPB_SG9( 0 ),
  AXI2VCI_BRIDGE( 0 ),
  VCI2APB_BRIDGE_PFSS( 0 ),
  VCI2APB_BRIDGE_SG1( 0 ),
  VCI2APB_BRIDGE_SG2_0( 0 ),
  VCI2APB_BRIDGE_SG2_1( 0 ),
  VCI2APB_BRIDGE_SG3( 0 ),
  VCI2APB_BRIDGE_SG4( 0 ),
  VCI2APB_BRIDGE_SG5( 0 ),
  VCI2APB_BRIDGE_SG6( 0 ),
  VCI2APB_BRIDGE_SG7( 0 ),
  VCI2APB_BRIDGE_SG8( 0 ),
  VCI2APB_BRIDGE_SG9( 0 ),
  CL_VCI2AXI_BRIDGE(),
  CL_sys_clk(),
  GAPB_PFSS_clk("GAPB_PFSS_clk"),
  GAPB_SG1_clk("GAPB_SG1_clk"),
  GAPB_SG2_0_clk("GAPB_SG2_0_clk"),
  GAPB_SG2_1_clk("GAPB_SG2_1_clk"),
  GAPB_SG3_clk("GAPB_SG3_clk"),
  GAPB_SG4_clk("GAPB_SG4_clk"),
  GAPB_SG5_clk("GAPB_SG5_clk"),
  GAPB_SG6_clk("GAPB_SG6_clk"),
  GAPB_SG7_clk("GAPB_SG7_clk"),
  GAPB_SG8_clk("GAPB_SG8_clk"),
  GAPB_SG9_clk("GAPB_SG9_clk"),
  VCI_clk("VCI_clk"),
  AXI_clk("AXI_clk"),
  From_clk("From_clk"),
  Cram_clk("Cram_clk"),
  SYS_BUS_clk("SYS_BUS_clk"),
  sys_reset("sys_reset"),
  fenmi("fenmi"),
  PE_feint(),
  PE_feint_type(),
  PE_eiint(),
  PE_eiint_type(),
  eiint(),
  eiint_type(),
  tsx("tsx"),
  tsf("tsf"),
  isx("isx"),
  isg_sg1("isg_sg1"),
  isg_sg2_0("isg_sg2_0"),
  isg_sg2_1("isg_sg2_1"),
  isg_sg3("isg_sg3"),
  isg_sg4("isg_sg4"),
  isg_sg5("isg_sg5"),
  isg_sg6("isg_sg6"),
  isg_sg7("isg_sg7"),
  isg_sg8("isg_sg8"),
  isg_sg9("isg_sg9"),
  G4PE( 0 ),
  INTC2( 0 ),
  IPIR( 0 ),
  OR_IPIR( 0 ),
  MEV( 0 ),
  BARR(0),
  TPTM(0),
  GUARD_TPTM(0),
  GUARD_BARR(0),
  GUARD_IPIR(0),
  GUARD_MEV(0),
  GUARD_TPTM_U2(0),
  GUARD_BARR_U2(0),
  GUARD_IPIR_U2(0),
  GC_DUMMY_SLAVE(),
  i2_g_eiint_sig(),
  ipir_sig(),
  i2_bcast_sig(),
  i1_p_intack_sig(),
  ipir2eiint_sig(),
  ipir_init_ch_sig(),
  ipir_ext_err_dummy_sig("ipir_ext_err_dummy_sig"),
  swdt_clk_sig(),
  sys_cpinit_sig(),
  pe_fli_areaerr_sig(),
  pe_cram_areaerr_sig(),
  pe_peg_err_m_sig(),
  pe_peg_err_s_sig(),
  exm_fli_areaerr_sig( 0 ),
  exm_cram_areaerr_sig( 0 ),
  cram_grderr_sig( 0 ),
  pe_fli_grderr_sig(),
  exm_fli_grderr_sig( 0 ),
  gvci_areaerr_sig(0),
  guard_barr_err_irq_sig( 0 ),
  guard_ipir_err_irq_sig( 0 ),
  guard_mev_err_irq_sig( 0 ),
  guard_tptm_err_irq_sig( 0 ),
  mpTf( 0 ),
  mMapFileName(),
  mPeNum( 1 ),
  mClNum( 1 ),
  mIntc2Num( 1 ),
  mIssMode( SIM_MODE_CA ),
  mpModName( module_name ),
  mPfssType( E2X_PFSS ),
  mGuardBarrChNum( 0 ),
  mGuardIpirChNum( 0 ),
  mGuardMevChNum( 0 ),
  mGuardTptmChNum( 0 )
{

  dmsgsc( mpModName << ".NSMVG4SSV01()" );
  char tname[64];
  unsigned int  i, j, k;

  // config parameters with some default values

  // ----------------------------------------
  // miscellaneous

  // ----------------------------------------
  // For G4PE setting
  

  // ----------------------------------------
  // When error detected in read_config_file() process,
  // command will be quit immediately.
  read_config_file(config_file);

  //----- instantiation pointer -----
  // ----------------------------------------
  G4PE = new NSMVG4PEV01("G4PE", mConfigFile);

#ifdef DEBUG_MESSAGE_OUTPUT
  dfopen("dmsg.txt");
  mpTf = sc_create_vcd_trace_file("wave");
  G4PE->setTraceEnable( mpTf );
#endif // DEBUG_MESSAGE_OUTPUT

  INTC2 = new G4_Intc2Wrapper*[mIntc2Num];
  for ( i = 0; i < mIntc2Num; i++ ) {
    sprintf( tname, "INTC2_%d", i );
    INTC2[i] = new G4_Intc2Wrapper( tname, mConfigFile );
    sprintf( tname, "GUARD_INTC2_%d", i );
    if( mPfssType == U2A_PFSS ){
      GUARD_INTC2_U2[i] = new INTC2G_U2( tname, 0, 0, GUARD_INTC2_U2_CH_NUM,  mIssMode );
      GUARD_INTC2[i] = 0;
    }
    else{
      GUARD_INTC2[i] = new INTC2G( tname, 0, 0, mIssMode );
      GUARD_INTC2_U2[i] = 0;
    }
    sprintf( tname, "guard_intc2%d_err_irq_sig", i );
    guard_intc2_err_irq_sig[i] = new sc_signal<bool>( tname );
  }
  for ( i = mIntc2Num; i < INTC2_MAX_NUM; i++ ) {
    GUARD_INTC2[i] = 0;
    GUARD_INTC2_U2[i] = 0;
    guard_intc2_err_irq_sig[i] = 0;
  }

  // for BARR,IPIR,MEV
  if ( 1 < mPeNum ) {
    BARR = new NSMVBARRV01( "BARR", mConfigFile );
    IPIR = new G4_IPIR( "IPIR", mConfigFile, 0, 0 );
    if( mGuardBarrChNum == 0 ){
      mGuardBarrChNum = GUARD_BARR_CH_NUM;
    }
    if( mPfssType == U2A_PFSS ){
      if( mGuardIpirChNum == 0 ){
	mGuardIpirChNum = 2 * mPeNum + 1;
      }
      GUARD_BARR_U2 = new IBG_U2( "GUARD_BARR", "BRG", mGuardBarrChNum, 0, 0, mIssMode );
      GUARD_IPIR_U2 = new IBG_U2( "GUARD_IPIR", "IPIG", mGuardIpirChNum, 0, 0, mIssMode );
    }
    else{
      if( mGuardIpirChNum == 0 ){
	mGuardIpirChNum = GUARD_IPIR_CH_NUM;
      }
      if( mGuardMevChNum == 0 ){
	mGuardMevChNum = GUARD_MEV_CH_NUM;
      }
      MEV  = new NSMVMEVV02( "MEV", mConfigFile );
      GUARD_BARR = new IBG( "GUARD_BARR", mGuardBarrChNum, 0, 0, mIssMode );
      GUARD_IPIR = new IBG( "GUARD_IPIR", mGuardIpirChNum, 0, 0, mIssMode );
      GUARD_MEV  = new IBG( "GUARD_MEV", mGuardMevChNum, 0, 0, mIssMode );
      guard_mev_err_irq_sig  = new sc_signal<bool>( "guard_mev_err_irq_sig" );
    }
    guard_barr_err_irq_sig = new sc_signal<bool>( "guard_barr_err_irq_sig" );
    guard_ipir_err_irq_sig = new sc_signal<bool>( "guard_ipir_err_irq_sig" );
  }
  else {
    // do nothing
  }
  OR_IPIR = new G4_OR_IPIR*[PE_MAX_NUM];
  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    sprintf( tname, "OR_IPIR_%d", i );
    OR_IPIR[i] = new G4_OR_IPIR( tname );
  }
  //for TPTM
  TPTM = new NSMVTPTMV01("TPTM", mConfigFile);
  if( mPfssType == U2A_PFSS ){
    if( mGuardTptmChNum == 0 ){
      mGuardTptmChNum = mPeNum + 2;
    }
    GUARD_TPTM_U2 = new IBG_U2( "GUARD_TPTM", "TPTG", mGuardTptmChNum, 0, 0, mIssMode );
  }
  else{
    if( mGuardTptmChNum == 0 ){
      mGuardTptmChNum = GUARD_TPTM_CH_NUM;
    }
    GUARD_TPTM = new IBG( "GUARD_TPTM", mGuardTptmChNum, 0, 0, mIssMode );
  }
  guard_tptm_err_irq_sig = new sc_signal<bool>( "guard_tptm_err_irq_sig" );
#if defined(PLTFRM_TST)
  cout << this->name() << "->GUARD_BARR = " << hex << showbase << (unsigned long long)GUARD_BARR << endl;
  cout << this->name() << "->GUARD_IPIR = " << hex << showbase << (unsigned long long)GUARD_IPIR << endl;
  cout << this->name() << "->GUARD_MEV = "  << hex << showbase << (unsigned long long)GUARD_MEV  << endl;
  cout << this->name() << "->GUARD_TPTM = " << hex << showbase << (unsigned long long)GUARD_TPTM << endl;
  for ( i = 0; i < INTC2_MAX_NUM; i++ ) {
    cout << this->name() << "->GUARD_INTC2_" << i << " = " << hex << showbase << (unsigned long long)GUARD_INTC2[i] << endl;
  }
  cout << this->name() << "->GUARD_BARR_U2 = " << hex << showbase << (unsigned long long)GUARD_BARR_U2 << endl;
  cout << this->name() << "->GUARD_IPIR_U2 = " << hex << showbase << (unsigned long long)GUARD_IPIR_U2 << endl;
  cout << this->name() << "->GUARD_TPTM_U2 = " << hex << showbase << (unsigned long long)GUARD_TPTM_U2 << dec << endl;
  for ( i = 0; i < INTC2_MAX_NUM; i++ ) {
    cout << this->name() << "->GUARD_INTC2(U2)_" << i << " = " << hex << showbase << (unsigned long long)GUARD_INTC2_U2[i] << dec << endl;
  }
  cout << this->name() << "->mGuardBarrChNum = " << (int)mGuardBarrChNum << endl;
  cout << this->name() << "->mGuardIpirChNum = " << (int)mGuardIpirChNum << endl;
  cout << this->name() << "->mGuardMevChNum = "  << (int)mGuardMevChNum  << endl;
  cout << this->name() << "->mGuardTptmChNum = " << (int)mGuardTptmChNum << endl;
  cout << "GUARD_INTC2_U2_CH_NUM_" << i << " = " << (int)GUARD_INTC2_U2_CH_NUM << endl;
#endif //PLTFRM_TST


  for( i=0; i<CL_MAX_NUM; i++ ){
    // AXI domain
    if( i < mClNum ){
      sprintf( tname, "VCI2AXI_%d", i );
      CL_VCI2AXI_BRIDGE[i] = new VCI2AXI( tname );
    }
    else{
      CL_VCI2AXI_BRIDGE[i] = 0;
    }
    // PE(cluster)'s clock
    sprintf( tname, "CL%d_sys_clk", i );
    CL_sys_clk[i] = new slct_sc_in<sc_dt::uint64>(tname);
  }

  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    for ( j = 0; j < FE_INTC1_CH_NUM; j++ ) {
      sprintf(tname, "PE%d_feint%d", i, j );
      PE_feint[i][j] = new slct_sc_in<bool>(tname); // PE_feint
      sprintf(tname, "PE%d_feint_type%d", i, j );
      PE_feint_type[i][j] = new slct_sc_in<bool>(tname); // PE_feint_type
    }
    for ( j = 0; j < INTC1_CH_NUM; j++ ) {
      sprintf(tname, "PE%d_eiint%d", i, j);
      PE_eiint[i][j] = new slct_sc_in<bool>(tname); // PE_eiint
      sprintf(tname, "PE%d_eiint_type%d", i, j);
      PE_eiint_type[i][j] = new slct_sc_in<bool>(tname); // PE_eiint_type
      sprintf( tname, "PE%d_ipir2eiint%d_sig", i, j );
      ipir2eiint_sig[i][j] = new sc_signal<bool>( tname ); // IPIR
    }
    for ( j = 0; j < INTC_IPIR_CH_NUM; j++ ) {
      sprintf( tname, "PE%d_ipir_ch%d_sig", i, j );
      ipir_sig[i][j] = new SC_SIGNAL(bool)( tname ); // IPIR
    }
  }

  // for INTC1-INTC2
  for ( j = 0; j < INTC2_MAX_NUM; j++ ) {
    for ( i = 0; i < PE_MAX_NUM; i++ ) {
      sprintf(tname, "i2%d_g%d_eiint_sig", j, i);
      i2_g_eiint_sig[j][i] = new sc_signal<uint64>( tname );
    }
  }
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    sprintf(tname, "i1_p%d_intack_sig", j);
    i1_p_intack_sig[j] = new sc_signal<unsigned int>( tname );
  }
  for ( j = 0; j < mIntc2Num; j++ ) {
    for ( i = 0; i < INTC_BCAST_NUM; i++ ) {
      sprintf( tname, "i2%d_bcast%d_sig", j, i );
      i2_bcast_sig[j][i] = new sc_signal<bool>( tname );
    }
  }
  // for IPIR
  if ( 1 < mPeNum ) {
    for ( j = 0; j < INTC_IPIR_CH_NUM; j++ ) {
      sprintf(tname, "ipir_init_ch%d_sig", j);
      ipir_init_ch_sig[j] = new sc_signal<bool>(tname); // IPIR
    }
  }
  //for TPTM
  tptm_irq_sig = new sc_signal<bool>*[mPeNum];
  tptm_inttptmu_sig = new sc_signal<bool>**[mPeNum];
  for (i = 0; i < mPeNum; i++) {
    //for INTERVAL_TIMER
    sprintf(tname, "tptm_irq%d_sig", i);
    tptm_irq_sig[i] = new sc_signal<bool>(tname);
    //for UP_COUNTER_TIMER 
    tptm_inttptmu_sig[i] = new sc_signal<bool>*[G4TPTM_UP_COUNTER_TIMER_CMP_NUM];
    for(j = 0; j < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; j++) {
      sprintf(tname, "tptm_inttptmu%d%d_sig", i, j);
      tptm_inttptmu_sig[i][j] = new sc_signal<bool>(tname);
    }
  }

  // for eiint from peripherals
  for ( i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    sprintf( tname, "eiint%d", i+INTC1_CH_NUM );
    eiint[i] = new slct_sc_in<bool>( tname );
    sprintf( tname, "eiint_type%d", i+INTC1_CH_NUM );
    eiint_type[i] = new slct_sc_in<bool>( tname );
  }

  // AXI domain
  H_BUS = new AXISS_E2( "H_BUS", mMapFileName );
  VCI2AXI_BRIDGE = new VCI2AXI( "VCI2AXI" );
  AXI2VCI_BRIDGE = new AXI2VCI( "AXI2VCI" );
  BW_DECODER = new BACKWARD_DECODER<BUS_WIDTH_VCI>( "BW_DECODER" );

  // global APB domain
  P_BUS   = new APISS( "P_BUS", mMapFileName );
  VCI2APB_BRIDGE_PFSS = new VCI2APB( "VCI2APB_PFSS" );
  VCI2APB_BRIDGE_SG1  = new VCI2APB( "VCI2APB_SG1" );
  VCI2APB_BRIDGE_SG2_0= new VCI2APB( "VCI2APB_SG2_0" );
  VCI2APB_BRIDGE_SG2_1= new VCI2APB( "VCI2APB_SG2_1" );
  VCI2APB_BRIDGE_SG3  = new VCI2APB( "VCI2APB_SG3" );
  VCI2APB_BRIDGE_SG4  = new VCI2APB( "VCI2APB_SG4" );
  VCI2APB_BRIDGE_SG5  = new VCI2APB( "VCI2APB_SG5" );
  VCI2APB_BRIDGE_SG6  = new VCI2APB( "VCI2APB_SG6" );
  VCI2APB_BRIDGE_SG7  = new VCI2APB( "VCI2APB_SG7" );
  VCI2APB_BRIDGE_SG8  = new VCI2APB( "VCI2APB_SG8" );
  VCI2APB_BRIDGE_SG9  = new VCI2APB( "VCI2APB_SG9" );
  GAPB_PFSS = new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_PFSS", mMapFileName );
  GAPB_SG1  = new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG1", mMapFileName );
  GAPB_SG2_0= new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG2_0", mMapFileName );
  GAPB_SG2_1= new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG2_1", mMapFileName );
  GAPB_SG3  = new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG3", mMapFileName );
  GAPB_SG4  = new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG4", mMapFileName );
  GAPB_SG5  = new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG5", mMapFileName );
  GAPB_SG6  = new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG6", mMapFileName );
  GAPB_SG7  = new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG7", mMapFileName );
  GAPB_SG8  = new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG8", mMapFileName );
  GAPB_SG9  = new OSCI2DCDR<BUS_WIDTH_APB>( "GAPB_SG9", mMapFileName );

  //----- connection ----
  // ----------------------------------------
  // global RAM access from H_BUS
  H_BUS->isx(G4PE->tscr);

  // Flash ROM access from outside
  tsf(G4PE->tsf);
 
  for ( i = 0; i < CL_MAX_NUM; i++ ) {
    // PE's clock
    (*((G4PE->CL_sys_clk)[i]))(*(CL_sys_clk[i]));
  }

  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    // for PE's feint
    for ( j = 0; j < FE_INTC1_CH_NUM; j++ ) {
      (*((G4PE->PE_feint)[i][j]))(*(PE_feint[i][j]));
      (*((G4PE->PE_feint_type)[i][j]))(*(PE_feint_type[i][j]));
    }
    // for PE's eiint
    for ( j = 0; j < INTC1_CH_NUM; j++ ) {
      G4PE->PE_eiint_type[i][j]->bind( *PE_eiint_type[i][j] );
      ipir2eiint_sig[i][j]->write( false );
      G4PE->PE_eiint[i][j]->bind( *ipir2eiint_sig[i][j] );
    }
  }

  G4PE->fenmi(fenmi); // for fenmi
 
  for( i=0; i<mClNum; i++ ){
    // AXI domain
    (*((G4PE->CL_ish)[i]))(*(CL_VCI2AXI_BRIDGE[i]->ts));
    (*(CL_VCI2AXI_BRIDGE[i]->is))(H_BUS->tsx);
    CL_VCI2AXI_BRIDGE[i]->vciFreqPort(*CL_sys_clk[i]);
    CL_VCI2AXI_BRIDGE[i]->axiFreqPort(AXI_clk);
    CL_VCI2AXI_BRIDGE[i]->resetPort(sys_reset);
    CL_VCI2AXI_BRIDGE[i]->setTlmTransType(mIssMode);
    // global APB domain
    (*((G4PE->CL_isgp)[i]))(P_BUS->ts);
  }

  // AXI domain
  tsx(H_BUS->tsx);
  H_BUS->isx(isx);
  H_BUS->isx(BW_DECODER->ts);
  VCI2AXI_BRIDGE->vciFreqPort(SYS_BUS_clk);
  VCI2AXI_BRIDGE->axiFreqPort(AXI_clk);
  VCI2AXI_BRIDGE->resetPort(sys_reset);
  VCI2AXI_BRIDGE->setTlmTransType(mIssMode);
  AXI2VCI_BRIDGE->axiFreqPort(AXI_clk);
  AXI2VCI_BRIDGE->vciFreqPort(SYS_BUS_clk);
  AXI2VCI_BRIDGE->resetPort(sys_reset);
  AXI2VCI_BRIDGE->setTlmTransType(mIssMode);
  G4PE->ish(*(VCI2AXI_BRIDGE->ts));
  (*(VCI2AXI_BRIDGE->is))(H_BUS->tsx);
  BW_DECODER->is(*(AXI2VCI_BRIDGE->ts));
  (*(AXI2VCI_BRIDGE->is))(G4PE->tss);

  /* DMA will be connected to the BACKWARD_DECODER */

  // global APB domain
  G4PE->isgp(P_BUS->ts);

  (*(VCI2APB_BRIDGE_PFSS->is))(GAPB_PFSS->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_PFSS->ts));
  VCI2APB_BRIDGE_PFSS->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_PFSS->apbFreqPort(GAPB_PFSS_clk);
  VCI2APB_BRIDGE_PFSS->resetPort(sys_reset);
  VCI2APB_BRIDGE_PFSS->setTlmTransType(mIssMode);
  
  GAPB_SG1->is(isg_sg1);
  (*(VCI2APB_BRIDGE_SG1->is))(GAPB_SG1->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG1->ts));
  VCI2APB_BRIDGE_SG1->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG1->apbFreqPort(GAPB_SG1_clk);
  VCI2APB_BRIDGE_SG1->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG1->setTlmTransType(mIssMode);

  GAPB_SG2_0->is(isg_sg2_0);
  (*(VCI2APB_BRIDGE_SG2_0->is))(GAPB_SG2_0->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG2_0->ts));
  VCI2APB_BRIDGE_SG2_0->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG2_0->apbFreqPort(GAPB_SG2_0_clk);
  VCI2APB_BRIDGE_SG2_0->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG2_0->setTlmTransType(mIssMode);

  GAPB_SG2_1->is(isg_sg2_1);
  (*(VCI2APB_BRIDGE_SG2_1->is))(GAPB_SG2_1->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG2_1->ts));
  VCI2APB_BRIDGE_SG2_1->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG2_1->apbFreqPort(GAPB_SG2_1_clk);
  VCI2APB_BRIDGE_SG2_1->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG2_1->setTlmTransType(mIssMode);

  GAPB_SG3->is(isg_sg3);
  (*(VCI2APB_BRIDGE_SG3->is))(GAPB_SG3->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG3->ts));
  VCI2APB_BRIDGE_SG3->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG3->apbFreqPort(GAPB_SG3_clk);
  VCI2APB_BRIDGE_SG3->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG3->setTlmTransType(mIssMode);

  GAPB_SG4->is(isg_sg4);
  (*(VCI2APB_BRIDGE_SG4->is))(GAPB_SG4->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG4->ts));
  VCI2APB_BRIDGE_SG4->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG4->apbFreqPort(GAPB_SG4_clk);
  VCI2APB_BRIDGE_SG4->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG4->setTlmTransType(mIssMode);

  GAPB_SG5->is(isg_sg5);
  (*(VCI2APB_BRIDGE_SG5->is))(GAPB_SG5->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG5->ts));
  VCI2APB_BRIDGE_SG5->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG5->apbFreqPort(GAPB_SG5_clk);
  VCI2APB_BRIDGE_SG5->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG5->setTlmTransType(mIssMode);

  GAPB_SG6->is(isg_sg6);
  (*(VCI2APB_BRIDGE_SG6->is))(GAPB_SG6->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG6->ts));
  VCI2APB_BRIDGE_SG6->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG6->apbFreqPort(GAPB_SG6_clk);
  VCI2APB_BRIDGE_SG6->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG6->setTlmTransType(mIssMode);

  GAPB_SG7->is(isg_sg7);
  (*(VCI2APB_BRIDGE_SG7->is))(GAPB_SG7->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG7->ts));
  VCI2APB_BRIDGE_SG7->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG7->apbFreqPort(GAPB_SG7_clk);
  VCI2APB_BRIDGE_SG7->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG7->setTlmTransType(mIssMode);

  GAPB_SG8->is(isg_sg8);
  (*(VCI2APB_BRIDGE_SG8->is))(GAPB_SG8->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG8->ts));
  VCI2APB_BRIDGE_SG8->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG8->apbFreqPort(GAPB_SG8_clk);
  VCI2APB_BRIDGE_SG8->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG8->setTlmTransType(mIssMode);

  GAPB_SG9->is(isg_sg9);
  (*(VCI2APB_BRIDGE_SG9->is))(GAPB_SG9->ts);
  P_BUS->is(*(VCI2APB_BRIDGE_SG9->ts));
  VCI2APB_BRIDGE_SG9->vciFreqPort(VCI_clk);
  VCI2APB_BRIDGE_SG9->apbFreqPort(GAPB_SG9_clk);
  VCI2APB_BRIDGE_SG9->resetPort(sys_reset);
  VCI2APB_BRIDGE_SG9->setTlmTransType(mIssMode);

  // IPIR, OR_IPIR, MEV, BARR, IBG
  if ( 1 < mPeNum ) {
    // IPIR
    IPIR->resetz( sys_reset );
    IPIR->clk( VCI_clk );
    ipir_ext_err_dummy_sig.write( false );
    IPIR->ext_err( ipir_ext_err_dummy_sig );
    for ( j = 0; j < INTC_IPIR_CH_NUM; j++ ) {
      for ( i = 0; i < PE_MAX_NUM; i++ ) {
	IPIR->ipir_int_pe_ch[i][j]->bind( *ipir_sig[i][j] );
      }
      IPIR->ipir_init_ch[j]->bind( *ipir_init_ch_sig[j] );
      ipir_init_ch_sig[j]->write( false );
    }
    // BARR
    BARR->cpu_clk(VCI_clk);
    BARR->sys_resetn(sys_reset);
    if( mPfssType == U2A_PFSS ){
      // GUARD_IPIR
      G4PE->isgc( *(GUARD_IPIR_U2->tsp) );
      GUARD_IPIR_U2->cpu_clk( VCI_clk );
      GUARD_IPIR_U2->resetPort( sys_reset );
      GUARD_IPIR_U2->isp->bind( *(IPIR->ts) );
      GUARD_IPIR_U2->ibgerr( *guard_ipir_err_irq_sig );
      // GUARD_BARR
      G4PE->isgc( *(GUARD_BARR_U2->tsp) );
      GUARD_BARR_U2->cpu_clk( VCI_clk );
      GUARD_BARR_U2->resetPort( sys_reset );
      GUARD_BARR_U2->isp->bind( BARR->ts );
      GUARD_BARR_U2->ibgerr( *guard_barr_err_irq_sig );
    }
    else{
      // MEV
      MEV->clk( VCI_clk );
      MEV->resetz( sys_reset );
      // GUARD_IPIR
      G4PE->isgc( *(GUARD_IPIR->tsp) );
      GUARD_IPIR->PCLK( VCI_clk );
      GUARD_IPIR->resetPort( sys_reset );
      GUARD_IPIR->isp->bind( *(IPIR->ts) );
      GUARD_IPIR->err_irq( *guard_ipir_err_irq_sig );
      // GUARD_MEV
      G4PE->isgc( *(GUARD_MEV->tsp) );
      GUARD_MEV->PCLK( VCI_clk );
      GUARD_MEV->resetPort( sys_reset );
      GUARD_MEV->isp->bind( *(MEV->ts) );
      GUARD_MEV->err_irq( *guard_mev_err_irq_sig );
      // GUARD_BARR
      G4PE->isgc( *(GUARD_BARR->tsp) );
      GUARD_BARR->PCLK( VCI_clk );
      GUARD_BARR->resetPort( sys_reset );
      GUARD_BARR->isp->bind( BARR->ts );
      GUARD_BARR->err_irq( *guard_barr_err_irq_sig );
    }
  }
  else{
    GC_DUMMY_SLAVE = new DummySlave<BUS_WIDTH_VCI32>( "GC_DUMMY_SLAVE" );
    G4PE->isgc( GC_DUMMY_SLAVE->ts );
    for ( i = 0; i < PE_MAX_NUM; i++ ) {
      for ( j = 0; j < INTC_IPIR_CH_NUM; j++ ) {
	ipir_sig[i][j]->write( false );
      }
    }
  }
  //IPIR
  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    for ( j = 0; j < INTC1_CH_NUM; j++ ) {
      OR_IPIR[i]->pe_eiint[j]->bind( *PE_eiint[i][j] );
      OR_IPIR[i]->out_pe_ch[j]->bind( *ipir2eiint_sig[i][j] );
    }
    for ( j = 0; j < INTC_IPIR_CH_NUM; j++ ) {
      OR_IPIR[i]->ipir_int_pe_ch[j]->bind( *ipir_sig[i][j] );
    }
    for ( j = 0; j < INTC_BCAST_NUM; j++ ) {
      // intc2_broadcast is NOT adapted to several INTC2
      OR_IPIR[i]->intc2_broadcast[j]->bind( *i2_bcast_sig[0][j] );
    }
  }
  // for TPTM
  TPTM->sys_resetn(sys_reset);
  TPTM->cpu_clk(VCI_clk);
  if( mPfssType == U2A_PFSS ){
    G4PE->isgc( *(GUARD_TPTM_U2->tsp) );
    GUARD_TPTM_U2->cpu_clk( VCI_clk );
    GUARD_TPTM_U2->resetPort( sys_reset );
    GUARD_TPTM_U2->isp->bind( TPTM->ts );
    GUARD_TPTM_U2->ibgerr( *guard_tptm_err_irq_sig );
  }
  else{
    G4PE->isgc( *(GUARD_TPTM->tsp) );
    GUARD_TPTM->PCLK( VCI_clk );
    GUARD_TPTM->resetPort( sys_reset );
    GUARD_TPTM->isp->bind( TPTM->ts );
    GUARD_TPTM->err_irq( *guard_tptm_err_irq_sig );
  }
  for (i = 0; i < mPeNum; i++) {
    (*(TPTM->TPTM_IRQ[i]))(*tptm_irq_sig[i]);
    for(j = 0; j < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; j++) {
      (*(TPTM->INTTPTMU[i][j]))(*tptm_inttptmu_sig[i][j]);
    }
  }


  // for INTC1-INTC2 connection
  for ( j = 0; j < INTC2_MAX_NUM; j++ ) {
    for ( i = 0; i < PE_MAX_NUM; i++ ) {
      G4PE->eiint[j][i]->bind( *i2_g_eiint_sig[j][i] );
      i2_g_eiint_sig[j][i]->write( 0 );
    }
  }
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    G4PE->intack[j]->bind( *i1_p_intack_sig[j] );
  }

  // all channels are connected to INTC2_0 only now.
  // in the future, nunber of INTC2, mapping of CH and port should be configurable if necessary 
  for ( k = 0; k < mIntc2Num; k++ ) {
    if( mPfssType == U2A_PFSS ){
      GAPB_PFSS->is( *(GUARD_INTC2_U2[k]->tspa) );
      GUARD_INTC2_U2[k]->PCLK( GAPB_PFSS_clk );
      GUARD_INTC2_U2[k]->resetPort( sys_reset );
      GUARD_INTC2_U2[k]->err_irq( *guard_intc2_err_irq_sig[k] );
      (*(GUARD_INTC2_U2[k]->a_is))( *(INTC2[k]->ts) );
    }
    else{
      GAPB_PFSS->is( *(GUARD_INTC2[k]->tsp) );
      GUARD_INTC2[k]->PCLK( GAPB_PFSS_clk );
      GUARD_INTC2[k]->resetPort( sys_reset );
      GUARD_INTC2[k]->err_irq( *guard_intc2_err_irq_sig[k] );
      (*(GUARD_INTC2[k]->isp))( *(INTC2[k]->ts) );
    }
#ifdef DEBUG_MESSAGE_OUTPUT
    INTC2[k]->setTraceEnable( mpTf );
#endif // DEBUG_MESSAGE_OUTPUT
    INTC2[k]->busresetz( sys_reset ); // system port
    INTC2[k]->busclk( GAPB_PFSS_clk ); // system port
    for ( i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
      INTC2[k]->eiintl[i]->bind( *eiint[i] ); // external port
      INTC2[k]->eiint_type[i]->bind( *eiint_type[i] ); // external port
    }
    for ( i = 0; i < PE_MAX_NUM; i++ ) {
      INTC2[k]->i2_g_eiint[i]->bind( *i2_g_eiint_sig[k][i] ); // INTC1
    }
    for ( i = 0; i < INTC_BCAST_NUM; i++ ) {
      INTC2[k]->i2_bcastrq[i]->bind( *i2_bcast_sig[k][i] ); // bcastProc
    }
    for ( j = 0; j < PE_MAX_NUM; j++ ) {
      INTC2[k]->i1_p_intack[j]->bind( *i1_p_intack_sig[j] ); // INTC1
      INTC2[k]->cpu_gm_info[j]->bind(*G4PE->cpugminfo_sig[j]); // CPU
    }
  }

  G4PE->VCI_clk(VCI_clk);
  G4PE->From_clk(From_clk);
  G4PE->Cram_clk(Cram_clk);
  G4PE->SYS_BUS_clk(SYS_BUS_clk);
  G4PE->sys_reset(sys_reset);

  // for connection of SWDT and BOOTCTRL to CPU
  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    sprintf(tname, "PE%d_swdt_clk_sig", i);  
    swdt_clk_sig[i] = new sc_signal<bool>( tname );
    *swdt_clk_sig[i] = false;
    (*((G4PE->getG4cpuPtr())->Swdt_clk[i]))(*swdt_clk_sig[i]);
    sprintf(tname, "PE%d_sys_cpinit_sig", i);  
    sys_cpinit_sig[i] = new sc_signal<bool>( tname );
    *sys_cpinit_sig[i] = false;
    (*((G4PE->getG4cpuPtr())->sys_cpinit[i]))(*sys_cpinit_sig[i]);
  }

  // for connection of ECM/ICUP to CPU
  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    sprintf(tname, "PE%d_fli_areaerr_sig", i);
    pe_fli_areaerr_sig[i] = new sc_signal<bool>( tname );
    sprintf(tname, "PE%d_cram_areaerr_sig", i);
    pe_cram_areaerr_sig[i] = new sc_signal<bool>( tname );
    sprintf(tname, "PE%d_peg_err_m_sig", i);
    pe_peg_err_m_sig[i] = new sc_signal<bool>( tname );
    sprintf(tname, "PE%d_peg_err_s_sig", i);
    pe_peg_err_s_sig[i] = new sc_signal<bool>( tname );
    sprintf(tname, "PE%d_fli_grderr_sig", i);
    pe_fli_grderr_sig[i] = new sc_signal<bool>( tname );
  }
  gvci_areaerr_sig = new SC_SIGNAL(bool)( "gvci_areaerr_sig" );
  exm_fli_areaerr_sig = new sc_signal<bool>( "exm_fli_areaerr_sig" );
  *exm_fli_areaerr_sig = false;
  exm_cram_areaerr_sig = new sc_signal<bool>( "exm_cram_areaerr_sig" );
  *exm_cram_areaerr_sig = false;
  cram_grderr_sig = new sc_signal<bool>( "cram_grderr_sig" );
  *cram_grderr_sig = false;
  exm_fli_grderr_sig = new sc_signal<bool>( "exm_fli_grderr_sig" );
  *exm_fli_grderr_sig = false;

  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    G4PE->getG4cpuPtr()->PE_CramAreaerr[i]->bind(*pe_cram_areaerr_sig[i]);
  }
  G4PE->GVCI_DECODER->freqPort(VCI_clk);
  G4PE->GVCI_DECODER->resperr(*gvci_areaerr_sig);
  G4PE->getG4cpuPtr()->ExM_CramAreaerr(*exm_cram_areaerr_sig);
  G4PE->getG4cpuPtr()->CramGrderr(*cram_grderr_sig);

  // for connection of CPU to ECM
  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    G4PE->getG4cpuPtr()->PE_PegErrM[i]->bind(*pe_peg_err_m_sig[i]);
    *pe_peg_err_m_sig[i] = false;
    G4PE->getG4cpuPtr()->PE_PegErrS[i]->bind(*pe_peg_err_s_sig[i]);
    *pe_peg_err_s_sig[i] = false;
  }

#ifdef DEBUG_MESSAGE_OUTPUT
  setTraceEnable( mpTf );
#endif
}


/// <summary>destructor</summary>
NSMVG4SSV01::
~NSMVG4SSV01(void){
  dmsgsc( mpModName << ".~NSMVG4SSV01()" );
  // Explicit call to destructors
  // ----------------------------------------
  // NSMVG4PEV01
  unsigned int i,j,k;
  if (G4PE)	 	 delete G4PE;

  for ( i = 0; i < mIntc2Num; i++ ) {
    delete INTC2[i];
    if( mPfssType == U2A_PFSS ){
      delete GUARD_INTC2_U2[i];
    }
    else{
      delete GUARD_INTC2[i];
    }
    delete guard_intc2_err_irq_sig[i];
  }
  delete[] INTC2;

  if ( 1 < mPeNum ) {
    delete BARR;
    delete MEV;
    delete IPIR;
    if( mPfssType == U2A_PFSS ){
      delete GUARD_BARR_U2;
      delete GUARD_IPIR_U2;
    }
    else{
      delete GUARD_BARR;
      delete GUARD_IPIR;
      delete GUARD_MEV;
      delete guard_mev_err_irq_sig;
    }
    delete guard_barr_err_irq_sig;
    delete guard_ipir_err_irq_sig;

    for ( i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
      delete ipir_init_ch_sig[i];
    }
  }

  delete TPTM;
  if( mPfssType == U2A_PFSS ){
    delete GUARD_TPTM_U2;
  }
  else{
    delete GUARD_TPTM;
  }
  delete guard_tptm_err_irq_sig;

  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    delete OR_IPIR[i];
    for ( j = 0; j < INTC1_CH_NUM; j++ ) {
      delete ipir2eiint_sig[i][j];
    }
    for ( j = 0; j < INTC_IPIR_CH_NUM; j++ ) {
      delete ipir_sig[i][j];
    }
  }
  delete[] OR_IPIR;

  for (i = 0; i< mPeNum; i++) {
    for(j = 0; j < G4TPTM_UP_COUNTER_TIMER_CMP_NUM; j++) {
      delete tptm_inttptmu_sig[i][j];
    }
    delete tptm_irq_sig[i];
    delete tptm_inttptmu_sig[i];
  }
  delete[] tptm_irq_sig;
  delete[] tptm_inttptmu_sig;

  for( i=0; i<mClNum; i++ ){
    delete CL_VCI2AXI_BRIDGE[i];
  }
  delete VCI2AXI_BRIDGE;
  delete AXI2VCI_BRIDGE;
  delete VCI2APB_BRIDGE_PFSS;
  delete VCI2APB_BRIDGE_SG1;
  delete VCI2APB_BRIDGE_SG2_0;
  delete VCI2APB_BRIDGE_SG2_1;
  delete VCI2APB_BRIDGE_SG3;
  delete VCI2APB_BRIDGE_SG4;
  delete VCI2APB_BRIDGE_SG5;
  delete VCI2APB_BRIDGE_SG6;
  delete VCI2APB_BRIDGE_SG7;
  delete VCI2APB_BRIDGE_SG8;
  delete VCI2APB_BRIDGE_SG9;
  delete P_BUS;
  delete H_BUS;
  delete GAPB_PFSS;
  delete GAPB_SG1;
  delete GAPB_SG2_0;
  delete GAPB_SG2_1;
  delete GAPB_SG3;
  delete GAPB_SG4;
  delete GAPB_SG5;
  delete GAPB_SG6;
  delete GAPB_SG7;
  delete GAPB_SG8;
  delete GAPB_SG9;

  for ( i = 0; i < CL_MAX_NUM; i++ ) {
    delete CL_sys_clk[i];
  }
  for ( j = 0; j < INTC2_MAX_NUM; j++ ) {
    for ( i = 0; i < PE_MAX_NUM; i++ ) {
      delete i2_g_eiint_sig[j][i];
    }
  }
  for ( j = 0; j < mIntc2Num; j++ ) {
    for ( i = 0; i < INTC_BCAST_NUM; i++ ) {
      delete i2_bcast_sig[j][i];
    }
  }
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    delete i1_p_intack_sig[j];
  }
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    for ( i = 0; i < FE_INTC1_CH_NUM; i++ ) {
      delete PE_feint[j][i];
      delete PE_feint_type[j][i];
    }
    for ( i = 0; i < INTC1_CH_NUM; i++ ) {
      delete PE_eiint[j][i];
      delete PE_eiint_type[j][i];
    }
  }
  for ( i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    delete eiint[i];
    delete eiint_type[i];
  }

  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    delete pe_fli_areaerr_sig[i];
    delete pe_cram_areaerr_sig[i];
    delete pe_peg_err_m_sig[i];
    delete pe_peg_err_s_sig[i];
    delete pe_fli_grderr_sig[i];
    delete swdt_clk_sig[i];
    delete sys_cpinit_sig[i];
  }
  delete gvci_areaerr_sig;
  delete exm_fli_areaerr_sig;
  delete exm_cram_areaerr_sig;
  delete cram_grderr_sig;
  delete exm_fli_grderr_sig;
}


/// <summary>start of simulation</summary>
void NSMVG4SSV01::start_of_simulation( void ){
  if ( 1 < mPeNum ) {
    if( mPfssType == U2A_PFSS ){
      IPIR->ts->setBaseAddressSize( GUARD_IPIR_U2->tsp->getBaseAddress(),
				    GUARD_IPIR_U2->tsp->getBaseSize() );
    }
    else{
      IPIR->ts->setBaseAddressSize( GUARD_IPIR->tsp->getBaseAddress(),
				    GUARD_IPIR->tsp->getBaseSize() );
    }
  }
  for ( int i = 0; i < mIntc2Num; i++ ) {
    if( mPfssType == U2A_PFSS ){
      INTC2[i]->ts->setBaseAddressSize( GUARD_INTC2_U2[i]->tspa->getBaseAddress(),
					GUARD_INTC2_U2[i]->tspa->getBaseSize() );
    }
    else{
      INTC2[i]->ts->setBaseAddressSize( GUARD_INTC2[i]->tsp->getBaseAddress(),
					GUARD_INTC2[i]->tsp->getBaseSize() );
    }
  }
}


/// <summary>for reading the configulation file and initialization</summary>
void
NSMVG4SSV01::
read_config_file( const char *filename ) {
  char	  word[512];
  char	  seps[] = " ,=();\t\n\r";
  char	  *token;

  // variables for parsing of IBG
  int ibgIndex;
  const char *ibgParamLabel[] = {"[GUARD_BARR_CH_NUM]",
				 "[GUARD_IPIR_CH_NUM]",
				 "[GUARD_MEV_CH_NUM]",
				 "[GUARD_TPTM_CH_NUM]"};
  unsigned char *ibgParamPtr[] = {&mGuardBarrChNum,
				  &mGuardIpirChNum,
				  &mGuardMevChNum,
				  &mGuardTptmChNum};

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
    // number of PE
    if ( 0 == strcmp(token, "[PE_NUM]") ) {
      token = strtok( 0, seps );
      if (token != NULL) {
        mPeNum = token_to_uint( token,
                                name(),
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

    // for PFSS TYPE
    if (strcmp(token, "[G4_PFSS_TYPE]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        if (strcmp(token, "E2X") == 0) {
            mPfssType = E2X_PFSS;
        } else if (strcmp(token, "U2A") == 0) {
            mPfssType = U2A_PFSS;
        } else {
	  char error_message_buf[1024];
	  sprintf(error_message_buf,
		  "Unexpected token was specified [%s] on [G4_PFSS_TYPE].",
		  token);
	  printErrorMessage(name(), "read_config_file", error_message_buf);
        }
      }
#if defined(PLTFRM_TST)
      cout << this->name() << "->mPfssType = " << mPfssType << endl;
#endif //PLTFRM_TST
      continue;
    }
    // bus map file name
    if( strcmp( token, "[BUS_MAP_FILE]" ) == 0 ) {
      token = strtok(0, seps);
      const char *fname = (!token)? "" : token;
      strcpy(mMapFileName, fname);
      continue;
    }

    // simulation mode
    if (strcmp(token, "[SIM_MODE]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "CAISS") == 0){
	SetSimMode(SIM_MODE_CA);
      } else if(strcmp(token, "FASTISS") == 0){
	SetSimMode(SIM_MODE_FAST);
      } else{
            char error_message_buf[1024];
            sprintf(error_message_buf,
                  "Unexpected token was specified [%s] on [SIM_MODE].",
                  token);
            printErrorMessage(name(), "read_config_file", error_message_buf);
      }
#if defined(PLTFRM_TST)
      cout << this->name() << "->token = " << token << endl;
#endif //PLTFRM_TST
      continue;
    }
    // for IBG channel number
    for( ibgIndex=0; ibgIndex<IBG_NUM; ibgIndex++ ){
      if (strcmp(token, ibgParamLabel[ibgIndex]) == 0) {
	token = strtok(0, seps);
	if (token != NULL){
	  unsigned int tmp = token_to_uint( token,
					    name(),
					    ibgParamLabel[ibgIndex]);
	  if( tmp != 0 && tmp < 256 ){
	    *ibgParamPtr[ibgIndex] = (unsigned char)tmp;
	  }
	  else{
	    char error_message_buf[1024];
	    sprintf(error_message_buf,
		    "%s must be except 0 and be less than 256 [%d].",
		    ibgParamLabel[ibgIndex], tmp);
	    printErrorMessage(name(), "read_config_file", error_message_buf);
	  }
	}
	break;
      }
    }
    if( ibgIndex < IBG_NUM ){
      continue;
    }

  }  // end of configuration file read loop(infinite)

}  // end of read_config_file()

///<summary>set simulation mode</summary>
///<param name="filename">simulation mode</param>
///<returns>none</returns>
void NSMVG4SSV01::SetSimMode( SIM_MODE_T iss_mode )
{
  mIssMode = iss_mode;
}



///<summary>for generate VCD file</summary>
///<param name="tf">the pointer of sc_trace_file</param>
///<returns>none</returns>
void
NSMVG4SSV01::setTraceEnable( sc_trace_file* tf )
{
  dmsgsc( mpModName << ".SetTraceEnable()" );
  unsigned int i, j;
  for ( j = 0; j < mIntc2Num; j++ ) {
    for ( i = 0; i < PE_MAX_NUM; i++ ) {
      sc_trace( tf, *i2_g_eiint_sig[j][i], i2_g_eiint_sig[j][i]->name() );
    }
    for ( i = 0; i < INTC_BCAST_NUM; i++ ) {
      sc_trace( tf, *i2_bcast_sig[j][i], i2_bcast_sig[j][i]->name() );
    }
  }
  for ( j = 0; j < mPeNum; j++ ) {
    if ( 1 < mPeNum ) {
      for ( i = 0; i < INTC1_CH_NUM; i++ ) {
        sc_trace( tf, *ipir2eiint_sig[j][i], ipir2eiint_sig[j][i]->name() );
      }
    }
  }
}

