// ---------------------------------------------------------------------
// $Id: ECC_DATAFLASH.cpp,v 1.6 2015/09/10 10:58:44 huepham Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "ECC_DATAFLASH.h"
#include "ECC_DATAFLASH_Func.h"

///Constructor of ECC_DATAFLASH class
ECC_DATAFLASH::ECC_DATAFLASH (sc_module_name name, unsigned int rLatency, unsigned int wLatency, const bool is_addr_cap, unsigned int addr_pawidth, std::string ECC_DATAFLASH_VERSION):
            sc_module(name),
            ECC_DATAFLASH_AgentController(),
            BusSlaveBase<32,1>(),
            PRESETN("PRESETN"),
            PCLK("PCLK"),
            eccsed("eccsed"),
            eccded("eccded"),
            eccovf("eccovf"),
            eccdisable("eccdisable"),
            secdisable("secdisable")
{//{{{
    //Initialize output port
    eccsed.initialize(false);
    eccded.initialize(false);
    eccovf.initialize(false);
    eccdisable.initialize(false);
    secdisable.initialize(false);

    //Initialize variables
    if (ECC_DATAFLASH_VERSION == "G3M_V30" || ECC_DATAFLASH_VERSION == "G3KH"){
        mECCDATAFLASHVersion = ECC_DATAFLASH_VERSION;
    } else {
        mECCDATAFLASHVersion = "G3M_V30";
        re_printf("warning","The configured value (%s) of ECC_DATAFLASH_VERSION is not supported. It is set to default value (G3M_V30) automatically.\n", ECC_DATAFLASH_VERSION.c_str());
    }
    mIsStartSimulation = false;
    mIsAddressCaptureEnable = is_addr_cap;
    if (addr_pawidth >= 8 && addr_pawidth <= 21){
        mPawidth = addr_pawidth;
    } else {
        mPawidth = 21;
        re_printf("warning","The configured value (%d) of ECC_DATAFLASH_PAWIDTH is invalid. It is set to default value (21) automatically.\n", addr_pawidth);
    }
    mPclkPeriod = 0;
    mPclkFreq = 0;
    mPclkOrgFreq = 0;
    mPclkFreqUnit = "Hz";
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mResetSignalCurrentValue = emResetInactive;
    mResetSignalPreviousValue = emResetInactive;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod =  0;
    mIsErrAddrSet = false;
    mErrorAddr = 0;
    Initialize();

    //Bind clock, reset port and target socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETN);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding socket

    mECC_DATAFLASH_Func = new ECC_DATAFLASH_Func((std::string)name, mIsAddressCaptureEnable, mPawidth, this, mECCDATAFLASHVersion);
    sc_assert(mECC_DATAFLASH_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mECC_DATAFLASH_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandlePclkSignalMethod);
    dont_initialize();
    sensitive<<PCLK;
 
    SC_METHOD(HandleResetSignalMethod);
    sensitive<<PRESETN;
    
    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive<<mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive<<mResetCmdEvent;
 
    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive<<mResetCmdCancelEvent;

    SC_METHOD(WriteEccsedMethod);
    dont_initialize();
    sensitive<<mWriteEccsedEvent;

    SC_METHOD(ClearEccsedMethod);
    dont_initialize();
    sensitive<<mClearEccsedEvent;

    SC_METHOD(WriteEccdedMethod);
    dont_initialize();
    sensitive<<mWriteEccdedEvent;

    SC_METHOD(ClearEccdedMethod);
    dont_initialize();
    sensitive<<mClearEccdedEvent;

    SC_METHOD(WriteEccovfMethod);
    dont_initialize();
    sensitive<<mWriteEccovfEvent;

    SC_METHOD(ClearEccovfMethod);
    dont_initialize();
    sensitive<<mClearEccovfEvent;

    SC_METHOD(WriteEccdisableMethod);
    dont_initialize();
    sensitive<<mWriteEccdisableEvent;

    SC_METHOD(WriteSecdisableMethod);
    dont_initialize();
    sensitive<<mWriteSecdisableEvent;
}//}}}

///Destructor of ECC_DATAFLASH class
ECC_DATAFLASH::~ECC_DATAFLASH (void)
{//{{{
    delete mECC_DATAFLASH_Func;
}//}}}

