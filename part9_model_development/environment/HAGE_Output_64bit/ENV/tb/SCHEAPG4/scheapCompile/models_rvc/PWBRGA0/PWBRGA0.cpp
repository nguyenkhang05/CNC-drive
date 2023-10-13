// ---------------------------------------------------------------------
// $Id: PWBRGA0.cpp,v 1.12 2020/01/15 13:23:55 landang Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "PWBRGA0.h"
#include "PWBRGA0_Func.h"

/// Constructor and Destructor
PWBRGA0::PWBRGA0(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,PWBRGA0_AgentController()
        ,rvc_common_model()
        ,PCLK("PCLK")
        ,PCLKRW("PCLKRW")
        ,PRESETZ("PRESETZ")
        ,SVSTOP("SVSTOP")
{//{{{ 
    // New instance and port
    mpPWBRGA0Func = new PWBRGA0_Func((std::string)name, this);
    for (unsigned int i = 0; i < emNumClkOut; i++){
        std::ostringstream port_name;
        port_name.str("");
        port_name << "PWBA_CKEN" << i;
        PWBA_CKEN[i] = new sc_out<sc_dt::uint64>(port_name.str().c_str());
        sc_assert(PWBA_CKEN[i] != NULL);
        PWBA_CKEN[i]->initialize(0);
    }
    
    // Configure Slave socket
    BusSlaveBase<32, 1>::setSlaveResetPort32(&PRESETZ);
    BusSlaveBase<32, 1>::setSlaveFreqPort32(&PCLKRW);
    BusSlaveBase<32, 1>::setTargetSocket32("ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setFuncModulePtr(mpPWBRGA0Func);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setReadLatency(rLatency);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize variables
    mIsPRESETZInit = true;
    mPCLKPeriod = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreq = 0;
    mPCLKFreqUnit = "Hz";
    mPCLKRWPeriod = 0;
    mPCLKRWOrgFreq = 0;
    mPCLKRWFreq = 0;
    mPCLKRWFreqUnit = "Hz";
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod = 0;

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mStartPCLKTime = 0;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mEnableRegisterMessage = true;

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorPCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(MonitorPCLKRWMethod);
    dont_initialize();
    sensitive << PCLKRW;

    SC_METHOD(MonitorPRESETZMethod);
    sensitive << PRESETZ;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;
    
    SC_METHOD(WriteTEBitMethod);
    dont_initialize();
    sensitive << mWriteTEBitEvent;

    for (unsigned int i = 0; i < emNumClkOut; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteCKENEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PWBRGA0::WriteCKENMethod, this, i),
                sc_core::sc_gen_unique_name("WriteCKENMethod"), &opt);
    }

    SC_METHOD(MonitorSVSTOPMethod);
    dont_initialize();
    sensitive << SVSTOP;
}//}}} 

/// Destructor
PWBRGA0::~PWBRGA0()
{//{{{
    delete mpPWBRGA0Func;
}//}}}

/// Initialize when reset activated
void PWBRGA0::Initialize(void)
{//{{{
    for (unsigned int i = 0; i < emNumClkOut; i++){
        mCKEN[i] = 0;
        mPendCountNum[i] = 0;
        mStartClockTime[i] = 0;
    }
    mTE = 0;
    mIsSVSTOP = 0;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Get reset status
bool PWBRGA0::GetResetStatus ()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Check clock is valid or not
bool PWBRGA0::CheckClockPeriod(std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    if (clock_name == PCLK.basename()) {
        clock_period = mPCLKPeriod;
    }else{// (clock_name == PCLKRW.basename()) 
        clock_period = mPCLKRWPeriod;
    }
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

/// Start counter n
void PWBRGA0::StartCounter(unsigned int next_te_value)
{//{{{
    // Update TEn bit
    double current_time = sc_time_stamp().to_double();
    mTE = next_te_value;
    mWriteTEBitEvent.cancel();
    mWriteTEBitEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, true, current_time, mStartPCLKTime), mTimeResolutionUnit);
}//}}}

/// Stop counter n
void PWBRGA0::StopCounter(unsigned int next_te_value)
{//{{{
    // Update TEn bit
    double current_time = sc_time_stamp().to_double();
    mTE = next_te_value;
    mWriteTEBitEvent.cancel();
    sc_time t (CalculateCLKPosEdge(mPCLKPeriod, true, current_time, mStartPCLKTime), mTimeResolutionUnit);
    sc_time delta_time (1, SC_FS);
    mWriteTEBitEvent.notify(t + delta_time);// need plus delta time for case TE = 0 at same time toggle signal

}//}}}

