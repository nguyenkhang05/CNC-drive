// ---------------------------------------------------------------------
// $Id: PIC1_E2.cpp 3883 2017-08-23 07:58:30Z binh.nguyen.ym $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "PIC1_E2.h"
#include "PIC1_E2_Func.h"

///*********************************
/// Function     : PIC1_E2
/// Description  : Constructor of PIC1_E2 class
///*********************************
PIC1_E2::PIC1_E2 (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency):
                sc_module(name),
                PIC1_E2_AgentController(),
                BusSlaveBase<32,1>(),
                CLK_HSB("CLK_HSB"),
                sys_resetz("sys_resetz"),
                OST0TSST("OST0TSST"),
                OST1TSST("OST1TSST"),
                OST2TSST("OST2TSST"),
                OST3TSST("OST3TSST"),
                OST4TSST("OST4TSST"),
                OST5TSST("OST5TSST"),
                OST6TSST("OST6TSST")
{//{{{
    //Initialize output signals
    OST0TSST.initialize(false);
    OST1TSST.initialize(false);
    OST2TSST.initialize(false);
    OST3TSST.initialize(false);
    OST4TSST.initialize(false);
    OST5TSST.initialize(false);
    OST6TSST.initialize(false);
    //--------
    mOutputDelay = 3;
    mOutputPeriod = 2;
    //Initialize variables - for clock, time resolution, reset
    mCLK_HSBPeriod = 0;
    mCLK_HSBFreq = 0;
    mCLK_HSBOrgFreq = 0;
    mCLK_HSBFreqUnit = "Hz";

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mResetCurVal = true;
    mResetPreVal = true;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod =  0;

    mTimeCLKChangePoint = 0;
    
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&CLK_HSB);
    setSlaveResetPort32(&sys_resetz);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mPIC1Func = new PIC1_E2_Func((std::string)name, this);
    sc_assert(mPIC1Func != NULL);

    mBusSlaveIf32[0]->setFuncModulePtr(mPIC1Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandleCLK_HSBSignalMethod);
    dont_initialize();
    sensitive << CLK_HSB;

    SC_METHOD(HandleSYS_RESETZSignalMethod);
    sensitive << sys_resetz;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    for (unsigned int signal_id = 0; signal_id < emmaxchannel; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRisingOSTOutEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC1_E2::RisingOSTOutSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("RisingOSTOutSignalsMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id  < emmaxchannel; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFallingOSTOutEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC1_E2::FallingOSTOutSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("FallingOSTOutSignalsMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id  < emmaxchannel; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mOutputWriteEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PIC1_E2::WriteOutSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("WriteOutSignalsMethod"), &opt);
    }

    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;
    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;
    
}//}}}

///*********************************
/// Function     : ~PIC1_E2
/// Description  : Destructor of PIC1_E2 class
///*********************************
PIC1_E2::~PIC1_E2 (void)
{//{{{ 
    delete mPIC1Func;
}//}}}

///*********************************
/// Function     : InitializeOutputSignals
/// Description  : Initialize output signals when reset is active
///*********************************
void PIC1_E2::InitializeOutputSignals (void)
{//{{{
    for (unsigned int i = 0; i < emmaxchannel; i++) {
        mOST[i] = false;
        mOutputWriteEvent[i].notify();
    }
}//}}}

///*********************************
/// Function     : SetMessageLevel
/// Description  : Set message level (fatal/error/warning/info)
///                Called by PythonIF
///*********************************
void PIC1_E2::SetMessageLevel (const std::string msg_lv)
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
    mPIC1Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mPIC1Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : DumpRegisterRW
/// Description  : Enable/disable dumping message when users access registers
///                Called by PythonIF
///*********************************
void PIC1_E2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mPIC1Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : AssertReset
/// Description  : Assert reset by software for sys_resetz
///                Called by PythonIF
///*********************************
void PIC1_E2::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "sys_resetz") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (sys_resetz) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of sys_resetz is ignored because it is called in the reset operation of the model.\n");
        }
    } else {
        re_printf("warning", "The name of reset signal is wrong. It should be sys_resetz.\n");
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq
/// Description  : Set clock value and clock unit for CLK_HSB
///                Called by PythonIF
///*********************************
void PIC1_E2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_HSB") {
            CheckFreq (mCLK_HSBFreq, mCLK_HSBFreqUnit, freq, unit);
            mCLK_HSBOrgFreq = freq;
            if (mCLK_HSBFreq > 0) {
                mCLK_HSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_HSBFreq)*(double)mTimeResolutionValue) + 0.5);
                mTimeCLKChangePoint = sc_time_stamp().to_double();
            } else {
                mCLK_HSBPeriod = 0;
                for (unsigned int i = 0; i < emmaxchannel; i++) {
                    mRisingOSTOutEvent[i].cancel();
                    mFallingOSTOutEvent[i].cancel();
                    mOutputWriteEvent[i].cancel();
                }
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : GetCLKFreq
/// Description  : Get clock value of CLK_HSB
///                Called by PythonIF
///*********************************
void PIC1_E2::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_HSB") {
            re_printf("info","CLK_HSB frequency is %0.0f %s.\n", (double)mCLK_HSBOrgFreq, mCLK_HSBFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    }
}//}}}

