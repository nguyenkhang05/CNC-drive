// ---------------------------------------------------------------------
// $Id: FCLA.cpp,v 1.5 2014/10/20 06:20:51 dungvannguyen Exp $
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
#include "FCLA.h"
#include "FCLA_Func.h" 

///Constructor of FCLA class
FCLA::FCLA (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency):
    sc_module(name),
    FCLA_AgentController(),
    BusSlaveBase<32,1>(),
    PCLK("PCLK"),
    SMPCLK("SMPCLK"),
    PRESETZ("PRESETZ"),
    SMPRESZ("SMPRESZ")
{//{{{
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETZ);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mFCLA_Func = new FCLA_Func((std::string)name, this);
    sc_assert(mFCLA_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mFCLA_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(1);

    std::ostringstream port_name;
    for (unsigned int index = 0; index < emFilterNum; index++) {
        port_name.str("");
        port_name << "FCLATSI" << index;
        FCLATSI[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(FCLATSI[index] != NULL);

        port_name.str("");
        port_name << "FCLATNFSI" << index;
        FCLATNFSI[index] = new sc_in<bool>(port_name.str().c_str());

        port_name.str("");
        port_name << "FCLATEDSI" << index;
        FCLATEDSI[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(FCLATEDSI[index] != NULL);

        port_name.str("");
        port_name << "FCLATINTR" << index;
        FCLATINTR[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(FCLATINTR[index] != NULL);

        port_name.str("");
        port_name << "FCLATINTF" << index;
        FCLATINTF[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(FCLATINTF[index] != NULL);

        port_name.str("");
        port_name << "FCLATSO" << index;
        FCLATSO[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(FCLATSO[index] != NULL);

        port_name.str("");
        port_name << "FCLATEDSO" << index;
        FCLATEDSO[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(FCLATEDSO[index] != NULL);

        //Initialize output signals
        FCLATINTR[index]->initialize(false);
        FCLATINTF[index]->initialize(false);
        FCLATSO[index]->initialize(false);
        FCLATEDSO[index]->initialize(false);
    }

    //Initialize variables
    mPCLKPeriod = 0;
    mSMPCLKPeriod = 0;
    mPCLKFreq = 0;
    mSMPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mSMPCLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz"; 
    mSMPCLKFreqUnit = "Hz"; 
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mPRESETZSignalCurrentValue = true;
    mPRESETZSignalPreviousValue = true;
    mIsPRESETZHardActive = false;
    mIsPRESETZCmdReceive = false;
    mIsPRESETZCmdActive = false;
    mPRESETZCmdPeriod =  0;

    mSMPRESZSignalCurrentValue = true;
    mSMPRESZSignalPreviousValue = true;
    mIsSMPRESZHardActive = false;
    mIsSMPRESZCmdReceive = false;
    mIsSMPRESZCmdActive = false;
    mSMPRESZCmdPeriod =  0; 

    for (unsigned char index = 0; index < emFilterNum; index++) {
        mFCLATINTRValue[index] = false;
        mFCLATINTFValue[index] = false;
        mFCLATSOValue[index] = false;
        mFCLATEDSOValue[index] = false;
    }

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive<<PCLK;
 
    SC_METHOD(HandleSMPCLKSignalMethod);
    dont_initialize();
    sensitive<<SMPCLK;
 
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

    SC_METHOD(HandleSMPRESZSignalMethod);
    sensitive<<SMPRESZ;
    
    SC_METHOD(HandleSMPRESZHardMethod);
    dont_initialize();
    sensitive<<mSMPRESZHardEvent;

    SC_METHOD(HandleSMPRESZCmdMethod);
    dont_initialize();
    sensitive<<mSMPRESZCmdEvent;
 
    SC_METHOD(CancelSMPRESZCmdMethod);
    dont_initialize();
    sensitive<<mSMPRESZCmdCancelEvent;

    for (unsigned char index = 0; index  < emFilterNum; index ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(FCLATSI[index]);
        opt.set_sensitivity(&mFilterBypassEnableEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&FCLA::DetectFCLATSIPortsMethod, this, index),
                          sc_core::sc_gen_unique_name("DetectFCLATSIPortsMethod"), &opt);
    }

    for (unsigned char index = 0; index  < emFilterNum; index ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(FCLATNFSI[index]);
        opt.set_sensitivity(&mFilterBypassDisableEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&FCLA::DetectFCLATNFSIPortsMethod, this, index),
                          sc_core::sc_gen_unique_name("DetectFCLATNFSIPortsMethod"), &opt);
    }

    for (unsigned char index = 0; index  < emFilterNum; index ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(FCLATEDSI[index]);
        opt.set_sensitivity(&mLevelDetectorDisableEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&FCLA::DetectFCLATEDSIPortsMethod, this, index),
                          sc_core::sc_gen_unique_name("DetectFCLATEDSIPortsMethod"), &opt);
    }

    for (unsigned char index = 0; index  < emFilterNum; index ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mEdgeSelectPortsWriteEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&FCLA::WriteEdgeSelectPortsMethod, this, index),
                          sc_core::sc_gen_unique_name("WriteEdgeSelectPortsMethod"), &opt);
    }

    for (unsigned char index = 0; index  < emFilterNum; index ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFCLATSOPortsWriteEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&FCLA::WriteFCLATSOPortsMethod, this, index),
                          sc_core::sc_gen_unique_name("WriteFCLATSOPortsMethod"), &opt);
    }

    for (unsigned char index = 0; index  < emFilterNum; index ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFCLATEDSOPortsWriteEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&FCLA::WriteFCLATEDSOPortsMethod, this, index),
                          sc_core::sc_gen_unique_name("WriteFCLATEDSOPortsMethod"), &opt);
    }

    for (unsigned char index = 0; index  < emFilterNum; index ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFCLATNFSIPortValueCheckAtSyncTimeEvent[index]);
        opt.set_sensitivity(&mLevelDetectorEnableEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&FCLA::CheckFCLATNFSIPortValueAtSyncTimeMethod, this, index),
                          sc_core::sc_gen_unique_name("CheckFCLATNFSIPortValueAtSyncTimeMethod"), &opt);
    }

    for (unsigned char index = 0; index  < emFilterNum; index ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFCLATEDSOPortActivateEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&FCLA::ActivateFCLATEDSOPortMethod, this, index),
                          sc_core::sc_gen_unique_name("ActivateFCLATEDSOPortMethod"), &opt);
    }

    for (unsigned char index = 0; index  < emFilterNum; index ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFCLATEDSOPortDeactivateEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&FCLA::DeactivateFCLATEDSOPortMethod, this, index),
                          sc_core::sc_gen_unique_name("DeactivateFCLATEDSOPortMethod"), &opt);
    }
}//}}}

