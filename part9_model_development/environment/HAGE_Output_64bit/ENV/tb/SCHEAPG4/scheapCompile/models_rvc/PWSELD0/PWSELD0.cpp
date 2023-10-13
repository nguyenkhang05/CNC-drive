// ---------------------------------------------------------------------
// $Id: PWSELD0.cpp,v 1.13 2020/01/15 13:23:55 landang Exp $
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

#include "PWSELD0.h"
#include "PWSELD0_Func.h"

/// Constructor and Destructor
PWSELD0::PWSELD0(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,PWSELD0_AgentController()
        ,rvc_common_model()
        ,PCLK("PCLK")
        ,PRESETZ("PRESETZ")
        ,SVSTOP("SVSTOP")
        ,PWS_PVCR_VALUE("PWS_PVCR_VALUE")
        ,PWS_INT_QFULL("PWS_INT_QFULL")
{//{{{ 
    // Initialize ports
    PWS_INT_QFULL.initialize(false);
    PWS_PVCR_VALUE.initialize(0x0);
    // New instance and port
    mpPWSELD0Func = new PWSELD0_Func((std::string)name, this);
    for (unsigned int i = 0; i < emNumConnectedPWG; i++){
        std::ostringstream port_name;
        port_name.str("");
        port_name << "PWG_TRGOUT" << i;
        PWG_TRGOUT[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(PWG_TRGOUT[i] != NULL);
    }
    for (unsigned int i = 0; i < emNumConnectedADC; i++){
        std::ostringstream port_name;
        port_name.str("");
        port_name << "ADC_CONV_END" << i;
        ADC_CONV_END[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(ADC_CONV_END[i] != NULL);
    }
    for (unsigned int i = 0; i < emNumConnectedADC; i++){
        std::ostringstream port_name;
        port_name.str("");
        port_name << "ADC_PWDDR" << i;
        ADC_PWDDR[i] = new sc_in<unsigned short>(port_name.str().c_str());
        sc_assert(ADC_PWDDR[i] != NULL);
    }
    for (unsigned int i = 0; i < emNumConnectedADC; i++){
        std::ostringstream port_name;
        port_name.str("");
        port_name << "PWS_ADTRG" << i;
        PWS_ADTRG[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(PWS_ADTRG[i] != NULL);
        PWS_ADTRG[i]->initialize(false);
    }
    
    // Configure Slave socket
    BusSlaveBase<32, 1>::setSlaveResetPort32(&PRESETZ);
    BusSlaveBase<32, 1>::setSlaveFreqPort32(&PCLK);
    BusSlaveBase<32, 1>::setTargetSocket32("ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setFuncModulePtr(mpPWSELD0Func);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setReadLatency(rLatency);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize variables
    mIsPRESETZInit = true;
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
    mDumpInterrupt = false;
    mEnableRegisterMessage = true;
    for (unsigned int i = 0; i < emNumConnectedPWG; i++){
        mPrePWG_TRGOUT[i] = 0; // store previous port of PWG_TRGOUT, not need initialize after reset
    }

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorPCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(MonitorPRESETZMethod);
    sensitive << PRESETZ;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(MonitorPWG_TRGOUTMethod);
    dont_initialize();
    for (unsigned int i = 0; i < emNumConnectedPWG; i++){
        sensitive << *PWG_TRGOUT[i];
    }

    for (unsigned int i = 0; i < 4; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStoreQueueEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PWSELD0::StoreQueueMethod, this, i),
                sc_core::sc_gen_unique_name("StoreQueueMethod"), &opt);
    }

    SC_METHOD(IssueADTRGMethod);
    dont_initialize();
    sensitive << mIssueADTRGEvent;

    SC_METHOD(WriteOutputToADCMethod);
    dont_initialize();
    sensitive << mWriteOutputToADCEvent;

    SC_METHOD(NegatePWS_ADTRGMethod);
    dont_initialize();
    sensitive << mNegatePWS_ADTRGEvent;

    SC_METHOD(MonitorADC_CONV_ENDMethod);
    dont_initialize();
    for (unsigned int i = 0; i < emNumConnectedADC; i++){
        sensitive << *ADC_CONV_END[i];
    }

    SC_METHOD(ReceiveADCMethod);
    dont_initialize();
    sensitive << mReceiveADCEvent;

    SC_METHOD(MonitorSVSTOPMethod);
    dont_initialize();
    sensitive << SVSTOP;

    SC_METHOD(SetDelayedSVSTOPMethod);
    dont_initialize();
    sensitive << mSetDelayedSVSTOPEvent;

    SC_METHOD(WritePWS_INT_QFULLMethod);
    dont_initialize();
    sensitive << mWritePWS_INT_QFULLEvent;

    for (unsigned int i = 0; i < emNumConnectedPWG; i++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mClearFlagEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PWSELD0::ClearFlagMethod, this, i),
                sc_core::sc_gen_unique_name("ClearFlagMethod"), &opt);
    }
    
}//}}} 

/// Destructor
PWSELD0::~PWSELD0()
{//{{{
    delete mpPWSELD0Func;
}//}}}

/// Initialize when reset activated
void PWSELD0::Initialize(void)
{//{{{
    for (unsigned int index = 0; index < 4; index++){
        for (unsigned int i = 0; i < emNumConnectedPWG; i++){
            mPWG_TRGOUT[index][i] = 0;
        }
    }
    for (unsigned int i = 0; i < emNumConnectedADC; i++){
        mPWS_ADTRG[i] = 0;
        mPreADC_CONV_END[i] = 0;
    }
    mPWS_PVCR_VALUE = 0;
    mPWS_INT_QFULL = 0;
    mEnablePWS = 0;
    mDelayedSVSTOP = 0;
    mSVSTOP = 0;
    mPendingADCInput = 0;
    mIndexCounter = 0;
    mIsWaitConvEnd = false;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Get reset status
bool PWSELD0::GetResetStatus()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Check clock is valid or not
bool PWSELD0::CheckClockPeriod()
{//{{{
    bool ret = false;
    if (mPCLKPeriod > 0){
        ret = true;
    }else{
        ret = false;
        re_printf("info","PCLK frequency is zero\n");
    }
    return ret;
}//}}}

/// Inform disable PWS (from PWSELD0_Func when writing to ENBL bit)
void PWSELD0::EnablePWS(bool is_enable)
{//{{{
    mEnablePWS = is_enable;
    // Clear PWS_ADTRGn, PWS_PVCR_VALUE, PWS_INT_QFULL in case disable PWS
    if (!is_enable){
        CancelEvents();
        for (unsigned int i = 0; i < emNumConnectedADC; i++){
            mPWS_ADTRG[i] = 0;
            mPreADC_CONV_END[i] = 0;
        }
        mPWS_PVCR_VALUE = 0;
        mWriteOutputToADCEvent.notify();
        mPWS_INT_QFULL = 0;
        mWritePWS_INT_QFULLEvent.notify();
        mPendingADCInput = 0;
    }
}//}}}

/// Assert interrupt PWS_INT_QFULL
void PWSELD0::AssertInterrupt()
{//{{{
    mPWS_INT_QFULL = 1;
    double current_time = sc_time_stamp().to_double();
    mWritePWS_INT_QFULLEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
}//}}}

/// Clear flag in PWS0PWDDIR register after 1 cycle
void PWSELD0::ClearFlag(unsigned int channel)
{//{{{ 
    mClearFlagEvent[channel].notify(mPCLKPeriod, mTimeResolutionUnit);
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor PCLK clock
void PWSELD0::MonitorPCLKMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

/// Monitor PRESETZ port
void PWSELD0::MonitorPRESETZMethod()
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
void PWSELD0::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETZ.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void PWSELD0::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of PRESETZ is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

/// Reset model and its sub-instance if reset is actived
void PWSELD0::EnableReset (const bool is_active)
{//{{{
    mpPWSELD0Func->EnableReset(is_active);
    if(is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        mWriteOutputToADCEvent.notify();
        mWritePWS_INT_QFULLEvent.notify();
    }
}//}}}

/// Cancel events
void PWSELD0::CancelEvents()
{//{{{
    for (unsigned int i = 0; i < 4; i++){
        mStoreQueueEvent[i].cancel();
    }
    mIssueADTRGEvent.cancel();
    mWriteOutputToADCEvent.cancel();
    mNegatePWS_ADTRGEvent.cancel();
    mReceiveADCEvent.cancel();
    mSetDelayedSVSTOPEvent.cancel();
    mWritePWS_INT_QFULLEvent.cancel();
    for (unsigned int i = 0; i < emNumConnectedPWG; i++){
        mClearFlagEvent[i].cancel();
    }
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
/// Monitor PWG_TRGOUT input port
void PWSELD0::MonitorPWG_TRGOUTMethod()
{//{{{
    if (mEnablePWS && CheckClockPeriod()){
        // Store request
        for (unsigned int i = 0; i < emNumConnectedPWG; i++){
            bool read_pwg_trgout = PWG_TRGOUT[i]->read();
            if ((mPrePWG_TRGOUT[i] != read_pwg_trgout) && read_pwg_trgout){// change from 0 to 1
                mPWG_TRGOUT[mIndexCounter][i] = 1;
            }else{
                mPWG_TRGOUT[mIndexCounter][i] = 0;
            }
            mPrePWG_TRGOUT[i] = read_pwg_trgout;
        }
        // Store request to queue if any
        for (unsigned int i = 0; i < emNumConnectedPWG; i++){
            if (mPWG_TRGOUT[mIndexCounter][i] > 0){
                double current_time = sc_time_stamp().to_double();
                mStoreQueueEvent[mIndexCounter].notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mStartPCLKTime) + 3*mPCLKPeriod, mTimeResolutionUnit);
                mIndexCounter = (mIndexCounter + 1) % 4;
                return;
            }
        }
    }
}//}}}

/// Store trigger channel to queue method
void PWSELD0::StoreQueueMethod(unsigned int time_index)
{//{{{
    for (unsigned int i = 0; i < emNumConnectedPWG; i++){
        // If have trigger -> store trigger channel to queue if any
        if (mPWG_TRGOUT[time_index][i]){
            bool can_store_trgoutchannel = mpPWSELD0Func->StoreTriggerToQueue(i);

            if (can_store_trgoutchannel){
                mIssueADTRGEvent.notify(mPCLKPeriod, mTimeResolutionUnit);
            }else{
                break;// break when no space to store request
            }
        }
    }
}//}}}

/// Prepare value to mPWS_ADTRG[selected_adc] and mPWS_PVCR_VALUE and notify event to write them
void PWSELD0::IssueADTRGMethod()
{//{{{
    if (mEnablePWS && CheckClockPeriod()){
        unsigned int trg_pwg_ch = mpPWSELD0Func->GetTopQueue();
        if (!mIsWaitConvEnd){ // Issue AD trigger if not waiting converted result from ADC
            if (trg_pwg_ch != emEmpty){
                // Prepare value to mPWS_ADTRG[selected_adc] and mPWS_PVCR_VALUE 
                unsigned int selected_adc = mpPWSELD0Func->GetSLADxBitOfChannel(trg_pwg_ch);
                mPWS_ADTRG[selected_adc] = 1;
                mPWS_PVCR_VALUE = mpPWSELD0Func->GetPVCRValueOfChannel(trg_pwg_ch);
                // Issue PWS_ADTRG and PWS_PVCR_VALUE if not in SVSTOP
                if (!mDelayedSVSTOP){
                    mWriteOutputToADCEvent.notify();
                    mIsWaitConvEnd = true;
                }
            }
        }
    }
}//}}}

/// Write value to PWS_ADTRG and PWS_PVCR_VALUE
void PWSELD0::WriteOutputToADCMethod()
{//{{{
    PWS_PVCR_VALUE.write(mPWS_PVCR_VALUE);
    // Write out value
    for (unsigned int i = 0; i < emNumConnectedADC; i++){
        PWS_ADTRG[i]->write(mPWS_ADTRG[i]);
    }

    // Pulse shape for PWS_ADTRG[i]
    for (unsigned int i = 0; i < emNumConnectedADC; i++){
        if (mPWS_ADTRG[i] == 1){// Negate all PWS_ADTRG if any port is 1
            mNegatePWS_ADTRGEvent.notify(mPCLKPeriod, mTimeResolutionUnit);
            break;
        }
    }
}//}}}

/// Negate PWS_ADTRG ports
void PWSELD0::NegatePWS_ADTRGMethod()
{//{{{
    for (unsigned int i = 0; i < emNumConnectedADC; i++){
        mPWS_ADTRG[i] = 0;
    }
    mWriteOutputToADCEvent.notify();
}//}}}

/// Monitor ADC_CONV_END port
void PWSELD0::MonitorADC_CONV_ENDMethod()
{//{{{
    if (mEnablePWS && CheckClockPeriod()){
        // Only process when selected ADC convert end
        unsigned int trg_pwg_ch = mpPWSELD0Func->GetTopQueue();
        if ((trg_pwg_ch != emEmpty) && mIsWaitConvEnd){
            unsigned int selected_adc = mpPWSELD0Func->GetSLADxBitOfChannel(trg_pwg_ch);
            if ( (mPreADC_CONV_END[selected_adc] == 0) && (ADC_CONV_END[selected_adc]->read() == 1) ){
                if (!mSVSTOP){
                    double current_time = sc_time_stamp().to_double();
                    mReceiveADCEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mStartPCLKTime) + 2*mPCLKPeriod, mTimeResolutionUnit);
                }
                mPendingADCInput = true;// Mark that has pending ADC Input to process
            }

        }
        // Update previous value of ADC_CONV_END
        for (unsigned int i = 0; i < emNumConnectedADC; i++){
            mPreADC_CONV_END[i] = ADC_CONV_END[i]->read();
        }
    }
}//}}}

