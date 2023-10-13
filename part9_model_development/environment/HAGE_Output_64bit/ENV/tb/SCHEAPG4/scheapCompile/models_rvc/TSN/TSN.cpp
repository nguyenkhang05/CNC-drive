// ---------------------------------------------------------------------
// $Id: TSN.cpp,v 1.1 2014/12/10 01:14:38 binhnguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "TSN.h"
#include "TSN_Func.h" 

///Constructor of TSN class
TSN::TSN (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency):
    sc_module(name),
    TSN_AgentController(),
    BusSlaveBase<32,1>(),
    PCLK("PCLK"),
    ADCLK("ADCLK"),
    presetz("presetz"),
    ADRESZ("ADRESZ"),
    // Input ports
    CHSLC("CHSLC"),
    INT_ADE("INT_ADE"),
    // Output ports
    TSOUT("TSOUT"),
    TSNERR("TSNERR")
{//{{{
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&presetz);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mTSN_Func = new TSN_Func((std::string)name, this);
    sc_assert(mTSN_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mTSN_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(1);

    //Initialize output signals
    TSOUT.initialize(0);
    TSNERR.initialize(false);

    //Initialize variables
    mPCLKPeriod = 0;
    mADCLKPeriod = 0;
    mPCLKFreq = 0;
    mADCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mADCLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz"; 
    mADCLKFreqUnit = "Hz"; 
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }

    mTempValue = 0;
    mErrMaskCount = 14;
    mStartupCount = 7984;
    mDumpInterrupt = false;
    
    mTSNERR = false;
    mTSOUT = 0;
    mState = emDISABLE;
    mEnableSelfDiag = false;

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleADCLKSignalMethod);
    dont_initialize();
    sensitive << ADCLK;

    SC_METHOD(HandlePresetzSignalMethod);
    sensitive << presetz;

    SC_METHOD(HandleADRESZSignalMethod);
    sensitive << ADRESZ;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&TSN::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&TSN::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&TSN::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(HandleCHSLCInputMethod);
    dont_initialize();
    sensitive << CHSLC;
    
    SC_METHOD(HandleCHSLCChangeMethod);
    dont_initialize();
    sensitive << mHandleCHSLCChangeEvent;

    SC_METHOD(HandleINT_ADEInputMethod);
    dont_initialize();
    sensitive << INT_ADE;
    
    SC_METHOD(HandleINT_ADEChangeMethod);
    dont_initialize();
    sensitive << mHandleINT_ADEChangeEvent;

    SC_METHOD(UpdateTSOUTOutputMethod);
    dont_initialize();
    sensitive << mUpdateTSOUTOutputEvent;
    
    SC_METHOD(WriteTSOUTOutputMethod);
    dont_initialize();
    sensitive << mWriteTSOUTOutputEvent;

    SC_METHOD(WriteTSNERRInterruptMethod);
    dont_initialize();
    sensitive << mWriteTSNERRInterruptEvent;

    SC_METHOD(AssertTSNERRMethod);
    dont_initialize();
    sensitive << mAssertTSNERREvent;
    
    SC_METHOD(DeassertTSNERRMethod);
    dont_initialize();
    sensitive << mDeassertTSNERREvent;
    
    SC_METHOD(FinishInitializeMethod);
    dont_initialize();
    sensitive << mFinishInitializeEvent;
    
    SC_METHOD(FinishDischargeMethod);
    dont_initialize();
    sensitive << mFinishDischargeEvent;
    
    SC_METHOD(FinishConvDoneMethod);
    dont_initialize();
    sensitive << mFinishConvDoneEvent;
}//}}}

///Destructor of TSN class
TSN::~TSN (void)
{//{{{
    delete mTSN_Func;
}//}}}

