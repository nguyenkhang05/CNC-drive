// ---------------------------------------------------------------------
// $Id: HI20FSGD2.cpp,v 1.8 2019/12/06 07:19:03 hadinhtran Exp $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "re_define.h"
#include "HI20FSGD2.h"
#include "HI20FSGD2_Common_Guard.h"
#include "HI20FSGD2_Func.h"

///Constructor of HI20FSGD2 class
HI20FSGD2::HI20FSGD2 (sc_module_name name,
    unsigned int rLatency,
    unsigned int wLatency,
    unsigned int iCHAN_NUM,
    SIM_MODE_T simmode):
    HI20FSGD2_AgentController(),
    rvc_common_model(),
    BusBridgeModuleBase<32,32>(name),
    BusBridgeSlaveBase<32,1>(),
    BusMasterBase<32,1>(),
    BusSlaveBase<32,1>(),
    PCLK("PCLK"),
    KCPROT_KCE("KCPROT_KCE"),
    PSLVERR("PSLVERR"),
    mLock(HI20FSGD2_AgentController::DEFAULT_LOCK)
{//{{{
    mChannelNum = iCHAN_NUM;
    
    if(mChannelNum==0 || mChannelNum > 16) {
        re_printf("warning","HI20FSGD2's id is wrong, it is set to the default value 6 automatically");
        mChannelNum = 6;
    }
    re_printf("info", "Number of channel is: %d\n", mChannelNum);
    mPCLKPeriod   = 0;
    mPCLKFreq     = 0;
    mPCLKOrgFreq  = 0;
    mPCLKFreqUnit = "Hz";
    
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    mResetCurVal        = true;
    mResetPreVal        = true;
    mIsResetHardActive  = false;
    mIsResetCmdReceive  = false;
    mIsResetCmdActive   = false;
    mResetCmdPeriod     = 0;
    mTimeCLKChangePoint = 0;
    
    mICUMHA             = false;
    mDumpInterrupt      = false;
    mEnableRegisterMessage = true;
  
    Initialize();
    //Initialize internal variables
    PSLVERR.initialize(mPSLVERR);

    //Initialize Bus
    setMasterResetPort32(&resetPort);
    setMasterFreqPort32(&PCLK);
    setInitiatorSocket32((char*)"is");

    setSlaveResetPort32(&resetPort);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32((char*)"tsp");

    setSlaveResetPort(&resetPort);
    setSlaveFreqPort(&PCLK);
    setTargetSocket((char*)"tsv");

    tsp = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding
    tsv = BusBridgeSlaveBase<32,1>::tSocket[0]; //Alias for binding
    is  = iSocket32[0]; //Alias for binding

    //Instance HI20FSGD2_Func
    mHI20FSGD2_Func = new HI20FSGD2_Func((std::string)name, this, mChannelNum);
    sc_assert(mHI20FSGD2_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mHI20FSGD2_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    //Instance Common_Guard
    mHI20FSGD2_DataHandler = new HI20FSGD2_Common_Guard((std::string)name, this, mBusSlaveIf[0], mBusMasterIf32[0], simmode, mChannelNum);
    sc_assert(mHI20FSGD2_DataHandler != NULL);
    BusBridgeModuleBase<32,32>::mpCore = mHI20FSGD2_DataHandler;
    mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)mHI20FSGD2_DataHandler);
    mBusMasterIf32[0]->setBusProtocol(BUS_APB);
    mBusMasterIf32[0]->setTransNmbLmt(0x1);
    mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mHI20FSGD2_DataHandler);
    mBusSlaveIf[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleresetPortSignalMethod);
    sensitive << resetPort;

    SC_METHOD(HandleKCPROT_KCESignalMethod);
    dont_initialize();
    sensitive << KCPROT_KCE;

    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(AssertPSLVERRMethod);
    dont_initialize();
    sensitive<<mAssertPSLVERREvent;

    SC_METHOD(DeassertPSLVERRMethod);
    dont_initialize();
    sensitive<<mDeassertPSLVERREvent;

    SC_METHOD(HandleErrorMethod);
    dont_initialize();
    sensitive<<mHI20FSGD2_DataHandler->mTransErrorEvent;

}//}}}

