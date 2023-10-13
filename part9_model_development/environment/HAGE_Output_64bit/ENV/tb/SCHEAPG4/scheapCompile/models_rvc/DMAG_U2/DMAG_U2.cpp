// ---------------------------------------------------------------------
// $Id: DMAG_U2.cpp,v 1.8 2020/02/19 09:09:44 hadinhtran Exp $
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


#include "DMAG_U2.h"
#include "DMAG_U2_Common_Guard.h"
#include "DMAG_U2_Func.h"

///Constructor of DMAG_U2 class
DMAG_U2::DMAG_U2 (sc_module_name name,
  unsigned int iCHAN_NUM,
  unsigned int rLatency,
  unsigned int wLatency,
  SIM_MODE_T simmode):
    DMAG_U2_AgentController(),
    BusBridgeModuleBase<32,32>(name),
    BusBridgeSlaveBase<32,1>(),
    BusMasterBase<32,1>(),
    BusSlaveBase<32,1>(),
    rvc_common_model(),
    PCLK("PCLK"),
    dmaguard_error("dmaguard_error")
{//{{{
    mChannelNum = iCHAN_NUM;

    if (mChannelNum == 0 || mChannelNum > emNUM_MAX_ChanNum) {
        re_printf("warning", "Number of channels is wrong, it is automatically set to the default value 18.\n");
        mChannelNum = emNUM_MAX_ChanNum;
    }
    mPCLKPeriod   = 0;
    mPCLKFreq     = 0;
    mPCLKOrgFreq  = 0;
    mPCLKFreqUnit = "Hz";

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mResetCurVal            = true;
    mResetPreVal            = true;
    mIsResetHardActive      = false;
    mIsResetCmdReceive      = false;
    mIsResetCmdActive       = false;
    mResetCmdPeriod         = 0;
    mTimeCLKChangePoint     = 0;

    mDumpInterrupt          = false;
    mEnableRegisterMessage  = true;

    mDMAGUARD_ERROR_value   = false;
    // Initialize internal variables
    dmaguard_error.initialize(false);

    //Initialize Bus
    setMasterResetPort32(&resetPort);
    setMasterFreqPort32(&PCLK);
    setInitiatorSocket32((char*)"isp");

    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&resetPort);
    setTargetSocket32((char*)"ts");

    setSlaveResetPort(&resetPort);
    setSlaveFreqPort(&PCLK);
    setTargetSocket((char*)"tsp");

    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding
    tsp = BusBridgeSlaveBase<32,1>::tSocket[0]; //Alias for binding
    isp  = iSocket32[0]; //Alias for binding

    //Instance DMAG_U2_Func
    mDMAG_U2_Func = new DMAG_U2_Func((std::string)name, this, mChannelNum);
    sc_assert(mDMAG_U2_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mDMAG_U2_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    //Instance DMAG_U2_Common_Guard
    mDMAG_U2_Common_Guard = new DMAG_U2_Common_Guard((std::string)name, this, this, mBusSlaveIf[0], mBusMasterIf32[0], simmode);
    BusBridgeModuleBase<32,32>::mpCore = mDMAG_U2_Common_Guard;
    mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)mDMAG_U2_Common_Guard);
    mBusMasterIf32[0]->setBusProtocol(BUS_APB);
    mBusMasterIf32[0]->setTransNmbLmt(0x1);
    mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mDMAG_U2_Common_Guard);
    mBusSlaveIf[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleResetPortSignalMethod);
    sensitive << resetPort;

    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(Assert_DMAGUARD_ERROR_Method);
    dont_initialize();
    sensitive<<mAssert_DMAGUARD_ERROR_Event;

    SC_METHOD(Deassert_DMAGUARD_ERROR_Method);
    dont_initialize();
    sensitive<<mDeassert_DMAGUARD_ERROR_Event;

    SC_METHOD(HandleErrorMethod);
    dont_initialize();
    sensitive<<mTransErrorEvent;

    SC_METHOD(Write_DMAGUARD_ERROR_Method);
    dont_initialize();
    sensitive<<mWrite_DMAGUARD_ERROR_Event;
}//}}}

///Destructor of DMAG_U2 class
DMAG_U2::~DMAG_U2 (void)
{//{{{
     delete mDMAG_U2_Func;
     delete mDMAG_U2_Common_Guard;
}//}}}

///Handle output port when dmaguard_error port is asserted
void DMAG_U2::Assert_DMAGUARD_ERROR_Method(void) {

    mDMAGUARD_ERROR_value = true;
    mWrite_DMAGUARD_ERROR_Event.notify(SC_ZERO_TIME);
    double current_time = sc_time_stamp().to_double();
    mDeassert_DMAGUARD_ERROR_Event.notify(CalculateCLKPosEdge((double) mPCLKPeriod, true, current_time, mTimeCLKChangePoint), mTimeResolutionUnit);
}

