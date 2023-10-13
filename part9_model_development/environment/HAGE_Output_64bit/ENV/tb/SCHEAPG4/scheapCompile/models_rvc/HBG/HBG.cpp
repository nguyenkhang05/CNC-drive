// ---------------------------------------------------------------------
// $Id: HBG.cpp,v 1.17 2020/03/08 06:28:17 huyquocpham Exp $
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


#include "HBG.h"
#include "HBG_Common_Guard.h"
#include "HBG_Func.h"
#include "HBGERRSLV_Func.h"

///Constructor of HBG class
HBG::HBG (sc_module_name name,
    unsigned int rLatency,
    unsigned int wLatency,
      SIM_MODE_T simmode):
      HBG_AgentController(),
      BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_AXI>(name),
      BusBridgeSlaveBase<BUS_WIDTH_AXI,1>(),
      BusMasterBase<BUS_WIDTH_AXI,1>(),
      BusSlaveBase<BUS_WIDTH_APB,2>(),
      PCLK("PCLK"),
      ACLK("ACLK"),
      AXI_GRERR("AXI_GRERR"),
      mLock(HBG_AgentController::DEFAULT_LOCK),
      mSec(HBG_AgentController::DEFAULT_SEC)
{//{{{
  mPCLKPeriod   = 0;
  mPCLKFreq     = 0;
  mPCLKOrgFreq  = 0;
  mPCLKFreqUnit = "Hz";

  mACLKPeriod   = 0;
  mACLKFreq     = 0;
  mACLKOrgFreq  = 0;
  mACLKFreqUnit = "Hz";

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

  mDumpInterrupt      = false;
  mEnableRegisterMessage = true;
  // Initialize internal variables
  AXI_GRERR.initialize(false);
  mAXI_GRERR_value = false;
  
  // Initialize Bus
  setSlaveResetPort32(&resetPort,&resetPort);
  setSlaveFreqPort32(&PCLK,&PCLK);
  setTargetSocket32("tsp1","tsp2");

  BusBridgeSlaveBase<BUS_WIDTH_AXI, 1>::setSlaveResetPort(&resetPort);
  BusBridgeSlaveBase<BUS_WIDTH_AXI, 1>::setSlaveFreqPort(&ACLK);
  BusBridgeSlaveBase<BUS_WIDTH_AXI, 1>::setTargetSocket("as_ts");

  BusMasterBase<BUS_WIDTH_AXI,1>::setMasterResetPort64(&resetPort);
  BusMasterBase<BUS_WIDTH_AXI,1>::setMasterFreqPort64(&ACLK);
  BusMasterBase<BUS_WIDTH_AXI,1>::setInitiatorSocket64("am_is");

  am_is = BusMasterBase<BUS_WIDTH_AXI, 1>::iSocket64[0];
  as_ts = BusBridgeSlaveBase<BUS_WIDTH_AXI, 1>::tSocket[0];
  tsp2 = BusSlaveBase<BUS_WIDTH_APB, 2>::tSocket32[1];
  tsp1 = BusSlaveBase<BUS_WIDTH_APB, 2>::tSocket32[0];

  //Instance HBG_Func
  mHBG_Func = new HBG_Func((std::string)name, this);
  sc_assert(mHBG_Func != NULL);
  mBusSlaveIf32[0]->setFuncModulePtr(mHBG_Func);
  mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
  mBusSlaveIf32[0]->setReadLatency(rLatency);
  mBusSlaveIf32[0]->setWriteLatency(wLatency);
  mBusSlaveIf32[0]->setTransNmbLmt(0x1);

  //Instance HBGERRSLV_Func
  mHBGERRSLV_Func = new HBGERRSLV_Func((std::string)name, this);
  sc_assert(mHBGERRSLV_Func != NULL);
  mBusSlaveIf32[1]->setFuncModulePtr(mHBGERRSLV_Func);
  mBusSlaveIf32[1]->setBusProtocol(BUS_APB);
  mBusSlaveIf32[1]->setReadLatency(rLatency);
  mBusSlaveIf32[1]->setWriteLatency(wLatency);
  mBusSlaveIf32[1]->setTransNmbLmt(0x1);

  //Instance HBG_Common_Guard
  mHBG_DataHandler = new HBG_Common_Guard((std::string)name, this, mBusSlaveIf[0], mBusMasterIf64[0], simmode);
  sc_assert(mHBG_DataHandler != NULL);
  BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_AXI>::mpCore = mHBG_DataHandler;
  mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *)mHBG_DataHandler);
  mBusSlaveIf[0]->setBusProtocol(BUS_AXI);
  mBusSlaveIf[0]->setTransNmbLmt(0xFFFFFFFF);
  mBusMasterIf64[0]->setFuncModulePtr((BusMasterFuncIf *)mHBG_DataHandler);
  mBusMasterIf64[0]->setBusProtocol(BUS_AXI);
  mBusMasterIf64[0]->setTransNmbLmt(0xFFFFFFFF);
  mBusMasterIf64[0]->setTlmTransType(simmode);

  //Set default values of parameter
  RESET_VALUE((unsigned int) HBG_AgentController::DEFAULT_RESET_VALUE);
  SPID_RESET_VALUE((unsigned int) HBG_AgentController::DEFAULT_SPID_RESET_VALUE);

  SC_METHOD(HandlePCLKSignalMethod);
  dont_initialize();
  sensitive << PCLK;

  SC_METHOD(HandleACLKSignalMethod);
  dont_initialize();
  sensitive << ACLK;
  
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

  SC_METHOD(AssertAXI_GRERRMethod);
  dont_initialize();
  sensitive<<mAssertAXI_GRERREvent;

  SC_METHOD(DeassertAXI_GRERRMethod);
  dont_initialize();
  sensitive<<mDeassertAXI_GRERREvent;

  SC_METHOD(HandleErrorMethod);
  dont_initialize();
  sensitive<<mHBG_DataHandler->mTransErrorEvent;

  SC_METHOD(WriteAXI_GRERRMethod);
  dont_initialize();
  sensitive<<mWriteAXI_GRERREvent;

}//}}}

