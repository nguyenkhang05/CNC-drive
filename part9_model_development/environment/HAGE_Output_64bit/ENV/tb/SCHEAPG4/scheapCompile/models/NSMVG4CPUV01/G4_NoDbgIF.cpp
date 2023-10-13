/*************************************************************************
 *
 *  G4_NoDbgIF.cpp
 *
 * Copyright(c) 2016-2018, 2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

/* $Id:$ */

#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno

#include "Common.h"

#include "G4_NoDbgIF.h" 
#include "G4_CaIss.h" 
#include "NSMVG4CPUV01.h"
#include "forest_common_api.h"
#include "forest_utility_api.h"
#ifndef FASTISS_NO_SUPPORT
#include "G4_FastIssIF.h"
#endif // #ifndef FASTISS_NO_SUPPORT

#include "CmErrMsg.h" // for WinMsg

using namespace CmErrMsg; // for WinMsg
#ifndef FASTISS_NO_SUPPORT
using namespace rh850g4; // for ASTC's fastiss
#endif // #ifndef FASTISS_NO_SUPPORT

extern int           token_to_int(char*, const char*, const char*);
extern unsigned int  token_to_uint(char*, const char*, const char*);
extern unsigned long token_to_ulong(char*, const char*, const char*);
extern unsigned long long token_to_ulonglong(char*, const char*, const char*);
extern double        token_to_double(char*, const char*, const char*);
extern unsigned int  token_to_uint_default( char *, unsigned int, const char *, char * );
extern unsigned long long token_to_ulonglong_default(char*, unsigned long long , const char*, const char*);

#define PS_UNIT 1.0E12

///<summary>Constructor</summary>
///<param name="config_file">file name of configulation file</param>
///<param name="pG4CPU">the pointer of NSMVG4CPUV01's instance</param>
///<returns>none</returns>
G4_NoDbgIF::G4_NoDbgIF( const char *config_file, NSMVG4CPUV01 *pG4CPU )
  : mpG4CPU(pG4CPU),
    mPeNum(PE_INIT_NUM),
    mClstrNum(1),
    mFastestPE(0),
    mIsGramWtbufEnable(true),
    mProfileTraceFormat(TRC_PC),
    mProfileTraceAddrRangeS(0),
    mProfileTraceAddrRangeE(0xffffffff),
    mProfileTraceTimeRangeS(0x0000000000000000ULL),
    mProfileTraceTimeRangeE(0xffffffffffffffffULL),
    mProfileMsgLevel(MSG_ERR),
    mCaissSyncPeriod(10),
    mFaissSyncPeriod(1),
    mFaissSyncTimeUnit(FA_SYNC_US),
    mFaissBlockSize(100),
    mpBrkInfoCA(0),
    mIsUnderReset(false),
    mIsFaissLinkFuncEnable(false),
    mIsTraceEnable(false),
    mIsCfgEnable(true),
    mIsCfgIllAccTest(false),
    mIsCrgIllAccTest(false),
    mCfgProtRegion(0x02000000),
    mIsAutoMode(true)
{
  for(int i=0; i<PE_MAX_NUM; i++){
    mSwd0pcadVal[i] = 0xffffffff;
    mIsSwdtEnable[i] = false;
  }

#ifdef TEST_E3
  mTestSetMpm = 0;
#endif
  read_config_file_PeNum(config_file);
  InitConfigParam();
  read_config_file_PeType(config_file);
  read_config_file(config_file);
  CheckPeType();
  CheckMemMap();
  CreateCForestG3M();
#ifndef FASTISS_NO_SUPPORT
  if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
    CreateFastIssIF();
  }
#endif // #ifndef FASTISS_NO_SUPPORT
  mpBrkInfoCA = mpForestUtil->GetBreakInfo();
  SetForestUtilPtr();
}


///<summary>Destructor</summary>
///<returns>none</returns>
G4_NoDbgIF::~G4_NoDbgIF(void)
{
  DeleteCForestG3M();
#ifndef FASTISS_NO_SUPPORT
  if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
    DeleteFastIssIF();
  }
#endif // #ifndef FASTISS_NO_SUPPORT
}


///<summary>initialize each parameter</summary>
///<returns>none</returns>
void G4_NoDbgIF::InitConfigParam(void)
{
  mpPeType      = new unsigned int[mPeNum];
  mpClockRatio  = new unsigned int[mPeNum];
  mpFpuMode     = new bool[mPeNum];
  mpFxuMode     = new bool[mPeNum];
  mpDmdp        = new bool[mPeNum];
  
  for(unsigned int i=0; i<mPeNum; i++){
    mpPeType[i]               = CPU_G4MH_1_0;
    mpPeInfo[i].peid_ival     = i;
    mpPeInfo[i].bmid_ival     = i;
    mpPeInfo[i].spid_ival     = i;
    mpPeInfo[i].gmspid_ival   = i;
    mpPeInfo[i].spidlist_ival = 0xffffffff;
    mpPeInfo[i].rbase_ival    = 0x00000000;
    mpPeInfo[i].snzcfg_ival   = SNZ_INIT_TIME;
    mpPeInfo[i].hvcfg_ival    = 0x00000000;
    mpPeInfo[i].ic_size       = IC_INIT_SIZE;
    mpPeInfo[i].mpu_entry     = MPU_INIT_NUM;
    mpClockRatio[i]           = 1;
    mpFpuMode[i]              = true;
    mpFxuMode[i]              = true;
    mpDmdp[i]                 = false;
    strcpy(mUserProgramName[i], "");
  }
  
  for(unsigned int i=0; i<mClstrNum; i++){
    mpCrgInfo[i].cramif_width = 19;
    mpCrgInfo[i].crg_ch_num   = 8; 
    mpCrgInfo[i].csg_ch_num   = 4;
    mpCrgInfo[i].keycode      = 0xa5a5a500;
  }

  for(unsigned int i=0; i<PE_MAX_NUM; i++){
    mpClkPeriod[i] = +HUGE_VAL ;
  }
  fp_trace = (FILE*)0;
  fp_memory = (FILE*)0;
  fp_trace_summary = (FILE*)0;
  fp_memory_summary = (FILE*)0;
  strcpy(mProfileTrace, "");
  strcpy(mProfileMemory, "");
  strcpy(mProfileTraceSummary, "");
  strcpy(mProfileMemorySummary, "");
}


///<summary>create the instance of CForestG3M</summary>
///<returns>none</returns>
void G4_NoDbgIF::CreateCForestG3M(void)
{
  CreateForestUtil();
  //When the compile version of SC-HEAP is changed to same of cforestg4x, the argument is changed to "FILE*(stdout)"
  SetExTrcFp("");
 
  mpForestUtil->CreateForest ();

  SetCfgInfo();
  SetCrgInfo();
  SetPegInfo();

  for(unsigned int i=0; i<mPeNum; i++ ){
    SetCpuInfo(i, mpPeType[i]);
    SetHvInfo(i,  mpPeType[i], mpPeInfo[i].hvcfg_ival);
    SetFpuMode(i, mpFpuMode[i]);
    SetFxuMode(i, mpFxuMode[i]);
    SetMpuInfo(i, mpPeInfo[i].mpu_entry);
    SetSnzTime(i, mpPeInfo[i].snzcfg_ival);
    SetNcRbase(i, mpPeInfo[i].rbase_ival);
    SetNcSpid(i, mpPeInfo[i].spid_ival);
    SetICacheInfo(i,  mpPeInfo[i].ic_size, 0);
    SetClockRatio(i, mpClockRatio[i]);
    SetDmdpMode(i, mpDmdp[i]);
    mpG4CPU->mpCaIss->SetPeType(i, (unsigned int)ChangeCpuTypeVal(mpPeType[i]));
#ifdef TEST_E3
    //mpForestUtil->SetExtPin(i, EXT_PIN_HAS_MPM_DBIT, mTestSetMpm);
#endif
  }

  SetGramWtBufInfo();
  SetExTrcAddrRange(mProfileTraceAddrRangeS, mProfileTraceAddrRangeE);
  SetExTrcTimeRange(mProfileTraceTimeRangeS, mProfileTraceTimeRangeE);

  mpForestUtil->GenPeripherals ();

  if((SIM_MODE_FAST == mpG4CPU->GetSimMode()) || (strcmp(mProfileTrace, "") == 0)){
#ifdef CAISS_DEBUG
    mpForestUtil->SetDebugMode(DBG_ON|DBG_RW);
    //fp_trace = (FILE*)stdout;
#else
    mpForestUtil->SetDebugMode(DBG_OFF);
#endif
  }else{
    if((TRC_PC == mProfileTraceFormat) || (TRC_OLD_PC == mProfileTraceFormat)){
      mpForestUtil->SetDebugMode(DBG_ON|DBG_RW);
    }else{
      mpForestUtil->SetDebugMode(DBG_ON|DBG_PIPE);
    }
  }
  mpForestUtil->SetMessageLevel(mProfileMsgLevel);

  SetMemMap();

  //When the compile version of SC-HEAP is changed to same of cforestg4x, the argument is changed to "FILE*(fp_trace)"
  SetExTrcFp(mProfileTrace);
  SetMemTrcFp(fp_memory);
}


///<summary>create the instances of FastIssIF</summary>
///<returns>none</returns>
void G4_NoDbgIF::CreateFastIssIF(void)
{
#ifndef FASTISS_NO_SUPPORT
  if(strcmp(mProfileTrace, "") !=0){
#if defined(TEST_RESET_DBG) || defined(TEST_SNZ_TIME)
    if(strcmp(mProfileTrace, "STDOUT") == 0){
      fp_trace = (FILE*)stdout;
    }else{
      fp_trace = fopen(mProfileTrace, "w");
    }
#else
    fp_trace = fopen(mProfileTrace, "w");
#endif
    if(fp_trace == (FILE*)0){
      char error_message_buf[1024];
      char error_mod_buf[1024];
      
      sprintf(error_mod_buf, "%s(%s)", mpG4CPU->name(), "G4_NoDbgIF");
      // for WinMsg
      sprintf(error_message_buf,
	      "Couldn't open file: %s", mProfileTrace);
      printErrorMessage(error_mod_buf, "CreateFastIssIF", error_message_buf);
      exit(1);
    }
  }
  mpFastIssIF = new G4_FastIssIF(mPeNum, fp_trace);
#endif
}

///<summary>create the instances of FastIssIF</summary>
///<returns>none</returns>
void G4_NoDbgIF::InitFastIssIF(void)
{
#ifndef FASTISS_NO_SUPPORT
  switch (mFaissSyncTimeUnit){
  case FA_SYNC_PS:
    mpFastIssIF->SetSyncPeriod(mFaissSyncPeriod, sc_core::SC_PS);
    break;
  case FA_SYNC_NS:
    mpFastIssIF->SetSyncPeriod(mFaissSyncPeriod, sc_core::SC_NS);
    break;
  case FA_SYNC_US:
    mpFastIssIF->SetSyncPeriod(mFaissSyncPeriod, sc_core::SC_US);
    break;
  case FA_SYNC_MS:
    mpFastIssIF->SetSyncPeriod(mFaissSyncPeriod, sc_core::SC_MS);
    break;
  case FA_SYNC_SEC:
    mpFastIssIF->SetSyncPeriod(mFaissSyncPeriod, sc_core::SC_SEC);
    break;
  default:
    mpFastIssIF->SetSyncPeriod(mFaissSyncPeriod, sc_core::SC_US);
    break;
  }

  for(unsigned int i=0; i<mPeNum; i++){
    mpFastIssIF->mpFastIssMemWrapper_ASTC[i]->configure(
                         mLRamInfoQue[i][0]->size,
                         mLRamInfoQue[i][0]->base,
                         mRomInfoQue[i/2][0]->size,
                         mRomInfoQue[i/2][0]->base,
                         mLRamInfoQue[i][0]->base + (0x200000*(i+1)));
    mpFastIssIF->SetBlockSize(i, mFaissBlockSize);   
    mpFastIssIF->SetCpuInfo(i, (CPU_KIND_T)mpPeType[i], &mpPeInfo[i]);
    mpFastIssIF->SetSwdtEnable(i, mIsSwdtEnable[i]);
    mpFastIssIF->SetSnzTime(i, mpPeInfo[i].snzcfg_ival);
  }
  mpFastIssIF->setLinkFuncEnable(mIsFaissLinkFuncEnable);
#endif // #ifndef FASTISS_NO_SUPPORT
}


///<summary>delete the instances of CForestG3M</summary>
///<returns>none</returns>
void G4_NoDbgIF::DeleteCForestG3M(void)
{
  if(mpForestUtil != NULL){
    delete mpForestUtil;
  }
}


///<summary>delete the instances of FastIssIF</summary>
///<returns>none</returns>
void G4_NoDbgIF::DeleteFastIssIF(void)
{
#ifndef FASTISS_NO_SUPPORT
  if(mpFastIssIF != NULL){
    delete mpFastIssIF;
  }
#endif // #ifndef FASTISS_NO_SUPPORT
}


///<summary>Get next execution type</summary>
///<returns>next execution type</returns>
ExecType G4_NoDbgIF::GetNextExecType( void )
{
#if defined(TEST_G4CPU_MISG_API_DBG)
  return EXEC_TYPE_STEP;
#else
  return EXEC_TYPE_RUN;
#endif
}

///<summary>Get next execution peid</summary>
///<returns>next execution peid</returns>
uint32_t G4_NoDbgIF::GetNextExecPeid( void )
{
  return 0;
}

