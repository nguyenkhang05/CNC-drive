// ---------------------------------------------------------------------
// $Id: ECC_PRAM_E2.cpp,v 1.9 2019/09/26 10:07:42 landang Exp $
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
#include "ECC_PRAM_E2.h"
#include "ECC_PRAM_E2_Func.h"

/// Constructor and Destructor
ECC_PRAM_E2::ECC_PRAM_E2(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,ECC_PRAM_E2_AgentController()
        // Clock/Reset
        ,pclk("pclk")
        ,presetn("presetn")
        ,ram_clk("ram_clk")
        ,ecterve("ecterve")
        ,ectere("ectere")
        ,ectiovf("ectiovf")
        ,ectie2("ectie2")
        ,ectie1("ectie1")
        ,ectpref("ectpref")
{//{{{ 
    // New instance and port
    mpECC_PRAM_E2Func = new ECC_PRAM_E2_Func((std::string)name, this);
    
    // Configure Slave socket
    BusSlaveBase<32, 1>::setSlaveResetPort32(&presetn);
    BusSlaveBase<32, 1>::setSlaveFreqPort32(&pclk);
    BusSlaveBase<32, 1>::setTargetSocket32((char*)"ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setFuncModulePtr(mpECC_PRAM_E2Func);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setReadLatency(rLatency);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize remain output ports
    ectiovf.initialize(false);
    ectie2.initialize(false);
    ectie1.initialize(false);
    ectpref.initialize(false);

    // Initialize variables
    mEnableRegisterMessage = true;
    for (int i = 0; i < emNumClk; i++){
        mClkPeriod[i]       = 0;
        mClkOrgFreq[i]      = 0;
        mClkFreq[i]         = 0;
        mClkFreqUnit[i]     = "Hz";
        mStartClkTime[i]    = 0;
    }
    mIspresetnInit          = true;
    mIsResetHardActive      = false;
    mIsResetCmdReceive      = false;
    mIsResetCmdActive       = false;
    mResetCmdPeriod         = 0;

    mTimeResolutionValue    = 1;
    mTimeResolutionUnit     = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mWORD           = DEFAULT_WORD;
    mERR_STAG       = DEFAULT_ERR_STAG;
    mDumpInterrupt  = false;

    mIsAddrConfig   = false;
    mErrorAddress   = 0;
    mECTERVESig     = false;
    mECTERESig      = false;

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorpclkMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(Monitorram_clkMethod);
    dont_initialize();
    sensitive << ram_clk;

    SC_METHOD(presetnMethod);
    sensitive << presetn;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(MonitorecterveMethod);
    dont_initialize();
    sensitive << ecterve;

    SC_METHOD(MonitorectereMethod);
    dont_initialize();
    sensitive << ectere;

    SC_METHOD(WriteECTIE1Method);
    dont_initialize();
    sensitive << mWriteECTIE1Event;

    SC_METHOD(WriteECTIE2Method);
    dont_initialize();
    sensitive << mWriteECTIE2Event;

    SC_METHOD(NegateECTIE1Method);
    dont_initialize();
    sensitive << mNegateECTIE1Event;

    SC_METHOD(NegateECTIE2Method);
    dont_initialize();
    sensitive << mNegateECTIE2Event;

    SC_METHOD(WriteECTIOVFMethod);
    dont_initialize();
    sensitive << mWriteECTIOVFEvent;

    SC_METHOD(NegateECTIOVFMethod);
    dont_initialize();
    sensitive << mNegateECTIOVFEvent;

    SC_METHOD(WriteECTPREFMethod);
    dont_initialize();
    sensitive << mWriteECTPREFEvent;

    SC_METHOD(NegateECTPREFMethod);
    dont_initialize();
    sensitive << mNegateECTPREFEvent;

    SC_METHOD(ClearECEMFMethod);
    dont_initialize();
    sensitive << mClearECEMFEvent;

    SC_METHOD(Process1bitErrorMethod);
    dont_initialize();
    sensitive << mProcess1bitErrorEvent;

    SC_METHOD(ProcessFatalErrorMethod);
    dont_initialize();
    sensitive << mProcessFatalErrorEvent;
}//}}} 

/// Destructor
ECC_PRAM_E2::~ECC_PRAM_E2()
{//{{{
    delete mpECC_PRAM_E2Func;
}//}}}

/// Initialize when reset activated
void ECC_PRAM_E2::Initialize(void)
{//{{{
    mECTIOVF    = false;
    mECTIE2     = false;
    mECTIE1     = false;
    mECTPREF    = false;
    mHasFatalError = false;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Set ERR_STAG parameter
unsigned int ECC_PRAM_E2::GetERR_STAG()
{//{{{
    return mERR_STAG;
}//}}}

/// Get value of ECTERVE port
bool ECC_PRAM_E2::GetECTERVESig()
{//{{{
    return mECTERVESig;
}//}}}

/// Get WORD parameter
unsigned int ECC_PRAM_E2::GetWORD()
{//{{{
    return mWORD;
}//}}}

/// Assert ectiovf
void ECC_PRAM_E2::AssertECTIOVFPort()
{//{{{
    mECTIOVF = true;
    mWriteECTIOVFEvent.notify();
}//}}}

/// Assert ectie1
void ECC_PRAM_E2::AssertECTIE1Port()
{//{{{
    mECTIE1 = true;
    mWriteECTIE1Event.notify();
}//}}}

/// Assert ectie2
void ECC_PRAM_E2::AssertECTIE2Port()
{//{{{
    mECTIE2 = true;
    mWriteECTIE2Event.notify();
}//}}}

/// Drive ectpref port
void ECC_PRAM_E2::DriveECTPREFPort()
{//{{{
    bool err_judge_en = mpECC_PRAM_E2Func->CheckPermitJudgeError();
    unsigned int exctl = mpECC_PRAM_E2Func->GetExCTLReg();
    bool ecthm = (bool)((exctl >> 7) & 0x1);
    bool ec2edic = (bool)((exctl >> 4) & 0x1);
    mECTPREF = err_judge_en & mHasFatalError & (mECTERESig | (ecthm & ec2edic));
    mHasFatalError = false;
    mWriteECTPREFEvent.notify();
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor pclk clock
void ECC_PRAM_E2::MonitorpclkMethod()
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

/// Monitor ram_clk clock
void ECC_PRAM_E2::Monitorram_clkMethod()
{//{{{
    sc_dt::uint64 freq_value = ram_clk.read();
    SetCLKFreq("ram_clk", freq_value, "Hz");
}//}}}

/// Monitor presetn port
void ECC_PRAM_E2::presetnMethod()
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
void ECC_PRAM_E2::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of presetn.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void ECC_PRAM_E2::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of presetn is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

/// Reset model and its sub-instance if reset is actived
void ECC_PRAM_E2::EnableReset (const bool is_active)
{//{{{
    mpECC_PRAM_E2Func->EnableReset(is_active);
    if(is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        mWriteECTIOVFEvent.cancel();
        mWriteECTIE2Event.cancel();
        mWriteECTIE1Event.cancel();
        mWriteECTPREFEvent.cancel();
    }
}//}}}

/// Cancel events
void ECC_PRAM_E2::CancelEvents()
{//{{{
    mWriteECTIOVFEvent.cancel();
    mWriteECTIE2Event.cancel();
    mWriteECTIE1Event.cancel();
    mNegateECTIE1Event.cancel();
    mNegateECTIE2Event.cancel();
    mNegateECTIOVFEvent.cancel();
    mWriteECTPREFEvent.cancel();
    mNegateECTPREFEvent.cancel();
    mClearECEMFEvent.cancel();
    mProcess1bitErrorEvent.cancel();
    mProcessFatalErrorEvent.cancel();
}//}}}

/// Get reset status
bool ECC_PRAM_E2::GetResetStatus ()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Check clock period 
bool ECC_PRAM_E2::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "pclk") ? mClkPeriod[empclkId] : mClkPeriod[emram_clkId];
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}} 

//========================================================
//============ Operation               ===================
//========================================================
/// Monitor ecterve port
void ECC_PRAM_E2::MonitorecterveMethod()
{//{{{
    mECTERVESig = ecterve.read();
    mpECC_PRAM_E2Func->UpdateErrorJudgement();
}//}}}

/// Monitor ectere port
void ECC_PRAM_E2::MonitorectereMethod()
{//{{{
    mECTERESig = ectere.read();
    DriveECTPREFPort();
}//}}}

/// Write ectie1 port
void ECC_PRAM_E2::WriteECTIE1Method()
{//{{{
    ectie1.write(mECTIE1);
    if (mDumpInterrupt){
        re_printf("info", "ectie1 is changed to %d\n", (unsigned int)mECTIE1);
    }
    // Negate if any
    if (mECTIE1){
        mNegateECTIE1Event.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write ectie2 port
void ECC_PRAM_E2::WriteECTIE2Method()
{//{{{
    ectie2.write(mECTIE2);
    if (mDumpInterrupt){
        re_printf("info", "ectie2 is changed to %d\n", (unsigned int)mECTIE2);
    }
    // Negate if any
    if (mECTIE2){
        mNegateECTIE2Event.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate ectie1 port
void ECC_PRAM_E2::NegateECTIE1Method()
{//{{{
    mECTIE1 = false;
    mWriteECTIE1Event.notify();
}//}}}

/// Negate ectie2 port
void ECC_PRAM_E2::NegateECTIE2Method()
{//{{{
    mECTIE2 = false;
    mWriteECTIE2Event.notify();
}//}}}

/// Write ectiovf port
void ECC_PRAM_E2::WriteECTIOVFMethod()
{//{{{
    ectiovf.write(mECTIOVF);
    if (mDumpInterrupt){
        re_printf("info", "ectiovf is changed to %d\n", (unsigned int)mECTIOVF);
    }
    // Negate if any
    if (mECTIOVF){
        mNegateECTIOVFEvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate ectiovf port
void ECC_PRAM_E2::NegateECTIOVFMethod()
{//{{{
    mECTIOVF = false;
    mWriteECTIOVFEvent.notify();
}//}}}

/// Write ectpref port
void ECC_PRAM_E2::WriteECTPREFMethod()
{//{{{
    ectpref.write(mECTPREF);
    // Negate if any
    if (mECTPREF){
        double current_time = sc_time_stamp().to_double();
        mNegateECTPREFEvent.notify(CalculateCLKPosEdge((double)mClkPeriod[emram_clkId], true, current_time, mStartClkTime[emram_clkId]), mTimeResolutionUnit);
    }
}//}}}

/// Negate ectpref port
void ECC_PRAM_E2::NegateECTPREFMethod()
{//{{{
    mECTPREF = false;
    mWriteECTPREFEvent.notify();
    mHasFatalError = false;
}//}}}

/// Clear ECEMF bit method
void ECC_PRAM_E2::ClearECEMFMethod()
{//{{{
    mpECC_PRAM_E2Func->SetECEMFbit(0);
}//}}}

/// Process 1bit error FIFO: update FIFO, assert error notification, update status bit
void ECC_PRAM_E2::Process1bitErrorMethod()
{//{{{
    mpECC_PRAM_E2Func->Process1bitError(m1bitError.address);
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit
void ECC_PRAM_E2::ProcessFatalErrorMethod()
{//{{{
    mpECC_PRAM_E2Func->ProcessFatalError(mFatalError.address);
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void ECC_PRAM_E2::SetMessageLevel (const std::string msg_lv)
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

        mpECC_PRAM_E2Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpECC_PRAM_E2Func->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpECC_PRAM_E2Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpECC_PRAM_E2Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void ECC_PRAM_E2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpECC_PRAM_E2Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information
void ECC_PRAM_E2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_PRAM_E2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_PRAM_E2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_PRAM_E2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Set WORD
void ECC_PRAM_E2::WORD(const unsigned int value)
{//{{{
    if ((MIN_WORD <= value) && (value <= MAX_WORD)){
        mWORD = value;
    }else{
        re_printf("warning", "Invalid WORD (%d) of the ECC_PRAM_E2_WORD parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_WORD, MAX_WORD, DEFAULT_WORD);
    }
}//}}}

/// Set ERR_STAG
void ECC_PRAM_E2::ERR_STAG(const unsigned int value)
{//{{{
    if ((MIN_ERR_STAG <= (int)value) && (value <= MAX_ERR_STAG)){
        mERR_STAG = value;
    }else{
        re_printf("warning", "Invalid ERR_STAG (%d) of the ECC_PRAM_E2_ERR_STAG parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERR_STAG, MAX_ERR_STAG, DEFAULT_ERR_STAG);
    }
}//}}}

/// Issue error to ECC
void ECC_PRAM_E2::IssueError(const unsigned int errortype)
{//{{{
    if (GetResetStatus() || (!CheckClockPeriod("ram_clk")) || (!CheckClockPeriod("pclk"))){
        re_printf("info", "Not receive error input during reset period or clock is stoped.\n");
        return;
    }

    if (!(mpECC_PRAM_E2Func->CheckPermitJudgeError())){
        re_printf("info", "Error judgment is prohibited.\n");
        return;
    }

    if ((errortype == em1bitError) || (errortype == em2bitError)){
        if (mIsAddrConfig){
            double current_time = sc_time_stamp().to_double();
            re_printf("info", "Error input Type: %d, Address: 0x%08X\n", (unsigned int)errortype, mErrorAddress);
            // 1bit error
            if (errortype == em1bitError){
                m1bitError.address = mErrorAddress;// Update address
                mProcess1bitErrorEvent.cancel();
                mProcess1bitErrorEvent.notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
            // Fatal error
            }else{
                mHasFatalError = true;
                DriveECTPREFPort();
                if (!mECTERESig){
                    re_printf("info", "2bit error judgment is prohibited by ectere port.\n");
                    return;
                }
                mFatalError.address = mErrorAddress; // Update address
                mProcessFatalErrorEvent.cancel();// Process fatal error
                mProcessFatalErrorEvent.notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
            }
            // active ECEMF
            mpECC_PRAM_E2Func->SetECEMFbit(1);
            mClearECEMFEvent.notify(CalculateCLKPosEdge((double)mClkPeriod[emram_clkId], false, current_time, mStartClkTime[emram_clkId]) + (double)mClkPeriod[emram_clkId], mTimeResolutionUnit);
        }else{
            re_printf("warning", "The error address and error location are required to configure before issuing error.\n");
        }
    }else{
        re_printf("warning", "Invalid error type (%d) of ECC_PRAM_E2_IssueError command.\n", errortype);
    }
}//}}}

/// Config error address
void ECC_PRAM_E2::SetErrorAddr(const unsigned int addr_value)
{//{{{
    mErrorAddress = addr_value;
    mIsAddrConfig = true;
}//}}}

/// Dump Status info
void ECC_PRAM_E2::DumpStatusInfo()
{//{{{
    std::string type = "StatInfo";
    printf("PROFILE(%s): ECC_PRAM_E2: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): ECC_PRAM_E2:   ECDEDF bits: 0x%02X\n", type.c_str(), mpECC_PRAM_E2Func->GetECDEDFBits());
    printf("PROFILE(%s): ECC_PRAM_E2:   ECSEDF bits: 0x%02X\n", type.c_str(), mpECC_PRAM_E2Func->GetECSEDFBits());
    printf("PROFILE(%s): ECC_PRAM_E2:   ECOVFF bit: 0x%08X\n", type.c_str(), mpECC_PRAM_E2Func->GetECOVFFBit());
    for (unsigned int i = 0; i < mERR_STAG; i++){
        printf("PROFILE(%s): ECC_PRAM_E2:   ExEAD%d = 0x%08X\n", type.c_str(), i, mpECC_PRAM_E2Func->GetExEADn(i));
    }
}//}}}


/// Command to assert reset
void ECC_PRAM_E2::AssertReset (const std::string reset_name, const double start_time, const double period)
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
void ECC_PRAM_E2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            mClkOrgFreq[empclkId] = freq;
            ConvertClockFreq (mClkFreq[empclkId], mClkFreqUnit[empclkId], freq, unit);
            if (mClkFreq[empclkId] > 0) {
                mClkPeriod[empclkId] = (sc_dt::uint64)(((1/(double)mClkFreq[empclkId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[empclkId] = sc_time_stamp().to_double();
            } else {
                mClkPeriod[empclkId] = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[empclkId].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "ram_clk") {
            mClkOrgFreq[emram_clkId] = freq;
            ConvertClockFreq (mClkFreq[emram_clkId], mClkFreqUnit[emram_clkId], freq, unit);
            if (mClkFreq[emram_clkId] > 0) {
                mClkPeriod[emram_clkId] = (sc_dt::uint64)(((1/(double)mClkFreq[emram_clkId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[emram_clkId] = sc_time_stamp().to_double();
            } else {
                mClkPeriod[emram_clkId] = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[emram_clkId].notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void ECC_PRAM_E2::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s\n", (double)mClkOrgFreq[empclkId], mClkFreqUnit[empclkId].c_str());
        } else if (word_vector[0] == "ram_clk") {
            re_printf("info","ram_clk frequency is %0.0f %s\n", (double)mClkOrgFreq[emram_clkId], mClkFreqUnit[emram_clkId].c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void ECC_PRAM_E2::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpECC_PRAM_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void ECC_PRAM_E2::ReleaseRegister (const std::string reg_name)
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
            mpECC_PRAM_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void ECC_PRAM_E2::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpECC_PRAM_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void ECC_PRAM_E2::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpECC_PRAM_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void ECC_PRAM_E2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpECC_PRAM_E2Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void ECC_PRAM_E2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_MessageLevel (\"ECC_PRAM_E2 instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_DumpRegisterRW (\"ECC_PRAM_E2 instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_DumpInterrupt (\"ECC_PRAM_E2 instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_WORD (\"ECC_PRAM_E2 instance\", value)                                                         Set MSB bit of register storing error address (Default: 31). Valid range to be set: 1-31");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_ERR_STAG (\"ECC_PRAM_E2 instance\", value)                                                     Set number of address registers (Default: 1). Valid range to be set: 0-8");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_EnableRegisterMessage (\"ECC_PRAM_E2 instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_IssueError (\"ECC_PRAM_E2 instance\", error_type)                                              Issue pseudo error to the ECC_PRAM_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_SetErrorAddr (\"ECC_PRAM_E2 instance\", addr_value)                                            Set error detection address");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_DumpStatusInfo (\"ECC_PRAM_E2 instance\")                                                      Dump information of the error status registers of the ECC_PRAM_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_AssertReset (\"ECC_PRAM_E2 instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the ECC_PRAM_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_SetCLKFreq (\"ECC_PRAM_E2 instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to ECC_PRAM_E2 model. Valid clock_name is pclk or ram_clk");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_GetCLKFreq (\"ECC_PRAM_E2 instance\", \"clock_name\")                                          Get clock frequency value of ECC_PRAM_E2 model. Valid clock_name is pclk or ram_clk");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_ForceRegister (\"ECC_PRAM_E2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_ReleaseRegister (\"ECC_PRAM_E2 instance\", \"block_name\", block_index, \"reg_name\")          Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_WriteRegister (\"ECC_PRAM_E2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_ReadRegister (\"ECC_PRAM_E2 instance\", \"block_name\", block_index, \"reg_name\")             Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_PRAM_E2_ListRegister (\"ECC_PRAM_E2 instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of ECC_PRAM_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_PRAM_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

void ECC_PRAM_E2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: ECC_PRAM_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_PRAM_E2_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_PRAM_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
