// ---------------------------------------------------------------------
// $Id: APB_UIMPL.cpp,v 1.2 2013/11/27 01:45:27 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "APB_UIMPL.h"

///Constructor of APB_UIMPL class
APB_UIMPL::APB_UIMPL (sc_module_name name, unsigned int rLatency,
                      unsigned int wLatency, unsigned int APB_UIMPL_VERSION):
           sc_module(name),
           APBAgentController_UIMPL(),
           BusSlaveBase<32,2>(),
           PRESETZ("PRESETZ"),
           PCLK("PCLK"),
           INTAPB("INTAPB")
{//{{{
    //Initialize output port
    INTAPB.initialize(false);

    //Initialize variables
    mPCLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mTimeResolutionValue = 1;
    mPCLKFreqUnit = "Hz"; 
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mDumpInterrupt = false;
    mIntrValue = false;

    mResetSignalCurrentValue = true;
    mResetSignalPreviousValue = true;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod =  0;

    //Bind clock, reset port and target socket to Bus IF
    setSlaveFreqPort32(&PCLK, &PCLK);
    setSlaveResetPort32(&PRESETZ, &PRESETZ);
    setTargetSocket32("tse","tss");
    tse = BusSlaveBase<32,2>::tSocket32[0]; //Alias for binding socket
    tss = BusSlaveBase<32,2>::tSocket32[1]; //Alias for binding socket

    mAPBERRSLV_UIMPL = new APBERRSLV_UIMPL((std::string)name, this, APB_UIMPL_VERSION);
    sc_assert(mAPBERRSLV_UIMPL != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mAPBERRSLV_UIMPL);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    mAPBDataHandler_UIMPL = new APBDataHandler_UIMPL((std::string)name, this);
    sc_assert(mAPBDataHandler_UIMPL != NULL);
    mBusSlaveIf32[1]->setFuncModulePtr(mAPBDataHandler_UIMPL);
    mBusSlaveIf32[1]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[1]->setReadInitLatency(rLatency);
    mBusSlaveIf32[1]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[1]->setTransNmbLmt(0xFFFFFFFF);

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive<<PCLK;
 
    SC_METHOD(HandlePRESETZSignalMethod);
    sensitive<<PRESETZ;
    
    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive<<mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive<<mResetCmdEvent;
 
    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive<<mResetCmdCancelEvent;

    SC_METHOD(HandleTransInfoMethod);
    dont_initialize();
    sensitive<<mHandleTransInfoEvent;

    SC_METHOD(WriteINTAPBMethod);
    dont_initialize();
    sensitive<<mWriteIntrEvent;

    SC_METHOD(ClearINTAPBMethod);
    dont_initialize();
    sensitive<<mClearIntrEvent;
}//}}}

///Destructor of APB_UIMPL class
APB_UIMPL::~APB_UIMPL (void)
{//{{{
    delete mAPBERRSLV_UIMPL;
    delete mAPBDataHandler_UIMPL;
}//}}}

///Allow all transactions between 0 and 0xFFFFFFFF, it is called by SystemC kernel
void APB_UIMPL::start_of_simulation (void)
{//{{{
    tss->setBaseAddressSize(0x00000000,0xFFFFFFFF);
}//}}}

///Dump information of registers
void APB_UIMPL::DumpStatusInfo (void)
{//{{{
    mAPBERRSLV_UIMPL->DumpStatusInfo();
}//}}}