///Assert reset by software for presetz and ADRESZ
void TSN::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetz") {
        if ((!mIsResetHardActive[0])&&(!mIsResetCmdReceive[0])) {
            mIsResetCmdReceive[0] = true;
            re_printf("info","The model will be reset (presetz) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[0].notify(start_time, SC_NS);
            mResetCmdPeriod[0] = period;
        } else {
            re_printf("warning","The software reset of presetz is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "ADRESZ") {
        if ((!mIsResetHardActive[1])&&(!mIsResetCmdReceive[1])) {
            mIsResetCmdReceive[1] = true;
            re_printf("info","The model will be reset (ADRESZ) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[1].notify(start_time, SC_NS);
            mResetCmdPeriod[1] = period;
        } else {
            re_printf("warning","The software reset of ADRESZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be presetz or ADRESZ\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit for PCLK and ADCLK
void TSN::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKPeriod = 0;
            }
        } else if (word_vector[0] == "ADCLK") {
            mADCLKOrgFreq = freq;
            ConvertClockFreq (mADCLKFreq, mADCLKFreqUnit, freq, unit);
            if (mADCLKFreq > 0) {
                mADCLKPeriod = (sc_dt::uint64)(((1/(double)mADCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                // If TSNCR.TSNEN = 1 -> Enable TSN immediately
                if ((mTSN_Func->IsTSNEnable()) && (mState == emDISABLE)) {
                    EnableSetting(true);
                }
            } else {
                mADCLKPeriod = 0;
                //Cancel events
                CancelEvents();
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Get clock value of PCLK and ADCLK
void TSN::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "ADCLK") {
            re_printf("info","ADCLK frequency is %0.0f %s\n", (double)mADCLKOrgFreq, mADCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void TSN::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mTSN_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void TSN::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mTSN_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void TSN::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mTSN_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void TSN::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mTSN_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///List all reigsters name
void TSN::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mTSN_Func->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void TSN::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"TSN_MessageLevel (fatal|error|warning|info)       Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"TSN_DumpRegisterRW (true/false)                   Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"TSN_DumpInterrupt (true/false)                    Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(),"TSN_ErrmaskCount (value)                          Set wait time for TSN to receive conversion error signal from ADC (with a number of ADCLK cycle) (Default: 14)");
            SC_REPORT_INFO(this->basename(),"TSN_StartupCount (value)                          Set start-up time for TSN to change to ready operation after enabled (with a number of ADCLK cycle) (Default: 7984)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"TSN_AssertReset (reset_name, start_time, period)  Assert and de-assert reset signal to the TSN model");
            SC_REPORT_INFO(this->basename(),"TSN_SetCLKFreq (clock_name, freq, unit)           Set clock frequency value to TSN model. Valid clock_name is PCLK and ADCLK");
            SC_REPORT_INFO(this->basename(),"TSN_GetCLKFreq (clock_name)                       Get clock frequency value of TSN model. Valid clock_name is PCLK and ADCLK");
            SC_REPORT_INFO(this->basename(),"TSN_ForceRegister (reg_name, reg_value)           Force register with setting value");
            SC_REPORT_INFO(this->basename(),"TSN_ReleaseRegister (reg_name)                    Release register from force value");
            SC_REPORT_INFO(this->basename(),"TSN_WriteRegister (reg_name, reg_value)           Write a value to a register");
            SC_REPORT_INFO(this->basename(),"TSN_ReadRegister (reg_name)                       Read a value from a register");
            SC_REPORT_INFO(this->basename(),"TSN_ListRegister ()                               Dump name of TSN's registers");
            SC_REPORT_INFO(this->basename(),"TSN_SetTemperature (value)                        Set the temperature value for TSN model");
            SC_REPORT_INFO(this->basename(),"TSN_DumpStatusInfo ()                             Dump the status information of the TSN model");
        } else {
            re_printf("warning","The name (%s) of TSN_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of TSN_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void TSN::SetMessageLevel (const std::string msg_lv)
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
    mTSN_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mTSN_Func->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void TSN::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mTSN_Func->RegisterHandler(cmd);
}//}}}

///Enable/disable dump interrupt information
void TSN::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: TSN_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","TSN_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: TSN_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Dump information of registers
void TSN::DumpStatusInfo (void)
{//{{{
    mTSN_Func->DumpStatusInfo(mState);
}//}}}

///Set temperature value
void TSN::SetTemperature (const double value)
{//{{{
    mTempValue = value;
    re_printf("info","TSN_SetTemperature: %f\n", value);
}//}}}

///Set wait time for TSN to receive conversion error signal from ADC
void TSN::SetErrmaskCount (const unsigned int value)
{//{{{
    mErrMaskCount = value;
    re_printf("info","TSN_ErrmaskCount: %u\n", value);
}//}}}

///Set start-up time for TSN to change to ready operation after enabled
void TSN::SetStartupCount (const unsigned int value)
{//{{{
    mStartupCount = value;
    re_printf("info","TSN_StartupCount: %u\n", value);
}//}}}

///Handle PCLK signal
void TSN::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle ADCLK signal
void TSN::HandleADCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = ADCLK.read();
    SetCLKFreq("ADCLK", freq_value, "Hz");
}//}}}

///Handle presetz signal
void TSN::HandlePresetzSignalMethod (void)
{//{{{
    mResetCurVal[0] = presetz.read();
    if (mResetCurVal[0] != mResetPreVal[0]) {       //mResetPreVal is "true" at initial state
        mResetPreVal[0] = mResetCurVal[0];
        if (CheckClockPeriod("PCLK")) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent[0].notify(CalculateCLKPosEdge("PCLK", current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent[0].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Handle ADRESZ signal
void TSN::HandleADRESZSignalMethod (void)
{//{{{
    mResetCurVal[1] = ADRESZ.read();
    if (mResetCurVal[1] != mResetPreVal[1]) {   //mResetPreVal is "true" at initial state
        mResetPreVal[1] = mResetCurVal[1];
        if (CheckClockPeriod("ADCLK")) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent[1].notify(CalculateCLKPosEdge("ADCLK", current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent[1].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when presetz/ADRESZ is active
void TSN::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset port presetz is asserted.\n");
            this->EnableReset("presetz", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port ADRESZ is asserted.\n");
            this->EnableReset("ADRESZ", mIsResetHardActive[reset_id]);
        }
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            if (reset_id == 0) {
                re_printf("info","The reset port presetz is de-asserted.\n");
                this->EnableReset("presetz", mIsResetHardActive[reset_id]);
            } else {
                re_printf("info","The reset port ADRESZ is de-asserted.\n");
                this->EnableReset("ADRESZ", mIsResetHardActive[reset_id]);
            }
        }
    }
    //Cancel AssertReset command when presetz is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

///Process reset function when presetz/ADRESZ command is active
void TSN::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "presetz";
    if (reset_id == 0) {
        reset_name = "presetz";
    } else { //reset_id == 1
        reset_name = "ADRESZ";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when presetz/ADRESZ command is active
void TSN::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "presetz";
    if (reset_id == 0) {
        reset_name = "presetz";
    } else { //reset_id == 1
        reset_name = "ADRESZ";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

///Execute reset operation
void TSN::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if (is_active) {
        if (GetResetStatus("ADRESZ") || GetResetStatus("presetz")) {
            mTSN_Func->Reset(is_active);
            mState = emRESET;
            //Cancel events
            CancelEvents();
            //Initialize variables
            mTSNERR = false;
            mTSOUT = 0;
            mEnableSelfDiag = false;
            //Initialize output signals
            mWriteTSOUTOutputEvent.notify(SC_ZERO_TIME);
            mWriteTSNERRInterruptEvent.notify(SC_ZERO_TIME);
        }
    } else {
        if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
            mTSN_Func->Reset(is_active);
            mState = emDISABLE;
        }
    }
}//}}}

/// Finish Initialize phase -> change to READY state
void TSN::FinishInitializeMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            if (mState == emENABLE) {
                re_printf("info","TSN is ready to measure temperature.\n");
                mState = emREADY;
                mTSN_Func->SetStatusFlag(true);
            }
        }
    }
}//}}}

/// Detect the changing of CHSLC port
void TSN::HandleCHSLCInputMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            double current_time = sc_time_stamp().to_double();
            mHandleCHSLCChangeEvent.notify(CalculateCLKPosEdge("ADCLK", current_time), mTimeResolutionUnit);
        }
    }
}//}}}

/// Process the changing of CHSLC port
void TSN::HandleCHSLCChangeMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            bool value = CHSLC.read();
            double current_time = sc_time_stamp().to_double();
            if (value) {
                if (mState == emREADY) {
                    // Change to DISCHARGE state
                    re_printf("info","Temperature measure request is accepted\n");
                    mState = emDISCHARGE;
                    // Wait to change to CONVERSION state
                    mFinishDischargeEvent.notify(6*mADCLKPeriod, mTimeResolutionUnit);
                }
            } else {
                if (mState == emCONVERSION) {
                    // Change to CONV_DONE state
                    mState = emCONV_DONE;
                    // Stop output temperature value
                    mTSOUT = 0;
                    mUpdateTSOUTOutputEvent.notify(SC_ZERO_TIME);
                    // Wait to change to READY state
                    double current_time = sc_time_stamp().to_double();
                    mFinishConvDoneEvent.notify(mErrMaskCount*mADCLKPeriod, mTimeResolutionUnit);
                } else if (mState == emDISCHARGE) {
                    // Return to READY state
                    mState = emREADY;
                    // Cancel discharge
                    mFinishDischargeEvent.cancel();
                }
            }
        }
    }
}//}}}

/// Detect the changing of INT_ADE port
void TSN::HandleINT_ADEInputMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            if (INT_ADE.read()) {
                double current_time = sc_time_stamp().to_double();
                mHandleINT_ADEChangeEvent.notify(CalculateCLKPosEdge("PCLK", current_time), mTimeResolutionUnit);
            }
        }
    }
}//}}}

/// Process the changing of INT_ADE port
void TSN::HandleINT_ADEChangeMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            if (mState == emCONV_DONE) {
                if (!mTSNERR) {
                    // Assert TSNERR interrupt
                    mAssertTSNERREvent.notify(SC_ZERO_TIME);
                }
            }
        }
    }
}//}}}

/// Finish Discharging -> prepare temperature value for TSOUT
void TSN::FinishDischargeMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            if (mState == emDISCHARGE) {
                mState = emCONVERSION;
                if (mEnableSelfDiag) {
                    mTSOUT = 0;
                } else {
                    mTSOUT = mTempValue;
                }
                mUpdateTSOUTOutputEvent.notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

/// Finish CONV_DONE -> TSN change to READY state
void TSN::FinishConvDoneMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            if (mState == emCONV_DONE) {
                re_printf("info","CONV_DONE state is over, change to READY state.\n");
                mState = emREADY;
            }
        }
    }
}//}}}