///Destructor of HBG class
HBG::~HBG (void)
{//{{{
   delete mHBGERRSLV_Func;
   delete mHBG_Func;
   delete mHBG_DataHandler;
}//}}}

//Set Value of HBGPROT1 to Common Guard
void HBG::SetHBGPROT1(const unsigned int spid) 
{//{{{
  mHBG_DataHandler->SetVAR("SPID",spid);
}//}}}

//Set Value of HBGPROT0 to Common Guard
void HBG::SetHBGPROT0(const unsigned int gen, const unsigned int dbg, const unsigned int um, const unsigned int wg, const unsigned int rg) 
{//{{{
    mHBG_DataHandler->SetVAR("GEN", gen);
    mHBG_DataHandler->SetVAR("DBG", dbg);
    mHBG_DataHandler->SetVAR("UM", um);
    mHBG_DataHandler->SetVAR("WG", wg);
    mHBG_DataHandler->SetVAR("RG", rg);
}//}}}

//Assert AXI  ERROR signal method
void HBG::AssertAXI_GRERRMethod(void) 
{//{{{
    bool pre_value = AXI_GRERR.read();
    mAXI_GRERR_value = true;
    mWriteAXI_GRERREvent.notify(SC_ZERO_TIME);
    DumpInterruptMsg("AXI_GRERR",true);
    double current_time = sc_time_stamp().to_double();
    mDeassertAXI_GRERREvent.notify(CalculateNextRisingEdge("PCLK", current_time,mTimeCLKChangePoint)+mPCLKPeriod,mTimeResolutionUnit);
}//}}}

//Deassert AXI ERROR signal method
void HBG::DeassertAXI_GRERRMethod(void) 
{//{{{
    bool pre_value = AXI_GRERR.read();
    mAXI_GRERR_value = false;
    mWriteAXI_GRERREvent.notify(SC_ZERO_TIME);
    if(pre_value) {
        DumpInterruptMsg("AXI_GRERR",false);
    }
}//}}}

//Write AXI ERROR signal method.
void HBG::WriteAXI_GRERRMethod(void) 
{//{{{
    AXI_GRERR.write(mAXI_GRERR_value);
}//}}}

//Set address of guard areas
void HBG::SetAreaAddress(const std::string group_id, const unsigned int start_addr, const unsigned int size)
{//{{{
    re_printf("info","HBG group_id: %s, start_addr: %x, size: %d\n",group_id.c_str(),start_addr,size);
    mHBG_DataHandler->SetAreaAddr(group_id, start_addr, size);
}//}}}

///Check access permission to peripherals
void HBG::HandleErrorMethod(void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if(mHBGERRSLV_Func->Get_SPIDERR() == 0){
        mHBGERRSLV_Func->SetVar("ADDR", mHBG_DataHandler->GetVAR("ADDR"));
        mHBGERRSLV_Func->SetVar("DBG", mHBG_DataHandler->GetVAR("DBG"));
        mHBGERRSLV_Func->SetVar("UM", mHBG_DataHandler->GetVAR("UM"));
        mHBGERRSLV_Func->SetVar("SPID",mHBG_DataHandler->GetVAR("SPID"));
        mHBGERRSLV_Func->SetVar("WRITE", mHBG_DataHandler->GetVAR("WRITE"));
        mHBGERRSLV_Func->SetVar("SEC", mHBG_DataHandler->GetVAR("SEC"));
    }
    if(((mHBGERRSLV_Func->Get_SPIDERR() >> (mHBG_DataHandler->GetVAR("SPID"))) & 0x1) == 0){
        mAssertAXI_GRERREvent.notify(CalculateNextRisingEdge("PCLK", current_time,mTimeCLKChangePoint),mTimeResolutionUnit);
    }
    mHBGERRSLV_Func->SetVar("SPIDERR",1 << (mHBG_DataHandler->GetVAR("SPID")));
}//}}}

