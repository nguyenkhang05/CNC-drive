// ---------------------------------------------------------------------
// $Id: IPG_LT.cpp,v 1.5 2015/06/12 08:19:09 dungvannguyen Exp $
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
#include "BusBridgeModuleBase.h"
#include "IPG_LT.h"
#include "IPG_DataHandlerLSU.h"
#include "IPG_DataHandlerIFU.h"
#include "IPG_Func.h" 

///Constructor of IPG class
IPG::IPG (sc_module_name name,
          unsigned int rLatency,
          unsigned int wLatency,
          std::string IPG_VERSION):
    sc_module(name),
    IPG_AgentController(),
    BusBridgeSlaveBase<32,2>(),
    BusMasterBase<32,1>(),
    BusSlaveBase<32,1>(),
    pclk("pclk"),
    lvcclk("lvcclk"),
    presetz("presetz"),
    lvcirreset("lvcirreset"),
    IpgErrInfo("IpgErrInfo"),
    mresetPortSig("mresetPortSig")
{//{{{
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&pclk);
    setSlaveResetPort32(&presetz);
    setTargetSocket32("tsp");

    setMasterResetPort32(&lvcirreset);
    setMasterFreqPort32(&lvcclk);
    setInitiatorSocket32("is");

    setSlaveResetPort(&lvcirreset,&lvcirreset);
    setSlaveFreqPort(&lvcclk,&lvcclk);
    setTargetSocket("tsl","tsi");

    tsp = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding
    tsl = BusBridgeSlaveBase<32,2>::tSocket[0]; //Alias for binding
    tsi = BusBridgeSlaveBase<32,2>::tSocket[1]; //Alias for binding
    is = iSocket32[0]; //Alias for binding

    mBusBridgeModule[0] = new BusBridgeModuleBase<32,32> ((std::string(name) + "_LSU").c_str());
    mBusBridgeModule[1] = new BusBridgeModuleBase<32,32> ((std::string(name) + "_IFU").c_str());
    (mBusBridgeModule[0]->resetPort).bind(mresetPortSig);
    (mBusBridgeModule[1]->resetPort).bind(mresetPortSig);
    mresetPortSig.write(true);//Inactivate resetPort of BusBridgeModuleBase's instances

    mIPG_Func = new IPG_Func((std::string)name, this, IPG_VERSION);
    sc_assert(mIPG_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mIPG_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    mIPG_DataHandlerLSU = new IPG_DataHandlerLSU((std::string)name, this->mBusBridgeModule[0], mBusSlaveIf[0], mBusMasterIf32[0], this);
    mBusBridgeModule[0]->mpCore = mIPG_DataHandlerLSU;
    mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)(mBusBridgeModule[0]->mpCore));
    mBusMasterIf32[0]->setBusProtocol(BUS_VCI);
    mBusMasterIf32[0]->setTransNmbLmt(1);
    mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)(mBusBridgeModule[0]->mpCore));
    mBusSlaveIf[0]->setBusProtocol(BUS_VCI);
    mBusSlaveIf[0]->setTransNmbLmt(0xFFFFFFFF);

    mIPG_DataHandlerIFU = new IPG_DataHandlerIFU((std::string)name, this->mBusBridgeModule[1], mBusSlaveIf[1], mBusMasterIf32[0], this);
    mBusBridgeModule[1]->mpCore = mIPG_DataHandlerIFU;
    mBusSlaveIf[1]->setFuncModulePtr( (BusSlaveFuncIf *)(mBusBridgeModule[1]->mpCore));
    mBusSlaveIf[1]->setBusProtocol(BUS_VCI);
    mBusSlaveIf[1]->setTransNmbLmt(0xFFFFFFFF);

    //Initialize variables
    mpclkPeriod = 0;
    mlvcclkPeriod = 0;
    mpclkFreq = 0;
    mlvcclkFreq = 0;
    mpclkOrgFreq = 0;
    mlvcclkOrgFreq = 0;
    mpclkFreqUnit = "Hz"; 
    mlvcclkFreqUnit = "Hz"; 
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mpresetzSignalCurrentValue = true;
    mpresetzSignalPreviousValue = true;
    mIspresetzHardActive = false;
    mIspresetzCmdReceive = false;
    mIspresetzCmdActive = false;
    mpresetzCmdPeriod =  0;

    mlvcirresetSignalCurrentValue = false;
    mlvcirresetSignalPreviousValue = false;
    mIslvcirresetHardActive = false;
    mIslvcirresetCmdReceive = false;
    mIslvcirresetCmdActive = false;
    mlvcirresetCmdPeriod =  0;

    mIsErrorProcessing = false;

    mIpgErrInfoValue.Initialize();
    
    //Initialize output port
    IpgErrInfo.initialize(mIpgErrInfoValue);

    SC_METHOD(HandlepclkSignalMethod);
    dont_initialize();
    sensitive<<pclk;
 
    SC_METHOD(HandlelvcclkSignalMethod);
    dont_initialize();
    sensitive<<lvcclk;
 
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

    SC_METHOD(HandlelvcirresetSignalMethod);
    sensitive<<lvcirreset;
    
    SC_METHOD(HandlelvcirresetHardMethod);
    dont_initialize();
    sensitive<<mlvcirresetHardEvent;

    SC_METHOD(HandlelvcirresetCmdMethod);
    dont_initialize();
    sensitive<<mlvcirresetCmdEvent;
 
    SC_METHOD(CancellvcirresetCmdMethod);
    dont_initialize();
    sensitive<<mlvcirresetCmdCancelEvent;

    SC_METHOD(SendOutputErrorMethod);
    dont_initialize();
    sensitive<<mSendOutputErrorEvent;

    SC_METHOD(ClearOutputErrorMethod);
    dont_initialize();
    sensitive<<mClearOutputErrorEvent;
}//}}}

