/*************************************************************************
 *
 *  G4_FastIssIF.cpp
 *
 * Copyright(c) 2015-2020 Renesas Electronics Corporation
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

#include "G4_FastIssIF.h"
#include "CmErrMsg.h" // for WinMsg

using namespace CmErrMsg; // for WinMsg
using namespace rh850g4;    // for ASTC's FastISS

///<summary>constructor</summary>
///<param name="peNum">valid PE number</param>
///<param name="fp_trace">file pointer of trace file</param>
///<returns>none</returns>
G4_FastIssIF::G4_FastIssIF( unsigned int peNum, FILE* fp_trace )
  : mPeNum(peNum),
    mFpTrace(fp_trace)
{
  for(unsigned int i=0; i<mPeNum; i++){
    mPeType[i]               = CPU_G4MH_1_0;
    mPeInfo[i].peid_ival     = i;
    mPeInfo[i].bmid_ival     = i;
    mPeInfo[i].spid_ival     = i;
    mPeInfo[i].gmspid_ival     = i;
    mPeInfo[i].spidlist_ival = 0xffffffff;
    mPeInfo[i].rbase_ival    = 0x0;
    mPeInfo[i].snzcfg_ival   = 0;
    mPeInfo[i].hvcfg_ival   = 0;
    ex_trace[i] = (rh850g4::execution_tracer*)0;
  }
  if(mFpTrace){
    SetTrcTitle();
  }
}


///<summary>destructor</summary>
///<returns>none</returns>
G4_FastIssIF::~G4_FastIssIF( void )
{
  for(unsigned int i=0; i<mPeNum; i++){
    if (ex_trace[i] != (rh850g4::execution_tracer*)0){
      delete ex_trace[i];
    }
  }
}


///<summary>cancel continue event to FastISS engine</summary>
///<param name="pe_id">PE ID</param>
///<returns>none</returns>
void G4_FastIssIF::CancelContEvent( unsigned int pe_id )
{
  if (pe_id < mPeNum){
    mpFastIss_ASTC[pe_id]->continue_event.cancel();
  }
}


///<summary>set continue event to FastISS engine</summary>
///<param name="pe_id">PE ID</param>
///<returns>none</returns>
void G4_FastIssIF::SetContEvent( unsigned int pe_id )
{
  if (pe_id < mPeNum){
    mpFastIss_ASTC[pe_id]->continue_event.notify(sc_core::SC_ZERO_TIME);
  }
}


///<summary>for Test : Output each register's value</summary>
///<returns>none</returns>
void G4_FastIssIF::TestResetLog( void )
{
  for(unsigned int i=0; i<mPeNum; i++){
    printf("%s ReaetLog:FastISS:PE%d\n", sc_time_stamp().to_string().c_str(), i);
    printf("  <<PC value>>\n");
    printf("    PC=0x%08x\n", mpFastIss_ASTC[i]->read_PC());
    printf("  <<Grobal regsiter>>\n");
    printf("     r0=0x%08x,  r1=0x%08x,  r2=0x%08x,  r3=0x%08x,  r4=0x%08x,  r5=0x%08x,  r6=0x%08x,  r7=0x%08x\n", 
	   mpFastIss_ASTC[i]->read_r0(),
	   mpFastIss_ASTC[i]->read_r1(),
	   mpFastIss_ASTC[i]->read_r2(),
	   mpFastIss_ASTC[i]->read_r3(),
	   mpFastIss_ASTC[i]->read_r4(),
	   mpFastIss_ASTC[i]->read_r5(),
	   mpFastIss_ASTC[i]->read_r6(),
	   mpFastIss_ASTC[i]->read_r7());
    printf("     r8=0x%08x,  r9=0x%08x, r10=0x%08x, r11=0x%08x, r12=0x%08x, r13=0x%08x, r14=0x%08x, r15=0x%08x\n", 
	   mpFastIss_ASTC[i]->read_r8(),
	   mpFastIss_ASTC[i]->read_r9(),
	   mpFastIss_ASTC[i]->read_r10(),
	   mpFastIss_ASTC[i]->read_r11(),
	   mpFastIss_ASTC[i]->read_r12(),
	   mpFastIss_ASTC[i]->read_r13(),
	   mpFastIss_ASTC[i]->read_r14(),
	   mpFastIss_ASTC[i]->read_r15());
    printf("    r16=0x%08x, r17=0x%08x, r18=0x%08x, r19=0x%08x, r20=0x%08x, r21=0x%08x, r22=0x%08x, r23=0x%08x\n", 
	   mpFastIss_ASTC[i]->read_r16(),
	   mpFastIss_ASTC[i]->read_r17(),
	   mpFastIss_ASTC[i]->read_r18(),
	   mpFastIss_ASTC[i]->read_r19(),
	   mpFastIss_ASTC[i]->read_r20(),
	   mpFastIss_ASTC[i]->read_r21(),
	   mpFastIss_ASTC[i]->read_r22(),
	   mpFastIss_ASTC[i]->read_r23());
    printf("    r24=0x%08x, r25=0x%08x, r26=0x%08x, r27=0x%08x, r28=0x%08x, r29=0x%08x, r30=0x%08x, r31=0x%08x\n", 
	   mpFastIss_ASTC[i]->read_r24(),
	   mpFastIss_ASTC[i]->read_r25(),
	   mpFastIss_ASTC[i]->read_r26(),
	   mpFastIss_ASTC[i]->read_r27(),
	   mpFastIss_ASTC[i]->read_r28(),
	   mpFastIss_ASTC[i]->read_r29(),
	   mpFastIss_ASTC[i]->read_r30(),
	   mpFastIss_ASTC[i]->read_r31());

    printf("  <<System regsiter : selID=0>>\n");
    printf("        EIPC=0x%08x,    EIPSW=0x%08x,     FEPC=0x%08x,    FEPSW=0x%08x,      sr4=%s,      PSW=0x%08x,     FPSR=0x%08x,    FPEPC=0x%08x\n", 
	   mpFastIss_ASTC[i]->read_EIPC(),
 	   mpFastIss_ASTC[i]->read_EIPSW(),
	   mpFastIss_ASTC[i]->read_FEPC(),
	   mpFastIss_ASTC[i]->read_FEPSW(),
	   "----------",
	   mpFastIss_ASTC[i]->read_PSW(),
	   mpFastIss_ASTC[i]->read_FPSR(),
	   mpFastIss_ASTC[i]->read_FPEPC());
    printf("        FPST=0x%08x,     FPCC=0x%08x,    FPCFG=0x%08x,     sr11=%s,     sr12=%s,     EIIC=0x%08x,     FEIC=0x%08x,     sr15=%s\n", 
	   mpFastIss_ASTC[i]->read_FPST(),
	   mpFastIss_ASTC[i]->read_FPCC(),
	   mpFastIss_ASTC[i]->read_FPCFG(),
	   "----------",
	   "----------",
	   mpFastIss_ASTC[i]->read_EIIC(),
	   mpFastIss_ASTC[i]->read_FEIC(),
	   "----------");
    printf("        CTPC=0x%08x,    CTPSW=0x%08x,     sr18=%s,     sr19=%s,     CTBP=0x%08x,   SNZCFG=0x%08x,     sr22=%s,     sr23=%s\n", 
	   mpFastIss_ASTC[i]->read_CTPC(),
	   mpFastIss_ASTC[i]->read_CTPSW(),
	   "----------",
	   "----------",
	   mpFastIss_ASTC[i]->read_CTBP(),
	   mpFastIss_ASTC[i]->read_SNZCFG(),
	   "----------",
	   "----------");
    printf("        sr24=%s,     sr25=%s,     sr26=%s,     sr27=%s,     EIWR=0x%08x,     FEWR=0x%08x,     sr30=%s,     sr31=%s\n", 
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   mpFastIss_ASTC[i]->read_EIWR(),
	   mpFastIss_ASTC[i]->read_FEWR(),
	   "----------",
	   "----------");

    printf("  <<System regsiter : selID=1>>\n");
    printf("        SPID=0x%08x, SPIDLIST=0x%08x,    RBASE=0x%08x,    EBASE=0x%08x,    INTBP=0x%08x,     MCTL=0x%08x,      PID=0x%08x,      sr7=%s\n", 
	   mpFastIss_ASTC[i]->read_SPID(),
	   mpFastIss_ASTC[i]->read_SPIDLIST(),
	   mpFastIss_ASTC[i]->read_RBASE(),
	   mpFastIss_ASTC[i]->read_EBASE(),
	   mpFastIss_ASTC[i]->read_INTBP(),
	   mpFastIss_ASTC[i]->read_MCTL(),
	   mpFastIss_ASTC[i]->read_PID(),
	   "----------");
    printf("      SVLOCK=0x%08x,      sr9=%s,     sr10=%s,    SCCFG=0x%08x,     SCBP=0x%08x,     sr13=%s,     sr14=%s,     sr15=%s\n", 
	   mpFastIss_ASTC[i]->read_SVLOCK(),
	   "----------",
	   "----------",
	   mpFastIss_ASTC[i]->read_SCCFG(),
	   mpFastIss_ASTC[i]->read_SCBP(),
	   "----------",
	   "----------",
	   "----------");
    printf("        sr16=%s,     sr17=%s,     sr18=%s,     sr19=%s,     sr20=%s,     sr21=%s,     sr22=%s,     sr23=%s\n", 
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    printf("        sr24=%s,     sr25=%s,     sr26=%s,     sr27=%s,     sr28=%s,     sr29=%s,     sr30=%s,     sr31=%s\n", 
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");

    printf("  <<System regsiter : selID=2>>\n");
    printf("        PEID=0x%08x,     BMID=0x%08x,      sr2=%s,      sr3=%s,      sr4=%s,     sr5=%s,       MEA=0x%08x,      sr7=%s\n", 
	   mpFastIss_ASTC[i]->read_PEID(),
	   mpFastIss_ASTC[i]->read_BMID(),
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   mpFastIss_ASTC[i]->read_MEA(),
	   "----------");
    printf("         MEI=0x%08x,      sr9=%s,     ISPR=0x%08x,     sr11=%s,     ICSR=0x%08x,   INTCFG=0x%08x,     PLMR=0x%08x,    RBCR0=0x%08x\n", 
	   mpFastIss_ASTC[i]->read_MEI(),
	   "----------",
	   mpFastIss_ASTC[i]->read_ISPR(),
	   "----------",
	   mpFastIss_ASTC[i]->read_ICSR(),
	   mpFastIss_ASTC[i]->read_INTCFG(),
	   mpFastIss_ASTC[i]->read_PLMR(),
	   mpFastIss_ASTC[i]->read_RBCR0());
    printf("       RBCR1=0x%08x,     RBNR=0x%08x,     RBIP=0x%08x,     sr19=%s,     sr20=%s,     sr21=%s,     sr22=%s,     sr23=%s\n", 
	   mpFastIss_ASTC[i]->read_RBCR1(),
	   mpFastIss_ASTC[i]->read_RBNR(),
	   mpFastIss_ASTC[i]->read_RBIP(),
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    printf("        sr24=%s,     sr25=%s,     sr26=%s,     sr27=%s,     sr28=%s,     sr29=%s,     sr30=%s,     sr31=%s\n", 
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");


    printf("  <<System regsiter : selID=4>>\n");
    printf("         sr0=%s,      sr1=%s,      sr2=%s,      sr3=%s,      sr4=%s,      sr5=%s,      sr6=%s,      sr7=%s\n", 
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    printf("         sr8=%s,      sr9=%s,     sr10=%s,     sr11=%s,     sr12=%s,     se13=%s,     sr14=%s,     sr15=%s\n", 
 	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    printf("      ICTAGL=0x%08x,   ICTAGH=0x%08x,   ICDATL=0x%08x,   ICDATH=0x%08x,     sr20=%s,     sr21=%s,     sr22=%s,     sr23=%s\n", 
	   mpFastIss_ASTC[i]->read_ICTAGL(),
	   mpFastIss_ASTC[i]->read_ICTAGH(),
	   mpFastIss_ASTC[i]->read_ICDATL(),
	   mpFastIss_ASTC[i]->read_ICDATH(),
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    printf("      ICCTRL=0x%08x,     sr25=%s,    ICCFG=0x%08x,     sr27=%s,    ICERR=0x%08x,     sr29=%s,     sr30=%s,     sr31=%s\n", 
	   mpFastIss_ASTC[i]->read_ICCTRL(),
	   "----------",
	   mpFastIss_ASTC[i]->read_ICCFG(),
	   "----------",
	   mpFastIss_ASTC[i]->read_ICERR(),
	   "----------",
	   "----------",
	   "----------");

    printf("  <<System regsiter : selID=5>>\n");
    printf("         MPM=0x%08x,      sr1=%s,    MPCFG=0x%08x,      sr3=%s,      sr4=%s,      sr5=%s,      sr6=%s,      sr7=%s\n", 
	   mpFastIss_ASTC[i]->read_MPM(),
	   "----------",
	   mpFastIss_ASTC[i]->read_MPCFG(),
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    printf("         MCA=0x%08x,      MCS=0x%08x,      MCC=0x%08x,      MCR=0x%08x,      MCI=0x%08x,     sr13=%s,     sr14=%s,     sr15=%s\n", 
	   mpFastIss_ASTC[i]->read_MCA(),
	   mpFastIss_ASTC[i]->read_MCS(),
	   mpFastIss_ASTC[i]->read_MCC(),
	   mpFastIss_ASTC[i]->read_MCR(),
	   mpFastIss_ASTC[i]->read_MCI(),
	   "----------",
	   "----------",
	   "----------");
    printf("       MPIDX=0x%08x,     sr17=%s,     sr18=%s,     sr19=%s,     MPLA=0x%08x,     MPUA=0x%08x,     MPAT=0x%08x,     sr23=%s\n", 
	   mpFastIss_ASTC[i]->read_MPIDX(),
	   "----------",
	   "----------",
	   "----------",
	   mpFastIss_ASTC[i]->read_MPLA(),
	   mpFastIss_ASTC[i]->read_MPUA(),
	   mpFastIss_ASTC[i]->read_MPAT(),
	   "----------");
    printf("       MPID0=0x%08x,    MPID1=0x%08x,    MPID2=0x%08x,    MPID3=0x%08x,    MPID4=0x%08x,    MPID5=0x%08x,    MPID6=0x%08x,    MPID7=0x%08x\n",
	   mpFastIss_ASTC[i]->read_MPID0(),
	   mpFastIss_ASTC[i]->read_MPID1(),
	   mpFastIss_ASTC[i]->read_MPID2(),
	   mpFastIss_ASTC[i]->read_MPID3(),
	   mpFastIss_ASTC[i]->read_MPID4(),
	   mpFastIss_ASTC[i]->read_MPID5(),
	   mpFastIss_ASTC[i]->read_MPID6(),
	   mpFastIss_ASTC[i]->read_MPID7());

    printf("  <<System regsiter : selID=10>>\n");
    printf("         sr0=%s,      sr1=%s,      sr2=%s,      sr3=%s,      sr4=%s,      sr5=%s,     FXSR=0x%08x,      sr7=%s\n", 
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   mpFastIss_ASTC[i]->read_FXSR(),
	   "----------");
    printf("        FXST=0x%08x,   FXINFO=0x%08x,    FXCFG=0x%08x,     sr11=%s,     FXXC=0x%08x,     FXXP=0x%08x,     sr14=%s,     sr15=%s\n", 
	   mpFastIss_ASTC[i]->read_FXST(),
	   mpFastIss_ASTC[i]->read_FXINFO(),
	   mpFastIss_ASTC[i]->read_FXCFG(),
	   "----------",
	   mpFastIss_ASTC[i]->read_FXXC(),
	   mpFastIss_ASTC[i]->read_FXXP(),
	   "----------",
	   "----------");
    printf("        sr16=%s,     sr17=%s,     sr18=%s,     sr19=%s,     sr20=%s,     sr21=%s,     sr22=%s,     sr23=%s\n", 
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    printf("        sr24=%s,     sr25=%s,     sr26=%s,     sr27=%s,     sr28=%s,     sr29=%s,     sr30=%s,     sr31=%s\n", 
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");

    printf("  <<System regsiter : selID=11>>\n");
    printf("    TSCOUNTL=0x%08x, TSCOUNTH=0x%08x,   TSCTRL=0x%08x,      sr3=%s,      sr4=%s,      sr5=%s,      sr6=%s,      sr7=%s\n", 
	   mpFastIss_ASTC[i]->read_TSCOUNTL(),
	   mpFastIss_ASTC[i]->read_TSCOUNTH(),
	   mpFastIss_ASTC[i]->read_TSCTRL(),
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    printf("    PMUMCTRL=0x%08x,      sr9=%s,     sr10=%s,     sr11=%s,     sr12=%s,     sr13=%s,     sr14=%s,     sr15=%s\n", 
	   mpFastIss_ASTC[i]->read_PMUMCTRL(),
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    printf("    PMCOUNT0=0x%08x,  PMCTRL0=0x%08x, PMCOUNT1=0x%08x,  PMCTRL1=0x%08x, PMCOUNT2=0x%08x,  PMCTRL2=0x%08x, PMCOUNT3=0x%08x,  PMCTRL3=0x%08x\n", 
	   mpFastIss_ASTC[i]->read_PMCOUNT0(),
	   mpFastIss_ASTC[i]->read_PMCTRL0(),
	   mpFastIss_ASTC[i]->read_PMCOUNT1(),
	   mpFastIss_ASTC[i]->read_PMCTRL1(),
	   mpFastIss_ASTC[i]->read_PMCOUNT2(),
	   mpFastIss_ASTC[i]->read_PMCTRL2(),
	   mpFastIss_ASTC[i]->read_PMCOUNT3(),
	   mpFastIss_ASTC[i]->read_PMCTRL3());
    printf("        sr24=%s,     sr25=%s,     sr26=%s,     sr27=%s,     sr28=%s,     sr29=%s,     sr30=%s,     sr31=%s\n", 
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "----------");
    fflush(stdout);
  }
}


///<summary>Set quantum period</summary>
///<param name="period">quantum period</param>
///<param name="unit">time unit</param>
///<returns>none</returns>
void G4_FastIssIF::SetSyncPeriod (unsigned int period, sc_core::sc_time_unit unit)
{
  for(unsigned int i=0; i<mPeNum; i++){
    mpFastIss_ASTC[i]->quantum_period.value = sc_core::sc_time(period, unit);
  }
}


///<summary>Set translation block size</summary>
///<param name="pe_id">PE ID</param>
///<param name="size">block size</param>
///<returns>none</returns>
void G4_FastIssIF::SetBlockSize ( unsigned int pe_id, unsigned int size )
{
  mpFastIss_ASTC[pe_id]->block_size.value = size;
}


///<summary>Set trace title</summary>
///<returns>none</returns>
void G4_FastIssIF::SetTrcTitle( void )
{
  fprintf(mFpTrace, "PE:      Num           PC          Ope-code           Mnemonic                            Result\n");
  fflush(mFpTrace);
}


///<summary>Set cpu information(cpu type and initial value of system register)</summary>
///<param name="pe_id">PE ID</param>
///<param name="type">cpu type</param>
///<param name="cpu_info">initial value of system registe</param>
///<returns>none</returns>
void G4_FastIssIF::SetCpuInfo( unsigned int pe_id, CPU_KIND_T type , g4cpu_pe_info* pe_info)
{
#ifdef TEST_RESET_DBG
  printf("%s G4_FastIssIF::SetCpuType call (peid=%d, CPUTYPE=%d) \n", sc_time_stamp().to_string().c_str(), pe_id, (unsigned int)type);
  fflush(stdout);
#endif // TEST_RESET_DBG

  mPeType[pe_id] = type;

  mPeInfo[pe_id].peid_ival     = pe_info->peid_ival;
  mPeInfo[pe_id].bmid_ival     = pe_info->bmid_ival;
  mPeInfo[pe_id].spid_ival     = pe_info->spid_ival;
  mPeInfo[pe_id].gmspid_ival   = pe_info->spid_ival;
  mPeInfo[pe_id].spidlist_ival = pe_info->spidlist_ival;
  mPeInfo[pe_id].rbase_ival    = pe_info->rbase_ival;
  mPeInfo[pe_id].snzcfg_ival   = pe_info->snzcfg_ival;
  switch(type){
  case CPU_G4MH_1_1:
    mPeInfo[pe_id].hvcfg_ival = 0;
    mpFastIss_ASTC[pe_id]->cpu_milestone.value=rh850g4::core::FASTISS_G4MH_1_1;
    break;
  case CPU_G4MH_2_0:
    mPeInfo[pe_id].hvcfg_ival = pe_info->hvcfg_ival;
    mpFastIss_ASTC[pe_id]->cpu_milestone.value=rh850g4::core::FASTISS_G4MH_2_0;
    break;
  default:
    // same as CPU_G4MH_1_0
    mPeInfo[pe_id].hvcfg_ival = 0;
    mpFastIss_ASTC[pe_id]->cpu_milestone.value=rh850g4::core::FASTISS_G4MH_1_0;
    break;
  }
}

///<summary>reset the value of system register</summary>
///<param name="pe_id">PE ID(PEID-1)</param>
///<returns>none</returns>
void G4_FastIssIF::ResetSysReg( unsigned int pe_id )
{
#ifdef TEST_RESET_DBG
  printf("%s G4_FastIssIF::SetCpuType call (peid=%d, CPUTYPE=%d) \n", sc_time_stamp().to_string().c_str(), pe_id, (unsigned int)mPeType[pe_id]);
  fflush(stdout);
#endif // TEST_RESET_DBG
  mpFastIss_ASTC[pe_id]->write_FPSR(0x00220000);

  switch(mPeType[pe_id]){
  case CPU_G4MH_1_1:
    mpFastIss_ASTC[pe_id]->set_peid(mPeInfo[pe_id].peid_ival);
    mpFastIss_ASTC[pe_id]->write_PEID(mPeInfo[pe_id].peid_ival);
    mpFastIss_ASTC[pe_id]->write_BMID(mPeInfo[pe_id].bmid_ival);
    mpFastIss_ASTC[pe_id]->write_RBASE(mPeInfo[pe_id].rbase_ival);
    mpFastIss_ASTC[pe_id]->write_SPID(mPeInfo[pe_id].spid_ival);
    mpFastIss_ASTC[pe_id]->write_SPIDLIST(mPeInfo[pe_id].spidlist_ival);
    mpFastIss_ASTC[pe_id]->write_SNZCFG(mPeInfo[pe_id].snzcfg_ival);
    if ((pe_id == 0) || (pe_id == 2)){
      mpFastIss_ASTC[pe_id]->write_PID(0x06040f01);
    }else{
      mpFastIss_ASTC[pe_id]->write_PID(0x06040701);
    }
    mpFastIss_ASTC[pe_id]->write_ICCTRL(0x00010003);
    mpFastIss_ASTC[pe_id]->write_ICCFG(0x00001044);
    mpFastIss_ASTC[pe_id]->write_GMPEID(mPeInfo[pe_id].peid_ival);
    mpFastIss_ASTC[pe_id]->write_GMSPID(mPeInfo[pe_id].gmspid_ival);
    mpFastIss_ASTC[pe_id]->write_GMSPIDLIST(mPeInfo[pe_id].spidlist_ival);
    mpFastIss_ASTC[pe_id]->write_HVCFG(mPeInfo[pe_id].hvcfg_ival);
    mpFastIss_ASTC[pe_id]->write_DBCFG(0x00000000);
    break;
  case CPU_G4MH_2_0:
    mpFastIss_ASTC[pe_id]->set_peid(mPeInfo[pe_id].peid_ival);
    mpFastIss_ASTC[pe_id]->write_PEID(mPeInfo[pe_id].peid_ival);
    mpFastIss_ASTC[pe_id]->write_BMID(mPeInfo[pe_id].bmid_ival);
    mpFastIss_ASTC[pe_id]->write_RBASE(mPeInfo[pe_id].rbase_ival);
    mpFastIss_ASTC[pe_id]->write_SPID(mPeInfo[pe_id].spid_ival);
    mpFastIss_ASTC[pe_id]->write_SPIDLIST(mPeInfo[pe_id].spidlist_ival);
    mpFastIss_ASTC[pe_id]->write_SNZCFG(mPeInfo[pe_id].snzcfg_ival);
    mpFastIss_ASTC[pe_id]->write_PID(0x07840700);
    mpFastIss_ASTC[pe_id]->write_ICCTRL(0x00010003);
    mpFastIss_ASTC[pe_id]->write_ICCFG(0x00001044);
    mpFastIss_ASTC[pe_id]->write_GMPEID(mPeInfo[pe_id].peid_ival);
    mpFastIss_ASTC[pe_id]->write_GMSPID(mPeInfo[pe_id].gmspid_ival);
    mpFastIss_ASTC[pe_id]->write_GMSPIDLIST(mPeInfo[pe_id].spidlist_ival);
    mpFastIss_ASTC[pe_id]->write_HVCFG(mPeInfo[pe_id].hvcfg_ival);
    mpFastIss_ASTC[pe_id]->write_DBCFG(0x0000000b);
    break;
  default:
    // include G4MH
    mpFastIss_ASTC[pe_id]->set_peid(mPeInfo[pe_id].peid_ival);
    mpFastIss_ASTC[pe_id]->write_PEID(mPeInfo[pe_id].peid_ival);
    mpFastIss_ASTC[pe_id]->write_BMID(mPeInfo[pe_id].bmid_ival);
    mpFastIss_ASTC[pe_id]->write_RBASE(mPeInfo[pe_id].rbase_ival);
    mpFastIss_ASTC[pe_id]->write_SPID(mPeInfo[pe_id].spid_ival);
    mpFastIss_ASTC[pe_id]->write_SPIDLIST(mPeInfo[pe_id].spidlist_ival);
    mpFastIss_ASTC[pe_id]->write_SNZCFG(mPeInfo[pe_id].snzcfg_ival);
    mpFastIss_ASTC[pe_id]->write_PID(0x06040f00);
    mpFastIss_ASTC[pe_id]->write_ICCTRL(0x00010003);
    mpFastIss_ASTC[pe_id]->write_ICCFG(0x00001044);
    mpFastIss_ASTC[pe_id]->write_GMPEID(mPeInfo[pe_id].peid_ival);
    mpFastIss_ASTC[pe_id]->write_GMSPID(mPeInfo[pe_id].gmspid_ival);
    mpFastIss_ASTC[pe_id]->write_GMSPIDLIST(mPeInfo[pe_id].spidlist_ival);
    mpFastIss_ASTC[pe_id]->write_HVCFG(mPeInfo[pe_id].hvcfg_ival);
    mpFastIss_ASTC[pe_id]->write_DBCFG(0x00000000);
    break;   
  }
}


///<summary>Set SWD0PCAD's value</summary>
///<param name="pe_id">PE ID</param>
///<param name="value">SWD0PCAD's value</param>
///<returns>none</returns>
void G4_FastIssIF::SetSwd0pcadValue( unsigned int pe_id, unsigned int value )
{
#ifdef TEST_SWDT_CPU
  printf("%s:G4_FastIssIF:SetSwd0pcadValue invoked. value=0x%08x\n",
	 sc_time_stamp().to_string().c_str(),
	 value);
  fflush(stdout);
#endif
  mpFastIss_ASTC[pe_id]->set_swd0pcad_val(value);
}


///<summary>Set SWDT's PC detection func mode</summary>
///<param name="pe_id">PE ID</param>
///<param name="mode">PC detection func mode(true:enable, false:disable)</param>
///<returns>none</returns>
void G4_FastIssIF::SetSwdtEnable( unsigned int pe_id, bool mode )
{
#ifdef TEST_SWDT_CPU
  printf("%s:G4_FastIssIF:SetSwdtEnable invoked. ",
	 sc_time_stamp().to_string().c_str());
  if(mode) printf("detection mode is enable\n");
  else     printf("detection mode is disable\n");
  fflush(stdout);
#endif
  mpFastIss_ASTC[pe_id]->set_swdt_enable(mode);
}


///<summary>set link function mode</summary>
///<param name="mode">link function mode(true:ON, false:OFF)</param>
///<returns>none</returns>
void G4_FastIssIF::setLinkFuncEnable( bool mode )
{
  for(unsigned int i=0; i<mPeNum; i++){
#ifdef TEST_LINK_DBG
    printf("%s:G4_FastIssIF::setLinkFuncEnable invoked.:link function of PE%d is %d\n",
	   sc_time_stamp().to_string().c_str(),
	   i+1,
	   mode);
    fflush(stdout);
#endif
    mpFastIss_ASTC[i]->link_func_enable.value = mode;
  }
}


///<summary>Set snooze time</summary>
///<param name="pe_id">PE ID</param>
///<param name="snz_time">snooze time(cycle)</param>
///<returns>none</returns>
void G4_FastIssIF::SetSnzTime( unsigned int pe_id, unsigned int snz_time )
{
#ifdef TEST_SNZ_TIME
  printf("%s:G4_FastIssIF:SetSnzTime invoked. snz_time=0x%08x\n",
	 sc_time_stamp().to_string().c_str(),
	 snz_time);
  fflush(stdout);
#endif
  mpFastIss_ASTC[pe_id]->snz_time.value = snz_time;
}


///<summary>Attach analysis uint</summary>
///<param name="is_old_fromat">Trace Format (true is not out put machine info, false is output machine info)</param>
///<returns>none</returns>
void G4_FastIssIF::AttachAnalysisUnit( bool is_old_fromat )
{
  if(mFpTrace){
    for(unsigned int i=0; i<mPeNum; i++){
      if(ex_trace[i] == (rh850g4::execution_tracer*)0){
	ex_trace[i] = new rh850g4::execution_tracer( i, mFpTrace, is_old_fromat);
      }
      ex_trace[i]->attach(mpFastIss_ASTC[i]);
    }
  }
}


///<summary>Detach analysis uint</summary>
///<returns>none</returns>
void G4_FastIssIF::DetachAnalysisUnit( void )
{
  if(mFpTrace){
    for(unsigned int i=0; i<mPeNum; i++){
      if(ex_trace[i] != (rh850g4::execution_tracer*)0){
	ex_trace[i]->detach();
      }
    }
  }
}

///<summary>clear the fastISS's traslation cache</summary>
///<param name="pe_id">pe id</param>
///<returns>none</returns>
void G4_FastIssIF::clearFastissCache( unsigned int pe_id )
{
  mpFastIss_ASTC[pe_id]->flush_cache();
}


///<summary>Get executed time of SWD0PCAD address</summary>
///<param name="pe_id">PE ID</param>
///<returns>executed time of SWD0PCAD address</returns>
unsigned int G4_FastIssIF::GetSwd0pcadExecCountValue( unsigned int pe_id )
{
  unsigned int val = mpFastIss_ASTC[pe_id]->get_swd0pcad_exec_count_val();
#ifdef TEST_SWDT_CPU
  printf("%s:G4_FastIssIF:GetSwd0pcadExecCountValue invoked. count=%d\n",
	 sc_time_stamp().to_string().c_str(),
	 val);
  fflush(stdout);
#endif
  return(val);
}

///<summary>Get VMID from HTCFG0.VCID</summary>
///<param name="pe_id">PE ID</param>
///<returns>VM ID</returns>
unsigned char G4_FastIssIF::GetVmid( unsigned int pe_id )
{
  unsigned int vmid=0;
  //vmid = (mpFastIss_ASTC[pe_id]->read_HTCFG0() & 0x00000700) >> 8;
  return((unsigned char)vmid);
}


///<summary>Get VMID from HTCFG0.TCID</summary>
///<param name="pe_id">PE ID</param>
///<returns>TCID</returns>
unsigned char G4_FastIssIF::GetTcid( unsigned int pe_id )
{
  unsigned int tcid=0;
  //tcid = mpFastIss_ASTC[pe_id]->read_HTCFG0() & 0x00000ff;
  return((unsigned char)tcid);
}


///<summary>Get SPID from MCFG0.SPID</summary>
///<param name="pe_id">PE ID</param>
///<returns>SPID</returns>
unsigned char G4_FastIssIF::GetSpid( unsigned int pe_id )
{
  unsigned int spid=0;
  spid = (mpFastIss_ASTC[pe_id]->read_SPID() & 0x001f0000) >> 16;
  return((unsigned char)spid);
}


///<summary>Get user mode from PSW.UM</summary>
///<param name="pe_id">PE ID</param>
///<returns>user mode</returns>
bool G4_FastIssIF::GetUserMode( unsigned int pe_id )
{
  unsigned int um=0;
  um = (mpFastIss_ASTC[pe_id]->read_PSW() & 0x40000000) >> 30;
  if(um == 0){
    return(false);
  }else{
    return(true);
  }
}


///<summary>Get virtual mode from PSW.VM</summary>
///<param name="pe_id">PE ID</param>
///<returns>virtual mode</returns>
bool G4_FastIssIF::GetVirtualMode( unsigned int pe_id )
{
  unsigned int vm=0;
  vm = (mpFastIss_ASTC[pe_id]->read_PSW() & 0x80000000) >> 31;
  if(vm == 0){
    return(false);
  }else{
    return(true);
  }
}


///<summary>Get memory access event type</summary>
///<param name="pe_id">PE ID</param>
///<returns>memory acces event type</returns>
unsigned int G4_FastIssIF::GetMemAccEvType( unsigned int pe_id )
{
  return(mpFastIss_ASTC[pe_id]->get_mem_acc_type());
}


///<summary>Set Local RAM area</summary>
///<param name="index">index</param>
///<param name="ptr">pointer of fast ISS</param>
///<returns>none</returns>
void G4_FastIssIF::SetFastIssPtr( unsigned int index, rh850g4::core* prr )
{
  if (index < PE_MAX_NUM){
    mpFastIss_ASTC[index] = prr;
    mpFastIss_ASTC[index]->blocking_tlm.value = true;
    mpFastIss_ASTC[index]->set_peid(index);
    mpFastIss_ASTC[index]->adaptive_count.value = 5;
    mpFastIss_ASTC[index]->set_swdt_enable(false);
  }
}


///<summary>Set Local RAM area</summary>
///<param name="index">index</param>
///<param name="ptr">pointer of fast ISS</param>
///<returns>none</returns>
void G4_FastIssIF::SetFastIssMemWrapperPtr( unsigned int index, rh850g4::memory_wrapper* prr )
{
  if (index < PE_MAX_NUM){
    mpFastIssMemWrapper_ASTC[index] = prr;
  }
}

///<summary>read the global register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readGrValue( unsigned int peid, unsigned int regid )
{  
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_r0();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_r1();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_r2();
    break; 
  case 3: value = mpFastIss_ASTC[peid]->read_r3();
    break; 
  case 4: value = mpFastIss_ASTC[peid]->read_r4();
    break; 
  case 5: value = mpFastIss_ASTC[peid]->read_r5();
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_r6();
    break; 
  case 7: value = mpFastIss_ASTC[peid]->read_r7();
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_r8();
    break; 
  case 9: value = mpFastIss_ASTC[peid]->read_r9();
    break; 
  case 10: value = mpFastIss_ASTC[peid]->read_r10();
    break; 
  case 11: value = mpFastIss_ASTC[peid]->read_r11();
    break; 
  case 12: value = mpFastIss_ASTC[peid]->read_r12();
    break; 
  case 13: value = mpFastIss_ASTC[peid]->read_r13();
    break; 
  case 14: value = mpFastIss_ASTC[peid]->read_r14();
    break; 
  case 15: value = mpFastIss_ASTC[peid]->read_r15();
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_r16();
    break; 
  case 17: value = mpFastIss_ASTC[peid]->read_r17();
    break; 
  case 18: value = mpFastIss_ASTC[peid]->read_r18();
    break; 
  case 19: value = mpFastIss_ASTC[peid]->read_r19();
    break; 
  case 20: value = mpFastIss_ASTC[peid]->read_r20();
    break; 
  case 21: value = mpFastIss_ASTC[peid]->read_r21();
    break; 
  case 22: value = mpFastIss_ASTC[peid]->read_r22();
    break; 
  case 23: value = mpFastIss_ASTC[peid]->read_r23();
    break; 
  case 24: value = mpFastIss_ASTC[peid]->read_r24();
    break; 
  case 25: value = mpFastIss_ASTC[peid]->read_r25();
    break; 
  case 26: value = mpFastIss_ASTC[peid]->read_r26();
    break; 
  case 27: value = mpFastIss_ASTC[peid]->read_r27();
    break; 
  case 28: value = mpFastIss_ASTC[peid]->read_r28();
    break; 
  case 29: value = mpFastIss_ASTC[peid]->read_r29();
    break; 
  case 30: value = mpFastIss_ASTC[peid]->read_r30();
    break; 
  case 31: value = mpFastIss_ASTC[peid]->read_r31();
    break; 
  }
  return(value);
}


///<summary>write the global register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeGrValue( unsigned int peid, unsigned int regid, unsigned int value)
{
  switch(regid){
  case 0:
    break; 
  case 1: mpFastIss_ASTC[peid]->write_r1(value);
    break; 
  case 2: mpFastIss_ASTC[peid]->write_r2(value);
    break;
  case 3: mpFastIss_ASTC[peid]->write_r3(value);
    break; 
  case 4: mpFastIss_ASTC[peid]->write_r4(value);
    break; 
  case 5: mpFastIss_ASTC[peid]->write_r5(value);
    break; 
  case 6: mpFastIss_ASTC[peid]->write_r6(value);
    break;
  case 7: mpFastIss_ASTC[peid]->write_r7(value);
    break; 
  case 8: mpFastIss_ASTC[peid]->write_r8(value);
    break; 
  case 9: mpFastIss_ASTC[peid]->write_r9(value);
    break; 
  case 10: mpFastIss_ASTC[peid]->write_r10(value);
    break; 
  case 11: mpFastIss_ASTC[peid]->write_r11(value);
    break; 
  case 12: mpFastIss_ASTC[peid]->write_r12(value);
    break; 
  case 13: mpFastIss_ASTC[peid]->write_r13(value);
    break; 
  case 14: mpFastIss_ASTC[peid]->write_r14(value);
    break; 
  case 15: mpFastIss_ASTC[peid]->write_r15(value);
    break; 
  case 16: mpFastIss_ASTC[peid]->write_r16(value);
    break; 
  case 17: mpFastIss_ASTC[peid]->write_r17(value);
    break; 
  case 18: mpFastIss_ASTC[peid]->write_r18(value);
    break; 
  case 19: mpFastIss_ASTC[peid]->write_r19(value);
    break; 
  case 20: mpFastIss_ASTC[peid]->write_r20(value);
    break; 
  case 21: mpFastIss_ASTC[peid]->write_r21(value);
    break; 
  case 22: mpFastIss_ASTC[peid]->write_r22(value);
    break; 
  case 23: mpFastIss_ASTC[peid]->write_r23(value);
    break; 
  case 24: mpFastIss_ASTC[peid]->write_r24(value);
    break; 
  case 25: mpFastIss_ASTC[peid]->write_r25(value);
    break; 
  case 26: mpFastIss_ASTC[peid]->write_r26(value);
    break; 
  case 27: mpFastIss_ASTC[peid]->write_r27(value);
    break; 
  case 28: mpFastIss_ASTC[peid]->write_r28(value);
    break; 
  case 29: mpFastIss_ASTC[peid]->write_r29(value);
    break; 
  case 30: mpFastIss_ASTC[peid]->write_r30(value);
    break; 
  case 31: mpFastIss_ASTC[peid]->write_r31(value);
    break; 
  }
}


///<summary>read the vector register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="isUpper">data position (true:upper 64bit, false:lower64bit)</param>
///<returns>register value</returns>
unsigned long long G4_FastIssIF::readWrValue( unsigned int peid, unsigned int regid, bool isUpper )
{  
  if(isUpper){
    return(readWrUpperValue(peid, regid));
  }else{
    return(readWrLowerValue(peid, regid));
  }
}


///<summary>write the vector register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="up_value">regester value (upper 64bit)</param>
///<param name="lo_value">regester value (lower 64bit)</param>
///<returns>none</returns>
void G4_FastIssIF::writeWrValue( unsigned int peid, unsigned int regid, unsigned long long up_value, unsigned long long lo_value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_wr0(lo_value);
    mpFastIss_ASTC[peid]->write_wr0_u(up_value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_wr1(lo_value);
    mpFastIss_ASTC[peid]->write_wr1_u(up_value);
    break; 
  case 2: mpFastIss_ASTC[peid]->write_wr2(lo_value);
    mpFastIss_ASTC[peid]->write_wr2_u(up_value);
    break;
  case 3: mpFastIss_ASTC[peid]->write_wr3(lo_value);
    mpFastIss_ASTC[peid]->write_wr3_u(up_value);
    break; 
  case 4: mpFastIss_ASTC[peid]->write_wr4(lo_value);
    mpFastIss_ASTC[peid]->write_wr4_u(up_value);
    break; 
  case 5: mpFastIss_ASTC[peid]->write_wr5(lo_value);
    mpFastIss_ASTC[peid]->write_wr5_u(up_value);
    break; 
  case 6: mpFastIss_ASTC[peid]->write_wr6(lo_value);
    mpFastIss_ASTC[peid]->write_wr6_u(up_value);
    break;
  case 7: mpFastIss_ASTC[peid]->write_wr7(lo_value);
    mpFastIss_ASTC[peid]->write_wr7_u(up_value);
    break; 
  case 8: mpFastIss_ASTC[peid]->write_wr8(lo_value);
    mpFastIss_ASTC[peid]->write_wr8_u(up_value);
    break; 
  case 9: mpFastIss_ASTC[peid]->write_wr9(lo_value);
    mpFastIss_ASTC[peid]->write_wr9_u(up_value);
    break; 
  case 10: mpFastIss_ASTC[peid]->write_wr10(lo_value);
    mpFastIss_ASTC[peid]->write_wr10_u(up_value);
    break; 
  case 11: mpFastIss_ASTC[peid]->write_wr11(lo_value);
    mpFastIss_ASTC[peid]->write_wr11_u(up_value);
    break; 
  case 12: mpFastIss_ASTC[peid]->write_wr12(lo_value);
    mpFastIss_ASTC[peid]->write_wr12_u(up_value);
    break; 
  case 13: mpFastIss_ASTC[peid]->write_wr13(lo_value);
    mpFastIss_ASTC[peid]->write_wr13_u(up_value);
    break; 
  case 14: mpFastIss_ASTC[peid]->write_wr14(lo_value);
    mpFastIss_ASTC[peid]->write_wr14_u(up_value);
    break; 
  case 15: mpFastIss_ASTC[peid]->write_wr15(lo_value);
    mpFastIss_ASTC[peid]->write_wr15_u(up_value);
    break; 
  case 16: mpFastIss_ASTC[peid]->write_wr16(lo_value);
    mpFastIss_ASTC[peid]->write_wr16_u(up_value);
    break; 
  case 17: mpFastIss_ASTC[peid]->write_wr17(lo_value);
    mpFastIss_ASTC[peid]->write_wr17_u(up_value);
    break; 
  case 18: mpFastIss_ASTC[peid]->write_wr18(lo_value);
    mpFastIss_ASTC[peid]->write_wr18_u(up_value);
    break; 
  case 19: mpFastIss_ASTC[peid]->write_wr19(lo_value);
    mpFastIss_ASTC[peid]->write_wr19_u(up_value);
    break; 
  case 20: mpFastIss_ASTC[peid]->write_wr20(lo_value);
    mpFastIss_ASTC[peid]->write_wr20_u(up_value);
    break; 
  case 21: mpFastIss_ASTC[peid]->write_wr21(lo_value);
    mpFastIss_ASTC[peid]->write_wr21_u(up_value);
    break; 
  case 22: mpFastIss_ASTC[peid]->write_wr22(lo_value);
    mpFastIss_ASTC[peid]->write_wr22_u(up_value);
    break; 
  case 23: mpFastIss_ASTC[peid]->write_wr23(lo_value);
    mpFastIss_ASTC[peid]->write_wr23_u(up_value);
    break; 
  case 24: mpFastIss_ASTC[peid]->write_wr24(lo_value);
    mpFastIss_ASTC[peid]->write_wr24_u(up_value);
    break; 
  case 25: mpFastIss_ASTC[peid]->write_wr25(lo_value);
    mpFastIss_ASTC[peid]->write_wr25_u(up_value);
    break; 
  case 26: mpFastIss_ASTC[peid]->write_wr26(lo_value);
    mpFastIss_ASTC[peid]->write_wr26_u(up_value);
    break; 
  case 27: mpFastIss_ASTC[peid]->write_wr27(lo_value);
    mpFastIss_ASTC[peid]->write_wr27_u(up_value);
    break; 
  case 28: mpFastIss_ASTC[peid]->write_wr28(lo_value);
    mpFastIss_ASTC[peid]->write_wr28_u(up_value);
    break; 
  case 29: mpFastIss_ASTC[peid]->write_wr29(lo_value);
    mpFastIss_ASTC[peid]->write_wr29_u(up_value);
    break; 
  case 30: mpFastIss_ASTC[peid]->write_wr30(lo_value);
    mpFastIss_ASTC[peid]->write_wr30_u(up_value);
    break; 
  case 31: mpFastIss_ASTC[peid]->write_wr31(lo_value);
    mpFastIss_ASTC[peid]->write_wr31_u(up_value);
    break; 
  }
}


///<summary>read the system register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="selid">selecter ID (0-13)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrValue( unsigned int peid, unsigned int regid, unsigned int selid )
{
  unsigned int value = 0;
  switch(selid){
  case 0: value = readSrSelid0(peid, regid);
    break;
  case 1: value = readSrSelid1(peid, regid);
    break;
  case 2: value = readSrSelid2(peid, regid);
    break;
  case 3: value = readSrSelid3(peid, regid);
    break;
  case 4: value = readSrSelid4(peid, regid);
    break;
  case 5: value = readSrSelid5(peid, regid);
    break;
  case 9: 
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      if((mpFastIss_ASTC[peid]->read_HVCFG() & 0x1) == 0x1){
	value = readSrSelid9(peid, regid);
      }
    }
    break;
  case 10: value = readSrSelid10(peid, regid);
    break;
  case 11: value = readSrSelid11(peid, regid);
    break;
  case 12: value = readSrSelid12(peid, regid);
    break;
  case 13: value = readSrSelid13(peid, regid);
    break;
  case 14: 
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = readSrSelid14(peid, regid);
    }
    break;
  case 15: 
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = readSrSelid15(peid, regid);
    }
    break;
  }
  return(value);
}


///<summary>write the system register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="selid">selecter ID (0-13)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrValue( unsigned int peid, unsigned int regid, unsigned int selid, unsigned int value)
{
  switch(selid){
  case 0: writeSrSelid0(peid, regid, value);
    break;
  case 1: writeSrSelid1(peid, regid, value);
    break;
  case 2: writeSrSelid2(peid, regid, value);
    break;
  case 3: writeSrSelid3(peid, regid, value);
    break;
  case 4: writeSrSelid4(peid, regid, value);
    break;
  case 5: writeSrSelid5(peid, regid, value);
    break;
  case 9: 
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      if((mpFastIss_ASTC[peid]->read_HVCFG() & 0x1) == 0x1){
	writeSrSelid9(peid, regid, value);
      }
    }
    break;
  case 10: writeSrSelid10(peid, regid, value);
    break;
  case 11: writeSrSelid11(peid, regid, value);
    break;
  case 12: writeSrSelid12(peid, regid, value);
    break;
  case 13: writeSrSelid13(peid, regid, value);
    break;
  case 14: 
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      writeSrSelid14(peid, regid, value);
    }
    break;
  case 15: 
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      writeSrSelid15(peid, regid, value);
    }
    break;
  }
}


///<summary>read the vector register value (lower 64bit)</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned long long G4_FastIssIF::readWrLowerValue( unsigned int peid, unsigned int regid )
{
  unsigned long long value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_wr0();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_wr1();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_wr2();
    break; 
  case 3: value = mpFastIss_ASTC[peid]->read_wr3();
    break; 
  case 4: value = mpFastIss_ASTC[peid]->read_wr4();
    break; 
  case 5: value = mpFastIss_ASTC[peid]->read_wr5();
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_wr6();
    break; 
  case 7: value = mpFastIss_ASTC[peid]->read_wr7();
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_wr8();
    break; 
  case 9: value = mpFastIss_ASTC[peid]->read_wr9();
    break; 
  case 10: value = mpFastIss_ASTC[peid]->read_wr10();
    break; 
  case 11: value = mpFastIss_ASTC[peid]->read_wr11();
    break; 
  case 12: value = mpFastIss_ASTC[peid]->read_wr12();
    break; 
  case 13: value = mpFastIss_ASTC[peid]->read_wr13();
    break; 
  case 14: value = mpFastIss_ASTC[peid]->read_wr14();
    break; 
  case 15: value = mpFastIss_ASTC[peid]->read_wr15();
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_wr16();
    break; 
  case 17: value = mpFastIss_ASTC[peid]->read_wr17();
    break; 
  case 18: value = mpFastIss_ASTC[peid]->read_wr18();
    break; 
  case 19: value = mpFastIss_ASTC[peid]->read_wr19();
    break; 
  case 20: value = mpFastIss_ASTC[peid]->read_wr20();
    break; 
  case 21: value = mpFastIss_ASTC[peid]->read_wr21();
    break; 
  case 22: value = mpFastIss_ASTC[peid]->read_wr22();
    break; 
  case 23: value = mpFastIss_ASTC[peid]->read_wr23();
    break; 
  case 24: value = mpFastIss_ASTC[peid]->read_wr24();
    break; 
  case 25: value = mpFastIss_ASTC[peid]->read_wr25();
    break; 
  case 26: value = mpFastIss_ASTC[peid]->read_wr26();
    break; 
  case 27: value = mpFastIss_ASTC[peid]->read_wr27();
    break; 
  case 28: value = mpFastIss_ASTC[peid]->read_wr28();
    break; 
  case 29: value = mpFastIss_ASTC[peid]->read_wr29();
    break; 
  case 30: value = mpFastIss_ASTC[peid]->read_wr30();
    break; 
  case 31: value = mpFastIss_ASTC[peid]->read_wr31();
    break; 
  }
  return(value);
}

///<summary>read the vector register value (upper 64bit)</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned long long G4_FastIssIF::readWrUpperValue( unsigned int peid, unsigned int regid )
{
  unsigned long long value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_wr0_u();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_wr1_u();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_wr2_u();
    break; 
  case 3: value = mpFastIss_ASTC[peid]->read_wr3_u();
    break; 
  case 4: value = mpFastIss_ASTC[peid]->read_wr4_u();
    break; 
  case 5: value = mpFastIss_ASTC[peid]->read_wr5_u();
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_wr6_u();
    break; 
  case 7: value = mpFastIss_ASTC[peid]->read_wr7_u();
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_wr8_u();
    break; 
  case 9: value = mpFastIss_ASTC[peid]->read_wr9_u();
    break; 
  case 10: value = mpFastIss_ASTC[peid]->read_wr10_u();
    break; 
  case 11: value = mpFastIss_ASTC[peid]->read_wr11_u();
    break; 
  case 12: value = mpFastIss_ASTC[peid]->read_wr12_u();
    break; 
  case 13: value = mpFastIss_ASTC[peid]->read_wr13_u();
    break; 
  case 14: value = mpFastIss_ASTC[peid]->read_wr14_u();
    break; 
  case 15: value = mpFastIss_ASTC[peid]->read_wr15_u();
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_wr16_u();
    break; 
  case 17: value = mpFastIss_ASTC[peid]->read_wr17_u();
    break; 
  case 18: value = mpFastIss_ASTC[peid]->read_wr18_u();
    break; 
  case 19: value = mpFastIss_ASTC[peid]->read_wr19_u();
    break; 
  case 20: value = mpFastIss_ASTC[peid]->read_wr20_u();
    break; 
  case 21: value = mpFastIss_ASTC[peid]->read_wr21_u();
    break; 
  case 22: value = mpFastIss_ASTC[peid]->read_wr22_u();
    break; 
  case 23: value = mpFastIss_ASTC[peid]->read_wr23_u();
    break; 
  case 24: value = mpFastIss_ASTC[peid]->read_wr24_u();
    break; 
  case 25: value = mpFastIss_ASTC[peid]->read_wr25_u();
    break; 
  case 26: value = mpFastIss_ASTC[peid]->read_wr26_u();
    break; 
  case 27: value = mpFastIss_ASTC[peid]->read_wr27_u();
    break; 
  case 28: value = mpFastIss_ASTC[peid]->read_wr28_u();
    break; 
  case 29: value = mpFastIss_ASTC[peid]->read_wr29_u();
    break; 
  case 30: value = mpFastIss_ASTC[peid]->read_wr30_u();
    break; 
  case 31: value = mpFastIss_ASTC[peid]->read_wr31_u();
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=0) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid0( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_EIPC();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_EIPSW();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_FEPC();
    break; 
  case 3: value = mpFastIss_ASTC[peid]->read_FEPSW();
    break; 
  case 4:
    break; 
  case 5: value = mpFastIss_ASTC[peid]->read_PSW();
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_FPSR();
    break; 
  case 7: value = mpFastIss_ASTC[peid]->read_FPEPC();
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_FPST();
    break; 
  case 9: value = mpFastIss_ASTC[peid]->read_FPCC();
    break; 
  case 10: value = mpFastIss_ASTC[peid]->read_FPCFG();
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13: value = mpFastIss_ASTC[peid]->read_EIIC();
    break; 
  case 14: value = mpFastIss_ASTC[peid]->read_FEIC();
    break; 
  case 15:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PSWH();
    }
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_CTPC();
    break; 
  case 17: value = mpFastIss_ASTC[peid]->read_CTPSW();
    break; 
  case 18:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_EIPSWH();
    }
    break; 
  case 19:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_FEPSWH();
    }
    break; 
  case 20: value = mpFastIss_ASTC[peid]->read_CTBP();
    break; 
  case 21: value = mpFastIss_ASTC[peid]->read_SNZCFG();
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28: value = mpFastIss_ASTC[peid]->read_EIWR();
    break; 
  case 29: value = mpFastIss_ASTC[peid]->read_FEWR();
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=1) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid1( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_SPID();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_SPIDLIST();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_RBASE();
    break; 
  case 3: value = mpFastIss_ASTC[peid]->read_EBASE();
    break; 
  case 4: value = mpFastIss_ASTC[peid]->read_INTBP();
    break; 
  case 5: value = mpFastIss_ASTC[peid]->read_MCTL();
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_PID();
    break; 
  case 7:
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_SVLOCK();
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11: value = mpFastIss_ASTC[peid]->read_SCCFG();
    break; 
  case 12: value = mpFastIss_ASTC[peid]->read_SCBP();
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_HVCFG();
    }
    break; 
  case 17:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_GMCFG();
    }
    break; 
  case 18:
    break; 
  case 19:
    break; 
  case 20:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_HVSB();
    }
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=2) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid2( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_PEID();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_BMID();
    break; 
  case 2:
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_MEA();
    break; 
  case 7:
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_MEI();
    break; 
  case 9:
    break; 
  case 10: value = mpFastIss_ASTC[peid]->read_ISPR();
    break; 
  case 11:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_IMSR();
    }
    break; 
  case 12: value = mpFastIss_ASTC[peid]->read_ICSR();
    break; 
  case 13: value = mpFastIss_ASTC[peid]->read_INTCFG();
    break; 
  case 14: value = mpFastIss_ASTC[peid]->read_PLMR();
    break; 
  case 15: value = mpFastIss_ASTC[peid]->read_RBCR0();
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_RBCR1();
    break; 
  case 17: value = mpFastIss_ASTC[peid]->read_RBNR();
    break; 
  case 18: value = mpFastIss_ASTC[peid]->read_RBIP();
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=3) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid3( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_DBGEN();
    }
    break; 
  case 1:
    break; 
  case 2:
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8:
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_DBPSWH();
    }
    break; 
  case 14:
    break;
  case 15: value = mpFastIss_ASTC[peid]->read_DBIC();
    break; 
  case 16: 
    break; 
  case 17: 
    break; 
  case 18: value = mpFastIss_ASTC[peid]->read_DBPC();
    break; 
  case 19: value = mpFastIss_ASTC[peid]->read_DBPSW();
    break; 
  case 20: value = mpFastIss_ASTC[peid]->read_DIR0();
    break; 
  case 21: value = mpFastIss_ASTC[peid]->read_DIR1();
    break; 
  case 22: value = mpFastIss_ASTC[peid]->read_BPC();
    break; 
  case 23:
    break; 
  case 24: value = mpFastIss_ASTC[peid]->read_BPAV();
    break; 
  case 25: value = mpFastIss_ASTC[peid]->read_BPAM();
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_DBCFG();
    }
    break; 
  case 29:
    break; 
  case 30: value = mpFastIss_ASTC[peid]->read_DBWR();
    break; 
  case 31:
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=4) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid4( unsigned int peid, unsigned int regid )
{
  unsigned int value = 0;
  switch(regid){
  case 0:
    break; 
  case 1:
    break; 
  case 2:
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8:
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13:
    break; 
  case 14:
    break;
  case 15:
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_ICTAGL();
    break; 
  case 17: value = mpFastIss_ASTC[peid]->read_ICTAGH();
    break; 
  case 18: value = mpFastIss_ASTC[peid]->read_ICDATL();
    break; 
  case 19: value = mpFastIss_ASTC[peid]->read_ICDATH();
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break;
  case 23:
    break; 
  case 24: value = mpFastIss_ASTC[peid]->read_ICCTRL();
    break; 
  case 25:
    break; 
  case 26: value = mpFastIss_ASTC[peid]->read_ICCFG();
    break; 
  case 27:
    break; 
  case 28: value = mpFastIss_ASTC[peid]->read_ICERR();
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=5) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid5( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_MPM();
    break; 
  case 1:
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_MPCFG();
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_MCA();
    break; 
  case 9: value = mpFastIss_ASTC[peid]->read_MCS();
    break; 
  case 10: value = mpFastIss_ASTC[peid]->read_MCC();
    break; 
  case 11: value = mpFastIss_ASTC[peid]->read_MCR();
    break; 
  case 12: value = mpFastIss_ASTC[peid]->read_MCI();
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_MPIDX();
    break; 
  case 17:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_MPBK();
    }
    break; 
  case 18:
    break;
  case 19:
    break; 
  case 20: value = mpFastIss_ASTC[peid]->read_MPLA();
    break; 
  case 21: value = mpFastIss_ASTC[peid]->read_MPUA();
    break; 
  case 22: value = mpFastIss_ASTC[peid]->read_MPAT();
    break; 
  case 23:
    break; 
  case 24: value = mpFastIss_ASTC[peid]->read_MPID0();
    break; 
  case 25: value = mpFastIss_ASTC[peid]->read_MPID1();
    break; 
  case 26: value = mpFastIss_ASTC[peid]->read_MPID2();
    break; 
  case 27: value = mpFastIss_ASTC[peid]->read_MPID3();
    break; 
  case 28: value = mpFastIss_ASTC[peid]->read_MPID4();
    break; 
  case 29: value = mpFastIss_ASTC[peid]->read_MPID5();
    break; 
  case 30: value = mpFastIss_ASTC[peid]->read_MPID6();
    break; 
  case 31: value = mpFastIss_ASTC[peid]->read_MPID7();
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=9) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid9( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_GMEIPC();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_GMEIPSW();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_GMFEPC();
    break; 
  case 3: value = mpFastIss_ASTC[peid]->read_GMFEPSW();
    break; 
  case 4:
    break; 
  case 5: value = mpFastIss_ASTC[peid]->read_GMPSW();
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_GMMEA();
    break; 
  case 7:
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_GMMEI();
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13: value = mpFastIss_ASTC[peid]->read_GMEIIC();
    break; 
  case 14: value = mpFastIss_ASTC[peid]->read_GMFEIC();
    break; 
  case 15:
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_GMSPID();
    break; 
  case 17: value = mpFastIss_ASTC[peid]->read_GMSPIDLIST();
    break; 
  case 18:
    break;
  case 19: value = mpFastIss_ASTC[peid]->read_GMEBASE();
    break; 
  case 20: value = mpFastIss_ASTC[peid]->read_GMINTBP();
    break; 
  case 21: value = mpFastIss_ASTC[peid]->read_GMINTCFG();
    break; 
  case 22: value = mpFastIss_ASTC[peid]->read_GMPLMR();
    break; 
  case 23:
    break; 
  case 24: value = mpFastIss_ASTC[peid]->read_GMSVLOCK();
    break; 
  case 25: value = mpFastIss_ASTC[peid]->read_GMMPM();
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28: value = mpFastIss_ASTC[peid]->read_GMEIWR();
    break; 
  case 29: value = mpFastIss_ASTC[peid]->read_GMFEWR();
    break; 
  case 30: value = mpFastIss_ASTC[peid]->read_GMPEID();
    break; 
  case 31:
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=10) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid10( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0:
    break; 
  case 1:
    break; 
  case 2:
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_FXSR();
    break; 
  case 7:
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_FXST();
    break; 
  case 9: value = mpFastIss_ASTC[peid]->read_FXINFO();
    break; 
  case 10: value = mpFastIss_ASTC[peid]->read_FXCFG();
    break; 
  case 11:
    break; 
  case 12: value = mpFastIss_ASTC[peid]->read_FXXC();
    break; 
  case 13: value = mpFastIss_ASTC[peid]->read_FXXP();
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    break; 
  case 17:
    break; 
  case 18:
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=11) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid11( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_TSCOUNTL();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_TSCOUNTH();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_TSCTRL();
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_PMUMCTRL();
    break; 
  case 9:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PMGMCTRL();
    }
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PMCOUNT0();
    }
    break; 
  case 17: 
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PMCTRL0();
    }
    break; 
  case 18: 
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PMCOUNT1();
    }
    break; 
  case 19:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PMCTRL1();
    }
    break; 
  case 20:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PMCOUNT2();
    }
    break; 
  case 21:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PMCTRL2();
    }
    break; 
  case 22:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PMCOUNT3();
    }
    break; 
  case 23:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_PMCTRL3();
    }
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break;
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=12) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid12( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_LSTEST0();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_LSTEST1();
    break; 
  case 2:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      value = mpFastIss_ASTC[peid]->read_LSCFG();
    }
    break; 
  case 3:
    break; 
  case 4: value = mpFastIss_ASTC[peid]->read_ICBKEY();
    break; 
  case 5: value = mpFastIss_ASTC[peid]->read_IFCR();
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_IFCR1();
    break; 
  case 7:
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_BRPCTRL0();
    break; 
  case 9: value = mpFastIss_ASTC[peid]->read_BRPCTRL1();
    break; 
  case 10: value = mpFastIss_ASTC[peid]->read_BRPCFG();
    break; 
  case 11: value = mpFastIss_ASTC[peid]->read_BRPACTRL();
    break; 
  case 12: value = mpFastIss_ASTC[peid]->read_BRPDATA();
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    break; 
  case 17:
    break; 
  case 18:
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break;
  }
  return(value);
}

///<summary>read the system register (selid=13) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid13( unsigned int peid, unsigned int regid )
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_DCBKEY();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_LSUCR();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_LSULNK0();
    break; 
  case 3: value = mpFastIss_ASTC[peid]->read_LSULNK1();
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8: value = mpFastIss_ASTC[peid]->read_L1RLCR();
    break; 
  case 9: value = mpFastIss_ASTC[peid]->read_L1RLNK0();
    break; 
  case 10: value = mpFastIss_ASTC[peid]->read_L1RLNK1();
    break; 
  case 11: 
    break; 
  case 12: value = mpFastIss_ASTC[peid]->read_L1RCFG();
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_DECFG();
    break;
  case 17: value = mpFastIss_ASTC[peid]->read_DECTRL();
    break; 
  case 18: value = mpFastIss_ASTC[peid]->read_DEVDS();
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24: value = mpFastIss_ASTC[peid]->read_RDBCR();
    break; 
  case 25: value = mpFastIss_ASTC[peid]->read_RDBACR();
    break; 
  case 26: value = mpFastIss_ASTC[peid]->read_RDBATAG();
    break; 
  case 27: value = mpFastIss_ASTC[peid]->read_RDBADAT0();
    break; 
  case 28: value = mpFastIss_ASTC[peid]->read_RDBADAT1();
    break; 
  case 29: value = mpFastIss_ASTC[peid]->read_RDBADAT2();
    break; 
  case 30: value = mpFastIss_ASTC[peid]->read_RDBADAT3();
    break; 
  case 31: value = mpFastIss_ASTC[peid]->read_RDBSTAT();
    break; 
  }
  return(value);
}

///<summary>read the system register (selid=14) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid14( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_PMCTRL0();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_PMCTRL1();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_PMCTRL2();
    break;
  case 3: value = mpFastIss_ASTC[peid]->read_PMCTRL3();
    break; 
  case 4: value = mpFastIss_ASTC[peid]->read_PMCTRL4();
    break; 
  case 5: value = mpFastIss_ASTC[peid]->read_PMCTRL5();
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_PMCTRL6();
    break; 
  case 7: value = mpFastIss_ASTC[peid]->read_PMCTRL7();
    break; 
  case 8: 
    break; 
  case 9: 
    break; 
  case 10: 
    break; 
  case 11: 
    break; 
  case 12: 
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16: value = mpFastIss_ASTC[peid]->read_PMCOUNT0();
    break; 
  case 17: value = mpFastIss_ASTC[peid]->read_PMCOUNT1();
    break; 
  case 18: value = mpFastIss_ASTC[peid]->read_PMCOUNT2();
    break; 
  case 19: value = mpFastIss_ASTC[peid]->read_PMCOUNT3();
    break; 
  case 20: value = mpFastIss_ASTC[peid]->read_PMCOUNT4();
    break; 
  case 21: value = mpFastIss_ASTC[peid]->read_PMCOUNT5();
    break; 
  case 22: value = mpFastIss_ASTC[peid]->read_PMCOUNT6();
    break; 
  case 23: value = mpFastIss_ASTC[peid]->read_PMCOUNT7();
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break;
  }
  return(value);
}

///<summary>read the system register (selid=15) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int G4_FastIssIF::readSrSelid15( unsigned int peid, unsigned int regid ) 
{
  unsigned int value = 0;
  switch(regid){
  case 0: value = mpFastIss_ASTC[peid]->read_PMSUBCND0();
    break; 
  case 1: value = mpFastIss_ASTC[peid]->read_PMSUBCND1();
    break; 
  case 2: value = mpFastIss_ASTC[peid]->read_PMSUBCND2();
    break; 
  case 3: value = mpFastIss_ASTC[peid]->read_PMSUBCND3();
    break; 
  case 4: value = mpFastIss_ASTC[peid]->read_PMSUBCND4();
    break; 
  case 5: value = mpFastIss_ASTC[peid]->read_PMSUBCND5();
    break; 
  case 6: value = mpFastIss_ASTC[peid]->read_PMSUBCND6();
    break;
  case 7: value = mpFastIss_ASTC[peid]->read_PMSUBCND7();
    break; 
  case 8: 
    break; 
  case 9: 
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    break; 
  case 17:
    break; 
  case 18:
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break;
  }
  return(value);
}

///<summary>write the system register (selid=0) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid0( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_EIPC(value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_EIPSW(value);
    break; 
  case 2: mpFastIss_ASTC[peid]->write_FEPC(value);
    break; 
  case 3: mpFastIss_ASTC[peid]->write_FEPSW(value);
    break; 
  case 4:
    break; 
  case 5: mpFastIss_ASTC[peid]->write_PSW(value);
    break; 
  case 6: mpFastIss_ASTC[peid]->write_FPSR(value);
    break; 
  case 7: mpFastIss_ASTC[peid]->write_FPEPC(value);
    break; 
  case 8: mpFastIss_ASTC[peid]->write_FPST(value);
    break; 
  case 9: mpFastIss_ASTC[peid]->write_FPCC(value);
    break; 
  case 10: mpFastIss_ASTC[peid]->write_FPCFG(value);
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13: mpFastIss_ASTC[peid]->write_EIIC(value);
    break; 
  case 14: mpFastIss_ASTC[peid]->write_FEIC(value);
    break; 
  case 15:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PSWH(value);
    }
    break; 
  case 16: mpFastIss_ASTC[peid]->write_CTPC(value);
    break; 
  case 17: mpFastIss_ASTC[peid]->write_CTPSW(value);
    break; 
  case 18: 
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_EIPSWH(value);
    }
    break; 
  case 19:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_FEPSWH(value);
    }
    break; 
  case 20: mpFastIss_ASTC[peid]->write_CTBP(value);
    break; 
  case 21: mpFastIss_ASTC[peid]->write_SNZCFG(value);
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28: mpFastIss_ASTC[peid]->write_EIWR(value);
    break; 
  case 29: mpFastIss_ASTC[peid]->write_FEWR(value);
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
}

///<summary>write the system register (selid=1) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid1( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_SPID(value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_SPIDLIST(value);
    break; 
  case 2: mpFastIss_ASTC[peid]->write_RBASE(value);
    break; 
  case 3: mpFastIss_ASTC[peid]->write_EBASE(value);
    break; 
  case 4: mpFastIss_ASTC[peid]->write_INTBP(value);
    break; 
  case 5: mpFastIss_ASTC[peid]->write_MCTL(value);
    break; 
  case 6: mpFastIss_ASTC[peid]->write_PID(value);
    break; 
  case 7:
    break; 
  case 8: mpFastIss_ASTC[peid]->write_SVLOCK(value);
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11: mpFastIss_ASTC[peid]->write_SCCFG(value);
    break; 
  case 12: mpFastIss_ASTC[peid]->write_SCBP(value);
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_HVCFG(value);
    }
    break; 
  case 17:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_GMCFG(value);
    }
    break; 
  case 18:
    break; 
  case 19:
    break; 
  case 20:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_HVSB(value);
    }
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
}

///<summary>write the system register (selid=2) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid2( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_PEID(value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_BMID(value);
    break; 
  case 2:
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6: mpFastIss_ASTC[peid]->write_MEA(value);
    break; 
  case 7:
    break; 
  case 8: mpFastIss_ASTC[peid]->write_MEI(value);
    break; 
  case 9:
    break; 
  case 10: mpFastIss_ASTC[peid]->write_ISPR(value);
    break; 
  case 11: mpFastIss_ASTC[peid]->write_IMSR(value);
    break; 
  case 12: mpFastIss_ASTC[peid]->write_ICSR(value);
    break; 
  case 13: mpFastIss_ASTC[peid]->write_INTCFG(value);
    break; 
  case 14: mpFastIss_ASTC[peid]->write_PLMR(value);
    break; 
  case 15: mpFastIss_ASTC[peid]->write_RBCR0(value);
    break; 
  case 16: mpFastIss_ASTC[peid]->write_RBCR1(value);
    break; 
  case 17: mpFastIss_ASTC[peid]->write_RBNR(value);
    break; 
  case 18: mpFastIss_ASTC[peid]->write_RBIP(value);
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
}

///<summary>write the system register (selid=3) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid3( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_DBGEN(value);
    }
    break; 
  case 1:
    break; 
  case 2:
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8:
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_DBPSWH(value);
    }
    break; 
  case 14:
    break;
  case 15: mpFastIss_ASTC[peid]->write_DBIC(value);
    break; 
  case 16: 
    break; 
  case 17: 
    break; 
  case 18: mpFastIss_ASTC[peid]->write_DBPC(value);
    break; 
  case 19: mpFastIss_ASTC[peid]->write_DBPSW(value);
    break; 
  case 20: mpFastIss_ASTC[peid]->write_DIR0(value);
    break; 
  case 21: mpFastIss_ASTC[peid]->write_DIR1(value);
    break; 
  case 22: mpFastIss_ASTC[peid]->write_BPC(value);
    break; 
  case 23:
    break; 
  case 24: mpFastIss_ASTC[peid]->write_BPAV(value);
    break; 
  case 25: mpFastIss_ASTC[peid]->write_BPAM(value);
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_DBCFG(value);
    }
    break; 
  case 29:
    break; 
  case 30: mpFastIss_ASTC[peid]->write_DBWR(value);
    break; 
  case 31:
    break; 
  }
}

///<summary>write the system register (selid=4) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid4( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0:
    break; 
  case 1:
    break; 
  case 2:
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8:
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13:
    break; 
  case 14:
    break;
  case 15:
    break; 
  case 16: mpFastIss_ASTC[peid]->write_ICTAGL(value);
    break; 
  case 17: mpFastIss_ASTC[peid]->write_ICTAGH(value);
    break; 
  case 18: mpFastIss_ASTC[peid]->write_ICDATL(value);
    break; 
  case 19: mpFastIss_ASTC[peid]->write_ICDATH(value);
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break;
  case 23:
    break; 
  case 24: mpFastIss_ASTC[peid]->write_ICCTRL(value);
    break; 
  case 25:
    break; 
  case 26: mpFastIss_ASTC[peid]->write_ICCFG(value);
    break; 
  case 27:
    break; 
  case 28: mpFastIss_ASTC[peid]->write_ICERR(value);
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
}

///<summary>write the system register (selid=5) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid5( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_MPM(value);
    break; 
  case 1:
    break; 
  case 2: mpFastIss_ASTC[peid]->write_MPCFG(value);
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8: mpFastIss_ASTC[peid]->write_MCA(value);
    break; 
  case 9: mpFastIss_ASTC[peid]->write_MCS(value);
    break; 
  case 10: mpFastIss_ASTC[peid]->write_MCC(value);
    break; 
  case 11: mpFastIss_ASTC[peid]->write_MCR(value);
    break; 
  case 12: mpFastIss_ASTC[peid]->write_MCI(value);
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16: mpFastIss_ASTC[peid]->write_MPIDX(value);
    break; 
  case 17:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_MPBK(value);
    }
    break; 
  case 18:
    break;
  case 19:
    break; 
  case 20: mpFastIss_ASTC[peid]->write_MPLA(value);
    break; 
  case 21: mpFastIss_ASTC[peid]->write_MPUA(value);
    break; 
  case 22: mpFastIss_ASTC[peid]->write_MPAT(value);
    break; 
  case 23:
    break; 
  case 24: mpFastIss_ASTC[peid]->write_MPID0(value);
    break; 
  case 25: mpFastIss_ASTC[peid]->write_MPID1(value);
    break; 
  case 26: mpFastIss_ASTC[peid]->write_MPID2(value);
    break; 
  case 27: mpFastIss_ASTC[peid]->write_MPID3(value);
    break; 
  case 28: mpFastIss_ASTC[peid]->write_MPID4(value);
    break; 
  case 29: mpFastIss_ASTC[peid]->write_MPID5(value);
    break; 
  case 30: mpFastIss_ASTC[peid]->write_MPID6(value);
    break; 
  case 31: mpFastIss_ASTC[peid]->write_MPID7(value);
    break; 
  }
}

///<summary>write the system register (selid=9) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid9( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_GMEIPC(value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_GMEIPSW(value);
    break; 
  case 2: mpFastIss_ASTC[peid]->write_GMFEPC(value);
    break; 
  case 3: mpFastIss_ASTC[peid]->write_GMFEPSW(value);
    break; 
  case 4:
    break; 
  case 5: mpFastIss_ASTC[peid]->write_GMPSW(value);
    break; 
  case 6: mpFastIss_ASTC[peid]->write_GMMEA(value);
    break; 
  case 7:
    break; 
  case 8: mpFastIss_ASTC[peid]->write_GMMEI(value);
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13: mpFastIss_ASTC[peid]->write_GMEIIC(value);
    break; 
  case 14: mpFastIss_ASTC[peid]->write_GMFEIC(value);
    break; 
  case 15:
    break; 
  case 16: mpFastIss_ASTC[peid]->write_GMSPID(value);
    break; 
  case 17: mpFastIss_ASTC[peid]->write_GMSPIDLIST(value);
    break; 
  case 18: 
    break; 
  case 19: mpFastIss_ASTC[peid]->write_GMEBASE(value);
    break; 
  case 20: mpFastIss_ASTC[peid]->write_GMINTBP(value);
    break; 
  case 21: mpFastIss_ASTC[peid]->write_GMINTCFG(value);
    break; 
  case 22: mpFastIss_ASTC[peid]->write_GMPLMR(value);
    break; 
  case 23:
    break; 
  case 24: mpFastIss_ASTC[peid]->write_GMSVLOCK(value);
    break; 
  case 25: mpFastIss_ASTC[peid]->write_GMMPM(value);
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28: mpFastIss_ASTC[peid]->write_GMEIWR(value);
    break; 
  case 29: mpFastIss_ASTC[peid]->write_GMFEWR(value);
    break; 
  case 30: mpFastIss_ASTC[peid]->write_GMPEID(value);
    break; 
  case 31:
    break; 
  }
}

///<summary>write the system register (selid=10) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid10( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0:
    break; 
  case 1:
    break; 
  case 2:
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6: mpFastIss_ASTC[peid]->write_FXSR(value);
    break; 
  case 7:
    break; 
  case 8: mpFastIss_ASTC[peid]->write_FXST(value);
    break; 
  case 9: mpFastIss_ASTC[peid]->write_FXINFO(value);
    break; 
  case 10: mpFastIss_ASTC[peid]->write_FXCFG(value);
    break; 
  case 11:
    break; 
  case 12: mpFastIss_ASTC[peid]->write_FXXC(value);
    break; 
  case 13: mpFastIss_ASTC[peid]->write_FXXP(value);
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    break; 
  case 17:
    break; 
  case 18:
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
}

///<summary>write the system register (selid=11) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid11( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_TSCOUNTL(value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_TSCOUNTH(value);
    break; 
  case 2: mpFastIss_ASTC[peid]->write_TSCTRL(value);
    break; 
  case 3:
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8: mpFastIss_ASTC[peid]->write_PMUMCTRL(value);
    break; 
  case 9:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PMGMCTRL(value);
    }
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PMCOUNT0(value);
    }
    break; 
  case 17:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PMCTRL0(value);
    }
    break; 
  case 18:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PMCOUNT1(value);
    }
    break; 
  case 19:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PMCTRL1(value);
    }
    break; 
  case 20:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PMCOUNT2(value);
    }
    break; 
  case 21:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PMCTRL2(value);
    }
    break; 
  case 22:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PMCOUNT3(value);
    }
    break; 
  case 23:
    if ((mPeType[peid] == CPU_G4MH_1_0) || (mPeType[peid] == CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_PMCTRL3(value);
    }
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break;
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break; 
  }
}

///<summary>write the system register (selid=12) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid12( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_LSTEST0(value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_LSTEST1(value);
    break; 
  case 2:
    if ((mPeType[peid] != CPU_G4MH_1_0) && (mPeType[peid] != CPU_G4MH_1_1)){
      mpFastIss_ASTC[peid]->write_LSCFG(value);
    }
    break; 
  case 3:
    break; 
  case 4: mpFastIss_ASTC[peid]->write_ICBKEY(value);
    break; 
  case 5: mpFastIss_ASTC[peid]->write_IFCR(value);
    break; 
  case 6: mpFastIss_ASTC[peid]->write_IFCR1(value);
    break; 
  case 7:
    break; 
  case 8: mpFastIss_ASTC[peid]->write_BRPCTRL0(value);
    break; 
  case 9: mpFastIss_ASTC[peid]->write_BRPCTRL1(value);
    break; 
  case 10: mpFastIss_ASTC[peid]->write_BRPCFG(value);
    break; 
  case 11: mpFastIss_ASTC[peid]->write_BRPACTRL(value);
    break; 
  case 12: mpFastIss_ASTC[peid]->write_BRPDATA(value);
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    break; 
  case 17:
    break; 
  case 18:
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break;
  }
}

///<summary>write the system register (selid=13) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid13( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_DCBKEY(value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_LSUCR(value);
    break; 
  case 2: mpFastIss_ASTC[peid]->write_LSULNK0(value);
    break; 
  case 3: mpFastIss_ASTC[peid]->write_LSULNK1(value);
    break; 
  case 4:
    break; 
  case 5:
    break; 
  case 6:
    break; 
  case 7:
    break; 
  case 8: mpFastIss_ASTC[peid]->write_L1RLCR(value);
    break; 
  case 9: mpFastIss_ASTC[peid]->write_L1RLNK0(value);
    break; 
  case 10: mpFastIss_ASTC[peid]->write_L1RLNK1(value);
    break; 
  case 11: 
    break; 
  case 12:  mpFastIss_ASTC[peid]->write_L1RCFG(value);
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16: mpFastIss_ASTC[peid]->write_DECFG(value);
    break;
  case 17: mpFastIss_ASTC[peid]->write_DECTRL(value);
    break; 
  case 18: mpFastIss_ASTC[peid]->write_DEVDS(value);
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24: mpFastIss_ASTC[peid]->write_RDBCR(value);
    break; 
  case 25: mpFastIss_ASTC[peid]->write_RDBACR(value);
    break; 
  case 26: mpFastIss_ASTC[peid]->write_RDBATAG(value);
    break; 
  case 27: mpFastIss_ASTC[peid]->write_RDBADAT0(value);
    break; 
  case 28: mpFastIss_ASTC[peid]->write_RDBADAT1(value);
    break; 
  case 29: mpFastIss_ASTC[peid]->write_RDBADAT2(value);
    break; 
  case 30: mpFastIss_ASTC[peid]->write_RDBADAT3(value);
    break; 
  case 31: mpFastIss_ASTC[peid]->write_RDBSTAT(value);
    break; 
  }
}

///<summary>write the system register (selid=14) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid14( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_PMCTRL0(value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_PMCTRL1(value);
    break; 
  case 2: mpFastIss_ASTC[peid]->write_PMCTRL2(value);
    break; 
  case 3: mpFastIss_ASTC[peid]->write_PMCTRL3(value);
    break; 
  case 4: mpFastIss_ASTC[peid]->write_PMCTRL4(value);
    break; 
  case 5: mpFastIss_ASTC[peid]->write_PMCTRL5(value);
    break; 
  case 6: mpFastIss_ASTC[peid]->write_PMCTRL6(value);
    break; 
  case 7: mpFastIss_ASTC[peid]->write_PMCTRL7(value);
    break; 
  case 8:
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16: mpFastIss_ASTC[peid]->write_PMCOUNT0(value);
    break; 
  case 17: mpFastIss_ASTC[peid]->write_PMCOUNT1(value);
    break; 
  case 18: mpFastIss_ASTC[peid]->write_PMCOUNT2(value);
    break; 
  case 19: mpFastIss_ASTC[peid]->write_PMCOUNT3(value);
    break; 
  case 20: mpFastIss_ASTC[peid]->write_PMCOUNT4(value);
    break; 
  case 21: mpFastIss_ASTC[peid]->write_PMCOUNT5(value);
    break; 
  case 22: mpFastIss_ASTC[peid]->write_PMCOUNT6(value);
    break; 
  case 23: mpFastIss_ASTC[peid]->write_PMCOUNT7(value);
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break;
  }
}

///<summary>write the system register (selid=15) value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void G4_FastIssIF::writeSrSelid15( unsigned int peid, unsigned int regid, unsigned int value )
{
  switch(regid){
  case 0: mpFastIss_ASTC[peid]->write_PMSUBCND0(value);
    break; 
  case 1: mpFastIss_ASTC[peid]->write_PMSUBCND1(value);
    break; 
  case 2: mpFastIss_ASTC[peid]->write_PMSUBCND2(value);
    break; 
  case 3: mpFastIss_ASTC[peid]->write_PMSUBCND3(value);
    break; 
  case 4: mpFastIss_ASTC[peid]->write_PMSUBCND4(value);
    break; 
  case 5: mpFastIss_ASTC[peid]->write_PMSUBCND5(value);
    break; 
  case 6: mpFastIss_ASTC[peid]->write_PMSUBCND6(value);
    break; 
  case 7: mpFastIss_ASTC[peid]->write_PMSUBCND7(value);
    break; 
  case 8:
    break; 
  case 9:
    break; 
  case 10:
    break; 
  case 11:
    break; 
  case 12:
    break; 
  case 13:
    break; 
  case 14:
    break; 
  case 15:
    break; 
  case 16:
    break; 
  case 17:
    break; 
  case 18:
    break; 
  case 19:
    break; 
  case 20:
    break; 
  case 21:
    break; 
  case 22:
    break; 
  case 23:
    break; 
  case 24:
    break; 
  case 25:
    break; 
  case 26:
    break; 
  case 27:
    break; 
  case 28:
    break; 
  case 29:
    break; 
  case 30:
    break; 
  case 31:
    break;
  }
}