///Set message level (fatal, error, warning, info)
void ECC_DATAFLASH::SetMessageLevel (const std::string msg_lv)
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
    mECC_DATAFLASH_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mECC_DATAFLASH_Func->RegisterHandler(cmd);
}//}}} 

///Enable/disable dumping message when users access registers
void ECC_DATAFLASH::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mECC_DATAFLASH_Func->RegisterHandler(cmd);
}//}}} 

///Set initial value for SEDIE
void ECC_DATAFLASH::SelectSEDIEInitVal (const std::string value)
{//{{{ 
    std::vector<std::string> cmd;
    SeparateString(cmd, value);
    if (cmd.size() == 1) {
        if (cmd[0] == "true") {
            mECC_DATAFLASH_Func->SetSEDIEInitValue(true, !mIsStartSimulation);
        } else if (cmd[0] == "false") {
            mECC_DATAFLASH_Func->SetSEDIEInitValue(false, !mIsStartSimulation);
        } else {
            re_printf("warning","Input argument is invalid.\n");
        }
    } else {
        re_printf("warning","Input argument is invalid.\n");
    }
}//}}}

///Issue an error
void ECC_DATAFLASH::IssueError (const unsigned int error_type)
{//{{{
    if (CheckClock()) {
        if (GetResetStatus() == (bool)emResetInactive) {
            if (error_type > 1) {
                re_printf("warning","Error type is invalid\n");
            } else {
                if (!mIsErrAddrSet) {
                    re_printf("warning","Error address is not set yet.\n");
                }
                mECC_DATAFLASH_Func->RecordErrInfo(error_type, mErrorAddr);
                std::string err_type_str = "2-bit error";
                if (error_type == 0) {
                    err_type_str = "1-bit error";
                }
                re_printf("info","An error is issued (error_type: %s; error address: 0x%X).\n", err_type_str.c_str(), mErrorAddr);
            }
        }
    }
}//}}}

///Issue error address
void ECC_DATAFLASH::SetErrorAddr (const int error_addr)
{//{{{
    if (CheckClock()) {
        if (GetResetStatus() == (bool)emResetInactive) {
            mIsErrAddrSet = true;
            mErrorAddr = (unsigned int)error_addr;
        }
    }
}//}}}

///Dump information of registers
void ECC_DATAFLASH::DumpStatusInfo (void)
{//{{{
    mECC_DATAFLASH_Func->DumpStatusInfo();
}//}}}

///Assert reset by software for Reset
void ECC_DATAFLASH::AssertReset (const double start_time, const double period)
{//{{{
    if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
        mIsResetCmdReceive = true;
        re_printf("info","The model will be reset for %f ns after %f ns.\n", period, start_time);
        mResetCmdEvent.notify(start_time, SC_NS);
        mResetCmdPeriod = period;
    } else {
        re_printf("warning","The software reset is called in the reset operation of the model. So it is ignored\n");
    }
}//}}}

///Set clock value and clock unit for PCLK 
void ECC_DATAFLASH::SetCLKFreq (const sc_dt::uint64 freq, const std::string unit)
{//{{{
    mPclkOrgFreq = freq;
    ConvertClockFreq (mPclkFreq, mPclkFreqUnit, freq, unit);
    if (mPclkFreq > 0) {
        mPclkPeriod = (sc_dt::uint64)(((1/(double)mPclkFreq)*(double)mTimeResolutionValue) + 0.5);
    } else {
        mPclkPeriod = 0;
        // Stop operations
        StopWritingToPort();
    }
}//}}}

///Get clock value of PCLK
void ECC_DATAFLASH::GetCLKFreq (void)
{//{{{
    re_printf("info","PCLK frequency is %0.0f %s.\n", (double)mPclkOrgFreq, mPclkFreqUnit.c_str());
}//}}}

