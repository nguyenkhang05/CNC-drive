// ---------------------------------------------------------------------
// $Id: HWCORE_PI.cpp,v 1.6 2020/11/08 05:38:30 minhquocha Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of HWCORE_PI.cpp
// Ref: {HWCORE_PI_UD_Copyright_001}
// Description: About the include header files of HWCORE_PI.cpp
// Ref: {HWCORE_PI_UD_ATT1_002}
#include "re_define.h"
#include "HWCORE_PI.h"
#include "HWCORE_PI_Func.h"
///*********************************
/// Function     : HWCORE_PI
/// Description  : Constructor of HWCORE_PI class
// Ref: {HWCORE_PI_UD_Summary_001, HWCORE_PI_UD_ATT1_004, HWCORE_PI_UD_ATT1_011, HWCORE_PI_UD_ReferenceDoc_001}
///*********************************
HWCORE_PI::HWCORE_PI(sc_module_name name)
    :sc_module(name)
    , HWCORE_PI_AgentController()
    , CRWInterface()
    , rvc_common_model()
    //Clock/reset
    , PCLK                  ("PCLK")
    , PRESETn               ("PRESETn")
    , rst_n                 ("rst_n")
    //Input ports
    , pi_start_sp           ("pi_start_sp")
    , FUNCIDLEGRPA1         ("FUNCIDLEGRPA1")
    , FUNCWAITGRPA          ("FUNCWAITGRPA")
    , FUNCFLGRPA1           ("FUNCFLGRPA1")
    , IPTRG                 ("IPTRG")
    , rdata_em2iidfixobuf   ("rdata_em2iidfixobuf")
    , rdata_em2iiqfixobuf   ("rdata_em2iiqfixobuf")
    , rdata_iiroutdat0      ("rdata_iiroutdat0")
    , rdata_iiroutdat1      ("rdata_iiroutdat1")
    //Output ports
    , pi_func_end           ("pi_func_end")
    , pi_func_st            ("pi_func_st")
    , pi_func_wait          ("pi_func_wait")
    , pi_func_busy          ("pi_func_busy")
    , rdata_em2ivd          ("rdata_em2ivd")
    , rdata_em2ivq          ("rdata_em2ivq")
    , id_dir_reg            ("id_dir_reg")
    , iq_dir_reg            ("iq_dir_reg")
    , id_reg                ("id_reg")
    , iq_reg                ("iq_reg")
    , id_sel_out            ("id_sel_out")
    , iq_sel_out            ("iq_sel_out")
    , pi_end_sp_reg         ("pi_end_sp_reg")
    , pi_ovfl_out           ("pi_ovfl_out")
    , pi_ovfl_func          ("pi_ovfl_func")
{//{{{

    mpHWCORE_PIFunc = new HWCORE_PI_Func((std::string)name, this);
    sc_assert(mpHWCORE_PIFunc != NULL);

    // Initialize variables
    mPCLKPeriod =           0;
    mPCLKOrgFreq =          0;
    mPCLKFreq =             0;
    mPCLKFreqUnit =      "Hz";

    // Initialize reset flags
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }

    // Initialize time variables
    mTimeResolutionValue =      1;
    mTimeResolutionUnit =   SC_NS;

    // Initialize Python parameters
    mEnableRegisterMessage = true;
    mDumpInterrupt =        false;
    mStartPCLKTime =        false;
    mEnableStatInfo =       false;
    mPI0_cyc =               0x01;
    mPI1_cyc =               0x0C;
    mPI_border_cyc =         0x24;
    mPI_cyc_sum =      0x00000000;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    // Initialize output port
    pi_func_end.initialize(0x00);
    pi_func_st.initialize(0x00);
    pi_func_wait.initialize(false);
    pi_func_busy.initialize(false);
    rdata_em2ivd.initialize(0x00000000);
    rdata_em2ivq.initialize(0x00000000);
    id_dir_reg.initialize(0x00000000);
    iq_dir_reg.initialize(0x00000000);
    id_reg.initialize(0x00000000);
    iq_reg.initialize(0x00000000);
    id_sel_out.initialize(0x00000000);
    iq_sel_out.initialize(0x00000000);
    pi_end_sp_reg.initialize(false);
    pi_ovfl_out.initialize(false);
    pi_ovfl_func.initialize(false);

    // Initialize internal variables
    InitializeBuffer();
    InitializeOperation();

    // SC_THREAD/METHOD
    //Clock/reset
    SC_METHOD(HandlePCLKSignalMethod);
    sensitive << PCLK;

    //PRESETn
    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;

    //rst_n
    SC_METHOD(Handlerst_nSignalMethod);
    sensitive << rst_n;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PI::HandleResetHardMethod, this, reset_id),
                        sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PI::HandleResetCmdMethod, this, reset_id),
                        sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PI::CancelResetCmdMethod, this, reset_id),
                        sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    //Operation
    SC_METHOD(PI0BlockProcessMethod);
    dont_initialize();
    sensitive << mPI0BlockProcessEvent;

    SC_METHOD(PI1BlockProcessMethod);
    dont_initialize();
    sensitive << mPI1BlockProcessEvent;

    SC_METHOD(SumCyclePIIP0Method);
    dont_initialize();
    sensitive << mSumCyclePIIP0Event;

    SC_METHOD(SumCyclePIIP1Method);
    dont_initialize();
    sensitive << mSumCyclePIIP1Event;

    //For input part
    SC_METHOD(PIIP_IDLE_Start_Method);
    dont_initialize();
    sensitive << pi_start_sp << FUNCIDLEGRPA1;

    SC_METHOD(PIIP_WAIT_Start_Method);
    dont_initialize();
    sensitive << FUNCWAITGRPA;

    SC_METHOD(HandleFUNCFLGRPA1Method);
    dont_initialize();
    sensitive << FUNCFLGRPA1;

    SC_METHOD(Handle_rdata_em2iidfixobuf_Method);
    dont_initialize();
    sensitive << rdata_em2iidfixobuf;

    SC_METHOD(Handle_rdata_em2iiqfixobuf_Method);
    dont_initialize();
    sensitive << rdata_em2iiqfixobuf;

    SC_METHOD(Handle_rdata_iiroutdat0_Method);
    dont_initialize();
    sensitive << rdata_iiroutdat0;

    SC_METHOD(Handle_rdata_iiroutdat1_Method);
    dont_initialize();
    sensitive << rdata_iiroutdat1;

    SC_METHOD(Handle_IPTRG_Method);
    dont_initialize();
    sensitive << IPTRG;

    //For output part
    for (unsigned int port_id = 0; port_id < emSignalPortNum; port_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWritePISignalPortsEvent[port_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PI::WritePISignalOutPortMethod, this, port_id),
                        sc_core::sc_gen_unique_name("WritePISignalOutPortMethod"), &opt);
    }

    for (unsigned int port_id = 0; port_id < emSignalPortNum; port_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertPISignalPortsEvent[port_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PI::AssertPISignalOutMethod, this, port_id),
                        sc_core::sc_gen_unique_name("AssertPISignalOutMethod"), &opt);
    }

    for (unsigned int port_id = 0; port_id < emSignalPortNum; port_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeassertPISignalPortsEvent[port_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PI::DeassertPISignalOutMethod, this, port_id),
                        sc_core::sc_gen_unique_name("DeassertPISignalOutMethod"), &opt);
    }

    for (unsigned int port_id = 0; port_id < emDataPortsNum; port_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWritePIDataPortsEvent[port_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PI::WritePIDataOutPortsMethod, this, port_id),
                        sc_core::sc_gen_unique_name("WritePIDataOutPortsMethod"), &opt);
    }

}//}}}

