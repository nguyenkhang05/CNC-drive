// ---------------------------------------------------------------------
// $Id: DNF.cpp,v 1.6 2020/02/06 11:52:29 ngathunguyen Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "DNF.h"
#include "DNF_Func.h"

///Constructor of DNF class
DNF::DNF (sc_module_name name, unsigned int rLatency, unsigned int wLatency, unsigned int channel_num):
           sc_module(name),
           DNF_AgentController(),
           BusSlaveBase<32,1>(),
           rvc_common_model(),
           PRESETZ("PRESETZ"),
           DNFATRESZ("DNFATRESZ"),
           PCLK("PCLK"),
           DNFATCKI("DNFATCKI")
{//{{{
    //Initialize output port
    std::ostringstream port_name;
    for (unsigned short i=0; i<MAX_CHANNEL_NUM; i++) {
        port_name.str("");
        port_name << "DNFATFIN" << i;
        DNFATFIN[i] = new sc_in<bool>(port_name.str().c_str());

        port_name.str("");
        port_name << "DNFATRIN" << i;
        DNFATRIN[i] = new sc_in<bool>(port_name.str().c_str());

        port_name.str("");
        port_name << "DNFATDGIN" << i;
        DNFATDGIN[i] = new sc_in<bool>(port_name.str().c_str());

        port_name.str("");
        port_name << "DNFATANIN" << i;
        DNFATANIN[i] = new sc_in<bool>(port_name.str().c_str());

        port_name.str("");
        port_name << "DNFATAED" << i;
        DNFATAED[i] = new sc_in<bool>(port_name.str().c_str());

        port_name.str("");
        port_name << "DNFATINT" << i;
        DNFATINT[i] = new sc_out<bool>(port_name.str().c_str());

        port_name.str("");
        port_name << "DNFATNFOUT" << i;
        DNFATNFOUT[i] = new sc_out<bool>(port_name.str().c_str());

        DNFATINT[i]->initialize(false);
        DNFATNFOUT[i]->initialize(false);
    }
    //Initialize variables
    mIsStartSimulation = false;
    mIsOperating = false;
    if ((channel_num > 16) || (channel_num < 1)){
        re_printf("error","Setting of channel number is invalid. It should be from 1 to 16 channel.\n");
        mChannelNumber = 16;
    } else {
        mChannelNumber = channel_num;
    }
    mStartPCLKTime = 0;
    mStartDNFATCKITime = 0;
    mPCLKPeriod = 0;
    mDNFATCKIPeriod = 0;
    mPCLKFreq = 0;
    mDNFATCKIFreq = 0;
    mPCLKOrgFreq = 0;
    mDNFATCKIOrgFreq = 0;
    mPCLKFreqUnit = "Hz";
    mDNFATCKIFreqUnit = "Hz";
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mEnableRegisterMessage = true;
    
    mPRESETZSignalCurrentValue = emResetInactive;
    mPRESETZSignalPreviousValue = emResetInactive;
    mIsPRESETZHardActive = false;
    mIsPRESETZCmdReceive = false;
    mIsPRESETZCmdActive = false;
    mPRESETZCmdPeriod =  0;

    mDNFATRESZSignalCurrentValue = emResetInactive;
    mDNFATRESZSignalPreviousValue = emResetInactive;
    mIsDNFATRESZHardActive = false;
    mIsDNFATRESZCmdReceive = false;
    mIsDNFATRESZCmdActive = false;
    mDNFATRESZCmdPeriod =  0;

    mEdgeDetection = new unsigned char[mChannelNumber];
    mNFOUTValue = new bool[mChannelNumber];
    mINTValue = new bool[mChannelNumber];
    for (unsigned int i=0; i<mChannelNumber; i++) {
        mEdgeDetection[i] = 0;
        mNFOUTValue[i] = false;
        mINTValue[i] = false;
    }
    //Bind clock, reset port and target socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETZ);
    setTargetSocket32((char*)"ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding socket

    mDNF_Func = new DNF_Func((std::string)name, mChannelNumber, this);
    sc_assert(mDNF_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mDNF_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive<<PCLK;
 
    SC_METHOD(HandleDNFATCKISignalMethod);
    dont_initialize();
    sensitive<<DNFATCKI;
 
    SC_METHOD(HandlePRESETZSignalMethod);
    sensitive<<PRESETZ;
    
    SC_METHOD(HandlePRESETZHardMethod);
    dont_initialize();
    sensitive<<mPRESETZHardEvent;

    SC_METHOD(HandlePRESETZCmdMethod);
    dont_initialize();
    sensitive<<mPRESETZCmdEvent;
 
    SC_METHOD(CancelPRESETZCmdMethod);
    dont_initialize();
    sensitive<<mPRESETZCmdCancelEvent;

    SC_METHOD(HandleDNFATRESZSignalMethod);
    sensitive<<DNFATRESZ;
    
    SC_METHOD(HandleDNFATRESZHardMethod);
    dont_initialize();
    sensitive<<mDNFATRESZHardEvent;
 
    SC_METHOD(HandleDNFATRESZCmdMethod);
    dont_initialize();
    sensitive<<mDNFATRESZCmdEvent;
 
    SC_METHOD(CancelDNFATRESZCmdMethod);
    dont_initialize();
    sensitive<<mDNFATRESZCmdCancelEvent;

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(DNFATFIN[index]);
        opt.set_sensitivity(&mReadDNFATFINEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::HandleDNFATFINMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleDNFATFINMethod"), &opt);
    }

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(DNFATRIN[index]);
        opt.set_sensitivity(&mReadDNFATRINEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::HandleDNFATRINMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleDNFATRINMethod"), &opt);
    }

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(DNFATDGIN[index]);
        opt.set_sensitivity(&mReadDNFATDGINEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::HandleDNFATDGINMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleDNFATDGINMethod"), &opt);
    }

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(DNFATANIN[index]);
        opt.set_sensitivity(&mReadDNFATANINEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::HandleDNFATANINMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleDNFATANINMethod"), &opt);
    }

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(DNFATAED[index]);
        opt.set_sensitivity(&mReadDNFATAEDEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::HandleDNFATAEDMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleDNFATAEDMethod"), &opt);
    }

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDNFATINTEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::WriteDNFATINTMethod, this, index),
                          sc_core::sc_gen_unique_name("WriteDNFATINTMethod"), &opt);
    }

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mClearDNFATINTEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::ClearDNFATINTMethod, this, index),
                          sc_core::sc_gen_unique_name("ClearDNFATINTMethod"), &opt);
    }

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDNFATNFOUTEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::WriteDNFATNFOUTMethod, this, index),
                          sc_core::sc_gen_unique_name("WriteDNFATNFOUTMethod"), &opt);
    }

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mSynchronizingEvent[index]);
        opt.set_sensitivity(&mGetNextDGINSampleEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::HandleSynchronizedSignalMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleSynchronizedSignalMethod"), &opt);
    }

    for (unsigned char index=0; index<MAX_CHANNEL_NUM; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mGetDGINSampleEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DNF::ProcessDGINMethod, this, index),
                          sc_core::sc_gen_unique_name("ProcessDGINMethod"), &opt);
    }
}//}}}

