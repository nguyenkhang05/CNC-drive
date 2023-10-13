// ---------------------------------------------------------------------
// $Id: MSPI000.cpp,v 1.19 2020/03/11 00:52:43 ducla Exp $
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "MSPI000.h"
#include "MSPI000_Func.h"
#include "MSPI000_INT_FACTOR_Func.h"
using namespace std;
/// Constructor and Destructor
///*********************************
/// Function     : MSPI000
/// Description  : Constructor
///*********************************
MSPI000::MSPI000(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency, const char* pCfgFileName)
        :sc_module(name)
        ,MSPI000_AgentController()
        ,BusSlaveBase<32,2>()
        // Clock/Reset
        ,PCLK("PCLK")
        ,MSPInCLK("MSPInCLK")
        ,PRESETn("PRESETn")
        ,MSPInRESB("MSPInRESB")
        // Input Output
        ,MSPInCSI("MSPInCSI")
        ,MSPInSOUTDRV("MSPInSOUTDRV")
        ,MSPInDCSI("MSPInDCSI")
        ,MSPInDCSIDRV("MSPInDCSIDRV")
        ,MSPInMD("MSPInMD")
        ,SPI_CONTROL("SPI_CONTROL")
{//{{{
    CommandInit((std::string)name);
    //channel_num
    mChannelNum = 8;
    // chip_select_num
    mChipselectNum = 8;
    // HW Trigger Num
    mHWTriggerNum = 41;
    readCfgFile(pCfgFileName);

    mComparisonTiming = 0.5;
    // New instance and port
    std::ostringstream portName;
    INTMSPInTXm = new sc_out<bool>*[mChannelNum];
    sc_assert(INTMSPInTXm != NULL);
    for (unsigned int i = 0; i < mChannelNum; i++){
        portName.str("");
        portName << "INTMSPInTX" << i;
        INTMSPInTXm[i] = new sc_out<bool>(portName.str().c_str());
        sc_assert(INTMSPInTXm[i] != NULL);
        INTMSPInTXm[i]->initialize(false);
    }

    INTMSPInRXm = new sc_out<bool>*[mChannelNum];
    sc_assert(INTMSPInRXm != NULL);
    for (unsigned int i = 0; i < mChannelNum; i++){
        portName.str("");
        portName << "INTMSPInRX" << i;
        INTMSPInRXm[i] = new sc_out<bool>(portName.str().c_str());
        sc_assert(INTMSPInRXm[i] != NULL);
        INTMSPInRXm[i]->initialize(false);
    }

    INTMSPInFEm = new sc_out<bool>*[mChannelNum];
    sc_assert(INTMSPInFEm != NULL);
    for (unsigned int i = 0; i < mChannelNum; i++){
        portName.str("");
        portName << "INTMSPInFE" << i;
        INTMSPInFEm[i] = new sc_out<bool>(portName.str().c_str());
        sc_assert(INTMSPInFEm[i] != NULL);
        INTMSPInFEm[i]->initialize(false);
    }

    INTMSPInERRm = new sc_out<bool>*[mChannelNum];
    sc_assert(INTMSPInERRm != NULL);
    for (unsigned int i = 0; i < mChannelNum; i++){
        portName.str("");
        portName << "INTMSPInERR" << i;
        INTMSPInERRm[i] = new sc_out<bool>(portName.str().c_str());
        sc_assert(INTMSPInERRm[i] != NULL);
        INTMSPInERRm[i]->initialize(false);
    }

    // Initialize remain output ports
    for (unsigned int i = 0; i < emNumDuplicatePort; i++) {
        MSPInSCK[i].initialize(emInitData);
    }
    
    MSPInCS = new sc_out<bool>*[mChipselectNum];
    sc_assert(MSPInCS != NULL);
    for (unsigned int i = 0; i < mChipselectNum; i++){
        portName.str("");
        portName << "MSPInCS" << i;
        MSPInCS[i] = new sc_out<bool>(portName.str().c_str());
        sc_assert(MSPInCS[i] != NULL);
        MSPInCS[i]->initialize(true);
    }

///////////////////////////////////////////
    MSPInHWTRG = new sc_in<bool>*[mHWTriggerNum];
    sc_assert(MSPInHWTRG != NULL);
    for (unsigned int i = 0; i < mHWTriggerNum; i++){
        portName.str("");
        portName << "MSPInHWTRG" << i;
        MSPInHWTRG[i] = new sc_in<bool>(portName.str().c_str());
        sc_assert(MSPInHWTRG[i] != NULL);
//        MSPInHWTRG[i]->initialize(false);
    }

//////////////////////////////////////////
    SPI_CONTROL.initialize(emInitData);
    for (unsigned int i = 0; i < emNumDuplicatePort; i++) {
        MOSI[i].initialize(emInitData);
    }
    MSPInSOUTDRV.initialize(false);
    MSPInDCSIDRV.initialize(false);
    MSPInMD.initialize(false);
//initialize combine interrupt port 
    INTMSPInTX.initialize(false);
    INTMSPInRX.initialize(false); 
    INTMSPInFE.initialize(false); 
    INTMSPInERR.initialize(false);

    //Bind clock, reset signal and socket to Bus IF
    setSlaveFreqPort32(&PCLK, &PCLK);
    setSlaveResetPort32(&PRESETn, &PRESETn);
    setTargetSocket32((char*)"ts",(char*)"tsif");

    ts = BusSlaveBase<32,2>::tSocket32[0]; //Alias for binding
    mpMSPI000_Func = new MSPI000_Func((std::string)name, mChannelNum, mChipselectNum,  this);
    sc_assert(mpMSPI000_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr((BusSlaveFuncIf *)mpMSPI000_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);
 
    tsif = BusSlaveBase<32,2>::tSocket32[1];
    mpMSPI000_INT_FACTOR_Func = new MSPI000_INT_FACTOR_Func((std::string)name, mChannelNum, this);
    sc_assert(mpMSPI000_INT_FACTOR_Func != NULL);
    mBusSlaveIf32[1]->setFuncModulePtr((BusSlaveFuncIf *)mpMSPI000_INT_FACTOR_Func);
    mBusSlaveIf32[1]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[1]->setReadInitLatency(rLatency);
    mBusSlaveIf32[1]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[1]->setTransNmbLmt(0x1);
    

    // Initialize variables
    mTimeResolutionValue    = 1;
    mTimeResolutionUnit     = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mDumpInterrupt = false;
    mEnableRegisterMessage = true;
    for (unsigned int i = 0; i < emNumClk; i++){
        mClkPeriod[i]       = 0;
        mClkOrgFreq[i]      = 0;
        mClkFreq[i]         = 0;
        mClkFreqUnit[i]     = "Hz";
        mStartClkTime[i]    = 0;
    }
    for (unsigned int i = 0; i < emNumReset; i++){
        mResetCurVal[i]         = true;
        mResetPreVal[i]         = true;
        mIsResetHardActive[i]   = false;
        mIsResetCmdReceive[i]   = false;
        mIsResetCmdActive[i]    = false;
        mResetCmdPeriod[i]      = 0;
    }

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleMSPInCLKSignalMethod);
    dont_initialize();
    sensitive << MSPInCLK;

    SC_METHOD(HandlePRESETnSignalMethod);
    dont_initialize();
    sensitive << PRESETn;

    SC_METHOD(HandleMSPInRESBSignalMethod);
    dont_initialize();
    sensitive << MSPInRESB;
    
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(HandleMSPInSCKISignalMethod);
    dont_initialize();
    sensitive << MSPInSCKI[0];
    sensitive << MSPInSCKI[1];
    sensitive << MSPInSCKI[2];
    sensitive << MSPInSCKI[3];

    SC_METHOD(HandleMSPInCSISignalMethod);
    dont_initialize();
    sensitive << MSPInCSI;

    SC_METHOD(HandleMSPInDCSISignalMethod);
    dont_initialize();
    sensitive << MSPInDCSI;

    SC_METHOD(HandleSPI_CONTROLSignalMethod);
    dont_initialize();
    sensitive << SPI_CONTROL;

    SC_METHOD(WriteMSPInSCKMethod);
    dont_initialize();
    sensitive << mWriteMSPInSCKEvent;

    SC_METHOD(WriteMOSIMethod);
    dont_initialize();
    sensitive << mWriteMOSIEvent;

    SC_METHOD(WriteSPICONTROLMethod);
    dont_initialize();
    sensitive << mWriteSPICONTROLEvent;

    SC_METHOD(WriteMSPInCSMethod);
    dont_initialize();
    sensitive << mWriteMSPInCSEvent;

    for (unsigned int id = 0; id < mHWTriggerNum; id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(MSPInHWTRG[id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::HandleMSPInHWTRGSignalMethod, this),
                          sc_core::sc_gen_unique_name("HandleMSPInHWTRGSignalMethod"), &opt);
    }

    SC_METHOD(WriteTXReserveBufferMethod);
    dont_initialize();
    sensitive << mWriteTXReserveBufferEvent;
    sensitive << mWriteTXReserveBufferEvent1;

    SC_METHOD(WriteTXShiftRegisterMethod);
    dont_initialize();
    sensitive << mWriteTXShiftRegisterEvent;

    SC_METHOD(WriteRXReserveBufferMethod);
    dont_initialize();
    sensitive << mWriteRXReserveBufferEvent;

    SC_METHOD(WriteRXShiftRegisterMethod);
    dont_initialize();
    sensitive << mWriteRXShiftRegisterEvent;

    SC_METHOD(WriteMSPInRXDAm0Method);
    dont_initialize();
    sensitive << mWriteMSPInRXDAm0Event;

    SC_METHOD(EndChannelMethod);
    dont_initialize();
    sensitive << mEndChannelEvent;

    SC_METHOD(EndChannelSlaveMethod);
    dont_initialize();
    sensitive << mEndChannelSlaveEvent;

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteINTMSPInTXmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::WriteINTMSPInTXmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("WriteINTMSPInTXmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertINTMSPInTXmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::AssertINTMSPInTXmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertINTMSPInTXmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeAssertINTMSPInTXmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::DeAssertINTMSPInTXmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("DeAssertINTMSPInTXmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteINTMSPInRXmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::WriteINTMSPInRXmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("WriteINTMSPInRXmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertINTMSPInRXmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::AssertINTMSPInRXmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertINTMSPInRXmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeAssertINTMSPInRXmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::DeAssertINTMSPInRXmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("DeAssertINTMSPInRXmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteINTMSPInFEmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::WriteINTMSPInFEmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("WriteINTMSPInFEmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertINTMSPInFEmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::AssertINTMSPInFEmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertINTMSPInFEmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeAssertINTMSPInFEmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::DeAssertINTMSPInFEmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("DeAssertINTMSPInFEmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteINTMSPInERRmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::WriteINTMSPInERRmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("WriteINTMSPInERRmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertINTMSPInERRmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::AssertINTMSPInERRmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertINTMSPInERRmMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < mChannelNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeAssertINTMSPInERRmEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MSPI000::DeAssertINTMSPInERRmMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("DeAssertINTMSPInERRmMethod"), &opt);
    }

    SC_METHOD(TransmitMethod);
    dont_initialize();
    sensitive << mTransmitEvent;
    
    SC_METHOD(CheckDataconsistencyMethod);
    dont_initialize();
    sensitive << mCheckDataconsistencyEvent;

    SC_METHOD(ReceiveMethod);
    dont_initialize();
    sensitive << mReceiveEvent;
    
    SC_METHOD(WriteINTMSPInTXMethod);
    dont_initialize();
    sensitive << mWriteINTMSPInTXEvent; 
    
    SC_METHOD(WriteINTMSPInRXMethod);
    dont_initialize();
    sensitive << mWriteINTMSPInRXEvent; 
    
    SC_METHOD(WriteINTMSPInFEMethod);
    dont_initialize();
    sensitive << mWriteINTMSPInFEEvent;
    
    SC_METHOD(WriteINTMSPInERRMethod);
    dont_initialize();
    sensitive << mWriteINTMSPInERREvent;
    

    SC_THREAD(CommunicationThread);
    sensitive << mCommunicationEvent;

    SC_THREAD(CommunicationSlaveThread);
    sensitive << mCommunicationSlaveEvent;

    SC_THREAD(EndFrameThread);
    sensitive << mEndFrameEvent;

    SC_THREAD(EndFrameSlaveThread);
    sensitive << mEndFrameSlaveEvent;
}//}}} 

///*********************************
/// Function     : ~MSPI000
/// Description  : Destructor
///*********************************
MSPI000::~MSPI000()
{//{{{
    if (mpMSPI000_Func != NULL) delete mpMSPI000_Func;
    if (mpMSPI000_INT_FACTOR_Func != NULL) delete mpMSPI000_INT_FACTOR_Func;
//    //Free each sub-array
    for(unsigned int i = 0; i < mChannelNum; i++) {
        if (INTMSPInTXm  [i] != NULL) delete INTMSPInTXm    [i];
        if (INTMSPInRXm  [i] != NULL) delete INTMSPInRXm    [i];
        if (INTMSPInFEm [i] != NULL) delete INTMSPInFEm    [i];
        if (INTMSPInERRm [i] != NULL) delete INTMSPInERRm   [i];
    }
    //Free the array of pointers
    if (INTMSPInTXm   != NULL) delete[] INTMSPInTXm;
    if (INTMSPInRXm   != NULL) delete[] INTMSPInRXm;
    if (INTMSPInFEm   != NULL) delete[] INTMSPInFEm;
    if (INTMSPInERRm  != NULL) delete[] INTMSPInERRm;
    
    for(unsigned int i = 0; i < mChipselectNum; i++) {
        if (MSPInCS[i] != NULL) delete MSPInCS[i];
    }
    if (MSPInCS!= NULL) delete[] MSPInCS;
    
    for(unsigned int i = 0; i < mHWTriggerNum; i++) {
        if (MSPInHWTRG[i] != NULL) delete MSPInHWTRG[i];
    }
    if (MSPInHWTRG!= NULL) delete[] MSPInHWTRG;

}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
///*********************************
/// Function     : SetMessageLevel 
/// Description  : Set message level (fatal, error, warning, info)
///*********************************
void MSPI000::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    if (true == mEnableRegisterMessage) {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpMSPI000_Func->RegisterHandler(cmd);
        mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpMSPI000_Func->RegisterHandler(cmd);
        mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);
    } else {
    //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
    
        mpMSPI000_Func->RegisterHandler(cmd);
        mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);
    
        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
    
        mpMSPI000_Func->RegisterHandler(cmd);
        mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);
    }
}//}}}
///*********************************
/// Function     : DumpRegisterRW 
/// Description  : Enable/disable dumping message when users access registers
///*********************************
void MSPI000::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpMSPI000_Func->RegisterHandler(cmd);
    mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);
}//}}}
///*********************************
/// Function     : DumpInterrupt 
/// Description  : Enable/disable dumping interrupt information
///*********************************
void MSPI000::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: MSPI000_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if(mDumpInterrupt){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info","MSPI000_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: MSPI000_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}
///*********************************
/// Function     : EnableRegisterMessage 
/// Description  : Enable/disable EnableRegisterMessage
///*********************************
void MSPI000::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: MSPI000_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        if (mEnableRegisterMessage) {
            re_printf("info","MSPI000_EnableRegisterMessage %s\n", "true"); 
        } else {
            re_printf("info","MSPI000_EnableRegisterMessage %s\n", "false"); 
        }

    } else {
        re_printf("warning","Invalid argument: MSPI000_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : AssertReset 
/// Description  : Command to assert reset
///*********************************
void MSPI000::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETnId])&&(!mIsResetCmdReceive[emPRESETnId])) {
            mIsResetCmdReceive[emPRESETnId] = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPRESETnId].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETnId] = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "MSPInRESB") {
        if ((!mIsResetHardActive[emMSPInRESBId])&&(!mIsResetCmdReceive[emMSPInRESBId])) {
            mIsResetCmdReceive[emMSPInRESBId] = true;
            re_printf("info","The model will be reset (MSPInRESB) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emMSPInRESBId].notify(start_time, SC_NS);
            mResetCmdPeriod[emMSPInRESBId] = period;
        } else {
            re_printf("warning","The software reset of MSPInRESB is called in the reset operation of the model. So it is ignored\n");
        }
    }else{
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn, or MSPInRESB.\n",reset_name.c_str());
    }
}//}}}
///*********************************
/// Function     : SetCLKFreq 
/// Description  : Set clock value
///*********************************
void MSPI000::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mClkOrgFreq[emPCLKId] = freq;
            ConvertClockFreq (mClkFreq[emPCLKId], mClkFreqUnit[emPCLKId], freq, unit);
            if (mClkFreq[emPCLKId] > 0) {
                mClkPeriod[emPCLKId] = (sc_dt::uint64)(((1/(double)mClkFreq[emPCLKId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[emPCLKId] = sc_time_stamp().to_double();
            } else {
                mClkFreq[emPCLKId] = 0;
                // Cancel events
                CancelEvents();
                mIPState = emIDLE;
                // Notify clock is zero
                mClkZeroEvent[emPCLKId].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "MSPInCLK") {
            mClkOrgFreq[emMSPInCLKId] = freq;
            ConvertClockFreq (mClkFreq[emMSPInCLKId], mClkFreqUnit[emMSPInCLKId], freq, unit);
            if (mClkFreq[emMSPInCLKId] > 0) {
                mClkPeriod[emMSPInCLKId] = (sc_dt::uint64)(((1/(double)mClkFreq[emMSPInCLKId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[emMSPInCLKId] = sc_time_stamp().to_double();
            } else {
                mClkPeriod[emMSPInCLKId] = 0;
                // Cancel events
                CancelEvents();
                mIPState = emIDLE;
                // Notify clock is zero
                mClkZeroEvent[emMSPInCLKId].notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}
///*********************************
/// Function     : GetCLKFreq 
/// Description  : Get clock value
///*********************************
void MSPI000::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mClkOrgFreq[emPCLKId], mClkFreqUnit[emPCLKId].c_str());
        } else if (word_vector[0] == "MSPInCLK") {
            re_printf("info","MSPInCLK frequency is %0.0f %s\n", (double)mClkOrgFreq[emMSPInCLKId], mClkFreqUnit[emMSPInCLKId].c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}
///*********************************
/// Function     : ForceRegister 
/// Description  : Force value to register
///*********************************
void MSPI000::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            if("INT_FACTOR"==FindRegAreaFromName(reg_name)){
                mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);
            }else{
                mpMSPI000_Func->RegisterHandler(cmd);
            }
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}
///*********************************
/// Function     : ReleaseRegister 
/// Description  : Release forced value after forcing registers
///*********************************
void MSPI000::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            if("INT_FACTOR"==FindRegAreaFromName(reg_name)){
                mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);
            }else{
                mpMSPI000_Func->RegisterHandler(cmd);
            }
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}
///*********************************
/// Function     : WriteRegister 
/// Description  : Write value to registers by software
///*********************************
void MSPI000::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            if("INT_FACTOR"==FindRegAreaFromName(reg_name)){
                mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);
            }else{
                mpMSPI000_Func->RegisterHandler(cmd);
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}
///*********************************
/// Function     : ReadRegister 
/// Description  : Read value of register by software
///*********************************
void MSPI000::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            if("INT_FACTOR"==FindRegAreaFromName(reg_name)){
                mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);
            }else{
                mpMSPI000_Func->RegisterHandler(cmd);
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}
///*********************************
/// Function     : ListRegister 
/// Description  : List all registers name
///*********************************
void MSPI000::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpMSPI000_Func->RegisterHandler(cmd);
    mpMSPI000_INT_FACTOR_Func->RegisterHandler(cmd);
}//}}}
///*********************************
/// Function     : Help 
/// Description  : Dump help message of all parameters or commands
///*********************************
void MSPI000::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"MSPI000_MessageLevel (\"MSPI000 instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"MSPI000_DumpRegisterRW (\"MSPI000 instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"MSPI000_DumpInterrupt (\"MSPI000 instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"MSPI000_EnableRegisterMessage (\"MSPI000 instance\", \"true/false\")                               Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"MSPI000_AssertReset (\"MSPI000 instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the MSPI000 model");
            SC_REPORT_INFO(this->basename(),"MSPI000_SetCLKFreq (\"MSPI000 instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to MSPI000 model. Valid clock_name is PCLK or MSPInCLK");
            SC_REPORT_INFO(this->basename(),"MSPI000_GetCLKFreq (\"MSPI000 instance\", \"clock_name\")                                          Get clock frequency value of MSPI000 model. Valid clock_name is PCLK or MSPInCLK");
            SC_REPORT_INFO(this->basename(),"MSPI000_ForceRegister (\"MSPI000 instance\",\"reg_name\", reg_value)                               Force register with setting value");
            SC_REPORT_INFO(this->basename(),"MSPI000_ReleaseRegister (\"MSPI000 instance\",\"reg_name\")                                        Release register from force value");
            SC_REPORT_INFO(this->basename(),"MSPI000_WriteRegister (\"MSPI000 instance\",\"reg_name\", reg_value)                               Write a value to a register");
            SC_REPORT_INFO(this->basename(),"MSPI000_ReadRegister (\"MSPI000 instance\",\"reg_name\")                                           Read a value from a register");
            SC_REPORT_INFO(this->basename(),"MSPI000_ListRegister (\"MSPI000 instance\")                                                        Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"MSPI000_SetMemSize (\"MSPI000 instance\")                                                          Set MSPI Memory size");
            SC_REPORT_INFO(this->basename(),"MSPI000_WriteDataMem (\"MSPI000 instance\")                                                        Set data memory");
            SC_REPORT_INFO(this->basename(),"MSPI000_ReadDataMem (\"MSPI000 instance\")                                                         Read data memory");
            SC_REPORT_INFO(this->basename(),"MSPI000_SetError (\"MSPI000 instance\")                                                            Set Error to model");
            SC_REPORT_INFO(this->basename(),"MSPI000_SetComparisonTiming (\"MSPI000 instance\")                                                 Set Comparison Timing to model");
        } else {
            re_printf("warning","The name (%s) of MSPI000_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of MSPI000_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}
///*********************************
/// Function     : WriteDataMem 
/// Description  : Write value to DataMem memory by software
///*********************************
void MSPI000::WriteDataMem (const unsigned int offset, const unsigned int value, const unsigned int size)
{//{{{
    if (!(mpMSPI000_Func->SetDataMem(offset, value, size))){
        re_printf("warning", "Should not write to the reservation area at Data memory offset: 0x%X.\n", offset);
    }
}//}}}
///*********************************
/// Function     : ReadDataMem 
/// Description  : Read value of DataMem memory by software
///*********************************
void MSPI000::ReadDataMem (const unsigned int offset, const unsigned int size)
{//{{{
    unsigned int value = 0;
    if (mpMSPI000_Func->GetDataMem(offset, value, size)){
        re_printf("info", "%d bytes Data Memory at offset 0x%X is 0x%08X.\n", size, offset, value);
    }
}//}}}
///*********************************
/// Function     : SetMemSize 
/// Description  : Set MSPI_MEM_SIZE
///*********************************
void MSPI000::SetMemSize (const unsigned int id)
{//{{{
    mpMSPI000_Func->SetMemSize(id);
}//}}}
///*********************************
/// Function     : SetError 
/// Description  : Set error to model
///*********************************
void MSPI000::SetError (const std::string name, const unsigned int ch_num, const std::string is_err)
{//{{{
    if (ch_num > mChannelNum) {
        re_printf("warning","Channel number (%d) is invalid\n", ch_num);
    } else {
        re_printf("info","Set (%s) error for channel (%d) to (%s) value\n", name.c_str(), ch_num, is_err.c_str());
        if (name == "Parity") {
            if (is_err == "true") {
                if (mCMode[ch_num] != emTX && mpMSPI000_Func->GetMSPInDECHKm(mCom_chid) == 1) {
                    mParityError[ch_num] = true;
                    if (mMMode[ch_num] == emDirect) {
                        mpMSPI000_Func->SetMSPInPEFSTm(ch_num, 1);
                    }
                } else {
                    re_printf("warning","Cannot set parity error. Check setting. mParityError[%d] = %d", ch_num, mParityError[ch_num]) ;
                }
            } else if (is_err == "false") {
                mParityError[ch_num] = false;
            } else {
                re_printf("warning","Invalid argument: Please change last argument to true or false %s %d\n", name.c_str(), ch_num);
            }
        } else if (name == "DataConsistency") {
            if (is_err == "true") {
                if (mCMode[ch_num] != emRX && mIsDataConsistencyCheck) {
                    mDataConsistencyError[ch_num] = true;
                    if (mMMode[ch_num] == emDirect) {
                        mpMSPI000_Func->SetMSPInDCEFSTm(ch_num, 1);
                    }
                } else {
                    re_printf("warning","Cannot set Data Consistency error. Check setting. mDataConsistencyError[%d] = %d", ch_num, mDataConsistencyError[ch_num]) ;
                }
            } else if (is_err == "false") {
                mDataConsistencyError[ch_num] = false;
            }
        } else if (name == "OverWrite") {
            if (is_err == "true") {
                if (mMMode[ch_num] == emFixedFIFO && mCMode[ch_num] != emRX) {
                    mOverWriteError[ch_num] = true; 
                    mAssertINTMSPInERRmEvent[ch_num].notify();
                }
            } else if (is_err == "false") {
                mOverWriteError[ch_num] = false;
            }
        } else if (name == "OverRead") {
            if (is_err == "true") {
                if (mMMode[ch_num] == emFixedFIFO && mCMode[ch_num] != emTX) {
                    mOverReadError[ch_num] = true;
                    mAssertINTMSPInERRmEvent[ch_num].notify();
                }
            } else if (is_err == "false") {
                mOverReadError[ch_num] = false;
            }
        } else if (name == "OverRun") {
            if (is_err == "true") {
                if (mMMode[ch_num] != emFixedBuffer && mCMode[ch_num] != emTX && mMode == emSlave) {
                    mOverRunError[ch_num] = true;
                }
            } else if (is_err == "false") {
                mOverRunError[ch_num] = false;
            }
        } else if (name == "CRC") {
            if (is_err == "true") {
                if (mIsSafeSPI[ch_num]) { //Safe-SPI function
                    mCRCError[ch_num] = true;
                    if (mMMode[ch_num] == emDirect) {
                        mpMSPI000_Func->SetMSPInCEFSTm(ch_num, 1);
                    }
                }
            } else if (is_err == "false") {
                mCRCError[ch_num] = false;
            }
        } else {
            re_printf("warning","Invalid argument: Please change the first argument (Parity, DataConsistency, OverWrite, OverRead, OverRun, CRC )\n");
        }
    }
}//}}}
///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void MSPI000::Initialize(void)
{//{{{
    // Initialize variables
    for (unsigned int i = 0; i < MAX_CH_NUM; i++) {
        mINTMSPInTX[i] = false;
        mINTMSPInRX[i] = false;
        mINTMSPInFE[i] = false;
        mINTMSPInERR[i] = false;
    }

    for (unsigned int i = 0; i < MAX_CH_NUM; i++) {
        mParityError[i] = false;
        mDataConsistencyError[i] = false;
        mOverWriteError[i] = false;
        mOverReadError[i] = false;
        mOverRunError[i] = false;
        mCRCError[i] = false;
    }
    mTX_Reserve_Buffer_Status = true;
    mCurrentSTBE = 0;
    mMSPInSCKPeriod = 0;
    mMSPInSCK = 0;
    mMSPInSCKI = 0;
    mCSI = true;
    mDCSI = 0;
    mHWTRGVal = 0;
    mReserve_chid = 0;
    mCom_chid = 0;
    mRx_chid = 0;
    mMode = emMaster;
    for (unsigned int i = 0; i < MAX_CH_NUM; i++) {
        mCMode[i] = emNoneTxRx;
        mMMode[i] = emDirect;
    }
    mIPState = emIDLE;
    for (unsigned int i = 0; i < MAX_CH_NUM; i++) {
        mChState[i] = emINIT;
    }
    for (unsigned int i = 0; i < emNumDelay; i++) {
        mDelay_value[i] = 0;
    }
    for (unsigned int i = 0; i < mChipselectNum; i++) {
        mMSPInCS[i] = true;                         //Value of MSPInCS port
        mMSPInCS_prev[i] = true;
    }
    mModelState = emModelINIT;
    mMOSI = 0;                            // Value of MOSI port
    mSPICONTROL = 0;
    for (unsigned int i = 0; i < MAX_CH_NUM; i++) {
        mInteral_Frame_Limit[i] = 1;
    }
    for (unsigned int i = 0; i < MAX_CH_NUM; i++) {
        mCount_com[i] = 0;
    }
    for (unsigned int i = 0; i < MAX_CH_NUM; i++) {
        mCount_reserve[i] = 0;
    }
    for (unsigned int i = 0; i < MAX_CH_NUM; i++) {
        mHasRequest[i] = 0;
        mRequestNumber[i] = 0;
    }
    mFrameEnd = true;
    for (unsigned int i = 0; i < MAX_CH_NUM; i++) {
        mIsSafeSPI[i] = false;
        mSizes[i] = 1;
        mIsClearCHEN[i] = false;
    }
    mIsLoopBack = false;
    mIsDataConsistencyCheck = false;
    mSlave_enable = false;
    mIsLockFunction = false;
    mIsJobEnable = false;
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
///Execute reset operation
///*********************************
/// Function     : EnableReset 
/// Description  : Enable Reset
///*********************************
void MSPI000::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if(reset_name=="PRESETn") {
        mpMSPI000_Func->EnableReset (is_active);
        mpMSPI000_INT_FACTOR_Func->EnableReset(is_active);
    }
    if (is_active) {
        CancelEvents();
        Initialize();
        for (unsigned int i = 0; i < mChannelNum; i++){
            mDeAssertINTMSPInTXmEvent[i].notify(SC_ZERO_TIME);
            mDeAssertINTMSPInRXmEvent[i].notify(SC_ZERO_TIME);
            mDeAssertINTMSPInFEmEvent[i].notify(SC_ZERO_TIME);
            mDeAssertINTMSPInERRmEvent[i].notify(SC_ZERO_TIME);
        }
        // Initialize remain output ports
        mWriteMSPInSCKEvent.notify(SC_ZERO_TIME);
        mWriteMSPInCSEvent.notify(SC_ZERO_TIME);
        mWriteSPICONTROLEvent.notify(SC_ZERO_TIME);
        mWriteMOSIEvent.notify(SC_ZERO_TIME);
        mWriteINTMSPInERREvent.notify(SC_ZERO_TIME);
        mWriteINTMSPInFEEvent.notify(SC_ZERO_TIME);
        mWriteINTMSPInTXEvent.notify(SC_ZERO_TIME);
        mWriteINTMSPInRXEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/// Cancel events
///*********************************
/// Function     : CancelEvents
/// Description  : Cancel all events
///*********************************
void MSPI000::CancelEvents()
{//{{{
    mTransmitEvent.cancel();
    mReceiveEvent.cancel();
    mCommunicationEvent.cancel();
    mCommunicationSlaveEvent.cancel();
    mEndFrameEvent.cancel();
    mEndFrameSlaveEvent.cancel();
    mWriteTXReserveBufferEvent.cancel();
    mWriteTXReserveBufferEvent1.cancel();
    mWriteTXShiftRegisterEvent.cancel();
    mWriteRXReserveBufferEvent.cancel();
    mWriteRXShiftRegisterEvent.cancel();
    mWriteMSPInRXDAm0Event.cancel();
    for (unsigned int id = 0; id < MAX_CH_NUM; id++) {
        mWriteINTMSPInTXmEvent[id].cancel();
        mAssertINTMSPInTXmEvent[id].cancel();
        mDeAssertINTMSPInTXmEvent[id].cancel();
        mWriteINTMSPInRXmEvent[id].cancel();
        mAssertINTMSPInRXmEvent[id].cancel();
        mDeAssertINTMSPInRXmEvent[id].cancel();
        mWriteINTMSPInFEmEvent[id].cancel();
        mAssertINTMSPInFEmEvent[id].cancel();
        mDeAssertINTMSPInFEmEvent[id].cancel();
        mWriteINTMSPInERRmEvent[id].cancel();
        mAssertINTMSPInERRmEvent[id].cancel();
        mDeAssertINTMSPInERRmEvent[id].cancel();
    }
    mEndChannelEvent.cancel();
    mEndChannelSlaveEvent.cancel();
    mWriteMSPInSCKEvent.cancel();
    mWriteMOSIEvent.cancel();
    mWriteSPICONTROLEvent.cancel();
    mWriteMSPInCSEvent.cancel();
    mReadMSPInRXDAm0Event.cancel();
    mWriteINTMSPInERREvent.cancel();
    mWriteINTMSPInFEEvent.cancel();
    mWriteINTMSPInTXEvent.cancel();
    mWriteINTMSPInRXEvent.cancel();
}//}}}
///*********************************
/// Function     : DumpInterruptMsg
/// Description  : Dump Interrupt Message
///*********************************
void MSPI000::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mDumpInterrupt) {
        if (value) {
            re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
        } else {
            re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
        }
   }
}//}}}
//========================================================
//============ Method/Threads               ===================
//========================================================
///*********************************
/// Function     : HandlePCLKSignalMethod 
/// Description  : Handle PCLK signal
///*********************************
void MSPI000::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleMSPInCLKSignalMethod 
/// Description  : Handle MSPInCLK signal
///*********************************
void MSPI000::HandleMSPInCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = MSPInCLK.read();
    SetCLKFreq("MSPInCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleMSPInSCKISignalMethod       
/// Description  : Handle SCKI Signal
///*********************************
void MSPI000::HandleMSPInSCKISignalMethod       (void)
{//{{{
    //In the slave mode, only channel 0 can be used.
    if (mMode == emSlave && mChState[0] == emREADY) {
        for ( int i =0; i<4; i++){
            if(MSPInSCKI[i].read()>0){
                mMSPInSCKI = (double) MSPInSCKI[i].read();
            }
        }
        if ((mpMSPI000_Func->GetMSPInCSIE() == 0) || (mpMSPI000_Func->GetMSPInCSIE() == 1 && !mCSI)) {
            mSlave_enable = true;
        }
        if (mMSPInSCKI == 0) {
            mSlave_enable = false;
        }
    }
}//}}}

///*********************************
/// Function     : HandleMSPInCSISignalMethod   
/// Description  : Handle MSPInCSI Signal
///*********************************
void MSPI000::HandleMSPInCSISignalMethod   (void)
{//{{{
   mCSI = MSPInCSI->read();
}//}}}

///*********************************
/// Function     : HandleMSPInDCSISignalMethod  
/// Description  : Handle MSPInDCSI Signal Method
///*********************************
void MSPI000::HandleMSPInDCSISignalMethod  (void)
{//{{{
    mDCSI = (unsigned int)MSPInDCSI.read();
}//}}}

///*********************************
/// Function     : HandleSPI_CONTROLSignalMethod
/// Description  : Handle SPI_CONTROL Signal Method
///*********************************
void MSPI000::HandleSPI_CONTROLSignalMethod(void)
{//{{{
    unsigned int spi_control_val = (unsigned int)SPI_CONTROL.read();
    unsigned int new_stbe = (spi_control_val >> 8)&0x3;
    if (new_stbe == 1 && mCurrentSTBE != 1 && mSlave_enable) {
        CommuniationSlave();
    }
    mCurrentSTBE = new_stbe;
}//}}}

///*********************************
/// Function     : HandleMSPInHWTRGSignalMethod 
/// Description  : Handle MSPInHWTRG Signal Method
///*********************************
void MSPI000::HandleMSPInHWTRGSignalMethod (void)
{//{{{
    //Hardware trigger
    mHWTRGVal = 0;
    for (unsigned int i = 0; i < mHWTriggerNum; i++) {
        mHWTRGVal = mHWTRGVal|(((sc_dt::uint64)(MSPInHWTRG[i]->read())) << i);
    }
}//}}}

///*********************************
/// Function     : HandlePRESETnSignalMethod 
/// Description  : Handle PRESETn signal
///*********************************
void MSPI000::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[0] = PRESETn.read();
    if (mResetCurVal[0] != mResetPreVal[0]) {       //mResetPreVal is "true" at initial state
        mResetPreVal[0] = mResetCurVal[0];
        if (mClkPeriod[emPCLKId] > 0) {
            mResetHardEvent[0].notify(CalculateCLK(emPCLKId), mTimeResolutionUnit);
        } else {
            mResetHardEvent[0].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///*********************************
/// Function     : HandleMSPInRESBSignalMethod 
/// Description  : Handle MSPInRESB signal
///*********************************
void MSPI000::HandleMSPInRESBSignalMethod (void)
{//{{{
    mResetCurVal[1] = MSPInRESB.read();
    if (mResetCurVal[1] != mResetPreVal[1]) {   //mResetPreVal is "true" at initial state
        mResetPreVal[1] = mResetCurVal[1];
        if (mClkPeriod[emMSPInCLKId] > 0) {
            mResetHardEvent[1].notify(CalculateCLK(emMSPInCLKId), mTimeResolutionUnit);
        } else {
            mResetHardEvent[1].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///*********************************
/// Function     : HandleResetHardMethod 
/// Description  : Process reset function when reset port is active
///*********************************
void MSPI000::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == 0) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port MSPInRESB is asserted.\n");
            this->EnableReset("MSPInRESB", mIsResetHardActive[reset_id]);
        }
    } else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == 0) {
            re_printf("info","The reset port PRESETn is de-asserted.\n");
            this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port MSPInRESB is de-asserted.\n");
            this->EnableReset("MSPInRESB", mIsResetHardActive[reset_id]);
        }    
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

///*********************************
/// Function     : HandleResetCmdMethod 
/// Description  : Process reset function when reset command is active
///*********************************
void MSPI000::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else { //reset_id == 1
        reset_name = "MSPInRESB";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when reset command is active
///*********************************
/// Function     : CancelResetCmdMethod 
/// Description  : Cancel ResetCmd Method
///*********************************
void MSPI000::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else { //reset_id == 1
        reset_name = "MSPInRESB";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

///*********************************
/// Function     : WriteINTMSPInTXmMethod
/// Description  : Write INTMSPInTX Method
///*********************************
void MSPI000::WriteINTMSPInTXmMethod(unsigned int ch_id)
{//{{{
    INTMSPInTXm[ch_id]->write(mINTMSPInTX[ch_id]);
    DumpInterruptMsg(INTMSPInTXm[ch_id]->basename(), mINTMSPInTX[ch_id]);
}//}}}

///*********************************
/// Function     : AssertINTMSPInTXmMethod
/// Description  : Assert INTMSPInTX Method
///*********************************
void MSPI000::AssertINTMSPInTXmMethod(unsigned int ch_id)
{//{{{
    mpMSPI000_Func->SetMSPInTXRQFm(ch_id, 1);
    if (mpMSPI000_Func->GetMSPInITXEm(ch_id) == 1) {
        mINTMSPInTX[ch_id] = true;
        mWriteINTMSPInTXmEvent[ch_id].notify(SC_ZERO_TIME);
        mpMSPI000_INT_FACTOR_Func->SetMSPInINTFk(emNumTX,ch_id);
        mWriteINTMSPInTXEvent.notify(SC_ZERO_TIME);
        mDeAssertINTMSPInTXmEvent[ch_id].notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
    } else {
        re_printf("warning","The interrupt INTMSPInTX[%d] is disabled by MSPInCFGm0.\n", ch_id);
    }
}//}}}

///*********************************
/// Function     : DeAssertINTMSPInTXmMethod
/// Description  : DeAssert INTMSPInTX Method
///*********************************
void MSPI000::DeAssertINTMSPInTXmMethod(unsigned int ch_id)
{//{{{
    mINTMSPInTX[ch_id] = false;
    mWriteINTMSPInTXmEvent[ch_id].notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : WriteINTMSPInRXmMethod
/// Description  : Write INTMSPInRX Method
///*********************************
void MSPI000::WriteINTMSPInRXmMethod(unsigned int ch_id)
{//{{{
    INTMSPInRXm[ch_id]->write(mINTMSPInRX[ch_id]);
    DumpInterruptMsg(INTMSPInRXm[ch_id]->basename(), mINTMSPInRX[ch_id]);
}//}}}

///*********************************
/// Function     : AssertINTMSPInRXmMethod
/// Description  : Assert INTMSPInRX Method
///*********************************
void MSPI000::AssertINTMSPInRXmMethod(unsigned int ch_id)
{//{{{
    mpMSPI000_Func->SetMSPInRXRQFm(ch_id, 1);
    if (mpMSPI000_Func->GetMSPInIRXEm(ch_id) == 1) {
        mINTMSPInRX[ch_id] = true;
        mWriteINTMSPInRXmEvent[ch_id].notify(SC_ZERO_TIME);
        mpMSPI000_INT_FACTOR_Func->SetMSPInINTFk(emNumRX,ch_id);
        mWriteINTMSPInRXEvent.notify(SC_ZERO_TIME);
        mDeAssertINTMSPInRXmEvent[ch_id].notify((double)mClkPeriod[emPCLKId] ,mTimeResolutionUnit);
    } else {
        re_printf("warning","The interrupt INTMSPInRX[%d] is disabled by MSPInCFGm0.\n", ch_id);
    }
}//}}}

///*********************************
/// Function     : DeAssertINTMSPInRXmMethod
/// Description  : DeAssert INTMSPInRX Method
///*********************************
void MSPI000::DeAssertINTMSPInRXmMethod(unsigned int ch_id)
{//{{{
    mINTMSPInRX[ch_id] = false;
    mWriteINTMSPInRXmEvent[ch_id].notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : WriteINTMSPInFEmMethod
/// Description  : Write INTMSPInFE Method
///*********************************
void MSPI000::WriteINTMSPInFEmMethod(unsigned int ch_id)
{//{{{
    INTMSPInFEm[ch_id]->write(mINTMSPInFE[ch_id]);
    DumpInterruptMsg(INTMSPInFEm[ch_id]->basename(), mINTMSPInFE[ch_id]);
}//}}}

///*********************************
/// Function     : AssertINTMSPInFEmMethod
/// Description  : Assert INTMSPInFE Method
///*********************************
void MSPI000::AssertINTMSPInFEmMethod(unsigned int ch_id)
{//{{{
    if (mpMSPI000_Func->GetMSPInIFEEm(ch_id) == 1) {
        mINTMSPInFE[ch_id] = true;
        mWriteINTMSPInFEmEvent[ch_id].notify(SC_ZERO_TIME);
        mpMSPI000_INT_FACTOR_Func->SetMSPInINTFk(emNumFE,ch_id);
        mWriteINTMSPInFEEvent.notify(SC_ZERO_TIME);
        mDeAssertINTMSPInFEmEvent[ch_id].notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
    } else {
        re_printf("warning","The interrupt INTMSPInFE[%d] is disabled by MSPInCFGm0.\n", ch_id);
    }
}//}}}

///*********************************
/// Function     : DeAssertINTMSPInFEmMethod
/// Description  : DeAssert INTMSPInFE Method
///*********************************
void MSPI000::DeAssertINTMSPInFEmMethod(unsigned int ch_id)
{//{{{
    mINTMSPInFE[ch_id] = false;
    mWriteINTMSPInFEmEvent[ch_id].notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : WriteINTMSPInERRmMethod
/// Description  : Write INTMSPInERR Method
///*********************************
void MSPI000::WriteINTMSPInERRmMethod(unsigned int ch_id)
{//{{{
    INTMSPInERRm[ch_id]->write(mINTMSPInERR[ch_id]);
    DumpInterruptMsg(INTMSPInERRm[ch_id]->basename(), mINTMSPInERR[ch_id]);
}//}}}

///*********************************
/// Function     : AssertINTMSPInERRmMethod
/// Description  : Assert INTMSPInERR Method
///*********************************
void MSPI000::AssertINTMSPInERRmMethod(unsigned int ch_id)
{//{{{
    mpMSPI000_Func->SetMSPInCESTm(ch_id, mOverReadError[ch_id], mOverWriteError[ch_id], mOverRunError[ch_id], mDataConsistencyError[ch_id], mCRCError[ch_id], mParityError[ch_id]);
    if (mpMSPI000_Func->GetMSPInIEREm(ch_id) == 1) {
        mINTMSPInERR[ch_id] = true;
        mWriteINTMSPInERRmEvent[ch_id].notify(SC_ZERO_TIME);
        mpMSPI000_INT_FACTOR_Func->SetMSPInINTFk(emNumERR,ch_id);
        mWriteINTMSPInERREvent.notify(SC_ZERO_TIME);
        mDeAssertINTMSPInERRmEvent[ch_id].notify((double)mClkPeriod[emPCLKId],mTimeResolutionUnit);
    } else {
        re_printf("warning","The interrupt INTMSPInERR[%d] is disabled by MSPInCFGm0.\n", ch_id);
    }
}//}}}

///*********************************
/// Function     : DeAssertINTMSPInERRmMethod
/// Description  : DeAssert INTMSPInERR Method
///*********************************
void MSPI000::DeAssertINTMSPInERRmMethod(unsigned int ch_id)
{//{{{
    mINTMSPInERR[ch_id] = false;
    mWriteINTMSPInERRmEvent[ch_id].notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : WriteTXReserveBufferMethod
/// Description  : Write TX_Reserve_Buffer Method
///*********************************
void MSPI000::WriteTXReserveBufferMethod()
{//{{{
    mCount_reserve[mReserve_chid]++;

    //TX Reserve Buffer is not ready
    mTX_Reserve_Buffer_Status = false;
    //write to buffer is clear this request
    mHasRequest[mReserve_chid]--;

    if (mMMode[mReserve_chid] == emFixedBuffer || mMMode[mReserve_chid] == emFixedFIFO) {
        mTX_Reserve_Buffer.data = mpMSPI000_Func->GetRAMValue(mReserve_chid, mSizes[mReserve_chid]);
    } else {
        mTX_Reserve_Buffer.data = mpMSPI000_Func->GetMSPInTXDAm0(mReserve_chid);
    }
    mTX_Reserve_Buffer.chid = mReserve_chid;
    mTX_Reserve_Buffer.request = mCount_reserve[mReserve_chid];
    mTX_Reserve_Buffer.csr = mpMSPI000_Func->GetMSPInCSRm(mReserve_chid);
    mTX_Reserve_Buffer.joben = mpMSPI000_Func->GetMSPInJOBENm(mReserve_chid);
    if (mMMode[mReserve_chid] == emDirect) {
        if ((mCount_reserve[mReserve_chid] / mInteral_Frame_Limit[mReserve_chid]) < mpMSPI000_Func->GetMSPInCFSETm(mReserve_chid)) {
            mAssertINTMSPInTXmEvent[mReserve_chid].notify();
        }
    }
    if (mpMSPI000_Func->GetMSPInLOCKm(mReserve_chid) == 1) {
        mIsLockFunction = true;
    }
    if ((mCount_reserve[mReserve_chid] / mInteral_Frame_Limit[mReserve_chid]) == mpMSPI000_Func->GetMSPInCFSETm(mReserve_chid)) {
        mIsLockFunction = false;
    }
    if ((mMMode[mReserve_chid] == emDirect) && (mTX_Reserve_Buffer.joben == 1) && (mCount_reserve[mReserve_chid] / mInteral_Frame_Limit[mReserve_chid]) < mpMSPI000_Func->GetMSPInCFSETm(mReserve_chid)) {
        mIsJobEnable = true;
    } else {
        mIsJobEnable = false;
    }if (mpMSPI000_Func->GetMSPInFLENm(mReserve_chid) < 32) {
        mTX_Reserve_Buffer.data &= (0xFFFFFFFF >> (32 - mpMSPI000_Func->GetMSPInFLENm(mReserve_chid)));
    }
}//}}}

///*********************************
/// Function     : WriteTXShiftRegisterMethod
/// Description  : Write TX_Shift_Register Method
///*********************************
void MSPI000::WriteTXShiftRegisterMethod()
{//{{{
    //Get priority
    //the channel priority is judged at the timing when the reservation buffer  is empty.
    if (!mIsLockFunction && !mIsJobEnable) {
        mReserve_chid = GetChannelPriority(mReserve_chid);
    }

    mCom_chid = mTX_Reserve_Buffer.chid;
    mTX_Shift_Register.data = mTX_Reserve_Buffer.data;
    mTX_Shift_Register.chid = mTX_Reserve_Buffer.chid;
    mTX_Shift_Register.request = mTX_Reserve_Buffer.request;
    mTX_Shift_Register.csr = mTX_Reserve_Buffer.csr;
    mTX_Shift_Register.joben = mTX_Reserve_Buffer.joben;
    //TX Reserve buffer is ready
    mTX_Reserve_Buffer_Status = true;
    if (mMode == emMaster) {
        mCommunicationEvent.notify();
    }
}//}}}

///*********************************
/// Function     : WriteRXReserveBufferMethod
/// Description  : Write RX_Reserve_Buffer Method
///*********************************
void MSPI000::WriteRXReserveBufferMethod()
{//{{{
    mRX_Reserve_Buffer.data = mRX_Shift_Register.data;
    mRX_Reserve_Buffer.chid = mRX_Shift_Register.chid;
    mRX_Reserve_Buffer.request = mRX_Shift_Register.request;
    mRX_Reserve_Buffer.csr = mRX_Shift_Register.csr;
    mRX_Reserve_Buffer.joben = mRX_Shift_Register.joben;
    mWriteMSPInRXDAm0Event.notify((double) mClkPeriod[emPCLKId], mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : WriteRXShiftRegisterMethod
/// Description  : Write RX_Shift_Register Method
///*********************************
void MSPI000::WriteRXShiftRegisterMethod()
{//{{{
    if (mIsLoopBack) {
        mRX_Shift_Register.data = mTX_Shift_Register.data;
        //The CRC error always occurs when MSPI operates Loop-Back mode in Safe-SPI protocol function.
        if (mIsSafeSPI[mRx_chid]) {
            mCRCError[mRx_chid] = true;
        }
    } else {
        mRX_Shift_Register.data = (unsigned int) MISO[0].read()|(unsigned int) MISO[1].read()|(unsigned int) MISO[2].read()|(unsigned int) MISO[3].read(); 
        if (mIsSafeSPI[mRx_chid] && mpMSPI000_Func->GetMSPInSAFSm(mRx_chid) == 0) { //in-frame format
            if (CalculateCRC((mRX_Shift_Register.data >> 3) & 0x00FFFFFF, 24, 3, 7, 0xB) != (mRX_Shift_Register.data & 0x7)) {
                mCRCError[mRx_chid] = true;
            }
        } else if (mIsSafeSPI[mRx_chid] && mpMSPI000_Func->GetMSPInSAFSm(mRx_chid) == 1) { //out-frame format
            if (CalculateCRC((mRX_Shift_Register.data >> 3), 29, 3, 5, 0xB) !=  (mRX_Shift_Register.data & 0x7)) {
                mCRCError[mRx_chid] = true;
            }
        }
        //The CRC error mask function in Safe-SPI protocol function first frame
        if (mpMSPI000_Func->GetMSPInSAFCMm(mRx_chid) == 1 && mpMSPI000_Func->GetMSPInCFSETm(mRx_chid) == mpMSPI000_Func->GetMSPInCFCNTm(mRx_chid)){
            mCRCError[mRx_chid] = false;
        }
    }
    if (mCRCError[mRx_chid] && mMMode[mRx_chid] == emDirect) {
        mpMSPI000_Func->SetMSPInCEFSTm(mRx_chid, 1);
    }
    mRX_Shift_Register.chid = mRx_chid;
    mRX_Shift_Register.request = mCount_com[mRx_chid];
    mRX_Shift_Register.csr = mpMSPI000_Func->GetMSPInCSRm(mRx_chid);
    mRX_Shift_Register.joben = mpMSPI000_Func->GetMSPInJOBENm(mRx_chid);
    if (mpMSPI000_Func->GetMSPInFLENm(mRx_chid) < 32) {
        mRX_Shift_Register.data &= (0xFFFFFFFF >> (32 - mpMSPI000_Func->GetMSPInFLENm(mRx_chid)));
    }
}//}}}

///*********************************
/// Function     : WriteMSPInRXDAm0Method
/// Description  : Write value frame reserve buffer to ram or RX register
///*********************************
void MSPI000::WriteMSPInRXDAm0Method()
{//{{{
    if (mMMode[mRx_chid] == emDirect) {
        mAssertINTMSPInRXmEvent[mRx_chid].notify();
    }
    if (mMMode[mRx_chid] == emFixedBuffer) {
        mpMSPI000_Func->SetRAMValue(mRx_chid, mRX_Reserve_Buffer.data, mSizes[mRx_chid]);
    } else if (mMMode[mRx_chid] == emFixedFIFO){
        mpMSPI000_Func->SetRAMValue(mRx_chid, mRX_Reserve_Buffer.data, mSizes[mRx_chid]);
    } else {
        mpMSPI000_Func->SetMSPInRXDAm0(mRx_chid, mRX_Reserve_Buffer.data);
    }
}//}}}

///*********************************
/// Function     : EndChannelMethod
/// Description  : End Channel Method
///*********************************
void MSPI000::EndChannelMethod(void)
{//{{{
    re_printf("info","EndChannelCommunicationMethod Channel %d.\n", mCom_chid);
    if (mpMSPI000_Func->GetMSPInCFCNTm(mCom_chid) == 0) {
        mAssertINTMSPInFEmEvent[mCom_chid].notify();
    }
    bool status_allchannel = false;
    //MSPInCHENm of All channels are cleared
    for (unsigned int i = 0; i < mChannelNum; i++) {
        if (mChState[i] == emREADY) {
            status_allchannel = true;
        }
    }

    if (!status_allchannel) {
        mpMSPI000_Func->SetMSPInCSF(0);

        mpMSPI000_Func->GetDelayValue(mCom_chid, mDelay_value);
        double hold_time = mDelay_value[emDHOLD] * (double) mClkPeriod[emMSPInCLKId];
        re_printf("info","hold time called. %d\n", mDelay_value[emDHOLD]);

        if (mpMSPI000_Func->GetMSPInCSRIm(mCom_chid) == 1 && (mpMSPI000_Func->GetMSPInFCCEm(mCom_chid) != 1 || mMMode[mCom_chid] != emDirect)) {
            //inactive cs
            for (unsigned int i = 0; i < mChipselectNum; i++) {
                mMSPInCS[i] = ((mpMSPI000_Func->GetMSPInCSP() & (1 << i)) == 0);
            }
            mWriteMSPInCSEvent.notify(hold_time, mTimeResolutionUnit);
        }
        mSPICONTROL = (0x0 << 8) | Create_SPI_Control(mCom_chid); //stbe = 0
        mWriteSPICONTROLEvent.notify(hold_time, mTimeResolutionUnit);

        //When all channels communication ends and MSPI communication stops, MSPInCLK returns the default level set by MSPInCTL1.MSPInCKR.
        mMSPInSCK= (double)mpMSPI000_Func->GetMSPInCKR();
        mWriteMSPInSCKEvent.notify(hold_time + 3 * mClkPeriod[emMSPInCLKId], mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : EndChannelSlaveMethod
/// Description  : End Channel Slave Method
///*********************************
void MSPI000::EndChannelSlaveMethod(void)
{//{{{
    re_printf("info","EndChannelSlaveMethod Channel %d.\n", mCom_chid);
    if (mOverRunError[0] == true) {
        mAssertINTMSPInERRmEvent[mCom_chid].notify((double) mClkPeriod[emPCLKId], mTimeResolutionUnit);
    }
    mAssertINTMSPInFEmEvent[mCom_chid].notify();
    mpMSPI000_Func->SetMSPInCSF(0);
}//}}}

///*********************************
/// Function     : TransmitMethod
/// Description  : Transmit Method
///*********************************
void MSPI000::TransmitMethod(void)
{//{{{
    //out SCK and MOSI
    mMOSI = mTX_Shift_Register.data;
    if (mMode == emMaster) {
        if (mIsSafeSPI[mCom_chid] && mpMSPI000_Func->GetMSPInSAFSm(mCom_chid) == 0) { //in-frame format
            mMOSI = (mMOSI & 0xFFFFFFE3) + ((CalculateCRC(mMOSI >> 5, 27, 3, 7, 0xB) & 7) << 2);
        } else if (mIsSafeSPI[mCom_chid] && mpMSPI000_Func->GetMSPInSAFSm(mCom_chid) == 1) {//out-of-frame format
            mMOSI = (mMOSI & 0xFFFFFFF8) + (CalculateCRC(mMOSI >> 3, 29, 3, 5, 0xB) & 7);
        }
    }
    mWriteMOSIEvent.notify();
}//}}}

///*********************************
/// Function     : ReceiveMethod
/// Description  : Receive Method
///*********************************
void MSPI000::ReceiveMethod(void)
{//{{{
    mRx_chid = mCom_chid;
    mWriteRXShiftRegisterEvent.notify((double) mClkPeriod[emPCLKId], mTimeResolutionUnit);
}//}}}
///*********************************
/// Function     : CheckDataconsistencyMethod
/// Description  : CheckDataconsistencyMethod
///*********************************
void MSPI000::CheckDataconsistencyMethod(void)
{//{{{
    if (mDCSI != mMOSI) {
        mDataConsistencyError[mCom_chid] = true;
        if (mMMode[mCom_chid] == emDirect) {
            mpMSPI000_Func->SetMSPInDCEFSTm(mCom_chid, 1);
        }
    } 
}//}}}


///*********************************
/// Function     : CommunicationThread
/// Description  : Start frame communication thread in master mode
///*********************************
void MSPI000::CommunicationThread(void)
{//{{{
    for(;;) {
        if (mIsResetHardActive[emMSPInRESBId]||mIsResetCmdActive[emMSPInRESBId]||((!CheckClockPeriod("MSPInCLK")) && mMode == emMaster)||(!CheckClockPeriod("PCLK"))||mIPState == emIDLE) {
            wait();
            continue;
        }
        try {
            while (1) {
                // indicate frame is communicating
                mFrameEnd = false;
                mCount_com[mCom_chid]++;
                re_printf("info","CommunicationThread called.\n");
                if (mpMSPI000_Func->GetMSPInCPOLm(mCom_chid) == 1 && mpMSPI000_Func->GetMSPInCKR() == 0 && mMSPInSCK == 0) {
                    mMSPInSCK = 1;
                    mWriteMSPInSCKEvent.notify();
                    wait(3 * (double) mClkPeriod[emMSPInCLKId], mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                    CheckWaitInThread();
                }
                if (mpMSPI000_Func->GetMSPInCPOLm(mCom_chid) == 0 && mpMSPI000_Func->GetMSPInCKR() == 1 && mMSPInSCK == 1) {
                    mMSPInSCK = 0;
                    mWriteMSPInSCKEvent.notify();
                    wait(3 * (double) mClkPeriod[emMSPInCLKId], mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                    CheckWaitInThread();
                }
                mpMSPI000_Func->GetDelayValue(mCom_chid, mDelay_value);
                mMSPInSCKPeriod = mClkPeriod[emMSPInCLKId] / mpMSPI000_Func->GetmMSPInSCKbaudarate(mCom_chid);
                mMSPInSCK = mClkFreq[emMSPInCLKId] * mpMSPI000_Func->GetmMSPInSCKbaudarate(mCom_chid);

                double setup_time = mDelay_value[emDSETUP] * (double) mClkPeriod[emMSPInCLKId];
                //active cs
                bool actived = Calculate_mMSPInCS();
                mWriteMSPInCSEvent.notify();
                if (actived) {
                    if (mCMode[mCom_chid] != emRX && mpMSPI000_Func->GetMSPInCPHAm(mCom_chid) == 0) {
                        mTransmitEvent.notify();
                    }
                }
                mSPICONTROL = (0x2 << 8) | Create_SPI_Control(mCom_chid); //stbe = 10
                mWriteSPICONTROLEvent.notify();
                if (actived && Compare_mMSPInCS()) { //active and cs different
                    re_printf("info","setup time called. %d\n", mDelay_value[emDSETUP]);
                    wait(setup_time, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                    CheckWaitInThread();
                } else if (!actived) {
                    re_printf("info","CS not setting active.\n");
                }
                if (actived) {
                    mWriteMSPInSCKEvent.notify();
                    mSPICONTROL = (0x1 << 8) | Create_SPI_Control(mCom_chid); //stbe = 01
                    mWriteSPICONTROLEvent.notify();
                    if (mCMode[mCom_chid] != emRX && mpMSPI000_Func->GetMSPInCPHAm(mCom_chid) == 1) {
                        mTransmitEvent.notify();
                    }
                    if (mCMode[mCom_chid] != emTX) {
                        mReceiveEvent.notify();
                    }
                    //wait end frame
                    if (mInteral_Frame_Limit[mCom_chid] > 1) {
                        wait(mMSPInSCKPeriod * 32, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                    } else {
                        wait(mMSPInSCKPeriod * mpMSPI000_Func->GetMSPInFLENm(mCom_chid), mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                    }
                    CheckWaitInThread();
                    if (mCMode[mCom_chid] != emTX) {
                        mWriteRXReserveBufferEvent.notify(CalculateCLK(emPCLKId), mTimeResolutionUnit);
                    }
                    if (mpMSPI000_Func->GetMSPInDECHKm(mCom_chid) == 1) {  //if parity is enable
                        if (mCMode[mCom_chid] != emRX) {
                            if(GetParity(mMOSI, mCom_chid)){
                                mMOSI = 1;
                            }else{
                                mMOSI = 0;
                            }
                            mWriteMOSIEvent.notify();
                        }
                        wait(mMSPInSCKPeriod, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);  //wait more a SCK period
                        CheckWaitInThread();
                    }
                    mEndFrameEvent.notify();
                }
                wait(mCommunicationEvent);
            }
        }
        catch (vpcl::eResetException){
        }
    }
}//}}}

///*********************************
/// Function     : EndFrameThread
/// Description  : End frame thread in master mode
///*********************************
void MSPI000::EndFrameThread(void)
{//{{{
    for(;;) {
        if (mIsResetHardActive[emMSPInRESBId]||mIsResetCmdActive[emMSPInRESBId]||((!CheckClockPeriod("MSPInCLK")) && mMode == emMaster)||(!CheckClockPeriod("PCLK"))||mIPState == emIDLE) {
            wait();
            continue;            
        }
        try {
            while (1) {
                re_printf("info","EndFrameThread called.\n");
                if (mDataConsistencyError[mCom_chid]) {
                    mpMSPI000_Func->SetMSPInDCEFSTm(mCom_chid, 0);
                }
                if (mCRCError[mCom_chid]) {
                    mpMSPI000_Func->SetMSPInCEFSTm(mCom_chid, 0);
                }
                if (mParityError[mCom_chid]) {
                    mpMSPI000_Func->SetMSPInPEFSTm(mCom_chid, 0);
                }
                mMSPInSCK = mpMSPI000_Func->GetMSPInCPOLm(mCom_chid);
                mWriteMSPInSCKEvent.notify();

                if ((mpMSPI000_Func->GetMSPInSOLS() & 1) == 1) { //inactive MOSI
                    if((mpMSPI000_Func->GetMSPInSOLS() & 2) == 0){
                        mMOSI = 0;
                    }else{
                        mMOSI = 0xFFFFFFFF;
                    }
                    mWriteMOSIEvent.notify();
                }
                if (HasError(mCom_chid)) {
                    mSPICONTROL = (0x3 << 8) | Create_SPI_Control(mCom_chid); //stbe = 11
                    mWriteSPICONTROLEvent.notify();
                    mAssertINTMSPInERRmEvent[mCom_chid].notify((double) mClkPeriod[emPCLKId], mTimeResolutionUnit);
                } else {
                    mSPICONTROL = (0x2 << 8) | Create_SPI_Control(mCom_chid); //stbe = 10
                    mWriteSPICONTROLEvent.notify();
                }
                // indicate is frame transmit done
                mFrameEnd = true;
                unsigned int mNext_chid;

                if (mpMSPI000_Func->GetMSPInRXRQFm(mCom_chid) == 1 && mMode == emMaster && mCMode[mCom_chid] != emTX) {
                    //Communication stop
                    re_printf("warning","The communication is stop because MSPInRXDAm0 not read");
                    wait(mReadMSPInRXDAm0Event|mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]);
                    CheckWaitInThread();
                    re_printf("info","The communication is continue after read MSPInRXDAm0");
                }

                mpMSPI000_Func->SetMSPInCFCNTm(mCom_chid, mpMSPI000_Func->GetMSPInCFSETm(mCom_chid) - mCount_com[mCom_chid] / mInteral_Frame_Limit[mCom_chid]);
                mNext_chid = mTX_Reserve_Buffer.chid;
                if (mTX_Shift_Register.csr != mTX_Reserve_Buffer.csr) { //current frame and next frame same csr
                    //delay hold time
                    mpMSPI000_Func->GetDelayValue(mCom_chid, mDelay_value);
                    double hold_time = mDelay_value[emDHOLD] * (double) mClkPeriod[emMSPInCLKId];
                    re_printf("info","hold time called. %d\n", mDelay_value[emDHOLD]);
                    wait(hold_time, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                    CheckWaitInThread();
                    //inactive cs
                    if (mpMSPI000_Func->GetMSPInICLSm(mCom_chid) == 1 && mCMode[mCom_chid] != emRX && mMMode[mCom_chid] == emDirect) {
                        for (unsigned int i = 0; i < mChipselectNum; i++) {
                            mMSPInCS[i] = ((mpMSPI000_Func->GetMSPInCSP() & (1 << i)) == 0);
                            if ((mTX_Reserve_Buffer.csr & (1 << i)) != 0) {
                                mMSPInCS[i] = !mMSPInCS[i];
                            }
                        }
                        mWriteMSPInCSEvent.notify();
                    } else {
                        for (unsigned int i = 0; i < mChipselectNum; i++) {
                            mMSPInCS[i] = ((mpMSPI000_Func->GetMSPInCSP() & (1 << i)) == 0);
                        }
                        mWriteMSPInCSEvent.notify();
                    }
                    //delay idle time
                    double idle_time = mDelay_value[emDIDLE] * (double) mClkPeriod[emMSPInCLKId];
                    if (mpMSPI000_Func->GetMSPInCPOLm(mNext_chid) != mpMSPI000_Func->GetMSPInCPOLm(mCom_chid)) { //two consecutive data packets are sent with different MSPInCFGm0.MSPInCPOLm configuration
                        if (mDelay_value[emDIDLE] < 6) { //the idle time is set to 5 or less
                            re_printf("info","idle time called. %d\n", 6);
                            wait(3 * (double) mClkPeriod[emMSPInCLKId], mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                            CheckWaitInThread();
                            mMSPInSCK = mpMSPI000_Func->GetMSPInCPOLm(mNext_chid);
                            mWriteMSPInSCKEvent.notify();
                            mSPICONTROL = (0x2 << 8) | Create_SPI_Control(mNext_chid); //stbe = 10
                            mWriteSPICONTROLEvent.notify();
                            wait(3 * (double) mClkPeriod[emMSPInCLKId], mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                            CheckWaitInThread();
                        } else { //the idle time is set to 6 or more
                            re_printf("info","idle time called. %d\n", mDelay_value[emDIDLE]);
                            wait((mDelay_value[emDIDLE] - 3) * (double) mClkPeriod[emMSPInCLKId], mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                            CheckWaitInThread();
                            mMSPInSCK = mpMSPI000_Func->GetMSPInCPOLm(mNext_chid);
                            mWriteMSPInSCKEvent.notify();
                            mSPICONTROL = (0x2 << 8) | Create_SPI_Control(mNext_chid); //stbe = 10
                            mWriteSPICONTROLEvent.notify();
                            wait(3 * (double) mClkPeriod[emMSPInCLKId], mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                            CheckWaitInThread();
                        }
                    } else {
                        re_printf("info","idle time called. %d\n", mDelay_value[emDIDLE]);
                        wait(idle_time, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                        CheckWaitInThread();
                    }
                    for (unsigned int i = 0; i < mChipselectNum; i++) {
                        mMSPInCS[i] = ((mpMSPI000_Func->GetMSPInCSP() & (1 << i)) == 0);
                    }
                } else {
                    mpMSPI000_Func->GetDelayValue(mCom_chid, mDelay_value);
                    if (mpMSPI000_Func->GetMSPInFIDLm(mCom_chid) == 1) {
                        //delay hold time
                        double hold_time = mDelay_value[emDHOLD] * (double) mClkPeriod[emMSPInCLKId];
                        re_printf("info","hold time called. %d\n", mDelay_value[emDHOLD]);
                        wait(hold_time, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                        CheckWaitInThread();
                        //inactive cs
                        if (mpMSPI000_Func->GetMSPInICLSm(mCom_chid) == 1 && mCMode[mCom_chid] != emRX && mMMode[mCom_chid] == emDirect) {
                            for (unsigned int i = 0; i < mChipselectNum; i++) {
                                mMSPInCS[i] = ((mpMSPI000_Func->GetMSPInCSP() & (1 << i)) == 0);
                                if ((mTX_Reserve_Buffer.csr & (1 << i)) != 0) {
                                    mMSPInCS[i] = !mMSPInCS[i];
                                }
                            }
                            mWriteMSPInCSEvent.notify();
                        } else {
                            for (unsigned int i = 0; i < mChipselectNum; i++) {
                                mMSPInCS[i] = ((mpMSPI000_Func->GetMSPInCSP() & (1 << i)) == 0);
                            }
                            mWriteMSPInCSEvent.notify();
                        }
                        //delay idle time
                        double idle_time = mDelay_value[emDIDLE] * (double) mClkPeriod[emMSPInCLKId];
                        re_printf("info","idle time called. %d\n", mDelay_value[emDIDLE]);
                        wait(idle_time, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                        CheckWaitInThread();
                        for (unsigned int i = 0; i < mChipselectNum; i++) {
                            mMSPInCS[i] = ((mpMSPI000_Func->GetMSPInCSP() & (1 << i)) == 0);
                        }
                    } else if (((mCount_com[mCom_chid] % mInteral_Frame_Limit[mCom_chid]) == 0) && !mTX_Reserve_Buffer_Status) {
                        re_printf("info","inter data time called. %d\n", mDelay_value[emDINDA]);
                        double inda_time = mDelay_value[emDINDA] * (double) mClkPeriod[emMSPInCLKId];
                        wait(inda_time, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                        CheckWaitInThread();
                    }
                }

                if (mCount_com[mCom_chid] / mInteral_Frame_Limit[mCom_chid] == mpMSPI000_Func->GetMSPInCFSETm(mCom_chid)) {
                    mpMSPI000_Func->SetMSPInCHENm(mCom_chid, 0);
                    mEndChannelEvent.notify();
                }

                if (mCMode[mCom_chid] != emRX) {
                    if (!mTX_Reserve_Buffer_Status) {  //buffer has value
                        mWriteTXShiftRegisterEvent.notify();
                        if ((CheckHasRequest() && !mIsLockFunction && !mIsJobEnable && !mIsClearCHEN[mCom_chid]) || ((mIsLockFunction || mIsJobEnable) && mHasRequest[mReserve_chid] > 0)) {
                            mWriteTXReserveBufferEvent.notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
                        }
                    } else {
                        re_printf("info","The communication is waiting for request");
                        if (mIsClearCHEN[mCom_chid]) {
                            mIsClearCHEN[mCom_chid] = false;
                            mpMSPI000_Func->SetMSPInCHENm(mCom_chid, 0);
                            mEndChannelEvent.notify();
                            if (mMMode[mCom_chid] == emFixedFIFO) {
                                mpMSPI000_Func->SetMSPInFIRXNm(mCom_chid);
                            }
                        }
                    }
                } else if (mChState[mCom_chid] == emREADY) {
                    mCommunicationEvent.notify();
                }
                wait(mEndFrameEvent);
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

///*********************************
/// Function     : CommunicationSlaveThread
/// Description  : Start communication frame thread in slave mode
///*********************************
void MSPI000::CommunicationSlaveThread(void)
{//{{{
    for(;;) {
        if (mIsResetHardActive[emMSPInRESBId]||mIsResetCmdActive[emMSPInRESBId]||(!CheckClockPeriod("PCLK"))||mIPState == emIDLE) {
            wait();
            continue;
        }
        try {
            while (1) {
                unsigned int fl = (unsigned int)SPI_CONTROL.read() & 0xFF;
                mCount_com[mCom_chid]++;
                re_printf("info","CommunicationSlaveThread called.\n");
                // indicate frame is communicating
                mFrameEnd = false;

                if (mCMode[mCom_chid] != emRX) {
                    mTransmitEvent.notify();
                }
                if (mCMode[mCom_chid] != emTX) {
                    mReceiveEvent.notify();
                }
                //wait end frame
                if (mInteral_Frame_Limit[mCom_chid] > 1) {
                    wait(mMSPInSCKPeriod * 32, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                } else {
                    wait(mMSPInSCKPeriod * fl, mTimeResolutionUnit, mResetHardEvent[emMSPInRESBId]|mResetCmdEvent[emMSPInRESBId]|mClkZeroEvent[emPCLKId]|mClkZeroEvent[emMSPInCLKId]);
                }
                CheckWaitInThread();
                if (mCMode[mCom_chid] != emTX) {
                    mWriteRXReserveBufferEvent.notify(CalculateCLK(emPCLKId), mTimeResolutionUnit);
                }
                mEndFrameSlaveEvent.notify();
                wait(mCommunicationSlaveEvent);
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

///*********************************
/// Function     : EndFrameSlaveThread
/// Description  : End frame thread in slave mode
///*********************************
void MSPI000::EndFrameSlaveThread(void)
{//{{{
    for(;;) {
        if (mIsResetHardActive[emMSPInRESBId]||mIsResetCmdActive[emMSPInRESBId]||(!CheckClockPeriod("PCLK"))||mIPState == emIDLE) {
            wait();
            continue;
        }
        try {
            while (1) {
                re_printf("info","EndFrameSlaveThread called.\n");
                if (mParityError[0] || mDataConsistencyError[0]) {
                    mAssertINTMSPInERRmEvent[0].notify((double) mClkPeriod[emPCLKId], mTimeResolutionUnit);
                }
                if (mpMSPI000_Func->GetMSPInRXRQFm(mCom_chid) == 1 && mCMode[mCom_chid] != emTX) {
                    mOverRunError[0] = true;
                }
                // indicate is frame transmit done
                mFrameEnd = true;
                mpMSPI000_Func->SetMSPInCFCNTm(mCom_chid, mpMSPI000_Func->GetMSPInCFSETm(mCom_chid) - mCount_com[mCom_chid] / mInteral_Frame_Limit[mCom_chid]);
                if (mCount_com[mCom_chid] / mInteral_Frame_Limit[mCom_chid] == mpMSPI000_Func->GetMSPInCFSETm(mCom_chid) || (mIsClearCHEN[mCom_chid] && mpMSPI000_Func->GetMSPInJOBENm(mCom_chid) == 0)) {
                    if (mpMSPI000_Func->GetMSPInFCCEm(mCom_chid) == 0 && mMMode[mCom_chid] == emDirect) {
                        mpMSPI000_Func->SetMSPInCHENm(mCom_chid, 0);
                        mEndChannelSlaveEvent.notify();
                    } else if (mpMSPI000_Func->GetMSPInFCCEm(mCom_chid) == 1 && mMMode[mCom_chid] == emDirect) {

                    } else {
                        mpMSPI000_Func->SetMSPInCHENm(mCom_chid, 0);
                        mEndChannelSlaveEvent.notify();
                    }
                } else {
                    if (mCMode[mCom_chid] != emRX) {
                        if (!mTX_Reserve_Buffer_Status) {  //buffer has value
                            mWriteTXShiftRegisterEvent.notify();
                            if (CheckHasRequest()) {
                                mWriteTXReserveBufferEvent.notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
                            }
                        } else {
                            re_printf("info","The communication is waiting for request");
                        }
                        if ((((unsigned int)SPI_CONTROL.read() >> 8) & 3) == 1) {
                            mCommunicationSlaveEvent.notify();
                        }
                    } else {
                        if ((((unsigned int)SPI_CONTROL.read() >> 8) & 3) == 1) {
                            mCommunicationSlaveEvent.notify();
                        }
                    }
                }
                wait(mEndFrameSlaveEvent);
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

///*********************************
/// Function     : StartFixedBufferCommunication
/// Description  : Start Fixed Buffer Communication
///*********************************
void MSPI000::StartFixedBufferCommunication(unsigned int chid)
{//{{{
    mHasRequest[chid] = mInteral_Frame_Limit[chid] * mpMSPI000_Func->GetMSPInCFSETm(chid);
    if (mTX_Reserve_Buffer_Status && mFrameEnd) {
        mReserve_chid = GetChannelPriority(mReserve_chid);
        mWriteTXReserveBufferEvent.notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
        mWriteTXShiftRegisterEvent.notify(2 * (double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
    }
    if (mpMSPI000_Func->GetMSPInCFSETm(chid) > 1) {
        mWriteTXReserveBufferEvent1.notify(4 * (double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : WriteMOSIMethod
/// Description  : Write MOSI Method
///*********************************
void MSPI000::WriteMOSIMethod(void)
{//{{{
    if (mMode == emMaster || (mMode == emSlave && mSlave_enable)) {
        if (!mIsLoopBack) {
            for (unsigned int i = 0; i < emNumDuplicatePort; i++) {
                MOSI[i].write((uint32_t)mMOSI);
            }
        }
    }
}//}}}

///*********************************
/// Function     : WriteSPICONTROLMethod
/// Description  : Write SPICONTROL Method
///*********************************
void MSPI000::WriteSPICONTROLMethod(void)
{//{{{
    if (mMode == emMaster) {
        if (!mIsLoopBack) {
            SPI_CONTROL.write((uint32_t)mSPICONTROL);
        }
    }
    if (mIsDataConsistencyCheck && ((mSPICONTROL>>8 & 0x3) ==0x1)) 
    {
        mCheckDataconsistencyEvent.notify((double)(mComparisonTiming * mMSPInSCKPeriod ),mTimeResolutionUnit);    
    }
      
}//}}}

///*********************************
/// Function     : WriteMSPInSCKMethod
/// Description  : Write MSPInSCK Method
///*********************************
void MSPI000::WriteMSPInSCKMethod(void)
{//{{{
    if (mMode == emMaster) {
        for (unsigned int i = 0; i < emNumDuplicatePort; i++) {
            if (!mIsLoopBack) {
                MSPInSCK[i].write((sc_dt::uint64)mMSPInSCK);
            }
        }
    }
}//}}}

///*********************************
/// Function     : WriteMSPInCSMethod
/// Description  : Write MSPInCS Method
///*********************************
void MSPI000::WriteMSPInCSMethod(void)
{//{{{
    if (mMode == emMaster) {
        for (unsigned int i = 0; i < mChipselectNum; i++) {
            if (!mIsLoopBack) {
                MSPInCS[i]->write(mMSPInCS[i]);
            }
        }
    }
}//}}}

///*********************************
/// Function     : GetChannelPriority
/// Description  : Get channel priority
///*********************************
unsigned int MSPI000::GetChannelPriority(unsigned int ch_com)
{//{{{
    //Calculate priority
    unsigned int ch_priority = mChannelNum-1;
    unsigned int *priority_array = new unsigned int [mChannelNum];
    sc_assert(priority_array != NULL);
    for (unsigned int i = 0; i < mChannelNum; i++) {
        if (mChState[i] == emREADY && (mHasRequest[i] > 0 || mCMode[i] == emRX)) {
            priority_array[i] = mpMSPI000_Func->GetMSPInPRIOm(i);
        } else {
            priority_array[i] = emLowestPriority;
        }
    }
    if (mInteral_Frame_Limit[ch_com] > 1 && (mHasRequest[ch_com] >= 1 || mCMode[ch_com] == emRX)) {
        //channel with big frame length take priority
        re_printf("info","Channel %d is big frame length", ch_com);
        ch_priority = ch_com;
    } else {
        //The priority level can be set independently for each channel by MSPInCFGm0.MSPInPRIOm[2:0]. The channel
        //priority levels can be set from 0 to 7, with 0 the highest and 7 the lowest level.
        //If the priority settings of several channels are the same, the lowest interrupt channel number has priority.
        unsigned int min = emLowestPriority;
        for (int i = (int) mChannelNum-1; i >= 0; i--) {
            if ((priority_array[i] <= min) && (priority_array[i] != emLowestPriority)) {
                min = priority_array[i];
                ch_priority = i;
            }
        }
    }
    delete [] priority_array;
    return ch_priority;
}//}}}
///*********************************
/// Function     : CheckStartCommunication
/// Description  : Check start communication for each channel
///*********************************
void MSPI000::CheckStartCommunication(unsigned int chid)
{//{{{
    if (mMode == emSlave && chid != 0) {
        re_printf("warning","In the slave mode, only channel 0 can be used.");
    } else {
        if (mMMode[chid] != emFixedBuffer && mCMode[chid] != emRX) {
            mAssertINTMSPInTXmEvent[chid].notify(CalculateCLK(emPCLKId), mTimeResolutionUnit);
        }
        mpMSPI000_Func->SetMSPInCFCNTm(chid, mpMSPI000_Func->GetMSPInCFSETm(chid));
        mModelState = emModelREADY;

        if (mpMSPI000_Func->GetMSPInFLENm(chid) >= 32) {
            re_printf("info","Channel %d starting with frame length >= 32 bits.", chid);
            mInteral_Frame_Limit[chid] = (unsigned int) mpMSPI000_Func->GetMSPInFLENm(chid) / 32;
            if (mpMSPI000_Func->GetMSPInFLENm(chid) % 32 != 0) {
                mInteral_Frame_Limit[chid]++;
            }
        } else if (mpMSPI000_Func->GetMSPInFLENm(chid) <= 16) {
            re_printf("info","Channel %d starting with frame length <= 16 bits.", chid);
            mInteral_Frame_Limit[chid] = 1;
        }
        if (mMode == emMaster) {
            //If MSPInCSRm of plural channels are the exact same, following parameters have to be set the same value.
            for (unsigned int i = 0; i < mChannelNum; i++) {
                if (mChState[i] == emREADY && i != chid) {
                    if (mpMSPI000_Func->GetMSPInCSRm(i) == mpMSPI000_Func->GetMSPInCSRm(chid)) {
                        mpMSPI000_Func->CheckSameValue(i, chid);
                    }
                }
            }
        }

        if (mMMode[chid] == emFixedBuffer) {
            re_printf("info","Channel %d starting in fixed buffer memory mode. Frame length = %d. Frame count = %d. Lock function = %d.", chid, mpMSPI000_Func->GetMSPInFLENm(chid), mpMSPI000_Func->GetMSPInCFSETm(chid), mpMSPI000_Func->GetMSPInLOCKm(chid));
            if (mCMode[chid] != emRX) {
                StartFixedBufferCommunication(chid);
            }
        } else if (mMMode[chid] == emFixedFIFO) {
            re_printf("info","Channel %d starting in fixed FIFO memory mode. Frame length = %d. Frame count = %d. Lock function = %d.", chid, mpMSPI000_Func->GetMSPInFLENm(chid), mpMSPI000_Func->GetMSPInCFSETm(chid), mpMSPI000_Func->GetMSPInLOCKm(chid));
        } else {
            re_printf("info","Channel %d starting in direct memory mode. Frame length = %d. Frame count = %d. Lock function = %d.", chid, mpMSPI000_Func->GetMSPInFLENm(chid), mpMSPI000_Func->GetMSPInCFSETm(chid), mpMSPI000_Func->GetMSPInLOCKm(chid));
        }
        if (mMode == emMaster) {
            if (mpMSPI000_Func->GetmIsSafeSPI(chid)) {
                mIsSafeSPI[chid] = true;
                if (mpMSPI000_Func->GetMSPInSAFSm(chid) == 1) {
                    re_printf("info","Channel %d operation Safe-SPI function out-of-frame format.", chid);
                } else {
                    re_printf("info","Channel %d operation Safe-SPI function in-frame format.", chid);
                }
                if (mpMSPI000_Func->GetMSPInSAFCMm(chid) == 1) {
                    re_printf("info","Masks the CRC error of the first frame");
                }
            }
            if (mCMode[chid] == emRX) { //after 3 PCLK + 3 MSPInCLK
                if (mFrameEnd) {
                    mCommunicationEvent.notify(3 * mClkPeriod[emPCLKId] + 3 * mClkPeriod[emMSPInCLKId] + CalculateCLK(emPCLKId), mTimeResolutionUnit);
                    mCom_chid = chid;
                }
            }
        }
    }
}//}}}

//Functions
///*********************************
/// Function     : Create_SPI_Control
/// Description  : Create SPI Control value
///*********************************
unsigned int MSPI000::Create_SPI_Control(unsigned int chid)
{//{{{
    unsigned int spi_control_val = 0;
    //read setting create SPI control for transmission.
    spi_control_val = mpMSPI000_Func->Get_SPI_Control_Setting(chid);
    return spi_control_val;
}//}}}

///*********************************
/// Function     : Calculate_mMSPInCS
/// Description  : Calculate CS value and check actived CS
///*********************************
bool MSPI000::Calculate_mMSPInCS(void)
{//{{{
    bool actived = false;
    unsigned int csr = mpMSPI000_Func->GetMSPInCSRm(mCom_chid);
    unsigned int csp = mpMSPI000_Func->GetMSPInCSP();
    for (unsigned int i = 0; i < mChipselectNum; i++) {
        mMSPInCS_prev[i] = mMSPInCS[i];
        if ((((csr >> i) & 0x1) == 1) && (((csp >> i) & 0x1) == 0)) {
            mMSPInCS[i] = false;
            actived = true;
        } else if ((((csr >> i) & 0x1) == 1) && (((csp >> i) & 0x1) == 1)){
            mMSPInCS[i] = true;
            actived = true;
        }
    }
    return actived;
}//}}}

///*********************************
/// Function     : Compare_mMSPInCS
/// Description  : Compare previous CS and current CS
///*********************************
bool MSPI000::Compare_mMSPInCS(void)
{//{{{
    for (unsigned int i = 0; i < mChipselectNum; i++) {
        if (mMSPInCS[i] != mMSPInCS_prev[i]) {
            return true;
        }
    }
    return false;
}//}}}

///*********************************
/// Function     : CalculateCLK
/// Description  : Calculate CLK to call CalculateCLKPosEdge
///*********************************
double MSPI000::CalculateCLK(eClockInfo id)
{//{{{
    double cur_timer = sc_time_stamp().to_double();
    return CalculateCLKPosEdge((double)mClkPeriod[id], false, cur_timer, mStartClkTime[id]);
}//}}}

///*********************************
/// Function     : GetParity
/// Description  : Get parity
///*********************************
bool MSPI000::GetParity(const unsigned int data, const unsigned int chid)
{//{{{
    unsigned char one_cnt = 0;
    unsigned int temp_data = (unsigned int) data;
    bool parity_bit = false;
    // Count number of 1 bit in data
    while (temp_data != 0) {
        temp_data &= (temp_data - 1);
        one_cnt ++;
    }
    // Process even/odd parity mode
    unsigned int ps = mpMSPI000_Func->GetMSPInPSm(chid);
    if (ps == 0) {  //old parity
          if((one_cnt&1) == 0){
              parity_bit="true";
          }else{
              parity_bit="false";
          }
    } else if (ps == 1) { //even parity
          if((one_cnt&1) == 1){
              parity_bit="true";
          }else{
              parity_bit="false";
          }
    } else  { //0 parity
        parity_bit = false;
    }
    return parity_bit;
}//}}}
///*********************************
/// Function     : HasError
/// Description  : Check has error
///*********************************
bool MSPI000::HasError(unsigned int chid)
{//{{{
    return (mParityError[chid] || mDataConsistencyError[chid] || mOverRunError[chid] || mCRCError[chid]);
}//}}}
///*********************************
/// Function     : CheckHasRequest
/// Description  : Check has request
///*********************************
bool MSPI000::CheckHasRequest(void)
{//{{{
    for (unsigned int i = 0; i < mChannelNum; i++) {
        if (mChState[i] == emREADY && mHasRequest[i] > 0) {
            return true;
        }
    }
    return false;
}//}}}
///*********************************
/// Function     : CommuniationSlave
/// Description  : Communiation in Slave mode
///*********************************
void MSPI000::CommuniationSlave(void)
{//{{{
    mMSPInSCKPeriod = (double) (sc_dt::uint64) (((1/mMSPInSCKI)*mTimeResolutionValue)+0.5);
    mCom_chid = 0;
    if (mFrameEnd) {
        mCommunicationSlaveEvent.notify();
    }
}//}}}

///Calculate CRC code
///*********************************
/// Function     : CalculateCRC
/// Description  : Calculate CRC
///*********************************
unsigned char MSPI000::CalculateCRC(unsigned int message, unsigned char message_length, unsigned char crc_bit_num, unsigned char crc_init, unsigned char crc_poly)
{//{{{
    unsigned long long remainder;
    // Right shift the polynomial
    unsigned char poly_shift_pos = message_length - 1; // 7
    unsigned long long poly = (unsigned long long)crc_poly << poly_shift_pos;
    unsigned char bit = message_length + crc_bit_num - 1; // 10

    // Initially, the dividend is the remainder
    remainder = (unsigned long long)message;
    remainder <<= crc_bit_num; // padding crc_bit_num bit zero at the lowermost
    remainder |= (unsigned long long)crc_init; // padding CRC initial value

    while ((remainder >> crc_bit_num) != 0) {
        if ((bool)((remainder >> bit) & 0x1)) {
            remainder ^= poly;
        }
        poly >>= 1;
        bit--;
    }
    return (unsigned char) remainder;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
///*********************************
/// Function     : CheckClockPeriod 
/// Description  : Check Clock Period
///*********************************
bool MSPI000::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;

    if (clock_name == "PCLK"){
        clock_period = mClkPeriod[emPCLKId];
    }else{// MSPInCLK
        clock_period = mClkPeriod[emMSPInCLKId];
    }

    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///*********************************
/// Function     : InactiveOutputPorts
/// Description  : Inactive Output Ports
///*********************************
void MSPI000::InactiveOutputPorts(const unsigned int csp, const unsigned int ckr, const unsigned int sols)
{//{{{
    re_printf("info","MSPInCS, MSPInSCK, MOSI Inactive Output Ports\n");
    for (unsigned int i = 0; i < mChipselectNum; i++) {
        mMSPInCS[i] = ((csp & (1 << i)) == 0);
    }
    double cur_time = sc_time_stamp().to_double();
    mWriteMSPInCSEvent.notify(2 * mClkPeriod[emMSPInCLKId] + CalculateCLK(emMSPInCLKId), mTimeResolutionUnit);
    if(ckr == 0){
        mMSPInSCK = 0;
    }else{
        mMSPInSCK = 1;
    }
    mWriteMSPInSCKEvent.notify(2 * mClkPeriod[emMSPInCLKId] + CalculateCLK(emMSPInCLKId), mTimeResolutionUnit);
    if((sols & 2) == 0){
        mMOSI = 0;
    }else{
        mMOSI = 0xFFFFFFFF;
    }
    mWriteMOSIEvent.notify(2 * mClkPeriod[emMSPInCLKId] + CalculateCLK(emMSPInCLKId), mTimeResolutionUnit);
    mTX_Reserve_Buffer_Status = true;

}//}}}
///*********************************
/// Function     : SetOperationMode
/// Description  : Set Operation Mode
///*********************************
void MSPI000::SetOperationMode(bool mode)
{//{{{
    if (mode) {
        mMode = emMaster;
        re_printf("info", "Master mode is enable.\n");
        MSPInMD.write(true);
    } else {
        mMode = emSlave;
        re_printf("info", "Slave mode is enable.\n");
        MSPInMD.write(false);
        mCount_com[0] = 0;
    }
}//}}}
///*********************************
/// Function     : SetCommunicationMode
/// Description  : Set Communication Mode
///*********************************
void MSPI000::SetCommunicationMode(unsigned int chid, unsigned int mode)
{//{{{
    if (mode == 0) {
        mCMode[chid] = emNoneTxRx;
    } else if (mode == 1) {
        mCMode[chid] = emTXRX;
    } else if (mode == 2) {
        mCMode[chid] = emTX;
    } else {
        mCMode[chid] = emRX;
    }
}//}}}
///*********************************
/// Function     : SetMemoryMode
/// Description  : Set Memory Mode
///*********************************
void MSPI000::SetMemoryMode(unsigned int chid, unsigned int mode)
{//{{{
    if (mode == 0) {
        mMMode[chid] = emDirect;
    } else if (mode == 1) {
        mMMode[chid] = emFixedBuffer;
    } else {
        mMMode[chid] = emFixedFIFO;
    }
}//}}}
///*********************************
/// Function     : SetIPState
/// Description  : Set IP state
///*********************************
void MSPI000::SetIPState(bool state)
{//{{{
    if (state) {
        mIPState = emOPERATION;
        mIsLoopBack = mpMSPI000_Func->GetIsLoopBack();
        mIsDataConsistencyCheck = mpMSPI000_Func->GetIsDataConsistency();
        if (mIsLoopBack) {
            re_printf("info", "Loop back mode is enable.\n");
        }
        if (mIsDataConsistencyCheck) {
            MSPInDCSIDRV.write(true);
            re_printf("info", "Data Consistency Check is enable.\n");
        } else {
            MSPInDCSIDRV.write(false);
        }
        mSPICONTROL = emInitData;
        mWriteSPICONTROLEvent.notify();
    } else {
        mIPState = emIDLE;
    }
}//}}}
///*********************************
/// Function     : SetChannelState
/// Description  : Set channel state
///*********************************
void MSPI000::SetChannelState(unsigned int chid, unsigned int state, unsigned int size)
{//{{{
    if (mIPState == emOPERATION) {
        if (state == 0) {
            mChState[chid] = emINIT;
        } else {
            mSizes[chid] = size;
            mChState[chid] = emREADY;
            CheckStartCommunication(chid);
            if (mMode == emMaster) {
                for (unsigned int i = 0; i < mChannelNum; i++) {
                    if (mCMode[i] != emRX) {
                        MSPInSOUTDRV.write(true);
                        break;
                    }
                }
            } else if (mMode == emSlave && ((mpMSPI000_Func->GetMSPInCSIE() == 0 && mCMode[0] != emRX) || (mpMSPI000_Func->GetMSPInCSIE() == 1 && mCMode[0] != emRX && mCSI == 0))) {
                MSPInSOUTDRV.write(true);
            } else {
                MSPInSOUTDRV.write(false);
            }
        }
    }
    unsigned int cnt = 0;
    for (unsigned int i = 0; i < mChannelNum; i++) {
        if (mChState[i] == emREADY) {
            cnt++;
        }
    }
    mpMSPI000_Func->SetMSPInCNUMF(cnt);
}//}}}
///*********************************
/// Function     : GetHWTRGValue
/// Description  : Get hardware trigger value
///*********************************
sc_dt::uint64 MSPI000::GetHWTRGValue()
{//{{{
    return mHWTRGVal;
}//}}}
///*********************************
/// Function     : UpdateRequestNumber
/// Description  : Update number request of each channel
///*********************************
void MSPI000::UpdateRequestNumber(unsigned int chid)
{//{{{
    if (mModelState == emModelREADY) {
        re_printf("info", "Channel %d update request number %d.", chid, mRequestNumber[chid]+1);
        mHasRequest[chid]++;
        mRequestNumber[chid]++;
        if (mMMode[chid] == emFixedFIFO && mpMSPI000_Func->GetmHalfFIFOFilled(chid) && mRequestNumber[chid] < (mpMSPI000_Func->GetMSPInCFSETm(chid) * mInteral_Frame_Limit[chid])) {
            mAssertINTMSPInTXmEvent[chid].notify();
        }
        if (mMode == emMaster) {
            if ((mTX_Reserve_Buffer_Status && mFrameEnd && !mIsLockFunction && !mIsJobEnable)
            || ((mIsLockFunction || mIsJobEnable) && mTX_Reserve_Buffer_Status && mFrameEnd && (chid == mTX_Reserve_Buffer.chid))) { //reserve empty and no channel communication
                mReserve_chid = GetChannelPriority(mReserve_chid);
                re_printf("info", "Channel %d request %d is written to TX Reserver Buffer after 1 PCLK period", chid, mRequestNumber[chid]);
                mWriteTXReserveBufferEvent.notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
                mWriteTXShiftRegisterEvent.notify( 2 * (double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
            } else if ((mTX_Reserve_Buffer_Status && !mFrameEnd && !mIsLockFunction && !mIsJobEnable)
              || ((mIsLockFunction || mIsJobEnable) && mTX_Reserve_Buffer_Status && !mFrameEnd && (chid == mTX_Reserve_Buffer.chid))) { //reserve empty and has channel communication
                if (mReserve_chid == (mChannelNum-1)) {
                    mReserve_chid = chid;
                }
                re_printf("info", "Channel %d request %d is written to TX Reserver Buffer after 2 PCLK period", chid, mRequestNumber[chid]);
                mWriteTXReserveBufferEvent.notify( 2 * (double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
            } else {
                re_printf("info", "Channel %d request %d TX Reserve Buffer busy", chid, mRequestNumber[chid]);
            }
        } else {
            if (mTX_Reserve_Buffer_Status && mRequestNumber[chid] == 1) { //reserve empty and no channel communication
                mReserve_chid = 0;
                re_printf("info", "Channel %d request %d is written to TX Reserver Buffer after 1 PCLK period", chid, mRequestNumber[chid]);
                mWriteTXReserveBufferEvent.notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
                mWriteTXShiftRegisterEvent.notify( 2 * (double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
            } else if (mTX_Reserve_Buffer_Status && mRequestNumber[chid] == 2) { //reserve empty and has channel communication
                mReserve_chid = 0;
                re_printf("info", "Channel %d request %d is written to TX Reserver Buffer after 2 PCLK period", chid, mRequestNumber[chid]);
                mWriteTXReserveBufferEvent.notify( 2 * (double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
            } else {
                re_printf("info", "Channel %d request %d TX Reserve Buffer busy", chid, mRequestNumber[chid]);
            }
        }
    }
}//}}}
///*********************************
/// Function     : AsserINTMSPIn
/// Description  : Notify interrupt
///*********************************
void MSPI000::AsserINTMSPIn(unsigned int chid, std::string int_name, std::string error_type)
{//{{{
    if (int_name == "RX") {
        mAssertINTMSPInRXmEvent[chid].notify();
    }
    if (int_name == "ER") {
        if (error_type == "Over-write") {
            mOverWriteError[chid] = true;
        }
        if (error_type == "Over-read") {
            mOverReadError[chid] = true;
        }
        mAssertINTMSPInERRmEvent[chid].notify();
    }
    if (int_name == "ReadMSPInRXDAm0") {
        mReadMSPInRXDAm0Event.notify();
    }
}//}}}
///*********************************
/// Function     : SetChannelStop
/// Description  : Stop channel communication
///*********************************
void MSPI000::SetChannelStop(unsigned int chid)
{//{{{
    if (chid != mCom_chid && !mTX_Reserve_Buffer_Status && mTX_Reserve_Buffer.chid != chid) {
        mIsClearCHEN[chid] = true;
        mpMSPI000_Func->SetMSPInCHENm(chid, 0);
    } else if ((!mTX_Reserve_Buffer_Status && mTX_Reserve_Buffer.chid == chid) || (mTX_Reserve_Buffer_Status && chid == mCom_chid)){
        mIsClearCHEN[chid] = true;
    }
}//}}}
///*********************************
/// Function     : CheckWaitInThread
/// Description  : Check when end wait in thread by reset or zero clock or IP inactive
///*********************************
void MSPI000::CheckWaitInThread()
{
    if (mIsResetHardActive[emMSPInRESBId]||mIsResetCmdActive[emMSPInRESBId]||((!CheckClockPeriod("MSPInCLK")) && mMode == emMaster)||(!CheckClockPeriod("PCLK"))||mIPState == emIDLE) {
        throw vpcl::emRstException;
    }
}
///*********************************
/// Function     : SetComparisonTiming
/// Description  : Set Comparision Timing check data consistency
///*********************************
void MSPI000::SetComparisonTiming(float value)
{
    mComparisonTiming = value;
}
///*********************************
/// Function     : NotifyEventOutputINT
/// Description  : Notify change Output Interrupt port. 
///*********************************
void MSPI000::NotifyEventOutputINT(unsigned int type)
{
    if(type == emNumTX){
        mWriteINTMSPInTXEvent.notify();
    }else if (type == emNumRX){
        mWriteINTMSPInRXEvent.notify();
    }else if(type == emNumFE){
        mWriteINTMSPInFEEvent.notify();
    }else{
        mWriteINTMSPInERREvent.notify();
    }
}
///*********************************
/// Function     : WriteINTMSPInTXMethod
/// Description  : Write to  INTMSPInTX port. 
///*********************************
void MSPI000::WriteINTMSPInTXMethod()
{
    unsigned int MSPInINTFValue = mpMSPI000_INT_FACTOR_Func->GetMSPInINTFk(emNumTX);
    unsigned int MSPInINTMSKkValue = mpMSPI000_INT_FACTOR_Func->GetMSPInINTMSKk(emNumTX);
    unsigned int temp = 0;
    bool TX_Port_value = false;
    temp = MSPInINTFValue & (~MSPInINTMSKkValue);
    for (unsigned int i =0; i<mChannelNum; i++){
        if ((bool)(((temp>>i)&0x1) == 0x1)){
            TX_Port_value = true;
            break;
        }
    }
    if(TX_Port_value != INTMSPInTX.read()){
        INTMSPInTX.write(TX_Port_value);
        DumpInterruptMsg("INTMSPInTX", TX_Port_value);
    }
}
///*********************************
/// Function     : WriteINTMSPInRXMethod
/// Description  : Write to  INTMSPInRX port. 
///*********************************
void MSPI000::WriteINTMSPInRXMethod()
{
    unsigned int MSPInINTFValue = mpMSPI000_INT_FACTOR_Func->GetMSPInINTFk(emNumRX);
    unsigned int MSPInINTMSKkValue = mpMSPI000_INT_FACTOR_Func->GetMSPInINTMSKk(emNumRX);
    unsigned int temp = 0;
    bool RX_Port_value = false;
    temp = MSPInINTFValue & (~MSPInINTMSKkValue);
    for (unsigned int i =0; i<mChannelNum; i++){
        if ((bool)(((temp>>i)&0x1) == 0x1)){
            RX_Port_value = true;
            break;
        }
    }
    if(RX_Port_value != INTMSPInRX.read()){
        INTMSPInRX.write(RX_Port_value);
        DumpInterruptMsg("INTMSPInRX", RX_Port_value);
    }
}
///*********************************
/// Function     : WriteINTMSPInFEMethod
/// Description  : Write to  INTMSPInFE port. 
///*********************************
void MSPI000::WriteINTMSPInFEMethod()
{
    unsigned int MSPInINTFValue = mpMSPI000_INT_FACTOR_Func->GetMSPInINTFk(emNumFE);
    unsigned int MSPInINTMSKkValue = mpMSPI000_INT_FACTOR_Func->GetMSPInINTMSKk(emNumFE);
    unsigned int temp = 0;
    bool FE_Port_value = false;
    temp = MSPInINTFValue & (~MSPInINTMSKkValue);
    for (unsigned int i =0; i<mChannelNum; i++){
        if ((bool)(((temp>>i)&0x1) == 0x1)){
            FE_Port_value = true;
            break;
        }
    }
    if(FE_Port_value != INTMSPInFE.read()){
        INTMSPInFE.write(FE_Port_value);
        DumpInterruptMsg("INTMSPInFE", FE_Port_value);
    }
}
///*********************************
/// Function     : WriteINTMSPInERRMethod
/// Description  : Write to  INTMSPInERR port. 
///*********************************
void MSPI000::WriteINTMSPInERRMethod()
{
    unsigned int MSPInINTFValue = mpMSPI000_INT_FACTOR_Func->GetMSPInINTFk(emNumERR);
    unsigned int MSPInINTMSKkValue = mpMSPI000_INT_FACTOR_Func->GetMSPInINTMSKk(emNumERR);
    unsigned int temp = 0;
    bool ERR_Port_value = false;
    temp = MSPInINTFValue & (~MSPInINTMSKkValue);
    for (unsigned int i =0; i<mChannelNum; i++){
        if ((bool)(((temp>>i)&0x1) == 0x1)){
            ERR_Port_value = true;
            break;
        }
    }
    if(ERR_Port_value != INTMSPInERR.read()){
        INTMSPInERR.write(ERR_Port_value);
        DumpInterruptMsg("INTMSPInERR", ERR_Port_value);
    }
}

bool MSPI000::GetResetStatus(void)
{//{{{
    return (mIsResetHardActive[emPRESETnId] || mIsResetCmdActive[emPRESETnId]);
}//}}}
void MSPI000::readCfgFile(const char *filename)
{//{{{
    char    word[512];
    char    seps[] = " ,=();\t\n\r";
    char*   token;

    if(filename == NULL)
    {
        return;
    }
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
        if ((NULL == token) || (0 == strncmp(token, "//", 2))) {
            continue;
        }

        // ----------------------------------------
        // detect end marker
        // NOTICE:You have to check whether token is NULL first.
        if ( 0 == strcmp(token, "[END]") ) {
            break;
        }

        // ----------------------------------------
        // get token
        // ----------------------------------------
        // For ICUM_PE_NUM
        if (0 == strcmp(token, "[MSPI_CH_NUM]")) {
            token = strtok(0, seps);
            if (token != NULL) {
                mChannelNum = token_to_uint( token,
                                        "MSPI000",
                                        "[MSPI_CH_NUM]" );
                if ((mChannelNum > MAX_CH_NUM)||(mChannelNum <1)) {
                    // for WinMsg
                    re_printf("warning", "[MSPI_CH_NUM] must be under %d and over %d, but specified (%d). Channel number  is assigned to default value (8)",
                            MAX_CH_NUM ,0, mChannelNum);
                    mChannelNum = 8;
                }
            }
            continue;
        } else if (0 == strcmp(token, "[MSPI_CS_NUM]")) {
            token = strtok(0, seps);
            if (token != NULL) {
                mChipselectNum = token_to_uint( token,
                                                  "MSPI000",
                                                  "[MSPI_CS_NUM]" );
                if ((mChipselectNum > MAX_CS_NUM) ||(mChipselectNum < 1)) {
                    // for WinMsg
                    re_printf("warning",
                            "[MSPI_CS_NUM] must be under %d and over %d, but specified (%d). Chip select number is assigned to default value (8).",
                            MAX_CS_NUM, 0, mChipselectNum );
                    mChipselectNum = 8;
                }
            }
        } else if (0 == strcmp(token, "[MSPI_HW_TRIGGER_NUM]")) {
            token = strtok(0, seps);
            if (token != NULL) {
                mHWTriggerNum = token_to_uint( token,
                                                  "MSPI000",
                                                  "[MSPI_HW_TRIGGER_NUM]" );
                if ((mHWTriggerNum > MAX_HWTRIGGER_NUM) ||(mHWTriggerNum < 1)) {
                    // for WinMsg
                    re_printf("warning",
                            "[MSPI_HW_TRIGGER_NUM] must be under %d and over %d, but specified (%d). Hardware Trigger number  is assigned to default value (41).",
                            MAX_HWTRIGGER_NUM, 0, mHWTriggerNum );
                    mHWTriggerNum = 41;
                }
            }
        }
    }  // end of configuration file read loop(infinite)
}//}}} end of readCfgFile()
std::string MSPI000::FindRegAreaFromName(const std::string reg_name)
{//{{{
    std::string area = "";
    if (((reg_name.find("INTFk")) != std::string::npos)||((reg_name.find("INTMSKk"))!=std::string::npos)||((reg_name.find("INTFCk")) != std::string::npos)){
        area =  "INT_FACTOR";
    }else{
        area =  "MSPI000";
    }
    return area;
}
void MSPI000::ResetBufferCounter(unsigned int id)
{
    mCount_reserve[id] =0;
    mCount_com[id] =0;
}
// vim600: set foldmethod=marker :