///*********************************
/// Function     : ~HWCORE_PI
/// Description  : Destructor
///*********************************
HWCORE_PI::~HWCORE_PI()
{//{{{
    delete mpHWCORE_PIFunc;
}//}}}

// Define read APIs
// Ref: {HWCORE_PI_UD_ATT1_006, HWCORE_PI_UD_Flow_004}
void HWCORE_PI::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) {
        return;
    }
    mpHWCORE_PIFunc->read(offsetAddress, data_ptr, size, debug);
}//}}}

// Define write APIs
// Ref: {HWCORE_PI_UD_ATT1_006, HWCORE_PI_UD_Flow_005, HWCORE_PI_UD_Direction_051}
void HWCORE_PI::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) {
        return;
    }
    mpHWCORE_PIFunc->write(offsetAddress, data_ptr, size, debug);
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
///*********************************
/// Function     : SetMessageLevel 
/// Description  : Set message level (fatal, error, warning, info)
// Ref: {HWCORE_PI_UD_Direction_001, HWCORE_PI_UD_Direction_017, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012}
///*********************************
void HWCORE_PI::SetMessageLevel (const std::string msg_lv)
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

        mpHWCORE_PIFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpHWCORE_PIFunc->RegisterHandler(cmd);
    } else {
    //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpHWCORE_PIFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpHWCORE_PIFunc->RegisterHandler(cmd);
    }
}//}}}