///Force value to register
void ECC_DATAFLASH::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mECC_DATAFLASH_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void ECC_DATAFLASH::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mECC_DATAFLASH_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void ECC_DATAFLASH::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mECC_DATAFLASH_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void ECC_DATAFLASH::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mECC_DATAFLASH_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///List all reigsters name
void ECC_DATAFLASH::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mECC_DATAFLASH_Func->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void ECC_DATAFLASH::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_SEDIEInitial (true/false)                     Select the initial value of DFERRINT.SEDIE bit (default: true).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_DumpStatusInfo                                Dump the status information.");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_AssertReset (start_time, period)              Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_SetCLKFreq (freq, unit)                       Set clock frequency to PCLK clock.");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_GetCLKFreq                                    Get clock frequency of PCLK clock.");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_ListRegister                                  List all registers of the model.");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_IssueError (error_type)                       Issue pseudo error in ECC model (0x0: ECC 1bits error; 0x1: ECC 2bits error; Other value: Ignore value).");
            SC_REPORT_INFO(this->basename(),"ECC_DATAFLASH_SetErrorAddr (error_addr)                     Set address where error is occurred.");
        } else {
            re_printf("warning","The name (%s) of ECC_DATAFLASH_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_DATAFLASH_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Handle Pclk signal
void ECC_DATAFLASH::HandlePclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq(freq_value, "Hz");
}//}}}

///Handle PRESETN signal
void ECC_DATAFLASH::HandleResetSignalMethod (void)
{//{{{
    mResetSignalCurrentValue = PRESETN.read();
    if (mResetSignalCurrentValue != mResetSignalPreviousValue) {//mResetSignalPreviousValue is "true" at initial state
        mResetSignalPreviousValue = mResetSignalCurrentValue;
        if (CheckClock()) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent.notify(CalculateCLKPosEdge(current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when PRESETN is active
void ECC_DATAFLASH::HandleResetHardMethod (void)
{//{{{
    if (mResetSignalCurrentValue == emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset signal is asserted.\n");
        this->EnableReset(mIsResetHardActive);
    } else if (mIsResetHardActive) {
        mIsResetHardActive = false;
        this->EnableReset(mIsResetHardActive);
        re_printf("info","The reset signal is de-asserted.\n");
    }
    //Cancel AssertReset command when PRESETN is active
    if (mIsResetCmdReceive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

///Process reset function when PRESETN command is active
void ECC_DATAFLASH::HandleResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETN.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when PRESETN command is active
void ECC_DATAFLASH::CancelResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of PRESETN is over.\n");
    this->EnableReset(mIsResetCmdActive); 
}//}}}

void ECC_DATAFLASH::WriteEccsedMethod (void)
{//{{{
    eccsed.write(mEccsedVal);
}//}}}

void ECC_DATAFLASH::ClearEccsedMethod (void)
{//{{{
    mEccsedVal = false;
    mWriteEccsedEvent.notify();
}//}}}

void ECC_DATAFLASH::WriteEccdedMethod (void)
{//{{{
    eccded.write(mEccdedVal);
}//}}}

void ECC_DATAFLASH::ClearEccdedMethod (void)
{//{{{
    mEccdedVal = false;
    mWriteEccdedEvent.notify();
}//}}}

void ECC_DATAFLASH::WriteEccovfMethod (void)
{//{{{
    eccovf.write(mEccovfVal);
}//}}}

void ECC_DATAFLASH::ClearEccovfMethod (void)
{//{{{
    mEccovfVal = false;
    mWriteEccovfEvent.notify();
}//}}}

void ECC_DATAFLASH::WriteEccdisableMethod (void)
{//{{{
    eccdisable.write(mEccdisableVal);
}//}}}

void ECC_DATAFLASH::WriteSecdisableMethod (void)
{//{{{
    secdisable.write(mSecdisableVal);
}//}}}

///Process reset function
void ECC_DATAFLASH::EnableReset (const bool is_active)
{//{{{
    mECC_DATAFLASH_Func->Reset(is_active);
    if (is_active) {//Initialize variables, output ports and cancel events
        Initialize();
        StopWritingToPort();
        mWriteEccsedEvent.notify();
        mWriteEccdedEvent.notify();
        mWriteEccovfEvent.notify();
        mWriteEccdisableEvent.notify();
        mWriteSecdisableEvent.notify();
    }
}//}}}

void ECC_DATAFLASH::Initialize(void)
{//{{{
    mEccsedVal = false;
    mEccdedVal = false;
    mEccovfVal = false;
    mEccdisableVal = false;
    mSecdisableVal = false;

}//}}}

void ECC_DATAFLASH::StopWritingToPort(void)
{//{{{
    mWriteEccsedEvent.cancel();
    mClearEccsedEvent.cancel();
    mWriteEccdedEvent.cancel();
    mClearEccdedEvent.cancel();
    mWriteEccovfEvent.cancel();
    mClearEccovfEvent.cancel();
    mWriteEccdisableEvent.cancel();
    mWriteSecdisableEvent.cancel();
}//}}}

///Check frequency value and frequency unit of Pclk clock
void ECC_DATAFLASH::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                                  sc_dt::uint64 freq_in, std::string unit_in)
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

///Separate some words from a string to store a vector
void ECC_DATAFLASH::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

///Get time resolution
void ECC_DATAFLASH::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///Calculate synchronous time with PCLK
double ECC_DATAFLASH::CalculateCLKPosEdge (const double time_point)
{//{{{
    double period_num = time_point / (double)mPclkPeriod;
    unsigned int mod_period_num = (unsigned int)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    pos_edge_point = (double)mod_period_num * (double)mPclkPeriod - time_point;
    return pos_edge_point;
}//}}}

bool ECC_DATAFLASH::GetResetStatus(void)
{//{{{
    if (mIsResetHardActive || mIsResetCmdActive) {
        return (bool)emResetActive;
    } else {
        return (bool)emResetInactive;
    }
}//}}}

void ECC_DATAFLASH::start_of_simulation(void)
{//{{{
    mIsStartSimulation = true;
}//}}}

///Check clock period
bool ECC_DATAFLASH::CheckClock(void)
{//{{{
    bool clock_status = false;
    if (mPclkPeriod > 0) {
        clock_status = true;
    } else {
        re_printf("info","PCLK frequency is zero.\n");
        clock_status = false;
    }
    return clock_status;
}//}}}

void ECC_DATAFLASH::ErrorNotification(const unsigned int port_id)
{//{{{
    if (CheckClock()) {
        double current_time = sc_time_stamp().to_double();
        if (port_id == emECCSED) {
            mEccsedVal = true;
            if (mECCDATAFLASHVersion == "G3M_V30"){
                mWriteEccsedEvent.notify(CalculateCLKPosEdge(current_time) + mPclkPeriod, mTimeResolutionUnit);
                mClearEccsedEvent.notify(CalculateCLKPosEdge(current_time) + 2*mPclkPeriod, mTimeResolutionUnit);
            }else if (mECCDATAFLASHVersion == "G3KH"){
                mWriteEccsedEvent.notify(CalculateCLKPosEdge(current_time), mTimeResolutionUnit);
                mClearEccsedEvent.notify(CalculateCLKPosEdge(current_time) + mPclkPeriod, mTimeResolutionUnit);
            }
        } else {
            mEccdedVal = true;
            if (mECCDATAFLASHVersion == "G3M_V30"){
                mWriteEccdedEvent.notify(CalculateCLKPosEdge(current_time) + mPclkPeriod, mTimeResolutionUnit);
                mClearEccdedEvent.notify(CalculateCLKPosEdge(current_time) + 2*mPclkPeriod, mTimeResolutionUnit);
            }else if (mECCDATAFLASHVersion == "G3KH"){
                mWriteEccdedEvent.notify(CalculateCLKPosEdge(current_time), mTimeResolutionUnit);
                mClearEccdedEvent.notify(CalculateCLKPosEdge(current_time) + mPclkPeriod, mTimeResolutionUnit);
            }
          }  
    }
}//}}}

void ECC_DATAFLASH::OverflowNotification(void)
{//{{{
    if (CheckClock()) {
        double current_time = sc_time_stamp().to_double();
        mEccovfVal = true;
        mWriteEccovfEvent.notify(CalculateCLKPosEdge(current_time) + mPclkPeriod, mTimeResolutionUnit);
        mClearEccovfEvent.notify(CalculateCLKPosEdge(current_time) + 2*mPclkPeriod, mTimeResolutionUnit);
    }
}//}}}

void ECC_DATAFLASH::ECCStatusNotification(const unsigned int port_id, const bool output_value)
{//{{{
    if (CheckClock()) {
        double current_time = sc_time_stamp().to_double();
        if (port_id == emEccDis) {
            mEccdisableVal = output_value;
            mWriteEccdisableEvent.notify(CalculateCLKPosEdge(current_time), mTimeResolutionUnit);
        } else {
            mSecdisableVal = output_value;
            mWriteSecdisableEvent.notify(CalculateCLKPosEdge(current_time), mTimeResolutionUnit);
        }
    }
}//}}}

// vim600: set foldmethod=marker :