///Destructor of HI20FSGD2 class
HI20FSGD2::~HI20FSGD2 (void)
{//{{{
     delete mHI20FSGD2_Func;
     delete mHI20FSGD2_DataHandler;
}//}}}

///Set value to PBGPROT1 register
void HI20FSGD2::SetPBGPROT1(const unsigned int spid, const unsigned int ch_id) {
    mHI20FSGD2_DataHandler->SetVAR("SPID",spid, ch_id);
}

///Set value to PBGPROT0 register
void HI20FSGD2::SetPBGPROT0(const unsigned int sec, const unsigned int gen, const unsigned int dbg, const unsigned int um, const unsigned int wg, const unsigned int rg, const unsigned int ch_id) {
    mHI20FSGD2_DataHandler->SetVAR("SEC", sec, ch_id);
    mHI20FSGD2_DataHandler->SetVAR("GEN", gen, ch_id);
    mHI20FSGD2_DataHandler->SetVAR("DBG", dbg, ch_id);
    mHI20FSGD2_DataHandler->SetVAR("UM", um, ch_id);
    mHI20FSGD2_DataHandler->SetVAR("WG", wg, ch_id);
    mHI20FSGD2_DataHandler->SetVAR("RG", rg, ch_id);
}

///Initialize function
void HI20FSGD2::Initialize(void)
{//{{{
    mKCPROT_KCE         = false;
    mPSLVERR_asserted   = false;
    mPSLVERR.initialize();
}//}}}

///Handle output port when PSLVERR port is asserted
void HI20FSGD2::AssertPSLVERRMethod(void) {
    PSLVERR.write(mPSLVERR);
    DumpInterruptMsg("PSLVERR", mPSLVERR_asserted);
    /// Negate after one clock period
    double current_time = sc_time_stamp().to_double();
    if(mPSLVERR_asserted) {
        mDeassertPSLVERREvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimeCLKChangePoint) + (double)mPCLKPeriod, mTimeResolutionUnit);
    }
}

///Handle output port when PSLVERR port is de-asserted
void HI20FSGD2::DeassertPSLVERRMethod(void) {
    mPSLVERR_asserted = false;
    mPSLVERR.initialize();
    mAssertPSLVERREvent.notify(SC_ZERO_TIME);
}

///Check access permission to peripherals
void HI20FSGD2::HandleErrorMethod(void)
{//{{{
    bool            mPBGRdERR;
    bool            mPBGWrERR    = mHI20FSGD2_DataHandler->GetVAR("WRITE");
    bool            mUM          = mHI20FSGD2_DataHandler->GetVAR("UM");
    bool            mDBG         = mHI20FSGD2_DataHandler->GetVAR("DBG");
    bool            mSecure      = mHI20FSGD2_DataHandler->GetVAR("SEC");
    unsigned int    mSPID        = mHI20FSGD2_DataHandler->GetVAR("SPID");
    unsigned int    mErrAddr     = mHI20FSGD2_DataHandler->GetVAR("ADDR");
    
    if (mPBGWrERR == true){
        mPBGRdERR = false;
    }
    else{
        mPBGRdERR = true;
    }
    
    mPSLVERR.PBGRdERR = mPBGRdERR;
    mPSLVERR.PBGWrERR = mPBGWrERR;
    mPSLVERR.SPID     = mSPID    ;
    mPSLVERR.UM       = mUM      ;
    mPSLVERR.DBG      = mDBG     ;
    mPSLVERR.SEC      = mSecure  ;
    mPSLVERR.ErrAddr  = mErrAddr ;
    
    mPSLVERR_asserted = true;
    double current_time = sc_time_stamp().to_double();
    mAssertPSLVERREvent.notify(CalculateCLKPosEdge((double) mPCLKPeriod, false, current_time, mTimeCLKChangePoint), mTimeResolutionUnit);
}//}}}

