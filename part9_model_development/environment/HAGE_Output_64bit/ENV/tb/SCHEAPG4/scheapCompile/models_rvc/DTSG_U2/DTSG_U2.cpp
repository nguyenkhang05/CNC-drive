// ---------------------------------------------------------------------
// $Id: DTSG_U2.cpp,v 1.12 2020/01/16 02:37:09 synguyen Exp $
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


#include "DTSG_U2.h"
#include "DTSG_U2_Common_Guard.h"
#include "DTSG_U2_Func.h"
#include "re_define.h"

///Constructor of DTSG_U2 class
DTSG_U2::DTSG_U2 (sc_module_name name,
    unsigned int rLatency,
    unsigned int wLatency,
    unsigned int numChannel,
      SIM_MODE_T simmode):
      DTSG_U2_AgentController(),
      rvc_common_model(),
      BusBridgeModuleBase<BUS_WIDTH_APB, BUS_WIDTH_APB>(name),
      BusBridgeSlaveBase<BUS_WIDTH_APB, 1>(),
      BusMasterBase<BUS_WIDTH_APB, 1>(),
      BusSlaveBase<BUS_WIDTH_APB, 1>(),
      clk("clk"),
      dtsguard_error("dtsguard_error")
{//{{{
  CommandInit((std::string)name);
  mCLKPeriod           = 0;
  mCLKFreq             = 0;
  mCLKOrgFreq          = 0;
  mCLKFreqUnit         = "Hz";

  mTimeResolutionValue = 1;
  mTimeResolutionUnit  = SC_NS;
  GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

  mResetCurVal         = true;
  mResetPreVal         = true;
  mIsResetHardActive   = false;
  mIsResetCmdReceive   = false;
  mIsResetCmdActive    = false;
  mResetCmdPeriod      = 0;
  mTimeCLKChangePoint  = 0;
  mDumpInterrupt       = false;
  mEnableRegisterMessage = true;
  
  // Initialize internal variables
  dtsguard_error.initialize(false);
  mDTSG_ERR_value      = false;
  mNumOfChannel        = numChannel;
  if ((mNumOfChannel < 1) || (mNumOfChannel > 129)) {
      re_printf("warning", "Number of channels is wrong, it is set to the default value 129 automatically");
      mNumOfChannel = 129;
  }
  // Initialize Bus
  BusSlaveBase<BUS_WIDTH_APB, 1>::setSlaveResetPort32(&resetPort);
  BusSlaveBase<BUS_WIDTH_APB, 1>::setSlaveFreqPort32(&clk);
  BusSlaveBase<BUS_WIDTH_APB, 1>::setTargetSocket32((char*)"tsp");

  BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::setSlaveResetPort(&resetPort);
  BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::setSlaveFreqPort(&clk);
  BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::setTargetSocket((char*)"tspa");

  BusMasterBase<BUS_WIDTH_APB, 1>::setMasterResetPort32(&resetPort);
  BusMasterBase<BUS_WIDTH_APB, 1>::setMasterFreqPort32(&clk);
  BusMasterBase<BUS_WIDTH_APB, 1>::setInitiatorSocket32((char*)"a_is");

  a_is = BusMasterBase<BUS_WIDTH_APB, 1>::iSocket32[0];
  tspa = BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::tSocket[0];
  tsp  = BusSlaveBase<BUS_WIDTH_APB, 1>::tSocket32[0];

  //Instance DTSG_U2_Func
  mDTSG_U2_Func = new DTSG_U2_Func((std::string)name, this, mNumOfChannel);
  sc_assert(mDTSG_U2_Func != NULL);
  mBusSlaveIf32[0]->setFuncModulePtr(mDTSG_U2_Func);
  mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
  mBusSlaveIf32[0]->setReadLatency(rLatency);
  mBusSlaveIf32[0]->setWriteLatency(wLatency);
  mBusSlaveIf32[0]->setTransNmbLmt(0x1);

  //Instance DTSG_U2_Common_Guard
  mDTSG_U2_DataHandler = new DTSG_U2_Common_Guard((std::string)name, this, this, mBusSlaveIf[0], mBusMasterIf32[0], mNumOfChannel, simmode);
  sc_assert(mDTSG_U2_DataHandler != NULL);
  // BusBridgeModuleBase
  BusBridgeModuleBase<BUS_WIDTH_APB, BUS_WIDTH_APB>::mpCore = mDTSG_U2_DataHandler;

  // BusBridgeSlaveBase
  mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *)mDTSG_U2_DataHandler);
  mBusSlaveIf[0]->setBusProtocol(BUS_APB);
  mBusSlaveIf[0]->setTransNmbLmt(0x1);

  // BusMasterBase
  mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)mDTSG_U2_DataHandler);
  mBusMasterIf32[0]->setBusProtocol(BUS_APB);
  mBusMasterIf32[0]->setTransNmbLmt(0x1);

  SC_METHOD(HandleCLKSignalMethod);
  dont_initialize();
  sensitive << clk;
  
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

  SC_METHOD(AssertDTSG_ERRMethod);
  dont_initialize();
  sensitive << mAssertDTSG_ERREvent;

  SC_METHOD(DeassertDTSG_ERRMethod);
  dont_initialize();
  sensitive << mDeassertDTSG_ERREvent;

  SC_METHOD(HandleErrorMethod);
  dont_initialize();
  sensitive << mDTSG_U2_DataHandler->mTransErrorEvent;

  SC_METHOD(WriteDTSG_ERRMethod);
  dont_initialize();
  sensitive << mWriteDTSG_ERREvent;

}//}}}

