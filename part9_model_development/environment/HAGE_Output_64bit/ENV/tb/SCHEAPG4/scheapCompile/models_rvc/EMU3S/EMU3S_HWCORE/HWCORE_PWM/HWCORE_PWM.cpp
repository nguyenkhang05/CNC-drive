// -----------------------------------------------------------------------------
// $Id: HWCORE_PWM.cpp,v 1.13 2020/11/17 06:42:24 ducla Exp $
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
// Description: Copyright of HWCORE_PWM.cpp
// Ref: {HWCORE_PWM_UD_Copyright_001}
// Description: About the include header files of HWCORE_PWM.cpp
// Ref: {HWCORE_PWM_UD_ATT1_002, HWCORE_PWM_UD_ATT1_004}
#include "HWCORE_PWM.h"
#include "HWCORE_PWM_Func.h"
#include "emu3s_sin_table.h"
using namespace std;
/// Constructor and Destructor
///*********************************
/// Function     : HWCORE_PWM
/// Description  : Constructor
///*********************************
// Description: Constructor of HWCORE_PWM class
// Ref: {HWCORE_PWM_UD_Summary_001, HWCORE_PWM_UD_Ports_001, HWCORE_PWM_UD_Ports_002, HWCORE_PWM_UD_Ports_003, HWCORE_PWM_UD_Ports_004, HWCORE_PWM_UD_Ports_005, HWCORE_PWM_UD_Ports_006}
// Ref: {HWCORE_PWM_UD_Ports_007, HWCORE_PWM_UD_Ports_008, HWCORE_PWM_UD_Ports_009, HWCORE_PWM_UD_Ports_010, HWCORE_PWM_UD_Ports_011, HWCORE_PWM_UD_Ports_012, HWCORE_PWM_UD_Ports_013}
// Ref: {HWCORE_PWM_UD_Ports_014, HWCORE_PWM_UD_Ports_015, HWCORE_PWM_UD_Ports_016, HWCORE_PWM_UD_Ports_017, HWCORE_PWM_UD_Ports_018, HWCORE_PWM_UD_Ports_019, HWCORE_PWM_UD_Ports_020}
// Ref: {HWCORE_PWM_UD_Ports_021, HWCORE_PWM_UD_Ports_022, HWCORE_PWM_UD_Ports_023, HWCORE_PWM_UD_Ports_024, HWCORE_PWM_UD_Ports_025, HWCORE_PWM_UD_Ports_026, HWCORE_PWM_UD_Ports_027}
// Ref: {HWCORE_PWM_UD_Ports_028, HWCORE_PWM_UD_Ports_029, HWCORE_PWM_UD_Ports_030, HWCORE_PWM_UD_Ports_031, HWCORE_PWM_UD_Ports_032, HWCORE_PWM_UD_Ports_033, HWCORE_PWM_UD_Ports_034}
// Ref: {HWCORE_PWM_UD_Ports_035, HWCORE_PWM_UD_Ports_036, HWCORE_PWM_UD_Ports_037, HWCORE_PWM_UD_Ports_038, HWCORE_PWM_UD_Ports_039, HWCORE_PWM_UD_Ports_040}
// Ref: {HWCORE_PWM_UD_BlockDiagram_001, HWCORE_PWM_UD_ATT1_010, HWCORE_PWM_UD_ReferenceDoc_001}
// Ref: {HWCORE_PWM_UD_Feature_001, HWCORE_PWM_UD_Feature_002, HWCORE_PWM_UD_Feature_003, HWCORE_PWM_UD_Feature_004, HWCORE_PWM_UD_Feature_005, HWCORE_PWM_UD_Feature_006, HWCORE_PWM_UD_Feature_007}
// Ref: {HWCORE_PWM_UD_Feature_008, HWCORE_PWM_UD_Feature_009, HWCORE_PWM_UD_Feature_010, HWCORE_PWM_UD_Feature_011, HWCORE_PWM_UD_Feature_012, HWCORE_PWM_UD_Feature_013, HWCORE_PWM_UD_Feature_014}
// Ref: {HWCORE_PWM_UD_ATT1_011}
HWCORE_PWM::HWCORE_PWM(sc_module_name name)
        :sc_module(name)
        ,HWCORE_PWM_AgentController()
        , CRWInterface()
        // Clock/Reset
        ,PCLK("PCLK")
        ,PRESETn("PRESETn")
        ,rst_n("rst_n"),
        crst_n("crst_n"),
        // Input port
        output_pwm_start_sp("output_pwm_start_sp"),
        FUNCIDLEGRPA2("FUNCIDLEGRPA2"),
        FUNCWAITGRPA("FUNCWAITGRPA"),
        FUNCFLGRPA2("FUNCFLGRPA2"),
        REFCTR("REFCTR"),
        theta_e_reg("theta_e_reg"),
        ia_u_lo_fix_reg("ia_u_lo_fix_reg"),
        ia_v_lo_fix_reg("ia_v_lo_fix_reg"),
        ia_w_lo_fix_reg("ia_w_lo_fix_reg"),
        id_sel_out("id_sel_out"),
        iq_sel_out("iq_sel_out"),
        id_dir_reg("id_dir_reg"),
        iq_dir_reg("iq_dir_reg"),
        id_reg("id_reg"),
        iq_reg("iq_reg"),
        rdata_em2ivd("rdata_em2ivd"),
        rdata_em2ivq("rdata_em2ivq"),
        theta_e_fix_reg("theta_e_fix_reg"),
        rdc_high_omega_pclk("rdc_high_omega_pclk"),
        // Output
        output_pwm_end_sp_reg("output_pwm_end_sp_reg"),
        pwm_func_st("pwm_func_st"),
        pwm_func_end("pwm_func_end"),
        pwm_func_wait("pwm_func_wait"),
        pwm_func_busy("pwm_func_busy"),
        tstwdcmpu_pclk("tstwdcmpu_pclk"),
        tstwdcmpv_pclk("tstwdcmpv_pclk"),
        tstwdcmpw_pclk("tstwdcmpw_pclk"),
        tstwdcmp0("tstwdcmp0"),
        tstwdcmpu("tstwdcmpu"),
        tstwdcmpv("tstwdcmpv"),
        tstwdcmpw("tstwdcmpw"),
        tstwecmp("tstwecmp"),
        pwm_ovfl_func("pwm_ovfl_func"),
        pwm_ovfl_out("pwm_ovfl_out"),
        pwm_zdfl_out("pwm_zdfl_out")
{//{{{
    CommandInit((std::string)name);

    mpHWCORE_PWMFunc = new HWCORE_PWM_Func((std::string)name, this);
    sc_assert(mpHWCORE_PWMFunc != NULL);

    // Initialize variables
    /* PCLK */
    mPCLKPeriod = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreq = 0;
    mPCLKFreqUnit = "Hz";
    mStartPCLKTime = 0;

    /* CCLK */
    mCCLKPeriod = 0;  
    mCCLKOrgFreq = 0; 
    mCCLKFreq = 0;    
    mCCLKFreqUnit = "Hz";
    mStartCCLKTime = 0;

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

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
        // Cycle value
    m_pwm_00_cyc = DEFAULT_PROC_TIME_PWM0;
    m_pwm_01_cyc = DEFAULT_PROC_TIME_PWM1;
    m_pwm_02_cyc = DEFAULT_PROC_TIME_PWM2;
    m_pwm_03_cyc = DEFAULT_PROC_TIME_PWM3;
    m_pwm_04_cyc = DEFAULT_PROC_TIME_PWM4;
    m_pwm_05_cyc = DEFAULT_PROC_TIME_PWM5;
    m_pwm_06_cyc = DEFAULT_PROC_TIME_PWM6;
    m_pwm_07_cyc = DEFAULT_PROC_TIME_PWM7;
    m_pwm_08_cyc = DEFAULT_PROC_TIME_PWM8;
    m_pwm_09_cyc = DEFAULT_PROC_TIME_PWM9;
    m_pwm_10_cyc = DEFAULT_PROC_TIME_PWM10;
    m_pwm_ip_border_cyc = DEFAULT_BORDER_CYCCLE_PWM;
    

    Initialize();

    // SC_THREAD/METHOD
    //Clock/Reset
    SC_METHOD(HandlePCLKSignalMethod);
    //dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleCCLKSignalMethod);
    //dont_initialize();
    sensitive << CCLK;

    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;
    
    SC_METHOD(Handle_rst_SignalMethod);
    //dont_initialize();
    sensitive << rst_n;

    SC_METHOD(Handle_crst_SignalMethod);
    //dont_initialize();
    sensitive << crst_n;
    
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PWM::HandleResetHardMethod, this, reset_id),
            sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PWM::HandleResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_PWM::CancelResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
    // Input port
    SC_METHOD(Start_pwm_ipMethod);
    dont_initialize();
    sensitive << output_pwm_start_sp << FUNCIDLEGRPA2;
    
    SC_METHOD(Pwm_ip_restart_from_waitMethod);
    dont_initialize();
    sensitive << FUNCWAITGRPA;
    
    // Output port
    SC_METHOD(Ver_output_from_pwm_outMethod);
    dont_initialize();
    sensitive << m_output_to_verification_buff;
    
    SC_METHOD(Handle_pwm_end_sp_regMethod);
    dont_initialize();
    sensitive << mAssert_pwm_end_sp_regEvent;

    SC_METHOD(Write_pwm_func_stMethod);
    dont_initialize();
    sensitive << mWrite_pwm_func_stEvent;
    SC_METHOD(Write_pwm_func_endMethod);
    dont_initialize();
    sensitive << mWrite_pwm_func_endEvent;
    SC_METHOD(Write_pwm_func_waitMethod);
    dont_initialize();
    sensitive << mWrite_pwm_func_waitEvent;
    SC_METHOD(Write_pwm_func_busyMethod);
    dont_initialize();
    sensitive << mWrite_pwm_func_busyEvent;

    SC_METHOD(Write_pwm_ovfl_outMethod);
    dont_initialize();
    sensitive << mWrite_pwm_ovfl_outEvent;
    SC_METHOD(Write_pwm_ovfl_funcMethod);
    dont_initialize();
    sensitive << mWrite_pwm_ovfl_funcEvent;
    SC_METHOD(Write_pwm_zdfl_outMethod);
    dont_initialize();
    sensitive << mWrite_pwm_zdfl_outEvent;

    // TSG3 Output
    SC_METHOD(Tsg_output_from_pwm_outMethod);
    dont_initialize();
    sensitive << m_tsg_output_from_pwm_out;

    // State input ip
    SC_METHOD(Func_pwm_ip_00Method);
    dont_initialize();
    sensitive << m_pwm00;
    SC_METHOD(Func_pwm_ip_01Method);
    dont_initialize();
    sensitive << m_pwm01;
    SC_METHOD(Func_pwm_ip_02Method);
    dont_initialize();
    sensitive << m_pwm02;
    SC_METHOD(Func_pwm_ip_03Method);
    dont_initialize();
    sensitive << m_pwm03;
    SC_METHOD(Func_pwm_ip_04Method);
    dont_initialize();
    sensitive << m_pwm04;
    SC_METHOD(Func_pwm_ip_05Method);
    dont_initialize();
    sensitive << m_pwm05;
    SC_METHOD(Func_pwm_ip_06Method);
    dont_initialize();
    sensitive << m_pwm06;
    SC_METHOD(Func_pwm_ip_07Method);
    dont_initialize();
    sensitive << m_pwm07;
    SC_METHOD(Func_pwm_ip_08Method);
    dont_initialize();
    sensitive << m_pwm08;
    SC_METHOD(Func_pwm_ip_09Method);
    dont_initialize();
    sensitive << m_pwm09;
    SC_METHOD(Func_pwm_ip_10Method);
    dont_initialize();
    sensitive << m_pwm10;
    SC_METHOD(Func_pwm_ip_outMethod);
    dont_initialize();
    sensitive << m_pwm_out;
    
    // Cycle for functions
    SC_METHOD(Add_cyc_pwm_ip_00Method);
    dont_initialize();
    sensitive << m_pwm00_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_01Method);
    dont_initialize();
    sensitive << m_pwm01_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_02Method);
    dont_initialize();
    sensitive << m_pwm02_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_03Method);
    dont_initialize();
    sensitive << m_pwm03_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_04Method);
    dont_initialize();
    sensitive << m_pwm04_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_05Method);
    dont_initialize();
    sensitive << m_pwm05_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_06Method);
    dont_initialize();
    sensitive << m_pwm06_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_07Method);
    dont_initialize();
    sensitive << m_pwm07_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_08Method);
    dont_initialize();
    sensitive << m_pwm08_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_09Method);
    dont_initialize();
    sensitive << m_pwm09_add_cyc;
    SC_METHOD(Add_cyc_pwm_ip_10Method);
    dont_initialize();
    sensitive << m_pwm10_add_cyc;
    
    Init_output_port();

}//}}} 