/// Get KCE bit Status
bool HI20FSGD2::CheckKceBitStatus (void)
{//{{{
    mKCPROT_KCE = KCPROT_KCE.read();
    if (mKCPROT_KCE){
        re_printf("info", "Port KCPROT_KCE is enabled.\n");
    }
    else{
        re_printf("info", "Port KCPROT_KCE is disabled.\n");
    }
    return mKCPROT_KCE;
}//}}}

/// Get Reset Status
bool HI20FSGD2::GetResetStatus (void)
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

///Check clock period value
bool HI20FSGD2::CheckClockPeriod (void)
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

///Update clock PCLK
void HI20FSGD2::PCLKUpdate (void)
{//{{{
    mHI20FSGD2_DataHandler->SetCLKPeriod(mPCLKPeriod);
    double current_time = sc_time_stamp().to_double();
    mTimeCLKChangePoint = current_time;
}//}}}

/// Cancel operation events
void HI20FSGD2::CancelEvents (void)
{//{{{
    mAssertPSLVERREvent.cancel();
    mDeassertPSLVERREvent.cancel();
}//}}}

/// Wrapper for accessing registers
void HI20FSGD2::RegisterHandler (const std::vector<std::string> cmd)
{//{{{
    mHI20FSGD2_Func->RegisterHandler(cmd);

}//}}}

///Execute reset operation
void HI20FSGD2::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mHI20FSGD2_Func->Reset(is_active);
    mHI20FSGD2_DataHandler->EnableReset(is_active);
    if (is_active) {
        CancelEvents();
        Initialize();
    }
}//}}}

///Process reset function when reset port is active
void HI20FSGD2::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == vpcl::emResetActive) {
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
void HI20FSGD2::HandleResetCmdMethod (void)
{//{{{
    std::string reset_name = "resetPort";
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when reset command is active
void HI20FSGD2::CancelResetCmdMethod (void)
{//{{{
    std::string reset_name = "resetPort";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
}//}}}

///Handle resetPort signal
void HI20FSGD2::HandleresetPortSignalMethod (void)
{//{{{
    mResetCurVal = resetPort.read();
    if (mResetCurVal != mResetPreVal) {   //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify();
    }
}//}}}

///Handle PCLK signal
void HI20FSGD2::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle PCLK signal
void HI20FSGD2::HandleKCPROT_KCESignalMethod (void)
{//{{{
    CheckKceBitStatus();
}//}}}

/// Enable/disable dumping interrupt information
void HI20FSGD2::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mDumpInterrupt) {
        if (value) {
            re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
        } else {
            re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
        }
   }
}//}}}

/// Get value LOCK bit's setting of PBGPROT0 register
unsigned int HI20FSGD2::GetLOCK_SUPPORT()
{
   return mLock;
}

//========================================================
//============         PYTHON IF       ===================
//========================================================

///Set message level (fatal, error, warning, info)
void HI20FSGD2::SetMessageLevel (const std::string msg_lv)
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

        mHI20FSGD2_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mHI20FSGD2_Func->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mHI20FSGD2_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mHI20FSGD2_Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping interrupt information
void HI20FSGD2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HI20FSGD2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","HI20FSGD2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HI20FSGD2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Enable/disable info/warning/error message of register IF
void HI20FSGD2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: HI20FSGD2_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","HI20FSGD2_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HI20FSGD2_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///Enable/disable dumping message when users access registers
void HI20FSGD2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mHI20FSGD2_Func->RegisterHandler(cmd);
}//}}}

