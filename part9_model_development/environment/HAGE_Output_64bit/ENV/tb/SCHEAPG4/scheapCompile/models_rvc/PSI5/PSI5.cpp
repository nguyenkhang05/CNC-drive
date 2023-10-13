// ---------------------------------------------------------------------
// $Id: PSI5.cpp,v 1.13 2020/04/23 07:12:11 hadinhtran Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "PSI5.h"
#include "PSI5_Func.h"

///********************************* 
/// Function     : PSI5 
/// Description  : Constructor of PSI5 class
///*********************************
PSI5::PSI5 (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency,
            unsigned short channel_num):
            sc_module(name),
            PSI5_AgentController(),
            BusSlaveBase<BUS_WIDTH_APB,1>(),
            PCLK("PCLK"),
            psi5_com_clk("psi5_com_clk"),
            PRESETn("PRESETn"),
            psi5_com_rst_n("psi5_com_rst_n"),
            I_CONTROL("I_CONTROL"),
            I_DATA("I_DATA"),
            psi5_ts_tick_in("psi5_ts_tick_in"),
            psi5_ts_clr_in("psi5_ts_clr_in"),
            V_DATA("V_DATA"),
            INT_SYNCED("INT_SYNCED"),
            INT_SYNCST("INT_SYNCST"),
            INT_TXDEMPTY("INT_TXDEMPTY"),
            INT_RXDSCNFERR("INT_RXDSCNFERR"),
            INT_RXDERR("INT_RXDERR"),
            INT_RXDFOVF("INT_RXDFOVF"),
            INT_RXDEXIST("INT_RXDEXIST"),
            INT_RXMFOVF("INT_RXMFOVF"),
            INT_RXMERR("INT_RXMERR"),
            INT_RXMEXIST("INT_RXMEXIST"),
            psi5_ts_tick_out("psi5_ts_tick_out"),
            psi5_ts_clr_out("psi5_ts_clr_out")
{//{{{
    //Initialize output signals
    V_DATA.initialize(false);
    INT_SYNCED.initialize(false);
    INT_SYNCST.initialize(false);
    INT_TXDEMPTY.initialize(false);
    INT_RXDSCNFERR.initialize(false);
    INT_RXDERR.initialize(false);
    INT_RXDFOVF.initialize(false);
    INT_RXDEXIST.initialize(false);
    INT_RXMFOVF.initialize(false);
    INT_RXMERR.initialize(false);
    INT_RXMEXIST.initialize(false);
    psi5_ts_tick_out.initialize(0x0);
    psi5_ts_clr_out.initialize(false);

    //Initialize variables
    mPCLKPeriod = 0;
    mPSI5_COM_CLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mPSI5_COM_CLKFreq = 0;
    mPSI5_COM_CLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz";
    mPSI5_COM_CLKFreqUnit = "Hz";
    mStartClkTime[0] = 0;
    mStartClkTime[1] = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mCounter_period_in_slave_mode = 0;
    mCounter_period_in_master_mode = 0;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }
    mDumpInterrupt = false;
    mEnableRegisterMessage  = true;
    for (unsigned int index = 0; index < emInterruptNum; index++) {
        mPreviousIntValue[index] = false;
    }

    Initialize();

    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETn);
    setTargetSocket32((char*)"ts");
    ts = BusSlaveBase<BUS_WIDTH_APB,1>::tSocket32[0]; //Alias for binding

    mPSI5_Func = new PSI5_Func((std::string)name, this);
    sc_assert(mPSI5_Func != NULL);

    mPSI5_Func->SetChannelNumber(channel_num);

    mBusSlaveIf32[0]->setFuncModulePtr(mPSI5_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    mSync_Pulse_Timer = new PSI5_Timer("Sync_Pulse_Timer");
    mSync_Pulse_Timer->SetupGeneralTimer();
    mStamp_Time_Counter = new PSI5_Timer("Stamp_Time_Counter");
    mStamp_Time_Counter->SetupGeneralTimer();
    for (unsigned int index = 0; index < emSlotNum; index++) {
        std::ostringstream slot_name;
        slot_name << "Slot_Timer_" << index;
        mSlot_Timer[index] =  new PSI5_Timer(slot_name.str().c_str());
        mSlot_Timer[index]->SetupGeneralTimer();
    }

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandlePSI5_COM_CLKSignalMethod);
    dont_initialize();
    sensitive << psi5_com_clk;

    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;
    
    SC_METHOD(HandleTimerOvf);
    dont_initialize();
    sensitive << (mStamp_Time_Counter->mOverflowEvent);
    SC_METHOD(StartTimerAfterOVF);
    dont_initialize();
    sensitive << mStartTimerAfterOVF;
    SC_METHOD(HandlePSI5_COM_RST_NSignalMethod);
    sensitive << psi5_com_rst_n;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSI5::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSI5::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSI5::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(HandleSyncPulseTimerCompareMatchMethod);
    dont_initialize();
    sensitive << mSync_Pulse_Timer->mCompareMatchEvent;

    for (unsigned int slot_index = 0; slot_index < emSlotNum; slot_index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mSlot_Timer[slot_index]->mCompareMatchEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSI5::HandleSlotTimerCompareMatchMethod, this, slot_index),
                          sc_core::sc_gen_unique_name("HandleSlotTimerCompareMatchMethod"), &opt);
    }

    SC_METHOD(Handlepsi5_ts_tick_inMethod);
    dont_initialize();
    sensitive << psi5_ts_tick_in;

    SC_METHOD(Handlepsi5_ts_clr_inMethod);
    dont_initialize();
    sensitive << psi5_ts_clr_in.pos();

    SC_METHOD(WriteV_DATAMethod);
    dont_initialize();
    sensitive << mV_DATAWriteEvent;

    SC_METHOD(AssertV_DATAMethod);
    dont_initialize();
    sensitive << mAssertV_DATAEvent;

    SC_METHOD(DeAssertV_DATAMethod);
    dont_initialize();
    sensitive << mDeAssertV_DATAEvent;

    SC_METHOD(Writepsi5_ts_clr_outMethod);
    dont_initialize();
    sensitive << mpsi5_ts_clr_outWriteEvent;

    SC_METHOD(Assertpsi5_ts_clr_outMethod);
    dont_initialize();
    sensitive << mAssertpsi5_ts_clr_outEvent;

    SC_METHOD(DeAssertpsi5_ts_clr_outMethod);
    dont_initialize();
    sensitive << mDeAssertpsi5_ts_clr_outEvent;

    SC_METHOD(Writepsi5_ts_tick_outMethod);
    dont_initialize();
    sensitive << mpsi5_ts_tick_outWriteEvent;

    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteInterruptPortsEvent[interrupt_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSI5::WriteInterruptPortsMethod, this, interrupt_id),
                          sc_core::sc_gen_unique_name("WriteInterruptPortsMethod"), &opt);
    }

    SC_THREAD(SyncPulseThread);
    dont_initialize();
    sensitive << mSyncPulseEvent;

    SC_THREAD(TransmissionThread);
    dont_initialize();
    sensitive << mTransmissionEvent;

    for (unsigned int slot_index = 0; slot_index < emSlotNum; slot_index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcessReceptionDataEvent[slot_index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSI5::ProcessReceptionDataMethod, this, slot_index),
                          sc_core::sc_gen_unique_name("ProcessReceptionDataMethod"), &opt);
    }

    SC_THREAD(AsyncReceptionThread);
    dont_initialize();
    sensitive << I_CONTROL;
}//}}}

