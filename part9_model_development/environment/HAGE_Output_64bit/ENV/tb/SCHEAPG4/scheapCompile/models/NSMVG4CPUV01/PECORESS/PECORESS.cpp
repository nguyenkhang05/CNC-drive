/*************************************************************************
 *
 *  PECORESS.cpp
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
#include <string.h>  // for strcpy, strtok, strcmp, strncmp
#include <stdio.h>   // for sprintf
#include <errno.h>   // for errno
#include <map>

#include "Common.h"
#include "CmErrMsg.h" // for WinMsg

#include "PECORESS.h"
#include "G4_PAYLOAD_ADDER.h"
#include "OSCI2DCDR.h"
#include "I_BRIDGE.h"
#include "G4_PEG_Wrapper.h"
#include "G4_PEG.h"
#include "G4_PEGCAP.h"

#ifdef VLAB_0
#include "TLMShortcut.h"
#endif

using namespace CmErrMsg; // for WinMsg
using namespace rh850g4;    // for ASTC's fastiss


///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<param name="config_file">file name of configulation file</param>
///<param name="peid">PE ID</param>
///<returns>none</returns>
PECORESS::
PECORESS(sc_module_name module_name, const char *config_file) : 
  sc_module(module_name),
  sys_clk("sys_clk"),
  sys_reset("sys_reset"),
  is("is"),
  ish("ish"),
  isgp("isgp"),
  isgc("isgc"),
  isi("isi"),
  ts("ts"),
  ts_peg_v("ts_peg_v"),
  ts_peg_p("ts_pep_p"),
  ts_peg_m("ts_pep_m"),
  ts_peg_s("ts_pep_s"),
  FenmiReq("FenmiReq"),
  FeintReq("FeintReq"),
  EiintReq("EiintReq"),
  FenmiAck("FenmiAck"),
  FeintAck("FeintAck"),
  EiintAck("EiintAck"),
  CpuGmInfo("CpuGmInfo"),
  PegInfoIn("PegInfoIn"),
  PegInfoOut("PegInfoOut"),
  PegErrM("PegErrM"),
  PegErrS("PegErrS"),
  mPeId(0),
  mPeType(CPU_G4MH_1_0),
  mIssMode(SIM_MODE_CA),
  mFastIssMode(FASTISS_NONE),
  mConfigFile(config_file),
  syserr_init_event()
{
  mpG4PegWrapper = 0;

#ifdef VLAB_0
  TLMShortcut* pTlmShortcut;

  bool tlm_shortcut = false;
  if (getenv("RH850_TLM_SHORTCUT") != NULL)
  {
    tlm_shortcut = true;
    printf("TLM Shortcut Enabled\n");
  }
#endif

  read_config_file(mConfigFile);

  if(mFastIssMode != FASTISS_NONE){
    /************************************/
    /* Cleate all instanses for PECORESS */
    /************************************/
    // cleate fast ISS
    mpFastIss_ASTC = new rh850g4::core("FastIss_ASTC", true);

    // create fast ISS memory wrapper
    mpFastIssMemWrapper_ASTC = new rh850g4::memory_wrapper("FastIssMemWrapper_ASTC");

    // cleate payload adder of fast ISS
    mpPayloadAdder = new G4_PAYLOAD_ADDER<BUS_WIDTH_VCI32>("payload_adder", mpFastIss_ASTC);
    mpPayloadAdder->setPeId(mPeId);
	
#ifdef VLAB_0
    if (tlm_shortcut)
    {
        char mod_name[64];
        sprintf(mod_name, "PE%dTlmShortcut", i+1);
        pTlmShortcut = new TLMShortcut(mod_name);
    }
