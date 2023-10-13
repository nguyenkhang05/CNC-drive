// ---------------------------------------------------------------------
// $Id: PBG.cpp,v 1.11 2019/06/16 06:02:16 nhuvo Exp $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------


#include "PBG.h"
#include "PBG_Common_Guard.h"
#include "PBG_Func.h"

///Constructor of PBG class
PBG::PBG (sc_module_name name,
  unsigned int rLatency,
  unsigned int wLatency,
  unsigned int iCHAN_NUM,
  SIM_MODE_T simmode):
    PBG_AgentController(),
    rvc_common_model(),
    BusBridgeModuleBase<32,32>(name),
    BusBridgeSlaveBase<32,1>(),
    BusMasterBase<32,1>(),
    BusSlaveBase<32,1>(),
    PCLK("PCLK"),
    PBG_ERR("PBG_ERR")
    
{//{{{
    mChannelNum = iCHAN_NUM;
    
    if(mChannelNum==0 || mChannelNum > 16) {
        re_printf("warning","PBG's id is wrong, it is set to the default value 6 automatically");
        mChannelNum = 6;
    }
    re_printf("info", "Number of channel is: %d .\n", mChannelNum);
    mPCLKPeriod   = 0;
    mPCLKFreq     = 0;
    mPCLKOrgFreq  = 0;
    mPCLKFreqUnit = "Hz";

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    mLock = false;
    mSec  = true;

    mResetCurVal        = true;
    mResetPreVal        = true;
    mIsResetHardActive  = false;
    mIsResetCmdReceive  = false;
    mIsResetCmdActive   = false;
    mResetCmdPeriod     = 0;
    mTimeCLKChangePoint = 0;

    mDumpInterrupt      = false;
    mEnableRegisterMessage = true;
    mPBG_ERR_value = false;

    // Initialize internal variables
    PBG_ERR.initialize(false);

    //Initialize Bus
    BusMasterBase<BUS_WIDTH_APB, 1>::setMasterResetPort32(&resetPort);
    BusMasterBase<BUS_WIDTH_APB, 1>::setMasterFreqPort32(&PCLK);
    BusMasterBase<BUS_WIDTH_APB, 1>::setInitiatorSocket32("is");

    BusSlaveBase<BUS_WIDTH_APB, 1>::setSlaveFreqPort32(&PCLK);
    BusSlaveBase<BUS_WIDTH_APB, 1>::setSlaveResetPort32(&resetPort);
    BusSlaveBase<BUS_WIDTH_APB, 1>::setTargetSocket32("tsp");

    BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::setSlaveResetPort(&resetPort);
    BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::setSlaveFreqPort(&PCLK);
    BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::setTargetSocket("tsv");

    tsp = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding
    tsv = BusBridgeSlaveBase<32,1>::tSocket[0]; //Alias for binding
    is  = iSocket32[0]; //Alias for binding

    //Instance PBG_Func
    mPBG_Func = new PBG_Func((std::string)name, this, mChannelNum);
    sc_assert(mPBG_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mPBG_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    //Instance PBG_Common_Guard
    mPBG_DataHandler = new PBG_Common_Guard((std::string)name, this, mBusSlaveIf[0], mBusMasterIf32[0], simmode, mChannelNum);
    mPBG_DataHandler->SetSecOperation(mSec);
    sc_assert(mPBG_DataHandler != NULL);
    BusBridgeModuleBase<32,32>::mpCore = mPBG_DataHandler;
    mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)mPBG_DataHandler);
    mBusMasterIf32[0]->setBusProtocol(BUS_APB);
    mBusMasterIf32[0]->setTransNmbLmt(0x1);
    mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mPBG_DataHandler);
    mBusSlaveIf[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleResetSignalMethod);
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

    SC_METHOD(AssertPBG_ERRMethod);
    dont_initialize();
    sensitive<<mAssertPBG_ERREvent;

    SC_METHOD(DeassertPBG_ERRMethod);
    dont_initialize();
    sensitive<<mDeassertPBG_ERREvent;

    SC_METHOD(HandleErrorMethod);
    dont_initialize();
    sensitive<<mPBG_DataHandler->mTransErrorEvent;

    SC_METHOD(WritePBG_ERRMethod);
    dont_initialize();
    sensitive<<mWritePBG_ERREvent;

}//}}}

///Destructor of PBG class
PBG::~PBG (void)
{//{{{
    delete mPBG_Func;
    delete mPBG_DataHandler;
}//}}}

void PBG::SetPBGPROT1(const unsigned int spid, const unsigned int ch_id) 
{//{{{
    mPBG_DataHandler->SetVAR("SPID",spid, ch_id);
}//}}}

