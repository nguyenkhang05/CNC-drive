/*************************************************************************
 *
 *  CLUSTERSS.cpp
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

#include "CLUSTERSS.h"
#ifndef FASTISS_NO_SUPPORT
#include "PECORESS.h"
#include "I_BRIDGE.h"
#endif
#include "I_CONNECTOR.h"
#include "VCI2VCI32.h"
#include "OSCI2DCDR.h"
#include "DummySlave.h"


using namespace CmErrMsg; // for WinMsg
#ifndef FASTISS_NO_SUPPORT
using namespace rh850g4;    // for ASTC's fastiss
#endif // #ifndef FASTISS_NO_SUPPORT

///<summary>Constructor</summary>
///<param name="module_name">module name</param>
///<param name="config_file">file name of configulation file</param>
///<param name="pe_num">PE number of this Cluster</param>
///<returns>none</returns>
CLUSTERSS::
CLUSTERSS(sc_module_name module_name, const char *config_file, unsigned int pe_num ) : 
  sc_module(module_name),
  mpExRouter(0),
  mpIsBridge(0),
  sys_clk("sys_clk"),
  sys_reset("sys_reset"),
  is(0),
  ish("ish"),
  isgp("isgp"),
  isgc("isgc"),
  ts(0),
  mIssMode(SIM_MODE_CA),
  mFastIssMode(FASTISS_NONE),
  mConfigFile(config_file),
  mPeNum(pe_num)
{

  char port_name[64];
  char mod_name[64];

  if(mPeNum > CL_PE_NUM){
    char error_message_buf[1024];
    // for WinMsg
    sprintf(error_message_buf, "The PE number of a CLUSTER must be under %d", CL_PE_NUM+1); 
    printErrorMessage(name(), "CLUSTERSS", error_message_buf);
    exit(1);
  }

  read_config_file(mConfigFile);

  /************************************/
  /* Cleate all instanses CLUSTERSS */
  /************************************/
#ifndef FASTISS_NO_SUPPORT
  // set pointer of FastISS instance
  if(mFastIssMode != FASTISS_NONE){
    is = new TlmInitiatorSocket<BUS_WIDTH_VCI32>("is");
    ts = new TlmTargetSocket<BUS_WIDTH_VCI32>("ts");
    // cleate router of fast ISS
    sprintf(mod_name, "is_router");
    mpExRouter = new OSCI2DCDR<BUS_WIDTH_VCI32>(mod_name, mFastissRouterMapFname);
    // cleate bridge for is
    mpIsBridge = new I_BRIDGE<BUS_WIDTH_VCI32, BUS_WIDTH_VCI32>("is_bridge");
  }
