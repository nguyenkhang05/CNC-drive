/*************************************************************************
 *
 *  G4_CaIss.cpp
 *
 * Copyright(c) 2016, 2018-2020 Renesas Electronics Corporation
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
#if defined(_MSC_VER) // for cforestg4x (cforestg4x uses std::min and max)
#undef min
#undef max
#endif

#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno

#include "G4_CaIss.h" 
#include "G4_IsVciIf.h"
#include "G4_IsLapbIf.h"
#include "G4_IsVpiIf.h"
#include "G4_TsIf.h"
#include "forest_utility_scheap.h"
#include "sideband.h"
#include "outstand.h"
#include "NSMVG4CPUV01.h"
#include "g4cpu_def.h"

#include "CmErrMsg.h" // for WinMsg

using namespace CmErrMsg; // for WinMsg

///<summary>constructor</summary>
///<param name="module_name">name of sc_module</param>
///<param name="g4cpuptr">the pointer of NSMVG4CPUV01 instance</param>
///<returns>none</returns>
G4_CaIss::G4_CaIss(sc_module_name module_name, NSMVG4CPUV01* g4cpuptr)
  : sc_module(module_name),
    BusMasterBase<BUS_WIDTH_VCI, (PE_MAX_NUM*4)>(),
    BusSlaveBase<BUS_WIDTH_FLSH, 1>(),
    BusSlaveBase<BUS_WIDTH_VCI, 2>(),
    BusTimeBase(),
    From_clk( "From_clk" ),
    Cram_clk( "Cram_clk" ),
    tsf((TlmTargetSocket<BUS_WIDTH_FLSH> *)0 ),
    tss((TlmTargetSocket<BUS_WIDTH_VCI> *)0 ),
    tscr((TlmTargetSocket<BUS_WIDTH_CRAM> *)0 ),
    sys_reset( "sys_reset" ),
    mpG4CPU(g4cpuptr),
    mpForestUtil( (ForestUtilScheap *)0 ),
    mpIsVciIf( (G4_IsVciIf *)0 ),
    mpIsVpiIf( (G4_IsVpiIf *)0 ),
    mpIsVpiIf_gc( (G4_IsVpiIf *)0 ),
    mpTsIf( (G4_TsIf *)0 ),
    mClkPeriod(+HUGE_VAL),
    mFcAddrS(FROM_INIT_BASE),
    mFcAddrE(FROM_MAX_SIZE-1),
    mCramAddrS(CRAM_INIT_BASE),
    mCramAddrE(CRAM_INIT_BASE + CRAM_MAX_SIZE - 1),
    mLramAddrS(LRAM_INIT_BASE - (LRAM_INIT_SIZE * 8)),
    mLramAddrE(LRAM_INIT_BASE + LRAM_INIT_SIZE - 1),
    mExMReadRomLatency(0),
    mExMWriteRomLatency(0),
    mExMReadCramLatency(0),
    mExMWriteCramLatency(0),
    mExMReadLramLatency(0),
    mExMWriteLramLatency(0),
    mExMReadLapbLatency(0),
    mExMWriteLapbLatency(0)
{
  mpPeType      = new unsigned int[PE_MAX_NUM];

  char port_name[64];
  for(int i=0; i<PE_MAX_NUM; i++){
    // for PE's clock
    sprintf(port_name, "PE%d_sys_clk", i+1);
    PE_sys_clk[i] = new sc_in<sc_dt::uint64>(port_name);

    // set default pe type
    mpPeType[i] = CORE_G4MH;
  }
  
  /***********************/
  /* for Master function */
  /***********************/
  // setting for 64 bit bus x ( (PE_MAX_NUM) + (PE_MAX_NUM) + (PE_MAX_NUM) )
  //                            for G4_IsVciIf    for G4_IsVpiIf    for G4_IsLapbIf
  setMasterResetPort64( &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset,
			&sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset,
			&sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset,
			&sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset, &sys_reset);
  setMasterFreqPort64( PE_sys_clk[0], PE_sys_clk[1], PE_sys_clk[2], PE_sys_clk[3], PE_sys_clk[4], PE_sys_clk[5], PE_sys_clk[6], PE_sys_clk[7], 
		       PE_sys_clk[0], PE_sys_clk[1], PE_sys_clk[2], PE_sys_clk[3], PE_sys_clk[4], PE_sys_clk[5], PE_sys_clk[6], PE_sys_clk[7],
		       PE_sys_clk[0], PE_sys_clk[1], PE_sys_clk[2], PE_sys_clk[3], PE_sys_clk[4], PE_sys_clk[5], PE_sys_clk[6], PE_sys_clk[7],
		       PE_sys_clk[0], PE_sys_clk[1], PE_sys_clk[2], PE_sys_clk[3], PE_sys_clk[4], PE_sys_clk[5], PE_sys_clk[6], PE_sys_clk[7]);
  setInitiatorSocket64( "PE0_ish", "PE1_ish", "PE2_ish", "PE3_ish", "PE4_ish", "PE5_ish", "PE6_ish", "PE7_ish",
			"PE0_isgp", "PE1_isgp", "PE2_isgp", "PE3_isgp", "PE4_isgp", "PE5_isgp", "PE6_isgp", "PE7_isgp",
			"PE0_isgc", "PE1_isgc", "PE2_isgc", "PE3_isgc", "PE4_isgc", "PE5_isgc", "PE6_isgc", "PE7_isgc",
			"PE0_isi", "PE1_isi", "PE2_isi", "PE3_isi", "PE4_isi", "PE5_isi", "PE6_isi", "PE7_isi");

  for (int i=0; i<PE_MAX_NUM; i++){
    PE_ish[i]  = iSocket64[i];
    PE_isgp[i] = iSocket64[PE_MAX_NUM + i];
    PE_isgc[i] = iSocket64[(PE_MAX_NUM * 2) + i];
    PE_isi[i]  = iSocket64[(PE_MAX_NUM * 3) + i];
  }

  // instantiation for Core function
  mpIsVciIf      = new G4_IsVciIf( this, mBusMasterIf64); 
  mpIsVpiIf      = new G4_IsVpiIf( this, mBusMasterIf64 + PE_MAX_NUM);
  mpIsVpiIf_gc   = new G4_IsVpiIf( this, mBusMasterIf64 + (PE_MAX_NUM*2));
  for (int i=0; i<PE_MAX_NUM; i++){
    mpIsLapbIf[i] = new G4_IsLapbIf( this, (mBusMasterIf64 + (PE_MAX_NUM * 3) + i));
  }

  for (int i=0; i<PE_MAX_NUM; i++){
    mBusMasterIf64[i]->setFuncModulePtr( (BusMasterFuncIf *) mpIsVciIf);
    mBusMasterIf64[i]->setBusProtocol( BUS_VCI );
    mBusMasterIf64[i]->setTransNmbLmt( 0xffffffff );

    mBusMasterIf64[PE_MAX_NUM + i]->setFuncModulePtr( (BusMasterFuncIf *) mpIsVpiIf);
    mBusMasterIf64[PE_MAX_NUM + i]->setBusProtocol( BUS_VCI );
    mBusMasterIf64[PE_MAX_NUM + i]->setTransNmbLmt( 0xffffffff );

    mBusMasterIf64[(PE_MAX_NUM * 2) + i]->setFuncModulePtr( (BusMasterFuncIf *) mpIsVpiIf_gc);
    mBusMasterIf64[(PE_MAX_NUM * 2) + i]->setBusProtocol( BUS_VCI );
    mBusMasterIf64[(PE_MAX_NUM * 2) + i]->setTransNmbLmt( 0xffffffff );

    mBusMasterIf64[(PE_MAX_NUM * 3) + i]->setFuncModulePtr( (BusMasterFuncIf *) mpIsLapbIf[i]);
    mBusMasterIf64[(PE_MAX_NUM * 3) + i]->setBusProtocol( BUS_VCI );
    mBusMasterIf64[(PE_MAX_NUM * 3) + i]->setTransNmbLmt( 0xffffffff );
  }

  /***********************/
  /* for Slave function */
  /***********************/
  // setting for 128 bit bus x 1
  setSlaveResetPort128( &sys_reset );
  setSlaveFreqPort128( &From_clk );
  setTargetSocket128( "tsf" );
  tsf = tSocket128[0]; // Aliase for binding

  // setting for 64 bit bus x 2
  setSlaveResetPort64( &sys_reset, &sys_reset);
  setSlaveFreqPort64( PE_sys_clk[0], &Cram_clk );
  setTargetSocket64( "tss", "tscr" );
  tss = tSocket64[0];
  tscr = tSocket64[1];
  
  mpTsIf = new G4_TsIf( this );

  mBusSlaveIf128[0]->setFuncModulePtr( mpTsIf );
  mBusSlaveIf128[0]->setBusProtocol( BUS_AXI );

  mBusSlaveIf64[0]->setFuncModulePtr( mpTsIf );
  mBusSlaveIf64[0]->setBusProtocol( BUS_VCI );

  mBusSlaveIf64[1]->setFuncModulePtr( mpTsIf );
  mBusSlaveIf64[1]->setBusProtocol( BUS_AXI );

  SC_METHOD(ChangeFreqPE0);
  sensitive << *(PE_sys_clk)[0];
  dont_initialize();

  SC_METHOD(ChangeFreqPE1);
  sensitive << *(PE_sys_clk)[1];
  dont_initialize();

  SC_METHOD(ChangeFreqPE2);
  sensitive << *(PE_sys_clk)[2];
  dont_initialize();

  SC_METHOD(ChangeFreqPE3);
  sensitive << *(PE_sys_clk)[3];
  dont_initialize();

  SC_METHOD(ChangeFreqPE4);
  sensitive << *(PE_sys_clk)[4];
  dont_initialize();

  SC_METHOD(ChangeFreqPE5);
  sensitive << *(PE_sys_clk)[5];
  dont_initialize();

  SC_METHOD(ChangeFreqPE6);
  sensitive << *(PE_sys_clk)[6];
  dont_initialize();

  SC_METHOD(ChangeFreqPE7);
  sensitive << *(PE_sys_clk)[7];
  dont_initialize();

  SC_METHOD(ChangeFreqFrom);
  sensitive << From_clk;
  dont_initialize();

  SC_METHOD(ChangeFreqCram);
  sensitive << Cram_clk;
  dont_initialize();

  SC_METHOD(reset);
  sensitive << sys_reset;
  dont_initialize();

}