void PBG::SetPBGPROT0(const unsigned int gen, const unsigned int dbg, const unsigned int um, const unsigned int wg, const unsigned int rg, const unsigned int sec, const unsigned int ch_id) 
{//{{{
    mPBG_DataHandler->SetVAR("GEN", gen, ch_id);
    mPBG_DataHandler->SetVAR("DBG", dbg, ch_id);
    mPBG_DataHandler->SetVAR("UM", um, ch_id);
    mPBG_DataHandler->SetVAR("WG", wg, ch_id);
    mPBG_DataHandler->SetVAR("RG", rg, ch_id);
    mPBG_DataHandler->SetVAR("SEC", sec, ch_id);
}//}}}

void PBG::AssertPBG_ERRMethod(void) 
{//{{{
    bool pre_value = PBG_ERR.read();

    mPBG_ERR_value = true;
    mWritePBG_ERREvent.notify(SC_ZERO_TIME);
    DumpInterruptMsg("PBG_ERR",true);
    double current_time = sc_time_stamp().to_double();
    mDeassertPBG_ERREvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, current_time, mTimeCLKChangePoint), mTimeResolutionUnit);
}//}}}

void PBG::DeassertPBG_ERRMethod(void) 
{//{{{
    bool pre_value = PBG_ERR.read();
    mPBG_ERR_value = false;
    mWritePBG_ERREvent.notify(SC_ZERO_TIME);
    DumpInterruptMsg("PBG_ERR",false);
}//}}}

void PBG::WritePBG_ERRMethod(void) 
{//{{{
    PBG_ERR.write(mPBG_ERR_value);
}//}}}

//Set address of guard areas
void PBG::SetAreaAddress(const std::string group_id, const unsigned int start_addr, const unsigned int size, const unsigned int ch_id)
{//{{{
    if (ch_id >= mChannelNum) {
        re_printf("error","Channel number is invalid");
    } else {
        if (size == 0) {
            re_printf("warning","Invalid setting size = 0x0. This setting is ignore.\n");
        } else {
            re_printf("info","PBG group_id: %s, start_addr: %x, size: %d,ch_id: %d\n",group_id.c_str(),start_addr,size,ch_id);
            mPBG_DataHandler->SetAreaAddr(group_id, start_addr, size, ch_id);
        }
    }
}//}}}

///Check access permission tp peripherals
void PBG::HandleErrorMethod(void)
{//{{{
    //Get value of ERR bit
    uint8_t ERR_bit = this->GetPBGERRSTAT() & 0x1;
    if (ERR_bit == 0x0) {
        double current_time = sc_time_stamp().to_double();
        mAssertPBG_ERREvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, true, current_time, mTimeCLKChangePoint), mTimeResolutionUnit);
    }
    mPBG_Func->SetVar("ADDR", mPBG_DataHandler->GetVAR("ADDR"));
    mPBG_Func->SetVar("DBG", mPBG_DataHandler->GetVAR("DBG"));
    mPBG_Func->SetVar("UM", mPBG_DataHandler->GetVAR("UM"));
    mPBG_Func->SetVar("SPID", mPBG_DataHandler->GetVAR("SPID"));
    mPBG_Func->SetVar("WRITE", mPBG_DataHandler->GetVAR("WRITE"));
    mPBG_Func->SetVar("SEC", mPBG_DataHandler->GetVAR("SEC"));
    mPBG_Func->SetVar("ERR", 1);
}//}}}

///Check clock period value
bool PBG::CheckClockPeriod (const std::string clock_name)
{//{{{
    return (mPCLKPeriod > 0);
}//}}}

/// Retrun status of model
bool PBG::GetResetStatus()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Update clock value to other modules
void PBG::PCLKUpdate (void)
{//{{{
    mPBG_DataHandler->SetCLKPeriod(mPCLKPeriod);
    double current_time = sc_time_stamp().to_double();
    mTimeCLKChangePoint = current_time;
}//}}}

/// Cancel operation events
void PBG::CancelEvents (void)
{//{{{
    mAssertPBG_ERREvent.cancel();
    mDeassertPBG_ERREvent.cancel();
    mWritePBG_ERREvent.cancel();
}//}}}


/// Wrapper for accessing registers
void PBG::RegisterHandler (const std::vector<std::string> cmd)
{//{{{
    mPBG_Func->RegisterHandler(cmd);

}//}}}