///Destructor of DTSG_U2 class
DTSG_U2::~DTSG_U2 (void)
{//{{{
   delete mDTSG_U2_Func;
   delete mDTSG_U2_DataHandler;
}//}}}

///Get value of DTSGPROT[channel_index]
void DTSG_U2::GetDTSGPROT(const unsigned int channel_index, unsigned int& mpid, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg)
{
    mDTSG_U2_Func->GetDTSGPROT(channel_index, mpid, gen, dbg, um, wg, rg);
}

///Get DTSGSPIDERRSTAT [SPIDERR]
unsigned int DTSG_U2::GetSPIDERR()
{
    return mDTSG_U2_Func->GetSPIDERR();
}

///Get GetDTSGMPID[SPID]
unsigned int DTSG_U2::GetDTSGMPID_SPID(const unsigned int channel_index)
{
    return mDTSG_U2_Func->GetDTSGMPID_SPID(channel_index);
}

///Assert dtsguard_error signal method
void DTSG_U2::AssertDTSG_ERRMethod(void) 
{//{{{
    mDTSG_ERR_value = true;
    mWriteDTSG_ERREvent.notify(SC_ZERO_TIME);
    
    double current_time = sc_time_stamp().to_double();
    mDeassertDTSG_ERREvent.notify(CalculateCLKPosEdge(mCLKPeriod, false, current_time, mTimeCLKChangePoint) + mCLKPeriod, mTimeResolutionUnit);
}//}}}

///Deassert dtsguard_error signal method
void DTSG_U2::DeassertDTSG_ERRMethod(void) 
{//{{{
    mDTSG_ERR_value = false;
    mWriteDTSG_ERREvent.notify(SC_ZERO_TIME);
    
}//}}}

///Write dtsguard_error signal method.
void DTSG_U2::WriteDTSG_ERRMethod(void) 
{//{{{
    bool pre_value = dtsguard_error.read();
    if(pre_value != mDTSG_ERR_value){
        DumpInterruptMsg("dtsguard_error", mDTSG_ERR_value);
    }
    dtsguard_error.write(mDTSG_ERR_value);
}//}}}

///Set address of guard areas
void DTSG_U2::SetAreaAddress(const unsigned int start_addr, const unsigned int size, const unsigned int channel)
{//{{{   
  if (channel > mNumOfChannel - 1) {
      re_printf("error", "Start_addr: 0x%x, size: 0x%x is assigned to channel %d out of supported channel = %d. This setting is ignored\n", start_addr, size, channel, mNumOfChannel);
  } else {
        if (size == 0) {
            re_printf("warning","Invalid setting size = 0x0. This setting is ignore.\n");
            return;
        } else if (channel < mNumOfChannel - 1){
            re_printf("info","Start_addr: 0x%x, size: 0x%x is assigned to channel %d\n", start_addr, size, channel );
        } else {
            re_printf("info","Start_addr: 0x%x, size: 0x%x is assigned to channel GR\n", start_addr, size);
        }
        mDTSG_U2_DataHandler->SetAreaAddr(start_addr, size, channel);
  }
}//}}}

