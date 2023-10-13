// ---------------------------------------------------------------------
// $Id: AHB_UIMPL.cpp,v 1.2 2013/11/27 01:47:00 dungvannguyen Exp $
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

#include "AHB_UIMPL.h"

///Constructor of AHB_UIMPL class
AHB_UIMPL::AHB_UIMPL (sc_module_name name, unsigned int rLatency,
                      unsigned int wLatency, unsigned int AHB_UIMPL_VERSION):
           sc_module(name),
           AHBAgentController_UIMPL(),
           BusSlaveBase<32,2>(),
           PRESETZ("PRESETZ"),
           HRESETZ("HRESETZ"),
           PCLK("PCLK"),
           HCLK("HCLK"),
           INTAHB("INTAHB")
{//{{{
    //Initialize output port
    INTAHB.initialize(false);

    //Initialize variables
    mPCLKPeriod = 0;
    mHCLKPeriod = 0;
    mPCLKFreq = 0;
    mHCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mHCLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz"; 
    mHCLKFreqUnit = "Hz"; 
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mDumpInterrupt = false;
    mIntrValue = false;
    mErrorAccessIgnore = false;
    mStatusClearIgnore = false;
    mClearOnlyOVF = false;

    mPRESETZSignalCurrentValue = true;
    mPRESETZSignalPreviousValue = true;
    mIsPRESETZHardActive = false;
    mIsPRESETZCmdReceive = false;
    mIsPRESETZCmdActive = false;
    mPRESETZCmdPeriod =  0;

    mHRESETZSignalCurrentValue = true;
    mHRESETZSignalPreviousValue = true;
    mIsHRESETZHardActive = false;
    mIsHRESETZCmdReceive = false;
    mIsHRESETZCmdActive = false;
    mHRESETZCmdPeriod =  0;

    //Bind clock, reset port and target socket to Bus IF
    setSlaveFreqPort32(&PCLK, &HCLK);
    setSlaveResetPort32(&PRESETZ, &HRESETZ);
    setTargetSocket32("tse","tss");
    tse = BusSlaveBase<32,2>::tSocket32[0]; //Alias for binding socket
    tss = BusSlaveBase<32,2>::tSocket32[1]; //Alias for binding socket

    mAHBERRSLV_UIMPL = new AHBERRSLV_UIMPL((std::string)name, this, AHB_UIMPL_VERSION);
    sc_assert(mAHBERRSLV_UIMPL != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mAHBERRSLV_UIMPL);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    mAHBDataHandler_UIMPL = new AHBDataHandler_UIMPL((std::string)name, this);
    sc_assert(mAHBDataHandler_UIMPL != NULL);
    mBusSlaveIf32[1]->setFuncModulePtr(mAHBDataHandler_UIMPL);
    mBusSlaveIf32[1]->setBusProtocol(BUS_AHB);
    mBusSlaveIf32[1]->setReadInitLatency(rLatency);
    mBusSlaveIf32[1]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[1]->setTransNmbLmt(0xFFFFFFFF);

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive<<PCLK;
 
    SC_METHOD(HandleHCLKSignalMethod);
    dont_initialize();
    sensitive<<HCLK;
 
    SC_METHOD(HandlePRESETZSignalMethod);
    sensitive<<PRESETZ;
    
    SC_METHOD(HandlePRESETZHardMethod);
    dont_initialize();
    sensitive<<mPRESETZHardEvent;

    SC_METHOD(HandlePRESETZCmdMethod);
    dont_initialize();
    sensitive<<mPRESETZCmdEvent;
 
    SC_METHOD(CancelPRESETZCmdMethod);
    dont_initialize();
    sensitive<<mPRESETZCmdCancelEvent;

    SC_METHOD(HandleHRESETZSignalMethod);
    sensitive<<HRESETZ;
    
    SC_METHOD(HandleHRESETZHardMethod);
    dont_initialize();
    sensitive<<mHRESETZHardEvent;

    SC_METHOD(HandleHRESETZCmdMethod);
    dont_initialize();
    sensitive<<mHRESETZCmdEvent;
 
    SC_METHOD(CancelHRESETZCmdMethod);
    dont_initialize();
    sensitive<<mHRESETZCmdCancelEvent;

    SC_METHOD(HandleTransInfoMethod);
    dont_initialize();
    sensitive<<mHandleTransInfoEvent;

    SC_METHOD(WriteINTAHBMethod);
    dont_initialize();
    sensitive<<mWriteIntrEvent;

    SC_METHOD(ClearINTAHBMethod);
    dont_initialize();
    sensitive<<mClearIntrEvent;

    SC_METHOD(ErrorAccessIgnoreMethod);
    dont_initialize();
    sensitive<<mErrorAccessIgnoreEvent;
    
    SC_METHOD(StatusClearMethod);
    dont_initialize(); 
    sensitive<<mStatusClearEvent;
}//}}}