///Execute reset operation
void PBG::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mPBG_Func->Reset(is_active);
    mPBG_DataHandler->EnableReset(is_active);
    if (is_active) {
        CancelEvents();
        mPBG_ERR_value = false;
        mWritePBG_ERREvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when reset port is active
void PBG::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset port resetPort is asserted.\n");
        this->EnableReset("resetPort", mIsResetHardActive);
    } else {
        mIsResetHardActive = false;
        re_printf("info","The reset port resetPort is de-asserted.\n");
        this->EnableReset("resetPort", mIsResetHardActive);
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
void PBG::HandleResetCmdMethod (void)
{//{{{
    std::string reset_name = "resetPort";
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when reset command is active
void PBG::CancelResetCmdMethod (void)
{//{{{
    std::string reset_name = "resetPort";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
}//}}}

///Handle resetPort signal
void PBG::HandleResetSignalMethod (void)
{//{{{
    mResetCurVal = resetPort.read();
    if (mResetCurVal != mResetPreVal) {   //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Handle PCLK signal
void PBG::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Enable/disable dumping interrupt information
void PBG::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: PBG_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
            std::string intr_name = "false";
            if (mDumpInterrupt) {
                intr_name = "true";
            } 
            re_printf("info","PBG_DumpInterrupt %s\n", intr_name.c_str());
    } else {
        re_printf("warning","Invalid argument: PBG_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Enable/disable dumping message when users access registers
void PBG::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mPBG_Func->RegisterHandler(cmd);
}//}}}

///Set message level (fatal, error, warning, info)
void PBG::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    if (true == mEnableRegisterMessage) {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mPBG_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mPBG_Func->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mPBG_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mPBG_Func->RegisterHandler(cmd);
    }
}//}}}

///Dump help message of all parameters or commands
void PBG::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"PBG_MessageLevel (fatal|error|warning|info)                       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"PBG_DumpRegisterRW (true/false)                                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(),"PBG_DumpInterrupt (true/false)                                    Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(),"PBG_EnableRegisterMessage (\"PBG instance\", \"true/false\")          Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"PBG_AssertReset (reset_name, start_time, period)                  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"PBG_SetCLKFreq (clock_name, freq, unit)                           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"PBG_GetCLKFreq (clock_name)                                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"PBG_ForceRegister (reg_name, reg_value)                           Force PBG register with setting value.");
            SC_REPORT_INFO(this->basename(),"PBG_ReleaseRegister (reg_name)                                    Release PBG register from force value.");
            SC_REPORT_INFO(this->basename(),"PBG_WriteRegister (reg_name, reg_value)                           Write a value to a PBG register.");
            SC_REPORT_INFO(this->basename(),"PBG_ReadRegister (reg_name)                                       Read a value from a PBG register.");
            SC_REPORT_INFO(this->basename(),"PBG_ListRegister ()                                               Dump name of model registers.");
            SC_REPORT_INFO(this->basename(),"PBG_SetAreaAddress (group_name, start_addr, size, channel_id)     Set Address of protected area.");
            SC_REPORT_INFO(this->basename(),"PBG_DumpStatusInfo ()                                             Dump information of the status registers of the PBG model");
            SC_REPORT_INFO(this->basename(),"PBG_LOCK_SUPPORT (value)                                          Set value for mLock which is related to lock bit");
            SC_REPORT_INFO(this->basename(),"PBG_P_SEC_DIS (value)                                             Set value of mSec which is related to sec bit");
        } else {
            re_printf("warning","The name (%s) of PBG_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of PBG_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

void PBG::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mPBG_Func->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void PBG::ReadRegister (const std::string reg_name)
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
void PBG::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
void PBG::ReleaseRegister (const std::string reg_name)
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
void PBG::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
void PBG::GetCLKFreq (const std::string clock_name)
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
void PBG::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
                re_printf("info","%s frequency is zero\n", clock_name.c_str());
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
void PBG::AssertReset (const std::string reset_name, const double start_time, const double period)
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
void PBG::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mDumpInterrupt) {
        if (value) {
            re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
        } else {
            re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
        }
   }
}//}}}

void PBG::DumpStatusInfo()
{//{{{
    std::string type = "StatInfo";
    printf("PROFILE(%s): PBG: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): PBG:   PBGERRSTAT bits: 0x%08X\n", type.c_str(), this->GetPBGERRSTAT());
}//}}}

uint32_t PBG::GetPBGERRSTAT()
{//{{{
    return mPBG_Func->GetPBGERRSTAT();
}//}}}

bool PBG::GetLOCK_SUPPORT()
{//{{{
   return mLock;
}//}}}

void PBG::LOCK_SUPPORT(unsigned int value)
{//{{{
  if (value <= 1){
        mLock = (bool)(value&1);
  }else{
        mLock = false;
        re_printf("warning", "Invalid value (%d) of the PBG_LOCK_SUPPORT parameter. Valid range is 0-1. Value 0 is set as default.\n", value);
  }
}//}}}

bool PBG::GetSEC_DISABLED()
{//{{{
   return mSec;
}//}}}

void PBG::SEC_SUPPORT(unsigned int value)
{//{{{
  if (value <= 1) {
      mSec = (bool)(value&1);
  } else {
      mSec = true;
      re_printf("warning", "Invalid value (%d) of the PBG_P_SEC_DIS parameter. Valid range is 0-1. Value 1 is set as default.\n", value);
  }
  mPBG_DataHandler->SetSecOperation(mSec);
}//}}}

void PBG::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: PBG_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
            std::string intr_name = "false";
            if (mEnableRegisterMessage) {
                intr_name = "true";
            } 
            re_printf("info","PBG_EnableRegisterMessage %s\n", intr_name.c_str());
    } else {
        re_printf("warning","Invalid argument: PBG_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