///Check clock period value
bool HBG::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    // ACLK is set to Common Guard directy
    if(clock_name == "PCLK") {
        clock_period = mPCLKPeriod;
    }
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

void HBG::PCLKUpdate (void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mTimeCLKChangePoint = current_time;
}//}}}

void HBG::ACLKUpdate (void)
{//{{{
    mHBG_DataHandler->SetCLKPeriod(mACLKPeriod);
}//}}}

/// Cancel operation events
void HBG::CancelEvents (void)
{//{{{
    mAssertAXI_GRERREvent.cancel();
    mDeassertAXI_GRERREvent.cancel();
    mWriteAXI_GRERREvent.cancel();
}//}}}

///Calculate synchronous time
double HBG::CalculateNextRisingEdge (const std::string clock_name,const double time_point,const double time_change_point)
{//{{{
    double clock_period = 0;
    if(clock_name == "PCLK") {
        clock_period = (double)mPCLKPeriod;
    }
    return CalculateCLKPosEdge(clock_period, false, time_point, time_change_point);    
}//}}}

/// Wrapper for accessing registers
void HBG::RegisterHandler (const std::vector<std::string> cmd, const std::string reg_name)
{//{{{
    if(reg_name == "HBGPROT0" || reg_name == "HBGPROT1"){
        mHBG_Func->RegisterHandler(cmd);
    }else {
        mHBGERRSLV_Func->RegisterHandler(cmd);
    }
}//}}}

