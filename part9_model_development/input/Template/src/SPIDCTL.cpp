// ---------------------------------------------------------------------
// $Id: SPIDCTL.cpp,v 1.4 2021/11/19 11:19:36 minhquocha Exp $
//
// Copyright(c) 2020-2021 Renesas Electronics Corporation
// Copyright(c) 2020-2021 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of SPIDCTL.cpp
// Ref: {SPIDCTL_UD_Copyright_001}
// Description: About the include header files of SPIDCTL.cpp
// Ref: {SPIDCTL_UD_ATT1_002}
#include "re_define.h"
#include "SPIDCTL.h"
#include "SPIDCTL_Func.h"
#include <ctype.h>
#include <iostream>
#include <cstring>
#include "Common.h"

#define KEY_NA  "N/A"

extern int token_to_int(char *token, const char *moduleName, const char *error_message);

// Description: Constructor of SPIDCTL class
// Ref: {SPIDCTL_UD_Summary_001, SPIDCTL_UD_ATT1_004}
SPIDCTL::SPIDCTL (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency, const char *config_file)
    :sc_module(name)
    , SPIDCTL_AgentController((std::string)name)
    , BusSlaveBase<BUS_WIDTH_APB, 1>()
    , PCLK("PCLK")
    , Reset("Reset")
{//{{{

    for (unsigned int i = 0; i < MAX_SPID; i++) {
        if(i <= 31) {
            mBMSPIDOUTInitVal[i] = i;
        } else {
            mBMSPIDOUTInitVal[i] = 31;
        }
    }

    ReadConfigFile(config_file);

    // Configure Slave socket
    setSlaveResetPort32(&Reset);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32((char*)"ts");

    ts = BusSlaveBase<BUS_WIDTH_APB, 1>::tSocket32[0];

    mpSPIDCTLFunc = new SPIDCTL_Func((std::string)name, this);
    sc_assert(mpSPIDCTLFunc != NULL);

    mBusSlaveIf32[0]->setFuncModulePtr(mpSPIDCTLFunc);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize variables
    mPCLKPeriod =           0;
    mPCLKOrgFreq =          0;
    mPCLKFreq =             0;
    mPCLKFreqUnit =         "Hz";
    mStartClkTime =         0;
    mResetCurVal =          true;
    mResetPreVal =          true;
    mIsResetHardActive =    false;
    mIsResetCmdReceive =    false;
    mIsResetCmdActive =     false;
    mResetCmdPeriod =       0;
    mTimeResolutionValue =  1;
    mTimeResolutionUnit =   SC_NS;
    mEnableRegisterMessage = true;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    // Initialize output port
    std::ostringstream port_name;
    for (unsigned int i = 0; i < MAX_SPID; i++) {
        port_name.str("");
        port_name << "BMSPIDOUT" << i;
        BMSPIDOUT[i] = new sc_out<unsigned char>(port_name.str().c_str());
        BMSPIDOUT[i]->initialize(mBMSPIDOUTInitVal[i]);
    }
    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleResetSignalMethod);
    dont_initialize();
    sensitive << Reset;

    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    for (unsigned int spid_id = 0; spid_id < MAX_SPID; spid_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mEventUpdateSPID[spid_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&SPIDCTL::HandleSPIDCTLMethod, this, spid_id),
            sc_core::sc_gen_unique_name("HandleSPIDCTLMethod"), &opt);
    }
}//}}}

// Description: Destructor
SPIDCTL::~SPIDCTL()
{//{{{
    delete mpSPIDCTLFunc;
    for (unsigned int i = 0; i < MAX_SPID; i++) {
        delete BMSPIDOUT[i];
    }
}//}}}

// Description: Initialize when reset activated
void SPIDCTL::Initialize(void)
{//{{{
    mKeyCode = false;
    // Initialize member variable
    for (unsigned int i = 0; i < MAX_SPID; i++) {
        mIsSPIDMaskLock[i] = false;
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
// Description: Check clock period
// Ref: {SPIDCTL_UD_Direction_039}
bool SPIDCTL::CheckClockPeriod(void)
{//{{{
    bool clock_status = false;
    if (mPCLKFreq > 0) {
        clock_status = true;
    }else{
        common_re_printf("info", "PCLK frequency is zero\n");
    }
    return clock_status;
}//}}}

// Description: Get reset status
bool SPIDCTL::GetResetStatus(void)
{//{{{
    return (mIsResetHardActive || mIsResetCmdActive);
}//}}}

// Description: Notify event when SPID/SPIDMSK reg is written
void SPIDCTL::NotifyUpdateSPIDValue(unsigned int Index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    
    //Not update ouput ports when reset is asserted or clock is zero
    if ((true == GetResetStatus()) || (false == CheckClockPeriod())){
        return;
    }
    
    mEventUpdateSPID[Index].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartClkTime), mTimeResolutionUnit);
}//}}}