#endif

  // cleate isgp connector
  sprintf(mod_name, "isgp_bw_decoder");
  mpIsgpConnector = new I_CONNECTOR<BUS_WIDTH_VCI>(mod_name);
  
  // cleate isgc connector
  sprintf(mod_name, "isgc_bw_decoder");
  mpIsgcConnector = new I_CONNECTOR<BUS_WIDTH_VCI>(mod_name);
  
  // cleate ish connector
  sprintf(mod_name, "ish_bw_decoder");
  mpIshConnector = new I_CONNECTOR<BUS_WIDTH_VCI>(mod_name);
  
  // cleate bridge for isgc
  mpIsgcVci2Vci32Bridge = new VCI2VCI32("isgc_vci2vci32_bridge");

  for(unsigned int i=0; i<CL_PE_NUM; i++){
    // for interrupt port
    sprintf(port_name, "PE%d_FenmiReq", i);
    PE_FenmiReq[i] = new sc_in<bool>(port_name);
    sprintf(port_name, "PE%d_FenmiAck", i);
    PE_FenmiAck[i] = new sc_out<bool>(port_name);
    sprintf(port_name, "PE%d_FeintReq", i);
    PE_FeintReq[i] = new sc_in<unsigned int>(port_name);
    sprintf(port_name, "PE%d_FeintAck", i);
    PE_FeintAck[i] = new sc_out<unsigned int>(port_name);
    sprintf(port_name, "PE%d_EiintReq", i);
    PE_EiintReq[i] = new sc_in<sc_dt::uint64>(port_name);
    sprintf(port_name, "PE%d_EiintAck", i);
    PE_EiintAck[i] = new sc_out<unsigned int>(port_name);
    sprintf(port_name, "PE%d_CpuGmInfo", i);
    PE_CpuGmInfo[i] = new sc_out<unsigned int>(port_name);

    if(i<mPeNum){
      // cleate isi (for INTC1 I/F)
      sprintf(port_name, "PE%d_isi", i);
      PE_isi[i] = new TlmInitiatorSocket<BUS_WIDTH_VCI32>(port_name);
      // cleate isi connector
      sprintf(mod_name, "PE%d_isi_bw_decoder", i);
      mpIsiConnector[i] = new I_CONNECTOR<BUS_WIDTH_VCI>(mod_name);
      // cleate bridge for isi
      sprintf(mod_name, "PE%d_isi_vci2vci32_bridge", i);
      mpIsiVci2Vci32Bridge[i] = new VCI2VCI32(mod_name);
      // cleate ts_psg_p, ts_peg_m, te_peg_s
      sprintf(port_name, "PE%d_ts_peg_p", i);
      PE_ts_peg_p[i] = new TlmTargetSocket<BUS_WIDTH_APB>(port_name);
      sprintf(port_name, "PE%d_ts_peg_m", i);
      PE_ts_peg_m[i] = new TlmTargetSocket<BUS_WIDTH_APB>(port_name);
      sprintf(port_name, "PE%d_ts_peg_s", i);
      PE_ts_peg_s[i] = new TlmTargetSocket<BUS_WIDTH_APB>(port_name);
#ifndef FASTISS_NO_SUPPORT
      if(mFastIssMode != FASTISS_NONE){
	// cleate PECORESS
	sprintf(mod_name, "PE%d_PECORESS", i);
	mpPECORESS[i] = new PECORESS(mod_name, mConfigFile);
	// for PEG error port
	sprintf(port_name, "PE%d_PegInfoIn", i);
	PE_PegInfoIn[i] = new sc_in<G4_PEG_Cerrinfo>(port_name);
	sprintf(port_name, "PE%d_PegInfoOut", i);
	PE_PegInfoOut[i] = new sc_out<G4_PEG_Cerrinfo>(port_name);
	sprintf(port_name, "PE%d_PegErrM", i);
	PE_PegErrM[i] = new sc_out<bool>(port_name);
	sprintf(port_name, "PE%d_PegErrS", i);
	PE_PegErrS[i] = new sc_out<bool>(port_name);
	mpSlvDummy_pegp[i] = 0;
	mpSlvDummy_pegm[i] = 0;
	mpSlvDummy_pegs[i] = 0;
      }else{
	mpPECORESS[i] = 0;
	PE_PegInfoIn[i] = 0;
	PE_PegInfoOut[i] = 0;
	PE_PegErrM[i] = 0;
	PE_PegErrS[i] = 0;
	sprintf(mod_name, "DummySlave%d_pegp", i);
	mpSlvDummy_pegp[i] = new DummySlave<BUS_WIDTH_APB>(mod_name);
	sprintf(mod_name, "DummySlave%d_pegm", i);
	mpSlvDummy_pegm[i] = new DummySlave<BUS_WIDTH_APB>(mod_name);
	sprintf(mod_name, "DummySlave%d_pegs", i);
	mpSlvDummy_pegs[i] = new DummySlave<BUS_WIDTH_APB>(mod_name);
      }
#else
      mpPECORESS[i] = 0;
      PE_PegInfoIn[i] = 0;
      PE_PegInfoOut[i] = 0;
      PE_PegErrM[i] = 0;
      PE_PegErrS[i] = 0;
      sprintf(mod_name, "DummySlave%d_pegp", i);
      mpSlvDummy_pegp[i] = new DummySlave<BUS_WIDTH_APB>(mod_name);
      sprintf(mod_name, "DummySlave%d_pegm", i);
      mpSlvDummy_pegm[i] = new DummySlave<BUS_WIDTH_APB>(mod_name);
      sprintf(mod_name, "DummySlave%d_pegs", i);
      mpSlvDummy_pegs[i] = new DummySlave<BUS_WIDTH_APB>(mod_name);
#endif
    }else{
      mpPECORESS[i]      = 0;
      PE_PegInfoIn[i]  = 0;
      PE_PegInfoOut[i]  = 0;
      PE_PegErrM[i]      = 0;
      PE_PegErrS[i]      = 0;
      mpSlvDummy_pegp[i] = 0;
      mpSlvDummy_pegm[i] = 0;
      mpSlvDummy_pegs[i] = 0;
      PE_isi[i]          = 0;
      PE_ts_peg_p[i]     = 0;
      PE_ts_peg_m[i]     = 0;
      PE_ts_peg_s[i]     = 0;
      mpIsiConnector[i]  = 0;
      mpIsiVci2Vci32Bridge[i]    = 0;
    }
  }


  /*************************************/
  /* Connect all instanse for CLUSTERSS */
  /*************************************/

