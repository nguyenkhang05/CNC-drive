/*************************************************************************
 *
 * $Id: NSMVINTC2V02.cpp,v 1.13 2020/04/09 08:22:32 ngathunguyen Exp $
 *
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include <cstdio>
#include <algorithm>
#include "NSMVINTC2V02.h"
#include "G4_Intc2Func.h"
#include "Common.h"
#include "CmErrMsg.h"

using namespace std;
using namespace CmErrMsg;
using namespace G4IntcTyp;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT


/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pCfgFileName">configuration file name</param>
/// <returns>none</returns>
NSMVINTC2V02::NSMVINTC2V02(
  sc_module_name name, 
  const char*    pCfgFileName ):
    sc_module( name ),
    BusSlaveBase<BUS_WIDTH_APB, 1>(),
    busresetz( "busresetz" ),
    busclk( "busclk" ),
    mpIntc2Func( 0 ),
    mPeNum( 1 ),
    mIntc2Type(INTC2_STANDARD),
    mILatency( 0 )
{
  mpModName = this->name();
  dmsgsc( mpModName << ".NSMVINTC2V02()" );
  char     port_name[32];
  unsigned int  i, j;

  // get configuration parameter
  readCfgFile( pCfgFileName );

  // Currently, PEID is numbered in order.
  // In the future, PEID will be able to be set the skipped numbers.
  for ( i = 0; i < mPeNum; i++ ) {
    // the index of array must be equal to PEID.
    mPeidInfo.push_back( new PeidInfo( i ) );
  }

  setSlaveResetPort32( &busresetz );
  setSlaveFreqPort32( &busclk );
  setTargetSocket32( (char*)"ts" );

  ts = tSocket32[0];

  mpIntc2Func = new G4_Intc2Func( &mPeidInfo, 0, this, (Intc2ImplType)mIntc2Type );
  mBusSlaveIf32[0]->setFuncModulePtr( mpIntc2Func );
  mBusSlaveIf32[0]->setBusProtocol( BUS_APB );
  mBusSlaveIf32[0]->setTransNmbLmt( 1 );

  // create the instances of ports
  for ( i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    sprintf( port_name, "eiintl_%d", (int)(INTC1_CH_NUM+i));
    eiintl[i] = new sc_in<bool>(port_name);
    sprintf( port_name, "eiint_type_%d", (int)(INTC1_CH_NUM+i));
    eiint_type[i] = new sc_in<bool>(port_name);
  }

  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    sprintf( port_name, "i2_g%d_eiint", (int)i);
    i2_g_eiint[i] = new sc_out<uint64>(port_name);
    i2_g_eiint[i]->initialize(0x0);
  }

  for ( i = 0; i < INTC_BCAST_NUM; i++) {
    sprintf( port_name, "i2_bcastrq_%d", (int)i);
    i2_bcastrq[i] = new sc_out<bool>(port_name);
    i2_bcastrq[i]->initialize(false);
  }

  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    sprintf( port_name, "i1_p_intack%d", (int)i);
    i1_p_intack[i] = new sc_in<unsigned int>(port_name);
  }

  for ( i = 0; i < PE_MAX_NUM; i++ ) { // <-- added by chanle 20180131 for G4MH2.0 HV
    sprintf( port_name, "cpu_gm_info%d", (int)i);
    cpu_gm_info[i] = new sc_in<unsigned int>(port_name);
  }

  // initialize variable (array)
  for ( i = 0; i < PE_MAX_NUM; i++ ) { // <-- added by chanle 20180131 for G4MH2.0 HV
    mCPUGM[i] = false; // Note: not reset value when reset active
    mCPUGPID[i] = 0;   // Note: not reset value when reset active
  }

  // Initialize Variable
  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    mEiReqInfo[i].clear();
    mEiCancelInfo[i].clear();
  }
  for ( i = 0; i < INTC_BCAST_NUM; i++ ) {
    mBcastInfo[i].clear();
  }
  for ( i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    mPrevEiintl[i] = false;
  }
  mReEntryReqList.clear();

  // initialize process
  SC_METHOD( InitProc );

  // reset process
  SC_METHOD( ResetProc );
  sensitive << busresetz.neg();

  // EIINT acknowledge process
  SC_METHOD( EiintAckProc );
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    sensitive << *i1_p_intack[j];
    mPrevIntack[j] = 0;
  }

  resendEiint2Forbid = false;
  dont_initialize();

  // i2_g_eiint port write process
  SC_METHOD( SetEiintReqPortProc );
  sensitive << meSetEiReq;
  dont_initialize();

  // i2_bcastrq port write process
  SC_METHOD( SetBcastReqPortProc );
  sensitive << meSetBcast;
  dont_initialize();

  // CyclicHandler() execution process
  SC_METHOD( ExecProc );
  sensitive << meExec;
  dont_initialize();
  
  // ResendEiintProc() execution process
  SC_METHOD( ResendEiintProc );
  sensitive << meResendEiintReq;
  dont_initialize();

  // Monitor CPU info (GM, GPID)
  SC_METHOD( MonitorCPUInfoMethod );
  dont_initialize();
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    sensitive << *cpu_gm_info[j];
  }
}


/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
NSMVINTC2V02::~NSMVINTC2V02()
{
  dmsgsc( mpModName << ".~NSMVINTC2V02()" );
  int  i, j;

  std::vector<PeidInfo*>::iterator it = mPeidInfo.begin ();
  while (it != mPeidInfo.end ()) {
    delete (*it);
    ++it;
  }
  mPeidInfo.clear();

  delete mpIntc2Func;
  for ( i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    delete eiintl[i];
    delete eiint_type[i];
  }
  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    delete i2_g_eiint[i];
  }
  for ( i = 0; i < INTC_BCAST_NUM; i++) {
    delete i2_bcastrq[i];
  }
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    delete i1_p_intack[j];
  }
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    delete cpu_gm_info[j];
  }
}

// set function
/// <summary>set read-latency of register</summary>
/// <param name="rLatency">read-latency</param>
/// <returns>none</returns>
void
NSMVINTC2V02::setReadRegLatency( uint64 rLatency )
{
  dmsgsc( mpModName << ".setReadRegLatency( 0x" << rLatency << " )" );
  mBusSlaveIf32[0]->setReadLatency( rLatency  );
}

/// <summary>set write-latency of register</summary>
/// <param name="wLatency">write-latency</param>
/// <returns>none</returns>
void
NSMVINTC2V02::setWriteRegLatency( uint64 wLatency )
{
  dmsgsc( mpModName << ".setWriteRegLatency( 0x" << wLatency << " )" );
  mBusSlaveIf32[0]->setWriteLatency( wLatency  );
}

/// <summary>set latency to send interrupt signal</summary>
/// <param name="iLatency">latency to send interrupt signal</param>
/// <returns>none</returns>
void
NSMVINTC2V02::setInterruptLatency( uint64 iLatency )
{
  dmsgsc( mpModName << ".setInterruptLatency( 0x" << iLatency << " )" );
  mILatency = iLatency;
}

// VCD output for debug
/// <summary>enable VCD-output</summary>
/// <param name="tf">pointer to sc_trace_file</param>
/// <returns>none</returns>
void
NSMVINTC2V02::setTraceEnable( sc_trace_file* tf )
{
  sc_assert(tf !=NULL);
  dmsgsc( mpModName << ".setTraceEnable()" );
  int i, j;
  sc_trace( tf, busresetz, busresetz.name() );
  sc_trace( tf, busclk, busclk.name() );
  for ( i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    sc_trace( tf, *eiintl[i], eiintl[i]->name() );
    sc_trace( tf, *eiint_type[i], eiint_type[i]->name() );
  }
  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    sc_trace( tf, *i2_g_eiint[i], i2_g_eiint[i]->name() );
  }
  for ( i = 0; i < INTC_BCAST_NUM; i++) {
    sc_trace( tf, *i2_bcastrq[i], i2_bcastrq[i]->name() );
  }
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    sc_trace( tf, *i1_p_intack[j], i1_p_intack[j]->name() );
  }
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    sc_trace( tf, *cpu_gm_info[j], cpu_gm_info[j]->name() );
  }
}

// public functions
/// <summary>write the EIINT request port</summary>
/// <param name="gpid">group id</param>
/// <param name="val">write value</param>
/// <param name="procPri">priority of the process calling this function</param>
/// <returns>none</returns>
void
NSMVINTC2V02::setEiintReqPort(
  unsigned int peid,
  uint64       val,
  PRI_PROC_T   procPri )
{
  dmsgsc( mpModName << ".setEiintReqPort( 0x" << peid << ", 0x" << val << ", 0x" << procPri << " )" );
  EiintReq req_cancel;
  bool     w_flag = false; // write flag
  bool     c_flag = false; // cancel flag 

  if ( PE_MAX_NUM > peid ) {
    EiintReq req_prev( mEiReqInfo[peid].data );
    EiintReq req_curr( val );
    dmsgsc("  req_prev.val:0x" << req_prev.val);
    dmsgsc("  req_curr.val:0x" << req_curr.val);
    dmsgsc("  req_prev.bit_val.pri:0x" << req_prev.bit_val.pri);
    dmsgsc("  req_curr.bit_val.pri:0x" << req_curr.bit_val.pri);
    dmsgsc("  req_prev.bit_val.ch:0x" << req_prev.bit_val.ch);
    dmsgsc("  req_curr.bit_val.ch:0x" << req_curr.bit_val.ch);
    dmsgsc("  req_prev.bit_val.gm:0x" << req_prev.bit_val.gm);
    dmsgsc("  req_curr.bit_val.gm:0x" << req_curr.bit_val.gm);
    dmsgsc("  req_prev.bit_val.gpid:0x" << req_prev.bit_val.gpid);
    dmsgsc("  req_curr.bit_val.gpid:0x" << req_curr.bit_val.gpid);
    dmsgsc("  req_prev.bit_val.bg:0x" << req_prev.bit_val.bg);
    dmsgsc("  req_curr.bit_val.bg:0x" << req_curr.bit_val.bg);
    dmsgsc("  req_prev.bit_val.bgpr:0x" << req_prev.bit_val.bgpr);
    dmsgsc("  req_curr.bit_val.bgpr:0x" << req_curr.bit_val.bgpr);

    if ( (procPri <= mEiReqInfo[peid].drvProcPri) && (req_prev.val != req_curr.val) ) {
      if ( (1 == req_prev.bit_val.req) && (1 == req_curr.bit_val.req) ) { // update request
        if ( (req_prev.bit_val.pri > req_curr.bit_val.pri) ||
             ( (req_prev.bit_val.pri == req_curr.bit_val.pri) &&
               (req_prev.bit_val.ch > req_curr.bit_val.ch) ) ) { // current request is prior
            mReEntryReqList.insert( req_prev.bit_val.ch );
            dmsgsc("  mReEntryReqList.push_back( 0x" << req_curr.bit_val.ch << " ) size:0x" << mReEntryReqList.size());
          req_cancel.val         = req_prev.val;
          req_cancel.bit_val.req = 0;
          c_flag = true;
          w_flag = true;
        }
        else { // previous request is prior
          if ( req_prev.bit_val.ch != req_curr.bit_val.ch ) { // different channel request 
              mReEntryReqList.insert( req_curr.bit_val.ch );
              dmsgsc("  mReEntryReqList.push_back( 0x" << req_curr.bit_val.ch << " ) size:0x" << mReEntryReqList.size());
          }
          // else same channel request
        }
      }
      else if ( 0 == req_curr.bit_val.req ) { // cancel request
        if ((req_curr.bit_val.ch != req_prev.bit_val.ch) && (req_prev.bit_val.req == 1)) {
              dmsgsc("  mReEntryReqList.push_back( 0x" << req_prev.bit_val.ch << " ) size:0x" << mReEntryReqList.size());
              mReEntryReqList.insert(req_prev.bit_val.ch);
        }
        req_cancel.val = req_curr.val;
        c_flag = true;
        w_flag = true;
      }
      else  { // new request if ( 0 == req_prev.bit_val.req )
        w_flag = true;
      } 
    }
    // else process priority is lower than previous process.

    dmsgsc("  w_flag:" << w_flag << " c_flag:" << c_flag);
    if ( w_flag ) {
      dmsgsc("  val:0x" << val << " procPri:0x" << procPri);
      if ( c_flag ) {
          dmsgsc("  mEiCancelInfo[" << peid << "].insert( 0x" << req_cancel.val << " )");
          mEiCancelInfo[peid].insert( req_cancel.val );
      }
      mEiReqInfo[peid].data       = val;
      mEiReqInfo[peid].drvProcPri = procPri;
      meSetEiReq.notify();
    }
  }
}

/// <summary>write the broadcast port</summary>
/// <param name="portNum">broadcast port number(=EIBD.BCP)</param>
/// <param name="val">write value</param>
/// <param name="procPri">priority of the process calling this function</param>
/// <returns>none</returns>
void
NSMVINTC2V02::setBcastReqPort(
  unsigned int portNum,
  bool         val,
  PRI_PROC_T   procPri,
  bool         isReset )
{
  dmsgsc( mpModName << ".setBcastReqPort( 0x" << portNum << ", 0x" << val << ", 0x" << procPri << " )" );
  if ( INTC_BCAST_NUM > portNum ) {
      mBcastInfo[portNum].data       = val;
      mBcastInfo[portNum].drvProcPri = procPri;
      double freq = (double)busclk.read();
      if ( isReset ) {
        meSetBcast.notify();
      }
      else if ( 0.0 == freq ) {
        sprintf( mErrMsg, "Clock frequency is 0." );
        printErrorMessage( mpModName, "setBcastReqPort", mErrMsg );
      }
      else {
        meSetBcast.notify( ((double)mILatency)/freq, SC_SEC );
        // add the latency because this process is independent from ExecProc() process.
      }
    }
    // else process priority is lower than previous process.
}

/// <summary>notify meExec event to drive ExecProc() process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::notifyExecEvent( void )
{
  dmsgsc( mpModName << ".notifyExecEvent()" );
  double freq = (double)busclk.read();
  if ( 0.0 == freq ) {
    sprintf( mErrMsg, "Clock frequency is 0." );
    printErrorMessage( mpModName, "notifyExecEvent", mErrMsg );
  }
  else {
    dmsgsc( "  latency:" << ((double)mILatency)/freq );
    meExec.notify( ((double)mILatency)/freq, SC_SEC );
  }
}


// private functions
/// <summary>set parameters from configuration file</summary>
/// <param name="filename">configuration file name</param>
/// <returns>none</returns>
void
NSMVINTC2V02::readCfgFile( const char *filename )
{
  dmsgsc( mpModName << ".readCfgFile()" );
  char    word[512];
  char    seps[] = " ,=();\t\n\r";
  char*   token;
  sc_assert(filename !=NULL);
  
  ifstream  config_file(filename);
  // read the config file
  while( 1 ) {
    config_file.getline( word, 512, '\n' );
    token = strtok( word, seps );

    // ----------------------------------------
    // For EOF
    if ( config_file.eof() ) {
      break;
    }
    // else not EOF

    // ----------------------------------------
    // For comment
    if ( (NULL == token) || (0 == strncmp(token, "//", 2)) ) {
      continue;
    }
    // else not comment

    // ----------------------------------------
    // detect end marker
    // NOTICE:You have to check whether token is NULL first.
    if ( 0 == strcmp(token, "[END]") ) {
      break;
    }
    // else not [END]

    // ----------------------------------------
    // get token
    // ----------------------------------------
    // For miscellaneous
    if ( 0 == strcmp(token, "[PE_NUM]") ) {
      token = strtok( 0, seps );
      if (token != NULL) {
        mPeNum = token_to_uint( token,
                                "NSMVINTC2V02",
                                "[PE_NUM]" );
        dmsgsc( "  mPeNum:0x" << mPeNum );
        if ( mPeNum > PE_MAX_NUM ) {
          char msg[1024];
          // for WinMsg
          sprintf( msg,
                   "[PE_NUM] must be under %d, but specified (%d).",
                   PE_MAX_NUM+1, (int)mPeNum);
          printErrorMessage( "NSMVINTC2V02", "readCfgFile", msg );
          exit( 1 );
        }
        // else mPeNum is under PE_MAX_NUM
      }
      // else token is NULL
      continue;
    }
    else if ( 0 == strcmp(token, "[G4CPU_PE_TYPE]") ) {
      unsigned int peid;
      token = strtok( 0, seps );
      if ( NULL != token ) {
        peid = token_to_uint( token,
                              "NSMVINTC1V02",
                              "[G4CPU_PE_TYPE] peid(1st parameter)" );
        dmsgsc( "  peid:0x" << peid );
        if ( peid >= PE_MAX_NUM ) {
          char msg[1024];
          // for WinMsg
          sprintf( msg,
                   "[G4CPU_PE_TYPE] must be under %d, but specified (%d).",
                   PE_MAX_NUM, (int)peid );
          printErrorMessage( "NSMVINTC1V02", "readCfgFile", msg );
          exit( 1 );
        }else{
          token = strtok(0, seps);
          if(strcmp(token, "G4MH_V20") == 0){
           mIntc2Type = INTC2_G4MH20;
          } 
        }
      }
    }
    // else token is not [PE_NUM] and [G4CPU_PE_TYPE]
  }  // end of configuration file read loop(infinite)
}  // end of readCfgFile()

/// <summary>initialize register</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::initReg( void )
{
  dmsgsc( mpModName << ".initReg()" );
  uint8_t eic_data  = 0x80;
  //unsigned int eibd_data = 0x0;
  uint8_t eibd_data_8[4];
  memset(eibd_data_8, 0, 4);

  for ( int i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    if ( eiint_type[i]->read() ) { // high level detection type
      dmsgsc( "  mpIntc2Func->TargetWriteDebug(0x" << INTC2_EIC_OFFSET+(i*2+1) << " eic_data:0x" << (int)eic_data << ")" );
      mpIntc2Func->TargetWriteDebug( INTC2_EIC_OFFSET+(i*2+1), &eic_data, 1 );
    }
    // else edge detection type
    dmsgsc( "  mpIntc2Func->TargetWriteDebug(0x" << INTC2_EIBD_OFFSET+(i*4) << " eibd_data:0x0" << ")" );
    mpIntc2Func->TargetWriteDebug( INTC2_EIBD_OFFSET+(i*4), eibd_data_8, 4 );
  }
}


// processes
/// <summary>initialization process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::InitProc( void )
{
  dmsgsc( mpModName << ".InitProc()" );
  char name[128];

  for ( unsigned int i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    sc_spawn_options spawn_opt;
    spawn_opt.spawn_method();
    spawn_opt.dont_initialize();
    spawn_opt.set_sensitivity( eiintl[i] );
    spawn_opt.set_sensitivity( &meEiReqForLv[i] );
    sprintf( name, "%s_eiintl%d", this->basename(), (int)i );
    sc_spawn( sc_bind(&NSMVINTC2V02::EiintReqProc, this, i), name, &spawn_opt );
  }
}

/// <summary>reset process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::ResetProc( void )
{
  dmsgsc( mpModName << ".ResetProc()" );
  int i, j;

  mpIntc2Func->reset();
  initReg();

  for ( i = 0; i < PE_MAX_NUM; i++ ) {
    mEiCancelInfo[i].clear();
    setEiintReqPort( i, 0, PRI_RESET_PROC );
    //Fix issue #121481(check port after reset)
    mEiReqInfo[i].clear();
  }
  for ( i = 0; i < INTC_BCAST_NUM; i++ ) {
    mBcastInfo[i].clear();
    setBcastReqPort( i, false, PRI_RESET_PROC, true );
  }
  for ( i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    mPrevEiintl[i] = false;
  }
  for ( j = 0; j < PE_MAX_NUM; j++ ) {
    mPrevIntack[j] = 0;
  }
  mReEntryReqList.clear();
  resendEiint2Forbid = false;
  for ( i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
     meEiReqForLv[i].cancel();
  }
  meExec.cancel();
  meResendEiintReq.cancel();
  meSetEiReq.cancel();
  meSetBcast.cancel();
}

// peripheral -> INTC2
/// <summary>EIINT request process(dynamic)</summary>
/// <param name="">port number</param>
/// <returns>none</returns>
void
NSMVINTC2V02::EiintReqProc( unsigned int ei_num )
{
  dmsgsc( mpModName << ".EiintReqProc()" );
  bool   req_flag = false;
  double freq     = (double)busclk.read();

  if ( busresetz.read() ) {
    bool      val  = eiintl[ei_num]->read();
    TRegEIC*  pEic  = mpIntc2Func->GetEICPtr ( INTC1_CH_NUM + ei_num );
    TRegEEIC*  pEeic  = mpIntc2Func->GetEEICPtr ( INTC1_CH_NUM + ei_num );
    TRegEIBD* pEibd = mpIntc2Func->GetEIBDPtr( INTC1_CH_NUM + ei_num );

    if ( !mPrevEiintl[ei_num] && val ) { // rising edge of the request
      dmsgsc( "  rising EIMK(ch:0x" << INTC1_CH_NUM+ei_num << "):" << mpIntc2Func->GetEICPtr(INTC1_CH_NUM+ei_num)->GetEIMK() );
      if ( 1 == pEibd->GetCST() && !eiint_type[ei_num]->read()  ) { // broadcast && edge detection type
        // broadcast request is outputted from broadcast port directly(not use the queue)
        // because it does not need to be judged by its priority
        if ( 0 == pEic->GetEIMK() ) {
          dmsgsc( "  setBcastReqPort()" );
            setBcastReqPort( pEibd->GetBCP(), true, PRI_EIREQ_PROC, false );
        }
      }
      else {
        dmsgsc( "  ReqIntByPeripheral()" );
        mpIntc2Func->ReqIntByPeripheral( INTC1_CH_NUM + ei_num );
        if ( 0 == pEic->GetEIMK() ) {
          req_flag = true;
        }
      }
    }
    else if ( val && eiint_type[ei_num]->read() && // request=1 && high level detection type
              !(mpIntc2Func->IsIntReqInQueue(INTC1_CH_NUM+ei_num)) ) { // the request is not in the queue
      if ( 0 == pEic->GetEIMK() ) {
        dmsgsc( "  req:1 level EIMK:" << mpIntc2Func->GetEICPtr(INTC1_CH_NUM + ei_num)->GetEIMK() );
        dmsgsc( "  ReqIntByPeripheral()" );
        mpIntc2Func->ReqIntByPeripheral( INTC1_CH_NUM + ei_num ); // re-entry of the request
        req_flag = true;
      }
    }
    else if ( mPrevEiintl[ei_num] && !val && // falling edge of the request
              eiint_type[ei_num]->read() ) { // high level detection type
      dmsgsc( "QueueSize:0x" << mpIntc2Func->getQueueSize() );
      pEic->SetEIRF( 0 );
      pEeic->SetEIRF( 0 );
      std::set<unsigned int>::iterator it1 = mReEntryReqList.end();
      std::set<unsigned int>::iterator it2 = std::find(mReEntryReqList.begin(), mReEntryReqList.end(), INTC1_CH_NUM + ei_num);
      if ( &it1 != &it2) {
        mReEntryReqList.erase( INTC1_CH_NUM + ei_num );
      }
      dmsgsc( "QueueSize:0x" << mpIntc2Func->getQueueSize() );
    }
    // else (remain the same value | falling edge) & edge detection type
    mPrevEiintl[ei_num] = val;

    if ( req_flag ) {
      notifyExecEvent();
    }
  }
  // else reset is active
}

// INTC2 <- INTC1
/// <summary>EIINT acknowledge process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::EiintAckProc( void )
{
  dmsgsc( mpModName << ".EiintAckProc()" );
  EiintReq  req;
  EiintAck  ack, prevAck;
  TRegEIC*  pEic;
  TRegEEIC*  pEeic;
  TRegEIBD* pEibd;
  double    freq = (double)busclk.read();

  if ( busresetz.read() ) {
    for ( int j = 0; j < PE_MAX_NUM; j++ ) {
        ack.val     = i1_p_intack[j]->read();
        prevAck.val = mPrevIntack[j];
        if ( !(bool)prevAck.bit_val.ack && (bool)ack.bit_val.ack ) { // rising edge of acknowledge signal
          pEic = mpIntc2Func->GetEICPtr( ack.bit_val.ch );
          sc_assert(pEic !=NULL);
          pEeic = mpIntc2Func->GetEEICPtr( ack.bit_val.ch );
          sc_assert(pEeic !=NULL);
          resendEiint2Forbid = true;
          pEic->SetEIRF( 0 );
          pEeic->SetEIRF( 0 );
          dmsgsc( "eiint_type:0x" << eiint_type[ack.bit_val.ch-INTC1_CH_NUM]->read() );
          if ( eiint_type[ack.bit_val.ch-INTC1_CH_NUM]->read() ) { // high level detection type
            dmsgsc( "eiintl:0x" << eiintl[ack.bit_val.ch-INTC1_CH_NUM]->read() );
            if ( eiintl[ack.bit_val.ch-INTC1_CH_NUM]->read() ) { // request == high
              meEiReqForLv[ack.bit_val.ch-INTC1_CH_NUM].notify( SC_ZERO_TIME );
            }
            // else no request
          }
          // else edge detection type
        }
        else if ( (bool)prevAck.bit_val.ack && !(bool)ack.bit_val.ack ) { // falling edge of acknowledge signal
          std::set<unsigned int>::iterator it = mReEntryReqList.begin();
          while ( it != mReEntryReqList.end() ) {
            pEibd   = mpIntc2Func->GetEIBDPtr( *it );
            sc_assert(pEibd !=NULL);
            req.val = i2_g_eiint[pEibd->GetPEID()]->read();
            if ( 0 == req.bit_val.req ) {
              dmsgsc( "  ReqIntByEIC(0x" << *it << ")" );
              mpIntc2Func->ReqIntByEIC( *it );
              mReEntryReqList.erase( it++ );
            }
            else {
              it++;
            }
          }
          if ( 0 != mpIntc2Func->getQueueSize() ) { // continue to drive ExecProc() until the queue will be empty
            if ( 0.0 == freq ) {
              sprintf( mErrMsg, "Clock frequency is 0." );
              printErrorMessage( mpModName, "EiintAckProc", mErrMsg );
            }
            else {
              meExec.notify( 1.0/freq, SC_SEC );
            }
          }
          // else no request in the queue
        }
        // else remain the same value
        mPrevIntack[j] = ack.val;
    }
  }
  // else reset is active
}

/// <summary> notify event to resend Eiint </summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::notifyResendEiintReqEvent( void )
{
  dmsgsc( mpModName << ".notifyResendEiintReqEvent()" );
  double    freq = (double)busclk.read();
  if ( 0.0 == freq ) {
    sprintf( mErrMsg, "Clock frequency is 0." );
    printErrorMessage( mpModName, "notifyResendEiintReqEvent", mErrMsg );
  }
  else {
    meResendEiintReq.notify( 1.0/freq, SC_SEC );
  }
}
/// <summary> resend Eiint</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::ResendEiintProc( void )
{
  std::set<unsigned int>::iterator it = mReEntryReqList.begin();
     while ( it != mReEntryReqList.end() ) {
         dmsgsc( "  ReqIntByEIC(0x" << *it << ")" );
         mpIntc2Func->ReqIntByEIC( *it );
         mReEntryReqList.erase( it++ );
     }
     if ( 0 != mpIntc2Func->getQueueSize() ) { // continue to drive ExecProc() until the queue will be empty
         meExec.notify( SC_ZERO_TIME );
     }
     // else no request in the queue
}

/// <summary>monitor to get the info of CPU: GM, GPID</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::MonitorCPUInfoMethod( void ) // <-- added by chanle 20180131 for G4MH2.0 to support HV
{
  dmsgsc( mpModName << ".MonitorCPUInfoMethod()" );
  for (unsigned int i = 0; i < PE_MAX_NUM; i++){
    CpuGmInfo input_cpu_info (cpu_gm_info[i]->read());
    mCPUGM[i] = input_cpu_info.bit_val.cpu_gm;
    mCPUGPID[i] = input_cpu_info.bit_val.cpu_gpid;
    dmsgsc("        cpu_gm["<< i << "]: " << mCPUGM[i] << ", cpu_gpid[" << i << "]: " << mCPUGPID[i]);
    mpIntc2Func->SetCpuGuestMode(i, mCPUGM[i]);
    mpIntc2Func->SetCpuGpid(i, mCPUGPID[i]);
    notifyExecEvent(); // resend interrupt which has flag still 1 when cpu changes mode
  }
}

// INTC2 -> INTC1
/// <summary>process to write the i2_g_eiint port</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::SetEiintReqPortProc( void )
{
  dmsgsc( mpModName << ".SetEiintReqPortProc()" );
  
  for ( int i = 0; i < PE_MAX_NUM; i++ ) {
    if ( busresetz.read() ) {
      dmsgsc("  mEiCancelInfo[" << i << "].size():0x" << mEiCancelInfo[i].size());
      dmsgsc( ".mEiReqInfo[i].data 0x" << mEiReqInfo[i].data );
      if ( 0 == mEiCancelInfo[i].size() ) {
        i2_g_eiint[i]->write( mEiReqInfo[i].data );
      }
      else {
        set<uint64>::iterator it = mEiCancelInfo[i].begin();
        i2_g_eiint[i]->write( *it );
        mEiCancelInfo[i].erase( it );
        meSetEiReq.notify( SC_ZERO_TIME );
      }
    }
    else { // reset is active
      i2_g_eiint[i]->write( 0 );
    }
    mEiReqInfo[i].drvProcPri = PRI_DEFAULT;
  }
}

// INTC2 -> G4SS
/// <summary>process to write the i2_bcastrq port</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::SetBcastReqPortProc( void )
{
  bool req_flag = false;
  dmsgsc( mpModName << ".SetBcastReqPortProc()" );
  for ( int i = 0; i < INTC_BCAST_NUM; i++ ) {
    if ( busresetz.read() ) {
      dmsgsc( "  mBcastInfo[" << i << "].data:" << mBcastInfo[i].data );
      i2_bcastrq[i]->write( mBcastInfo[i].data );
      if ( mBcastInfo[i].data ) {
        mBcastInfo[i].data = false;
        req_flag           = true;
      }
    }
    else { // reset is active
      i2_bcastrq[i]->write( false );
    }
    mBcastInfo[i].drvProcPri = PRI_DEFAULT;
  }
  if ( req_flag ) {
    double freq = (double)busclk.read();
    if ( 0.0 == freq ) {
      sprintf( mErrMsg, "Clock frequency is 0." );
      printErrorMessage( mpModName, "SetBcastReqPortProc", mErrMsg );
    }
    else { // drop the request after 1 clock
      meSetBcast.notify( 1.0/freq, SC_SEC );
    }
  }
}

/// <summary>process to execute INTC2 function</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC2V02::ExecProc( void )
{
  dmsgsc( mpModName << ".ExecProc()" );

  if ( busresetz.read() ) {
    mpIntc2Func->CyclicHandler();
  }
  // else reset is active
}

/// <summary>get info GM of a CPU</summary>
/// <param name="cpu_index">index of CPU</param>
/// <returns>bool: value of GM of a CPU</returns>
bool
NSMVINTC2V02::getCPUGM( unsigned int cpu_index )
{
  return mCPUGM[cpu_index];
}

/// <summary>get info GPID of a CPU</summary>
/// <param name="cpu_index">index of CPU</param>
/// <returns>unsigned int: value of GPID of a CPU</returns>
unsigned int
NSMVINTC2V02::getCPUGPID( unsigned int cpu_index )
{
  return mCPUGPID[cpu_index];
}