#endif

    // cleate router of fast ISS
    mpIsRouter = new OSCI2DCDR<BUS_WIDTH_VCI32>("is_router", mFastissRouterMapFname);
	
    // cleate bridge for isgp
    mpIsgpBridge = new I_BRIDGE<BUS_WIDTH_VCI, BUS_WIDTH_VCI32>("isgp_bridge");

    // cleate bridge for isgc
    mpIsgcBridge = new I_BRIDGE<BUS_WIDTH_VCI, BUS_WIDTH_VCI32>("isgc_bridge");
    
    // cleate bridge for ish
    mpIshBridge = new I_BRIDGE<BUS_WIDTH_VCI, BUS_WIDTH_VCI32>("ish_bridge");

    // cleate bridge for isi
    mpIsiBridge = new I_BRIDGE<BUS_WIDTH_VCI, BUS_WIDTH_VCI32>("isi_bridge");

    // cleate bridge for is
    mpIsBridge = new I_BRIDGE<BUS_WIDTH_VCI32, BUS_WIDTH_VCI32>("is_bridge");

    // cleate bridge for G4_PEG_Wrapper tsv
    mpTspegBridge = new I_BRIDGE<BUS_WIDTH_VCI, BUS_WIDTH_VCI32>("tspeg_bridge");

    // cleate bridge for G4_PEG_Wrapper is
    mpIspegBridge = new I_BRIDGE<BUS_WIDTH_VCI32, BUS_WIDTH_VCI>("ispeg_bridge");

    // cleate G4_PEG_Wrapper (PEG, PEGCAP_M, PEGCAP_S)
    mpG4PegWrapper = new G4_PEG_Wrapper("G4_PEG_Wrapper", mConfigFile, 0, 0, mPeId);

    /*************************************/
    /* Cleate signal                     */
    /*************************************/
    // create Syserr ack signal
    SyserrAckNm_sig = new sc_signal<bool>("SyserrAckNm_sig");
    
    // create Syserr req signal
    SyserrReqNm_sig = new sc_signal<bool>("SyserrReqNm_sig");
    
    // create Syserr code signal
    SyserrCodeNm_sig = new sc_signal<sc_uint<8> >("SyserrCodeNm_sig");

    // create Syserr mode signal
    SyserrModeNm_sig = new sc_signal<bool>("SyserrModeNm_sig");
  
    /*************************************/
    /* Connect all instanse for PECORESS */
    /*************************************/
    // connect fast ISS's initiator with payload adder's target
#ifdef VLAB
    if (tlm_shortcut)
    {
      (mpFastIssMemWrapper_ASTC->bus_initiator)(pTlmShortcut->target_socket);
      (pTlmShortcut->initiator_socket)(mpPayloadAdder->ts);
    }
    else
    {
      (mpFastIssMemWrapper_ASTC->bus_initiator)(mpPayloadAdder->ts);
    }
#else
    (mpFastIssMemWrapper_ASTC->bus_initiator)(mpPayloadAdder->ts);