///<summary>excecute ISS</summary>
///<returns>none</returns>
void G4_NoDbgIF::ExecuteISS( void )
{
  unsigned int lc_cycle;

#ifndef FASTISS_NO_SUPPORT
  // clear continue event
  if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
    for(unsigned int i=0; i<mPeNum; i++){
      mpFastIssIF->CancelContEvent(i);
    }
  }
#endif // #ifndef FASTISS_NO_SUPPORT

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    BreakHitAttribute break_type;
    GuardErrType grd_err = GD_NONE_ERR;
    unsigned int pe_id   = 0;
    unsigned int code    = 0;
    if(!mIsAutoMode){
#ifdef TEST_CORE_START_DBG
      printf("%s NoDbgIF::ExecuteISS (BOOTCTRL)\n", 
	     sc_time_stamp().to_string().c_str()); 
      fflush(stdout);
#endif
      for(unsigned int i=0; i<mPeNum; i++){
	// remove all PE from execution target
	mpForestUtil->SetRunTarget(i, false);
      }
    }
    while(1){
#if defined(EXEC_DBG)
      printf("%s:G4_NoDbgIF::ExecuteISS:wait ececute while clock frequency is 0\n", sc_time_stamp().to_string().c_str());
      fflush(stdout);
#endif
      mpG4CPU->waitWhl0freq();
      if(!mIsUnderReset){
#if defined(EXEC_DBG) || defined(TEST_RESET_DBG)
	printf("\n");
	printf("before CycleExecute current time = %s\n", sc_time_stamp().to_string().c_str());
	fflush(stdout);
#endif
	// The call to GetNextExecType must be before the call to GetNextExecPeid
	ExecType type = GetNextExecType();
	pe_id = GetNextExecPeid();
	lc_cycle = (unsigned int)mpForestUtil->CycleExecute(pe_id, mCaissSyncPeriod, type);
#if defined(TEST_G4CPU_MISG_API_DBG)
	unsigned long long temp_data_l, temp_data_u;
	unsigned int  temp_length;
	unsigned long long temp_pc = getCurrentPc(0);
	printf("getCurrentPc(PE0): executed PC=0x%016llx\n", temp_pc);
	unsigned int temp_w_cnt   = getMemAccCntCurrentPc(0, true);
	printf("getMemAccCntCurrentPc(PE0):write: access num=%d\n", temp_w_cnt);
	for (int i=0; i<temp_w_cnt; i++){
	  getMemAccDataCurrentPc(0, i, &temp_data_l, &temp_data_u, & temp_length, true);
	  printf("getMemAccCntCurrentPc(PE0):write: index=%d: data_upper=0x%016llx, data_lower=0x%016llx, length=%d\n",
		 i, temp_data_u, temp_data_l, temp_length);
	}
	fflush(stdout);
#endif
	flushTraceData();
	break_type = CheckBreakType(pe_id, code, grd_err);
#if defined(EXEC_DBG) || defined(TEST_RESET_DBG)
	printf("after CycleExecute lc_cycle=%d\n", lc_cycle);
	fflush(stdout);
#endif
	if(0 < lc_cycle){
	  mpG4CPU->mpCaIss->elapseTime(lc_cycle * mpClkPeriod[mFastestPE]);
#if defined(EXEC_DBG) || defined(TEST_RESET_DBG)
	  printf("after CycleExecute current time = %s\n", sc_time_stamp().to_string().c_str());
	  fflush(stdout);
#endif
	}
	if( BRK_ACCEPT_INT == (break_type & BRK_ACCEPT_INT)){
	  // Interrupt acceptance
	  SetAckInfo(pe_id, code);
	}
	if( BRK_CHANGE_PSWH == (break_type & BRK_CHANGE_PSWH)){
	  // CPU mode is changed
	  unsigned int pswh_val = GetPswh(pe_id);
	  bool cpu_gm = false;
	  if ((pswh_val & 0x80000000) == 0x80000000) cpu_gm = true;
	  unsigned int cpu_gpid = ((pswh_val >> 8) & 0x7);
	  mpG4CPU->SetCpuGmInfoFromCa(pe_id, cpu_gm, cpu_gpid);
	}
	if(BRK_PC_MATCH == (break_type & BRK_PC_MATCH)){
	  // PC detection
	  mpG4CPU->SetSwdtClkOutEvFromCa(pe_id);
	}
	if(BRK_GUARD_ERR == (break_type & BRK_GUARD_ERR)){
	  // Guard Error
#ifdef TEST_GRD_CORE_DBG
	  unsigned long long inst_count = mpForestUtil->GetStepCount(pe_id);
#if defined(_MSC_VER)
	  printf("%s G4_NoDbgIF::ExecuteISS::Guard Error: pe=%d, inst num=%I64d, error=%d\n", 
		 sc_time_stamp().to_string().c_str(), pe_id, inst_count, grd_err);
#else
	  printf("%s G4_NoDbgIF::ExecuteISS::Guard Error: pe=%d, inst num=%lld, error=%d\n", 
		 sc_time_stamp().to_string().c_str(), pe_id, inst_count, grd_err);
#endif
	  fflush(stdout);
#endif
	  NSMVG4CPUV01::GRD_ERR_SRC_T grd_err_typ = (NSMVG4CPUV01::GRD_ERR_SRC_T)GetGrdSrc(grd_err);
	  if(grd_err_typ == NSMVG4CPUV01::GRD_ERR_PE_GRD_S){
	    unsigned int grd_id = pe_id;
	    GetAccTypeAtGrdErr(pe_id, grd_id);
#ifdef TEST_GRD_CORE_DBG
	    printf("G4_NoDbgIF::ExecuteISS:PEG guard error (PEGCAP_S:PE%d)\n", grd_id);
	    fflush(stdout);
#endif
	    mpG4CPU->SetGrdErrFromCa(grd_id, grd_err_typ);
	  }else{
#ifdef TEST_GRD_CORE_DBG
	    if(grd_err_typ == NSMVG4CPUV01::GRD_ERR_PE_GRD_M){
	      printf("G4_NoDbgIF::ExecuteISS:PEG guard error (PEGCAP_M:PE%d)\n", pe_id);
	      fflush(stdout);
	    }else{
	      printf("G4_NoDbgIF::ExecuteISS:not PEG guard error error type %d, PE%d\n", grd_err_typ, pe_id);
	      fflush(stdout);
	    }
#endif
	    mpG4CPU->SetGrdErrFromCa(pe_id, grd_err_typ);
	  }
	}
      }else{
	lc_cycle = mCaissSyncPeriod;
#if defined(EXEC_DBG) || defined(TEST_RESET_DBG)
	printf("reset wait lc_cycle=%d\n", lc_cycle);
	fflush(stdout);
#endif
	if(0 < lc_cycle){
	  mpG4CPU->mpCaIss->elapseTime(lc_cycle * mpClkPeriod[mFastestPE]);
#if defined(EXEC_DBG) || defined(TEST_RESET_DBG)
	  printf("after reset wait current time = %s\n", sc_time_stamp().to_string().c_str());
	  fflush(stdout);
#endif
	}
      }
    }
  }else{ // SIM_MODE_FAST == mIssMode
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      if(mIsAutoMode){
	for(unsigned int i=0; i<mPeNum; i++){
	  mpFastIssIF->SetContEvent(i);
	}
      }
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>set the PE's cycle period and check the period ratio</summary>
///<param name="clstr_num">CLUSTER ID</param>
///<param name="period">cycle period</param>
///<returns>none</returns>
void G4_NoDbgIF::SetPeClkPeriod( unsigned int clstr_num, double period )
{
  if(mClstrNum > clstr_num){
    if(mpClkPeriod[mFastestPE] > period){
      mFastestPE = clstr_num;
      mpG4CPU->mpCaIss->SetClkPeriod(period);
    }
  }
  mpClkPeriod[clstr_num] = period;
}


///<summary>get ForestUtilApi instance's pointer</summary>
///<returns>ForestUtilApi instance's pointer</returns>
ForestUtilApi* G4_NoDbgIF::GetForestUtilPtr( void )
{
  return(mpForestUtil);
}

#ifndef FASTISS_NO_SUPPORT
///<summary>get FastIssIF instance's pointer</summary>
///<returns>FastIssIF instance's pointer</returns>
G4_FastIssIF* G4_NoDbgIF::GetFastIssIFPtr( void )
{
  return(mpFastIssIF);
}
#endif // #ifndef FASTISS_NO_SUPPORT


///<summary>reset operation</summary>
///<returns>none</returns>
void G4_NoDbgIF::reset( void )
{
#ifdef TEST_RESET_DBG
  printf("%s G4_NoDbgIF::reset call\n", sc_time_stamp().to_string().c_str());
  fflush(stdout);
#endif // #ifdef TEST_RESET_DBG

  mpForestUtil->HardReset();

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    // nothing
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      for(unsigned int i=0; i<mPeNum; i++){
	mpFastIssIF->ResetSysReg(i);
      }
#ifdef TEST_RESET_DBG
      mpFastIssIF->TestResetLog();
#endif // #ifdef TEST_RESET_DBG
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>Check break type</summary>
///<param name="peid">PE ID</param>
///<param name="code">Exception code</param>
///<returns>break type</returns>
BreakHitAttribute G4_NoDbgIF::CheckBreakType(unsigned int& peid, unsigned int& code, GuardErrType& gderr)
{
  BreakInfo* pBrkInf = 0;
  pBrkInf = mpBrkInfoCA;

  gderr = pBrkInf->gd_err;
  if ( (BRK_ACCEPT_INT) == (pBrkInf->attr & BRK_ACCEPT_INT)) {
    //must check
    code = mpForestUtil->GetExpCause(pBrkInf->peid);
  }else{
    code = 0;
  }
  peid = pBrkInf->peid;
  
  return(pBrkInf->attr);
}


///<summary>Set Ack info(CA ISS ->INTC1)</summary>
///<param name="peid">PE ID</param>
///<param name="code">Exception code</param>
///<returns>none</returns>
void G4_NoDbgIF::SetAckInfo(unsigned int peid, unsigned int code)
{
  if(0xe0 == code){
    mpG4CPU->SetFenmiAckFromCa( peid, true);
  }else if(0xf0 == (code & 0x1ff0)){
    mpG4CPU->SetFeintAckFromCa( peid, true, code-0xF0);
  }else if(code < 0x2000){
    mpG4CPU->SetEiintAckFromCa( peid, true, code-0x1000);
  }
}


///<summary>Set Fenmi's request for CAISS</summary>
///<param name="pe_id">PE ID</param>
///<param name="int_req">Request value</param>
///<returns>None</returns>
void G4_NoDbgIF::SetFenmiReq( unsigned int pe_id, bool int_req )
{
#ifdef TEST_INTC1_DBG
  printf("NoDbg::SetFenmiReq request to CA(pe%d) req=%d\n",pe_id, int_req);
  fflush(stdout);
#endif
  if(int_req){
    mpForestUtil->ReqFenmiToCpu(pe_id);
  }else{
    mpForestUtil->CancelFenmiToCpu(pe_id);
  }
}


///<summary>Set Feint's request for CAISS</summary>
///<param name="pe_id">PE ID</param>
///<param name="int_req">Request value</param>
///<param name="int_ch">Channel value</param>
///<param name="is_gm">true:GMFEINT/BGFEINT, false:FEINT</param>
///<param name="gpid">target GPID</param>
///<param name="is_bgint">true:BGFEINT, false:FEINT/GMFEINT</param>
///<returns>None</returns>
void G4_NoDbgIF::SetFeintReq( unsigned int pe_id, bool int_req, unsigned int int_ch, bool is_gm, unsigned int gpid, bool is_bgint )
{
#ifdef TEST_INTC1_DBG
  printf("G4_NoDbgIF::SetFeintReq request to CA(pe%d) req=%d\n", pe_id, int_req);
  fflush(stdout);
#endif
  if(int_req){
    if(is_gm){// GMFEINT
      mpForestUtil->ReqFeintToCpu(pe_id, int_ch, true, gpid, false);
    }else if(is_bgint){// BGFEINT
      mpForestUtil->ReqFeintToCpu(pe_id, int_ch, true, gpid, true);
    }else{ //FEINT
      mpForestUtil->ReqFeintToCpu(pe_id, int_ch, false, 0, false);
    }
  }else{
    mpForestUtil->CancelFeintToCpu(pe_id);
  }
}


///<summary>Set Eiint's request for CAISS</summary>
///<param name="pe_id">PE ID</param>
///<param name="int_req">Request value</param>
///<param name="int_pri">Priority value</param>
///<param name="int_ch">Channel value</param>
///<param name="int_eitb">EITB value</param>
///<param name="is_gm">true:GMEIINT/BGEIINT, false:EIINT</param>
///<param name="gpid">target GPID</param>
///<param name="is_bgint">true:BGEIINT, false:EIINT/GMEIINT</param>
/////<returns>None</returns>
void G4_NoDbgIF::SetEiintReq( unsigned int pe_id, bool int_req, unsigned int int_pri, unsigned int int_ch, bool int_eitb, bool is_gm, unsigned int gpid, bool is_bgint )
{
#ifdef TEST_INTC1_DBG
  printf("G4_NoDbgIF::SetEiintReq request to CA(pe%d) req=%d\n", pe_id, int_req);
  fflush(stdout);
#endif
  if(int_req){
    if(is_gm){// GMEIINT
      mpForestUtil->ReqInterruptToCpu(pe_id, int_ch, int_pri, int_eitb, true, gpid, false);
    }else if(is_bgint){// BGEIINT
      mpForestUtil->ReqInterruptToCpu(pe_id, int_ch, int_pri, int_eitb, true, gpid, true);
    }else{//EIINT
      mpForestUtil->ReqInterruptToCpu(pe_id, int_ch, int_pri, int_eitb, false, 0, false);
    }
  }else{
    mpForestUtil->CancelInterruptToCpu(pe_id, int_ch);
  }
}


///<summary>Set Execution mode</summary>
///<param name="pe_id">PE ID</param>
//<param name="isRun">Run mode (true:execution start, false:execution stop)</param>
///<returns>None</returns>
void G4_NoDbgIF::SetExecutionMode( unsigned int pe_id, bool isRun )
{
  if(pe_id >= mPeNum) return;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
#ifdef TEST_CORE_START_DBG
    if (isRun){
      printf("%s G4_NoDbgIF::SetExecutionMode(CAISS) PE%d run\n", 
	     sc_time_stamp().to_string().c_str(), pe_id);
    }else{
      printf("%s G4_NoDbgIF::SetExecutionMode(CAISS) PE%d stop\n", 
	     sc_time_stamp().to_string().c_str(), pe_id);
    }
    fflush(stdout);
#endif
    mpForestUtil->SetRunTarget(pe_id, isRun);
  }else{ 
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      if(isRun){
#ifdef TEST_CORE_START_DBG
	printf("%s G4_NoDbgIF::SetExecutionMode(FASTISS) PE%d run\n", 
	     sc_time_stamp().to_string().c_str(), pe_id);
	fflush(stdout);
#endif
	mpFastIssIF->SetContEvent(pe_id);
      }else{
#ifdef TEST_CORE_START_DBG
	printf("%s G4_NoDbgIF::SetExecutionMode(FASTISS) PE%d stop\n", 
	     sc_time_stamp().to_string().c_str(), pe_id);
	fflush(stdout);
#endif
	mpFastIssIF->CancelContEvent(pe_id);
      }
    }
#endif
  }
}