///Handle output port when dmaguard_error port is deasserted
void DMAG_U2::Deassert_DMAGUARD_ERROR_Method(void) {

    mDMAGUARD_ERROR_value = false;
    mWrite_DMAGUARD_ERROR_Event.notify(SC_ZERO_TIME);
}

///Write current value of dmaguard_error port to output port
void DMAG_U2::Write_DMAGUARD_ERROR_Method(void) {

    bool pre_value = dmaguard_error.read();
    if(pre_value != mDMAGUARD_ERROR_value) {
        DumpInterruptMsg("dmaguard_error", mDMAGUARD_ERROR_value);
        dmaguard_error.write(mDMAGUARD_ERROR_value);
    }
}

///Set address of guard areas
void DMAG_U2::SetAreaAddress(const unsigned int ch_id, const unsigned int start_addr, const unsigned int size)
{//{{{
    re_printf("info","DMAG_U2 ch_id: %d, start_addr: %x, size: %x\n", ch_id, start_addr, size);
    if (ch_id >= mChannelNum) {
        re_printf("error","channel number %d is invalid", ch_id);
     }
    else
    {
        mDMAG_U2_Common_Guard->SetAreaAddr(ch_id, start_addr, size);
    }
}//}}}

///Get permitted SPID from channel setting registers
unsigned int DMAG_U2::getPermittedSPID(unsigned char channel_id)
{
    return mDMAG_U2_Func->GetPermittedSPID(channel_id);
}

///Get protection setting info from channel setting registers
void DMAG_U2::getDMAGPROT_n(unsigned char ch_id, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg)
{//{{{
    mDMAG_U2_Func->GetDMAGPROT_n(ch_id, gen, dbg, um, wg, rg);
}//}}}

/// Handle output data and signal
void DMAG_U2::HandleErrorMethod(void)
{//{{{
    bool isNotifyDMAGUAR_ERROR = false;
    if (mDMAG_U2_Func->IsTheFirstERR()) {
        mDMAG_U2_Func->SetVar("ADDR", mDMAG_U2_Common_Guard->GetVAR("ADDR"));
        mDMAG_U2_Func->SetVar("SEC", mDMAG_U2_Common_Guard->GetVAR("SEC"));
        mDMAG_U2_Func->SetVar("DBG", mDMAG_U2_Common_Guard->GetVAR("DBG"));
        mDMAG_U2_Func->SetVar("UM", mDMAG_U2_Common_Guard->GetVAR("UM"));
        mDMAG_U2_Func->SetVar("SPID", mDMAG_U2_Common_Guard->GetVAR("SPID"));
        mDMAG_U2_Func->SetVar("WRITE", mDMAG_U2_Common_Guard->GetVAR("WRITE"));
        isNotifyDMAGUAR_ERROR = true;
    }
    else {
        mDMAG_U2_Func->SetOverFlowBit();
        if (!(mDMAG_U2_Func->IsTheSameSPID(mDMAG_U2_Common_Guard->GetVAR("SPID")))) {
            isNotifyDMAGUAR_ERROR = true;
        }
    }

    if (isNotifyDMAGUAR_ERROR) {
        mDMAG_U2_Func->SetVar("SPIDERR", mDMAG_U2_Common_Guard->GetVAR("SPID"));
        mAssert_DMAGUARD_ERROR_Event.notify(SC_ZERO_TIME);
    }
}//}}}

/// Notify guard error has occurred
void DMAG_U2::NotifyTransErrorEvent(void)
{
    mTransErrorEvent.notify(SC_ZERO_TIME);
}

/// Get Reset Status
bool DMAG_U2::GetResetStatus (void)
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

///Check clock period value
bool DMAG_U2::CheckClockPeriod (void)
{//{{{
    bool clock_status = false;
    if (mPCLKFreq > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","PCLK frequency is zero\n");
    }
    return clock_status;
}//}}}

void DMAG_U2::PCLKUpdate (void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mTimeCLKChangePoint = current_time;
}//}}}

/// Cancel operation events
void DMAG_U2::CancelEvents (void)
{//{{{
    mAssert_DMAGUARD_ERROR_Event.cancel();
    mDeassert_DMAGUARD_ERROR_Event.cancel();
    mWrite_DMAGUARD_ERROR_Event.cancel();
    mTransErrorEvent.cancel();
}//}}}

/// Wrapper for accessing registers
void DMAG_U2::RegisterHandler (const std::vector<std::string> cmd)
{//{{{
    mDMAG_U2_Func->RegisterHandler(cmd);

}//}}}

