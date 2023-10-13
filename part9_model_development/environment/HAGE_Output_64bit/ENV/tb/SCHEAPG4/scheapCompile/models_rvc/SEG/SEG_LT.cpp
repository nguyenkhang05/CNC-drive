// ---------------------------------------------------------------------
// $Id: SEG_LT.cpp,v 1.9 2015/06/12 08:20:40 dungvannguyen Exp $
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
#include "SEG_LT.h"
#include "SEG_Func.h" 

///Constructor of SEG class
SEG::SEG (sc_module_name name,
          unsigned int rLatency,
          unsigned int wLatency,
          std::string SEG_VERSION):
    sc_module(name),
    SEG_AgentController(),
    BusSlaveBase<32,1>(),
    pclk("pclk"),
    clk("clk"),
    presetz("presetz"),
    reset("reset"),
    VciErrInfo("VciErrInfo"),
    TcmErrInfo("TcmErrInfo"),
    IpgErrInfo("IpgErrInfo"),
    cmt1cmSyserrAckNm("cmt1cmSyserrAckNm"),
    g1nnSyserrReqNm("g1nnSyserrReqNm"),
    g1nnSyserrCodeNm("g1nnSyserrCodeNm"),
    g1nnSyserrEbvNm("g1nnSyserrEbvNm")
{//{{{
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&pclk);
    setSlaveResetPort32(&presetz);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mSEG_Func = new SEG_Func((std::string)name, this, SEG_VERSION);
    sc_assert(mSEG_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mSEG_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);
   
    //Initialize output signals
    g1nnSyserrReqNm.initialize(false);
    g1nnSyserrCodeNm.initialize(0);
    g1nnSyserrEbvNm.initialize(false);

    //Initialize variables
    mSEGVersion = SEG_VERSION;
    mpclkPeriod = 0;
    mclkPeriod = 0;
    mpclkFreq = 0;
    mclkFreq = 0;
    mpclkOrgFreq = 0;
    mclkOrgFreq = 0;
    mpclkFreqUnit = "Hz"; 
    mclkFreqUnit = "Hz"; 
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mpresetzSignalCurrentValue = true;
    mpresetzSignalPreviousValue = true;
    mIspresetzHardActive = false;
    mIspresetzCmdReceive = false;
    mIspresetzCmdActive = false;
    mpresetzCmdPeriod =  0;

    mresetSignalCurrentValue = false;
    mresetSignalPreviousValue = false;
    mIsresetHardActive = false;
    mIsresetCmdReceive = false;
    mIsresetCmdActive = false;
    mresetCmdPeriod =  0; 

    mVciErrInfoValue.Initialize();
    mTcmErrInfoValue.Initialize();
    mIpgErrInfoValue.Initialize();
    mErrFactorOutputSignalValue = 0;
    mErrRequestOutputSignalValue = false;
    mIsErrRequestOutputSignal = true;
    mIsErrFactorOutputSignal = true;

    SC_METHOD(HandlepclkSignalMethod);
    dont_initialize();
    sensitive<<pclk;
 
    SC_METHOD(HandleclkSignalMethod);
    dont_initialize();
    sensitive<<clk;
 
    SC_METHOD(HandlepresetzSignalMethod);
    sensitive<<presetz;
    
    SC_METHOD(HandlepresetzHardMethod);
    dont_initialize();
    sensitive<<mpresetzHardEvent;

    SC_METHOD(HandlepresetzCmdMethod);
    dont_initialize();
    sensitive<<mpresetzCmdEvent;
 
    SC_METHOD(CancelpresetzCmdMethod);
    dont_initialize();
    sensitive<<mpresetzCmdCancelEvent;

    SC_METHOD(HandleresetSignalMethod);
    sensitive<<reset;
    
    SC_METHOD(HandleresetHardMethod);
    dont_initialize();
    sensitive<<mresetHardEvent;

    SC_METHOD(HandleresetCmdMethod);
    dont_initialize();
    sensitive<<mresetCmdEvent;
 
    SC_METHOD(CancelresetCmdMethod);
    dont_initialize();
    sensitive<<mresetCmdCancelEvent;

    SC_METHOD(ReceiveVciErrInfoSignalMethod);
    dont_initialize();
    sensitive<<VciErrInfo;

    SC_METHOD(ReceiveTcmErrInfoSignalMethod);
    dont_initialize();
    sensitive<<TcmErrInfo;

    SC_METHOD(ReceiveIpgErrInfoSignalMethod);
    dont_initialize();
    sensitive<<IpgErrInfo;

    SC_METHOD(HandleVciErrInfoMethod);
    dont_initialize();
    sensitive<<mVciErrInfoEvent;

    SC_METHOD(HandleTcmErrInfoMethod);
    dont_initialize();
    sensitive<<mTcmErrInfoEvent;

    SC_METHOD(HandleIpgErrInfoMethod);
    dont_initialize();
    sensitive<<mIpgErrInfoEvent;

    SC_METHOD(Receivecmt1cmSyserrAckNmSignalMethod);
    dont_initialize();
    sensitive<<cmt1cmSyserrAckNm.pos();
   
    SC_METHOD(Handlecmt1cmSyserrAckNmMethod);
    dont_initialize();
    sensitive<<mg1nnSyserrReqNmEvent;

    SC_METHOD(HanldeErrorOutputMethod);
    dont_initialize();
    sensitive<<mErrorOutputEvent;

    SC_METHOD(ArbitrateErrorsMethod);
    dont_initialize();
    sensitive<<mArbitrateErrorsEvent;
}//}}}