///*********************************
/// Function     : DumpRegisterRW
/// Description  : Enable/disable dumping message when users access registers
// Ref: {HWCORE_PI_UD_Direction_002, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012}
///*********************************
void HWCORE_PI::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_PIFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : EnableRegisterMessage
/// Description  : Enable/disable EnableRegisterMessage
// Ref: {HWCORE_PI_UD_Direction_003, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_046, HWCORE_PI_UD_Direction_035}
///*********************************
void HWCORE_PI::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_PI_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        if (mEnableRegisterMessage) {
            re_printf("info","HWCORE_PI_EnableRegisterMessage %s\n", "true");
        } else {
            re_printf("info","HWCORE_PI_EnableRegisterMessage %s\n", "false");
        }
    } else {
        re_printf("warning","Invalid argument: HWCORE_PI_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : DumpInterrupt
/// Description  : Enable/disable dumping interrupt information
// Ref: {HWCORE_PI_UD_Direction_004, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_049, HWCORE_PI_UD_Direction_035, HWCORE_PI_UD_Flow_012}
///*********************************
void HWCORE_PI::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_PI_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if(mDumpInterrupt){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info","HWCORE_PI_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HWCORE_PI_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : EnableStatisticalInfo
/// Description  : Enable/disable dumping statistical information
///*********************************
// Description: EnableStatInfo
/// Ref: {HWCORE_PI_UD_Direction_005, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012}
void HWCORE_PI::EnableStatInfo(const std::string is_enable)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_enable);
    if (temp.size() == 1) {
        if (temp[0] == "true") {
            mEnableStatInfo = true;
        }
        else if (temp[0] == "false") {
            mEnableStatInfo = false;
        }
        else {
            re_printf("warning", "Invalid argument: HWCORE_PI_EnableStatInfo %s\n", is_enable.c_str());
        }
    }
    else if (temp.size() == 0) {
        std::string temp_arg = "";
        if (mEnableStatInfo == true) {
            temp_arg = "true";
        }
        else {
            temp_arg = "false";
        }
        re_printf("info", "HWCORE_PI_EnableStatInfo %s\n", temp_arg.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: HWCORE_PI_EnableStatInfo %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : DumpStatusInfo
/// Description  : Dump register's value
// Ref: {HWCORE_PI_UD_Direction_006, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_057}
///*********************************
void HWCORE_PI::DumpStatInfo(void)
{//{{{
    //Input Buffers
    re_printf("info", "mPICTR_FSUMID_buf = 0x%08X\n", mPICTR_FSUMID_buf);
    re_printf("info", "mPICTR_FSUMIQ_buf = 0x%08X\n", mPICTR_FSUMIQ_buf);
    re_printf("info", "mIDIN_buf         = 0x%08X\n", mIDIN_buf);
    re_printf("info", "mIQIN_buf         = 0x%08X\n", mIQIN_buf);
    re_printf("info", "mID_buf           = 0x%08X\n", mID_buf);
    re_printf("info", "mIQ_buf           = 0x%08X\n", mIQ_buf);
    re_printf("info", "mGPD0_buf         = 0x%08X\n", mGPD0_buf);
    re_printf("info", "mGPQ0_buf         = 0x%08X\n", mGPQ0_buf);
    re_printf("info", "mGPD_buf          = 0x%08X\n", mGPD_buf);
    re_printf("info", "mGPQ_buf          = 0x%08X\n", mGPQ_buf);
    re_printf("info", "mGID_buf          = 0x%08X\n", mGID_buf);
    re_printf("info", "mGIQ_buf          = 0x%08X\n", mGIQ_buf);
    re_printf("info", "mGIDMAX_buf       = 0x%08X\n", mGIDMAX_buf);
    re_printf("info", "mGIQMAX_buf       = 0x%08X\n", mGIQMAX_buf);
    re_printf("info", "mSUMID_buf        = 0x%08X\n", mSUMID_buf);
    re_printf("info", "mSUMIQ_buf        = 0x%08X\n", mSUMIQ_buf);
    re_printf("info", "mVDMAX_buf        = 0x%08X\n", mVDMAX_buf);
    re_printf("info", "mVQMAX_buf        = 0x%08X\n", mVQMAX_buf);
    //Output Buffers
    re_printf("info", "VDOBUF            = 0x%08X\n", mpHWCORE_PIFunc->GetRegVal("VDOBUF"));
    re_printf("info", "VQOBUF            = 0x%08X\n", mpHWCORE_PIFunc->GetRegVal("VQOBUF"));
}//}}}

///*********************************
/// Function     : AssertReset
/// Description  : Command to assert reset
// Ref: {HWCORE_PI_UD_Direction_007, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_036, HWCORE_PI_UD_Direction_037, HWCORE_PI_UD_Direction_042}
///*********************************
void HWCORE_PI::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETnId])&&(!mIsResetCmdReceive[emPRESETnId])) {
            mIsResetCmdReceive[emPRESETnId] = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPRESETnId].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETnId] = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else if (reset_name == "rst_n") {
        if ((!mIsResetHardActive[emrst_nId])&&(!mIsResetCmdReceive[emrst_nId])) {
            mIsResetCmdReceive[emrst_nId] = true;
            re_printf("info","The model will be reset (rst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emrst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[emrst_nId] = period;
        } else {
            re_printf("warning","The software reset of rst_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else{
        re_printf("warning","The reset name (%s) is wrong. It should be rst_n\n",reset_name.c_str());
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq
/// Description  : Set clock value
// Ref: {HWCORE_PI_UD_Direction_008, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_034}
///*********************************
void HWCORE_PI::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                // Cancel events
                CancelEvents();
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : GetCLKFreq
/// Description  : Get clock value
// Ref: {HWCORE_PI_UD_Direction_009, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_045, HWCORE_PI_UD_Direction_034}
///*********************************
void HWCORE_PI::GetCLKFreq (const std::string clock_name)
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

///*********************************
/// Function     : ForceRegister
/// Description  : Force value to register
// Ref: {HWCORE_PI_UD_Direction_010, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_031}
///*********************************
void HWCORE_PI::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_PIFunc->RegisterHandler(cmd);
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
// Ref: {HWCORE_PI_UD_Direction_011, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_031}
///*********************************
void HWCORE_PI::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHWCORE_PIFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : WriteRegister
/// Description  : Write value to registers by software
// Ref: {HWCORE_PI_UD_Direction_012, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_031}
///*********************************
void HWCORE_PI::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            // Call RegisterHandler of accroding block name
            mpHWCORE_PIFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReadRegister
/// Description  : Read value of register by software
// Ref: {HWCORE_PI_UD_Direction_013, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_031}
///*********************************
void HWCORE_PI::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpHWCORE_PIFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ListRegister
/// Description  : List all registers name
// Ref: {HWCORE_PI_UD_Direction_014, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012}
///*********************************
void HWCORE_PI::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHWCORE_PIFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : Help
/// Description  : Dump help message of all parameters or commands
// Ref: {HWCORE_PI_UD_Direction_015, HWCORE_PI_UD_Direction_018, HWCORE_PI_UD_Direction_019, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_030, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012}
///*********************************
void HWCORE_PI::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_MessageLevel (\"HWCORE_PI instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_DumpRegisterRW (\"HWCORE_PI instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_DumpInterrupt (\"HWCORE_PI instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_EnableRegisterMessage (\"HWCORE_PI instance\", \"true/false\")                               Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_EnableStatInfo (\"HWCORE_PI instance\", \"true/false\")                                      Enable/disable dumping the internal buffer registers' values of the HWCORE_PI model (Default: false)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_DumpStatInfo (\"HWCORE_PI instance\")                                                        Dump the values of the internal buffer registers of the HWCORE_PI model");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_AssertReset (\"HWCORE_PI instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the HWCORE_PI model");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_SetCLKFreq (\"HWCORE_PI instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to HWCORE_PI model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_GetCLKFreq (\"HWCORE_PI instance\", \"clock_name\")                                          Get clock frequency value of HWCORE_PI model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_ForceRegister (\"HWCORE_PI instance\",\"reg_name\", reg_value)                               Force register with setting value");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_ReleaseRegister (\"HWCORE_PI instance\",\"reg_name\")                                        Release register from force value");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_WriteRegister (\"HWCORE_PI instance\",\"reg_name\", reg_value)                               Write a value to a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_ReadRegister (\"HWCORE_PI instance\",\"reg_name\")                                           Read a value from a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_ListRegister (\"HWCORE_PI instance\")                                                        Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"HWCORE_PI_ChgProcessingTime (\"HWCORE_PI instance\", \"func_name\", \"cycle\")                         Change model processing time");
        } else {
            re_printf("warning","The name (%s) of HWCORE_PI_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of HWCORE_PI_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///*********************************
/// Function     : ChgProcessingTime
/// Description  : Change the process time
// Ref: {HWCORE_PI_UD_Direction_016, HWCORE_PI_UD_Flow_003, HWCORE_PI_UD_Flow_012, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_050, HWCORE_PI_UD_Direction_058}
///*********************************
void HWCORE_PI::ChgProcessingTime(const std::string func_name, const unsigned int cycle)
{//{{{
    bool flag = true;
    std::vector<std::string> word_vector;
    SeparateString(word_vector, func_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pi0") {
            mPI0_cyc = cycle;
        }
        else if (word_vector[0] == "pi1") {
            mPI1_cyc = cycle;
        }
        else if (word_vector[0] == "pi_border") {
            mPI_border_cyc = cycle;
        }
        else {
            flag = false;
        }
    } else {
        flag = false;
    }

    if (flag == true) {
        re_printf("info", "Change the process cycle of %s to %d.\n", func_name.c_str(), cycle);
    }
    else {
        re_printf("warning", "Function name (%s) is invalid.\n", func_name.c_str());
    }
}//}}}

//========================================================
//============ Method/Threads          ===================
//========================================================
///*********************************
/// Function     : HandlePCLKSignalMethod
/// Description  : Handle PCLK signal
// Ref: {HWCORE_PI_UD_Flow_002}
///*********************************
void HWCORE_PI::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandlePRESETnSignalMethod
/// Description  : Handle PRESETn signal
///*********************************
void HWCORE_PI::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[emPRESETnId] = PRESETn.read();
    if (mResetCurVal[emPRESETnId] != mResetPreVal[emPRESETnId] ) {       //mResetPreVal is "true" at initial state
        mResetPreVal[emPRESETnId]  = mResetCurVal[emPRESETnId] ;
        mResetHardEvent[emPRESETnId].notify(SC_ZERO_TIME);
    }
}//}}}

///*********************************
/// Function     : Handlerst_nSignalMethod
/// Description  : Handle rst_n signal
///*********************************
void HWCORE_PI::Handlerst_nSignalMethod (void)
{//{{{
    mResetCurVal[emrst_nId] = rst_n.read();
    if (mResetCurVal[emrst_nId] != mResetPreVal[emrst_nId]) {       //mResetPreVal is "false" at initial state
        mResetPreVal[emrst_nId] = mResetCurVal[emrst_nId];
        mResetHardEvent[emrst_nId].notify(SC_ZERO_TIME);
    }
}//}}}

///*********************************
/// Function     : PIIP_IDLE_Start_Method
/// Description  : Start PI control IP from IDLE state
// Ref: {HWCORE_PI_UD_Flow_006, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_052, HWCORE_PI_UD_Direction_055, HWCORE_PI_UD_Direction_059}
///*********************************
void HWCORE_PI::PIIP_IDLE_Start_Method(void)
{//{{{
    bool            mpi_start_sp   = pi_start_sp.read();
    unsigned short  mfuncidlegrpa1 = FUNCIDLEGRPA1.read();
    if ((mpi_start_sp == true) || (((mfuncidlegrpa1 >> 8) & 0x11) == 0x11)) {
        //Reset is asserted or clock is zero
        if (GetResetStatus("rst_n") == true || CheckClockPeriod() == false) {
            re_printf("warning", "Cannot start IP during reset period\n");
            return;
        }

        PISetBufferRegisters();
        if(mpHWCORE_PIFunc->GetRegVal("PICTR", "FPIIBTH") == 0) {
            mWritePIDataPortsEvent[emid_dir_reg].notify();
            mWritePIDataPortsEvent[emiq_dir_reg].notify();
            mWritePIDataPortsEvent[emid_reg].notify();
            mWritePIDataPortsEvent[emiq_reg].notify();
        }

        re_printf("info", "HWCORE_PI start process.\n");
        if (mEnableStatInfo == true) {
            re_printf("info", "Information of buffers (start): \n");
            DumpStatInfo();
        }
        if (((mfuncidlegrpa1 >> 8) & 0x1) == 0x00)            /* not select function block */
        {
            mSumCyclePIIP0Event.notify((double)(mPI0_cyc * mPCLKPeriod), mTimeResolutionUnit);
            SetStateBUSYPIIP();
        }
        else                                                                /* select function block */
        {
            switch (mfuncidlegrpa1)
            {
            case 0x0100:               /* run pi0 */
            case 0x1100:
                mSumCyclePIIP0Event.notify((double)(mPI0_cyc * mPCLKPeriod), mTimeResolutionUnit);
                SetStateBUSYPIIP();
                break;

            case 0x0101:               /* run pi1 */
            case 0x1101:
                mSumCyclePIIP1Event.notify((double)(mPI1_cyc * mPCLKPeriod), mTimeResolutionUnit);
                SetStateBUSYPIIP();
                break;

            default:                /* violation setting */
                break;
            }
        }
    }
}//}}}

///*********************************
/// Function     : PIIP_WAIT_Start_Method
/// Description  : Start PI control IP from WAIT state
// Ref: {HWCORE_PI_UD_Flow_007}
///*********************************
void HWCORE_PI::PIIP_WAIT_Start_Method(void)
{//{{{
    unsigned short mfuncwaitgrpa = FUNCWAITGRPA.read();
    if (pi_func_wait.read() == false) {
        return;
    }
    switch (mfuncwaitgrpa)
    {
    case 0x0200:               /* run pi0 */
        mSumCyclePIIP0Event.notify((double)(mPI0_cyc * mPCLKPeriod), mTimeResolutionUnit);
        break;

    case 0x0201:               /* run pi1 */
        mSumCyclePIIP1Event.notify((double)(mPI1_cyc * mPCLKPeriod), mTimeResolutionUnit);
        break;

    case 0x0202:               /* change to IDLE state */
        SetStateIDLEPIIP();
        EndPIIP();
        break;

    default:                /* violation setting */
        break;
    }
}//}}}

///*********************************
/// Function     : HandleFUNCFLGRPA1Method
/// Description  : Handle FUNCFLGRPA1 port
///*********************************
void HWCORE_PI::HandleFUNCFLGRPA1Method(void)
{//{{{
    mFUNCFLGRPA1 = FUNCFLGRPA1.read();
}//}}}

///*********************************
/// Function     : Handle_rdata_em2iidfixobuf_Method
/// Description  : Handle rdata_em2iidfixobuf port
///*********************************
void HWCORE_PI::Handle_rdata_em2iidfixobuf_Method(void)
{//{{{
    mIDFIXOBUF = rdata_em2iidfixobuf.read();
    if (mpHWCORE_PIFunc->GetRegVal("PICTR", "IDSEL") == 1) {
        return;
    }
    mIDSELOUT = mIDFIXOBUF;
    mWritePIDataPortsEvent[emid_sel_out].notify();
}//}}}

///*********************************
/// Function     : Handle_rdata_em2iiqfixobuf_Method
/// Description  : Handle rdata_em2iiqfixobuf port
///*********************************
void HWCORE_PI::Handle_rdata_em2iiqfixobuf_Method(void)
{//{{{
    mIQFIXOBUF = rdata_em2iiqfixobuf.read();
    if (mpHWCORE_PIFunc->GetRegVal("PICTR", "IQSEL") == 1) {
        return;
    }
    mIQSELOUT = mIQFIXOBUF;
    mWritePIDataPortsEvent[emiq_sel_out].notify();
}//}}}

///*********************************
/// Function     : Handle_rdata_iiroutdat0_Method
/// Description  : Handle rdata_iiroutdat0 port
///*********************************
void HWCORE_PI::Handle_rdata_iiroutdat0_Method(void)
{//{{{
    mIIROUTDAT0 = rdata_iiroutdat0.read();
    if (mpHWCORE_PIFunc->GetRegVal("PICTR", "IDSEL") == 0) {
        return;
    }
    mIDSELOUT = mIIROUTDAT0;
    mWritePIDataPortsEvent[emid_sel_out].notify();
}//}}}

///*********************************
/// Function     : Handle_rdata_iiroutdat1_Method
/// Description  : Handle rdata_iiroutdat1 port
///*********************************
void HWCORE_PI::Handle_rdata_iiroutdat1_Method(void)
{//{{{
    mIIROUTDAT1 = rdata_iiroutdat1.read();
    if (mpHWCORE_PIFunc->GetRegVal("PICTR", "IQSEL") == 0) {
        return;
    }
    mIQSELOUT = mIIROUTDAT1;
    mWritePIDataPortsEvent[emiq_sel_out].notify();
}//}}}

///*********************************
/// Function     : Handle_IPTRG_Method
/// Description  : Handle IPTRG port
///*********************************
void HWCORE_PI::Handle_IPTRG_Method(void)
{//{{{
    mIPTRG_PIIPTRG = (bool)((IPTRG.read() >> 2) & 0x01);
}//}}}

//========================================================
//============ Reset and Clock  ==========================
//========================================================
///*********************************
/// Function     : HandleResetHardMethod
/// Description  : Process reset function when reset port is active
// Ref: {HWCORE_PI_UD_Flow_001, HWCORE_PI_UD_Direction_043, HWCORE_PI_UD_Direction_044, HWCORE_PI_UD_Direction_020}
///*********************************
void HWCORE_PI::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        } else {
            re_printf("info","The reset port rst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        }
    } else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        } else {
            re_printf("info","The reset port rst_n is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        }
    }

    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id] && mIsResetHardActive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

///*********************************
/// Function     : HandleResetCmdMethod
/// Description  : Process reset function when reset command is active
// Ref: {HWCORE_PI_UD_Direction_041, HWCORE_PI_UD_Direction_020}
///*********************************
void HWCORE_PI::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else { //reset_id == emrst_nId
        reset_name = "rst_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when reset command is active
///*********************************
/// Function     : CancelResetCmdMethod
/// Description  : Cancel ResetCmd Method
// Ref: {HWCORE_PI_UD_Direction_040, HWCORE_PI_UD_Direction_020}
///*********************************
void HWCORE_PI::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else {
        reset_name = "rst_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Execute reset operation
// Ref: {HWCORE_PI_UD_Flow_001, HWCORE_PI_UD_Flow_011}
///*********************************
void HWCORE_PI::EnableReset (const bool is_active, const unsigned int reset_id)
{//{{{
    mpHWCORE_PIFunc->EnableReset (is_active);
    if (is_active) {
        InitializeBuffer();
        if (reset_id == emrst_nId){
            InitializeOperation();
            CancelEvents();
            ResetPorts();
        }
    }
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize buffer variables
// Ref: {HWCORE_PI_UD_Flow_001, HWCORE_PI_UD_Flow_011}
///*********************************
void HWCORE_PI::InitializeBuffer(void)
{//{{{
    mPICTR_FSUMID_buf = false;
    mPICTR_FSUMIQ_buf = false;
    mIDIN_buf = 0x00000000;
    mIQIN_buf = 0x00000000;
    mID_buf = 0x00000000;
    mIQ_buf = 0x00000000;
    mGPD0_buf = 0x00000000;
    mGPQ0_buf = 0x00000000;
    mGPD_buf = 0x00000000;
    mGPQ_buf = 0x00000000;
    mGID_buf = 0x00000000;
    mGIQ_buf = 0x00000000;
    mGIDMAX_buf = 0x00000000;
    mGIQMAX_buf = 0x00000000;
    mSUMID_buf = 0x00000000;
    mSUMIQ_buf = 0x00000000;
    mVDMAX_buf = 0x00000000;
    mVQMAX_buf = 0x00000000;
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize operation variables
// Ref: {HWCORE_PI_UD_Flow_001, HWCORE_PI_UD_Flow_011}
///*********************************
void HWCORE_PI::InitializeOperation(void)
{//{{{
    msum_id = 0x00000000;
    msum_iq = 0x00000000;
    mg_n_sumidm = 0x00000000;
    mg_n_sumiqm = 0x00000000;

    mpi_func_wait = false;
    mpi_func_busy = false;
    mpi_ovfl_func = false;
    mpi_ovfl_flag = false;

    mIDFIXOBUF = 0x00000000;
    mIQFIXOBUF = 0x00000000;
    mIIROUTDAT0 = 0x00000000;
    mIIROUTDAT1 = 0x00000000;
    mIDDIRREG = 0x00000000;
    mIQDIRREG = 0x00000000;
    mIDREG = 0x00000000;
    mIQREG = 0x00000000;
    mIDSELOUT = 0x00000000;
    mIQSELOUT = 0x00000000;
    mFUNCFLGRPA1 = 0x00;
    mIPTRG_PIIPTRG = false;

    mPISignal_NextVal = false;
    mPIFuncEnd_NextVal = false;
    mPIFuncSt_NextVal = false;
}//}}}

///*********************************
/// Function     : Reset_Ports
/// Description  : Initialize output ports
// Ref: {HWCORE_PI_UD_Flow_001, HWCORE_PI_UD_Flow_011}
///*********************************
void HWCORE_PI::ResetPorts(void)
{//{{{
    //Reset Data ports
    for (unsigned char port_id = 0; port_id < emDataPortsNum; port_id++)
    {
        mWritePIDataPortsEvent[port_id].notify();
    }

    //Reset Signal ports
    for (unsigned char port_id = 0; port_id < emSignalPortNum; port_id++)
    {
        mDeassertPISignalPortsEvent[port_id].notify();
    }
}//}}}

///*********************************
/// Function     : CancelEvents
/// Description  : Cancel all events
// Ref: {HWCORE_PI_UD_Flow_001, HWCORE_PI_UD_Flow_011}
///*********************************
void HWCORE_PI::CancelEvents()
{//{{{
    mPI0BlockProcessEvent.cancel();
    mPI1BlockProcessEvent.cancel();
    mSumCyclePIIP0Event.cancel();
    mSumCyclePIIP1Event.cancel();

    for (unsigned char port_id = 0; port_id < emDataPortsNum; port_id++)
    {
        mWritePIDataPortsEvent[port_id].cancel();
    }

    for (unsigned char port_id = 0; port_id < emSignalPortNum; port_id++)
    {
        mWritePISignalPortsEvent[port_id].cancel();
    }

    for (unsigned char port_id = 0; port_id < emSignalPortNum; port_id++)
    {
        mAssertPISignalPortsEvent[port_id].cancel();
    }

    for (unsigned char port_id = 0; port_id < emSignalPortNum; port_id++)
    {
        mDeassertPISignalPortsEvent[port_id].cancel();
    }
}//}}}

///*********************************
/// Function     : DumpInterruptMsg
/// Description  : Dump Interrupt Message
// Ref: {HWCORE_PI_UD_Direction_048, HWCORE_PI_UD_Direction_020}
///*********************************
void HWCORE_PI::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
    if (mDumpInterrupt) {
        if (value) {
            re_printf("info","[%s] is changed to %s.\n",intr_name.c_str(),"1");
        } else {
            re_printf("info","[%s] is changed to %s .\n",intr_name.c_str(),"0");
        }
    }
}//}}}

///*********************************
/// Function     : WritePIDataOutPortsMethod
/// Description  : Write PI control IP output data ports
// Ref: {HWCORE_PI_UD_Direction_047, HWCORE_PI_UD_Direction_020}
///*********************************
void HWCORE_PI::WritePIDataOutPortsMethod(const unsigned int port_id)
{//{{{
    signed int rdata_em2ivd_regval = mpHWCORE_PIFunc->GetRegVal("VD");
    signed int rdata_em2ivq_regval = mpHWCORE_PIFunc->GetRegVal("VQ");
    bool       pictr_fpiibth_curval = mpHWCORE_PIFunc->GetRegVal("PICTR", "FPIIBTH");
    signed int idin_regval = mpHWCORE_PIFunc->GetRegVal("IDIN");
    signed int iqin_regval = mpHWCORE_PIFunc->GetRegVal("IQIN");
    signed int id_regval = mpHWCORE_PIFunc->GetRegVal("ID");
    signed int iq_regval = mpHWCORE_PIFunc->GetRegVal("IQ");

    switch(port_id)
    {
    case emrdata_em2ivd:
        if (rdata_em2ivd_regval != rdata_em2ivd.read()) {
            rdata_em2ivd.write(rdata_em2ivd_regval);
            re_printf("info", "Port [rdata_em2ivd] is set to 0x%08x.\n",
                    rdata_em2ivd_regval);
        }
        break;

    case emrdata_em2ivq:
        if (rdata_em2ivq_regval != rdata_em2ivq.read()) {
            rdata_em2ivq.write(rdata_em2ivq_regval);
            re_printf("info", "Port [rdata_em2ivq] is set to 0x%08x.\n",
                    rdata_em2ivq_regval);
        }
        break;

    case emid_dir_reg:
        if(pictr_fpiibth_curval == 0) {
            if (mIDIN_buf != id_dir_reg.read()) {
                id_dir_reg.write(mIDIN_buf);
                re_printf("info", "Port [id_dir_reg] is set to 0x%08x.\n", mIDIN_buf);
            }
        } else {
            if (idin_regval != id_dir_reg.read()) {
                id_dir_reg.write(idin_regval);
                re_printf("info", "Port [id_dir_reg] is set to 0x%08x.\n", idin_regval);
            }
        }
        break;

    case emiq_dir_reg:
        if(pictr_fpiibth_curval == 0) {
            if (mIQIN_buf != iq_dir_reg.read()) {
                iq_dir_reg.write(mIQIN_buf);
                re_printf("info", "Port [iq_dir_reg] is set to 0x%08x.\n", mIQIN_buf);
            }
        } else {
            if (iqin_regval != iq_dir_reg.read()) {
                iq_dir_reg.write(iqin_regval);
                re_printf("info", "Port [iq_dir_reg] is set to 0x%08x.\n", iqin_regval);
            }
        }
        break;

    case emid_reg:
        if(pictr_fpiibth_curval == 0) {
            if (mID_buf != id_reg.read()) {
                id_reg.write(mID_buf);
                re_printf("info", "Port [id_reg] is set to 0x%08x.\n", mID_buf);
            }
        } else {
            if (id_regval != id_reg.read()) {
                id_reg.write(id_regval);
                re_printf("info", "Port [id_reg] is set to 0x%08x.\n", id_regval);
            }
        }
        break;

    case emiq_reg:
        if(pictr_fpiibth_curval == 0) {
            if (mIQ_buf != iq_reg.read()) {
                iq_reg.write(mIQ_buf);
                re_printf("info", "Port [iq_reg] is set to 0x%08x.\n", mIQ_buf);
            }
        } else {
            if (iq_regval != iq_reg.read()) {
                iq_reg.write(iq_regval);
                re_printf("info", "Port [iq_reg] is set to 0x%08x.\n", iq_regval);
            }
        }
        break;

    case emid_sel_out:
        if (mIDSELOUT != id_sel_out.read()) {
            id_sel_out.write(mIDSELOUT);
            re_printf("info", "Port [id_sel_out] is set to 0x%08x.\n", mIDSELOUT);
        }
        break;

    case emiq_sel_out:
        if (mIQSELOUT != iq_sel_out.read()) {
            iq_sel_out.write(mIQSELOUT);
            re_printf("info", "Port [iq_sel_out] is set to 0x%08x.\n", mIQSELOUT);
        }
        break;

    case empi_func_wait:
        if (mpi_func_wait != pi_func_wait.read()) {
            pi_func_wait.write(mpi_func_wait);
            re_printf("info", "Port [pi_func_wait] is set to 0x%08x.\n",
                    mpi_func_wait);
        }
        break;

    case empi_func_busy:
        if (mpi_func_busy != pi_func_busy.read()) {
            pi_func_busy.write(mpi_func_busy);
            re_printf("info", "Port [pi_func_busy] is set to 0x%08x.\n",
                    mpi_func_busy);
        }
        break;

    case empi_ovfl_func:
        if (mpi_ovfl_func != pi_ovfl_func.read()) {
            pi_ovfl_func.write(mpi_ovfl_func);
            re_printf("info", "Port [pi_ovfl_func] is set to 0x%08x.\n",
                    mpi_ovfl_func);
        }
        break;

    default:
        break;
    }
}//}}}

///*********************************
/// Function     : WritePISignalOutPortMethod
/// Description  : Write PI control IP signal port Method
// Ref: {HWCORE_PI_UD_Direction_047, HWCORE_PI_UD_Direction_020}
///*********************************
void HWCORE_PI::WritePISignalOutPortMethod(const unsigned int port_id)
{//{{{
    switch(port_id)
    {
    case empi_func_end:
        if (mPIFuncEnd_NextVal != pi_func_end.read())
        {
            pi_func_end.write(mPIFuncEnd_NextVal);
            re_printf("info","Port [pi_func_end] is set to 0x%08x.\n", mPIFuncEnd_NextVal);
        }
        break;

    case empi_func_st:
        if (mPIFuncSt_NextVal != pi_func_st.read())
        {
            pi_func_st.write(mPIFuncSt_NextVal);
            re_printf("info","Port [pi_func_st] is set to 0x%08x.\n", mPIFuncSt_NextVal);
        }
        break;

    case empi_end_sp_reg:
        if (mPISignal_NextVal != pi_end_sp_reg.read())
        {
            pi_end_sp_reg.write(mPISignal_NextVal);
            DumpInterruptMsg("pi_end_sp_reg", mPISignal_NextVal);
        }
        break;

    case empi_ovfl_out:
        if (mPISignal_NextVal != pi_ovfl_out.read())
        {
            pi_ovfl_out.write(mPISignal_NextVal);
            DumpInterruptMsg("pi_ovfl_out", mPISignal_NextVal);
        }
        break;

    default:
        break;
    }
}//}}}

///*********************************
/// Function     : AssertPISignalOutMethod
/// Description  : Assert PI control IP signal port Method
///*********************************
void HWCORE_PI::AssertPISignalOutMethod(const unsigned int port_id)
{
    mPISignal_NextVal = true;
    mWritePISignalPortsEvent[port_id].notify();
    // Deassert after 1 cycle
    double current_time = sc_time_stamp().to_double();
    mDeassertPISignalPortsEvent[port_id].notify(CalculateCLKPosEdge(
        (double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
}

///*********************************
/// Function     : DeassertPISignalOutMethod
/// Description  : DeAssert PI control IP signal port Method
///*********************************
void HWCORE_PI::DeassertPISignalOutMethod(const unsigned int port_id)
{
    if (port_id == empi_func_st) {
        mPIFuncSt_NextVal = 0x00;
    } else if (port_id == empi_func_end) {
        mPIFuncEnd_NextVal = 0x00;
    } else {
        mPISignal_NextVal = false;
    }

    mWritePISignalPortsEvent[port_id].notify();
}

///*********************************
/// Function     : PISetBufferRegisters
/// Description  : Set internal buffers of PI Control IP registers
// Ref: {HWCORE_PI_UD_Flow_006}
///*********************************
void HWCORE_PI::PISetBufferRegisters(void)
{
    mPICTR_FSUMID_buf = mpHWCORE_PIFunc->GetRegVal("PICTR", "FSUMID");
    mPICTR_FSUMIQ_buf = mpHWCORE_PIFunc->GetRegVal("PICTR", "FSUMIQ");
    mIDIN_buf         = mpHWCORE_PIFunc->GetRegVal("IDIN");
    mIQIN_buf         = mpHWCORE_PIFunc->GetRegVal("IQIN");
    mID_buf           = mpHWCORE_PIFunc->GetRegVal("ID");
    mIQ_buf           = mpHWCORE_PIFunc->GetRegVal("IQ");
    mGPD0_buf         = mpHWCORE_PIFunc->GetRegVal("GPD0");
    mGPQ0_buf         = mpHWCORE_PIFunc->GetRegVal("GPQ0");
    mGPD_buf          = mpHWCORE_PIFunc->GetRegVal("GPD");
    mGPQ_buf          = mpHWCORE_PIFunc->GetRegVal("GPQ");
    mGID_buf          = mpHWCORE_PIFunc->GetRegVal("GID");
    mGIQ_buf          = mpHWCORE_PIFunc->GetRegVal("GIQ");
    mGIDMAX_buf       = mpHWCORE_PIFunc->GetRegVal("GIDMAX");
    mGIQMAX_buf       = mpHWCORE_PIFunc->GetRegVal("GIQMAX");
    mSUMID_buf        = mpHWCORE_PIFunc->GetRegVal("SUMID");
    mSUMIQ_buf        = mpHWCORE_PIFunc->GetRegVal("SUMIQ");
    mVDMAX_buf        = mpHWCORE_PIFunc->GetRegVal("VDMAX");
    mVQMAX_buf        = mpHWCORE_PIFunc->GetRegVal("VQMAX");
}

///*********************************
/// Function     : PI0BlockProcessMethod
/// Description  : State process of Func(pi0) block
// Ref: {HWCORE_PI_UD_Flow_008}
///*********************************
void HWCORE_PI::PI0BlockProcessMethod(void)
{
    /* pi0 process */
    FuncPI0();

    double current_time = sc_time_stamp().to_double();
    /* decide next state and process */
    switch (mFUNCFLGRPA1 & 0x03)
    {
    case 0x00:              /* run next block */
        SetStateBUSYPIIP();
        mSumCyclePIIP1Event.notify((double)(mPI1_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pi01 */
        break;

    case 0x01:              /* change state to IDLE */
        SetStateIDLEPIIP();
        EndPIIP();
        mPIFuncEnd_NextVal = 0x01;    /* set last finish func */
        mAssertPISignalPortsEvent[empi_func_end].notify(CalculateCLKPosEdge(
            (double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);    /* set last finish func */
        break;

    case 0x02:              /* change state to WAIT */
        SetStateWAITPIIP();
        mPIFuncEnd_NextVal = 0x01;    /* set last finish func */
        mAssertPISignalPortsEvent[empi_func_end].notify(CalculateCLKPosEdge(
            (double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);    /* set last finish func */
        break;

    default:                /* violation setting */
        break;
    }

    /* set finish flag */
    mPIFuncSt_NextVal = 0x01;
    mAssertPISignalPortsEvent[empi_func_st].notify(CalculateCLKPosEdge(
        (double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);    /* pi0 finish flag */
}

///*********************************
/// Function     : PI0BlockProcessMethod
/// Description  : State process of Func(pi1) block
// Ref: {HWCORE_PI_UD_Flow_009}
///*********************************
void HWCORE_PI::PI1BlockProcessMethod(void)
{
    /* pi1 process */
    PI1ProcessSetting();

    double current_time = sc_time_stamp().to_double();
    /* decide next state and process */
    switch ((mFUNCFLGRPA1 >> 2) & 0x03)
    {
    case 0x00:              /* change state to IDLE */
        SetStateIDLEPIIP();
        EndPIIP();
        break;

    case 0x01:              /* change state to IDLE */
        SetStateIDLEPIIP();
        EndPIIP();
        mPIFuncEnd_NextVal = 0x02;    /* set last finish func */
        mAssertPISignalPortsEvent[empi_func_end].notify(CalculateCLKPosEdge(
            (double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);    /* set last finish func */
        break;

    case 0x02:              /* change state to WAIT */
        SetStateWAITPIIP();
        mPIFuncEnd_NextVal = 0x02;    /* set last finish func */
        mAssertPISignalPortsEvent[empi_func_end].notify(CalculateCLKPosEdge(
            (double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);    /* set last finish func */
        break;

    default:                /* violation setting */
        break;
    }

    /* set finish flag */
    mPIFuncSt_NextVal = 0x02;
    mAssertPISignalPortsEvent[empi_func_st].notify(CalculateCLKPosEdge(
        (double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);    /* pi1 finish flag */
}

///*********************************
/// Function     : PI1ProcessSetting
/// Description  : Pre and Post process setting for Func(pi1) operation
// Ref: {HWCORE_PI_UD_Flow_009}
///*********************************
void HWCORE_PI::PI1ProcessSetting(void)
{
    signed int in_id        = 0x00000000;
    signed int in_iq        = 0x00000000;
    signed int out_vd       = 0x00000000;
    signed int out_vq       = 0x00000000;

    /* pi01 pre process */
    if (mpHWCORE_PIFunc->GetRegVal("PICTR", "FPIIBTH") == 0) {
        mIDREG = mID_buf;
        mIQREG = mIQ_buf;
        mIDDIRREG = mIDIN_buf;
        mIQDIRREG = mIQIN_buf;
    } else {
        mIDREG = mpHWCORE_PIFunc->GetRegVal("ID");
        mIQREG = mpHWCORE_PIFunc->GetRegVal("IQ");
        mIDDIRREG = mpHWCORE_PIFunc->GetRegVal("IDIN");
        mIQDIRREG = mpHWCORE_PIFunc->GetRegVal("IQIN");
    }

    if(mIPTRG_PIIPTRG == 0){
        in_id = mIDREG;
        in_iq = mIQREG;
    } else {
        in_id = mIDSELOUT;
        in_iq = mIQSELOUT;
    }

    /* run func block pi1 */
    FuncPI1(in_id, in_iq, &out_vd, &out_vq);

    /* pi1 post process */
    mpHWCORE_PIFunc->SetRegVal("VD", (unsigned int)out_vd);
    mpHWCORE_PIFunc->SetRegVal("VQ", (unsigned int)out_vq);
    mpHWCORE_PIFunc->SetRegVal("SUMIDM", "DATA", mg_n_sumidm);
    mpHWCORE_PIFunc->SetRegVal("SUMIQM", "DATA", mg_n_sumiqm);

    // Set output port
    mWritePIDataPortsEvent[emrdata_em2ivd].notify();
    mWritePIDataPortsEvent[emrdata_em2ivq].notify();

    /* over flow flag set */
    if (mpi_ovfl_flag == true)
    {
        double current_time = sc_time_stamp().to_double();
        mAssertPISignalPortsEvent[empi_ovfl_out].notify(CalculateCLKPosEdge(
            (double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
        mpi_ovfl_func = 0x1;
        mWritePIDataPortsEvent[empi_ovfl_func].notify();
    }
}

///*********************************
/// Function     : FuncPI0
/// Description  : Start Func(pi0)
// Ref: {HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_054}
///*********************************
void HWCORE_PI::FuncPI0(void)
{//{{{
}//}}}

///*********************************
/// Function     : FuncPI1
/// Description  : Start Func(pi1)
// Ref: {HWCORE_PI_UD_Flow_010, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_054}
///*********************************
void HWCORE_PI::FuncPI1(signed int in_id, signed int in_iq, signed int *out_vd, signed int *out_vq)
{/* int32_t in_id, int32_t in_iq, int32_t *out_vd, int32_t *out_vq */

    assert(out_vd);
    assert(out_vq);
    //static signed int msum_id = 0, msum_iq = 0;   /* int32_t msum_id=0, msum_iq=0 */
    signed int ids = 0, iqs = 0;                /* int32_t ids,iqs            */

    /* PI IP completed start-up */
    ids = ((long long int)(mIDDIRREG - in_id) * (long long int)mGPD0_buf) >> 16;
    iqs = ((long long int)(mIQDIRREG - in_iq) * (long long int)mGPQ0_buf) >> 16;

    /*Ovf Check*/
    Chk_overflow_s((OVFC_S)msum_id + (((OVFC_S)ids * (OVFC_S)mGID_buf) >> 16), &mpi_ovfl_flag);
    msum_id += (((long long int)ids * (long long int)mGID_buf) >> 16);
    /*Ovf Check*/
    Chk_overflow_s((OVFC_S)msum_iq + (((OVFC_S)iqs * (OVFC_S)mGIQ_buf) >> 16), &mpi_ovfl_flag);
    msum_iq += (((long long int)iqs * (long long int)mGIQ_buf) >> 16);

    if ((long long int)mGIDMAX_buf < (long long int)msum_id)
    {
        msum_id = mGIDMAX_buf;
    }
    else if ((long long int)msum_id < -(long long int)(mGIDMAX_buf))
    {
        msum_id = -mGIDMAX_buf;
    }

    if ((long long int)mGIQMAX_buf < (long long int)msum_iq)
    {
        msum_iq = mGIQMAX_buf;
    }
    else if ((long long int)msum_iq < -(long long int)(mGIQMAX_buf))
    {
        msum_iq = -mGIQMAX_buf;
    }

    if (mPICTR_FSUMID_buf == false)
    {
        msum_id = mSUMID_buf;
    }

    if (mPICTR_FSUMIQ_buf == false)
    {
        msum_iq = mSUMIQ_buf;
    }

    mg_n_sumidm = msum_id;
    mg_n_sumiqm = msum_iq;

    *out_vd = (long long int)msum_id + (((long long int)ids * (long long int)mGPD_buf) >> 16);
    /*Ovf Check*/
    Chk_overflow_s((OVFC_S)msum_id + (((OVFC_S)ids * (OVFC_S)mGPD_buf) >> 16), &mpi_ovfl_flag);
    *out_vq = (long long int)msum_iq + (((long long int)iqs * (long long int)mGPQ_buf) >> 16);
    /*Ovf Check*/
    Chk_overflow_s((OVFC_S)msum_iq + (((OVFC_S)iqs * (OVFC_S)mGPQ_buf) >> 16), &mpi_ovfl_flag);

    if ((long long int)mVDMAX_buf < (long long int)*out_vd)
    {
        *out_vd = mVDMAX_buf;
    }
    else if ((long long int)*out_vd < -(long long int)(mVDMAX_buf))
    {
        *out_vd = -mVDMAX_buf;
    }

    if ((long long int)mVQMAX_buf < (long long int)*out_vq)
    {
        *out_vq = mVQMAX_buf;
    }
    else if ((long long int)*out_vq < -(long long int)(mVQMAX_buf))
    {
        *out_vq = -mVQMAX_buf;
    }

}

///*********************************
/// Function     : SumCyclePIIP0Method
/// Description  : Sum Func(pi0) process cycles to total cycle
///*********************************
void HWCORE_PI::SumCyclePIIP0Method(void)
{
    bool end_flag = false;
    unsigned int wait_cyc = 0x00000000;

    /* wait about border cycle */
    end_flag = JudgePIIPEnd(emBLOCK00);
    wait_cyc = JudgeCycPIIP(mPI0_cyc, end_flag);
    re_printf("info", "HWCORE_PI start Func(pi0)\n");
    mPI0BlockProcessEvent.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}

///*********************************
/// Function     : SumCyclePIIP1Method
/// Description  : Sum Func(pi1) process cycles to total cycle
///*********************************
void HWCORE_PI::SumCyclePIIP1Method(void)
{
    bool end_flag = false;
    unsigned int wait_cyc = 0x00000000;

    /* wait about border cycle */
    end_flag = JudgePIIPEnd(emBLOCK01);
    wait_cyc = JudgeCycPIIP(mPI1_cyc, end_flag);
    re_printf("info", "HWCORE_PI start Func(pi1)\n");
    mPI1BlockProcessEvent.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}

///*********************************
/// Function     : JudgeCycPIIP
/// Description  : Jugde PI Control IP total cycles with border cycles
///*********************************
unsigned int HWCORE_PI::JudgeCycPIIP(unsigned int cyc, bool end_flag)
{
    unsigned int wait_cyc = 0x00000000;

    mPI_cyc_sum += cyc;                        /* add calculation cycle */

    /* end process at this block */
    if (end_flag == true)
    {
        /* calculation cycle is more than border cycle */
        if (mPI_cyc_sum >= mPI_border_cyc)
        {
            wait_cyc = 0;
        }

        /* calculation cycle is less than border cycle */
        else
        {
            wait_cyc = mPI_border_cyc - mPI_cyc_sum;
        }
    }

    /* continue process */
    else
    {
        wait_cyc = 0;
    }
    return wait_cyc;
}

///*********************************
/// Function     : JudgePIIPEnd
/// Description  : Jugde PI Control IP end process
///*********************************
bool HWCORE_PI::JudgePIIPEnd(unsigned char shift)
{
    unsigned char value    =  0x00;
    bool          end_flag = false;
    switch (shift)
    {
    case emBLOCK00:
        /* shift now block */
        value = (0x03 & ((unsigned char)(mFUNCFLGRPA1 >> shift)));

        /* run next block */
        if (value == 0x00)
        {
            end_flag = false;
        }

        /* WAIT or IDLE */
        else if ((value == 0x01) || (value == 0x02))
        {
            end_flag = true;
        }

        break;

    case emBLOCK01:
        end_flag = true;
        break;

    default:
        break;
    }
    return end_flag;
}


///*********************************
/// Function     : EndPIIP
/// Description  : PI Control IP end operation
// Ref: {HWCORE_PI_UD_Flow_006, HWCORE_PI_UD_Flow_007, HWCORE_PI_UD_Flow_008, HWCORE_PI_UD_Flow_009, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_053, HWCORE_PI_UD_Direction_056}
///*********************************
void HWCORE_PI::EndPIIP(void)
{
    // Set buffer values end
    mpHWCORE_PIFunc->SetRegVal("VDOBUF", mpHWCORE_PIFunc->GetRegVal("VD"));
    mpHWCORE_PIFunc->SetRegVal("VQOBUF", mpHWCORE_PIFunc->GetRegVal("VQ"));

    re_printf("info", "HWCORE_PI end process.\n");
    if (mEnableStatInfo == true) {
        re_printf("info", "Information of buffers (end): \n");
        DumpStatInfo();
    }

    double current_time = sc_time_stamp().to_double();
    mAssertPISignalPortsEvent[empi_end_sp_reg].notify(CalculateCLKPosEdge(
        (double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);

    /* clear cycle sum */
    mPI_cyc_sum = 0;
}

///*********************************
/// Function     : SetStateIDLEPIIP
/// Description  : Set state idle
// Ref: {HWCORE_PI_UD_Flow_007, HWCORE_PI_UD_Flow_008, HWCORE_PI_UD_Flow_009, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_051}
///*********************************
void HWCORE_PI::SetStateIDLEPIIP(void)
{//{{{
    re_printf("info", "HWCORE_PI switch to IDLE state\n");
    mpi_func_busy = 0x0;
    mpi_func_wait = 0x0;
    mWritePIDataPortsEvent[empi_func_wait].notify();
    mWritePIDataPortsEvent[empi_func_busy].notify();
}//}}}

///*********************************
/// Function     : SetStateWAITPIIP
/// Description  : Set state wait
// Ref: {HWCORE_PI_UD_Flow_006, HWCORE_PI_UD_Flow_008, HWCORE_PI_UD_Flow_009, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_051}
///*********************************
void HWCORE_PI::SetStateWAITPIIP(void)
{//{{{
    re_printf("info", "HWCORE_PI switch to WAIT state\n");
    mpi_func_busy = 0x0;
    mpi_func_wait = 0x1;
    mWritePIDataPortsEvent[empi_func_wait].notify();
    mWritePIDataPortsEvent[empi_func_busy].notify();
}//}}}

///*********************************
/// Function     : SetStateBUSYPIIP
/// Description  : Set state busy
// Ref: {HWCORE_PI_UD_Flow_006, HWCORE_PI_UD_Flow_007, HWCORE_PI_UD_Flow_008, HWCORE_PI_UD_Direction_020, HWCORE_PI_UD_Direction_051}
///*********************************
void HWCORE_PI::SetStateBUSYPIIP(void)
{//{{{
    re_printf("info", "HWCORE_PI switch to BUSY state\n");
    mpi_func_wait = 0x0;
    mpi_func_busy = 0x1;
    mWritePIDataPortsEvent[empi_func_wait].notify();
    mWritePIDataPortsEvent[empi_func_busy].notify();
}//}}}

///*********************************
/// Function     : Chk_overflow_s
/// Description  : Check for arithmetic operation overflow.
///*********************************
void HWCORE_PI::Chk_overflow_s(signed long long input, bool* ovfflag)
{//{{{
    if (ovfflag == NULL) {
        return;
    }

    signed long long check_val1 = 0x000000007fffffffLL;
    signed long long check_val2 = 0xffffffff80000000LL;
    if( ( input > check_val1 ) || (input < check_val2) ) /* Overflow or underflow upon the occurrence */
    {
    /* Overflow flag ON */
        *ovfflag = true;
    }

}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
///*********************************
/// Function     : CheckClockPeriod 
/// Description  : Check Clock Period
// Ref: {HWCORE_PI_UD_Direction_039}
///*********************************
bool HWCORE_PI::CheckClockPeriod (void)
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

///*********************************
/// Function     : GetResetStatus 
/// Description  : Get Reset Status
///*********************************
bool HWCORE_PI::GetResetStatus(const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "rst_n"){
        if (mIsResetHardActive[emrst_nId] || mIsResetCmdActive[emrst_nId]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///*********************************
/// Function     : Callback_IDIN
/// Description  : callback function for IDIN
// Ref: {HWCORE_PI_UD_Flow_013}
///*********************************
void HWCORE_PI::Callback_IDIN(void)
{//{{{
    mWritePIDataPortsEvent[emid_dir_reg].notify();
}//}}}

///*********************************
/// Function     : Callback_IQIN
/// Description  : callback function for IQIN
// Ref: {HWCORE_PI_UD_Flow_013}
///*********************************
void HWCORE_PI::Callback_IQIN(void)
{//{{{
    mWritePIDataPortsEvent[emiq_dir_reg].notify();
}//}}}

///*********************************
/// Function     : Callback_ID
/// Description  : callback function for ID
// Ref: {HWCORE_PI_UD_Flow_013}
///*********************************
void HWCORE_PI::Callback_ID(void)
{//{{{
    mWritePIDataPortsEvent[emid_reg].notify();
}//}}}

///*********************************
/// Function     : Callback_IQ
/// Description  : callback function for IQ
// Ref: {HWCORE_PI_UD_Flow_013}
///*********************************
void HWCORE_PI::Callback_IQ(void)
{//{{{
    mWritePIDataPortsEvent[emiq_reg].notify();
}//}}}

///*********************************
/// Function     : Callback_VD
/// Description  : callback function for VD
// Ref: {HWCORE_PI_UD_Flow_014}
///*********************************
void HWCORE_PI::Callback_VD(void)
{//{{{
    mWritePIDataPortsEvent[emrdata_em2ivd].notify();
}//}}}

///*********************************
/// Function     : Callback_VQ
/// Description  : callback function for VQ
// Ref: {HWCORE_PI_UD_Flow_014}
///*********************************
void HWCORE_PI::Callback_VQ(void)
{//{{{
    mWritePIDataPortsEvent[emrdata_em2ivq].notify();
}//}}}