///Destructor of AHB_UIMPL class
AHB_UIMPL::~AHB_UIMPL (void)
{//{{{
    delete mAHBERRSLV_UIMPL;
    delete mAHBDataHandler_UIMPL;
}//}}}

///Allow all transaction between 0 and 0xFFFFFFFF, it is called by SystemC kernel
void AHB_UIMPL::start_of_simulation (void)
{//{{{
    tss->setBaseAddressSize(0x00000000,0xFFFFFFFF);
}//}}}

///Dump information of registers
void AHB_UIMPL::DumpStatusInfo (void)
{//{{{
    mAHBERRSLV_UIMPL->DumpStatusInfo();
}//}}}

///Assert reset by software for PRESETZ and HRESETZ
void AHB_UIMPL::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETZ") {
        if ((!mIsPRESETZHardActive)&&(!mIsPRESETZCmdReceive)) {
            mIsPRESETZCmdReceive = true;
            re_printf("info","The model will be reset (PRESETZ) for %f ns after %f ns.\n", period, start_time);
            mPRESETZCmdEvent.notify(start_time, SC_NS);
            mPRESETZCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "HRESETZ") {
        if ((!mIsHRESETZHardActive)&&(!mIsHRESETZCmdReceive)) {
            mIsHRESETZCmdReceive = true;
            re_printf("info","The model will be reset (HRESETZ) for %f ns after %f ns.\n", period, start_time);
            mHRESETZCmdEvent.notify(start_time, SC_NS);
            mHRESETZCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of HRESETZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error","The reset name (%s) is wrong. It should be PRESETZ or HRESETZ.\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit for PCLK and HCLK
void AHB_UIMPL::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
                mErrorAccessIgnoreEvent.cancel();
                mStatusClearEvent.cancel();
            }
        } else if (word_vector[0] == "HCLK") {
            mHCLKOrgFreq = freq;
            ConvertClockFreq (mHCLKFreq, mHCLKFreqUnit, freq, unit);
            if (mHCLKFreq > 0) {
                mHCLKPeriod = (sc_dt::uint64)(((1/(double)mHCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mHCLKPeriod = 0;
                mHandleTransInfoEvent.cancel();
                mWriteIntrEvent.cancel();
                mClearIntrEvent.cancel();
                mErrorAccessIgnoreEvent.cancel();
                mStatusClearEvent.cancel();
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock value of PCLK and HCLK
void AHB_UIMPL::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "HCLK") {
            re_printf("info","HCLK frequency is %0.0f %s.\n", (double)mHCLKOrgFreq, mHCLKFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void AHB_UIMPL::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("force");
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mAHBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Release forced value after forcing registers
void AHB_UIMPL::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("release");
    mAHBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Write value to registers by software
void AHB_UIMPL::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mAHBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void AHB_UIMPL::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    mAHBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///List all reigsters name
void AHB_UIMPL::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mAHBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void AHB_UIMPL::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_DumpInterrupt (true/false)                    Enable/disable dumping interrupt information (default: false).");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_DumpFileNameLineNum (true/false)              Select dump information about file name and line number. (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_DumpStatusInfo                                Dump the status information.");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_AssertReset (reset_name, start_time, period)  Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to PCLK, HCLK clock.");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_GetCLKFreq (clock_name)                       Get clock frequency of PCLK, HCLK clock.");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"AHB_UIMPL_ListRegister                                  List all registers of the model.");
        } else {
            re_printf("error","The name (%s) of AHB_UIMPL_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of AHB_UIMPL_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void AHB_UIMPL::SetMessageLevel (const std::string msg_lv)
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
    mAHBERRSLV_UIMPL->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAHBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void AHB_UIMPL::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mAHBERRSLV_UIMPL->RegisterHandler(cmd);
}//}}}

///Enable/disable dump interrupt information
void AHB_UIMPL::DumpInterrupt (const std::string is_enable)
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
        re_printf("info","AHB_UIMPL_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("error","Invalid argument: AHB_UIMPL_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Handle PCLK signal
void AHB_UIMPL::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle HCLK signal
void AHB_UIMPL::HandleHCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = HCLK.read();
    SetCLKFreq("HCLK", freq_value, "Hz");
}//}}}

///Handle PRESETZ signal
void AHB_UIMPL::HandlePRESETZSignalMethod (void)
{//{{{
    mPRESETZSignalCurrentValue = PRESETZ.read();
    if (mPRESETZSignalCurrentValue != mPRESETZSignalPreviousValue) {//mPRESETZSignalPreviousValue is "true" at initial state
        mPRESETZSignalPreviousValue = mPRESETZSignalCurrentValue;
        if (mPCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mPRESETZHardEvent.notify(CalculateCLKPosEdge("PCLK",false, current_time), mTimeResolutionUnit);
        } else {
            mPRESETZHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Handle HRESETZ signal
void AHB_UIMPL::HandleHRESETZSignalMethod (void)
{//{{{
    mHRESETZSignalCurrentValue = HRESETZ.read();
    if (mHRESETZSignalCurrentValue != mHRESETZSignalPreviousValue) {//mHRESETZSignalPreviousValue is "true" at initial state
        mHRESETZSignalPreviousValue = mHRESETZSignalCurrentValue;
        if (mHCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mHRESETZHardEvent.notify(CalculateCLKPosEdge("HCLK",false, current_time), mTimeResolutionUnit);
        } else {
            mHRESETZHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when PRESETZ is active
void AHB_UIMPL::HandlePRESETZHardMethod (void)
{//{{{
    if (mPRESETZSignalCurrentValue == emResetActive) {
        mIsPRESETZHardActive = true;
        re_printf("info","The reset signal PRESETZ is asserted.\n");
        this->EnableReset("PRESETZ", mIsPRESETZHardActive);
    } else if (mIsPRESETZHardActive) {
        mIsPRESETZHardActive = false;
        this->EnableReset("PRESETZ", mIsPRESETZHardActive);
        re_printf("info","The reset signal PRESETZ is de-asserted.\n");
    }
    //Cancel AssertReset command when PRESETZ is active
    if (mIsPRESETZCmdReceive) {
        mPRESETZCmdEvent.cancel();
        mPRESETZCmdCancelEvent.cancel();
        mIsPRESETZCmdReceive = false;
        mIsPRESETZCmdActive = false;
    }
}//}}}

///Process reset function when HRESETZ is active
void AHB_UIMPL::HandleHRESETZHardMethod (void)
{//{{{
    if (mHRESETZSignalCurrentValue == emResetActive) {
        mIsHRESETZHardActive = true;
        re_printf("info","The reset signal HRESETZ is asserted.\n");
        this->EnableReset("HRESETZ", mIsHRESETZHardActive);
    } else if (mIsHRESETZHardActive) {
        mIsHRESETZHardActive = false;
        this->EnableReset("HRESETZ", mIsHRESETZHardActive);
        re_printf("info","The reset signal HRESETZ is de-asserted.\n");
    }
    //Cancel AssertReset command when HRESETZ is active
    if (mIsHRESETZCmdReceive) {
        mHRESETZCmdEvent.cancel();
        mHRESETZCmdCancelEvent.cancel();
        mIsHRESETZCmdReceive = false;
        mIsHRESETZCmdActive = false;
    }
}//}}}

///Process reset function when PRESETZ command is active
void AHB_UIMPL::HandlePRESETZCmdMethod (void)
{//{{{
    mIsPRESETZCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETZ.\n");
    this->EnableReset("PRESETZ", mIsPRESETZCmdActive);
    mPRESETZCmdCancelEvent.notify(mPRESETZCmdPeriod, SC_NS);
}//}}}

///Process reset function when HRESETZ command is active
void AHB_UIMPL::HandleHRESETZCmdMethod (void)
{//{{{
    mIsHRESETZCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of HRESETZ.\n");
    this->EnableReset("HRESETZ", mIsHRESETZCmdActive);
    mHRESETZCmdCancelEvent.notify(mHRESETZCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when PRESETZ command is active
void AHB_UIMPL::CancelPRESETZCmdMethod (void)
{//{{{
    mIsPRESETZCmdActive = false;
    mIsPRESETZCmdReceive = false;
    re_printf("info","Reset period of PRESETZ is over.\n");
    this->EnableReset("PRESETZ", mIsPRESETZCmdActive); 
}//}}}

///Cancel reset function when HRESETZ command is active
void AHB_UIMPL::CancelHRESETZCmdMethod (void)
{//{{{
    mIsHRESETZCmdActive = false;
    mIsHRESETZCmdReceive = false;
    re_printf("info","Reset period of HRESETZ is over.\n");
    this->EnableReset("HRESETZ", mIsHRESETZCmdActive); 
}//}}}

///Handle the transaction information
void AHB_UIMPL::HandleTransInfoMethod (void)
{//{{{
    mAHBERRSLV_UIMPL->HandleTransInfo();
}//}}}

///Write the value to interrupt port
void AHB_UIMPL::WriteINTAHBMethod (void)
{//{{{
    INTAHB.write(mIntrValue);
    if (mDumpInterrupt) {
        if (mIntrValue) {
            re_printf("info","INT [INTAHB] Assert.\n");
        } else {
            if ((!mIsHRESETZHardActive)&&(!mIsHRESETZCmdActive)) {
                re_printf("info","INT [INTAHB] De-assert.\n");
            }
        }
    }
}//}}}

///Clear interrupt port
void AHB_UIMPL::ClearINTAHBMethod (void)
{//{{{
    mIntrValue = false;
    mWriteIntrEvent.notify();
}//}}}

///Ignore error access
void AHB_UIMPL::ErrorAccessIgnoreMethod (void)
{//{{{          
    mErrorAccessIgnore = false;
}//}}}

///Clear error status
void AHB_UIMPL::StatusClearMethod (void)
{//{{{         
    mStatusClearIgnore = false;
    if (mClearOnlyOVF) {
        re_printf("info", "Overflow error flag (OVF) is cleared\n");
    } else {
        re_printf("info", "Error flags (OVF, ERR) are cleared\n");
    }
    mAHBERRSLV_UIMPL->ClearErrorFlags (mClearOnlyOVF);
}//}}}

///Process reset function
void AHB_UIMPL::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mAHBERRSLV_UIMPL->Reset(reset_name, is_active);
    if ((reset_name == "HRESETZ") && (is_active)) {//Initialize variables, output ports and cancel events
        mHandleTransInfoEvent.cancel();
        mWriteIntrEvent.cancel();
        mClearIntrEvent.cancel();
        mErrorAccessIgnoreEvent.cancel();
        mStatusClearEvent.cancel();
        mIntrValue = false;
        mErrorAccessIgnore = false;
        mStatusClearIgnore = false;
        mClearOnlyOVF = true;
        mWriteIntrEvent.notify();
    }
}//}}}

///Check frequency value and frequency unit of PCLK clock
void AHB_UIMPL::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
void AHB_UIMPL::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void AHB_UIMPL::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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
double AHB_UIMPL::CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point)
{//{{{
    double period_num = 0;
    if (clock_name == "PCLK") {
        period_num = time_point / (double)mPCLKPeriod;
    } else {//clock_name is HCLK
        period_num = time_point / (double)mHCLKPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if (!add_period) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "PCLK") {
        pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - time_point;
    } else {//clock_name is HCLK
        pos_edge_point = (double)mod_period_num * (double)mHCLKPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Check period value of PCLK, HCLK clock
bool AHB_UIMPL::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "PCLK")?mPCLKPeriod:mHCLKPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Control interrupt port INTAHB
void AHB_UIMPL::ControlIntr (void)
{//{{{
    double current_time = sc_time_stamp().to_double(); //get current time
    //synchronization with HCLK before adding 2*HCLK
    double time_to_hclk_first = CalculateCLKPosEdge("HCLK", false, current_time) + 2*(double)mHCLKPeriod;
    //adding synchronization with PCLK before adding 3*HCLK
    double time_to_pclk = CalculateCLKPosEdge("PCLK", true, current_time + time_to_hclk_first) + 3*(double)mPCLKPeriod;
    //synchronization with HCLK
    double time_to_hclk_second = CalculateCLKPosEdge("HCLK", false, current_time + time_to_hclk_first + time_to_pclk);
    mIntrValue = true;
    //Assert INTAHB after 2*HCLK + Max(PCLK) + 3*PCLK
    mWriteIntrEvent.notify(time_to_hclk_first + time_to_pclk, mTimeResolutionUnit);
    //Clear INTAHB after 1*PCLK
    mClearIntrEvent.notify(time_to_hclk_first + time_to_pclk + (double)mPCLKPeriod, mTimeResolutionUnit);
    //Release restricted time space 2*HCLK + Max(PCLK) + 3*PCLK + Max (HCLK)
    mErrorAccessIgnoreEvent.notify(time_to_hclk_first + time_to_pclk + time_to_hclk_second, mTimeResolutionUnit);
}//}}}

///Transfer transaction infomation to AHBERRSLV_UIMPL block
void AHB_UIMPL::TransferTransInfo (TlmBasicPayload &trans)
{//{{{
    if (CheckClockPeriod("PCLK")&&CheckClockPeriod("HCLK")) {
        double current_time = sc_time_stamp().to_double();
        if (!mErrorAccessIgnore) {
            mErrorAccessIgnore = true;
            mAHBERRSLV_UIMPL->InformTransInfo(trans);
            mHandleTransInfoEvent.notify(CalculateCLKPosEdge("HCLK", true, current_time), mTimeResolutionUnit);
        }
    }
}//}}}

///Check reset status
bool AHB_UIMPL::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "HRESETZ") {
        if (mIsHRESETZHardActive || mIsHRESETZCmdActive) {
            reset_status = true;
        }
    } else {//PRESETZ
        if (mIsPRESETZHardActive || mIsPRESETZCmdActive) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///Wait a synchronous time to clear error status
void AHB_UIMPL::WaitTimeToClearErrorFlags (bool ovf_flag)
{//{{{       
    if (CheckClockPeriod("PCLK")&&CheckClockPeriod("HCLK")) {
        double current_time = sc_time_stamp().to_double();
        //synchronization with PCLK before adding 3*PCLK and 2*HCLK
        double time_to_pclk_first = CalculateCLKPosEdge("PCLK", false, current_time) + 3*(double)mPCLKPeriod + 2*(double)mHCLKPeriod;
        //adding synchronization with HCLK
        double time_to_hclk =  CalculateCLKPosEdge("HCLK", true, current_time + time_to_pclk_first);
        //adding synchronization with PCLK
        double time_to_pclk_second = CalculateCLKPosEdge("PCLK", true, current_time + time_to_pclk_first + time_to_hclk);
        if(!mStatusClearIgnore){
            mStatusClearIgnore = true;
            if (ovf_flag) {
                mClearOnlyOVF = true;
            } else {
                mClearOnlyOVF = false;
            }
            mStatusClearEvent.notify(time_to_pclk_first + time_to_hclk + time_to_pclk_second, mTimeResolutionUnit);
        }    
    }        
}//}}}
// vim600: set foldmethod=marker :