///<summary>destructor</summary>
G4_CaIss::~G4_CaIss(void)
{
  delete mpIsVciIf; 
  delete mpIsVpiIf;
  delete mpIsVpiIf_gc;
  for (int i=0; i<PE_MAX_NUM; i++){
    delete mpIsLapbIf[i];
  }
  delete mpTsIf;
}


///<summary>set the pointer of ForestUtilScheap instance</summary>
///<param name="ptr">the pointer of ForestUtilScheap instance</param>
///<returns>none</returns>
void G4_CaIss::SetForestUtilScheapPtr(ForestUtilScheap* ptr)
{
  mpForestUtil = ptr;
  AddBusIf2Fsbus();
}


///<summary>add the each memory area to fsbus</summary>
///<returns>none</returns>
void G4_CaIss::AddBusIf2Fsbus(void)
{
  mpForestUtil->SetFsbusPointer();
  PhysAddr s_addr = LRSRC_INIT_BASE;
  PhysAddr e_addr = LRSRC_INIT_BASE + LRSRC_INIT_SIZE - 1 ;
 
  mpForestUtil->AddTargetVC(0x10000000ULL, 0xEBFFFFFFULL, mpIsVciIf, 0, GUARD_NONE);
  mpForestUtil->AddTargetVC(0xFF000000ULL, 0xFFFB7FFFULL, mpIsVpiIf, 0, GUARD_NONE);
  mpForestUtil->AddTargetVC(0xFFFE4000ULL, 0xFFFFFFFFULL, mpIsVpiIf, 0, GUARD_NONE);
  mpForestUtil->AddTargetVC(0xFFFB8000ULL, 0xFFFBFFFFULL, mpIsVpiIf_gc, 0, GUARD_NONE);
  
  for (unsigned int i=0; i<PE_MAX_NUM; i++){
    s_addr = s_addr + LRSRC_INIT_SIZE;
    e_addr = e_addr + LRSRC_INIT_SIZE;
    if(i < mpG4CPU->mPeNum){
      mpForestUtil->AddTargetVC(s_addr, e_addr, mpIsLapbIf[i], i, GUARD_PEG);
    }else{
      mpForestUtil->AddTargetVC(s_addr, e_addr, mpIsVpiIf, 0, GUARD_NONE);
    }
  }
}


///<summary>caluclate the cycle period from PE0's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqPE0(void)
{
  mPeriodPE[0] = periodCal((sc_dt::uint64)(*PE_sys_clk[0]).read());
#ifdef BUSIF_TEST_LOG
  printf("%s: PE0's clock period=%f\n", this->name(), mPeriodPE[0]);
  fflush(stdout);
#endif
}


///<summary>caluclate the cycle period from PE1's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqPE1(void)
{
  mPeriodPE[1] = periodCal((sc_dt::uint64)(*PE_sys_clk[1]).read());
#ifdef BUSIF_TEST_LOG
  printf("%s: PE1's clock period=%f\n", this->name(), mPeriodPE[1]);
  fflush(stdout);
#endif
}


///<summary>caluclate the cycle period from PE2's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqPE2(void)
{
  mPeriodPE[2] = periodCal((sc_dt::uint64)(*PE_sys_clk[2]).read());
#ifdef BUSIF_TEST_LOG
  printf("%s: PE2's clock period=%f\n", this->name(), mPeriodPE[2]);
  fflush(stdout);
#endif
}


///<summary>caluclate the cycle period from PE3's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqPE3(void)
{
  mPeriodPE[3] = periodCal((sc_dt::uint64)(*PE_sys_clk[3]).read());
#ifdef BUSIF_TEST_LOG
  printf("%s: PE3's clock period=%f\n", this->name(), mPeriodPE[3]);
  fflush(stdout);
#endif
}