#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    for(unsigned int i=0; i<mPeNum; i++){
      // for interrupt port
      (mpPECORESS[i]->FenmiReq)(*((PE_FenmiReq)[i]));
      (mpPECORESS[i]->FenmiAck)(*((PE_FenmiAck)[i]));
      (mpPECORESS[i]->FeintReq)(*((PE_FeintReq)[i]));
      (mpPECORESS[i]->FeintAck)(*((PE_FeintAck)[i]));
      (mpPECORESS[i]->EiintReq)(*((PE_EiintReq)[i]));
      (mpPECORESS[i]->EiintAck)(*((PE_EiintAck)[i]));
      (mpPECORESS[i]->CpuGmInfo)(*((PE_CpuGmInfo)[i]));
      (mpPECORESS[i]->PegInfoIn)(*((PE_PegInfoIn)[i]));
      (mpPECORESS[i]->PegInfoOut)(*((PE_PegInfoOut)[i]));
      (mpPECORESS[i]->PegErrM)(*((PE_PegErrM)[i]));
      (mpPECORESS[i]->PegErrS)(*((PE_PegErrS)[i]));

      // for clock and reset port
      (mpPECORESS[i]->sys_clk)(sys_clk);
      (mpPECORESS[i]->sys_reset)(sys_reset);

      // for bus port
      (mpPECORESS[i]->isgp)(mpIsgpConnector->ts);
      (mpPECORESS[i]->isgc)(mpIsgcConnector->ts);
      (mpPECORESS[i]->ish)(mpIshConnector->ts);
      (mpPECORESS[i]->isi)(mpIsiConnector[i]->ts);
      (mpPECORESS[i]->is)(mpExRouter->ts);
      (mpExRouter->is)(mpPECORESS[i]->ts);
      (mpExRouter->is)(mpPECORESS[i]->ts_peg_v);
      (*(PE_ts_peg_p[i]))(mpPECORESS[i]->ts_peg_p);
      (*(PE_ts_peg_m[i]))(mpPECORESS[i]->ts_peg_m);
      (*(PE_ts_peg_s[i]))(mpPECORESS[i]->ts_peg_s);
    }

    // for bus port
    (mpExRouter->is)(mpIsBridge->ts);
    (mpIsBridge->is)(*is);
    (*ts)(mpExRouter->ts);
  }else{
    for(unsigned int i=0; i<mPeNum; i++){
      (*(PE_ts_peg_p[i]))(mpSlvDummy_pegp[i]->ts);
      (*(PE_ts_peg_m[i]))(mpSlvDummy_pegm[i]->ts);
      (*(PE_ts_peg_s[i]))(mpSlvDummy_pegs[i]->ts);
    }
  }
#else
  for(unsigned int i=0; i<mPeNum; i++){
    (*(PE_ts_peg_p[i]))(mpSlvDummy_pegp[i]->ts);
    (*(PE_ts_peg_m[i]))(mpSlvDummy_pegm[i]->ts);
    (*(PE_ts_peg_s[i]))(mpSlvDummy_pegs[i]->ts);
  }
#endif // #ifndef FASTISS_NO_SUPPORT

  // for clock and reset port
  (mpIsgcVci2Vci32Bridge->vci32FreqPort)(sys_clk);
  (mpIsgcVci2Vci32Bridge->vciFreqPort)(sys_clk);
  (mpIsgcVci2Vci32Bridge->resetPort)(sys_reset);
  mpIsgcVci2Vci32Bridge->setTlmTransType(mIssMode);
  for(unsigned int i=0; i<mPeNum; i++){
    (mpIsiVci2Vci32Bridge[i]->vci32FreqPort)(sys_clk);
    (mpIsiVci2Vci32Bridge[i]->vciFreqPort)(sys_clk);
    (mpIsiVci2Vci32Bridge[i]->resetPort)(sys_reset);
    mpIsiVci2Vci32Bridge[i]->setTlmTransType(mIssMode);
  }
  // for bus port
  (mpIsgpConnector->is)(isgp);
  (mpIsgcConnector->is)(*mpIsgcVci2Vci32Bridge->ts);
  (*mpIsgcVci2Vci32Bridge->is)(isgc);
  (mpIshConnector->is)(ish);
  for(unsigned int i=0; i<mPeNum; i++){
    (mpIsiConnector[i]->is)(*mpIsiVci2Vci32Bridge[i]->ts);
    (*mpIsiVci2Vci32Bridge[i]->is)(*((PE_isi)[i]));
  }
}


