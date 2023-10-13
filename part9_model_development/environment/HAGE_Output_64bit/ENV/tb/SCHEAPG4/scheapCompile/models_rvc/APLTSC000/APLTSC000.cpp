// ---------------------------------------------------------------------
// $Id: APLTSC000.cpp,v 1.3 2020/03/21 06:59:29 chuonghoangle Exp $
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

#include "APLTSC000.h"
#include "APLTSC000_Func.h"

/// Constructor and Destructor
APLTSC000::APLTSC000(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,APLTSC000_AgentController()
        ,PCLK("PCLK")
        ,PRESETn("PRESETn")
        ,app1_rst_n("app1_rst_n")
        ,SVSTOP("SVSTOP")
{//{{{ 
    mpAPLTSC000Func = new APLTSC000_Func((std::string)name, this);


    // Configure Slave socket
    setSlaveResetPort32(&PRESETn);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32((char*)"ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];

    mBusSlaveIf32[0]->setFuncModulePtr(mpAPLTSC000Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize variables
    mPCLKPeriod = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreq = 0;
    mPCLKFreqUnit = "Hz";
    for (int i = 0; i < emNumReset; i++){
        mResetCurVal[i]         = true;
        mResetPreVal[i]         = true;
        mIsResetHardActive[i]   = false;
        mIsResetCmdReceive[i]   = false;
        mIsResetCmdActive[i]    = false;
        mResetCmdPeriod[i]      = 0;
    }
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    
    mIsCounterRunning = false;
    mSvstop = false;        // svstop port value

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mEnableRegisterMessage = true;

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorPCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(MonitorPRESETnMethod);
    sensitive << PRESETn;
    
    SC_METHOD(Monitorapp1_rst_nMethod);
    sensitive << app1_rst_n;
    sensitive << mGetRMEvent;

    SC_METHOD(MonitorSVSTOPMethod);
    sensitive << SVSTOP;
    
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&APLTSC000::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&APLTSC000::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&APLTSC000::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
}//}}}

/// Destructor
APLTSC000::~APLTSC000()
{//{{{
    delete mpAPLTSC000Func;
}//}}}

/// Initialize when reset activated
void APLTSC000::Initialize(void)
{//{{{
    mEmulationMode = false; 
    mIsCounterRunning = false;
    mStartCounterValue = 0;
    mStartCounterTime = 0;
}//}}}


//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Get reset status
bool APLTSC000::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if(reset_name == "PRESETn") {
        if (mIsResetHardActive[emPRESETnId] || mIsResetCmdActive[emPRESETnId]) {
            reset_status = true;
        }
    }else { // (reset_name == "app1_rst_n")
        if (mIsResetHardActive[emapp1_rst_nId] || mIsResetCmdActive[emapp1_rst_nId]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

/// Check clock period 
bool APLTSC000::CheckClockPeriod ()
{//{{{ 
    bool clock_status = false;
    if (mPCLKFreq > 0) {
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
void APLTSC000::MonitorPRESETnMethod()
{//{{{
    mResetCurVal[emPRESETnId] = PRESETn.read();
    if (mResetCurVal[emPRESETnId] != mResetPreVal[emPRESETnId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[emPRESETnId] = mResetCurVal[emPRESETnId];
        // Reset signals are asynchronous
        mResetHardEvent[emPRESETnId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Monitor app1_rst_n port
void APLTSC000::Monitorapp1_rst_nMethod()
{//{{{
    if (mpAPLTSC000Func->GetRMReg() == 0){
        mResetCurVal[emapp1_rst_nId] = app1_rst_n.read();
        if (mResetCurVal[emapp1_rst_nId] != mResetPreVal[emapp1_rst_nId]) { //mResetPreVal is "true" at initial state
            mResetPreVal[emapp1_rst_nId] = mResetCurVal[emapp1_rst_nId];
            // Reset signals are asynchronous
            mResetHardEvent[emapp1_rst_nId].notify(SC_ZERO_TIME);
        }
    }
    // Init SVSDIS regardless RM bit
    if (mResetCurVal[emapp1_rst_nId] == vpcl::emResetActive){
        mpAPLTSC000Func->SetSVSDISReg(0);
    }
}//}}}

/// Reset model and its sub-instance if reset is actived
void APLTSC000::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events
        // Initialize variables
        Initialize();
    }

    mpAPLTSC000Func->EnableReset(is_active);
}//}}}


/// Monitor PCLK clock
void APLTSC000::MonitorPCLKMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

/// Check frequency value and frequency unit
void APLTSC000::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in)
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

/// Process reset function when reset port is active
void APLTSC000::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port app1_rst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
    } else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port app1_rst_n is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id] && mIsResetHardActive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
} //}}}
 
/// Process reset function when reset command is active
void APLTSC000::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else { //reset_id == emapp1_rst_nId
        reset_name = "app1_rst_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void APLTSC000::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else {
        reset_name = "app1_rst_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]);
}//}}}

//========================================================
//============ Operation               ===================
//========================================================

/// Call to start counter
void APLTSC000::NotifyUpdateRMbit()
{//{{{  
    mGetRMEvent.notify();
 }//}}} 
 
/// Call to start counter
void APLTSC000::StartCounter ()
{//{{{  
    if(mEmulationMode == true){
        // do nothing
    }else if(!(GetResetStatus("PRESETn") || GetResetStatus("app1_rst_n") || (CheckClockPeriod() == false))){
        //Start Counter
        mStartCounterValue = GetCounterInReg();
        mStartCounterTime = sc_time_stamp().to_double();
    }
}//}}}

/// Call to stop counter
void APLTSC000::StopCounter ()
{//{{{  
    re_printf("info", "Counter is stopped.\n");
    //Stop Counter
    mStartCounterValue = GetCounterInReg();
}//}}} 

/// Monitor SVSDIS 
void APLTSC000::MonitorSVSDIS()
{//{{{ 
    unsigned int svsdis = mpAPLTSC000Func->GetSVSDISReg();
    Emulation(mSvstop,svsdis);
}//}}}

/// Monitor SVSTOP port
void APLTSC000::MonitorSVSTOPMethod()
{//{{{
    mSvstop = SVSTOP.read();
    unsigned int svsdis = mpAPLTSC000Func->GetSVSDISReg();
    Emulation(mSvstop,svsdis);
}//}}}

/// Process emulation function
void APLTSC000::Emulation(bool svstop, unsigned int svsdis)
{//{{{ 
    if (svsdis == 0){
        if (svstop == true){
            re_printf("info", "Counter is processing emulation.\n");
            ReturnValueCounter();
            mEmulationMode = true; 
            mStartCounterValue = GetCounterInReg();
        }else{
            if ( (mpAPLTSC000Func->GetCSTReg() == 0x1) && (mEmulationMode == true) ){
                mEmulationMode = false;
                mStartCounterValue = GetCounterInReg();
                mStartCounterTime = sc_time_stamp().to_double();
            }
        }
    }else{
        if ( (mpAPLTSC000Func->GetCSTReg() == 0x1) && (mEmulationMode == true) ){
            mEmulationMode = false;
            mStartCounterValue = GetCounterInReg();
            mStartCounterTime = sc_time_stamp().to_double();
        }
    }
}//}}}

sc_dt::uint64 APLTSC000::GetCounterInReg()
{//{{{
    unsigned int high_value = mpAPLTSC000Func->GetCNTHReg();
    unsigned int low_value = mpAPLTSC000Func->GetCNTLReg();
    sc_dt::uint64 ret = ((sc_dt::uint64)high_value << 32) | ((sc_dt::uint64)low_value);
    return ret;
}//}}}

/// Call to return value counter
void APLTSC000::ReturnValueCounter ()
{//{{{
    if (!(GetResetStatus("PRESETn") || GetResetStatus("app1_rst_n") || (CheckClockPeriod() == false))){
        sc_dt::uint64 counter = 0;
        if ( (mpAPLTSC000Func->GetCSTReg() == 0x1) && (mEmulationMode == false) ){
            sc_dt::uint64 num_steps = CalCounterStep();
            counter = num_steps + mStartCounterValue;
        }else{
            counter = mStartCounterValue;
        }
        unsigned int CNTHValue = (unsigned int)((counter >> 32) & 0xFFFFFFFF);
        unsigned int CNTLValue = (unsigned int)(counter & 0xFFFFFFFF);
        mpAPLTSC000Func->SetCNTLReg(CNTLValue);
        mpAPLTSC000Func->SetCNTHReg(CNTHValue);
    }
}//}}}

void APLTSC000::UpdateCounterValue()
{//{{{
    if ( (mpAPLTSC000Func->GetCSTReg() == 0x0) || (mEmulationMode == true) ){
        mStartCounterValue = GetCounterInReg();
    }
}//}}}

/// Calculate the counter step from start timer
sc_dt::uint64 APLTSC000::CalCounterStep()
{//{{{
    double current_time = sc_time_stamp().to_double();
    double duration_time = current_time - mStartCounterTime;
    sc_dt::uint64 count_step = 0;

    if (sc_get_time_resolution() > sc_time((double)mPCLKPeriod, mTimeResolutionUnit)) {
        sc_dt::uint64 rate = mPCLKFreq / mTimeResolutionValue; // 1 time unit, counter counts up "rate" times
        sc_dt::uint64 temp = emMax/rate;
        sc_dt::uint64 num_loop = (sc_dt::uint64)(duration_time/(double)temp + ((double)temp-1)/(double)temp);
        for (sc_dt::uint64 i = 0; i < num_loop; i++){
            if (i == num_loop -1){// last
                count_step += (sc_dt::uint64)((sc_dt::uint64)duration_time - temp*(num_loop -1)) * rate;
            }else{
                count_step += temp*rate;
            }
        }
    }else{
        count_step += (duration_time/mPCLKPeriod);
    }
    return count_step;
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void APLTSC000::SetMessageLevel (const std::string msg_lv)
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

    mpAPLTSC000Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    mpAPLTSC000Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable info/warning/error message of register IF
void APLTSC000::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: APLTSC000_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","APLTSC000_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: APLTSC000_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

/// Enable/disable dumping message when users access registers
void APLTSC000::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpAPLTSC000Func->RegisterHandler(cmd);
}//}}}

/// Dump status register's value
void APLTSC000::DumpStatusInfo ()
{//{{{
    mpAPLTSC000Func->DumpStatusInfo();
}//}}}

/// Command to assert reset
void APLTSC000::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETnId])&&(!mIsResetCmdReceive[emPRESETnId])) {
            mIsResetCmdReceive[emPRESETnId] = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPRESETnId].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETnId] = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else if (reset_name == "app1_rst_n") {
        if ((!mIsResetHardActive[emapp1_rst_nId])&&(!mIsResetCmdReceive[emapp1_rst_nId])) {
            mIsResetCmdReceive[emapp1_rst_nId] = true;
            re_printf("info","The model will be reset (app1_rst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emapp1_rst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[emapp1_rst_nId] = period;
        } else {
            re_printf("warning","The software reset of app1_rst_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be preset_n or app1_rst_n.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void APLTSC000::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            // Get the counter value and store it into counter register before clock is zero
            // If clock is zero, we cannot get the counter value by ReturnValueCounter()
            ReturnValueCounter();
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                // Start counter if counter have already been started beforehand (LTSCnCST = 1)
                if (mpAPLTSC000Func->GetCSTReg() == 0x1) {
                    StartCounter();
                }
            } else {
                mPCLKPeriod = 0;
                // Stop counter if counter have already been started beforehand (LTSCnCST = 1)
                if (mpAPLTSC000Func->GetCSTReg() == 0x1) {
                    StopCounter();
                }
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void APLTSC000::GetCLKFreq (const std::string clock_name)
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
void APLTSC000::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpAPLTSC000Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void APLTSC000::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpAPLTSC000Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void APLTSC000::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpAPLTSC000Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void APLTSC000::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpAPLTSC000Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void APLTSC000::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpAPLTSC000Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void APLTSC000::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"APLTSC000_MessageLevel (\"APLTSC000 instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"APLTSC000_DumpRegisterRW (\"APLTSC000 instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"APLTSC000_DumpStatusInfo (\"APLTSC000 instance\")                                    Dump the status info of model");
            SC_REPORT_INFO(this->basename(),"APLTSC000_AssertReset (\"APLTSC000 instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the APLTSC000 model");
            SC_REPORT_INFO(this->basename(),"APLTSC000_SetCLKFreq (\"APLTSC000 instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to APLTSC000 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"APLTSC000_GetCLKFreq (\"APLTSC000 instance\", \"clock_name\")                        Get clock frequency value of APLTSC000 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"APLTSC000_ForceRegister (\"APLTSC000 instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(),"APLTSC000_ReleaseRegister (\"APLTSC000 instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(),"APLTSC000_WriteRegister (\"APLTSC000 instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(),"APLTSC000_ReadRegister (\"APLTSC000 instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(),"APLTSC000_ListRegister (\"APLTSC000 instance\")                                      Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of APLTSC000_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of APLTSC000_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