///<summary>caluclate the cycle period from PE4's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqPE4(void)
{
  mPeriodPE[4] = periodCal((sc_dt::uint64)(*PE_sys_clk[4]).read());
#ifdef BUSIF_TEST_LOG
  printf("%s: PE4's clock period=%f\n", this->name(), mPeriodPE[4]);
  fflush(stdout);
#endif
}


///<summary>caluclate the cycle period from PE5's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqPE5(void)
{
  mPeriodPE[5] = periodCal((sc_dt::uint64)(*PE_sys_clk[5]).read());
#ifdef BUSIF_TEST_LOG
  printf("%s: PE5's clock period=%f\n", this->name(), mPeriodPE[5]);
  fflush(stdout);
#endif
}


///<summary>caluclate the cycle period from PE6's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqPE6(void)
{
  mPeriodPE[6] = periodCal((sc_dt::uint64)(*PE_sys_clk[6]).read());
#ifdef BUSIF_TEST_LOG
  printf("%s: PE6's clock period=%f\n", this->name(), mPeriodPE[6]);
  fflush(stdout);
#endif
}


///<summary>caluclate the cycle period from PE7's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqPE7(void)
{
  mPeriodPE[7] = periodCal((sc_dt::uint64)(*PE_sys_clk[7]).read());
#ifdef BUSIF_TEST_LOG
  printf("%s: PE7's clock period=%f\n", this->name(), mPeriodPE[7]);
  fflush(stdout);
#endif
}


///<summary>caluclate the cycle period from Flash rom's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqFrom(void)
{
  mPeriodFrom = periodCal((sc_dt::uint64)From_clk.read());
#ifdef BUSIF_TEST_LOG
  printf("%s: Flash IF's clock period=%f\n", this->name(), mPeriodFrom);
  fflush(stdout);
#endif
}


///<summary>caluclate the cycle period from cluster ram's clock frequency</summary>
///<returns>none</returns>
void G4_CaIss::ChangeFreqCram(void)
{
  mPeriodCram = periodCal((sc_dt::uint64)Cram_clk.read());
#ifdef BUSIF_TEST_LOG
  printf("%s: Cluster ram IF's clock period=%f\n", this->name(), mPeriodCram);
  fflush(stdout);
#endif
}


///<summary>reset operation</summary>
///<returns>none</returns>
void G4_CaIss::reset( void )
{
  if(sys_reset.read()){
    mpIsVciIf->HardReset(); 
    mpIsVpiIf->HardReset();
    mpIsVpiIf_gc->HardReset();
    for(int i=0; i<PE_MAX_NUM; i++){
      mpIsLapbIf[i]->HardReset();
    }
  }
}


///<summary>set the base clock period (ns)</summary>
///<param name="period">base clock period (ns)</param>
///<returns>none</returns>
void G4_CaIss::SetClkPeriod( double period )
{
  mClkPeriod = period;
#ifdef EX_MASTER_TEST_LOG
  printf("G4_CaIss::SetClkPeriod: clock=%lf\n", mClkPeriod);
  fflush(stdout);
#endif
}


///<summary>notify CForestG3M of the timing to send the transaction</summary>
///<param name="sideband">sideband</param>
///<param name="addr">address</param>
///<param name="time">transport time</param>
///<returns>none</returns>
void G4_CaIss::NfyTransSend( SideBand sideband, PhysAddr addr, BusTime_t time )
{
#ifdef BUSIF_TEST_LOG
  printf("G4_CaIss::NfyTransSend: clock=%f, time=%s\n", mClkPeriod, time.to_string().c_str());
  fflush(stdout);
#endif // #if BUSIF_TEST_LOG
}


///<summary>notify CForestG3M of the timing to complete the transaction</summary>
///<param name="sideband">sideband</param>
///<param name="addr">address</param>
///<param name="time">transport time</param>
///<param name="data">read value</param>
///<param name="acc_ev">access event(write/read/fetch)</param>
///<returns>none</returns>
void G4_CaIss::NfyTransComp( SideBand sideband, PhysAddr addr, BusTime_t time, unsigned long long data, unsigned int acc_ev )
{
#ifdef BUSIF_TEST_LOG
  //printf("G4_CaIss::NfyTransComp: clock=%f, time=%s\n", mClkPeriod, time.to_string().c_str());
  printf("G4_CaIss::NfyTransComp: clock=%f, time=%f\n", mClkPeriod, time.to_double());
  fflush(stdout);
#endif
  double dbl_latency = time.to_double() / mClkPeriod;
  unsigned int latency = (unsigned int)(dbl_latency);
  if((dbl_latency - latency) >= 0.5){
#ifdef BUSIF_TEST_LOG
    printf("G4_CaIss::NfyTransComp : the vlaue after the decimal point of [trans time] div [clock period] = %f >= 0.5\n", dbl_latency - latency);
    fflush(stdout);
#endif
    latency++;
  }else{
#ifdef BUSIF_TEST_LOG
    printf("G4_CaIss::NfyTransComp : the vlaue after the decimal point of [trans time] div [clock period] = %f < 0.5\n", dbl_latency - latency);
    fflush(stdout);
#endif
  }
  if((G4_IsIfBase::ACC_MODE_T)acc_ev == G4_IsIfBase::ACC_WRITE){
    mpForestUtil->NotifyWriteLatency(sideband, addr, latency);
#ifdef BUSIF_TEST_LOG
    printf("Call NotifyWriteLatency:sideband(peid(0x%x),spid(0x%x),um(%d),bitpos(0x%0x),bitop(0x%x)), ",
	   sideband.GetBMID(), 
	   sideband.GetSPID(), 
	   sideband.GetUM(), 
	   sideband.GetBitPos(), 
	   sideband.GetBitOp());
    printf("addr=0x%08x, latency=%d\n", (unsigned int)addr, latency);
    fflush(stdout);
#endif
  }else if((G4_IsIfBase::ACC_MODE_T)acc_ev == G4_IsIfBase::ACC_READ){
    mpForestUtil->NotifyReadLatency(sideband, addr, latency, data);
#ifdef BUSIF_TEST_LOG
    printf("Call NotifyReadLatency:sideband(peid(0x%x),spid(0x%x),um(%d),bitpos(0x%0x),bitop(0x%x)), ",
	   sideband.GetBMID(), 
	   sideband.GetSPID(), 
	   sideband.GetUM(), 
	   sideband.GetBitPos(), 
	   sideband.GetBitOp());
    printf("addr=0x%08x, latency=%d\n", (unsigned int)addr, latency);
    fflush(stdout);
#endif
  }else{
    mpForestUtil->NotifyFetchLatency(sideband, addr, latency);
#ifdef BUSIF_TEST_LOG
    printf("Call NotifyFetchLatency:sideband(peid(0x%x),spid(0x%x),um(%d),bitpos(0x%0x),bitop(0x%x)), ",
	   sideband.GetBMID(), 
	   sideband.GetSPID(), 
	   sideband.GetUM(), 
	   sideband.GetBitPos(), 
	   sideband.GetBitOp());
    printf("addr=0x%08x, latency=%d\n", (unsigned int)addr, latency);
    fflush(stdout);
#endif
  }
}