#endif
      
    // connect payload adder's initiator with router's target
    (mpPayloadAdder->is)(mpIsRouter->ts);
    
    // connect router's initiator with bridge's target (for isgp)
    (mpIsRouter->is)(mpIsgpBridge->ts);

    // connect router's initiator with bridge's target (for isgc)
    (mpIsRouter->is)(mpIsgcBridge->ts);
    
    // connect router's initiator with bridge's target (for ish)
    (mpIsRouter->is)(mpIshBridge->ts);
    
    // connect router's initiator with bridge's target (for isi)
    (mpIsRouter->is)(mpIsiBridge->ts);
    
    // connect router's initiator with bridge's target (for is)
    (mpIsRouter->is)(mpIsBridge->ts);
    
    // connect bridge's initiator with FastISS's target
    (mpIsRouter->is)(mpFastIssMemWrapper_ASTC->external_bus_target);

    // connect FastISS initiator memory wrapper target
    (mpFastIss_ASTC->bus_initiator)(mpFastIssMemWrapper_ASTC->fastiss_bus_target);

    // connect bridge's initiator with PECORESS.is (for Other PE resource or GRAM)
    (mpIsBridge->is)(is);

    // connect bridge's initiator with PECORESS.isgp, PECORESS.isgc, PECORESS.ish, PECORESS.isi
    (mpIsgpBridge->is)(isgp);
    (mpIsgcBridge->is)(isgc);
    (mpIshBridge->is)(ish);
    (mpIsiBridge->is)(isi);
    
    // connect router's target soccket with PECORESS.ts (for Other master accsess)
    (ts)(mpIsRouter->ts);

    // connect PEG(wrapper)'s to each bus I/F
    (mpG4PegWrapper->is)(mpIspegBridge->ts);
    (mpIspegBridge->is)(mpIsRouter->ts);
    (ts_peg_v)(mpTspegBridge->ts);
    (mpTspegBridge->is)(mpG4PegWrapper->tsv);
    (ts_peg_p)(mpG4PegWrapper->tsp);
    (ts_peg_m)(mpG4PegWrapper->ts_m);
    (ts_peg_s)(mpG4PegWrapper->ts_s);

    // connect clock ans reset 
    (mpPayloadAdder->sys_reset)(sys_reset);
    (mpFastIss_ASTC->reset)(sys_reset);
    (mpG4PegWrapper->presetz)(sys_reset);
    (mpFastIss_ASTC->clock)(sys_clk);
    (mpG4PegWrapper->PCLK)(sys_clk);
  
    // connect interrupt port      
    (mpFastIss_ASTC->fenmirq)(FenmiReq);
    (mpFastIss_ASTC->feintrq)(FeintReq);
    (mpFastIss_ASTC->eiintrq)(EiintReq);
    (mpFastIss_ASTC->fenmirq_ack)(FenmiAck);
    (mpFastIss_ASTC->feintrq_ack)(FeintAck);
    (mpFastIss_ASTC->eiintrq_ack)(EiintAck);
    (mpFastIss_ASTC->cpugminfo)(CpuGmInfo);

    // connect PEG error port      
    (mpG4PegWrapper->ERR_INFO)(PegInfoIn);
    (mpG4PegWrapper->PEG_ERR_M)(PegInfoOut);
    (mpG4PegWrapper->GRDERR_M)(PegErrM);
    (mpG4PegWrapper->GRDERR_S)(PegErrS);

    (*SyserrAckNm_sig) = false;
    (*SyserrReqNm_sig) = false;
    (*SyserrCodeNm_sig) = 0;
    (*SyserrModeNm_sig) = false;

    // connect syserr signal and FastISS
    (mpFastIss_ASTC->syserr_ack)(*SyserrAckNm_sig);
    (mpFastIss_ASTC->syserr_req)(*SyserrReqNm_sig);
    (mpFastIss_ASTC->syserr_code)(*SyserrCodeNm_sig);
    (mpFastIss_ASTC->syserr_mode)(*SyserrModeNm_sig);
  }


  SC_METHOD( SyserrAckProcess );
  sensitive << (*SyserrAckNm_sig);
  dont_initialize();

  SC_METHOD( Reset );
  sensitive << sys_reset;
  dont_initialize();

  SC_METHOD( SyserrInitProcess );
  sensitive << syserr_init_event;
  dont_initialize();
}


///<summary>Destructor</summary>
///<returns>none</returns>
PECORESS::
~PECORESS()
{
  if(mFastIssMode != FASTISS_NONE){
    delete mpFastIss_ASTC;
    delete mpFastIssMemWrapper_ASTC;
    delete mpPayloadAdder;
#ifdef VLAB_0
    if (tlm_shortcut)
    {
      delete pTlmShortcut;
    }
#endif
    delete mpIsRouter;
    delete mpIsgpBridge;
    delete mpIsgcBridge;
    delete mpIshBridge;
    delete mpIsiBridge;
    delete mpIsBridge;
    delete mpG4PegWrapper;
    delete SyserrAckNm_sig;
    delete SyserrReqNm_sig;
    delete SyserrCodeNm_sig;
    delete SyserrModeNm_sig;

  }
} 