///*********************************
/// Function     : ForceRegister
/// Description  : Force value to register
///                Called by PythonIF
///*********************************
void PIC1_E2::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("force");
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mPIC1Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : ReleaseRegister
/// Description  : Release forced value after forcing registers
///                Called by PythonIF
///*********************************
void PIC1_E2::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("release");
    mPIC1Func->RegisterHandler(cmd);
}//}}} 

///*********************************
/// Function     : WriteRegister
/// Description  : Write value to registers by software
///                Called by PythonIF
///*********************************
void PIC1_E2::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{ 
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mPIC1Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : ReadRegister
/// Description  : Read value of register by software
///                Called by PythonIF
///*********************************
void PIC1_E2::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    mPIC1Func->RegisterHandler(cmd);
}//}}} 

///*********************************
/// Function     : SetOutputSyn
/// Description  : Setting the delay time of output signal
///                Called by PythonIF
///*********************************
void PIC1_E2::SetOutputSyn (const unsigned int pulse_latency, const unsigned int pulse_period)
{//{{{
    mOutputDelay = pulse_latency;
    if (pulse_period > 0) {
        mOutputPeriod = pulse_period;
    } else {
        re_printf ("warning","The number of output's period is invalid value, so it is set as default");
        mOutputPeriod = 2;
    }
}//}}}

///*********************************
/// Function     : ListRegister
/// Description  : List all reigsters name
///                Called by PythonIF
///*********************************
void PIC1_E2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mPIC1Func->RegisterHandler(cmd);
}//}}} 

///*********************************
/// Function     : Help
/// Description  : Dump help message of all parameters or commands
///                Called by PythonIF
///*********************************
void PIC1_E2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"--- parameters ---");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_MessageLevel   (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_DumpRegisterRW (true/false)                     Enable/disable dumping access register (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"--- commands ---");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_AssertReset    (reset_name, start_time, period) Assert and de-assert reset signal (sys_resetz).");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_SetCLKFreq     (clock_name, freq, unit)         Set clock frequency to CLK_HSB clock.");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_GetCLKFreq     (clock_name)                     Get clock frequency of CLK_HSB clock.");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_ForceRegister  (reg_name, value)                Force register with setting value.");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_ReleaseRegister(reg_name)                       Release register from force value.");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_WriteRegister  (reg_name, value)                Write a value to register.");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_ReadRegister   (reg_name)                       Read value from register.");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_ListRegister                                    Dump name of PIC1_E2's registers.");
            SC_REPORT_INFO(this->basename(),"PIC1_E2_SetOutputSyn   (pulse_latency, pulse_period)    Set the behavior of output ports.");
        } else {
            re_printf("warning","The name (%s) of PIC1_E2_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of PIC1_E2_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}} 

void PIC1_E2::CheckFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
    } else {//Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong; frequency unit (Hz) is set as default.\n", unit_in.c_str());
    }
}//}}}

///*********************************
/// Function     : GetTimeResolution
/// Description  : Get time resolution
///*********************************
void PIC1_E2::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///*********************************
/// Function     : SeparateString
/// Description  : Separate some words from a string to store a vector
///*********************************
void PIC1_E2::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

///*********************************
/// Function     : CalculateCLKPostrue
/// Description  : Calculate synchronous time with CLK_HSB
///*********************************
double PIC1_E2::CalculateCLKPosEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time)
{//{{{
    double period_num = 0;
    period_num = (current_time - start_time) / clock_period;
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (is_pos) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        if (((double)mod_period_num + 0.5) < period_num) {
            mod_period_num++;
        }
    }
    double mod_period_num_db = 0;
    if (is_pos) {
        mod_period_num_db = (double)mod_period_num;
    } else {
        mod_period_num_db = (double)mod_period_num + 0.5;
    }
    return ((mod_period_num_db * (double)clock_period) - (current_time - start_time));
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void PIC1_E2::EnableReset (const bool is_active)
{//{{{
    mPIC1Func->EnableReset(is_active);
    if (is_active) {
        for (unsigned int i = 0; i < emmaxchannel; i++) {
            mRisingOSTOutEvent[i].cancel();
            mFallingOSTOutEvent[i].cancel();
            mOutputWriteEvent[i].cancel(); 
        }
        InitializeOutputSignals();
    }
}//}}}

///*********************************
/// Function     : HandleCLK_HSBSignalMethod
/// Description  : Handle CLK_HSB signal
///*********************************
void PIC1_E2::HandleCLK_HSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_HSB.read();
    SetCLKFreq("CLK_HSB", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleResetCmdMethod
/// Description  : Process reset function when sys_reset command is active
///*********************************
void PIC1_E2::HandleResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of sys_resetz.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///*********************************
/// Function     : CancelResetCmdMethod
/// Description  : Cancel reset function when sys_reset command is active
///*********************************
void PIC1_E2::CancelResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of sys_resetz is over.\n");
    this->EnableReset(mIsResetCmdActive); 
}//}}}

