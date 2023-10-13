// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1.cpp,v 1.4 2020/03/27 04:14:18 duongphan Exp $
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
#include "QLAPBFICTLAV1.h"
#include "QLAPBFICTLAV1_Func.h"
#include "QLAPBFICTLAV1_Sub.h"
#include "QLAPBFICTLAV1_Sub_Func.h"

///Constructor of QLAPBFICTLAV1 class

QLAPBFICTLAV1::QLAPBFICTLAV1 (    sc_module_name name,
                                unsigned int rLatency,
                                unsigned int wLatency,
                                const unsigned int nChannelNumber):
sc_module(name),
QLAPBFICTLAV1_AgentController(),
BusSlaveBase<32,1>(),
mQlapbfictlav1Channel(nChannelNumber),
PCLK("PCLK"),
SMPCLK("SMPCLK"),
SMPRESZ("SMPRESZ"),
PRESETZ("PRESETZ")
{//{{{
    CommandInit((std::string)name);

    if (mQlapbfictlav1Channel == 0) {
        re_printf("warning","This setting is prohibited. mQlapbfictlav1Channel is set to 8 (default)\n");
        mQlapbfictlav1Channel = 8;
    } else if (mQlapbfictlav1Channel > 8) {
        re_printf("warning","Invalid setting mQlapbfictlav1Channel = %d. mQlapbfictlav1Channel is set to 8 (default)\n", mQlapbfictlav1Channel);
        mQlapbfictlav1Channel = 8;
    }

    //Bind clock, reset signal and socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETZ);
    setTargetSocket32((char*)"ts");

    mQLAPBFICTLAV1_Func = new QLAPBFICTLAV1_Func(std::string(name), this);

    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding
    mBusSlaveIf32[0]->setFuncModulePtr(mQLAPBFICTLAV1_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    std::ostringstream port_name;
    for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {
        port_name.str("");
        port_name << "FCLATSI" << index;
        FCLATSI[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(FCLATSI[index] != NULL);
    }
    for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {
        port_name.str("");
        port_name << "FCLATNFSI" << index;
        FCLATNFSI[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(FCLATNFSI[index] != NULL);
    }
    for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {
        port_name.str("");
        port_name << "FCLATEDSI" << index;
        FCLATEDSI[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(FCLATEDSI[index] != NULL);
    }
    for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {
        port_name.str("");
        port_name << "FCLATINTR" << index;
        FCLATINTR[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(FCLATINTR[index] != NULL);
    }
    for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {
        port_name.str("");
        port_name << "FCLATINTF" << index;
        FCLATINTF[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(FCLATINTF[index] != NULL);
    }
    for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {
        port_name.str("");
        port_name << "FCLATSO" << index;
        FCLATSO[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(FCLATSO[index] != NULL);
    }
    for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {
        port_name.str("");
        port_name << "FCLATEDSO" << index;
        FCLATEDSO[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(FCLATEDSO[index] != NULL);
    }

    std::ostringstream ins_name;
    for (unsigned int i = 0; i < mQlapbfictlav1Channel; i++) {
        ins_name.str("");
        ins_name << "mQLAPBFICTLAV1_Sub" << i;
        mQLAPBFICTLAV1_Sub[i]  = new QLAPBFICTLAV1_Sub(ins_name.str().c_str() , this, i );
        sc_assert(mQLAPBFICTLAV1_Sub[i] != NULL);
    }
    /// Port connections
    for (unsigned int i = 0; i < mQlapbfictlav1Channel; i++) {
        ((mQLAPBFICTLAV1_Sub[i]->FCLATNFSI))(*(FCLATNFSI[i]));
        ((mQLAPBFICTLAV1_Sub[i]->FCLATEDSI))(*(FCLATEDSI[i]));
        ((mQLAPBFICTLAV1_Sub[i]->FCLATSI))(*(FCLATSI[i]));
        ((mQLAPBFICTLAV1_Sub[i]->FCLATINTR))(*(FCLATINTR[i]));
        ((mQLAPBFICTLAV1_Sub[i]->FCLATINTF))(*(FCLATINTF[i]));
        ((mQLAPBFICTLAV1_Sub[i]->FCLATSO))(*(FCLATSO[i]));
        ((mQLAPBFICTLAV1_Sub[i]->FCLATEDSO))(*(FCLATEDSO[i]));
    }
    // Initialize output ports
    for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {
        FCLATINTR[index]->initialize(false);
        FCLATINTF[index]->initialize(false);
        FCLATSO[index]->initialize(false);
        FCLATEDSO[index]->initialize(false);
    }
    //Initialize variables
    mPCLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mfclasmpclkFreq = 0;
    mPCLKFreqUnit = "Hz";
    mfclasmpclkPeriod = 0;
    mfclasmpclkOrgFreq = 0;
    mfclasmpclkFreqUnit = "Hz";
    mEnableRegisterMessage = true;
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  false;
    }

    mTimeCLKChangePoint[0] = 0;
    mTimeCLKChangePoint[1] = 0;

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandlefclasmpclkSignalMethod);
    dont_initialize();
    sensitive << SMPCLK;

    SC_METHOD(HandlePRESETZSignalMethod);
    sensitive << PRESETZ;

    SC_METHOD(HandleSMPRESZSignalMethod);
    sensitive << SMPRESZ;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&QLAPBFICTLAV1::HandleResetHardMethod, this, reset_id),
        sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&QLAPBFICTLAV1::HandleResetCmdMethod, this, reset_id),
        sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&QLAPBFICTLAV1::CancelResetCmdMethod, this, reset_id),
        sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
    for (unsigned int clkzero_id = 0; clkzero_id < emClkZeroNum; clkzero_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mClkZeroEvent[clkzero_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&QLAPBFICTLAV1::HandleZeroCLKMethod, this, clkzero_id),
        sc_core::sc_gen_unique_name("HandleZeroCLKMethod"), &opt);
    }
}//}}}

///Destructor of QLAPBFICTLAV1 class

QLAPBFICTLAV1::~QLAPBFICTLAV1 (void)
{//{{{
    delete mQLAPBFICTLAV1_Func ;

    for (unsigned int i = 0; i < mQlapbfictlav1Channel; i++) {
        delete mQLAPBFICTLAV1_Sub[i];
        delete FCLATEDSO[i];
        delete FCLATSO[i];
        delete FCLATINTF[i];
        delete FCLATINTR[i];
        delete FCLATSI[i];
        delete FCLATNFSI[i];
        delete FCLATEDSI[i];
    }
}//}}}

///Assert reset by software

void QLAPBFICTLAV1::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETZ") {
        if ((!mIsResetHardActive[0])&&(!mIsResetCmdReceive[0])) {
            mIsResetCmdReceive[0] = true;
            re_printf("info","The model will be reset (PRESETZ) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[0].notify(start_time, SC_NS);
            mResetCmdPeriod[0] = period;
        } else {
            re_printf("warning","The software reset of PRESETZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "SMPRESZ") {
        if ((!mIsResetHardActive[1])&&(!mIsResetCmdReceive[1])) {
            mIsResetCmdReceive[1] = true;
            re_printf("info","The model will be reset (SMPRESZ) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[1].notify(start_time, SC_NS);
            mResetCmdPeriod[1] = period;
        } else {
            re_printf("warning","The software reset of SMPRESZ is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETZ or SMPRESZ\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit

void QLAPBFICTLAV1::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mTimeCLKChangePoint[0] = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[0].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "SMPCLK") {
            mfclasmpclkOrgFreq = freq;
            ConvertClockFreq (mfclasmpclkFreq, mfclasmpclkFreqUnit, freq, unit);
            if (mfclasmpclkFreq > 0) {
                mfclasmpclkPeriod = (sc_dt::uint64)(((1/(double)mfclasmpclkFreq)*(double)mTimeResolutionValue) + 0.5);
                mTimeCLKChangePoint[1] = sc_time_stamp().to_double();
            } else {
                mfclasmpclkPeriod = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[1].notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Get clock value

void QLAPBFICTLAV1::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "SMPCLK") {
            re_printf("info","SMPCLK frequency is %0.0f %s\n", (double)mfclasmpclkOrgFreq, mfclasmpclkFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}


///Force value to register

void QLAPBFICTLAV1::ForceRegister (const unsigned int ch_id, const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandler(ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers

void QLAPBFICTLAV1::ReleaseRegister (const unsigned int ch_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandler(ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software

void QLAPBFICTLAV1::WriteRegister (const unsigned int ch_id, const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandler(ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software

void QLAPBFICTLAV1::ReadRegister (const unsigned int ch_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandler(ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/// Wrapper for accessing registers

void QLAPBFICTLAV1::RegisterHandler (const unsigned int ch_id, const std::vector<std::string> &args)
{//{{{
    if (ch_id < mQlapbfictlav1Channel) {
        std::string msg_return = "";
        if (args.size() == 1) {//Dump registers name
            for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {//Dump Reg of all sub
                mQLAPBFICTLAV1_Sub[index]->mQLAPBFICTLAV1_Sub_Func->RegisterHandler(args);
            }
        } else {
            if (args[1] == "*") {//Set message level for each register
                for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {//Set Message level to all sub
                    mQLAPBFICTLAV1_Sub[index]->mQLAPBFICTLAV1_Sub_Func->RegisterHandler(args);
                }
            } else if  (args[1] == "DumpRegisterRW") {
                for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {//Set Message level to all sub
                    mQLAPBFICTLAV1_Sub[index]->mQLAPBFICTLAV1_Sub_Func->RegisterHandler(args);
                }
            } else {
                mQLAPBFICTLAV1_Sub[ch_id]->mQLAPBFICTLAV1_Sub_Func->RegisterHandler(args);
            }
        }
    } else {
        re_printf("warning","channel id (%d) is invalid\n", ch_id);
    }
}//}}}

///List all registers name

void QLAPBFICTLAV1::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    RegisterHandler(0,cmd);
}//}}}

///Dump help message of all parameters or commands

void QLAPBFICTLAV1::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_MessageLevel (fatal|error|warning|info)                       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_DumpRegisterRW (true/false)                                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(), "QLAPBFICTLAV1_EnableRegisterMessage (true/false) Enable/disable dumping register information (Default: false)");//

        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_AssertReset (reset_name, start_time, period)                  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_SetCLKFreq (clock_name, freq, unit)                           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_GetCLKFreq (clock_name)                                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_ForceRegister (ch_id, reg_name, reg_value)                    Force QLAPBFICTLAV1 of register with setting value.");
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_ReleaseRegister (ch_id, reg_name)                             Release QLAPBFICTLAV1 of register from force value.");
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_WriteRegister (ch_id, reg_name, reg_value)                    Write a value to a QLAPBFICTLAV1 of register.");
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_ReadRegister (ch_id, reg_name)                                Read a value from a QLAPBFICTLAV1 of register.");
            SC_REPORT_INFO(this->basename(),"QLAPBFICTLAV1_ListRegister ()                                               Dump name of model's registers.");
        } else {
            re_printf("warning","The name (%s) of QLAPBFICTLAV1_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of QLAPBFICTLAV1_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)

void QLAPBFICTLAV1::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    if (mEnableRegisterMessage)
    {
    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    RegisterHandler(0,cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    RegisterHandler(0,cmd);

    }
    else
    {
    //Disable message level for Register IF class (except fatal level)
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    cmd.push_back("fatal");
    RegisterHandler(0,cmd);

    //Disable message level for each register in Register IF class (except fatal level)
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    cmd.push_back("fatal");
    RegisterHandler(0,cmd);      
    }
}//}}}

///Enable/disable dumping message when users access registers

void QLAPBFICTLAV1::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    RegisterHandler(0,cmd);
}//}}}

/// Enable/disable dumping info/warning/error message of register IF
void QLAPBFICTLAV1::EnableRegisterMessage(const std::string is_enable)//
{ //{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1)
    {
        if (word_vector[0] == "true")
        {
            mEnableRegisterMessage = true;
        }
        else if (word_vector[0] == "false")
        {
            mEnableRegisterMessage = false;
        }
        else
        {
            re_printf("warning", "Invalid argument: QLAPBFICTLAV1_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }
    else if (word_vector.size() == 0)
    {
        std::string temp = "false";
        if (mEnableRegisterMessage) {
            temp = "true";
        }
        re_printf("info", "QLAPBFICTLAV1_EnableRegisterMessage %s\n", temp.c_str());
    }
    else
    {
        re_printf("warning", "Invalid argument: QLAPBFICTLAV1_EnableRegisterMessage %s\n", is_enable.c_str());
    }
} //}}}


///Check reset status

bool QLAPBFICTLAV1::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if(reset_name=="SMPRESZ") {
        if (mIsResetHardActive[1] || mIsResetCmdActive[1]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///Check clock period value

bool QLAPBFICTLAV1::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;

    if(clock_name == "PCLK"){
        clock_period = mPCLKPeriod;
    }else{
        clock_period = mfclasmpclkPeriod;
    }

    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}



///Handle PCLK signal

void QLAPBFICTLAV1::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle fclasmpclk signal

void QLAPBFICTLAV1::HandlefclasmpclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = SMPCLK.read();
    SetCLKFreq("SMPCLK", freq_value, "Hz");
}//}}}


///Handle PRESETZ signal

void QLAPBFICTLAV1::HandlePRESETZSignalMethod (void)
{//{{{
    mResetCurVal[0] = PRESETZ.read();
    if (mResetCurVal[0] != mResetPreVal[0]) {       //mResetPreVal is "true" at initial state
        mResetPreVal[0] = mResetCurVal[0];
        // Reset signals are asynchronous
        mResetHardEvent[0].notify(SC_ZERO_TIME);
    }
}//}}}

///Handle SMPRESZ signal

void QLAPBFICTLAV1::HandleSMPRESZSignalMethod (void)
{//{{{
    mResetCurVal[1] = SMPRESZ.read();
    if (mResetCurVal[1] != mResetPreVal[1]) {   //mResetPreVal is "true" at initial state
        mResetPreVal[1] = mResetCurVal[1];
        // Reset signals are asynchronous
        mResetHardEvent[1].notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when reset port is active

void QLAPBFICTLAV1::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset port PRESETZ is asserted.\n");
            this->EnableReset("PRESETZ", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port SMPRESZ is asserted.\n");
            this->EnableReset("SMPRESZ", mIsResetHardActive[reset_id]);
        }
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            if (reset_id == 0) {
                re_printf("info","The reset port PRESETZ is de-asserted.\n");
                this->EnableReset("PRESETZ", mIsResetHardActive[reset_id]);
            } else {
                re_printf("info","The reset port SMPRESZ is de-asserted.\n");
                this->EnableReset("SMPRESZ", mIsResetHardActive[reset_id]);
            }
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

// Handle Zero SMPCLK clock

void QLAPBFICTLAV1::HandleZeroCLKMethod (const unsigned int clkzero_id)
{//{{{
    if(clkzero_id == 1){
         for (unsigned int i = 0; i < mQlapbfictlav1Channel; i++) {
             mQLAPBFICTLAV1_Sub[i]->CancelEventsZeroClock ();
         }
    }
}

///Process reset function when reset command is active

void QLAPBFICTLAV1::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETZ";
    if (reset_id == 0) {
        reset_name = "PRESETZ";
    } else { //reset_id == 1
        reset_name = "SMPRESZ";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when reset command is active

void QLAPBFICTLAV1::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETZ";
    if (reset_id == 0) {
        reset_name = "PRESETZ";
    } else { //reset_id == 1
        reset_name = "SMPRESZ";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

///Execute reset operation

void QLAPBFICTLAV1::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if((reset_name=="SMPRESZ") && is_active) {
        // Cancel events
        CancelEvents();
    }
    for (unsigned int i = 0; i < mQlapbfictlav1Channel; i++) {
        mQLAPBFICTLAV1_Sub[i]->EnableReset (reset_name, is_active);
    }
}//}}}

// Get Calculate synchronous
sc_time QLAPBFICTLAV1::CalculateClockEdge (const std::string clock_name){
    double current_time = sc_time_stamp().to_double();

    double SyncTPoint = 0;
    double Period = 0;
    if (clock_name == "PCLK") {
        Period = (double)mPCLKPeriod;
        SyncTPoint = mTimeCLKChangePoint[0];
    } else{
        Period = (double)mfclasmpclkPeriod;
        SyncTPoint = mTimeCLKChangePoint[1];
    }
    return sc_time(CalculateCLKPosEdge(Period, false, current_time, SyncTPoint),mTimeResolutionUnit);

}

/// Cancel operation events

void QLAPBFICTLAV1::CancelEvents (void)
{//{{{
    for (unsigned int index = 0; index < mQlapbfictlav1Channel; index++) {//Set Message level to all sub
        mQLAPBFICTLAV1_Sub[index]->CancelEvents();
    }
}//}}}

// Get channel ID

unsigned int QLAPBFICTLAV1::GetChId(void)
{//{{{
        return mQlapbfictlav1Channel;
}//}}}

// vim600: set foldmethod=marker :

