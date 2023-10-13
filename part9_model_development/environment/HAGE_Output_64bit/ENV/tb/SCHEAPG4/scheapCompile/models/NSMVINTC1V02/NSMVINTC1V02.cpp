/*************************************************************************
 *
 * $Id: NSMVINTC1V02.cpp,v 1.17 2020/04/09 08:22:25 ngathunguyen Exp $
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
#include "NSMVINTC1V02.h"
#include "G4_Intc1Func.h"
#include "Common.h"
#include "CmErrMsg.h"


using namespace std;
using namespace CmErrMsg;
using namespace G4IntcTyp;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT


class Intc2Dummy: public CIntc2 {
public:
  Intc2Dummy( std::vector<PeidInfo*>* pPeidInfo, Intc2ImplType intc2_type ): CIntc2( pPeidInfo, 0, intc2_type ) { /* do nothing */ }
  virtual ~Intc2Dummy(void) { ; }
  virtual void  StartCyclicHandler( void ) { /* do nothing */ }
  virtual void  StopCyclicHandler( void ) { /* do nothing */ }
  virtual void  ReqIntToIntc1( uint32_t peid, uint32_t channel, IntReqInfoBase* reqinfo ) { /* do nothing */ }
  virtual void  CancelIntToIntc1( uint32_t peid, uint32_t channel ) { /* do nothing */ }
  virtual void  CancelIntByMask( uint32_t channel ) { /* do nothing */ }
  virtual void  UpdateHighPriority( uint32_t peid ) { /* do nothing */ }
  virtual void  UpdateHighPriorityOfIntc1( uint32_t peid, uint32_t channel ) { /* do nothing */ }
  virtual bool  IsIntReqInQueueOfIntc1( uint32_t peid, uint32_t channel ) { return false; }
  virtual void  AddBcasIntInfo( uint32_t channel ) { /* do nothing */ }
};