///*********************************
/// Function     : HandleSYS_RESETZSignalMethod
/// Description  : Handle sys_resetz signal
///*********************************
void PIC1_E2::HandleSYS_RESETZSignalMethod (void)
{//{{{
    mResetCurVal = sys_resetz.read();
    if (mResetCurVal != mResetPreVal) {//mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        if (mCLK_HSBPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent.notify(CalculateCLKPosEdge((double)mCLK_HSBPeriod, true, current_time,mTimeCLKChangePoint), mTimeResolutionUnit);
        } else {
            mResetHardEvent.notify();
        }
    }
}//}}}

///*********************************
/// Function     : HandleResetHardMethod
/// Description  : Process reset function when sys_reset is active
///*********************************
void PIC1_E2::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset signal sys_resetz is asserted.\n");
        this->EnableReset(mIsResetHardActive);
    } else {
        if (mIsResetHardActive) {
            mIsResetHardActive = false;
            this->EnableReset(mIsResetHardActive);
            re_printf("info","The reset signal sys_resetz is de-asserted.\n");
        }
    }
    //Cancel AssertReset command when sys_resetz is active
    if (mIsResetCmdReceive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

///*********************************
/// Function     : RisingOSTOutSignalsMethod
/// Description  : Activate OSTxTSST output signals
///*********************************
void PIC1_E2::RisingOSTOutSignalsMethod (const unsigned int signal_id)
{//{{{
    if ((!mIsResetCmdActive) && (!mIsResetHardActive)) {
        mOST[signal_id] = true;
        mOutputWriteEvent[signal_id].notify();
        mFallingOSTOutEvent[signal_id].notify(mCLK_HSBPeriod * mOutputPeriod, mTimeResolutionUnit);
     }
}//}}}

///*********************************
/// Function     : FallingOSTOutSignalsMethod
/// Description  : Deactivate OST output signals
///*********************************
void PIC1_E2::FallingOSTOutSignalsMethod (const unsigned int signal_id)
{//{{{
    if ((!mIsResetCmdActive) && (!mIsResetHardActive)) {
        mOST[signal_id] = false;
        mOutputWriteEvent[signal_id].notify();
    }
}//}}}

///*********************************
/// Function     : WriteOutSignalsMethod
/// Description  : Write value to OSTxTSST output signals
///*********************************
void PIC1_E2::WriteOutSignalsMethod (const unsigned int signal_id)
{//{{{
    switch (signal_id){
        case emchannel0:
            OST0TSST.write(mOST[signal_id]);
            break;
        case emchannel1:
            OST1TSST.write(mOST[signal_id]);
            break;
        case emchannel2:
            OST2TSST.write(mOST[signal_id]);
            break;
        case emchannel3:
            OST3TSST.write(mOST[signal_id]);
            break;
        case emchannel4:
            OST4TSST.write(mOST[signal_id]);
            break;
        case emchannel5:
            OST5TSST.write(mOST[signal_id]);
            break;
        case emchannel6:
            OST6TSST.write(mOST[signal_id]);
            break;
        default:
            break;
    }
}//}}}

///*********************************
/// Function     : CheckClockPeriod
/// Description  : Check clock period of CLK_HSB
///                Virtual function of PIC1_E2_AgentController class
///*********************************
bool PIC1_E2::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool status = false;
    if (clock_name == "CLK_HSB") {
        if (mCLK_HSBPeriod > 0) {
            status = true;
        }
    }
    if (!status) {
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return status;
}//}}}

///*********************************
/// Function     : StartSimultaneous
/// Description  : Control OSTxTSST
///                Virtual function of PIC1_E2_AgentController class
///*********************************
void PIC1_E2::StartSimultaneous (const std::string output_channel)
{//{{{
    double cur_time = sc_time_stamp().to_double();
    double clk_hsb_syn_period = CalculateCLKPosEdge((double)mCLK_HSBPeriod, true, cur_time,mTimeCLKChangePoint);
    double rising_period  = clk_hsb_syn_period + (mCLK_HSBPeriod * mOutputDelay);
    if (output_channel == "OST0TSST") {
        mRisingOSTOutEvent[emchannel0].notify(rising_period, mTimeResolutionUnit);
     }
    if (output_channel == "OST1TSST") {
        mRisingOSTOutEvent[emchannel1].notify(rising_period, mTimeResolutionUnit);
     }
    if (output_channel == "OST2TSST") {
        mRisingOSTOutEvent[emchannel2].notify(rising_period, mTimeResolutionUnit);
    }
    if (output_channel == "OST3TSST") {
        mRisingOSTOutEvent[emchannel3].notify(rising_period, mTimeResolutionUnit);
    }
    if (output_channel == "OST4TSST") {
        mRisingOSTOutEvent[emchannel4].notify(rising_period, mTimeResolutionUnit);
    }
    if (output_channel == "OST5TSST") {
        mRisingOSTOutEvent[emchannel5].notify(rising_period, mTimeResolutionUnit);
    }
    if (output_channel == "OST6TSST") {
        mRisingOSTOutEvent[emchannel6].notify(rising_period, mTimeResolutionUnit);
    }
}//}}}
// vim600: set foldmethod=marker :