/// Assert TSNERR interrupt
void TSN::AssertTSNERRMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            mTSNERR = true;
            if (mDumpInterrupt) {
                re_printf("info","INT [TSNERR] Assert.\n");
            }
            mWriteTSNERRInterruptEvent.notify(SC_ZERO_TIME);
            mDeassertTSNERREvent.notify(mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

/// De-assert TSNERR interrupt
void TSN::DeassertTSNERRMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            mTSNERR = false;
            if (mDumpInterrupt) {
                re_printf("info","INT [TSNERR] De-assert.\n");
            }
            mWriteTSNERRInterruptEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

/// Update temperature value of TSOUT port
void TSN::UpdateTSOUTOutputMethod (void)
{//{{{
    if ((!GetResetStatus("ADRESZ")) && (!GetResetStatus("presetz"))) {
        if (CheckClockPeriod("ADCLK")) {
            mWriteTSOUTOutputEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

/// Output temperature value to TSOUT port
void TSN::WriteTSOUTOutputMethod (void)
{//{{{
    TSOUT.write(mTSOUT);
}//}}}

/// Write to TSNERR interrupt port
void TSN::WriteTSNERRInterruptMethod (void)
{//{{{
    TSNERR.write(mTSNERR);
}//}}}

/// Enable/Disable TSN
void TSN::EnableSetting (const bool is_enable)
{//{{{
    if (is_enable) {
        re_printf("info","TSN is enabled.\n");
        mState = emENABLE;
        // Wait to change to READY state
        double current_time = sc_time_stamp().to_double();
        mFinishInitializeEvent.notify(CalculateCLKPosEdge("ADCLK", current_time) + mStartupCount*mADCLKPeriod, mTimeResolutionUnit);
    } else {
        re_printf("info","TSN is disabled.\n");
        mState = emDISABLE;
        //Cancel events
        CancelEvents();
        //Initialize variables
        mTSNERR = false;
        mTSOUT = 0;
        //Initialize output signals
        mWriteTSOUTOutputEvent.notify(SC_ZERO_TIME);
        mWriteTSNERRInterruptEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/// Enable/Disable Self-diagnosis function
void TSN::SelfDiagSetting (const bool is_enable)
{//{{{
    mEnableSelfDiag = is_enable;
}//}}}

/// Get current state of TSN
TSN_AgentController::eState TSN::GetState (void)
{//{{{
    return mState;
}//}}}

/// Cancel operation events
void TSN::CancelEvents (void)
{//{{{
    mUpdateTSOUTOutputEvent.cancel();
    mWriteTSOUTOutputEvent.cancel();
    mWriteTSNERRInterruptEvent.cancel();
    mAssertTSNERREvent.cancel();
    mDeassertTSNERREvent.cancel();
    mFinishInitializeEvent.cancel();
    mFinishDischargeEvent.cancel();
    mFinishConvDoneEvent.cancel();
    mHandleCHSLCChangeEvent.cancel();
    mHandleINT_ADEChangeEvent.cancel();
}//}}}

///Check frequency value and frequency unit of PCLK clock
void TSN::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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

///Separate some words from a string to store a vector
void TSN::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

///Get time resolution
void TSN::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
  if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
      resolution_value = 1000000000000000LL;
      resolution_unit = SC_FS;
  } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
      resolution_value = 1000000000000LL;
      resolution_unit = SC_PS;
  } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
      resolution_value = 1000000000;
      resolution_unit = SC_NS;
  } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
      resolution_value = 1000000;
      resolution_unit = SC_US;
  } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
      resolution_value = 1000;
      resolution_unit = SC_MS;
  } else {
      resolution_value = 1;
      resolution_unit = SC_SEC;
  }
}//}}}

///Calculate synchronous time with PCLK
double TSN::CalculateCLKPosEdge (const std::string clock_name, const double time_point)
{//{{{
    double period_num = 0;
    if (clock_name == "PCLK") {
        period_num = time_point / (double)mPCLKPeriod;
    } else {//clock_name is ADCLK
        period_num = time_point / (double)mADCLKPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "PCLK") {
        pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - time_point;
    } else {//clock_name is ADCLK
        pos_edge_point = (double)mod_period_num * (double)mADCLKPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Check reset status
bool TSN::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "ADRESZ") {
        if (mIsResetHardActive[1] || mIsResetCmdActive[1]) {
            reset_status = true;
        }
    } else {        // presetz
        if (mIsResetHardActive[0] || mIsResetCmdActive[0]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///Check period value of PCLK, ADCLK clock
bool TSN::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "PCLK")?mPCLKPeriod:mADCLKPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

// vim600: set foldmethod=marker :