/// Receive ADC cnoversion result
void PWSELD0::ReceiveADCMethod()
{//{{{
    if (mPendingADCInput){
        // Store conversion value if ARSE bit = 1
        if (mpPWSELD0Func->GetARSEBit() == 1){
            unsigned int trg_pwg_ch = mpPWSELD0Func->GetTopQueue();
            if (trg_pwg_ch != emEmpty){
                unsigned int selected_adc = mpPWSELD0Func->GetSLADxBitOfChannel(trg_pwg_ch);
                mpPWSELD0Func->StorePWS0PWDDIR(trg_pwg_ch, (unsigned int)ADC_PWDDR[selected_adc]->read());
                mIsWaitConvEnd = false;
            }
        }

        // Shift queue, continue another ADC trigger
        mpPWSELD0Func->ShiftQueue();
        mPendingADCInput = false;
        mIssueADTRGEvent.notify(mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/// Monitor SVSTOP port
void PWSELD0::MonitorSVSTOPMethod()
{//{{{
    double current_time = sc_time_stamp().to_double();
    // Pending
    if ( (mpPWSELD0Func->GetSVSDISBit() == 0) && (SVSTOP.read() == 1) ){
        mSVSTOP = true;
        mSetDelayedSVSTOPEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mStartPCLKTime) + mPCLKPeriod/2, mTimeResolutionUnit);
    // Resume
    }else{
        mSVSTOP = false;
        mDelayedSVSTOP = false;
        if (!mPendingADCInput && !mIsWaitConvEnd){
            mIssueADTRGEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mStartPCLKTime) + mPCLKPeriod, mTimeResolutionUnit);
        }
        if (mPendingADCInput){
            mReceiveADCEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mStartPCLKTime) + mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

/// Set mDelayedSVSTOP. This variable is used for case SVSTOP and issue ADC trigger
void PWSELD0::SetDelayedSVSTOPMethod()
{//{{{
    mDelayedSVSTOP = true;
}//}}}

/// Write PWS_INT_QFULL method
void PWSELD0::WritePWS_INT_QFULLMethod()
{//{{{
    PWS_INT_QFULL.write(mPWS_INT_QFULL);
    if (mPWS_INT_QFULL){
        if (mDumpInterrupt){
            re_printf("info", "PWS_INT_QFULL is asserted.\n");
        }
        mPWS_INT_QFULL = false;
        mWritePWS_INT_QFULLEvent.notify(mPCLKPeriod, mTimeResolutionUnit);
    }else{
        if (mDumpInterrupt){
            re_printf("info", "PWS_INT_QFULL is negated.\n");
        }
    }
}//}}}

/// Clear flag in PWS0PWDDIR register
void PWSELD0::ClearFlagMethod(unsigned int channel)
{//{{{ 
    mpPWSELD0Func->ClearFlag(channel);
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void PWSELD0::SetMessageLevel (const std::string msg_lv)
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

        mpPWSELD0Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpPWSELD0Func->RegisterHandler(cmd);
    }else{
        //Disable message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpPWSELD0Func->RegisterHandler(cmd);

        //Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpPWSELD0Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void PWSELD0::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpPWSELD0Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt message when it is asserted
void PWSELD0::DumpInterrupt (const std::string is_enable) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: PWSELD0_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","PWSELD0_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: PWSELD0_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Enable/disable dumping info/warning/error message of register IF
void PWSELD0::EnableRegisterMessage (const std::string is_enable) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: PWSELD0_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","PWSELD0_EnableRegisterMessage %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: PWSELD0_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

/// Command to assert reset
void PWSELD0::AssertReset (const std::string reset_name, const double start_time, const double period)
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
void PWSELD0::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void PWSELD0::GetCLKFreq (const std::string clock_name)
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
void PWSELD0::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpPWSELD0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void PWSELD0::ReleaseRegister (const std::string reg_name)
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
            mpPWSELD0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void PWSELD0::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpPWSELD0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void PWSELD0::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpPWSELD0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void PWSELD0::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpPWSELD0Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void PWSELD0::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"PWSELD0_MessageLevel (\"PWSELD0 instance\", \"fatal|error|warning|info\")  Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"PWSELD0_DumpRegisterRW (\"PWSELD0 instance\", \"true/false\")              Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"PWSELD0_DumpInterrupt (true/false)                                         Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(),"PWSELD0_EnableRegisterMessage (true/false)                                 Enable/disable dumping info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"PWSELD0_AssertReset (\"PWSELD0 instance\", \"reset_name\", start_time, period) Assert and de-assert reset signal to the PWSELD0 model");
            SC_REPORT_INFO(this->basename(),"PWSELD0_SetCLKFreq (\"PWSELD0 instance\", \"clock_name\", freq, \"unit\")      Set clock frequency value to PWSELD0 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"PWSELD0_GetCLKFreq (\"PWSELD0 instance\", \"clock_name\")                      Get clock frequency value of PWSELD0 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"PWSELD0_ForceRegister (\"PWSELD0 instance\", \"reg_name\", reg_value)          Force register with setting value");
            SC_REPORT_INFO(this->basename(),"PWSELD0_ReleaseRegister (\"PWSELD0 instance\", \"reg_name\")                   Release register from force value");
            SC_REPORT_INFO(this->basename(),"PWSELD0_WriteRegister (\"PWSELD0 instance\", \"reg_name\", reg_value)          Write a value to a register");
            SC_REPORT_INFO(this->basename(),"PWSELD0_ReadRegister (\"PWSELD0 instance\", \"reg_name\")                      Read a value from a register");
            SC_REPORT_INFO(this->basename(),"PWSELD0_ListRegister (\"PWSELD0 instance\")                                    Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of PWSELD0_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of PWSELD0_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
