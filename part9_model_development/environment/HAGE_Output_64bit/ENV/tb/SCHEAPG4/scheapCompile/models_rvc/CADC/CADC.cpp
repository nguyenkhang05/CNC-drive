// ---------------------------------------------------------------------
// $Id: CADC.cpp,v 1.2 2018/11/20 08:01:49 chanle Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "CADC.h"
#include "CADC_Func.h"
#include "CADC_UNIT.h"
#include "CADC_UNIT_Func.h"

///Constructor of CADC class
CADC::CADC (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency,
            unsigned int ch_num):
    sc_module(name),
    CADC_AgentController(),
    BusSlaveBase<32,1>(),
    CLK_LSB("CLK_LSB"),
    CLK_HSB("CLK_HSB"),
    PRESETn("PRESETn"),
    resetad_n("resetad_n"),
    // Input ports
    ADSVREFH("ADSVREFH"),
    ADSVREFL("ADSVREFL"),
    CAN000P("CAN000P"),
    CAN000N("CAN000N"),
    CAN001P("CAN001P"),
    CAN001N("CAN001N"),
    CAN002P("CAN002P"),
    CAN002N("CAN002N"),
    CAN003P("CAN003P"),
    CAN003N("CAN003N"),
    CADTRG00("CADTRG00"),
    CADRGT00("CADRGT00"),
    // Input ports
    CADEND00("CADEND00"),
    CADI00("CADI00"),
    CADE00("CADE00"),
    CADPE00("CADPE00"),
    CADDFREQ000("CADDFREQ000"),
    CADDFTAG00("CADDFTAG00"),
    CADDFDATA00("CADDFDATA00"),
    CADUE00("CADUE00"),
    CADLE00("CADLE00")
{//{{{
    //Bind clock, reset signal and socket to Bus IF
    setSlaveFreqPort32(&CLK_LSB);
    setSlaveResetPort32(&PRESETn);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mCADC_Func = new CADC_Func((std::string)name, this);
    sc_assert(mCADC_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mCADC_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    std::ostringstream port_name;
    for (unsigned int index = 0; index < emVCNum; index++) {
        port_name.str("");
        port_name << "CADCVCI00" << index;
        CADCVCI00[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(CADCVCI00[index] != NULL);
    }
    
    mAD_Unit[emAD00] = new CADC_UNIT("AD00", this, emAD00, ch_num);
    for (unsigned int i = 0; i < emADNum; i++) {
        sc_assert(mAD_Unit[i] != NULL);
    }
    
    // Port connection
    for (unsigned int i = 0; i < emADNum; i++) {
        (mAD_Unit[i]->ADSVREFH)(ADSVREFH);
        (mAD_Unit[i]->ADSVREFL)(ADSVREFL);
    }
     
    (*(mAD_Unit[emAD00]->AN[0]))(CAN000P);
    (*(mAD_Unit[emAD00]->AN[1]))(CAN000N);
    (*(mAD_Unit[emAD00]->AN[2]))(CAN001P);
    (*(mAD_Unit[emAD00]->AN[3]))(CAN001N);
    (*(mAD_Unit[emAD00]->AN[4]))(CAN002P);
    (*(mAD_Unit[emAD00]->AN[5]))(CAN002N);
    (*(mAD_Unit[emAD00]->AN[6]))(CAN003P);
    (*(mAD_Unit[emAD00]->AN[7]))(CAN003N);
    
    (mAD_Unit[emAD00]->CADTRG)(CADTRG00);
    
    (mAD_Unit[emAD00]->CADRGT)(CADRGT00);
    
    (mAD_Unit[emAD00]->CADEND)(CADEND00);
    
    (mAD_Unit[emAD00]->CADI)(CADI00);
    
    (mAD_Unit[emAD00]->CADE)(CADE00);
    
    (mAD_Unit[emAD00]->CADPE)(CADPE00);
    
    (mAD_Unit[emAD00]->CADDFREQ)(CADDFREQ000);
    
    (mAD_Unit[emAD00]->CADDFTAG)(CADDFTAG00);
    
    (mAD_Unit[emAD00]->CADDFDATA)(CADDFDATA00);
    
    (mAD_Unit[emAD00]->CADUE)(CADUE00);
    
    (mAD_Unit[emAD00]->CADLE)(CADLE00);
    
    for (unsigned int i = 0; i < emVCNum; i++) {
        (*(mAD_Unit[emAD00]->CADCVCI[i]))(*(CADCVCI00[i]));
    }

    //Initialize variables
    CADEND00.initialize(0);
    CADI00.initialize(0);
    CADE00.initialize(0);
    CADPE00.initialize(0);
    CADDFREQ000.initialize(0);
    CADDFTAG00.initialize(0);
    CADDFDATA00.initialize(0);
    CADUE00.initialize(0);
    CADLE00.initialize(0);
    for (unsigned int i = 0; i < emVCNum; i++) {
        CADCVCI00[i]->initialize(0);
    }

    mCLK_LSBPeriod = 0;
    mCLK_LSBFreq = 0;
    mCLK_LSBOrgFreq = 0;
    mCLK_LSBFreqUnit = "Hz";
    mCLK_HSBPeriod = 0;
    mCLK_HSBFreq = 0;
    mCLK_HSBOrgFreq = 0;
    mCLK_HSBFreqUnit = "Hz";
    mM16Period = 0;
    mM16Freq = 0;
    mM16OrgFreq = 0;
    mM16FreqUnit = "Hz";

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

    mLSBSyncTPoint = 0;
    mHSBSyncTPoint = 0;
    mM16SyncTPoint = 0;
    mDumpInterrupt = false;
    mADSVCC = 5;
    mADSVSS = 0;
    mtW = 25;
    mtR = 20;
    mtCLB = 100;
    mtCoeff = 1;
    mtDFAver = 48.5;
    mtSAver = 20;
    mtDFNotAver = 68.5;
    mtSNotAver = 40;
    for (unsigned int i = 0; i < emADNum; i++) {
        mParityError[i] = false;
        mIDError[i] = false;
        mDisconnectError[i] = false;
    }

    //Initialize variables - for parameters setting by Heap configure and Python IF
    if ((ch_num >= 1) && (ch_num <= 8)) {
        mCHNUM = ch_num;
    } else {
        re_printf("warning","The argument (%d) of CADC_CHNUM is invalid. (8) is set as default.\n",ch_num);
        mCHNUM = 8;
    }

    mAdIdMap.insert(std::pair<std::string, unsigned int>("ad_00", emAD00));
    
    SC_METHOD(HandleCLK_LSBSignalMethod);
    dont_initialize();
    sensitive << CLK_LSB;
    
    SC_METHOD(HandleCLK_HSBSignalMethod);
    dont_initialize();
    sensitive << CLK_HSB;

    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;
    
    SC_METHOD(HandleResetad_nSignalMethod);
    sensitive << resetad_n;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&CADC::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&CADC::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&CADC::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
}//}}}

///Destructor of CADC class
CADC::~CADC (void)
{//{{{
    delete mCADC_Func;
    for (unsigned int i = 0; i < emADNum; i++) {
        delete mAD_Unit[i];
    }
}//}}}

///Assert reset by software
void CADC::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[0])&&(!mIsResetCmdReceive[0])) {
            mIsResetCmdReceive[0] = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[0].notify(start_time, SC_NS);
            mResetCmdPeriod[0] = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "resetad_n") {
        if ((!mIsResetHardActive[1])&&(!mIsResetCmdReceive[1])) {
            mIsResetCmdReceive[1] = true;
            re_printf("info","The model will be reset (resetad_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[1].notify(start_time, SC_NS);
            mResetCmdPeriod[1] = period;
        } else {
            re_printf("warning","The software reset of resetad_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn or resetad_n\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit
void CADC::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            mCLK_LSBOrgFreq = freq;
            ConvertClockFreq (mCLK_LSBFreq, mCLK_LSBFreqUnit, freq, unit);
            if (mCLK_LSBFreq > 0) {
                mCLK_LSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_LSBFreq)*(double)mTimeResolutionValue) + 0.5);
                mLSBSyncTPoint = sc_time_stamp().to_double();
            } else {
                mCLK_LSBPeriod = 0;
                mLSBSyncTPoint = sc_time_stamp().to_double();
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[0].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "CLK_HSB") {
            mCLK_HSBOrgFreq = freq;
            ConvertClockFreq (mCLK_HSBFreq, mCLK_HSBFreqUnit, freq, unit);
            if (mCLK_HSBFreq > 0) {
                mCLK_HSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_HSBFreq)*(double)mTimeResolutionValue) + 0.5);
                mHSBSyncTPoint = sc_time_stamp().to_double();
            } else {
                mCLK_HSBPeriod = 0;
                mHSBSyncTPoint = sc_time_stamp().to_double();
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[1].notify(SC_ZERO_TIME);
            }
            mM16OrgFreq = freq;
            ConvertClockFreq (mM16Freq, mM16FreqUnit, freq, unit);
            if (mM16Freq > 0) {
                mM16Period = mCLK_HSBPeriod * 5;
                mM16SyncTPoint = sc_time_stamp().to_double();
            } else {
                mM16Period = 0;
                mM16SyncTPoint = sc_time_stamp().to_double();
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Get clock value
void CADC::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            re_printf("info","CLK_LSB frequency is %0.0f %s\n", (double)mCLK_LSBOrgFreq, mCLK_LSBFreqUnit.c_str());
        } else if (word_vector[0] == "CLK_HSB") {
            re_printf("info","CLK_HSB frequency is %0.0f %s\n", (double)mCLK_HSBOrgFreq, mCLK_HSBFreqUnit.c_str());
        } else if (word_vector[0] == "CYCLK16M") {
            re_printf("info","CYCLK16M frequency is %0.0f %s\n", (double)mM16OrgFreq, mM16FreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}


///Force value to register
void CADC::ForceRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandlerWrapper(is_com_reg, ad_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void CADC::ReleaseRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandlerWrapper(is_com_reg, ad_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void CADC::WriteRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandlerWrapper(is_com_reg, ad_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void CADC::ReadRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandlerWrapper(is_com_reg, ad_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/// Wrapper for accessing registers
void CADC::RegisterHandlerWrapper (const bool is_com_reg, const std::string ad_id, const std::vector<std::string> cmd)
{//{{{
    if (is_com_reg) {
        mCADC_Func->RegisterHandler(cmd);
    } else {
        std::map<std::string, unsigned int>::iterator it;
        bool is_match = false;
        for (it = mAdIdMap.begin(); it != mAdIdMap.end(); it++) {
            if (it->first == ad_id) {
                is_match = true;
                mAD_Unit[it->second]->mCADC_UNIT_Func->RegisterHandler(cmd);
                break;
            }
        }
        if (!is_match) {
            re_printf("warning","AD id (%s) is invalid\n", ad_id.c_str());
        }
    }
}//}}}

///List all registers name
void CADC::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mCADC_Func->RegisterHandler(cmd);
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->mCADC_UNIT_Func->RegisterHandler(cmd);
    }
}//}}}

///Dump help message of all parameters or commands
void CADC::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"CADC_MessageLevel (fatal|error|warning|info)       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"CADC_DumpRegisterRW (true/false)                   Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(),"CADC_DumpInterrupt (true/false)                    Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(),"CADC_ADSVCC (value)                                Set voltage value of ADSVCC.");
            SC_REPORT_INFO(this->basename(),"CADC_ADSVSS (value)                                Set voltage value of ADSVSS.");
            SC_REPORT_INFO(this->basename(),"CADC_tW (value)                                    Set Internal stabilization time (number of CLK_LSB cycle).");
            SC_REPORT_INFO(this->basename(),"CADC_tSD (value)                                   Set Virtual channel switching delay time (number of Over-sampling cycle).");
            SC_REPORT_INFO(this->basename(),"CADC_tCLB (value)                                  Set Calibration processing time (number of CLK_LSB cycle).");
            SC_REPORT_INFO(this->basename(),"CADC_tCoeff (value)                                Set correction coefficients obtained in calibration.");
            SC_REPORT_INFO(this->basename(),"CADC_tDFAver (value)                               Set processing delay time with averaging.");
            SC_REPORT_INFO(this->basename(),"CADC_tDFNotAver (value)                            Set processing delay time without averaging");
            SC_REPORT_INFO(this->basename(),"CADC_tSAver (value)                                Set sampling period with averaging");
            SC_REPORT_INFO(this->basename(),"CADC_tSNotAver (value)                             Set sampling period without averaging");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"CADC_AssertReset (reset_name, start_time, period)  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"CADC_SetCLKFreq (clock_name, freq, unit)           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"CADC_GetCLKFreq (clock_name)                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"CADC_ForceRegister (reg_name, reg_value)           Force CADC’s common register with setting value.");
            SC_REPORT_INFO(this->basename(),"CADC_ReleaseRegister (reg_name)                    Release CADC’s common register from force value.");
            SC_REPORT_INFO(this->basename(),"CADC_WriteRegister (reg_name, reg_value)           Write a value to a CADC’s common register.");
            SC_REPORT_INFO(this->basename(),"CADC_ReadRegister (reg_name)                       Read a value from a CADC’s common register.");
            SC_REPORT_INFO(this->basename(),"CADC_ListRegister ()                               Dump name of model's registers.");
            SC_REPORT_INFO(this->basename(),"CADC_SetParityError (ad_num, value)                Set parity error occurrence for each AD unit.");
            SC_REPORT_INFO(this->basename(),"CADC_SetIDError (ad_num, value)                    Set ID error occurrence for each AD unit.");
            SC_REPORT_INFO(this->basename(),"CADC_SetDisconnectError (ad_num, value)            Set input disconnection error occurrence for each AD unit.");
            SC_REPORT_INFO(this->basename(),"CADC_UNIT_ForceRegister (ad_num, reg_name, value)  Force CADC unit’s specific register with setting value.");
            SC_REPORT_INFO(this->basename(),"CADC_UNIT_ReleaseRegister (ad_num, reg_name)       Release CADC unit’s specific register from force value.");
            SC_REPORT_INFO(this->basename(),"CADC_UNIT_WriteRegister (ad_num, reg_name, value)  Write a value to CADC unit’s specific register.");
            SC_REPORT_INFO(this->basename(),"CADC_UNIT_ReadRegister (ad_num, reg_name)          Read value from CADC unit’s specific register.");
        } else {
            re_printf("warning","The name (%s) of CADC_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of CADC_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void CADC::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    for (unsigned int i = 0; i < emADNum; i++) {
        std::string msg_ret = mAD_Unit[i]->handleCommand(cmd);
        if (msg_ret != "") {
            SC_REPORT_INFO(mAD_Unit[i]->basename(), msg_ret.c_str());
        }
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mCADC_Func->RegisterHandler(cmd);
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->mCADC_UNIT_Func->RegisterHandler(cmd);
    }

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mCADC_Func->RegisterHandler(cmd);
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->mCADC_UNIT_Func->RegisterHandler(cmd);
    }
}//}}}

///Enable/disable dumping message when users access registers
void CADC::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mCADC_Func->RegisterHandler(cmd);
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->mCADC_UNIT_Func->RegisterHandler(cmd);
    }
}//}}}

