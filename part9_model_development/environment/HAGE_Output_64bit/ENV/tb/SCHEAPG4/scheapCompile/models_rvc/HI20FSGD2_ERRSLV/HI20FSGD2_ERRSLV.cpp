// ---------------------------------------------------------------------
// $Id: HI20FSGD2_ERRSLV.cpp,v 1.4 2019/12/06 07:20:20 hadinhtran Exp $
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "re_define.h"
#include "HI20FSGD2_ERRSLV.h"
#include "HI20FSGD2_ERRSLV_Func.h"

/// Description: Constructor of HI20FSGD2_ERRSLV class, define Clock port, Reset port
/// Constructor
HI20FSGD2_ERRSLV::HI20FSGD2_ERRSLV(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency, unsigned int iGUARD_NUM):
              sc_module(name),
          HI20FSGD2_ERRSLV_AgentController(),
          rvc_common_model(),
          BusSlaveBase<32,1>(),
          PCLK("PCLK"),
          Reset("Reset"),
          KCPROT_KCE("KCPROT_KCE"),
          PBG_ERR("PBG_ERR")
{//{{{
    mGuardNum = iGUARD_NUM;
    
    if(mGuardNum == 0 || mGuardNum > 4) {
        re_printf("warning","Number Guard Module is wrong, it is set to the default value 1 automatically");
        mGuardNum = 1;
    }
    re_printf("info", "Number of Guard Module is: %d\n", mGuardNum);
    // Initialize output ports
    PBG_ERR.initialize(false);
    KCPROT_KCE.initialize(false);

    mpHI20FSGD2_ERRSLVFunc = new HI20FSGD2_ERRSLV_Func((std::string)name, this);
    sc_assert(mpHI20FSGD2_ERRSLVFunc != NULL);
    // Configure Slave socket
    setSlaveResetPort32(&Reset);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32((char*)"ts");

    ts = tSocket32[0];

    mBusSlaveIf32[0]->setFuncModulePtr(mpHI20FSGD2_ERRSLVFunc);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    // Initialize variables
    mPCLKPeriod = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreq = 0;
    mPCLKFreqUnit = "Hz";
    mResetCurVal = true;
    mResetPreVal = true;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mEnableRegisterMessage = true;
    mDumpInterrupt = false;
    mStartPCLKTime = 0;
    mPrePCLKPeriod = 0;
    
    mDumpInterrupt = false;
    mEnableRegisterMessage = true;
    
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    Initialize();
    // SC_THREAD/METHOD
    PSLVERR = new sc_in<HI20FSGD2_Cerrinfo>*[mGuardNum];
    sc_assert(PSLVERR != NULL);
    for (unsigned int guard_id = 0; guard_id < mGuardNum; guard_id ++)
    {   port_name.str("");
        port_name<<"PSLVERR"<<guard_id;
        PSLVERR[guard_id] = new sc_in<HI20FSGD2_Cerrinfo>(port_name.str().c_str());
        sc_assert(PSLVERR[guard_id] != NULL);
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(PSLVERR[guard_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HI20FSGD2_ERRSLV::HandleErrorMethod, this, guard_id), 
                            sc_core::sc_gen_unique_name("HandleErrorMethod"), &opt);
    }
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleResetSignalMethod);
    sensitive << Reset;

    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(AssertGuardErrorMethod);
    dont_initialize();
    sensitive << mAssertGuardErrorEvent;

    SC_METHOD(DeassertGuardErrorMethod);
    dont_initialize();
    sensitive << mDeassertGuardErrorEvent;

    SC_METHOD(WriteGuardErrorMethod);
    dont_initialize();
    sensitive << mWriteGuardErrorEvent;
}//}}}

///Destructor of HI20FSGD2_ERRSLV class
HI20FSGD2_ERRSLV::~HI20FSGD2_ERRSLV (void)
{//{{{
    for (unsigned int guard_id = 0; guard_id < mGuardNum; guard_id ++){
        delete PSLVERR[guard_id];
    }
    delete [] PSLVERR;
    delete mpHI20FSGD2_ERRSLVFunc;
}//}}}

/// Initialize when reset activated
void HI20FSGD2_ERRSLV::Initialize(void)
{//{{{
    mErrorAddr             = (unsigned int)emZero;
    mGuard_Error_value     = false;
}//}}}

///Handle output port when Guard_Error port is asserted
void HI20FSGD2_ERRSLV::AssertGuardErrorMethod(void)
{//{{{
    mGuard_Error_value = true;
    mWriteGuardErrorEvent.notify(SC_ZERO_TIME);

    double current_time = sc_time_stamp().to_double();
    mDeassertGuardErrorEvent.notify((double)mPCLKPeriod *1, mTimeResolutionUnit);
}//}}}