///<summary>read the configulation file to get effective PE number</summary>
///<param name="filename">file name of configulation file</param>
///<returns>none</returns>
void G4_NoDbgIF::read_config_file_PeNum( const char *filename )
{
  char    word[512];
  char    seps[] = " ,=();\t\n\r";
  char    *token;
  bool      error_detected = false;

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
    if (strcmp(token, "[PE_NUM]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        mPeNum = token_to_uint( token,
				"G4_NoDbgIF",
				"[PE_NUM]" );
	mClstrNum = (mPeNum / 2) + (mPeNum % 2);
	if(( mPeNum > PE_MAX_NUM ) || ((mPeNum !=1) && ((mPeNum % 2) !=0)) || ( mPeNum == 0 )){
	  char error_message_buf[1024];
	  char error_mod_buf[1024];
	  // for WinMsg
	  sprintf(error_message_buf,
		  "[PE_NUM] must be 1, 2, 4, 6, or 8, but specified [%d]."
		  , mPeNum);
	  sprintf(error_mod_buf,
		  "%s(%s)"
		  , mpG4CPU->name(), "G4_NoDbgIF");
	  printErrorMessage(error_mod_buf, "read_config_file_PeNum", error_message_buf);
	  exit(1);
	}
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[PE_NUM] :mPeNum=%d, mClstrNum=%d\n",
	     mPeNum, mClstrNum);
      fflush(stdout);
#endif
      continue;
    }
  }  // end of configuration file read loop(infinite)
}  // end of read_config_file_PeNum()


///<summary>read the configulation file to get effective PE type</summary>
///<param name="filename">file name of configulation file</param>
///<returns>none</returns>
void G4_NoDbgIF::read_config_file_PeType( const char *filename )
{
  char    word[512];
  char    seps[] = " ,=();\t\n\r";
  char    *token;
  bool      error_detected = false;
  char error_mod_buf[1024];

  sprintf(error_mod_buf, "%s(%s)", mpG4CPU->name(), "G4_NoDbgIF");

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
    if (strcmp(token, "[G4CPU_PE_TYPE]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_PE_TYPE] peid(1st parameter)" );
        if( peid >= mPeNum ){
          char error_message_buf[1024];
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_PE_TYPE] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(error_mod_buf, "read_config_file_PeType", error_message_buf);
        }else{
          token = strtok(0, seps);
	  if(strcmp(token, "G4MH") == 0){
	    mpPeType[peid] = CPU_G4MH_1_0;
	  }else if(strcmp(token, "G4MH_V10") == 0){
	    mpPeType[peid] = CPU_G4MH_1_0;
	  }else if(strcmp(token, "G4MH_V11") == 0){
	    mpPeType[peid] = CPU_G4MH_1_1;
	  }else if(strcmp(token, "G4MH_V20") == 0){
	    mpPeType[peid] = CPU_G4MH_2_0;
	  } else{
            char error_message_buf[1024];
            sprintf(error_message_buf,
		    "Unexpected token was specified [%s] on [G4CPU_PE_TYPE] PE type(2nd parmeter).",
		    token);
            printErrorMessage(error_mod_buf, "read_config_file_PeType", error_message_buf);
            error_detected = true;
	  }
	}
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_PE_TYPE]:PE%d :mpPeType[%d]=%d\n",
	     peid, peid, mpPeType[peid]);
      fflush(stdout);
#endif
      continue;
    }
  }  // end of configuration file read loop(infinite)
}  // end of read_config_file_PeType()


///<summary>read the configulation file to get each parameter except the effective PE number</summary>
///<param name="filename">file name of configulation file</param>
///<returns>none</returns>
void G4_NoDbgIF::read_config_file( const char *filename )
{
  char    word[512];
  char    seps[] = " ,=();\t\n\r";
  char    *token;
  bool    error_detected = false;
  char error_mod_buf[1024];
  char error_message_buf[1024];

  sprintf(error_mod_buf, "%s(%s)", mpG4CPU->name(), "G4_NoDbgIF");

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
    if (strcmp(token, "[G4CPU_PROGRAM]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_PROGRAM] peid(1st parameter)" );
        if( peid >= mPeNum ){
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_PROGRAM] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
          if (token != NULL){
            strcpy(mUserProgramName[peid], token);
          } else {
            strcpy(mUserProgramName[peid], "");
          }
        }
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_PROGRAM]:PE%d :mUserProgramName[%d]=%s\n",
	     peid, peid, mUserProgramName[peid]);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_PE_INFO]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_PE_INFO] pe index(1st parameter)" );
        if( peid >= mPeNum ){
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_PE_INFO] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpPeInfo[peid].peid_ival = token_to_uint( token,
						       "G4_NoDbgIF",
						       "[G4CPU_PE_INFO] peid(2nd parameter)" );
	    if(mpPeInfo[peid].peid_ival > PEID_MAX_NUM){
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_PE_INFO] peid(2nd parameter).",
		      token);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpPeInfo[peid].bmid_ival = token_to_uint( token,
						      "G4_NoDbgIF",
						      "[G4CPU_PE_INFO] bus master id(3rd parameter)" );
	    if(mpPeInfo[peid].bmid_ival > BMID_MAX_NUM){
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_PE_INFO] bus master id(3rd parameter).",
		      token);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpPeInfo[peid].spid_ival = token_to_uint( token,
						       "G4_NoDbgIF",
						       "[G4CPU_PE_INFO] spid(4th parameter)" );
	    if(mpPeInfo[peid].spid_ival > SPID_MAX_NUM){
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_PE_INFO] spid(4th parameter).",
		      token);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpPeInfo[peid].spidlist_ival = token_to_uint( token,
							   "G4_NoDbgIF",
							   "[G4CPU_PE_INFO] spidlist(5th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpPeInfo[peid].rbase_ival = token_to_uint( token,
							"G4_NoDbgIF",
							"[G4CPU_PE_INFO] rbase(6th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    unsigned int snz_time = token_to_uint( token,
						   "G4_NoDbgIF",
						   "[G4CPU_PE_INFO] snooze time(7th parameter)" );
	    if(snz_time < 256){
	      mpPeInfo[peid].snzcfg_ival = snz_time;
	    }else{
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_PE_INFO] snooze time(7th parameter).",
		      token);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }
        }
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_PE_INFO]:PE%d :peid=%d, bmid=%d, spid=%d, spidlist=0z%08x, rbase=0x%08x, snzcfg=%d\n",
	     peid, mpPeInfo[peid].peid_ival, mpPeInfo[peid].bmid_ival, mpPeInfo[peid].spid_ival, mpPeInfo[peid].spidlist_ival, mpPeInfo[peid].rbase_ival, mpPeInfo[peid].snzcfg_ival);
      fflush(stdout);
