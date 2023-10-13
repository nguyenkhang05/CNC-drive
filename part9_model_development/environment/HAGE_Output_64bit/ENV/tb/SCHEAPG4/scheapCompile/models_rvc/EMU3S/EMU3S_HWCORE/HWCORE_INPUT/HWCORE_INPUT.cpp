// -----------------------------------------------------------------------------
// $Id: HWCORE_INPUT.cpp,v 1.21 2020/11/04 09:37:01 quynhnhunguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// Description: Copyright of HWCORE_INPUT.cpp
// Ref: {HWCORE_INPUT_UD_Copyright_001}
// Description: About the include header files of HWCORE_INPUT.cpp
// Ref: {HWCORE_INPUT_UD_ATT1_002, HWCORE_INPUT_UD_ATT1_004}
#include "HWCORE_INPUT.h"
#include "HWCORE_INPUT_Func.h"
#include "emu3s_sin_table.h"
using namespace std;
/// Constructor and Destructor
///*********************************
/// Function     : HWCORE_INPUT
/// Description  : Constructor
///*********************************
// Description: Constructor of HWCORE_INPUT class
// Ref: {HWCORE_INPUT_UD_Summary_001, HWCORE_INPUT_UD_Ports_001, HWCORE_INPUT_UD_Ports_002, HWCORE_INPUT_UD_Ports_003, HWCORE_INPUT_UD_Ports_004, HWCORE_INPUT_UD_Ports_005, HWCORE_INPUT_UD_Ports_006}
// Ref: {HWCORE_INPUT_UD_Ports_007, HWCORE_INPUT_UD_Ports_008, HWCORE_INPUT_UD_Ports_009, HWCORE_INPUT_UD_Ports_010, HWCORE_INPUT_UD_Ports_011, HWCORE_INPUT_UD_Ports_012, HWCORE_INPUT_UD_Ports_013}
// Ref: {HWCORE_INPUT_UD_Ports_014, HWCORE_INPUT_UD_Ports_015, HWCORE_INPUT_UD_Ports_016, HWCORE_INPUT_UD_Ports_017, HWCORE_INPUT_UD_Ports_018, HWCORE_INPUT_UD_Ports_019, HWCORE_INPUT_UD_Ports_020}
// Ref: {HWCORE_INPUT_UD_Ports_021, HWCORE_INPUT_UD_Ports_022, HWCORE_INPUT_UD_Ports_023, HWCORE_INPUT_UD_Ports_024, HWCORE_INPUT_UD_Ports_025, HWCORE_INPUT_UD_Ports_026, HWCORE_INPUT_UD_Ports_035, HWCORE_INPUT_UD_Ports_036, HWCORE_INPUT_UD_Ports_037}
// Ref: {HWCORE_INPUT_UD_Ports_030, HWCORE_INPUT_UD_Ports_031, HWCORE_INPUT_UD_Ports_032, HWCORE_INPUT_UD_Ports_033, HWCORE_INPUT_UD_BlockDiagram_001, HWCORE_INPUT_UD_ATT1_010, HWCORE_INPUT_UD_ReferenceDoc_001}
// Ref: {HWCORE_INPUT_UD_Feature_001, HWCORE_INPUT_UD_Feature_002, HWCORE_INPUT_UD_Feature_003, HWCORE_INPUT_UD_Feature_004, HWCORE_INPUT_UD_Feature_005, HWCORE_INPUT_UD_ATT1_011, HWCORE_INPUT_UD_ATT1_012}
HWCORE_INPUT::HWCORE_INPUT(sc_module_name name)
        :sc_module(name)
        ,HWCORE_INPUT_AgentController()
        , CRWInterface()
        // Clock/Reset
        ,PCLK("PCLK")
        ,PRESETn("PRESETn")
        ,rst_n("rst_n"),
    //Input port
    input_start_sp("input_start_sp"),
    FUNCIDLEGRPA0("FUNCIDLEGRPA0"),
    FUNCWAITGRPA("FUNCWAITGRPA"),
    FUNCFLGRPA0("FUNCFLGRPA0"),
    adtrg_ch0_dat("adtrg_ch0_dat"),
    adtrg_ch1_dat("adtrg_ch1_dat"),
    adtrg_ch2_dat("adtrg_ch2_dat"),
    adtrg_thte("adtrg_thte"),
    adtrg_thtr("adtrg_thtr"),
    //Output port
    input_end_sp_reg("input_end_sp_reg"),
    input_func_end("input_func_end"),
    input_func_st("input_func_st"),
    input_func_wait("input_func_wait"),
    input_func_busy("input_func_busy"),
    em2ictrinmd_instctr("em2ictrinmd_instctr"),
    theta_e_sel("theta_e_sel"),
    ia_v_lo_fix_reg("ia_v_lo_fix_reg"),
    ia_w_lo_fix_reg("ia_w_lo_fix_reg"),
    ia_u_lo_fix_reg("ia_u_lo_fix_reg"),
    rdata_em2iidfixobuf("rdata_em2iidfixobuf"),
    rdata_em2iiqfixobuf("rdata_em2iiqfixobuf"),
    em3ictrinmd_iirdq("em3ictrinmd_iirdq"),
    rdata_em2ithtrefixin("rdata_em2ithtrefixin"),
    kcl_int("kcl_int"),
    in_ovfl_out("in_ovfl_out"),
    in_zdfl_out("in_zdfl_out"),
    in_ovfl_func("in_ovfl_func"),
    ia_1ch_lo_org_sel("ia_1ch_lo_org_sel"),
    ia_2ch_lo_org_sel("ia_2ch_lo_org_sel"),
    ia_3ch_lo_org_sel("ia_3ch_lo_org_sel")
{//{{{
    CommandInit((std::string)name);

    mpHWCORE_INPUTFunc = new HWCORE_INPUT_Func((std::string)name,this);
    sc_assert(mpHWCORE_INPUTFunc != NULL);

    // Initialize variables
    mPCLKPeriod = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreq = 0;
    mPCLKFreqUnit = "Hz";
    for (int i = 0; i < emNumReset; i++) {
        mResetCurVal[i] = true;
        mResetPreVal[i] = true;
        mIsResetHardActive[i] = false;
        mIsResetCmdReceive[i] = false;
        mIsResetCmdActive[i] = false;
        mResetCmdPeriod[i] = 0;
    }
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mEnableRegisterMessage = true;
    mEnableStatInfo = false;
    mDumpInterrupt = false;
    mStartPCLKTime = 0;
    minput_00_cyc = DEFAULT_PROC_TIME_INPUT0;
    minput_01_cyc = DEFAULT_PROC_TIME_INPUT1;
    minput_02_cyc = DEFAULT_PROC_TIME_INPUT2;
    minput_03_cyc = DEFAULT_PROC_TIME_INPUT3;
    minput_ip_border_cyc = DEFAULT_BORDER_CYCCLE_INPUT;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    Initialize();

    // SC_THREAD/METHOD
    //Clock/Reset
    SC_METHOD(HandlePCLKSignalMethod);
    sensitive << PCLK;

    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;
    
    SC_METHOD(Handlerst_nSignalMethod);
    sensitive << rst_n;
    
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_INPUT::HandleResetHardMethod, this, reset_id),
            sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_INPUT::HandleResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_INPUT::CancelResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
    //Input port
    SC_METHOD(Handleinput_startMethod);
    dont_initialize();
    sensitive << input_start_sp << FUNCIDLEGRPA0;
    SC_METHOD(HandleFUNCWAITGRPAMethod);
    dont_initialize();
    sensitive << FUNCWAITGRPA;
    SC_METHOD(HandleFUNCFLGRPA0Method);
    dont_initialize();
    sensitive << FUNCFLGRPA0;
    SC_METHOD(HandleADTRGMethod);
    dont_initialize();
    sensitive << adtrg_ch0_dat << adtrg_ch1_dat << adtrg_ch2_dat << adtrg_thte << adtrg_thtr;
    //Output port
    SC_METHOD(Writeinput_end_sp_regMethod);
    dont_initialize();
    sensitive << mWriteinput_end_sp_regEvent;
    SC_METHOD(Assertinput_end_sp_regMethod);
    dont_initialize();
    sensitive << mAssertinput_end_sp_regEvent;
    SC_METHOD(Deassertinput_end_sp_regMethod);
    dont_initialize();
    sensitive << mDeassertinput_end_sp_regEvent;

    SC_METHOD(Writeinput_func_endMethod);
    dont_initialize();
    sensitive << mWriteinput_func_endEvent;

    SC_METHOD(Writeinput_func_stMethod);
    dont_initialize();
    sensitive << mWriteinput_func_stEvent;

    SC_METHOD(Writeinput_func_waitMethod);
    dont_initialize();
    sensitive << mWriteinput_func_waitEvent;
    SC_METHOD(Writeinput_func_busyMethod);
    dont_initialize();
    sensitive << mWriteinput_func_busyEvent;
    SC_METHOD(Writein_ovfl_outMethod);
    dont_initialize();
    sensitive << mWritein_ovfl_outEvent;
    SC_METHOD(Writein_zdfl_outMethod);
    dont_initialize();
    sensitive << mWritein_zdfl_outEvent;
    SC_METHOD(Writein_ovfl_funcMethod);
    dont_initialize();
    sensitive << mWritein_ovfl_funcEvent;

    SC_METHOD(Writeem2ictrinmd_instctrMethod);
    dont_initialize();
    sensitive << mWriteem2ictrinmd_instctrEvent;
    SC_METHOD(Writetheta_e_selMethod);
    dont_initialize();
    sensitive << mWritetheta_e_selEvent;
    SC_METHOD(Writeia_v_lo_fix_regMethod);
    dont_initialize();
    sensitive << mWriteia_v_lo_fix_regEvent;
    SC_METHOD(Writeia_w_lo_fix_regMethod);
    dont_initialize();
    sensitive << mWriteia_w_lo_fix_regEvent;
    SC_METHOD(Writeia_u_lo_fix_regMethod);
    dont_initialize();
    sensitive << mWriteia_u_lo_fix_regEvent;
    SC_METHOD(Writerdata_em2iidfixobufMethod);
    dont_initialize();
    sensitive << mWriterdata_em2iidfixobufEvent;
    SC_METHOD(Writerdata_em2iiqfixobufMethod);
    dont_initialize();
    sensitive << mWriterdata_em2iiqfixobufEvent;
    SC_METHOD(Writeem3ictrinmd_iirdqMethod);
    dont_initialize();
    sensitive << mWriteem3ictrinmd_iirdqEvent;
    SC_METHOD(Writerdata_em2ithtrefixinMethod);
    dont_initialize();
    sensitive << mWriterdata_em2ithtrefixinEvent;
    SC_METHOD(Writekcl_intMethod);
    dont_initialize();
    sensitive << mWritekcl_intEvent;
    SC_METHOD(Writeia_1ch_lo_org_selMethod);
    dont_initialize();
    sensitive << mWriteia_1ch_lo_org_selEvent;
    SC_METHOD(Writeia_2ch_lo_org_selMethod);
    dont_initialize();
    sensitive << mWriteia_2ch_lo_org_selEvent;
    SC_METHOD(Writeia_3ch_lo_org_selMethod);
    dont_initialize();
    sensitive << mWriteia_3ch_lo_org_selEvent;
    // State input ip
    SC_METHOD(HandleState_input_ip_00Method);
    dont_initialize();
    sensitive << m_input00;
    SC_METHOD(HandleState_input_ip_01Method);
    dont_initialize();
    sensitive << m_input01;
    SC_METHOD(HandleState_input_ip_02Method);
    dont_initialize();
    sensitive << m_input02;
    SC_METHOD(HandleState_input_ip_03Method);
    dont_initialize();
    sensitive << m_input03;
    //Cycle for functions
    SC_METHOD(HandleAdd_cyc_input_ip_00Method);
    dont_initialize();
    sensitive << m_input00_add_cyc;
    SC_METHOD(HandleAdd_cyc_input_ip_01Method);
    dont_initialize();
    sensitive << m_input01_add_cyc;
    SC_METHOD(HandleAdd_cyc_input_ip_02Method);
    dont_initialize();
    sensitive << m_input02_add_cyc;
    SC_METHOD(HandleAdd_cyc_input_ip_03Method);
    dont_initialize();
    sensitive << m_input03_add_cyc;

    Init_output_port();
}//}}} 