///<summary>set fastiss mode</summary>
///<returns>pointer of fast ISS</returns>
rh850g4::core* PECORESS::GetFastIssPtr( void )
{
  return(mpFastIss_ASTC);
}


///<summary>get fastiss memory wrapper</summary>
///<returns>pointer of fast ISS memory wrapper</returns>
rh850g4::memory_wrapper* PECORESS::GetFastIssMemWrapperPtr( void )
{
  return(mpFastIssMemWrapper_ASTC);
}


/// <summary>set PEID</summary>
/// <param name="peid">PEID</param>
/// <returns>none</returns>
void PECORESS::SetPeId(unsigned int peid)
{
  mPeId = peid;
  mpPayloadAdder->setPeId(mPeId);
  mpG4PegWrapper->SetPeId(mPeId);
  read_config_file_PeType(mConfigFile);
}


///<summary>set Syserr output request</summary>
///<param name="code">error code</param>
///<returns>none</returns>
void PECORESS::setSyserrOut( unsigned int code )
{
#ifdef TEST_CORE_DBG
  printf("%s:%s: called setSyserrOut(%d, 0x%2x)\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name(),
	 mPeId,
	 code);
  fflush(stdout);
#endif
  (*SyserrReqNm_sig) = true;
  (*SyserrCodeNm_sig) = ((sc_uint<8> )(code & 0x1f));
  (*SyserrModeNm_sig) = true;
}


///<summary>set Syserr (Operand factor) output request</summary>
///<param name="code">error code</param>
///<returns>none</returns>
void PECORESS::setSyserrOperandOut( unsigned int code )
{
#ifdef TEST_CORE_DBG
  printf("%s:%s: called setSyserrOperandOut(%d, 0x%2x)\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name(),
	 mPeId,
	 code);
  fflush(stdout);
#endif
  bool isValidFactor = false;

  if(mPeType == CPU_G4MH_2_0){
    if ((code == 0x18) || (code == 0x19) || (code == 0x1a)){
      (*SyserrReqNm_sig) = true;
      (*SyserrCodeNm_sig) = ((sc_uint<8> )(code & 0x1f));
      (*SyserrModeNm_sig) = false;
      isValidFactor = true;
    }
  }
  if(!isValidFactor){
    char error_message_buf[1024];
    // for WinMsg
    sprintf(error_message_buf,
	    "Unexpected syserr factor was specified [0x%1x] by setSyserrOperandOut().",
	    code);
    printWarningMessage(name(), "setSyserrOperandOut", error_message_buf);     
  }
}

/// <summary>notify syserr ack from fastISS</summary>
/// <returns>none</returns>
void PECORESS::SyserrAckProcess(void)
{
  if(*SyserrAckNm_sig == true){
    syserr_init_event.notify();
  }
}


/// <summary>Reset function</summary>
/// <returns>none</returns>
void PECORESS::Reset(void)
{
  if(sys_reset.read()){
    syserr_init_event.notify();
  }
}


/// <summary>Initialize the syserr info signal function</summary>
/// <returns>none</returns>
void PECORESS::SyserrInitProcess(void)
{
  (*SyserrReqNm_sig) = false;
  (*SyserrCodeNm_sig) = 0;
  (*SyserrModeNm_sig) = false;
}


///<summary>set the PE guard's parameter</summary>
///<returns>none</returns>
void PECORESS::SetPegInfo ( void )
{
  unsigned int base_addr = PEG_INIT_BASE;
  mpG4PegWrapper->mG4_PEG->tsp->setBaseAddressSize(base_addr+(0x100*mPeId), 0x100);
  if(mPeType == CPU_G4MH_2_0){
    mpG4PegWrapper->mG4_PEGCAP_M->ts->setBaseAddressSize(base_addr+(0x800+(0x40*mPeId)), 0x20);
    mpG4PegWrapper->mG4_PEGCAP_S->ts->setBaseAddressSize(base_addr+(0x820+(0x40*mPeId)), 0x20);
  }else{ 
    mpG4PegWrapper->mG4_PEGCAP_M->ts->setBaseAddressSize(base_addr+(0x800+(0x20*mPeId)), 0x10);
    mpG4PegWrapper->mG4_PEGCAP_S->ts->setBaseAddressSize(base_addr+(0x810+(0x20*mPeId)), 0x10);
  }
}