#endif
      continue;
    }


    if (strcmp(token, "[G4CPU_HV_INFO]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_HV_INFO] pe index(1st parameter)" );
        if( peid >= mPeNum ){
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_HV_INFO] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
	  if(strcmp(token, "true") == 0){
	    mpPeInfo[peid].hvcfg_ival = 1;
	  } else{
	    mpPeInfo[peid].hvcfg_ival = 0;
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpPeInfo[peid].gmspid_ival = token_to_uint( token,
							"G4_NoDbgIF",
							"[G4CPU_HV_INFO] gmspid(3rd parameter)" );
	    if(mpPeInfo[peid].gmspid_ival > SPID_MAX_NUM){
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_HV_INFO] gmspid(3rd parameter).",
		      token);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }
        }
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_HV_INFO]:PE%d : hvcfg=0x%08x, gmspid=0x%08x\n",
	     peid, mpPeInfo[peid].hvcfg_ival, mpPeInfo[peid].gmspid_ival);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_ICACHE]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_ICACHE] peid(1st parameter)" );
        if( peid >= mPeNum ){
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_ICACHE] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
	  if (token != NULL){
	    unsigned int ic_size = token_to_uint( token,
					       "G4_NoDbgIF",
					       "[G4CPU_ICACHE] size(2nd parameter)" );
	    if((ic_size == 0)  ||
	       (ic_size == 1)  ||
	       (ic_size == 2)  ||
	       (ic_size == 4)  ||
	       (ic_size == 8)  ||
	       (ic_size == 16) ||	
	       (ic_size == 32) ||
	       (ic_size == 64)){
	      mpPeInfo[peid].ic_size = ic_size;
	    }else{
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_ICACHE] size(2nd parameter).",
		      token);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }
        }
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_ICACHE]:PE%d :mpIcSize[%d]=%d\n",
	     peid, peid, mpPeInfo[peid].ic_size);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_ROM_INFO]") == 0) {
      unsigned int clid = 0;
      mem_info *rom_info = new mem_info;
      rom_info->base=FROM_INIT_BASE;
      rom_info->size=FROM_INIT_SIZE;
      rom_info->f_latency=FROM_INIT_F_LATENCY;
      rom_info->r_latency=FROM_INIT_R_LATENCY;
      rom_info->w_latency=FROM_INIT_W_LATENCY;
      rom_info->cl_f_latency=FROM_INIT_CL_F_LATENCY;
      rom_info->cl_r_latency=FROM_INIT_CL_R_LATENCY;
      rom_info->cl_w_latency=FROM_INIT_CL_W_LATENCY;
      rom_info->ex_f_latency=FROM_INIT_EX_F_LATENCY;
      rom_info->ex_r_latency=FROM_INIT_EX_R_LATENCY;
      rom_info->ex_w_latency=FROM_INIT_EX_W_LATENCY;
      rom_info->outstand_latency=FROM_OUTSTAND;
      rom_info->repeat_latency=FROM_REPEAT;
      for (int i=0; i<BANK_LIST_MAX; i++){
	rom_info->bank_size[i]=0;
      }
      rom_info->bank_size[0]=rom_info->size;
      token = strtok(0, seps);
      if (token != NULL){
        clid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_ROM_INFO] peid(1st parameter)" );
        if( clid >= mClstrNum ){
          sprintf(error_message_buf,
                  "[G4CPU_ROM_INFO] cluster index must be under %d, but specified [%d]."
                  , mClstrNum, clid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->base = token_to_uint( token,
					    "G4_NoDbgIF",
					    "[G4CPU_ROM_INFO] base address(2nd parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->size = token_to_uint( token,
					    "G4_NoDbgIF",
					    "[G4CPU_ROM_INFO] rom size(3rd parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->f_latency = token_to_uint( token,
						 "G4_NoDbgIF",
						 "[G4CPU_ROM_INFO] fetch latency from own cluster(4th parameter)" );
	    rom_info->cl_f_latency = rom_info->f_latency;
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->r_latency = token_to_uint( token,
						 "G4_NoDbgIF",
						 "[G4CPU_ROM_INFO] read latency from own cluster(5th parameter)" );
	    rom_info->cl_r_latency = rom_info->r_latency;
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->w_latency = token_to_uint( token,
						 "G4_NoDbgIF",
						 "[G4CPU_ROM_INFO] write latency from own cluster(6th parameter)" );
	    rom_info->cl_w_latency = rom_info->w_latency;
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->ex_f_latency = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_ROM_INFO] fetch latency from other cluster(7rd parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->ex_r_latency = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_ROM_INFO] read latency from other cluster(8th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->ex_w_latency = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_ROM_INFO] write latency from other cluster(9th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->outstand_latency = token_to_uint( token,
							"G4_NoDbgIF",
							"[G4CPU_ROM_INFO] outstand latency(10th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    rom_info->repeat_latency = token_to_uint( token,
						      "G4_NoDbgIF",
						      "[G4CPU_ROM_INFO] repeat latency(11th parameter)" );
	  }
	  unsigned int total_bank_size = 0;
	  for (int j=0; j<BANK_LIST_MAX; j++){
	    token = strtok(0, seps);
	    if (token != NULL){
	      char err_msg_buf[64];
	      int bank_size=0;
	      // for WinMsg
	      sprintf(err_msg_buf, "[G4CPU_ROM_INFO] bank size(%dth parameter)", 12+j);
	      bank_size = token_to_int( token,
					"G4_NoDbgIF",
					err_msg_buf );
	      rom_info->bank_size[j] = bank_size * 1024;
	      total_bank_size += rom_info->bank_size[j];
	      if(total_bank_size > rom_info->size){
		printErrorMessage(error_mod_buf, "read_config_file", "The total bank size of the size list is over the ROM area on [G4CPU_ROM_INFO].");
		error_detected = true;
		break;
	      }
	    }else{
	      break;
	    }
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    sprintf(error_message_buf,
		    "The number of the bank size list is over %d on [G4CPU_ROM_INFO]. Then the remaining settings are ignored.",
		    BANK_LIST_MAX);
	    printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
	  }
	}
      }
      if( clid < mClstrNum ){
	mRomInfoQue[clid].push_back(rom_info);
#ifdef TEST_CORE_DBG
	printf("configuration test(G4_NoDbgIF):[G4CPU_ROM_INFO]:CL%d :rom_info->base=0x%08x, rom_info->size=0x%08x, rom_info->f_latency=%d, rom_info->r_latency=%d, rom_info->w_latency=%d, rom_info->cl_f_latency=%d, rom_info->cl_r_latency=%d, rom_info->cl_w_latency=%d, rom_info->ex_f_latency=%d, rom_info->ex_r_latency=%d, rom_info->ex_w_latency=%d, rom_info->outstand_latency=%d, rom_info->repeat_latency=%d\n",
	       clid, rom_info->base, rom_info->size, rom_info->f_latency, rom_info->r_latency, rom_info->w_latency, rom_info->cl_f_latency, rom_info->cl_r_latency, rom_info->cl_w_latency, rom_info->ex_f_latency, rom_info->ex_r_latency, rom_info->ex_w_latency, rom_info->outstand_latency, rom_info->repeat_latency);
	printf("      ");
	for(int i=0; i<BANK_LIST_MAX; i++){
	  printf(", rom_info->bank_size[%d]=%d", i, rom_info->bank_size[i]);
	}
	printf("\n");
	fflush(stdout);
#endif
      }
      continue;
    }

    if (strcmp(token, "[G4CPU_CRAM_INFO]") == 0) { 
      unsigned int clid = 0;
      mem_info *cram_info = new mem_info;
      cram_info->base=CRAM_INIT_BASE;
      cram_info->size=CRAM_INIT_SIZE;
      cram_info->f_latency=CRAM_INIT_F_LATENCY;
      cram_info->r_latency=CRAM_INIT_R_LATENCY;
      cram_info->w_latency=CRAM_INIT_W_LATENCY;
      cram_info->cl_f_latency=CRAM_INIT_CL_F_LATENCY;
      cram_info->cl_r_latency=CRAM_INIT_CL_R_LATENCY;
      cram_info->cl_w_latency=CRAM_INIT_CL_W_LATENCY;
      cram_info->ex_f_latency=CRAM_INIT_EX_F_LATENCY;
      cram_info->ex_r_latency=CRAM_INIT_EX_R_LATENCY;
      cram_info->ex_w_latency=CRAM_INIT_EX_W_LATENCY;
      cram_info->outstand_latency=CRAM_OUTSTAND;
      cram_info->repeat_latency=CRAM_REPEAT;
      for (int i=0; i<BANK_LIST_MAX; i++){
	cram_info->bank_size[i]=0;
      }
      cram_info->bank_size[0]=cram_info->size;
      token = strtok(0, seps);
      if (token != NULL){
        clid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_CRAM_INFO] peid(1st parameter)" );
        if( clid >= CL_MAX_NUM ){
          sprintf(error_message_buf,
                  "[G4CPU_CRAM_INFO] cluster index must be under %d, but specified [%d]."
                  , CL_MAX_NUM, clid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->base = token_to_uint( token,
					     "G4_NoDbgIF",
					     "[G4CPU_CRAM_INFO] base address(2md parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->size = token_to_uint( token,
					     "G4_NoDbgIF",
					     "[G4CPU_CRAM_INFO] rom size(3rd parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->f_latency = token_to_uint( token,
						  "G4_NoDbgIF",
						  "[G4CPU_CRAM_INFO] fetch latency from own cluster(4th parameter)" );
	    cram_info->cl_f_latency = cram_info->f_latency;
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->r_latency = token_to_uint( token,
						  "G4_NoDbgIF",
						  "[G4CPU_CRAM_INFO] read latency from own cluster(5th parameter)" );
	    cram_info->cl_r_latency = cram_info->r_latency;
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->w_latency = token_to_uint( token,
						  "G4_NoDbgIF",
						  "[G4CPU_CRAM_INFO] write latency from own cluster(6th parameter)" );
	    cram_info->cl_w_latency = cram_info->w_latency;
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->ex_f_latency = token_to_uint( token,
						     "G4_NoDbgIF",
						     "[G4CPU_CRAM_INFO] fetch latency from other cluster(7th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->ex_r_latency = token_to_uint( token,
						     "G4_NoDbgIF",
						     "[G4CPU_CRAM_INFO] read latency from other cluster(8th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->ex_w_latency = token_to_uint( token,
						     "G4_NoDbgIF",
						     "[G4CPU_CRAM_INFO] write latency from other cluster(9th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->outstand_latency = token_to_uint( token,
							 "G4_NoDbgIF",
							 "[G4CPU_CRAM_INFO] outstand latency(10th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    cram_info->repeat_latency = token_to_uint( token,
						       "G4_NoDbgIF",
						       "[G4CPU_CRAM_INFO] repeat latency(11th parameter)" );
	  }
	  unsigned int total_bank_size = 0;
	  for(int j=0; j<BANK_LIST_MAX; j++){
	    token = strtok(0, seps);
	    if (token != NULL){
	      char err_msg_buf[64];
	      int bank_size=0;
	      // for WinMsg
	      sprintf(err_msg_buf, "[G4CPU_CRAM_INFO] bank size(%dth parameter)", 12+j);
	      bank_size = token_to_int( token,
					"G4_NoDbgIF",
					err_msg_buf );
	      cram_info->bank_size[j] = bank_size * 1024;
	      total_bank_size += cram_info->bank_size[j];
	      if(total_bank_size > cram_info->size){
		printErrorMessage(error_mod_buf, "read_config_file", "The total bank size of the size list is over the cluster RAM area on [G4CPU_CRAM_INFO].");
		error_detected = true;
		break;
	      }
	    }else{
	      break;
	    }
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    sprintf(error_message_buf,
		    "The number of the bank size list is over %d on [G4CPU_CRAM_INFO]. Then the remaining settings are ignored.",
		    BANK_LIST_MAX);
	    printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
	  }
	}
      } 
      if( clid < CL_MAX_NUM ){
	mCRamInfoQue[clid].push_back(cram_info);
#ifdef TEST_CORE_DBG
	printf("configuration test(G4_NoDbgIF):[G4CPU_CRAM_INFO]:cl%d :cram_info->base=0x%08x, cram_info->size=0x%08x, cram_info->f_latency=%d, cram_info->r_latency=%d, cram_info->w_latency=%d, cram_info->cl_f_latency=%d, cram_info->cl_r_latency=%d, cram_info->cl_w_latency=%d, cram_info->ex_f_latency=%d, cram_info->ex_r_latency=%d, cram_info->ex_w_latency=%d, cram_info->outstand_latency=%d, cram_info->repeat_latency=%d\n",
	       clid, cram_info->base, cram_info->size, cram_info->f_latency, cram_info->r_latency, cram_info->w_latency, cram_info->cl_f_latency, cram_info->cl_r_latency, cram_info->cl_w_latency, cram_info->ex_f_latency, cram_info->ex_r_latency, cram_info->ex_w_latency, cram_info->outstand_latency, cram_info->repeat_latency);
	printf("      ");
	for(int i=0; i<BANK_LIST_MAX; i++){
	  printf(", cram_info->bank_size[%d]=%d", i, cram_info->bank_size[i]);
	}
	printf("\n");
	fflush(stdout);
#endif
      }
      continue;
    }

    if (strcmp(token, "[G4CPU_LRAM_INFO]") == 0) {
      unsigned int peid=0;
      mem_info *lram_info = new mem_info;
      lram_info->base=LRAM_INIT_BASE;
      lram_info->size=LRAM_INIT_BODY_SIZE;
      lram_info->f_latency=LRAM_INIT_F_LATENCY;
      lram_info->r_latency=LRAM_INIT_R_LATENCY;
      lram_info->w_latency=LRAM_INIT_W_LATENCY;
      lram_info->cl_f_latency=LRAM_INIT_CL_F_LATENCY;
      lram_info->cl_r_latency=LRAM_INIT_CL_R_LATENCY;
      lram_info->cl_w_latency=LRAM_INIT_CL_W_LATENCY;
      lram_info->ex_f_latency=LRAM_INIT_EX_F_LATENCY;
      lram_info->ex_r_latency=LRAM_INIT_EX_R_LATENCY;
      lram_info->ex_w_latency=LRAM_INIT_EX_W_LATENCY;
      lram_info->outstand_latency=LRAM_OUTSTAND;
      lram_info->repeat_latency=LRAM_REPEAT;
      for (int i=0; i<BANK_LIST_MAX; i++){
	lram_info->bank_size[i]=0;
      }
      lram_info->bank_size[0]=lram_info->size;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_LRAM_INFO] peid(1st parameter)" );
        if( peid >= mPeNum ){
          sprintf(error_message_buf,
                  "[G4CPU_LRAM_INFO] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->base = token_to_uint( token,
					    "G4_NoDbgIF",
					    "[G4CPU_LRAM_INFO] base address(2nd parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->size = token_to_uint( token,
					    "G4_NoDbgIF",
					    "[G4CPU_LRAM_INFO] rom size(3rd parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->f_latency = token_to_uint( token,
						 "G4_NoDbgIF",
						 "[G4CPU_LRAM_INFO] fetch latency form own PE(4th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->r_latency = token_to_uint( token,
						 "G4_NoDbgIF",
						 "[G4CPU_LRAM_INFO] read latency from own PE(5th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->w_latency = token_to_uint( token,
						 "G4_NoDbgIF",
						 "[G4CPU_LRAM_INFO] write latency from own PE(6th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->cl_f_latency = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_LRAM_INFO] fetch latency from other PE of same cluster(7th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->cl_r_latency = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_LRAM_INFO] read latency from other PE of same cluster(8th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->cl_w_latency = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_LRAM_INFO] write latency from other PE of same cluster(9th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->ex_f_latency = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_LRAM_INFO] fetch latency from other cluster(10th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->ex_r_latency = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_LRAM_INFO] read latency from other cluster(11th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->ex_w_latency = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_LRAM_INFO] write latency from other cluster(12th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->outstand_latency = token_to_uint( token,
							 "G4_NoDbgIF",
							 "[G4CPU_LRAM_INFO] outstand latency(13th parameter)" );
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    lram_info->repeat_latency = token_to_uint( token,
						       "G4_NoDbgIF",
						       "[G4CPU_LRAM_INFO] repeat latency(14th parameter)" );
	  }
	}
      }
      mLRamInfoQue[peid].push_back(lram_info);
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_LRAM_INFO]:PE%d :lram_info->base=0x%08x, lram_info->size=0x%08x, lram_info->f_latency=%d, lram_info->r_latency=%d, lram_info->w_latency=%d, lram_info->cl_f_latency=%d, lram_info->cl_r_latency=%d, lram_info->cl_w_latency=%d, lram_info->ex_f_latency=%d, lram_info->ex_r_latency=%d, lram_info->ex_w_latency=%d, lram_info->outstand_latency=%d, lram_info->repeat_latency=%d\n",
	     peid, lram_info->base, lram_info->size, lram_info->f_latency, lram_info->r_latency, lram_info->w_latency, lram_info->cl_f_latency, lram_info->cl_r_latency, lram_info->cl_w_latency, lram_info->ex_f_latency, lram_info->ex_r_latency, lram_info->ex_w_latency, lram_info->outstand_latency, lram_info->repeat_latency);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_PROFILE_TRACE]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
	mIsTraceEnable = true;
        strcpy(mProfileTrace, token);
#if 0 
	// When the compile version of SC-HEAP is changed to same of cforestg4x, 
	// SC-HEAP's behavior will be changed to open the file and set file pointer into cforestg4x.
#if defined(TEST_RESET_DBG) || defined(TEST_SNZ_TIME)
	if(strcmp(mProfileTrace, "STDOUT") == 0){
	  fp_trace = (FILE*)stdout;
	}else{
	  fp_trace = fopen(mProfileTrace, "w");
	}
#else
	fp_trace = fopen(mProfileTrace, "w");
#endif
	if(fp_trace == (FILE*)0){
	  // for WinMsg
	  sprintf(error_message_buf,
		  "Couldn't open file: %s", mProfileTrace);
	  printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	  exit(1);
	}