///*********************************
/// Function     : ~HWCORE_INPUT
/// Description  : Destructor
///*********************************
HWCORE_INPUT::~HWCORE_INPUT()
{//{{{
    delete mpHWCORE_INPUTFunc;
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void HWCORE_INPUT::Initialize(void)
{//{{{
    //Port value
    minput_start_sp = false;
    mfuncidlegrpa0 = 0x0;
    mfuncwaitgrpa = 0x0;
    mfuncflgrpa0 = 0x0;
    madtrg_ch0_dat = 0x0;
    madtrg_ch1_dat = 0x0;
    madtrg_ch2_dat = 0x0;
    madtrg_thte = 0x0;
    madtrg_thtr = 0x0;
    minput_end_sp_reg = false;
    minput_func_end = 0x0;
    minput_func_st = 0x0;
    min_ovfl_out = false;
    min_ovfl_func = 0x0;
    minput_func_wait = false;
    minput_func_busy = false;
    mem2ictrinmd_instctr = 0x0;
    mtheta_e_sel = 0x0;
    mia_v_lo_fix_reg= 0x0;
    mia_w_lo_fix_reg= 0x0;
    mia_u_lo_fix_reg= 0x0;
    mrdata_em2iidfixobuf= 0x0;
    mrdata_em2iiqfixobuf= 0x0;
    mem3ictrinmd_iirdq = false;
    mrdata_em2ithtrefixin = 0x0;
    mkcl_int = false;
    mia_1ch_lo_org_sel = 0x0;
    mia_2ch_lo_org_sel = 0x0;
    mia_3ch_lo_org_sel = 0x0;
    //Register value 
    mCTRINMD_KCL = false;
    mCTRINMD_CMES = false;
    mCTRINMD_CMUVM = 0x0;
    mIUFIX = 0x0;
    mIVFIX = 0x0;
    mIWFIX = 0x0;
    mKCLCTR_DATA = 0x0;
    mKCLCTR_EN = false;
    mKCLJUD = 0x0;
    mSR2 = 0x0;
    // Init of cycle
    minput_ip_cyc_sum = 0x0;
    //Init of buffer value
    mCTRINMD_FREGIN_buf_value = true;
    mTHTESFT_buf_value = 0x0;
    mEARD_buf_value = 0x0;
    mADOFS0_buf_value = 0x0;
    mADOFS1_buf_value = 0x0;
    mADOFS2_buf_value = 0x0;
    mDIVLSB_buf_value = 0x0;
}//}}}
///*********************************
/// Function     : Init_output_port 
/// Description  : Initialize output port
///*********************************
void HWCORE_INPUT::Init_output_port(void)
{//{{{
    input_end_sp_reg.initialize(false);
    input_func_end.initialize(0x0);
    input_func_st.initialize(0x0);
    input_func_wait.initialize(false);
    input_func_busy.initialize(false);
    in_ovfl_out.initialize(false);
    in_zdfl_out.initialize(false);
    in_ovfl_func.initialize(0x0);
    em2ictrinmd_instctr.initialize(0x0);
    theta_e_sel.initialize(0x0);
    ia_v_lo_fix_reg.initialize(0x0);
    ia_w_lo_fix_reg.initialize(0x0);
    ia_u_lo_fix_reg.initialize(0x0);
    rdata_em2iidfixobuf.initialize(0x0);
    rdata_em2iiqfixobuf.initialize(0x0);
    em3ictrinmd_iirdq.initialize(false);
    rdata_em2ithtrefixin.initialize(0x0);
    kcl_int.initialize(false);
    ia_1ch_lo_org_sel.initialize(0x0);
    ia_2ch_lo_org_sel.initialize(0x0);
    ia_3ch_lo_org_sel.initialize(0x0);
}//}}}

//========================================================
//============         PYTHON IF       ===================
//========================================================
///*********************************
/// Function     : SetMessageLevel 
/// Description  : Set message level (fatal, error, warning, info)
///*********************************
// Description: SetMessageLevel
// Ref: {HWCORE_INPUT_UD_Direction_001}
void HWCORE_INPUT::SetMessageLevel (const std::string msg_lv)
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

        mpHWCORE_INPUTFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpHWCORE_INPUTFunc->RegisterHandler(cmd);
    } else {
    //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        SeparateString(cmd, msg_lv);
        mpHWCORE_INPUTFunc->RegisterHandler(cmd);
        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        SeparateString(cmd, msg_lv);
        mpHWCORE_INPUTFunc->RegisterHandler(cmd);
    }
}//}}}
///*********************************
/// Function     : DumpRegisterRW 
/// Description  : Enable/disable dumping message when users access registers
///*********************************
// Description: DumpRegisterRW
// Ref: {HWCORE_INPUT_UD_Direction_002, HWCORE_INPUT_UD_Flow_007}
void HWCORE_INPUT::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_INPUTFunc->RegisterHandler(cmd);
}//}}}
///*********************************
/// Function     : DumpInterrupt 
/// Description  : Enable/disable dumping interrupt information
///*********************************
// Description: DumpInterrupt
// Ref: {HWCORE_INPUT_UD_Direction_003,  HWCORE_INPUT_UD_Direction_049, HWCORE_INPUT_UD_Direction_037, HWCORE_INPUT_UD_Direction_051}
void HWCORE_INPUT::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_INPUT_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if(mDumpInterrupt){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info","HWCORE_INPUT_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HWCORE_INPUT_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}
///*********************************
/// Function     : DumpStatInfo 
/// Description  : DumpStatInfo
///*********************************
 // Description: DumpStatInfo
 // Ref: {HWCORE_INPUT_UD_Direction_015, HWCORE_INPUT_UD_Direction_058}
void HWCORE_INPUT::DumpStatInfo(void)
{//{{{
    //"*** Input buffer ***
    re_printf("info", "%s = 0x%08X", "THTESFT_buf", mTHTESFT_buf_value);
    re_printf("info", "%s = 0x%08X", "EARD_buf", mEARD_buf_value);
    re_printf("info", "%s = 0x%08X", "AD0OFS_buf", mADOFS0_buf_value);
    re_printf("info", "%s = 0x%08X", "AD1OFS_buf", mADOFS1_buf_value);
    re_printf("info", "%s = 0x%08X", "AD2OFS_buf", mADOFS2_buf_value);
    re_printf("info", "%s = 0x%08X", "DIVLSB_buf", mDIVLSB_buf_value);
    re_printf("info", "%s = 0x%08X", "AD0IBUFAD0IBUF", mpHWCORE_INPUTFunc->GetRegUnsignedVal("AD0IBUF"));
    re_printf("info", "%s = 0x%08X", "AD1IBUFAD1IBUF", mpHWCORE_INPUTFunc->GetRegUnsignedVal("AD1IBUF"));
    re_printf("info", "%s = 0x%08X", "AD2IBUFAD2IBUF", mpHWCORE_INPUTFunc->GetRegUnsignedVal("AD2IBUF"));
    re_printf("info", "%s = 0x%08X", "THTEIBUF", mpHWCORE_INPUTFunc->GetRegUnsignedVal("THTEIBUF"));

    //*** Output buffer ***
    re_printf("info", "%s = 0x%08X", "IUFIXOBUF_buf", mpHWCORE_INPUTFunc->GetRegSignedVal("IUFIXOBUF"));
    re_printf("info", "%s = 0x%08X", "IVFIXOBUF_buf", mpHWCORE_INPUTFunc->GetRegSignedVal("IVFIXOBUF"));
    re_printf("info", "%s = 0x%08X", "IWFIXOBUF_buf", mpHWCORE_INPUTFunc->GetRegSignedVal("IWFIXOBUF"));
    re_printf("info", "%s = 0x%08X", "IDFIXOBUF_buf", mpHWCORE_INPUTFunc->GetRegSignedVal("IDFIXOBUF"));
    re_printf("info", "%s = 0x%08X", "IQFIXOBUF_buf", mpHWCORE_INPUTFunc->GetRegSignedVal("IQFIXOBUF"));
    re_printf("info", "%s = 0x%08X", "THTRESFIXIN_buf",mpHWCORE_INPUTFunc->GetRegSignedVal("THTRESFIXIN"));
}//}}}

///*********************************
/// Function     : EnableStatInfo
/// Description  : Enable/disable dumping statistical information
///*********************************
 // Description: EnableStatInfo
 // Ref: {HWCORE_INPUT_UD_Direction_005, HWCORE_INPUT_UD_Flow_007, HWCORE_INPUT_UD_Direction_052}