///Destructor of IPG class
IPG::~IPG (void)
{//{{{
    delete mIPG_Func;
    delete mIPG_DataHandlerLSU;
    delete mIPG_DataHandlerIFU;
    delete mBusBridgeModule[0];
    delete mBusBridgeModule[1];
}//}}}

///Set TLM transaction type
void IPG::setTlmTransType (SIM_MODE_T simMode)
{//{{{
    mBusBridgeModule[0]->setTlmTransType(simMode);
    mBusBridgeModule[1]->setTlmTransType(simMode);
}//}}}

///Set address for guarded areas
void IPG::SetGuardAreas (const std::string action, const std::string area_name,
                         const unsigned int start_addr, const unsigned int size)
{//{{{
    if ((area_name == "GVCI")||(area_name == "VPISS")||(area_name == "INTC1")||(area_name == "MECNT")||(area_name == "Debug")||
        (area_name == "SEC")||(area_name == "LVCI")||(area_name == "AXI")||(area_name == "GRAM0")||(area_name == "GRAM1")) {
        if (action == "setvalue") {
            mIPG_Func->SetGuardAreasInfo(area_name, start_addr, size);
        } else if (action == "getvalue") {
             mIPG_Func->GetGuardAreasInfo(area_name);
        } else {
            re_printf("error","The argument of action is invalid, it should be setvalue, getvalue\n");
        }
    } else {
        re_printf("error","The guarded area name is invalid, it should be GVCI, VPISS, INTC1, MECNT, Debug, SEC, LVCI, AXI, GRAM0, GRAM1\n");
    }
}//}}}

///Dump information of registers
void IPG::DumpStatusInfo (void)
{//{{{
    mIPG_Func->DumpStatusInfo();
}//}}}