#endif // #if 0
      } else {
	mIsTraceEnable = false;
        strcpy(mProfileTrace, "");
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_PROFILE_TRACE]: mProfileTrace=%s\n",
	     mProfileTrace);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_PROFILE_TRACE_FORMAT]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "PC") == 0){
            mProfileTraceFormat = TRC_PC;
      } else if(strcmp(token, "PIPE") == 0){
            mProfileTraceFormat = TRC_PIPE;
      } else if(strcmp(token, "OLD_PC") == 0){
            mProfileTraceFormat = TRC_OLD_PC;
      } else{
            sprintf(error_message_buf,
                  "Unexpected token was specified [%s] on [G4CPU_PROFILE_TRACE_FORMAT]. Simulation cannot start.",
                  token);
            printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
            error_detected = true;
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_PROFILE_TRACE_FORMAT]: mProfileTraceFormat=%d\n",
	     mProfileTraceFormat);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_PROFILE_MESSAGE_LEVEL]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "LEVEL1") == 0){
	mProfileMsgLevel = MSG_ERR;
      } else if(strcmp(token, "LEVEL2") == 0){
	mProfileMsgLevel = MSG_WAR;
      } else if(strcmp(token, "LEVEL3") == 0){
	mProfileMsgLevel = MSG_EXP;
      } else{
	if(CheckMsgLevel(token) == false){
	  sprintf(error_message_buf,
		  "Unexpected token was specified [%s] on [G4CPU_PROFILE_MESSAGE_LEVEL].",
                  token);
	  printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	  error_detected = true;
	}
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_PROFILE_MESSAGE_LEVEL]: mProfileMsgLevel=%d\n",
	     mProfileMsgLevel);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_PROFILE_MEMORY]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        strcpy(mProfileMemory, token);
	fp_memory = fopen(mProfileMemory, "w");
	if(fp_memory == (FILE*)0){
	  // for WinMsg
	  sprintf(error_message_buf,
		  "Couldn't open file: %s", mProfileMemory);
	  printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	  exit(1);
	}
      } else {
        strcpy(mProfileMemory, "");
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_PROFILE_MEMORY]: mProfileMemory=%s\n",
	     mProfileMemory);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_PROFILE_TRACE_SUMMARY]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        strcpy(mProfileTraceSummary, token);
	fp_trace_summary = fopen(mProfileTraceSummary, "w");
	if(fp_trace_summary == (FILE*)0){
	  // for WinMsg
	  sprintf(error_message_buf,
		  "Couldn't open file: %s", mProfileTraceSummary);
	  printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	  exit(1);
	}
      } else {
        strcpy(mProfileTraceSummary, "");
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_PROFILE_TRACE_SUMMARY]: mProfileTraceSummary=%s\n",
	     mProfileTraceSummary);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_PROFILE_MEMORY_SUMMARY]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        strcpy(mProfileMemorySummary, token);
	fp_memory_summary = fopen(mProfileMemorySummary, "w");
	if(fp_memory_summary == (FILE*)0){
	  // for WinMsg
	  sprintf(error_message_buf,
		  "Couldn't open file: %s", mProfileMemorySummary);
	  printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	  exit(1);
	}
      } else {
        strcpy(mProfileMemorySummary, "");
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_PROFILE_MEMORY_SUMMARY]: mProfileMemorySummary=%s\n",
	     mProfileMemorySummary);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_CAISS_SYNC_PERIOD]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        mCaissSyncPeriod = token_to_uint( token,
					  "G4_NoDbgIF",
					  "[G4CPU_CAISS_SYNC_PERIOD] sync period(1st parameter)" );
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_CAISS_SYNC_PERIOD]: mCaissSyncPeriod=%d\n",
	     mCaissSyncPeriod);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_FASTISS_SYNC_PERIOD]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        mFaissSyncPeriod = token_to_uint( token,
					  "G4_NoDbgIF",
					  "[G4CPU_FASTISS_SYNC_PERIOD] sync period(1st parameter)" );
      }
      token = strtok(0, seps);
      if (token != NULL){
	if(strcmp(token, "PS") == 0){
	  mFaissSyncTimeUnit = FA_SYNC_PS;
	}else if(strcmp(token, "NS") == 0){
	  mFaissSyncTimeUnit = FA_SYNC_NS;
	}else if(strcmp(token, "US") == 0){
	  mFaissSyncTimeUnit = FA_SYNC_US;
	}else if(strcmp(token, "MS") == 0){
	  mFaissSyncTimeUnit = FA_SYNC_MS;
	}else if(strcmp(token, "SEC") == 0){
	  mFaissSyncTimeUnit = FA_SYNC_SEC;
	}else{
	  sprintf(error_message_buf,
		  "Unexpected token was specified [%s] on [G4CPU_FASTISS_SYNC_PERIOD] time unit(2nd parameter).",
		  token);
	  printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	  error_detected = true;
	}
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_FASTISS_SYNC_PERIOD]: mFaissSyncPeriod=%d, mFaissSyncTimeUnit=%d\n",
	     mFaissSyncPeriod, mFaissSyncTimeUnit);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_FASTISS_BLOCK_SIZE]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
	mFaissBlockSize = token_to_uint( token,
					 "G4_NoDbgIF",
					 "[G4CPU_FASTISS_BLOCK_SIZE] block size(1st parameter)" );
	if(mFaissBlockSize > 100){
          sprintf(error_message_buf,
                  "[G4CPU_FASTISS_BLOCK_SIZE] block size must be under 101, but specified [%d]. Treat 100 instead."
                  , mFaissBlockSize);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
	  mFaissBlockSize = 100;
	}
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_FASTISS_BLOCK_SIZE]: mFaissBlockSize=%d\n",
	     mFaissBlockSize);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_FASTISS_LINK_FUNC_ENABLE]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "true") == 0){
	mIsFaissLinkFuncEnable = true;
#ifdef TEST_CORE_DBG
	printf("configuration test(G4_NoDbgIF):[G4CPU_FASTISS_LINK_FUNC_ENABLE]: mIsFaissLinkFuncEnable=true\n");
	fflush(stdout);
#endif
      } else{
	mIsFaissLinkFuncEnable = false;
#ifdef TEST_CORE_DBG
	printf("configuration test(G4_NoDbgIF):[G4CPU_FASTISS_LINK_FUNC_ENABLE]: mIsFaissLinkFuncEnable=false\n");
	fflush(stdout);
#endif
      }
      continue;
    }

    if (strcmp(token, "[G4CPU_SWDT_ENABLE]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "true") == 0){
	for(int i=0; i<PE_MAX_NUM; i++){
	  mIsSwdtEnable[i] = true;
	}
#ifdef TEST_CORE_DBG
	printf("configuration test(G4_NoDbgIF):[G4CPU_SWDT_ENABLE]: mIsSwdtEnable=true\n");
	fflush(stdout);
#endif
      } else{
	for(int i=0; i<PE_MAX_NUM; i++){
	  mIsSwdtEnable[i] = false;
	}
#ifdef TEST_CORE_DBG
	printf("configuration test(G4_NoDbgIF):[G4CPU_SWDT_ENABLE]: mIsSwdtEnable=false\n");
	fflush(stdout);
#endif
      }
      continue;
    }
    
    if (strcmp(token, "[G4CPU_DMDP_FUNC_ENABLE]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_DMDP_FUNC_ENABLE] peid(1st parameter)" );
        if( peid >= mPeNum ){
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_DMDP_FUNC_ENABLE] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
	  if(strcmp(token, "true") == 0){
	    mpDmdp[peid] = true;
#ifdef TEST_CORE_DBG
	    printf("configuration test(G4_NoDbgIF):[G4CPU_DMDP_FUNC_ENABLE]: mpDmdp[%d]=true\n", peid);
	    fflush(stdout);
#endif
	  } else{
	    mpDmdp[peid] = false;
#ifdef TEST_CORE_DBG
	    printf("configuration test(G4_NoDbgIF):[G4CPU_DMDP_FUNC_ENABLE]: mpDmdp[%d]=false\n", peid);
	    fflush(stdout);
#endif
	  }
	}
      }
      continue;
    }
   
    if (strcmp(token, "[G4CPU_FXU_ENABLE]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_FXU_ENABLE] peid(1st parameter)" );
        if( peid >= mPeNum ){
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_FXU_ENABLE] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
	  if(strcmp(token, "true") == 0){
	    mpFxuMode[peid] = true;
#ifdef TEST_CORE_DBG
	    printf("configuration test(G4_NoDbgIF):[G4CPU_FXU_ENABLE]: mpFxuMode[%d]=true\n", peid);
	    fflush(stdout);
#endif
	  } else{
	    mpFxuMode[peid] = false;
#ifdef TEST_CORE_DBG
	    printf("configuration test(G4_NoDbgIF):[G4CPU_FXU_ENABLE]: mpFxuMode[%d]=false\n", peid);
	    fflush(stdout);
#endif
	  }
        }
      }
      continue;
    }

    if (strcmp(token, "[G4CPU_MPU_ENTRY]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_MPU_ENTRY] peid(1st parameter)" );
        if( peid >= mPeNum ){
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_MPU_ENTRY] peid must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
        }else{
	  token = strtok(0, seps);
	  if (token != NULL){
	    unsigned int mpu_entry = token_to_uint( token,
						    "G4_NoDbgIF",
						    "[G4CPU_MPU_ENTRY] entry num(2nd parameter)" );
	    if((mpu_entry < 1) || (mpu_entry > MPU_MAX_NUM )){
	      sprintf(error_message_buf,
		      "[G4CPU_MPU_ENTRY] entry number must be 1 to %d, but specified [%d]",
		      MPU_MAX_NUM, mpu_entry);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }else{
	      mpPeInfo[peid].mpu_entry = mpu_entry;
	    }
	  }
        }
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_MPU_ENTRY]:PE%d : mpu_entry=%d\n",
	     peid, mpPeInfo[peid].mpu_entry);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_CRG_INFO]") == 0) {
      unsigned int clid=0;
      token = strtok(0, seps);
      if (token != NULL){
        clid = token_to_uint( token,
                              "G4_NoDbgIF",
                              "[G4CPU_CRG_INFO] cluster index(1st parameter)" );
        if( clid >= CL_MAX_NUM ){
          // for WinMsg
          sprintf(error_message_buf,
		  "[G4CPU_CRG_INFO] cluster index must be under %d, but specified [%d]."
                  , CL_MAX_NUM, clid);
          printWarningMessage(error_mod_buf, "read_config_file", error_message_buf);
	}else{
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpCrgInfo[clid].cramif_width = token_to_uint( token,
							  "G4_NoDbgIF",
							  "[G4CPU_CRG_INFO] CRAM-IF address width(2nd parameter)" );
	    if((mpCrgInfo[clid].cramif_width != 18) &&
	       (mpCrgInfo[clid].cramif_width != 19) &&
	       (mpCrgInfo[clid].cramif_width != 20) &&
	       (mpCrgInfo[clid].cramif_width != 21)){
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_CRG_INFO] CRAMM-IF address width(2nd parameter).",
		      token);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpCrgInfo[clid].crg_ch_num = token_to_uint( token,
							"G4_NoDbgIF",
							"[G4CPU_CRG_INFO] CRG channel number(3rd parameter)" );
	    if((mpCrgInfo[clid].crg_ch_num == 0) || (mpCrgInfo[clid].crg_ch_num > CRG_CH_MAX_NUM)){
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_CRG_INFO] CRG channel number(3rd parameter).",
		      token);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }	  
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpCrgInfo[clid].csg_ch_num = token_to_uint( token,
							"G4_NoDbgIF",
							"[G4CPU_CRG_INFO] CSG channel number(4th parameter)" );
	    if((mpCrgInfo[clid].csg_ch_num == 0) || (mpCrgInfo[clid].csg_ch_num > CSG_CH_MAX_NUM)){
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_CRG_INFO] CSG channel number(4th parameter).",
		      token);
	      printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }

	  token = strtok(0, seps);
	  if (token != NULL){
	    mpCrgInfo[clid].keycode = token_to_uint( token,
						     "G4_NoDbgIF",
						     "[G4CPU_CRG_INFO] CRG regoster's keycode(5th parameter)" );
	  }
        }
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(G4_NoDbgIF):[G4CPU_CRG_INFO]:clid=%d, mpCrgInfo[%d].cramif_width=%d, mpCrgInfo[%d].crg_ch_num=%d, mpCrgInfo[%d].csg_ch_num=%d, mpCrgInfo[%d].keycode=0x%08x\n",
	     clid, clid, mpCrgInfo[clid].cramif_width, clid, mpCrgInfo[clid].crg_ch_num, clid, mpCrgInfo[clid].csg_ch_num, clid, mpCrgInfo[clid].keycode);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_START_MODE]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "AUTO") == 0){
	mIsAutoMode = true;
#ifdef TEST_CORE_DBG
	printf("configuration test(G4_NoDbgIF):[G4CPU_START_MODE]: mIsAutoMode=true\n");
	fflush(stdout);
#endif
      }else if(strcmp(token, "BOOTCTRL") == 0){
	mIsAutoMode = false;
#ifdef TEST_CORE_DBG
	printf("configuration test(G4_NoDbgIF):[G4CPU_START_MODE]: mIsAutoMode=false\n");
	fflush(stdout);
#endif
      } else {
	sprintf(error_message_buf,
		"Unexpected token was specified [%s] on [G4CPU_START_MODE].",
		token);
	printErrorMessage(error_mod_buf, "read_config_file", error_message_buf);
	error_detected = true;
      }
      continue;
    }

    // *********************************
    // disable until 2016/9/E version  *
    // *********************************