///Enable/disable dumping interrupt information
void CADC::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: CADC_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","CADC_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: CADC_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Check frequency value and frequency unit
void CADC::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

///Separate some words from a string to store a vector
void CADC::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

///Get time resolution
void CADC::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///Calculate synchronous time
double CADC::CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point)
{//{{{
    double period_num = 0;
    double SyncTpoint = 0;
    if (clock_name == "CLK_LSB") {
        SyncTpoint = mLSBSyncTPoint;
        period_num = (time_point-SyncTpoint) / (double)mCLK_LSBPeriod;
    } else if (clock_name == "CLK_HSB") {
        SyncTpoint = mHSBSyncTPoint;
        period_num = (time_point-SyncTpoint) / (double)mCLK_HSBPeriod;
    } else {//clock_name is CYCLK16M
        SyncTpoint = mM16SyncTPoint;
        period_num = (time_point-SyncTpoint) / (double)mM16Period;
    }
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
    double edge_point = 0, mod_period_num_db = 0;
    if (is_pos) {
        mod_period_num_db = (double)mod_period_num;
    } else {
        mod_period_num_db = (double)mod_period_num + 0.5;
    }
    if (clock_name == "CLK_LSB") {
        edge_point = mod_period_num_db * (double)mCLK_LSBPeriod - (time_point-SyncTpoint);
    } else if (clock_name == "CLK_HSB") {
        edge_point = mod_period_num_db * (double)mCLK_HSBPeriod - (time_point-SyncTpoint);
    } else {//clock_name is CYCLK16M
        edge_point = mod_period_num_db * (double)mM16Period - (time_point-SyncTpoint);
    }
    return edge_point;
}//}}}