///<summary>write data to CForestG3M's memory</summary>
///<param name="trans">target transaction</param>
///<param name="debug">debug access flag</param>
///<param name="t">transport time</param>
///<returns>none</returns>
void G4_CaIss::MemWriteToCForest( TlmBasicPayload& trans, bool debug, sc_time* t )
{
  SideBand       sb        = GetSideBandInfo(trans);
  PhysAddr       addr      = (PhysAddr)trans.get_address();
  unsigned int   bytes     = trans.get_data_length();
  unsigned char* data_ptr  = trans.get_data_ptr();
  bool           isDma     = false;
  TlmVciExtension::VciRequest_t vci_req_t = TlmVciExtension::CMD_WRITE;

  TlmG3mExtension *g3mExt = (TlmG3mExtension *)0;
  trans.get_extension( g3mExt );
  if(g3mExt) isDma = g3mExt->isDmaAccess();
  
  TlmVciExtension *vciExt = (TlmVciExtension *)0;
  trans.get_extension( vciExt );
  if(vciExt) vci_req_t = vciExt->getRequestType();
  
  if(bytes <= CFOREST_MEM_ACC_MAX){
    MemWriteToCForestBody(sb, addr, data_ptr, bytes, isDma, debug, vci_req_t, t);
  }else{
    int loop_num  = bytes / CFOREST_MEM_ACC_MAX;
    int last_size = bytes % CFOREST_MEM_ACC_MAX;
    for (int i=0; i<loop_num; i++){
      MemWriteToCForestBody(sb,
			    (addr+(i*CFOREST_MEM_ACC_MAX)), 
			    (data_ptr+(i*CFOREST_MEM_ACC_MAX)), 
			    CFOREST_MEM_ACC_MAX,
			    isDma,
			    debug,
			    vci_req_t,
			    t);
    }
    if(last_size > 0){
      MemWriteToCForestBody(sb,
			    (addr+(loop_num*CFOREST_MEM_ACC_MAX)), 
			    (data_ptr+(loop_num*CFOREST_MEM_ACC_MAX)), 
			    last_size,
			    isDma,
			    debug,
			    vci_req_t,
			    t);
    }
  }
}


///<summary>read data from CForestG3M's memory</summary>
///<param name="trans">target transaction</param>
///<param name="debug">debug access flag</param>
///<param name="t">transport time</param>
///<returns>none</returns>
void G4_CaIss::MemReadToCForest( TlmBasicPayload& trans, bool debug, sc_time* t )
{
  SideBand       sb       = GetSideBandInfo(trans);
  PhysAddr       addr     = (PhysAddr)trans.get_address();
  unsigned int   bytes    = trans.get_data_length();
  unsigned char* data_ptr = trans.get_data_ptr();
  bool           isDma    = false;
  TlmVciExtension::VciRequest_t vci_req_t = TlmVciExtension::CMD_READ;

  TlmG3mExtension *g3mExt = (TlmG3mExtension *)0;
  trans.get_extension( g3mExt );
  if(g3mExt) isDma = g3mExt->isDmaAccess();
  
  TlmVciExtension *vciExt = (TlmVciExtension *)0;
  trans.get_extension( vciExt );
  if(vciExt) vci_req_t = vciExt->getRequestType();
  
  if(bytes <= CFOREST_MEM_ACC_MAX){
    MemReadToCForestBody(sb, addr, data_ptr, bytes, isDma, debug, vci_req_t, t);
  }else{
    int loop_num  = bytes / CFOREST_MEM_ACC_MAX;
    int last_size = bytes % CFOREST_MEM_ACC_MAX;
    for (int i=0; i<loop_num; i++){
      MemReadToCForestBody(sb,
			   (addr+(i*CFOREST_MEM_ACC_MAX)), 
			   (data_ptr+(i*CFOREST_MEM_ACC_MAX)), 
			   CFOREST_MEM_ACC_MAX,
			   isDma,
			   debug,
			   vci_req_t,
			   t);
    }
    if(last_size > 0){
      MemReadToCForestBody(sb,
			   (addr+(loop_num*CFOREST_MEM_ACC_MAX)), 
			   (data_ptr+(loop_num*CFOREST_MEM_ACC_MAX)), 
			   last_size,
			   isDma,
			   debug,
			   vci_req_t,
			   t);
    }
  }
}


///<summary>caluclate the cycle period from input value(clock frequency)</summary>
///<param name="new_freq">new clock frecuency</param>
///<returns>cycle period</returns>
double G4_CaIss::periodCal( sc_dt::uint64 new_freq )
{
  double period = +HUGE_VAL;
  if (new_freq == 0){
    return (+HUGE_VAL);
  }
  switch(glb_resolution_unit){
  case SC_SEC:
    period = (double)((sc_dt::uint64)(1.0)/new_freq);
    break;
  case SC_MS:
    period = (double)((sc_dt::uint64)(1.0e+3)/new_freq);
    break;
  case SC_US:
    period = (double)((sc_dt::uint64)(1.0e+6)/new_freq);
    break;
  case SC_NS:
    period = (double)((sc_dt::uint64)(1.0e+9)/new_freq);
    break;
  case SC_PS:
    period = (double)((sc_dt::uint64)(1.0e+12)/new_freq);
    break;
  case SC_FS:
    period = (double)((sc_dt::uint64)(1.0e+15)/new_freq);
    break;
  }
  return(period);
}


///<summary>Set pe type</summary>
///<param name="pe_id">PE ID</param>
///<param name="pe_type">PE Type</param>
///<returns>none</returns>
void G4_CaIss::SetPeType( unsigned int pe_id, unsigned int pe_type)
{
  mpPeType[pe_id] = pe_type;
}


///<summary>Get pe type</summary>
///<param name="pe_id">PE ID</param>
///<returns>Pe Type</returns>
unsigned int G4_CaIss::GetPeType( unsigned int pe_id)
{
  return mpPeType[pe_id];
}


///<summary>Get sideband information from payload</summary>
///<param name="trans">target transaction</param>
///<returns>sideband</returns>
SideBand G4_CaIss::GetSideBandInfo( TlmBasicPayload& trans )
{
  SideBand         sb;
  TlmG3mExtension* pG3mExtension;
  
  trans.get_extension( pG3mExtension );
  //sb.SetDMA(pG3mExtension->isDmaAccess());
  sb.SetBMID(pG3mExtension->getPeId());
  sb.SetSPID(pG3mExtension->getSpId());
  sb.SetUM(pG3mExtension->isUserMode());
  sb.SetSPCLD(0);
  return(sb);
}



///<summary>Get SEGSIDE value from SideBand val (not use for G4MH model)</summary>
///<param name="sideband">CForestG3M's SideBand val</param>
///<returns>SEGSIDE value </returns>
unsigned int G4_CaIss::GetSegSideVal( SideBand sb )
{
  unsigned int val  = 0;
#if 0 
  unsigned int peid = sb.GetPEID();
  unsigned int val  = ((sb.GetVCID() & 0x7) << 7) | ((sb.GetUM() & 0x1) << 1)  | (sb.GetVM() & 0x1);

  if(mpPeType[peid-1] == CORE_CCG3M30){
    // 15:13 12:10 9:7   6:2   1   0
    // PEID  TCID  VCID  SPID  UM  VM
    val |= ((sb.GetPEID() & 0x7) << 13) | ((sb.GetTCID() & 0x7) << 10) | ((sb.GetSPID() & 0x1f) << 2);   
  }else{
    // 15:10 9:7   6:4   6:2   1   0
    // TCID  VCID  PEID  SPID  UM  VM
    val |= ((sb.GetTCID() & 0x3f) << 10) | ((sb.GetPEID() & 0x7) << 4) | ((sb.GetSPID() & 0x3) << 2);
  }
#endif
  return(val);
}