///Handle output port when Guard_Error port is deasserted
void HI20FSGD2_ERRSLV::DeassertGuardErrorMethod(void)
{//{{{
    mGuard_Error_value = false;
    mWriteGuardErrorEvent.notify(SC_ZERO_TIME);
}//}}}

/// Description: Dump interrupt message
void HI20FSGD2_ERRSLV::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mDumpInterrupt) {
        if (value) {
            re_printf("info","%s is changed to true\n",intr_name.c_str());
        } else {
            re_printf("info","%s is changed to false\n",intr_name.c_str());
        }
   }
}//}}}

///Write current value of Guard_Error port to output port
void HI20FSGD2_ERRSLV::WriteGuardErrorMethod(void)
{//{{{
    bool pre_value = PBG_ERR.read();
    
    if(pre_value != mGuard_Error_value) {
        PBG_ERR.write(mGuard_Error_value);
        DumpInterruptMsg("PBG_ERR", mGuard_Error_value);
    }
}//}}}

///Enable Key Code bit to output port KCPROT_KCE
void HI20FSGD2_ERRSLV::SetKceBit(const bool value)
{//{{{
    if(value == true) {
        KCPROT_KCE.write(true);
        re_printf("info", "KCPROT_KCE bit is enabled");
    }
    else{
        KCPROT_KCE.write(false);
        re_printf("info", "KCPROT_KCE bit is disabled");
    }
}//}}}