void HI20FSGD2::LOCK_SUPPORT(unsigned int value)
{
  if (value <= HI20FSGD2_AgentController::MAX_LOCK){
        mLock = value;
  }else{
        re_printf("warning", "Invalid value (%d) of the HI20FSGD2_LOCK_SUPPORT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, HI20FSGD2_AgentController::MIN_LOCK, HI20FSGD2_AgentController::MAX_LOCK, HI20FSGD2_AgentController::DEFAULT_LOCK);
  }
}

void HI20FSGD2::SEC_DIS(unsigned int value)
{
  if (value <= HI20FSGD2_AgentController::MAX_SEC){
    mHI20FSGD2_DataHandler->SetSecureFunc(value);
  }else{
        re_printf("warning", "Invalid value (%d) of the HI20FSGD2_SEC_DIS parameter. Valid range is %d-%d. Value 1 is set as default.\n", value, HI20FSGD2_AgentController::MIN_LOCK, HI20FSGD2_AgentController::MAX_LOCK);
  }
}

///Enable/disable target module only ICUMHA
void HI20FSGD2::SetICUMHA (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mICUMHA = true;
            re_printf("info","Guard module is the ICUMHA. \n");
        } else if (word_vector[0] == "false") {
            mICUMHA = false;
            re_printf("info","Guard module is not the ICUMHA. \n");
        } else {
            re_printf("warning","Invalid argument: HI20FSGD2_SetICUMHA %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mICUMHA) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","HI20FSGD2_SetICUMHA %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HI20FSGD2_SetICUMHA %s\n", is_enable.c_str());
    }
}//}}}

/// List all registers name
void HI20FSGD2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mHI20FSGD2_Func->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void HI20FSGD2::ReadRegister (const std::string reg_name)
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
void HI20FSGD2::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
void HI20FSGD2::ReleaseRegister (const std::string reg_name)
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
void HI20FSGD2::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
void HI20FSGD2::GetCLKFreq (const std::string clock_name)
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
void HI20FSGD2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
void HI20FSGD2::AssertReset (const std::string reset_name, const double start_time, const double period)
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

///Set address of guard areas
void HI20FSGD2::SetAreaAddress(const std::string group_id, const unsigned int start_addr, const unsigned int size, const unsigned int ch_id)
{//{{{
    re_printf("info","PBG group_id: %s, start_addr: %x, size: %d,ch_id: %d\n",group_id.c_str(),start_addr,size,ch_id);
    if (ch_id >= mChannelNum) {
        re_printf("error","channel number is invalid");
     }
    else
    {
        mHI20FSGD2_DataHandler->SetAreaAddr(group_id, start_addr, size, ch_id);
    }
}//}}}

///Set initial value to register
void HI20FSGD2::SetInitialValue (const std::string reg_name, const unsigned int ch_id, unsigned int value)
{//{{{
    mHI20FSGD2_Func->SetInitValReg(reg_name, ch_id, value);
}//}}}

///Dump help message of all parameters or commands
void HI20FSGD2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_MessageLevel (fatal|error|warning|info)                       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_DumpRegisterRW (true/false)                                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_DumpInterrupt (true/false)                                    Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_EnableRegisterMessage (true/false)                            Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_LOCK_SUPPORT (true/false)                                     Check value of parameter and judge LOCK bit's setting of HI20FSGD2PROT0_m register is used or not (Default: false).");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_SEC_DIS (true/false)                                          Check value of parameter and judge SEC bit's setting of HI20FSGD2PROT0_m register is used or not (Default: true).");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_SetICUMHA (true/false)                                        Set target of module is used for ICUMHA or not(Default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_AssertReset (reset_name, start_time, period)                  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_SetCLKFreq (clock_name, freq, unit)                           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_GetCLKFreq (clock_name)                                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ForceRegister (reg_name, reg_value)                           Force HI20FSGD2’s register with setting value.");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ReleaseRegister (reg_name)                                    Release HI20FSGD2’s register from force value.");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_WriteRegister (reg_name, reg_value)                           Write a value to a HI20FSGD2’s register.");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ReadRegister (reg_name)                                       Read a value from a HI20FSGD2’s register.");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ListRegister ()                                               Dump name of model's registers.");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_SetAreaAddress (group_name, start_addr, size, channel_id)     Set Address of protected area.");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_SetInitialValue (reg_name, ch_id, value)                      Set initial value to PBGPROT0 or PBGPROT1 registers.");
        } else {
            re_printf("warning","The name (%s) of HI20FSGD2_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of HI20FSGD2_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