///<summary>Get VCI's request type</summary>
///<param name="bytes">accsess size</param>
///<param name="type">VCI's request type</param>
///<returns>SEGTYPE's RTYPE Value</returns>
unsigned int G4_CaIss::GetVciReqType( unsigned int bytes, TlmVciExtension::VciRequest_t type )
{
  unsigned int val = (unsigned int)type;
  if((type == TlmVciExtension::CMD_READ) || (type == TlmVciExtension::CMD_WRITE)){
    val = val << 2 ;
    switch(bytes){
    case 2:
      val |= 0x1;
      break;
    case 4:
      val |= 0x2;
      break;
    case 8:
      val |= 0x3;
      break;
    default:
      break;
    }
  }else if(type == TlmVciExtension::CMD_BITOP_SET){
    val = 0x8;
  }else if(type == TlmVciExtension::CMD_BITOP_AND){
    val = 0x9;
  }else if(type == TlmVciExtension::CMD_BITOP_OR){
    val = 0xa;
  }else if(type == TlmVciExtension::CMD_BITOP_XOR){
    val = 0xb;
  }else if(type == TlmVciExtension::CMD_SYNC_BUS){
    val = 0xf;
  }else if(type == TlmVciExtension::CMD_SYNC_MASTER){
    val = 0xe;
  }else if(type == TlmVciExtension::CMD_CAXI){
    val = 0x10;
  }else if(type == TlmVciExtension::CMD_EXCL_LDLW){
    val = 0x14;
  }else if(type == TlmVciExtension::CMD_EXCL_STCW){
    val = 0x15;
  }else{
    val = 0;
  }
  return(val);
}



///<summary>write data to CForestG3M's memory</summary>
///<param name="sideband">sideband information</param>
///<param name="addr">address</param>
///<param name="data">data pointer</param>
///<param name="bytes">data length</param>
///<param name="isDma">DMA access flag</param>
///<param name="debug">debug access flag</param>
///<param name="type">vci request type</param>
///<param name="t">transport time</param>
///<returns>none</returns>
void G4_CaIss::MemWriteToCForestBody(SideBand sideband, 
				     PhysAddr addr, 
				     unsigned char* data, 
				     unsigned int bytes, 
				     bool isDma, 
				     bool debug,
				     TlmVciExtension::VciRequest_t type,
				     sc_time* t)
{
  unsigned int ex_m_latency = 0;
  if(debug){
    mpForestUtil->BusWriteDebug(addr, data, bytes);    
  }else{
    GuardErrType grd_err = GD_NONE_ERR;
    SysErrCause sys_err = SYSERR_NONE;
    if((mFcAddrS <=addr) && (addr <= mFcAddrE)){
      // access to Flash
      if(sideband.GetBMID() >= mpG4CPU->mPeNum){
	unsigned int trans_num = (bytes * 8) / BUS_WIDTH_FLSH;
	if(((bytes * 8) % BUS_WIDTH_FLSH) > 0) trans_num++;
	ex_m_latency = mExMWriteRomLatency * trans_num;
#ifdef EX_MASTER_TEST_LOG
	printf("%s G4_CaIss::MemWriteToCForestBody::Flash(from Axi Master):addr=0x%08x, data_length=%d, trans_num=%d, mExMWriteRomLatency=%d, ex_m_latency=%d\n", 
	       sc_time_stamp().to_string().c_str(), (unsigned int)addr, bytes, trans_num, mExMWriteRomLatency, ex_m_latency);
	fflush(stdout);
#endif
	grd_err = mpForestUtil->FlashAccessSAxi (sideband, addr, data, bytes, DATA_WRITE);
      }else{
#ifdef EX_MASTER_TEST_LOG
	printf("%s G4_CaIss::MemWriteToCForestBody::Flash(from PE%d):addr=0x%08x\n", 
	       sc_time_stamp().to_string().c_str(), sideband.GetBMID(), (unsigned int)addr);
	fflush(stdout);
#endif
	grd_err = mpForestUtil->FlashAccessPe (sideband, addr, data, bytes, DATA_WRITE);
      }
    }else if((mCramAddrS <=addr) && (addr <= mCramAddrE)){
      // access to CRAM
      if(sideband.GetBMID() >= mpG4CPU->mPeNum){
	unsigned int trans_num = (bytes * 8) / BUS_WIDTH_CRAM;
	if(((bytes * 8) % BUS_WIDTH_CRAM) > 0) trans_num++;
	ex_m_latency = mExMWriteCramLatency * trans_num;
#ifdef EX_MASTER_TEST_LOG
	printf("%s G4_CaIss::MemWriteToCForestBody::CRAM(from Axi Master):addr=0x%08x, data_length=%d, trans_num=%d, mExMWriteCramLatency=%d, ex_m_latency=%d\n", 
	       sc_time_stamp().to_string().c_str(), (unsigned int)addr, bytes, trans_num, mExMWriteCramLatency, ex_m_latency);
	fflush(stdout);
#endif
	grd_err = mpForestUtil->CramAccessSAxi (sideband, addr, data, bytes, DATA_WRITE);
      }else{
#ifdef EX_MASTER_TEST_LOG
	printf("%s G4_CaIss::MemWriteToCForestBody::CRAM(from PE%d):addr=0x%08x\n", 
	       sc_time_stamp().to_string().c_str(), sideband.GetBMID(), (unsigned int)addr);
	fflush(stdout);
#endif
	grd_err = mpForestUtil->CramAccessPe (sideband, addr, data, bytes, DATA_WRITE);
      }
    }else{
      // access to others
#ifdef EX_MASTER_TEST_LOG
      printf("%s G4_CaIss::MemWriteToCForestBody::Other:addr=0x%08x\n", 
	     sc_time_stamp().to_string().c_str(), (unsigned int)addr);
      fflush(stdout);
#endif
      unsigned int trans_num = (bytes * 8) / BUS_WIDTH_VCI;
      if(((bytes * 8) % BUS_WIDTH_VCI) > 0) trans_num++;
      if((mLramAddrS <=addr) && (addr <= mLramAddrE)){
	// Local RAM
	ex_m_latency = mExMWriteLramLatency * trans_num; 
#ifdef EX_MASTER_TEST_LOG
	printf("Local RAM area: data_length=%d, trans_num=%d, mExMWriteLramLatency=%d, ex_m_latency=%d\n",
	       bytes, trans_num, mExMWriteLramLatency, ex_m_latency);
	fflush(stdout);
#endif
      }else{
	// Local APB
	ex_m_latency = mExMWriteLapbLatency * trans_num;
#ifdef EX_MASTER_TEST_LOG
	printf("Local APB area: data_length=%d, trans_num=%d, mExMWriteLapbLatency=%d, ex_m_latency=%d\n",
	       bytes, trans_num, mExMWriteLapbLatency, ex_m_latency);
	fflush(stdout);
#endif
      }
      sys_err = mpForestUtil->BusWritePhys(sideband, addr, data, bytes);
    }

    if(grd_err != GD_NONE_ERR){
      ex_m_latency = 0; // clear the latency value
#ifdef TEST_CORE_DBG
      printf("%s G4_CaIss::MemWriteToCForestBody::error=%d\n", 
	     sc_time_stamp().to_string().c_str(), grd_err);
      fflush(stdout);
#endif
      switch(grd_err){
      case GD_CFG_ERR:
	mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CF_GRD);
	break;
      case GD_CFG_ILLACC:
	mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CF_ILL_ACC_SAXI);
	break;
      case GD_CRG_ERR:
	if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
	  mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CRAM_GRD);
	}else{
#ifndef FASTISS_NO_SUPPORT
	  if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
	    if(sideband.GetBMID() >= mpG4CPU->mPeNum){
	      mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CRAM_GRD);
	    }else{
	      mpG4CPU->SetGrdErrFromCa(sideband.GetBMID(), NSMVG4CPUV01::GRD_ERR_CRAM_GRD);
	    }
	  }
