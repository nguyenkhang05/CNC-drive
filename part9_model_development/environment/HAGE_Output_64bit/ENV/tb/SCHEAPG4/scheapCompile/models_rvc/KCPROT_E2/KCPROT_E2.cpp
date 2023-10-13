// ---------------------------------------------------------------------
// $Id: KCPROT_E2.cpp,v 1.6 2019/09/06 08:50:02 landang Exp $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "Common.h"
#include "KCPROT_E2.h"
#include "KCPROT_E2_Func.h"

/// Constructor and Destructor
KCPROT_E2::KCPROT_E2(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,KCPROT_E2_AgentController()
        ,PCLK("PCLK")
        ,PRESETn("PRESETn")
        ,ICUM_CFAPT_lock("ICUM_CFAPT_lock")
        ,keyopen("keyopen")
        ,keysecopen("keysecopen")
{//{{{ 
    mpKCPROT_E2Func = new KCPROT_E2_Func((std::string)name, this);

    // Configure Slave socket
    setSlaveResetPort32(&PRESETn);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32((char*)"ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];

    mBusSlaveIf32[0]->setFuncModulePtr(mpKCPROT_E2Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize variables
    mEnableRegisterMessage = true;
    mIsPRESETnInit = true;
    mPCLKPeriod = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreq = 0;
    mPCLKFreqUnit = "Hz";
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mStartPCLKTime = 0;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mKeyCode = emDefaultKeyCode;

    Initialize();
    keyopen.initialize(false);
    keysecopen.initialize(false);
    // SC_THREAD/METHOD
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(PRESETnMethod);
    sensitive << PRESETn;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(ICUM_CFAPT_lockMethod);
    dont_initialize();
    sensitive << ICUM_CFAPT_lock;

    SC_METHOD(WriteKeyOpenMethod);
    dont_initialize();
    sensitive << mWriteKeyOpenEvent;

    SC_METHOD(WriteKeySecOpenMethod);
    dont_initialize();
    sensitive << mWriteKeySecOpenEvent;
}//}}}

/// Destructor
KCPROT_E2::~KCPROT_E2()
{//{{{
    delete mpKCPROT_E2Func;
}//}}}

/// Initialize when reset activated
void KCPROT_E2::Initialize(void)
{//{{{
    mKeyOpen = false;
    mICUMCFAPTlock = false;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Write keyopen port
void KCPROT_E2::UpdateKeyCodeEnable(bool key_code_enable)
{//{{{
    mKeyOpen = key_code_enable;
    double current_time = sc_time_stamp().to_double();
    mWriteKeyOpenEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, current_time, mStartPCLKTime), this->mTimeResolutionUnit);
    mWriteKeySecOpenEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, current_time, mStartPCLKTime), this->mTimeResolutionUnit);
}//}}}

/// Get key code
unsigned int KCPROT_E2::GetKeyCode()
{//{{{
    return mKeyCode;
}//}}}

/// Get reset status
bool KCPROT_E2::GetResetStatus ()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Check clock period 
bool KCPROT_E2::CheckClockPeriod ()
{//{{{ 
    bool clock_status = false;
    if (mPCLKPeriod > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","pclk frequency is zero\n");
    }
    return clock_status;
}//}}} 

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor PRESETn port
void KCPROT_E2::PRESETnMethod()
{//{{{
    bool reset = PRESETn.read();
    if (reset == vpcl::emResetActive){
        mIsPRESETnInit = false;
        mIsResetHardActive = true;
        re_printf("info", "The reset port PRESETn is asserted.\n");
        EnableReset(true);
    }else{
        if (mIsPRESETnInit){
            mIsPRESETnInit = false;
        }else{
            mIsResetHardActive = false;
            re_printf("info", "The reset port PRESETn is de-asserted.\n");
            EnableReset(false);
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

/// Reset model and its sub-instance if reset is actived
void KCPROT_E2::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events
        mWriteKeyOpenEvent.cancel();
        mWriteKeySecOpenEvent.cancel();
        // Initialize variables
        Initialize();
        // Initialize output ports
        mWriteKeyOpenEvent.notify();
        mWriteKeySecOpenEvent.notify();
    }
    mpKCPROT_E2Func->EnableReset(is_active);
}//}}}

/// Monitor PCLK clock
void KCPROT_E2::PCLKMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

/// Process reset function when reset command is active
void KCPROT_E2::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETn.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void KCPROT_E2::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of PRESETn is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
/// Monitor ICUM_CFAPT_lock port
void KCPROT_E2::ICUM_CFAPT_lockMethod()
{//{{{
    mICUMCFAPTlock = ICUM_CFAPT_lock.read();
    double current_time = sc_time_stamp().to_double();
    mWriteKeyOpenEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, current_time, mStartPCLKTime), this->mTimeResolutionUnit);
    mWriteKeySecOpenEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, current_time, mStartPCLKTime), this->mTimeResolutionUnit);
}//}}}

/// Write value to keyopen port
void KCPROT_E2::WriteKeyOpenMethod()
{//{{{
    keyopen.write(mKeyOpen);
}//}}}

/// Write value to keysecopen port
void KCPROT_E2::WriteKeySecOpenMethod()
{//{{{
    bool value = mKeyOpen & (!mICUMCFAPTlock);
    keysecopen.write(value);
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void KCPROT_E2::SetMessageLevel (const std::string msg_lv)
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
    if (true == mEnableRegisterMessage) {
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpKCPROT_E2Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpKCPROT_E2Func->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpKCPROT_E2Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpKCPROT_E2Func->RegisterHandler(cmd);
    }    
}//}}}

/// Enable/disable dumping message when users access registers
void KCPROT_E2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpKCPROT_E2Func->RegisterHandler(cmd);
}//}}}

/// Set keycode
void KCPROT_E2::KEYCODE(const unsigned int keycode)
{//{{{
    mKeyCode = keycode;
}//}}}

/// Command to assert reset
void KCPROT_E2::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void KCPROT_E2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void KCPROT_E2::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void KCPROT_E2::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpKCPROT_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void KCPROT_E2::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpKCPROT_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void KCPROT_E2::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpKCPROT_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void KCPROT_E2::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpKCPROT_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void KCPROT_E2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpKCPROT_E2Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void KCPROT_E2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"KCPROT_MessageLevel (\"KCPROT instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"KCPROT_DumpRegisterRW (\"KCPROT instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"KCPROT_DumpInterrupt (\"KCPROT instance\", \"true/false\")                     Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(),"KCPROT_EnableRegisterMessage (\"KCPROT instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"KCPROT_AssertReset (\"KCPROT instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the KCPROT model");
            SC_REPORT_INFO(this->basename(),"KCPROT_SetCLKFreq (\"KCPROT instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to KCPROT model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"KCPROT_GetCLKFreq (\"KCPROT instance\", \"clock_name\")                        Get clock frequency value of KCPROT model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"KCPROT_ForceRegister (\"KCPROT instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(),"KCPROT_ReleaseRegister (\"KCPROT instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(),"KCPROT_WriteRegister (\"KCPROT instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(),"KCPROT_ReadRegister (\"KCPROT instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(),"KCPROT_ListRegister (\"KCPROT instance\")                                      Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of KCPROT_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of KCPROT_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}
void KCPROT_E2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: KCPROT_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","KCPROT_E2_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: KCPROT_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