///*********************************
/// Function     : ~PSI5 
/// Description  : Destructor of PSI5 class
///*********************************
PSI5::~PSI5 (void)
{//{{{
    delete mPSI5_Func;
    delete mSync_Pulse_Timer;
    delete mStamp_Time_Counter;
    for (unsigned int index = 0; index < emSlotNum; index++) {
        delete mSlot_Timer[index];
    }
}//}}}

///*********************************
/// Function     : SetMessageLevel 
/// Description  : Set message level (fatal/error/warning/info)
///*********************************
void PSI5::SetMessageLevel (const std::string msg_lv)
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
        mPSI5_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        mPSI5_Func->RegisterHandler(cmd);
    }
    else {
        // Disable message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mPSI5_Func->RegisterHandler(cmd);

        // Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mPSI5_Func->RegisterHandler(cmd);
    }
}//}}}

///*********************************
/// Function     : DumpRegisterRW 
/// Description  : Enable/disable dumping message when users access registers
///*********************************
void PSI5::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mPSI5_Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : DumpInterrupt 
/// Description  : Enable/disable dumping interrupt message
///*********************************
void PSI5::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_enable);
    if (temp.size() == 1) {
        if (temp[0] == "true") {
            mDumpInterrupt = true;
        } else if (temp[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: PSI5_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (temp.size() == 0){
//        std::string temp_arg = (mDumpInterrupt) ? "true" : "false";
          std::string temp_arg = "";
          if(mDumpInterrupt == true){
              temp_arg = "true";
          }else{
              temp_arg = "false";
          }
        re_printf("info","PSI5_DumpInterrupt     %s\n",temp_arg.c_str());
    } else {
        re_printf("warning","Invalid argument: PSI5_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : AssertReset 
/// Description  : Assert reset by software for PRESETn, psi5_com_rst_n
///*********************************
void PSI5::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPresetn])&&(!mIsResetCmdReceive[emPresetn])) {
            mIsResetCmdReceive[emPresetn] = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPresetn].notify(start_time, SC_NS);
            mResetCmdPeriod[emPresetn] = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else if (reset_name == "psi5_com_rst_n") {
        if ((!mIsResetHardActive[emPsi5_com_rst_n])&&(!mIsResetCmdReceive[emPsi5_com_rst_n])) {
            mIsResetCmdReceive[emPsi5_com_rst_n] = true;
            re_printf("info","The model will be reset (psi5_com_rst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPsi5_com_rst_n].notify(start_time, SC_NS);
            mResetCmdPeriod[emPsi5_com_rst_n] = period;
        } else {
            re_printf("warning","The software reset of psi5_com_rst_n is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning", "The name of reset signal is wrong. It should be PRESETn or psi5_com_rst_n.\n");
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq 
/// Description  : Set clock value and clock unit for PCLK, psi5_com_clk
///*********************************
void PSI5::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            CheckFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            mPCLKOrgFreq = freq;
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[0] = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                mSyncPulseEvent.cancel();
                mTransmissionEvent.cancel();
                for (unsigned char index = 0; index < emSlotNum; index++) {
                    mProcessReceptionDataEvent[index].cancel();
                } 
                mV_DATAWriteEvent.cancel();
                mAssertV_DATAEvent.cancel();
                mDeAssertV_DATAEvent.cancel();
                mpsi5_ts_clr_outWriteEvent.cancel();
                mAssertpsi5_ts_clr_outEvent.cancel();
                mDeAssertpsi5_ts_clr_outEvent.cancel();
                for (unsigned char index = 0x0; index < emInterruptNum; index++) {
                    mWriteInterruptPortsEvent[index].cancel();
                }
                mSync_Pulse_Timer->StartStop(false);
                mStamp_Time_Counter->StartStop(false);
                for (unsigned int index = 0; index < emSlotNum; index++) {
                    mSlot_Timer[index]->StartStop(false);
                }
            }
        } else if (word_vector[0] == "psi5_com_clk") {
            CheckFreq (mPSI5_COM_CLKFreq, mPSI5_COM_CLKFreqUnit, freq, unit);
            mPSI5_COM_CLKOrgFreq = freq;
            if (mPSI5_COM_CLKFreq > 0) {
                mPSI5_COM_CLKPeriod = (sc_dt::uint64)(((1/(double)mPSI5_COM_CLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[1] = sc_time_stamp().to_double();
            } else {
                mPSI5_COM_CLKPeriod = 0;
                mSyncPulseEvent.cancel();
                mTransmissionEvent.cancel();
                for (unsigned char index = 0; index < emSlotNum; index++) {
                    mProcessReceptionDataEvent[index].cancel();
                } 
                mV_DATAWriteEvent.cancel();
                mAssertV_DATAEvent.cancel();
                mDeAssertV_DATAEvent.cancel();
                mpsi5_ts_clr_outWriteEvent.cancel();
                mAssertpsi5_ts_clr_outEvent.cancel();
                mDeAssertpsi5_ts_clr_outEvent.cancel();
                for (unsigned char index = 0; index < emInterruptNum; index++) {
                    mWriteInterruptPortsEvent[index].cancel();
                }
                mSync_Pulse_Timer->StartStop(false);
                mStamp_Time_Counter->StartStop(false);
                for (unsigned int index = 0; index < emSlotNum; index++) {
                    mSlot_Timer[index]->StartStop(false);
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
/// Description  : Get clock value of PCLK, psi5_com_clk
///*********************************
void PSI5::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "psi5_com_clk") {
            re_printf("info","psi5_com_clk frequency is %0.0f %s.\n", (double)mPSI5_COM_CLKOrgFreq, mPSI5_COM_CLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ForceRegister 
/// Description  : Force value to register
///*********************************
void PSI5::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mPSI5_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReleaseRegister 
/// Description  : Release forced value after forcing registers
///*********************************
void PSI5::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mPSI5_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : WriteRegister 
/// Description  : Write value to registers
///*********************************
void PSI5::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mPSI5_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReadRegister 
/// Description  : Read value of register
///*********************************
void PSI5::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mPSI5_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}
void PSI5::EnableRegisterMessage(const std::string is_enable)
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
            re_printf("warning", "Invalid argument: PSI5_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } 
    else if (word_vector.size() == 0){
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info", "PSI5_EnableRegisterMessage %s\n", temp.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: PSI5_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}
///*********************************
/// Function     : ListRegister 
/// Description  : List all registers name
///*********************************
void PSI5::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mPSI5_Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : Help 
/// Description  : Dump help message of all parameters or commands
///*********************************
void PSI5::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"--- parameters ---");
            SC_REPORT_INFO(this->basename(),"PSI5_MessageLevel   (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"PSI5_DumpRegisterRW (true/false)                     Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"PSI5_DumpInterrupt  (true/false)                     Enable/disable dumping interrupt message (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"--- commands ---");
            SC_REPORT_INFO(this->basename(),"PSI5_AssertReset    (reset_name, start_time, period) Assert and de-assert reset signal (PRESETn or psi5_com_rst_n).");
            SC_REPORT_INFO(this->basename(),"PSI5_SetCLKFreq     (clock_name, freq, unit)         Set clock frequency to PCLK, psi5_com_clk clock.");
            SC_REPORT_INFO(this->basename(),"PSI5_GetCLKFreq     (clock_name)                     Get clock frequency of PCLK, psi5_com_clk clock.");
            SC_REPORT_INFO(this->basename(),"PSI5_ForceRegister  (reg_name, value)                Force register with setting value.");
            SC_REPORT_INFO(this->basename(),"PSI5_ReleaseRegister(reg_name)                       Release register from force value.");
            SC_REPORT_INFO(this->basename(),"PSI5_WriteRegister  (reg_name, value)                Write the value to register.");
            SC_REPORT_INFO(this->basename(),"PSI5_ReadRegister   (reg_name)                       Read value from register.");
            SC_REPORT_INFO(this->basename(),"PSI5_ListRegister                                    Dump name of PSI5's registers.");
        } else {
            re_printf("warning","The name (%s) of PSI5_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of PSI5_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///*********************************
/// Function     : HandlePCLKSignalMethod 
/// Description  : Handle PCLK signal
///*********************************
void PSI5::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandlePSI5_COM_CLKSignalMethod 
/// Description  : Handle psi5_com_clk signal
///*********************************
void PSI5::HandlePSI5_COM_CLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = psi5_com_clk.read();
    SetCLKFreq("psi5_com_clk", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleResetCmdMethod 
/// Description  : Process reset function when PRESETn or psi5_com_rst_n command is active
///*********************************
void PSI5::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == 0) {
        reset_name = "PRESETn";
    } else { //reset_id == 1
        reset_name = "psi5_com_rst_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///*********************************
/// Function     : CancelResetCmdMethod 
/// Description  : Cancel reset function when PRESETn or psi5_com_rst_n command is active
///*********************************
void PSI5::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == 0) {
        reset_name = "PRESETn";
    } else { //reset_id == 1
        reset_name = "psi5_com_rst_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id); 
}//}}}

///*********************************
/// Function     : HandlePRESETnSignalMethod 
/// Description  : Handle PRESETn signal
///*********************************
void PSI5::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[emPresetn] = PRESETn.read();
    if (mResetCurVal[emPresetn] != mResetPreVal[emPresetn]) {//mResetPreVal is "true" at initial state
        mResetPreVal[emPresetn] = mResetCurVal[emPresetn];
        if (mPCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent[emPresetn].notify(CalculateCLKPosEdge("PCLK", current_time - mStartClkTime[0]), mTimeResolutionUnit);
        } else {
            mResetHardEvent[emPresetn].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///*********************************
/// Function     : HandlePSI5_COM_RST_NSignalMethod 
/// Description  : Handle psi5_com_rst_n signal
///*********************************
void PSI5::HandlePSI5_COM_RST_NSignalMethod (void)
{//{{{
    mResetCurVal[emPsi5_com_rst_n] = psi5_com_rst_n.read();
    if (mResetCurVal[emPsi5_com_rst_n] != mResetPreVal[emPsi5_com_rst_n]) {//mResetPreVal is "true" at initial state
        mResetPreVal[emPsi5_com_rst_n] = mResetCurVal[emPsi5_com_rst_n];
        if (mPSI5_COM_CLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent[emPsi5_com_rst_n].notify(CalculateCLKPosEdge("psi5_com_clk", current_time - mStartClkTime[1]), mTimeResolutionUnit);
        } else {
            mResetHardEvent[emPsi5_com_rst_n].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///*********************************
/// Function     : HandleResetHardMethod 
/// Description  : Process reset function when PRESETn or psi5_com_rst_n is active
///*********************************
void PSI5::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset signal PRESETn is asserted.\n");
        } else {
            re_printf("info","The reset signal psi5_com_rst_n is asserted.\n");
        }
        this->EnableReset(mIsResetHardActive[reset_id], reset_id);
    } else {
        mIsResetHardActive[reset_id] = false;
        this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        if (reset_id == 0) {
            re_printf("info","The reset signal PRESETn is de-asserted.\n");
        } else {
            re_printf("info","The reset signal psi5_com_rst_n is de-asserted.\n");
        }
    }
    //Cancel AssertReset command when PRESETn is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

///*********************************
/// Function     : HandleSyncPulseTimerCompareMatchMethod 
/// Description  : Handle compare match event of Sync Pulse Timer
///*********************************
void PSI5::HandleSyncPulseTimerCompareMatchMethod (void)
{//{{{
    //When transmission is completed and communication mode is different from Variable Time Triggered Sync Mode
    if (mIsSyncPulseSending && (mPSI5_Func->GetCommunicationMode() != 0x4)) {
        SendSyncPulse(true);
    } else if (!mIsSyncPulseSending) {
        double current_time = sc_time_stamp().to_double();
        mTransmissionEvent.notify(CalculateCLKPosEdge("psi5_com_clk", current_time - mStartClkTime[1]), mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : HandleSlotTimerCompareMatchMethod 
/// Description  : Handle compare match event of Slot Timer
///*********************************
void PSI5::HandleSlotTimerCompareMatchMethod (const unsigned int slot_index)
{//{{{
    unsigned int i_control_value = I_CONTROL.read();
    if (((i_control_value>>6)&0x3) != 0x2) {//Strobe bit is wrong
        mI_CONTROLValue = i_control_value;
        mI_DATAValue = I_DATA.read();
        if (((mI_CONTROLValue>>6)&0x3) != 0x0) {//Reception is ready
            re_printf("info","Begin to receive the data from slot %d\n",slot_index+1);
            mIsSlotReceptionCompleted[slot_index] = false;
            mProcessReceptionDataEvent[slot_index].notify(mPSI5_Func->CalculateReceptionBitsTotalTime(slot_index), mTimeResolutionUnit);
        } else {
            mIsSlotReceptionCompleted[slot_index] = true;
        }
    } else {
        re_printf("warning","Strobe bit of I_CONTROL port should be different from value 0x2\n");
    }
}//}}}

///*********************************
/// Function     : Handlepsi5_ts_tick_inMethod 
/// Description  : Handle psi5_ts_tick_in port
///*********************************
void PSI5::Handlepsi5_ts_tick_inMethod (void)
{//{{{
    sc_dt::uint64 value = psi5_ts_tick_in.read();
    if ((value == 0x0)&&(!(mPSI5_Func->IsMasterModeTimer()))) {//Stop timer
        StartStopTimer(emStampTimeCounter, false);
        unsigned int cnt_value = 0;
        cnt_value = GetStampTimeCounterValue();
        mPSI5_Func->UpdateIPTIMERValue(cnt_value);
    } else {//Start timer
        if (mPSI5_Func->CheckTimerEnable() && (!(mPSI5_Func->IsMasterModeTimer()))){
            sc_dt::uint64 counter_period = (sc_dt::uint64)(((1/(double)value)*(double)mTimeResolutionValue) + 0.5);
            mCounter_period_in_slave_mode = (double)counter_period;
            SetTimerCounterIntialValue(emStampTimeCounter, mPSI5_Func->GetIPTIMERValue());
            SetTimerCounterPeriod(emStampTimeCounter, mCounter_period_in_slave_mode);
            SetTimerCounterMode(emStampTimeCounter, true);//free_run mode
            StartStopTimer(emStampTimeCounter, true);
        }
    }
}//}}}

///*********************************
/// Function     : Handlepsi5_ts_clr_inMethod 
/// Description  : Handle psi5_ts_clr_in port
///*********************************
void PSI5::Handlepsi5_ts_clr_inMethod (void)
{//{{{
    if (!(mPSI5_Func->IsMasterModeTimer())) {
        SetTimerCounterIntialValue(emStampTimeCounter, 0x0);
        mPSI5_Func->UpdateIPTIMERValue(0x0);
    }
}//}}}

///*********************************
/// Function     : WriteV_DATAMethod 
/// Description  : Control V_DATA port
///*********************************
void PSI5::WriteV_DATAMethod (void)
{//{{{
    V_DATA.write(mV_DATAValue);
}//}}}

///*********************************
/// Function     : AssertV_DATAMethod 
/// Description  : Control V_DATA port
///*********************************
void PSI5::AssertV_DATAMethod (void)
{//{{{
    mV_DATAValue = true;
    mV_DATAWriteEvent.notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : DeAssertV_DATAMethod 
/// Description  : Control V_DATA port
///*********************************
void PSI5::DeAssertV_DATAMethod (void)
{//{{{
    mV_DATAValue = false;
    mV_DATAWriteEvent.notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : Writepsi5_ts_clr_outMethod 
/// Description  : Control psi5_ts_clr_out port
///*********************************
void PSI5::Writepsi5_ts_clr_outMethod (void)
{//{{{
    psi5_ts_clr_out.write(mpsi5_ts_clr_outValue);
}//}}}

///*********************************
/// Function     : Assertpsi5_ts_clr_outMethod 
/// Description  : Control psi5_ts_clr_out port
///*********************************
void PSI5::Assertpsi5_ts_clr_outMethod (void)
{//{{{
    mpsi5_ts_clr_outValue = true;
    mpsi5_ts_clr_outWriteEvent.notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : DeAssertpsi5_ts_clr_outMethod 
/// Description  : Control psi5_ts_clr_out port
///*********************************
void PSI5::DeAssertpsi5_ts_clr_outMethod (void)
{//{{{
    mpsi5_ts_clr_outValue = false;
    mpsi5_ts_clr_outWriteEvent.notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : Writepsi5_ts_tick_outMethod 
/// Description  : Control psi5_ts_tick_out port
///*********************************
void PSI5::Writepsi5_ts_tick_outMethod (void)
{//{{{
    psi5_ts_tick_out.write(mCountFreq);
}//}}}

///*********************************
/// Function     : SyncPulseThread 
/// Description  : Control synchronous pulse
///*********************************
void PSI5::SyncPulseThread (void)
{//{{{
    for(;;) {
        if(mIsResetHardActive[emPsi5_com_rst_n]||mIsResetCmdActive[emPsi5_com_rst_n]||!mPSI5_Func->CheckChannelEnable()) {
            wait();
            continue;
        }
        try {
            while (true) {
                double current_time = sc_time_stamp().to_double();
                mAssertV_DATAEvent.notify(SC_ZERO_TIME);//Assert V_DATA port
                mPSI5_Func->ProcessSyncPulseStart();
                if ((!mIsSlotReceptionCompleted[emSlot1]) || (!mIsSlotReceptionCompleted[emSlot2]) 
                  ||(!mIsSlotReceptionCompleted[emSlot3]) || (!mIsSlotReceptionCompleted[emSlot4])
                  ||(!mIsSlotReceptionCompleted[emSlot5]) || (!mIsSlotReceptionCompleted[emSlot6])
                  ||(!mIsSlotReceptionCompleted[emSlot7]) || (!mIsSlotReceptionCompleted[emSlot8])) {
                    mPSI5_Func->SendSyncPulseCollision(); 
                }
                StartSlotTimer();
                double pulse_width = 0;
                if (mPSI5_Func->GetSyncPulseMode()||mPSI5_Func->GetSyncPulseDefault()) {//Tooth Gap method or zero default in Pulse Width method
                    pulse_width = mPSI5_Func->CalculateSyncPulseWidth(true); 
                } else {//Pulse Width
                    pulse_width = mPSI5_Func->CalculateSyncPulseWidth(false); 
                }
                wait(pulse_width, mTimeResolutionUnit, mResetHardEvent[1]|mResetCmdEvent[1]|mDisableChannelEvent);
                if (mIsResetHardActive[emPsi5_com_rst_n]||mIsResetCmdActive[emPsi5_com_rst_n]||!mPSI5_Func->CheckChannelEnable()) {
                    throw vpcl::emRstException;
                }
                mDeAssertV_DATAEvent.notify(SC_ZERO_TIME);//Deassert V_DATA port
                mPSI5_Func->ProcessSyncPulseEnd();
                wait();
                if (mIsResetHardActive[emPsi5_com_rst_n]||mIsResetCmdActive[emPsi5_com_rst_n]||!mPSI5_Func->CheckChannelEnable()) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

///*********************************
/// Function     : TransmissionThread 
/// Description  : Send the data frame
///*********************************
void PSI5::TransmissionThread (void)
{//{{{
    for(;;) {
        if(mIsResetHardActive[emPsi5_com_rst_n]||mIsResetCmdActive[emPsi5_com_rst_n]||!mPSI5_Func->CheckChannelEnable()) {
            wait();
            continue;
        }
        try {
            while (true) {
                if (mTxBitPos != mTxBitTotalNum) {
                    bool bit_value = (bool)((mTxDataFrame>>mTxBitPos)&0x1);
                    mTxBitPos = mTxBitPos + 1;
                    if (mPSI5_Func->GetSyncPulseMode()||bit_value) {
                        mAssertV_DATAEvent.notify(SC_ZERO_TIME);//Assert V_DATA port
                    } else {
                        mDeAssertV_DATAEvent.notify(SC_ZERO_TIME);//Not assert V_DATA port
                    }
                    mPSI5_Func->ProcessSyncPulseStart();
                    wait(mPSI5_Func->CalculateSyncPulseWidth(bit_value),mTimeResolutionUnit,mResetHardEvent[1]|mResetCmdEvent[1]|mDisableChannelEvent);
                    if (mIsResetHardActive[emPsi5_com_rst_n]||mIsResetCmdActive[emPsi5_com_rst_n]||!mPSI5_Func->CheckChannelEnable()) {
                        throw vpcl::emRstException;
                    }
                    mDeAssertV_DATAEvent.notify(SC_ZERO_TIME);//Deassert V_DATA port
                    mPSI5_Func->ProcessSyncPulseEnd();
                } else {//Transmission is completed
                    re_printf("info","The transmission is completed\n");
                    mPSI5_Func->UpdateCompletedTransmissionStatus();
                    mTxBitPos = 0x0;
                    mIsSyncPulseSending = true;
                }
                wait();
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

///*********************************
/// Function     : ProcessReceptionDataMethod 
/// Description  : Receive the data according to time slot
///*********************************
void PSI5::ProcessReceptionDataMethod (const unsigned int slot_index)
{//{{{
    unsigned int i_control_value = I_CONTROL.read();
    if (((i_control_value>>6)&0x3) == 0x0) {//Strobe bit is changed to value 0
        re_printf("warning","Should not change strobe bit of I_CONTROL port during reception of the model\n");
    }
    mPSI5_Func->ProcessReceptionData(mI_CONTROLValue, mI_DATAValue, slot_index);
    mIsSlotReceptionCompleted[slot_index] = true;
}//}}}

///*********************************
/// Function     : AsyncReceptionThread 
/// Description  : Receive the data in asynchronous mode
///*********************************
void PSI5::AsyncReceptionThread (void)
{//{{{
    for(;;) {
        if(mIsResetHardActive[emPsi5_com_rst_n]||mIsResetCmdActive[emPsi5_com_rst_n]||!mPSI5_Func->CheckChannelEnable()) {
            wait();
            continue;
        }
        try {
            while (true) {
                unsigned int i_control_value = I_CONTROL.read();
                unsigned int i_data_value = I_DATA.read();
                if ((mPSI5_Func->GetCommunicationMode() != 0x1) && (mPSI5_Func->GetCommunicationMode() != 0x2)
                   && (mPSI5_Func->GetCommunicationMode() != 0x3) && (mPSI5_Func->GetCommunicationMode() != 0x4)) {
                    if (((i_control_value>>6)&0x3) != 0x2) {
                        if (((i_control_value>>6)&0x3) != 0x0) {//Reception is ready
                            if (mPSI5_Func->CheckSlotEnable(emSlot1)) {//If slot 1 is enable
                                re_printf("info","Begin to receive the data from slot %d\n",emSlot1+1);
                                wait(mPSI5_Func->CalculateReceptionBitsTotalTime(emSlot1),mTimeResolutionUnit,mResetHardEvent[1]|mResetCmdEvent[1]|mDisableChannelEvent);
                                if (mIsResetHardActive[emPsi5_com_rst_n]||mIsResetCmdActive[emPsi5_com_rst_n]||!mPSI5_Func->CheckChannelEnable()) {
                                    throw vpcl::emRstException;
                                }
                                unsigned int i_control_value_temp = I_CONTROL.read();
                                if (((i_control_value_temp>>6)&0x3) == 0x0) {//Strobe bit is changed to value 0
                                    re_printf("warning","Should not change strobe bit of I_CONTROL port during reception of the model\n");
                                }
                                mPSI5_Func->ProcessReceptionData(i_control_value, i_data_value, emSlot1);
                            }
                        }
                    } else {
                        re_printf("warning","Strobe bit of I_CONTROL port should be different from value 0x2\n");
                    }
                }
                wait();
                if (mIsResetHardActive[emPsi5_com_rst_n]||mIsResetCmdActive[emPsi5_com_rst_n]||!mPSI5_Func->CheckChannelEnable()) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

///*********************************
/// Function     : Initialize 
/// Description  : Initialize variables at initial state or when reset is active
///*********************************
void PSI5::Initialize (void)
{//{{{
    mIsSyncPulseSending = true;
    mTxDataFrame = 0x0;
    mTxBitTotalNum = 0x0;
    mTxBitPos = 0x0;
    for (unsigned int index = 0; index < emSlotNum; index++) {
        mIsSlotReceptionCompleted[index] = true;
    }
    mCountFreq = 0x0;
    mI_CONTROLValue = 0x0;
    mI_DATAValue = 0x0;
    mIsActiveInt = false;
    mV_DATAValue = false;
    mpsi5_ts_clr_outValue = false;
}//}}}

///*********************************
/// Function     : CheckFreq 
/// Description  : Check frequency value and frequency unit
///*********************************
void PSI5::CheckFreq (sc_dt::uint64 &freq_out
                     ,std::string &unit_out
                     ,sc_dt::uint64 freq_in
                     ,std::string unit_in)
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
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

///*********************************
/// Function     : CalculateCLKPosEdge 
/// Description  : Calculate synchronous time with PCLK or psi5_com_clk
///*********************************
double PSI5::CalculateCLKPosEdge (const std::string clock_name, const double active_period)
{//{{{
    //Calculate the time point of next raising edge of PCLK
    double period_num = 0;
    if (clock_name == "PCLK") {
        period_num = active_period / (double)mPCLKPeriod;
    } else {//psi5_com_clk
        period_num = active_period / (double)mPSI5_COM_CLKPeriod;
    }
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "PCLK") {
        pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - active_period;
    } else {//psi5_com_clk
        pos_edge_point = (double)mod_period_num * (double)mPSI5_COM_CLKPeriod - active_period;
    }
    return pos_edge_point;
}//}}}

///*********************************
/// Function     : EnableReset 
/// Description  : Process reset function
///*********************************
void PSI5::EnableReset (const bool is_active, const unsigned int reset_id)
{//{{{
    if (is_active) {
        mSync_Pulse_Timer->EnableReset(is_active);
        mStamp_Time_Counter->EnableReset(is_active);
        for (unsigned int index = 0; index < emSlotNum; index++) {
            mSlot_Timer[index]->EnableReset(is_active);
        }
        mV_DATAWriteEvent.cancel();
        mAssertV_DATAEvent.cancel();
        mDeAssertV_DATAEvent.cancel();
        mDeAssertV_DATAEvent.notify(SC_ZERO_TIME);//Initialize V_DATA port
        mCountFreq = 0;
        mpsi5_ts_tick_outWriteEvent.notify(SC_ZERO_TIME);//Initialize psi5_tx_tick_out port
        for (unsigned char index = 0; index < emInterruptNum; index++) {
            mWriteInterruptPortsEvent[index].cancel();
            mIsActiveInt = false;
            mWriteInterruptPortsEvent[index].notify(SC_ZERO_TIME);//Initialize interrupt ports
        }
    } else {
        mSync_Pulse_Timer->EnableReset(is_active);
        mStamp_Time_Counter->EnableReset(is_active);
        mSync_Pulse_Timer->SetupGeneralTimer();
        mStamp_Time_Counter->SetupGeneralTimer();
        for (unsigned int index = 0; index < emSlotNum; index++) {
            mSlot_Timer[index]->EnableReset(is_active);
            mSlot_Timer[index]->SetupGeneralTimer();
        }
    }
    if (reset_id == 0) {
        mPSI5_Func->EnableReset(is_active, "PCLK_domain");
        if(is_active) {
            mDisableChannelEvent.notify();
        }
    } else {
        mPSI5_Func->EnableReset(is_active, "psi5_com_clk_domain");
    }
    if (is_active) {
        Initialize();
        mSyncPulseEvent.cancel();
        mTransmissionEvent.cancel();
        for (unsigned char index = 0; index < emSlotNum; index++) {
            mProcessReceptionDataEvent[index].cancel();
        } 
        mpsi5_ts_clr_outWriteEvent.cancel();
        mAssertpsi5_ts_clr_outEvent.cancel();
        mDeAssertpsi5_ts_clr_outEvent.cancel();
        mDeAssertpsi5_ts_clr_outEvent.notify(SC_ZERO_TIME);//Initialize psi5_tx_clr_out port
    }
}//}}}

///*********************************
/// Function     : WriteInterruptPortsMethod 
/// Description  : Activate/deactivate interrupt ports
///*********************************
void PSI5::WriteInterruptPortsMethod (const unsigned int interrupt_id)
{//{{{
    std::string interrupt_name = "";
    bool is_port_changed = false;
    bool value  = mIsActiveInt;
    switch (interrupt_id) {
        case emINT_SYNCED: //int_synced
            interrupt_name = "INT_SYNCED";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_SYNCED.write(value);
            }
            break;
        case emINT_SYNCST: //int_syncst
            interrupt_name = "INT_SYNCST";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_SYNCST.write(value);
            }
            break;
        case emINT_TXDEMPTY: //int_txdempty
            interrupt_name = "INT_TXDEMPTY";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_TXDEMPTY.write(value);
            }
            break;
        case emINT_RXDSCNFERR: //int_rxdscnferr
            interrupt_name = "INT_RXDSCNFERR";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_RXDSCNFERR.write(value);
            }
            break;
        case emINT_RXDERR: //int_rxderr
            interrupt_name = "INT_RXDERR";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_RXDERR.write(value);
            }
            break;
        case emINT_RXDFOVF: //int_rxdfovf
            interrupt_name = "INT_RXDFOVF";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_RXDFOVF.write(value);
            }
            break;
        case emINT_RXDEXIST: //int_rxdexist
            interrupt_name = "INT_RXDEXIST";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_RXDEXIST.write(value);
            }
            break;
        case emINT_RXMFOVF: //int_rxmfovf
            interrupt_name = "INT_RXMFOVF";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_RXMFOVF.write(value);
            }
            break;
        case emINT_RXMERR: //int_rxmerr
            interrupt_name = "INT_RXMERR";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_RXMERR.write(value);
            }
            break;
        default: //int_rxmexist
            interrupt_name = "INT_RXMEXIST";
            if (value != mPreviousIntValue[interrupt_id]) {
                mPreviousIntValue[interrupt_id] = value;
                is_port_changed = true;
                INT_RXMEXIST.write(value);
            }
    }
    if (mDumpInterrupt && is_port_changed) {
        if (value) {
            re_printf("info","[INT][%s] Assert\n",interrupt_name.c_str());
        } else {
            re_printf("info","[INT][%s] De-assert\n",interrupt_name.c_str());
        }
    }
}//}}}

///*********************************
/// Function     : PrepareTxDataFrame 
/// Description  : Prepare TX data frame
///*********************************
void PSI5::PrepareTxDataFrame (const unsigned int data_format, const unsigned int data)
{//{{{
    sc_dt::uint64 crc_code = 0x0;
    sc_dt::uint64 data_temp = (sc_dt::uint64)data;
    switch (data_format) {
        case emLongFrame:
            crc_code = (sc_dt::uint64)(mPSI5_Func->CalculateCRC(data,16,3));
            mTxDataFrame = 0x2|(0x1<<3)|((data_temp&0x7)<<4)|(0x1<<7)|((data_temp&0x38)<<5)|(0x1<<11)
                        |((data_temp&0x1C0)<<6)|(0x1<<15)|((data_temp&0xE00)<<7)|(0x1<<19)
                        |((data_temp&0x7000)<<8)|(0x1<<23)|((data_temp&0x8000)<<9)|((crc_code&0x4)<<23)
                        |((crc_code&0x2)<<25)|(0x1<<27)|((crc_code&0x1)<<28);
            mTxBitTotalNum = 29;
            break;
        case emXLongFrame:
            crc_code = (sc_dt::uint64)(mPSI5_Func->CalculateCRC(data,22,3));
            mTxDataFrame = 0x2|(0x1<<3)|((data_temp&0x7)<<4)|(0x1<<7)|((data_temp&0x38)<<5)|(0x1<<11)
                        |((data_temp&0x1C0)<<6)|(0x1<<15)|((data_temp&0xE00)<<7)|(0x1<<19)
                        |((data_temp&0x7000)<<8)|(0x1<<23)|((data_temp&0x38000)<<9)|(0x1<<27)
                        |((data_temp&0x1C0000)<<10)|(0x1ULL<<31)|((data_temp&0x200000)<<11)|((crc_code&0x4)<<31)
                        |((crc_code&0x2)<<33)|(0x1ULL<<35)|((crc_code&0x1)<<36);
            mTxBitTotalNum = 37;
            break;
        case emXXLongFrame:
            crc_code = (sc_dt::uint64)(mPSI5_Func->CalculateCRC(data,24,6));
            mTxDataFrame = 0x0FE|((data_temp&0x3F)<<9)|((data_temp&0xFC0)<<10)|((data_temp&0x3F000)<<11)
                        |((data_temp&0xFC0000)<<12)|(crc_code<<37);
            mTxBitTotalNum = 43;
            break;
        default: //Short frame
            crc_code = (sc_dt::uint64)(mPSI5_Func->CalculateCRC(data,6,3));
            mTxDataFrame = 0x2|(0x1<<3)|((data_temp&0x7)<<4)|(0x1<<7)|((data_temp&0x38)<<5)|(0x1<<11)
                        |((crc_code&0x4)<<10)|((crc_code&0x2)<<12)|((crc_code&0x1)<<14);
            mTxBitTotalNum = 15;
    }
}//}}}

///*********************************
/// Function     : StartSlotTimer 
/// Description  : start timer for each slot
///*********************************
void PSI5::StartSlotTimer (void)
{//{{{
    for (unsigned int slot_index = 0; slot_index < emSlotNum; slot_index++) {
        if (mPSI5_Func->CheckSlotEnable(slot_index)) {
            mPSI5_Func->StartSlotTimer(slot_index);
        }
    }
}//}}}

///Virtual function of PSI5_AgentController class
///*********************************
/// Function     : GetResetStatus 
/// Description  : Get reset status
///*********************************
bool PSI5::GetResetStatus (void)
{//{{{
    return mIsResetHardActive[1];
}//}}}
bool PSI5::GetPresetnStatus(void)
{//{{{
    return (mIsResetHardActive[0] || mIsResetCmdActive[0]);
}//}}}
///*********************************
/// Function     : CheckClockPeriod 
/// Description  : Check clock period of PCLK, psi5_com_clk
///*********************************
bool PSI5::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool status = false;
    if (clock_name == "PCLK") {
        if (mPCLKPeriod > 0) {
            status = true;
        }
    } else {//psi5_com_clk
        if (mPSI5_COM_CLKPeriod > 0) {
            status = true;
        }
    }
    if (!status) {
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return status;
}//}}}

///*********************************
/// Function     : GetCommunicationPeriod 
/// Description  : Get communication period
///*********************************
sc_dt::uint64 PSI5::GetCommunicationPeriod (void)
{//{{{
    return mPSI5_COM_CLKPeriod; 
}//}}}

///*********************************
/// Function     : SendSyncPulse 
/// Description  : Send a sync pulse via V_DATA port
///*********************************
void PSI5::SendSyncPulse (const bool is_sync_mode
                         ,const unsigned int sync_pulse_default_value)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if (is_sync_mode) {
        mSyncPulseEvent.notify(CalculateCLKPosEdge("psi5_com_clk", current_time - mStartClkTime[1]), mTimeResolutionUnit);
    } else {
        if (sync_pulse_default_value == 0x0) {//Sending 0 as default of sync pulse
            mDeAssertV_DATAEvent.notify(CalculateCLKPosEdge("psi5_com_clk", current_time - mStartClkTime[1]), mTimeResolutionUnit);
        } else {//Sending 1 as default of sync pulse
            mAssertV_DATAEvent.notify(CalculateCLKPosEdge("psi5_com_clk", current_time - mStartClkTime[1]), mTimeResolutionUnit);
        }
    }
}//}}}

///*********************************
/// Function     : StartStopTimer 
/// Description  : Start/stop timer
///*********************************
void PSI5::StartStopTimer (const unsigned char timer_type, const bool is_start)
{//{{{
    if (timer_type == emSyncPulseTimer) {//Sync pulse timer
        mSync_Pulse_Timer->StartStop(is_start);
    } else if ((timer_type != emSyncPulseTimer) && (timer_type != emStampTimeCounter)) {//Slot timer
        mSlot_Timer[timer_type]->StartStop(is_start);
    } else {//Stamp time counter
        mStamp_Time_Counter->StartStop(is_start);
    }
}//}}}

///*********************************
/// Function     : SetTimerCompareMatchValue 
/// Description  : Set compare match for each timer
///*********************************
void PSI5::SetTimerCompareMatchValue (const unsigned char timer_type, const unsigned int value)
{//{{{
    if (timer_type == emSyncPulseTimer) {//Sync pulse timer
        mSync_Pulse_Timer->SetCompareMatchValue(value);
    } else {//Slot timer
        mSlot_Timer[timer_type]->SetCompareMatchValue(value);
    }
}//}}}

///*********************************
/// Function     : SetTimerCounterIntialValue 
/// Description  : Set counter initial value of timer
///*********************************
void PSI5::SetTimerCounterIntialValue (const unsigned char timer_type, const unsigned int value)
{//{{{
    if (timer_type == emSyncPulseTimer) {//Sync pulse timer
        mSync_Pulse_Timer->SetCounterIntialValue(value);
    } else if ((timer_type != emSyncPulseTimer) && (timer_type != emStampTimeCounter)) {//Slot timer
        mSlot_Timer[timer_type]->SetCounterIntialValue(value);
    } else {//Stamp time counter
        mStamp_Time_Counter->SetCounterIntialValue(value);
    }
}//}}}

///*********************************
/// Function     : SetTimerCounterPeriod 
/// Description  : Set counter period of each timer
///*********************************
void PSI5::SetTimerCounterPeriod (const unsigned char timer_type, const double counter_period)
{//{{{
    if (timer_type == emSyncPulseTimer) {//Sync pulse timer
        mSync_Pulse_Timer->SetCounterPeriod(counter_period);
    } else if ((timer_type != emSyncPulseTimer) && (timer_type != emStampTimeCounter)) {//Slot timer
        mSlot_Timer[timer_type]->SetCounterPeriod(counter_period);
    } else {//Stamp time counter
        mStamp_Time_Counter->SetCounterPeriod(counter_period);
    }
}//}}}

///*********************************
/// Function     : SetTimerCounterMode 
/// Description  : Set counter mode of each timer
///*********************************
void PSI5::SetTimerCounterMode (const unsigned char timer_type, const bool is_freerun)
{//{{{
    if (timer_type == emSyncPulseTimer) {//Sync pulse timer
        mSync_Pulse_Timer->SetCounterMode(is_freerun);
    } else if ((timer_type != emSyncPulseTimer) && (timer_type != emStampTimeCounter)) {//Slot timer
        mSlot_Timer[timer_type]->SetCounterMode(is_freerun);
    } else {//Stamp time counter
        mStamp_Time_Counter->SetCounterMode(is_freerun);
    }
}//}}}

///*********************************
/// Function     : GetStampTimeCounterValue 
/// Description  : Get counter value of each timer
///*********************************
unsigned int PSI5::GetStampTimeCounterValue ()
{//{{{
    return mStamp_Time_Counter->GetCounterValue();
}//}}}
///*********************************
/// Function     : ControlInterruptPorts 
/// Description  : Notify events to control interrupt ports
///*********************************
void PSI5::ControlInterruptPorts (const unsigned int interrupt_id, const bool is_active)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mIsActiveInt = is_active;
    mWriteInterruptPortsEvent[interrupt_id].notify(CalculateCLKPosEdge("psi5_com_clk", current_time - mStartClkTime[1]), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : SendData 
/// Description  : Prepare data format and notify a event to send the data via V_DATA port
///*********************************
void PSI5::SendData (const unsigned int data_format, const unsigned int data)
{//{{{
    mIsSyncPulseSending = false;//Stop sending sync pulse
    PrepareTxDataFrame(data_format,data);
}//}}}

///*********************************
/// Function     : Writepsi5_ts_tick_outPort 
/// Description  : Write frequency value to psi5_ts_tick_out port
///*********************************
void PSI5::Writepsi5_ts_tick_outPort (const unsigned int bitrate_value, const bool is_clear)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if (is_clear) {
        mCountFreq = 0;
    } else {
        mCountFreq = mPSI5_COM_CLKFreq/(bitrate_value + 1);
    }
    mpsi5_ts_tick_outWriteEvent.notify(CalculateCLKPosEdge("psi5_com_clk", current_time - mStartClkTime[1]), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Sendpsi5_ts_clr_outPort 
/// Description  : Send psi5_ts_clr_out pulse
///*********************************
void PSI5::Sendpsi5_ts_clr_outPort (void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mAssertpsi5_ts_clr_outEvent.notify(CalculateCLKPosEdge("psi5_com_clk", current_time - mStartClkTime[1]), mTimeResolutionUnit);
    mDeAssertpsi5_ts_clr_outEvent.notify(CalculateCLKPosEdge("psi5_com_clk", current_time - mStartClkTime[1]) + mPSI5_COM_CLKPeriod, mTimeResolutionUnit);
}///}}}

///*********************************
/// Function     : EnableSoftwareReset
/// Description  : Enable software reset
///*********************************
void PSI5::EnableSoftwareReset(const bool is_active)
{//{{{
    this->EnableReset(is_active, 0);
}//}}}
///*********************************
/// Function     : HandleTimerOvf
/// Description  : Handle Timer after Overflow
///*********************************
void PSI5::HandleTimerOvf(void){
    double counter_period = 0;
    mStamp_Time_Counter->StartStop(false);
    mPSI5_Func->UpdateIPTIMERValue(0xFFFFFFFF);
    mStamp_Time_Counter->SetCounterIntialValue(0xFFFFFFFF);
    if(!(mPSI5_Func->IsMasterModeTimer())){
        counter_period = mCounter_period_in_slave_mode;
    }else{
        counter_period = mCounter_period_in_master_mode;
    }
    mStartTimerAfterOVF.notify(counter_period,mTimeResolutionUnit);
}
///*********************************
/// Function     : StartTimerAfterOVF
/// Description  : Start Timer After Overflow.
///
void PSI5::StartTimerAfterOVF()
{
    mStamp_Time_Counter->SetCounterIntialValue(0x0);
    mStamp_Time_Counter->StartStop(true);
}
void PSI5::SetCounter_period_in_master_mode(double period)
{
    mCounter_period_in_master_mode = period;
}
// vim600: set foldmethod=marker :
