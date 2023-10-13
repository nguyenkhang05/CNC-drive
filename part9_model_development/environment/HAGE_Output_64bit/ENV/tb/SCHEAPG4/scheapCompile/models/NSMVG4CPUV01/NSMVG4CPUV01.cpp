/*************************************************************************
 *
 *  NSMVG4CPUV01.cpp
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

#include "systemc.h"
#if defined(_MSC_VER) // for cforestg4x (cforestg4x uses std::min and max)
#undef min
#undef max
#endif

#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno
#include <map>

#include "Common.h"
#include "CmErrMsg.h" // for WinMsg

#include "NSMVG4CPUV01.h"
#include "G4_NoDbgIF.h"
#ifdef VLAB
#include "VlabDbgIF.h"
#endif
#include "Rteserv2Controller.h" // For MULTI
#include "G4_CaIss.h"
#include "G4_intc_typ.h"
#include "CLUSTERSS.h"
#include "I_CONNECTOR.h"
#include "I_BRIDGE.h"
#include "OSCI2DCDR.h"
#include "DummySlave.h"
#include "DummyMaster.h"

#ifndef FASTISS_NO_SUPPORT
#include "G4_FastIssIF.h"
#include "PECORESS.h"
#include "G4_LinkManager.h"
#include "T_SELECTOR.h"
#include "G4_PEGERR_OR.h"
#endif // #ifndef FASTISS_NO_SUPPORT

#if 1 // temporarily
#include "ATLTSLAVE32.h"
#endif

#ifdef VLAB_0
#include "TLMShortcut.h"
#endif


using namespace CmErrMsg; // for WinMsg
using namespace G4IntcTyp;  // for INTC's signal

extern int           token_to_int(char*, const char*, const char*);
extern unsigned int  token_to_uint(char*, const char*, const char*);
extern unsigned long token_to_ulong(char*, const char*, const char*);
extern unsigned long long token_to_ulonglong(char*, const char*, const char*);
extern double        token_to_double(char*, const char*, const char*);
extern unsigned int  token_to_uint_default( char *, unsigned int, const char *, char * );
extern unsigned long long token_to_ulonglong_default(char*, unsigned long long , const char*, const char*);


///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<returns>none</returns>
NSMVG4CPUV01::
FenmiAdapter::FenmiAdapter( sc_module_name module_name ):
  mpG4CPU(NULL),
  mPeId(0),
  mAckVal(false),
  fenmireq_intc1("fenmireq_intc1"),
  fenmiack_intc1("fenmiack_intc1"),
  fenmireq_faiss("fenmireq_faiss"),
  fenmiack_faiss("fenmiack_faiss"),
  ack_caiss_event()
{
  fenmireq_iss_sig = false;
  fenmiack_iss_sig = false;

  fenmireq_faiss(fenmireq_iss_sig);
  fenmiack_faiss(fenmiack_iss_sig);

  SC_METHOD( FenmiReqProcess );
  sensitive << fenmireq_intc1;
  dont_initialize();

  SC_METHOD( FenmiAckProcess );
  sensitive << fenmiack_faiss;
  sensitive << ack_caiss_event;
  dont_initialize();

}


///<summary>Destructor</summary>
///<returns>none</returns>
NSMVG4CPUV01::
FenmiAdapter::~FenmiAdapter( void )
{
}


///<summary>notify the fenmi request from INTC1 to ISS</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
FenmiAdapter::FenmiReqProcess(void)
{
  bool req = fenmireq_intc1.read();
  
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
#ifdef TEST_INTC1_DBG
    printf("NSMVG4CPUV01::FenmiAdapter::FenmiReqProcess(CA) req=%d\n", req);
    fflush(stdout);
#endif
    mpG4CPU->mpNoDbgIF->SetFenmiReq(mPeId, req);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
#ifdef TEST_INTC1_DBG
      printf("%s ", sc_time_stamp().to_string().c_str());
      printf("NSMVG4CPUV01::FenmiAdapter::FenmiReqProcess(FA) req=%d\n", req);
      fflush(stdout);
#endif
      fenmireq_faiss.write(req);
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>notify the acknowledgement of fenmi request from ISS to INTC1</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
FenmiAdapter::FenmiAckProcess(void)
{
  bool cpu2intc_ack;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    cpu2intc_ack = mAckVal;
    mAckVal = false;
    if(cpu2intc_ack){
#ifdef TEST_INTC1_DBG
      printf("NSMVG4CPUV01::FenmiAdapter::FenmiAckProcess ca iss ack done ack=%d\n", cpu2intc_ack);
      fflush(stdout);
#endif
      ack_caiss_event.notify(mpG4CPU->mPeriodCL[mPeId/2], glb_resolution_unit);
    }
  }else{
    cpu2intc_ack = fenmiack_faiss.read();
#ifdef TEST_INTC1_DBG
    printf("NSMVG4CPUV01::FenmiAdapter::FenmiAckProcess fast iss ack done ack=%d\n", cpu2intc_ack);
    fflush(stdout);
#endif
  }
  fenmiack_intc1.write(cpu2intc_ack);
}


///<summary>set PE ID</summary>
///<param name="peid">PE ID</param>
///<returns>none</returns>
void NSMVG4CPUV01::
FenmiAdapter::SetPeId(unsigned int peid)
{
  mPeId = peid;
}


///<summary>set pointer of NSMVG4CPUV01's instance</summary>
///<param name="pG4CPU">pointer of NSMVG4CPUV01's instance</param>
///<returns>none</returns>
void NSMVG4CPUV01::
FenmiAdapter::SetG4cpuPtr(NSMVG4CPUV01* pG4CPU)
{
  mpG4CPU = pG4CPU;
}


///<summary>reset</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
FenmiAdapter::Reset(void)
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    mAckVal = false;
#ifdef TEST_INTC1_DBG
    printf("%s NSMVG4CPUV01::FenmiAdapter::Reset notify ack\n", sc_time_stamp().to_string().c_str());
    fflush(stdout);
#endif
    ack_caiss_event.notify();
  }
}


///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<returns>none</returns>
NSMVG4CPUV01::
FeintAdapter::FeintAdapter( sc_module_name module_name ) :
  mpG4CPU(NULL),
  mPeId(0),
  mAckVal(0),
  feintreq_iss_sig(0),
  feintack_iss_sig(0),
  feintreq_intc1("feintreq_intc1"),
  feintack_intc1("feintack_intc1"),
  feintreq_faiss("feintreq_faiss"),
  feintack_faiss("feintack_faiss"),
  ack_caiss_event()
{

  feintreq_faiss(feintreq_iss_sig);
  feintack_faiss(feintack_iss_sig);

  SC_METHOD( FeintReqProcess );
  sensitive << feintreq_intc1;
  dont_initialize();

  SC_METHOD( FeintAckProcess );
  sensitive << feintack_faiss;
  sensitive << ack_caiss_event;
  dont_initialize();
}


///<summary>Destructor</summary>
///<returns>none</returns>
NSMVG4CPUV01::
FeintAdapter::~FeintAdapter( void )
{
}


///<summary>notify the feint request from INTC1 to ISS</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
FeintAdapter::FeintReqProcess(void)
{
  union FeintReq req;
  req.val = (unsigned int)feintreq_intc1.read();

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
#ifdef TEST_INTC1_DBG
    printf("%s NSMVG4CPUV01::FeintAdapter::FeintReqProcess req to CA req=%d, ch=%d\n", sc_time_stamp().to_string().c_str(), req.bit_val.req, req.bit_val.ch, req.bit_val.gm, req.bit_val.gpid, req.bit_val.bg);
    fflush(stdout);
#endif
    mpG4CPU->mpNoDbgIF->SetFeintReq(mPeId, req.bit_val.req, req.bit_val.ch, req.bit_val.gm, req.bit_val.gpid, req.bit_val.bg);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
#ifdef TEST_INTC1_DBG
      printf("%s NSMVG4CPUV01::FeintAdapter::FeintReqProcess req to FA req=%d, ch=%d\n", sc_time_stamp().to_string().c_str(), req.bit_val.req, req.bit_val.ch, req.bit_val.gm, req.bit_val.gpid, req.bit_val.bg);
      fflush(stdout);
#endif
      feintreq_faiss.write(req.val);
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>notify the acknowledgement of feint request from ISS to INTC1</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
FeintAdapter::FeintAckProcess(void)
{
  unsigned int cpu2intc_ack;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    cpu2intc_ack = mAckVal;
    mAckVal = 0;
    if(cpu2intc_ack){
#ifdef TEST_INTC1_DBG
      printf("%s NSMVG4CPUV01::FeintAdapter::FeintAckProcess ca iss ack done ack=%d\n", sc_time_stamp().to_string().c_str(), cpu2intc_ack);
      fflush(stdout);
#endif
      ack_caiss_event.notify(mpG4CPU->mPeriodCL[mPeId/2], glb_resolution_unit);
    }
  }else{
    cpu2intc_ack = feintack_faiss.read();
#ifdef TEST_INTC1_DBG
    printf("%s NSMVG4CPUV01::FeintAdapter::FeintAckProcess fast iss ack done ack=%d\n", sc_time_stamp().to_string().c_str(), cpu2intc_ack);
    fflush(stdout);
#endif
  }
  feintack_intc1.write(cpu2intc_ack);
}


///<summary>set PE ID</summary>
///<param name="peid">PE ID</param>
///<returns>none</returns>
void NSMVG4CPUV01::
FeintAdapter::SetPeId(unsigned int peid)
{
  mPeId = peid;
}


///<summary>set pointer of NSMVG4CPUV01's instance</summary>
///<param name="pG4CPU">pointer of NSMVG4CPUV01's instance</param>
///<returns>none</returns>
void NSMVG4CPUV01::
FeintAdapter::SetG4cpuPtr(NSMVG4CPUV01* pG4CPU)
{
  mpG4CPU = pG4CPU;
}


///<summary>reset</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
FeintAdapter::Reset(void)
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
#ifdef TEST_INTC1_DBG
    printf("%s NSMVG4CPUV01::FeintAdapter::Reset notify ack\n", sc_time_stamp().to_string().c_str());
    fflush(stdout);
#endif
    mAckVal = 0;
    ack_caiss_event.notify();
  }
}


///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<returns>none</returns>
NSMVG4CPUV01::
EiintAdapter::EiintAdapter( sc_module_name module_name ) :
  mpG4CPU(NULL),
  mPeId(0),
  mAckVal(0),
  eiintreq_iss_sig(0),
  eiintack_iss_sig(0),
  eiintreq_intc1("eiintreq_intc1"),
  eiintack_intc1("eiintack_intc1"),
  eiintreq_faiss("eiintreq_faiss"),
  eiintack_faiss("eiintack_faiss"),
  ack_caiss_event()
{

  eiintreq_faiss(eiintreq_iss_sig);
  eiintack_faiss(eiintack_iss_sig);

  SC_METHOD( EiintReqProcess );
  sensitive << eiintreq_intc1;
  dont_initialize();

  SC_METHOD( EiintAckProcess );
  sensitive << eiintack_faiss;
  sensitive << ack_caiss_event;
  dont_initialize();
}

///<summary>Destructor</summary>
///<returns>none</returns>
NSMVG4CPUV01::
EiintAdapter::~EiintAdapter( void )
{
}


///<summary>notify the eiint request from INTC1 to ISS</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
EiintAdapter::EiintReqProcess(void)
{
  union EiintReq req;
  req.val = (sc_dt::uint64)eiintreq_intc1.read();

  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
#ifdef TEST_INTC1_DBG
#if defined(_MSC_VER)
    printf("%s NSMVG4CPUV01::EiintAdapter::EiintReqProcess req to CA req=0x%I64x, ", sc_time_stamp().to_string().c_str(), req.val);
    printf("PE-ID=%d\n", mPeId);
    fflush(stdout);
#else
    printf("%s NSMVG4CPUV01::EiintAdapter::EiintReqProcess req to CA req=0x%llx, ", sc_time_stamp().to_string().c_str(), req.val);
    printf("PE-ID=%d\n", mPeId);
    fflush(stdout);
#endif
#endif
    mpG4CPU->mpNoDbgIF->SetEiintReq(mPeId,
				    req.bit_val.req,
				    req.bit_val.pri,
				    req.bit_val.ch,
				    req.bit_val.tb, 
				    req.bit_val.gm, 
				    req.bit_val.gpid, 
				    req.bit_val.bg);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
#ifdef TEST_INTC1_DBG
#if defined(_MSC_VER)
      printf("%s NSMVG4CPUV01::EiintAdapter::EiintReqProcess req to FA req=0x%I64x, ", sc_time_stamp().to_string().c_str(), req.val);
      printf("req.pri=%d, ", req.bit_val.pri);
      printf("req.bind=%d, req.gm=%d, req.gpid=%d, req.bg=%d, ", req.bit_val.bind, req.bit_val.gm, req.bit_val.gpid, req.bit_val.bg);
      printf("PE-ID=%d\n", mPeId);
      fflush(stdout);
#else
      printf("%s NSMVG4CPUV01::EiintAdapter::EiintReqProcess req to FA req=0x%llx, ", sc_time_stamp().to_string().c_str(), req.val);
      printf("req.pri=%d, ", req.bit_val.pri);
      printf("req.bind=%d, req.gm=%d, req.gpid=%d, req.bg=%d, ", req.bit_val.bind, req.bit_val.gm, req.bit_val.gpid, req.bit_val.bg);
      printf("PE-ID=%d\n", mPeId);
      fflush(stdout);
#endif
#endif
      eiintreq_faiss.write(req.val);
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}

///<summary>notify the acknowledgement of eiint request from ISS to INTC1</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
EiintAdapter::EiintAckProcess(void)
{
  unsigned int cpu2intc_ack;
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    cpu2intc_ack = mAckVal;
    mAckVal = 0;
    if(cpu2intc_ack != 0){
#ifdef TEST_INTC1_DBG
      printf("%s NSMVG4CPUV01::EiintAdapter::EiintAckProcess ca iss ack done ack=0x%x\n", sc_time_stamp().to_string().c_str(), cpu2intc_ack);
    fflush(stdout);
#endif
      ack_caiss_event.notify(mpG4CPU->mPeriodCL[mPeId/2], glb_resolution_unit);
#ifdef TEST_INTC1_DBG
    }else{
      printf("%s NSMVG4CPUV01::EiintAdapter::EiintAckProcess ca iss ack done ack=0x%x\n", sc_time_stamp().to_string().c_str(), cpu2intc_ack);
    fflush(stdout);
#endif
    }
  }else{
    cpu2intc_ack = eiintack_faiss.read();
#ifdef TEST_INTC1_DBG
    printf("%s NSMVG4CPUV01::EiintAdapter::EiintAckProcess fast iss ack done ack=0x%x\n", sc_time_stamp().to_string().c_str(), cpu2intc_ack);
    fflush(stdout);
#endif
  }
  eiintack_intc1.write(cpu2intc_ack);
}


///<summary>set PE ID</summary>
///<param name="peid">PE ID</param>
///<returns>none</returns>
void NSMVG4CPUV01::
EiintAdapter::SetPeId(unsigned int peid)
{
  mPeId = peid;
}


///<summary>set pointer of NSMVG4CPUV01's instance</summary>
///<param name="pG4CPU">pointer of NSMVG4CPUV01's instance</param>
///<returns>none</returns>
void NSMVG4CPUV01::
EiintAdapter::SetG4cpuPtr(NSMVG4CPUV01* pG4CPU)
{
  mpG4CPU = pG4CPU;
}


///<summary>reset</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
EiintAdapter::Reset(void)
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    mAckVal = 0;
#ifdef TEST_INTC1_DBG
    printf("%s NSMVG4CPUV01::EiintAdapter::Reset notify ack\n", sc_time_stamp().to_string().c_str());
    fflush(stdout);
#endif
    ack_caiss_event.notify();
  }
}


///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<returns>none</returns>
NSMVG4CPUV01::
CpuInitAdapter::CpuInitAdapter( sc_module_name module_name ) :
  mpG4CPU(NULL),
  mPeId(0),
  mIsAutoMode(true),
  mIsStart(true),
  sys_cpinit("sys_cpinit")
{
  // default mode is AUTO (mIsAutoMode=true) and under execution cpu status (mIsStart=true).

  SC_METHOD( CpuStartProcess );
  sensitive << sys_cpinit;
  dont_initialize();
}

///<summary>Destructor</summary>
///<returns>none</returns>
NSMVG4CPUV01::
CpuInitAdapter::~CpuInitAdapter( void )
{
}


///<summary>notify the CPU start request from BOOTCTRL</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
CpuInitAdapter::CpuStartProcess(void)
{
#ifdef TEST_CORE_START_DBG
  printf("%s %s::CpuStartProcess\n", 
	 sc_time_stamp().to_string().c_str(), 
	 this->name());
  fflush(stdout);
#endif
  if(!mIsAutoMode){
#ifdef TEST_CORE_START_DBG
    printf("%s %s::CpuStartProcess(BOOTCTRL mode)\n", 
	   sc_time_stamp().to_string().c_str(), 
	   this->name());
    fflush(stdout);
#endif
    if((sys_cpinit.read()) && !mIsStart){
#ifdef TEST_CORE_START_DBG
      printf("%s %s::CpuStartProcess(sys_cpinit rise and PE%d doesn't start)\n", 
	     sc_time_stamp().to_string().c_str(), 
	     this->name(),
	     mPeId);
      fflush(stdout);
#endif
      mIsStart = true;
      mpG4CPU->mpNoDbgIF->SetExecutionMode(mPeId, mIsStart);
    }
  }
}


///<summary>set CPU start mode</summary>
///<param name="isAuto">CPU start mode (true:AUTO start, false:BOOTCTRL start)</param>
///<returns>none</returns>
void NSMVG4CPUV01::
CpuInitAdapter::SetCpuStartMode(bool isAuto)
{
  mIsAutoMode = isAuto;

#ifdef TEST_CORE_START_DBG
  printf("%s %s::SetCpuStartMode: mIsAutoMode=%d, mIsStart=%d\n", 
	 sc_time_stamp().to_string().c_str(), 
	 this->name(),
	 mIsAutoMode,
	 mIsStart);
  fflush(stdout);
#endif
}


///<summary>set PE ID</summary>
///<param name="peid">PE ID</param>
///<returns>none</returns>
void NSMVG4CPUV01::
CpuInitAdapter::SetPeId(unsigned int peid)
{
  mPeId = peid;
}


///<summary>set pointer of NSMVG4CPUV01's instance</summary>
///<param name="pG4CPU">pointer of NSMVG4CPUV01's instance</param>
///<returns>none</returns>
void NSMVG4CPUV01::
CpuInitAdapter::SetG4cpuPtr(NSMVG4CPUV01* pG4CPU)
{
  mpG4CPU = pG4CPU;
}


///<summary>reset</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
CpuInitAdapter::Reset(void)
{
  // This function is called, when reset_sig rises.
#ifdef TEST_CORE_START_DBG
  printf("%s %s::Reset\n", 
	 sc_time_stamp().to_string().c_str(), 
	 this->name());
  fflush(stdout);
#endif
  if(!mIsAutoMode) {
    mIsStart = false;
    mpG4CPU->mpNoDbgIF->SetExecutionMode(mPeId, mIsStart);
  }
}


///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<returns>none</returns>
NSMVG4CPUV01::
CpuGmInfoAdapter::CpuGmInfoAdapter( sc_module_name module_name ) :
  mpG4CPU(NULL),
  mPeId(0),
  mCpuGmInfo(0),
  cpu_gm_info_sig(0),
  cpu_gm_info_faiss("cpu_gm_info_faiss"),
  cpu_gm_info_intc("cpu_gm_info_intc"),
  cpugminfo_caiss_event()
{

  cpu_gm_info_faiss(cpu_gm_info_sig);

  SC_METHOD( CpuGmInfoProcess );
  sensitive << cpu_gm_info_faiss;
  sensitive << cpugminfo_caiss_event;
  dont_initialize();
}

///<summary>Destructor</summary>
///<returns>none</returns>
NSMVG4CPUV01::
CpuGmInfoAdapter::~CpuGmInfoAdapter( void )
{
}


///<summary>notify the acknowledgement of eiint request from ISS to INTC1</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
CpuGmInfoAdapter::CpuGmInfoProcess(void)
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
#if defined(TEST_INTC1_DBG) || defined(TEST_RESET_DBG)
    union CpuGmInfo temp_info;
    temp_info.val = mCpuGmInfo;
    printf("%s NSMVG4CPUV01::CpuGmInfoAdapter::CpuGmInfoProcess ca iss GM information is changed (GM=0x%x, GPID=0x%x) \n", sc_time_stamp().to_string().c_str(), temp_info.bit_val.cpu_gm, temp_info.bit_val.cpu_gpid);
    fflush(stdout);
#endif
  }else{
    mCpuGmInfo = cpu_gm_info_faiss.read();
#if defined(TEST_INTC1_DBG) || defined(TEST_RESET_DBG)
    union CpuGmInfo temp_info;
    temp_info.val = mCpuGmInfo;
    printf("%s NSMVG4CPUV01::CpuGmInfoAdapter::CpuGmInfoProcess fast iss GM information is changed (GM=0x%x, GPID=0x%x) \n", sc_time_stamp().to_string().c_str(), temp_info.bit_val.cpu_gm, temp_info.bit_val.cpu_gpid);
    fflush(stdout);
#endif
  }
  cpu_gm_info_intc.write(mCpuGmInfo);
}


///<summary>set PE ID</summary>
///<param name="peid">PE ID</param>
///<returns>none</returns>
void NSMVG4CPUV01::
CpuGmInfoAdapter::SetPeId(unsigned int peid)
{
  mPeId = peid;
}


///<summary>set pointer of NSMVG4CPUV01's instance</summary>
///<param name="pG4CPU">pointer of NSMVG4CPUV01's instance</param>
///<returns>none</returns>
void NSMVG4CPUV01::
CpuGmInfoAdapter::SetG4cpuPtr(NSMVG4CPUV01* pG4CPU)
{
  mpG4CPU = pG4CPU;
}


///<summary>reset</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
CpuGmInfoAdapter::Reset(void)
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    mCpuGmInfo = 0;
#if defined(TEST_INTC1_DBG) || defined(TEST_RESET_DBG)
    printf("%s NSMVG4CPUV01::CpuGmInfoAdapter::Reset ca iss GM information\n", sc_time_stamp().to_string().c_str());
    fflush(stdout);
#endif
    cpugminfo_caiss_event.notify();
  }
}


///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<returns>none</returns>
NSMVG4CPUV01::
GrdErrAdapter::GrdErrAdapter( sc_module_name module_name ) :
  mpG4CPU(NULL),
  mPeId(0),
  mErrReqMode(false),
  err_fastiss("err_fastiss"),
  err_req("err_req"),
  err_req_event()
{
  err_fastiss_sig = new SC_SIGNAL(bool)("err_fastiss_sig");
  *err_fastiss_sig = mErrReqMode;
  err_fastiss(*err_fastiss_sig);

  // When cforest's CRG (includeing FastISS access) and PEG detect the guard error, err_req_event is issued.
  // When FastISS's PEG detects guard error, the signal of err_fastiss becomes high.
  SC_METHOD( ErrReqProcess );
  sensitive << err_req_event;
  sensitive << err_fastiss;
  dont_initialize();
}

///<summary>Destructor</summary>
///<returns>none</returns>
NSMVG4CPUV01::
GrdErrAdapter::~GrdErrAdapter( void )
{
}


///<summary>notify the guard error request to ECM or ICUP</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
GrdErrAdapter::ErrReqProcess(void)
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    if(mErrReqMode == false){
      if(mPeId >= PE_MAX_NUM){
	// external master
	err_req_event.notify(mpG4CPU->mPeriodVci, glb_resolution_unit);
      }else{
	// PEn
	err_req_event.notify(mpG4CPU->mPeriodCL[mPeId/2], glb_resolution_unit);
      }
      mErrReqMode = true;
#ifdef TEST_CORE_DBG
      printf("%s %s::ErrReqProcess::output high signal\n", 
	     sc_time_stamp().to_string().c_str(), 
	     this->name());
      fflush(stdout);
#endif
    }else{
      mErrReqMode = false;
#ifdef TEST_CORE_DBG
      printf("%s %s::ErrReqProcess::output low signal\n", 
	     sc_time_stamp().to_string().c_str(), 
	     this->name());
      fflush(stdout);
#endif
    }
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mpG4CPU->GetFastIssMode()){
      //if(mErrReqMode == false){
      if((mErrReqMode == false) || (err_fastiss.read() == true)){
	if(!err_fastiss.read()){
	  if(mPeId >= PE_MAX_NUM){
	    // external master
	    err_req_event.notify(mpG4CPU->mPeriodVci, glb_resolution_unit);
	  }else{
	    // PEn
	    err_req_event.notify(mpG4CPU->mPeriodCL[mPeId/2], glb_resolution_unit);
	  }
	}
	mErrReqMode = true;
      }else{
	mErrReqMode = false;
      }
#ifdef TEST_CORE_DBG
      printf("%s %s::ErrReqProcess::output err signal = %d\n", 
	     sc_time_stamp().to_string().c_str(), 
	     this->name(),
	     mErrReqMode);
      fflush(stdout);
#endif
    }
#endif
  }
  err_req.write(mErrReqMode);
}


///<summary>set PE ID</summary>
///<param name="peid">PE ID</param>
///<returns>none</returns>
void NSMVG4CPUV01::
GrdErrAdapter::SetPeId(unsigned int peid)
{
  mPeId = peid;
}


///<summary>set pointer of NSMVG4CPUV01's instance</summary>
///<param name="pG4CPU">pointer of NSMVG4CPUV01's instance</param>
///<returns>none</returns>
void NSMVG4CPUV01::
GrdErrAdapter::SetG4cpuPtr(NSMVG4CPUV01* pG4CPU)
{
  mpG4CPU = pG4CPU;
}


///<summary>reset</summary>
///<returns>none</returns>
void NSMVG4CPUV01::
GrdErrAdapter::Reset(void)
{
  if(SIM_MODE_CA == mpG4CPU->GetSimMode()){
    mErrReqMode = true;
    err_req_event.notify();
  }
}


///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<param name="config_file">file name of configulation file</param>
///<returns>none</returns>
NSMVG4CPUV01::
NSMVG4CPUV01(sc_module_name module_name, const char *config_file ) : 
  sc_module(module_name),
  mDebuggerType(DBG_NONE),
  mIssMode(SIM_MODE_CA),
  mFastIssMode(FASTISS_NONE),
  mConfigFile(config_file),
  mProgramLoaded(false),
  mAsyncFlag(false),
  mIsOldTraceFormat(false),
  mIsAutoMode(true),
  reset_test_event(),
  mpRtesrv2Ctrl(0),
  mpPegErrOr(0),
  Sys_bus_clk("Sys_bus_clk"),
  VCI_clk("VCI_clk"),
  From_clk("From_clk"),
  Cram_clk("Cram_clk"),
  sys_reset("sys_reset"),
  ish("ish"),
  isgp("isgp"),
  tsf("tsf"),
  tss("tss"),
  tscr("tscr"),
  CramGrderr("CramGrderr"),
  ExM_CramAreaerr("ExM_CramAreaerr"),
  mPeNum(PE_INIT_NUM),
  mClstrNum(1)
{
  char port_name[64];
  char fereq_port_name[64];
  char feack_port_name[64];
  char eireq_port_name[64];
  char eiack_port_name[64];
  char gminfo_port_name[64];
  char femod_name[64];
  char eimod_name[64];
  char gminfo_mod_name[64];
  char cpuinit_mod_name[64];
  char mod_name[64];

#ifdef VLAB  // Slight performance boost by not doing TLM tracing within this component.
  tlm::set_tlm_interception_status(false);
#endif 

#ifdef VLAB_0
  TLMShortcut* pTlmShortcut[PE_MAX_NUM];

  bool tlm_shortcut = false;
  if (getenv("RH850_TLM_SHORTCUT") != NULL)
  {
    tlm_shortcut = true;
    printf("TLM Shortcut Enabled\n");
  }
#endif

  for(int i=0; i<PE_MAX_NUM; i++){
    mPeType[i] = "G4MH_V10";
  }

  read_config_file_PeNum(mConfigFile);
  read_config_file(mConfigFile);

  mpCaIss = new G4_CaIss("CAISS", this);

  InitVal();

  CreateDbgIF();

#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    mpLinkManager = new G4_LinkManager();
    mpLinkManager->SetPeNum(mPeNum);
    for(unsigned int i=0; i<mPeNum; i++){
      mpPeInfo[i].peid_ival = i;
    }
  }
#endif

  // initialize SWDT function, sys_cpinit port
  for(int i=0; i<PE_MAX_NUM; i++){
    mSwd0pcadVal[i] = 0xffffffff;
    sprintf(port_name, "PE%d_swdt_clk", i);
    Swdt_clk[i] = new sc_out<bool>(port_name);
    mIsSwdtClkHi[i] = false;
    sprintf(port_name, "PE%d_sys_cpinit", i);
    sys_cpinit[i] = new sc_in<bool>(port_name);
  }

  for(unsigned int i=0; i<CL_MAX_NUM; i++){
    // create isgp (P-Bus area I/F)
    sprintf(port_name, "CL%d_isgp", i);
    CL_isgp[i] = new TlmInitiatorSocket<BUS_WIDTH_VCI>(port_name);

    // cleate isgc (I-Bus area I/F)
    sprintf(port_name, "CL%d_isgc", i);
    CL_isgc[i] = new TlmInitiatorSocket<BUS_WIDTH_VCI32>(port_name);

    // cleate ish (H-Bus area I/F)
    sprintf(port_name, "CL%d_ish", i);
    CL_ish[i] = new TlmInitiatorSocket<BUS_WIDTH_VCI>(port_name);

    // for PE clock
    sprintf(port_name, "CL%d_sys_clk", i);
    CL_sys_clk[i] = new sc_in<sc_dt::uint64>(port_name);

    if(i < mClstrNum){
      // cleate CLUSRSS
      sprintf(mod_name, "CLUSTERSS_%d", i);
      if((i==0) && (mPeNum == 1)){	
	mpCluster[i] = new CLUSTERSS(mod_name, mConfigFile, 1);
      }else{
	mpCluster[i] = new CLUSTERSS(mod_name, mConfigFile, 2);
      }
      mpMstIsgpDummy[i] = ((DummyMaster<BUS_WIDTH_VCI>*)0);
      mpMstIshDummy[i] = ((DummyMaster<BUS_WIDTH_VCI>*)0);
      mpMstIsgcDummy[i] = ((DummyMaster<BUS_WIDTH_VCI32>*)0);

#ifndef FASTISS_NO_SUPPORT
      if(mFastIssMode != FASTISS_NONE){
	if((i==0) && (mPeNum== 1)){
	  mpFastIssIF->SetFastIssPtr(i*2, mpCluster[i]->mpPECORESS[0]->GetFastIssPtr());
	  mpFastIssIF->SetFastIssMemWrapperPtr(i*2, mpCluster[i]->mpPECORESS[0]->GetFastIssMemWrapperPtr());
	  mpFastIssIF->mpFastIss_ASTC[i*2]->set_link_manager(mpLinkManager);
	}else{
	  mpFastIssIF->SetFastIssPtr(i*2, mpCluster[i]->mpPECORESS[0]->GetFastIssPtr());
	  mpFastIssIF->SetFastIssPtr(i*2+1, mpCluster[i]->mpPECORESS[1]->GetFastIssPtr());
	  mpFastIssIF->SetFastIssMemWrapperPtr(i*2, mpCluster[i]->mpPECORESS[0]->GetFastIssMemWrapperPtr());
	  mpFastIssIF->SetFastIssMemWrapperPtr(i*2+1, mpCluster[i]->mpPECORESS[1]->GetFastIssMemWrapperPtr());
	  mpFastIssIF->mpFastIss_ASTC[i*2]->set_link_manager(mpLinkManager);
	  mpFastIssIF->mpFastIss_ASTC[i*2+1]->set_link_manager(mpLinkManager);
	}
      }
#endif
    }else{
      // cleate Dummy Master
      sprintf(mod_name, "DummyMaster%d_isgp", i);
      mpMstIsgpDummy[i] = new DummyMaster<BUS_WIDTH_VCI>(mod_name);
      sprintf(mod_name, "DummyMaster%d_ish", i);
      mpMstIshDummy[i] = new DummyMaster<BUS_WIDTH_VCI>(mod_name);
      sprintf(mod_name, "DummyMaster%d_isgc", i);
      mpMstIsgcDummy[i] = new DummyMaster<BUS_WIDTH_VCI32>(mod_name);
    }
  }

  for(unsigned int i=0; i<PE_MAX_NUM; i++){
    // cleate isi (INTC1 area I/F)
    sprintf(port_name, "PE%d_isi", i);
    PE_isi[i] = new TlmInitiatorSocket<BUS_WIDTH_VCI32>(port_name);

    if(i < mPeNum){
      // initialize Dummy Slave
      mpSlvDummy_isgp[i] = ((DummySlave<BUS_WIDTH_VCI>*)0);
      mpSlvDummy_isgc[i] = ((DummySlave<BUS_WIDTH_VCI>*)0);
      mpSlvDummy_ish[i] = ((DummySlave<BUS_WIDTH_VCI>*)0);
      mpSlvDummy_isi[i] = ((DummySlave<BUS_WIDTH_VCI>*)0);
      mpMstIsiDummy[i] = ((DummyMaster<BUS_WIDTH_VCI32>*)0);
    }else{
      // cleate Dummy Slave
      sprintf(mod_name, "DummySlave%d_isgp", i);
      mpSlvDummy_isgp[i] = new DummySlave<BUS_WIDTH_VCI>(mod_name);
      sprintf(mod_name, "DummySlave%d_isgc", i);
      mpSlvDummy_isgc[i] = new DummySlave<BUS_WIDTH_VCI>(mod_name);
      sprintf(mod_name, "DummySlave%d_ish", i);
      mpSlvDummy_ish[i] = new DummySlave<BUS_WIDTH_VCI>(mod_name);
      sprintf(mod_name, "DummySlave%d_isi", i);
      mpSlvDummy_isi[i] = new DummySlave<BUS_WIDTH_VCI>(mod_name);
      sprintf(mod_name, "DummyMaster%d_isi", i);
      mpMstIsiDummy[i] = new DummyMaster<BUS_WIDTH_VCI32>(mod_name);
    }
  }

  // cleate isgp connector
  mpIsgpConnector = new I_CONNECTOR<BUS_WIDTH_VCI>("isgp_bw_decoder");
  
  // cleate ish connector
  mpIshConnector = new I_CONNECTOR<BUS_WIDTH_VCI>("ish_bw_decoder");

  // cleate CL0_isgc connector
  mpIsgcConnector = new I_CONNECTOR<BUS_WIDTH_VCI32>("isgc_bw_decoder");

  // cleate bridge for isgp
  mpIsgcBridge = new I_BRIDGE<BUS_WIDTH_VCI32, BUS_WIDTH_VCI>("isgc_bridge");

  // cleate router (for VCI ) selector
  mpTssRouter = new OSCI2DCDR<BUS_WIDTH_VCI> ("tss_router", mCaissRouterMapFname);

#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    // cleate VCI I/F target selector
    mpTssSelector = new T_SELECTOR<BUS_WIDTH_VCI, BUS_WIDTH_VCI32>("tss_selector");
    // cleate cluster RAM I/F target selector
    mpTscrSelector = new T_SELECTOR<BUS_WIDTH_CRAM, BUS_WIDTH_VCI32>("tscr_selector");
    // cleate flash I/F target selector
    mpTsfSelector = new T_SELECTOR<BUS_WIDTH_FLSH, BUS_WIDTH_VCI32>("tsf_selector");
    // cleate router (for VCI ) selector
    mpIsRouter = new OSCI2DCDR<BUS_WIDTH_VCI32> ("is_router", mFastissRouterMapFname);
    // cleate bridge for tscr
    mpTscrBridge = new I_BRIDGE<BUS_WIDTH_CRAM, BUS_WIDTH_VCI32>("tscr_bridge");
    // cleate bridge for isgp
    mpIsgpBridge = new I_BRIDGE<BUS_WIDTH_VCI, BUS_WIDTH_VCI32>("isgp_bridge");
    // cleate bridge for ish
    mpIshBridge = new I_BRIDGE<BUS_WIDTH_VCI, BUS_WIDTH_VCI32>("ish_bridge");
  }
#endif // #ifndef FASTISS_NO_SUPPORT

  // connection
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    mpTscrSelector->setTlmTransType(mIssMode);
    mpTsfSelector->setTlmTransType(mIssMode);
    mpTssSelector->setTlmTransType(mIssMode);
    // connection
    tscr(mpTscrSelector->ts);
    (mpTscrSelector->isa)(*mpCaIss->tscr);
    (mpTscrSelector->isl)(mpIsRouter->ts);
    tsf(mpTsfSelector->ts);
    (mpTsfSelector->isa)(*mpCaIss->tsf);
    (mpTsfSelector->isl)(mpIsRouter->ts);
    tss(mpTssSelector->ts);
    (mpTssSelector->isa)(mpTssRouter->ts);
    (mpTssSelector->isl)(mpIsRouter->ts);
    (mpIsRouter->is)(mpTscrBridge->ts);
    (mpTscrBridge->is)(*mpCaIss->tscr);
    (mpIsRouter->is)(mpIsgpBridge->ts);
    (mpIsgpBridge->is)(mpIsgpConnector->ts);
    (mpIsRouter->is)(mpIshBridge->ts);
    (mpIshBridge->is)(mpIshConnector->ts);
    (mpIsRouter->is)(mpIsgcConnector->ts);

    for(unsigned int i=0; i<mClstrNum; i++){
      (*((mpCluster[i])->is))(mpIsRouter->ts);
      (mpIsRouter->is)(*((mpCluster[i])->ts));
    }
  }else{
#endif // #ifndef FASTISS_NO_SUPPORT
    tscr(*mpCaIss->tscr);
    tsf(*mpCaIss->tsf);
    tss(mpTssRouter->ts);
#ifndef FASTISS_NO_SUPPORT
  }
#endif // #ifndef FASTISS_NO_SUPPORT

  for(unsigned int i=0; i<PE_MAX_NUM; i++){
    if(i < mPeNum){
      // connect caiss's initiator with connector's target of CLUSTERSS
      (*((mpCaIss->PE_isgp)[i]))(mpCluster[i/2]->mpIsgpConnector->ts);
      (*((mpCaIss->PE_isgc)[i]))(mpCluster[i/2]->mpIsgcConnector->ts);
      (*((mpCaIss->PE_ish)[i]))(mpCluster[i/2]->mpIshConnector->ts);
      (*((mpCaIss->PE_isi)[i]))(mpCluster[i/2]->mpIsiConnector[i%2]->ts);
    }else{
      (*((mpCaIss->PE_isgp)[i]))(mpSlvDummy_isgp[i]->ts);
      (*((mpCaIss->PE_isgc)[i]))(mpSlvDummy_isgc[i]->ts);
      (*((mpCaIss->PE_ish)[i]))(mpSlvDummy_ish[i]->ts);
      (*((mpCaIss->PE_isi)[i]))(mpSlvDummy_isi[i]->ts);
    }
    (*((mpCaIss->PE_sys_clk)[i]))(*(CL_sys_clk[i/2]));

    // for fenmireq and ack
    sprintf(femod_name, "PE%d_FenmiAdapter", i);
    mpFenmiAdapter[i] = new FenmiAdapter( femod_name );
    mpFenmiAdapter[i]->SetPeId(i);
    mpFenmiAdapter[i]->SetG4cpuPtr(this);
    sprintf(port_name, "PE%d_FenmiReq", i);
    PE_FenmiReq[i] = new sc_in<bool>(port_name);
    sprintf(port_name, "PE%d_FenmiAck", i);
    PE_FenmiAck[i] = new sc_out<bool>(port_name);
    (mpFenmiAdapter[i]->fenmireq_intc1)(*((PE_FenmiReq)[i]));
    (mpFenmiAdapter[i]->fenmiack_intc1)(*((PE_FenmiAck)[i]));
    if(i/2 < mClstrNum){
      (*((mpCluster[i/2])->PE_FenmiReq[i%2]))(mpFenmiAdapter[i]->fenmireq_faiss);
      (*((mpCluster[i/2])->PE_FenmiAck[i%2]))(mpFenmiAdapter[i]->fenmiack_iss_sig);
    }

    // for feintreq and ack, eiint and ack
    sprintf(femod_name, "PE%d_FeintAdapter", i);
    sprintf(eimod_name, "PE%d_EiintAdapter", i);
    sprintf(cpuinit_mod_name, "PE%d_CpuInitAdapter", i);
    sprintf(gminfo_mod_name, "PE%d_CpuGmInfoAdapter", i);
    sprintf(fereq_port_name, "PE%d_FeintReq", i);
    sprintf(eireq_port_name, "PE%d_EiintReq", i);
    sprintf(feack_port_name, "PE%d_FeintAck", i);
    sprintf(eiack_port_name, "PE%d_EiintAck", i);
    sprintf(gminfo_port_name, "PE%d_CpuGmInfo", i);

    mpFeintAdapter[i] = new FeintAdapter( femod_name );
    mpFeintAdapter[i]->SetPeId(i);
    mpFeintAdapter[i]->SetG4cpuPtr(this);

    mpEiintAdapter[i] = new EiintAdapter( eimod_name );
    mpEiintAdapter[i]->SetPeId(i);
    mpEiintAdapter[i]->SetG4cpuPtr(this);

    mpCpuInitAdapter[i] = new CpuInitAdapter( cpuinit_mod_name );
    mpCpuInitAdapter[i]->SetCpuStartMode(mIsAutoMode);
    mpCpuInitAdapter[i]->SetPeId(i);
    mpCpuInitAdapter[i]->SetG4cpuPtr(this);

    mpCpuGmInfoAdapter[i] = new CpuGmInfoAdapter( gminfo_mod_name );
    mpCpuGmInfoAdapter[i]->SetPeId(i);
    mpCpuGmInfoAdapter[i]->SetG4cpuPtr(this);

    PE_FeintReq[i] = new sc_in<unsigned int>(fereq_port_name);
    PE_EiintReq[i] = new sc_in<sc_dt::uint64>(eireq_port_name);
    PE_FeintAck[i] = new sc_out<unsigned int>(feack_port_name);
    PE_EiintAck[i] = new sc_out<unsigned int>(eiack_port_name);
    PE_CpuGmInfo[i] = new sc_out<unsigned int>(gminfo_port_name);
    
    (mpFeintAdapter[i]->feintreq_intc1)(*((PE_FeintReq)[i]));
    (mpFeintAdapter[i]->feintack_intc1)(*((PE_FeintAck)[i]));
    (mpEiintAdapter[i]->eiintreq_intc1)(*((PE_EiintReq)[i]));
    (mpEiintAdapter[i]->eiintack_intc1)(*((PE_EiintAck)[i]));
    (mpCpuInitAdapter[i]->sys_cpinit)(*((sys_cpinit)[i]));
    (mpCpuGmInfoAdapter[i]->cpu_gm_info_intc)(*((PE_CpuGmInfo)[i]));
    if(i/2 < mClstrNum){
      (*((mpCluster[i/2])->PE_FeintReq[i%2]))(mpFeintAdapter[i]->feintreq_faiss);
      (*((mpCluster[i/2])->PE_FeintAck[i%2]))(mpFeintAdapter[i]->feintack_iss_sig);
      (*((mpCluster[i/2])->PE_EiintReq[i%2]))(mpEiintAdapter[i]->eiintreq_faiss);
      (*((mpCluster[i/2])->PE_EiintAck[i%2]))(mpEiintAdapter[i]->eiintack_iss_sig);
      (*((mpCluster[i/2])->PE_CpuGmInfo[i%2]))(mpCpuGmInfoAdapter[i]->cpu_gm_info_sig);
    }
  }

  for(unsigned int i=0; i<CL_MAX_NUM; i++){
    if(i < mClstrNum){
      // connect caiss's initiator with connector's target
      if(i==0){
        (mpCluster[i]->isgc)(mpIsgcConnector->ts);
        (mpIsgcConnector->is)(*(CL_isgc[i]));
      }else{
        (mpCluster[i]->isgc)(*(CL_isgc[i]));
      }
      (mpCluster[i]->isgp)(*(CL_isgp[i]));
      (mpCluster[i]->ish)(*(CL_ish[i]));
      (*((mpCluster[i])->PE_isi[0]))(*(PE_isi[i*2]));
      if(mPeNum == 1){
	(mpMstIsiDummy[1]->is)(*(PE_isi[1]));
      }else{
	(*((mpCluster[i])->PE_isi[1]))(*(PE_isi[(i*2)+1]));
      }
      // connect clock and reset port
      (mpCluster[i]->sys_clk)(*(CL_sys_clk[i]));
      (mpCluster[i]->sys_reset)(sys_reset);
    }else{
      (mpMstIsgpDummy[i]->is)(*(CL_isgp[i]));
      (mpMstIsgcDummy[i]->is)(*(CL_isgc[i]));
      (mpMstIshDummy[i]->is)(*(CL_ish[i]));
      (mpMstIsiDummy[i*2]->is)(*(PE_isi[i*2]));
      (mpMstIsiDummy[(i*2)+1]->is)(*(PE_isi[(i*2)+1]));
    }
  }

  (mpTssRouter->is)(*mpCaIss->tss);
  (mpTssRouter->is)(mpIshConnector->ts);
  (mpTssRouter->is)(mpIsgpConnector->ts);
  (mpTssRouter->is)(mpIsgcBridge->ts);
  (mpIsgcBridge->is)(mpIsgcConnector->ts);
  mpIshConnector->is(ish);
  mpIsgpConnector->is(isgp);
  
  mpCaIss->From_clk(From_clk);
  mpCaIss->Cram_clk(Cram_clk);
  mpCaIss->sys_reset(sys_reset);

#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    // create reset signal for high active
    hi_activ_reset_sig = new SC_SIGNAL(bool)("hi_activ_reset_sig");
    (*hi_activ_reset_sig) = false;
  }
#endif

  // for ECM
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    mpPegErrOr = new G4_PEGERR_OR("g4_pegerr_or");
  }
#endif
  for(int i=0; i<PE_MAX_NUM; i++){
    sprintf(port_name, "PE%d_CramAreaerr", i);
    PE_CramAreaerr[i] = new sc_out<bool>(port_name);
    sprintf(port_name, "PE%d_PegErrM", i);
    PE_PegErrM[i] = new sc_out<bool>(port_name);
    sprintf(port_name, "PE%d_PegErrS", i);
    PE_PegErrS[i] = new sc_out<bool>(port_name);
#ifndef FASTISS_NO_SUPPORT
    if(mFastIssMode != FASTISS_NONE){
      sprintf(port_name, "PE%d_PegInfoOut_sig", i);
      mpPegInfoOut_sig[i] = new SC_SIGNAL(G4_PEG_Cerrinfo)(port_name);
      sprintf(port_name, "PE%d_PegInfoIn_sig", i);
      mpPegInfoIn_sig[i] = new SC_SIGNAL(G4_PEG_Cerrinfo)(port_name);
    }else{
      mpPegInfoOut_sig[i] = 0;
      mpPegInfoIn_sig[i] = 0;
    }
#else
    mpPegInfoOut_sig[i] = 0;
    mpPegInfoIn_sig[i] = 0;
#endif
  }
  for(unsigned int i=0; i<mPeNum; i++){
    // for CRAM Illegal access(master:PE)
    sprintf(mod_name, "PE%d_CramAreaerrAdapter", i);
    mpPeCramAreaerrAdapter[i] = new GrdErrAdapter(mod_name);
    mpPeCramAreaerrAdapter[i]->SetPeId(i);
    mpPeCramAreaerrAdapter[i]->SetG4cpuPtr(this);
    // for PEG error(master:PE)
    sprintf(mod_name, "PE%d_PegErrMAdapter", i);
    mpPePegErrMAdapter[i] = new GrdErrAdapter(mod_name);
    mpPePegErrMAdapter[i]->SetPeId(i);
    mpPePegErrMAdapter[i]->SetG4cpuPtr(this);
    sprintf(mod_name, "PE%d_PegErrSAdapter", i);
    mpPePegErrSAdapter[i] = new GrdErrAdapter(mod_name);
    mpPePegErrSAdapter[i]->SetPeId(i);
    mpPePegErrSAdapter[i]->SetG4cpuPtr(this);
    // for Connection
    (mpPeCramAreaerrAdapter[i]->err_req)(*((PE_CramAreaerr)[i]));
    (mpPePegErrMAdapter[i]->err_req)(*((PE_PegErrM)[i]));
    (mpPePegErrSAdapter[i]->err_req)(*((PE_PegErrS)[i]));
  }
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){ 
    for(unsigned int i=0; i<PE_MAX_NUM; i++){
      if(i<mPeNum){
	(*((mpCluster[i/2])->PE_PegErrM[i%2]))(*(mpPePegErrMAdapter[i]->err_fastiss_sig));
	(*((mpCluster[i/2])->PE_PegErrS[i%2]))(*(mpPePegErrSAdapter[i]->err_fastiss_sig));
	(*((mpCluster[i/2])->PE_PegInfoIn[i%2]))(*mpPegInfoIn_sig[i]);
	(*(mpPegErrOr->PE_PegInfoOut[i]))(*mpPegInfoIn_sig[i]);
	(*((mpCluster[i/2])->PE_PegInfoOut[i%2]))(*mpPegInfoOut_sig[i]);
	(*(mpPegErrOr->PE_PegInfoIn[i]))(*mpPegInfoOut_sig[i]);
      }else{
	(*(mpPegErrOr->PE_PegInfoOut[i]))(*mpPegInfoIn_sig[i]);
	(*(mpPegErrOr->PE_PegInfoIn[i]))(*mpPegInfoOut_sig[i]);
      }
      (*(mpPegErrOr->PE_sys_clk[i]))(*(CL_sys_clk[i/2]));
    }
  }
#endif
  // for CRAM Illegal access(master:external)
  mpExMCramAreaerrAdapter = new GrdErrAdapter("ExMCramAreaerrAdapter");
  mpExMCramAreaerrAdapter->SetPeId(0xffffffff);
  mpExMCramAreaerrAdapter->SetG4cpuPtr(this);
  // for CRAM guard error(master:external and each PE)
  mpCramGrderrAdapter = new GrdErrAdapter("CramGrderrAdapter");
  mpCramGrderrAdapter->SetG4cpuPtr(this);
  (mpExMCramAreaerrAdapter->err_req)(ExM_CramAreaerr);
  (mpCramGrderrAdapter->err_req)(CramGrderr);

#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    // Initalize FastIssIF
    mpNoDbgIF->InitFastIssIF();
    for(unsigned int i=0; i<mClstrNum; i++){
      if(mPeNum==1){
	mpCluster[i]->SetPeId(mpPeInfo[i].peid_ival, 0xffffffff);
      }else{
	mpCluster[i]->SetPeId(mpPeInfo[i*2].peid_ival, mpPeInfo[i*2+1].peid_ival);
      }
    }
  }
#endif

  SC_METHOD(ChangeFreqClstr0);
  sensitive << *(CL_sys_clk)[0];

  SC_METHOD(ChangeFreqClstr1);
  sensitive << *(CL_sys_clk)[1];

  SC_METHOD(ChangeFreqClstr2);
  sensitive << *(CL_sys_clk)[2];

  SC_METHOD(ChangeFreqClstr3);
  sensitive << *(CL_sys_clk)[3];

  SC_METHOD(ChangeFreqSysBus);
  sensitive << Sys_bus_clk;
  
  SC_METHOD(ChangeFreqVci);
  sensitive << VCI_clk;

  SC_METHOD(ChangeFreqFrom);
  sensitive << From_clk;

  SC_METHOD(ChangeFreqCram);
  sensitive << Cram_clk;

  SC_METHOD(reset);
  sensitive << sys_reset;
  dont_initialize();

  SC_METHOD(reset_test);
  sensitive << reset_test_event;
  dont_initialize();

  SC_METHOD(SwdtClkOutPe0);
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    sensitive << mpFastIssIF->mpFastIss_ASTC[0]->swd0pcad_exec_done_event;
  }
#endif // #ifndef FASTISS_NO_SUPPORT
  sensitive << swdt_clk_out_event[0];
  dont_initialize();

  SC_METHOD(SwdtClkOutPe1);
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    if(mPeNum>=2){
      sensitive << mpFastIssIF->mpFastIss_ASTC[1]->swd0pcad_exec_done_event;
    }
  }
#endif // #ifndef FASTISS_NO_SUPPORT
  sensitive << swdt_clk_out_event[1];
  dont_initialize();

  SC_METHOD(SwdtClkOutPe2);
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    if(mPeNum>=3){
      sensitive << mpFastIssIF->mpFastIss_ASTC[2]->swd0pcad_exec_done_event;
    }
  }
#endif // #ifndef FASTISS_NO_SUPPORT
  sensitive << swdt_clk_out_event[2];
  dont_initialize();

  SC_METHOD(SwdtClkOutPe3);
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    if(mPeNum>=4){
      sensitive << mpFastIssIF->mpFastIss_ASTC[3]->swd0pcad_exec_done_event;
    }
  }
#endif // #ifndef FASTISS_NO_SUPPORT
  sensitive << swdt_clk_out_event[3];
  dont_initialize();

  SC_METHOD(SwdtClkOutPe4);
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    if(mPeNum>=5){
      sensitive << mpFastIssIF->mpFastIss_ASTC[4]->swd0pcad_exec_done_event;
    }
  }
#endif // #ifndef FASTISS_NO_SUPPORT
  sensitive << swdt_clk_out_event[4];
  dont_initialize();

  SC_METHOD(SwdtClkOutPe5);
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    if(mPeNum>=6){
      sensitive << mpFastIssIF->mpFastIss_ASTC[5]->swd0pcad_exec_done_event;
    }
  }
#endif // #ifndef FASTISS_NO_SUPPORT
  sensitive << swdt_clk_out_event[5];
  dont_initialize();

  SC_METHOD(SwdtClkOutPe6);
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    if(mPeNum>=7){
      sensitive << mpFastIssIF->mpFastIss_ASTC[6]->swd0pcad_exec_done_event;
    }
  }
#endif // #ifndef FASTISS_NO_SUPPORT
  sensitive << swdt_clk_out_event[6];
  dont_initialize();

  SC_METHOD(SwdtClkOutPe7);
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    if(mPeNum>=8){
      sensitive << mpFastIssIF->mpFastIss_ASTC[7]->swd0pcad_exec_done_event;
    }
  }
#endif // #ifndef FASTISS_NO_SUPPORT
  sensitive << swdt_clk_out_event[7];
  dont_initialize();

  SC_THREAD( execProcess );

#ifdef VLAB  // Slight performance boost by not doing TLM tracing within this component.
  tlm::set_tlm_interception_status(true);
#endif 
}


///<summary>Destructor</summary>
///<returns>none</returns>
NSMVG4CPUV01::~NSMVG4CPUV01(void){
  delete mpCaIss;

  switch(mDebuggerType){
  case DBG_MULTI:
    delete mpRtesrv2Ctrl; // For MULTI
    break;
  case DBG_CUBESUITE:
    break;
  default:
    delete mpNoDbgIF;
  }

#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    delete mpLinkManager;
    delete mpTssSelector;
    delete mpTscrSelector;
    delete mpTsfSelector;
    delete mpIsRouter;
    delete mpTscrBridge;
    delete mpIsgpBridge;
    delete mpIshBridge;
    delete hi_activ_reset_sig;
    delete mpPegErrOr;

    for(int i=0; i<PE_MAX_NUM; i++){
      delete mpPegInfoOut_sig[i];
      delete mpPegInfoIn_sig[i];
    }
  }
#endif

  delete mpIsgpConnector;
  delete mpIshConnector;
  delete mpIsgcConnector;
  delete mpIsgcBridge;
  delete mpTssRouter;
  delete mpExMCramAreaerrAdapter;
  delete mpCramGrderrAdapter;

  for(unsigned int i=0; i<PE_MAX_NUM; i++){
    delete Swdt_clk[i];
    delete PE_isi[i];
    delete mpFenmiAdapter[i];
    delete PE_FenmiReq[i];
    delete PE_FenmiAck[i];
    delete mpFeintAdapter[i];
    delete PE_FeintReq[i];
    delete PE_EiintReq[i];
    delete mpEiintAdapter[i];
    delete PE_FeintAck[i];
    delete PE_EiintAck[i];
    delete mpCpuGmInfoAdapter[i];
    delete mpCpuInitAdapter[i];
    delete PE_CpuGmInfo[i];
    delete PE_CramAreaerr[i];
    delete PE_PegErrM[i];
    delete PE_PegErrS[i];

    if(i < mPeNum){
    }else{
      delete mpSlvDummy_isgp[i];
      delete mpSlvDummy_isgc[i];
      delete mpSlvDummy_ish[i];
      delete mpSlvDummy_isi[i];
      delete mpMstIsiDummy[i];
    }
  }

  for(unsigned int i=0; i<mPeNum; i++){
    delete mpPeCramAreaerrAdapter[i];
    delete mpPePegErrMAdapter[i];
    delete mpPePegErrSAdapter[i];
  }

  for(unsigned int i=0; i<CL_MAX_NUM; i++){
    delete CL_isgp[i];
    delete CL_isgc[i];
    delete CL_ish[i];
    delete CL_sys_clk[i];

    if(i < mClstrNum){
      delete mpCluster[i];
    }else{
      delete mpMstIsgpDummy[i];
      delete mpMstIshDummy[i];
      delete mpMstIsgcDummy[i];
    }
  }

}

///<summary>get the simulation mode (CA or FastISS)</summary>
///<returns>simulation mode (SIM_MODE_CA or SIM_MODE_FAST)</returns>
SIM_MODE_T NSMVG4CPUV01::GetSimMode( void )
{
  return mIssMode;
}


///<summary>get the debugger mode(None or MULTI or CubeSuite+)</summary>
///<returns>debugger mode (DBG_NONE or DBG_MULTI or DBG_CUBESUITE)</returns>
NSMVG4CPUV01::DBG_KIND_T NSMVG4CPUV01::GetDbgMode( void )
{
  return mDebuggerType;
}


///<summary>get the PE type</summary>
///<param name="pe_id">PE ID</param>
///<returns>PE type (G4MH)</returns>
std::string NSMVG4CPUV01::GetPeType( unsigned int pe_id )
{
  return(mPeType[pe_id]);
}


///<summary>get the FastIss moode (NONE or ASTC)</summary>
///<returns>FastIss mode</returns>
FASTISS_MODE_T NSMVG4CPUV01::GetFastIssMode( void )
{
  return mFastIssMode;
}


///<summary>Initialize the member values</summary>
///<returns>none</returns>
void NSMVG4CPUV01::InitVal( void )
{

  mPeriodVci  = +HUGE_VAL;
  mPeriodFrom = +HUGE_VAL;
  mPeriodCram = +HUGE_VAL;

  for(int i=0; i<CL_MAX_NUM; i++){
    mPeriodCL[i] = +HUGE_VAL;
  }
}


///<summary>Set Fenmi's Ack from CAISS</summary>
///<param name="pe_id">PE ID</param>
///<param name="int_ack">Ack value</param>
///<returns>None</returns>
void NSMVG4CPUV01::SetFenmiAckFromCa( unsigned int pe_id, bool int_ack )
{
#ifdef TEST_INTC1_DBG
  printf("NSMVG4CPUV01::SetFenmiAckFromCa notify ack\n");
  fflush(stdout);
#endif
  mpFenmiAdapter[pe_id]->mAckVal = int_ack;
  mpFenmiAdapter[pe_id]->ack_caiss_event.notify(mPeriodCL[pe_id/2], glb_resolution_unit);
}


///<summary>Set Feint's Ack from CAISS</summary>
///<param name="pe_id">PE ID</param>
///<param name="int_ack">Ack value</param>
///<param name="int_ch">Channel value</param>
///<returns>None</returns>
void NSMVG4CPUV01::SetFeintAckFromCa( unsigned int pe_id, bool int_ack, unsigned int int_ch )
{
#ifdef TEST_INTC1_DBG
  printf("NSMVG4CPUV01::SetFeintAckFromCa notify ack\n");
  fflush(stdout);
#endif
  union FeintAck ack;
  ack.bit_val.ack = int_ack;
  ack.bit_val.ch  = int_ch;
  mpFeintAdapter[pe_id]->mAckVal = ack.val;
  mpFeintAdapter[pe_id]->ack_caiss_event.notify(mPeriodCL[pe_id/2], glb_resolution_unit);
}


///<summary>Set Eiint's Ack from CAISS</summary>
///<param name="pe_id">PE ID</param>
///<param name="int_ack">Ack value</param>
///<param name="int_ch">Channel value</param>
///<returns>None</returns>
void NSMVG4CPUV01::SetEiintAckFromCa( unsigned int pe_id, bool int_ack, unsigned int int_ch)
{
#ifdef TEST_INTC1_DBG
  printf("NSMVG4CPUV01::SetEiintAckFromCa notify ack\n");
  fflush(stdout);
#endif
  union EiintAck ack;
  ack.bit_val.ack = int_ack;
  ack.bit_val.ch  = int_ch;
  mpEiintAdapter[pe_id]->mAckVal = ack.val;
  mpEiintAdapter[pe_id]->ack_caiss_event.notify(mPeriodCL[pe_id/2], glb_resolution_unit);
}


///<summary>Set CPU GM information from CAISS</summary>
///<param name="pe_id">PE ID</param>
///<param name="cpu_gm">CPU mode</param>
///<param name="cpu_gpid">CPU GPID</param>
///<returns>None</returns>
void NSMVG4CPUV01::SetCpuGmInfoFromCa( unsigned int pe_id, bool cpu_gm, unsigned int cpu_gpid)
{
#ifdef TEST_INTC1_DBG
  printf("NSMVG4CPUV01::SetCpuGmInfoFromCa GM info is changed\n");
  fflush(stdout);
#endif
  union CpuGmInfo gminfo;
  gminfo.bit_val.cpu_gm   = cpu_gm;
  gminfo.bit_val.cpu_gpid = cpu_gpid;
  mpCpuGmInfoAdapter[pe_id]->mCpuGmInfo = gminfo.val;
  mpCpuGmInfoAdapter[pe_id]->cpugminfo_caiss_event.notify(mPeriodCL[pe_id/2], glb_resolution_unit);
}


///<summary>Set PC match event from CAISS(for SWDT)</summary>
///<param name="pe_id">PE ID(reserved:for binding each PE to each SWDT)</param>
///<returns>None</returns>
void NSMVG4CPUV01::SetSwdtClkOutEvFromCa( unsigned int pe_id )
{
  if(pe_id < PE_MAX_NUM){
    if((pe_id == 0) && 
       ((mPeType[pe_id] == "G4MH") || (mPeType[pe_id] == "G4MH_V10") || (mPeType[pe_id] == "G4MH_V11") || (mPeType[pe_id] == "G4MH_V20"))){
      swdt_clk_out_event[pe_id].notify();
    }
  }
}


///<summary>Set Guard error event from CAISS(for Guard)</summary>
///<param name="pe_id">PE ID</param>
///<param name="error_type">Guard error type</param>
///<returns>None</returns>
void NSMVG4CPUV01::SetGrdErrFromCa( unsigned int pe_id, GRD_ERR_SRC_T error_type )
{
#ifdef TEST_CORE_DBG
  printf("%s NSMVG4CPUV01::SetGrdErrFromCa::peid=0x%08x, error=%d\n", 
	 sc_time_stamp().to_string().c_str(), pe_id, error_type);
  fflush(stdout);
#endif
  if(pe_id==0xffffffff){
    // access from the external master
    switch(error_type){
    case GRD_ERR_CF_GRD:
      // E2x-FCC1 doesn't generate CFG error
      break;
    case GRD_ERR_CF_ILL_ACC_SAXI:
      // E2x-FCC1 doesn't generate CFG error
      break;
    case GRD_ERR_CRAM_ILL_ACC_SAXI:
      mpExMCramAreaerrAdapter->err_req_event.notify(mPeriodVci, glb_resolution_unit);
      break;
    case GRD_ERR_CRAM_GRD:
      mpCramGrderrAdapter->err_req_event.notify(mPeriodVci, glb_resolution_unit);
      break;
    default:
      break;
    }
  }else{
    // access from PE
    if(pe_id >= mPeNum){
      //outout warning message
      char error_message_buf[128];
      sprintf(error_message_buf, 
	      "PE ID is expected from 0 to %d. but specified %d.\n", mPeNum-1, pe_id); 
      printWarningMessage(name(), "SetGrdErrFromCa", error_message_buf);
      return;
    }
    switch(error_type){
    case GRD_ERR_CF_GRD:
      // E2x-FCC1 doesn't generate CFG error
      break;
    case GRD_ERR_CF_ILL_ACC_PE:
      // E2x-FCC1 doesn't generate CFG error
      break;
    case GRD_ERR_CRAM_ILL_ACC_PE:
      mpPeCramAreaerrAdapter[pe_id]->err_req_event.notify(mPeriodCL[pe_id/2], glb_resolution_unit);
      break;
    case GRD_ERR_CRAM_GRD:
      mpCramGrderrAdapter->err_req_event.notify(mPeriodCL[pe_id/2], glb_resolution_unit);
      break;
    case GRD_ERR_PE_GRD_M:
      mpPePegErrMAdapter[pe_id]->err_req_event.notify(mPeriodCL[pe_id/2], glb_resolution_unit);
      break;
    case GRD_ERR_PE_GRD_S:
      mpPePegErrSAdapter[pe_id]->err_req_event.notify(mPeriodCL[pe_id/2], glb_resolution_unit);
      break;
    default:
      break;
    }
  }
}


///<summary>create the debugger I/F module</summary>
///<returns>none</returns>
void NSMVG4CPUV01::CreateDbgIF( void )
{
  switch(mDebuggerType){
  case DBG_MULTI:
    mpRtesrv2Ctrl = new Rteserv2Controller(mConfigFile, this); // For MULTI
	mpNoDbgIF = mpRtesrv2Ctrl;
#ifndef FASTISS_NO_SUPPORT
    if(mFastIssMode != FASTISS_NONE){
      mpFastIssIF   = mpRtesrv2Ctrl->GetFastIssIFPtr();
    }
#endif // #ifndef FASTISS_NO_SUPPORT
    break;
  case DBG_CUBESUITE:
    break;
#ifdef VLAB
  case DBG_VLAB:
    mpNoDbgIF = new VlabDbgIF(mConfigFile, this);
#ifndef FASTISS_NO_SUPPORT
    if(mFastIssMode != FASTISS_NONE){
      mpFastIssIF  = mpNoDbgIF->GetFastIssIFPtr();
    }
#endif // #ifndef FASTISS_NO_SUPPORT
    break;
#endif
  default:
    mpNoDbgIF    = new G4_NoDbgIF(mConfigFile, this);
#ifndef FASTISS_NO_SUPPORT
    if(mFastIssMode != FASTISS_NONE){
      mpFastIssIF  = mpNoDbgIF->GetFastIssIFPtr();
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>execute the ISS's simulation</summary>
///<returns>none</returns>
void NSMVG4CPUV01::execProcess( void )
{
  if(!mProgramLoaded){
    mProgramLoaded = true;
    if(mDebuggerType == DBG_NONE){
      mpNoDbgIF->LoadHex();
    }
  }

  reset();

#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    // wait to rh850g4::reset_done_event
    if(mPeNum > 1){
      sc_event_and_list all_reset_done_ev = (mpFastIssIF->mpFastIss_ASTC[0]->reset_done_event &
					     mpFastIssIF->mpFastIss_ASTC[1]->reset_done_event);
      for(unsigned int i=2; i<mPeNum; i++){
	all_reset_done_ev &= mpFastIssIF->mpFastIss_ASTC[i]->reset_done_event;
      }
      wait(all_reset_done_ev);
    }else{
      wait(mpFastIssIF->mpFastIss_ASTC[0]->reset_done_event);
    }
  }
#endif
  switch(mDebuggerType){
  case DBG_MULTI:
    mpRtesrv2Ctrl->start(); // For MULTI
    break;
  case DBG_CUBESUITE:
    break;
  default:
    mpNoDbgIF->ExecuteISS();
    break;
  }
}


///<summary>calculate the cycle period from input value (clock frequency)</summary>
///<param name="new_freq">clock frequency</param>
///<returns>cycle period</returns>
double NSMVG4CPUV01::periodCal( sc_dt::uint64 new_freq)
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


///<summary>set the cycle period to the debugger I/F</summary>
///<param name="pe_num">PE ID</param>
///<param name="period">cycle period</param>
///<returns>none</returns>
void NSMVG4CPUV01::SetPeClkPd2DbgIF(unsigned int pe_num, double period)
{
  switch(mDebuggerType){
  case DBG_MULTI:
    mpRtesrv2Ctrl->SetPeClkPeriod(pe_num, period); // For MULTI
    break;
  case DBG_CUBESUITE:
    break;
  default:
    mpNoDbgIF->SetPeClkPeriod(pe_num, period);
  }
}

///<summary>change the Cluster0's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void NSMVG4CPUV01::ChangeFreqClstr0(void)
{
  mPeriodCL[0] = periodCal((sc_dt::uint64)(*CL_sys_clk[0]).read());
  SetPeClkPd2DbgIF(0, mPeriodCL[0]);
}


///<summary>change the Cluster1's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void NSMVG4CPUV01::ChangeFreqClstr1(void)
{
  mPeriodCL[1] = periodCal((sc_dt::uint64)(*CL_sys_clk[1]).read());
  SetPeClkPd2DbgIF(1, mPeriodCL[1]);
}


///<summary>change the Cluster2's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void NSMVG4CPUV01::ChangeFreqClstr2(void)
{
  mPeriodCL[2] = periodCal((sc_dt::uint64)(*CL_sys_clk[2]).read());
  SetPeClkPd2DbgIF(2, mPeriodCL[2]);
}


///<summary>change the Cluster3's cycle period and set it to the debugger I/F</summary>
///<returns>none</returns>
void NSMVG4CPUV01::ChangeFreqClstr3(void)
{
  mPeriodCL[3] = periodCal((sc_dt::uint64)(*CL_sys_clk[3]).read());
  SetPeClkPd2DbgIF(3, mPeriodCL[3]);
}


///<summary>change the System bus's cycle period</summary>
///<returns>none</returns>
void NSMVG4CPUV01::ChangeFreqSysBus(void)
{
  mPeriodSysBus = periodCal((sc_dt::uint64)Sys_bus_clk.read());
}


///<summary>change the VCI bus's cycle period</summary>
///<returns>none</returns>
void NSMVG4CPUV01::ChangeFreqVci(void)
{
  mPeriodVci = periodCal((sc_dt::uint64)VCI_clk.read());
}


///<summary>change the Flash rom I/F's cycle period</summary>
///<returns>none</returns>
void NSMVG4CPUV01::ChangeFreqFrom(void)
{
  mPeriodFrom = periodCal((sc_dt::uint64)From_clk.read());
}


///<summary>change the cluster ram I/F's cycle period</summary>
///<returns>none</returns>
void NSMVG4CPUV01::ChangeFreqCram(void)
{
  mPeriodCram = periodCal((sc_dt::uint64)Cram_clk.read());
}


///<summary>reset operation</summary>
///<returns>none</returns>
void NSMVG4CPUV01::reset(void)
{
  if(!sys_reset.read()){
#ifndef FASTISS_NO_SUPPORT
    if(mFastIssMode != FASTISS_NONE){
      (*hi_activ_reset_sig) = true;
    }
#endif
#ifdef TEST_RESET_DBG
    printf("%s NSMVG4CPUV01::reset start\n", sc_time_stamp().to_string().c_str());
    fflush(stdout);
#endif
    switch(mDebuggerType){
    case DBG_MULTI:
      mpRtesrv2Ctrl->SetResetStatus(true); // For MULTI
      break;
    case DBG_CUBESUITE:
      break;
    default:
      mpNoDbgIF->SetResetStatus(true);
    }
  }else{
#ifdef TEST_RESET_DBG
    printf("%s NSMVG4CPUV01::reset end\n", sc_time_stamp().to_string().c_str());
    fflush(stdout);
#endif
#ifndef FASTISS_NO_SUPPORT
    if(mFastIssMode != FASTISS_NONE){
      (*hi_activ_reset_sig) = false;
      mpLinkManager->ResetLink();
    }
#endif
    switch(mDebuggerType){
    case DBG_MULTI:
      mpRtesrv2Ctrl->SetResetStatus(false); // For MULTI
      mpRtesrv2Ctrl->reset(); // For MULTI
      break;
    case DBG_CUBESUITE:
      break;
    default:
      mpNoDbgIF->SetResetStatus(false);
      mpNoDbgIF->reset();
    }
    
    for(int i=0; i<PE_MAX_NUM; i++){
      mpFenmiAdapter[i]->Reset();
      mpFeintAdapter[i]->Reset();
      mpEiintAdapter[i]->Reset();
      mpCpuGmInfoAdapter[i]->Reset();
      mpCpuInitAdapter[i]->Reset();
    }

    for(unsigned int i=0; i<mPeNum; i++){
      mpPeCramAreaerrAdapter[i]->Reset();
      mpPePegErrMAdapter[i]->Reset();
      mpPePegErrSAdapter[i]->Reset();
    }

    mpExMCramAreaerrAdapter->Reset();
    mpCramGrderrAdapter->Reset();

    
    for(int i=0; i<PE_MAX_NUM; i++){
      mIsSwdtClkHi[i] = false;
    }
#ifdef TEST_RESET_DBG
#ifndef FASTISS_NO_SUPPORT
    if(mFastIssMode != FASTISS_NONE){
      reset_test_event.notify(SC_ZERO_TIME);
    }
#endif
#endif
  }
}

///<summary>output log for reset test</summary>
///<returns>none</returns>
void NSMVG4CPUV01::reset_test(void)
{
#ifdef TEST_RESET_DBG
#ifndef FASTISS_NO_SUPPORT
    if(mFastIssMode != FASTISS_NONE){
      mpFastIssIF->TestResetLog();
    }
#endif
#endif
}


///<summary>output swdt clock to SWDT for PE0</summary>
///<returns>none</returns>
void NSMVG4CPUV01::SwdtClkOutPe0(void)
{
  SwdtClkOut(0);
}


///<summary>output swdt clock to SWDT for PE1</summary>
///<returns>none</returns>
void NSMVG4CPUV01::SwdtClkOutPe1(void)
{
  SwdtClkOut(1);
}


///<summary>output swdt clock to SWDT for PE2</summary>
///<returns>none</returns>
void NSMVG4CPUV01::SwdtClkOutPe2(void)
{
  SwdtClkOut(2);
}


///<summary>output swdt clock to SWDT for PE3</summary>
///<returns>none</returns>
void NSMVG4CPUV01::SwdtClkOutPe3(void)
{
  SwdtClkOut(3);
}


///<summary>output swdt clock to SWDT for PE4</summary>
///<returns>none</returns>
void NSMVG4CPUV01::SwdtClkOutPe4(void)
{
  SwdtClkOut(4);
}


///<summary>output swdt clock to SWDT for PE5</summary>
///<returns>none</returns>
void NSMVG4CPUV01::SwdtClkOutPe5(void)
{
  SwdtClkOut(5);
}


///<summary>output swdt clock to SWDT for PE6</summary>
///<returns>none</returns>
void NSMVG4CPUV01::SwdtClkOutPe6(void)
{
  SwdtClkOut(6);
}


///<summary>output swdt clock to SWDT for PE7</summary>
///<returns>none</returns>
void NSMVG4CPUV01::SwdtClkOutPe7(void)
{
  SwdtClkOut(7);
}


///<summary>output swdt clock to SWDT</summary>
///<param name="pe_id">PE ID</param>
///<returns>none</returns>
void NSMVG4CPUV01::SwdtClkOut(unsigned int pe_id)
{
#ifdef TEST_SWDT_CPU
  printf("%s:%s:PE%d:SwdtClkOut invoked. ",
	 sc_time_stamp().to_string().c_str(),
	 this->name(),
	 pe_id);
#endif
  if(!mIsSwdtClkHi[pe_id]){
#ifdef TEST_SWDT_CPU
    printf("signal=high\n");
    fflush(stdout);
#endif
    Swdt_clk[pe_id]->write(true);
    mIsSwdtClkHi[pe_id] = true;
    swdt_clk_out_event[pe_id].notify(mPeriodCL[pe_id/2], glb_resolution_unit);
    if(SIM_MODE_FAST == mIssMode){
#ifndef FASTISS_NO_SUPPORT
      if(mFastIssMode != FASTISS_NONE){
	unsigned int val = mpFastIssIF->GetSwd0pcadExecCountValue(pe_id);
	if(val > 1){
	  //outout warning message
	  char error_message_buf[128];
	  sprintf(error_message_buf, 
		  "0x%08x is executed %d times in this quantum.\n", 
		  mSwd0pcadVal[pe_id], 
		  val);
	  char mod_buf[128];
	  sprintf(mod_buf, "SwdtClkOutPe%d", pe_id);
          printWarningMessage(name(), mod_buf, error_message_buf);
	}
      }
#endif
    }
  }else{
#ifdef TEST_SWDT_CPU
    printf("signal=low\n");
    fflush(stdout);
#endif
    Swdt_clk[pe_id]->write(false);
    mIsSwdtClkHi[pe_id] = false;
  }
}


///<summary>read the configulation file to get effective PE number</summary>
///<param name="filename">file name of configulation file</param>
///<returns>none</returns>
void NSMVG4CPUV01::read_config_file_PeNum( const char *filename )
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

    // effective PE number
    if (strcmp(token, "[PE_NUM]") == 0) {
      token = strtok(0, seps);
      if (token != NULL){
        mPeNum = token_to_uint( token,
				name(),
				"[PE_NUM]" );
	mClstrNum = (mPeNum / 2) + (mPeNum % 2);
	if(( mPeNum > PE_MAX_NUM ) || ((mPeNum !=1) && ((mPeNum % 2) !=0)) || ( mPeNum == 0 )){
	  char error_message_buf[1024];
	  // for WinMsg
	  sprintf(error_message_buf,
		  "[PE_NUM] must be 1, 2, 4, 6, or 8, but specified [%d]."
		  , mPeNum);
	  printErrorMessage(name(), "read_config_file_PeNum", error_message_buf);
	  exit(1);
	}
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(NSMVG4CPUV01):[PE_NUM] :mPeNum=%d, mClstrNum=%d\n",
	     mPeNum, mClstrNum);
      fflush(stdout);
#endif
      continue;
    }
  }
}

///<summary>read the configulation file to get each parameter</summary>
///<param name="filename">file name of configulation file</param>
///<returns>none</returns>
void NSMVG4CPUV01::read_config_file( const char *filename )
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

    if( strcmp( token, "[FASTISS_MAP_FILE]" ) == 0 ) {
      token = strtok(0, seps);
      const char *fname = (!token)? "" : token;
      strcpy(mFastissRouterMapFname, fname);
#ifdef TEST_CORE_DBG
      printf("configuration test(NSMVG4CPUV01):%s :mFastissRouterMapFname=%s\n",
	     "[FASTISS_MAP_FILE]", mFastissRouterMapFname);
      fflush(stdout);
#endif
      continue;
    }

    if( strcmp( token, "[CAISS_MAP_FILE]") == 0 ) {
      token = strtok(0, seps);
      const char *fname = (!token)? "" : token;
      strcpy(mCaissRouterMapFname, fname);
#ifdef TEST_CORE_DBG
      printf("configuration test(NSMVG4CPUV01):%s :mCaissRouterMapFname=%s\n",
	     "[CAISS_MAP_FILE]", mCaissRouterMapFname);
      fflush(stdout);
#endif
      continue;
    }

    // mode of connecting debugger I/F
    if (strcmp(token, "[G4CPU_DEBUG_MODE]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "NONE") == 0){
	mDebuggerType = DBG_NONE;
      } else if(strcmp(token, "MULTI") == 0){
	mDebuggerType = DBG_MULTI;
      } else if(strcmp(token, "CUBESUITE+") == 0){
	mDebuggerType = DBG_CUBESUITE;
      } else if(strcmp(token, "VLAB") == 0){
  mDebuggerType = DBG_VLAB;
      } else{
	char error_message_buf[1024];
	sprintf(error_message_buf,
		"Unexpected token was specified [%s] on [G4CPU_DEBUG_MODE].",
		token);
	printErrorMessage(name(), "read_config_file", error_message_buf);
	error_detected = true;
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(NSMVG4CPUV01):[G4CPU_DEBUG_MODE] :mDebuggerType=%d\n",
	     mDebuggerType);
      fflush(stdout);
#endif
      continue;
    }

    // simulation mode
    if (strcmp(token, "[SIM_MODE]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "CAISS") == 0){
	SetSimMode(SIM_MODE_CA);
      } else if(strcmp(token, "FASTISS") == 0){
	SetSimMode(SIM_MODE_FAST);
	SetFastIssMode(FASTISS_ASTC);
      } else{
	char error_message_buf[1024];
	sprintf(error_message_buf,
		"Unexpected token was specified [%s] on [SIM_MODE].",
		token);
	printErrorMessage(name(), "read_config_file", error_message_buf);
	error_detected = true;
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(NSMVG4CPUV01):[SIM_MODE] mIssMode:=%d\n",
	     mIssMode);
      fflush(stdout);
#endif
      continue;
    }

    // fast iss mode
    if (strcmp(token, "[FASTISS]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "NONE") == 0){
	SetFastIssMode(FASTISS_NONE);
      } else if(strcmp(token, "ASTC") == 0){
	SetFastIssMode(FASTISS_ASTC);
      } else{
	char error_message_buf[1024];
	sprintf(error_message_buf,
		"Unexpected token was specified [%s] on [FASTISS].",
		token);
	printErrorMessage(name(), "read_config_file", error_message_buf);
	error_detected = true;
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(NSMVG4CPUV01):[FASTISS] :mFastIssMode=%d\n",
	     mFastIssMode);
      fflush(stdout);
#endif
      continue;
    }

    // For MULTI
    if ( 0 == strcmp( token, "[V850E2R_MULTI]" ) ) {
      mDebuggerType = DBG_MULTI;
      continue;
    }

    // for PE TYPE
    if (strcmp(token, "[G4CPU_PE_TYPE]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              name(),
                              "[G4CPU_PE_TYPE] peid(1st parameter)" );
        if( peid >= mPeNum ){
          char error_message_buf[1024];
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_PE_TYPE] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(name(), "read_config_file", error_message_buf);
        }else{
          token = strtok(0, seps);
	  mPeType[peid] = token;
	  if((strcmp(token, "G4MH") != 0) && (strcmp(token, "G4MH_V10") != 0) && (strcmp(token, "G4MH_V11") != 0) && (strcmp(token, "G4MH_V20") != 0)){
	    char error_message_buf[1024];
	    sprintf(error_message_buf,
		    "Unexpected token was specified [%s] on [G4CPU_PE_TYPE] PE type(2nd parmeter).",
		    token);
	    printErrorMessage(name(), "read_config_file", error_message_buf);
	    error_detected = true;
	  }
	}
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(NSMVG4CPUV01):[G4CPU_PE_TYPE]:PE%d :mPeType[%d]=%s\n",
	     peid, peid, mPeType[peid].c_str());
      fflush(stdout);
#endif
      continue;
    }
    if (strcmp(token, "[G4CPU_PE_INFO]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              name(),
                              "[G4CPU_PE_INFO] pe index(1st parameter)" );
        if( peid >= mPeNum ){
          char error_message_buf[1024];
          // for WinMsg
          sprintf(error_message_buf,
                  "[G4CPU_PE_INFO] pe index must be under %d, but specified [%d]."
                  , mPeNum, peid);
          printWarningMessage(name(), "read_config_file", error_message_buf);
         }else{
	  token = strtok(0, seps);
	  if (token != NULL){
	    mpPeInfo[peid].peid_ival = token_to_uint( token,
						      name(),
						      "[G4CPU_PE_INFO] peid(2nd parameter)" );
	    if(mpPeInfo[peid].peid_ival > PEID_MAX_NUM){
	      char error_message_buf[1024];
	      sprintf(error_message_buf,
		      "Unexpected token was specified [%s] on [G4CPU_PE_INFO] peid(2nd parameter).",
		      token);
	      printErrorMessage(name(), "read_config_file", error_message_buf);
	      error_detected = true;
	    }
	  }
	}
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(NSMVG4CPUV01):[G4CPU_PE_INFO]:PE%d :peid=%d\n",
	     peid, mpPeInfo[peid].peid_ival);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_PROFILE_TRACE_FORMAT]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "OLD_PC") == 0){
	mIsOldTraceFormat = true;
      } else{
	mIsOldTraceFormat = false;
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(NSMVG4CPUV01):[G4CPU_PROFILE_TRACE_FORMAT]: mIsOldTraceFormat=%d\n",
	     mIsOldTraceFormat);
      fflush(stdout);
#endif
      continue;
    }

    if (strcmp(token, "[G4CPU_START_MODE]") == 0) {
      token = strtok(0, seps);
      if(strcmp(token, "AUTO") == 0){
	mIsAutoMode = true;
#ifdef TEST_CORE_DBG
	printf("configuration test(NSMVG4CPUV01):[G4CPU_START_MODE]: mIsAutoMode=true\n");
	fflush(stdout);
#endif
      }else if(strcmp(token, "BOOTCTRL") == 0){
	mIsAutoMode = false;
#ifdef TEST_CORE_DBG
	printf("configuration test(NSMVG4CPUV01):[G4CPU_START_MODE]: mIsAutoMode=false\n");
	fflush(stdout);
#endif
      } else {
	char error_message_buf[1024];
	sprintf(error_message_buf,
		"Unexpected token was specified [%s] on [G4CPU_START_MODE].",
		token);
	printErrorMessage(name(), "read_config_file", error_message_buf);
	error_detected = true;
      }
      continue;
    }
  }  // end of configuration file read loop(infinite)
}  // end of read_config_file()


///<summary>set simulation mode</summary>
///<param name="iss_mode">simulation mode</param>
///<returns>none</returns>
void NSMVG4CPUV01::SetSimMode( SIM_MODE_T iss_mode )
{
  mIssMode = iss_mode;
}


///<summary>set fastiss mode</summary>
///<param name="fastiss_mode">fastiss mode</param>
///<returns>none</returns>
void NSMVG4CPUV01::SetFastIssMode( FASTISS_MODE_T fastiss_mode )
{
  mFastIssMode = fastiss_mode;
}


///<summary>get the file pointer of ececution trace file</summary>
///<returns>the file pointer of ececution trace file</returns>
FILE* NSMVG4CPUV01::GetTraceFp()
{
  return (mpNoDbgIF->GetTraceFp());
}


/// <summary>start of elaboration</summary>
/// <returns>none</returns>
void NSMVG4CPUV01::start_of_simulation( void )
{
  mpCaIss->tsf->setBaseAddressSize(FROM_INIT_BASE, FROM_MAX_SIZE);
  mpCaIss->tss->setBaseAddressSize(FROM_INIT_BASE, 0xffffffff);
#ifdef TEST_E3
  mpCaIss->tscr->setBaseAddressSize(0x02000000, 0xfd000000);
#else
  mpCaIss->tscr->setBaseAddressSize(CRAM_INIT_BASE, CRAM_MAX_SIZE);
#endif

#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    if (mIsOldTraceFormat) {
      mpFastIssIF->AttachAnalysisUnit(true);
    }else{
      mpFastIssIF->AttachAnalysisUnit(false);
    }
    for(unsigned int i=0; i<mPeNum; i++){
      mpCluster[i/2]->mpPECORESS[i%2]->SetPegInfo();
    }
  }
#endif
}


/// <summary>end of simulation</summary>
/// <returns>none</returns>
void NSMVG4CPUV01::end_of_simulation( void )
{
  mpNoDbgIF->flushTraceData();
  for(unsigned int i=0; i<mPeNum; i++){
    mpNoDbgIF->dispExInstNum(i, true);
    mpNoDbgIF->dispIcacheAccNum(i, true);
  }
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    mpFastIssIF->DetachAnalysisUnit();
  }
#endif
}


///<summary>read memory value</summary>
///<param name="peid">pe id</param>
///<param name="addr">address</param>
///<param name="byte">data length</param>
///<returns>memory value</returns>
unsigned long long NSMVG4CPUV01::readMemValue( unsigned int peid, 
                                                unsigned int addr, 
                                                unsigned int byte )
{
  return mpNoDbgIF->readMemValue(peid, addr, byte);
}

///<summary>write memory value</summary>
///<param name="peid">pe id</param>
///<param name="addr">address</param>
///<param name="value">write value</param>
///<param name="byte">data length</param>
///<returns>memory value</returns>
unsigned long long NSMVG4CPUV01::writeMemValue( unsigned int peid, 
                                                 unsigned int addr, 
                                                 unsigned long long value, 
                                                 unsigned int byte )
{
  return mpNoDbgIF->writeMemValue(peid, addr, value, byte);
}

///<summary>display executed instruction number</summary>
///<param name="peid">pe id</param>
///<returns>none</returns>
void NSMVG4CPUV01::dispExInstNum( unsigned int peid )
{
  return mpNoDbgIF->dispExInstNum(peid);
}


///<summary>set clock frequency ratio</summary>
///<param name="peid">pe id</param>
///<param name="ratio">clock frequency ratio</param>
///<returns>none</returns>
void NSMVG4CPUV01::setClockRatio( unsigned int peid, unsigned int ratio )
{
  return mpNoDbgIF->SetClockRatio(peid, ratio);
}


///<summary>wait while freqency is 0</summary>
///<returns>none</returns>
void NSMVG4CPUV01::waitWhl0freq()
{
  sc_event_or_list all_clk_ev = ((CL_sys_clk[0]->get_interface())->default_event() |
				 (CL_sys_clk[1]->get_interface())->default_event());
  for(unsigned int i=2; i<mClstrNum; i++){
    all_clk_ev |= (CL_sys_clk[i]->get_interface())->default_event();
  }

  while(1) {
    int cond = 0;
    for(unsigned int i=0; i<mClstrNum; i++){
      if (cond |= (*CL_sys_clk[i] == 0)) break;
    }
    if (cond) {
      // if there is a freq == 0
#if defined(EXEC_DBG)
      printf("%s:NSMVG4CPUV01::waitWhl0freq: clock frequency is 0\n", sc_time_stamp().to_string().c_str());
      fflush(stdout);
#endif
      wait(all_clk_ev);
    }
    else {
      // if there isn't freq == 0
#if defined(EXEC_DBG)
      printf("%s:NSMVG4CPUV01::waitWhl0freq: clock frequency isn't 0\n", sc_time_stamp().to_string().c_str());
      fflush(stdout);
#endif
      break;
    }
  }
}

///<summary>get the total accessed times of I-Cache</summary>
///<param name="peid">pe id</param>
///<returns>total accessed times</returns>
unsigned long long NSMVG4CPUV01::getIcacheAccNum( unsigned int peid )
{
  return(mpNoDbgIF->getIcacheAccNum(peid));
}


///<summary>get the hit times of I-Cache</summary>
///<param name="peid">pe id</param>
///<returns>hit times</returns>
unsigned long long NSMVG4CPUV01::getIcacheHitNum( unsigned int peid )
{
  return(mpNoDbgIF->getIcacheHitNum(peid));
}


///<summary>get the miss hit times of I-Cache</summary>
///<param name="peid">pe id</param>
///<returns>miss hit times</returns>
unsigned long long NSMVG4CPUV01::getIcacheMissNum( unsigned int peid )
{
  return(mpNoDbgIF->getIcacheMissNum(peid));
}


///<summary>set PC check function mode for SWDT</summary>
///<param name="peid">pe id</param>
///<param name="mode">PC check function mode(true:ON, false:OFF)</param>
///<returns>none</returns>
void NSMVG4CPUV01::setSwdtEnable( unsigned int peid, bool mode )
{
#ifdef TEST_SWDT_CPU
  printf("%s:%s: called setSwdtEnable(%d, %d)\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name(),
	 peid,
	 mode);
  fflush(stdout);
#endif

  if((mPeType[peid] == "G4MH") || (mPeType[peid] == "G4MH_V10") || (mPeType[peid] == "G4MH_V11") || (mPeType[peid] == "G4MH_V20")){
    if(peid != 0) return;
  }else{
    return;
  }

  mpNoDbgIF->setSwdtEnable(peid, mode);
}


///<summary>set Syserr output request</summary>
///<param name="peid">pe id</param>
///<param name="code">error code</param>
///<returns>none</returns>
void NSMVG4CPUV01::setSyserrOut( unsigned int peid, unsigned int code )
{
#ifdef TEST_CORE_DBG
  printf("%s:%s: called setSyserrOut(%d, 0x%2x)\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name(),
	 peid,
	 code);
  fflush(stdout);
#endif

  if(SIM_MODE_CA == mIssMode){
    mpNoDbgIF->setSyserrOut(peid, code);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mFastIssMode){
      if(peid/2 < mClstrNum){
	mpCluster[peid/2]->setSyserrOut(peid%2, code);
      }
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>set Syserr (Operand factor) output request</summary>
///<param name="peid">pe id</param>
///<param name="code">error code</param>
///<returns>none</returns>
void NSMVG4CPUV01::setSyserrOperandOut( unsigned int peid, unsigned int code )
{
#ifdef TEST_CORE_DBG
  printf("%s:%s: called setSyserrOperandOut(%d, 0x%2x)\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name(),
	 peid,
	 code);
  fflush(stdout);
#endif

  if(SIM_MODE_CA == mIssMode){
    mpNoDbgIF->setSyserrOperandOut(peid, code);
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mFastIssMode){
      if(peid/2 < mClstrNum){
	mpCluster[peid/2]->setSyserrOperandOut(peid%2, code);
      }
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>set Trace output function mode</summary>
///<param name="mode">Trace output mode(true:ON, false:OFF)</param>
///<returns>none</returns>
void NSMVG4CPUV01::setTraceEnable( bool mode )
{
  mpNoDbgIF->setTraceEnable(mode);
}


///<summary>clear the fastISS's traslation cache</summary>
///<param name="peid">pe id</param>
///<returns>none</returns>
void NSMVG4CPUV01::clearFastissCache( unsigned int peid )
{
  mpNoDbgIF->clearFastissCache(peid);
}


///<summary>Set the read latency of Flash ROM for the External master(not CPU)</summary>
///<param name="latency">read latency of Flash ROM for the External master</param>
///<returns>none</returns>
void NSMVG4CPUV01::setExMReadRomLatency( unsigned int latency )
{
  mpNoDbgIF->setExMReadRomLatency(latency);
}


///<summary>Set the write latency of Flash ROM for the External master(not CPU)</summary>
///<param name="latency">write latency of Flash ROM for the External master</param>
///<returns>none</returns>
void NSMVG4CPUV01::setExMWriteRomLatency( unsigned int latency )
{
  mpNoDbgIF->setExMWriteRomLatency(latency);
}


///<summary>Set the read latency of Cluster RAM for the External master(not CPU)</summary>
///<param name="latency">read latency of Cluster RAM for the External master</param>
///<returns>none</returns>
void NSMVG4CPUV01::setExMReadCramLatency( unsigned int latency )
{
  mpNoDbgIF->setExMReadCramLatency(latency);
}


///<summary>Set the write latency of Cluster RAM for the External master(not CPU)</summary>
///<param name="latency">write latency of Cluster RAM for the External master</param>
///<returns>none</returns>
void NSMVG4CPUV01::setExMWriteCramLatency( unsigned int latency )
{
  mpNoDbgIF->setExMWriteCramLatency(latency);
}


///<summary>Set the read latency of Local RAM for the External master(not CPU)</summary>
///<param name="latency">read latency of Local RAM for the External master</param>
///<returns>none</returns>
void NSMVG4CPUV01::setExMReadLramLatency( unsigned int latency )
{
  mpNoDbgIF->setExMReadLramLatency(latency);
}


///<summary>Set the write latency of Local RAM for the External master(not CPU)</summary>
///<param name="latency">write latency of Local RAM for the External master</param>
///<returns>none</returns>
void NSMVG4CPUV01::setExMWriteLramLatency( unsigned int latency )
{
  mpNoDbgIF->setExMWriteLramLatency(latency);
}


///<summary>Set the read latency of Local APB's IP for the External master(not CPU)</summary>
///<param name="latency">read latency of Local APB's IP for the External master</param>
///<returns>none</returns>
void NSMVG4CPUV01::setExMReadLapbLatency( unsigned int latency )
{
  mpNoDbgIF->setExMReadLapbLatency(latency);
}


///<summary>Set the write latency of Local APB's IP for the External master(not CPU)</summary>
///<param name="latency">write latncy of Local APB's IP fof the External master</param>
///<returns>none</returns>
void NSMVG4CPUV01::setExMWriteLapbLatency( unsigned int latency )
{
  mpNoDbgIF->setExMWriteLapbLatency(latency);
}


///<summary>Get the read latency of Flash ROM for the External master(not CPU)</summary>
///<returns>read latncy of Flash ROM for the External master</returns>
unsigned int NSMVG4CPUV01::getExMReadRomLatency( void )
{
  return(mpNoDbgIF->getExMReadRomLatency());
}


///<summary>Get the write latency of Flash ROM for the External master(not CPU)</summary>
///<returns>write latncy of Flash ROM for the External master</returns>
unsigned int NSMVG4CPUV01::getExMWriteRomLatency( void )
{
  return(mpNoDbgIF->getExMWriteRomLatency());
}


///<summary>Get the read latency of Cluster RAM for the External master(not CPU)</summary>
///<returns>read latncy of Cluster RAM for the External master</returns>
unsigned int NSMVG4CPUV01::getExMReadCramLatency( void )
{
  return(mpNoDbgIF->getExMReadCramLatency());
}


///<summary>Get the write latency of Cluster RAM for the External master(not CPU)</summary>
///<returns>write latncy of Cluster RAM for the External master</returns>
unsigned int NSMVG4CPUV01::getExMWriteCramLatency( void )
{
  return(mpNoDbgIF->getExMWriteCramLatency());
}


///<summary>Get the read latency of Local RAM for the External master(not CPU)</summary>
///<returns>read latncy of Local RAM for the External master</returns>
unsigned int NSMVG4CPUV01::getExMReadLramLatency( void )
{
  return(mpNoDbgIF->getExMReadLramLatency());
}


///<summary>Get the write latency of Local RAM for the External master(not CPU)</summary>
///<returns>write latncy of Local RAM for the External master</returns>
unsigned int NSMVG4CPUV01::getExMWriteLramLatency( void )
{
  return(mpNoDbgIF->getExMWriteLramLatency());
}


///<summary>Get the read latency of Local APB's IP for the External master(not CPU)</summary>
///<returns>read latncy of Local APB's IP for the External master</returns>
unsigned int NSMVG4CPUV01::getExMReadLapbLatency( void )
{
  return(mpNoDbgIF->getExMReadLapbLatency());
}


///<summary>Get the write latency of Local APB's IP for the External master(not CPU)</summary>
///<returns>write latncy of Local APB's IP for the External master</returns>
unsigned int NSMVG4CPUV01::getExMWriteLapbLatency( void )
{
  return(mpNoDbgIF->getExMWriteLapbLatency());
}


///<summary>set the simulation to Sync-mode</summary>
///<returns>none</returns>
void NSMVG4CPUV01::setSyncMode( void )
{
  mAsyncFlag = false;
}


///<summary>set the simulation to Async-mode</summary>
///<returns>none</returns>
void NSMVG4CPUV01::setAsyncMode( void )
{
  mAsyncFlag = true;
}


///<summary>return the Sync-mode of the simulation</summary>
///<returns>true:Sync-mode, false:Async-mode</returns>
bool NSMVG4CPUV01::isSyncMode( void )
{
  return !mAsyncFlag;
}


///<summary>return the Async-mode of the simulation</summary>
///<returns>true:Async-mode, false:Sync-mode</returns>
bool NSMVG4CPUV01::isAsyncMode( void )
{
  return mAsyncFlag;
}


///<summary>set SWD0PCAD's value</summary>
///<param name="value">WD0PCAD's value</param>
///<returns>none</returns>
void NSMVG4CPUV01::SetSwd0pcadValue( unsigned int peid, unsigned int value )
{
#ifdef TEST_SWDT_CPU
  printf("%s:%s: called SetSwd0pcadValue(%d, 0x%08x)\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name(),
	 peid,
	 value);
  fflush(stdout);
#endif

  if((mPeType[peid] == "G4MH") || (mPeType[peid] == "G4MH_V10") || (mPeType[peid] == "G4MH_V11") || (mPeType[peid] == "G4MH_V20")){
    value = value & 0x007fffff;
    if(peid != 0) return;
  }else{
    return;
  }

  mpNoDbgIF->SetSwd0pcadValue(peid, value);
  mSwd0pcadVal[peid] = value;
}

///<summary>get current PC address</summary>
///<param name="peid">pe id</param>
///<returns>PC address</returns>
unsigned long long NSMVG4CPUV01::getCurrentPc( unsigned int peid )
{
  return(mpNoDbgIF->getCurrentPc( peid ));
}


///<summary>get memory access count of the current PC's instruction</summary>
///<param name="peid">pe id</param>
///<param name="isWrite">access type (trus:write, false:read)</param>
///<returns>memory access count</returns>
unsigned int NSMVG4CPUV01::getMemAccCntCurrentPc( unsigned int peid, bool isWrite )
{
  return(mpNoDbgIF->getMemAccCntCurrentPc( peid, isWrite ));
}

///<summary>get memory access data and length of the current PC's instruction</summary>
///<param name="peid">pe id</param>
///<param name="acc_cnt">memory access number</param>
///<param name="data_l">data (lower 64bit)</param>
///<param name="data_u">data (upper 64bit)</param>
///<param name="data_length">data length</param>
///<param name="isWrite">access type (trus:write, false:read)</param>
///<returns>none</returns>
void NSMVG4CPUV01::getMemAccDataCurrentPc( unsigned int peid, unsigned int acc_cnt, unsigned long long *data_l, unsigned long long *data_u, unsigned int *data_length, bool isWrite )
{
  mpNoDbgIF->getMemAccDataCurrentPc( peid, acc_cnt, data_l, data_u, data_length, isWrite );
}


///<summary>read the global register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<returns>register value</returns>
unsigned int NSMVG4CPUV01::readGrValue( unsigned int peid, unsigned int regid )
{
  if((0 <= peid) && (peid < mPeNum)){
    return(mpNoDbgIF->readGrValue(peid, regid));
  }else{
    return(0);
  }
}


///<summary>write the global register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void NSMVG4CPUV01::writeGrValue( unsigned int peid, unsigned int regid, unsigned int value )
{
  if((0 <= peid) && (peid < mPeNum)){
    mpNoDbgIF->writeGrValue(peid, regid, value);
  }
}


///<summary>read the vector register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="isUpper">data position (true:upper 64bit, false:lower64bit)</param>
///<returns>register value</returns>
unsigned long long NSMVG4CPUV01::readWrValue( unsigned int peid, unsigned int regid, bool isUpper )
{
  if((0 <= peid) && (peid < mPeNum)){
    return(mpNoDbgIF->readWrValue(peid, regid, isUpper));
  }else{
    return(0);
  }
}


///<summary>write the vector register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="up_value">regester value (upper 64bit)</param>
///<param name="lo_value">regester value (lower 64bit)</param>
///<returns>none</returns>
void NSMVG4CPUV01::writeWrValue( unsigned int peid, unsigned int regid, unsigned long long up_value, unsigned long long lo_value )
{
  if((0 <= peid) && (peid < mPeNum)){
    mpNoDbgIF->writeWrValue(peid, regid, up_value, lo_value);
  }
}


///<summary>read the system register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="selid">selecter ID (0-13)</param>
///<returns>register value</returns>
unsigned int NSMVG4CPUV01::readSrValue( unsigned int peid, unsigned int regid, unsigned int selid )
{
  if((0 <= peid) && (peid < mPeNum)){
    return(mpNoDbgIF->readSrValue(peid, regid, selid));
  }else{
    return(0);
  }
}


///<summary>write the system register value</summary>
///<param name="peid">pe id</param>
///<param name="regid">regester ID (0-31)</param>
///<param name="selid">selecter ID (0-13)</param>
///<param name="value">regester value</param>
///<returns>none</returns>
void NSMVG4CPUV01::writeSrValue( unsigned int peid, unsigned int regid, unsigned int selid, unsigned int value )
{
  if((0 <= peid) && (peid < mPeNum)){
    mpNoDbgIF->writeSrValue(peid, regid, selid, value);
  }
}

///<summary>read or write the value to CFG register</summary>
///<param name="trans">target transaction</param>
///<param name="isRead">access mode(read or write)</param>
///<returns>none</returns>
void NSMVG4CPUV01::CfgRegAcc( TlmBasicPayload& trans, bool isRead )
{
  // temp common function (CAISS and FastISS)
  if(isRead){
#ifdef TEST_CORE_DBG
    unsigned long addr = trans.get_address();
    unsigned int  size = trans.get_data_length();
    printf("NSMVG4CPUV01::CfgRegAcc(read) addr=0x%08x, size=%d\n", (unsigned int)addr, size);
    fflush(stdout);
#endif
    mpCaIss->ReadCfgReg(trans);
  }else{
#ifdef TEST_CORE_DBG
    unsigned long addr = trans.get_address();
    unsigned int  size = trans.get_data_length();
    printf("NSMVG4CPUV01::CfgRegAcc(write) addr=0x%08x, size=%d\n", (unsigned int)addr, size);
    fflush(stdout);
#endif
    mpCaIss->WriteCfgReg(trans);
  }
}


///<summary>read or write the value from CRG register</summary>
///<param name="trans">target transaction</param>
///<param name="isRead">access mode(read or write)</param>
///<returns>none</returns>
void NSMVG4CPUV01::CrgRegAcc( TlmBasicPayload& trans, bool isRead )
{
  // common function(CAISS and FastISS)
  if(isRead){
#ifdef TEST_CORE_DBG
    unsigned long addr = trans.get_address();
    unsigned int  size = trans.get_data_length();
    printf("NSMVG4CPUV01::CrgRegAcc(read) addr=0x%08x, size=%d\n", (unsigned int)addr, size);
    fflush(stdout);
#endif
    mpCaIss->ReadCrgReg(trans);
  }else{
#ifdef TEST_CORE_DBG
    unsigned long addr = trans.get_address();
    unsigned int  size = trans.get_data_length();
    printf("NSMVG4CPUV01::CrgRegAcc(write) addr=0x%08x, size=%d\n", (unsigned int)addr, size);
    fflush(stdout);
#endif
    mpCaIss->WriteCrgReg(trans);
  }
}

///<summary>read or write the value from PEG register</summary>
///<param name="trans">target transaction</param>
///<param name="isRead">access mode(read or write)</param>
///<returns>none</returns>
void NSMVG4CPUV01::PegRegAcc( TlmBasicPayload& trans, bool isRead )
{
  // common function(CAISS and FastISS)
  if(isRead){
#ifdef TEST_CORE_DBG
    unsigned long addr = trans.get_address();
    unsigned int  size = trans.get_data_length();
    printf("NSMVG4CPUV01::PegRegAcc(read) addr=0x%08x, size=%d\n", (unsigned int)addr, size);
    fflush(stdout);
#endif
    mpCaIss->ReadPegReg(trans);
  }else{
#ifdef TEST_CORE_DBG
    unsigned long addr = trans.get_address();
    unsigned int  size = trans.get_data_length();
    printf("NSMVG4CPUV01::PegRegAcc(write) addr=0x%08x, size=%d\n", (unsigned int)addr, size);
    fflush(stdout);
#endif
    mpCaIss->WritePegReg(trans);
  }
}

