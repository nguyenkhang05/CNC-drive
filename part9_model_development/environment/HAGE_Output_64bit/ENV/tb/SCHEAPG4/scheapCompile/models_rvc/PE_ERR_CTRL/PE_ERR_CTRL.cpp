// ---------------------------------------------------------------------
// $Id: PE_ERR_CTRL.cpp,v 1.4 2014/05/13 01:23:45 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "PE_ERR_CTRL.h"
#include "ERRCTRL.h"

///Constructor of PE_ERR_CTRL class
PE_ERR_CTRL::PE_ERR_CTRL (sc_module_name name, unsigned int rLatency, unsigned int wLatency):
           sc_module(name),
           PE_ERR_CTRL_AgentController(),
           BusSlaveBase<32,1>(),
           presetz("presetz"),
           lvcreset("lvcreset"),
           pclk("pclk"),
           lvcclk("lvcclk"),
           ERR_INFO("ERR_INFO"),
           INTAPB("INTAPB")
{//{{{
    //Initialize output port
    INTAPB.initialize(false);

    //Initialize variables
    mPclkPeriod = 0;
    mLvcclkPeriod = 0;
    mPclkFreq = 0;
    mLvcclkFreq = 0;
    mPclkOrgFreq = 0;
    mLvcclkOrgFreq = 0;
    mPclkFreqUnit = "Hz";
    mLvcclkFreqUnit = "Hz";
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mDumpInterrupt = false;
    mIntrValue = false;
    mCurrentErrInfo.Initialize();

    mPresetzSignalCurrentValue = emPresetzInactive;
    mPresetzSignalPreviousValue = emPresetzInactive;
    mIsPresetzHardActive = false;
    mIsPresetzCmdReceive = false;
    mIsPresetzCmdActive = false;
    mPresetzCmdPeriod =  0;

    mLvcresetSignalCurrentValue = emLvcresetInactive;
    mLvcresetSignalPreviousValue = emLvcresetInactive;
    mIsLvcresetHardActive = false;
    mIsLvcresetCmdReceive = false;
    mIsLvcresetCmdActive = false;
    mLvcresetCmdPeriod =  0;

    //Bind clock, reset port and target socket to Bus IF
    setSlaveFreqPort32(&pclk);
    setSlaveResetPort32(&presetz);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding socket

    mERRCTRL = new ERRCTRL((std::string)name, this);
    sc_assert(mERRCTRL != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mERRCTRL);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    SC_METHOD(HandlePclkSignalMethod);
    dont_initialize();
    sensitive<<pclk;
 
    SC_METHOD(HandleLvcclkSignalMethod);
    dont_initialize();
    sensitive<<lvcclk;
 
    SC_METHOD(HandlePresetzSignalMethod);
    sensitive<<presetz;
    
    SC_METHOD(HandlePresetzHardMethod);
    dont_initialize();
    sensitive<<mPresetzHardEvent;

    SC_METHOD(HandlePresetzCmdMethod);
    dont_initialize();
    sensitive<<mPresetzCmdEvent;
 
    SC_METHOD(CancelPresetzCmdMethod);
    dont_initialize();
    sensitive<<mPresetzCmdCancelEvent;

    SC_METHOD(HandleLvcresetSignalMethod);
    sensitive<<lvcreset;
    
    SC_METHOD(HandleLvcresetHardMethod);
    dont_initialize();
    sensitive<<mLvcresetHardEvent;

    SC_METHOD(HandleLvcresetCmdMethod);
    dont_initialize();
    sensitive<<mLvcresetCmdEvent;
 
    SC_METHOD(CancelLvcresetCmdMethod);
    dont_initialize();
    sensitive<<mLvcresetCmdCancelEvent;

    SC_METHOD(HandleERR_INFOMethod);
    dont_initialize();
    sensitive<<ERR_INFO;

    SC_METHOD(RecordErrInfoMethod);
    dont_initialize();
    sensitive<<mRecordErrInfoEvent;

    SC_METHOD(WriteINTAPBMethod);
    dont_initialize();
    sensitive<<mWriteIntrEvent;

    SC_METHOD(ClearINTAPBMethod);
    dont_initialize();
    sensitive<<mClearIntrEvent;
}//}}}