///*********************************
/// Function     : ~HWCORE_PWM
/// Description  : Destructor
///*********************************
HWCORE_PWM::~HWCORE_PWM()
{//{{{
    delete mpHWCORE_PWMFunc;
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void HWCORE_PWM::Initialize(void)
{//{{{
    // Port value
    m_pwm_start_sp = false;

    m_pwm_end_sp_reg = false;
    m_pwm_func_st = 0x0;
    m_pwm_func_end = 0x0;
    m_pwm_func_wait = false;
    m_pwm_func_busy = false;
    m_pwm_ovfl_out = false;
    m_pwm_zdiv_out = false;
    m_pwm_ovfl_func = 0x0;

    // Registers value
    /* pwm1 */ 
    m_buf_rdc_omega = 0x0;
    m_vdcrct = 0x0;
    m_vqcrct = 0x0;
    m_pwmctr_setdec = false;
    m_pwmctr_setvel = false;
    m_pwmctr_decidq = 0x0;

    m_velsft = 0x0;
    m_vel = 0x0;
    m_decvelg = 0x0;
    m_decflux = 0x0;
    m_decld = 0x0;
    m_declq = 0x0;
    m_vd2max = 0x0;
    m_vq2max = 0x0;

    /* pwm2 */
    m_pwmctr_flininip = 0x0;
    m_thtforesft = 0x0;
    m_phi = 0x0;
    m_gtht = 0x0;

    /* pwm3 */
    m_pwmctr_vdqsel = false;
    m_sr23 = 0x0;

    /* pwm4 */
    m_pwmctr_setharm = 0x0;

    /* pwm5 */
    m_pwmk1 = 0x0;
    m_volv = 0x0;

    /* pwm6 */
    m_vuofs = 0x0;
    m_vvofs = 0x0;
    m_vwofs = 0x0;

    /* pwm7 */
    m_dtul = 0x0;
    m_dtll = 0x0;

    /* pwm8 */
    m_pwmctr_pwmsel = false;
    m_pwmctr_pwmop = 0x0;
    m_carr = 0x0;
    m_dtt = 0x0;
    m_pwmk2 = 0x0;

    /* pwm9 */
    m_pwmctr_setdto = false;
    m_dtoth = 0x0;
    m_dtopv = 0x0;
    m_dtonv = 0x0;

    /* pwm10 */
    m_pwmul = 0x0;
    m_pwmll = 0x0;

    /* pwm_out */
    m_pwmctr_setpwm = false;
    m_pwmctr_shipwm = false;
    m_pwmdt_pwmdt = false;
    m_pwmu = 0x0;
    m_pwmv = 0x0;
    m_pwmw = 0x0;

    /* pwm end */
    m_pwmdt_write_flag = false;
    m_pwm_end_flag = false;

    /* temporary variables for TSG3 output */
    m_tstwdcmpu_temp = 0x0;
    m_tstwdcmpv_temp = 0x0;
    m_tstwdcmpw_temp = 0x0;
    m_tstwdcmp0_temp = 0x0;
    m_tstwecmp_temp = false;
    m_change_tstwecmp_flag = false;

    m_pwm_ip_cyc_sum = 0x0;
}//}}}
///*********************************
/// Function     : Initialize 
/// Description  : Initialize output port
///*********************************
void HWCORE_PWM::Init_output_port(void)
{//{{{
    output_pwm_end_sp_reg.initialize(false);
    pwm_func_st.initialize(0x0);
    pwm_func_end.initialize(0x0);
    pwm_func_wait.initialize(false);
    pwm_func_busy.initialize(false);

    tstwdcmpu_pclk.initialize(0x0);
    tstwdcmpv_pclk.initialize(0x0);
    tstwdcmpw_pclk.initialize(0x0);

    pwm_ovfl_func.initialize(0x0);
    pwm_ovfl_out.initialize(false);
    pwm_zdfl_out.initialize(false);
    
    tstwdcmp0.initialize(0x0);
    tstwdcmpu.initialize(0x0);
    tstwdcmpv.initialize(0x0);
    tstwdcmpw.initialize(0x0);
    tstwecmp.initialize(false);
}//}}}

///*********************************
/// Function     : Reset_output_port
/// Description  : Initialize output port after reset
///*********************************
void HWCORE_PWM::Reset_output_port(void)
{//{{{
    mAssert_pwm_end_sp_regEvent.notify(SC_ZERO_TIME);
    mWrite_pwm_func_stEvent.notify(SC_ZERO_TIME);
    mWrite_pwm_func_endEvent.notify(SC_ZERO_TIME);
    mWrite_pwm_func_waitEvent.notify(SC_ZERO_TIME);
    mWrite_pwm_func_busyEvent.notify(SC_ZERO_TIME);
    
    // reset output port to Verification buffer
    m_output_to_verification_buff.notify(SC_ZERO_TIME);
    
    mWrite_pwm_ovfl_outEvent.notify(SC_ZERO_TIME);
    mWrite_pwm_ovfl_funcEvent.notify(SC_ZERO_TIME);
    mWrite_pwm_zdfl_outEvent.notify(SC_ZERO_TIME);
    
    if (CheckClockPeriod("CCLK") && (!GetResetStatus("crst_n"))) {
        double current_time = sc_time_stamp().to_double();
        m_tsg_output_from_pwm_out.notify(CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mStartCCLKTime), mTimeResolutionUnit);
    }
}//}}}

//========================================================
//============         PYTHON IF       ===================
//========================================================
///*********************************
/// Function     : SetMessageLevel 
/// Description  : Set message level (fatal, error, warning, info)
///*********************************
// Description: SetMessageLevel
// Ref: {HWCORE_PWM_UD_Direction_001}
void HWCORE_PWM::SetMessageLevel (const std::string msg_lv)
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

        mpHWCORE_PWMFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpHWCORE_PWMFunc->RegisterHandler(cmd);
    } else {
    //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        SeparateString(cmd, msg_lv);
        mpHWCORE_PWMFunc->RegisterHandler(cmd);
        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        SeparateString(cmd, msg_lv);
        mpHWCORE_PWMFunc->RegisterHandler(cmd);
    }
}//}}}
///*********************************
/// Function     : DumpRegisterRW 
/// Description  : Enable/disable dumping message when users access registers
///*********************************
// Description: DumpRegisterRW
// Ref: {HWCORE_PWM_UD_Direction_002, HWCORE_PWM_UD_Flow_007}
void HWCORE_PWM::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_PWMFunc->RegisterHandler(cmd);
}//}}}
///*********************************
/// Function     : DumpInterrupt 
/// Description  : Enable/disable dumping interrupt information
///*********************************
// Description: DumpInterrupt
// Ref: {HWCORE_PWM_UD_Direction_004, HWCORE_PWM_UD_Direction_039, HWCORE_PWM_UD_Direction_053, HWCORE_PWM_UD_Direction_054}
void HWCORE_PWM::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_PWM_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if(mDumpInterrupt){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info","HWCORE_PWM_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HWCORE_PWM_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}
///*********************************
/// Function     : DumpStatInfo 
/// Description  : DumpStatInfo
///*********************************
 // Description: DumpStatInfo
 // Ref: {HWCORE_PWM_UD_Direction_015}
void HWCORE_PWM::DumpStatInfo(void)
{//{{{
    //"*** Input buffer ***
    re_printf("info", "mPWMCTR_buffer = 0x%08X", mpHWCORE_PWMFunc->mPWMCTR_buf_value);
    re_printf("info", "mVDCRCT_buffer = 0x%08X", mpHWCORE_PWMFunc->mVDCRCT_buf_value);
    re_printf("info", "mVQCRCT_buffer = 0x%08X", mpHWCORE_PWMFunc->mVQCRCT_buf_value);
    re_printf("info", "mDECVELG_buffer = 0x%08X", mpHWCORE_PWMFunc->mDECVELG_buf_value);
    re_printf("info", "mDECFLUX_buffer = 0x%08X", mpHWCORE_PWMFunc->mDECFLUX_buf_value);
    re_printf("info", "mDECLD_buffer = 0x%08X", mpHWCORE_PWMFunc->mDECLD_buf_value);
    re_printf("info", "mDECLQ_buffer = 0x%08X", mpHWCORE_PWMFunc->mDECLQ_buf_value);
    re_printf("info", "mPHI_buffer = 0x%08X", mpHWCORE_PWMFunc->mPHI_buf_value);
    re_printf("info", "mGTHT_buffer = 0x%08X", mpHWCORE_PWMFunc->mGTHT_buf_value);
    re_printf("info", "mTHTFORESFT_buffer = 0x%08X", mpHWCORE_PWMFunc->mTHTFORESFT_buf_value);
    re_printf("info", "mTHTVSFT_buffer = 0x%08X", mpHWCORE_PWMFunc->mTHTVSFT_buf_value);
    re_printf("info", "mVDQSFT_buffer = 0x%08X", mpHWCORE_PWMFunc->mVDQSFT_buf_value);
    re_printf("info", "mTMAX_buffer = 0x%08X", mpHWCORE_PWMFunc->mTMAX_buf_value);
    re_printf("info", "mVOLV_buffer = 0x%08X", mpHWCORE_PWMFunc->mVOLV_buf_value);
    re_printf("info", "mVUOFS_buffer = 0x%08X", mpHWCORE_PWMFunc->mVUOFS_buf_value);
    re_printf("info", "mVVOFS_buffer = 0x%08X", mpHWCORE_PWMFunc->mVVOFS_buf_value);
    re_printf("info", "mVWOFS_buffer = 0x%08X", mpHWCORE_PWMFunc->mVWOFS_buf_value);
    re_printf("info", "mCARR_buffer = 0x%08X", mpHWCORE_PWMFunc->mCARR_buf_value);
}//}}}

///*********************************
/// Function     : EnableStatisticalInfo
/// Description  : Enable/disable dumping statistical information
///*********************************
 // Description: EnableStatInfo
 // Ref: {HWCORE_PWM_UD_Direction_005, HWCORE_PWM_UD_Flow_007, HWCORE_PWM_UD_Direction_055}
