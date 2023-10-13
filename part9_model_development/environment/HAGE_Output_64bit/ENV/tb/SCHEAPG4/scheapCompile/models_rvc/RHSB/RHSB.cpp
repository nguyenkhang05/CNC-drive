// ---------------------------------------------------------------------
// $Id: RHSB.cpp,v 1.4 2018/08/11 07:54:56 ngathunguyen Exp $
//
// Copyright(c) 2016 - 2018 Renesas Electronics Corporation
// Copyright(c) 2016 - 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "RHSB.h"
#include "RHSB_Func.h"

///Constructor of RHSB class
RHSB::RHSB (sc_module_name name, unsigned int rLatency, unsigned int wLatency):
    BusSlaveBase<32,1>(),
    Cgeneral_timer(name),
    CLK_LSB("CLK_LSB"),
    CLK_RHSB("CLK_RHSB"),
    PRESETn("PRESETn"),
    rstc_n("rstc_n"),
    EMRG("EMRG"),
    tmr("tmr"),
    U_CONTROL0("U_CONTROL0"),
    U_CONTROL1("U_CONTROL1"),
    SI0("SI0"),
    SI1("SI1"),
    FCL("FCL"),
    D_CONTROL("D_CONTROL"),
    SO("SO"),
    CSD0("CSD0"),
    CSD1("CSD1"),
    INTL0("INTL0"),
    INTL1("INTL1"),
    INTL2("INTL2"),
    INTL3("INTL3"),
    INTL4("INTL4"),
    INTDWNDATADMA("INTDWNDATADMA"),
    INTDWNCMDDMA("INTDWNCMDDMA"),
    INTUPDMA("INTUPDMA")
{//{{{
    //Initialize output signals
    FCL.initialize(0x0);
    D_CONTROL.initialize(0x0);
    SO.initialize(0xFFFFFFFFFFFFFFFFULL);
    CSD0.initialize(false);
    CSD1.initialize(false);
    INTL0.initialize(false);
    INTL1.initialize(false);
    INTL2.initialize(false);
    INTL3.initialize(false);
    INTL4.initialize(false);
    INTDWNDATADMA.initialize(false);
    INTDWNCMDDMA.initialize(false);
    INTUPDMA.initialize(false);

    //Initialize variables
    mCLK_LSBPeriod = 0;
    mCLK_RHSBPeriod = 0;
    mCLK_LSBFreq = 0;
    mCLK_LSBOrgFreq = 0;
    mCLK_RHSBFreq = 0;
    mCLK_RHSBOrgFreq = 0;
    mCLK_LSBFreqUnit = "Hz";
    mCLK_RHSBFreqUnit = "Hz";

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
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
    Initialize();
    
    //General timer setting
    Cgeneral_timer::setCCLR(emInFactor);//Clear mode
    Cgeneral_timer::setCounterDirection(emCountUp);//Count up
    Cgeneral_timer::setTimeUnit(mTimeResolutionUnit);//Time unit
    Cgeneral_timer::setCMS(32);//Counter size
    Cgeneral_timer::setCMM(emFreeRun);//Free-running mode
    
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&CLK_LSB);
    setSlaveResetPort32(&PRESETn);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mRHSB_Func = new RHSB_Func((std::string)name, this);
    sc_assert(mRHSB_Func != NULL);

    mBusSlaveIf32[0]->setFuncModulePtr(mRHSB_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandleCLK_LSBSignalMethod);
    dont_initialize();
    sensitive << CLK_LSB;

    SC_METHOD(HandleCLK_RHSBSignalMethod);
    dont_initialize();
    sensitive << CLK_RHSB;

    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;

    SC_METHOD(Handlerstc_nSignalMethod);
    sensitive << rstc_n;

    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mIntrUpdateEvent[interrupt_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&RHSB::HandleIntrMethod, this, interrupt_id),
                          sc_core::sc_gen_unique_name("HandleIntrMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&RHSB::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&RHSB::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&RHSB::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(HandleU_CONTROL0SignalMethod);
    dont_initialize();
    sensitive << U_CONTROL0;
    sensitive << mTestmodeSlave0Event;

    SC_METHOD(HandleU_CONTROL1SignalMethod);
    dont_initialize();
    sensitive << U_CONTROL1;
    sensitive << mTestmodeSlave1Event;

    SC_METHOD(EmergencyMethod);
    dont_initialize();
    sensitive << EMRG;

    SC_METHOD(StoreReceptionMethod);
    dont_initialize();
    sensitive << mStoreReceptionEvent;

    for (unsigned int line_id = 0; line_id < emSlaveChannelNum; line_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mReceptionDataEvent[line_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&RHSB::DataReceptionMethod, this, line_id),
                          sc_core::sc_gen_unique_name("DataReceptionMethod"), &opt);
    }

    for (unsigned int line_id = 0; line_id < emSlaveChannelNum; line_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mUpstreamTimeOutEvent[line_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&RHSB::UpstreamTimeOutMethod, this, line_id),
                          sc_core::sc_gen_unique_name("UpstreamTimeOutMethod"), &opt);
    }

    SC_THREAD(DataFrameTransmissionThread);
    dont_initialize();
    sensitive << mDataFrameTransmissionEvent;

    SC_THREAD(CommandFrameTransmissionThread);
    dont_initialize();
    sensitive << mCommandFrameTransmissionEvent;

    SC_THREAD(DownstreamCommunicationThread);
    dont_initialize();
    sensitive << mDownstreamTransmissionEvent;

    SC_METHOD(StartTimerMethod);
    dont_initialize();
    sensitive << mStartTimerEvent;

    SC_METHOD(ACCChangeDoneMethod);
    dont_initialize();
    sensitive << mACCChangeDoneEvent;

    SC_METHOD(HandleFCLPortMethod);
    dont_initialize();
    sensitive << mFCLPortEvent;

    SC_METHOD(HandleD_CONTROLPortMethod);
    dont_initialize();
    sensitive << mD_CONTROLPortEvent;

    SC_METHOD(HandleSOPortMethod);
    dont_initialize();
    sensitive << mSOPortEvent;

    for (unsigned int slaveNum = 0; slaveNum < emSlaveChannelNum; slaveNum++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCSDPortEvent[slaveNum]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&RHSB::HandleCSDPortMethod, this, slaveNum),
                          sc_core::sc_gen_unique_name("HandleCSDPortMethod"), &opt);
    }
    SC_METHOD(HandleDataTransStatusMethod);
    dont_initialize();
    sensitive << DTSF_sig;
    sensitive << DTF_sig;

    SC_METHOD(HandleCommandTransStatusMethod);
    dont_initialize();
    sensitive << CTF_sig;
    sensitive << TSF_sig;

    SC_METHOD(HandleERRTimeOutDataLostMethod);
    dont_initialize();
    sensitive << UEF_sig;
    sensitive << TOF_sig;
    sensitive << DLF_sig;

}//}}}

///Destructor of RHSB class
RHSB::~RHSB (void)
{//{{{
    delete mRHSB_Func;
}//}}}

// Set data transation port value
void RHSB::HandleDataTransStatusMethod()
{//{{{
    bool pre_value = INTL0.read();
    bool INTL0_val = DTSF_sig.read() | DTF_sig.read();
    INTL0.write(INTL0_val);
    if (pre_value != INTL0_val) {
        DumpInterruptMsg("INTL0", INTL0_val);
    }
}//}}}

// Set command transation port value
void RHSB::HandleCommandTransStatusMethod()
{//{{{
    bool pre_value = INTL1.read();
    bool INTL1_val = CTF_sig.read() | TSF_sig.read();
    INTL1.write(INTL1_val);
    if (pre_value != INTL1_val) {
        DumpInterruptMsg("INTL1", INTL1_val);
    }
}//}}}

// Set status transation port error, time out, data losts value
void RHSB::HandleERRTimeOutDataLostMethod()
{//{{{
    bool pre_value = INTL4.read();
    bool INTL4_val = (UEF_sig.read() | TOF_sig.read() | DLF_sig);
    INTL4.write(INTL4_val);
    if (pre_value != INTL4_val) {
        DumpInterruptMsg("INTL4", INTL4_val);
    }
}//}}}
///Set message level (fatal/error/warning/info)
void RHSB::SetMessageLevel (const std::string msg_lv)
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
    mRHSB_Func->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mRHSB_Func->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void RHSB::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mRHSB_Func->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping interrupt message