#endif
	}
 	break;
      case GD_CRG_ILLACC:
	if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
	  mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CRAM_ILL_ACC_SAXI);
	}else{
#ifndef FASTISS_NO_SUPPORT
	  if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
	    if(sideband.GetBMID() >= mpG4CPU->mPeNum){
	      mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CRAM_ILL_ACC_SAXI);
	    }else{
	      mpG4CPU->SetGrdErrFromCa(sideband.GetBMID(), NSMVG4CPUV01::GRD_ERR_CRAM_ILL_ACC_PE);
	    }
	  }
#endif
	}
 	break;
      default:
	break;
      }
    }else{
      if((sys_err & SYSERR_PEG) == SYSERR_PEG){
	// PEG error
	unsigned int peg_peid;
	ex_m_latency = 0; // clear the latency value
	peg_peid = GetResourcePeFromAddress((unsigned int)addr, bytes);
	if(peg_peid < mpG4CPU->mPeNum){
	  mpG4CPU->SetGrdErrFromCa(peg_peid, NSMVG4CPUV01::GRD_ERR_PE_GRD_S);
	}
      }
    }
  }
  if(ex_m_latency > 0){
    *t += sc_time( (mClkPeriod * ex_m_latency), glb_resolution_unit );
  }
}


///<summary>read data from CForestG3M's memory</summary>
///<param name="sideband">sideband information</param>
///<param name="addr">address</param>
///<param name="data">data pointer</param>
///<param name="bytes">data length</param>
///<param name="isDma">DMA access flag</param>
///<param name="debug">debug access flag</param>
///<param name="type">vci request type</param>
///<param name="t">transport time</param>
///<returns>none</returns>
void G4_CaIss::MemReadToCForestBody(SideBand sideband, 
				    PhysAddr addr, 
				    unsigned char* data, 
				    unsigned int bytes, 
				    bool isDma, 
				    bool debug,
				    TlmVciExtension::VciRequest_t type,
				    sc_time* t)
{
  unsigned int ex_m_latency = 0;
  if(debug){
    mpForestUtil->BusReadDebug(addr, data, bytes);
  }else{
    GuardErrType grd_err = GD_NONE_ERR;
    SysErrCause sys_err = SYSERR_NONE;
    if((mFcAddrS <=addr) && (addr <= mFcAddrE)){
      // access to Flash
      if(sideband.GetBMID() >= mpG4CPU->mPeNum){
	unsigned int trans_num = (bytes * 8) / BUS_WIDTH_FLSH;
	if(((bytes * 8) % BUS_WIDTH_FLSH) > 0) trans_num++;
	ex_m_latency = mExMReadRomLatency * trans_num;
#ifdef EX_MASTER_TEST_LOG
	printf("%s G4_CaIss::MemReadToCForestBody::Flash(from Axi Master):addr=0x%08x, data_length=%d, trans_num=%d, mExMReadRomLatency=%d, ex_m_latency=%d\n", 
	       sc_time_stamp().to_string().c_str(), (unsigned int)addr, bytes, trans_num, mExMReadRomLatency, ex_m_latency);
	fflush(stdout);
#endif
	grd_err = mpForestUtil->FlashAccessSAxi (sideband, addr, data, bytes, DATA_READ);
      }else{
#ifdef EX_MASTER_TEST_LOG
	printf("%s G4_CaIss::MemReadToCForestBody::Flash(from PE%d):addr=0x%08x\n", 
	       sc_time_stamp().to_string().c_str(), sideband.GetBMID(), (unsigned int)addr);
	fflush(stdout);
#endif
	grd_err = mpForestUtil->FlashAccessPe (sideband, addr, data, bytes, DATA_READ);
      }
    }else if((mCramAddrS <=addr) && (addr <= mCramAddrE)){
      // access to CRAM
      if(sideband.GetBMID() >= mpG4CPU->mPeNum){
	unsigned int trans_num = (bytes * 8) / BUS_WIDTH_CRAM;
	if(((bytes * 8) % BUS_WIDTH_CRAM) > 0) trans_num++;
	ex_m_latency = mExMReadCramLatency * trans_num;
#ifdef EX_MASTER_TEST_LOG
	printf("%s G4_CaIss::MemReadToCForestBody::CRAM(from Axi Master):addr=0x%08x, data_length=%d, trans_num=%d, mExMReadCramLatency=%d, ex_m_latency=%d\n", 
	       sc_time_stamp().to_string().c_str(), (unsigned int)addr, bytes, trans_num, mExMReadCramLatency, ex_m_latency);
	fflush(stdout);
#endif
	grd_err = mpForestUtil->CramAccessSAxi (sideband, addr, data, bytes, DATA_READ);
      }else{
#ifdef EX_MASTER_TEST_LOG
	printf("%s G4_CaIss::MemReadToCForestBody::CRAM(from PE%d):addr=0x%08x\n", 
	       sc_time_stamp().to_string().c_str(), sideband.GetBMID(), (unsigned int)addr);
	fflush(stdout);
#endif
	grd_err = mpForestUtil->CramAccessPe (sideband, addr, data, bytes, DATA_READ);
      }
    }else{
      // access to others
#ifdef EX_MASTER_TEST_LOG
      printf("%s G4_CaIss::MemReadToCForestBody::Other:addr=0x%08x\n", 
	     sc_time_stamp().to_string().c_str(), (unsigned int)addr);
      fflush(stdout);
#endif
      unsigned int trans_num = (bytes * 8) / BUS_WIDTH_VCI;
      if(((bytes * 8) % BUS_WIDTH_VCI) > 0) trans_num++;
      if((mLramAddrS <=addr) && (addr <= mLramAddrE)){
	// Local RAM
	ex_m_latency = mExMReadLramLatency * trans_num;
#ifdef EX_MASTER_TEST_LOG
	printf("Local RAM area: data_length=%d, trans_num=%d, mExMReadLramLatency=%d, ex_m_latency=%d\n", 
	       bytes, trans_num, mExMReadLramLatency, ex_m_latency);
	fflush(stdout);
#endif
      }else{
	// Local APB
	ex_m_latency = mExMReadLapbLatency * trans_num;
#ifdef EX_MASTER_TEST_LOG
	printf("Local APB area: data_length=%d, trans_num=%d, mExMReadLapbLatency=%d, ex_m_latency=%d\n",
	       bytes, trans_num, mExMReadLapbLatency, ex_m_latency);
	fflush(stdout);
#endif
      }
      sys_err = mpForestUtil->BusReadPhys(sideband, addr, data, bytes);
    }

    if(grd_err != GD_NONE_ERR){
	ex_m_latency = 0; // clear the latency value
#ifdef TEST_CORE_DBG
      printf("%s G4_CaIss::MemReadToCForestBody::error=%d\n", 
	     sc_time_stamp().to_string().c_str(), grd_err);
      fflush(stdout);
#endif
      switch(grd_err){
      case GD_CFG_ERR:
	mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CF_GRD);
	break;
      case GD_CFG_ILLACC:
	mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CF_ILL_ACC_SAXI);
	break;
      case GD_CRG_ERR:
	if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
	  mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CRAM_GRD);
	}else{
#ifndef FASTISS_NO_SUPPORT
	  if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
	    if(sideband.GetBMID() >= mpG4CPU->mPeNum){
	      mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CRAM_GRD);
	    }else{
	      mpG4CPU->SetGrdErrFromCa(sideband.GetBMID(), NSMVG4CPUV01::GRD_ERR_CRAM_GRD);
	    }
	  }