void HWCORE_INPUT::EnableStatInfo(const std::string is_enable)
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
            re_printf("warning", "Invalid argument: HWCORE_INPUT_EnableStatInfo %s\n", is_enable.c_str());
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
        re_printf("info", "HWCORE_INPUT_EnableStatInfo     %s\n", temp_arg.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: HWCORE_INPUT_EnableStatInfo %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : EnableRegisterMessage 
/// Description  : Enable/disable EnableRegisterMessage
///*********************************
// Description: EnableRegisterMessage
// Ref: {HWCORE_INPUT_UD_Direction_004, HWCORE_INPUT_UD_Flow_007, HWCORE_INPUT_UD_Direction_049, HWCORE_INPUT_UD_Direction_036, HWCORE_INPUT_UD_Direction_048}
void HWCORE_INPUT::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_INPUT_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        if (mEnableRegisterMessage) {
            re_printf("info","HWCORE_INPUT_EnableRegisterMessage %s\n", "true"); 
        } else {
            re_printf("info","HWCORE_INPUT_EnableRegisterMessage %s\n", "false"); 
        }

    } else {
        re_printf("warning","Invalid argument: HWCORE_INPUT_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : ChgProcessingTime 
/// Description  : Change the process time
///*********************************
 // Description: ChgProcessingTime
 // Ref: {HWCORE_INPUT_UD_Direction_014, HWCORE_INPUT_UD_Flow_007, HWCORE_INPUT_UD_Direction_055, HWCORE_INPUT_UD_Direction_056}
void HWCORE_INPUT::ChgProcessingTime(const std::string func_name, const unsigned int time_value)
{//{{{
    bool flag = true;
    std::vector<std::string> word_vector;
    SeparateString(word_vector, func_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "input0") {
            minput_00_cyc = time_value;
        }
        else if (word_vector[0] == "input1") {
            minput_01_cyc = time_value;
        }
        else if (word_vector[0] == "input2") {
            minput_02_cyc = time_value;
        }
        else if (word_vector[0] == "input3") {
            minput_03_cyc = time_value;
        }
        else if (word_vector[0] == "input_border") {
            minput_ip_border_cyc = time_value;
        }
        else {
            flag = false;
        }
    }
    else {
        flag = false;
    }
    if (flag == true) {
        re_printf("info", "Change the processing time of cycle %s to 0x%08X.\n", func_name.c_str(), time_value);
    }
    else {
        re_printf("warning", "Function name (%s) is invalid.\n", func_name.c_str());
    }
}//}}}

///*********************************
/// Function     : AssertReset 
/// Description  : Command to assert reset
///*********************************
// Description: AssertReset
// Ref: {HWCORE_INPUT_UD_Direction_006, HWCORE_INPUT_UD_Flow_003, HWCORE_INPUT_UD_Flow_007, HWCORE_INPUT_UD_Direction_044, HWCORE_INPUT_UD_Direction_039, HWCORE_INPUT_UD_Direction_038}
void HWCORE_INPUT::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETnId])&&(!mIsResetCmdReceive[emPRESETnId])) {
            mIsResetCmdReceive[emPRESETnId] = true;
            re_printf("info","The model will be reset (%s) for %f ns after %f ns.\n", reset_name.c_str(), period, start_time);
            mResetCmdEvent[emPRESETnId].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETnId] = period;
        } else {
            re_printf("warning","The software reset of %s is called in the reset operation of the model. So it is ignored.\n", reset_name.c_str());
        }
    } else if (reset_name == "rst_n") {
        if ((!mIsResetHardActive[emrst_nId])&&(!mIsResetCmdReceive[emrst_nId])) {
            mIsResetCmdReceive[emrst_nId] = true;
            re_printf("info","The model will be reset (%s) for %f ns after %f ns.\n", reset_name.c_str(), period, start_time);
            mResetCmdEvent[emrst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[emrst_nId] = period;
        } else {
            re_printf("warning","The software reset of %s is called in the reset operation of the model. So it is ignored\n", reset_name.c_str());
        }
    } else{
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn",reset_name.c_str());
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq 
/// Description  : Set clock value
///*********************************
// Description: SetCLKFreq
// Ref: {HWCORE_INPUT_UD_Direction_007, HWCORE_INPUT_UD_Flow_003, HWCORE_INPUT_UD_Flow_007, HWCORE_INPUT_UD_Direction_036}
void HWCORE_INPUT::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
///*********************************
// Description: GetCLKFreq
// Ref: {HWCORE_INPUT_UD_Direction_008, HWCORE_INPUT_UD_Flow_003, HWCORE_INPUT_UD_Flow_007, HWCORE_INPUT_UD_Direction_036, HWCORE_INPUT_UD_Direction_035, HWCORE_INPUT_UD_Direction_047}
void HWCORE_INPUT::GetCLKFreq (const std::string clock_name)
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
///*********************************
// Description: ForceRegister
// Ref: {HWCORE_INPUT_UD_Direction_009, HWCORE_INPUT_UD_Flow_003, HWCORE_INPUT_UD_Flow_007}
void HWCORE_INPUT::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_INPUTFunc->RegisterHandler(cmd);
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
// Description: ReleaseRegister
// Ref: {HWCORE_INPUT_UD_Direction_010, HWCORE_INPUT_UD_Flow_003, HWCORE_INPUT_UD_Flow_007}
///*********************************
void HWCORE_INPUT::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHWCORE_INPUTFunc->RegisterHandler(cmd);
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
///*********************************
// Description: WriteRegister
// Ref: {HWCORE_INPUT_UD_Direction_011, HWCORE_INPUT_UD_Flow_003, HWCORE_INPUT_UD_Flow_007}
void HWCORE_INPUT::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_INPUTFunc->RegisterHandler(cmd);
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
///*********************************
// Description: ReadRegister
// Ref: {HWCORE_INPUT_UD_Direction_012, HWCORE_INPUT_UD_Flow_003, HWCORE_INPUT_UD_Flow_007}
void HWCORE_INPUT::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpHWCORE_INPUTFunc->RegisterHandler(cmd);
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
///*********************************
// Description: ListRegister
// Ref: {HWCORE_INPUT_UD_Direction_013, HWCORE_INPUT_UD_Flow_003, HWCORE_INPUT_UD_Flow_007}
void HWCORE_INPUT::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHWCORE_INPUTFunc->RegisterHandler(cmd);
}//}}}
///*********************************
/// Function     : Help 
/// Description  : Dump help message of all parameters or commands
///*********************************
// Description: Help
// Ref: {HWCORE_INPUT_UD_Direction_015, HWCORE_INPUT_UD_Direction_014, HWCORE_INPUT_UD_Flow_003, HWCORE_INPUT_UD_Flow_007, HWCORE_INPUT_UD_Direction_028, HWCORE_INPUT_UD_Direction_057, HWCORE_INPUT_UD_Direction_017, HWCORE_INPUT_UD_Direction_018}
void HWCORE_INPUT::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_MessageLevel (\"HWCORE_INPUT instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_DumpRegisterRW (\"HWCORE_INPUT instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_DumpInterrupt (\"HWCORE_INPUT instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_EnableRegisterMessage (\"HWCORE_INPUT instance\", \"true/false\")                               Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(), "HWCORE_INPUT_EnableStatInfo (\"HWCORE_INPUT instance\", \"true/false\")                                     Enable/disable automatically  statistic information of the HWCORE_INPUT(Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_AssertReset (\"HWCORE_INPUT instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the HWCORE_INPUT model");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_SetCLKFreq (\"HWCORE_INPUT instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to HWCORE_INPUT model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_GetCLKFreq (\"HWCORE_INPUT instance\", \"clock_name\")                                          Get clock frequency value of HWCORE_INPUT model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_ForceRegister (\"HWCORE_INPUT instance\",\"reg_name\", reg_value)                               Force register with setting value");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_ReleaseRegister (\"HWCORE_INPUT instance\",\"reg_name\")                                        Release register from force value");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_WriteRegister (\"HWCORE_INPUT instance\",\"reg_name\", reg_value)                               Write a value to a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_ReadRegister (\"HWCORE_INPUT instance\",\"reg_name\")                                           Read a value from a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_ChgProcessingTime (\"HWCORE_INPUT instance\",\"func_name\", value)                              Change the processing of function");
            SC_REPORT_INFO(this->basename(), "HWCORE_INPUT_DumpStatInfo (\"HWCORE_INPUT instance\")                                                       Dump the status information of the HWCORE_INPUT");
            SC_REPORT_INFO(this->basename(),"HWCORE_INPUT_ListRegister (\"HWCORE_INPUT instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of HWCORE_INPUT_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of HWCORE_INPUT_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

//========================================================
//================ Reset and clock  ======================
//========================================================
///Execute reset operation
///*********************************
/// Function     : EnableReset 
/// Description  : Enable Reset
///*********************************
// Description: EnableReset
// Ref: {HWCORE_INPUT_UD_Flow_006}
void HWCORE_INPUT::EnableReset (const bool is_active)
{//{{{
    mpHWCORE_INPUTFunc->EnableReset (is_active);
    if (is_active) {
        Initialize();
        CancelEvents();
        ResetOutputPort();
    }
}//}}}

/// Cancel events
///*********************************
/// Function     : CancelEvents
/// Description  : Cancel all events
///*********************************
void HWCORE_INPUT::CancelEvents()
{//{{{
    mWriteinput_end_sp_regEvent.cancel();
    mAssertinput_end_sp_regEvent.cancel();
    mDeassertinput_end_sp_regEvent.cancel();
    mWriteinput_func_endEvent.cancel();
    mWriteinput_func_stEvent.cancel();
    mWriteinput_func_waitEvent.cancel();
    mWriteinput_func_busyEvent.cancel();
    mWritein_ovfl_outEvent.cancel();
    mWritein_zdfl_outEvent.cancel();
    mWritein_ovfl_funcEvent.cancel();

    mWriteem2ictrinmd_instctrEvent.cancel();
    mWritetheta_e_selEvent.cancel();
    mWriteia_v_lo_fix_regEvent.cancel();
    mWriteia_w_lo_fix_regEvent.cancel();
    mWriteia_u_lo_fix_regEvent.cancel();
    mWriterdata_em2iidfixobufEvent.cancel();
    mWriterdata_em2iiqfixobufEvent.cancel();
    mWriteem3ictrinmd_iirdqEvent.cancel();
    mWriterdata_em2ithtrefixinEvent.cancel();
    mWritekcl_intEvent.cancel();
    mWriteia_1ch_lo_org_selEvent.cancel();
    mWriteia_2ch_lo_org_selEvent.cancel();
    mWriteia_3ch_lo_org_selEvent.cancel();

    m_input00.cancel();
    m_input01.cancel();
    m_input02.cancel();
    m_input03.cancel();

    m_input00_add_cyc.cancel();
    m_input01_add_cyc.cancel();
    m_input02_add_cyc.cancel();
    m_input03_add_cyc.cancel();
}//}}}
///*********************************
/// Function     : DumpInterruptMsg
/// Description  : Dump Interrupt Message
///*********************************
// Description: DumpInterruptMsg
// Ref: {HWCORE_INPUT_UD_Direction_050}
void HWCORE_INPUT::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mDumpInterrupt) {
        if (value) {
            re_printf("info","[%s] is changed to %s.\n",intr_name.c_str(),"1");
        } else {
            re_printf("info","[%s] is changed to %s .\n",intr_name.c_str(),"0");
        }
   }
}//}}}
//========================================================
//============     Method/Threads      ===================
//========================================================
///*********************************
/// Function     : HandlePCLKSignalMethod 
/// Description  : Handle PCLK signal
///*********************************
// Description: HandlePCLKSignalMethod
// Ref: {HWCORE_INPUT_UD_Flow_002}
void HWCORE_INPUT::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandlePRESETnSignalMethod 
/// Description  : Handle PRESETn signal
///*********************************
void HWCORE_INPUT::HandlePRESETnSignalMethod (void)
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
void HWCORE_INPUT::Handlerst_nSignalMethod (void)
{//{{{
    mResetCurVal[emrst_nId] = rst_n.read();      
    mResetPreVal[emrst_nId] = mResetCurVal[emrst_nId]; //mResetPreVal is "true" at initial state
    mResetHardEvent[emrst_nId].notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : HandleResetHardMethod 
/// Description  : Process reset function when reset port is active
///*********************************
// Description: HandleResetHardMethod
// Ref: {HWCORE_INPUT_UD_Flow_001, HWCORE_INPUT_UD_Direction_046, HWCORE_INPUT_UD_Direction_045}
void HWCORE_INPUT::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->mpHWCORE_INPUTFunc->EnableReset(mIsResetHardActive[reset_id]);
            ResetOutputPort();
        } else {
            re_printf("info","The reset port rst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
    } else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is de-asserted.\n");
            this->mpHWCORE_INPUTFunc->EnableReset(mIsResetHardActive[reset_id]);
            ResetOutputPort();
        } else {
            re_printf("info","The reset port rst_n is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
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
///*********************************
// Description: HandleResetCmdMethod
// Ref: {HWCORE_INPUT_UD_Direction_043}
void HWCORE_INPUT::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    mIsResetCmdActive[reset_id] = true;
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
        this->mpHWCORE_INPUTFunc->EnableReset(mIsResetCmdActive[reset_id]);
        ResetOutputPort();
    } else { //reset_id == emrst_nId
        reset_name = "rst_n";
        this->EnableReset(mIsResetCmdActive[reset_id]);
    }
    re_printf("info", "The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
    
}//}}}

///Cancel reset function when reset command is active
///*********************************
/// Function     : CancelResetCmdMethod 
/// Description  : Cancel ResetCmd Method
///*********************************
// Description: CancelResetCmdMethod
// Ref: {HWCORE_INPUT_UD_Direction_042}
void HWCORE_INPUT::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
        this->mpHWCORE_INPUTFunc->EnableReset(mIsResetCmdActive[reset_id]);
    } else {
        reset_name = "rst_n";
        this->EnableReset(mIsResetCmdActive[reset_id]);
    }
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
///*********************************
/// Function     : CheckClockPeriod 
/// Description  : Check Clock Period
///*********************************
// Description: CheckClockPeriod
// Ref: {HWCORE_INPUT_UD_Direction_041}
bool HWCORE_INPUT::CheckClockPeriod (void)
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
bool HWCORE_INPUT::GetResetStatus(const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "rst_n") {
        if (mIsResetHardActive[emrst_nId] || mIsResetCmdActive[emrst_nId]) {
            reset_status = true;
        }
    }
    else {
        if (mIsResetHardActive[emPRESETnId] || mIsResetCmdActive[emPRESETnId]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///*********************************
/// Function     : Handleinput_start_spMethod 
/// Description  : Handle start input ip Method
///*********************************
// Description: Handleinput_start_spMethod
// Ref: {HWCORE_INPUT_UD_Flow_013, HWCORE_INPUT_UD_Direction_053, HWCORE_INPUT_UD_Flow_014, HWCORE_INPUT_UD_Direction_059}
void HWCORE_INPUT::Handleinput_startMethod(void)
{//{{{
    //Reset is asserted or clock is zero
    if (GetResetStatus("rst_n") == true || CheckClockPeriod() == false) {
        return;
    }
    minput_start_sp = input_start_sp.read();
    mfuncidlegrpa0 = FUNCIDLEGRPA0.read();
    if ((minput_start_sp == true) || (((mfuncidlegrpa0 >> 8) & 0x11) == 0x11)) {
        Set_buf_input_ip_start();
        re_printf("info", "HWCORE_INPUT is triggered start up.\n");
        if (mEnableStatInfo == true) {
            re_printf("info", "Information of buffers (start): \n");
            DumpStatInfo();
        }
        if (((mfuncidlegrpa0 >> 8) & 0x1) == 0x00)            /* not select function block */
        {
            m_input00_add_cyc.notify((double)(minput_00_cyc * mPCLKPeriod), mTimeResolutionUnit);
            Set_state_busy_ip();
        }
        else                                                                /* select function block */
        {
            switch (mfuncidlegrpa0)
            {
            case 0x0100:               /* run input0 */
            case 0x1100:
                m_input00_add_cyc.notify((double)(minput_00_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            case 0x0101:               /* run input1 */
            case 0x1101:
                m_input01_add_cyc.notify((double)(minput_01_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            case 0x0102:               /* run input2 */
            case 0x1102:
                m_input02_add_cyc.notify((double)(minput_02_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            case 0x0103:               /* run input3 */
            case 0x1103:
                m_input03_add_cyc.notify((double)(minput_03_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            default:                /* violation setting */
                break;
            }
        }
    }
}//}}}

///*********************************
/// Function     : HandleFUNCWAITGRPAMethod 
/// Description  : Handle FUNCWAITGRPA Method
///*********************************
void HWCORE_INPUT::HandleFUNCWAITGRPAMethod(void)
{//{{{
    mfuncwaitgrpa = FUNCWAITGRPA.read();
    if (input_func_wait.read() == true)    // input ip is wait
    {
        Input_ip_restart_from_wait();
    }
}//}}}

///*********************************
/// Function     : HandleFUNCFLGRPA0Method 
/// Description  : Handle FUNCFLGRPA0 Method
///*********************************
void HWCORE_INPUT::HandleFUNCFLGRPA0Method(void)
{//{{{
    mfuncflgrpa0 = FUNCFLGRPA0.read();
}//}}}

///*********************************
/// Function     : HandleADTRGMethod 
/// Description  : HandleADTRGMethod
///*********************************
void HWCORE_INPUT::HandleADTRGMethod(void)
{//{{{
    madtrg_ch0_dat = adtrg_ch0_dat.read();
    madtrg_ch1_dat = adtrg_ch1_dat.read();
    madtrg_ch2_dat = adtrg_ch2_dat.read();
    madtrg_thte = adtrg_thte.read();
    madtrg_thtr = adtrg_thtr.read();
}//}}}
///*********************************
/// Function     : Writeinput_end_sp_regMethod 
/// Description  : Write input_end_sp_reg Method
///*********************************
void HWCORE_INPUT::Writeinput_end_sp_regMethod(void)
{//{{{
    bool current_value = input_end_sp_reg.read();
    if (current_value != minput_end_sp_reg) {
        input_end_sp_reg.write(minput_end_sp_reg);
        DumpInterruptMsg("input_end_sp_reg", minput_end_sp_reg);
        if (minput_end_sp_reg == true) {
            minput_end_sp_reg = false;
            // Deassert after 1 cycle
            mAssertinput_end_sp_regEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}
///*********************************
/// Function     : Assertinput_end_sp_regMethod 
/// Description  : Assert input_end_sp_reg Method 
///*********************************   
void HWCORE_INPUT::Assertinput_end_sp_regMethod(void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    minput_end_sp_reg = true;
    mWriteinput_end_sp_regEvent.notify();
    // Deassert after 1 cycle
    mDeassertinput_end_sp_regEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
}//}}}
///*********************************
/// Function     : Deassertinput_end_sp_regMethod 
/// Description  : DeAssert input_end_sp_reg Method 
///*********************************
void HWCORE_INPUT::Deassertinput_end_sp_regMethod(void)
{//{{{
    minput_end_sp_reg = false;
    mWriteinput_end_sp_regEvent.notify();
}//}}}

///*********************************
/// Function     : Writeinput_func_endMethod 
/// Description  : Write input_func_end Method
///*********************************
void HWCORE_INPUT::Writeinput_func_endMethod(void)
{//{{{
    unsigned char current_value = input_func_end.read();
    double current_time = sc_time_stamp().to_double();
    if (current_value != minput_func_end) {
        input_func_end.write(minput_func_end);
        minput_func_end = 0x0;
        mWriteinput_func_endEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
    }  
}//}}}

///*********************************
/// Function     : Writeinput_func_stMethod 
/// Description  : Write input_func_st Method
///*********************************
void HWCORE_INPUT::Writeinput_func_stMethod(void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    unsigned char current_value = input_func_st.read();
    if (current_value != minput_func_st) {
        input_func_st.write(minput_func_st);
        minput_func_st = 0x0;
        mWriteinput_func_stEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : Writein_ovfl_outMethod 
/// Description  : Write in_ovfl_out Method
///*********************************   
void HWCORE_INPUT::Writein_ovfl_outMethod(void)
{//{{{
    bool current_value = in_ovfl_out.read();
    if (current_value != min_ovfl_out) {
        in_ovfl_out.write(min_ovfl_out);
        min_ovfl_out = 0x0;
        mWritein_ovfl_outEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : Writein_ovfl_funcMethod 
/// Description  : Write in_ovfl_func Method
///*********************************   
void HWCORE_INPUT::Writein_ovfl_funcMethod(void)
{//{{{
    unsigned char current_value = in_ovfl_func.read();
    if (current_value != min_ovfl_func) {
        in_ovfl_func.write(min_ovfl_func & 0x7);
        min_ovfl_func = 0x0;
        mWritein_ovfl_funcEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : Writeinput_func_busyMethod
/// Description  : Writeinput_func_busyMethod
///*********************************
void HWCORE_INPUT::Writeinput_func_busyMethod(void)
{//{{{
    bool current_value = input_func_busy.read();
    if (current_value != minput_func_busy) {
        input_func_busy.write(minput_func_busy);
    }
}//}}}

///*********************************
/// Function     : Writeinput_func_waitMethod
/// Description  : Writeinput_func_waitMethod
///*********************************
void HWCORE_INPUT::Writeinput_func_waitMethod(void)
{//{{{
    bool current_value = input_func_wait.read();
    if (current_value != minput_func_wait) {
        input_func_wait.write(minput_func_wait);
    }
}//}}}

///*********************************
/// Function     : Writein_zdfl_outMethod 
/// Description  : Write in_zdfl_out Method
///*********************************   
void HWCORE_INPUT::Writein_zdfl_outMethod(void)
{//{{{
    in_zdfl_out.write(0x0);
}
///*********************************
/// Function     : Writeem2ictrinmd_instctrMethod 
/// Description  : Writeem2ictrinmd_instctrMethod
///*********************************   
void HWCORE_INPUT::Writeem2ictrinmd_instctrMethod(void)
{
    unsigned char current_value = em2ictrinmd_instctr.read();
    if (current_value != mem2ictrinmd_instctr) {
        em2ictrinmd_instctr.write(mem2ictrinmd_instctr);
    }
}
///*********************************
/// Function     : Writetheta_e_selMethod 
/// Description  : Writetheta_e_selMethod
///*********************************   
void HWCORE_INPUT::Writetheta_e_selMethod(void)
{
    unsigned short current_value = theta_e_sel.read();
    if (current_value != mtheta_e_sel) {
        theta_e_sel.write(mtheta_e_sel);
    }
}
///*********************************
/// Function     : Writeia_v_lo_fix_regMethod 
/// Description  : Write ia_v_lo_fix_reg Method
///*********************************   
void HWCORE_INPUT::Writeia_v_lo_fix_regMethod(void)
{
    signed int current_value = ia_v_lo_fix_reg.read();
    if (current_value != mia_v_lo_fix_reg) {
        ia_v_lo_fix_reg.write(mia_v_lo_fix_reg);
    }
}
///*********************************
/// Function     : Writeia_w_lo_fix_regMethod 
/// Description  : Write ia_w_lo_fix_reg Method
///*********************************   
void HWCORE_INPUT::Writeia_w_lo_fix_regMethod(void)
{
    signed int current_value = ia_w_lo_fix_reg.read();
    if (current_value != mia_w_lo_fix_reg) {
        ia_w_lo_fix_reg.write(mia_w_lo_fix_reg);
    }
}
///*********************************
/// Function     : Writeia_u_lo_fix_regMethod 
/// Description  : Write ia_u_lo_fix_reg Method
///*********************************   
void HWCORE_INPUT::Writeia_u_lo_fix_regMethod(void)
{
    signed int current_value = ia_u_lo_fix_reg.read();
    if (current_value != mia_u_lo_fix_reg) {
        ia_u_lo_fix_reg.write(mia_u_lo_fix_reg);
    }
}
///*********************************
/// Function     : Writerdata_em2iidfixobufMethod 
/// Description  : Writerdata_em2iidfixobufMethod
///*********************************   
void HWCORE_INPUT::Writerdata_em2iidfixobufMethod(void)
{
    signed int current_value = rdata_em2iidfixobuf.read();
    if (current_value != mrdata_em2iidfixobuf) {
        rdata_em2iidfixobuf.write(mrdata_em2iidfixobuf);
    }
}
///*********************************
/// Function     : Writerdata_em2iiqfixobufMethod 
/// Description  : Writerdata_em2iiqfixobufMethod
///*********************************   
void HWCORE_INPUT::Writerdata_em2iiqfixobufMethod(void)
{
    signed int current_value = rdata_em2iiqfixobuf.read();
    if (current_value != mrdata_em2iiqfixobuf) {
        rdata_em2iiqfixobuf.write(mrdata_em2iiqfixobuf);
    }
}
///*********************************
/// Function     : Writeem3ictrinmd_iirdqMethod 
/// Description  : Write em3ictrinmd_iirdq Method
///*********************************   
void HWCORE_INPUT::Writeem3ictrinmd_iirdqMethod(void)
{
    bool current_value = em3ictrinmd_iirdq.read();
    if (current_value != mem3ictrinmd_iirdq) {
        em3ictrinmd_iirdq.write(mem3ictrinmd_iirdq);
    }
}
///*********************************
/// Function     : Writerdata_em2ithtrefixinMethod 
/// Description  : Write rdata_em2ithtrefixin Method
///*********************************   
void HWCORE_INPUT::Writerdata_em2ithtrefixinMethod(void)
{
    unsigned short current_value = rdata_em2ithtrefixin.read();
    if (current_value != mrdata_em2ithtrefixin) {
        rdata_em2ithtrefixin.write(mrdata_em2ithtrefixin);
    }
}
///*********************************
/// Function     : Writekcl_intMethod 
/// Description  : Write kcl_int Method
///*********************************   
void HWCORE_INPUT::Writekcl_intMethod(void)
{
    bool current_value = kcl_int.read();
    if (current_value != mkcl_int) {
        kcl_int.write(mkcl_int);
    }
}
///*********************************
/// Function     : Writeia_1ch_lo_org_selMethod 
/// Description  : Write ia_1ch_lo_org_sel Method
///*********************************   
void HWCORE_INPUT::Writeia_1ch_lo_org_selMethod(void)
{
    unsigned short current_value = ia_1ch_lo_org_sel.read();
    if (current_value != mia_1ch_lo_org_sel) {
        ia_1ch_lo_org_sel.write(mia_1ch_lo_org_sel);
    }
}

///*********************************
/// Function     : Writeia_2ch_lo_org_selMethod 
/// Description  : Write ia_2ch_lo_org_sel Method
///*********************************   
void HWCORE_INPUT::Writeia_2ch_lo_org_selMethod(void)
{
    unsigned short current_value = ia_2ch_lo_org_sel.read();
    if (current_value != mia_2ch_lo_org_sel) {
        ia_2ch_lo_org_sel.write(mia_2ch_lo_org_sel);
    }
}

///*********************************
/// Function     : Writeia_3ch_lo_org_selMethod 
/// Description  : Write ia_3ch_lo_org_sel Method
///*********************************   
void HWCORE_INPUT::Writeia_3ch_lo_org_selMethod(void)
{
    unsigned short current_value = ia_3ch_lo_org_sel.read();
    if (current_value != mia_3ch_lo_org_sel) {
        ia_3ch_lo_org_sel.write(mia_3ch_lo_org_sel);
    }
}

///*********************************
/// Function     : ResetOutputPort 
/// Description  : ResetOutputPort
///********************************* 
void HWCORE_INPUT::ResetOutputPort(void)
{
    if (GetResetStatus("PRESETn")) {
        mtheta_e_sel = 0x0;
        mia_u_lo_fix_reg = 0x0;
        mia_v_lo_fix_reg = 0x0;
        mia_w_lo_fix_reg = 0x0;
        mrdata_em2iidfixobuf = 0x0;
        mrdata_em2iiqfixobuf = 0x0;
        mia_1ch_lo_org_sel = 0x0;
        mia_2ch_lo_org_sel = 0x0;
        mia_3ch_lo_org_sel = 0x0;
        mem2ictrinmd_instctr = 0x0;
        mem3ictrinmd_iirdq = false;
        mWriteia_u_lo_fix_regEvent.notify(SC_ZERO_TIME);
        mWriteia_v_lo_fix_regEvent.notify(SC_ZERO_TIME);
        mWriteia_w_lo_fix_regEvent.notify(SC_ZERO_TIME);
        mWriterdata_em2iidfixobufEvent.notify(SC_ZERO_TIME);
        mWriterdata_em2iiqfixobufEvent.notify(SC_ZERO_TIME);
        mWriteia_1ch_lo_org_selEvent.notify(SC_ZERO_TIME);
        mWriteia_2ch_lo_org_selEvent.notify(SC_ZERO_TIME);
        mWriteia_3ch_lo_org_selEvent.notify(SC_ZERO_TIME);
        mWritetheta_e_selEvent.notify(SC_ZERO_TIME);
        mWriteem3ictrinmd_iirdqEvent.notify(SC_ZERO_TIME);
        mWriteem2ictrinmd_instctrEvent.notify(SC_ZERO_TIME);
    }
    else if (GetResetStatus("rst_n")){
        mWriteinput_end_sp_regEvent.notify(SC_ZERO_TIME);
        mWriteinput_func_endEvent.notify(SC_ZERO_TIME);
        mWriteinput_func_stEvent.notify(SC_ZERO_TIME);
        mWriteinput_func_waitEvent.notify(SC_ZERO_TIME);
        mWriteinput_func_busyEvent.notify(SC_ZERO_TIME);
        mWritein_ovfl_outEvent.notify(SC_ZERO_TIME);
        mWritein_zdfl_outEvent.notify(SC_ZERO_TIME);
        mWritein_ovfl_funcEvent.notify(SC_ZERO_TIME);
        mWriteem2ictrinmd_instctrEvent.notify(SC_ZERO_TIME);
        mWritetheta_e_selEvent.notify(SC_ZERO_TIME);
        mWriteia_v_lo_fix_regEvent.notify(SC_ZERO_TIME);
        mWriteia_w_lo_fix_regEvent.notify(SC_ZERO_TIME);
        mWriteia_u_lo_fix_regEvent.notify(SC_ZERO_TIME);
        mWriterdata_em2iidfixobufEvent.notify(SC_ZERO_TIME);
        mWriterdata_em2iiqfixobufEvent.notify(SC_ZERO_TIME);
        mWriteem3ictrinmd_iirdqEvent.notify(SC_ZERO_TIME);
        mWriterdata_em2ithtrefixinEvent.notify(SC_ZERO_TIME);
        mWritekcl_intEvent.notify(SC_ZERO_TIME);
        mWriteia_1ch_lo_org_selEvent.notify(SC_ZERO_TIME);
        mWriteia_2ch_lo_org_selEvent.notify(SC_ZERO_TIME);
        mWriteia_3ch_lo_org_selEvent.notify(SC_ZERO_TIME);
    }
}
//}}}

///*********************************
/// Function     : HandleState_input_ip_00Method
/// Description  : Set state input0 Method
///*********************************
// Description: HandleState_input_ip_00Method
// Ref: {HWCORE_INPUT_UD_Flow_013}
void HWCORE_INPUT::HandleState_input_ip_00Method(void)
{//{{{
    /* input0 process */
    Input0();
    /* decide next state and process */
    switch (mfuncflgrpa0 & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_input01_add_cyc.notify((double)(minput_01_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run input01 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_input_ip();
        minput_func_st = 0x1;    /* set last finish func */
        mWriteinput_func_stEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        minput_func_st = 0x1;    /* set last finish func */
        mWriteinput_func_stEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    minput_func_end = 0x1;       /* input0 finish flag set */
    mWriteinput_func_endEvent.notify();
}//}}}
///*********************************
/// Function     : HandleState_input_ip_01Method
/// Description  : Set state input1 Method
///*********************************
// Description: HandleState_input_ip_01Method
// Ref: {HWCORE_INPUT_UD_Flow_013}
void HWCORE_INPUT::HandleState_input_ip_01Method(void)
{//{{{
    /* input1 process */
    Input1_IO();
    /* decide next state and process */
    switch ((mfuncflgrpa0 >> 2) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_input02_add_cyc.notify((double)(minput_02_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run input01 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_input_ip();
        minput_func_st = 0x2;    /* set last finish func */
        mWriteinput_func_stEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        minput_func_st = 0x2;    /* set last finish func */
        mWriteinput_func_stEvent.notify();
        break;
    default:                /* violation setting */
        break;
    }
    minput_func_end = 0x2;       /* input1 finish flag set */
    mWriteinput_func_endEvent.notify();
}//}}}

///*********************************
/// Function     : HandleState_input_ip_02Method
/// Description  : Set state input2 Method
///*********************************
// Description: HandleState_input_ip_02Method
// Ref: {HWCORE_INPUT_UD_Flow_013}
void HWCORE_INPUT::HandleState_input_ip_02Method(void)
{//{{{
    /* input2 process */
    Input2_IO();
    /* decide next state and process */
    switch ((mfuncflgrpa0 >> 4) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_input03_add_cyc.notify((double)(minput_03_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run input01 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_input_ip();
        minput_func_st = 0x4;    /* set last finish func */
        mWriteinput_func_stEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        minput_func_st = 0x4;    /* set last finish func */
        mWriteinput_func_stEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    minput_func_end = 0x4;       /* input2 finish flag set */
    mWriteinput_func_endEvent.notify();
}//}}}
///*********************************
/// Function     : HandleState_input_ip_03Method
/// Description  : Set state input3 Method
///*********************************
// Description: HandleState_input_ip_03Method
// Ref: {HWCORE_INPUT_UD_Flow_013}
void HWCORE_INPUT::HandleState_input_ip_03Method(void)
{//{{{
    /* input3 process */
    Input3_IO();
    /* decide next state and process */
    switch ((mfuncflgrpa0 >> 6) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_idle_ip();
        End_input_ip();
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_input_ip();
        minput_func_st = 0x8;    /* set last finish func */
        mWriteinput_func_stEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        minput_func_st = 0x8;    /* set last finish func */
        mWriteinput_func_stEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    minput_func_end = 0x8;       /* input3 finish flag set */
    mWriteinput_func_endEvent.notify();
}//}}}
///*********************************
/// Function     : HandleAdd_cyc_input_ip_00Method
/// Description  : Add cycle for input0 Method
///*********************************
void HWCORE_INPUT::HandleAdd_cyc_input_ip_00Method(void)
{//{{{
    bool end_flag;
    unsigned short wait_cyc;

    /* wait about border cycle */
    end_flag = Judge_input_ip_end(BLOCK_00);
    wait_cyc = Judge_cyc_input_ip((unsigned short)(minput_00_cyc), end_flag);
    m_input00.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}
///*********************************
/// Function     : HandleAdd_cyc_input_ip_01Method
/// Description  : Add cycle for input1 Method
///*********************************
void HWCORE_INPUT::HandleAdd_cyc_input_ip_01Method(void)
{//{{{
    bool end_flag;
    unsigned short wait_cyc;

    /* wait about border cycle */
    end_flag = Judge_input_ip_end(BLOCK_01);
    wait_cyc = Judge_cyc_input_ip((unsigned short)(minput_01_cyc), end_flag);
    m_input01.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}
///*********************************
/// Function     : HandleAdd_cyc_input_ip_02Method
/// Description  : Add cycle for input2 Method
///*********************************
void HWCORE_INPUT::HandleAdd_cyc_input_ip_02Method(void)
{//{{{
    bool end_flag;
    unsigned short wait_cyc;

    /* wait about border cycle */
    end_flag = Judge_input_ip_end(BLOCK_02);
    wait_cyc = Judge_cyc_input_ip((unsigned short)(minput_02_cyc), end_flag);
    m_input02.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}
///*********************************
/// Function     : HandleAdd_cyc_input_ip_03Method
/// Description  : Add cycle for input1 Method
///*********************************
void HWCORE_INPUT::HandleAdd_cyc_input_ip_03Method(void)
{//{{{
    bool end_flag;
    unsigned short wait_cyc;

    /* wait about border cycle */
    end_flag = Judge_input_ip_end(BLOCK_03);
    wait_cyc = Judge_cyc_input_ip((unsigned short)(minput_03_cyc), end_flag);
    m_input03.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : read 
/// Description  : Read API (target socket)
///*********************************
// Description: read register
// Ref: {HWCORE_INPUT_UD_Flow_004, HWCORE_INPUT_UD_Direction_031, HWCORE_INPUT_UD_ATT1_005}
void HWCORE_INPUT::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) {
        return;
    }
    mpHWCORE_INPUTFunc->read(offsetAddress, data_ptr, size, debug);
}//}}}

///*********************************
/// Function     : write 
/// Description  : Write API (target socket)
///*********************************
// Description: write register
// Ref: {HWCORE_INPUT_UD_Flow_005, HWCORE_INPUT_UD_Direction_030}
void HWCORE_INPUT::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) {
        return;
    }
    mpHWCORE_INPUTFunc->write(offsetAddress, data_ptr, size, debug);
}//}}}
///*********************************
/// Function     : Input0 
/// Description  : Input0 Method
///*********************************
// Ref: {HWCORE_INPUT_UD_Direction_062}
void HWCORE_INPUT::Input0(void)
{//{{{
    re_printf("info", "Start HWCORE_INPUT from Input0");
}//}}}
///*********************************
/// Function     : Input1_IO 
/// Description  : Input1 Input and output Method
///*********************************
// Description: Input1_IO
// Ref: {HWCORE_INPUT_UD_Flow_010}
void HWCORE_INPUT::Input1_IO(void)
{//{{{
    unsigned short ethti = 0;
    unsigned short ethto = 0;

    /* input01 pre process */
    ethti = ((unsigned short) (mpHWCORE_INPUTFunc->GetRegUnsignedVal("THTEIBUF"))) & 0xFFFF;
    ethto = mpHWCORE_INPUTFunc->GetRegUnsignedVal("THTESEL");
    /* run func block input1 */
    Input1(ethti, &ethto);

    /* input1 post process */
    mpHWCORE_INPUTFunc->SetRegUnsignedVal("THTESEL",ethto);
    mtheta_e_sel = (ethto & 0xFFF);
    mWritetheta_e_selEvent.notify();
    /* over flow set */
    if(min_ovfl_out == true){
        mWritein_ovfl_outEvent.notify();
        min_ovfl_func = 0x1;
        mWritein_ovfl_funcEvent.notify();
    }
    mWritein_zdfl_outEvent.notify();

}//}}}
///*********************************
/// Function     : Input1 
/// Description  : Input1 Method
///*********************************
// Description: Input1
// Ref: {HWCORE_INPUT_UD_Flow_010}
void HWCORE_INPUT::Input1(unsigned short ethti, unsigned short *ethto)
{//{{{
    if (ethto == NULL) {
        return;
    }
    if (mCTRINMD_FREGIN_buf_value == false)  /* if the angle data selection bit is 0 */
    {
        *ethto = mTHTESFT_buf_value;        /* and outputs the input IP for electrical angle soft input register */
    }
    else                             /* if the angle data selection bit is 1                              */
    {
        *ethto = (ethti + mEARD_buf_value) & 0x0FFF;
        Chk_overflow_u(((OVFC_U)ethti + (OVFC_U)mEARD_buf_value), 0xFFF, &min_ovfl_out);// Ovf Check
    }
}//}}}
///*********************************
/// Function     : Input2_IO 
/// Description  : Input2 Input and output Method
///*********************************
// Description: Input2_IO
// Ref: {HWCORE_INPUT_UD_Flow_011}
void HWCORE_INPUT::Input2_IO(void)
{//{{{
    signed short adi0 = 0;
    signed short adi1 = 0;
    signed short adi2 = 0;
    signed int iuo = 0;
    signed int ivo = 0;
    signed int iwo = 0;
    bool kclint = false;
    unsigned char kclcnt = 0;
    bool kclstat = false;

    /* input02 pre process */
    adi0 = ((signed short) mpHWCORE_INPUTFunc->GetRegSignedVal("AD0IBUF")) & 0xFFFF;
    adi1 = ((signed short) mpHWCORE_INPUTFunc->GetRegSignedVal("AD1IBUF")) & 0xFFFF;
    adi2 = ((signed short) mpHWCORE_INPUTFunc->GetRegSignedVal("AD2IBUF")) & 0xFFFF;
    mCTRINMD_CMES = (bool)((mpHWCORE_INPUTFunc->GetRegUnsignedVal("CTRINMD") >> 2) & 0x1);
    mCTRINMD_CMUVM = (unsigned char)((mpHWCORE_INPUTFunc->GetRegUnsignedVal("CTRINMD") >> 3) & 0x07);
    mCTRINMD_KCL = (bool)((mpHWCORE_INPUTFunc->GetRegUnsignedVal("CTRINMD") >> 1) & 0x1);
    mKCLCTR_DATA = (unsigned char)(mpHWCORE_INPUTFunc->GetRegUnsignedVal("KCLCTR") & 0xFF);
    mKCLCTR_EN = (bool)((mpHWCORE_INPUTFunc->GetRegUnsignedVal("KCLCTR") >>8) & 0x1);
    mKCLJUD = mpHWCORE_INPUTFunc->GetRegUnsignedVal("KCLJUD");
    kclcnt = mpHWCORE_INPUTFunc->GetRegUnsignedVal("KCLCNT");
    /* run func block input4 */
    Input2(adi0, adi1, adi2, &iuo, &ivo, &iwo, &kclstat, &kclcnt, &kclint);

    /* input2 post process */
    mpHWCORE_INPUTFunc->SetRegSignedVal("IUFIX",iuo);
    mpHWCORE_INPUTFunc->SetRegSignedVal("IVFIX",ivo);
    mpHWCORE_INPUTFunc->SetRegSignedVal("IWFIX",iwo);
    mpHWCORE_INPUTFunc->SetRegUnsignedVal("KCLSTAT",kclstat);
    mpHWCORE_INPUTFunc->SetRegUnsignedVal("KCLCNT",kclcnt);
    mia_u_lo_fix_reg = iuo;
    mia_v_lo_fix_reg = ivo;
    mia_w_lo_fix_reg = iwo;
    mWriteia_u_lo_fix_regEvent.notify();
    mWriteia_v_lo_fix_regEvent.notify();
    mWriteia_w_lo_fix_regEvent.notify();
    mia_1ch_lo_org_sel = mpHWCORE_INPUTFunc->GetRegUnsignedVal("AD0IBUF") & 0xFFF;
    mia_2ch_lo_org_sel = mpHWCORE_INPUTFunc->GetRegUnsignedVal("AD1IBUF") & 0xFFF;
    mia_3ch_lo_org_sel = mpHWCORE_INPUTFunc->GetRegUnsignedVal("AD2IBUF") & 0xFFF;
    mWriteia_1ch_lo_org_selEvent.notify();
    mWriteia_2ch_lo_org_selEvent.notify();
    mWriteia_3ch_lo_org_selEvent.notify();
    /* Kirchhoff violation */
    mkcl_int = kclint;
    mWritekcl_intEvent.notify();
    /* over flow set */
    if (min_ovfl_out == true) {
        mWritein_ovfl_outEvent.notify();
        min_ovfl_func = 0x2;
        mWritein_ovfl_funcEvent.notify();
    }
    mWritein_zdfl_outEvent.notify();
}//}}}
///*********************************
/// Function     : Input2 
/// Description  : Input2 Method
///*********************************
// Description: Input2
// Ref: {HWCORE_INPUT_UD_Flow_011}
void HWCORE_INPUT::Input2(signed short adi0, signed short adi1, signed short adi2, signed int *iuo, 
                            signed int *ivo, signed int *iwo, bool *kclstat, unsigned char *kclcnt, bool *kclint)
{//{{{
    signed short tmp0 = 0, tmp1 = 0, tmp2 = 0;   /* int16_t tmp0, tmp1, tmp2      */
    signed int tmpu = 0, kcl = 0;                /* int32_t tmpu, kcl             */
    if ((ivo == NULL) || (iwo == NULL) || (kclstat == NULL)|| 
        (kclcnt == NULL)|| (kclint == NULL)|| (iuo == NULL)) {
        return;
    }
    tmp0 = adi0 - mADOFS0_buf_value;
    tmp1 = adi1 - mADOFS1_buf_value;
    tmp2 = adi2 - mADOFS2_buf_value;
    *ivo = ((long long int)tmp0 * (long long int)mDIVLSB_buf_value) >> 16;
    *iwo = ((long long int)tmp1 * (long long int)mDIVLSB_buf_value) >> 16;
    tmpu = ((long long int)tmp2 * (long long int)mDIVLSB_buf_value) >> 16;
    if ((mCTRINMD_KCL == true))// && (mCTRINMD_CMES == true) && (mCTRINMD_CMUVM == 0))
    { /* Kirchhoff's current law violation detection */
        kcl = tmpu + *ivo + *iwo;
        /*Ovf Check*/ 
        Chk_overflow_s(((OVFC_S)tmpu + (OVFC_S)*ivo + (OVFC_S)*iwo), &min_ovfl_out);
        if (((long long int)kcl > (long long int)mKCLJUD) || ((long long int)kcl < -(long long int)(mKCLJUD)))
        {
            *kclstat = true;
            if (mKCLCTR_EN == true)
            {
                if ((*kclcnt + 1) > mKCLCTR_DATA)
                {
                    *kclint = true;           /* Interrupt factor occurrence */
                    // An interrupt source is generated when the number of violations exceeded the threshold
                } else {
                    *kclint = false;
                }
                if (*kclcnt < mKCLCTR_DATA)
                {
                    *kclcnt += 1;
                }
                    // *kclcnt is limited EMU3nKCLCTR.DATA or below.
            } else {
                *kclint = true;           /* Interrupt factor occurrence */
            }
        }
        else {
            *kclint = false;
        }
    }
    else
    {
        *kclint = false;
        *kclstat = false;
    }

    if (mCTRINMD_CMES == false)
    {
        *iuo = -(*ivo + *iwo);
        Chk_overflow_s(((OVFC_S)*iwo + (OVFC_S)*iuo), &min_ovfl_out); //Ovf Check
    }
    else
    {
        *iuo = tmpu;

        if (mCTRINMD_CMUVM == 2)
        {
            *ivo = -(*iwo + *iuo);
            Chk_overflow_s(((OVFC_S)*iwo + (OVFC_S)*iuo), &min_ovfl_out); //Ovf Check
        }
        else if (mCTRINMD_CMUVM == 4)
        {
            *iwo = -(*iuo + *ivo);
            Chk_overflow_s(((OVFC_S)*iuo + (OVFC_S)*ivo), &min_ovfl_out); //Ovf Check
        }
        else if (mCTRINMD_CMUVM == 1)
        {
            *iuo = -(*iwo + *ivo);
            Chk_overflow_s(((OVFC_S)*iwo + (OVFC_S)*ivo), &min_ovfl_out); //Ovf Check
        }
    }
}//}}}

///*********************************
/// Function     : Input3_IO 
/// Description  : Input3 Input and output Method
///*********************************
// Description: Input3_IO
// Ref: {HWCORE_INPUT_UD_Flow_012}
void HWCORE_INPUT::Input3_IO(void)
{//{{{
    signed int iui = 0;
    signed int ivi = 0;
    signed int iwi = 0;
    unsigned short ethti = 0;
    signed int ido = 0;
    signed int iqo = 0;

    /* input3 pre process */
    iui = mpHWCORE_INPUTFunc->GetRegSignedVal("IUFIX");
    ivi = mpHWCORE_INPUTFunc->GetRegSignedVal("IVFIX");
    iwi = mpHWCORE_INPUTFunc->GetRegSignedVal("IWFIX");
    ethti = (unsigned short) mpHWCORE_INPUTFunc->GetRegUnsignedVal("THTESEL") & 0xFFFF;
    mSR2 = mpHWCORE_INPUTFunc->GetRegSignedVal("SR2");

    /* run func block input1 */
    Input3(iui, ivi, iwi, ethti, &ido, &iqo);

    /* input5 post process */
    mpHWCORE_INPUTFunc->SetRegSignedVal("IDFIX",ido);
    mpHWCORE_INPUTFunc->SetRegSignedVal("IQFIX",iqo);
    mrdata_em2iidfixobuf = ido;
    mrdata_em2iiqfixobuf = iqo;
    mWriterdata_em2iidfixobufEvent.notify();
    mWriterdata_em2iiqfixobufEvent.notify();
    /* over flow flag set */
    /* over flow flag set */
    if(min_ovfl_out == true){
        mWritein_ovfl_outEvent.notify();
        min_ovfl_func = 0x3;
        mWritein_ovfl_funcEvent.notify();
    }
    mWritein_zdfl_outEvent.notify();
}//}}}
///*********************************
/// Function     : Input3
/// Description  : Input3 Method
///*********************************
// Description: Input3
// Ref: {HWCORE_INPUT_UD_Flow_012}
void HWCORE_INPUT::Input3(signed int iui, signed int ivi, signed int iwi, unsigned short ethti, signed int *ido, signed int *iqo)
{//{{{
    if ((ido == NULL) || (iqo == NULL)) {
        return;
    }

    signed int sin60 = 0, sin30 = 0;    /* int32_t sin60,sin30 */
    signed int cos60 = 0, cos30 = 0;    /* int32_t cos60,cos30 */
    signed int A = 0;

    /* sin(Theta + 60) = (sin(Theta) * cos(60) + cos(Theta) * sin(60) ) >> 15; */
    sin60 = ((long long int)q_sin[ethti & 0x0FFF] * 0x4000 + (long long int)q_sin[(ethti + 0x400) & 0x0FFF] * 0x6EDA) >> 15;
    /*Ovf Check*/ 
    Chk_overflow_s(((OVFC_S)q_sin[ethti & 0x0FFF] * 0x4000 + (OVFC_S)q_sin[(ethti + 0x400) & 0x0FFF] * 0x6EDA) >> 15, &min_ovfl_out);
    /* sin(Theta + 30) = (sin(Theta) * cos(30) + cos(Theta) * sin(30) ) >> 15; */
    sin30 = ((long long int)q_sin[ethti & 0x0FFF] * 0x6EDA + (long long int)q_sin[(ethti + 0x400) & 0x0FFF] * 0x4000) >> 15;
    /*Ovf Check*/ 
    Chk_overflow_s(((OVFC_S)q_sin[ethti & 0x0FFF] * 0x6EDA + (OVFC_S)q_sin[(ethti + 0x400) & 0x0FFF] * 0x4000) >> 15, &min_ovfl_out);
    /* cos(Theta + 60) = (cos(Theta) * cos(60) - sin(Theta) * sin(60)) >> 15; */
    cos60 = ((long long int)q_sin[(ethti + 0x400) & 0x0FFF] * 0x4000 - (long long int)q_sin[ethti & 0x0FFF] * 0x6EDA) >> 15;
    /*Ovf Check*/ 
    Chk_overflow_s(((OVFC_S)q_sin[(ethti + 0x400) & 0x0FFF] * 0x4000 - (OVFC_S)q_sin[ethti & 0x0FFF] * 0x6EDA) >> 15, &min_ovfl_out);
    /* cos(Theta + 30) = (cos(Theta) * cos(30) - sin(Theta) * sin(30)) >> 15; */
    cos30 = ((long long int)q_sin[(ethti + 0x400) & 0x0FFF] * 0x6EDA - (long long int)q_sin[ethti & 0x0FFF] * 0x4000) >> 15;
    /*Ovf Check*/ 
    Chk_overflow_s(((OVFC_S)q_sin[(ethti + 0x400) & 0x0FFF] * 0x6EDA - (OVFC_S)q_sin[ethti & 0x0FFF] * 0x4000) >> 15, &min_ovfl_out);

    *ido = ((long long int)mSR2 * (((((long long int)q_sin[(ethti + 0x400) & 0x0FFF] * (long long int)iui) + A) >> 15)
        - ((((long long int)cos60 * (long long int)ivi) + A) >> 15)
        - ((((long long int)sin30 * (long long int)iwi) + A) >> 15))) >> 16;
    /*Ovf Check*/ 
    Chk_overflow_s(((OVFC_S)mSR2 * ((((OVFC_S)q_sin[(ethti + 0x400) & 0x0FFF] * (OVFC_S)iui + A) >> 15)
        - (((OVFC_S)cos60 * (OVFC_S)ivi + A) >> 15) 
        - (((OVFC_S)sin30 * (OVFC_S)iwi + A) >> 15))) >> 16, &min_ovfl_out);
    *iqo = ((long long int)mSR2 * (-(((long long int)(q_sin[ethti & 0x0FFF] * (long long int)iui) + A) >> 15)
        + ((((long long int)sin60 * (long long int)ivi) + A) >> 15)
        - ((((long long int)cos30 * (long long int)iwi) + A) >> 15))) >> 16;
    /*Ovf Check*/
    Chk_overflow_s(((OVFC_S)mSR2 * (-((OVFC_S)(q_sin[ethti & 0x0FFF] * (OVFC_S)iui + A) >> 15)
        + (((OVFC_S)sin60 * (OVFC_S)ivi + A) >> 15) 
        - (((OVFC_S)cos30 * (OVFC_S)iwi + A) >> 15))) >> 16, &min_ovfl_out);

    /* sin(30) = 0x4000; "sin(30)" is 0x8000 multiplied, it is the value that is set to decimal part 15bit. */
    /* cos(60) = 0x4000; "cos(60)"is 0x8000 multiplied, it is the value that is set to decimal part 15bit.  */
    /* sin(60) = 0x6EDA; "sin(60)"is 0x8000 multiplied, it is the value that is set to decimal part 15bit.  */
    /* cos(30) = 0x6EDA; "cos(30)"is 0x8000 multiplied, it is the value that is set to decimal part 15bit.  */
    /* q_sin, q_sin shows a table of fixed-point data of the fractional part 15bit.                 */
    /* Table resolution is 12bit, to apply the electrical angle of 12bit.                               */
    /* It is a table of "sin values and cos value of each electrical angle 1bit".                           */
}//}}}

///*********************************
/// Function     : Chk_overflow_u
/// Description  : Chk_overflow_u Method
///*********************************
// Description: Chk_overflow_u
// Ref: {HWCORE_INPUT_UD_Flow_015}
void HWCORE_INPUT::Chk_overflow_u( OVFC_U input, OVFC_U check, bool* ovfflag)
{//{{{
    if (ovfflag == NULL) {
        return;
    }
    /* It is determined that the overflow if it exceeds the check value */
    if( (input > check ) )
    {
        /* Overflow flag ON */
        *ovfflag = true;
    }
}//}}}

///*********************************
/// Function     : Chk_overflow_s
/// Description  : Chk_overflow_s Method
///*********************************
// Description: Chk_overflow_s
// Ref: {HWCORE_INPUT_UD_Flow_015}
void HWCORE_INPUT::Chk_overflow_s( OVFC_S input, bool* ovfflag)
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

///*********************************
/// Function     : Set_buf_input_ip_start
/// Description  : Set bufffer input_ip start Method
///*********************************
void HWCORE_INPUT::Set_buf_input_ip_start(void)
{//{{{
    mCTRINMD_FREGIN_buf_value = (bool)(mpHWCORE_INPUTFunc->GetRegUnsignedVal("CTRINMD") & 0x1);
    mTHTESFT_buf_value = mpHWCORE_INPUTFunc->GetRegUnsignedVal("THTESFT");
    mEARD_buf_value = (unsigned short) mpHWCORE_INPUTFunc->GetRegUnsignedVal("EARD") & 0xFFFF;
    mADOFS0_buf_value = mpHWCORE_INPUTFunc->GetRegUnsignedVal("AD0OFS");
    mADOFS1_buf_value = mpHWCORE_INPUTFunc->GetRegUnsignedVal("AD1OFS");
    mADOFS2_buf_value = mpHWCORE_INPUTFunc->GetRegUnsignedVal("AD2OFS");
    mDIVLSB_buf_value = mpHWCORE_INPUTFunc->GetRegUnsignedVal("DIVLSB");
    /* set AD0IBUF register */
    mpHWCORE_INPUTFunc->SetRegUnsignedVal("AD0IBUF",(madtrg_ch0_dat & 0x0FFF));
    /* set AD1IBUF register */
    mpHWCORE_INPUTFunc->SetRegUnsignedVal("AD1IBUF",(madtrg_ch1_dat & 0x0FFF));
    /* set AD2IBUF register */
    mpHWCORE_INPUTFunc->SetRegUnsignedVal("AD2IBUF",(madtrg_ch2_dat & 0x0FFF));

    mpHWCORE_INPUTFunc->SetRegUnsignedVal("THTEIBUF",(madtrg_thte & 0x0FFF));
}//}}}

///*********************************
/// Function     : Judge_cyc_input_ip
/// Description  : Judge cycle for input ip
///*********************************
unsigned short HWCORE_INPUT::Judge_cyc_input_ip(unsigned short cyc, bool end_flag)
{//{{{
    unsigned short wait_cyc;
    minput_ip_cyc_sum += cyc;                        /* add calculation cycle */
    /* end process at this block */
    if (end_flag == true)
    {
        /* calculation cycle is more than border cycle */
        if (minput_ip_cyc_sum >= minput_ip_border_cyc)
        {
            wait_cyc = 0;
        }
        /* calculation cycle is less than border cycle */
        else
        {
            wait_cyc = (unsigned short)(minput_ip_border_cyc - minput_ip_cyc_sum);
        }
    }
    /* continue process */
    else
    {
        wait_cyc = 0;
    }
    return wait_cyc;
}//}}}

///*********************************
/// Function     : Judge_input_ip_end
/// Description  : Judge for input ip end
///*********************************
bool HWCORE_INPUT::Judge_input_ip_end(unsigned char shift)
{//{{{
    unsigned short reg_value;
    unsigned char value;
    bool end_flag = false;

    switch (shift)
    {
    case BLOCK_00:
    case BLOCK_01:
    case BLOCK_02:
    case BLOCK_03:
        /* get register value(whether run next block or end(WAIT or IDLE)) */
        reg_value = FUNCFLGRPA0.read();

        /* shift now block */
        value = (0x03 & ((unsigned char)(reg_value >> shift)));

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

        else
        {
            ;
        }
        break;

    default:
        break;
    }
    return end_flag;
}//}}}

///*********************************
/// Function     : End_input_ip
/// Description  : complete input ip 
///*********************************
// Description: Input3_IO
// Ref: {HWCORE_INPUT_UD_Direction_054, HWCORE_INPUT_UD_Direction_060}
void HWCORE_INPUT::End_input_ip(void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    re_printf("info", "HWCORE_INPUT is end.\n");
    if (mEnableStatInfo == true) {
        re_printf("info", "Information of buffers (end): \n");
        DumpStatInfo();
    }
    mAssertinput_end_sp_regEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
    minput_ip_cyc_sum = 0x0; /* set BUF register */
    mpHWCORE_INPUTFunc->SetRegSignedVal("IUFIXOBUF",(mpHWCORE_INPUTFunc->GetRegSignedVal("IUFIX")));
    mpHWCORE_INPUTFunc->SetRegSignedVal("IVFIXOBUF",(mpHWCORE_INPUTFunc->GetRegSignedVal("IVFIX")));
    mpHWCORE_INPUTFunc->SetRegSignedVal("IWFIXOBUF",(mpHWCORE_INPUTFunc->GetRegSignedVal("IWFIX")));
    mpHWCORE_INPUTFunc->SetRegSignedVal("IDFIXOBUF",(mpHWCORE_INPUTFunc->GetRegSignedVal("IDFIX")));
    mpHWCORE_INPUTFunc->SetRegSignedVal("IQFIXOBUF",(mpHWCORE_INPUTFunc->GetRegSignedVal("IQFIX")));
    mpHWCORE_INPUTFunc->SetRegUnsignedVal("THTRESFIXIN",(madtrg_thtr & 0xFFF));
    mrdata_em2ithtrefixin = (madtrg_thtr & 0xFFF);
    mWriterdata_em2ithtrefixinEvent.notify();
}//}}}

///*********************************
/// Function     : NotifyUpdateOutputPorts
/// Description  : Notify Update Output Ports
///*********************************
// Ref: {HWCORE_INPUT_UD_Direction_061}
void HWCORE_INPUT::NotifyUpdateOutputPorts(const std::string port_name, unsigned int value)
{//{{{
    if (port_name == "theta_e_sel") {
        mtheta_e_sel = (unsigned short)(value & 0xFFF);
        mWritetheta_e_selEvent.notify();
    }
    else if (port_name == "ia_u_lo_fix_reg") {
        mia_u_lo_fix_reg = (signed int)(value);
        mWriteia_u_lo_fix_regEvent.notify();
    }
    else if (port_name == "ia_v_lo_fix_reg") {
        mia_v_lo_fix_reg = (signed int)(value);
        mWriteia_v_lo_fix_regEvent.notify();
    }
    else if (port_name == "ia_w_lo_fix_reg") {
        mia_w_lo_fix_reg = (signed int)(value);
        mWriteia_w_lo_fix_regEvent.notify();
    }
    else if (port_name == "rdata_em2iidfixobuf") {
        mrdata_em2iidfixobuf = (signed int)(value);
        mWriterdata_em2iidfixobufEvent.notify();
    }
    else if (port_name == "rdata_em2iiqfixobuf") {
        mrdata_em2iiqfixobuf = (signed int)(value);
        mWriterdata_em2iiqfixobufEvent.notify();
    }
    else if (port_name == "ia_1ch_lo_org_sel") {
        mia_1ch_lo_org_sel = (unsigned short)(value & 0xFFF);
        mWriteia_1ch_lo_org_selEvent.notify();
    }
    else if (port_name == "ia_2ch_lo_org_sel") {
        mia_2ch_lo_org_sel = (unsigned short)(value & 0xFFF);
        mWriteia_2ch_lo_org_selEvent.notify();
    }
    else  if (port_name == "ia_3ch_lo_org_sel") {
        mia_3ch_lo_org_sel = (unsigned short)(value & 0xFFF);
        mWriteia_3ch_lo_org_selEvent.notify();
    }
    else {
        re_printf("info", "The INSTCTR and IIRDQ bits of CTRINMD register are changed value.\n");
        mem2ictrinmd_instctr = (value & 0x3);
        mem3ictrinmd_iirdq = ((bool)((value >> 6) & 0x1));
        mWriteem3ictrinmd_iirdqEvent.notify();
        mWriteem2ictrinmd_instctrEvent.notify();
    }

}//}}}


///*********************************
/// Function     : Input_ip_restart_from_wait
/// Description  : Restart Input IP from wait
///*********************************
// Description: Input_ip_restart_from_wait
// Ref: {HWCORE_INPUT_UD_Flow_013}
void HWCORE_INPUT::Input_ip_restart_from_wait(void)
{//{{{
    switch (mfuncwaitgrpa)
    {
    case 0x0100:               /* run input0 */
        m_input00_add_cyc.notify((double)(minput_00_cyc * mPCLKPeriod), mTimeResolutionUnit);
        break;

    case 0x0101:               /* run input1 */
        m_input01_add_cyc.notify((double)(minput_01_cyc * mPCLKPeriod), mTimeResolutionUnit);
        break;

    case 0x0102:               /* run input2 */
        m_input02_add_cyc.notify((double)(minput_02_cyc * mPCLKPeriod), mTimeResolutionUnit);
        break;

    case 0x0103:               /* run input3 */
        m_input03_add_cyc.notify((double)(minput_03_cyc * mPCLKPeriod), mTimeResolutionUnit);
        break;

    case 0x0107:               /* end input IP */
        Set_state_idle_ip();
        End_input_ip();
        break;

    default:                /* violation setting */
        break;
    }
}//}}}

///*********************************
/// Function     : Set_state_idle_ip
/// Description  : Set sate idle
///*********************************
void HWCORE_INPUT::Set_state_idle_ip(void)
{//{{{
    minput_func_busy = 0x0;
    minput_func_wait = 0x0;
    mWriteinput_func_waitEvent.notify();
    mWriteinput_func_busyEvent.notify();
}//}}}

///*********************************
/// Function     : Set_state_wait_ip
/// Description  : Set state wait
///*********************************
void HWCORE_INPUT::Set_state_wait_ip(void)
{//{{{
    minput_func_busy = 0x0;
    minput_func_wait = 0x1;
    mWriteinput_func_busyEvent.notify();
    mWriteinput_func_waitEvent.notify();
}//}}}

///*********************************
/// Function     : Set_state_busy_ip
/// Description  : Set state busy
///*********************************
void HWCORE_INPUT::Set_state_busy_ip(void)
{//{{{
    minput_func_wait = 0x0;
    minput_func_busy = 0x1;
    mWriteinput_func_busyEvent.notify();
    mWriteinput_func_waitEvent.notify();
}//}}}
// vim600: set foldmethod=marker :