#ifdef TEST_E3
    if (strcmp(token, "[TEST_SET_MPM]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        mTestSetMpm = token_to_uint( token,
				     "G4_NoDbgIF",
				     "[TEST_SET_MPM] MPM mode(1st palameter)" );
      }
      continue;
    }
    if (strcmp(token, "[TEST_CFG_ENABLE]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "true") == 0){
	mIsCfgEnable = true;
      }else{
	mIsCfgEnable = false;
      }
      continue;
    }
    if (strcmp(token, "[TEST_CFG_PROT_REGION]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        mCfgProtRegion = token_to_uint( token,
					"G4_NoDbgIF",
					"[TEST_CFG_PROT_REGION] CFG protected region(1st palameter)" );
      }
      continue;
    }
    if (strcmp(token, "[TEST_CFG_ILLACC]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "true") == 0){
	mIsCfgIllAccTest = true;
      }else{
	mIsCfgIllAccTest = false;
      }
      continue;
    }
    if (strcmp(token, "[TEST_GRG_ILLACC]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "true") == 0){
	mIsCrgIllAccTest = true;
      }else{
	mIsCrgIllAccTest = false;
      }
      continue;
    }
#endif

  }  // end of configuration file read loop(infinite)
}  // end of read_config_file()


// 以下はコンフィグレーション内容をCForestG3Mに設定するための関数

///<summary>set the address mask</summary>
///<param name="mask_val">address mask</param>
///<returns>none</returns>
void G4_NoDbgIF::SetAddrMask( unsigned int mask_val )
{
}


///<summary>set each PE's CPU information (PE type) to cforestg4</summary>
///<param name="pe_id">PE ID</param>
///<param name="pe_type">PE type</param>
///<returns>none</returns>
void G4_NoDbgIF::SetCpuInfo ( unsigned int pe_id, unsigned int pe_type )
{
  if(pe_type == CPU_G4MH_1_1){
    mpForestUtil->SetPe (pe_id/2, pe_id, (CoreType)ChangeCpuTypeVal(pe_type), CORE_VER_11);
  }else if (pe_type == CPU_G4MH_2_0){
    mpForestUtil->SetPe (pe_id/2, pe_id, (CoreType)ChangeCpuTypeVal(pe_type), CORE_VER_20);
  }else{
    mpForestUtil->SetPe (pe_id/2, pe_id, (CoreType)ChangeCpuTypeVal(pe_type), CORE_VER_10);
  }
}


///<summary>set each PE's HV information to cforestg4</summary>
///<param name="pe_id">PE ID</param>
///<param name="pe_type">PE type</param>
///<param name="hvcfg_val">The inital value of HVCFG</param>
///<returns>none</returns>
void G4_NoDbgIF::SetHvInfo ( unsigned int pe_id, unsigned int pe_type, unsigned int hvcfg_val)
{
  if(pe_type == CPU_G4MH_2_0){
    mpForestUtil->SetExtPin (pe_id, EXT_PIN_HAS_HV, true);
  }else{
    mpForestUtil->SetExtPin (pe_id, EXT_PIN_HAS_HV, false);
  }
  mpForestUtil->SetExtPin (pe_id, EXT_PIN_HVE, (hvcfg_val & 0x1));
}


///<summary>set each PE's FPU mode(ON/OFF) to CForestG3M</summary>
///<param name="pe_id">PE ID</param>
///<param name="isFpuOn">FPU mode (enable or disable)</param>
///<returns>none</returns>
void G4_NoDbgIF::SetFpuMode ( unsigned int pe_id, bool isFpuOn )
{
  mpForestUtil->SetExtPin (pe_id, EXT_PIN_HAS_FPU, isFpuOn); //for v20121119
}


///<summary>set each PE's FXU mode(ON/OFF) to CForestG3M</summary>
///<param name="pe_id">PE ID</param>
///<param name="isFxuOn">FXU mode (enable or disable)</param>
///<returns>none</returns>
void G4_NoDbgIF::SetFxuMode ( unsigned int pe_id, bool isFxuOn )
{
  mpForestUtil->SetExtPin (pe_id, EXT_PIN_HAS_FXU, isFxuOn);
}


///<summary>set each PE's MPU information(area number) to CForestG3M</summary>
///<param name="pe_id">PE ID</param>
///<param name="mpu_area_num">MPU area number</param>
///<returns>none</returns>
void G4_NoDbgIF::SetMpuInfo ( unsigned int pe_id, unsigned int mpu_area_num )
{
  mpForestUtil->SetExtPin (pe_id, EXT_PIN_MPTRGN, mpu_area_num);
}


///<summary>set each PE's Snooze time to CForestG3M</summary>
///<param name="pe_id">PE ID</param>
///<param name="snz_time">Snooze time</param>
///<returns>none</returns>
void G4_NoDbgIF::SetSnzTime ( unsigned int pe_id, unsigned int snz_time )
{
  mpForestUtil->SetExtPin (pe_id, EXT_PIN_SNZ_PERIOD, snz_time & 0xFFFF);
}


///<summary>set each PE's NC reset base to CForestG3M</summary>
///<param name="pe_id">PE ID</param>
///<param name="rbase_addr">NC reset base</param>
///<returns>none</returns>
void G4_NoDbgIF::SetNcRbase ( unsigned int pe_id, unsigned int rbase_addr )
{
  mpForestUtil->SetExtPin(pe_id, EXT_PIN_RBASE, rbase_addr >> 9);
  mpForestUtil->SetExtPin(pe_id, EXT_PIN_RINT, rbase_addr);
}


///<summary>set the NC SPID to CForestG3M</summary>
///<param name="pe_id">PE ID</param>
///<param name="spid_val">SPID</param>
///<returns>none</returns>
void G4_NoDbgIF::SetNcSpid ( unsigned int pe_id, unsigned int spid_val )
{
  mpForestUtil->SetExtPin(pe_id, EXT_PIN_SPID, spid_val);
}


///<summary>set the I-cache information (size, way) to CForestG3M</summary>
///<param name="pe_id">PE ID</param>
///<param name="ic_size">I-Cache size</param>
///<param name="ic_way">I-Cache way number</param>
///<returns>none</returns>
void G4_NoDbgIF::SetICacheInfo ( unsigned int pe_id, unsigned int ic_size, unsigned int ic_way )
{
  if(ic_size != 0){
    mpForestUtil->SetExtPin(pe_id, EXT_PIN_ICHEN, 1);
  }
  mpForestUtil->SetIcacheSize (pe_id, ic_size*1024);
}


///<summary>set the D-cache information (size, way) to CForestG3M</summary>
///<param name="dc_size">D-Cache size</param>
///<param name="dc_way">D-Cache way number</param>
///<returns>none</returns>
void G4_NoDbgIF::SetDCacheInfo ( unsigned int dc_size, unsigned int dc_way )
{
}


///<summary>check the memory map</summary>
///<returns>none</returns>
void G4_NoDbgIF::CheckMemMap ( void )
{
  for(unsigned int i=0; i<mClstrNum; i++){
    if(mRomInfoQue[i].size() <= 0){
      if(i>0){
	if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
	  break;
	}
      }
      mem_info *rom_info = new mem_info;
      // default value is for E2xFCC1(E2GM/E2M)
      rom_info->base      = FROM_INIT_BASE + (FROM_INIT_SIZE * i);
      rom_info->size      = FROM_INIT_SIZE;
      rom_info->f_latency = FROM_INIT_F_LATENCY;
      rom_info->r_latency = FROM_INIT_R_LATENCY;
      rom_info->w_latency = FROM_INIT_W_LATENCY;
      rom_info->cl_f_latency     = FROM_INIT_CL_R_LATENCY;
      rom_info->cl_r_latency     = FROM_INIT_CL_R_LATENCY;
      rom_info->cl_w_latency     = FROM_INIT_CL_W_LATENCY;
      rom_info->ex_f_latency     = FROM_INIT_EX_F_LATENCY;
      rom_info->ex_r_latency     = FROM_INIT_EX_R_LATENCY;
      rom_info->ex_w_latency     = FROM_INIT_EX_W_LATENCY;
      rom_info->outstand_latency = FROM_OUTSTAND;
      rom_info->repeat_latency   = FROM_REPEAT;
      for (int j=0; j<BANK_LIST_MAX; j++){
	rom_info->bank_size[j] = 0;
      }
      rom_info->bank_size[0] = (FROM_INIT_SIZE / 2); // Bank A (4MB)
      rom_info->bank_size[1] = (FROM_INIT_SIZE / 2); // Bank B (4MB)
      mRomInfoQue[i].push_back(rom_info);
    }
    if(mCRamInfoQue[0].size() <= 0){
      mem_info *cram_info = new mem_info;
      // default value is for E2xFCC1
      cram_info->base      = CRAM_INIT_BASE + (CRAM_INIT_SIZE * i);
      cram_info->size      = CRAM_INIT_SIZE;
      cram_info->f_latency = CRAM_INIT_F_LATENCY;
      cram_info->r_latency = CRAM_INIT_R_LATENCY;
      cram_info->w_latency = CRAM_INIT_W_LATENCY;
      cram_info->cl_f_latency     = CRAM_INIT_CL_F_LATENCY;
      cram_info->cl_r_latency     = CRAM_INIT_CL_R_LATENCY;
      cram_info->cl_w_latency     = CRAM_INIT_CL_W_LATENCY;
      cram_info->ex_f_latency     = CRAM_INIT_EX_F_LATENCY;
      cram_info->ex_r_latency     = CRAM_INIT_EX_R_LATENCY;
      cram_info->ex_w_latency     = CRAM_INIT_EX_W_LATENCY;
      cram_info->outstand_latency = CRAM_OUTSTAND;
      cram_info->repeat_latency   = CRAM_REPEAT;
      for (int j=0; j<BANK_LIST_MAX; j++){
	cram_info->bank_size[j] = 0;
      }
      cram_info->bank_size[0] = cram_info->size;
      mCRamInfoQue[0].push_back(cram_info);
    }
  }
  for(unsigned int i=0; i<mPeNum; i++){
    if(mLRamInfoQue[i].size() <= 0){
      mem_info *lram_info = new mem_info;
      lram_info->base=LRAM_INIT_BASE - (LRAM_INIT_SIZE * (i + 1));
      lram_info->size=LRAM_INIT_SIZE;
      lram_info->f_latency=LRAM_INIT_F_LATENCY;
      lram_info->r_latency=LRAM_INIT_R_LATENCY;
      lram_info->w_latency=LRAM_INIT_W_LATENCY;
      lram_info->cl_f_latency=LRAM_INIT_CL_F_LATENCY;
      lram_info->cl_r_latency=LRAM_INIT_CL_R_LATENCY;
      lram_info->cl_w_latency=LRAM_INIT_CL_W_LATENCY;
      lram_info->ex_f_latency=LRAM_INIT_EX_F_LATENCY;
      lram_info->ex_r_latency=LRAM_INIT_EX_R_LATENCY;
      lram_info->ex_w_latency=LRAM_INIT_EX_W_LATENCY;
      lram_info->outstand_latency=LRAM_OUTSTAND;
      lram_info->repeat_latency=LRAM_REPEAT;
      for (int j=0; j<BANK_LIST_MAX; j++){
	lram_info->bank_size[j] = 0;
      }
      mLRamInfoQue[i].push_back(lram_info);
    }
  }
}

///<summary>set the executiont trace format to CForestG3M</summary>
///<param name="ex_trc_format">trace format (TRC_PC or TRC_PIPE)</param>
///<returns>none</returns>
void G4_NoDbgIF::SetExTrcFormat ( unsigned int ex_trc_format )
{
}


///<summary>set the executiont trace output address range to CForestG3M</summary>
///<param name="ex_trc_saddr">trace start address</param>
///<param name="ex_trc_eaddr">trace end address</param>
///<returns>none</returns>
void G4_NoDbgIF::SetExTrcAddrRange ( unsigned int ex_trc_saddr, unsigned int ex_trc_eaddr )
{
}


///<summary>set the executiont trace output time range to CForestG3M</summary>
///<param name="ex_trc_stime">trace start time</param>
///<param name="ex_trc_etime">trace end time</param>
///<returns>none</returns>
void G4_NoDbgIF::SetExTrcTimeRange ( unsigned long long ex_trc_stime, unsigned long long ex_trc_etime )
{
}


///<summary>set the cache trace event to CForestG3M</summary>
///<param name="mem_trc_ev">trace event</param>
///<returns>none</returns>
void G4_NoDbgIF::SetMemTrcEvent ( unsigned int mem_trc_ev )
{
}


///<summary>set the cache trace output address range to CForestG3M</summary>
///<param name="mem_trc_saddr">trace start address</param>
///<param name="mem_trc_eaddr">trace end address</param>
///<returns>none</returns>
void G4_NoDbgIF::SetMemTrcAddrRange ( unsigned int mem_trc_saddr, unsigned int mem_trc_eaddr )
{
}


///<summary>set the cache trace output time range to CForestG3M</summary>
///<param name="mem_trc_stime">trace start time</param>
///<param name="mem_trc_etime">trace end time</param>
///<returns>none</returns>
void G4_NoDbgIF::SetMemTrcTimeRange ( unsigned long long mem_trc_stime, unsigned long long mem_trc_etime )
{
}

