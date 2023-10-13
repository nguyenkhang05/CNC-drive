// ---------------------------------------------------------------------
// $Id: CRC64APBV01.cpp,v 1.9 2019/12/17 01:50:15 huepham Exp $
//
// Copyright(c) 2018 - 2019 Renesas Electronics Corporation
// Copyright(c) 2018 - 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "CRC64APBV01.h"
#include "CRC64APBV01_Func.h"

/// Constructor and Destructor
CRC64APBV01::CRC64APBV01(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,rvc_common_model()
        ,CRC64APBV01_AgentController()
        ,PCLK("PCLK")
        ,PRESETn("PRESETn")
{//{{{ 
    mpCRC64APBV01Func = new CRC64APBV01_Func((std::string)name, this);

    // Initialize output port

    // Configure Slave socket
    setSlaveResetPort32(&PRESETn);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32("ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];

    mBusSlaveIf32[0]->setFuncModulePtr(mpCRC64APBV01Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize variables
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
    mEnableRegisterMessage = true;
    mStartPCLKTime = 0;
    //mDumpInterrupt = false;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    // SC_THREAD/METHOD
    SC_METHOD(MonitorPCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(MonitorPRESETnMethod);
    sensitive << PRESETn;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

}//}}}

/// Destructor
CRC64APBV01::~CRC64APBV01()
{//{{{
    delete mpCRC64APBV01Func;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Get reset status
bool CRC64APBV01::GetResetStatus ()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Check clock period 
bool CRC64APBV01::CheckClockPeriod ()
{//{{{ 
    bool clock_status = false;
    if (mPCLKPeriod > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","PCLK frequency is zero\n");
    }
    return clock_status;
}//}}} 

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor PRESETn port
void CRC64APBV01::MonitorPRESETnMethod()
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
void CRC64APBV01::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events

        // Initialize variables

        // Initialize output ports
        
        // Notify reset event
        mResetEvent.notify();
    }
    mpCRC64APBV01Func->EnableReset(is_active);
}//}}}

/// Monitor PCLK clock
void CRC64APBV01::MonitorPCLKMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

/// Process reset function when reset command is active
void CRC64APBV01::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETn.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void CRC64APBV01::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of PRESETn is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

//========================================================
//============ Operation               ===================
//========================================================

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void CRC64APBV01::SetMessageLevel (const std::string msg_lv)
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

        mpCRC64APBV01Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpCRC64APBV01Func->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpCRC64APBV01Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpCRC64APBV01Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void CRC64APBV01::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpCRC64APBV01Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information
//void CRC64APBV01::DumpInterrupt (const std::string is_enable)
//{//{{{
//    std::vector<std::string> word_vector;
//    SeparateString(word_vector, is_enable);
//    if (word_vector.size() == 1) {
//        if (word_vector[0] == "true") {
//            mDumpInterrupt = true;
//        } else if (word_vector[0] == "false") {
//            mDumpInterrupt = false;
//        } else {
//            re_printf("warning","Invalid argument: CRC64APBV01_DumpInterrupt %s\n", is_enable.c_str());
//        }
//    } else if (word_vector.size() == 0){
//        std::string temp = (mDumpInterrupt) ? "true" : "false";
//        re_printf("info","CRC64APBV01_DumpInterrupt %s\n",temp.c_str());
//    } else {
//        re_printf("warning","Invalid argument: CRC64APBV01_DumpInterrupt %s\n", is_enable.c_str());
//    }
//}//}}}

/// Dump status register's value
void CRC64APBV01::DumpStatusInfo ()
{//{{{
    mpCRC64APBV01Func->DumpStatusInfo();
}//}}}

/// Command to assert reset
void CRC64APBV01::AssertReset (const std::string reset_name, const double start_time, const double period)
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
void CRC64APBV01::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
void CRC64APBV01::GetCLKFreq (const std::string clock_name)
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
void CRC64APBV01::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpCRC64APBV01Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void CRC64APBV01::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpCRC64APBV01Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void CRC64APBV01::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpCRC64APBV01Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void CRC64APBV01::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpCRC64APBV01Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Enable/disable info/warning/error message of register IF
void CRC64APBV01::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: CRC64APBV01_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","CRC64APBV01_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: CRC64APBV01_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

/// List all registers name
void CRC64APBV01::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpCRC64APBV01Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void CRC64APBV01::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_MessageLevel (\"CRC64APBV01 instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_DumpRegisterRW (\"CRC64APBV01 instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_EnableRegisterMessage (\"CRC64APBV01 instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_DumpStatusInfo (\"CRC64APBV01 instance\")                                    Dump the status info of model");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_AssertReset (\"CRC64APBV01 instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the CRC64APBV01 model");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_SetCLKFreq (\"CRC64APBV01 instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to CRC64APBV01 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_GetCLKFreq (\"CRC64APBV01 instance\", \"clock_name\")                        Get clock frequency value of CRC64APBV01 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_ForceRegister (\"CRC64APBV01 instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_ReleaseRegister (\"CRC64APBV01 instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_WriteRegister (\"CRC64APBV01 instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_ReadRegister (\"CRC64APBV01 instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(),"CRC64APBV01_ListRegister (\"CRC64APBV01 instance\")                                      Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of CRC64APBV01_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of CRC64APBV01_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
