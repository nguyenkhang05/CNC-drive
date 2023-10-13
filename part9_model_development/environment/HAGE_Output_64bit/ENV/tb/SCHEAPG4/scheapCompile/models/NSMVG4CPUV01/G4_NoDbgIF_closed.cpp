/*************************************************************************
 *
 *  G4_NoDbgIF_closed.cpp
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

#include "G4_NoDbgIF.h" 
#include "G4_CaIss.h" 
#include "NSMVG4CPUV01.h"
#include "forest_utility_scheap.h"
#include "sideband.h"

#include "CmErrMsg.h" // for WinMsg

using namespace CmErrMsg; // for WinMsg

///<summary>create ForestUtilScheap instance</summary>
///<returns>none</returns>
void G4_NoDbgIF::CreateForestUtil(void)
{
  mpForestUtilFull = new ForestUtilScheap();
  mpForestUtil = mpForestUtilFull->GetForestUtilApi();
}


///<summary>set the pointer of ForestUtilScheap instance to CAISS</summary>
///<returns>none</returns>
void G4_NoDbgIF::SetForestUtilPtr( void )
{
  mpG4CPU->mpCaIss->SetForestUtilScheapPtr(mpForestUtilFull);
}


///<summary>check the configuration value whther correect or not</summary>
///<param name="token">configuration value</param>
///<returns>correct or not</returns>
bool G4_NoDbgIF::CheckMsgLevel ( char* token )
{
  bool isCorrectVal = false;
  if(strcmp(token, "LEVEL4") == 0){
    mProfileMsgLevel = MSG_INF;
    isCorrectVal = true;
  }
  return isCorrectVal;
}

///<summary>set Gram Write Through Buffer's parameter of CAISS</summary>
///<returns>none</returns>
void G4_NoDbgIF::SetGramWtBufInfo( void )
{
#if 0 // not support for current G4MH model
  if(mIsGramWtbufEnable){
    mpForestUtilFull->SetExtPin(EXT_PIN_GRWTB_EN, 1);
  }else{
    mpForestUtilFull->SetExtPin(EXT_PIN_GRWTB_EN, 0);
  }
  mpForestUtilFull->SetExtPin(EXT_PIN_GRWTB_INVEN, 1);
  mpForestUtilFull->SetExtPin(EXT_PIN_GRWTB_INVPERI, 3);
#endif
}


///<summary>set the code flash guard's parameter</summary>
///<returns>none</returns>
void  G4_NoDbgIF::SetCfgInfo ( void )
{
#if 0 // not support for current G4MH model
  if(mpPeType[0] == CORE_CCG3M30){
    mpForestUtilFull->SetRomArea(0x00000000, 0x0fffffff);
    mpForestUtilFull->SetCfgProtEnable(mIsCfgEnable);
    mpForestUtilFull->SetCfgProtRegion((PhysAddr)mCfgProtRegion);
    if(mIsCfgIllAccTest){
      mpG4CPU->mpCaIss->SetFromArea(0x00000000, 0x02ffffff);
    }else{
      mpG4CPU->mpCaIss->SetFromArea(0x00000000, 0x0fffffff);
    }
  }
#endif
}


///<summary>set the Cluster ram guard's parameter</summary>
///<returns>none</returns>
void G4_NoDbgIF::SetCrgInfo ( void )
{
  mpForestUtilFull->SetCramArea(CRAM_INIT_BASE, (CRAM_INIT_BASE + CRAM_MAX_SIZE - 1));
  mpG4CPU->mpCaIss->SetCramArea(CRAM_INIT_BASE, (CRAM_INIT_BASE + CRAM_MAX_SIZE - 1));

  // The value of base_address is for E2xFCC1, E2xFCC2, and U2A's CRG base address.
  // This value is written in E2xFCC1's UM (1.0), E2xFCC2's UM (0.5), and U2A eva1 spec (rev0.9).
  // In future, we need to re-consider how to set the base address for the various products 
  // (relation ticket is Requirement #100411). 
  unsigned int base_addr = CRG_INIT_BASE;
  for (unsigned int i=0; i<mPeNum; i++){
    if((mpPeType[i] == CPU_G4MH_1_0) || (mpPeType[i] == CPU_G4MH_1_1)){
      mpForestUtilFull->SetGuard(0x4, i,    base_addr+(0x800+(0x10*i)), true); // for CRGCAP_PEnCLm
    }else{ //CPU_G4MH_2_0
      mpForestUtilFull->SetGuard(0x4, i,    base_addr+(0x800+(0x20*i)), true); // for CRGCAP_PEnCLm
    }
  }
  for (unsigned int i=0; i<CL_MAX_NUM; i++){
    if((mpPeType[i] == CPU_G4MH_1_0) || (mpPeType[i] == CPU_G4MH_1_1)){
      mpForestUtilFull->SetGuard(0x3, i, base_addr+(0x100*i),        true); // for GUARD_CRAMCRGm(CRG_CLm)
      mpForestUtilFull->SetGuard(0x5, i, base_addr+(0x900+(0x40*i)), true); // for CRGCAP_CRAMHCLm
      mpForestUtilFull->SetGuard(0x6, i, base_addr+(0x910+(0x40*i)), true); // for CRGCAP_CRAMLCLm
      mpForestUtilFull->SetGuard(0x7, i, base_addr+(0x920+(0x40*i)), true); // for CRGCAP_SX2MBHCLm
      mpForestUtilFull->SetGuard(0x8, i, base_addr+(0x930+(0x40*i)), true); // for CRGCAP_SX2MBLCLm
    }else{ // CPU_G4MH_2_0
      mpForestUtilFull->SetGuard(0x3, i, base_addr+(0x200*i),        true); // for GUARD_CRAMCRGm0 (CRG_CLm)
      mpForestUtilFull->SetGuard(0x5, i, base_addr+(0xA00+(0x80*i)), true); // for CRGCAP_CRAMHCLm
      mpForestUtilFull->SetGuard(0x6, i, base_addr+(0xA20+(0x80*i)), true); // for CRGCAP_CRAMLCLm
      mpForestUtilFull->SetGuard(0x7, i, base_addr+(0xA40+(0x80*i)), true); // for CRGCAP_SX2MBHCLm
      mpForestUtilFull->SetGuard(0x8, i, base_addr+(0xA60+(0x80*i)), true); // for CRGCAP_SX2MBLCLm
      mpForestUtilFull->SetGuard(0xe, i, base_addr+(0x1800+(0x200*i)), true); // for GUARD_CRAMCRGm1(CRG_IVC_CLm)
      mpForestUtilFull->SetGuard(0xf, i, base_addr+(0x1000+(0x200*i)), true); // for GUARD_CRSECCLm(CRG_CSG_CLm)

      mpForestUtilFull->SetGuardParam(0x3, i, 0x0, mpCrgInfo[i].cramif_width);
    }
  }
}

///<summary>set the PE guard's parameter</summary>
///<returns>none</returns>
void G4_NoDbgIF::SetPegInfo ( void )
{
  // The value of base_address is for E2xFCC1 and 2's, U2A's PEG base address.
  // This value is written in E2xFCC1's UM (1.0), E2xFCC2's UM (0.5), and U2A eva1 spec (rev0.9).
  // In future, we need to re-consider how to set the base address for the various products 
  // (relation ticket is Requirement #100411).
  unsigned int base_addr = PEG_INIT_BASE;
  for (unsigned int i=0; i<mPeNum; i++){
    if((mpPeType[i] == CPU_G4MH_1_0) || (mpPeType[i] == CPU_G4MH_1_1)){
      mpForestUtilFull->SetGuard(0x0, i, base_addr+(0x100*i),        true); // for PEG_PEnCLm
      mpForestUtilFull->SetGuard(0x1, i, base_addr+(0x800+(0x20*i)), true); // for PEGCAP_M_PEnClm
      mpForestUtilFull->SetGuard(0x2, i, base_addr+(0x810+(0x20*i)), true); // for PEGCAP_S_PEnCLm
    }else{ // CPU_G4MH_2_0
      mpForestUtilFull->SetGuard(0x0, i, base_addr+(0x100*i),        true); // for PEG_PEnCLm
      mpForestUtilFull->SetGuard(0x1, i, base_addr+(0x800+(0x40*i)), true); // for PEGCAP_M_PEnClm
      mpForestUtilFull->SetGuard(0x2, i, base_addr+(0x820+(0x40*i)), true); // for PEGCAP_S_PEnCLm
    }
  }
}


///<summary>change the guard error value from CForestG3M's value to G3MCPU's value</summary>
///<param name="grd_err_t">CForestG3M's error value</param>
///<returns>G3MCPU's error value</returns>
unsigned int G4_NoDbgIF::GetGrdSrc(GuardErrType grd_err_t)
{
  switch(grd_err_t){
  case GD_CFG_ERR:    return((unsigned int)NSMVG4CPUV01::GRD_ERR_CF_GRD);
  case GD_CRG_ERR:    return((unsigned int)NSMVG4CPUV01::GRD_ERR_CRAM_GRD);
  case GD_CFG_ILLACC: return((unsigned int)NSMVG4CPUV01::GRD_ERR_CF_ILL_ACC_PE);
  case GD_CRG_ILLACC: return((unsigned int)NSMVG4CPUV01::GRD_ERR_CRAM_ILL_ACC_PE);
  case GD_PEG_ERR_M:  return((unsigned int)NSMVG4CPUV01::GRD_ERR_PE_GRD_M);
  case GD_PEG_ERR_S:  return((unsigned int)NSMVG4CPUV01::GRD_ERR_PE_GRD_S);
  default:            return((unsigned int)NSMVG4CPUV01::GRD_ERR_NONE);
  }
}


///<summary>set memory map of CAISS</summary>
///<returns>none</returns>
void G4_NoDbgIF::SetMemMap( void )
{
  unsigned int peid;

  // make memory and set latency
  // FlashROM area
  for(unsigned int j=0; j<mClstrNum; j++){
    for(unsigned int i=0; i<mRomInfoQue[j].size(); i++){
      mpForestUtilFull->MemResourceAlloc (MRT_READ_ENABLE | MRT_FETCH_ENABLE,
					  mRomInfoQue[j][i]->f_latency,
					  mRomInfoQue[j][i]->r_latency,
					  mRomInfoQue[j][i]->w_latency,
					  mRomInfoQue[j][i]->cl_f_latency,
					  mRomInfoQue[j][i]->cl_r_latency,
					  mRomInfoQue[j][i]->cl_w_latency,
					  mRomInfoQue[j][i]->ex_f_latency,
					  mRomInfoQue[j][i]->ex_r_latency,
					  mRomInfoQue[j][i]->ex_w_latency,
					  mRomInfoQue[j][i]->base,
					  (mRomInfoQue[j][i]->base + mRomInfoQue[j][i]->size - 0x00000001),
					  MI_CF_FROM,
					  0,
					  MEM_LOCATE_GLOBAL,
					  static_cast<MemLocationType> (j),
					  mRomInfoQue[j][i]->outstand_latency,
					  mRomInfoQue[j][i]->repeat_latency,
					  mRomInfoQue[j][i]->bank_size);
#ifdef TEST_CORE_DBG
      printf("CL%d:FROM%d:0x%08x - 0x%08x\n", j, i, mRomInfoQue[j][i]->base, (mRomInfoQue[j][i]->base + mRomInfoQue[j][i]->size - 0x00000001));
      printf("      f(own)=%d, r(own)=%d, w(own)=%d\n",  mRomInfoQue[j][i]->f_latency, mRomInfoQue[j][i]->r_latency, mRomInfoQue[j][i]->w_latency);
      printf("      f(cl)=%d,  r(cl)=%d,  w(cl)=%d\n",  mRomInfoQue[j][i]->cl_f_latency, mRomInfoQue[j][i]->cl_r_latency, mRomInfoQue[j][i]->cl_w_latency);
      printf("      f(ex)=%d,  r(ex)=%d,  w(ex)=%d\n",  mRomInfoQue[j][i]->ex_f_latency, mRomInfoQue[j][i]->ex_r_latency, mRomInfoQue[j][i]->ex_w_latency);
      printf("      outstand=%d, repeat=%d\n",  mRomInfoQue[j][i]->outstand_latency, mRomInfoQue[j][i]->repeat_latency);
      fflush(stdout);
#endif // #ifdef TEST_CORE_DBG
    }
  }


  // LocalRAM area
  for(unsigned int i=0; i<mPeNum; i++){
    for(unsigned int j=0; j<mLRamInfoQue[i].size(); j++){
      peid = i;
      mpForestUtilFull->MemResourceAlloc (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE,
					  mLRamInfoQue[i][j]->f_latency,
					  mLRamInfoQue[i][j]->r_latency,
					  mLRamInfoQue[i][j]->w_latency,
					  mLRamInfoQue[i][j]->cl_f_latency,
					  mLRamInfoQue[i][j]->cl_r_latency,
					  mLRamInfoQue[i][j]->cl_w_latency,
					  mLRamInfoQue[i][j]->ex_f_latency,
					  mLRamInfoQue[i][j]->ex_r_latency,
					  mLRamInfoQue[i][j]->ex_w_latency,
					  mLRamInfoQue[i][j]->base,
					  (mLRamInfoQue[i][j]->base + mLRamInfoQue[i][j]->size - 0x00000001),
					  MI_CF_LRAM,
					  0,
					  peid,
					  static_cast<MemLocationType> (peid/2),
					  mLRamInfoQue[i][j]->outstand_latency,
					  mLRamInfoQue[i][j]->repeat_latency,
					  mLRamInfoQue[i][j]->bank_size);

#ifdef TEST_CORE_DBG
      printf("PE%dLRAM%d:0x%08x - 0x%08x\n", peid, j, mLRamInfoQue[i][j]->base, (mLRamInfoQue[i][j]->base + mLRamInfoQue[i][j]->size - 0x00000001));
      printf("        f(own)=%d, r(own)=%d, w(own)=%d\n", mLRamInfoQue[i][j]->f_latency, mLRamInfoQue[i][j]->r_latency, mLRamInfoQue[i][j]->w_latency);
      printf("        f(cl)=%d,  r(cl)=%d,  w(cl)=%d\n",  mLRamInfoQue[i][j]->cl_f_latency, mLRamInfoQue[i][j]->cl_r_latency, mLRamInfoQue[i][j]->cl_w_latency);
      printf("        f(ex)=%d,  r(ex)=%d,  w(ex)=%d\n", mLRamInfoQue[i][j]->ex_f_latency, mLRamInfoQue[i][j]->ex_r_latency, mLRamInfoQue[i][j]->ex_w_latency);
      printf("        outstand=%d, repeat=%d\n", mLRamInfoQue[i][j]->outstand_latency, mLRamInfoQue[i][j]->repeat_latency);
      fflush(stdout);
#endif // #ifdef TEST_CORE_DBG
    }
  }
    
  // Cluster RAM
  for(unsigned int j=0; j<CL_MAX_NUM; j++){
    for(unsigned int i=0; i<mCRamInfoQue[j].size(); i++){
      mpForestUtilFull->MemResourceAlloc (MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE,
					  mCRamInfoQue[j][i]->f_latency,
					  mCRamInfoQue[j][i]->r_latency,
					  mCRamInfoQue[j][i]->w_latency,
					  mCRamInfoQue[j][i]->cl_f_latency,
					  mCRamInfoQue[j][i]->cl_r_latency,
					  mCRamInfoQue[j][i]->cl_w_latency,
					  mCRamInfoQue[j][i]->ex_f_latency,
					  mCRamInfoQue[j][i]->ex_r_latency,
					  mCRamInfoQue[j][i]->ex_w_latency,
					  mCRamInfoQue[j][i]->base,
					  (mCRamInfoQue[j][i]->base + mCRamInfoQue[j][i]->size - 0x00000001),
					  MI_CF_CRAM,
					  0,
					  MEM_LOCATE_GLOBAL,
					  static_cast<MemLocationType> (j),
					  mCRamInfoQue[j][i]->outstand_latency,
					  mCRamInfoQue[j][i]->repeat_latency,
					  mCRamInfoQue[j][i]->bank_size);
      
#ifdef TEST_CORE_DBG
      printf("CL%d:CRAM%d:0x%08x - 0x%08x\n", j, i, mCRamInfoQue[j][i]->base, (mCRamInfoQue[j][i]->base + mCRamInfoQue[j][i]->size - 0x00000001));
      printf("      f(own)=%d, r(own)=%d, w(own)=%d\n", mCRamInfoQue[j][i]->f_latency, mCRamInfoQue[j][i]->r_latency, mCRamInfoQue[j][i]->w_latency);
      printf("      f(cl)=%d,  r(cl)=%d,  w(cl)=%d\n",  mCRamInfoQue[j][i]->cl_f_latency, mCRamInfoQue[j][i]->cl_r_latency, mCRamInfoQue[j][i]->cl_w_latency);
      printf("      f(ex)=%d,  r(ex)=%d,  w(ex)=%d\n", mCRamInfoQue[j][i]->ex_f_latency, mCRamInfoQue[j][i]->ex_r_latency, mCRamInfoQue[j][i]->ex_w_latency);
      printf("      outstand=%d, repeat=%d\n", mCRamInfoQue[j][i]->outstand_latency, mCRamInfoQue[j][i]->repeat_latency);
      fflush(stdout);
#endif // #ifdef TEST_CORE_DBG
    }
  }

  // LocalRAM
  mpForestUtil->SetSelfArea(LRAM_INIT_BASE, (LRAM_INIT_BASE + LRAM_INIT_SIZE - 1), -LRAM_INIT_SIZE);

  // LocalAPBは常に有効
  // LocalAPB
  // Body @ PE1 : 0xFFFC4000 - 0xFFFC7FFF (self + (0x4000) * peid+1)
  // SELF       : 0xFFFC0000 - 0xFFFC3FFF
  mpForestUtil->SetSelfArea (LRSRC_INIT_BASE, (LRSRC_INIT_BASE + LRSRC_INIT_SIZE - 1), LRSRC_INIT_SIZE);

  // set LRAM full area ( for unimplement )
  for(unsigned int i=0; i<PE_MAX_NUM; i++){
    unsigned int e_addr = LRAM_INIT_BASE - (LRAM_INIT_SIZE*i) - 1;
    unsigned int s_addr = e_addr - LRAM_INIT_SIZE + 1;
    mpForestUtil->SetPeLramArea(i, s_addr, e_addr);
  }
}


///<summary>get spid, when error occures</summary>
///<param name="pe_id">PE ID</param>
///<returns>SPID</returns>
unsigned char G4_NoDbgIF::GetSpIdAtGrdErr( unsigned int pe_id )
{
  unsigned char spid_val = 0;
  unsigned long long data_l = 0;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    // 2nd arg of RegReadDebug() is calculated by "256 + (selID << 5) + regID"
    mpForestUtilFull->RegReadDebug(pe_id, 288, (RegData64*)(&data_l)); 
    spid_val = (unsigned char)(data_l & 0x0000001FULL);
  }
  return(spid_val);
}


///<summary>get pswh, when CycleExecute breaks by BRK_CHANGE_PSWH</summary>
///<param name="pe_id">PE ID</param>
///<returns>PSWH</returns>
unsigned int G4_NoDbgIF::GetPswh( unsigned int pe_id )
{
  unsigned int pswh_val = 0;
  unsigned long long data_l = 0;

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    // 2nd arg of RegReadDebug() is calculated by "256 + (selID << 5) + regID"
    mpForestUtilFull->RegReadDebug(pe_id, (256 + (0 << 5) + 15), (RegData64*)(&data_l)); 
    pswh_val = (unsigned int)(data_l & 0xFFFFFFFFULL);
  }
  return(pswh_val);
}


///<summary>get access event, when error occures</summary>
///<param name="pe_id">PE ID</param>
///<param name="grd_id">Guard module's PE ID</param>
///<returns>none</returns>
void G4_NoDbgIF::GetAccTypeAtGrdErr(unsigned int pe_id, unsigned int& grd_id)
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    unsigned int scid = 0;
    scid = mpForestUtilFull->GetScId (pe_id);
    const TraceInfo* trace_info = mpForestUtilFull->GetTraceInfo(scid);
    if(trace_info->wmem_count > 0){
      grd_id = mpG4CPU->mpCaIss->GetResourcePeFromAddress(trace_info->wmem_addr[trace_info->wmem_count-1], 
							   trace_info->wmem_size[trace_info->wmem_count-1]);
    }else if(trace_info->rmem_count > 0){
      grd_id = mpG4CPU->mpCaIss->GetResourcePeFromAddress(trace_info->rmem_addr[trace_info->rmem_count-1], 
							   trace_info->rmem_size[trace_info->rmem_count-1]);
    }else if(trace_info->fetch_count > 0){     
      grd_id = mpG4CPU->mpCaIss->GetResourcePeFromAddress(trace_info->fetch_addr[trace_info->fetch_count-1], 
							   16);
    }else{
      // no pe guard error
    }
  }
}

///<summary>set Syserr output request</summary>
///<param name="peid">pe id</param>
///<param name="code">error code</param>
///<returns>none</returns>
void G4_NoDbgIF::setSyserrOut( unsigned int peid, unsigned int code )
{
#ifdef TEST_CORR_DBG
  printf("%s:G4_NoDbgIF:setSyserrOut  invoked\n",
	 sc_time_stamp().to_string().c_str());
  fflush(stdout);
#endif

  if(peid >= mPeNum) return;

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    mpForestUtilFull->ReqPseudoFetchSyserrToCpu(peid, (code & 0x1f));
  }else{ // FastISS
    // ignore
  }
}


///<summary>set Syserr (Operand factor) output request</summary>
///<param name="peid">pe id</param>
///<param name="code">error code</param>
///<returns>none</returns>
void G4_NoDbgIF::setSyserrOperandOut( unsigned int peid, unsigned int code )
{
#ifdef TEST_CORR_DBG
  printf("%s:G4_NoDbgIF:setSyserrOperandOut  invoked\n",
	 sc_time_stamp().to_string().c_str());
  fflush(stdout);
#endif
  bool isValidFactor = false;

  if(peid >= mPeNum) return;

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    if(mpPeType[peid] == CPU_G4MH_2_0){
      if ((code == 0x18) || (code == 0x19) || (code == 0x1a)){
	mpForestUtilFull->NoticeOperandErrorResponse(peid, code);
	isValidFactor = true;
      }
    }
    if(!isValidFactor){
      char error_mod_buf[1024];
      char error_message_buf[1024];
      sprintf(error_mod_buf, "%s(%s)", mpG4CPU->name(), "G4_NoDbgIF");
      // for WinMsg
      sprintf(error_message_buf,
	      "Unexpected syserr factor was specified [0x%2x] by setSyserrOperandOut().",
	      code);
      printWarningMessage(error_mod_buf, "setSyserrOperandOut", error_message_buf);     
    }
  }else{ // FastISS
    // ignore
  }
}


///<summary>check setting all PE's type </summary>
///<returns>none</returns>
void G4_NoDbgIF::CheckPeType( void )
{
#if 0 // not support for current G4MH model
  bool is5bitSpidType = false;
  bool is2bitSpidType = false;
  for(unsigned int i=0; i<mPeNum; i++){
    if( (mpPeType[i] == CORE_CCG3M30) || (mpPeType[i] == CORE_G3MH) ){
      is5bitSpidType = true;
    }else{
      is2bitSpidType = true;
    }
  }
  if((is5bitSpidType) && (is2bitSpidType)){
    char error_message_buf[1024];
    char error_mod_buf[1024];
    sprintf(error_mod_buf, "%s(%s)", mpG4CPU->name(), "G4_NoDbgIF");
    sprintf(error_message_buf,
	    "This combination of PE types isn't supported, because the spid types are different.\n");
    printErrorMessage(error_mod_buf, "CheckPeType", error_message_buf);
    exit(1);
  }
#endif
}

///<summary>set the file pointer of ececution trace file to CForestG3M</summary>
///<param name="fname">file name of ececution trace file</param>
///<returns>none</returns>
// When the compile version of SC-HEAP is changed to same of cforestg4x, this function is deleted"
void G4_NoDbgIF::SetExTrcFp ( char* fname )
{
  if((SIM_MODE_FAST == mpG4CPU->GetSimMode()) || (strcmp(fname, "") == 0)){
    mpForestUtilFull->SetDebugMode(DBG_OFF);
  }else{
    mpForestUtilFull->SetFnameAndOpen(fname);
  }
}


unsigned long long G4_NoDbgIF::getCurrentPc( unsigned int peid )
{
  unsigned long long pc_addr = 0;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    unsigned int scid = 0;
    scid = mpForestUtilFull->GetScId (peid);
    const TraceInfo* trace_info = mpForestUtilFull->GetTraceInfo(scid);
    pc_addr = (unsigned long long)trace_info->inst_addr;
  }
  return(pc_addr);
}

unsigned int G4_NoDbgIF::getMemAccCntCurrentPc(unsigned int peid, bool isWrite )
{
  unsigned int acc_cnt = 0;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    unsigned int scid = 0;
    scid = mpForestUtilFull->GetScId (peid);
    const TraceInfo* trace_info = mpForestUtilFull->GetTraceInfo(scid);
    if(isWrite) acc_cnt = trace_info->wmem_count;
    else        acc_cnt = trace_info->rmem_count;
  }
  return(acc_cnt);
}

void G4_NoDbgIF::getMemAccDataCurrentPc( unsigned int peid, unsigned int acc_cnt, unsigned long long *data_l, unsigned long long *data_u, unsigned int *data_length, bool isWrite )
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    unsigned int scid = 0;
    scid = mpForestUtilFull->GetScId (peid);
    const TraceInfo* trace_info = mpForestUtilFull->GetTraceInfo(scid);
    if(isWrite){
      if(acc_cnt < (unsigned int)trace_info->wmem_count){
	*data_l = trace_info->wmem_value_l64[acc_cnt];
	*data_u = trace_info->wmem_value_u64[acc_cnt];
	*data_length = trace_info->wmem_size[acc_cnt];
      }
    }else{
      if(acc_cnt < (unsigned int)trace_info->rmem_count){
	*data_l = trace_info->rmem_value_l64[acc_cnt];
	*data_u = trace_info->rmem_value_u64[acc_cnt];
	*data_length = trace_info->rmem_size[acc_cnt];
      }
    }
  }
}

///<summary>Cancel Eiint/BGEIINT/GMEIINT's request for CAISS</summary>
///<param name="pe_id">PE ID</param>
/////<returns>None</returns>
void G4_NoDbgIF::CancelInterrupt( unsigned int pe_id )
{
#ifdef TEST_INTC1_DBG
  printf("G4_NoDbgIF::CancelInterrupt request to CA(pe%d)\n", pe_id);
  fflush(stdout);
#endif
  mpForestUtilFull->CancelInterruptToCpu(pe_id);
}