///Check access permission to peripherals
void DTSG_U2::HandleErrorMethod(void)

{//{{{
    double current_time = sc_time_stamp().to_double();
    if(GetSPIDERR() == 0){
        mDTSG_U2_Func->SetVar("ADDR", mDTSG_U2_DataHandler->GetVAR("ADDR"));
        mDTSG_U2_Func->SetVar("SEC", mDTSG_U2_DataHandler->GetVAR("SEC"));
        mDTSG_U2_Func->SetVar("DBG", mDTSG_U2_DataHandler->GetVAR("DBG"));
        mDTSG_U2_Func->SetVar("UM", mDTSG_U2_DataHandler->GetVAR("UM"));
        mDTSG_U2_Func->SetVar("SPID", mDTSG_U2_DataHandler->GetVAR("SPID"));
        mDTSG_U2_Func->SetVar("WRITE", mDTSG_U2_DataHandler->GetVAR("WRITE"));
    }

    if(((GetSPIDERR() >> (mDTSG_U2_DataHandler->GetVAR("SPID"))) & 0x1) == 0){
        mAssertDTSG_ERREvent.notify(CalculateCLKPosEdge(mCLKPeriod, false, current_time, mTimeCLKChangePoint), mTimeResolutionUnit);
    }
    mDTSG_U2_Func->SetVar("SPIDERR",1 << (mDTSG_U2_DataHandler->GetVAR("SPID")));
}//}}}

///Check clock period value
bool DTSG_U2::CheckClockPeriod (const std::string clock_name)