///Check reset status
bool CADC::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "resetad_n") {
        if (mIsResetHardActive[1] || mIsResetCmdActive[1]) {
            reset_status = true;
        }
    } else {    // PRESETn
        if (mIsResetHardActive[0] || mIsResetCmdActive[0]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///Check clock period value
bool CADC::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "CLK_LSB") ? mCLK_LSBPeriod : mCLK_HSBPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

/// Set Vss/Vcc voltage
void CADC::SetVoltage (const std::string name, const double value)
{//{{{
    if (name == "ADSVCC") {
        mADSVCC = value;
    } else if (name == "ADSVSS") {
        mADSVSS = value;
    }
}//}}}

/// Set tW/... parameters
void CADC::SetParameter (const std::string name, const unsigned int value)
{//{{{
    if (name == "tW") {
        mtW = value;
    } else if (name == "tR") {
        mtR = value;
    } else if (name == "tCLB") {
        mtCLB = value;
    } else if (name == "tDFAver") {
        mtDFAver = value;
    } else if (name == "tSAver") {
        mtSAver = value;
    } else if (name == "tDFNotAver") {
        mtDFNotAver = value;
    } else if (name == "tSNotAver") {
        mtSNotAver = value;
    }
}//}}}

/// Set Correction Coefficient value of Calibration
void CADC::SetCoefficient (const double value)
{//{{{
    mtCoeff = value;
}//}}}

/// Issue error to model
void CADC::SetError (const std::string name, const std::string ad_id, const std::string is_err)
{//{{{
    std::map<std::string, unsigned int>::iterator it;
    bool is_match = false;
    for (it = mAdIdMap.begin(); it != mAdIdMap.end(); it++) {
        if (it->first == ad_id) {
            is_match = true;
            if (name == "Parity") {
                if (is_err == "true") {
                    mParityError[it->second] = true;
                } else if (is_err == "false") {
                    mParityError[it->second] = false;
                } else {
                    re_printf("warning","Invalid argument: CADC_SetParityError %s %s\n", ad_id.c_str(), is_err.c_str());
                }
            } else if (name == "ID") {
                if (is_err == "true") {
                    mIDError[it->second] = true;
                } else if (is_err == "false") {
                    mIDError[it->second] = false;
                } else {
                    re_printf("warning","Invalid argument: CADC_SetIDError %s %s\n", ad_id.c_str(), is_err.c_str());
                }
            } else if (name == "Disconnect") {
                if (is_err == "true") {
                    mDisconnectError[it->second] = true;
                } else if (is_err == "false") {
                    mDisconnectError[it->second] = false;
                } else {
                    re_printf("warning","Invalid argument: CADC_SetDisconnectError %s %s\n", ad_id.c_str(), is_err.c_str());
                }
            }
            break;
        }
    }
    if (!is_match) {
        re_printf("warning","AD id (%s) is invalid\n", ad_id.c_str());
    }
}//}}}

///Handle CLK_LSB signal
void CADC::HandleCLK_LSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_LSB.read();
    SetCLKFreq("CLK_LSB", freq_value, "Hz");
}//}}}