///Execute reset operation
void DMAG_U2::EnableReset (const bool is_active)
{//{{{
    mDMAG_U2_Func->EnableReset(is_active);
    mDMAG_U2_Common_Guard->EnableReset(is_active);
    if (is_active) {
        CancelEvents();
        mDMAGUARD_ERROR_value = false;
        mWrite_DMAGUARD_ERROR_Event.notify(SC_ZERO_TIME);
}
}//}}}

///Process reset function when reset port is active
void DMAG_U2::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset port resetPort is asserted.\n");
        this->EnableReset(mIsResetHardActive);
    } else {
        mIsResetHardActive = false;
        re_printf("info","The reset port resetPort is de-asserted.\n");
        this->EnableReset(mIsResetHardActive);
        
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

///Process reset function when reset command is active
void DMAG_U2::HandleResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of resetPort.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when reset command is active
void DMAG_U2::CancelResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of resetPort is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

///Handle resetPort signal
void DMAG_U2::HandleResetPortSignalMethod (void)
{//{{{
    mResetCurVal = resetPort.read();
    if (mResetCurVal != mResetPreVal) {   //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Handle PCLK signal
void DMAG_U2::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

//========================================================
//============         PYTHON IF       ===================
//========================================================
/// Set initial value of DMAGMPIDm registers
void DMAG_U2::DMAG_U2_P_RESET_VALUE0_m(const unsigned int value, const unsigned int index)
{//{{{
    mDMAG_U2_Func->DMAG_U2_P_RESET_VALUE0_m(value, index);
}//}}}

/// Set initial value of DMAGPROT_n registers
void DMAG_U2::DMAG_U2_P_RESET_VALUE1_n(const unsigned int value, const unsigned int index)
{//{{{
    mDMAG_U2_Func->DMAG_U2_P_RESET_VALUE1_n(value, index);
}//}}}

///Enable/disable dumping interrupt information
void DMAG_U2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: DMAG_U2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","DMAG_U2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: DMAG_U2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Enable/disable dumping message when users access registers
void DMAG_U2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mDMAG_U2_Func->RegisterHandler(cmd);
}//}}}

///Enable register message
void DMAG_U2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }
        else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        }
        else {
            re_printf("warning", "Invalid argument: DMAG_U2_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } 
    else if (word_vector.size() == 0){
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info", "DMAG_U2_EnableRegisterMessage %s\n", temp.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: DMAG_U2_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void DMAG_U2::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    if (mEnableRegisterMessage) {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        mDMAG_U2_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        mDMAG_U2_Func->RegisterHandler(cmd);
    }
    else {
        // Disable message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mDMAG_U2_Func->RegisterHandler(cmd);

        // Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mDMAG_U2_Func->RegisterHandler(cmd);
    }
}//}}}

///Dump help message of all parameters or commands
void DMAG_U2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"DMAG_U2_MessageLevel (fatal|error|warning|info)                       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_DumpRegisterRW (true/false)                                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_DumpInterrupt (true/false)                                    Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_EnableRegisterMessage (true/false)                            Enable/disable dumping info/warning/error message of register IF.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_P_RESET_VALUE0_m (value, index)                               Set initial value of DMAGMPIDm registers.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_P_RESET_VALUE1_n (value, index)                               Set initial value of DMAGPROT_n registers.");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"DMAG_U2_AssertReset (reset_name, start_time, period)                  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_SetCLKFreq (clock_name, freq, unit)                           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_GetCLKFreq (clock_name)                                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_ForceRegister (reg_name, reg_value)                           Force a register with setting value.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_ReleaseRegister (reg_name)                                    Release a register from force value.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_WriteRegister (reg_name, reg_value)                           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_ReadRegister (reg_name)                                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_ListRegister ()                                               Dump name of registers in model.");
            SC_REPORT_INFO(this->basename(),"DMAG_U2_SetAreaAddress (channel_id, start_addr, size)                 Set Address of protected area.");
        } else {
            re_printf("warning","The name (%s) of DMAG_U2_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of DMAG_U2_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///List all registers
void DMAG_U2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mDMAG_U2_Func->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void DMAG_U2::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void DMAG_U2::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void DMAG_U2::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Force value to register
void DMAG_U2::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Get clock value
void DMAG_U2::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Set clock value and clock unit
void DMAG_U2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent.notify(SC_ZERO_TIME);
            }
            PCLKUpdate();
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Assert reset by software
void DMAG_U2::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "resetPort") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (resetPort) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of resetPort is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be resetPort\n",reset_name.c_str());
    }
}//}}}

/// Dump interrupt message
void DMAG_U2::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mDumpInterrupt) {
        if (value) {
            re_printf("info","Port %s Assert\n",intr_name.c_str());
        } else {
            re_printf("info","Port %s De-assert\n",intr_name.c_str());
        }
   }
}//}}}

