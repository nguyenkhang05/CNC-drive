// ---------------------------------------------------------------------
// $Id: ECC_ETHERNET_E2.cpp,v 1.8 2019/09/26 10:07:23 landang Exp $
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

#include "ECC_def.h"
#include "ECC_ETHERNET_E2.h"
#include "ECC_ETHERNET_E2_Func.h"

/// Constructor and Destructor
ECC_ETHERNET_E2::ECC_ETHERNET_E2(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,ECC_ETHERNET_E2_AgentController()
        // Clock/Reset
        ,pclk("pclk")
        ,presetn("presetn")
        ,secdisable_rx("secdisable_rx")
        ,secdisable_tx("secdisable_tx")
        ,dederrout("dederrout")
        ,sederrout("sederrout")
{//{{{ 
    // New instance and port
    mpECC_ETHERNET_E2Func = new ECC_ETHERNET_E2_Func((std::string)name, this);
    
    // Configure Slave socket
    BusSlaveBase<32, 1>::setSlaveResetPort32(&presetn);
    BusSlaveBase<32, 1>::setSlaveFreqPort32(&pclk);
    BusSlaveBase<32, 1>::setTargetSocket32((char*)"ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setFuncModulePtr(mpECC_ETHERNET_E2Func);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setReadLatency(rLatency);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize remain output ports
    secdisable_rx.initialize(false);
    secdisable_tx.initialize(false);
    dederrout.initialize(false);
    sederrout.initialize(false);

    // Initialize variables
    mEnableRegisterMessage = true;
    mPCLKPeriod     = 0;
    mPCLKOrgFreq    = 0;
    mPCLKFreq       = 0;
    mPCLKFreqUnit   = "Hz";
    mStartPCLKTime  = 0;

    mIspresetnInit          = true;
    mIsResetHardActive      = false;
    mIsResetCmdReceive      = false;
    mIsResetCmdActive       = false;
    mResetCmdPeriod         = 0;

    mTimeResolutionValue    = 1;
    mTimeResolutionUnit     = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mDumpInterrupt  = false;

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorpclkMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(presetnMethod);
    sensitive << presetn;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(WriteSECDISABLE_TXMethod);
    dont_initialize();
    sensitive << mWriteSECDISABLE_TXEvent;

    SC_METHOD(WriteSECDISABLE_RXMethod);
    dont_initialize();
    sensitive << mWriteSECDISABLE_RXEvent;

    SC_METHOD(WriteSEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteSEDERROUTEvent;

    SC_METHOD(WriteDEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteDEDERROUTEvent;

    SC_METHOD(NegateSEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateSEDERROUTEvent;

    SC_METHOD(NegateDEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateDEDERROUTEvent;

    SC_METHOD(AssertSEDERROUTMethod);
    dont_initialize();
    sensitive << mAssertSEDERROUTEvent;

    SC_METHOD(AssertDEDERROUTMethod);
    dont_initialize();
    sensitive << mAssertDEDERROUTEvent;
}//}}} 

/// Destructor
ECC_ETHERNET_E2::~ECC_ETHERNET_E2()
{//{{{
    delete mpECC_ETHERNET_E2Func;
}//}}}

/// Initialize when reset activated
void ECC_ETHERNET_E2::Initialize(void)
{//{{{
    mSECDISABLE_RX = false;
    mSECDISABLE_TX = false;
    mDEDERROUT     = false;
    mSEDERROUT     = false;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Assert secdisable_tx
void ECC_ETHERNET_E2::WriteTXDisPort(bool value)
{//{{{
    mSECDISABLE_TX = value;
    mWriteSECDISABLE_TXEvent.notify();
}//}}}

/// Assert secdisable_rx
void ECC_ETHERNET_E2::WriteRXDisPort(bool value)
{//{{{
    mSECDISABLE_RX = value;
    mWriteSECDISABLE_RXEvent.notify();
}//}}}


//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor pclk clock
void ECC_ETHERNET_E2::MonitorpclkMethod()
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

/// Monitor presetn port
void ECC_ETHERNET_E2::presetnMethod()
{//{{{
    bool reset = presetn.read();
    if (reset == vpcl::emResetActive){
        mIspresetnInit = false;
        mIsResetHardActive = true;
        re_printf("info", "The reset port presetn is asserted.\n");
        EnableReset(true);
    }else{
        if (mIspresetnInit){
            mIspresetnInit = false;
        }else{
            mIsResetHardActive = false;
            re_printf("info", "The reset port presetn is de-asserted.\n");
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

/// Process reset function when reset command is active
void ECC_ETHERNET_E2::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of presetn.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void ECC_ETHERNET_E2::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of presetn is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

/// Reset model and its sub-instance if reset is actived
void ECC_ETHERNET_E2::EnableReset (const bool is_active)
{//{{{
    mpECC_ETHERNET_E2Func->EnableReset(is_active);
    if(is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        mWriteDEDERROUTEvent.notify();
        mWriteSEDERROUTEvent.notify();
        mWriteSECDISABLE_TXEvent.notify();
        mWriteSECDISABLE_RXEvent.notify();
    }
}//}}}

/// Cancel events
void ECC_ETHERNET_E2::CancelEvents()
{//{{{
    mAssertDEDERROUTEvent.cancel();
    mAssertSEDERROUTEvent.cancel();
    mWriteDEDERROUTEvent.cancel();
    mWriteSEDERROUTEvent.cancel();
    mNegateSEDERROUTEvent.cancel();
    mNegateDEDERROUTEvent.cancel();
    mWriteSECDISABLE_TXEvent.cancel();
    mWriteSECDISABLE_RXEvent.cancel();
}//}}}

/// Get reset status
bool ECC_ETHERNET_E2::GetResetStatus ()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Check clock period 
bool ECC_ETHERNET_E2::CheckClockPeriod ()
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
//============ Operation               ===================
//========================================================
/// Write secdisable_tx
void ECC_ETHERNET_E2::WriteSECDISABLE_TXMethod()
{//{{{
    secdisable_tx.write(mSECDISABLE_TX);
}//}}}

/// Write secdisable_rx
void ECC_ETHERNET_E2::WriteSECDISABLE_RXMethod()
{//{{{
    secdisable_rx.write(mSECDISABLE_RX);
}//}}}

/// Assert sederrout
void ECC_ETHERNET_E2::AssertSEDERROUTMethod()
{//{{{
    mSEDERROUT = true;
    mWriteSEDERROUTEvent.notify();
}//}}}

/// Assert dederrout
void ECC_ETHERNET_E2::AssertDEDERROUTMethod()
{//{{{
    mDEDERROUT = true;
    mWriteDEDERROUTEvent.notify();
}//}}}

/// Write sederrout port
void ECC_ETHERNET_E2::WriteSEDERROUTMethod()
{//{{{
    sederrout.write(mSEDERROUT);
    if (mDumpInterrupt){
        re_printf("info", "sederrout is changed to %d\n", (unsigned int)mSEDERROUT);
    }
    // Negate if any
    if (mSEDERROUT){
        mNegateSEDERROUTEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/// Write dederrout port
void ECC_ETHERNET_E2::WriteDEDERROUTMethod()
{//{{{
    dederrout.write(mDEDERROUT);
    if (mDumpInterrupt){
        re_printf("info", "dederrout is changed to %d\n", (unsigned int)mDEDERROUT);
    }
    // Negate if any
    if (mDEDERROUT){
        mNegateDEDERROUTEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/// Negate sederrout port
void ECC_ETHERNET_E2::NegateSEDERROUTMethod()
{//{{{
    mSEDERROUT = false;
    mWriteSEDERROUTEvent.notify();
}//}}}

/// Negate dederrout port
void ECC_ETHERNET_E2::NegateDEDERROUTMethod()
{//{{{
    mDEDERROUT = false;
    mWriteDEDERROUTEvent.notify();
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void ECC_ETHERNET_E2::SetMessageLevel (const std::string msg_lv)
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

        mpECC_ETHERNET_E2Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpECC_ETHERNET_E2Func->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpECC_ETHERNET_E2Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpECC_ETHERNET_E2Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void ECC_ETHERNET_E2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpECC_ETHERNET_E2Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information
void ECC_ETHERNET_E2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_ETHERNET_E2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_ETHERNET_E2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_ETHERNET_E2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Issue error to ECC
void ECC_ETHERNET_E2::IssueError(const unsigned int errortype)
{//{{{
    if (GetResetStatus() || (!CheckClockPeriod())){
        re_printf("info", "Not receive error input during reset period or clock is stoped.\n");
        return;
    }

    if ((errortype == em1bitError) || (errortype == em2bitError)){
        double current_time = sc_time_stamp().to_double();
        re_printf("info", "Error input Type: %d\n", (unsigned int)errortype);
        // 1bit error
        if (errortype == em1bitError){
            mAssertSEDERROUTEvent.cancel();
            mAssertSEDERROUTEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, current_time, mStartPCLKTime), mTimeResolutionUnit);

        // Fatal error
        }else{
            mAssertDEDERROUTEvent.cancel();
            mAssertDEDERROUTEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, current_time, mStartPCLKTime), mTimeResolutionUnit);
        }
    }else{
        re_printf("warning", "Invalid error type (%d) of ECC_ETHERNET_E2_IssueError command.\n", errortype);
    }
}//}}}

/// Command to assert reset
void ECC_ETHERNET_E2::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetn") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (presetn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of presetn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be presetn.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void ECC_ETHERNET_E2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent.notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void ECC_ETHERNET_E2::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void ECC_ETHERNET_E2::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            // Call RegisterHandler of accroding block name
            mpECC_ETHERNET_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void ECC_ETHERNET_E2::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            // Call RegisterHandler of accroding block name
            mpECC_ETHERNET_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void ECC_ETHERNET_E2::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpECC_ETHERNET_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void ECC_ETHERNET_E2::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpECC_ETHERNET_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void ECC_ETHERNET_E2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpECC_ETHERNET_E2Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void ECC_ETHERNET_E2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_MessageLevel (\"ECC_ETHERNET_E2 instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_DumpRegisterRW (\"ECC_ETHERNET_E2 instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_DumpInterrupt (\"ECC_ETHERNET_E2 instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_EnableRegisterMessage (\"ECC_ETHERNET_E2 instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_IssueError (\"ECC_ETHERNET_E2 instance\", error_type)                                              Issue pseudo error to the ECC_ETHERNET_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_AssertReset (\"ECC_ETHERNET_E2 instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the ECC_ETHERNET_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_SetCLKFreq (\"ECC_ETHERNET_E2 instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to ECC_ETHERNET_E2 model. Valid clock_name is pclk or ram_clk");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_GetCLKFreq (\"ECC_ETHERNET_E2 instance\", \"clock_name\")                                          Get clock frequency value of ECC_ETHERNET_E2 model. Valid clock_name is pclk or ram_clk");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_ForceRegister (\"ECC_ETHERNET_E2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_ReleaseRegister (\"ECC_ETHERNET_E2 instance\", \"block_name\", block_index, \"reg_name\")          Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_WriteRegister (\"ECC_ETHERNET_E2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_ReadRegister (\"ECC_ETHERNET_E2 instance\", \"block_name\", block_index, \"reg_name\")             Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_ETHERNET_E2_ListRegister (\"ECC_ETHERNET_E2 instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of ECC_ETHERNET_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_ETHERNET_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

void ECC_ETHERNET_E2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: ECC_ETHERNET_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_ETHERNET_E2_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_ETHERNET_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