/// <summary>constructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
Intc2ReqInfo::Intc2ReqInfo( unsigned int intc2_type ) :
  mpIntc2Dummy( 0 ),
  mpPesDummy( 0 ),
  mpPeInfoDummy( 0 )
{
  dmsgsc( "Intc2ReqInfo.Intc2ReqInfo()" );

  mpPesDummy    = new std::vector<PeidInfo*>;
  mpPeInfoDummy = new PeidInfo( 0 );
  mpPesDummy->push_back( NULL );
  mpPesDummy->push_back( mpPeInfoDummy );
  mpIntc2Dummy  = new Intc2Dummy( mpPesDummy, (Intc2ImplType)intc2_type );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
Intc2ReqInfo::~Intc2ReqInfo( void )
{
  dmsgsc( "Intc2ReqInfo.~Intc2ReqInfo()" );

  delete mpIntc2Dummy;
  delete mpPesDummy;
  delete mpPeInfoDummy;
}

/// <summary>reset function</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
Intc2ReqInfo::reset( void )
{
  dmsgsc( "Intc2ReqInfo.reset()" );

  mpIntc2Dummy->HardReset();
  initReg();
}

/// <summary>set EIINT request data from INTC2 to data-base</summary>
/// <param name="intReq">EIINT request data</param>
/// <param name="pe_id">PE Id</param>
/// <returns>none</returns>
void
Intc2ReqInfo::setReqInfo( G4IntcTyp::EiintReq intReq, unsigned int pe_id)
{
  dmsgsc( "Intc2ReqInfo.setReqInfo()" );
  if ( INTC1_CH_NUM > intReq.bit_val.ch || EI_ALL_CH_NUM <= intReq.bit_val.ch ) {
    sprintf( mErrMsg, "Invalid CH(%llu) value. (CH must be %d-%d).", intReq.bit_val.ch, INTC1_CH_NUM, EI_ALL_CH_NUM-1 );
    printErrorMessage( "NSMVINTC1V02::Intc2ReqInfo", "setReqInfo", mErrMsg );
  }
  else {
    TRegEIC* pEic = getEicPtr( intReq.bit_val.ch );
    TRegEEIC* pEeic = getEeicPtr(intReq.bit_val.ch);
    TRegEIBD* pEibd = mpIntc2Dummy->GetEIBDPtr(intReq.bit_val.ch);
    TRegEIBG* pEibg = getEibgPtr(pe_id);
    pEic->SetEIRF( 1 ); // exist request
    pEic->SetEIMK( 0 ); // unmasked(because request has been received)
    pEic->SetEITB( intReq.bit_val.tb );
    pEic->SetEIP( intReq.bit_val.pri & 0xF); // 4 bits priority in EIC.EIP
    pEeic->SetEIRF( 1 ); // exist request
    pEeic->SetEIMK( 0 ); // unmasked(because request has been received)
    pEeic->SetEITB( intReq.bit_val.tb );
    pEeic->SetEIP( intReq.bit_val.pri); // higher bits in priority are set to EEIC.EIP
    if ( (intReq.bit_val.gm > 0) || (intReq.bit_val.bg > 0) ){
      pEibd->SetGM (1);
    }
    pEibd->SetGPID (intReq.bit_val.gpid);
    if (intReq.bit_val.bg != 0){// only set BGPR bit in case BGxxx interrupt (BGPR bit is used by ReqEiintToCpu())
      pEibg->SetBGPR(intReq.bit_val.bgpr);
    }else{
      pEibg->SetBGPR(0);
    }

  }
}

/// <summary>get EIINT request data from INTC2 from data-base</summary>
/// <param name="channel">number of channel</param>
/// <returns>pointer to IntReqInfoBase(declared in CForest INTC2)</returns>
IntReqInfoBase*
Intc2ReqInfo::getReqInfo( unsigned int channel )
{
  dmsgsc( "Intc2ReqInfo.getReqInfo() ch:0x" << channel );

  return mpIntc2Dummy->CreateEiintReqInfo( channel );
}

/// <summary>get the pointer to the EEIC register</summary>
/// <param name="channel">number of channel</param>
/// <returns>pointer to the EEIC register</returns>
TRegEEIC*
Intc2ReqInfo::getEeicPtr( unsigned int channel )
{
  dmsgsc( "Intc2ReqInfo.getEeicPtr() ch:0x" << channel );

  return mpIntc2Dummy->GetEEICPtr( channel );
}

/// <summary>get the pointer to the EIC register</summary>
/// <param name="channel">number of channel</param>
/// <returns>pointer to the EIC register</returns>
TRegEIC*
Intc2ReqInfo::getEicPtr( unsigned int channel )
{
  dmsgsc( "Intc2ReqInfo.getEicPtr() ch:0x" << channel );

  return mpIntc2Dummy->GetEICPtr( channel );
}

/// <summary>get the pointer to the EIBD register</summary>
/// <param name="channel">number of channel</param>
/// <returns>pointer to the EIBD register</returns>
TRegEIBD*
Intc2ReqInfo::getEibdPtr( unsigned int channel )
{
  dmsgsc( "Intc2ReqInfo.getEibdPtr() ch:0x" << channel );

  return mpIntc2Dummy->GetEIBDPtr( channel );
}

/// <summary>get the pointer to the EIBG register</summary>
/// <param name="channel">number of channel</param>
/// <returns>pointer to the EIBG register</returns>
TRegEIBG*
Intc2ReqInfo::getEibgPtr( unsigned int peid )
{
  dmsgsc( "Intc2ReqInfo.getEibgPtr() peid:0x" << peid );

  return mpIntc2Dummy->GetEIBGPtr( peid );
}

/// <summary>initialize register</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
Intc2ReqInfo::initReg( void )
{
  dmsgsc( "Intc2ReqInfo.initReg()" );
  uint8_t eibd_data = 0x0;

  for ( int i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    dmsgsc( "  mpIntc2Dummy->TargetWriteDebug(0x" << INTC2_EIBD_OFFSET+(i*4) << " eibd_data:0x" << (int)eibd_data << ")" );
    mpIntc2Dummy->TargetWriteDebug( INTC2_EIBD_OFFSET+(i*4), &eibd_data, 1 );
  }
}

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pCfgFileName">configuration file name</param>
/// <param name="peid">PEID</param>
/// <returns>none</returns>
NSMVINTC1V02::NSMVINTC1V02( 
  sc_module_name name,
  const char*    pCfgFileName,
  unsigned int   peid ):
    sc_module( name ),
    BusSlaveBase<BUS_WIDTH_VCI32, 1>(),  
    BusTimeBase(),
    resetz( "resetz" ),
    clk( "clk" ),
    fenmi( "fenmi" ),
    t_eiint_1l( "t_eiint_1l" ),
    t_intack_1l( "t_intack_1l" ),
    fenmirq_1l( "fenmirq_1l" ),
    fenmiack_1l( "fenmiack_1l" ),
    t_feintrq_1l( "t_feintrq_1l" ),
    t_feintack_1l( "t_feintack_1l" ),
    cpu_gm_info( "cpu_gm_info"), /* added by Yohsinaga for G4MH 2.0 */
    mpModName( 0 ),
    mpIntc2ReqInfo( 0 ), 
    mpIntc1Func( 0 ),
    mPrevExFnReq( false ),
    mFeintCancelForce( false ), 
    mPeid( peid ),
    mIntc2Type( INTC2_STANDARD ),
    mILatency( 0 ),
    mCPUGM(false),
    mCPUGPID(0),
    mCoreVersion(CORE_VER_10),
    mHvcfgIniVal(0)
{
  mpModName = this->name();
  dmsgsc( mpModName << ".NSMVINTC1V02()" );
  char port_name[32];
  int  i;

  // get configuration parameter
  readCfgFile( pCfgFileName );

  setSlaveResetPort32( &resetz );
  setSlaveFreqPort32( &clk );
  setTargetSocket32( (char*)"ts" );
  mBusSlaveIf32[0]->setEnableToCheckDataSize();
  ts = tSocket32[0];

  mpIntc2ReqInfo = new Intc2ReqInfo( mIntc2Type );

  // added the 4th argement by Yoshinaga for G4MH2.0 (new cforestg4)
  mpIntc1Func = new G4_Intc1Func( peid, 0, this, mCoreVersion );
  mpIntc1Func->SetIntc2Ptr( mpIntc2ReqInfo->mpIntc2Dummy );

  // added the setting by Yoshinaga for G4MH2.0 (new cforestg4) 
  // refer to intc1_g4mh20_development_20171211.xlsx, line 145-146
  if((mCoreVersion == CORE_VER_10) || (mCoreVersion == CORE_VER_11)){
    mpIntc1Func->SetHasHVExtPin(0);
    mpIntc1Func->SetHVEExtPin(0);
  }else{
    // CORE_VER_20
    mpIntc1Func->SetHasHVExtPin(1);
    mpIntc1Func->SetHVEExtPin(mHvcfgIniVal);
  }


  mBusSlaveIf32[0]->setFuncModulePtr( mpIntc1Func );
  mBusSlaveIf32[0]->setBusProtocol( BUS_VCI );
  mBusSlaveIf32[0]->setTransNmbLmt( 1 );

  // initialize output port
  t_eiint_1l.initialize(0x0);
  fenmirq_1l.initialize(false);
  t_feintrq_1l.initialize(0x0);
  // create the instances of ports
  for ( i = 0; i < INTC1_CH_NUM; i++ ) {
    sprintf( port_name, "eiintl_%d", i);
    eiintl[i] = new sc_in<bool>(port_name);
    sprintf( port_name, "eiint_type_%d", i);
    eiint_type[i] = new sc_in<bool>(port_name);
  }
  
  for ( i = 0; i < INTC1_FEINT_CH_NUM; i++ ) {
    sprintf( port_name, "feint_%d", i);
    feint[i] = new sc_in<bool>(port_name);
    sprintf( port_name, "feint_type_%d", i);
    feint_type[i] = new sc_in<bool>(port_name);
  }

  i1_t_intack = new sc_out<unsigned int>("i1_t_intack");
  i1_t_intack->initialize(0x0);

  i2_g_eiint = new sc_in<uint64>("i2_g_eiint");

  // Initialize Variable
  mEiReqInfo.clear();
  mEiAckInfo.clear();
  mFiReqInfo.clear();
  mEiCancelInfo.clear();
  mFeintCancelInfo.clear();
  mFnReqInfo.clear();
  mReEntryReqList.clear();
  mPrevExFiAck = false;
  // reset process
  SC_METHOD( ResetProc );
  sensitive << resetz.neg();

  // EIINT request process (from INTC2)
  SC_METHOD( I2EiintReqProc );
  sensitive << *i2_g_eiint;
  dont_initialize();
  mPrevI2EiReq = 0;

  // EIINT acknowledge process
  SC_METHOD( EiintAckProc );
  sensitive << t_intack_1l;
  dont_initialize();
  mPrevI2EiAck = 0;
  resendEiintForbid = false;

  // EIINT request process (from peripheral)
  SC_METHOD( ExEiintReqProc );
  for ( i = 0; i < INTC1_CH_NUM; i++ ) {
    sensitive << *eiintl[i];
    mPrevExEiReq[i] = false;
  }
  sensitive << meEiReq1ForLvDetect;
  dont_initialize();

  // FENMI request process
  SC_METHOD( ExFenmiReqProc );
  sensitive << fenmi;
  dont_initialize();

  // FENMI acknowledge process
  SC_METHOD( ExFenmiAckProc );
  sensitive << fenmiack_1l;
  dont_initialize();

  // FEINT request process
  SC_METHOD( ExFeintReqProc );
  for ( i = 0; i < INTC1_FEINT_CH_NUM; i++ ) {
    sensitive << *feint[i];
    mPrevExFiReq[i] =  false;
  }
  sensitive << meFiReqForLvDetect;
  dont_initialize();

  // FEINT acknowledge process
  SC_METHOD( ExFeintAckProc );
  sensitive << t_feintack_1l;
  dont_initialize();
  resendFeintForbid = false;
  
  // t_eiint_1l port write process
  SC_METHOD( SetEiintReqPortProc );
  sensitive << meSetEiReq;
  dont_initialize();

  // i1_t_intack port write process
  SC_METHOD( SetEiintAckPortProc );
  sensitive << meSetEiAck;
  dont_initialize();

  // fenmirq_1l port write process
  SC_METHOD( SetFenmiReqPortProc );
  sensitive << meSetFnReq;
  dont_initialize();

  // t_feintrq_1l port write process
  SC_METHOD( SetFeintReqPortProc );
  sensitive << meSetFiReq;
  dont_initialize();

  // CyclicHandler() execution process
  SC_METHOD( ExecProc );
  sensitive << meExec;
  dont_initialize();

  // Monitor info of CPU <-- added by chanle 20180129 for G4MH2.0
  SC_METHOD(MonitorCPUInfoMethod);
  dont_initialize();
  sensitive << cpu_gm_info;
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
NSMVINTC1V02::~NSMVINTC1V02( void )
{
  dmsgsc( mpModName << ".~NSMVINTC1V02()" );
  int i, j;

  delete mpIntc2ReqInfo;
  delete mpIntc1Func;
  for ( i = 0; i < INTC1_CH_NUM; i++ ) {
    delete eiintl[i];
    delete eiint_type[i];
  }
  delete i1_t_intack;
  delete i2_g_eiint;
  for (j = 0; j < INTC1_FEINT_CH_NUM; j++) {
      delete feint[j];
      delete feint_type[j];
  }
}


// set function
/// <summary>set read-latency of register access</summary>
/// <param name="rLatency">read-latency</param>
/// <returns>none</returns>
void
NSMVINTC1V02::setReadRegLatency( uint64 rLatency )
{
  dmsgsc( mpModName << ".setReadRegLatency( 0x" << rLatency << " )" );
  mBusSlaveIf32[0]->setReadLatency( rLatency  );
}

/// <summary>set write-latency of register access</summary>
/// <param name="wLatency">write-latency</param>
/// <returns>none</returns>
void
NSMVINTC1V02::setWriteRegLatency( uint64 wLatency )
{
  dmsgsc( mpModName << ".setWriteRegLatency( 0x" << wLatency << " )" );
  mBusSlaveIf32[0]->setWriteLatency( wLatency  );
}

/// <summary>set latency to send interrupt signal</summary>
/// <param name="iLatency">latency to send interrupt signal</param>
/// <returns>none</returns>
void
NSMVINTC1V02::setInterruptLatency( uint64 iLatency )
{
  dmsgsc( mpModName << ".setInterruptLatency( 0x" << iLatency << " )" );
  mILatency = iLatency;
}

// VCD output for debug
/// <summary>enable VCD-output</summary>
/// <param name="tf">pointer to sc_trace_file</param>
/// <returns>none</returns>
void
NSMVINTC1V02::setTraceEnable( sc_trace_file* tf )
{
  dmsgsc( mpModName << ".SetTraceEnable()" );
  int i;
  sc_assert(tf != NULL);
  sc_trace( tf, resetz, resetz.name() );
  sc_trace( tf, clk, clk.name() );
  for ( i = 0; i < INTC1_CH_NUM; i++ ) {
    sc_trace( tf, *eiintl[i], eiintl[i]->name() );
    sc_trace( tf, *eiint_type[i], eiint_type[i]->name() );
  }
  sc_trace( tf, fenmi, fenmi.name() );
  for ( i = 0; i < INTC1_FEINT_CH_NUM; i++ ) {
    sc_trace( tf, *feint[i], feint[i]->name() );
    sc_trace( tf, *feint_type[i], feint_type[i]->name() );
  }
  sc_trace( tf, fenmirq_1l, fenmirq_1l.name() );
  sc_trace( tf, fenmiack_1l, fenmiack_1l.name() );
  sc_trace( tf, t_eiint_1l, t_eiint_1l.name() );
  sc_trace( tf, t_intack_1l, t_intack_1l.name() );
  sc_trace( tf, t_feintrq_1l, t_feintrq_1l.name() );
  sc_trace( tf, t_feintack_1l, t_feintack_1l.name() );
  sc_trace( tf, *i1_t_intack, i1_t_intack->name() );
  sc_trace( tf, *i2_g_eiint, i2_g_eiint->name() );
  sc_trace( tf, cpu_gm_info, cpu_gm_info.name() );
}

// public functions
// INTC1 -> CPU
/// <summary>write the EIINT request port</summary>
/// <param name="val">write value</param>
/// <param name="procPri">priority of the process calling this function</param>
/// <returns>none</returns>
void
NSMVINTC1V02::setEiintReqPort(
  uint64       val,
  PRI_PROC_T   procPri )
{
  dmsgsc( mpModName << ".setEiintReqPort( 0x" << val << " )" );
  EiintReq req_cancel;
  bool     w_flag = false; // write flag
  bool     c_flag = false; // cancel flag 

  EiintReq req_prev( mEiReqInfo.data );
  EiintReq req_curr( val );
  dmsgsc("  req_prev.val:0x" << req_prev.val);
  dmsgsc("  req_curr.val:0x" << req_curr.val);
  dmsgsc("  mEiReqInfo.drvProcPri:0x" <<  mEiReqInfo.drvProcPri);
  dmsgsc("  procPri:0x" << procPri);
  if ( (procPri <= mEiReqInfo.drvProcPri) && (req_prev.val != req_curr.val) ) {
    if ( (1 == req_prev.bit_val.req) && (1 == req_curr.bit_val.req) ) { // update request
      if ( (req_prev.bit_val.pri > req_curr.bit_val.pri) ||
           ( (req_prev.bit_val.pri == req_curr.bit_val.pri) && (req_prev.bit_val.ch > req_curr.bit_val.ch) ) ) {
        // current request is prior
        mReEntryReqList.insert( req_prev.bit_val.ch );
        dmsgsc("  mReEntryReqList.insert( 0x" << req_prev.bit_val.ch << " ) size:0x" << mReEntryReqList.size());
        req_cancel.val         = req_prev.val;
        req_cancel.bit_val.req = 0;
        c_flag = true;
        w_flag = true;
      }
      else { // previous request is prior
          mReEntryReqList.insert( req_curr.bit_val.ch );
          dmsgsc("  mReEntryReqList.insert( 0x" << req_curr.bit_val.ch << " ) size:0x" << mReEntryReqList.size());
      }
    }
    else if ( 0 == req_curr.bit_val.req ) { // cancel request
      req_cancel.val = req_curr.val;
      c_flag = true;
      w_flag = true;
    }
    else{ // new request
      w_flag = true;
    }
  }

  dmsgsc("  w_flag:" << w_flag << " c_flag:" << c_flag);
  if ( w_flag ) {
    dmsgsc("  val:0x" << val << " procPri:0x" << procPri);
    if ( c_flag ) {
        dmsgsc("  mEiCancelInfo.insert( 0x" << req_cancel.val << " )");
        mEiCancelInfo.insert( req_cancel.val );
    }
    mEiReqInfo.data       = val;
    mEiReqInfo.drvProcPri = procPri;
    meSetEiReq.notify();
  }
}

// INTC2 -> INTC1
/// <summary>write the EIINT acknowledge port</summary>
/// <param name="val">write value</param>
/// <param name="procPri">priority of the process calling this function</param>
/// <returns>none</returns>
void
NSMVINTC1V02::setEiintAckPort(
  unsigned int val,
  PRI_PROC_T procPri )
{
  dmsgsc( mpModName << ".setEiintAckPort( 0x" << val << " )" );
  if ( procPri <= mEiAckInfo.drvProcPri ) {
    mEiAckInfo.data       = val;
    mEiAckInfo.drvProcPri = procPri;
    meSetEiAck.notify();
  }
  // else process priority is lower than previous process.
}

// INTC1 -> CPU
/// <summary>write the FENMI request port</summary>
/// <param name="val">write value</param>
/// <param name="procPri">priority of the process calling this function</param>
/// <returns>none</returns>
void
NSMVINTC1V02::setFenmiReqPort(
  bool         val,
  PRI_PROC_T   procPri )
{
  dmsgsc( mpModName << ".setFenmiReqPort( 0x" << val << " )" );
  if ( procPri <= mFnReqInfo.drvProcPri ) {
    mFnReqInfo.data       = val;
    mFnReqInfo.drvProcPri = procPri;
    meSetFnReq.notify();
  }
  // else process priority is lower than previous process.
}

// INTC1 -> CPU
/// <summary>write the FEINT request port</summary>
/// <param name="val">write value</param>
/// <param name="procPri">priority of the process calling this function</param>
/// <returns>none</returns>
void
NSMVINTC1V02::setFeintReqPort(
  unsigned int   val,
  PRI_PROC_T   procPri )
{
  FeintReq req_cancel; 
  FeintReq req_prev( mFiReqInfo.data ); 
  FeintReq req_curr( val );
  bool     w_flag = false;
  bool     c_flag = false; 
  dmsgsc( mpModName << ".setFeintReqPort( 0x" << val << ", " << procPri << " )" );
  dmsgsc( mpModName << "mFiReqInfo.drvProcPri=" << mFiReqInfo.drvProcPri << ", mFeintCancelForce=" << mFeintCancelForce );
  
  if ( (procPri <= mFiReqInfo.drvProcPri) && (req_prev.val != req_curr.val) ) {
    if ( (1 == req_prev.bit_val.req) && (1 == req_curr.bit_val.req) ) { // update request
      if (req_prev.bit_val.ch > req_curr.bit_val.ch) {
        // current request is prior
        req_cancel.val         = req_prev.val;
        req_cancel.bit_val.req = 0;
        c_flag = true;
        w_flag = true;
      }
    }
    else if ( 0 == req_curr.bit_val.req ) { // cancel request
      req_cancel.val = req_curr.val;
      c_flag = true;
      w_flag = true;
    }
    else { // new request
      w_flag = true;
    }
  }

  dmsgsc("  w_flag:" << w_flag << " c_flag:" << c_flag);
  if ( w_flag ) {
    dmsgsc("  val:0x" << val << " procPri:0x" << procPri);
    if ( c_flag ) {
        dmsgsc("  mFeintCancelInfo.insert( 0x" << req_cancel.val << " )");
        mFeintCancelInfo.insert( req_cancel.val );
    }
    mFiReqInfo.data       = val;
    mFiReqInfo.drvProcPri = procPri;
    meSetFiReq.notify();
  }
  
}

// INTC1 -> CPU
/// <summary>notify meExec event to drive ExecProc() process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::notifyExecEvent( void )
{
  dmsgsc( mpModName << ".notifyExecEvent()" );
  double freq = (double)clk.read();
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
NSMVINTC1V02::readCfgFile( const char *filename )
{
  dmsgsc( mpModName << ".readCfgFile()" );
  char    word[512];
  char    msg[1024];
  char    seps[] = " ,=();\t\n\r";
  char*   token;
  sc_assert(filename != NULL);
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
    if ( 0 == strcmp(token, "[G4CPU_PE_TYPE]") ) {
      unsigned int peid;
      token = strtok( 0, seps );
      if (NULL != token) {
        peid = token_to_uint( token,
                              "NSMVINTC1V02",
                              "[G4CPU_PE_TYPE] peid(1st parameter)" );
        dmsgsc( "  peid:0x" << peid );
        if ( peid >= PE_MAX_NUM ) {
          // for WinMsg
          sprintf( msg,
                   "[G4CPU_PE_TYPE] must be under %d, but specified (%u).",
                   PE_MAX_NUM, peid );
          printErrorMessage( "NSMVINTC1V02", "readCfgFile", msg );
          exit( 1 );
        }
#if 0 // original code for G4MH 1.x (E2xFCC1 and E2xFCC2) 
        else if ( 1 == peid ) { 
          token = strtok( 0, seps );
          mIntc2Type = INTC2_STANDARD;
        }
#else // changed by Yoshinaga for G4MH 1.x and 2.0
        else {
          token = strtok(0, seps);
          if(strcmp(token, "G4MH") == 0){
            mCoreVersion = CORE_VER_10;
          }else if(strcmp(token, "G4MH_V10") == 0){
            mCoreVersion = CORE_VER_10;
          }else if(strcmp(token, "G4MH_V11") == 0){
            mCoreVersion = CORE_VER_11;
          }else if(strcmp(token, "G4MH_V20") == 0){
           mCoreVersion = CORE_VER_20;
           mIntc2Type = INTC2_G4MH20;
          } else{
            mCoreVersion = CORE_VER_10;
            // Please consider and add the error or warning message operation for yourself        
          } 
        }
#endif
      }
      continue; // added by Yoshinaga for G4MH1.x and 2.0
    }

    // added by Yoshinaga for G4MH1.x and 2.0
    if (strcmp(token, "[G4CPU_HV_INFO]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (NULL != token){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_HV_INFO] pe index(1st parameter)" );
        if( peid == mPeid ){
          token = strtok(0, seps);
          if(strcmp(token, "true") == 0){
            mHvcfgIniVal = 1;
          } else {
            mHvcfgIniVal = 0;
          }
        }
      }
      continue;
    }

#if 0 // deleted by Yoshinaga
    // else token is not [G4CPU_PE_NUM]
#endif

  }  // end of configuration file read loop(infinite)
}  // end of readCfgFile()

/// <summary>initialize register</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::initReg( void )
{
  dmsgsc( mpModName << ".initReg()" );
  uint8_t data = 0x80;

  for ( int i = 0; i < INTC1_CH_NUM; i++ ) {
    if ( eiint_type[i]->read() ) { // high level detection type
      mpIntc1Func->TargetWriteDebug( INTC1_EIC_OFFSET+(i*2+1), &data, 1 );
    }
    // else edge detection type
  }

  for ( int i = 0; i < INTC1_FEINT_CH_NUM; i++ ) {
    if ( feint_type[i]->read() ) { // high level detection type
      mpIntc1Func->TargetWriteDebug( INTC1_FIC_OFFSET+(i*2+1), &data, 1 );
    }
    // else edge detection type
  }
}

/// <summary>reset process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::ResetProc( void )
{
  dmsgsc( mpModName << ".ResetProc()" );
  int i;

  mpIntc1Func->reset();
  initReg();
  mpIntc2ReqInfo->reset(); 
  mReEntryReqList.clear();

  setFenmiReqPort( false, PRI_RESET_PROC );
  mPrevI2EiReq = 0;

  mPrevI2EiAck = 0;
  mPrevExFiAck = false;
  mEiAckInfo.clear();
  mEiCancelInfo.clear();
  mFeintCancelInfo.clear();
  setEiintReqPort( 0,     PRI_RESET_PROC );
  setFeintReqPort( 0,     PRI_RESET_PROC );
  setEiintAckPort( 0,     PRI_RESET_PROC );
  //Fix issue #121481(check port after reset)
  mEiReqInfo.clear();
  mFiReqInfo.clear();

  for ( i = 0; i < INTC1_CH_NUM; i++ ) {
    mPrevExEiReq[i] = false;
  }
  mPrevExFnReq = false;
  for ( i = 0; i < INTC1_FEINT_CH_NUM; i++ ) {
    mPrevExFiReq[i] = false;
  }
  mFnReqInfo.clear();
  resendEiintForbid = false;
  resendFeintForbid = false;
  meSetEiReq.cancel();
  meSetEiAck.cancel();
  meSetFnReq.cancel();
  meSetFiReq.cancel();
  meExec.cancel();
  meEiReq1ForLvDetect.cancel();
  meFiReqForLvDetect.cancel();
}

// INTC2 -> INTC1
/// <summary>EIINT(from INTC2) request process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::I2EiintReqProc( void )
{
  dmsgsc( mpModName << ".I2EiintReqProc()" ); 
  EiintReq     req, prevReq;
  EiintReqBind bind, prevBind;
  bool         req_flag = false;

  if ( resetz.read() ) {
    req.val      = i2_g_eiint->read();
    prevReq.val  = mPrevI2EiReq;
    bind.val     = req.bit_val.bind;
    prevBind.val = prevReq.bit_val.bind;

    if ( (bool)req.bit_val.req && (req.val != prevReq.val) ) { // request is 1 && request port has been changed
      if ( INTC1_CH_NUM > req.bit_val.ch || EI_ALL_CH_NUM <= req.bit_val.ch ) { // check of channel value
        sprintf( mErrMsg, "Invalid CH(%llu) value. (CH must be %d-%d)", req.bit_val.ch, INTC1_CH_NUM, EI_ALL_CH_NUM-1 );
        printErrorMessage( mpModName, "I2EiintReqProc", mErrMsg );
      }
      else {
        if ( (bool)prevReq.bit_val.req ) { // changed value except request
          IntReqInfoBase* pReqInfo = mpIntc1Func->getQueueReqInfo( prevReq.bit_val.ch );
          if ( 0 != pReqInfo ) { // previous request is still in the queue
            if ( req.bit_val.pri < static_cast<unsigned int>(pReqInfo->GetPriority()) ) { // current priority is higher
              mpIntc1Func->CancelInt( req.bit_val.ch ); // remove the request from queue in INTC1 and drop the request
              TRegEIC* pEic = mpIntc2ReqInfo->getEicPtr( req.bit_val.ch );
              TRegEEIC* pEeic = mpIntc2ReqInfo->getEeicPtr( req.bit_val.ch );
              dmsgsc("  pEic->SetEIRF( 0 ) (dummy)ch:0x" << req.bit_val.ch);
              pEic->SetEIRF( 0 );
              pEeic->SetEIRF( 0 );
              set<unsigned int>::iterator it1 = mReEntryReqList.end();
              set<unsigned int>::iterator it2 = std::find(mReEntryReqList.begin(), mReEntryReqList.end(), req.bit_val.ch);
              if ( &it1 != &it2) {
                mReEntryReqList.erase( req.bit_val.ch );
              }
            }
            // else current priority is lower
          }
          // else previous request is not in the queue
        }
        // else rising edge of request
        mpIntc2ReqInfo->setReqInfo( req , mPeid); 
        mpIntc1Func->ReqIntByIntc2( req.bit_val.ch, mpIntc2ReqInfo->getReqInfo(req.bit_val.ch) );
        req_flag = true;
      }
    }
    else if ( !(bool)req.bit_val.req && mpIntc1Func->IsIntReqInQueue( req.bit_val.ch ) ) {
      // the request exists in queue(=dropped the request before receiving acknowledge signal)
      dmsgsc( "  cancel the request  ch:0x" << req.bit_val.ch ); 
      if ( INTC1_CH_NUM > req.bit_val.ch || EI_ALL_CH_NUM <= req.bit_val.ch ) { // check of channel value
        sprintf( mErrMsg, "Invalid CH(%llu) value. (CH must be %d-%d)", req.bit_val.ch, INTC1_CH_NUM, EI_ALL_CH_NUM-1 );
        printErrorMessage( mpModName, "I2EiintReqProc", mErrMsg );
      }
      else {
        mpIntc1Func->CancelInt( req.bit_val.ch ); // remove the request from queue in INTC1 and drop the request
        TRegEIC* pEic = mpIntc2ReqInfo->getEicPtr( req.bit_val.ch );
        TRegEEIC* pEeic = mpIntc2ReqInfo->getEeicPtr( req.bit_val.ch );
        dmsgsc("  pEic->SetEIRF( 0 ) (dummy) ch:0x" << req.bit_val.ch);
        pEic->SetEIRF( 0 );
        pEeic->SetEIRF( 0 );
        set<unsigned int>::iterator it1 = mReEntryReqList.end();
        set<unsigned int>::iterator it2 = std::find(mReEntryReqList.begin(), mReEntryReqList.end(), req.bit_val.ch);
        if ( &it1 != &it2) {
          mReEntryReqList.erase( req.bit_val.ch );
        }
      }
    }
    // else remain same value || request is 0
    mPrevI2EiReq = req.val;
    dmsgsc( "    mPrevI2EiReq:0x" << mPrevI2EiReq );

    if ( req_flag ) {
      notifyExecEvent();
    }
  }
  // else reset is active
}

// INTC1 <- CPU
/// <summary>EIINT acknowledge process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::EiintAckProc( void )
{
  dmsgsc( mpModName << ".EiintAckProc()" );
  EiintAck  ack, prevAck;
  TRegEIC*  pEic;
  TRegEEIC*  pEeic;
  double    freq = (double)clk.read();

  if ( resetz.read() ) {
      ack.val     = t_intack_1l->read();
      prevAck.val = mPrevI2EiAck;
      if ( !(bool)prevAck.bit_val.ack && (bool)ack.bit_val.ack ) { // rising edge of acknowledge port
        if ( INTC1_CH_NUM > ack.bit_val.ch ) { // request from peripheral // detection type is effective
          pEic = mpIntc1Func->GetEICPtr( ack.bit_val.ch );
          sc_assert(pEic != NULL);
          pEeic = mpIntc1Func->GetEEICPtr( ack.bit_val.ch );
          sc_assert(pEeic != NULL);
          dmsgsc("  pEic->SetEIRF( 0 ) ch:0x" << ack.bit_val.ch);
          resendEiintForbid = true; // ACK -> not need call resend (process for lower priority in request queue)
          pEic->SetEIRF( 0 );
          pEeic->SetEIRF( 0 );
          dmsgsc( "  eiint_type:0x" << eiint_type[ack.bit_val.ch]->read() << " eiintl:0x" << eiintl[ack.bit_val.ch]->read() );
          if ( eiint_type[ack.bit_val.ch]->read() && eiintl[ack.bit_val.ch]->read() ) {
            // high level detection type && request = high
            meEiReq1ForLvDetect.notify( SC_ZERO_TIME );
            dmsgsc( "  re-request ch:0x" << ack.bit_val.ch );
          }
          // else edge detection type || no request
        }
        else { // request from INTC2
          pEic = mpIntc2ReqInfo->getEicPtr( ack.bit_val.ch );
          pEeic = mpIntc2ReqInfo->getEeicPtr( ack.bit_val.ch );
          dmsgsc("  pEic->SetEIRF( 0 ) ch:0x" << ack.bit_val.ch);
          pEic->SetEIRF( 0 );
          pEeic->SetEIRF( 0 );
          setEiintAckPort( ack.val, PRI_EIACK_PROC ); // send acknowledge signal to INTC2
        }
      }
      else if ( (bool)prevAck.bit_val.ack && !(bool)ack.bit_val.ack ) { // falling edge of acknowledge port
        if ( INTC1_CH_NUM <= prevAck.bit_val.ch ) { // request from INTC2
          setEiintAckPort( false, PRI_EIACK_PROC ); // drop acknowledge port to INTC2
        }
        // else request from peripheral
        set<unsigned int>::iterator it = mReEntryReqList.begin();
        while ( it != mReEntryReqList.end() ) {
            TRegEIBD* pEibd;
            if (INTC1_CH_NUM > *it){
                pEibd = mpIntc1Func->GetEIBDPtr( *it );
            } else{
                pEibd = mpIntc2ReqInfo->getEibdPtr( *it );
            }
            if ( INTC1_CH_NUM > *it ) { // 0-31 ch
              dmsgsc( "  ReqIntByEIC(0x" << *it << ")" );
              mpIntc1Func->ReqIntByEIC( *it );
            }
            else { // 32-2047 ch
              dmsgsc( "  ReqIntByIntc2(0x" << *it << ")" );
              mpIntc1Func->ReqIntByIntc2( *it, mpIntc2ReqInfo->getReqInfo(*it) );
            }
            mReEntryReqList.erase( it++ );
        }
        if ( 0 != mpIntc1Func->getQueueSize() ) { // continue to drive ExecProc() until the queue will be empty
          dmsgsc( "  QueueSize:0x" << mpIntc1Func->getQueueSize() );
          dmsgsc( "  clk.read():0x" << (int)clk.read() );
          
          if ( 0.0 == freq ) {
            sprintf( mErrMsg, "Clock frequency is 0." );
            printErrorMessage( mpModName, "EiintAckProc", mErrMsg );
          }
          else {
            meExec.notify( 1.0/freq, SC_SEC );
          }
        }
        // else queue is empty
      }
      // else remain the same value
      mPrevI2EiAck = ack.val;
    }
  // else reset is active
}


// peripheral -> INTC1
/// <summary>EIINT(from peripheral) request process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::ExEiintReqProc( void )
{
  dmsgsc( mpModName << ".ExEiintReqProc()" );

  if ( resetz.read() ) {
    for ( int i = 0; i < INTC1_CH_NUM; i++ ) {
      if ( ( eiintl[i]->read() &&
             ( (!mPrevExEiReq[i] && !eiint_type[i]->read()) || // rising edge & edge detection type
               eiint_type[i]->read() ) ) ) { // high level detection type
        dmsgsc( "  QueueSize:0x" << mpIntc1Func->getQueueSize() << " CH:0x" << i );
        mpIntc1Func->ReqIntByPeripheral( i );
        dmsgsc( "  QueueSize:0x" << mpIntc1Func->getQueueSize() << " CH:0x" << i );
        notifyExecEvent();
      }
      else if ( mPrevExEiReq[i] && !eiintl[i]->read() && // falling edge
                eiint_type[i]->read() ) { // high level detection type
        TRegEIC*  pEic  = mpIntc1Func->GetEICPtr( i );
        TRegEEIC*  pEeic  = mpIntc1Func->GetEEICPtr( i );
        dmsgsc("  pEic->SetEIRF( 0 ) ch:0x" << i);
        pEic->SetEIRF( 0 );
        pEeic->SetEIRF( 0 );
        set<unsigned int>::iterator it1 = mReEntryReqList.end();
        set<unsigned int>::iterator it2 = std::find(mReEntryReqList.begin(), mReEntryReqList.end(), i);
        if ( &it1 != &it2) {
          mReEntryReqList.erase( i );
        }
      }
      // else remain the same value, or falling edge & edge detection type
      mPrevExEiReq[i] = eiintl[i]->read();
    }
  }
  // else reset is active
}

// peripheral -> INTC1
/// <summary>FENMI request process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::ExFenmiReqProc( void )
{
  dmsgsc( mpModName << ".ExFenmiReqProc()" );

  if ( resetz.read() ) {
    if ( fenmi.read() && !mPrevExFnReq ) {// rising edge & edge detection type
      mpIntc1Func->ReqIntByFenmi();
      notifyExecEvent();
    }
    // else no request
    mPrevExFnReq = fenmi->read();
  }
  // else reset is active
}

// INTC1 <- CPU
/// <summary>FENMI acknowledge process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::ExFenmiAckProc( void )
{
  dmsgsc( mpModName << ".ExFenmiAckProc()" );
  double freq = (double)clk.read();

  if ( resetz.read() ) {
    if ( fenmiack_1l.read() ) { // rising edge of acknowledge port
      mpIntc1Func->ResFenmiFromCpu( mpIntc1Func ); // remove the request
    }
    else { // falling edge of acknowledge port
      if ( 0 != mpIntc1Func->getQueueSize() ) { // continue to drive ExecProc() until the queue will be empty
        dmsgsc( "  QueueSize:0x" << mpIntc1Func->getQueueSize() );
        dmsgsc( "  clk.read():0x" << (int)clk.read() );
        
        if ( 0.0 == freq ) {
          sprintf( mErrMsg, "Clock frequency is 0." );
          printErrorMessage( mpModName, "ExFenmiAckProc", mErrMsg );
        }
        else {
          meExec.notify( 1.0/freq, SC_SEC );
        }
      }
      // else no request in the queue
    }
  }
  // else reset is active
}
/// <summary>process to resend eiint</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::resendEiintReqPort( void )
{
  dmsgsc( mpModName << ".resendEiintReqPort()" );
  set<unsigned int>::iterator it = mReEntryReqList.begin();
  double freq = (double)clk.read();
  
  while ( it != mReEntryReqList.end() ) {
      TRegEIBD* pEibd;
      if (INTC1_CH_NUM > *it){
          pEibd = mpIntc1Func->GetEIBDPtr( *it );
      } else {
          pEibd = mpIntc2ReqInfo->getEibdPtr( *it );
      }
      if ( INTC1_CH_NUM > *it ) { // 0-31 ch
        dmsgsc( "  ReqIntByEIC(0x" << *it << ")" );
        mpIntc1Func->ReqIntByEIC( *it );
      }
      else { // 32-2047 ch
        dmsgsc( "  ReqIntByIntc2(0x" << *it << ")" );
        mpIntc1Func->ReqIntByIntc2( *it, mpIntc2ReqInfo->getReqInfo(*it) );
      }
      mReEntryReqList.erase( it++ );
  }
  if ( 0 != mpIntc1Func->getQueueSize() ) { // continue to drive ExecProc() until the queue will be empty
    dmsgsc( "  QueueSize:0x" << mpIntc1Func->getQueueSize() );
    dmsgsc( "  clk.read():0x" << (int)clk.read() );
    
    if ( 0.0 == freq ) {
      sprintf( mErrMsg, "Clock frequency is 0." );
      printErrorMessage( mpModName, "resendEiintReqPort", mErrMsg );
    }
    else {
      meExec.notify( 1.0/freq, SC_SEC );
    }
  }
  // else no request in the queue
}
// peripheral -> INTC1
/// <summary>FEINT request process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::ExFeintReqProc( void )
{
  dmsgsc( mpModName << ".ExFeintReqProc()" );

  if ( resetz.read() ) {
    for( int i = 0; i < INTC1_FEINT_CH_NUM; i++) {
      if ( ( feint[i]->read() &&
             ( (!mPrevExFiReq[i] && !feint_type[i]->read()) || // rising edge & edge detection type
               feint_type[i]->read() ) ) ) { // high level detection type
        mpIntc1Func->ReqIntByFeint(i);
        notifyExecEvent();
      }
      else if ( mPrevExFiReq[i] && !feint[i]->read() && // falling edge
                feint_type[i]->read() ) { // high level detection type

        TRegFIC* pFic = mpIntc1Func->GetFICPtr (i);
        dmsgsc("  pFic->SetFIRF( 0 ) ch:0x" << i);
        pFic->SetFIRF(0);
        mpIntc1Func->ResFeintFromCpu(i, mpIntc1Func ); // remove the request

      }
      // else remain the same value, or falling edge & edge detection type
      mPrevExFiReq[i] = feint[i]->read();
    }
  }
  // else reset is active
}
/// <summary>FEINT resend process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::resendFeintReqPort( void )
{
  dmsgsc( mpModName << ".resendFeintReqPort()" );
  double freq = (double)clk.read();
  if ( 0 != mpIntc1Func->getQueueSize() ) { // continue to drive ExecProc() until the queue will be empty
    dmsgsc( "  QueueSize:0x" << mpIntc1Func->getQueueSize() );
    dmsgsc( "  clk.read():0x" << (int)clk.read() );
    
    if ( 0.0 == freq ) {
      sprintf( mErrMsg, "Clock frequency is 0." );
      printErrorMessage( mpModName, "resendFeintReqPort", mErrMsg );
    }
    else {
      meExec.notify( 1.0/freq, SC_SEC );
    }
  }
}
// INTC1 <- CPU
/// <summary>FEINT acknowledge process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::ExFeintAckProc( void )
{
  dmsgsc( mpModName << ".ExFeintAckProc()" );
  double freq = (double)clk.read();
  FeintAck  ack, prevAck; 
  if ( resetz.read() ) {
      ack.val     = t_feintack_1l->read(); 
      if ( !mPrevExFiAck && t_feintack_1l.read() ) { // rising edge of acknowledge port
        // even if the detection type is high level, FEINT request port must be dropped
        // because the CPU needs the rising edge.
        if ( INTC1_FEINT_CH_NUM > ack.bit_val.ch ) {
          unsigned int i = ack.bit_val.ch;
          resendFeintForbid = true;
          mpIntc1Func->ResFeintFromCpu(i, mpIntc1Func ); // remove the request
          if ( feint_type[i]->read() ) { // high level detection type
            if ( 0.0 == freq ) {
              sprintf( mErrMsg, "Clock frequency is 0." );
              printErrorMessage( mpModName, "ExFeintAckProc", mErrMsg );
            }
            else if ( feint[i]->read() ) { // request = high
              meFiReqForLvDetect.notify( 1.0/freq, SC_SEC );
            }
            // else no request
          }
        }
      }
      else if ( mPrevExFiAck && !(t_feintack_1l.read()) ) { // falling edge of acknowledge port
        if ( 0 != mpIntc1Func->getQueueSize() ) { // continue to drive ExecProc() until the queue will be empty
          dmsgsc( "  QueueSize:0x" << mpIntc1Func->getQueueSize() );
          dmsgsc( "  clk.read():0x" << (int)clk.read() );

          if ( 0.0 == freq ) {
            sprintf( mErrMsg, "Clock frequency is 0." );
            printErrorMessage( mpModName, "ExFeintAckProc", mErrMsg );
          }
          else {
            meExec.notify( 1.0/freq, SC_SEC );
          }
        }
        // else no request in the queue
      }
      // else remain the same value
      mPrevExFiAck = t_feintack_1l.read();
    }
  // else reset is active
}

// INTC1 -> CPU
/// <summary>process to write the t_eiint_1l port</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::SetEiintReqPortProc( void )
{
  dmsgsc( mpModName << ".SetEiintReqPortProc()" );

  if ( resetz.read() ) {
    dmsgsc("  mEiCancelInfo.size():0x" << mEiCancelInfo.size());
    if ( 0 == mEiCancelInfo.size() ) {
      t_eiint_1l.write( mEiReqInfo.data );
      dmsgsc ("  t_eiint_1l.write: 0x" << mEiReqInfo.data);
    }
    else {
      set<uint64>::iterator it = mEiCancelInfo.begin();
      t_eiint_1l.write( *it );
      dmsgsc ("  t_eiint_1l.write: 0x" << *it);
      mEiCancelInfo.erase( it );
      meSetEiReq.notify( SC_ZERO_TIME );
    }
  }
  else { // reset is active
    t_eiint_1l.write( 0 );
    dmsgsc ("  t_eiint_1l.write: 0x0");
  }
  mEiReqInfo.drvProcPri = PRI_DEFAULT;
}

// INTC2 <- INTC1
/// <summary>process to write the i1_t_intack port</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::SetEiintAckPortProc( void )
{
  dmsgsc( mpModName << ".SetEiintAckPortProc()" );

    if ( resetz.read() ) {
      i1_t_intack->write( mEiAckInfo.data );
    }
    else { // reset is active
      i1_t_intack->write( 0 );
    }
    mEiAckInfo.drvProcPri = PRI_DEFAULT;
}

// INTC1 -> CPU
/// <summary>process to write the fenmirq_1l port</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::SetFenmiReqPortProc( void )
{
  dmsgsc( mpModName << ".SetFenmiReqPortProc()" );

  if ( resetz.read() ) {
    fenmirq_1l->write( mFnReqInfo.data );
    dmsgsc("  fenmirq_1l.write: 0x"<< mFnReqInfo.data);
  }
  else { // reset is active
    fenmirq_1l->write( false );
    dmsgsc("  fenmirq_1l.write: 0x0");
  }
  mFnReqInfo.drvProcPri = PRI_DEFAULT;
}

// INTC1 -> CPU
/// <summary>process to write the feintrq_1l port</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::SetFeintReqPortProc( void )
{
  dmsgsc( mpModName << ".SetFeintReqPortProc()" );

  if ( resetz.read() ) {
    dmsgsc("  mFeintCancelInfo size:" << mFeintCancelInfo.size());
    if ( 0 == mFeintCancelInfo.size() ) {
      t_feintrq_1l.write( mFiReqInfo.data );
      dmsgsc("  t_feintrq_1l.write: 0x"<< mFiReqInfo.data);
    }
    else {
      set<uint64>::iterator it = mFeintCancelInfo.begin();
      t_feintrq_1l.write( (unsigned int)(*it) );
      dmsgsc("  t_feintrq_1l.write: 0x"<< *it);
      mFeintCancelInfo.erase( it );
      meSetFiReq.notify( SC_ZERO_TIME );
    }
  }
  else { // reset is active
    t_feintrq_1l.write( false );
    dmsgsc("  t_feintrq_1l.write: 0x0");
  }
  mFiReqInfo.drvProcPri = PRI_DEFAULT;
}

/// <summary>process to execute INTC1 function</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::ExecProc( void )
{
  dmsgsc( mpModName << ".ExecProc()" );

  if ( resetz.read() ) {
    mpIntc1Func->CyclicHandler();
  }
  // else reset is active
}

/// <summary>monitor to get the info of CPU: GM, GPID</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::MonitorCPUInfoMethod( void ) // <-- added by chanle 20180129 for G4MH2.0 to support HV
{
  CpuGmInfo input_cpu_info (cpu_gm_info.read());
  mCPUGM = (bool)input_cpu_info.bit_val.cpu_gm;
  mCPUGPID = input_cpu_info.bit_val.cpu_gpid;
  dmsgsc( mpModName << ".MonitorCPUInfoMethod(), cpu_gm: " << mCPUGM << ", cpu_gpid: " << mCPUGPID);
  mpIntc1Func->SetCpuGuestMode(mCPUGM);
  mpIntc1Func->SetCpuGpid(mCPUGPID);
  notifyExecEvent(); // raise interrupt if EIRF still 1 when CPU changes mode.
}

/// <summary>set force cancel flag for feint </summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVINTC1V02::setFeintForceCanceFlag( bool isSet )
{
  dmsgsc( mpModName << ".setFeintForceCanceFlag()" << isSet );
  mFeintCancelForce = isSet;
}

/// <summary>get info GM of CPU</summary>
/// <param name="">none</param>
/// <returns>bool: value of GM of CPU</returns>
bool
NSMVINTC1V02::getCPUGM( void )
{
  return mCPUGM;
}

/// <summary>get info GPID of CPU</summary>
/// <param name="">none</param>
/// <returns>unsigned int: value of GPID of CPU</returns>
unsigned int
NSMVINTC1V02::getCPUGPID( void )
{
  return mCPUGPID;
}