void RHSB::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_enable);
    if (temp.size() == 1) {
        if (temp[0] == "true") {
            mDumpInterrupt = true;
        } else if (temp[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: RHSB_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (temp.size() == 0){
        std::string temp_arg = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","RHSB_DumpInterrupt     %s\n",temp_arg.c_str());
    } else {
        re_printf("warning","Invalid argument: RHSB_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Assert reset by software for PRESETn, rstc_n
void RHSB::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETn])&&(!mIsResetCmdReceive[emPRESETn])) {
            mIsResetCmdReceive[emPRESETn] = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPRESETn].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETn] = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else if (reset_name == "rstc_n") {
        if ((!mIsResetHardActive[emrstc_n])&&(!mIsResetCmdReceive[emrstc_n])) {
            mIsResetCmdReceive[emrstc_n] = true;
            re_printf("info","The model will be reset (rstc_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emrstc_n].notify(start_time, SC_NS);
            mResetCmdPeriod[emrstc_n] = period;
        } else {
            re_printf("warning","The software reset of rstc_n is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning", "The name of reset signal is wrong. It should be PRESETn or rstc_n.\n");
    }
}//}}}

///Set clock value and clock unit for CLK_LSB, CLK_RHSB
void RHSB::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            ConvertClockFreq (mCLK_LSBFreq, mCLK_LSBFreqUnit, freq, unit);
            mCLK_LSBOrgFreq = freq;
            if (mCLK_LSBFreq > 0) {
                mCLK_LSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_LSBFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mCLK_LSBPeriod = 0;
                CancelEvents();
            }
        } else if (word_vector[0] == "CLK_RHSB") {
            ConvertClockFreq (mCLK_RHSBFreq, mCLK_RHSBFreqUnit, freq, unit);
            mCLK_RHSBOrgFreq = freq;
            if (mCLK_RHSBFreq > 0) {
                mCLK_RHSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_RHSBFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mCLK_RHSBPeriod = 0;
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent.notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock value of CLK_LSB, CLK_RHSB
void RHSB::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            re_printf("info","CLK_LSB frequency is %0.0f %s.\n", (double)mCLK_LSBOrgFreq, mCLK_LSBFreqUnit.c_str());
        } else if (word_vector[0] == "CLK_RHSB") {
            re_printf("info","CLK_RHSB frequency is %0.0f %s.\n", (double)mCLK_RHSBOrgFreq, mCLK_RHSBFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void RHSB::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mRHSB_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void RHSB::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mRHSB_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers
void RHSB::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mRHSB_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register
void RHSB::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mRHSB_Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///List all registers name
void RHSB::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mRHSB_Func->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void RHSB::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"--- parameters ---");
            SC_REPORT_INFO(this->basename(),"RHSB_MessageLevel   (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"RHSB_DumpRegisterRW (true/false)                     Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"RHSB_DumpInterrupt  (true/false)                     Enable/disable dumping interrupt message (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"--- commands ---");
            SC_REPORT_INFO(this->basename(),"RHSB_AssertReset    (reset_name, start_time, period) Assert and de-assert reset signal (PRESETn or rstc_n).");
            SC_REPORT_INFO(this->basename(),"RHSB_SetCLKFreq     (clock_name, freq, unit)         Set clock frequency to CLK_LSB, CLK_RHSB clock.");
            SC_REPORT_INFO(this->basename(),"RHSB_GetCLKFreq     (clock_name)                     Get clock frequency of CLK_LSB, CLK_RHSB clock.");
            SC_REPORT_INFO(this->basename(),"RHSB_ForceRegister  (reg_name, value)                Force register with setting value.");
            SC_REPORT_INFO(this->basename(),"RHSB_ReleaseRegister(reg_name)                       Release register from force value.");
            SC_REPORT_INFO(this->basename(),"RHSB_WriteRegister  (reg_name, value)                Write the value to register.");
            SC_REPORT_INFO(this->basename(),"RHSB_ReadRegister   (reg_name)                       Read value from register.");
            SC_REPORT_INFO(this->basename(),"RHSB_ListRegister                                    Dump name of RHSB's registers.");
        } else {
            re_printf("warning","The name (%s) of RHSB_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of RHSB_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Handle CLK_LSB signal
void RHSB::HandleCLK_LSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_LSB.read();
    SetCLKFreq("CLK_LSB", freq_value, "Hz");
}//}}}

///Handle CLK_RHSB signal
void RHSB::HandleCLK_RHSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_RHSB.read();
    SetCLKFreq("CLK_RHSB", freq_value, "Hz");
}//}}}

///Process reset function when PRESETn or rstc_n command is active
void RHSB::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == 0) {
        reset_name = "PRESETn";
    } else { //reset_id == 1
        reset_name = "rstc_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when PRESETn or rstc_n command is active
void RHSB::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == 0) {
        reset_name = "PRESETn";
    } else { //reset_id == 1
        reset_name = "rstc_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]); 
}//}}}

///Handle PRESETn signal
void RHSB::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[emPRESETn] = PRESETn.read();
    if (mResetCurVal[emPRESETn] != mResetPreVal[emPRESETn]) {//mResetPreVal is "true" at initial state
        mResetPreVal[emPRESETn] = mResetCurVal[emPRESETn];
        if (mCLK_LSBPeriod > 0) {
            double current_time = GetCurTime();
            mResetHardEvent[emPRESETn].notify(CalculateCLKPosEdge("CLK_LSB", current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent[emPRESETn].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Handle rstc_n signal
void RHSB::Handlerstc_nSignalMethod (void)
{//{{{
    mResetCurVal[emrstc_n] = rstc_n.read();
    if (mResetCurVal[emrstc_n] != mResetPreVal[emrstc_n]) {//mResetPreVal is "true" at initial state
        mResetPreVal[emrstc_n] = mResetCurVal[emrstc_n];
        if (mCLK_RHSBPeriod > 0) {
            double current_time = GetCurTime();
            mResetHardEvent[emrstc_n].notify(CalculateCLKPosEdge("CLK_RHSB", current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent[emrstc_n].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when PRESETn or rstc_n is active
void RHSB::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset signal PRESETn is asserted.\n");
        } else {
            re_printf("info","The reset signal rstc_n is asserted.\n");
        }
        this->EnableReset(mIsResetHardActive[reset_id]);
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            this->EnableReset(mIsResetHardActive[reset_id]);
            if (reset_id == 0) {
                re_printf("info","The reset signal PRESETn is de-asserted.\n");
            } else {
                re_printf("info","The reset signal rstc_n is de-asserted.\n");
            }
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

///Handle interrupt 
void RHSB::HandleIntrMethod(const unsigned int interrupt_id)
{//{{{
    if (CheckClockPeriod("CLK_RHSB")) {
        std::string intr_name = "";
        bool pre_value = false;
        bool value = mIntrValue[interrupt_id];
        switch (interrupt_id) {
            case emINTDTSF:
                intr_name = "DTSF";
                pre_value = DTSF_sig.read();
                DTSF_sig.write(value);
                break;

            case emINTDTF:
                intr_name = "DTF";
                pre_value = DTF_sig.read();
                DTF_sig.write(value);
                break;

            case emINTCTF:
                intr_name = "CTF";
                pre_value = CTF_sig.read();
                CTF_sig.write(value);
                break;

            case emINTTSF:
                intr_name = "TSF";
                pre_value = TSF_sig.read();
                TSF_sig.write(value);
                break;

            case emINTL2:
                intr_name = "INTL2";
                pre_value = INTL2.read();
                INTL2.write(value);
                break;

            case emINTL3:
                intr_name = "INTL3";
                pre_value = INTL3.read();
                INTL3.write(value);
                break;

            case emINTUEF:
                intr_name = "UEF";
                pre_value = UEF_sig.read();
                UEF_sig.write(value);
                break;

            case emINTTOF:
                intr_name = "TOF";
                pre_value = TOF_sig.read();
                TOF_sig.write(value);
                break;

            case emINTDLF:
                intr_name = "DLF";
                pre_value = DLF_sig.read();
                DLF_sig.write(value);
                break;

            case emDownDataDMA:
                intr_name = "INTDWNDATADMA";
                pre_value = INTDWNDATADMA.read();
                INTDWNDATADMA.write(value);
                if (value == 1) {
                    ControlInterruptPorts(false, emDownDataDMA);
                }
                break;

            case emDownCommandDMA:
                intr_name = "INTDWNCMDDMA";
                pre_value = INTDWNCMDDMA.read();
                INTDWNCMDDMA.write(value);
                if (value == 1) {
                    ControlInterruptPorts(false, emDownCommandDMA);
                }
                break;

            default:// emUpDMA
                intr_name = "INTUPDMA";
                pre_value = INTUPDMA.read();
                INTUPDMA.write(value);
                if (value == 1) {
                    ControlInterruptPorts(false, emUpDMA);
                }
                break;
        }
        if (pre_value != value) {
            DumpInterruptMsg(intr_name,value);
        }
    }
}//}}}

///Handle upstream communication from U_CONTROL0 signal
void RHSB::HandleU_CONTROL0SignalMethod (void)
{//{{{
    bool stop_reception = false;
    if (mRHSB_Func->GetCurrentState() == "TEST") {
        mCurSTBE[emSlave0Channel] = (mD2U_CONTROLval>>6) & 0x3;  
    } else {
        mCurSTBE[emSlave0Channel] = (U_CONTROL0.read()>>6) & 0x3;
    }
    if ((!mRHSB_Func->IsResetState()) && (CheckClockPeriod("CLK_RHSB"))) {
        if (((mRHSB_Func->GetCurrentState() == "ACTIVE")||(mRHSB_Func->GetCurrentState() == "TEST")) && (mRHSB_Func->IsUpstreamEnable())) {// If upstream is enable in ACTIVE/TEST state
            double expected_databittime = mRHSB_Func->GetContentBitNum(emSlave0Channel)*mRHSB_Func->GetUpstreamBitTime(emSlave0Channel);
            double expected_stopbittime = mRHSB_Func->GetStopBitNum(emSlave0Channel)*mRHSB_Func->GetUpstreamBitTime(emSlave0Channel);
            if (mPreSTBE[emSlave0Channel] == 2) {
                if (mIsDataReceiving){ //Finish receive stop bits
                    double total_stopbittime = GetCurTime() - mStartReceptionTime[emSlave0Channel] - mRHSB_Func->GetUpstreamBitTime(emSlave0Channel) - expected_databittime;
                    if (total_stopbittime != expected_stopbittime) {
                        re_printf("warning","Receive unexpected stop bit number\n");
                    }
                    mRHSB_Func->StoreReceptionDataFrame(mCurrentReceptionChannel, mReceptionData[mCurrentReceptionChannel], mIsRX_FERR);
                    if ((!mIsRX_FERR)&&(!mRHSB_Func->IsPERR())) {
                        mIsRX_Valid = true;
                    }
                    if (mIsRX_Valid) {
                        mUpstreamTimeOutEvent[0].cancel();//Stop time out event
                        re_printf ("info","Timeout counter is stopped in channel %d.\n",emSlave0Channel);
                    }
                    mIsDataReceiving = false;
                    re_printf("info","Finish reception process in channel %d.\n",emSlave0Channel);
                    mRHSB_Func->SetUpstreamActivityStatus(0);//completed receiving data on upstream communication
                    mIsRX_FERR = false;
                }
            } else if (mPreSTBE[emSlave0Channel] == 1) {
                if (mIsDataReceiving){
                    double total_databittime = GetCurTime() - mStartReceptionTime[emSlave0Channel] - mRHSB_Func->GetUpstreamBitTime(emSlave0Channel);
                    if ((total_databittime != expected_databittime)||(mCurSTBE[emSlave0Channel] != 2)) {
                        re_printf("warning","Receive unexpected data bit number or U_CONTROL%d.STBE is change to %d during reception process\n",mCurrentReceptionChannel,mCurSTBE[emSlave0Channel]);
                        stop_reception = true;
                        mIsDataReceiving = false;
                        mIsRX_Valid = false;
                    }
                }
            }
            if (mCurSTBE[emSlave0Channel] == 1) { //Begin receive upstream frame
                mIsDataReceiving = true;
                if ((!(mRHSB_Func->IsDedicatedUpstreamMode())) ||//upstream mode is shared mode
                    (mRHSB_Func->IsDedicatedUpstreamMode() && (mRHSB_Func->GetActiveChannel() == emSlave0Channel))) {//upstream mode is dedicated mode & line 0 is selected
                    re_printf("info","Start reception process in channel %d.\n",emSlave0Channel);
                    mCurrentReceptionChannel = emSlave0Channel;
                    if (mRHSB_Func->GetCurrentState() == "TEST") {
                        mSIZE[emSlave0Channel] = (mD2U_CONTROLval&0xF);
                        mBITT[emSlave0Channel] = (mD2U_CONTROLval>>16) & 0xFFFF;
                    } else {
                        mSIZE[emSlave0Channel] = U_CONTROL0.read() & 0xF;
                        mBITT[emSlave0Channel] = ((U_CONTROL0.read()>>16) & 0xFFFF);
                    }
                    mRHSB_Func->SetUpstreamActivityStatus(1);
                    mStartReceptionTime[emSlave0Channel] = GetCurTime();
                    mReceptionDataEvent[emSlave0Channel].notify(mRHSB_Func->GetUpstreamBitTime(emSlave0Channel),mTimeResolutionUnit); // Start after 1 upstream bit time
                }
            }
        }
    }
    if (stop_reception) {
        mReceptionDataEvent[emSlave0Channel].cancel();
        mStoreReceptionEvent.cancel();
    }
    mPreSTBE[emSlave0Channel] = mCurSTBE[emSlave0Channel];
}//}}}

///Handle upstream communication from U_CONTROL1 signal
void RHSB::HandleU_CONTROL1SignalMethod (void)
{//{{{
    bool stop_reception = false;
    if (mRHSB_Func->GetCurrentState() == "TEST") {
        mCurSTBE[emSlave1Channel] = (mD2U_CONTROLval>>6) & 0x3;
    } else {
        mCurSTBE[emSlave1Channel] = (U_CONTROL1.read()>>6) & 0x3;
    }
    if ((!mRHSB_Func->IsResetState()) && (CheckClockPeriod("CLK_RHSB"))) {
        if (((mRHSB_Func->GetCurrentState() == "ACTIVE")||(mRHSB_Func->GetCurrentState() == "TEST")) && (mRHSB_Func->IsUpstreamEnable()) && //upstream is enable in ACTIVE/TEST state
            (mRHSB_Func->GetActiveChannel() == emSlave1Channel)) {//line 1 is selected
            if (!(mRHSB_Func->IsDedicatedUpstreamMode())) {//upstream mode is shared mode
                re_printf("warning","The channel 1 is always inactive in shared mode.\n");
            } else {//upstream mode is dedicated mode
                double expected_databittime = mRHSB_Func->GetContentBitNum(emSlave1Channel)*mRHSB_Func->GetUpstreamBitTime(emSlave1Channel);
                double expected_stopbittime = mRHSB_Func->GetStopBitNum(emSlave1Channel)*mRHSB_Func->GetUpstreamBitTime(emSlave1Channel);
                if (mPreSTBE[emSlave1Channel] == 2) {
                    if (mIsDataReceiving){ //Finish receive stop bits
                        double total_stopbittime = GetCurTime() - mStartReceptionTime[emSlave1Channel] - mRHSB_Func->GetUpstreamBitTime(emSlave1Channel) - expected_databittime;
                        if (total_stopbittime != expected_stopbittime) {
                            re_printf("warning","Receive unexpected stop bit number\n");
                        }
                        mRHSB_Func->StoreReceptionDataFrame(mCurrentReceptionChannel, mReceptionData[mCurrentReceptionChannel], mIsRX_FERR);
                        if ((!mIsRX_FERR)&&(!mRHSB_Func->IsPERR())) {
                            mIsRX_Valid = true;
                        }
                        if (mIsRX_Valid) {
                            mUpstreamTimeOutEvent[1].cancel();//Stop time out event
                            re_printf ("info","Timeout counter is stopped in channel %d.\n",emSlave1Channel);
                        }
                        mIsDataReceiving = false;
                        re_printf("info","Finish reception process in channel %d.\n",emSlave1Channel);
                        mRHSB_Func->SetUpstreamActivityStatus(0);//completed receiving data on upstream communication
                        mIsRX_FERR = false;
                    }
                } else if (mPreSTBE[emSlave1Channel] == 1) {
                    if (mIsDataReceiving){
                        double total_databittime = GetCurTime() - mStartReceptionTime[emSlave1Channel] - mRHSB_Func->GetUpstreamBitTime(emSlave1Channel);
                        if ((total_databittime != expected_databittime)||(mCurSTBE[emSlave1Channel] != 2)) {
                            re_printf("warning","Receive unexpected data bit number or U_CONTROL%d.STBE is change to %d during reception process\n",mCurrentReceptionChannel,mCurSTBE[emSlave0Channel]);
                            stop_reception = true;
                            mIsDataReceiving = false;
                            mIsRX_Valid = false;
                        }
                    }
                }
                if (mCurSTBE[emSlave1Channel] == 1) { //Begin receive upstream frame
                    mIsDataReceiving = true;
                    if ((!(mRHSB_Func->IsDedicatedUpstreamMode())) ||//upstream mode is shared mode
                        (mRHSB_Func->IsDedicatedUpstreamMode() && (mRHSB_Func->GetActiveChannel() == emSlave1Channel))) {//upstream mode is dedicated mode & line 0 is selected
                        re_printf("info","Start reception process in channel %d.\n",emSlave1Channel);
                        mCurrentReceptionChannel = emSlave1Channel;
                        if (mRHSB_Func->GetCurrentState() == "TEST") {
                            mSIZE[emSlave1Channel] = (mD2U_CONTROLval&0xF);
                            mBITT[emSlave1Channel] = (mD2U_CONTROLval>>16) & 0xFFFF;
                        } else {
                            mSIZE[emSlave1Channel] = U_CONTROL1.read() & 0xF;
                            mBITT[emSlave1Channel] = ((U_CONTROL1.read()>>16) & 0xFFFF);
                        }
                        mRHSB_Func->SetUpstreamActivityStatus(1);
                        mStartReceptionTime[emSlave1Channel] = GetCurTime();
                        mReceptionDataEvent[emSlave1Channel].notify(mRHSB_Func->GetUpstreamBitTime(emSlave1Channel),mTimeResolutionUnit); // Start after 1 upstream bit time
                    }
                }
            }
        }
    }
    if (stop_reception) {
        mReceptionDataEvent[emSlave1Channel].cancel();
        mStoreReceptionEvent.cancel();
    }
    mPreSTBE[emSlave1Channel] = mCurSTBE[emSlave1Channel];
}//}}}

///Handle emergency transmission
void RHSB::EmergencyMethod (void)
{//{{{
    bool emrg_value = EMRG.read();
    if ((!mRHSB_Func->IsResetState()) && (CheckClockPeriod("CLK_RHSB"))) {//if reset of rstc_n is inactive
        if ((emrg_value && (mRHSB_Func->GetEmergencyPolarity() == 0x0)) || // high level or raising edge
            (!emrg_value && (mRHSB_Func->GetEmergencyPolarity() == 0x1))) {// low level or falling edge
            mRHSB_Func->SetEmergencySignalRaisingFlag();
            if (mRHSB_Func->GetEmergencyEnableValue() == 0x1) {//Emergency is enable and automatic stop disabled
                mIsEmergencyEnabled = true; 
            } else if (mRHSB_Func->GetEmergencyEnableValue() == 0x3) {//Emergency is enable and automatic stop enabled
                mIsEmergencyEnabled = true; 
                mIsEmergencyAutoStop = true;
                mEmergencyFrameCnt = 0;
            }
        } else {
            if (mRHSB_Func->IsEmergencyLevelSensitive()) {
                if(mIsDataTransferring){
                    re_printf("warning","EMRG level is changed during an emergency frame transmission with level sensitive input mode.\n");
                }
                mIsEmergencyEnabled = false; 
                mIsEmergencyAutoStop = false; 
                mEmergencyFrameCnt = 0;
            }
        }
    }
}//}}}

void RHSB::DataReceptionMethod (const unsigned int line_id)
{//{{{
    if ((!mRHSB_Func->IsResetState()) && (CheckClockPeriod("CLK_RHSB"))) {
        if (line_id == emSlave0Channel) {
            mReceptionData[line_id] = SI0.read();
        } else {
            mReceptionData[line_id] = SI1.read();
        }
        mStoreReceptionEvent.notify(mRHSB_Func->CalculateReceptionBitsTotalTime(line_id),mTimeResolutionUnit);
    }
}//}}}

void RHSB::StoreReceptionMethod (void)
{//{{{
    if ((!mRHSB_Func->IsResetState()) && (CheckClockPeriod("CLK_RHSB"))) {//if reset of rstc_n is inactive
        if (mIsDataReceiving) {
            unsigned int reception_data = 0;
            if (mCurrentReceptionChannel == emSlave0Channel) {//Data is received on line 0
                reception_data = SI0.read();
            } else {//Data is received on line 1
                reception_data = SI1.read();
            }
            if (reception_data != mReceptionData[mCurrentReceptionChannel]) {
                mIsRX_FERR = true;
            }
        }
    }
}//}}}

///Time out process
void RHSB::UpstreamTimeOutMethod (const unsigned int line_id)
{//{{{
    if ((!mRHSB_Func->IsResetState()) && (CheckClockPeriod("CLK_RHSB"))) {//if reset of rstc_n is inactive
        mRHSB_Func->SetTimeOutOperation(line_id);
        re_printf ("info","Timeout is detected in channel %d\n", line_id);
    }
}//}}}


///Transfer data frame
void RHSB::DataFrameTransmissionThread (void)
{//{{{
    for(;;) {
        if((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
            wait();
            continue;
        }
        try {
            while (true) {
                mIsDataTransferring = true;
                sc_dt::uint64 data = 0x0;
                unsigned int sequence_length = 0x0;
                unsigned int position = 0x0;
                unsigned int trans_total_bit = 0x0;
                unsigned int slave = mRHSB_Func->GetDataTransmissionSelectedSlave(0);
                unsigned int CPS_length = 0x0;
                unsigned int assertion_phase_length = 0x0;
                unsigned int deassertion_phase_length = 0x0;
                unsigned int bit_len = 0x0;
                bool chip_selection_polarity = true;
                for (unsigned int index = 0; index < 4; index++) {
                    mData[index] = 0x0;
                }
                mIsEmergencyData = false;
                if (mRHSB_Func->IsMultiPeriodDownstreamMode()) {
                    if (mIsFstData && (mRHSB_Func->GetTransmissionPeriod() == 1)){
                        mRHSB_Func->SwitchTransmissionPeriod();
                        mIsFstData = false;
                    }
                    sequence_length = mRHSB_Func->GetSeqquenceLength(mRHSB_Func->GetTransmissionPeriod());
                    mRHSB_Func->DataFrameAssemble(mIsEmergencyEnabled, mData, mRHSB_Func->GetTransmissionPeriod());
                } else {
                    sequence_length = mRHSB_Func->GetSeqquenceLength(0);
                    mRHSB_Func->DataFrameAssemble(mIsEmergencyEnabled, mData, 0);
                }
                mIsEmergencyData = mIsEmergencyEnabled;
                double current_time = GetCurTime();
                wait(CalculateCLKPosEdge("FCL", current_time),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                    throw vpcl::emRstException;
                }
                ControlInterruptPorts(false, emDownDataDMA);
                mFCLVal = mRHSB_Func->GetDownstreamBitRate();
                mFCLPortEvent.notify(1,SC_FS);

                mRHSB_Func->SetInterruptStatus("DTSF");
                ControlInterruptPorts(false, emINTDTSF);
                mRHSB_Func->SetInterruptStatus("TSF");
                ControlInterruptPorts(false, emINTTSF);
                unsigned int fstDFTE = 0;
                for (unsigned int dfte = 0; dfte <= sequence_length; dfte++) {
                    bit_len = mRHSB_Func->GetDataBitNum(dfte);
                    if(slave != mRHSB_Func->GetDataTransmissionSelectedSlave(dfte)){
                        assertion_phase_length = mRHSB_Func->GetAssertionPhaseLength(slave);
                        CPS_length = mRHSB_Func->GetCPS(slave);
                        deassertion_phase_length = mRHSB_Func->GetDeassertionPhaseLength(slave);
                        chip_selection_polarity = mRHSB_Func->GetChipSelectionPolarity(slave);
                        if (slave == 0) {
                            mCSD0Val = chip_selection_polarity;
                        } else {
                            mCSD1Val = chip_selection_polarity;
                        }
                        mCSDPortEvent[slave].notify(1,SC_FS);

                        //Wait for assertion phase
                        wait(assertion_phase_length*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                            throw vpcl::emRstException;
                        }
                        if (mRHSB_Func->GetCurrentState() == "ACTIVE") {
                            mSOVal = data;
                            mSOPortEvent.notify(1,SC_FS);
                            mDCONTROLVal = (((mRHSB_Func->GetCLP()&0x1)<<9)|((mRHSB_Func->GetSOLP(slave)&0x1)<<8)|((trans_total_bit&0x7F)<<1)|0);
                            mD_CONTROLPortEvent.notify(1,SC_FS);
                           
                        } else {
                            mSO2SI_val = data;
                            mD2U_CONTROLval = ((((unsigned int)mRHSB_Func->GetDownstreamBitTime()&0xFFFF)<<16)|(0x1<<6)|(trans_total_bit&0xF));
                            if (slave == 0) {
                                mTestmodeSlave0Event.notify(SC_ZERO_TIME);
                            } else { //slave == 1
                                mTestmodeSlave1Event.notify(SC_ZERO_TIME);
                            }
                        }
                        //Wait data length
                        wait(CPS_length*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                            throw vpcl::emRstException;
                        }
                        for (unsigned int cur_dfte = fstDFTE; cur_dfte < dfte; cur_dfte++) {
                            mRHSB_Func->SetDownstreamActivity(4+cur_dfte);
                            wait((mRHSB_Func->GetDataBitNum(cur_dfte))*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                            if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                throw vpcl::emRstException;
                            }
                        }
                        if (mRHSB_Func->GetCurrentState() == "ACTIVE") {
                            mSOVal = 0xFFFFFFFFFFFFFFFFULL;
                            mSOPortEvent.notify(1,SC_FS); 
                        } else {
                            mD2U_CONTROLval = ((((unsigned int)mRHSB_Func->GetDownstreamBitTime()&0xFFFF)<<16)|(0x2<<6)|(trans_total_bit&0xF));
                        }
                        //Wait for deassertion phase length
                        wait(deassertion_phase_length*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                            throw vpcl::emRstException;
                        }
                        if (slave == 0) {
                            mCSD0Val = !chip_selection_polarity;
                        } else {
                            mCSD1Val = !chip_selection_polarity;
                        }
                        mCSDPortEvent[slave].notify(1,SC_FS);
                        mD2U_CONTROLval = 0;
                        slave = mRHSB_Func->GetDataTransmissionSelectedSlave(dfte);
                        trans_total_bit = bit_len;
                        position = 0x0;
                        data = 0x0;
                        PrepareData(data, mData[dfte], position, bit_len);
                        position = position + bit_len;
                        fstDFTE = dfte;
                    } else {
                        trans_total_bit = trans_total_bit + bit_len;
                        PrepareData(data, mData[dfte], position, bit_len);
                        position = position + bit_len;
                    }
                    if (dfte == sequence_length){
                        assertion_phase_length = mRHSB_Func->GetAssertionPhaseLength(slave);
                        CPS_length = mRHSB_Func->GetCPS(slave);
                        deassertion_phase_length = mRHSB_Func->GetDeassertionPhaseLength(slave);
                        chip_selection_polarity = mRHSB_Func->GetChipSelectionPolarity(slave);
                        if (slave == 0) {
                            mCSD0Val = chip_selection_polarity;
                        } else {
                            mCSD1Val = chip_selection_polarity;
                        }
                        mCSDPortEvent[slave].notify(1,SC_FS);
                        //Wait for assertion phase
                        wait(assertion_phase_length*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                            throw vpcl::emRstException;
                        }
                        if (mRHSB_Func->GetCurrentState() == "ACTIVE") {
                            mSOVal = data;
                            mSOPortEvent.notify(1,SC_FS); 
                            mDCONTROLVal = (((mRHSB_Func->GetCLP()&0x1)<<9)|((mRHSB_Func->GetSOLP(slave)&0x1)<<8)|((trans_total_bit&0x7F)<<1)|0);
                            mD_CONTROLPortEvent.notify(1,SC_FS);
                        } else {
                            mSO2SI_val = data;
                            mD2U_CONTROLval = ((((unsigned int)mRHSB_Func->GetDownstreamBitTime()&0xFFFF)<<16)|(0x1<<6)|(trans_total_bit&0xF));
                            if (slave == 0) {
                                mTestmodeSlave0Event.notify(SC_ZERO_TIME);
                            } else { //slave == 1
                                mTestmodeSlave1Event.notify(SC_ZERO_TIME);
                            }
                        }
                        //Wait data length
                        wait(CPS_length*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                            throw vpcl::emRstException;
                        }
                        for (unsigned int cur_dfte = fstDFTE; cur_dfte <= dfte; cur_dfte++) {
                            mRHSB_Func->SetDownstreamActivity(4+cur_dfte);
                            wait((mRHSB_Func->GetDataBitNum(cur_dfte))*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                            if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                throw vpcl::emRstException;
                            }
                        }
                        if (mRHSB_Func->GetCurrentState() == "ACTIVE") {
                            mSOVal = 0xFFFFFFFFFFFFFFFFULL;
                            mSOPortEvent.notify(1,SC_FS);
                        } else {
                            mD2U_CONTROLval = ((((unsigned int)mRHSB_Func->GetDownstreamBitTime()&0xFFFF)<<16)|(0x2<<6)|(trans_total_bit&0xF));
                        }
                        //Wait for deassertion phase length
                        wait(deassertion_phase_length*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                            throw vpcl::emRstException;
                        }
                        if (slave == 0) {
                            mCSD0Val = !chip_selection_polarity;
                        } else {
                            mCSD1Val = !chip_selection_polarity;
                        }
                        mCSDPortEvent[slave].notify(1,SC_FS);
                        mD2U_CONTROLval = 0;
                        mRHSB_Func->SetDownstreamActivity(0x1);
                        if (!mRHSB_Func->IsClockSerialActive()) {
                            mFCLVal = 0;
                            mFCLPortEvent.notify(1,SC_FS);
                        }
                        //Wait for passive length
                        wait(mRHSB_Func->GetPassivePhaseLength()*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                            throw vpcl::emRstException;
                        }
                        mRHSB_Func->SetDownstreamActivity(0x0);
                        mRHSB_Func->SetInterruptStatus("DTF");
                        ControlInterruptPorts(false, emINTDTF);
                    }
                }
                if (mIsEmergencyEnabled && mIsEmergencyData) {//If emergency is enable
                    mRHSB_Func->SetInterruptStatus("ETF");
                    ControlInterruptPorts(false, emINTL2);
                    if ((mRHSB_Func->GetTransmissionPeriod() == 0)||(!mRHSB_Func->IsMultiPeriodDownstreamMode())){
                        mEmergencyFrameCnt = mEmergencyFrameCnt + 1;
                    }
                }
                mDataFrameTransmissionCompletionEvent.notify(SC_ZERO_TIME);
                mIsDataTransferring = false;
                wait();

            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

void RHSB::CommandFrameTransmissionThread (void)
{//{{{
    for(;;) {
        if((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
            wait();
            continue;
        }
        try {
            while (true) {
                unsigned long long int data = 0x0;
                unsigned int slave = mRHSB_Func->GetCommandTransmissionSelectedSlave();
                unsigned int bit_len = mRHSB_Func->GetCommandBitNum();
                unsigned int assertion_phase_length = mRHSB_Func->GetAssertionPhaseLength(slave);
                unsigned int deassertion_phase_length = mRHSB_Func->GetDeassertionPhaseLength(slave);
                bool chip_selection_polarity = mRHSB_Func->GetChipSelectionPolarity(slave);
                unsigned int trans_total_bit = 0x0;
                mRHSB_Func->CommandFrameAssemble(data, bit_len);
                double current_time = GetCurTime();
                wait(CalculateCLKPosEdge("FCL", current_time),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                    throw vpcl::emRstException;
                }

                ControlInterruptPorts(false, emDownCommandDMA);
                mFCLVal = mRHSB_Func->GetDownstreamBitRate();
                mFCLPortEvent.notify(1,SC_FS);
                mRHSB_Func->SetInterruptStatus("TSF");
                ControlInterruptPorts(false, emINTTSF);
                if (slave == 0) {
                    mCSD0Val = chip_selection_polarity;
                } else {
                    mCSD1Val = chip_selection_polarity;
                }
                mCSDPortEvent[slave].notify(1,SC_FS);
                mRHSB_Func->SetDownstreamActivity(0x2);
                //Wait for assertion phase
                wait(assertion_phase_length*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                    throw vpcl::emRstException;
                }
                if (mRHSB_Func->GetCurrentState() == "ACTIVE") {
                    mSOVal = data;
                    mSOPortEvent.notify(1,SC_FS);
                    mDCONTROLVal = (((mRHSB_Func->GetCLP()&0x1)<<9)|((mRHSB_Func->GetSOLP(slave)&0x1)<<8)|((bit_len&0x7F)<<1)|1);
                    mD_CONTROLPortEvent.notify(1,SC_FS);
                } else {
                    mSO2SI_val = data;
                    mD2U_CONTROLval = ((((unsigned int)mRHSB_Func->GetDownstreamBitTime()&0xFFFF)<<16)|(0x1<<6)|(trans_total_bit&0xF));
                    if (slave == 0) {
                        mTestmodeSlave0Event.notify(SC_ZERO_TIME);
                    } else { //slave == 1
                        mTestmodeSlave1Event.notify(SC_ZERO_TIME);
                    }
                }
                //Wait data length (selection bit is always added)
                wait((bit_len+1)*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                    throw vpcl::emRstException;
                }
                if (mRHSB_Func->GetCurrentState() == "ACTIVE") {
                    mSOVal = 0xFFFFFFFFFFFFFFFFULL;
                    mSOPortEvent.notify(1,SC_FS);
                } else {
                    mD2U_CONTROLval = ((((unsigned int)mRHSB_Func->GetDownstreamBitTime()&0xFFFF)<<16)|(0x2<<6)|(trans_total_bit&0xF));
                }
                //Wait for deassertion phase length
                wait(deassertion_phase_length*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                    throw vpcl::emRstException;
                }
                if (slave == 0) {
                    mCSD0Val = !chip_selection_polarity;
                 } else {
                    mCSD1Val = !chip_selection_polarity;
                }
                mCSDPortEvent[slave].notify(1,SC_FS);
                mD2U_CONTROLval = 0;
                mRHSB_Func->SetDownstreamActivity(0x3);
                if (!mRHSB_Func->IsClockSerialActive()) {
                    mFCLVal = 0;
                    mFCLPortEvent.notify(1,SC_FS);
                }
                //Wait for passive length
                wait(2*mRHSB_Func->GetDownstreamBitTime(),mTimeResolutionUnit, mRstStateEvent | mClkZeroEvent);
                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                    throw vpcl::emRstException;
                }
                mRHSB_Func->SetInterruptStatus("CTF");
                ControlInterruptPorts(false, emINTCTF);
                mRHSB_Func->SetDownstreamActivity(0x0);
                mRHSB_Func->ClearCommandTransmissionRequest();
                if (mRHSB_Func->IsUpstreamEnable() && mRHSB_Func->IsTimeOutEnable()) {
                    double counttime = CalculateCLKPosEdge("Time_out", current_time) + (mRHSB_Func->GetTimeOutCounter()*mRHSB_Func->GetCountDownTime(slave));
                    mUpstreamTimeOutEvent[slave].notify(counttime,mTimeResolutionUnit);
                    re_printf("info","Timeout counter is started in channel %d\n", slave);
                }
                mCommandFrameTransmissionCompletionEvent.notify(SC_ZERO_TIME);
                wait();

            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

///Downstream communication thread
void RHSB::DownstreamCommunicationThread (void)
{//{{{
    for(;;) {
        if((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
            wait();
            continue;
        }
        try {
            while (true) {
                if (mRHSB_Func->IsDataTransmissionEnable()) {//Data transmission is enable
                    if (mRHSB_Func->IsTriggerDownstreamMode()) {//If mode is trigger downstream
                        if (mRHSB_Func->IsCommandRequestPending()) {//command request
                            mCommandFrameTransmissionEvent.notify(SC_ZERO_TIME);
                            wait(mCommandFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                            if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                throw vpcl::emRstException;
                            }
                        }
                        if(mRHSB_Func->IsDataAvailable()) {
                            mDataFrameTransmissionEvent.notify(SC_ZERO_TIME);
                            wait(mDataFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                            if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                throw vpcl::emRstException;
                            }
                            if (mIsEmergencyAutoStop) { //Stop data transmission when its finish
                                mRHSB_Func->ClearDataTransmissionEnable();
                            }
                        }
                        if (mRHSB_Func->IsTransmissionStop()) {
                            mRHSB_Func->ClearDataTransmissionEnable();
                        }
                    } else {//Period repetition mode
                        bool is_error = false;
                        if (mRHSB_Func->IsMultiPeriodDownstreamMode()){
                            if (mRHSB_Func->GetSeqquenceLength(0) < mRHSB_Func->GetSeqquenceLength(1)){
                                re_printf("error","Sequence length setting for Period 1 should be equal to or smaller than for Period 0.\n");
                                is_error = true;
                            }
                            unsigned int data_frame = GetFrameLength(false, 1);
                            unsigned int remain_interframe = mRHSB_Func->GetRepetitionPeriodLength() - data_frame;
                            unsigned int command_frame = GetFrameLength(true, 0);
                            if (remain_interframe < command_frame) {
                                re_printf("error","Setting command frame length is longer than the inter-frame passive length of Period 1 is prohibited.\n");
                                is_error = true;
                            }
                        }
                        if (mRHSB_Func->GetRepetitionPeriodLength() < GetFrameLength(false, 0)) {
                            is_error = true;
                            re_printf("error","Repetition period setting is invalid. It should be bigger or equal to data frame length.\n");
                        }
                        if (!is_error) {
                            if (mRHSB_Func->IsMultiPeriodDownstreamMode()){//Support Best effort method only
                                if (mTimeTick == GetCurTime()) { //Send data frame
                                    mDataFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                    wait(mDataFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                    if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                        throw vpcl::emRstException;
                                    }
                                    FinishTransDataFrm();
                                }
                                if ((mRHSB_Func->GetTransmissionPeriod() == 1) && (mRHSB_Func->IsCommandRequestPending())){
                                    if (mRHSB_Func->IsCmdTransDelay()&&(mTransCmdCnt >= 1)){
                                        re_printf("info","The command frame transmission is restricted to have at least one data frame between two command frames.\n");
                                    } else {
                                        if (!mRHSB_Func->IsCommandFrameTimeOverTimeStick()) { //send command frame immediately
                                            mCommandFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                            wait(mCommandFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                            if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                                throw vpcl::emRstException;
                                            }
                                            mTransCmdCnt = mTransCmdCnt + 1;
                                        }
                                    }
                                }
                            } else { //SinglePeriodDownstreamMode
                                if (mRHSB_Func->IsTimeSlotMethod()) {
                                    if (mTimeTick == GetCurTime()) {
                                        if (mRHSB_Func->IsCommandRequestPending()) {//Command request is pending
                                            if (mRHSB_Func->IsCmdTransDelay()&&(mTransCmdCnt >= 1)){
                                                re_printf("info","The command frame transmission is restricted to have at least one data frame between two command frames.\n");
                                                mDataFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                                wait(mDataFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                                    throw vpcl::emRstException;
                                                }
                                                FinishTransDataFrm();
                                            } else {
                                                mCommandFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                                wait(mCommandFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                                    throw vpcl::emRstException;
                                                }
                                                mTransCmdCnt = mTransCmdCnt + 1;
                                            }
                                        } else {
                                            mDataFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                            wait(mDataFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                            if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                                throw vpcl::emRstException;
                                            }
                                            FinishTransDataFrm();
                                        }
                                    }
                                } else {
                                    bool rqtCmdinDataFrame = false;
                                    if (mTimeTick == GetCurTime()) { //Send data frame
                                        mDataFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                        wait(mDataFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                            throw vpcl::emRstException;
                                        }
                                        FinishTransDataFrm();
                                        if (mRHSB_Func->IsCommandRequestPending()){
                                            rqtCmdinDataFrame = true;
                                        }
                                    }
                                    if (mRHSB_Func->IsCommandRequestPending()){
                                        if (mRHSB_Func->IsCmdTransDelay()&&(mTransCmdCnt >= 1)){
                                            re_printf("info","The command frame transmission is restricted to have at least one data frame between two command frames.\n");
                                        } else {
                                            if (mRHSB_Func->IsImmediateMethod()) {
                                                if (mRHSB_Func->IsCommandFrameTimeOverTimeStick()) {
                                                    StartStopTimer(false);
                                                    mTimeTick = 0;
                                                }
                                                mCommandFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                                wait(mCommandFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                                    throw vpcl::emRstException;
                                                }
                                                mTransCmdCnt = mTransCmdCnt + 1;
                                                if (!mIsTimerRunning){
                                                    StartStopTimer(true);
                                                }
                                            } else {
                                                if (mRHSB_Func->IsCommandFrameTimeOverTimeStick()) { //send command frame immediately
                                                    if (rqtCmdinDataFrame){
                                                        mCommandFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                                        wait(mCommandFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                                            throw vpcl::emRstException;
                                                        }
                                                        mDataFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                                        wait(mDataFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                                            throw vpcl::emRstException;
                                                        }
                                                        FinishTransDataFrm();
                                                    }
                                                } else {
                                                    mCommandFrameTransmissionEvent.notify(SC_ZERO_TIME);
                                                    wait(mCommandFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                                                    if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                                        throw vpcl::emRstException;
                                                    }
                                                    mTransCmdCnt = mTransCmdCnt + 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if (mTimeTick != GetCurTime()) {
                                wait(mRepetitionPeriodExpireEvent|mContCmdEvent|mStopDataTransEvent|mRstStateEvent| mClkZeroEvent);
                                if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                                    throw vpcl::emRstException;
                                }
                            }
                            if ((mIsEmergencyAutoStop && (mEmergencyFrameCnt == 1) && (mTimeTick == GetCurTime())) || (mRHSB_Func->IsTransmissionStop())) {
                                mRHSB_Func->ClearDataTransmissionEnable();
                                mEmergencyFrameCnt = 0;
                                StartStopTimer(false);
                                mTimeTick = 0;
                            }
                        } else {
                            mRHSB_Func->ClearDataTransmissionEnable();
                            mEmergencyFrameCnt = 0;
                            StartStopTimer(false);
                            mTimeTick = 0;
                        }
                    }
                } else {//Data transmission is disable
                    if (mRHSB_Func->IsCommandRequestPending()) {
                        mCommandFrameTransmissionEvent.notify(SC_ZERO_TIME);
                        wait(mCommandFrameTransmissionCompletionEvent|mRstStateEvent | mClkZeroEvent);
                        if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                            throw vpcl::emRstException;
                        }
                    }
                }
                if (!(mRHSB_Func->IsCommandRequestPending() ||
                     ((mTimeTick == GetCurTime()) && mRHSB_Func->IsDataTransmissionEnable() && !(mRHSB_Func->IsTriggerDownstreamMode())) ||
                     (mRHSB_Func->IsDataAvailable() && mRHSB_Func->IsDataTransmissionEnable() && mRHSB_Func->IsTriggerDownstreamMode()))){
                    wait();
                    if ((mRHSB_Func->IsResetState()) || (!CheckClockPeriod("CLK_RHSB"))) {
                        throw vpcl::emRstException;
                    }
                }
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

///Initialize variables at initial state or when reset is active
void RHSB::Initialize (void)
{//{{{
    for (unsigned int index = 0; index < emInterruptNum; index++) {
        mIntrValue[index] = false;
    }
    for (unsigned int index = 0; index < emSlaveChannelNum; index++) {
        mPreSTBE[index] = 0x0;
        mCurSTBE[index] = 0x0;
        mReceptionData[index] = 0x0;
        mStartReceptionTime[index] = 0x0;
        mBITT[index] = 0x0;
        mSIZE[index] = 0x0;
    }
    mCurrentReceptionChannel = 0;
    mIsDataTransferring = false;
    mIsDataReceiving = false;
    mIsRX_Valid = false;
    mIsRX_FERR = false;
    for (unsigned int index = 0; index < 4; index++) {
        mData[index] = 0x0;
    }
    mIsTimerRunning = false;
    mTimeTick = 0;
    mIsEmergencyEnabled = false;
    mIsEmergencyAutoStop = false;
    mIsEmergencyData = false;
    mEmergencyFrameCnt = 0;
    mTransCmdCnt = 0;
    mIsFstData = false;
    for (unsigned int index = 0; index < 3; index++) {
        mIsDMARequest[index] = false;
    }
    mSO2SI_val = 0;
    mD2U_CONTROLval = 0;
    mFCLVal = 0;
    mDCONTROLVal =0 ;
    mSOVal= 0;
    mCSD0Val = false;
    mCSD1Val = false;
}//}}}

///Reset output port when reset is active
void RHSB::ResetOutputPort (void)
{//{{{
    // Reset port FCL
    mFCLVal = 0;
    mFCLPortEvent.notify(1,SC_FS);
    // Reset port D_CONTROL
    mDCONTROLVal = 0;
    mD_CONTROLPortEvent.notify(1,SC_FS);
    // Reset port SO
    mSOVal = 0xFFFFFFFFFFFFFFFFULL;
    mSOPortEvent.notify(1,SC_FS);
    // Reset port CSD0
    mCSD0Val = false;
    mCSDPortEvent[0].notify(1,SC_FS);
    // Reset port CSD1
    mCSD1Val = false;
    mCSDPortEvent[1].notify(1,SC_FS);
    // Reset port INTDTSF
    ControlInterruptPorts(true, emINTDTSF);
    // Reset port INTDTF
    ControlInterruptPorts(true, emINTDTF);
    // Reset port INTCTF
    ControlInterruptPorts(true, emINTCTF);
    // Reset port INTTSF
    ControlInterruptPorts(true, emINTTSF);
    // Reset port INTL2
    ControlInterruptPorts(true, emINTL2);
    // Reset port INTL3
    ControlInterruptPorts(true, emINTL3);
    // Reset port INTUEF
    ControlInterruptPorts(true, emINTUEF);
    // Reset port INTTOF
    ControlInterruptPorts(true, emINTTOF);
    // Reset port INTDLF
    ControlInterruptPorts(true, emINTDLF);
    // Reset port INTDWNDATADMA
    ControlInterruptPorts(true, emDownDataDMA);
    // Reset port INTDWNCMDDMA
    ControlInterruptPorts(true, emDownCommandDMA);
    // Reset port INTUPDMA
    ControlInterruptPorts(true, emUpDMA);
}//}}}

///Check frequency value and frequency unit
void RHSB::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out
                            ,sc_dt::uint64 freq_in, std::string unit_in)
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

///Get time resolution
void RHSB::GetTimeResolution (sc_dt::uint64 &resolution_value
                             ,sc_time_unit &resolution_unit)
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

///Separate some words from a string to store a vector
void RHSB::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

///Calculate synchronous time with CLK_LSB or CLK_RHSB
double RHSB::CalculateCLKPosEdge (const std::string clock_name, const double time_point)
{//{{{
    //Calculate the time point of next raising edge of CLK_LSB
    double period_num = 0;
    unsigned int slave = mRHSB_Func->GetCommandTransmissionSelectedSlave();
    if (clock_name == "CLK_LSB") {
        period_num = time_point / (double)mCLK_LSBPeriod;
    } else if (clock_name == "CLK_RHSB"){//CLK_RHSB
        period_num = time_point / (double)mCLK_RHSBPeriod;
    } else if (clock_name == "FCL"){//FCL
        period_num = time_point / mRHSB_Func->GetDownstreamBitTime();
    } else {//Time_out
        period_num = time_point / mRHSB_Func->GetCountDownTime(slave);
    }
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "CLK_LSB") {
        pos_edge_point = (double)mod_period_num * (double)mCLK_LSBPeriod - time_point;
    } else if (clock_name == "CLK_RHSB"){//CLK_RHSB
        pos_edge_point = (double)mod_period_num * (double)mCLK_RHSBPeriod - time_point;
    } else if (clock_name == "FCL"){//FCL
        pos_edge_point = (double)mod_period_num * mRHSB_Func->GetDownstreamBitTime() - time_point;
    } else {//Time_out
        pos_edge_point = (double)mod_period_num * mRHSB_Func->GetCountDownTime(slave) - time_point;
    }
    return pos_edge_point;
}//}}}

///Process reset function
void RHSB::EnableReset (const bool is_active)
{//{{{
    mRHSB_Func->EnableReset(is_active);
    if (is_active) {
        mRstStateEvent.notify(SC_ZERO_TIME);
        CancelEvents();
        ResetOutputPort();
        Initialize();
        StartStopTimer(false);
    }
}//}}}

///Return CLK_RHSB period
sc_dt::uint64 RHSB::GetCLK_RHSBPeriod (void)
{//{{{
    return mCLK_RHSBPeriod;
}//}}}

///Return CLK_RHSB frequency
sc_dt::uint64 RHSB::GetCLK_RHSBFreq (void)
{//{{{
    return mCLK_RHSBFreq;
}//}}}

///Check clock period of CLK_LSB, CLK_RHSB
bool RHSB::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool status = false;
    if (clock_name == "CLK_RHSB") {
        if (mCLK_RHSBPeriod > 0) {
            status = true;
        }
    }
    if (!status) {
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return status;
}//}}}

/// Cancel operation events
void RHSB::CancelEvents (void)
{//{{{
    for (unsigned int channel = 0; channel < emSlaveChannelNum; channel++) {
        mReceptionDataEvent[channel].cancel();
        mUpstreamTimeOutEvent[channel].cancel();
    }
    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        mIntrUpdateEvent[interrupt_id].cancel();
    }
    mStoreReceptionEvent.cancel();
    mDownstreamTransmissionEvent.cancel();
    mDataFrameTransmissionEvent.cancel();
    mCommandFrameTransmissionEvent.cancel();
    mDataFrameTransmissionCompletionEvent.cancel();
    mCommandFrameTransmissionCompletionEvent.cancel();
    mRepetitionPeriodExpireEvent.cancel();
}//}}}

///Notify data transmission
void RHSB::NotifyDownstreamTransmission (void)
{//{{{
    mDownstreamTransmissionEvent.notify(SC_ZERO_TIME);
}//}}}

///Notify data transmission
void RHSB::NotifyDMAResponse (void)
{//{{{
    
    for (unsigned int index = 0; index < 3; index++) {
        if (mIsDMARequest[index]) {
            mIsDMARequest[index] = false;
        }
    }
}//}}}

void RHSB::NotifyStopDataTrans (void)
{//{{{
    mStopDataTransEvent.notify(SC_ZERO_TIME);
}//}}}

void RHSB::NotifyCmdRequest (void)
{//{{{
    if (!mRHSB_Func->IsTimeSlotMethod()) {
        mContCmdEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Prepare transmission data
void RHSB::PrepareData (sc_dt::uint64 &output_data, const unsigned short input_data, const unsigned int pos, const unsigned int bit_len)
{//{{{
   unsigned short bit_mask = 0xFFFF;
   sc_dt::uint64 data = 0x0;
   if (bit_len != 16) {
       bit_mask = (0x1<<bit_len) - 1;
   }
   data = (sc_dt::uint64)(input_data & bit_mask);
   output_data = output_data | (data << pos);
}//}}}

///Get value from timer input port
sc_dt::uint64 RHSB::GetTimerInputPort (void)
{//{{{
    sc_dt::uint64 timer_input_port_value = tmr.read();
    return timer_input_port_value;
}//}}}

///Write FCL port
void RHSB::HandleFCLPortMethod ()
{//{{{
    FCL.write(mFCLVal);
}//}}}

///Write D_CONTROL port
void RHSB::HandleD_CONTROLPortMethod ()
{//{{{
    D_CONTROL.write(mDCONTROLVal);
}//}}}

///Write SO port
void RHSB::HandleSOPortMethod ()
{//{{{
    SO.write(mSOVal);
}//}}}

///Write CSD0, CSD1 port
void RHSB::HandleCSDPortMethod (const unsigned int slave_num)
{//{{{
    if (slave_num == emSlave0Channel) {
        CSD0.write(mCSD0Val);
    } else {
        CSD1.write(mCSD1Val);
    }
}//}}}

///Control interrupt ports
void RHSB::ControlInterruptPorts (const bool is_reset, const unsigned int interrupt_id)
{//{{{
    bool value = false;
    switch (interrupt_id) {
        case emINTDTSF:
            if (!is_reset && mRHSB_Func->IsInterruptEnable("DTSIE") && mRHSB_Func->IsInterruptStatusActive("DTSF")) {
                value = true;
            }
            break;

        case emINTDTF:
            if (!is_reset && mRHSB_Func->IsInterruptEnable("DTIE") && mRHSB_Func->IsInterruptStatusActive("DTF")) {
                value = true;
            }
            break;

        case emINTCTF:
            if (!is_reset && mRHSB_Func->IsInterruptEnable("CTIE") && mRHSB_Func->IsInterruptStatusActive("CTF")) {
                value = true;
            }
            break;
        case emINTTSF:
            if (!is_reset && mRHSB_Func->IsInterruptEnable("TSIE") && mRHSB_Func->IsInterruptStatusActive("TSF")) {
                value = true;
            }
            break;

        case emINTL2:
            if (!is_reset && mRHSB_Func->IsInterruptEnable("ETIE") && mRHSB_Func->IsInterruptStatusActive("ETF")) {
                value = true;
            }
            break;

        case emINTL3:
            if (!is_reset && mRHSB_Func->IsInterruptEnable("DRIE") && mRHSB_Func->IsInterruptStatusActive("DRF")) {
                value = true;
            }
            break;

        case emINTUEF:
            if (!is_reset && mRHSB_Func->IsInterruptEnable("UEIE") && mRHSB_Func->IsInterruptStatusActive("UEF")) {
                value = true;
            }
            break;

        case emINTTOF:
            if (!is_reset && mRHSB_Func->IsInterruptEnable("TOIE") && mRHSB_Func->IsInterruptStatusActive("TOF")) {
                value = true;
            }
            break;

        case emINTDLF:
            if (!is_reset && mRHSB_Func->IsInterruptEnable("DLIE") && mRHSB_Func->IsInterruptStatusActive("DLF")) {
                value = true;
            }
            break;

        case emDownDataDMA:
            if (!is_reset && mRHSB_Func->IsDownstreamDataDMAEnable() && !(mIsDMARequest[0])) {
                value = true;
                mIsDMARequest[0] = true;
            }
            break;
        case emDownCommandDMA:
            if (!is_reset && mRHSB_Func->IsDownstreamCommandDMAEnable() && !(mIsDMARequest[1])) {
                value = true;
                mIsDMARequest[1] = true;
            }
            break;
        default: // emUpDMA
            if (!is_reset && mRHSB_Func->IsUpstreamDMAEnable() && !(mIsDMARequest[2])) {
                value = true;
                mIsDMARequest[2] = true;
            }
    }
    mIntrValue[interrupt_id] = value;
    if (mCLK_LSBPeriod > 0) {
        mIntrUpdateEvent[interrupt_id].notify(CalculateCLKPosEdge("CLK_LSB", GetCurTime()), mTimeResolutionUnit);
    } else {
        mIntrUpdateEvent[interrupt_id].notify(SC_ZERO_TIME);
    }
}//}}}

///Dump interrupt message
void RHSB::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mDumpInterrupt) {
        if (value) {
            re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
        } else {
            re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
        }
   }
}//}}}

///Set counter period
void RHSB::SetCounterPeriod (const double counter_period)
{//{{{
    Cgeneral_timer::setClkCountPeriod(counter_period);
}//}}}

///Get counter value
unsigned int RHSB::GetCounterValue (void)
{//{{{
    return (Cgeneral_timer::getCNT());
}//}}}

unsigned int RHSB::GetFrameLength (const bool is_cmd_frame, const unsigned int period_num)
{//{{{
    unsigned int sequence_length = 0;
    unsigned int slave = 0;
    unsigned int CPS_length = 0;
    unsigned int assertion_phase_length = 0;
    unsigned int deassertion_phase_length = 0;
    unsigned int bit_len = 0;
    unsigned int frame_length = 0;
    if (is_cmd_frame) {
        slave = mRHSB_Func->GetCommandTransmissionSelectedSlave();
        assertion_phase_length = mRHSB_Func->GetAssertionPhaseLength(slave);
        deassertion_phase_length = mRHSB_Func->GetDeassertionPhaseLength(slave);
        bit_len = mRHSB_Func->GetCommandBitNum();
        frame_length = assertion_phase_length + 1 + bit_len + deassertion_phase_length + 2;
    } else {
        sequence_length = mRHSB_Func->GetSeqquenceLength(period_num);
        slave = mRHSB_Func->GetDataTransmissionSelectedSlave(0);
        CPS_length = 0x0;
        assertion_phase_length = 0x0;
        deassertion_phase_length = 0x0;
        bit_len = 0x0;
        frame_length = 0x0;
        for (unsigned int dfte = 0; dfte <= sequence_length; dfte++) {
            bit_len = mRHSB_Func->GetDataBitNum(dfte);
            frame_length = frame_length + bit_len;
            if(slave != mRHSB_Func->GetDataTransmissionSelectedSlave(dfte)){ //Change slave

                assertion_phase_length = mRHSB_Func->GetAssertionPhaseLength(slave);
                CPS_length = mRHSB_Func->GetCPS(slave);
                deassertion_phase_length = mRHSB_Func->GetDeassertionPhaseLength(slave);
                frame_length = frame_length + assertion_phase_length + CPS_length + deassertion_phase_length;
                slave = mRHSB_Func->GetDataTransmissionSelectedSlave(dfte);
            }
            if(dfte == sequence_length){ //last slave
                assertion_phase_length = mRHSB_Func->GetAssertionPhaseLength(slave);
                CPS_length = mRHSB_Func->GetCPS(slave);
                deassertion_phase_length = mRHSB_Func->GetDeassertionPhaseLength(slave);
                frame_length = frame_length + assertion_phase_length + CPS_length + deassertion_phase_length;
            }
        }
        frame_length = frame_length + mRHSB_Func->GetPassivePhaseLength();
    }
    return frame_length;
}//}}}

///Set compare match value
void RHSB::SetCompareMatchValue (const unsigned int match_value)
{//{{{
    Cgeneral_timer::setCOR(match_value);
}//}}}

///Start/stop timer
void RHSB::StartStopTimer (const bool is_start)
{//{{{
    double current_time = GetCurTime();
    mIsTimerRunning = is_start;
    if (mIsTimerRunning && ((mCLK_RHSBPeriod == 0) || (mCLK_LSBPeriod == 0))) {
        mIsTimerRunning = false;
        re_printf("warning","Timer cannot start due to zero clock\n");
    }
    if (mIsTimerRunning) {
        mStartTimerEvent.notify(CalculateCLKPosEdge("FCL", current_time),mTimeResolutionUnit);
    } else {
        mStartTimerEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Implementation of virtual function of Cgeneral_timer class
void RHSB::cmpMatchOutputHandling (bool value)
{//{{{
    if (CheckClockPeriod("CLK_RHSB")&& mIsTimerRunning){
        if (mRHSB_Func->IsMultiPeriodDownstreamMode()) {
            mRHSB_Func->SwitchTransmissionPeriod();
            mIsFstData = false;
        }
        mTimeTick = GetCurTime();
        mRepetitionPeriodExpireEvent.notify(SC_ZERO_TIME);
        NotifyDownstreamTransmission();
    }
}//}}}

void RHSB::StartTimerMethod (void)
{//{{{
    Cgeneral_timer::setSTR(mIsTimerRunning);
    if (mIsTimerRunning) {
        Cgeneral_timer::setCNT(0);
        mTimeTick = GetCurTime();
        mRepetitionPeriodExpireEvent.notify(SC_ZERO_TIME);
        NotifyDownstreamTransmission();
    }
}//}}}

void RHSB::ACCChangeDoneMethod (void)
{//{{{
    mRHSB_Func->FinishConfChange();
}//}}}

void RHSB::ACCChangeDone(void)
{//{{{
    double current_time = GetCurTime();
    if ((mCLK_RHSBPeriod != 0) && (mCLK_LSBPeriod != 0)) {
        mACCChangeDoneEvent.notify(CalculateCLKPosEdge("FCL", current_time),mTimeResolutionUnit);
    }
}//}}}

double RHSB::GetCurTime(void)
{//{{{
    return sc_time_stamp().to_double();
}//}}}

void RHSB::FinishTransDataFrm(void)
{//{{{
    mTransCmdCnt = 0;
    if (mRHSB_Func->IsTransmissionStop()) {
        mRHSB_Func->ClearDataTransmissionEnable();
        mEmergencyFrameCnt = 0;
        StartStopTimer(false);
        mTimeTick = 0;
    }
}//}}}
// vim600: set foldmethod=marker :