/// Description: Handle output data and signal
void HI20FSGD2_ERRSLV::HandleErrorMethod(const unsigned int guard_id)
{//{{{
    // Get the current time 
    double current_time = sc_time_stamp().to_double();
    bool isNotifyGUARD_ERROR = false;
    HI20FSGD2_Cerrinfo mError_Info;
    mError_Info = PSLVERR[guard_id]->read();
    // Check error occurs or not
    if (!((mError_Info.PBGWrERR == false) && (mError_Info.PBGRdERR == false))){
        if (mError_Info.PBGRdERR == true)
        {
            mError_Info.PBGWrERR = false;
        }
        // Check the first guard error or not
        if (mpHI20FSGD2_ERRSLVFunc->IsTheFirstERR()) {
            mpHI20FSGD2_ERRSLVFunc->SetVar("ADDR", mError_Info.ErrAddr);
            mpHI20FSGD2_ERRSLVFunc->SetVar("SEC", mpHI20FSGD2_ERRSLVFunc->GetAccessInfo("SEC", mError_Info.SEC));
            mpHI20FSGD2_ERRSLVFunc->SetVar("DBG", mpHI20FSGD2_ERRSLVFunc->GetAccessInfo("DBG", mError_Info.DBG));
            mpHI20FSGD2_ERRSLVFunc->SetVar("UM", mpHI20FSGD2_ERRSLVFunc->GetAccessInfo("UM", mError_Info.UM));
            mpHI20FSGD2_ERRSLVFunc->SetVar("SPID", mpHI20FSGD2_ERRSLVFunc->GetAccessInfo("SPID", mError_Info.SPID));
            mpHI20FSGD2_ERRSLVFunc->SetVar("WRITE", mpHI20FSGD2_ERRSLVFunc->GetAccessInfo("WRITE", mError_Info.PBGWrERR));
            isNotifyGUARD_ERROR = true;
        }
        else {
            // Set overflow bit is true
            mpHI20FSGD2_ERRSLVFunc->SetOverFlowBit();
            // Check the same spid or not
            if (!(mpHI20FSGD2_ERRSLVFunc->IsTheSameSPID(mpHI20FSGD2_ERRSLVFunc->GetAccessInfo("SPID", mError_Info.SPID)))) {
                isNotifyGUARD_ERROR = true;
            }
        }    
        if (isNotifyGUARD_ERROR) {
            // Store access information
            mpHI20FSGD2_ERRSLVFunc->SetVar("SPIDERR", mpHI20FSGD2_ERRSLVFunc->GetAccessInfo("SPID", mError_Info.SPID));
            // Notify guard error
            mAssertGuardErrorEvent.notify(CalculateCLKPosEdge((double) mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
        }
    }
}//}}}

/// Description: Check clock period
bool HI20FSGD2_ERRSLV::CheckClockPeriod (void)
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

/// Get Reset Status
bool HI20FSGD2_ERRSLV::GetResetStatus (void)
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Cancel operation events
void HI20FSGD2_ERRSLV::CancelEvents (void)
{//{{{
    mAssertGuardErrorEvent.cancel();
    mDeassertGuardErrorEvent.cancel();
    mWriteGuardErrorEvent.cancel();
    mTransErrorEvent.cancel();
}//}}}

///Execute reset operation
void HI20FSGD2_ERRSLV::EnableReset (const bool is_active)
{//{{{
     mpHI20FSGD2_ERRSLVFunc->EnableReset(is_active);
     if (is_active) {
         // Cancel events
         CancelEvents();
         // Initialize variables
         Initialize();
         // Notify to write guard error
         mWriteGuardErrorEvent.notify(SC_ZERO_TIME);
     }   
}//}}}

/// Description: Process reset function when reset port is active
void HI20FSGD2_ERRSLV::HandleResetHardMethod(void)
{//{{{
    if (mResetCurVal == vpcl::emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset port Reset is asserted.\n");
        this->EnableReset(mIsResetHardActive);
    } else {
        mIsResetHardActive = false;
        re_printf("info","The reset port Reset is de-asserted.\n");
        this->EnableReset(mIsResetHardActive);
    }

    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
} //}}}

void HI20FSGD2_ERRSLV::HandleResetCmdMethod(void)
{//{{{
    std::string reset_name = "Reset";
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Description: Cancel reset function when reset command is active
void HI20FSGD2_ERRSLV::CancelResetCmdMethod(void)
{//{{{
    std::string reset_name = "Reset";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive);
}//}}}

/// Description: Monitor Reset port
void HI20FSGD2_ERRSLV::HandleResetSignalMethod (void)
{//{{{
    mResetCurVal = Reset.read();
    if (mResetCurVal != mResetPreVal) {   //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify();
    }
}//}}}

/// Description: Monitor PCLK clock
void HI20FSGD2_ERRSLV::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

//========================================================
//============         PYTHON IF       ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void HI20FSGD2_ERRSLV::SetMessageLevel (const std::string msg_lv)
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

    mpHI20FSGD2_ERRSLVFunc->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    mpHI20FSGD2_ERRSLVFunc->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping message when users access registers
void HI20FSGD2_ERRSLV::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHI20FSGD2_ERRSLVFunc->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information
void HI20FSGD2_ERRSLV::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HI20FSGD2_ERRSLV_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","HI20FSGD2_ERRSLV_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HI20FSGD2_ERRSLV_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Enable/disable info/warning/error message of register IF
void HI20FSGD2_ERRSLV::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {

            re_printf("warning","Invalid argument: HI20FSGD2_ERRSLV_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","HI20FSGD2_ERRSLV_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HI20FSGD2_ERRSLV_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

/// Command to assert reset
void HI20FSGD2_ERRSLV::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "Reset") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (Reset) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of Reset is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be Reset.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void HI20FSGD2_ERRSLV::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void HI20FSGD2_ERRSLV::GetCLKFreq (const std::string clock_name)
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

/// Force value to register
void HI20FSGD2_ERRSLV::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpHI20FSGD2_ERRSLVFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void HI20FSGD2_ERRSLV::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHI20FSGD2_ERRSLVFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void HI20FSGD2_ERRSLV::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpHI20FSGD2_ERRSLVFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void HI20FSGD2_ERRSLV::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpHI20FSGD2_ERRSLVFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void HI20FSGD2_ERRSLV::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHI20FSGD2_ERRSLVFunc->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void HI20FSGD2_ERRSLV::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_MessageLevel (\"HI20FSGD2_ERRSLV instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_DumpRegisterRW (\"HI20FSGD2_ERRSLV instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_DumpInterrupt (\"HI20FSGD2_ERRSLV instance\", \"true/false\")                     Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_EnableRegisterMessage (\"HI20FSGD2_ERRSLV instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_AssertReset (\"HI20FSGD2_ERRSLV instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the HI20FSGD2_ERRSLV model");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_SetCLKFreq (\"HI20FSGD2_ERRSLV instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to HI20FSGD2_ERRSLV model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_GetCLKFreq (\"HI20FSGD2_ERRSLV instance\", \"clock_name\")                        Get clock frequency value of HI20FSGD2_ERRSLV model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_ForceRegister (\"HI20FSGD2_ERRSLV instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_ReleaseRegister (\"HI20FSGD2_ERRSLV instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_WriteRegister (\"HI20FSGD2_ERRSLV instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_ReadRegister (\"HI20FSGD2_ERRSLV instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(),"HI20FSGD2_ERRSLV_ListRegister (\"HI20FSGD2_ERRSLV instance\")                                      Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of HI20FSGD2_ERRSLV_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of HI20FSGD2_ERRSLV_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