///Destructor of PE_ERR_CTRL class
PE_ERR_CTRL::~PE_ERR_CTRL (void)
{//{{{
    delete mERRCTRL;
}//}}}

///Dump information of registers
void PE_ERR_CTRL::DumpStatusInfo (void)
{//{{{
    mERRCTRL->DumpStatusInfo();
}//}}}

///Assert reset by software for Presetz and Lvcreset
void PE_ERR_CTRL::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetz") {
        if ((!mIsPresetzHardActive)&&(!mIsPresetzCmdReceive)) {
            mIsPresetzCmdReceive = true;
            re_printf("info","The model will be reset (presetz) for %f ns after %f ns.\n", period, start_time);
            mPresetzCmdEvent.notify(start_time, SC_NS);
            mPresetzCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of presetz is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "lvcreset") {
        if ((!mIsLvcresetHardActive)&&(!mIsLvcresetCmdReceive)) {
            mIsLvcresetCmdReceive = true;
            re_printf("info","The model will be reset (lvcreset) for %f ns after %f ns.\n", period, start_time);
            mLvcresetCmdEvent.notify(start_time, SC_NS);
            mLvcresetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of lvcreset is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error","The reset name (%s) is wrong. It should be presetz or lvcreset.\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit for pclk and lvcclk
void PE_ERR_CTRL::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            mPclkOrgFreq = freq;
            ConvertClockFreq (mPclkFreq, mPclkFreqUnit, freq, unit);
            if (mPclkFreq > 0) {
                mPclkPeriod = (sc_dt::uint64)(((1/(double)mPclkFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPclkPeriod = 0;
                mRecordErrInfoEvent.cancel();
                mWriteIntrEvent.cancel();
                mClearIntrEvent.cancel();
            }
        } else if (word_vector[0] == "lvcclk") {
            mLvcclkOrgFreq = freq;
            ConvertClockFreq (mLvcclkFreq, mLvcclkFreqUnit, freq, unit);
            if (mLvcclkFreq > 0) {
                mLvcclkPeriod = (sc_dt::uint64)(((1/(double)mLvcclkFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mLvcclkPeriod = 0;
                mRecordErrInfoEvent.cancel();
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

///Get clock value of pclk and lvcclk
void PE_ERR_CTRL::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s.\n", (double)mPclkOrgFreq, mPclkFreqUnit.c_str());
        } else if (word_vector[0] == "lvcclk") {
            re_printf("info","lvcclk frequency is %0.0f %s.\n", (double)mLvcclkOrgFreq, mLvcclkFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void PE_ERR_CTRL::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mERRCTRL->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void PE_ERR_CTRL::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mERRCTRL->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void PE_ERR_CTRL::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mERRCTRL->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void PE_ERR_CTRL::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mERRCTRL->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///List all reigsters name
void PE_ERR_CTRL::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mERRCTRL->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void PE_ERR_CTRL::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_DumpInterrupt (true/false)                    Enable/disable dumping interrupt information (default: false).");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_DumpFileNameLineNum (true/false)              Select dump information about file name and line number. (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_DumpStatusInfo                                Dump the status information.");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_AssertReset (reset_name, start_time, period)  Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to pclk, lvcclk clock.");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_GetCLKFreq (clock_name)                       Get clock frequency of pclk, lvcclk clock.");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"PE_ERR_CTRL_ListRegister                                  List all registers of the model.");
        } else {
            re_printf("error","The name (%s) of PE_ERR_CTRL_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of PE_ERR_CTRL_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void PE_ERR_CTRL::SetMessageLevel (const std::string msg_lv)
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
    mERRCTRL->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mERRCTRL->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void PE_ERR_CTRL::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mERRCTRL->RegisterHandler(cmd);
}//}}}

///Enable/disable dump interrupt information
void PE_ERR_CTRL::DumpInterrupt (const std::string is_enable)
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
        re_printf("info","PE_ERR_CTRL_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("error","Invalid argument: PE_ERR_CTRL_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Handle Pclk signal
void PE_ERR_CTRL::HandlePclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

///Handle Lvcclk signal
void PE_ERR_CTRL::HandleLvcclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = lvcclk.read();
    SetCLKFreq("lvcclk", freq_value, "Hz");
}//}}}

///Handle presetz signal
void PE_ERR_CTRL::HandlePresetzSignalMethod (void)
{//{{{
    mPresetzSignalCurrentValue = presetz.read();
    if (mPresetzSignalCurrentValue != mPresetzSignalPreviousValue) {//mPresetzSignalPreviousValue is "true" at initial state
        mPresetzSignalPreviousValue = mPresetzSignalCurrentValue;
        if (mPclkPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mPresetzHardEvent.notify(CalculateCLKPosEdge("pclk", current_time), mTimeResolutionUnit);
        } else {
            mPresetzHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Handle lvcreset signal
void PE_ERR_CTRL::HandleLvcresetSignalMethod (void)
{//{{{
    mLvcresetSignalCurrentValue = lvcreset.read();
    if (mLvcresetSignalCurrentValue != mLvcresetSignalPreviousValue) {//mLvcresetSignalPreviousValue is "true" at initial state
        mLvcresetSignalPreviousValue = mLvcresetSignalCurrentValue;
        if (mLvcclkPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mLvcresetHardEvent.notify(CalculateCLKPosEdge("lvcclk", current_time), mTimeResolutionUnit);
        } else {
            mLvcresetHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when presetz is active
void PE_ERR_CTRL::HandlePresetzHardMethod (void)
{//{{{
    if (mPresetzSignalCurrentValue == emPresetzActive) {
        mIsPresetzHardActive = true;
        re_printf("info","The reset signal presetz is asserted.\n");
        this->EnableReset("presetz", mIsPresetzHardActive);
    } else if (mIsPresetzHardActive) {
        mIsPresetzHardActive = false;
        this->EnableReset("presetz", mIsPresetzHardActive);
        re_printf("info","The reset signal presetz is de-asserted.\n");
    }
    //Cancel AssertReset command when presetz is active
    if (mIsPresetzCmdReceive) {
        mPresetzCmdEvent.cancel();
        mPresetzCmdCancelEvent.cancel();
        mIsPresetzCmdReceive = false;
        mIsPresetzCmdActive = false;
    }
}//}}}

///Process reset function when lvcreset is active
void PE_ERR_CTRL::HandleLvcresetHardMethod (void)
{//{{{
    if (mLvcresetSignalCurrentValue == emLvcresetActive) {
        mIsLvcresetHardActive = true;
        re_printf("info","The reset signal lvcreset is asserted.\n");
        this->EnableReset("lvcreset", mIsLvcresetHardActive);
    } else if (mIsLvcresetHardActive) {
        mIsLvcresetHardActive = false;
        this->EnableReset("lvcreset", mIsLvcresetHardActive);
        re_printf("info","The reset signal lvcreset is de-asserted.\n");
    }
    //Cancel AssertReset command when lvcreset is active
    if (mIsLvcresetCmdReceive) {
        mLvcresetCmdEvent.cancel();
        mLvcresetCmdCancelEvent.cancel();
        mIsLvcresetCmdReceive = false;
        mIsLvcresetCmdActive = false;
    }
}//}}}

///Process reset function when presetz command is active
void PE_ERR_CTRL::HandlePresetzCmdMethod (void)
{//{{{
    mIsPresetzCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of presetz.\n");
    this->EnableReset("presetz", mIsPresetzCmdActive);
    mPresetzCmdCancelEvent.notify(mPresetzCmdPeriod, SC_NS);
}//}}}

///Process reset function when lvcreset command is active
void PE_ERR_CTRL::HandleLvcresetCmdMethod (void)
{//{{{
    mIsLvcresetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of lvcreset.\n");
    this->EnableReset("lvcreset", mIsLvcresetCmdActive);
    mLvcresetCmdCancelEvent.notify(mLvcresetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when presetz command is active
void PE_ERR_CTRL::CancelPresetzCmdMethod (void)
{//{{{
    mIsPresetzCmdActive = false;
    mIsPresetzCmdReceive = false;
    re_printf("info","Reset period of presetz is over.\n");
    this->EnableReset("presetz", mIsPresetzCmdActive); 
}//}}}

///Cancel reset function when lvcreset command is active
void PE_ERR_CTRL::CancelLvcresetCmdMethod (void)
{//{{{
    mIsLvcresetCmdActive = false;
    mIsLvcresetCmdReceive = false;
    re_printf("info","Reset period of lvcreset is over.\n");
    this->EnableReset("lvcreset", mIsLvcresetCmdActive); 
}//}}}

///Record error information
void PE_ERR_CTRL::RecordErrInfoMethod (void)
{//{{{
    mERRCTRL->RecordErrTransInfo(mCurrentErrInfo);
}//}}}

void PE_ERR_CTRL::HandleERR_INFOMethod(void)
{//{{{
    if (mLvcclkPeriod > 0) {
        Cerrinfo temp_value;
        temp_value = ERR_INFO.read();
        if (temp_value.PEGWrERR != temp_value.PEGRdERR) {
            if (!GetResetStatus("lvcreset")) {
                mCurrentErrInfo = temp_value;
                double current_time = sc_time_stamp().to_double();
                mRecordErrInfoEvent.notify(CalculateCLKPosEdge("lvcclk", current_time), mTimeResolutionUnit);
                ControlIntr();
            } else {
                re_printf("info","Cannot capture an error during reset period (lvcreset).\n");
            }
        }
    } else {
        re_printf("info","lvcclk frequency is zero.\n");
    }
}//}}}

///Write the value to interrupt port
void PE_ERR_CTRL::WriteINTAPBMethod (void)
{//{{{
    INTAPB.write(mIntrValue);
    if (mDumpInterrupt) {
        if (mIntrValue) {
            re_printf("info","INT [INTAPB] Assert.\n");
        } else {
            if ((!mIsLvcresetHardActive)&&(!mIsLvcresetCmdActive)) {
                re_printf("info","INT [INTAPB] De-assert.\n");
            }
        }
    }
}//}}}

///Clear interrupt port
void PE_ERR_CTRL::ClearINTAPBMethod (void)
{//{{{
    mIntrValue = false;
    mWriteIntrEvent.notify();
}//}}}

///Process reset function
void PE_ERR_CTRL::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mERRCTRL->Reset(reset_name, is_active);
    if ((reset_name == "lvcreset") && (is_active)) {//Initialize variables, output ports and cancel events
        mCurrentErrInfo.Initialize();
        mRecordErrInfoEvent.cancel();
        mWriteIntrEvent.cancel();
        mClearIntrEvent.cancel();
        mIntrValue = false;
        mWriteIntrEvent.notify();
    }
}//}}}

///Check frequency value and frequency unit of Pclk clock
void PE_ERR_CTRL::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
void PE_ERR_CTRL::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void PE_ERR_CTRL::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///Calculate synchronous time with pclk
double PE_ERR_CTRL::CalculateCLKPosEdge (const std::string clock_name, const double time_point)
{//{{{
    double period_num = 0;
    if (clock_name == "pclk") {
        period_num = time_point / (double)mPclkPeriod;
    } else {//clock_name is lvcclk
        period_num = time_point / (double)mLvcclkPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "pclk") {
        pos_edge_point = (double)mod_period_num * (double)mPclkPeriod - time_point;
    } else {//clock_name is lvcclk
        pos_edge_point = (double)mod_period_num * (double)mLvcclkPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Check clock period
bool PE_ERR_CTRL::CheckClock(const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "pclk")?mPclkPeriod:mLvcclkPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        re_printf("info","%s frequency is zero.\n",clock_name.c_str());
        clock_status = false;
    }
    return clock_status;
}//}}}

///Control interrupt port INTAPB
void PE_ERR_CTRL::ControlIntr (void)
{//{{{
    mIntrValue = true;
    double current_time = sc_time_stamp().to_double();
    double time_point = CalculateCLKPosEdge("lvcclk", current_time);
    mWriteIntrEvent.notify(time_point, mTimeResolutionUnit);
    mClearIntrEvent.notify(time_point + (double)mLvcclkPeriod, mTimeResolutionUnit);
}//}}}

///Check reset status
bool PE_ERR_CTRL::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "lvcreset") {
        if (mIsLvcresetHardActive || mIsLvcresetCmdActive) {
            reset_status = true;
        }
    } else {//Presetz
        if (mIsPresetzHardActive || mIsPresetzCmdActive) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

// vim600: set foldmethod=marker :