// When the compile version of SC-HEAP is changed to same of cforestg4x, this function is used"
#if 0
///<summary>set the file pointer of ececution trace file to CForestG3M</summary>
///<param name="fp">file pointer of ececution trace file</param>
///<returns>none</returns>
void G4_NoDbgIF::SetExTrcFp ( FILE* fp )
{
  if((FILE*)0 == fp){
    mpForestUtil->SetDebugMode(DBG_OFF);
  }else{
    mpForestUtil->SetFileOut (fp);
  }
}
#endif

///<summary>set the file pointer of meory trace file to CForestG3M</summary>
///<param name="fp">file pointer of meory trace file</param>
///<returns>none</returns>
void G4_NoDbgIF::SetMemTrcFp ( FILE* fp )
{
  if((FILE*)0 == fp){
  }else{
  }
}


///<summary>get the file pointer of ececution trace file</summary>
///<returns>the file pointer of ececution trace file</returns>
FILE* G4_NoDbgIF::GetTraceFp()
{
  return (fp_trace);
}


///<summary>Load hex data to CForestG3M or FastISS</summary>
///<returns>none</returns>
void G4_NoDbgIF::LoadHex ( void )
{
  FILE *fp;
  char error_message_buf[1024];
  char error_mod_buf[1024];

#ifdef TEST_E3
#ifndef FASTISS_NO_SUPPORT
  if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
    for(unsigned int j=0; j<mClstrNum; j++){
      for(unsigned int i=0; i<mRomInfoQue[j].size(); i++ ){
	unsigned int addr = mRomInfoQue[j][i]->base;
	unsigned int addr_end = mRomInfoQue[j][i]->base + mRomInfoQue[j][i]->size;
	unsigned char init_data[4];
	for(int i=0; i<4; i++){
	  init_data[i]=0xff;
	}  
	while(addr < addr_end){
	  for(unsigned int j=0; j<mPeNum; j++ ){
	    GetFastIssIFPtr()->mpFastIss_ASTC[j]->iN_store(addr, 4, init_data);
	  }
	  addr = addr + 4;
	}
      }
    }
  }
#endif // #ifndef FASTISS_NO_SUPPORT
#endif

  sprintf(error_mod_buf, "%s(%s)", mpG4CPU->name(), "G4_NoDbgIF");

  for(unsigned int i=0; i<mPeNum; i++ ){
    if (strcmp(mUserProgramName[i],"") == 0) {
      sprintf(error_message_buf,
	      "Program Load Error!\nPE%d's user program isn't specified.", 
	      i);
      printErrorMessage(error_mod_buf, "LoadHex", error_message_buf);
      exit(1);
      return;
    }

    fp=fopen(mUserProgramName[i], "r");
    if (fp==NULL){
      sprintf(error_message_buf, 
	      "Program Load Error!\nFile open error: %s", 
	      mUserProgramName[i]);
      printErrorMessage(error_mod_buf, "LoadHex", error_message_buf);
      exit(1);
      return;
    }
    fclose(fp);

    // load hex to ISS
    std::string hex_fname (mUserProgramName[i]);
    LoadSrec(hex_fname.c_str(), 0, i);
  }
}


/// <summary>load the hex file to iss</summary>
/// <param name="addr">address</param>
/// <param name="data">data</param>
/// <param name="peid">PE ID</param>
/// <returns>none</returns>
void G4_NoDbgIF::LoadSrecBody (unsigned int addr, unsigned char data, unsigned int peid)
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    // load hex to CA ISS
    mpForestUtil->VMemWriteDebug( peid, addr, (unsigned int)data, 1 );
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      // load hex to Fast ISS
      mpFastIssIF->mpFastIss_ASTC[peid]->iN_store(addr, 1, &data);

    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>read memory value</summary>
///<param name="peid">PE ID</param>
///<param name="addr">address</param>
///<param name="byte">data length</param>
///<returns>memory value</returns>
unsigned long long G4_NoDbgIF::readMemValue(unsigned int peid, unsigned int addr, unsigned int byte)
{
  char str[128];
  unsigned char value[8];
  unsigned long long rst = 0;

  if (byte > 8) byte = 8;
  for (int i=0; i<8; i++){
    value[i] = 0x00;
  }
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    GetForestUtilPtr()->BusReadDebug(addr, value, byte);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      unsigned int bytes_read = GetFastIssIFPtr()->mpFastIss_ASTC[peid]->iN_load(addr, byte, value);
      if (bytes_read != byte){
	// error message
      }
    }
    else {
      // error message
    }
#endif
  }
  sprintf(str, "Read at 0x%x : 0x", addr);
  int len = strlen(str);
  for (unsigned int i=byte; i>0; i--) {
    sprintf(&str[len+(byte-i)*2], "%02x", value[i-1]);
    rst |= ((unsigned long long)value[i-1] << (8*(i-1)));
  }
  cout << str << endl;
  
  return(rst);
}

///<summary>write memory value</summary>
///<param name="peid">PE ID</param>
///<param name="addr">address</param>
///<param name="data">write value</param>
///<param name="byte">data length</param>
///<returns>memory value</returns>
unsigned long long G4_NoDbgIF::writeMemValue(unsigned int peid, unsigned int addr, unsigned long long data, unsigned int byte)
{
  char str[128];
  unsigned char value[8];
  unsigned long long rst = 0;
  if (byte > 8) byte = 8;
  for (unsigned int i=0; i<byte; i++) {
    value[i] = (unsigned char) ((data >> (8*i)) & 0xff);
  }
  
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    GetForestUtilPtr()->BusWriteDebug(addr, value, byte);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      unsigned int bytes_written = GetFastIssIFPtr()->mpFastIss_ASTC[peid]->iN_store(addr, byte, value);
      if (bytes_written != byte){
	// error message
      }
      rst = data;
    }
    else {
      // error message
    }
#endif
  }
  sprintf(str, "Wrote at 0x%x : 0x", addr);
  int len = strlen(str);
  for (int i=byte; i>0; i--) {
    sprintf(&str[len+(byte-i)*2], "%02x", value[i-1]);
  }
  cout << str << endl;
  return(rst);
}


///<summary>disp executed instruction number</summary>
///<param name="peid">PE ID</param>
///<param name="isToFile">output flag(true:specified file, false:stdout)</param>
///<returns>none</returns>
void G4_NoDbgIF::dispExInstNum(unsigned int peid, bool isToFile)
{
  unsigned long long inst_count = 0;
  unsigned long long inst_cycle = 0;
  unsigned long long inst_time  = 0;
  
  FILE* fp;

  if(isToFile) fp=fp_trace_summary;
  else         fp=stdout;

  if(fp == (FILE*)0) return;

  if(isToFile){ // output to file
    if(peid == 0){
      fprintf(fp, " Execution Summary\n");
      fprintf(fp, "===================\n");
#if defined(_MSC_VER)
      fprintf(fp, " Total simulation time : %12I64d psec\n", (unsigned long long)(sc_time_stamp().to_seconds() * PS_UNIT));
#else
      fprintf(fp, " Total simulation time : %12lld psec\n", (unsigned long long)(sc_time_stamp().to_seconds() * PS_UNIT));
#endif
    }
  }else{ // output to python console
    fprintf(fp, "Execution Summary\n");
  }

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    inst_count = mpForestUtil->GetStepCount(peid);
    inst_cycle = mpForestUtil->GetCycleCount(peid);
    inst_time  = inst_cycle * (unsigned long long)mpClkPeriod[peid];
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      inst_count = mpFastIssIF->mpFastIss_ASTC[peid]->get_total_inst_num();
      inst_cycle = mpFastIssIF->mpFastIss_ASTC[peid]->get_total_cycle();
      inst_time  = (unsigned long long)(mpFastIssIF->mpFastIss_ASTC[peid]->get_total_time_sec() * PS_UNIT);
    }
#endif
  }

  fprintf(fp, " PE%d's information\n", peid);
#if defined(_MSC_VER)
  fprintf(fp, "  PE%d: Instructions: %12I64d, Cycle: %12I64d, Time: %12I64d psec\n", 
	  peid, inst_count, inst_cycle, inst_time );
#else
  fprintf(fp, "  PE%d: Instructions: %12lld, Cycle: %12lld, Time: %12lld psec\n",
	  peid, inst_count, inst_cycle, inst_time );
#endif

  fflush(fp);
}

///<summary>display I-cache access number</summary>
///<param name="peid">PE ID</param>
///<param name="isToFile">output flag(true:specified file, false:stdout)</param>
///<returns>none</returns>
void G4_NoDbgIF::dispIcacheAccNum(unsigned int peid,  bool isToFile)
{
  unsigned long long ic_acc_num = 0;
  unsigned long long ic_hit_num = 0;
  unsigned long long ic_miss_num = 0;
  double hit_ratio = 0;
  double miss_ratio = 0;
  
  FILE* fp;

  if(isToFile) fp=fp_memory_summary;
  else         fp=stdout;
  
  if(fp == (FILE*)0) return;

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    if(isToFile){ // output to file
      if(peid == 0){
	fprintf(fp, " Cache Summary\n");
	fprintf(fp, "===============\n");
      }
    }
    ic_acc_num  = getIcacheAccNum( peid );
    ic_hit_num  = getIcacheHitNum( peid );
    ic_miss_num = getIcacheMissNum( peid );
    hit_ratio   = (double)(ic_hit_num * 100) / (double)(ic_acc_num);
    miss_ratio  = (double)(ic_miss_num * 100) / (double)(ic_acc_num);
    fprintf(fp, " PE%d's information\n", peid);
#if defined(_MSC_VER)
    fprintf(fp, "  Total number of I-cache access  : %12I64d\n", ic_acc_num);   
    fprintf(fp, "  Total number of I-cache hit     : %12I64d(%3.2f%%)\n", ic_hit_num, hit_ratio);
    fprintf(fp, "  Total number of I-cache miss hit: %12I64d(%3.2f%%)\n", ic_miss_num, miss_ratio);    
#else
    fprintf(fp, "  Total number of I-cache access  : %12lld\n", ic_acc_num);   
    fprintf(fp, "  Total number of I-cache hit     : %12lld(%3.2f%%)\n", ic_hit_num, hit_ratio);
    fprintf(fp, "  Total number of I-cache miss hit: %12lld(%3.2f%%)\n", ic_miss_num, miss_ratio);    
#endif
    fflush(fp);
  } 
}

///<summary>flush trace file data</summary>
///<returns>none</returns>
void G4_NoDbgIF::flushTraceData()
{
// When the compile version of SC-HEAP is changed to same of cforestg4x 
// and SC-HEAP's behavior will be changed to open the file and set file pointer into cforestg4x, 
// this operation is changed to the effective code.
#if 0
  if (fp_trace) {
    fflush(fp_trace);
  }
#endif
}


///<summary>set clock frequency ratio</summary>
///<param name="peid">PE ID</param>
///<param name="ratio">clock frequency ratio</param>
///<returns>none</returns>
void G4_NoDbgIF::SetClockRatio(unsigned int peid, unsigned int ratio)
{
  mpForestUtil->SetFrequency(peid, ratio);
}

///<summary>set MPCFG.DMDP value</summary>
///<param name="peid">PE ID</param>
///<param name="mode">MPCFG.DMDP value</param>
///<returns>none</returns>
void G4_NoDbgIF::SetDmdpMode(unsigned int peid, bool mode)
{
  mpForestUtil->SetExtPin (peid, EXT_PIN_MDP_DMDP, mode);
}


///<summary>Change cpu type value to cforestG4 type</summary>
///<param name="type">cpu type of SC-HEAP</param>
///<returns>cpu type of CForestG4</returns>
unsigned int G4_NoDbgIF::ChangeCpuTypeVal(unsigned int type)
{
  switch((CPU_KIND_T)type){
  default:         return((unsigned int)CORE_G4MH);
  }
}

///<summary>Set reset status</summary>
///<param name="stat">reset status(tur:under reset, false:not reset)</param>
///<returns>none</returns>
void G4_NoDbgIF::SetResetStatus(bool stat)
{
  mIsUnderReset = stat;
}


///<summary>get the total accessed times of I-Cache</summary>
///<param name="peid">PE ID<param>
///<returns>total accessed times</returns>
unsigned long long G4_NoDbgIF::getIcacheAccNum( unsigned int peid )
{
  unsigned long long acc_num = 0;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    acc_num = mpForestUtil->GetIcacheAccessCount(peid);
  }
  return(acc_num);
}

///<summary>get the hit times of I-Cache</summary>
///<param name="peid">PE ID</param>
///<returns>hit times</returns>
unsigned long long G4_NoDbgIF::getIcacheHitNum( unsigned int peid )
{
  unsigned long long hit_num = 0;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    hit_num = mpForestUtil->GetIcacheHitCount(peid);
  }
  return(hit_num);
}


///<summary>get the miss hit times of I-Cache</summary>
///<param name="peid">PE ID</param>
///<returns>miss hit times</returns>
unsigned long long G4_NoDbgIF::getIcacheMissNum( unsigned int peid )
{
  unsigned long long miss_num = 0;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    miss_num = mpForestUtil->GetIcacheMissCount(peid);
  }
  return(miss_num);
}