///Handle CLK_HSB signal
void CADC::HandleCLK_HSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_HSB.read();
    SetCLKFreq("CLK_HSB", freq_value, "Hz");
}//}}}

///Handle PRESETn signal
void CADC::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[0] = PRESETn.read();
    if (mResetCurVal[0] != mResetPreVal[0]) {       //mResetPreVal is "true" at initial state
        mResetPreVal[0] = mResetCurVal[0];
        // Reset signals are asynchronous
        mResetHardEvent[0].notify(SC_ZERO_TIME);
    }
}//}}}

///Handle resetad_n signal
void CADC::HandleResetad_nSignalMethod (void)
{//{{{
    mResetCurVal[1] = resetad_n.read();
    if (mResetCurVal[1] != mResetPreVal[1]) {   //mResetPreVal is "true" at initial state
        mResetPreVal[1] = mResetCurVal[1];
        // Reset signals are asynchronous
        mResetHardEvent[1].notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when reset port is active
void CADC::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port resetad_n is asserted.\n");
            this->EnableReset("resetad_n", mIsResetHardActive[reset_id]);
        }
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            if (reset_id == 0) {
                re_printf("info","The reset port PRESETn is de-asserted.\n");
                this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
            } else {
                re_printf("info","The reset port resetad_n is de-asserted.\n");
                this->EnableReset("resetad_n", mIsResetHardActive[reset_id]);
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

///Process reset function when reset command is active
void CADC::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == 0) {
        reset_name = "PRESETn";
    } else { //reset_id == 1
        reset_name = "resetad_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when reset command is active
void CADC::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == 0) {
        reset_name = "PRESETn";
    } else { //reset_id == 1
        reset_name = "resetad_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

///Execute reset operation
void CADC::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "PRESETn") {
        mCADC_Func->Reset(is_active);
        for (unsigned int i = 0; i < emADNum; i++) {
            mAD_Unit[i]->mCADC_UNIT_Func->Reset(is_active);
        }
    }
    if (is_active) {
        // Cancel events
        CancelEvents();
        //Initialize variables
        Initialize();
    }
}//}}}

/// Cancel operation events
void CADC::CancelEvents (void)
{//{{{
    re_printf("info","CancelEvents called");
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->CancelEvents();
    }
}//}}}

/// Initialize internal variables
void CADC::Initialize (void)
{//{{{
    re_printf("info","Initialize called");
    for (unsigned int i = 0; i < emADNum; i++) {
        mAD_Unit[i]->Initialize();
    }
}//}}}

// vim600: set foldmethod=marker :