void HWCORE_PWM::EnableStatInfo(const std::string is_enable)
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
            re_printf("warning", "Invalid argument: HWCORE_PWM_EnableStatInfo %s\n", is_enable.c_str());
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
        re_printf("info", "HWCORE_PWM_EnableStatInfo     %s\n", temp_arg.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: HWCORE_PWM_EnableStatInfo %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : EnableRegisterMessage 
/// Description  : Enable/disable EnableRegisterMessage
///*********************************
// Description: EnableRegisterMessage
// Ref: {HWCORE_PWM_UD_Direction_003, HWCORE_PWM_UD_Flow_007, HWCORE_PWM_UD_Direction_038, HWCORE_PWM_UD_Direction_052}
void HWCORE_PWM::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_PWM_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        if (mEnableRegisterMessage) {
            re_printf("info","HWCORE_PWM_EnableRegisterMessage %s\n", "true"); 
        } else {
            re_printf("info","HWCORE_PWM_EnableRegisterMessage %s\n", "false"); 
        }

    } else {
        re_printf("warning","Invalid argument: HWCORE_PWM_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : ChgProcessingTime 
/// Description  : Change the process time
///*********************************
 // Description: ChgProcessingTime
 // Ref: {HWCORE_PWM_UD_Direction_014, HWCORE_PWM_UD_Flow_007, HWCORE_PWM_UD_Direction_058, HWCORE_PWM_UD_Direction_059}
void HWCORE_PWM::ChgProcessingTime(const std::string func_name, const unsigned int time_value)
{//{{{
    bool flag = true;
    std::vector<std::string> word_vector;
    SeparateString(word_vector, func_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pwm0") {
            m_pwm_00_cyc = time_value;
        } else if (word_vector[0] == "pwm1") {
            m_pwm_01_cyc = time_value;
        } else if (word_vector[0] == "pwm2") {
            m_pwm_02_cyc = time_value;
        } else if (word_vector[0] == "pwm3") {
            m_pwm_03_cyc = time_value;
        } else if (word_vector[0] == "pwm4") {
            m_pwm_04_cyc = time_value;
        } else if (word_vector[0] == "pwm5") {
            m_pwm_05_cyc = time_value;
        } else if (word_vector[0] == "pwm6") {
            m_pwm_06_cyc = time_value;
        } else if (word_vector[0] == "pwm7") {
            m_pwm_07_cyc = time_value;
        } else if (word_vector[0] == "pwm8") {
            m_pwm_08_cyc = time_value;
        } else if (word_vector[0] == "pwm9") {
            m_pwm_09_cyc = time_value;
        } else if (word_vector[0] == "pwm10") {
            m_pwm_10_cyc = time_value;
        } else if (word_vector[0] == "pwm_border") {
            m_pwm_ip_border_cyc = time_value;
        } else {
            flag = false;
        }
    } else {
        flag = false;
    }

    if (flag == true) {
        re_printf("info", "Change the processing time of cycle %s to 0x%08X.\n", func_name.c_str(), time_value);
    } else {
        re_printf("warning", "Function name (%s) is invalid.\n", func_name.c_str());
    }
}//}}}

///*********************************
/// Function     : AssertReset 
/// Description  : Command to assert reset
///*********************************
// Description: AssertReset
// Ref: {HWCORE_PWM_UD_Direction_005, HWCORE_PWM_UD_Flow_001, HWCORE_PWM_UD_Flow_006, HWCORE_PWM_UD_Flow_007, HWCORE_PWM_UD_Direction_046, HWCORE_PWM_UD_Direction_040, HWCORE_PWM_UD_Direction_041, HWCORE_PWM_UD_Direction_056, HWCORE_PWM_UD_Direction_047, HWCORE_PWM_UD_Direction_006}
void HWCORE_PWM::AssertReset (const std::string reset_name, const double start_time, const double period)
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

    } else if (reset_name == "crst_n") {
        if ((!mIsResetHardActive[emcrst_nId])&&(!mIsResetCmdReceive[emcrst_nId])) {
            mIsResetCmdReceive[emcrst_nId] = true;
            re_printf("info","Assert the reset (%s) for %f ns after %f ns.\n", reset_name.c_str(), period, start_time);
            mResetCmdEvent[emcrst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[emcrst_nId] = period;
        } else {
            re_printf("warning","The software reset of %s is called in the reset operation of the model. So it is ignored\n", reset_name.c_str());
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn",reset_name.c_str());
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq 
/// Description  : Set clock value
///*********************************
// Description: SetCLKFreq
// Ref: {HWCORE_PWM_UD_Direction_007, HWCORE_PWM_UD_Flow_002, HWCORE_PWM_UD_Flow_007, HWCORE_PWM_UD_Direction_038}
void HWCORE_PWM::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
        } else if (word_vector[0] == "CCLK") {
            mCCLKOrgFreq = freq;
            ConvertClockFreq (mCCLKFreq, mCCLKFreqUnit, freq, unit);
            if (mCCLKFreq > 0) {
                mCCLKPeriod = (sc_dt::uint64)(((1/(double)mCCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartCCLKTime = sc_time_stamp().to_double();
            } else {
                mCCLKPeriod = 0;
                // Cancel events
                CancelEvents_CCLK();
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
// Ref: {HWCORE_PWM_UD_Direction_008, HWCORE_PWM_UD_Flow_002, HWCORE_PWM_UD_Flow_007, HWCORE_PWM_UD_Direction_038, HWCORE_PWM_UD_Direction_050, HWCORE_PWM_UD_Direction_051}
void HWCORE_PWM::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "CCLK") {
            re_printf("info","CCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
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
// Ref: {HWCORE_PWM_UD_Direction_009, HWCORE_PWM_UD_Flow_003, HWCORE_PWM_UD_Flow_007}
void HWCORE_PWM::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_PWMFunc->RegisterHandler(cmd);
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
// Ref: {HWCORE_PWM_UD_Direction_010, HWCORE_PWM_UD_Flow_003, HWCORE_PWM_UD_Flow_007}
///*********************************
void HWCORE_PWM::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHWCORE_PWMFunc->RegisterHandler(cmd);
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
// Ref: {HWCORE_PWM_UD_Direction_011, HWCORE_PWM_UD_Flow_003, HWCORE_PWM_UD_Flow_007}
void HWCORE_PWM::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_PWMFunc->RegisterHandler(cmd);
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
// Ref: {HWCORE_PWM_UD_Direction_012, HWCORE_PWM_UD_Flow_003, HWCORE_PWM_UD_Flow_007}
void HWCORE_PWM::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpHWCORE_PWMFunc->RegisterHandler(cmd);
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
// Ref: {HWCORE_PWM_UD_Direction_013, HWCORE_PWM_UD_Flow_003, HWCORE_PWM_UD_Flow_007}
void HWCORE_PWM::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHWCORE_PWMFunc->RegisterHandler(cmd);
}//}}}
///*********************************
/// Function     : Help 
/// Description  : Dump help message of all parameters or commands
///*********************************
// Description: Help
// Ref: {HWCORE_PWM_UD_Direction_016, HWCORE_PWM_UD_Direction_019, HWCORE_PWM_UD_Flow_003, HWCORE_PWM_UD_Flow_007, HWCORE_PWM_UD_Direction_029, HWCORE_PWM_UD_Direction_017, HWCORE_PWM_UD_Direction_018}
void HWCORE_PWM::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_MessageLevel (\"HWCORE_PWM instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_DumpRegisterRW (\"HWCORE_PWM instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_DumpInterrupt (\"HWCORE_PWM instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_EnableRegisterMessage (\"HWCORE_PWM instance\", \"true/false\")                               Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(), "HWCORE_PWM_EnableStatInfo (\"HWCORE_PWM instance\", \"true/false\")                                     Enable/disable automatically  statistic information of the HWCORE_PWM(Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_AssertReset (\"HWCORE_PWM instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the HWCORE_PWM model");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_SetCLKFreq (\"HWCORE_PWM instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to HWCORE_PWM model. Valid clock_name is PCLK or CCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_GetCLKFreq (\"HWCORE_PWM instance\", \"clock_name\")                                          Get clock frequency value of HWCORE_PWM model. Valid clock_name is PCLK or CCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_ForceRegister (\"HWCORE_PWM instance\",\"reg_name\", reg_value)                               Force register with setting value");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_ReleaseRegister (\"HWCORE_PWM instance\",\"reg_name\")                                        Release register from force value");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_WriteRegister (\"HWCORE_PWM instance\",\"reg_name\", reg_value)                               Write a value to a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_ReadRegister (\"HWCORE_PWM instance\",\"reg_name\")                                           Read a value from a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_ChgProcessingTime (\"HWCORE_PWM instance\",\"func_name\", value)                              Change the processing of function");
            SC_REPORT_INFO(this->basename(), "HWCORE_PWM_DumpStatInfo (\"HWCORE_PWM instance\")                                                       Dump the status information of the HWCORE_PWM");
            SC_REPORT_INFO(this->basename(),"HWCORE_PWM_ListRegister (\"HWCORE_PWM instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of HWCORE_PWM_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of HWCORE_PWM_Help argument is wrong (commands or parameters)\n", type.c_str());
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
void HWCORE_PWM::EnableReset (const bool is_active)
{//{{{
    mpHWCORE_PWMFunc->EnableReset (is_active);
    if (is_active) {
        CancelEvents();
        Initialize();
        Reset_output_port();
    }
}//}}}

///Execute reset operation
///*********************************
/// Function     : EnableResetRegister 
/// Description  : Enable Reset for PRESETn signal
///*********************************
void HWCORE_PWM::EnableResetRegister (const bool is_active)
{//{{{
    mpHWCORE_PWMFunc->EnableResetRegister (is_active);
}//}}}

///Execute reset operation
///*********************************
/// Function     : EnableReset_CCLK 
/// Description  : Enable Reset for CCLK clock
///*********************************
void HWCORE_PWM::EnableReset_CCLK (const bool is_active)
{//{{{
    if (is_active) {
        CancelEvents_CCLK();
    
        m_tstwdcmpu_temp = 0x0;
        m_tstwdcmpv_temp = 0x0;
        m_tstwdcmpw_temp = 0x0;
        m_tstwdcmp0_temp = 0x0;
        m_tstwecmp_temp = false;
    
        if (CheckClockPeriod("CCLK")) {
            double current_time = sc_time_stamp().to_double();
            m_tsg_output_from_pwm_out.notify(CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mStartCCLKTime), mTimeResolutionUnit);
        }
    }
}//}}}

/// Cancel events
///*********************************
/// Function     : CancelEvents
/// Description  : Cancel all events
///*********************************
void HWCORE_PWM::CancelEvents()
{//{{{
    mAssert_pwm_end_sp_regEvent.cancel();

    mWrite_pwm_func_stEvent.cancel();
    mWrite_pwm_func_endEvent.cancel();
    mWrite_pwm_func_waitEvent.cancel();
    mWrite_pwm_func_busyEvent.cancel();

    mWrite_pwm_ovfl_outEvent.cancel();
    mWrite_pwm_ovfl_funcEvent.cancel();
    mWrite_pwm_zdfl_outEvent.cancel();

    m_pwm00.cancel();
    m_pwm01.cancel();
    m_pwm02.cancel();
    m_pwm03.cancel();
    m_pwm04.cancel();
    m_pwm05.cancel();
    m_pwm06.cancel();
    m_pwm07.cancel();
    m_pwm08.cancel();
    m_pwm09.cancel();
    m_pwm10.cancel();
    m_pwm_out.cancel();

    m_pwm00_add_cyc.cancel();
    m_pwm01_add_cyc.cancel();
    m_pwm02_add_cyc.cancel();
    m_pwm03_add_cyc.cancel();
    m_pwm04_add_cyc.cancel();
    m_pwm05_add_cyc.cancel();
    m_pwm06_add_cyc.cancel();
    m_pwm07_add_cyc.cancel();
    m_pwm08_add_cyc.cancel();
    m_pwm09_add_cyc.cancel();
    m_pwm10_add_cyc.cancel(); 
}//}}}

/// Cancel events related to CCLK
///*********************************
/// Function     : CancelEvents_CCLK
/// Description  : Cancel all events related to CCLK
///*********************************
void HWCORE_PWM::CancelEvents_CCLK(void)
{//{{{
    m_tsg_output_from_pwm_out.cancel();
}//}}}

///*********************************
/// Function     : DumpInterruptMsg
/// Description  : Dump Interrupt Message
///*********************************
// Description: DumpInterruptMsg
// Ref: {HWCORE_PWM_UD_Direction_054}
void HWCORE_PWM::DumpInterruptMsg (const std::string intr_name, const bool value)
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
// Ref: {HWCORE_PWM_UD_Flow_002}
void HWCORE_PWM::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleCCLKSignalMethod 
/// Description  : Handle CCLK signal
///*********************************
// Description: HandleCCLKSignalMethod
// Ref: {HWCORE_PWM_UD_Flow_002}
void HWCORE_PWM::HandleCCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CCLK.read();
    SetCLKFreq("CCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandlePRESETnSignalMethod 
/// Description  : Handle PRESETn signal
///*********************************
void HWCORE_PWM::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[emPRESETnId] = PRESETn.read();
    if (mResetCurVal[emPRESETnId] != mResetPreVal[emPRESETnId] ) {       //mResetPreVal is "false" at initial state
        mResetPreVal[emPRESETnId]  = mResetCurVal[emPRESETnId] ;
        mResetHardEvent[emPRESETnId] .notify(SC_ZERO_TIME);
    }
}//}}}

///**********************************************
/// Function     : Handlerst_nSignalMethod 
/// Description  : Handle rst_n and crst_n signal
///**********************************************
void HWCORE_PWM::Handle_rst_SignalMethod (void)
{//{{{
    mResetCurVal[emrst_nId] = rst_n.read();
    
    mResetPreVal[emrst_nId] = mResetCurVal[emrst_nId];
    mResetHardEvent[emrst_nId].notify(SC_ZERO_TIME);
}//}}}

///**********************************************
/// Function     : Handlerst_nSignalMethod 
/// Description  : Handle rst_n and crst_n signal
///**********************************************
void HWCORE_PWM::Handle_crst_SignalMethod (void)
{//{{{
    mResetCurVal[emcrst_nId] = crst_n.read();
    
    mResetPreVal[emcrst_nId] = mResetCurVal[emcrst_nId];
    mResetHardEvent[emcrst_nId].notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : HandleResetHardMethod 
/// Description  : Process reset function when reset port is active
///*********************************
// Description: HandleResetHardMethod
// Ref: {HWCORE_PWM_UD_Flow_001, HWCORE_PWM_UD_Direction_048, HWCORE_PWM_UD_Direction_049}
void HWCORE_PWM::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableResetRegister(mIsResetHardActive[reset_id]);
        } else if (reset_id == emrst_nId) {
            re_printf("info","The reset port rst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port crst_n is asserted.\n");
            this->EnableReset_CCLK(mIsResetHardActive[reset_id]);
        }
    } else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is de-asserted.\n");
            this->EnableResetRegister(mIsResetHardActive[reset_id]);
        } else if (reset_id == emrst_nId) {
            re_printf("info","The reset port rst_n is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port crst_n is de-asserted.\n");
            this->EnableReset_CCLK(mIsResetHardActive[reset_id]);
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
// Ref: {HWCORE_PWM_UD_Direction_046}
void HWCORE_PWM::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
        mIsResetCmdActive[reset_id] = true;
        re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
        this->EnableResetRegister(mIsResetCmdActive[reset_id]);
        mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
    } else if (reset_id == emrst_nId) {
        reset_name = "rst_n";
        mIsResetCmdActive[reset_id] = true;
        re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
        this->EnableReset(mIsResetCmdActive[reset_id]);
        mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
    } else {
        reset_name = "crst_n";
        mIsResetCmdActive[reset_id] = true;
        re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
        this->EnableReset_CCLK(mIsResetCmdActive[reset_id]);
        mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
    }
}//}}}

///Cancel reset function when reset command is active
///*********************************
/// Function     : CancelResetCmdMethod 
/// Description  : Cancel ResetCmd Method
///*********************************
// Description: CancelResetCmdMethod
// Ref: {HWCORE_PWM_UD_Direction_045}
void HWCORE_PWM::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
        mIsResetCmdActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
        this->EnableResetRegister(mIsResetCmdActive[reset_id]);
    } else if (reset_id == emrst_nId) {
        reset_name = "rst_n";
        mIsResetCmdActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
        this->EnableReset(mIsResetCmdActive[reset_id]);
    } else {
        reset_name = "crst_n";
        mIsResetCmdActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
        this->EnableReset_CCLK(mIsResetCmdActive[reset_id]);
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
///*********************************
/// Function     : CheckClockPeriod 
/// Description  : Check Clock Period
///*********************************
// Description: CheckClockPeriod
// Ref: {HWCORE_PWM_UD_Direction_043, HWCORE_PWM_UD_Direction_044}
bool HWCORE_PWM::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    
    if (word_vector[0] == "PCLK") {
        if (mPCLKFreq > 0) {
            clock_status = true;
        } else {
            clock_status = false;
            re_printf("info","PCLK frequency is zero\n");
        }
    } else {
        if (mCCLKFreq > 0) {
            clock_status = true;
        } else {
            clock_status = false;
            re_printf("info","CCLK frequency is zero\n");
        }
    }
    
    return clock_status;
}//}}}
///*********************************
/// Function     : GetResetStatus 
/// Description  : Get Reset Status
///*********************************
bool HWCORE_PWM::GetResetStatus(const std::string reset_name)
{//{{{
    bool reset_status = false;
   
    if (reset_name == "rst_n") {
        if (mIsResetHardActive[emrst_nId] || mIsResetCmdActive[emrst_nId]) {
            reset_status = true;
        }
    } else {
        if (mIsResetHardActive[emcrst_nId] || mIsResetCmdActive[emcrst_nId]) {
            reset_status = true;
        }
    } 
    return reset_status;
}//}}}

///*********************************
/// Function     : Start_pwm_ipMethod 
/// Description  : Handle start pwm ip Method
///*********************************
// Description: Handle start pwm ip Method
// Ref: {HWCORE_PWM_UD_Flow_021, HWCORE_PWM_UD_Flow_022, HWCORE_PWM_UD_Direction_056}
void HWCORE_PWM::Start_pwm_ipMethod(void)
{//{{{
    //Reset is asserted or clock is zero
    if (GetResetStatus("rst_n") == true || CheckClockPeriod("PCLK") == false) {
        return;
    }
    
    m_pwm_start_sp = output_pwm_start_sp.read();
    unsigned short mfuncidlegrpa2 = FUNCIDLEGRPA2.read();

    if (m_pwm_start_sp || (((mfuncidlegrpa2 >> 8) & 0x11) == 0x11)) {
        Set_buf_pwm_ip_start();
        re_printf("info", "HWCORE_PWM is triggered start up.\n");
        if (mEnableStatInfo) {
            re_printf("info", "Information of buffers (start): \n");
            DumpStatInfo();
        }
        if (((mfuncidlegrpa2 >> 8) & 0x1) == 0x00) {          /* not select function block */
            m_pwm00_add_cyc.notify((double)(m_pwm_00_cyc * mPCLKPeriod), mTimeResolutionUnit);
            Set_state_busy_ip();
        }
        else                           /* select function block */
        {
            switch (mfuncidlegrpa2)
            {
            case 0x0100:               /* run pwm0 */
            case 0x1100:
                m_pwm00_add_cyc.notify((double)(m_pwm_00_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            case 0x0101:               /* run pwm1 */
            case 0x1101:
                m_pwm01_add_cyc.notify((double)(m_pwm_01_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            case 0x0102:               /* run pwm2 */
            case 0x1102:
                m_pwm02_add_cyc.notify((double)(m_pwm_02_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            case 0x0103:               /* run pwm3 */
            case 0x1103:
                m_pwm03_add_cyc.notify((double)(m_pwm_03_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;
            
            case 0x0104:               /* run pwm4 */
            case 0x1104:
                m_pwm04_add_cyc.notify((double)(m_pwm_04_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;
            
            case 0x0105:               /* run pwm5 */
            case 0x1105:
                m_pwm05_add_cyc.notify((double)(m_pwm_05_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;
            
            case 0x0106:               /* run pwm6 */
            case 0x1106:
                m_pwm06_add_cyc.notify((double)(m_pwm_06_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;
            
            case 0x0107:               /* run pwm7 */
            case 0x1107:
                m_pwm07_add_cyc.notify((double)(m_pwm_07_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            case 0x0108:               /* run pwm8 */
            case 0x1108:
                m_pwm08_add_cyc.notify((double)(m_pwm_08_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;
            
            case 0x0109:               /* run pwm9 */
            case 0x1109:
                m_pwm09_add_cyc.notify((double)(m_pwm_09_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            case 0x010A:               /* run pwm10 */
            case 0x110A:
                m_pwm10_add_cyc.notify((double)(m_pwm_10_cyc * mPCLKPeriod), mTimeResolutionUnit);
                Set_state_busy_ip();
                break;

            default:                /* violation setting */
                break;
            }
        }
    }
}//}}}

///*********************************
/// Function     : Set_buf_pwm_ip_start
/// Description  : Set bufffer input_ip start Method
///*********************************
void HWCORE_PWM::Set_buf_pwm_ip_start(void)
{//{{{
    m_buf_rdc_omega = rdc_high_omega_pclk.read();
    if ((m_buf_rdc_omega & 0x02000000) == 0x02000000) {
        m_buf_rdc_omega = (0xFC000000 | m_buf_rdc_omega);
    }
    unsigned char mrefctr = REFCTR.read();
    if ((mrefctr & 0x01) == 1) {
        mpHWCORE_PWMFunc->mVDCRCT_buf_value     = mpHWCORE_PWMFunc->GetRegVal("VDCRCT");
        mpHWCORE_PWMFunc->mVQCRCT_buf_value     = mpHWCORE_PWMFunc->GetRegVal("VQCRCT");
        mpHWCORE_PWMFunc->mPHI_buf_value        = mpHWCORE_PWMFunc->GetRegVal("PHI");
        mpHWCORE_PWMFunc->mDECVELG_buf_value    = mpHWCORE_PWMFunc->GetRegVal("DECVELG");
        mpHWCORE_PWMFunc->mDECLD_buf_value      = mpHWCORE_PWMFunc->GetRegVal("DECLD");
        mpHWCORE_PWMFunc->mDECLQ_buf_value      = mpHWCORE_PWMFunc->GetRegVal("DECLQ");
        mpHWCORE_PWMFunc->mDECFLUX_buf_value    = mpHWCORE_PWMFunc->GetRegVal("DECFLUX");
        mpHWCORE_PWMFunc->mTMAX_buf_value       = mpHWCORE_PWMFunc->GetRegVal("TMAX");
        mpHWCORE_PWMFunc->mTHTVSFT_buf_value    = mpHWCORE_PWMFunc->GetRegVal("THTVSFT");
        mpHWCORE_PWMFunc->mVDQSFT_buf_value     = mpHWCORE_PWMFunc->GetRegVal("VDQSFT");
    }
    mpHWCORE_PWMFunc->mGTHT_buf_value       = mpHWCORE_PWMFunc->GetRegVal("GTHT");
    mpHWCORE_PWMFunc->mTHTFORESFT_buf_value = mpHWCORE_PWMFunc->GetRegVal("THTFORESFT");
    mpHWCORE_PWMFunc->mVOLV_buf_value       = mpHWCORE_PWMFunc->GetRegVal("VOLV");
    mpHWCORE_PWMFunc->mVUOFS_buf_value      = mpHWCORE_PWMFunc->GetRegVal("VUOFS");
    mpHWCORE_PWMFunc->mVVOFS_buf_value      = mpHWCORE_PWMFunc->GetRegVal("VVOFS");
    mpHWCORE_PWMFunc->mVWOFS_buf_value      = mpHWCORE_PWMFunc->GetRegVal("VWOFS");
    mpHWCORE_PWMFunc->mCARR_buf_value       = mpHWCORE_PWMFunc->GetRegVal("CARR");
    mpHWCORE_PWMFunc->mPWMCTR_buf_value     = mpHWCORE_PWMFunc->GetRegVal("PWMCTR");
}//}}}

///*********************************
/// Function     : Pwm_ip_restart_from_waitMethod
/// Description  : start pwm_ip from wait state
///*********************************
void HWCORE_PWM::Pwm_ip_restart_from_waitMethod(void)
{//{{{
    unsigned short mfuncwaitgrpa = FUNCWAITGRPA.read();

    if (pwm_func_wait.read() == true) {
        switch (mfuncwaitgrpa) {
    
        case 0x0300:    // run pwm0
            m_pwm00_add_cyc.notify((double)(m_pwm_00_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;
        
        case 0x0301:    // run pwm1
            m_pwm01_add_cyc.notify((double)(m_pwm_01_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;
        
        case 0x0302:    // run pwm2
            m_pwm02_add_cyc.notify((double)(m_pwm_02_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;

        case 0x0303:    // run pwm3
            m_pwm03_add_cyc.notify((double)(m_pwm_03_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;

        case 0x0304:    // run pwm4
            m_pwm04_add_cyc.notify((double)(m_pwm_04_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;

        case 0x0305:    // run pwm5
            m_pwm05_add_cyc.notify((double)(m_pwm_05_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;

        case 0x0306:    // run pwm6
            m_pwm06_add_cyc.notify((double)(m_pwm_06_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;

        case 0x0307:    // run pwm7
            m_pwm07_add_cyc.notify((double)(m_pwm_07_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;

        case 0x0308:    // run pwm8
            m_pwm08_add_cyc.notify((double)(m_pwm_08_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;

        case 0x0309:    // run pwm9
            m_pwm09_add_cyc.notify((double)(m_pwm_09_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;

        case 0x030A:    // run pwm10
            m_pwm10_add_cyc.notify((double)(m_pwm_10_cyc * mPCLKPeriod), mTimeResolutionUnit);
            break;
        
        case 0x030B:    // end pwm IP
            Set_state_idle_ip();
            End_pwm_ip();
            break;

        default:        // Violation setting
            break;
        }
    }
}//}}}

///*********************************
/// Function     : Set_state_idle_ip 
/// Description  : Set state idle for pwm ip
///*********************************
void HWCORE_PWM::Set_state_idle_ip(void)
{//{{{
    m_pwm_func_busy = false;
    m_pwm_func_wait = false;
    mWrite_pwm_func_waitEvent.notify();
    mWrite_pwm_func_busyEvent.notify();
}//}}}

///*********************************
/// Function     : Set_state_wait_ip 
/// Description  : Set state wait for pwm ip
///*********************************
void HWCORE_PWM::Set_state_wait_ip(void)
{//{{{
    m_pwm_func_busy = false;
    m_pwm_func_wait = true;
    mWrite_pwm_func_busyEvent.notify();
    mWrite_pwm_func_waitEvent.notify();
}//}}}

///*********************************
/// Function     : Set_state_busy_ip
/// Description  : Set state busy
///*********************************
void HWCORE_PWM::Set_state_busy_ip(void)
{//{{{
    m_pwm_func_busy = true;
    m_pwm_func_wait = false;
    mWrite_pwm_func_busyEvent.notify();
    mWrite_pwm_func_waitEvent.notify();
}//}}}

///*********************************
/// Function     : Handle_pwm_end_sp_regMethod 
/// Description  : Assert output_pwm_end_sp_reg Method and deassert it after 1 PCLK period 
///*********************************   
void HWCORE_PWM::Handle_pwm_end_sp_regMethod(void)
{//{{{
    bool current_value = output_pwm_end_sp_reg.read();
    if (current_value != m_pwm_end_sp_reg) {
        output_pwm_end_sp_reg.write(m_pwm_end_sp_reg);
        DumpInterruptMsg("output_pwm_end_sp_reg", m_pwm_end_sp_reg);
        if (m_pwm_end_sp_reg == true) {
            m_pwm_end_sp_reg = false;
            // Deassert after 1 cycle
            mAssert_pwm_end_sp_regEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

///*********************************
/// Function     : Write_pwm_func_busyMethod
/// Description  : Write pwm_func_busy Method
///*********************************
void HWCORE_PWM::Write_pwm_func_busyMethod(void)
{//{{{
    bool current_value = pwm_func_busy.read();

    if (current_value != m_pwm_func_busy) {
        pwm_func_busy.write(m_pwm_func_busy);
    }
}//}}}


///*********************************
/// Function     : Write_pwm_func_waitMethod
/// Description  : Write pwm_func_wait Method
///*********************************
void HWCORE_PWM::Write_pwm_func_waitMethod(void)
{//{{{
    bool current_value = pwm_func_wait.read();
    if (current_value != m_pwm_func_wait) {
        pwm_func_wait.write(m_pwm_func_wait);
    }
}//}}}

///*********************************
/// Function     : Write_pwm_func_stMethod 
/// Description  : Write pwm_func_st Method
///*********************************
void HWCORE_PWM::Write_pwm_func_stMethod(void)
{//{{{
    pwm_func_st.write(m_pwm_func_st);

    if (m_pwm_func_st != 0x0) {
        m_pwm_func_st = 0x0;
        mWrite_pwm_func_stEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : Write_pwm_func_endMethod 
/// Description  : Write pwm_func_end Method
///*********************************
void HWCORE_PWM::Write_pwm_func_endMethod(void)
{//{{{
    pwm_func_end.write(m_pwm_func_end);

    if (m_pwm_func_end != 0x0) {
        m_pwm_func_end = 0x0;
        mWrite_pwm_func_endEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : Write_pwm_ovfl_outMethod 
/// Description  : Write pwm_ovfl_out Method
///*********************************   
void HWCORE_PWM::Write_pwm_ovfl_outMethod(void)
{//{{{
    pwm_ovfl_out.write(m_pwm_ovfl_out);
    
    if (m_pwm_ovfl_out) {
        m_pwm_ovfl_out = false;
        mWrite_pwm_ovfl_outEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : Write_pwm_ovfl_funcMethod 
/// Description  : Write pwm_ovfl_func Method
///*********************************   
void HWCORE_PWM::Write_pwm_ovfl_funcMethod(void)
{//{{{
    pwm_ovfl_func.write(m_pwm_ovfl_func);
    
    if (m_pwm_ovfl_func != 0x0) {
        m_pwm_ovfl_func = 0x0;
        mWrite_pwm_ovfl_funcEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : Write_pwm_zdfl_outMethod 
/// Description  : Write pwm_zdfl_out Method
///*********************************   
void HWCORE_PWM::Write_pwm_zdfl_outMethod(void)
{//{{{
    pwm_zdfl_out.write(m_pwm_zdiv_out);
    
    if (m_pwm_zdiv_out) {
        m_pwm_zdiv_out = false;
        mWrite_pwm_zdfl_outEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : Func_pwm_ip_00Method
/// Description  : Set state pwm0 Method
///*********************************
// Description: Set state pwm0 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_00Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm0_io();

    /* decide next state and process */
    switch (mfuncflgrpa2 & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm01_add_cyc.notify((double)(m_pwm_01_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm01 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x1;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x1;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x1;       /* pwm0 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_01Method
/// Description  : Set state pwm1 Method
///*********************************
// Description: Set state pwm1 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_01Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm1_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 2) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm02_add_cyc.notify((double)(m_pwm_02_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm02 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x2;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x2;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x2;       /* pwm1 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_02Method
/// Description  : Set state pwm2 Method
///*********************************
// Description: Set state pwm2 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_02Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm2_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 4) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm03_add_cyc.notify((double)(m_pwm_03_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm03 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x4;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x4;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x4;       /* pwm2 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_03Method
/// Description  : Set state pwm3 Method
///*********************************
// Description: Set state pwm3 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_03Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm3_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 6) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm04_add_cyc.notify((double)(m_pwm_04_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm02 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x8;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x8;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x8;       /* pwm3 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_04Method
/// Description  : Set state pwm4 Method
///*********************************
// Description: Set state pwm4 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_04Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm4_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 8) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm05_add_cyc.notify((double)(m_pwm_05_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm02 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x10;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x10;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x10;       /* pwm4 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_05Method
/// Description  : Set state pwm5 Method
///*********************************
// Description: Set state pwm5 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_05Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm5_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 10) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm06_add_cyc.notify((double)(m_pwm_06_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm06 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x20;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x20;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x20;       /* pwm5 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_06Method
/// Description  : Set state pwm6 Method
///*********************************
// Description: Set state pwm6 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_06Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm6_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 12) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm07_add_cyc.notify((double)(m_pwm_07_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm07 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x40;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x40;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x40;       /* pwm6 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_07Method
/// Description  : Set state pwm7 Method
///*********************************
// Description: Set state pwm7 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_07Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm7_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 14) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm08_add_cyc.notify((double)(m_pwm_08_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm08 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x80;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x80;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x80;       /* pwm7 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_08Method
/// Description  : Set state pwm8 Method
///*********************************
// Description: Set state pwm8 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_08Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm8_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 16) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm09_add_cyc.notify((double)(m_pwm_09_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm09 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x100;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x100;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x100;       /* pwm8 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_09Method
/// Description  : Set state pwm9 Method
///*********************************
// Description: Set state pwm9 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_09Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm9_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 18) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_busy_ip();
        m_pwm10_add_cyc.notify((double)(m_pwm_10_cyc * mPCLKPeriod), mTimeResolutionUnit);     /* run pwm10 */
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x200;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x200;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x200;       /* pwm9 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}

///*********************************
/// Function     : Func_pwm_ip_10Method
/// Description  : Set state pwm10 Method
///*********************************
// Description: Set state pwm10 Method
// Ref: {HWCORE_PWM_UD_Flow_021}
void HWCORE_PWM::Func_pwm_ip_10Method(void)
{//{{{
    unsigned int mfuncflgrpa2 = FUNCFLGRPA2.read();

    /* pwm0 process */
    Pwm10_io();

    /* decide next state and process */
    switch ((mfuncflgrpa2 >> 20) & 0x03)
    {
    case 0x00:              /* run next block */
        Set_state_idle_ip();
        End_pwm_ip();
        break;

    case 0x01:              /* change state to IDLE */
        Set_state_idle_ip();
        End_pwm_ip();
        m_pwm_func_end = 0x400;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    case 0x02:              /* change state to WAIT */
        Set_state_wait_ip();
        m_pwm_func_end = 0x400;    /* set last finish func */
        mWrite_pwm_func_endEvent.notify();
        break;

    default:                /* violation setting */
        break;
    }
    m_pwm_func_st = 0x400;       /* pwm10 finish flag set */
    mWrite_pwm_func_stEvent.notify();
}//}}}


///*********************************
/// Function     : Add_cyc_pwm_ip_00Method
/// Description  : Add cycle for pwm0 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_00Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_00);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_00_cyc), end_flag);
    m_pwm00.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_01Method
/// Description  : Add cycle for pwm1 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_01Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_01);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_01_cyc), end_flag);
    m_pwm01.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_02Method
/// Description  : Add cycle for pwm2 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_02Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_02);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_02_cyc), end_flag);
    m_pwm02.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_03Method
/// Description  : Add cycle for pwm3 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_03Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_03);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_03_cyc), end_flag);
    m_pwm03.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_04Method
/// Description  : Add cycle for pwm4 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_04Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_04);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_04_cyc), end_flag);
    m_pwm04.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_05Method
/// Description  : Add cycle for pwm5 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_05Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_05);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_05_cyc), end_flag);
    m_pwm05.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_06Method
/// Description  : Add cycle for pwm6 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_06Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_06);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_06_cyc), end_flag);
    m_pwm06.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_07Method
/// Description  : Add cycle for pwm7 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_07Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_07);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_07_cyc), end_flag);
    m_pwm07.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_08Method
/// Description  : Add cycle for pwm8 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_08Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_08);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_08_cyc), end_flag);
    m_pwm08.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_09Method
/// Description  : Add cycle for pwm9 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_09Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_09);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_09_cyc), end_flag);
    m_pwm09.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Add_cyc_pwm_ip_10Method
/// Description  : Add cycle for pwm10 Method
///*********************************
void HWCORE_PWM::Add_cyc_pwm_ip_10Method(void)
{//{{{
    bool end_flag = false;
    unsigned short wait_cyc = 0;

    /* wait about border cycle */
    end_flag = Judge_pwm_ip_end(BLOCK_10);
    wait_cyc = Judge_cyc_pwm_ip((unsigned short)(m_pwm_10_cyc), end_flag);
    m_pwm10.notify((double)(wait_cyc * mPCLKPeriod), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : Judge_pwm_ip_end
/// Description  : Check if pwm_ip is going to end
///*********************************
bool HWCORE_PWM::Judge_pwm_ip_end(unsigned char shift)
{//{{{
    unsigned int mfuncflgrpa2 = 0;
    unsigned char value = 0;
    bool end_flag = false;

    switch (shift)
    {
    case BLOCK_00:
    case BLOCK_01:
    case BLOCK_02:
    case BLOCK_03:
    case BLOCK_04:
    case BLOCK_05:
    case BLOCK_06:
    case BLOCK_07:
    case BLOCK_08:
    case BLOCK_09:
        /* Check whether pwm_ip will run next block or end(WAIT or IDLE)) */
        mfuncflgrpa2 = FUNCFLGRPA2.read();
        
        value = (0x03 & ((unsigned int)(mfuncflgrpa2 >> shift)));     /* shift now block */
    
        if (value == 0x00) {                                        /* run next block */   
            end_flag = false;
        } else if ((value == 0x01) || (value == 0x02)) {            /* WAIT or IDLE */
            end_flag = true;
        } else {
            break;
        }
        break;
    
    case BLOCK_10:
        end_flag = true;
        break;
    
    default:
        break;
    }
    return end_flag;
}//}}}


///*********************************
/// Function     : Judge_cyc_pwm_ip
/// Description  : 
///*********************************
unsigned short HWCORE_PWM::Judge_cyc_pwm_ip(unsigned short cyc, bool end_flag)
{//{{{
    unsigned short wait_cyc = 0;

    m_pwm_ip_cyc_sum += cyc;      /* add calculation cycle */
    
    /* end process at this block */
    if (end_flag == true) {
        /* calculation cycle is more than border cycle */
        if (m_pwm_ip_cyc_sum >= m_pwm_ip_border_cyc) {
            wait_cyc = 0;
        }
        /* calculation cycle is less than border cycle */
        else {
            wait_cyc = (unsigned short)(m_pwm_ip_border_cyc - m_pwm_ip_cyc_sum);
        }
    }
    /* continue process */
    else {
        wait_cyc = 0;
    }
    return wait_cyc;
}//}}}

///*********************************
/// Function     : read 
/// Description  : Read API (target socket)
///*********************************
// Description: read register
// Ref: {HWCORE_PWM_UD_Flow_004, HWCORE_PWM_UD_Direction_031, HWCORE_PWM_UD_ATT1_005, HWCORE_PWM_UD_Direction_033}
void HWCORE_PWM::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) { return; }
    mpHWCORE_PWMFunc->read(offsetAddress, data_ptr, size, debug);
}//}}}

///*********************************
/// Function     : write 
/// Description  : Write API (target socket)
///*********************************
// Description: write register
// Ref: {HWCORE_PWM_UD_Flow_005, HWCORE_PWM_UD_Direction_032, HWCORE_PWM_UD_Direction_034, HWCORE_PWM_UD_Direction_035}
void HWCORE_PWM::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) { return; }
    mpHWCORE_PWMFunc->write(offsetAddress, data_ptr, size, debug);
}//}}}

///*********************************
/// Function     : pwm0_io 
/// Description  : preprocess I/O of pwm0 Method
///*********************************
// Description: pwm0_io
// Ref: {HWCORE_PWM_UD_Flow_022}
void HWCORE_PWM::Pwm0_io(void)
{//{{{
    Pwm0();
}//}}}

///*********************************
/// Function     : pwm1_io 
/// Description  : preprocess I/O of pwm1 Method
///*********************************
// Description: pwm1_io
// Ref: {HWCORE_PWM_UD_Flow_010}
void HWCORE_PWM::Pwm1_io(void)
{//{{{
    int vel(0), vdi(0), vqi(0), idi(0), iqi(0);
    int vdo(0), vqo(0);

    /* pwm01 pre process */
    vdi = rdata_em2ivd.read();
    vqi = rdata_em2ivq.read();
    idi = id_sel_out.read();
    iqi = iq_sel_out.read();

    if (mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "FPWMIBTH") == 0x00) { // Get buffer data
        vel = m_buf_rdc_omega;
        m_vdcrct = mpHWCORE_PWMFunc->mVDCRCT_buf_value;
        m_vqcrct = mpHWCORE_PWMFunc->mVQCRCT_buf_value;
        m_decvelg = mpHWCORE_PWMFunc->mDECVELG_buf_value;
        m_decflux = mpHWCORE_PWMFunc->mDECFLUX_buf_value;
        m_decld = mpHWCORE_PWMFunc->mDECLD_buf_value;
        m_declq = mpHWCORE_PWMFunc->mDECLQ_buf_value;
    } else {
        vel = rdc_high_omega_pclk.read();           // 25-bit
        if ((vel & 0x02000000) == 0x02000000) {     // signed bit extension
            vel = (0xFC000000 | vel);
        }
        
        m_vdcrct = mpHWCORE_PWMFunc->GetRegVal("VDCRCT");
        m_vqcrct = mpHWCORE_PWMFunc->GetRegVal("VQCRCT");
        m_decvelg = mpHWCORE_PWMFunc->GetRegVal("DECVELG");
        m_decflux = mpHWCORE_PWMFunc->GetRegVal("DECFLUX");
        m_decld = mpHWCORE_PWMFunc->GetRegVal("DECLD");
        m_declq = mpHWCORE_PWMFunc->GetRegVal("DECLQ");
    }

    m_pwmctr_setdec = (bool)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "SETDEC") & 0x01);
    m_pwmctr_setvel = (bool)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "SETVEL") & 0x01);
    m_pwmctr_decidq = (unsigned char)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "DECIDQ") & 0xFF);

    m_velsft = mpHWCORE_PWMFunc->GetRegVal("VELSFT");
    m_vd2max = mpHWCORE_PWMFunc->GetRegVal("VD2MAX");
    m_vq2max = mpHWCORE_PWMFunc->GetRegVal("VQ2MAX");

    /* run func block pwm1 */
    Pwm1(vel, vdi, vqi, idi, iqi, &vdo, &vqo);

    /* pwm1 post process */
    mpHWCORE_PWMFunc->SetRegVal("VD2", vdo);
    mpHWCORE_PWMFunc->SetRegVal("VQ2", vqo);
    mpHWCORE_PWMFunc->SetRegVal("VEL", m_vel);

    if(m_pwm_ovfl_out) {
        mWrite_pwm_ovfl_outEvent.notify();
        m_pwm_ovfl_func = 0x1;
        mWrite_pwm_ovfl_funcEvent.notify();
    }
    mWrite_pwm_zdfl_outEvent.notify();
}//}}}

///*********************************
/// Function     : pwm2_io 
/// Description  : preprocess I/O of pwm2 Method
///*********************************
// Description: pwm2_io
// Ref: {HWCORE_PWM_UD_Flow_011}
void HWCORE_PWM::Pwm2_io(void)
{//{{{
    unsigned short etht1i(0), etht2i(0);  // 12 bit unsigned
    unsigned short ethto(0);           // 12 bit unsigned

    /* pwm02 pre process */
    etht1i = theta_e_reg.read();
    etht2i = theta_e_fix_reg.read();

    if (mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "FPWMIBTH") == 0x00) { // Get buffer data
        m_pwmctr_flininip = (unsigned char)((mpHWCORE_PWMFunc->mPWMCTR_buf_value >> 4) & 0x3);
        m_thtforesft = (unsigned short)(mpHWCORE_PWMFunc->mTHTFORESFT_buf_value & 0xFFFF);
        m_phi = (unsigned short)(mpHWCORE_PWMFunc->mPHI_buf_value & 0xFFFF);
        m_gtht = mpHWCORE_PWMFunc->mGTHT_buf_value;
    } else {
        m_pwmctr_flininip = (unsigned char)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "FLININIP") & 0xFF);
        m_thtforesft = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("THTFORESFT") & 0xFFFF);
        m_phi = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("PHI") & 0xFFFF);
        m_gtht = mpHWCORE_PWMFunc->GetRegVal("GTHT");
    }

    /* run func block pwm2 */
    Pwm2(etht1i, etht2i, &ethto);
    
    /* pwm2 post process */
    mpHWCORE_PWMFunc->SetRegVal("THTEPWM", ethto & 0xFFF);

    if(m_pwm_ovfl_out) {
        mWrite_pwm_ovfl_outEvent.notify();
        m_pwm_ovfl_func = 0x2;
        mWrite_pwm_ovfl_funcEvent.notify();
    }
    mWrite_pwm_zdfl_outEvent.notify();
}//}}}

///*********************************
/// Function     : pwm3_io 
/// Description  : preprocess I/O of pwm3 Method
///*********************************
// Description: pwm3_io
// Ref: {HWCORE_PWM_UD_Flow_012}
void HWCORE_PWM::Pwm3_io(void)
{//{{{
    unsigned short ethti(0), vthti(0);
    int vdi(0), vqi(0), vdqi(0);
    int vuo(0), vvo(0), vwo(0);

    /* pwm03 pre process */
    ethti = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("THTEPWM") & 0xFFFF);

    if (mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "FPWMIBTH") == 0x00) { // Get buffer data
        vthti = (unsigned short)(mpHWCORE_PWMFunc->mTHTVSFT_buf_value & 0xFFFF);
        vdqi = mpHWCORE_PWMFunc->mVDQSFT_buf_value;
    } else {
        vthti = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("THTVSFT") & 0xFFFF);
        vdqi = mpHWCORE_PWMFunc->GetRegVal("VDQSFT");
    }

    vdi = mpHWCORE_PWMFunc->GetRegVal("VD2");
    vqi = mpHWCORE_PWMFunc->GetRegVal("VQ2");

    m_pwmctr_vdqsel = (bool)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "VDQSEL") & 0x01);
    m_sr23 = mpHWCORE_PWMFunc->GetRegVal("SR23");

    /* run func block pwm3 */
    Pwm3(ethti, vthti, vdi, vqi, vdqi, &vuo, &vvo, &vwo);

    /* pwm3 post process */
    mpHWCORE_PWMFunc->SetRegVal("VU", vuo);
    mpHWCORE_PWMFunc->SetRegVal("VV", vvo);
    mpHWCORE_PWMFunc->SetRegVal("VW", vwo);

    if(m_pwm_ovfl_out) {
        mWrite_pwm_ovfl_outEvent.notify();
        m_pwm_ovfl_func = 0x3;
        mWrite_pwm_ovfl_funcEvent.notify();
    }
    mWrite_pwm_zdfl_outEvent.notify();
}//}}}

///*********************************
/// Function     : pwm4_io 
/// Description  : preprocess I/O of pwm4 Method
///*********************************
// Description: pwm4_io
// Ref: {HWCORE_PWM_UD_Flow_013}
void HWCORE_PWM::Pwm4_io(void)
{//{{{
    int tmax(0);
    unsigned short ethti(0), vthti(0);
    int vdqi(0), vui(0), vvi(0), vwi(0);
    int vuo(0), vvo(0), vwo(0);

    /* pwm04 pre process */
    if (mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "FPWMIBTH") == 0x00) { // Get buffer data
        tmax = mpHWCORE_PWMFunc->mTMAX_buf_value;
        vthti = (unsigned short)(mpHWCORE_PWMFunc->mTHTVSFT_buf_value & 0xFFFF);
        vdqi = mpHWCORE_PWMFunc->mVDQSFT_buf_value;
    } else {
        tmax = mpHWCORE_PWMFunc->GetRegVal("TMAX");
        vthti = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("THTVSFT") & 0xFFFF);
        vdqi = mpHWCORE_PWMFunc->GetRegVal("VDQSFT");
    }

    ethti = (unsigned short) (mpHWCORE_PWMFunc->GetRegVal("THTEPWM") & 0xFFFF);
    vui = mpHWCORE_PWMFunc->GetRegVal("VU");
    vvi = mpHWCORE_PWMFunc->GetRegVal("VV");
    vwi = mpHWCORE_PWMFunc->GetRegVal("VW");

    m_pwmctr_setharm = (unsigned char)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "SETHARM") & 0xFF);
    m_sr23 = mpHWCORE_PWMFunc->GetRegVal("SR23");

    /* run func block pwm4 */
    Pwm4(tmax, ethti, vthti, vdqi, vui, vvi, vwi, &vuo, &vvo, &vwo);

    /* pwm4 post process */
    mpHWCORE_PWMFunc->SetRegVal("VU0", vuo);
    mpHWCORE_PWMFunc->SetRegVal("VV0", vvo);
    mpHWCORE_PWMFunc->SetRegVal("VW0", vwo);

    if(m_pwm_ovfl_out) {
        mWrite_pwm_ovfl_outEvent.notify();
        m_pwm_ovfl_func = 0x4;
        mWrite_pwm_ovfl_funcEvent.notify();
    }
    mWrite_pwm_zdfl_outEvent.notify();
}//}}}

///*********************************
/// Function     : pwm5_io 
/// Description  : preprocess I/O of pwm5 Method
///*********************************
// Description: pwm5_io
// Ref: {HWCORE_PWM_UD_Flow_014}
void HWCORE_PWM::Pwm5_io(void)
{//{{{
    int vui(0), vvi(0), vwi(0);
    int vuo(0), vvo(0), vwo(0);

    /* pwm05 pre process */
    vui = mpHWCORE_PWMFunc->GetRegVal("VU0");
    vvi = mpHWCORE_PWMFunc->GetRegVal("VV0");
    vwi = mpHWCORE_PWMFunc->GetRegVal("VW0");
    m_pwmk1 = mpHWCORE_PWMFunc->GetRegVal("PWMK1");

    if (mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "FPWMIBTH") == 0x00) { // Get buffer data
        m_volv = (unsigned short)(mpHWCORE_PWMFunc->mVOLV_buf_value & 0xFFFF);
    } else {
        m_volv = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("VOLV") & 0xFFFF);
    }

    /* run func block pwm5 */
    Pwm5(vui, vvi, vwi, &vuo, &vvo, &vwo);

    /* pwm5 post process */
    mpHWCORE_PWMFunc->SetRegVal("VU1", vuo);
    mpHWCORE_PWMFunc->SetRegVal("VV1", vvo);
    mpHWCORE_PWMFunc->SetRegVal("VW1", vwo);

    if(m_pwm_ovfl_out || m_pwm_zdiv_out) {
        mWrite_pwm_ovfl_outEvent.notify();
        mWrite_pwm_zdfl_outEvent.notify();
        m_pwm_ovfl_func = 0x5;
        mWrite_pwm_ovfl_funcEvent.notify();
    }
}//}}}

///*********************************
/// Function     : pwm6_io 
/// Description  : preprocess I/O of pwm6 Method
///*********************************
// Description: pwm6_io
// Ref: {HWCORE_PWM_UD_Flow_015}
void HWCORE_PWM::Pwm6_io(void)
{//{{{
    int vui(0), vvi(0), vwi(0);
    int vuo(0), vvo(0), vwo(0);

    /* pwm06 pre process */
    vui = mpHWCORE_PWMFunc->GetRegVal("VU1");
    vvi = mpHWCORE_PWMFunc->GetRegVal("VV1");
    vwi = mpHWCORE_PWMFunc->GetRegVal("VW1");

    if (mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "FPWMIBTH") == 0x00) { // Get buffer data
        m_vuofs = (unsigned short)(mpHWCORE_PWMFunc->mVUOFS_buf_value & 0xFFFF);
        m_vvofs = (unsigned short)(mpHWCORE_PWMFunc->mVVOFS_buf_value & 0xFFFF);
        m_vwofs = (unsigned short)(mpHWCORE_PWMFunc->mVWOFS_buf_value & 0xFFFF);
    } else {
        m_vuofs = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("VUOFS") & 0xFFFF);
        m_vvofs = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("VVOFS") & 0xFFFF);
        m_vwofs = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("VWOFS") & 0xFFFF);
    }

    /* run func block pwm6 */
    Pwm6(vui, vvi, vwi, &vuo, &vvo, &vwo);

    /* pwm6 post process */
    mpHWCORE_PWMFunc->SetRegVal("VU2", vuo);
    mpHWCORE_PWMFunc->SetRegVal("VV2", vvo);
    mpHWCORE_PWMFunc->SetRegVal("VW2", vwo);

    if(m_pwm_ovfl_out) {
        mWrite_pwm_ovfl_outEvent.notify();
        m_pwm_ovfl_func = 0x6;
        mWrite_pwm_ovfl_funcEvent.notify();
    }
    mWrite_pwm_zdfl_outEvent.notify();
}//}}}

///*********************************
/// Function     : pwm7_io 
/// Description  : preprocess I/O of pwm7 Method
///*********************************
// Description: pwm7_io
// Ref: {HWCORE_PWM_UD_Flow_016}
void HWCORE_PWM::Pwm7_io(void)
{//{{{
    int vu1i(0), vv1i(0), vw1i(0);
    int vuo(0), vvo(0), vwo(0);
    
    /* pwm07 pre process */
    vu1i = mpHWCORE_PWMFunc->GetRegVal("VU2");
    vv1i = mpHWCORE_PWMFunc->GetRegVal("VV2");
    vw1i = mpHWCORE_PWMFunc->GetRegVal("VW2");

    m_dtul = mpHWCORE_PWMFunc->GetRegVal("DTUL");
    m_dtll = mpHWCORE_PWMFunc->GetRegVal("DTLL");

    /* run func block pwm7 */
    Pwm7(vu1i, vv1i, vw1i, &vuo, &vvo, &vwo);

    /* pwm7 post process */
    mpHWCORE_PWMFunc->SetRegVal("VUFIX", vuo);
    mpHWCORE_PWMFunc->SetRegVal("VVFIX", vvo);
    mpHWCORE_PWMFunc->SetRegVal("VWFIX", vwo);
}//}}}

///*********************************
/// Function     : pwm8_io 
/// Description  : preprocess I/O of pwm8 Method
///*********************************
// Description: pwm8_io
// Ref: {HWCORE_PWM_UD_Flow_017}
void HWCORE_PWM::Pwm8_io(void)
{//{{{
    int vui(0), vvi(0), vwi(0);
    int pwmuo(0), pwmvo(0), pwmwo(0);

    /* pwm08 pre process */
    vui = mpHWCORE_PWMFunc->GetRegVal("VUFIX");
    vvi = mpHWCORE_PWMFunc->GetRegVal("VVFIX");
    vwi = mpHWCORE_PWMFunc->GetRegVal("VWFIX");

    m_pwmctr_pwmsel = (bool)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "PWMSEL") & 0x01);
    m_pwmctr_pwmop = (unsigned char)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "PWMOP") & 0xFF);
    m_pwmk2 = mpHWCORE_PWMFunc->GetRegVal("PWMK2");
    m_dtt = mpHWCORE_PWMFunc->GetRegVal("DTT");

    if (mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "FPWMIBTH") == 0x00) { // Get buffer data
        m_carr = mpHWCORE_PWMFunc->mCARR_buf_value;
    } else {
        m_carr = mpHWCORE_PWMFunc->GetRegVal("CARR");
    }

    /* run func block pwm8 */
    Pwm8(vui, vvi, vwi, &pwmuo, &pwmvo, &pwmwo);

    /* pwm8 post process */
    mpHWCORE_PWMFunc->SetRegVal("PWMU0", pwmuo);
    mpHWCORE_PWMFunc->SetRegVal("PWMV0", pwmvo);
    mpHWCORE_PWMFunc->SetRegVal("PWMW0", pwmwo);

    if(m_pwm_ovfl_out) {
        mWrite_pwm_ovfl_outEvent.notify();
        m_pwm_ovfl_func = 0x8;
        mWrite_pwm_ovfl_funcEvent.notify();
    }
    mWrite_pwm_zdfl_outEvent.notify();
}//}}}

///*********************************
/// Function     : pwm9_io 
/// Description  : preprocess I/O of pwm9 Method
///*********************************
// Description: pwm9_io
// Ref: {HWCORE_PWM_UD_Flow_018}
void HWCORE_PWM::Pwm9_io(void)
{//{{{
    int pwmui(0), pwmvi(0), pwmwi(0);
    int iui(0), ivi(0), iwi(0);
    int pwmuo(0), pwmvo(0), pwmwo(0);

    /* pwm09 pre process */
    pwmui = mpHWCORE_PWMFunc->GetRegVal("PWMU0");
    pwmvi = mpHWCORE_PWMFunc->GetRegVal("PWMV0");
    pwmwi = mpHWCORE_PWMFunc->GetRegVal("PWMW0");

    iui = ia_u_lo_fix_reg.read();
    ivi = ia_v_lo_fix_reg.read();
    iwi = ia_w_lo_fix_reg.read();

    m_pwmctr_setdto = (bool)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "SETDTO") & 0x01);
    m_dtoth = mpHWCORE_PWMFunc->GetRegVal("DTOTH");
    m_dtopv = mpHWCORE_PWMFunc->GetRegVal("DTOPV");
    m_dtonv = mpHWCORE_PWMFunc->GetRegVal("DTONV");

    /* run func block pwm9 */
    Pwm9(pwmui, pwmvi, pwmwi, iui, ivi, iwi, &pwmuo, &pwmvo, &pwmwo);

    /* pwm9 post process */
    mpHWCORE_PWMFunc->SetRegVal("PWMUDT", pwmuo);
    mpHWCORE_PWMFunc->SetRegVal("PWMVDT", pwmvo);
    mpHWCORE_PWMFunc->SetRegVal("PWMWDT", pwmwo);

    if(m_pwm_ovfl_out) {
        mWrite_pwm_ovfl_outEvent.notify();
        m_pwm_ovfl_func = 0x9;
        mWrite_pwm_ovfl_funcEvent.notify();
    }
    mWrite_pwm_zdfl_outEvent.notify();
}//}}}

///*********************************
/// Function     : pwm10_io 
/// Description  : preprocess I/O of pwm10 Method
///*********************************
// Description: pwm10_io
// Ref: {HWCORE_PWM_UD_Flow_019}
void HWCORE_PWM::Pwm10_io(void)
{//{{{
    int pwmui(0), pwmvi(0), pwmwi(0);
    unsigned int pwmuo(0), pwmvo(0), pwmwo(0), carro(0);

    /* pwm10 pre process */
    pwmui = mpHWCORE_PWMFunc->GetRegVal("PWMUDT");
    pwmvi = mpHWCORE_PWMFunc->GetRegVal("PWMVDT");
    pwmwi = mpHWCORE_PWMFunc->GetRegVal("PWMWDT");

    m_pwmul = mpHWCORE_PWMFunc->GetRegVal("PWMUL");
    m_pwmll = mpHWCORE_PWMFunc->GetRegVal("PWMLL");
    m_dtt = mpHWCORE_PWMFunc->GetRegVal("DTT");

    if (mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "FPWMIBTH") == 0x00) { // Get buffer data 
        m_carr = mpHWCORE_PWMFunc->mCARR_buf_value;
        m_volv = (unsigned short)(mpHWCORE_PWMFunc->mVOLV_buf_value & 0xFFFF);
    } else {
        m_carr = mpHWCORE_PWMFunc->GetRegVal("CARR");
        m_volv = (unsigned short)(mpHWCORE_PWMFunc->GetRegVal("VOLV") & 0xFFFF);
    }

    /* run func block pwm10 */
    Pwm10(pwmui, pwmvi, pwmwi, &pwmuo, &pwmvo, &pwmwo, &carro);

    /* pwm10 post process */
    mpHWCORE_PWMFunc->SetRegVal("PWMUIP", pwmuo);
    mpHWCORE_PWMFunc->SetRegVal("PWMVIP", pwmvo);
    mpHWCORE_PWMFunc->SetRegVal("PWMWIP", pwmwo);
    mpHWCORE_PWMFunc->SetRegVal("CARRBUF", carro);

    if(m_pwm_ovfl_out) {
        mWrite_pwm_ovfl_outEvent.notify();
        m_pwm_ovfl_func = 0xA;
        mWrite_pwm_ovfl_funcEvent.notify();
    }
    mWrite_pwm_zdfl_outEvent.notify();
}//}}}


///*********************************
/// Function     : Func_pwm_ip_outMethod
/// Description  : Set state pwm_out Method
///*********************************
// Description: Func_pwm_ip_outMethod
// Ref: {HWCORE_PWM_UD_Flow_020}
void HWCORE_PWM::Func_pwm_ip_outMethod(void)
{//{{{
    unsigned int pwmui(0), pwmvi(0), pwmwi(0), carri(0);
    unsigned int pwmuo(0), pwmvo(0), pwmwo(0), carro(0);

    /* pwm_out pre process */
    pwmui = mpHWCORE_PWMFunc->GetRegVal("PWMUIP");
    pwmvi = mpHWCORE_PWMFunc->GetRegVal("PWMVIP");
    pwmwi = mpHWCORE_PWMFunc->GetRegVal("PWMWIP");
    carri = mpHWCORE_PWMFunc->GetRegVal("CARRBUF");

    m_pwmctr_setpwm = (bool)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "SETPWM") & 0x01);
    m_pwmctr_shipwm = (bool)(mpHWCORE_PWMFunc->GetRegVal("PWMCTR", "SHIPWM") & 0x01);
    m_pwmdt_pwmdt = m_pwmdt_write_flag;
    m_pwmu = mpHWCORE_PWMFunc->GetRegVal("PWMU");
    m_pwmv = mpHWCORE_PWMFunc->GetRegVal("PWMV");
    m_pwmw = mpHWCORE_PWMFunc->GetRegVal("PWMW");
    m_carr = mpHWCORE_PWMFunc->GetRegVal("CARR");

    pwmuo = m_tstwdcmpu_temp;
    pwmvo = m_tstwdcmpv_temp;
    pwmwo = m_tstwdcmpw_temp;
    carro = m_tstwdcmp0_temp;

    /* run pwm_out */
    Pwm_out(pwmui, pwmvi, pwmwi, carri, &pwmuo, &pwmvo, &pwmwo, &carro);

    m_tstwdcmpu_temp = pwmuo;
    m_tstwdcmpv_temp = pwmvo;
    m_tstwdcmpw_temp = pwmwo;
    m_tstwdcmp0_temp = carro;

    if (m_change_tstwecmp_flag == true) { // send write enable pulse
        m_tstwecmp_temp = true;           // set write enable pulse value
    } else {
        m_tstwecmp_temp = false;
    }

    m_change_tstwecmp_flag = false;        // flag clear
    m_pwmdt_write_flag = false;

    /* Output to Verification buffer */
    m_output_to_verification_buff.notify();

    // synchronize with CCLK to output to tsg3
    if (CheckClockPeriod("CCLK") && (!GetResetStatus("crst_n"))) {
        double current_time = sc_time_stamp().to_double();
        m_tsg_output_from_pwm_out.notify(CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mStartCCLKTime), mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : Ver_output_from_pwm_out
/// Description  : Output to Verification buffer
///*********************************
void HWCORE_PWM::Ver_output_from_pwm_outMethod(void)
{//{{{
    tstwdcmpu_pclk.write(m_tstwdcmpu_temp);
    tstwdcmpv_pclk.write(m_tstwdcmpv_temp);
    tstwdcmpw_pclk.write(m_tstwdcmpw_temp);
}//}}}

///*********************************
/// Function     : Tsg_output_from_pwm_out
/// Description  : Output to TSG3
///*********************************
void HWCORE_PWM::Tsg_output_from_pwm_outMethod(void)
{//{{{
    tstwdcmpu.write(m_tstwdcmpu_temp);
    tstwdcmpv.write(m_tstwdcmpv_temp);
    tstwdcmpw.write(m_tstwdcmpw_temp);
    tstwdcmp0.write(m_tstwdcmp0_temp);
    tstwecmp.write(m_tstwecmp_temp);

    if (m_tstwecmp_temp) {
        m_tstwecmp_temp = false;
        m_tsg_output_from_pwm_out.notify((double)(2 * mCCLKPeriod), mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : End_pwm_ip
/// Description  : complete pwm ip 
///*********************************
// Description: End_pwm_ip
// Ref: {HWCORE_PWM_UD_Direction_057}
void HWCORE_PWM::End_pwm_ip(void)
{//{{{
    /* flag for pwm_out */
    m_pwm_end_flag = true;

    /* clear cycle sum */
    m_pwm_ip_cyc_sum = 0;

    m_pwm_out.notify();

    double current_time = sc_time_stamp().to_double();
    re_printf("info", "HWCORE_PWM is end.\n");
    if (mEnableStatInfo == true) {
        re_printf("info", "Information of buffers (end): \n");
        DumpStatInfo();
    }
    m_pwm_end_sp_reg = true; // to write to output_pwm_end_sp_reg
    mAssert_pwm_end_sp_regEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : pwm0 
/// Description  : pwm0 Method
///*********************************
void HWCORE_PWM::Pwm0(void)
{//{{{
    re_printf("info", "Start HWCORE_PWM from pwm0");
}//}}}

///*********************************
/// Function     : pwm1 
/// Description  : pwm1 Method
///*********************************
// Description: pwm1
// Ref: {HWCORE_PWM_UD_Flow_010}
void HWCORE_PWM::Pwm1(int vel, int vdi, int vqi, int idi, int iqi, int *vdo, int *vqo)
{//{{{
    if (vdo == NULL || vqo == NULL) { return; }
    int tmpd(0), tmpq(0);

    m_vel = ((long long int)vel * 0x001DF5E7) >> 16;

    if (m_pwmctr_setdec == 0) {
        tmpd = vdi + m_vdcrct;
        Chk_overflow_s((OVFC_S)vdi + (OVFC_S)m_vdcrct, 0xFFFFFFFF, &m_pwm_ovfl_out);
        tmpq = vqi + m_vqcrct;
        Chk_overflow_s((OVFC_S)vqi + (OVFC_S)m_vqcrct, 0xFFFFFFFF, &m_pwm_ovfl_out);
    } else {
        int tmp_vel(0);
        int tmp_id(0), tmp_iq(0);
        int tmp0(0), tmp1(0), tmp2(0), tmp3(0);

        if (m_pwmctr_setvel == 0) {
            tmp_vel = m_velsft;
        } else {
            tmp_vel = m_vel;
        }

        switch (m_pwmctr_decidq)
        {
        case 0:
            tmp_id = id_reg.read();
            tmp_iq = iq_reg.read();
            break;
        case 1:
            tmp_id = idi;
            tmp_iq = iqi;
            break;
        case 2:
            tmp_id = id_dir_reg.read();
            tmp_iq = iq_dir_reg.read();
            break;
        default:
            break;
        }

        tmp0 = ((long long int)tmp_vel * (long long int)m_decvelg) >> 16;
        Chk_overflow_s(((OVFC_S)tmp_vel * (OVFC_S)m_decvelg) >> 16, 0xFFFFFFFF, &m_pwm_ovfl_out);
        
        tmp1 = ((long long int)tmp_id * (long long int)m_decld) >> 16;
        Chk_overflow_s(((OVFC_S)tmp_id * (OVFC_S)m_decld) >> 16, 0xFFFFFFFF, &m_pwm_ovfl_out);
        
        tmp2 = ((long long int)tmp_iq * (long long int)m_declq) >> 16;
        Chk_overflow_s(((OVFC_S)tmp_iq * (OVFC_S)m_declq) >> 16, 0xFFFFFFFF, &m_pwm_ovfl_out);

        Chk_overflow_s(((OVFC_S)tmp0 * (OVFC_S)tmp1) >> 12, 0xFFFFFFFF, &m_pwm_ovfl_out);
        tmp1 = ((long long int)tmp0 * (long long int)tmp1) >> 12;
        
        Chk_overflow_s(((OVFC_S)tmp0 * (OVFC_S)tmp2) >> 12, 0xFFFFFFFF, &m_pwm_ovfl_out);
        tmp2 = ((long long int)tmp0 * (long long int)tmp2) >> 12;
        
        tmp3 = ((long long int)tmp0 * (long long int)m_decflux) >> 28;
        Chk_overflow_s(((OVFC_S)tmp0 * (OVFC_S)m_decflux) >> 28, 0xFFFFFFFF, &m_pwm_ovfl_out);

        tmpd = vdi - tmp2;
        Chk_overflow_s((OVFC_S)vdi - (OVFC_S)tmp2, 0xFFFFFFFF, &m_pwm_ovfl_out);
        tmpq = vqi + tmp1 + tmp3;  /*Ovf Check*/
        Chk_overflow_s((OVFC_S)vqi + (OVFC_S)tmp1 + (OVFC_S)tmp3, 0xFFFFFFFF, &m_pwm_ovfl_out);
    }

    if ((long long int)tmpd > (long long int)m_vd2max) {
        *vdo = m_vd2max;
    } else if ((long long int)tmpd < -(long long int)(m_vd2max)) {
        *vdo = -m_vd2max;
    } else {
        *vdo = tmpd;
    }

    if ((long long int)tmpq > (long long int)m_vq2max) {
        *vqo = m_vq2max;
    } else if ((long long int)tmpq < -(long long int)(m_vq2max)) {
        *vqo = -m_vq2max;
    } else {
        *vqo = tmpq;
    }
}//}}}

///*********************************
/// Function     : pwm2 
/// Description  : pwm2 Method
///*********************************
// Description: pwm2
// Ref: {HWCORE_PWM_UD_Flow_011}
void HWCORE_PWM::Pwm2(unsigned short etht1i, unsigned short etht2i, unsigned short *ethto)
{//{{{
    if (ethto == NULL) { return; }
    unsigned short ethtin(0);
    unsigned int carrbuf(0);

    switch(m_pwmctr_flininip)
    {
    case 0:
        ethtin = m_thtforesft & 0xFFF;
        break;
    case 1:
        ethtin = etht1i & 0xFFF;
        break;
    case 2:
        ethtin = etht2i & 0xFFF;
        break;
    default:
        break;
    }

    carrbuf = mpHWCORE_PWMFunc->GetRegVal("CARRBUF");
    Chk_overflow_u((((OVFC_U)carrbuf * (OVFC_U)m_gtht) >> 32), 0xFFF, &m_pwm_ovfl_out);
    
    *ethto = ((long long int)ethtin + (((long long int)carrbuf * (long long int)m_gtht) >> 32) + (long long int)m_phi) & 0xFFF;
}//}}}

///*********************************
/// Function     : pwm3 
/// Description  : pwm3 Method
///*********************************
// Description: pwm3
// Ref: {HWCORE_PWM_UD_Flow_012}
void HWCORE_PWM::Pwm3(unsigned short ethti, unsigned short vthti, int vdi, int vqi, int vdqi, int *vuo, int *vvo, int *vwo)
{//{{{
    if (vuo == NULL || vvo == NULL || vwo == NULL) { return; }

    int tmp(0), tmpd(0);
    int A1(0), A2(0), B1(0), B2(0);

    // sin() and cos() represent signed fixed-point values with fractional parts 15 bits long.
    // static int cos0 = 0x00008000; // Value obtained by multiplying "cos(0)" by 0x8000
    // static int sin0 = 0x00000000; // Value obtained by multiplying "sin(0)" by 0x8000
    static int cos120 = 0xFFFFC000; // Value obtained by multiplying "cos(120)" by 0x8000
    static int sin120 = 0x00006EDA; // Value obtained by multiplying "sin(120)" by 0x8000

    if (m_pwmctr_vdqsel == false) {
        tmpd = vdi;
        A1 = q_sin[(ethti + 0x400) & 0xFFF]; // cos(theta)
        A2 = q_sin[ethti & 0xFFF];           // sin(theta)   
        // cos(theta+120) = ( cos(theta) * cos(120) - sin(theta) * sin(120) ) >> 15
        B1 = ((long long int)A1 * (long long int)cos120 - (long long int)A2 * (long long int)sin120) >> 15;
        Chk_overflow_s(((OVFC_S)A1 * (OVFC_S)cos120 - (OVFC_S)A2 * (OVFC_S)sin120) >> 15, 0xFFFFFFFF, &m_pwm_ovfl_out);
        // sin(theta+120) = ( sin(theta) * cos(120) + cos(theta) * sin(120) ) >> 15
        B2 = ((long long int)A2 * (long long int)cos120 + (long long int)A1 * (long long int)sin120) >> 15;
        Chk_overflow_s(((OVFC_S)A2 * (OVFC_S)cos120 + (OVFC_S)A1 * (OVFC_S)sin120) >> 15, 0xFFFFFFFF, &m_pwm_ovfl_out);
    } else { // Vdq/VTheta
        tmpd = vdqi;
        A1 = q_sin[(ethti + vthti + 0x400) & 0xFFF];   // cos(theta + VTheta)
        A2 = 0;
        // cos((theta+Vtheta)+120) = ( cos(theta+Vtheta) * cos(120) - sin(theta+Vtheta) * sin(120) ) >> 15
        B1 = ((long long int)A1 * (long long int)cos120 - (long long int)q_sin[(ethti + vthti) & 0xFFF] * (long long int)sin120) >> 15;
        Chk_overflow_s(((OVFC_S)A1 * (OVFC_S)cos120 - (OVFC_S)q_sin[(ethti + vthti) & 0xFFF] * (OVFC_S)sin120) >> 15, 0xFFFFFFFF, &m_pwm_ovfl_out);
        B2 = 0;
    }

    *vuo = ((long long int)m_sr23 * ((((long long int)A1 * (long long int)tmpd + (long long int)tmp) >> 15) - (((long long int)A2 * (long long int)vqi + (long long int)tmp) >> 15))) >> 16;
    Chk_overflow_s(((OVFC_S)m_sr23 * ((((OVFC_S)A1 * (OVFC_S)tmpd + (OVFC_S)tmp) >> 15) - (((OVFC_S)A2 * (OVFC_S)vqi + (OVFC_S)tmp) >> 15))) >> 16, 0xFFFFFFFF, &m_pwm_ovfl_out);
    *vwo = ((long long int)m_sr23 * ((((long long int)B1 * (long long int)tmpd + (long long int)tmp) >> 15) - (((long long int)B2 * (long long int)vqi + (long long int)tmp) >> 15))) >> 16;
    Chk_overflow_s(((OVFC_S)m_sr23 * ((((OVFC_S)B1 * (OVFC_S)tmpd + (OVFC_S)tmp) >> 15) - (((OVFC_S)B2 * (OVFC_S)vqi + (OVFC_S)tmp) >> 15))) >> 16, 0xFFFFFFFF, &m_pwm_ovfl_out);
    *vvo = - (*vuo + *vwo);
    Chk_overflow_s((OVFC_S)*vuo + (OVFC_S)*vwo, 0xFFFFFFFF, &m_pwm_ovfl_out);
}//}}}

///*********************************
/// Function     : pwm4 
/// Description  : pwm4 Method
///*********************************
// Description: pwm4
// Ref: {HWCORE_PWM_UD_Flow_013}
void HWCORE_PWM::Pwm4(int tmax, unsigned short ethti, unsigned short vthti, int vdqi, int vui,  int vvi, int vwi, int *vuo, int *vvo, int *vwo)
{//{{{
    if (vuo == NULL || vvo == NULL || vwo == NULL) { return; }

    int third = 0;
    int max = vui, min = vui;
    bool vu_gt_vv = false;
    bool vu_gt_vw = false;
    bool vv_gt_vw = false;

    /* get max, min value */
    if (vvi > max) {
        max = vvi;
    } else if (vvi < min) {
        min = vvi;
    }

    if (vwi > max) {
        max = vwi;
    } else if (vwi < min) {
        min = vwi;
    }

    if (vui > vvi) {vu_gt_vv = true;}
    if (vui > vwi) {vu_gt_vw = true;}
    if (vvi > vwi) {vv_gt_vw = true;}

    switch(m_pwmctr_setharm)
    {
    case 1: // Midpoint method
        third =(max + min) >> 1;
        Chk_overflow_s(((OVFC_S)max + (OVFC_S)min) >> 1, 0xFFFFFFFF, &m_pwm_ovfl_out);
        break;
    case 2: // 2-phase modulation
        third = min;
        break;
    case 3: // Discontinuous PWM modulation method 0
        if ((vu_gt_vv ^ vu_gt_vw ^ vv_gt_vw) == 0) {
            third = max - tmax;
            Chk_overflow_s((OVFC_S)max - (OVFC_S)tmax, 0xFFFFFFFF, &m_pwm_ovfl_out);
        } else {
            third = min + tmax;
            Chk_overflow_s((OVFC_S)min + (OVFC_S)tmax, 0xFFFFFFFF, &m_pwm_ovfl_out);
        }
        break;
    case 4: // Discontinuous PWM modulation mode 1
        if ((((long long int)max + (long long int)min) >> 1) >= 0) {
            third = max - tmax;
            Chk_overflow_s((OVFC_S)max - (OVFC_S)tmax, 0xFFFFFFFF, &m_pwm_ovfl_out);
        } else {
            third = min + tmax;
            Chk_overflow_s((OVFC_S)min + (OVFC_S)tmax, 0xFFFFFFFF, &m_pwm_ovfl_out);
        }
        break;
    case 5: // Discontinuous PWM modulation mode 2
        if ((vu_gt_vv ^ vu_gt_vw ^ vv_gt_vw) == 0) {
            third = min + tmax;
            Chk_overflow_s((OVFC_S)min + (OVFC_S)tmax, 0xFFFFFFFF, &m_pwm_ovfl_out);
        } else {
            third = max - tmax;
            Chk_overflow_s((OVFC_S)max - (OVFC_S)tmax, 0xFFFFFFFF, &m_pwm_ovfl_out);
        }
        break;
    case 6: // Discontinuous PWM modulation mode 3
        if ((((long long int)max + (long long int)min) >> 1) >= 0) {
            third = min + tmax;
            Chk_overflow_s((OVFC_S)min + (OVFC_S)tmax, 0xFFFFFFFF, &m_pwm_ovfl_out);
        } else {
            third = max - tmax;
            Chk_overflow_s((OVFC_S)max - (OVFC_S)tmax, 0xFFFFFFFF, &m_pwm_ovfl_out);
        }
        break;
    case 7: // Third harmonic wave superposition
        int tmp;
        tmp = ((long long int)m_sr23 * (long long int)vdqi) >> 16;
        Chk_overflow_s(((OVFC_S)m_sr23 * (OVFC_S)vdqi) >> 16, 0xFFFFFFFF, &m_pwm_ovfl_out);
        
        Chk_overflow_s(((OVFC_S)tmp * (OVFC_S)q_sin[(((ethti + vthti) * 3) + 0x400) & 0xFFF]) >> 15, 0xFFFFFFFF, &m_pwm_ovfl_out);
        tmp = ((long long int)tmp * (long long int)q_sin[(((ethti + vthti) * 3) + 0x400) & 0xFFF]) >> 15;
        third = tmp / 6;
        break;
    default: // Through mode(Sine wave modulation)
        third = 0;
        break;
    }

    *vuo = vui - third;
    Chk_overflow_s((OVFC_S)vui - (OVFC_S)third, 0xFFFFFFFF, &m_pwm_ovfl_out);
    *vvo = vvi - third;
    Chk_overflow_s((OVFC_S)vvi - (OVFC_S)third, 0xFFFFFFFF, &m_pwm_ovfl_out);
    *vwo = vwi - third;
    Chk_overflow_s((OVFC_S)vwi - (OVFC_S)third, 0xFFFFFFFF, &m_pwm_ovfl_out);
}//}}}

///*********************************
/// Function     : pwm5 
/// Description  : pwm5 Method
///*********************************
// Description: pwm5
// Ref: {HWCORE_PWM_UD_Flow_014}
void HWCORE_PWM::Pwm5(int vui, int vvi, int vwi, int *vuo, int *vvo, int *vwo)
{//{{{
    if (vuo == NULL || vvo == NULL || vwo == NULL) { return; }

    Calc_division((int)(((long long int)vui * (long long int)m_pwmk1) >> 16), m_volv, vuo, &m_pwm_zdiv_out);
    Calc_division((int)(((long long int)vvi * (long long int)m_pwmk1) >> 16), m_volv, vvo, &m_pwm_zdiv_out);
    Calc_division((int)(((long long int)vwi * (long long int)m_pwmk1) >> 16), m_volv, vwo, &m_pwm_zdiv_out);
    
    if (m_volv != 0) {
        Chk_overflow_s((((OVFC_S)vui * (OVFC_S)m_pwmk1) >> 16) / (OVFC_S)m_volv, 0xFFFFFFFF, &m_pwm_ovfl_out);
        Chk_overflow_s((((OVFC_S)vvi * (OVFC_S)m_pwmk1) >> 16) / (OVFC_S)m_volv, 0xFFFFFFFF, &m_pwm_ovfl_out);
        Chk_overflow_s((((OVFC_S)vwi * (OVFC_S)m_pwmk1) >> 16) / (OVFC_S)m_volv, 0xFFFFFFFF, &m_pwm_ovfl_out);
    }
}//}}}

///*********************************
/// Function     : pwm6 
/// Description  : pwm6 Method
///*********************************
// Description: pwm6
// Ref: {HWCORE_PWM_UD_Flow_015}
void HWCORE_PWM::Pwm6(int vui, int vvi, int vwi, int *vuo, int *vvo, int *vwo)
{//{{{
    if (vuo == NULL || vvo == NULL || vwo == NULL) { return; }

    *vuo = vui + m_vuofs;
    Chk_overflow_s((OVFC_S)vui + (OVFC_S)m_vuofs, 0xFFFFFFFF, &m_pwm_ovfl_out);
    *vvo = vvi + m_vvofs;
    Chk_overflow_s((OVFC_S)vvi + (OVFC_S)m_vvofs, 0xFFFFFFFF, &m_pwm_ovfl_out);
    *vwo = vwi + m_vwofs;
    Chk_overflow_s((OVFC_S)vwi + (OVFC_S)m_vwofs, 0xFFFFFFFF, &m_pwm_ovfl_out);
}//}}}

///*********************************
/// Function     : pwm7
/// Description  : pwm7 Method
///*********************************
// Description: pwm7
// Ref: {HWCORE_PWM_UD_Flow_016}
void HWCORE_PWM::Pwm7(int vu1i, int vv1i, int vw1i, int *vuo, int *vvo, int *vwo)
{//{{{
    if (vuo == NULL || vvo == NULL || vwo == NULL) { return; }

    int tmpU(0), tmpV(0), tmpW(0);

    tmpU = vu1i;
    tmpV = vv1i;
    tmpW = vw1i;

    if (tmpU > m_dtul) {
        *vuo = m_dtul;
    } else if (tmpU < m_dtll) {
        *vuo = m_dtll;
    } else {
        *vuo = tmpU;
    }

    if (tmpV > m_dtul) {
        *vvo = m_dtul;
    } else if (tmpV < m_dtll) {
        *vvo = m_dtll;
    } else {
        *vvo = tmpV;
    }

    if (tmpW > m_dtul) {
        *vwo = m_dtul;
    } else if (tmpW < m_dtll) {
        *vwo = m_dtll;
    } else {
        *vwo = tmpW;
    }
}//}}}

///*********************************
/// Function     : pwm8
/// Description  : pwm8 Method
///*********************************
// Description: pwm8
// Ref: {HWCORE_PWM_UD_Flow_017}
void HWCORE_PWM::Pwm8(int vui, int vvi, int vwi, int *pwmuo, int *pwmvo, int *pwmwo)
{//{{{
    if (pwmuo == NULL || pwmvo == NULL || pwmwo == NULL) { return; }

    unsigned int tmp_base(0), tmp_zero(0);    // 18 bit unsigned
    int tmpU(0), tmpV(0), tmpW(0);
    int round(0);
    unsigned int remainder(0);

    if (m_pwmctr_pwmsel == 0) {
        tmp_base = m_carr >> 1;
    } else {
        tmp_base = ((m_carr + m_dtt) & 0x3ffff) >> 1;
        //tmp_base = tmp_base & 0x3FFFF;
        Chk_overflow_u(((OVFC_U)m_carr + (OVFC_U)m_dtt) >> 1, 0x3FFFF, &m_pwm_ovfl_out);
    }

    if (m_pwmctr_pwmop == 2) {
        tmp_zero = 0;
    } else {
        tmp_zero = ((m_carr + m_dtt) & 0x3ffff) >> 1;
       // tmp_zero = tmp_zero & 0x3FFFF;
        Chk_overflow_u(((OVFC_U)m_carr + (OVFC_U)m_dtt) >> 1, 0x3FFFF, &m_pwm_ovfl_out);
    }

    round = ((OVFC_S)vui * (OVFC_S)m_pwmk2) >> 32;
    remainder = ((OVFC_S)vui * (OVFC_S)m_pwmk2) & 0xFFFFFFFF;
    tmpU = (OVFC_S)round * (OVFC_S)tmp_base + (((OVFC_S)remainder * (OVFC_S)tmp_base) >> 32);
    Chk_overflow_s((OVFC_S)round * (OVFC_S)tmp_base + (((OVFC_S)remainder * (OVFC_S)tmp_base) >> 32) , 0xFFFFFFFF, &m_pwm_ovfl_out);
    
    round = ((OVFC_S)vvi * (OVFC_S)m_pwmk2) >> 32;
    remainder = ((OVFC_S)vvi * (OVFC_S)m_pwmk2) & 0xFFFFFFFF;
    tmpV = (OVFC_S)round * (OVFC_S)tmp_base + (((OVFC_S)remainder * (OVFC_S)tmp_base) >> 32);
    Chk_overflow_s((OVFC_S)round * (OVFC_S)tmp_base + (((OVFC_S)remainder * (OVFC_S)tmp_base) >> 32) , 0xFFFFFFFF, &m_pwm_ovfl_out);
    
    round = ((OVFC_S)vwi * (OVFC_S)m_pwmk2) >> 32;
    remainder = ((OVFC_S)vwi * (OVFC_S)m_pwmk2) & 0xFFFFFFFF;
    tmpW = (OVFC_S)round * (OVFC_S)tmp_base + (((OVFC_S)remainder * (OVFC_S)tmp_base) >> 32);
    Chk_overflow_s((OVFC_S)round * (OVFC_S)tmp_base + (((OVFC_S)remainder * (OVFC_S)tmp_base) >> 32) , 0xFFFFFFFF, &m_pwm_ovfl_out);

    switch (m_pwmctr_pwmop)
    {
    case 0:
        *pwmuo = tmp_zero + tmpU;
        Chk_overflow_s((OVFC_S)tmp_zero + (OVFC_S)tmpU, 0xFFFFFFFF, &m_pwm_ovfl_out);
        *pwmvo = tmp_zero + tmpV; //check ovf
        Chk_overflow_s((OVFC_S)tmp_zero + (OVFC_S)tmpV, 0xFFFFFFFF, &m_pwm_ovfl_out);
        *pwmwo = tmp_zero + tmpW; //check ovf
        Chk_overflow_s((OVFC_S)tmp_zero + (OVFC_S)tmpW, 0xFFFFFFFF, &m_pwm_ovfl_out);
        break;
    case 1:
        *pwmuo = tmp_zero - tmpU; //check ovf
        Chk_overflow_s((OVFC_S)tmp_zero - (OVFC_S)tmpU, 0xFFFFFFFF, &m_pwm_ovfl_out);
        *pwmvo = tmp_zero - tmpV; //check ovf
        Chk_overflow_s((OVFC_S)tmp_zero - (OVFC_S)tmpV, 0xFFFFFFFF, &m_pwm_ovfl_out);
        *pwmwo = tmp_zero - tmpW; //check ovf
        Chk_overflow_s((OVFC_S)tmp_zero - (OVFC_S)tmpW, 0xFFFFFFFF, &m_pwm_ovfl_out);
        break;
    case 2:
        *pwmuo = tmpU; 
        *pwmvo = tmpV; 
        *pwmwo = tmpW;
        break;
    default:
        break;
    }
}//}}}

///*********************************
/// Function     : pwm9
/// Description  : pwm9 Method
///*********************************
// Description: pwm9
// Ref: {HWCORE_PWM_UD_Flow_018}
void HWCORE_PWM::Pwm9(int pwmui, int pwmvi, int pwmwi, int iui, int ivi, int iwi, int *pwmuo, int *pwmvo, int *pwmwo)
{//{{{
    if (pwmuo == NULL || pwmvo == NULL || pwmwo == NULL) { return; }

    if (((long long int)iui > (long long int)m_dtoth) && (m_pwmctr_setdto == true)) {
        *pwmuo = pwmui + m_dtopv;
        Chk_overflow_s((OVFC_S)pwmui + (OVFC_S)m_dtopv, 0xFFFFFFFF, &m_pwm_ovfl_out);
    } else if (((long long int)iui < -(long long int)m_dtoth) && (m_pwmctr_setdto == true)) {
        *pwmuo = pwmui + m_dtonv;
        Chk_overflow_s((OVFC_S)pwmui + (OVFC_S)m_dtonv, 0xFFFFFFFF, &m_pwm_ovfl_out);
    } else {
        *pwmuo = pwmui;
    }

    if (((long long int)ivi > (long long int)m_dtoth) && (m_pwmctr_setdto == true)) {
        *pwmvo = pwmvi + m_dtopv;
        Chk_overflow_s((OVFC_S)pwmvi + (OVFC_S)m_dtopv, 0xFFFFFFFF, &m_pwm_ovfl_out);
    } else if (((long long int)ivi < -(long long int)m_dtoth) && (m_pwmctr_setdto == true)) {
        *pwmvo = pwmvi + m_dtonv;
        Chk_overflow_s((OVFC_S)pwmvi + (OVFC_S)m_dtonv, 0xFFFFFFFF, &m_pwm_ovfl_out);
    } else {
        *pwmvo = pwmvi;
    }

    if (((long long int)iwi > (long long int)m_dtoth) && (m_pwmctr_setdto == true)) {
        *pwmwo = pwmwi + m_dtopv;
        Chk_overflow_s((OVFC_S)pwmwi + (OVFC_S)m_dtopv, 0xFFFFFFFF, &m_pwm_ovfl_out);
    } else if (((long long int)iwi < -(long long int)m_dtoth) && (m_pwmctr_setdto == true)) {
        *pwmwo = pwmwi + m_dtonv;
        Chk_overflow_s((OVFC_S)pwmwi + (OVFC_S)m_dtonv, 0xFFFFFFFF, &m_pwm_ovfl_out);
    } else {
        *pwmwo = pwmwi;
    }
}//}}}

///*********************************
/// Function     : pwm10
/// Description  : pwm10 Method
///*********************************
// Description: pwm10
// Ref: {HWCORE_PWM_UD_Flow_019}
void HWCORE_PWM::Pwm10(int pwmui, int pwmvi, int pwmwi, unsigned int *pwmuo, unsigned int *pwmvo, unsigned int *pwmwo, unsigned int *carro)
{//{{{
    if (pwmuo == NULL || pwmvo == NULL || pwmwo == NULL || carro == NULL) { return; }

    int pwm_max = ((m_carr + m_dtt) - m_pwmul); // uint_18 pwm_max
    Chk_overflow_u((OVFC_U)m_carr + (OVFC_U)m_dtt - (OVFC_U)m_pwmul, 0x3FFFF, &m_pwm_ovfl_out);
    pwm_max = pwm_max & 0x3FFFF;

    if (m_volv == 0) {
        *pwmuo = ((m_carr + m_dtt) & 0x3FFFF) >> 1;
       // *pwmuo = (*pwmuo) & 0x3FFFF;
        Chk_overflow_u(((OVFC_U)m_carr + (OVFC_U)m_dtt) >> 1, 0x3FFFF, &m_pwm_ovfl_out);
    } else if (pwmui >= pwm_max) {
        *pwmuo = m_carr + m_dtt;
        *pwmuo = (*pwmuo) & 0x3FFFF;
        Chk_overflow_u((OVFC_U)m_carr + (OVFC_U)m_dtt, 0x3FFFF, &m_pwm_ovfl_out);
    } else if (pwmui <= (int)m_pwmll) {
        *pwmuo = 0;
    } else {
        *pwmuo = pwmui & 0x3FFFF;
    }

    if (m_volv == 0) {
        *pwmvo = ((m_carr + m_dtt) & 0x3FFFF) >> 1;
       // *pwmvo = (*pwmvo) & 0x3FFFF;
        Chk_overflow_u(((OVFC_U)m_carr + (OVFC_U)m_dtt) >> 1, 0x3FFFF, &m_pwm_ovfl_out);
    } else if (pwmvi >= pwm_max) {
        *pwmvo = m_carr + m_dtt;
        *pwmvo = (*pwmvo) & 0x3FFFF;
        Chk_overflow_u((OVFC_U)m_carr + (OVFC_U)m_dtt, 0x3FFFF, &m_pwm_ovfl_out);
    } else if (pwmvi <= (int)m_pwmll) {
        *pwmvo = 0;
    } else {
        *pwmvo = pwmvi & 0x3FFFF;
    }

    if (m_volv == 0) {
        *pwmwo = ((m_carr + m_dtt) & 0x3FFFF) >> 1;
       //*pwmwo = (*pwmwo) & 0x3FFFF;
        Chk_overflow_u(((OVFC_U)m_carr + (OVFC_U)m_dtt) >> 1, 0x3FFFF, &m_pwm_ovfl_out);
    } else if (pwmwi >= pwm_max) {
        *pwmwo = m_carr + m_dtt;
        *pwmwo = (*pwmwo) & 0x3FFFF;
        Chk_overflow_u((OVFC_U)m_carr + (OVFC_U)m_dtt, 0x3FFFF, &m_pwm_ovfl_out);
    } else if (pwmwi <= (int)m_pwmll) {
        *pwmwo = 0;
    } else {
        *pwmwo = pwmwi & 0x3FFFF;
    }
    
    *carro = m_carr;
}//}}}

///*********************************
/// Function     : pwm_out 
/// Description  : pwm_out Method
///*********************************
// Description: pwm_out
// Ref: {HWCORE_PWM_UD_Flow_020}
void HWCORE_PWM::Pwm_out(unsigned int pwmui, unsigned int pwmvi, unsigned int pwmwi, unsigned int carri, unsigned int *pwmuo, unsigned int *pwmvo, unsigned int *pwmwo, unsigned int *carro)
{//{{{
    if (pwmuo == NULL || pwmvo == NULL || pwmwo == NULL || carro == NULL) { return; }

    unsigned int tmpu(0), tmpv(0), tmpw(0), tmpc(0);
    unsigned int tmpsu(0), tmpsv(0), tmpsw(0), tmpsc(0);

    if (m_pwmctr_setpwm == 0) { // Output carrier/PWM comparison value input by software
        tmpu = m_pwmu;
        tmpv = m_pwmv;
        tmpw = m_pwmw;
        tmpc = m_carr;
    } else {
        tmpu = pwmui;
        tmpv = pwmvi;
        tmpw = pwmwi;
        tmpc = carri;
    }

    if (m_pwmctr_shipwm == 1) { //Shift the output carrier/PWM comparison value 1 bit to the left
        tmpsu = ((unsigned int)(tmpu << 1)) & 0x3FFFF;
        tmpsv = ((unsigned int)(tmpv << 1)) & 0x3FFFF;
        tmpsw = ((unsigned int)(tmpw << 1)) & 0x3FFFF;
        tmpsc = ((unsigned int)(tmpc << 1)) & 0x3FFFF;
    } else {
        tmpsu = tmpu;
        tmpsv = tmpv;
        tmpsw = tmpw;
        tmpsc = tmpc;
    }

    if ((m_pwmctr_setpwm == false && m_pwmdt_pwmdt == true) || (m_pwmctr_setpwm == true && m_pwm_end_flag == true)) {
        *pwmuo = tmpsu;
        *pwmvo = tmpsv;
        *pwmwo = tmpsw;
        *carro = tmpsc;

        m_change_tstwecmp_flag = true;
    } else {
        m_change_tstwecmp_flag = false;
    }

    m_pwm_end_flag = false;

}//}}}

///*********************************
/// Function     : Handle_PWMDT
/// Description  : Call back function for EMU3nPWMDT_PWMDT bit
///*********************************
// Description: Handle_PWMDT
// Ref: {HWCORE_PWM_UD_Flow_008}
void HWCORE_PWM::Handle_PWMDT(void)
{//{{{
    m_pwmdt_write_flag = true;
    m_pwm_out.notify();
    mpHWCORE_PWMFunc->SetRegVal("PWMDT", 0x0);
}//}}}


//----------------------------------------------
//             Helper Function
//----------------------------------------------

///*********************************
/// Function     : Chk_overflow_u
/// Description  : Chk_overflow_u Method
///*********************************
void HWCORE_PWM::Chk_overflow_u( OVFC_U input, OVFC_U check, bool* ovfflag)
{//{{{
    if (ovfflag  == NULL) { return; }
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
void HWCORE_PWM::Chk_overflow_s( OVFC_S input, OVFC_U check, bool* ovfflag)
{//{{{
    if (ovfflag  == NULL) { return; }
    signed long long check_val1 = 0x000000007fffffffLL;
    signed long long check_val2 = 0xffffffff80000000LL;

    if( ( input > check_val1 ) || (input < check_val2) ) { /* Overflow or underflow upon the occurrence */
        /* Overflow flag ON */
        *ovfflag = true;
    }
}//}}}

///*********************************
/// Function     : Calc_division
/// Description  : Calculate division arithmetic
///*********************************
void HWCORE_PWM::Calc_division(int devidend, short int divisor, int *ret, bool *pwm_Zdiv)
{//{{{
    if (pwm_Zdiv == NULL || ret == NULL) { return; }
    
    if (divisor == 0) {
        if (devidend == 0) {
            *ret = 0;
        } else if (devidend > 0) {
            *ret = 0x7FFFFFFF;
        } else {   // (devidend < 0)
            *ret = 0x80000000;
        }
        *pwm_Zdiv = true;
    } else {
        *ret = devidend / (signed int)divisor;
    }
}//}}}

// vim600: set foldmethod=marker :