///Destructor of DNF class
DNF::~DNF (void)
{//{{{
    delete mDNF_Func;
    delete[] mEdgeDetection;
    delete[] mNFOUTValue;
    delete[] mINTValue;
    for (unsigned int i=0; i<MAX_CHANNEL_NUM; i++) {
        delete DNFATFIN[i];
        delete DNFATRIN[i];
        delete DNFATDGIN[i];
        delete DNFATANIN[i];
        delete DNFATAED[i];
        delete DNFATINT[i];
        delete DNFATNFOUT[i];
    }
}//}}}

///Assert reset by software for PRESETZ and DNFATRESZ
void DNF::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETZ") {
        if ((!mIsPRESETZHardActive)&&(!mIsPRESETZCmdReceive)) {
            mIsPRESETZCmdReceive = true;
            re_printf("info","The model will be reset (PRESETZ) for %f ns after %f ns.\n", period, start_time);
            mPRESETZCmdEvent.notify(start_time, SC_NS);
            mPRESETZCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "DNFATRESZ") {
        if ((!mIsDNFATRESZHardActive)&&(!mIsDNFATRESZCmdReceive)) {
            mIsDNFATRESZCmdReceive = true;
            re_printf("info","The model will be reset (DNFATRESZ) for %f ns after %f ns.\n", period, start_time);
            mDNFATRESZCmdEvent.notify(start_time, SC_NS);
            mDNFATRESZCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of DNFATRESZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error","The reset name (%s) is wrong. It should be PRESETZ or DNFATRESZ.\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit for PCLK and DNFATCKI
void DNF::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        bool dont_care = false;
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
            }

        } else if (word_vector[0] == "DNFATCKI") {
            mDNFATCKIOrgFreq = freq;
            ConvertClockFreq (mDNFATCKIFreq, mDNFATCKIFreqUnit, freq, unit);
            if (mDNFATCKIFreq > 0) {
                mDNFATCKIPeriod = (sc_dt::uint64)(((1/(double)mDNFATCKIFreq)*(double)mTimeResolutionValue) + 0.5);
                mStartDNFATCKITime = sc_time_stamp().to_double();
            } else {
                mDNFATCKIPeriod = 0;
            }
        } else {
            dont_care = true;
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
        if (!dont_care) {
            if ((mPCLKPeriod != 0) && (mDNFATCKIPeriod != 0)) {
                StartOperation(); //Start operation when PCLK and DNFATCKI are greater than 0
            } else {
                StopOperation(); //Stop operation when PCLK or DNFATCKI is 0
            }
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock value of PCLK and DNFATCKI
void DNF::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "DNFATCKI") {
            re_printf("info","DNFATCKI frequency is %0.0f %s.\n", (double)mDNFATCKIOrgFreq, mDNFATCKIFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void DNF::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mDNF_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void DNF::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mDNF_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void DNF::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mDNF_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void DNF::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mDNF_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///List all reigsters name
void DNF::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mDNF_Func->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void DNF::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"DNF_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"DNF_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"DNF_AssertReset (reset_name, start_time, period)  Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"DNF_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to PCLK, DNFATCKI clock.");
            SC_REPORT_INFO(this->basename(),"DNF_GetCLKFreq (clock_name)                       Get clock frequency of PCLK, DNFATCKI clock.");
            SC_REPORT_INFO(this->basename(),"DNF_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"DNF_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"DNF_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"DNF_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"DNF_ListRegister                                  List all registers of the model.");
        } else {
            re_printf("error","The name (%s) of DNF_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of DNF_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void DNF::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mDNF_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mDNF_Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable info/warning/error message of register IF
void DNF::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: DNF_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","DNF_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: DNF_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///Enable/disable dumping message when users access registers
void DNF::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mDNF_Func->RegisterHandler(cmd);
}//}}}

///Handle PCLK signal
void DNF::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle DNFATCKI signal
void DNF::HandleDNFATCKISignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = DNFATCKI.read();
    SetCLKFreq("DNFATCKI", freq_value, "Hz");
}//}}}

///Handle PRESETZ signal
void DNF::HandlePRESETZSignalMethod (void)
{//{{{
    mPRESETZSignalCurrentValue = PRESETZ.read();
    if (mPRESETZSignalCurrentValue != mPRESETZSignalPreviousValue) {//mPRESETZSignalPreviousValue is "true" at initial state
        mPRESETZSignalPreviousValue = mPRESETZSignalCurrentValue;
        if (mPRESETZSignalCurrentValue == emResetActive) {
            mIsPRESETZHardActive = true;
            re_printf("info","The reset signal PRESETZ is asserted.\n");
            this->EnableReset("PRESETZ", mIsPRESETZHardActive);
        } else {
            if (mPCLKPeriod > 0) {
                double current_time = sc_time_stamp().to_double();
                mPRESETZHardEvent.notify(CalculateCLKPosEdge("PCLK", false, current_time), mTimeResolutionUnit);
            } else {
                mPRESETZHardEvent.notify(SC_ZERO_TIME);
            }
        }
        //Cancel AssertReset command when PRESETZ is active
        if (mIsPRESETZCmdReceive) {
            mPRESETZCmdEvent.cancel();
            mPRESETZCmdCancelEvent.cancel();
            mIsPRESETZCmdReceive = false;
            mIsPRESETZCmdActive = false;
        }
    }
}//}}}

///Handle DNFATRESZ signal
void DNF::HandleDNFATRESZSignalMethod (void)
{//{{{
    mDNFATRESZSignalCurrentValue = DNFATRESZ.read();
    if (mDNFATRESZSignalCurrentValue != mDNFATRESZSignalPreviousValue) {//mDNFATRESZSignalPreviousValue is "true" at initial state
        mDNFATRESZSignalPreviousValue = mDNFATRESZSignalCurrentValue;
        if (mDNFATRESZSignalCurrentValue == emResetActive) {
            mIsDNFATRESZHardActive = true;
            re_printf("info","The reset signal DNFATRESZ is asserted.\n");
            this->EnableReset("DNFATRESZ", mIsDNFATRESZHardActive);
        } else {
            //Notify event to synchronize
            if (mDNFATCKIPeriod > 0) {
                double current_time = sc_time_stamp().to_double();
                mDNFATRESZHardEvent.notify(CalculateCLKPosEdge("DNFATCKI", true, current_time), mTimeResolutionUnit);
            } else {
                mDNFATRESZHardEvent.notify(SC_ZERO_TIME);
            }
        }
        //Cancel AssertReset command when DNFATRESZ is active
        if (mIsDNFATRESZCmdReceive) {
            mDNFATRESZCmdEvent.cancel();
            mDNFATRESZCmdCancelEvent.cancel();
            mIsDNFATRESZCmdReceive = false;
            mIsDNFATRESZCmdActive = false;
        }
    }
}//}}}

///Process reset function when PRESETZ is active
void DNF::HandlePRESETZHardMethod (void)
{//{{{
    mIsPRESETZHardActive = false;
    this->EnableReset("PRESETZ", mIsPRESETZHardActive);
    re_printf("info","The reset signal PRESETZ is de-asserted.\n");
}//}}}

void DNF::HandleDNFATRESZHardMethod(void)
{//{{{
    mIsDNFATRESZHardActive = false;
    this->EnableReset("DNFATRESZ", mIsDNFATRESZHardActive);
    re_printf("info","The reset signal DNFATRESZ is de-asserted.\n");
}//}}}

///Process reset function when PRESETZ command is active
void DNF::HandlePRESETZCmdMethod (void)
{//{{{
    mIsPRESETZCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETZ.\n");
    this->EnableReset("PRESETZ", mIsPRESETZCmdActive);
    mPRESETZCmdCancelEvent.notify(mPRESETZCmdPeriod, SC_NS);
}//}}}

///Process reset function when DNFATRESZ command is active
void DNF::HandleDNFATRESZCmdMethod (void)
{//{{{
    mIsDNFATRESZCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of DNFATRESZ.\n");
    this->EnableReset("DNFATRESZ", mIsDNFATRESZCmdActive);
    mDNFATRESZCmdCancelEvent.notify(mDNFATRESZCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when PRESETZ command is active
void DNF::CancelPRESETZCmdMethod (void)
{//{{{
    mIsPRESETZCmdActive = false;
    mIsPRESETZCmdReceive = false;
    re_printf("info","Reset period of PRESETZ is over.\n");
    this->EnableReset("PRESETZ", mIsPRESETZCmdActive); 
}//}}}

///Cancel reset function when DNFATRESZ command is active
void DNF::CancelDNFATRESZCmdMethod (void)
{//{{{
    mIsDNFATRESZCmdActive = false;
    mIsDNFATRESZCmdReceive = false;
    re_printf("info","Reset period of DNFATRESZ is over.\n");
    this->EnableReset("DNFATRESZ", mIsDNFATRESZCmdActive); 
}//}}}

//Handle DNFATFIN input ports
void DNF::HandleDNFATFINMethod(const unsigned int channel_id)
{//{{{
    if (CheckBasicCondition(channel_id)) {
        bool DNFATFIN_val = DNFATFIN[channel_id]->read();
        SetEdgeDetection(channel_id, true, DNFATFIN_val);
    }
}//}}}

//Handle DNFATRIN input ports
void DNF::HandleDNFATRINMethod(const unsigned int channel_id)
{//{{{
    if (CheckBasicCondition(channel_id)) {
        bool DNFATRIN_val = DNFATRIN[channel_id]->read();
        SetEdgeDetection(channel_id, false, DNFATRIN_val);
    }
}//}}}

//Handle DNFATDGIN input ports (before synchronizing)
void DNF::HandleDNFATDGINMethod(const unsigned int channel_id)
{//{{{
    if (CheckBasicCondition(channel_id)) {
        if (mDNF_Func->IsChannelEnabled(channel_id)) {
            // Synchronize 2 DNFATCKI clock
            double current_time = sc_time_stamp().to_double();
            double waiting_period = (double)mDNFATCKIPeriod + CalculateCLKPosEdge("DNFATCKI", true, current_time);
            mSynchronizingEvent[channel_id].notify(waiting_period, mTimeResolutionUnit);
        }
    }
}//}}}

//Handle DNFATANIN input ports
void DNF::HandleDNFATANINMethod(const unsigned int channel_id)
{//{{{
    if (CheckBasicCondition(channel_id)) {
        if (!(mDNF_Func->IsChannelEnabled(channel_id))) { //Channel is disable
            bool ANIN_val = DNFATANIN[channel_id]->read();
            UpdateNFOUTValue(channel_id, ANIN_val);
            mWriteDNFATNFOUTEvent[channel_id].notify(SC_ZERO_TIME);
        }
    }
}//}}}

//Handle DNFATAED input ports
void DNF::HandleDNFATAEDMethod(const unsigned int channel_id)
{//{{{
    if (CheckBasicCondition(channel_id)) {
        if (!(mDNF_Func->IsChannelEnabled(channel_id))) {
            bool AED_val = DNFATAED[channel_id]->read();
            UpdateINTValue(channel_id, AED_val);
            mWriteDNFATINTEvent[channel_id].notify(SC_ZERO_TIME);
        }
    }
}//}}}

//Write to DNFATINT output ports
void DNF::WriteDNFATINTMethod(const unsigned int channel_id)
{//{{{
    DNFATINT[channel_id]->write(mINTValue[channel_id]);
}//}}}

//Clear DNFATINT output ports
void DNF::ClearDNFATINTMethod(const unsigned int channel_id)
{//{{{
    UpdateINTValue(channel_id, false);
    mWriteDNFATINTEvent[channel_id].notify(SC_ZERO_TIME);
}//}}}

//Write to DNFATNFOUT output ports
void DNF::WriteDNFATNFOUTMethod(const unsigned int channel_id)
{//{{{
    bool current_val = DNFATNFOUT[channel_id]->read();
    DNFATNFOUT[channel_id]->write(mNFOUTValue[channel_id]);
    if (mDNF_Func->IsChannelEnabled(channel_id)) {
        ControlINTport(channel_id, mNFOUTValue[channel_id], current_val);
    }
}//}}}

//Handle DNFATDGIN signal after synchronizing (x2 flipflops)
void DNF::HandleSynchronizedSignalMethod(const unsigned int channel_id)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mGetDGINSampleEvent[channel_id].notify(CalculateCLKPosEdge("SAMPLING_CLOCK", true, current_time), mTimeResolutionUnit);
}//}}}

//Get DNFATDGIN samples
void DNF::ProcessDGINMethod(const unsigned int channel_id)
{//{{{
    bool DGIN_val = DNFATDGIN[channel_id]->read();
    mDNF_Func->HandleInputSample(channel_id, DGIN_val);

}//}}}

//Check value of clocks and status of resets
bool DNF::CheckBasicCondition(const unsigned int channel_id)
{//{{{
    if (CheckClock("PCLK") && CheckClock("DNFATCKI")) {
        if (!GetResetStatus("DNFATRESZ")) {
            if (channel_id < mChannelNumber) {
                return true;
            }
        }
    }
    return false;
}//}}}

///Process reset function
void DNF::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mDNF_Func->Reset(reset_name, is_active);
    if (reset_name == "DNFATRESZ") {
        if (is_active) {//Initialize variables, output ports and cancel events
            StopOperation();
            for (unsigned int i=0; i<mChannelNumber; i++) {
                mEdgeDetection[i] = emNoEdge;
                UpdateNFOUTValue(i, false);
                UpdateINTValue(i, false);
                mWriteDNFATINTEvent[i].notify();
                mWriteDNFATNFOUTEvent[i].notify();
            }
        } else {
            StartOperation();
        }
    }
}//}}}

///Check frequency value and frequency unit of PCLK clock
void DNF::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                                  sc_dt::uint64 freq_in, std::string unit_in)
{//{{{
    if(unit_in == "GHz"){
        freq_out = freq_in * (sc_dt::uint64)(1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

///Calculate synchronous time with PCLK
double DNF::CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point)
{//{{{
    double period_num = 0;
    double sampling_period = CalculateSamplingClock();
    if (clock_name == "PCLK") {
        //mPCLKPeriod is check before enter this function
        period_num = (time_point - mStartPCLKTime) / (double)mPCLKPeriod;
    } else if (clock_name == "DNFATCKI") {
        //mDNFATCKIPeriod is check before enter this function
        period_num = (time_point - mStartDNFATCKITime) / (double)mDNFATCKIPeriod;
    } else {// sampling clock
        //sampling_period is never 0 because mDNFATCKIPeriod and GetClockPrescaler are not 0
        period_num = (time_point - mStartDNFATCKITime) / sampling_period;
    }
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (!add_period) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
            }
    } else {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "PCLK") {
        pos_edge_point = mStartPCLKTime  + (double)mod_period_num * (double)mPCLKPeriod - time_point;
    } else if (clock_name == "DNFATCKI") {//clock_name is DNFATCKI
        pos_edge_point = mStartDNFATCKITime + (double)mod_period_num * (double)mDNFATCKIPeriod - time_point;
    } else {
        pos_edge_point = mStartDNFATCKITime + (double)mod_period_num * sampling_period - time_point;
    }
    return pos_edge_point;
}//}}}

//Set edge detection based on value of FIN and RIN
void DNF::SetEdgeDetection(const unsigned int channel_id, const bool is_FIN, const bool value)
{//{{{
    if (is_FIN) {
        mEdgeDetection[channel_id] = (mEdgeDetection[channel_id] & 2) | (unsigned int)value;
    } else {
        mEdgeDetection[channel_id] = (mEdgeDetection[channel_id] & 1) | ((unsigned int)value << 1);
    }
}//}}}

//Calculate sampling clock based on DNFATCKI and prescaler
double DNF::CalculateSamplingClock(void)
{//{{{
    double sampling_clock = (double)mDNFATCKIPeriod * (double)(mDNF_Func->GetClockPrescaler());
    return sampling_clock;
}//}}}

//Update value of DNFATNFOUT before writing to DNFATNFOUT port
void DNF::UpdateNFOUTValue(const unsigned int channel_id, const bool value)
{//{{{
    mNFOUTValue[channel_id] = value;
}//}}}

//Update value of DNFATINT before writing to DNFATINT port
void DNF::UpdateINTValue(const unsigned int channel_id, const bool value)
{//{{{
    mINTValue[channel_id] = value;
}//}}}

//Control the operation of DNFATINT port
void DNF::ControlINTport(const unsigned int channel_id, const bool cur_NFOUT_val, const bool pre_val)
{//{{{
    bool is_output = true;
    if (pre_val != cur_NFOUT_val) {
        switch (mEdgeDetection[channel_id]) {
            case emBothEdge :
                UpdateINTValue(channel_id, true);
                break;
            case emFallingEdge :
                if (!cur_NFOUT_val) {
                    UpdateINTValue(channel_id, true);
                }
                break;
            case emRaisingEdge :
                if (cur_NFOUT_val) {
                    UpdateINTValue(channel_id, true);
                }
                break;
            default :
                is_output = false;
                break;
        }
    } else {
        is_output = false;
    }
    if (is_output) {
        mWriteDNFATINTEvent[channel_id].notify(SC_ZERO_TIME);
        mClearDNFATINTEvent[channel_id].notify((double)mDNFATCKIPeriod, mTimeResolutionUnit);
    }
}//}}}

//Start reading input ports
void DNF::StartOperation(void)
{//{{{
    if (mIsStartSimulation) {
        if (!mIsOperating) {
            mIsOperating = true;
            for (unsigned int i=0; i<mChannelNumber; i++) {
                mReadDNFATFINEvent[i].notify(SC_ZERO_TIME);
                mReadDNFATRINEvent[i].notify(SC_ZERO_TIME);
                mReadDNFATDGINEvent[i].notify(SC_ZERO_TIME);
                mReadDNFATANINEvent[i].notify(SC_ZERO_TIME);
                mReadDNFATAEDEvent[i].notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

//Stop reading input ports, writing to output ports
void DNF::StopOperation(void)
{//{{{
    if (mIsOperating) {
        mIsOperating = false;
        for (unsigned int i=0; i<mChannelNumber; i++) {
            mReadDNFATFINEvent[i].cancel();
            mReadDNFATRINEvent[i].cancel();
            mReadDNFATDGINEvent[i].cancel();
            mReadDNFATANINEvent[i].cancel();
            mReadDNFATAEDEvent[i].cancel();
            mWriteDNFATINTEvent[i].cancel();
            mClearDNFATINTEvent[i].cancel();
            mWriteDNFATNFOUTEvent[i].cancel();
            mSynchronizingEvent[i].cancel();
            mGetDGINSampleEvent[i].cancel();
            mGetNextDGINSampleEvent[i].cancel();
        }
    }
}//}}}

void DNF::start_of_simulation(void)
{//{{{
    mIsStartSimulation = true;
    if ((mPCLKPeriod != 0) && (mDNFATCKIPeriod != 0)) {
        StartOperation();
    }
}//}}}

///Check clock period
bool DNF::CheckClock(const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    if (clock_name == "PCLK") {
        clock_period = mPCLKPeriod;
    } else {
        clock_period = mDNFATCKIPeriod;
    }
    
    if (clock_period > 0) {
        clock_status = true;
    } else {
        re_printf("info","%s frequency is zero.\n",clock_name.c_str());
        clock_status = false;
    }
    return clock_status;
}//}}}

///Check reset status
bool DNF::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "DNFATRESZ") {
        if (mIsDNFATRESZHardActive || mIsDNFATRESZCmdActive) {
            reset_status = true;
        }
    } else { //PRESETZ
        if (mIsPRESETZHardActive || mIsPRESETZCmdActive) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

//Switch a DNF channel to operate in ENABLE mode
void DNF::SwitchToEnableMode(const unsigned int channel_id)
{//{{{
    if (CheckClock("PCLK") && CheckClock("DNFATCKI") && (mDNF_Func->IsChannelEnabled(channel_id))) {
        mReadDNFATDGINEvent[channel_id].notify(SC_ZERO_TIME);
    }
}//}}}

//Switch a DNF channel to operate in DISABLE mode
void DNF::SwitchToDisableMode(const unsigned int channel_id)
{//{{{
    mSynchronizingEvent[channel_id].cancel();
    mGetNextDGINSampleEvent[channel_id].cancel();
    mGetDGINSampleEvent[channel_id].cancel();
    mWriteDNFATNFOUTEvent[channel_id].cancel();
    mWriteDNFATINTEvent[channel_id].cancel();
    mReadDNFATANINEvent[channel_id].notify();
    mReadDNFATAEDEvent[channel_id].notify();
}//}}}

//Control writing to DNFATNFOUT port
void DNF::OutputDNFATNFOUT(const unsigned int channel_id, const bool value)
{//{{{
    UpdateNFOUTValue(channel_id, value);
    mWriteDNFATNFOUTEvent[channel_id].notify(SC_ZERO_TIME);
}//}}}

//Get next sample of DNFATDGIN input port
void DNF::GetNextSample(const unsigned int channel_id)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mGetNextDGINSampleEvent[channel_id].notify(SC_ZERO_TIME);
}//}}}

// vim600: set foldmethod=marker :