/*********************/
/*  Private function */
/*********************/

///<summary>read the configulation file to get each parameter</summary>
///<param name="filename">file name of configulation file</param>
///<returns>none</returns>
void PECORESS::read_config_file( const char *filename )
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

    // fast ISS's router map file name
    if( strcmp( token, "[FASTISS_MAP_FILE]" ) == 0 ) {
      token = strtok(0, seps);
      const char *fname = (!token)? "" : token;
      strcpy(mFastissRouterMapFname, fname);
#ifdef TEST_CORE_DBG
      printf("configuration test(PECORESS):%s :mFastissRouterMapFname=%s\n",
	     "[FASTISS_MAP_FILE]", mFastissRouterMapFname);
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
      printf("configuration test(PECORESS):[SIM_MODE] mIssMode:=%d\n",
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
      printf("configuration test(PECORESS):[FASTISS] :mFastIssMode=%d\n",
	     mFastIssMode);
      fflush(stdout);
#endif
      continue;
    } 
  }  // end of configuration file read loop(infinite)
}  // end of read_config_file()


///<summary>read the configulation file to get effective PE type</summary>
///<param name="filename">file name of configulation file</param>
///<returns>none</returns>
void PECORESS::read_config_file_PeType( const char *filename )
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
    if (strcmp(token, "[G4CPU_PE_TYPE]") == 0) {
      unsigned int peid=0;
      token = strtok(0, seps);
      if (token != NULL){
        peid = token_to_uint( token,
                              name(),
                              "[G4CPU_PE_TYPE] peid(1st parameter)" );
        if( peid == mPeId ){
          token = strtok(0, seps);
	  if(strcmp(token, "G4MH") == 0){
	    mPeType = CPU_G4MH_1_0;
	  }else if(strcmp(token, "G4MH_V10") == 0){
	    mPeType = CPU_G4MH_1_0;
	  }else if(strcmp(token, "G4MH_V11") == 0){
	    mPeType = CPU_G4MH_1_1;
	  }else if(strcmp(token, "G4MH_V20") == 0){
	    mPeType = CPU_G4MH_2_0;
	  } else{
            char error_message_buf[1024];
            sprintf(error_message_buf,
		    "Unexpected token was specified [%s] on [G4CPU_PE_TYPE] PE type(2nd parmeter).",
		    token);
            printErrorMessage(name(), "read_config_file_PeType", error_message_buf);
            error_detected = true;
	  }
	}
      }
#ifdef TEST_CORE_DBG
      printf("configuration test(PECORESS):[G4CPU_PE_TYPE]:PE%d :mPeType=%d\n",
	     peid, mPeType);
      fflush(stdout);
#endif
      continue;
    }
  }  // end of configuration file read loop(infinite)
}  // end of read_config_file()

///<summary>set simulation mode</summary>
///<param name="iss_mode">simulation mode</param>
///<returns>none</returns>
void PECORESS::SetSimMode( SIM_MODE_T iss_mode )
{
  mIssMode = iss_mode;
}


///<summary>set fastiss mode</summary>
///<param name="fastiss_mode">fastiss mode</param>
///<returns>none</returns>
void PECORESS::SetFastIssMode( FASTISS_MODE_T fastiss_mode )
{
  mFastIssMode = fastiss_mode;
}

///<summary>set fastiss mode</summary>
///<returns>none</returns>
void PECORESS::SetInitVal( void )
{
}