///Assert reset by software for presetz and reset
void IPG::AssertReset (const std::string reset_name, const double start_time, const double period)
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
    } else if (reset_name == "lvcirreset") {
        if ((!mIslvcirresetHardActive)&&(!mIslvcirresetCmdReceive)) {
            mIslvcirresetCmdReceive = true;
            re_printf("info","The model will be reset (lvcirreset) for %f ns after %f ns.\n", period, start_time);
            mlvcirresetCmdEvent.notify(start_time, SC_NS);
            mlvcirresetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of lvcirreset is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error","The reset name (%s) is wrong. It should be presetz or lvcirreset.\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit for pclk and clk
void IPG::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
        } else if (word_vector[0] == "lvcclk") {
            mlvcclkOrgFreq = freq;
            ConvertClockFreq (mlvcclkFreq, mlvcclkFreqUnit, freq, unit);
            if (mlvcclkFreq > 0) {
                mlvcclkPeriod = (sc_dt::uint64)(((1/(double)mlvcclkFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mlvcclkPeriod = 0;
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock value of pclk and clk
void IPG::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s.\n", (double)mpclkOrgFreq, mpclkFreqUnit.c_str());
        } else if (word_vector[0] == "lvcclk") {
            re_printf("info","lvcclk frequency is %0.0f %s.\n", (double)mlvcclkOrgFreq, mlvcclkFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void IPG::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mIPG_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void IPG::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mIPG_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void IPG::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mIPG_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void IPG::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mIPG_Func->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///List all reigsters name
void IPG::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mIPG_Func->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void IPG::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"IPG_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"IPG_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"IPG_SetGuardAreas (action, area_name, start_addr, end_addr) Set/get address for guarded areas.");
            SC_REPORT_INFO(this->basename(),"IPG_DumpStatusInfo                                Dump the status information.");
            SC_REPORT_INFO(this->basename(),"IPG_AssertReset (reset_name, start_time, period)  Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"IPG_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to pclk, lvcclk clock.");
            SC_REPORT_INFO(this->basename(),"IPG_GetCLKFreq (clock_name)                       Get clock frequency of pclk, lvcclk clock.");
            SC_REPORT_INFO(this->basename(),"IPG_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"IPG_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"IPG_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"IPG_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"IPG_ListRegister                                  List all registers of the model.");
        } else {
            re_printf("error","The name (%s) of IPG_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of IPG_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void IPG::SetMessageLevel (const std::string msg_lv)
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
    mIPG_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mIPG_Func->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void IPG::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mIPG_Func->RegisterHandler(cmd);
}//}}}

///Handle pclk signal
void IPG::HandlepclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

///Handle lvcclk signal
void IPG::HandlelvcclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = lvcclk.read();
    SetCLKFreq("lvcclk", freq_value, "Hz");
}//}}}

///Handle presetz signal
void IPG::HandlepresetzSignalMethod (void)
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

///Handle lvcirreset signal
void IPG::HandlelvcirresetSignalMethod (void)
{//{{{
    mlvcirresetSignalCurrentValue = lvcirreset.read();
    mresetPortSig.write(!mlvcirresetSignalCurrentValue);
    if (mlvcirresetSignalCurrentValue != mlvcirresetSignalPreviousValue) {//mlvcirresetSignalPreviousValue is "false" at initial state
        mlvcirresetSignalPreviousValue = mlvcirresetSignalCurrentValue;
        if (mlvcclkPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mlvcirresetHardEvent.notify(CalculateCLKPosEdge("lvcclk", current_time), mTimeResolutionUnit);
        } else {
            mlvcirresetHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when presetz is active
void IPG::HandlepresetzHardMethod (void)
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

///Process reset function when lvcirreset is active
void IPG::HandlelvcirresetHardMethod (void)
{//{{{
    if (mlvcirresetSignalCurrentValue == emlvcirresetActive) {
        mIslvcirresetHardActive = true;
        re_printf("info","The reset signal lvcirreset is asserted.\n");
        this->EnableReset("lvcirreset", mIslvcirresetHardActive);
    } else if (mIslvcirresetHardActive) {
        mIslvcirresetHardActive = false;
        this->EnableReset("lvcirreset", mIslvcirresetHardActive);
        re_printf("info","The reset signal lvcirreset is de-asserted.\n");
    }
    //Cancel AssertReset command when lvcirreset is active
    if (mIslvcirresetCmdReceive) {
        mlvcirresetCmdEvent.cancel();
        mlvcirresetCmdCancelEvent.cancel();
        mIslvcirresetCmdReceive = false;
        mIslvcirresetCmdActive = false;
    }
}//}}}

///Process reset function when presetz command is active
void IPG::HandlepresetzCmdMethod (void)
{//{{{
    mIspresetzCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of presetz.\n");
    this->EnableReset("presetz", mIspresetzCmdActive);
    mpresetzCmdCancelEvent.notify(mpresetzCmdPeriod, SC_NS);
}//}}}

///Process reset function when lvcirreset command is active
void IPG::HandlelvcirresetCmdMethod (void)
{//{{{
    mIslvcirresetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of lvcirreset.\n");
    this->EnableReset("lvcirreset", mIslvcirresetCmdActive);
    mlvcirresetCmdCancelEvent.notify(mlvcirresetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when presetz command is active
void IPG::CancelpresetzCmdMethod (void)
{//{{{
    mIspresetzCmdActive = false;
    mIspresetzCmdReceive = false;
    re_printf("info","Reset period of presetz is over.\n");
    this->EnableReset("presetz", mIspresetzCmdActive); 
}//}}}

///Cancel reset function when lvcirreset command is active
void IPG::CancellvcirresetCmdMethod (void)
{//{{{
    mIslvcirresetCmdActive = false;
    mIslvcirresetCmdReceive = false;
    re_printf("info","Reset period of lvcirreset is over.\n");
    this->EnableReset("lvcirreset", mIslvcirresetCmdActive); 
}//}}}

///Send output error port
void IPG::SendOutputErrorMethod (void)
{//{{{
    if (CheckClockPeriod("lvcclk")) {
        IpgErrInfo.write(mIpgErrInfoValue);
        mClearOutputErrorEvent.notify((double)mlvcclkPeriod,mTimeResolutionUnit);
    }
}//}}}

///Clear output error port
void IPG::ClearOutputErrorMethod (void)
{//{{{
    if (CheckClockPeriod("lvcclk")) {
        mIsErrorProcessing = false;
        mIpgErrInfoValue.Initialize();
        IpgErrInfo.write(mIpgErrInfoValue);
    }
}//}}}

///Process reset function
void IPG::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mIPG_Func->Reset(reset_name, is_active);
    if ((reset_name == "lvcirreset") && (is_active)) {//Initialize variables, output ports and cancel events
        mIsErrorProcessing = false;
        mIpgErrInfoValue.Initialize();
        mSendOutputErrorEvent.cancel();
        mClearOutputErrorEvent.cancel();
        IpgErrInfo.write(mIpgErrInfoValue);
    }
}//}}}

///Check frequency value and frequency unit of pclk, lvcclk clock
void IPG::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
void IPG::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void IPG::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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
double IPG::CalculateCLKPosEdge (const std::string clock_name, const double time_point)
{//{{{
    double period_num = 0;
    if (clock_name == "pclk") {
        period_num = time_point / (double)mpclkPeriod;
    } else {//clock_name is lvcclk
        period_num = time_point / (double)mlvcclkPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "pclk") {
        pos_edge_point = (double)mod_period_num * (double)mpclkPeriod - time_point;
    } else {//clock_name is lvcclk
        pos_edge_point = (double)mod_period_num * (double)mlvcclkPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Check period value of pclk, lvcclk clock
bool IPG::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "pclk")?mpclkPeriod:mlvcclkPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Check reset status
bool IPG::GetResetStatus (void)
{//{{{
    bool reset_status = false;
    if (mIslvcirresetHardActive || mIslvcirresetCmdActive) {
        reset_status = true;
    }
    return reset_status;
}//}}}

///Check access permission
bool IPG::CheckAccessPermission (const bool is_ifu, TlmBasicPayload& trans)
{//{{{
    return mIPG_Func->CheckAccessAddress(is_ifu, trans);
}//}}}

///Send output error port
void IPG::SendOutputErrorPort (const Csyserripg err_port_value)
{//{{{
    if (!mIsErrorProcessing) {
        mIsErrorProcessing = true;
        mIpgErrInfoValue = err_port_value;
        double current_time = sc_time_stamp().to_double();
        mSendOutputErrorEvent.notify(CalculateCLKPosEdge("lvcclk", current_time) + (double)mlvcclkPeriod,mTimeResolutionUnit);
    }
}//}}}
// vim600: set foldmethod=marker :