/// Update emulation status
void PWBRGA0::UpdateEmulationStatus()
{//{{{
    mIsSVSTOP =((!mpPWBRGA0Func->GetSVSDISBit()) && (SVSTOP.read()));
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor PCLK clock
void PWBRGA0::MonitorPCLKMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

/// Monitor PCLKRW clock
void PWBRGA0::MonitorPCLKRWMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLKRW.read();
    SetCLKFreq("PCLKRW", freq_value, "Hz");
}//}}}

/// Monitor PRESETZ port
void PWBRGA0::MonitorPRESETZMethod()
{//{{{
    bool reset = PRESETZ.read();
    if (reset == vpcl::emResetActive){
        mIsPRESETZInit = false;
        mIsResetHardActive = true;
        re_printf("info", "The reset port PRESETZ is asserted.\n");
        EnableReset(true);
    }else{
        if (mIsPRESETZInit){
            mIsPRESETZInit = false;
        }else{
            mIsResetHardActive = false;
            re_printf("info", "The reset port PRESETZ is de-asserted.\n");
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
void PWBRGA0::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETZ.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void PWBRGA0::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of PRESETZ is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

/// Reset model and its sub-instance if reset is actived
void PWBRGA0::EnableReset (const bool is_active)
{//{{{
    mpPWBRGA0Func->EnableReset(is_active);
    if(is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        for (unsigned int i = 0; i < emNumClkOut; i++){
            mWriteCKENEvent[i].notify();
        }
    }
}//}}}

/// Cancel events
void PWBRGA0::CancelEvents()
{//{{{
    for (unsigned int i = 0; i < emNumClkOut; i++){
        mWriteCKENEvent[i].cancel();
    }
    mWriteTEBitEvent.cancel();
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
/// Call function to write TE bit
void PWBRGA0::WriteTEBitMethod()
{//{{{
    if ( GetResetStatus() || (!CheckClockPeriod("PCLK")) ){
        return;
    }
    // get previous value of TE
    bool pre_te_value[emNumClkOut];
    for (unsigned int i = 0; i < emNumClkOut; i++){
        pre_te_value[i] = mpPWBRGA0Func->GetTEBit(i);
    }
    // Update TE bit
    mpPWBRGA0Func->WriteTEBit(mTE);
    // Check the change of TE and do according
    for (unsigned int i = 0; i < emNumClkOut; i++){
        if ( (((mTE >> i) & 0x1) == 1) && (!pre_te_value[i]) ){
            double current_time = sc_time_stamp().to_double();
            mWriteCKENEvent[i].cancel();
            if (!mIsSVSTOP){
                unsigned int brs_value = mpPWBRGA0Func->GetPWBA0BRSRegister(i);
                mCKEN[i] = 2 * brs_value * mPCLKPeriod;
                mStartClockTime[i] = 0;
                if (brs_value == 0){
                    mCKEN[i] = mPCLKPeriod;
                    mWriteCKENEvent[i].notify(mPCLKPeriod, mTimeResolutionUnit);
                }else{
                    mWriteCKENEvent[i].notify(brs_value*mPCLKPeriod, mTimeResolutionUnit);
                }
            }
        }
        if ( (((mTE >> i) & 0x1) == 0) && (pre_te_value[i]) ){
            mCKEN[i] = 0;
            mWriteCKENEvent[i].notify(mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

/// Write PWBA_CKEN port
void PWBRGA0::WriteCKENMethod(unsigned int index)
{//{{{
    PWBA_CKEN[index]->write(mCKEN[index]);
    mStartClockTime[index] = sc_time_stamp().to_double();// time of rising edge to calculate counter when resume
}//}}}

/// Monitor SVSTOP port
void PWBRGA0::MonitorSVSTOPMethod()
{//{{{
    if ( GetResetStatus() || (!CheckClockPeriod("PCLK")) ){
        return;
    }
    // Pending if SVSTOP = 1, and SVSDIS bit = 0
    double current_time = sc_time_stamp().to_double();
    UpdateEmulationStatus();
    if (mIsSVSTOP ){
        for (unsigned int i = 0; i < emNumClkOut; i++){
            mPendCountNum[i] = GetCounter(i);
            mCKEN[i] = 0;
            mWriteCKENEvent[i].notify(CalculateCLKPosEdge(mPCLKPeriod, true, current_time, mStartPCLKTime), mTimeResolutionUnit);
        }

    // Resume operation after pending
    }else{
        for (unsigned int i = 0; i < emNumClkOut; i++){
            if (mpPWBRGA0Func->GetTEBit(i)){
                unsigned int brs_value = mpPWBRGA0Func->GetPWBA0BRSRegister(i);
                if (brs_value > 0){
                    unsigned int remain_count = 2*brs_value - mPendCountNum[i];
                    mCKEN[i] = 2*brs_value*mPCLKPeriod;
                    mWriteCKENEvent[i].notify(CalculateCLKPosEdge(mPCLKPeriod, true, current_time, mStartPCLKTime) + remain_count * mPCLKPeriod, mTimeResolutionUnit);
                }else{
                    mCKEN[i] = mPCLKPeriod;
                    mWriteCKENEvent[i].notify(CalculateCLKPosEdge(mPCLKPeriod, true, current_time, mStartPCLKTime), mTimeResolutionUnit);
                }
            }
        }
    }
}//}}} 

/// Calculate counter value (used in resume after SVSTOP = 0)
unsigned int PWBRGA0::GetCounter(unsigned int index)
{//{{{
    double current_time = sc_time_stamp().to_double();
    sc_dt::uint64 period = (sc_dt::uint64)(current_time - mStartClockTime[index]);
    sc_dt::uint64 num_period = period/mPCLKPeriod;
    unsigned int brs_value = mpPWBRGA0Func->GetPWBA0BRSRegister(index);
    unsigned int ret = 0;
    if (brs_value > 0){
        if (mStartClockTime[index] == 0){
            ret = brs_value - (unsigned int)num_period;
        }else{
            ret = (unsigned int)(num_period % (2*(sc_dt::uint64)brs_value));
        }
    }
    return ret;
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void PWBRGA0::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    if (mEnableRegisterMessage){
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpPWBRGA0Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpPWBRGA0Func->RegisterHandler(cmd);
    }else{
        //Disable message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpPWBRGA0Func->RegisterHandler(cmd);

        //Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpPWBRGA0Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void PWBRGA0::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpPWBRGA0Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping info/warning/error message of register IF
void PWBRGA0::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: PWBRGA0_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","PWBRGA0_EnableRegisterMessage %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: PWBRGA0_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

/// Command to assert reset
void PWBRGA0::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETZ") {
        if ((!mIsResetHardActive) && (!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETZ) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETZ is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETZ.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void PWBRGA0::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent.notify(SC_ZERO_TIME);
            }
        }else if (word_vector[0] == "PCLKRW") {
            mPCLKRWOrgFreq = freq;
            ConvertClockFreq (mPCLKRWFreq, mPCLKRWFreqUnit, freq, unit);
            if (mPCLKRWFreq > 0) {
                mPCLKRWPeriod = (sc_dt::uint64)(((1/(double)mPCLKRWFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKRWPeriod = 0;
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void PWBRGA0::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        }else if (word_vector[0] == "PCLKRW") {
            re_printf("info","PCLKRW frequency is %0.0f %s\n", (double)mPCLKRWOrgFreq, mPCLKRWFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void PWBRGA0::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpPWBRGA0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void PWBRGA0::ReleaseRegister (const std::string reg_name)
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
            mpPWBRGA0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void PWBRGA0::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpPWBRGA0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void PWBRGA0::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpPWBRGA0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void PWBRGA0::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpPWBRGA0Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void PWBRGA0::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"PWBRGA0_MessageLevel (\"PWBRGA0 instance\", \"fatal|error|warning|info\")      Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"PWBRGA0_DumpRegisterRW (\"PWBRGA0 instance\", \"true/false\")                  Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"PWBRGA0_EnableRegisterMessage (true/false)                                     Enable/disable dumping info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"PWBRGA0_AssertReset (\"PWBRGA0 instance\", \"reset_name\", start_time, period) Assert and de-assert reset signal to the PWBRGA0 model");
            SC_REPORT_INFO(this->basename(),"PWBRGA0_SetCLKFreq (\"PWBRGA0 instance\", \"clock_name\", freq, \"unit\")      Set clock frequency value to PWBRGA0 model. Valid clock_name is PCLK or PCLKRW");
            SC_REPORT_INFO(this->basename(),"PWBRGA0_GetCLKFreq (\"PWBRGA0 instance\", \"clock_name\")                      Get clock frequency value of PWBRGA0 model. Valid clock_name is PCLK or PCLKRW");
            SC_REPORT_INFO(this->basename(),"PWBRGA0_ForceRegister (\"PWBRGA0 instance\", \"reg_name\", reg_value)          Force register with setting value");
            SC_REPORT_INFO(this->basename(),"PWBRGA0_ReleaseRegister (\"PWBRGA0 instance\", \"reg_name\")                   Release register from force value");
            SC_REPORT_INFO(this->basename(),"PWBRGA0_WriteRegister (\"PWBRGA0 instance\", \"reg_name\", reg_value)          Write a value to a register");
            SC_REPORT_INFO(this->basename(),"PWBRGA0_ReadRegister (\"PWBRGA0 instance\", \"reg_name\")                      Read a value from a register");
            SC_REPORT_INFO(this->basename(),"PWBRGA0_ListRegister (\"PWBRGA0 instance\")                                    Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of PWBRGA0_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of PWBRGA0_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