///Destructor of FCLA class
FCLA::~FCLA (void)
{//{{{
    delete mFCLA_Func;
    for (unsigned char index = 0; index < emFilterNum; index++) {
        delete FCLATSI[index];
        delete FCLATNFSI[index];
        delete FCLATEDSI[index];
        delete FCLATINTR[index];
        delete FCLATINTF[index];
        delete FCLATSO[index];
        delete FCLATEDSO[index];
    }
}//}}}

///Assert reset by software for PRESETZ and SMPRESZ
void FCLA::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETZ") {
        if ((!mIsPRESETZHardActive)&&(!mIsPRESETZCmdReceive)) {
            mIsPRESETZCmdReceive = true;
            re_printf("info","The model will be reset (PRESETZ) for %f ns after %f ns\n", period, start_time);
            mPRESETZCmdEvent.notify(start_time, SC_NS);
            mPRESETZCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "SMPRESZ") {
        if ((!mIsSMPRESZHardActive)&&(!mIsSMPRESZCmdReceive)) {
            mIsSMPRESZCmdReceive = true;
            re_printf("info","The model will be reset (SMPRESZ) for %f ns after %f ns.\n", period, start_time);
            mSMPRESZCmdEvent.notify(start_time, SC_NS);
            mSMPRESZCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of SMPRESZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error","The reset name (%s) is wrong. It should be PRESETZ or SMPRESZ\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit for PCLK and SMPCLK
void FCLA::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
        } else if (word_vector[0] == "SMPCLK") {
            mSMPCLKOrgFreq = freq;
            ConvertClockFreq (mSMPCLKFreq, mSMPCLKFreqUnit, freq, unit);
            if (mSMPCLKFreq > 0) {
                mSMPCLKPeriod = (sc_dt::uint64)(((1/(double)mSMPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mSMPCLKPeriod = 0;
                //Cancel events
                for (unsigned char index = 0; index < emFilterNum; index++) {
                    mEdgeSelectPortsWriteEvent[index].cancel();
                    mFCLATSOPortsWriteEvent[index].cancel();
                    mFCLATEDSOPortsWriteEvent[index].cancel();
                    mFCLATNFSIPortValueCheckAtSyncTimeEvent[index].cancel();
                    mFCLATEDSOPortActivateEvent[index].cancel();
                    mFCLATEDSOPortDeactivateEvent[index].cancel();
                    mFilterBypassEnableEvent[index].cancel();
                    mFilterBypassDisableEvent[index].cancel();
                    mLevelDetectorEnableEvent[index].cancel();
                    mLevelDetectorDisableEvent[index].cancel();
                }
            }
        } else {
            re_printf("error","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Get clock value of PCLK and SMPCLK
void FCLA::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "SMPCLK") {
            re_printf("info","SMPCLK frequency is %0.0f %s\n", (double)mSMPCLKOrgFreq, mSMPCLKFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void FCLA::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mFCLA_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void FCLA::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mFCLA_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void FCLA::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mFCLA_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void FCLA::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mFCLA_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///List all reigsters name
void FCLA::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mFCLA_Func->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void FCLA::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"FCLA_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"FCLA_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"FCLA_AssertReset (reset_name, start_time, period)  Assert/De-assert PRESETZ, SMPRESZ signal");
            SC_REPORT_INFO(this->basename(),"FCLA_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to PCLK, SMPCLK clock");
            SC_REPORT_INFO(this->basename(),"FCLA_GetCLKFreq (clock_name)                       Get clock frequency of PCLK, SMPCLK clock");
            SC_REPORT_INFO(this->basename(),"FCLA_ForceRegister (reg_name, reg_value)           Force a value to a register");
            SC_REPORT_INFO(this->basename(),"FCLA_ReleaseRegister (reg_name)                    Release forced value of a register");
            SC_REPORT_INFO(this->basename(),"FCLA_WriteRegister (reg_name, reg_value)           Write a value to a register");
            SC_REPORT_INFO(this->basename(),"FCLA_ReadRegister (reg_name)                       Read a value from a register");
            SC_REPORT_INFO(this->basename(),"FCLA_ListRegister                                  List all registers of the model");
        } else {
            re_printf("error","The name (%s) of FCLA_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of FCLA_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void FCLA::SetMessageLevel (const std::string msg_lv)
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
    mFCLA_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mFCLA_Func->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void FCLA::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mFCLA_Func->RegisterHandler(cmd);
}//}}}

///Handle PCLK signal
void FCLA::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle SMPCLK signal
void FCLA::HandleSMPCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = SMPCLK.read();
    SetCLKFreq("SMPCLK", freq_value, "Hz");
}//}}}

///Handle PRESETZ signal
void FCLA::HandlePRESETZSignalMethod (void)
{//{{{
    mPRESETZSignalCurrentValue = PRESETZ.read();
    if (mPRESETZSignalCurrentValue != mPRESETZSignalPreviousValue) {//mPRESETZSignalPreviousValue is "true" at initial state
        mPRESETZSignalPreviousValue = mPRESETZSignalCurrentValue;
        mPRESETZHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Handle SMPRESZ signal
void FCLA::HandleSMPRESZSignalMethod (void)
{//{{{
    mSMPRESZSignalCurrentValue = SMPRESZ.read();
    if (mSMPRESZSignalCurrentValue != mSMPRESZSignalPreviousValue) {//mSMPRESZSignalPreviousValue is "false" at initial state
        mSMPRESZSignalPreviousValue = mSMPRESZSignalCurrentValue;
        mSMPRESZHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when PRESETZ is active
void FCLA::HandlePRESETZHardMethod (void)
{//{{{
    if (mPRESETZSignalCurrentValue == emPRESETZActive) {
        mIsPRESETZHardActive = true;
        re_printf("info","The reset signal PRESETZ is asserted\n");
        this->EnableReset("PRESETZ", mIsPRESETZHardActive);
    } else if (mIsPRESETZHardActive) {
        mIsPRESETZHardActive = false;
        this->EnableReset("PRESETZ", mIsPRESETZHardActive);
        re_printf("info","The reset signal PRESETZ is de-asserted\n");
    }
    //Cancel AssertReset command when PRESETZ is active
    if (mIsPRESETZCmdReceive) {
        mPRESETZCmdEvent.cancel();
        mPRESETZCmdCancelEvent.cancel();
        mIsPRESETZCmdReceive = false;
        mIsPRESETZCmdActive = false;
    }
}//}}}

///Process reset function when SMPRESZ is active
void FCLA::HandleSMPRESZHardMethod (void)
{//{{{
    if (mSMPRESZSignalCurrentValue == emSMPRESZActive) {
        mIsSMPRESZHardActive = true;
        re_printf("info","The reset signal SMPRESZ is asserted\n");
        this->EnableReset("SMPRESZ", mIsSMPRESZHardActive);
    } else if (mIsSMPRESZHardActive) {
        mIsSMPRESZHardActive = false;
        this->EnableReset("SMPRESZ", mIsSMPRESZHardActive);
        re_printf("info","The reset signal SMPRESZ is de-asserted\n");
    }
    //Cancel AssertReset command when SMPRESZ is active
    if (mIsSMPRESZCmdReceive) {
        mSMPRESZCmdEvent.cancel();
        mSMPRESZCmdCancelEvent.cancel();
        mIsSMPRESZCmdReceive = false;
        mIsSMPRESZCmdActive = false;
    }
}//}}}

///Process reset function when PRESETZ command is active
void FCLA::HandlePRESETZCmdMethod (void)
{//{{{
    mIsPRESETZCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETZ\n");
    this->EnableReset("PRESETZ", mIsPRESETZCmdActive);
    mPRESETZCmdCancelEvent.notify(mPRESETZCmdPeriod, SC_NS);
}//}}}

///Process reset function when SMPRESZ command is active
void FCLA::HandleSMPRESZCmdMethod (void)
{//{{{
    mIsSMPRESZCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of SMPRESZ\n");
    this->EnableReset("SMPRESZ", mIsSMPRESZCmdActive);
    mSMPRESZCmdCancelEvent.notify(mSMPRESZCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when PRESETZ command is active
void FCLA::CancelPRESETZCmdMethod (void)
{//{{{
    mIsPRESETZCmdActive = false;
    mIsPRESETZCmdReceive = false;
    re_printf("info","Reset period of PRESETZ is over\n");
    this->EnableReset("PRESETZ", mIsPRESETZCmdActive); 
}//}}}

///Cancel reset function when SMPRESZ command is active
void FCLA::CancelSMPRESZCmdMethod (void)
{//{{{
    mIsSMPRESZCmdActive = false;
    mIsSMPRESZCmdReceive = false;
    re_printf("info","Reset period of SMPRESZ is over\n");
    this->EnableReset("SMPRESZ", mIsSMPRESZCmdActive); 
}//}}}

///Detect the changing of FCLATEDSI ports
void FCLA::DetectFCLATEDSIPortsMethod (const unsigned int filter_no)
{//{{{
    if (!GetResetStatus("SMPRESZ")&&CheckClockPeriod("SMPCLK")) {
        if (!(mFCLA_Func->IsLevelDetectorEnable(filter_no))) {
            mFCLATEDSOValue[filter_no] = FCLATEDSI[filter_no]->read();
            mFCLATEDSOPortsWriteEvent[filter_no].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Detect the changing of FCLATSI ports
void FCLA::DetectFCLATSIPortsMethod (const unsigned int filter_no)
{//{{{
    if (!GetResetStatus("SMPRESZ")&&CheckClockPeriod("SMPCLK")) {
        if (mFCLA_Func->IsFilterBypassEnable(filter_no)) {//If filter bypass function is enable
            mFCLATSOValue[filter_no] = FCLATSI[filter_no]->read();
            mFCLATSOPortsWriteEvent[filter_no].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Detect the changing of FCLATNFSI ports
void FCLA::DetectFCLATNFSIPortsMethod (const unsigned int filter_no)
{//{{{
    if (!GetResetStatus("SMPRESZ")&&CheckClockPeriod("SMPCLK")) {
        bool FCLATNFSICurrentValue = FCLATNFSI[filter_no]->read();
        if (!(mFCLA_Func->IsFilterBypassEnable(filter_no))) {//If filter bypass function is disable
            mFCLATSOValue[filter_no] = FCLATNFSICurrentValue;
            mFCLATSOPortsWriteEvent[filter_no].notify(SC_ZERO_TIME);
        }
        if (mFCLA_Func->IsLevelDetectorEnable(filter_no)) {//If level detector is enable
            double current_time = sc_time_stamp().to_double();
            mFCLATNFSIPortValueCheckAtSyncTimeEvent[filter_no].notify(CalculateCLKPosEdge("SMPCLK", current_time), mTimeResolutionUnit);
        }
    }
}//}}}

///Write a value to FCLATINTR and FCLATINTF ports
void FCLA::WriteEdgeSelectPortsMethod (const unsigned int filter_no)
{//{{{
    FCLATINTR[filter_no]->write(mFCLATINTRValue[filter_no]);
    FCLATINTF[filter_no]->write(mFCLATINTFValue[filter_no]);
}//}}}

///Write the value to FCLATSO ports
void FCLA::WriteFCLATSOPortsMethod (const unsigned int filter_no)
{//{{{
    FCLATSO[filter_no]->write(mFCLATSOValue[filter_no]);
}//}}}

///Write the value to FCLATEDSO ports
void FCLA::WriteFCLATEDSOPortsMethod (const unsigned int filter_no)
{//{{{
    FCLATEDSO[filter_no]->write(mFCLATEDSOValue[filter_no]);
}//}}}

///Check the value of FCLATNFSI ports at synchronous time with SMPCLK
void FCLA::CheckFCLATNFSIPortValueAtSyncTimeMethod (const unsigned int filter_no)
{//{{{
    bool FCLATNFSIValueAtSyncTime = FCLATNFSI[filter_no]->read();
    if (mFCLA_Func->IsLevelDetectorEnable(filter_no)){//If Level detector is enable
        if (((mFCLA_Func->IsRisingDetectLevel(filter_no))&&(FCLATNFSIValueAtSyncTime==true))||
            ((!(mFCLA_Func->IsRisingDetectLevel(filter_no)))&&(FCLATNFSIValueAtSyncTime==false))) {
            mFCLATEDSOPortActivateEvent[filter_no].notify(2*mSMPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

///Activate FCLATEDSO ports
void FCLA::ActivateFCLATEDSOPortMethod (const unsigned int filter_no)
{//{{{
    bool FCLATNFSIValueAtActiveTimeFCLATED = FCLATNFSI[filter_no]->read();
    if (mFCLA_Func->IsLevelDetectorEnable(filter_no)){//If Level detector is enable
        mFCLATEDSOValue[filter_no] = true;
        mFCLATEDSOPortsWriteEvent[filter_no].notify(SC_ZERO_TIME);
        mFCLATEDSOPortDeactivateEvent[filter_no].notify(mSMPCLKPeriod, mTimeResolutionUnit);
        if (((mFCLA_Func->IsRisingDetectLevel(filter_no))&&(FCLATNFSIValueAtActiveTimeFCLATED==true))||
            ((!(mFCLA_Func->IsRisingDetectLevel(filter_no)))&&(FCLATNFSIValueAtActiveTimeFCLATED==false))) {
            mFCLATEDSOPortActivateEvent[filter_no].notify(2*mSMPCLKPeriod, mTimeResolutionUnit);
        }
    }
}///}}}

///Deactivate FCLATEDSO ports
void FCLA::DeactivateFCLATEDSOPortMethod (const unsigned int filter_no)
{//{{{
    mFCLATEDSOValue[filter_no] = false;
    mFCLATEDSOPortsWriteEvent[filter_no].notify(SC_ZERO_TIME);
}///}}}

///Execute reset operation
void FCLA::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "PRESETZ") {
        mFCLA_Func->Reset(is_active);
    }
    if (is_active) {
        if (reset_name == "SMPRESZ") {//Initialize variables, output signals and cancel events
            for (unsigned char index = 0; index < emFilterNum; index++) {
                //Cancel events
                mEdgeSelectPortsWriteEvent[index].cancel();
                mFCLATSOPortsWriteEvent[index].cancel();
                mFCLATEDSOPortsWriteEvent[index].cancel();
                mFCLATNFSIPortValueCheckAtSyncTimeEvent[index].cancel();
                mFCLATEDSOPortActivateEvent[index].cancel();
                mFCLATEDSOPortDeactivateEvent[index].cancel();
                mFilterBypassEnableEvent[index].cancel();
                mFilterBypassDisableEvent[index].cancel();
                mLevelDetectorEnableEvent[index].cancel();
                mLevelDetectorDisableEvent[index].cancel();
                //Initialize output signals
                mFCLATINTRValue[index] = false;
                mFCLATINTFValue[index] = false;
                mFCLATSOValue[index] = false;
                mFCLATEDSOValue[index] = false;
                mEdgeSelectPortsWriteEvent[index].notify(SC_ZERO_TIME);
                mFCLATSOPortsWriteEvent[index].notify(SC_ZERO_TIME);
                mFCLATEDSOPortsWriteEvent[index].notify(SC_ZERO_TIME);
            }
        } else {//reset_name is PRESETZ. When registers are initliazed, FCLATEDSIn and FCLATSIn must be detected because Bypass Filter is disable and Level Detector is disable
            for (unsigned char index = 0; index < emFilterNum; index++) {
                if (CheckClockPeriod("SMCPLK")&&CheckClockPeriod("PCLK")) {
                    ControlEdgeSelectPorts(index, mFCLA_Func->IsRisingDetectLevel(index), mFCLA_Func->IsFallingDetectLevel(index));
                    ControlFilterBypass(index,mFCLA_Func->IsFilterBypassEnable(index));
                    ControlLevelDetector(index,mFCLA_Func->IsLevelDetectorEnable(index));
                }
            }
        }
    } else {
        if (reset_name == "SMPRESZ") {//Control output ports according to the value of register
            for (unsigned char index = 0; index < emFilterNum; index++) {
                ControlEdgeSelectPorts(index, mFCLA_Func->IsRisingDetectLevel(index), mFCLA_Func->IsFallingDetectLevel(index));
                ControlFilterBypass(index,mFCLA_Func->IsFilterBypassEnable(index));
                ControlLevelDetector(index,mFCLA_Func->IsLevelDetectorEnable(index));
            }
        }
    }
}//}}}

///Check frequency value and frequency unit of PCLK clock
void FCLA::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
void FCLA::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void FCLA::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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
double FCLA::CalculateCLKPosEdge (const std::string clock_name, const double time_point)
{//{{{
    double period_num = 0;
    if (clock_name == "PCLK") {
        period_num = time_point / (double)mPCLKPeriod;
    } else {//clock_name is SMPCLK
        period_num = time_point / (double)mSMPCLKPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "PCLK") {
        pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - time_point;
    } else {//clock_name is SMPCLK
        pos_edge_point = (double)mod_period_num * (double)mSMPCLKPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Check reset status
bool FCLA::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "SMPRESZ") {
        if (mIsSMPRESZHardActive || mIsSMPRESZCmdActive) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///Check period value of PCLK, SMPCLK clock
bool FCLA::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "PCLK")?mPCLKPeriod:mSMPCLKPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Control edge select ports
void FCLA::ControlEdgeSelectPorts (const unsigned int filter_no, const bool FCLATINTR_value, const bool FCLATINTF_value)
{//{{{
    if (CheckClockPeriod("SMPCLK")&&(CheckClockPeriod("PCLK"))) {
        mFCLATINTRValue[filter_no] = FCLATINTR_value;
        mFCLATINTFValue[filter_no] = FCLATINTF_value;
        double current_time = sc_time_stamp().to_double();
        mEdgeSelectPortsWriteEvent[filter_no].notify(CalculateCLKPosEdge("PCLK", current_time), mTimeResolutionUnit);
    } else {
        re_printf("warning","Can not control FCLATINTR[%d] and FCLATINTF[%d] ports when SMPCLK or PCLK is invalid\n", filter_no, filter_no);
    }
}//}}}

///Control filter bypass
void FCLA::ControlFilterBypass (const unsigned int filter_no, const bool is_enable)
{//{{{
    if (CheckClockPeriod("SMPCLK")) {
        if (is_enable) {
            mFilterBypassEnableEvent[filter_no].notify(SC_ZERO_TIME);
        } else {
            mFilterBypassDisableEvent[filter_no].notify(SC_ZERO_TIME);
        }
    } else {
        re_printf("warning","The filter %d of FCLA model can not operate when SMPCLK is invalid\n", filter_no);
    }
}//}}}

///Control level detector
void FCLA::ControlLevelDetector (const unsigned int filter_no, const bool is_enable)
{//{{{
    if (CheckClockPeriod("SMPCLK")) {
        if (is_enable) {
            double current_time = sc_time_stamp().to_double();
            mLevelDetectorEnableEvent[filter_no].notify(CalculateCLKPosEdge("SMPCLK", current_time), mTimeResolutionUnit); 
        } else {
            mLevelDetectorDisableEvent[filter_no].notify(SC_ZERO_TIME);
        }
    } else {
        re_printf("warning","The filter %d of FCLA model can not operate when SMPCLK is invalid\n", filter_no);
    }
}//}}}
// vim600: set foldmethod=marker :