///Execute reset operation
void HBG::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mHBG_Func->Reset(is_active);
    mHBGERRSLV_Func->Reset(is_active);
    mHBG_DataHandler->EnableReset(is_active);
    if (is_active) {
        CancelEvents();
        mAXI_GRERR_value = false;
        mWriteAXI_GRERREvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when reset port is active
void HBG::HandleResetHardMethod (void)
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
void HBG::HandleResetCmdMethod (void)
{//{{{
    std::string reset_name = "resetPort";
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when reset command is active
void HBG::CancelResetCmdMethod (void)
{//{{{
    std::string reset_name = "resetPort";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
}//}}}

///Handle resetPort signal
void HBG::HandleResetPortSignalMethod (void)
{//{{{
    mResetCurVal = resetPort.read();
    if (mResetCurVal != mResetPreVal) {   //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Handle PCLK signal
void HBG::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle ACLK signal
void HBG::HandleACLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = ACLK.read();
    SetCLKFreq("ACLK", freq_value, "Hz");
}//}}}

///Enable/disable dumping interrupt information
void HBG::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HBG_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mDumpInterrupt)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
        re_printf("info","HBG_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HBG_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Enable/disable dumping info/warning/error message of register IF
void HBG::EnableRegisterMessage(const std::string is_enable)
{
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
            re_printf("warning", "Invalid argument: HBG_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }
    else if (word_vector.size() == 0) {
        std::string status = "true";
        if (mEnableRegisterMessage == false)
        {
            status = "false";
        }
        re_printf("info", "HBG_EnableRegisterMessage %s\n", status.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: HBG_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}

///Enable/disable dumping message when users access registers
void HBG::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mHBG_Func->RegisterHandler(cmd);
    mHBGERRSLV_Func->RegisterHandler(cmd);
}//}}}

///Set message level (fatal, error, warning, info)
void HBG::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    if (mEnableRegisterMessage == false) {
        // Disable message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mHBG_Func->RegisterHandler(cmd);
        mHBGERRSLV_Func->RegisterHandler(cmd);

        // Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mHBG_Func->RegisterHandler(cmd);
        mHBGERRSLV_Func->RegisterHandler(cmd);
    }
    else
    {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        mHBG_Func->RegisterHandler(cmd);
        mHBGERRSLV_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        mHBG_Func->RegisterHandler(cmd);
        mHBGERRSLV_Func->RegisterHandler(cmd);
    }
}//}}}

///Dump help message of all parameters or commands
void HBG::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(), "HBG_MessageLevel (fatal|error|warning|info)                       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(), "HBG_DumpRegisterRW (true/false)                                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(), "HBG_DumpInterrupt (true/false)                                    Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(), "HBG_EnableRegisterMessage (true/false)                            Enable/disable dumping info/warning/error message of register IF (Default: true).");
            SC_REPORT_INFO(this->basename(), "HBG_LOCK_SUPPORT (true/false)                                     Check value of parameter and judge LOCK bit's setting of HBGPROT0_m register is used or not (Default: false).");
            SC_REPORT_INFO(this->basename(), "HBG_SEC_DIS (true/false)                                          Check value of parameter and judge SEC bit's setting of HBGPROT0_m register is used or not (Default: true).");
            SC_REPORT_INFO(this->basename(), "HBG_RESET_VALUE (value)                                           Set value of parameter to init value of HBGPROT0 register (Default: 0x1F3).");
            SC_REPORT_INFO(this->basename(), "HBG_SPID_RESET_VALUE (value)                                      Set value of parameter to init value of HBGPROT1 register (Default: 0xFFFFFFFF).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(), "HBG_AssertReset (reset_name, start_time, period)                  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(), "HBG_SetCLKFreq (clock_name, freq, unit)                           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(), "HBG_GetCLKFreq (clock_name)                                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(), "HBG_ForceRegister (reg_name, reg_value)                           Force HBG's register with setting value.");
            SC_REPORT_INFO(this->basename(), "HBG_ReleaseRegister (reg_name)                                    Release HBG's register from force value.");
            SC_REPORT_INFO(this->basename(), "HBG_WriteRegister (reg_name, reg_value)                           Write a value to a HBG's register.");
            SC_REPORT_INFO(this->basename(), "HBG_ReadRegister (reg_name)                                       Read a value from a HBG's register.");
            SC_REPORT_INFO(this->basename(), "HBG_ListRegister ()                                               Dump name of model's registers.");
            SC_REPORT_INFO(this->basename(), "HBG_SetAreaAddress (group_name, start_addr, size)                 Set Address of protected area.");
        } else {
            re_printf("warning","The name (%s) of HBG_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of HBG_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

void HBG::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mHBG_Func->RegisterHandler(cmd);
    mHBGERRSLV_Func->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void HBG::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandler(cmd, reg_name);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void HBG::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandler(cmd, reg_name);      
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void HBG::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandler(cmd, reg_name); 
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Force value to register
void HBG::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandler(cmd, reg_name); 
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Get clock value
void HBG::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "ACLK") {
            re_printf("info","ACLK frequency is %0.0f %s\n", (double)mACLKOrgFreq, mACLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Set clock value and clock unit
void HBG::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
        } else if (word_vector[0] == "ACLK") {
            mACLKOrgFreq = freq;
            ConvertClockFreq (mACLKFreq, mACLKFreqUnit, freq, unit);
            if (mACLKFreq > 0) {
                mACLKPeriod = (sc_dt::uint64)(((1/(double)mACLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mACLKPeriod = 0;
            }
            ACLKUpdate();
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Assert reset by software
void HBG::AssertReset (const std::string reset_name, const double start_time, const double period)
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

//DumpInterrupt Message 
void HBG::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
    if (mDumpInterrupt) {
        if (value) {
            re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
        } else {
            re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
        }
    }
}//}}}

//Get value of LOCK_SUPPORT
unsigned int HBG::GetLOCK_SUPPORT()
{//{{{
    return mLock;
}//}}}

//Set value from P_LOCK_SUPPORT parameter
void HBG::LOCK_SUPPORT(unsigned int value)
{//{{{
    if (value <= HBG_AgentController::MAX_LOCK){
        mLock = value;
    }else{
        re_printf("warning", "Invalid value (%d) of the HBG_LOCK_SUPPORT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, HBG_AgentController::MIN_LOCK, HBG_AgentController::MAX_LOCK, HBG_AgentController::DEFAULT_LOCK);
    }
}//}}}

//Get value from P_Sec_Dis parameter
void HBG::SEC_DIS(unsigned int value)
{//{{{
    if (value <= HBG_AgentController::MAX_LOCK){
        mSec = value;
        mHBG_DataHandler->SEC_DIS((bool) value);
    }else{
        re_printf("warning", "Invalid value (%d) of the HBG_SEC_DIS parameter. Valid range is %d-%d. Value 1 is set as default.\n", value, HBG_AgentController::MIN_LOCK, HBG_AgentController::MAX_LOCK);
    }
}//}}}

/// Set/Get Value of Reset_Value
void HBG::RESET_VALUE(unsigned int value)
{//{{{
    mHBG_Func->SetInitHBGPROT0(value);
}//}}}

/// Set/Get Value of SPID_Reset_Value
void HBG::SPID_RESET_VALUE(unsigned int value)
{//{{{
    mHBG_Func->SetInitHBGPROT1(value);
}//}}}

// Get KCE bit 
unsigned int HBG::Get_KCE()
{//{{{
    return mHBGERRSLV_Func->Get_KCE();
}//}}}

// vim600: set foldmethod=marker :