#endif
	}
 	break;
      case GD_CRG_ILLACC:
	if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
	  mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CRAM_ILL_ACC_SAXI);
	}else{
#ifndef FASTISS_NO_SUPPORT
	  if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
	    if(sideband.GetBMID() >= mpG4CPU->mPeNum){
	      mpG4CPU->SetGrdErrFromCa(0xffffffff, NSMVG4CPUV01::GRD_ERR_CRAM_ILL_ACC_SAXI);
	    }else{
	      mpG4CPU->SetGrdErrFromCa(sideband.GetBMID(),  NSMVG4CPUV01::GRD_ERR_CRAM_ILL_ACC_PE);
	    }
	  }
#endif
	}
 	break;
      default:
	break;
      }
    }else{
      if((sys_err & SYSERR_PEG) == SYSERR_PEG){
	// PEG error
	unsigned int peg_peid;
	ex_m_latency = 0; // clear the latency value
	peg_peid = GetResourcePeFromAddress((unsigned int)addr, bytes);
	if(peg_peid < mpG4CPU->mPeNum){
	  mpG4CPU->SetGrdErrFromCa(peg_peid, NSMVG4CPUV01::GRD_ERR_PE_GRD_S);
	}
      }
    }
  }
  if(ex_m_latency > 0){
    *t += sc_time( (mClkPeriod * ex_m_latency), glb_resolution_unit );
  }
}


///<summary>set Flash ROM area</summary>
///<param name="s_addr">start address</param>
///<param name="e_addr">end address</param>
///<returns>none</returns>
void G4_CaIss::SetFromArea(unsigned int s_addr, unsigned int e_addr)
{
  mFcAddrS = s_addr;
  mFcAddrE = e_addr;
}


///<summary>set Cluster RAM area</summary>
///<param name="s_addr">start address</param>
///<param name="e_addr">end address</param>
///<returns>none</returns>
void G4_CaIss::SetCramArea(unsigned int s_addr, unsigned int e_addr)
{
  mCramAddrS = s_addr;
  mCramAddrE = e_addr;
}


///<summary>set Local RAM area</summary>
///<param name="s_addr">start address</param>
///<param name="e_addr">end address</param>
///<returns>none</returns>
void G4_CaIss::SetLramArea(unsigned int s_addr, unsigned int e_addr)
{
  mLramAddrS = s_addr;
  mLramAddrE = e_addr;
}


///<summary>write the value to the code flash guard register</summary>
///<param name="trans">target transaction</param>
///<returns>none</returns>
void G4_CaIss::WriteCfgReg(TlmBasicPayload& trans)
{
  SideBand       sb    = GetSideBandInfo(trans);
  PhysAddr       addr  = (PhysAddr)trans.get_address();
  unsigned int   size  = trans.get_data_length();
  unsigned char* data  = trans.get_data_ptr();
  SysErrCause    sys_err;

#ifdef TEST_CORE_DBG
  printf("G4_CaIss::WriteCfgReg addr=0x%08x, ", (unsigned int)addr);
#endif
  sys_err = mpForestUtil->CfgRegAccess(sb, addr, data, (ByteSize)size, DATA_WRITE);
#ifdef TEST_CORE_DBG
  printf("error=0x%x\n", (unsigned int)sys_err);
  fflush(stdout);
#endif
}


///<summary>read the value to the code flash guard register</summary>
///<param name="trans">target transaction</param>
///<returns>none</returns>
void G4_CaIss::ReadCfgReg(TlmBasicPayload& trans)
{
  SideBand       sb    = GetSideBandInfo(trans);
  PhysAddr       addr  = (PhysAddr)trans.get_address();
  unsigned int   size  = trans.get_data_length();
  unsigned char* data  = trans.get_data_ptr();
  SysErrCause    sys_err;

#ifdef TEST_CORE_DBG
  printf("G4_CaIss::ReadCfgReg addr=0x%08x, ", (unsigned int)addr);
#endif
  sys_err = mpForestUtil->CfgRegAccess(sb, addr, data, (ByteSize)size, DATA_READ);
#ifdef TEST_CORE_DBG
  printf("error=0x%x\n", (unsigned int)sys_err);
  fflush(stdout);
#endif
}


///<summary>write the value to the cluster ram guard register</summary>
///<param name="trans">target transaction</param>
///<returns>none</returns>
void G4_CaIss::WriteCrgReg(TlmBasicPayload& trans)
{
  SideBand       sb    = GetSideBandInfo(trans);
  PhysAddr       addr  = (PhysAddr)trans.get_address();
  unsigned int   size  = trans.get_data_length();
  unsigned char* data  = trans.get_data_ptr();
  SysErrCause    sys_err;

  sys_err = mpForestUtil->CrgRegAccess(sb, addr, data, (ByteSize)size, DATA_WRITE);
}


///<summary>read the value to the cluster ram guard register</summary>
///<param name="trans">target transaction</param>
///<returns>none</returns>
void G4_CaIss::ReadCrgReg(TlmBasicPayload& trans)
{
  SideBand       sb    = GetSideBandInfo(trans);
  PhysAddr       addr  = (PhysAddr)trans.get_address();
  unsigned int   size  = trans.get_data_length();
  unsigned char* data  = trans.get_data_ptr();
  SysErrCause    sys_err;

  sys_err = mpForestUtil->CrgRegAccess(sb, addr, data, (ByteSize)size, DATA_READ);
}