///Destructor of SEG class
SEG::~SEG (void)
{//{{{
    delete mSEG_Func;
}//}}}

///Update SEGCONT register value at initial state, it is called by SystemC kernel
void SEG::start_of_simulation (void)
{//{{{
    mSEG_Func->StartOfSimulation();
}//}}}

///Dump information of registers
void SEG::DumpStatusInfo (void)
{//{{{
    mSEG_Func->DumpStatusInfo();
}//}}}

///Assert reset by software for presetz and reset
void SEG::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetz") {
        if ((!mIspresetzHardActive)&&(!mIspresetzCmdReceive)) {
            mIspresetzCmdReceive = true;
            re_printf("info","The model will be reset (presetz) for %f ns after %f ns.\n", period, start_time);
            mpresetzCmdEvent.notify(start_time, SC_NS);
            mpresetzCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of presetz is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "reset") {
        if ((!mIsresetHardActive)&&(!mIsresetCmdReceive)) {
            mIsresetCmdReceive = true;
            re_printf("info","The model will be reset (reset) for %f ns after %f ns.\n", period, start_time);
            mresetCmdEvent.notify(start_time, SC_NS);
            mresetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of reset is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error","The reset name (%s) is wrong. It should be presetz or reset.\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit for pclk and clk
void SEG::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            mpclkOrgFreq = freq;
            ConvertClockFreq (mpclkFreq, mpclkFreqUnit, freq, unit);
            if (mpclkFreq > 0) {
                mpclkPeriod = (sc_dt::uint64)(((1/(double)mpclkFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mpclkPeriod = 0;
            }
        } else if (word_vector[0] == "clk") {
            mclkOrgFreq = freq;
            ConvertClockFreq (mclkFreq, mclkFreqUnit, freq, unit);
            if (mclkFreq > 0) {
                mclkPeriod = (sc_dt::uint64)(((1/(double)mclkFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mclkPeriod = 0;
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock value of pclk and clk
void SEG::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s.\n", (double)mpclkOrgFreq, mpclkFreqUnit.c_str());
        } else if (word_vector[0] == "clk") {
            re_printf("info","clk frequency is %0.0f %s.\n", (double)mclkOrgFreq, mclkFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void SEG::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mSEG_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void SEG::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mSEG_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void SEG::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mSEG_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void SEG::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mSEG_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///List all reigsters name
void SEG::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mSEG_Func->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void SEG::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"SEG_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"SEG_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"SEG_VLD_INI (value)                               Set initial value to register SEGCONT.");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"SEG_DumpStatusInfo                                Dump the status information.");
            SC_REPORT_INFO(this->basename(),"SEG_AssertReset (reset_name, start_time, period)  Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"SEG_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to pclk, clk clock.");
            SC_REPORT_INFO(this->basename(),"SEG_GetCLKFreq (clock_name)                       Get clock frequency of pclk, clk clock.");
            SC_REPORT_INFO(this->basename(),"SEG_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"SEG_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"SEG_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"SEG_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"SEG_ListRegister                                  List all registers of the model.");
        } else {
            re_printf("error","The name (%s) of SEG_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of SEG_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void SEG::SetMessageLevel (const std::string msg_lv)
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
    mSEG_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mSEG_Func->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void SEG::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mSEG_Func->RegisterHandler(cmd);
}//}}}

///Set initial value to register SEGCONT
void SEG::SetSEGCONTInitialValue (const unsigned int value)
{//{{{
    mSEG_Func->SetSEGCONTInitValue(value);
}//}}}

///Handle pclk signal
void SEG::HandlepclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

///Handle clk signal
void SEG::HandleclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = clk.read();
    SetCLKFreq("clk", freq_value, "Hz");
}//}}}

///Handle presetz signal
void SEG::HandlepresetzSignalMethod (void)
{//{{{
    mpresetzSignalCurrentValue = presetz.read();
    if (mpresetzSignalCurrentValue != mpresetzSignalPreviousValue) {//mpresetzSignalPreviousValue is "true" at initial state
        mpresetzSignalPreviousValue = mpresetzSignalCurrentValue;
        if (mpclkPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mpresetzHardEvent.notify(CalculateCLKPosEdge("pclk", current_time), mTimeResolutionUnit);
        } else {
            mpresetzHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Handle reset signal
void SEG::HandleresetSignalMethod (void)
{//{{{
    mresetSignalCurrentValue = reset.read();
    if (mresetSignalCurrentValue != mresetSignalPreviousValue) {//mresetSignalPreviousValue is "false" at initial state
        mresetSignalPreviousValue = mresetSignalCurrentValue;
        if (mclkPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mresetHardEvent.notify(CalculateCLKPosEdge("clk", current_time), mTimeResolutionUnit);
        } else {
            mresetHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when presetz is active
void SEG::HandlepresetzHardMethod (void)
{//{{{
    if (mpresetzSignalCurrentValue == empresetzActive) {
        mIspresetzHardActive = true;
        re_printf("info","The reset signal presetz is asserted.\n");
        this->EnableReset("presetz", mIspresetzHardActive);
    } else if (mIspresetzHardActive) {
        mIspresetzHardActive = false;
        this->EnableReset("presetz", mIspresetzHardActive);
        re_printf("info","The reset signal presetz is de-asserted.\n");
    }
    //Cancel AssertReset command when presetz is active
    if (mIspresetzCmdReceive) {
        mpresetzCmdEvent.cancel();
        mpresetzCmdCancelEvent.cancel();
        mIspresetzCmdReceive = false;
        mIspresetzCmdActive = false;
    }
}//}}}

///Process reset function when reset is active
void SEG::HandleresetHardMethod (void)
{//{{{
    if (mresetSignalCurrentValue == emresetActive) {
        mIsresetHardActive = true;
        re_printf("info","The reset signal reset is asserted.\n");
        this->EnableReset("reset", mIsresetHardActive);
    } else if (mIsresetHardActive) {
        mIsresetHardActive = false;
        this->EnableReset("reset", mIsresetHardActive);
        re_printf("info","The reset signal reset is de-asserted.\n");
    }
    //Cancel AssertReset command when reset is active
    if (mIsresetCmdReceive) {
        mresetCmdEvent.cancel();
        mresetCmdCancelEvent.cancel();
        mIsresetCmdReceive = false;
        mIsresetCmdActive = false;
    }
}//}}}

///Process reset function when presetz command is active
void SEG::HandlepresetzCmdMethod (void)
{//{{{
    mIspresetzCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of presetz.\n");
    this->EnableReset("presetz", mIspresetzCmdActive);
    mpresetzCmdCancelEvent.notify(mpresetzCmdPeriod, SC_NS);
}//}}}

///Process reset function when reset command is active
void SEG::HandleresetCmdMethod (void)
{//{{{
    mIsresetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of reset.\n");
    this->EnableReset("reset", mIsresetCmdActive);
    mresetCmdCancelEvent.notify(mresetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when presetz command is active
void SEG::CancelpresetzCmdMethod (void)
{//{{{
    mIspresetzCmdActive = false;
    mIspresetzCmdReceive = false;
    re_printf("info","Reset period of presetz is over.\n");
    this->EnableReset("presetz", mIspresetzCmdActive); 
}//}}}

///Cancel reset function when reset command is active
void SEG::CancelresetCmdMethod (void)
{//{{{
    mIsresetCmdActive = false;
    mIsresetCmdReceive = false;
    re_printf("info","Reset period of reset is over.\n");
    this->EnableReset("reset", mIsresetCmdActive); 
}//}}}

///Receive VciErrorInfo signal
void SEG::ReceiveVciErrInfoSignalMethod (void)
{//{{{
    if (mSEGVersion != "G3KH") {
        if (!GetResetStatus("reset")) {
            if (CheckClockPeriod("clk")) {
                Csyserrvci VciErrInfoValueCurrent;
                VciErrInfoValueCurrent = VciErrInfo.read();
                if (VciErrInfoValueCurrent.SysErrVCIVld) {//VCI error system occurs
                    re_printf("info","VCI error system occurs\n");
                    mVciErrInfoValue = VciErrInfoValueCurrent;
                    double current_time = sc_time_stamp().to_double();
                    mVciErrInfoEvent.notify(CalculateCLKPosEdge("clk", current_time) + (double)mclkPeriod, mTimeResolutionUnit);
                }
            }
        } else {
            re_printf("error","Can not receive error during reset operation.\n");
        }
    }
}//}}}

///Receive TcmErrorInfo signal
void SEG::ReceiveTcmErrInfoSignalMethod (void)
{//{{{
    if (!GetResetStatus("reset")) {
        if (CheckClockPeriod("clk")) {
            Csyserrtcm TcmErrInfoValueCurrent;
            TcmErrInfoValueCurrent = TcmErrInfo.read();
            if (TcmErrInfoValueCurrent.SysErrTCMVld) {//TCM error system occurs
                re_printf("info","TCM error system occurs\n");
                mTcmErrInfoValue = TcmErrInfoValueCurrent;
                double current_time = sc_time_stamp().to_double();
                mTcmErrInfoEvent.notify(CalculateCLKPosEdge("clk", current_time) + (double)mclkPeriod,mTimeResolutionUnit);
            }
        }
    } else {
        re_printf("error","Can not receive error during reset operation.\n");
    }
}//}}}

///Receive IpgErrorInfo signal
void SEG::ReceiveIpgErrInfoSignalMethod (void)
{//{{{
    if (!GetResetStatus("reset")) {
        if (CheckClockPeriod("clk")) {
            Csyserripg IpgErrInfoValueCurrent;
            IpgErrInfoValueCurrent = IpgErrInfo.read();
            if (IpgErrInfoValueCurrent.SysErrIPG) {//IPG error system occurs
                re_printf("info","IPG error system occurs\n");
                mIpgErrInfoValue = IpgErrInfoValueCurrent;
                double current_time = sc_time_stamp().to_double();
                mIpgErrInfoEvent.notify(CalculateCLKPosEdge("clk", current_time) + (double)mclkPeriod,mTimeResolutionUnit);
            }
        }
    } else {
        re_printf("error","Can not receive error during reset operation.\n");
    }
}//}}}

///Receive ecmt1cmSyserrAckNm signal
void SEG::Receivecmt1cmSyserrAckNmSignalMethod (void)
{//{{{
    if (!GetResetStatus("reset")) {
        if (CheckClockPeriod("clk")) {
            re_printf("info","Receive acknowledge from input port cmt1cmSyserrAckNm\n");
            double current_time = sc_time_stamp().to_double();
            mg1nnSyserrReqNmEvent.notify(CalculateCLKPosEdge("clk", current_time) + (double)mclkPeriod,mTimeResolutionUnit);
        }
    } else {
        re_printf("error","Can not receive acknowledge from CPU during reset operation.\n");
    }
}//}}}

///Handle VCI Error Info 
void SEG::HandleVciErrInfoMethod (void)
{//{{{
    if (CheckClockPeriod("clk")) {
        mSEG_Func->ExchangeErrorInfo(mVciErrInfoValue, mTcmErrInfoValue, mIpgErrInfoValue);
        if (mVciErrInfoValue.SysErrVCI) {
            mSEG_Func->UpdateSEGFLAGRegister(true,"vcierror_both");
        } else {
            mSEG_Func->UpdateSEGFLAGRegister(true,"vcierror_only");
        }
    }
}//}}}

///Handle TCM Error Info
void SEG::HandleTcmErrInfoMethod (void)
{//{{{
    if (CheckClockPeriod("clk")) {
        mSEG_Func->ExchangeErrorInfo(mVciErrInfoValue, mTcmErrInfoValue, mIpgErrInfoValue);
        if (mTcmErrInfoValue.SysErrTCM) {
            mSEG_Func->UpdateSEGFLAGRegister(true,"tcmerror_both");
        } else {
            mSEG_Func->UpdateSEGFLAGRegister(true,"tcmerror_only");
        }
    }
}//}}}

///Handle IPG Error Info
void SEG::HandleIpgErrInfoMethod (void)
{//{{{
    if (CheckClockPeriod("clk")) {
        mSEG_Func->ExchangeErrorInfo(mVciErrInfoValue, mTcmErrInfoValue, mIpgErrInfoValue);
        mSEG_Func->UpdateSEGFLAGRegister(true,"ipgerror");
    }
}//}}}

///Handle ecmt1cmSyserrAckNm operation
void SEG::Handlecmt1cmSyserrAckNmMethod (void)
{//{{{
    if (CheckClockPeriod("clk")) {
        mIsErrRequestOutputSignal = true;
        mIsErrFactorOutputSignal = false;
        mErrRequestOutputSignalValue = false;
        WriteErrorOutputSignal(mIsErrRequestOutputSignal, mIsErrFactorOutputSignal);
    }
}//}}}

///Handle error output
void SEG::HanldeErrorOutputMethod (void)
{//{{{
    if (CheckClockPeriod("clk")) {   
        mIsErrFactorOutputSignal = true;
        mErrRequestOutputSignalValue = true;
        WriteErrorOutputSignal(mIsErrRequestOutputSignal, mIsErrFactorOutputSignal);
    }
}//}}}

///Arbitrate errors to retain error information
void SEG::ArbitrateErrorsMethod (void)
{//{{{
    mSEG_Func->RetainErrInfo();
}//}}}

void SEG::WriteErrorOutputSignal (const bool is_err_request_output_signal, const bool is_err_factor_output_signal)
{//{{{
    if (is_err_request_output_signal) {//Only active signal g1nnSyserrReqNm when error is from error input signal with SEGFLAG != 0
        g1nnSyserrReqNm.write(mErrRequestOutputSignalValue);
    }
    if (is_err_factor_output_signal) {//If it is not acknowledge request
        g1nnSyserrCodeNm.write((sc_uint<4>)mErrFactorOutputSignalValue);
    }
}//}}}

///Process reset function
void SEG::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mSEG_Func->Reset(reset_name, is_active);
    if (is_active) {
        if (reset_name == "reset") {//Initialize variables, output signals and cancel events
            mVciErrInfoValue.Initialize();
            mTcmErrInfoValue.Initialize();
            mIpgErrInfoValue.Initialize();
            mErrFactorOutputSignalValue = 0;
            mErrRequestOutputSignalValue = false;
            mIsErrRequestOutputSignal = true;
            mIsErrFactorOutputSignal = true;
            WriteErrorOutputSignal(mIsErrRequestOutputSignal, mIsErrFactorOutputSignal);
            g1nnSyserrEbvNm.write(false);
            mVciErrInfoEvent.cancel();
            mTcmErrInfoEvent.cancel();
            mIpgErrInfoEvent.cancel();
            mg1nnSyserrReqNmEvent.cancel();
            mErrorOutputEvent.cancel();
            mArbitrateErrorsEvent.cancel();
        } else {
            mIsErrRequestOutputSignal = false;
            mIsErrFactorOutputSignal = true;
            mErrFactorOutputSignalValue = 0;
            WriteErrorOutputSignal(mIsErrRequestOutputSignal, mIsErrFactorOutputSignal);
        }
    }
}//}}}

///Check frequency value and frequency unit of pclk clock
void SEG::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
void SEG::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void SEG::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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
double SEG::CalculateCLKPosEdge (const std::string clock_name, const double time_point)
{//{{{
    double period_num = 0;
    if (clock_name == "pclk") {
        period_num = time_point / (double)mpclkPeriod;
    } else {//clock_name is clk
        period_num = time_point / (double)mclkPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "pclk") {
        pos_edge_point = (double)mod_period_num * (double)mpclkPeriod - time_point;
    } else {//clock_name is clk
        pos_edge_point = (double)mod_period_num * (double)mclkPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Check reset status
bool SEG::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "reset") {
        if (mIsresetHardActive || mIsresetCmdActive) {
            reset_status = true;
        }
    } else {//presetz
        if (mIspresetzHardActive || mIspresetzCmdActive) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///Check period value of pclk, clk clock
bool SEG::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "pclk")?mpclkPeriod:mclkPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Notify error output
void SEG::NotifyErrorOutput (const unsigned char index, const bool is_err_request_output_signal)
{//{{{
    mErrFactorOutputSignalValue = index;
    mIsErrRequestOutputSignal = is_err_request_output_signal;
    double current_time = sc_time_stamp().to_double();
    mErrorOutputEvent.notify(CalculateCLKPosEdge("clk", current_time),mTimeResolutionUnit);
}//}}}

///Arbitrate errors
void SEG::ArbitrateErrors (void)
{//{{{
    mArbitrateErrorsEvent.notify(SC_ZERO_TIME);
}//}}}
// vim600: set foldmethod=marker :