///Assert reset by software for PRESETZ
void APB_UIMPL::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETZ") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            mResetCmdPeriod = period;
            mResetCmdEvent.notify(start_time, SC_NS);
            re_printf("info","The model will be reset (PRESETZ) for %f ns after %f ns.\n", period, start_time);
        } else {
            re_printf("warning","The software reset of PRESETZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error","The reset name (%s) is wrong. It should be PRESETZ.\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit for PCLK
void APB_UIMPL::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKPeriod = 0;
                mHandleTransInfoEvent.cancel();
                mWriteIntrEvent.cancel();
                mClearIntrEvent.cancel();
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock value of PCLK
void APB_UIMPL::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void APB_UIMPL::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("force");
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mAPBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Release forced value after forcing registers
void APB_UIMPL::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("release");
    mAPBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Write value to registers by software
void APB_UIMPL::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mAPBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void APB_UIMPL::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    mAPBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///List all reigsters name
void APB_UIMPL::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mAPBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void APB_UIMPL::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_DumpInterrupt (true/false)                    Enable/disable dumping interrupt information (default: false).");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_DumpFileNameLineNum (true/false)              Select dump information about file name and line number. (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_DumpStatusInfo                                Dump the status information.");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_AssertReset (reset_name, start_time, period)  Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to PCLK clock.");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_GetCLKFreq (clock_name)                       Get clock frequency of PCLK clock.");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"APB_UIMPL_ListRegister                                  List all registers of the model.");
        } else {
            re_printf("error","The name (%s) of APB_UIMPL_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of APB_UIMPL_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void APB_UIMPL::SetMessageLevel (const std::string msg_lv)
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
    mAPBERRSLV_UIMPL->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAPBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void APB_UIMPL::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mAPBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Enable/disable dump interrupt information
void APB_UIMPL::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("error","Invalid argument: %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","APB_UIMPL_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("error","Invalid argument: APB_UIMPL_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Handle PCLK signal
void APB_UIMPL::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle PRESETZ signal
void APB_UIMPL::HandlePRESETZSignalMethod (void)
{//{{{
    mResetSignalCurrentValue = PRESETZ.read();
    if (mResetSignalCurrentValue != mResetSignalPreviousValue) {//mResetSignalPreviousValue is "true" at initial state
        mResetSignalPreviousValue = mResetSignalCurrentValue;
        if (mPCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent.notify(CalculateCLKPosEdge(false, current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when PRESETZ is active
void APB_UIMPL::HandleResetHardMethod (void)
{//{{{
    if (mResetSignalCurrentValue == emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset signal PRESETZ is asserted.\n");
        this->EnableReset(mIsResetHardActive);
    } else if (mIsResetHardActive) {
        mIsResetHardActive = false;
        this->EnableReset(mIsResetHardActive);
        re_printf("info","The reset signal PRESETZ is de-asserted.\n");
    }
    //Cancel AssertReset command when PRESETZ is active
    if (mIsResetCmdReceive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

///Process reset function when PRESETZ command is active
void APB_UIMPL::HandleResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when PRESETZ command is active
void APB_UIMPL::CancelResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period is over.\n");
    this->EnableReset(mIsResetCmdActive); 
}//}}}

///Handle the transaction information
void APB_UIMPL::HandleTransInfoMethod (void)
{//{{{
    mAPBERRSLV_UIMPL->HandleTransInfo();
}//}}}

///Write the value to interrupt port
void APB_UIMPL::WriteINTAPBMethod (void)
{//{{{
    INTAPB.write(mIntrValue);
    if (mDumpInterrupt) {
        if (mIntrValue) {
            re_printf("info","INT [INTAPB] Assert.\n");
        } else {
            if ((!mIsResetHardActive)&&(!mIsResetCmdActive)) {
                re_printf("info","INT [INTAPB] De-assert.\n");
            }
        }
    }
}//}}}

///Clear interrupt port
void APB_UIMPL::ClearINTAPBMethod (void)
{//{{{
    mIntrValue = false;
    mWriteIntrEvent.notify();
}//}}}

///Process reset function
void APB_UIMPL::EnableReset (const bool is_active)
{//{{{
    mAPBERRSLV_UIMPL->Reset(is_active);
    if (is_active) {//Initialize variables, output ports and cancel events
        mHandleTransInfoEvent.cancel();
        mWriteIntrEvent.cancel();
        mClearIntrEvent.cancel();
        mIntrValue = false;
        mWriteIntrEvent.notify();
    }
}//}}}

///Check frequency value and frequency unit of PCLK clock
void APB_UIMPL::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
void APB_UIMPL::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void APB_UIMPL::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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
double APB_UIMPL::CalculateCLKPosEdge (const bool add_period, const double time_point)
{//{{{
    double period_num = 0;
    period_num = time_point / (double)mPCLKPeriod;
    unsigned int mod_period_num = (unsigned int)period_num;
    if (!add_period) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - time_point;
    return pos_edge_point;
}//}}}

///Check period value of PCLK clock
bool APB_UIMPL::CheckClockPeriod (void)
{//{{{
    bool clock_status = false;
    if (mPCLKPeriod > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","PCLK frequency is zero.\n");
    }
    return clock_status;
}//}}}

///Control interrupt port INTAPB
void APB_UIMPL::ControlIntr (void)
{//{{{
    mIntrValue = true;
    mWriteIntrEvent.notify();
    mClearIntrEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
}//}}}

///Transfer transaction infomation to APBERRSLV_UIMPL block
void APB_UIMPL::TransferTransInfo (TlmBasicPayload &trans)
{//{{{
    if (CheckClockPeriod()) {
        double current_time = sc_time_stamp().to_double();
        mAPBERRSLV_UIMPL->InformTransInfo(trans);
        mHandleTransInfoEvent.notify(CalculateCLKPosEdge(true, current_time), mTimeResolutionUnit);
    }
}//}}}

///Check reset status
bool APB_UIMPL::GetResetStatus (void)
{//{{{
    bool reset_status = false;
    if (mIsResetHardActive || mIsResetCmdActive) {
        reset_status = true;
    } else {
        reset_status = false;
    }
    return reset_status;
}//}}}
// vim600: set foldmethod=marker :