///<summary>write the value to the PE guard register</summary>
///<param name="trans">target transaction</param>
///<returns>none</returns>
void G4_CaIss::WritePegReg(TlmBasicPayload& trans)
{
  SideBand       sb    = GetSideBandInfo(trans);
  PhysAddr       addr  = (PhysAddr)trans.get_address();
  unsigned int   size  = trans.get_data_length();
  unsigned char* data  = trans.get_data_ptr();
  SysErrCause    sys_err;

  sys_err = mpForestUtil->PegRegAccess(sb, addr, data, (ByteSize)size, DATA_WRITE);
}


///<summary>read the value to the PE guard register</summary>
///<param name="trans">target transaction</param>
///<returns>none</returns>
void G4_CaIss::ReadPegReg(TlmBasicPayload& trans)
{
  SideBand       sb    = GetSideBandInfo(trans);
  PhysAddr       addr  = (PhysAddr)trans.get_address();
  unsigned int   size  = trans.get_data_length();
  unsigned char* data  = trans.get_data_ptr();
  SysErrCause    sys_err;

  sys_err = mpForestUtil->PegRegAccess(sb, addr, data, (ByteSize)size, DATA_READ);
}


///<summary>Set the read latency of Flash ROM for the External master(not CPU)</summary>
///<param name="latency">read latency of Flash ROM for the External master</param>
///<returns>none</returns>
void G4_CaIss::setExMReadRomLatency( unsigned int latency )
{
  mExMReadRomLatency = latency;
}


///<summary>Set the write latency of Flash ROM for the External master(not CPU)</summary>
///<param name="latency">write latency of Flash ROM for the External master</param>
///<returns>none</returns>
void G4_CaIss::setExMWriteRomLatency( unsigned int latency )
{
  mExMWriteRomLatency = latency;
}


///<summary>Set the read latency of Cluster RAM for the External master(not CPU)</summary>
///<param name="latency">read latency of Cluster RAM for the External master</param>
///<returns>none</returns>
void G4_CaIss::setExMReadCramLatency( unsigned int latency )
{
  mExMReadCramLatency = latency;
}


///<summary>Set the write latency of Cluster RAM for the External master(not CPU)</summary>
///<param name="latency">write latency of Cluster RAM for the External master</param>
///<returns>none</returns>
void G4_CaIss::setExMWriteCramLatency( unsigned int latency )
{
  mExMWriteCramLatency = latency;
}


///<summary>Set the read latency of Local RAM for the External master(not CPU)</summary>
///<param name="latency">read latency of Local RAM for the External master</param>
///<returns>none</returns>
void G4_CaIss::setExMReadLramLatency( unsigned int latency )
{
  mExMReadLramLatency = latency;
}


///<summary>Set the write latency of Local RAM for the External master(not CPU)</summary>
///<param name="latency">write latency of Local RAM for the External master</param>
///<returns>none</returns>
void G4_CaIss::setExMWriteLramLatency( unsigned int latency )
{
  mExMWriteLramLatency = latency;
}


///<summary>Set the read latency of Local APB's IP for the External master(not CPU)</summary>
///<param name="latency">read latency of Local APB's IP for the External master</param>
///<returns>none</returns>
void G4_CaIss::setExMReadLapbLatency( unsigned int latency )
{
  mExMReadLapbLatency = latency;
}


///<summary>Set the write latency of Local APB's IP for the External master(not CPU)</summary>
///<param name="latency">write latncy of Local APB's IP fof the External master</param>
///<returns>none</returns>
void G4_CaIss::setExMWriteLapbLatency( unsigned int latency )
{
  mExMWriteLapbLatency = latency;
}


///<summary>Get the read latency of Flash ROM for the External master(not CPU)</summary>
///<returns>read latncy of Flash ROM for the External master</returns>
unsigned int G4_CaIss::getExMReadRomLatency( void )
{
  return mExMReadRomLatency;
}


///<summary>Get the write latency of Flash ROM for the External master(not CPU)</summary>
///<returns>write latncy of Flash ROM for the External master</returns>
unsigned int G4_CaIss::getExMWriteRomLatency( void )
{
  return mExMWriteRomLatency;
}


///<summary>Get the read latency of Cluster RAM for the External master(not CPU)</summary>
///<returns>read latncy of Cluster RAM for the External master</returns>
unsigned int G4_CaIss::getExMReadCramLatency( void )
{
  return mExMReadCramLatency;
}


///<summary>Get the write latency of Cluster RAM for the External master(not CPU)</summary>
///<returns>write latncy of Cluster RAM for the External master</returns>
unsigned int G4_CaIss::getExMWriteCramLatency( void )
{
  return mExMWriteCramLatency;
}


///<summary>Get the read latency of Local RAM for the External master(not CPU)</summary>
///<returns>read latncy of Local RAM for the External master</returns>
unsigned int G4_CaIss::getExMReadLramLatency( void )
{
  return mExMReadLramLatency;
}


///<summary>Get the write latency of Local RAM for the External master(not CPU)</summary>
///<returns>write latncy of Local RAM for the External master</returns>
unsigned int G4_CaIss::getExMWriteLramLatency( void )
{
  return mExMWriteLramLatency;
}


///<summary>Get the read latency of Local APB's IP for the External master(not CPU)</summary>
///<returns>read latncy of Local APB's IP for the External master</returns>
unsigned int G4_CaIss::getExMReadLapbLatency( void )
{
  return mExMReadLapbLatency;
}


///<summary>Get the write latency of Local APB's IP for the External master(not CPU)</summary>
///<returns>write latncy of Local APB's IP for the External master</returns>
unsigned int G4_CaIss::getExMWriteLapbLatency( void )
{
  return mExMWriteLapbLatency;
}


///<summary>Get rousource's PE from addoress </summary>
///<param name="addr">address</param>
///<param name="size">size</param>
///<returns>PEID(0:not PE's resource)</returns>
unsigned int G4_CaIss::GetResourcePeFromAddress( unsigned int addr, unsigned int size )
{
  unsigned int peid  = 0;
  unsigned int penum = mpG4CPU->mPeNum;

#ifdef TEST_CORE_DBG
  printf("G4_CaIss::GetResourcePeFromAddress: addr=0x%08x, size=%d\n", addr, size);
  fflush(stdout);
#endif

  for(unsigned int i=0; i<penum; i++){
    if(( (0xFDC00000 - (0x200000 * i)) <= addr ) &&
       ( (addr + size) <= (0xFDE00000 - (0x200000 * i)) )){
      // check Local LAM area
      peid = i;
#ifdef TEST_CORE_DBG
      printf("G4_CaIss::GetResourcePeFromAddress: PE%d LRAM hit\n", peid);
      fflush(stdout);
#endif
      break;
    }else if(( (0xFFFC4000 + (0x4000 * i)) <= addr ) &&
	   ( (addr + size) <= (0xFFFC8000 + (0x4000 * i)) )){
      // check Local APB area
      peid = i;
#ifdef TEST_CORE_DBG
      printf("G4_CaIss::GetResourcePeFromAddress: PE%d CPU peripheral area hit\n", peid);
      fflush(stdout);
#endif
      break;
    }else{
      // not PE's resource
    }
  }
  return(peid);
}
