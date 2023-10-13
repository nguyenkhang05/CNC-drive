//----------------------------------------------------------------------
//* File: DummyMasterRvc.cpp
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of DummyMasterRvc.cpp
// Ref: {SPIDCTL_UT_VRFCopyRight_001}
#include "DummyMasterRvc.h"

/*********************************
// Function     : DummyMasterRvc
// Description  : Constructor of DummyMasterRvc class
// Parameter    :
//      name            Module name
//      rLatency        Read latency
//      wLatency        Write latency
// Return value : None
**********************************/
// Description: DummyMasterRvc model is used for signal issuing/receiving (control ports)
// Ref: {SPIDCTL_UT_VRFDummyMaster_001}
DummyMasterRvc::DummyMasterRvc(sc_module_name name, unsigned int rLatency, unsigned int wLatency, SIM_MODE_T simmode):
                               sc_module(name),
                               BusMasterBase<64,1>(),
							   BusSlaveBase<32,1>(),
                               freqPortApb("freqPortApb"),
                               resetPort("resetPort")
{
    setMasterResetPort64(&resetPort);
    setMasterFreqPort64(&freqPortApb);
    setInitiatorSocket64("is");

    setSlaveResetPort32(&resetPort);
    setSlaveFreqPort32(&freqPortApb);
    setTargetSocket32("ts");

    is = BusMasterBase<64,1>::iSocket64[0];    //Alias for binding
    ts = BusSlaveBase<32,1>::tSocket32[0];

    mDummyMasterRvc_Func = new DummyMasterRvc_Func((std::string)name, this, mBusMasterIf64[0], mBusSlaveIf32[0], simmode);
    mBusMasterIf64[0]->setFuncModulePtr(mDummyMasterRvc_Func);
    mBusMasterIf64[0]->setBusProtocol(BUS_APB);
    mBusMasterIf64[0]->setTransNmbLmt(0x1);
    mBusMasterIf64[0]->setTlmTransType(simmode);

    mBusSlaveIf32[0]->setFuncModulePtr(mDummyMasterRvc_Func);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);
    
    if (simmode == SIM_MODE_CA){
        mDummyMasterRvc_Func->SetSimMode("AT");
    }else{
        mDummyMasterRvc_Func->SetSimMode("LT");
    }
 
    SC_METHOD(TransMethod);
    dont_initialize();
    sensitive << trans_event;

    mEnableDumpMessage = true;
    extenstionnull = false;
}

// Ref: {SPIDCTL_UT_VRFIntroduction_001}
/*********************************
// Function     : ~DummyMasterRvc
// Description  : Destructor of DummyMasterRvc class
// Parameter    : None
// Return value : None
**********************************/
DummyMasterRvc::~DummyMasterRvc (void)
{
    delete mDummyMasterRvc_Func;
}

/*********************************
// Function     : NotifyTransTransfer
// Description  : Notify a transaction
// Parameter    : None
// Return value : None
**********************************/
void DummyMasterRvc::NotifyTransTransfer(void)
{
    trans_event.notify(SC_ZERO_TIME);
}

/*********************************
// Function     : TransMethod
// Description  : Issue a transaction to target bus by calling
//                TransProcess() of mDummyMasterFunc
// Parameter    : None
// Return value : None
**********************************/
void DummyMasterRvc::TransMethod(void)
{//{{{
    if (mEnableDumpMessage){
        printf("Info [%s] (%s) Issuing transaction to bus ...\n", sc_time_stamp().to_string().c_str(), this->name());
		// mDummyMasterRvc_Func->DumpTransactionInfo();
    }
    if (extenstionnull == true) { // extenstion null 
        mDummyMasterRvc_Func->TransProcessextnull();
    } else {
        mDummyMasterRvc_Func->TransProcess();   
    }
    // clear flag check set extenstion null
    extenstionnull = false; 
}//}}}

/*********************************
// Function     : IssueTransaction
// Description  : Issue a transaction to target bus 
//                This function is call by PythonIF
// Parameter    : None
// Return value : None
**********************************/
void DummyMasterRvc::IssueTransaction(const unsigned int command, const unsigned int addr, const unsigned int write_size, const unsigned int data, const unsigned int debug, const unsigned int extenstion)
{//{{{
    bool cmd;
    bool debug_mode;
    if(command==0) {
        cmd = false;
    }
    else {
        cmd = true;
    }
    if(debug==0) {
        debug_mode = false;
    }
    else {
        debug_mode = true;
    }
    mDummyMasterRvc_Func->ConfigTransaction(cmd ,addr, (unsigned char)write_size, data, debug_mode, extenstion);
}//}}}


/*********************************
// Function     : IssueTransaction
// Description  : Issue a transaction to target bus 
//                This function is call by PythonIF with extenstion null
// Parameter    : None
// Return value : None
**********************************/
void DummyMasterRvc::IssueTransactionextnull(const unsigned int command, const unsigned int addr, const unsigned int write_size, const unsigned int data, const unsigned int debug, std::string extenstion)
{//{{{
    bool cmd;
    bool debug_mode;
    if(command==0) {
        cmd = false;
    }
    else {
        cmd = true;
    }
    if(debug==0) {
        debug_mode = false;
    }
    else {
        debug_mode = true;
    }
    if (extenstion == "NULL") {
        extenstionnull = true;        
    }
    mDummyMasterRvc_Func->ConfigTransaction(cmd ,addr, (unsigned char)write_size, data, debug_mode, (unsigned int)0x0);        
}//}}}

/*********************************
// Function     : ReadReceivedData
// Description  : Read received data and judge 
//                This function is call by PythonIF
// Parameter    : None
// Return value : None
**********************************/
void DummyMasterRvc::ReadReceivedData(const unsigned int ex_val) 
{//{{{
    if(mDummyMasterRvc_Func->ReadReceivedData()!=ex_val) {
        printf("[%s] [%s] FAIL: Read data = 0x%08X, Expected = 0x%08X\n", this->name(), sc_time_stamp().to_string().c_str(), (unsigned int)mDummyMasterRvc_Func->ReadReceivedData(), ex_val);
        Judgement(0);
    }
}//}}}

/// Set simulation mode
void DummyMasterRvc::SetSimMode(std::string simmode)
{//{{{
    mDummyMasterRvc_Func->SetSimMode(simmode);
    if (simmode == "AT"){
        mBusMasterIf64[0]->setTlmTransType(SIM_MODE_CA);
    }else{
        mBusMasterIf64[0]->setTlmTransType(SIM_MODE_FAST);
    }
}//}}}

/// Set simulation mode
void DummyMasterRvc::SetTMPass()
{//{{{
    Judgement(1);
}//}}}

/// Set dump info message
void DummyMasterRvc::EnableDumpMessage(bool enable)
{//{{{
    mEnableDumpMessage = enable;
    mDummyMasterRvc_Func->EnableDumpMessage(enable);
}//}}}

/*********************************
// Function     : Judgement
// Description  : Judge this fail or pass
//                
// Parameter    : None
// Return value : None
**********************************/
void DummyMasterRvc::Judgement(unsigned int value) 
{//{{{
    if (value == 0) {
        printf("--------------------------------\n");
        printf("-          TM is FAIL          -\n");
        printf("--------------------------------\n");
        exit(1);
    } else if (value == 1) {
        printf("--------------------------------\n");
        printf("-          TM is PASS          -\n");
        printf("--------------------------------\n");
    }
}//}}}

// vim600: set foldmethod=marker :