///<summary>Destructor</summary>
///<returns>none</returns>
CLUSTERSS::~CLUSTERSS(void)
{
#ifndef FASTISS_NO_SUPPORT
  if(mFastIssMode != FASTISS_NONE){
    delete is;
    delete ts;
    delete mpExRouter;
    delete mpIsBridge;
  }
#endif
  delete mpIsgpConnector;
  delete mpIsgcConnector;
  delete mpIshConnector;
  delete mpIsgcVci2Vci32Bridge;

  for(unsigned int i=0; i<CL_PE_NUM; i++){
    delete PE_FenmiReq[i];
    delete PE_FenmiAck[i];
    delete PE_FeintReq[i];
    delete PE_FeintAck[i];
    delete PE_EiintReq[i];
    delete PE_EiintAck[i];
    delete PE_CpuGmInfo[i];

    if(i<mPeNum){
      delete PE_isi[i];
      delete mpIsiConnector[i];
      delete mpIsiVci2Vci32Bridge[i];
      delete PE_ts_peg_p[i];
      delete PE_ts_peg_m[i];
      delete PE_ts_peg_s[i];

#ifndef FASTISS_NO_SUPPORT
      if(mFastIssMode != FASTISS_NONE){
	delete mpPECORESS[i];
	delete PE_PegInfoIn[i];
	delete PE_PegInfoOut[i];
	delete PE_PegErrM[i];
	delete PE_PegErrS[i];
      }else{
	delete mpSlvDummy_pegp[i];
	delete mpSlvDummy_pegm[i];
	delete mpSlvDummy_pegs[i];
      }
#else
      delete mpSlvDummy_pegp[i];
      delete mpSlvDummy_pegm[i];
      delete mpSlvDummy_pegs[i];
#endif
    }
  }
}


/// <summary>set PEID</summary>
/// <param name="pe0_peid">PEID(PE0)</param>
/// <param name="pe1_peid">PEID(PE1)</param>
void CLUSTERSS::SetPeId( unsigned int pe0_peid, unsigned int pe1_peid)
{
#ifndef FASTISS_NO_SUPPORT
  if(mPeNum==2){
    mpPECORESS[1]->SetPeId(pe1_peid);
  }
  mpPECORESS[0]->SetPeId(pe0_peid);
#endif
}


///<summary>set Syserr output request</summary>
///<param name="peid">pe id</param>
///<param name="code">error code</param>
///<returns>none</returns>
void CLUSTERSS::setSyserrOut( unsigned int peid, unsigned int code )
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
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mFastIssMode){
      if(peid < mPeNum){
	mpPECORESS[peid]->setSyserrOut(code);
      }
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>set Syserr (Opeand factor) output request</summary>
///<param name="peid">pe id</param>
///<param name="code">error code</param>
///<returns>none</returns>
void CLUSTERSS::setSyserrOperandOut( unsigned int peid, unsigned int code )
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
    // ignore
  }else{
#ifndef FASTISS_NO_SUPPORT
    if(FASTISS_NONE != mFastIssMode){
      if(peid < mPeNum){
	mpPECORESS[peid]->setSyserrOperandOut(code);
      }
    }
#endif // #ifndef FASTISS_NO_SUPPORT
  }
}


///<summary>read the configulation file to get each parameter</summary>
///<param name="filename">file name of configulation file</param>
///<returns>none</returns>
void CLUSTERSS::read_config_file( const char *filename )
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
      printf("configuration test(CLUSTERSS):%s :mFastissRouterMapFname=%s\n",
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
      printf("configuration test(CLUSTERSS):[SIM_MODE] mIssMode:=%d\n",
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
      printf("configuration test(CLUSTERSS):[FASTISS] :mFastIssMode=%d\n",
	     mFastIssMode);
      fflush(stdout);
#endif
      continue;
    }
  }  // end of configuration file read loop(infinite)
}  // end of read_config_file()


///<summary>set simulation mode</summary>
///<param name="iss_mode">simulation mode</param>
///<returns>none</returns>
void CLUSTERSS::SetSimMode( SIM_MODE_T iss_mode )
{
  mIssMode = iss_mode;
}


///<summary>set fastiss mode</summary>
///<param name="fastiss_mode">fastiss mode</param>
///<returns>none</returns>
void CLUSTERSS::SetFastIssMode( FASTISS_MODE_T fastiss_mode )
{
  mFastIssMode = fastiss_mode;
}