// Description: Get key code enable bit
bool SPIDCTL::IsKeyCodeEnable(void)
{//{{{
    return mKeyCode;
}//}}}

// Description: Set key code enable bit
void SPIDCTL::SetKeyCodeEnable(bool isEnable)
{//{{{
    mKeyCode = isEnable;
}//}}}

// Description: Get lock status of register
bool SPIDCTL::IsMaskLocked(unsigned int RegIndex)
{//{{{
    return mIsSPIDMaskLock[RegIndex];
}//}}}

// Description: Set lock status of register
void SPIDCTL::SetMaskLocked(unsigned int RegIndex, bool isLocked)
{//{{{
    mIsSPIDMaskLock[RegIndex] = isLocked;
}//}}}
//========================================================
//============ Reset and clock  ==========================
//========================================================
// Description: Monitor Reset port
// Ref: {SPIDCTL_UD_Feature_002}
void SPIDCTL::HandleResetSignalMethod(void)
{//{{{
    mResetCurVal = Reset.read();
    if (mResetCurVal != mResetPreVal) { //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

// Description: Monitor PCLK clock
// Ref: {SPIDCTL_UD_Feature_001, SPIDCTL_UD_Flow_002}
void SPIDCTL::HandlePCLKSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

// Description: Process reset function when reset port is active
// Ref: {SPIDCTL_UD_ATT1_005, SPIDCTL_UD_Direction_043, SPIDCTL_UD_Direction_044, SPIDCTL_UD_Flow_001, SPIDCTL_UD_Flow_011}
void SPIDCTL::HandleResetHardMethod(void)
{//{{{
    if (mResetCurVal == vpcl::emResetActive) {
        mIsResetHardActive = true;
        common_re_printf("info", "The reset port Reset is asserted.\n");
        this->EnableReset(mIsResetHardActive);
    }else{
        mIsResetHardActive = false;
        common_re_printf("info", "The reset port Reset is de-asserted.\n");
        this->EnableReset(mIsResetHardActive);
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

// Description: Process reset function when reset command is active
// Ref: {SPIDCTL_UD_Direction_041}
void SPIDCTL::HandleResetCmdMethod(void)
{//{{{
    std::string reset_name = "Reset";
    mIsResetCmdActive = true;
    common_re_printf("info", "The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

// Description: Cancel reset function when reset command is active
// Ref: {SPIDCTL_UD_Direction_040}
void SPIDCTL::CancelResetCmdMethod(void)
{//{{{
    std::string reset_name = "Reset";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    common_re_printf("info", "Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive);
}//}}}

// Description: Reset model and its sub-instance if reset is actived
void SPIDCTL::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variables
        Initialize();
        // Initialize output port
        for (unsigned int i = 0; i < MAX_SPID; i++) {
            mEventUpdateSPID[i].notify(SC_ZERO_TIME);
        }
    }
    mpSPIDCTLFunc->EnableReset(is_active);
}//}}}
//========================================================
//============ Operation               ===================
//========================================================
// Description: Cancel operation events
void SPIDCTL::CancelEvents(void)
{//{{{
    // Operation events are canceled
    for (unsigned int event_id = 0; event_id < MAX_SPID; event_id++) {
        mEventUpdateSPID[event_id].cancel();
    }
}//}}}

// Description: Handle SPIDCTL setting
// Ref: {SPIDCTL_UD_Feature_004, SPIDCTL_UD_Feature_005, SPIDCTL_UD_Flow_010}
void SPIDCTL::HandleSPIDCTLMethod(unsigned int spid_id)
{//{{{
    unsigned int SPIDVal = mpSPIDCTLFunc->GetSPIDReg_SPIDBit(spid_id);
    unsigned int SPIDMaskVal = mpSPIDCTLFunc->GetSPIDMskReg(spid_id);

    // In case all SPID numbers are not allowed
    if (ZERO_SPIDMASK == SPIDMaskVal) {
        return;
    }

    // Check valid of SPID value
    unsigned int SPIDValid = (SPIDMaskVal >> SPIDVal) & 0x1;

    if (1 == SPIDValid) {
        // Update output port
        BMSPIDOUT[spid_id]->write((unsigned char)SPIDVal);
    }
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
// Description: Set message level (fatal, error, warning, info)
void SPIDCTL::SetMessageLevel(const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = HandleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    if (true == mEnableRegisterMessage) {
        SeparateString(cmd, msg_lv);
    }else{
        //Set message level for Register IF class (except fatal level)
        cmd.push_back("fatal");
    }
    mpSPIDCTLFunc->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    if (true == mEnableRegisterMessage) {
        SeparateString(cmd, msg_lv);
    }else{
        //Set message level for Register IF class (except fatal level)
        cmd.push_back("fatal");
    }
    mpSPIDCTLFunc->RegisterHandler(cmd);
}//}}}

// Description: Enable/disable dumping message when users access registers
void SPIDCTL::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpSPIDCTLFunc->RegisterHandler(cmd);
}//}}}

// Description: Enable/disable info/warning/error message of register IF
// Ref: {SPIDCTL_UD_Direction_035, SPIDCTL_UD_Direction_046}
void SPIDCTL::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }else if(word_vector[0] == "false"){
            mEnableRegisterMessage = false;
        }else{
            common_re_printf("warning", "Invalid argument: SPIDCTL_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }else if(word_vector.size() == 0){
        std::string temp = "";
        if(mEnableRegisterMessage){
            temp = "true";
        }else{
            temp = "false";
        }
        common_re_printf("info", "SPIDCTL_EnableRegisterMessage %s\n", temp.c_str());
    }else{
        common_re_printf("warning", "Invalid argument: SPIDCTL_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

// Description: Dump status register's value
void SPIDCTL::DumpStatusInfo(void)
{//{{{
    mpSPIDCTLFunc->DumpStatusInfo();
}//}}}

// Description: Command to assert reset
// Ref: {SPIDCTL_UD_Direction_042, SPIDCTL_UD_Direction_037, SPIDCTL_UD_Direction_036}
void SPIDCTL::AssertReset(const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "Reset") {
        if ((!mIsResetHardActive) && (!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            common_re_printf("info", "The model will be reset (Reset) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        }else{
            common_re_printf("warning", "The software reset of Reset is called in the reset operation of the model. So it is ignored.\n");
        }
    }else{
        common_re_printf("warning", "The reset name (%s) is wrong. It should be Reset.\n", reset_name.c_str());
    }
}//}}}

// Description: Set clock value and clock unit
// Ref: {SPIDCTL_UD_Direction_034}
void SPIDCTL::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq(mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1 / (double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartClkTime = sc_time_stamp().to_double();
                for(unsigned int i=0;i<MAX_SPID;i++){
                    mEventUpdateSPID[i].notify(SC_ZERO_TIME);
                }
            }else{
                mPCLKPeriod = 0;
                // Prepare for next run
                CancelEvents();
            }
        }else{
            common_re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        common_re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

// Description: Get clock value
// Ref: {SPIDCTL_UD_Direction_045, SPIDCTL_UD_Direction_034}
void SPIDCTL::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            common_re_printf("info", "PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        }else{
            common_re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        common_re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

// Description: Force value to register
// Ref: {SPIDCTL_UD_Direction_026}
void SPIDCTL::ForceRegister(const std::string reg_name, const unsigned int reg_value)
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
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mpSPIDCTLFunc->RegisterHandler(cmd);
        }else{
            common_re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        common_re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Release forced value after forcing registers
// Ref: {SPIDCTL_UD_Direction_026}
void SPIDCTL::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpSPIDCTLFunc->RegisterHandler(cmd);
        }else{
            common_re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        common_re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Write value to registers by software
// Ref: {SPIDCTL_UD_Direction_026}
void SPIDCTL::WriteRegister(const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mpSPIDCTLFunc->RegisterHandler(cmd);
        }else{
            common_re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        common_re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Read value of register by software
// Ref: {SPIDCTL_UD_Direction_026}
void SPIDCTL::ReadRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpSPIDCTLFunc->RegisterHandler(cmd);
        }else{
            common_re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        common_re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: List all registers name
// Ref: {SPIDCTL_UD_Direction_012}
void SPIDCTL::ListRegister(void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpSPIDCTLFunc->RegisterHandler(cmd);
}//}}}

// Description: Dump help message of all parameters or commands
// Ref: {SPIDCTL_UD_Direction_013, SPIDCTL_UD_Direction_014, SPIDCTL_UD_Direction_025, SPIDCTL_UD_Direction_049}
void SPIDCTL::Help(const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(), "SPIDCTL_MessageLevel (\"SPIDCTL instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_DumpRegisterRW (\"SPIDCTL instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_EnableRegisterMessage (\"SPIDCTL instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        }else if (word_vector[0] == "commands"){
            SC_REPORT_INFO(this->basename(), "SPIDCTL_DumpStatusInfo (\"SPIDCTL instance\")                                    Dump the status info of model");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_AssertReset (\"SPIDCTL instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the SPIDCTL model");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_SetCLKFreq (\"SPIDCTL instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to SPIDCTL model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_GetCLKFreq (\"SPIDCTL instance\", \"clock_name\")                        Get clock frequency value of SPIDCTL model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_ForceRegister (\"SPIDCTL instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_ReleaseRegister (\"SPIDCTL instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_WriteRegister (\"SPIDCTL instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_ReadRegister (\"SPIDCTL instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(), "SPIDCTL_ListRegister (\"SPIDCTL instance\")                                      Dump name of all registers");
        }else{
            common_re_printf("warning", "The name (%s) of SPIDCTL_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    }else{
        common_re_printf("warning", "The name (%s) of SPIDCTL_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// Ref: {SPIDCTL_UD_Direction_050}
///*********************************
/// Function     : GetArrCfgValue
/// Description  : return array of interger from given string
///*********************************
bool SPIDCTL::GetArrCfgValue(std::string s, unsigned int *arr)
{
    assert(arr != NULL);

    bool valid = false;

    if ((strcmp(s.c_str(), KEY_NA) == 0) || (isdigit((unsigned char)s[0]) == false)) {
        common_re_printf("warning", "Invalid setting. The default initial value will be used.\n");
        return valid;
    }

    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    unsigned int index = 0;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        arr[index] = token_to_int(&token[0], "", "");
        index++;
        valid = true;
    }
    arr[index] = token_to_int(&s[0], "", "");
    return valid;
}

///*********************************
/// Function     : ReadConfigFile
/// Description  : return map of configuration from given list of parameter
///*********************************
void SPIDCTL::ReadConfigFile(const char *config_file)
{
    std::string token;

    if(config_file == NULL) {
        return;
    }

    std::ifstream file(config_file);
    while (std::getline(file, token)) {
        std::istringstream line(token);

        std::string delimiter = "=";

        size_t pos = 0;
        std::string s_token;
        if ((pos = token.find(delimiter)) != std::string::npos) {
            std::string key, value;
            s_token = token.substr(0, pos);
            key = GetStrBetweenTwoStr(s_token, "[", "]");

            if (strcmp(key.c_str(), "SPID_BUS_MASTER") == 0) {
                token.erase(0, pos + delimiter.length());
                value = GetStrBetweenTwoStr(token, "(", ")");
                value.erase(remove(value.begin(), value.end(), ' '), value.end());

                unsigned int p_arr[2];
                if(GetArrCfgValue(value, p_arr)) {
                    mSPIDInitValMap.insert(std::pair<unsigned int, unsigned int>(p_arr[0], p_arr[1]));
                    mBMSPIDOUTInitVal[p_arr[0]] = (unsigned char)p_arr[1];
                }
            }
        }
    }
}

///*********************************
/// Function     : GetStrBetweenTwoStr
/// Description  : get string between two string
///*********************************
std::string SPIDCTL::GetStrBetweenTwoStr(const std::string &s, const std::string &start_delim, const std::string &stop_delim)
{
    size_t first_delim_pos = s.find(start_delim);
    size_t end_pos_of_first_delim = first_delim_pos + start_delim.length();
    size_t last_delim_pos = s.find(stop_delim);

    return s.substr(end_pos_of_first_delim,
        last_delim_pos - end_pos_of_first_delim);
}

// vim600: set foldmethod=marker :