{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = mCLKPeriod;
    
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Update clk clock period
void DTSG_U2::CLKUpdate (void)
{//{{{
    mDTSG_U2_DataHandler->SetCLKPeriod(mCLKPeriod);
    double current_time = sc_time_stamp().to_double();
    mTimeCLKChangePoint = current_time;
}//}}}

///Cancel operation events
void DTSG_U2::CancelEvents (void)
{//{{{
    mAssertDTSG_ERREvent.cancel();
    mDeassertDTSG_ERREvent.cancel();
    mWriteDTSG_ERREvent.cancel();
}//}}}


///Wrapper for accessing registers
void DTSG_U2::RegisterHandler (const std::vector<std::string> cmd, const std::string reg_name)
{//{{{
    mDTSG_U2_Func->RegisterHandler(cmd);  
}//}}}

///Execute reset operation
void DTSG_U2::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mDTSG_U2_Func->Reset(is_active);
    mDTSG_U2_DataHandler->EnableReset(is_active);
    if (is_active) {
        CancelEvents();
        mDTSG_ERR_value = false;
        mWriteDTSG_ERREvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when reset port is active
void DTSG_U2::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == vpcl::emResetActive) {
        mIsResetHardActive = true;
        re_printf("info", "The reset port resetPort is asserted.\n");
        this->EnableReset("resetPort", mIsResetHardActive);
    } else {
        mIsResetHardActive = false;
        re_printf("info", "The reset port resetPort is de-asserted.\n");
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
void DTSG_U2::HandleResetCmdMethod (void)
{//{{{
    std::string reset_name = "resetPort";
    mIsResetCmdActive = true;
    re_printf("info", "The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when reset command is active
void DTSG_U2::CancelResetCmdMethod (void)
{//{{{
    std::string reset_name = "resetPort";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info", "Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
}//}}}

///Handle resetPort signal
void DTSG_U2::HandleResetSignalMethod (void)
{//{{{
    mResetCurVal = resetPort.read();
    if (mResetCurVal != mResetPreVal) {   //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Handle PCLK signal
void DTSG_U2::HandleCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = clk.read();
    SetCLKFreq("clk", freq_value, "Hz");
}//}}}

///Enable/disable dumping interrupt information
void DTSG_U2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: DTSG_U2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string status = "true";
        if (mDumpInterrupt == false)
        {
            status = "false";
        }
        re_printf("info", "DTSG_U2_DumpInterrupt %s\n", status.c_str());
    } else {
        re_printf("warning", "Invalid argument: DTSG_U2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Enable/disable dumping info/warning/error message of register IF
void DTSG_U2::EnableRegisterMessage (const std::string is_enable)
{
  std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }
        else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning", "Invalid argument: DTSG_U2_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } 
    else if (word_vector.size() == 0) {
        std::string status = "true";
        if (mEnableRegisterMessage == false)
        {
            status = "false";
        }
        re_printf("info", "DTSG_U2_EnableRegisterMessage %s\n", status.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: DTSG_U2_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}

///Enable/disable dumping message when users access registers
void DTSG_U2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mDTSG_U2_Func->RegisterHandler(cmd);
}//}}}

///Set message level (fatal, error, warning, info)
void DTSG_U2::SetMessageLevel (const std::string msg_lv)
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
        mDTSG_U2_Func->RegisterHandler(cmd);

        // Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mDTSG_U2_Func->RegisterHandler(cmd);
    } else {
    //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        mDTSG_U2_Func->RegisterHandler(cmd);
        
        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        mDTSG_U2_Func->RegisterHandler(cmd);
    }
}//}}}

///Dump help message of all parameters or commands
void DTSG_U2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(), "DTSG_U2_MessageLevel (fatal|error|warning|info)                       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_DumpRegisterRW (true/false)                                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_DumpInterrupt (true/false)                                    Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_EnableRegisterMessage (true/false)                            Enable/disable dumping info/warning/error message of register IF (Default: true).");

        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(), "DTSG_U2_AssertReset (reset_name, start_time, period)                  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_SetCLKFreq (clock_name, freq, unit)                           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_GetCLKFreq (clock_name)                                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_ForceRegister (reg_name, reg_value)                           Force DTSG_U2 register with setting value.");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_ReleaseRegister (reg_name)                                    Release DTSG_U2 register from force value.");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_WriteRegister (reg_name, reg_value)                           Write a value to a DTSG_U2 register.");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_ReadRegister (reg_name)                                       Read a value from a DTSG_U2 register.");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_ListRegister ()                                               Dump name of model registers.");
            SC_REPORT_INFO(this->basename(), "DTSG_U2_SetAreaAddress (start_addr, size, channel_idx)                Set Address of protected area for a channel.");
        } else {
            re_printf("warning", "The name (%s) of DTSG_U2_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning", "The name (%s) of DTSG_U2_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///List all registers of model
void DTSG_U2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mDTSG_U2_Func->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void DTSG_U2::ReadRegister (const std::string reg_name)
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
            re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void DTSG_U2::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
void DTSG_U2::ReleaseRegister (const std::string reg_name)
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
void DTSG_U2::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
void DTSG_U2::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "clk") {
            re_printf("info","clk frequency is %0.0f %s\n", (double)mCLKOrgFreq, mCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Set clock value and clock unit
void DTSG_U2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "clk") {
            mCLKOrgFreq = freq;
            ConvertClockFreq (mCLKFreq, mCLKFreqUnit, freq, unit);
            if (mCLKFreq > 0) {
                if (mCLKPeriod == 0 && mDTSG_ERR_value){ // Change clock from Zero Clock
                    mDTSG_ERR_value = false;
                    mWriteDTSG_ERREvent.notify(SC_ZERO_TIME);
                }
                mCLKPeriod = (sc_dt::uint64)(((1/(double)mCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mCLKPeriod = 0;
                // Cancel events
                CancelEvents();
            }
            CLKUpdate();
        } else {
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Assert reset by software
void DTSG_U2::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "resetPort") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info", "The model will be reset (resetPort) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning", "The software reset of resetPort is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning", "The reset name (%s) is wrong. It should be resetPort\n", reset_name.c_str());
    }
}//}}}

///DumpInterrupt Message 
void DTSG_U2::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
    if (mDumpInterrupt) {
        if (value) {
            re_printf("info", "INT [%s] Assert.\n", intr_name.c_str());
        } else {
            re_printf("info", "INT [%s] De-assert.\n", intr_name.c_str());
        }
    }
}//}}}


// vim600: set foldmethod=marker :
