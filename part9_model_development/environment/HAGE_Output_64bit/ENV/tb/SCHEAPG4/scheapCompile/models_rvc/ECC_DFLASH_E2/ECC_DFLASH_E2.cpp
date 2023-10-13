// ---------------------------------------------------------------------
// $Id: ECC_DFLASH_E2.cpp,v 1.10 2019/10/14 06:04:57 landang Exp $
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
#include "ECC_DFLASH_E2.h"
#include "ECC_DFLASH_E2_Func.h"

/// Constructor and Destructor
ECC_DFLASH_E2::ECC_DFLASH_E2(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency):
              sc_module(name),
          ECC_DFLASH_E2_AgentController(),
          rvc_common_model(),
          BusSlaveBase<32,1>(),
          PCLK("PCLK"),
          PRESETn("PRESETn"),
          eccdisable("eccdisable"),
          secdisable("secdisable"),
          eccsed("eccsed"),
          eccded("eccded"),
          eccovf("eccovf")
{//{{{
    // Initialize output ports
    eccdisable.initialize(false);
    secdisable.initialize(false);
    eccsed.initialize(false);
    eccded.initialize(false);
    eccovf.initialize(false);

    mpECC_DFLASH_E2Func = new ECC_DFLASH_E2_Func((std::string)name, this);
    
    // Configure Slave socket
    setSlaveResetPort32(&PRESETn);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32((char*)"ts");

    ts = tSocket32[0];

    mBusSlaveIf32[0]->setFuncModulePtr(mpECC_DFLASH_E2Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize variables
    mEnableRegisterMessage = true;
    mIsPRESETnInit = true;
    mPCLKPeriod = 0;
    mclkOrgFreq = 0;
    mclkFreq = 0;
    mclkFreqUnit = "Hz";
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mStartPCLKTime = 0;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    mDumpInterrupt = false;
    mIsErrAddrSet = false;
    mErrorAddr = 0;
    mSEDIEInitVal = true;
    mIsSupport = false;

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(PRESETnMethod);
    sensitive << PRESETn;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << meHandleResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << meCancelResetCmdEvent;

    SC_METHOD(RaiseEccSedInterruptMethod);
    dont_initialize();
    sensitive << meRaiseEccSedInterruptEvent;

    SC_METHOD(NegateEccSedInterruptMethod);
    dont_initialize();
    sensitive << meNegateEccSedInterruptEvent;

    SC_METHOD(DriveEccSedInterruptMethod);
    dont_initialize();
    sensitive << meDriveEccSedInterruptEvent;

    SC_METHOD(RaiseEccDedInterruptMethod);
    dont_initialize();
    sensitive << meRaiseEccDedInterruptEvent;

    SC_METHOD(NegateEccDedInterruptMethod);
    dont_initialize();
    sensitive << meNegateEccDedInterruptEvent;

    SC_METHOD(DriveEccDedInterruptMethod);
    dont_initialize();
    sensitive << meDriveEccDedInterruptEvent;

    SC_METHOD(RaiseErrOvfInterruptMethod);
    dont_initialize();
    sensitive << meRaiseErrOvfInterruptEvent;

    SC_METHOD(NegateErrOvfInterruptMethod);
    dont_initialize();
    sensitive << meNegateErrOvfInterruptEvent;

    SC_METHOD(DriveErrOvfInterruptMethod);
    dont_initialize();
    sensitive << meDriveErrOvfInterruptEvent;

    SC_METHOD(WriteEccDisableMethod);
    dont_initialize();
    sensitive << meWriteEccDisableEvent;

    SC_METHOD(WriteSecDisableMethod);
    dont_initialize();
    sensitive << meWriteSecDisableEvent;

}//}}}

/// Destructor
ECC_DFLASH_E2::~ECC_DFLASH_E2()
{//{{{
    delete mpECC_DFLASH_E2Func;
}//}}}

/// Initialize when reset activated
void ECC_DFLASH_E2::Initialize(void)
{//{{{
    mEccDisableVal = false;
    mSecDisableVal = false;
    mEccSedVal     = false;
    mEccDedVal     = false;
    mEccOvfVal     = false;
    mErrorAddr     = 0;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================

/// Get PCLK period
sc_dt::uint64 ECC_DFLASH_E2::GetClockPeriod()
{//{{{
    return mPCLKPeriod;
}//}}}

/// Get PRESETn status
bool ECC_DFLASH_E2::GetResetStatus()
{//{{{
    bool reset_status = false;
    if (mIsResetHardActive || mIsResetCmdActive) {
        reset_status = true;
    }
    return reset_status;
}//}}}

/// Notify ECC status
void ECC_DFLASH_E2::NotifyEccStatus(const unsigned int port_id, const bool output_value)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if (port_id == emEccDisable) {
        mEccDisableVal = output_value;
        meWriteEccDisableEvent.notify(CalculateCLKPosEdge((double) mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
    } else if (port_id == emSecDisable) {
        mSecDisableVal = output_value;
        meWriteSecDisableEvent.notify(CalculateCLKPosEdge((double) mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
    } else {
        // No operation
    }
}//}}}

/// Drive interrupts
void ECC_DFLASH_E2::DriveInterrupt(const unsigned int port_id)
{//{{{
    double current_time = sc_time_stamp().to_double();
    switch (port_id) {
        case emEccSed:
            meRaiseEccSedInterruptEvent.notify(CalculateCLKPosEdge((double) mPCLKPeriod, false, current_time, mStartPCLKTime) + mPCLKPeriod *2, mTimeResolutionUnit);
            break;
        case emEccDed:
            meRaiseEccDedInterruptEvent.notify(CalculateCLKPosEdge((double) mPCLKPeriod, false, current_time, mStartPCLKTime) + mPCLKPeriod *2, mTimeResolutionUnit);
            break;
        case emErrOvf:
            meRaiseErrOvfInterruptEvent.notify(CalculateCLKPosEdge((double) mPCLKPeriod, false, current_time, mStartPCLKTime) + mPCLKPeriod *2, mTimeResolutionUnit);
            break;
        default:
            // No operation
            break;
    }
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor PRESETn port
void ECC_DFLASH_E2::PRESETnMethod()
{//{{{
    bool reset = PRESETn.read();
    if (reset == vpcl::emResetActive){ // emResetActive is const "false"
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
        meHandleResetCmdEvent.cancel();
        meCancelResetCmdEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

/// Reset model and its sub-instance if reset is activated
void ECC_DFLASH_E2::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events
        meRaiseEccSedInterruptEvent.cancel();
        meNegateEccSedInterruptEvent.cancel();
        meDriveEccSedInterruptEvent.cancel();
        meRaiseEccDedInterruptEvent.cancel();
        meNegateEccDedInterruptEvent.cancel();
        meDriveEccDedInterruptEvent.cancel();
        meRaiseErrOvfInterruptEvent.cancel();
        meNegateErrOvfInterruptEvent.cancel();
        meDriveErrOvfInterruptEvent.cancel();
        meWriteEccDisableEvent.cancel();
        meWriteSecDisableEvent.cancel();
        // Initialize variables
        Initialize();
        // Initialize output ports
        meDriveEccSedInterruptEvent.notify();
        meDriveEccDedInterruptEvent.notify();
        meDriveErrOvfInterruptEvent.notify();
        meWriteEccDisableEvent.notify();
        meWriteSecDisableEvent.notify();
        mpECC_DFLASH_E2Func->SetSEDIEInitVal(mSEDIEInitVal);
    }
    mpECC_DFLASH_E2Func->EnableReset(is_active);
}//}}}

/// Monitor PCLK clock
void ECC_DFLASH_E2::PCLKMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

/// Process reset function when reset command is active
void ECC_DFLASH_E2::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETn.\n");
    this->EnableReset(mIsResetCmdActive);
    meCancelResetCmdEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is inactive
void ECC_DFLASH_E2::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of resetz is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
/// Raise the interrupt port eccsed
void ECC_DFLASH_E2::RaiseEccSedInterruptMethod()
{//{{{
    mEccSedVal = true;
    meDriveEccSedInterruptEvent.notify();
    meNegateEccSedInterruptEvent.notify((double)mPCLKPeriod *1, mTimeResolutionUnit);// Interrupt eccsed last 1 cycles
}//}}}

/// Negate the interrupt port eccsed
void ECC_DFLASH_E2::NegateEccSedInterruptMethod()
{//{{{
    mEccSedVal = false;
    meDriveEccSedInterruptEvent.notify();
}//}}}

/// Drive the interrupt port eccsed
void ECC_DFLASH_E2::DriveEccSedInterruptMethod()
{//{{{
    eccsed.write(mEccSedVal);
    // Dump message
    if (mDumpInterrupt) {
        if (mEccSedVal) {
            re_printf("info","INT [eccsed] Assert.\n");
    } else {
            re_printf("info","INT [eccsed] De-assert.\n");
    }
    }
}//}}}

/// Raise the interrupt port eccded
void ECC_DFLASH_E2::RaiseEccDedInterruptMethod()
{//{{{
    mEccDedVal = true;
    meDriveEccDedInterruptEvent.notify();
    meNegateEccDedInterruptEvent.notify((double)mPCLKPeriod *1, mTimeResolutionUnit);// Interrupt eccded last 1 cycles
}//}}}

/// Negate the interrupt port eccded
void ECC_DFLASH_E2::NegateEccDedInterruptMethod()
{//{{{
    mEccDedVal = false;
    meDriveEccDedInterruptEvent.notify();
}//}}}

/// Drive the interrupt port eccded
void ECC_DFLASH_E2::DriveEccDedInterruptMethod()
{//{{{
    eccded.write(mEccDedVal);
    // Dump message
    if (mDumpInterrupt) {
        if (mEccDedVal) {
            re_printf("info","INT [eccded] Assert.\n");
    } else {
            re_printf("info","INT [eccded] De-assert.\n");
    }
    }
}//}}}

/// Raise the interrupt port eccovf
void ECC_DFLASH_E2::RaiseErrOvfInterruptMethod()
{//{{{
    mEccOvfVal = true;
    meDriveErrOvfInterruptEvent.notify();
    meNegateErrOvfInterruptEvent.notify((double)mPCLKPeriod *1, mTimeResolutionUnit);// Interrupt eccovf last 1 cycles
}//}}}

/// Negate the interrupt port eccovf
void ECC_DFLASH_E2::NegateErrOvfInterruptMethod()
{//{{{
    mEccOvfVal = false;
    meDriveErrOvfInterruptEvent.notify();
}//}}}

/// Drive the interrupt port eccovf
void ECC_DFLASH_E2::DriveErrOvfInterruptMethod()
{//{{{
    eccovf.write(mEccOvfVal);
    // Dump message
    if (mDumpInterrupt) {
        if (mEccOvfVal) {
            re_printf("info","INT [eccovf] Assert.\n");
    } else {
            re_printf("info","INT [eccovf] De-assert.\n");
    }
    }
}//}}}

/// Write out eccdisable status
void ECC_DFLASH_E2::WriteEccDisableMethod()
{//{{{
    eccdisable.write(mEccDisableVal);
}//}}}

/// Write out secdisable status
void ECC_DFLASH_E2::WriteSecDisableMethod()
{//{{{
    secdisable.write(mSecDisableVal);
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void ECC_DFLASH_E2::SetMessageLevel (const std::string msg_lv)
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

        mpECC_DFLASH_E2Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpECC_DFLASH_E2Func->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpECC_DFLASH_E2Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpECC_DFLASH_E2Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void ECC_DFLASH_E2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpECC_DFLASH_E2Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information
void ECC_DFLASH_E2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_DFLASH_E2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_DFLASH_E2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_DFLASH_E2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Select the initial value of DFERRINT.SEDIE bit
void ECC_DFLASH_E2::SetSEDIEInitVal (const std::string is_one)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_one);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mSEDIEInitVal = true;
        } else if (word_vector[0] == "false") {
            mSEDIEInitVal = false;
        } else {
            re_printf("warning","Invalid argument: ECC_DFLASH_E2_SEDIE_INIT %s\n", is_one.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mSEDIEInitVal) ? "true" : "false";
        re_printf("info","ECC_DFLASH_E2_SEDIE_INIT %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_DFLASH_E2_SEDIE_INIT %s\n", is_one.c_str());
    }
}//}}}

/// Enable/disable the support of address capture function
void ECC_DFLASH_E2::SetADDCAPValue (const std::string is_support)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_support);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mIsSupport = true;
        } else if (word_vector[0] == "false") {
            mIsSupport = false;
        } else {
            re_printf("warning","Invalid argument: ECC_DFLASH_E2_ADDCAP %s\n", is_support.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mIsSupport) ? "true" : "false";
        re_printf("info","ECC_DFLASH_E2_ADDCAP %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_DFLASH_E2_ADDCAP %s\n", is_support.c_str());
    }
    mpECC_DFLASH_E2Func->IsAddrCaptureSupport(mIsSupport);
}//}}}

/// Config the bit width of captured address (DFEADR.DFEADR bit width)
void ECC_DFLASH_E2::SetPAWIDTHValue (const unsigned int pawidth)
{//{{{
    if ((pawidth >= 8) && (pawidth <=21)) {
        mpECC_DFLASH_E2Func->GetPAWIDTHValue(pawidth);
    }
}//}}}

/// Command to assert reset
void ECC_DFLASH_E2::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            meHandleResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void ECC_DFLASH_E2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mclkOrgFreq = freq;
            ConvertClockFreq (mclkFreq, mclkFreqUnit, freq, unit);
            if (mclkFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mclkFreq)*(double)mTimeResolutionValue) + 0.5);
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
void ECC_DFLASH_E2::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mclkOrgFreq, mclkFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void ECC_DFLASH_E2::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpECC_DFLASH_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void ECC_DFLASH_E2::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpECC_DFLASH_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void ECC_DFLASH_E2::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpECC_DFLASH_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void ECC_DFLASH_E2::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpECC_DFLASH_E2Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void ECC_DFLASH_E2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpECC_DFLASH_E2Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void ECC_DFLASH_E2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_MessageLevel (\"ECC_DFLASH_E2 instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_DumpRegisterRW (\"ECC_DFLASH_E2 instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_DumpInterrupt (\"ECC_DFLASH_E2 instance\", \"true/false\")                     Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_SEDIE_INIT (\"ECC_DFLASH_E2 instance\", \"true/false\")                        Select the initial value of DFERRINT.SEDIE bit (Default: true)");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_ADDCAP (\"ECC_DFLASH_E2 instance\", \"true/false\")                            Enable/disable the support of address capture function (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_PAWIDTH (\"ECC_DFLASH_E2 instance\", \"8 -> 21\")                              Configure the bit width of captured address (DFEADR.DFEADR bit width) (Default: 21)");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_EnableRegisterMessage (\"ECC_DFLASH_E2 instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_AssertReset (\"ECC_DFLASH_E2 instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the ECC_DFLASH_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_SetCLKFreq (\"ECC_DFLASH_E2 instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to ECC_DFLASH_E2 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_GetCLKFreq (\"ECC_DFLASH_E2 instance\", \"clock_name\")                        Get clock frequency value of ECC_DFLASH_E2 model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_ForceRegister (\"ECC_DFLASH_E2 instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_ReleaseRegister (\"ECC_DFLASH_E2 instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_WriteRegister (\"ECC_DFLASH_E2 instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_ReadRegister (\"ECC_DFLASH_E2 instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_ListRegister (\"ECC_DFLASH_E2 instance\")                                      Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_IssueError (\"ECC_DFLASH_E2 instance\", error_type)                            Injecting pseudo error into ECC model");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_SetErrorAddr (\"ECC_DFLASH_E2 instance\", addr_value)                          Set error address");
            SC_REPORT_INFO(this->basename(),"ECC_DFLASH_E2_DumpStatusInfo (\"ECC_DFLASH_E2 instance\")                                    Dump status information of the ECC_DFLASH_E2 model");
        } else {
            re_printf("warning","The name (%s) of ECC_DFLASH_E2_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_DFLASH_E2_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

/// Injecting pseudo error into ECC model
void ECC_DFLASH_E2::IssueError (const unsigned int error_type)
{//{{{
    if (GetClockPeriod() != 0) {
        if ((!mIsResetHardActive) && (!mIsResetCmdActive)) {
        if (error_type > 1) {
            re_printf("warning","Invalid error type 0x%d of the ECC_DFLASH_E2_IssueError command.\n", error_type);
        } else {
            if (!mIsErrAddrSet) {
                re_printf("warning","The error address is required to configured before issuing error.\n");
        } else {
                    // Only handle ECC error when ECC is enabled
                    if (!mEccDisableVal) {
                    re_printf("info","Error input Type: %d, Address: 0x%08X.\n", error_type, mErrorAddr);
                        mpECC_DFLASH_E2Func->ErrorProcess(error_type, mErrorAddr); // Then, processing error flags
                    }
        }
        }
    } else {
        re_printf("warning","Cannot capture error during reset period.\n");
    }
    } else {
    re_printf("warning","Cannot capture error when clock is stopped.\n");
    }
}//}}}

/// Set error address 
void ECC_DFLASH_E2::SetErrorAddr (const unsigned int error_addr)
{//{{{
    if (GetClockPeriod() != 0) {
        // Reset (hard and soft) must be inactive for next process
    if ((!mIsResetHardActive) && (!mIsResetCmdActive)) {
        mIsErrAddrSet = true;
        mErrorAddr = error_addr;
    }
    }
}//}}}

/// Dump status information of the model
void ECC_DFLASH_E2::DumpStatusInfo (void)
{//{{{
    mpECC_DFLASH_E2Func->DumpStatusInfo();
}//}}}

void ECC_DFLASH_E2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: ECC_DFLASH_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_DFLASH_E2_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_DFLASH_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :

