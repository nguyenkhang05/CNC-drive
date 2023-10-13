// ---------------------------------------------------------------------
// $Id: ECC_APB.cpp,v 1.4 2018/02/05 04:36:00 chanle Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "ECC_APB.h"
#include "ECC_APB_Func.h"

/// Constructor and Destructor
ECC_APB::ECC_APB(sc_module_name name, std::string kind, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,ECC_APB_AgentController()
        // Clock/Reset
        ,pclk("pclk")
        ,presetn("presetn")
        ,asecdisable("asecdisable")
        ,aeccdisable("aeccdisable")
        ,secdisable("secdisable")
        ,eccdisable("eccdisable")
        ,errovfout("errovfout")
        ,adederrout("adederrout")
        ,asederrout("asederrout")
        ,dederrout("dederrout")
        ,sederrout("sederrout")
{//{{{ 
    // New instance and port
    sc_assert ((kind == "CF_A")
        || (kind == "IFU_D") 
        || (kind == "LSUS_DA") || (kind == "LSUM_D") 
        || (kind == "SGn_D") || (kind == "SG7_DA") 
        || (kind == "PDMA_D") 
        || (kind == "APEC0_DA") || (kind == "APECn_DA"));
    mIsCheckErrorInData = CheckKindSupportErrorInData(kind);
    mIsCheckErrorInAddress = CheckKindSupportErrorInAddress(kind);
    mpECC_APBFunc = new ECC_APB_Func((std::string)name, kind, this);
    
    // Configure Slave socket
    BusSlaveBase<32, 1>::setSlaveResetPort32(&presetn);
    BusSlaveBase<32, 1>::setSlaveFreqPort32(&pclk);
    BusSlaveBase<32, 1>::setTargetSocket32("ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setFuncModulePtr(mpECC_APBFunc);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setReadLatency(rLatency);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize remain output ports
    asecdisable.initialize(false);
    aeccdisable.initialize(false);
    secdisable.initialize(false); 
    eccdisable.initialize(false); 
    errovfout.initialize(false);  
    adederrout.initialize(false); 
    asederrout.initialize(false); 
    dederrout.initialize(false);  
    sederrout.initialize(false);  


    // Initialize variables
    mIspresetnInit = true;
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

    mDumpInterrupt  = false;

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorpclkMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(MonitorpresetnMethod);
    sensitive << presetn;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(WriteDisablePortMethod);
    dont_initialize();
    sensitive << mWriteDisablePortEvent;

    SC_METHOD(WriteERROVFOUTMethod);
    dont_initialize();
    sensitive << mWriteERROVFOUTEvent;

    SC_METHOD(WriteADEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteADEDERROUTEvent;

    SC_METHOD(WriteASEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteASEDERROUTEvent;

    SC_METHOD(WriteDEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteDEDERROUTEvent;

    SC_METHOD(WriteSEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteSEDERROUTEvent;

    SC_METHOD(NegateERROVFOUTMethod);
    dont_initialize();
    sensitive << mNegateERROVFOUTEvent;

    SC_METHOD(NegateADEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateADEDERROUTEvent;

    SC_METHOD(NegateASEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateASEDERROUTEvent;

    SC_METHOD(NegateDEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateDEDERROUTEvent;

    SC_METHOD(NegateSEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateSEDERROUTEvent;

    SC_METHOD(ProcessDataErrorMethod);
    dont_initialize();
    sensitive << mProcessDataErrorEvent;

    SC_METHOD(ProcessAddressErrorMethod);
    dont_initialize();
    sensitive << mProcessAddressErrorEvent;
}//}}} 

/// Destructor
ECC_APB::~ECC_APB()
{//{{{
    delete mpECC_APBFunc;
}//}}}

/// Initialize when reset activated
void ECC_APB::Initialize(void)
{//{{{
    mECCDISABLE   = 0;
    mSECDISABLE   = 0;
    mAECCDISABLE  = 0;
    mASECDISABLE  = 0;
    mERROVFOUT    = 0;
    mADEDERROUT   = 0;
    mASEDERROUT   = 0;
    mDEDERROUT    = 0;
    mSEDERROUT    = 0;
    
    mIsAddrConfig = 0;
    mErrorAddress = 0;
    mDataError.Initialize();
    mAddressError.Initialize();
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Check clock period 
bool ECC_APB::CheckClockPeriod ()
{//{{{
    return (bool) (mPCLKPeriod > 0);
}//}}} 

/// Get reset status
bool ECC_APB::GetResetStatus ()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Prepare to write detect/correct prohibition port
void ECC_APB::WriteDisablePort(unsigned int dis_value)
{//{{{
    mECCDISABLE = (bool) (dis_value & 0x1);
    mSECDISABLE = (bool) ((dis_value >> 1) & 0x1);
    mAECCDISABLE = (bool) ((dis_value >> 2) & 0x1);
    mASECDISABLE = (bool) ((dis_value >> 3) & 0x1);
    mWriteDisablePortEvent.cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisablePortEvent.notify(CalculateCLKPosEdge(false, current_time), mTimeResolutionUnit);
}//}}}

/// Prepare to assert interrupt base on the input interrupt name
void ECC_APB::AssertInterrupt(std::string int_name)
{//{{{
    sc_assert ( (int_name == "SEDF") || (int_name == "DEDF") || (int_name == "ASEDF") || (int_name == "ADEDF") || (int_name == "ERROVF") );
    if (int_name == "SEDF"){
        mSEDERROUT = true;
        mWriteSEDERROUTEvent.notify();
    }else if (int_name == "DEDF"){
        mDEDERROUT = true;
        mWriteDEDERROUTEvent.notify();
    }else if (int_name == "ASEDF"){
        mASEDERROUT = true;
        mWriteASEDERROUTEvent.notify();
    }else if (int_name == "ADEDF"){
        mADEDERROUT = true;
        mWriteADEDERROUTEvent.notify();
    }else { //if (int_name == "ERROVF")
        mERROVFOUT = true;
        mWriteERROVFOUTEvent.notify();
    }
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor pclk clock
void ECC_APB::MonitorpclkMethod()
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

/// Monitor presetn port
void ECC_APB::MonitorpresetnMethod()
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
void ECC_APB::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of presetn.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void ECC_APB::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of presetn is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

/// Reset model and its sub-instance if reset is actived
void ECC_APB::EnableReset (const bool is_active)
{//{{{
    mpECC_APBFunc->EnableReset(is_active);
    if(is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        mWriteDisablePortEvent.notify();
        mWriteERROVFOUTEvent.notify();
        mWriteADEDERROUTEvent.notify();
        mWriteASEDERROUTEvent.notify();
        mWriteDEDERROUTEvent.notify();
        mWriteSEDERROUTEvent.notify();
    }
}//}}}

/// Check frequency value and frequency unit
void ECC_APB::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in)
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

/// Get time resolution
void ECC_APB::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

/// Calculate the synchronous time with clock
double ECC_APB::CalculateCLKPosEdge (const bool add_period, const double time_point)
{//{{{
    //calcualte the time point of next pos edge of PCLK
    double period_num = 0;

    if (mPCLKPeriod == 0){
        return 0;
    }
    period_num = (time_point - mStartPCLKTime)/ (double)mPCLKPeriod;

    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (!add_period) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        mod_period_num++;
    }
    double pos_edge_point = mStartPCLKTime + (double)mod_period_num * (double)mPCLKPeriod - time_point;

    return pos_edge_point;
}//}}}

/// Cancel events
void ECC_APB::CancelEvents()
{//{{{
    mWriteDisablePortEvent.cancel();
    mWriteERROVFOUTEvent.cancel();
    mWriteADEDERROUTEvent.cancel();
    mWriteASEDERROUTEvent.cancel();
    mWriteDEDERROUTEvent.cancel();
    mWriteSEDERROUTEvent.cancel();
    mNegateERROVFOUTEvent.cancel();
    mNegateADEDERROUTEvent.cancel();
    mNegateASEDERROUTEvent.cancel();
    mNegateDEDERROUTEvent.cancel();
    mNegateSEDERROUTEvent.cancel();
    mProcessDataErrorEvent.cancel();
    mProcessAddressErrorEvent.cancel();
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
/// Write asecdisable/aeccdisable/secdisable/eccdisable port
void ECC_APB::WriteDisablePortMethod()
{//{{{
    asecdisable.write(mASECDISABLE);
    aeccdisable.write(mAECCDISABLE);
    secdisable.write(mSECDISABLE);
    eccdisable.write(mECCDISABLE);
}//}}}

/// Write errovfout port
void ECC_APB::WriteERROVFOUTMethod()
{//{{{
    errovfout.write(mERROVFOUT);
    if (mDumpInterrupt){
        re_printf("info", "errovfout is changed to %d\n", (unsigned int)mERROVFOUT);
    }
    // Negate if any
    if (mERROVFOUT){
        mNegateERROVFOUTEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/// Negate errovfout port
void ECC_APB::NegateERROVFOUTMethod()
{//{{{
    mERROVFOUT = false;
    mWriteERROVFOUTEvent.notify();
}//}}}

/// Write adederrout port
void ECC_APB::WriteADEDERROUTMethod()
{//{{{
    adederrout.write(mADEDERROUT);
    if (mDumpInterrupt){
        re_printf("info", "adederrout is changed to %d\n", (unsigned int)mADEDERROUT);
    }
    // Negate if any
    if (mADEDERROUT){
        mNegateADEDERROUTEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/// Negate adederrout port
void ECC_APB::NegateADEDERROUTMethod()
{//{{{
    mADEDERROUT = false;
    mWriteADEDERROUTEvent.notify();
}//}}}

/// Write asederrout port
void ECC_APB::WriteASEDERROUTMethod()
{//{{{
    asederrout.write(mASEDERROUT);
    if (mDumpInterrupt){
        re_printf("info", "asederrout is changed to %d\n", (unsigned int)mASEDERROUT);
    }
    // Negate if any
    if (mASEDERROUT){
        mNegateASEDERROUTEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/// Negate asederrout port
void ECC_APB::NegateASEDERROUTMethod()
{//{{{
    mASEDERROUT = false;
    mWriteASEDERROUTEvent.notify();
}//}}}

/// Write dederrout port
void ECC_APB::WriteDEDERROUTMethod()
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

/// Negate dederrout port
void ECC_APB::NegateDEDERROUTMethod()
{//{{{
    mDEDERROUT = false;
    mWriteDEDERROUTEvent.notify();
}//}}}

/// Write sederrout port
void ECC_APB::WriteSEDERROUTMethod()
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

/// Negate sederrout port
void ECC_APB::NegateSEDERROUTMethod()
{//{{{
    mSEDERROUT = false;
    mWriteSEDERROUTEvent.notify();
}//}}}

/// Process 1bit/2bit error in data
void ECC_APB::ProcessDataErrorMethod()
{//{{{
    mpECC_APBFunc->ProcessDataError(mDataError);
}//}}}

/// Process 1bit/2bit error in address
void ECC_APB::ProcessAddressErrorMethod()
{//{{{
    mpECC_APBFunc->ProcessAddressError(mAddressError);
}//}}}

/// Check the input ECC_APB kind support error in data or not
/// Update this function if any change about ECC_APB kind
bool ECC_APB::CheckKindSupportErrorInData(std::string kind)
{//{{{
    return ( (kind == "IFU_D") || (kind == "LSUS_DA") || (kind == "LSUM_D") || (kind == "SGn_D") 
        || (kind == "SG7_DA") || (kind == "PDMA_D") || (kind == "APEC0_DA") || (kind == "APECn_DA") );
}//}}}

/// Check the input ECC_APB kind support error in address or not
/// Update this function if any change about ECC_APB kind
bool ECC_APB::CheckKindSupportErrorInAddress(std::string kind)
{//{{{
    return ( (kind == "CF_A") || (kind == "LSUS_DA") || (kind == "SG7_DA") || (kind == "APEC0_DA") || (kind == "APECn_DA") );
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void ECC_APB::SetMessageLevel (const std::string msg_lv)
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

    mpECC_APBFunc->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    mpECC_APBFunc->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping message when users access registers
void ECC_APB::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpECC_APBFunc->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information
void ECC_APB::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_APB_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","ECC_APB_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_APB_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Issue error to ECC
void ECC_APB::IssueError(const unsigned int errortype)
{//{{{
    if (GetResetStatus() || (!CheckClockPeriod())){
        re_printf("info", "Not receive error input during reset period or clock is stoped.\n");
        return;
    }

    double current_time = sc_time_stamp().to_double();
    if (mIsAddrConfig){
        // Process error in data
        if ( ((errortype == em1bitError) || (errortype == em2bitError)) && mIsCheckErrorInData){
            re_printf("info", "Error input Type: %d, Address: 0x%08X.\n", errortype, mErrorAddress);
            mDataError.address = mErrorAddress;
            mDataError.type = (eErrorType)errortype;
            mProcessDataErrorEvent.cancel();
            mProcessDataErrorEvent.notify(CalculateCLKPosEdge(true, current_time), mTimeResolutionUnit);

        // Process error in address
        } else if ( ((errortype == em1bitAddressError) || (errortype == em2bitAddressError)) &&  mIsCheckErrorInAddress){
            re_printf("info", "Error input Type: %d, Address: 0x%08X.\n", errortype, mErrorAddress);
            mAddressError.address = mErrorAddress;
            mAddressError.type = (eErrorType)errortype;
            mProcessAddressErrorEvent.cancel();
            mProcessAddressErrorEvent.notify(CalculateCLKPosEdge(true, current_time), mTimeResolutionUnit);

        // Process case invalid error
        } else {
            re_printf("warning", "Invalid error type (%d) of ECC_APB_IssueError command.\n", errortype);
        }
    }else {
        re_printf("warning", "The error address is required to configure before issuing error.\n");
    }
}//}}}

/// Config error address
void ECC_APB::SetErrorAddr(const unsigned int addr_value)
{//{{{
    mErrorAddress = addr_value;
    mIsAddrConfig = true;
}//}}}

/// Dump Status info
void ECC_APB::DumpStatusInfo()
{//{{{
    mpECC_APBFunc->DumpStatusInfo();
}//}}}

/// Command to assert reset
void ECC_APB::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetn") {
        if ((!mIsResetHardActive) && (!mIsResetCmdReceive)) {
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
void ECC_APB::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
void ECC_APB::GetCLKFreq (const std::string clock_name)
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
void ECC_APB::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpECC_APBFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void ECC_APB::ReleaseRegister (const std::string reg_name)
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
            mpECC_APBFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void ECC_APB::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpECC_APBFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void ECC_APB::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpECC_APBFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void ECC_APB::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpECC_APBFunc->RegisterHandler(cmd);
}//}}}

/// Separate some words from a string to store a vector
void ECC_APB::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

/// Dump help message of all parameters or commands
void ECC_APB::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_APB_MessageLevel (\"ECC_APB instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_APB_DumpRegisterRW (\"ECC_APB instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_APB_DumpInterrupt (\"ECC_APB instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_APB_IssueError (\"ECC_APB instance\", error_type)                                              Issue pseudo error to the ECC_APB model");
            SC_REPORT_INFO(this->basename(),"ECC_APB_SetErrorAddr (\"ECC_APB instance\", addr_value)                                            Set error detection address");
            SC_REPORT_INFO(this->basename(),"ECC_APB_DumpStatusInfo (\"ECC_APB instance\")                                                      Dump information of the error status registers of the ECC_APB model");
            SC_REPORT_INFO(this->basename(),"ECC_APB_AssertReset (\"ECC_APB instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the ECC_APB model");
            SC_REPORT_INFO(this->basename(),"ECC_APB_SetCLKFreq (\"ECC_APB instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to ECC_APB model. Valid clock_name is pclk");
            SC_REPORT_INFO(this->basename(),"ECC_APB_GetCLKFreq (\"ECC_APB instance\", \"clock_name\")                                          Get clock frequency value of ECC_APB model. Valid clock_name is pclk");
            SC_REPORT_INFO(this->basename(),"ECC_APB_ForceRegister (\"ECC_APB instance\", \"block_name\", block_index, \"reg_name\", reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_APB_ReleaseRegister (\"ECC_APB instance\", \"block_name\", block_index, \"reg_name\")          Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_APB_WriteRegister (\"ECC_APB instance\", \"block_name\", block_index, \"reg_name\", reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_APB_ReadRegister (\"ECC_APB instance\", \"block_name\", block_index, \"reg_name\")             Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_APB_ListRegister (\"ECC_APB instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of ECC_APB_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_APB_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