///<summary>set PC check function mode for SWDT</summary>
///<param name="peid">PE ID</param>
///<param name="mode">PC check function mode(true:ON, false:OFF)</param>
///<returns>none</returns>
void G4_NoDbgIF::setSwdtEnable( unsigned int peid, bool mode )
{
#ifdef TEST_SWDT_CPU
  printf("%s:G4_NoDbgIF: setSwdtEnable invoked\n",
	 sc_time_stamp().to_string().c_str());
  fflush(stdout);
#endif

  if((mpPeType[peid] == CPU_G4MH_1_0) || (mpPeType[peid] == CPU_G4MH_1_1) || (mpPeType[peid] == CPU_G4MH_2_0)){
    if(peid != 0) return;
  }else{
    // nothing
  }

  if((!mIsSwdtEnable[peid]) && (mode)){ // OFF->ON
    mIsSwdtEnable[peid] = true;
    if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
#ifdef TEST_SWDT_CPU
      printf("%s:G4_NoDbgIF: setSwdtEnable(OFF->ON):set PE%d:SWD0PCAD=0x%08x\n",
	     sc_time_stamp().to_string().c_str(),
	     peid,
	     mSwd0pcadVal[peid]);
      fflush(stdout);
#endif
      mpForestUtil->AddBreakMatchPc(peid, mSwd0pcadVal[peid]);
    }else{ // FastISS
      // ignore
    }
  }else if ((mIsSwdtEnable[peid]) && (!mode)){ // ON->OFF
    mIsSwdtEnable[peid] = false;
    if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
#ifdef TEST_SWDT_CPU
      printf("%s:G4_NoDbgIF: setSwdtEnable(ON->OFF):clear PE%d;SWD0PCAD=0x%08x\n",
	     sc_time_stamp().to_string().c_str(),
	     peid,
	     mSwd0pcadVal[peid]);
      fflush(stdout);
#endif
      mpForestUtil->ClearBreakMatchPc(peid, mSwd0pcadVal[peid]);
    }else{ // FastISS
      // ignore
    }
  }else{ // ON->ON or OFF->OFF
    // ignore
  }
}


///<summary>set SWD0PCAD's value</summary>
///<param name="peid">PE ID</param>
///<param name="value">SWD0PCAD's value</param>
///<returns>none</returns>
void G4_NoDbgIF::SetSwd0pcadValue( unsigned int peid, unsigned int value )
{
#ifdef TEST_SWDT_CPU
  printf("%s:G4_NoDbgIF:SetSwd0pcadValue(PE%d) invoked. value=0x%08x\n",
	 sc_time_stamp().to_string().c_str(),
	 peid,
	 value);
  fflush(stdout);
#endif

  if((mpPeType[peid] == CPU_G4MH_1_0) || (mpPeType[peid] == CPU_G4MH_1_1) || (mpPeType[peid] == CPU_G4MH_2_0)){
    if(peid != 0) return;
  }else{
    // nothing
  }

  if(mIsSwdtEnable[peid]){
    if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
#ifdef TEST_SWDT_CPU
      printf("    call CA:ClearBreakMatchPc(%d, 0x%08x)\n", peid, mSwd0pcadVal[peid]);
      printf("    call CA:AddBreakMatchPc(%d, 0x%08x)\n", peid, value);
      fflush(stdout);
#endif
      mpForestUtil->ClearBreakMatchPc(peid, mSwd0pcadVal[peid]);
      mpForestUtil->AddBreakMatchPc(peid, value);
    }else{ // SIM_MODE_FAST == mIssMode
#ifndef FASTISS_NO_SUPPORT
      if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
#ifdef TEST_SWDT_CPU
	printf("    call FA:SetSwd0pcadValue(0x%08x)\n", value);
	fflush(stdout);
#endif
	mpFastIssIF->SetSwd0pcadValue(peid, value);
      }
#endif // #ifndef FASTISS_NO_SUPPORT
    }
  }

  mSwd0pcadVal[peid] = value;
}


///<summary>set Trace output function mode</summary>
///<param name="mode">Trace output mode(true:ON, false:OFF)</param>
///<returns>none</returns>
void G4_NoDbgIF::setTraceEnable( bool mode )
{
  if(strcmp(mProfileTrace, "") == 0) return;

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    // CAISS
    if((!mIsTraceEnable) && (mode)){
      // OFF->ON
      if((TRC_PC == mProfileTraceFormat) || (TRC_OLD_PC == mProfileTraceFormat)){
        mpForestUtil->SetDebugMode(DBG_ON|DBG_RW);
      }else{
        mpForestUtil->SetDebugMode(DBG_ON|DBG_PIPE);
      }
    }else if((mIsTraceEnable) && (!mode)){
      // ON->OFF
      mpForestUtil->SetDebugMode(DBG_OFF);
    }else{
      // ignore (ON->ON or OFF->OFF)
    }
  }else{ 
#ifndef FASTISS_NO_SUPPORT
    // fastISS
    if((!mIsTraceEnable) && (mode)){ 
      // OFF->ON
      if (TRC_OLD_PC == mProfileTraceFormat) {
	mpFastIssIF->AttachAnalysisUnit(true);
      }else{
	mpFastIssIF->AttachAnalysisUnit(false);
      }
    }else if((mIsTraceEnable) && (!mode)){
      // ON->OFF
      mpFastIssIF->DetachAnalysisUnit();
    }else{
      // ignore (ON->ON or OFF->OFF)
    }
#endif
  }
  mIsTraceEnable = mode;
}



///<summary>clear the fastISS's traslation cache</summary>
///<param name="peid">pe id</param>
///<returns>none</returns>
void G4_NoDbgIF::clearFastissCache( unsigned int peid )
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    // ignore
  }else{ // SIM_MODE_FAST == mIssMode
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      mpFastIssIF->clearFastissCache(peid);
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>Set the read latency of Flash ROM for the External master(not CPU)</summary>
///<param name="latency">read latency of Flash ROM for the External master</param>
///<returns>none</returns>
void G4_NoDbgIF::setExMReadRomLatency( unsigned int latency )
{
  mpG4CPU->mpCaIss->setExMReadRomLatency(latency);
}


///<summary>Set the write latency of Flash ROM for the External master(not CPU)</summary>
///<param name="latency">write latency of Flash ROM for the External master</param>
///<returns>none</returns>
void G4_NoDbgIF::setExMWriteRomLatency( unsigned int latency )
{
  mpG4CPU->mpCaIss->setExMWriteRomLatency(latency);
}


///<summary>Set the read latency of Cluster RAM for the External master(not CPU)</summary>
///<param name="latency">read latency of Cluster RAM for the External master</param>
///<returns>none</returns>
void G4_NoDbgIF::setExMReadCramLatency( unsigned int latency )
{
  mpG4CPU->mpCaIss->setExMReadCramLatency(latency);
}


///<summary>Set the write latency of Cluster RAM for the External master(not CPU)</summary>
///<param name="latency">write latency of Cluster RAM for the External master</param>
///<returns>none</returns>
void G4_NoDbgIF::setExMWriteCramLatency( unsigned int latency )
{
  mpG4CPU->mpCaIss->setExMWriteCramLatency(latency);
}


///<summary>Set the read latency of Local RAM for the External master(not CPU)</summary>
///<param name="latency">read latency of Local RAM for the External master</param>
///<returns>none</returns>
void G4_NoDbgIF::setExMReadLramLatency( unsigned int latency )
{
  mpG4CPU->mpCaIss->setExMReadLramLatency(latency);
}


///<summary>Set the write latency of Local RAM for the External master(not CPU)</summary>
///<param name="latency">write latency of Local RAM for the External master</param>
///<returns>none</returns>
void G4_NoDbgIF::setExMWriteLramLatency( unsigned int latency )
{
  mpG4CPU->mpCaIss->setExMWriteLramLatency(latency);
}


///<summary>Set the read latency of Local APB's IP for the External master(not CPU)</summary>
///<param name="latency">read latency of Local APB's IP for the External master</param>
///<returns>none</returns>
void G4_NoDbgIF::setExMReadLapbLatency( unsigned int latency )
{
  mpG4CPU->mpCaIss->setExMReadLapbLatency(latency);
}


///<summary>Set the write latency of Local APB's IP for the External master(not CPU)</summary>
///<param name="latency">write latncy of Local APB's IP fof the External master</param>
///<returns>none</returns>
void G4_NoDbgIF::setExMWriteLapbLatency( unsigned int latency )
{
  mpG4CPU->mpCaIss->setExMWriteLapbLatency(latency);
}


///<summary>Get the read latency of Flash ROM for the External master(not CPU)</summary>
///<returns>read latncy of Flash ROM for the External master</returns>
unsigned int G4_NoDbgIF::getExMReadRomLatency( void )
{
  return(mpG4CPU->mpCaIss->getExMReadRomLatency());
}


///<summary>Get the write latency of Flash ROM for the External master(not CPU)</summary>
///<returns>write latncy of Flash ROM for the External master</returns>
unsigned int G4_NoDbgIF::getExMWriteRomLatency( void )
{
  return(mpG4CPU->mpCaIss->getExMWriteRomLatency());
}


///<summary>Get the read latency of Cluster RAM for the External master(not CPU)</summary>
///<returns>read latncy of Cluster RAM for the External master</returns>
unsigned int G4_NoDbgIF::getExMReadCramLatency( void )
{
  return(mpG4CPU->mpCaIss->getExMReadCramLatency());
}


///<summary>Get the write latency of Cluster RAM for the External master(not CPU)</summary>
///<returns>write latncy of Cluster RAM for the External master</returns>
unsigned int G4_NoDbgIF::getExMWriteCramLatency( void )
{
  return(mpG4CPU->mpCaIss->getExMWriteCramLatency());
}


///<summary>Get the read latency of Local RAM for the External master(not CPU)</summary>
///<returns>read latncy of Local RAM for the External master</returns>
unsigned int G4_NoDbgIF::getExMReadLramLatency( void )
{
  return(mpG4CPU->mpCaIss->getExMReadLramLatency());
}


///<summary>Get the write latency of Local RAM for the External master(not CPU)</summary>
///<returns>write latncy of Local RAM for the External master</returns>
unsigned int G4_NoDbgIF::getExMWriteLramLatency( void )
{
  return(mpG4CPU->mpCaIss->getExMWriteLramLatency());
}


///<summary>Get the read latency of Local APB's IP for the External master(not CPU)</summary>
///<returns>read latncy of Local APB's IP for the External master</returns>
unsigned int G4_NoDbgIF::getExMReadLapbLatency( void )
{
  return(mpG4CPU->mpCaIss->getExMReadLapbLatency());
}


///<summary>Get the write latency of Local APB's IP for the External master(not CPU)</summary>
///<returns>write latncy of Local APB's IP for the External master</returns>
unsigned int G4_NoDbgIF::getExMWriteLapbLatency( void )
{
  return(mpG4CPU->mpCaIss->getExMWriteLapbLatency());
}

///<summary>read the global register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_NoDbgIF::readGrValue( unsigned int peid, unsigned int regid )
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    //unsigned long long value = 0;
    RegData64 value = 0;
    mpForestUtil->RegReadDebug(peid, regid, &value);
    return((unsigned int)(value & 0xFFFFFFFFUL));
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      return(mpFastIssIF->readGrValue(peid, regid));
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
  return(0);
}


///<summary>write the global register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_NoDbgIF::writeGrValue( unsigned int peid, unsigned int regid, unsigned int value )
{
  if (regid == 0) return;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    mpForestUtil->RegWriteDebug(peid, regid, (unsigned long long)value);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      mpFastIssIF->writeGrValue(peid, regid, value);
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }  
}


///<summary>read the vector register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="isUpper">data position (true:upper 64bit, false:lower64bit)</param>
///<returns>register value</returns>
unsigned long long G4_NoDbgIF::readWrValue( unsigned int peid, unsigned int regid, bool isUpper )
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    //unsigned long long lo_value = 0;
    //unsigned long long up_value = 0;
    RegData64 lo_value = 0;
    RegData64 up_value = 0;
    mpForestUtil->RegReadDebug(peid, regid+0x20, &lo_value, &up_value);
    if(isUpper){
      return(up_value);
    }else{
      return(lo_value);
    } 
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      return(mpFastIssIF->readWrValue(peid, regid, isUpper));
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
  return(0);
}


///<summary>write the vector register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="up_value">regester value (upper 64bit)</param>
///<param name="lo_value">regester value (lower 64bit)</param>
///<returns>none</returns>
void G4_NoDbgIF::writeWrValue( unsigned int peid, unsigned int regid, unsigned long long up_value, unsigned long long lo_value )
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    mpForestUtil->RegWriteDebug(peid, regid+0x20, lo_value, up_value);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      mpFastIssIF->writeWrValue(peid, regid, up_value, lo_value);
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }  
}


///<summary>read the system register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="selid">selecter ID (0-13)</param>
///<returns>register value</returns>
unsigned int G4_NoDbgIF::readSrValue( unsigned int peid, unsigned int regid, unsigned int selid )
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    //unsigned long long value = 0;
    RegData64 value = 0;
    // 2nd arg of RegReadDebug() is calculated by "256(0x100) + (selID << 5) + regID"
    mpForestUtil->RegReadDebug(peid, 0x100+(selid << 5)+regid, &value);
    return((unsigned int)(value & 0xFFFFFFFFUL));
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      return(mpFastIssIF->readSrValue(peid, regid, selid));
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }  
  return(0);
}


///<summary>write the system register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="selid">selecter ID (0-13)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_NoDbgIF::writeSrValue( unsigned int peid, unsigned int regid, unsigned int selid, unsigned int value )
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    // 2nd arg of RegReadDebug() is calculated by "256(0x100) + (selID << 5) + regID"
    mpForestUtil->RegWriteDebug(peid, 0x100+(selid << 5)+regid, (unsigned long long)value);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      mpFastIssIF->writeSrValue(peid, regid, selid, value);
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}
