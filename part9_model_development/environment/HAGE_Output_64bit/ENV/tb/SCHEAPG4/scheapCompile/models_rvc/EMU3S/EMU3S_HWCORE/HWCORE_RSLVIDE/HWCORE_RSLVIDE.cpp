// ---------------------------------------------------------------------
// $Id: HWCORE_RSLVIDE.cpp,v 1.13 2020/11/17 06:13:14 ngathunguyen Exp $
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
// Description: Copyright of HWCORE_RSLVIDE.cpp
// Ref: {HWCORE_RSLVIDE_UD_Copyright_001}
// Description: About the include header files of HWCORE_RSLVIDE.cpp
// Ref: {HWCORE_RSLVIDE_UD_ATT1_002, HWCORE_RSLVIDE_UD_Direction_028}
#include "re_define.h"
#include "HWCORE_RSLVIDE.h"
#include "HWCORE_RSLVIDE_Func.h"
// Description: Constructor of HWCORE_RSLVIDE class
// Ref: {HWCORE_RSLVIDE_UD_Summary_001, HWCORE_RSLVIDE_UD_ATT1_004}
HWCORE_RSLVIDE::HWCORE_RSLVIDE(sc_module_name name)
    :sc_module(name)
    ,HWCORE_RSLVIDE_AgentController()
    ,CRWInterface()
    ,rvc_common_model()
    ,PCLK("PCLK")
    ,PRESETn("PRESETn")
    ,mip_prst_n("mip_prst_n")
    //HWCORE_MEASURE 
    ,phi_out_mip_clk("phi_out_mip_clk")
    //EMU3S_HWCORE
    ,FUNCFLGRPB("FUNCFLGRPB")
    ,FUNCIDLEGRPB("FUNCIDLEGRPB")
    ,FUNCWAITGRPB("FUNCWAITGRPB")
    //HWCORE_RCTG"/HWCORE_RCTG
    ,em2icmp0("em2icmp0")
    ,wc_em2icmp0("wc_em2icmp0")
    ,em2icmp1("em2icmp1")
    ,wc_em2icmp1("wc_em2icmp1")
    ,rdata_em2iipcmp0("rdata_em2iipcmp0")
    ,em2ibrecctr_slctcmp0_reg("em2ibrecctr_slctcmp0_reg")
    ,output_rctg_end_sp_reg("output_rctg_end_sp_reg")
    //Output port 
    ,rslv_func_st("rslv_func_st")
    ,rslv_func_end("rslv_func_end")
    ,rslv_func_wait("rslv_func_wait")
    ,rslv_func_busy("rslv_func_busy")
    ,ANGCTR("ANGCTR")
    ,RESTHSFT("RESTHSFT")
    ,rdata_em2ithtresfix("rdata_em2ithtresfix")
    ,rdata_em2ithtefix("rdata_em2ithtefix")
    ,presml_ang_comp0_eq_sp("presml_ang_comp0_eq_sp")
    ,ang_comp1_eq_sp("ang_comp1_eq_sp")   
{//{{{ 

    mpHWCORE_RSLVIDEFunc = new HWCORE_RSLVIDE_Func((std::string)name, this);
    sc_assert(mpHWCORE_RSLVIDEFunc != NULL);
    // Initialize variables
    mPCLKPeriod             = 0;    
    mPCLKOrgFreq            = 0;   
    mPCLKFreq               = 0;      
    mPCLKFreqUnit           = "Hz";  
    mStartPCLKTime          = 0; 
    
    // Angle cycle
    m_ang_func_cyc[emANG00]        = 0x1;
    m_ang_func_cyc[emANG01]        = 0x1;
    m_ang_func_cyc[emANG02]        = 0x2;
    m_ang_func_cyc[emANG03]        = 0x2;
    m_ang_ip_border_cyc            = 0x3;
    
    mTimeResolutionValue    = 1;
    mTimeResolutionUnit     = SC_NS;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id]       = true;
        mIsResetHardActive[reset_id] = false;

        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id]  = false;
        mResetCmdPeriod[reset_id]    =  0;
    };   
    
    mDumpInterrupt                   = false;
    mEnableRegisterMessage           = true;
    mEnableStatInfo                  = false;    
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    // Initialize all output port and variable
    InitializeVariable(emMip_prst_nId);
    InitializeOutputPort();
    // SC_THREAD/METHOD
    SC_METHOD(HandlePCLKSignalMethod);
    sensitive << PCLK;

    SC_METHOD(HandlePRESETNSignalMethod);
    sensitive << PRESETn;
    
    SC_METHOD(HandleMip_prst_nSignalMethod);
    sensitive << mip_prst_n;
    
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RSLVIDE::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RSLVIDE::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RSLVIDE::CancelResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
    for (unsigned int port_id = 0; port_id < emOUPUT_PORT_NUM; port_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&m_update_output_port_event[port_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RSLVIDE::HandleOutputEdgePortMethod, this, port_id),
            sc_core::sc_gen_unique_name("HandleOutputEdgePortMethod"), &opt);
    }
    for (unsigned char func_id = 0; func_id < emANG_FUNC_NUM; func_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&m_ang_add_cyc_event[func_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RSLVIDE::CalAngleCycle, this, func_id),
            sc_core::sc_gen_unique_name("CalAngleCycle"), &opt);
    }
    
    for (unsigned char func_id = 0; func_id < emANG_FUNC_NUM; func_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&m_ang_start_event[func_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RSLVIDE::GeneralAngleFunc, this, func_id),
            sc_core::sc_gen_unique_name("GeneralAngleFunc"), &opt);
    }
    
    SC_METHOD(HandlePhiOutPortMethod);
    dont_initialize();
    sensitive << phi_out_mip_clk;
    
    SC_METHOD(HandleFuncwaitgrpbPortMethod);
    dont_initialize();
    sensitive << FUNCWAITGRPB;
    
    SC_METHOD(HandleRCTGSignalMethod);
    dont_initialize();
    sensitive << wc_em2icmp0;
    sensitive << wc_em2icmp1;
    sensitive << output_rctg_end_sp_reg;
        
}//}}}

// Description: Destructor
HWCORE_RSLVIDE::~HWCORE_RSLVIDE(void)
{//{{{
    delete mpHWCORE_RSLVIDEFunc;
}//}}}


// Description: Initialize variable
void HWCORE_RSLVIDE::InitializeVariable(const unsigned int reset_id)
{//{{{
    //----Initialize variable--------
    for(unsigned char i = 0; i < emOUPUT_PORT_NUM; i++) {
        if ((i == emPRESML_ANG_COMP0_EQ_SP) || (i == emANG_COMP1_EQ_SP)) { 
           if((reset_id == emMip_prst_nId)) {
                m_output_port_value[i]   = 0x0;
           } else {
                continue;
           }
        } else if (i == emANGCTR) {
            m_output_port_value[i]   = 0x1;
        } else {
            m_output_port_value[i]   = 0x0;
        }
    }
    if (reset_id == emMip_prst_nId) {
        m_ang_status                   = emANG_IDLE;
        m_ang_ip_cyc_num               = 0x0;
                                       
        // Angle operation
        m_ang3_comp0match_check_flag   = false;
        m_ang3_comp0_match_flag        = false;
        m_ang3_comp1_match_flag        = false;
        m_comp0_match1                 = false;
        m_comp1_match                  = false;
        m_wc_em2icmp0_value            = false;
        m_wc_em2icmp1_value            = false;
        m_output_rctg_end_sp_reg_value = false;
        //Register internal variable
        m_resolver_ang_pre_value       = 0;
        m_resolver_ang_curr_value      = 0;
        m_angofs_buff_value            = 0;
        m_thtresfix_pre_value          = 0;
        m_thtefix_pre_value            = 0;
        m_rescnt_pre_value             = 0;            
    }  
    
}//}}}

// Description: Initialize output port in SSCG domain
void HWCORE_RSLVIDE::InitializeOutputPort()
{//{{{
//----Initialize output port---------
    presml_ang_comp0_eq_sp.initialize(false);
    ang_comp1_eq_sp.initialize(false);
    rslv_func_st.initialize(0x0);                
    rslv_func_end.initialize(0x0);         
    rslv_func_wait.initialize(false);        
    rslv_func_busy.initialize(false);        
    ANGCTR.initialize(0x1);                
    RESTHSFT.initialize(0x0);              
    rdata_em2ithtresfix.initialize(0x0);   
    rdata_em2ithtefix.initialize(0x0);  
}//}}}   
//========================================================
//============ Reset and clock  ==========================
//========================================================
// Description: Monitor PRESETN port
// Ref: {HWCORE_RSLVIDE_UD_Ports_002, HWCORE_RSLVIDE_UD_Feature_002}
void HWCORE_RSLVIDE::HandlePRESETNSignalMethod(void)
{//{{{
    mResetCurVal[emPRESETnId] = PRESETn.read();
    // Reset signals are asynchronous
    mResetHardEvent[emPRESETnId].notify(SC_ZERO_TIME);
}//}}}

// Description: Monitor mip_prst_n port
// Ref: {HWCORE_RSLVIDE_UD_Ports_001, HWCORE_RSLVIDE_UD_Feature_003}
void HWCORE_RSLVIDE::HandleMip_prst_nSignalMethod()
{//{{{
    mResetCurVal[emMip_prst_nId] = mip_prst_n.read();
    // Reset signals are asynchronous
    mResetHardEvent[emMip_prst_nId].notify(SC_ZERO_TIME);
}//}}}

// Description: Monitor PCLK clock
// Ref: {HWCORE_RSLVIDE_UD_Ports_003, HWCORE_RSLVIDE_UD_Feature_001, HWCORE_RSLVIDE_UD_Flow_002}
void HWCORE_RSLVIDE::HandlePCLKSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

// Description: Process reset function when reset port is active
// Ref: {HWCORE_RSLVIDE_UD_Direction_042, HWCORE_RSLVIDE_UD_Direction_043, HWCORE_RSLVIDE_UD_Flow_001 HWCORE_RSLVIDE_UD_Direction_041}
void HWCORE_RSLVIDE::HandleResetHardMethod(const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info", "The reset port PRESETn is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        } else {
            re_printf("info", "The reset port mip_prst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        }
    }
    else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) {
            re_printf("info", "The reset port PRESETn is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        }
        else {
            re_printf("info", "The reset port mip_prst_n is de-asserted.\n");
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

// Description: Handle AssertReset Command Method
// Ref: {HWCORE_RSLVIDE_UD_Direction_039}
void HWCORE_RSLVIDE::HandleResetCmdMethod(const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else {
        reset_name = "mip_prst_n";
    } 
    mIsResetCmdActive[reset_id] = true;
    re_printf("info", "The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

// Description: Cancel AssertReset Command Method
// Ref: {HWCORE_RSLVIDE_UD_Direction_038}
void HWCORE_RSLVIDE::CancelResetCmdMethod(const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else {
        reset_name = "mip_prst_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info", "Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
}//}}}

// Description: Reset model and its sub-instance if PRESETn is activated
// Ref: {HWCORE_RSLVIDE_UD_Flow_001}
void HWCORE_RSLVIDE::EnableReset(const bool is_active, const unsigned int reset_id)
{//{{{
    if (is_active) {
        // Cancel events
        CancelEvents(reset_id);
        // Initialize variables, output port
        InitializeVariable(reset_id);
        // Update output port
        for(unsigned char i = 0; i < emOUPUT_PORT_NUM; i++) {
            m_update_output_port_event[i].notify();
        }
    }
    mpHWCORE_RSLVIDEFunc->EnableReset(is_active);
   
}//}}}

// Description: Function to check reset and zero clock
bool HWCORE_RSLVIDE::ModelinResetStateorZeroClock()
{//{{{
    bool reset = true;
    bool clock = true;
    reset = GetResetStatus();
    clock = CheckClockPeriod();
    return (reset || !clock);
}//}}}
//========================================================
//============ PYTHON IF               ===================
//========================================================
// Description: Set message level (fatal, error, warning, info)
void HWCORE_RSLVIDE::SetMessageLevel(const std::string msg_lv)
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
    if (true == mEnableRegisterMessage) {
        SeparateString(cmd, msg_lv);
    }else{
        //Set message level for Register IF class (except fatal level)
        cmd.push_back("fatal");
    }
    mpHWCORE_RSLVIDEFunc->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    if (true == mEnableRegisterMessage) {
        SeparateString(cmd, msg_lv);
    }else{
        //Set message level for Register IF class (except fatal level)
        cmd.push_back("fatal");
    }
    mpHWCORE_RSLVIDEFunc->RegisterHandler(cmd);
}//}}}

// Description: Enable/disable dumping message when users access registers
void HWCORE_RSLVIDE::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_RSLVIDEFunc->RegisterHandler(cmd);
}//}}}

// Description: Enable/disable info/warning/error message of register IF
// Ref: {HWCORE_RSLVIDE_UD_Direction_033, HWCORE_RSLVIDE_UD_Direction_044}
void HWCORE_RSLVIDE::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }else if(word_vector[0] == "false"){
            mEnableRegisterMessage = false;
        }else{
            re_printf("warning", "Invalid argument: HWCORE_RSLVIDE_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }else if(word_vector.size() == 0){
        std::string temp = "";
        if(mEnableRegisterMessage){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info", "HWCORE_RSLVIDE_EnableRegisterMessage %s\n", temp.c_str());
    }else{
        re_printf("warning", "Invalid argument: HWCORE_RSLVIDE_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

// Description: Enable/disable dumping message when users access registers
// Ref: {HWCORE_RSLVIDE_UD_Direction_033, HWCORE_RSLVIDE_UD_Direction_044}
void HWCORE_RSLVIDE::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_RSLVIDE_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string status = "true";
        if (mDumpInterrupt == false)
        {
            status = "false";
        }
        re_printf("info", "HWCORE_RSLVIDE_DumpInterrupt %s\n", status.c_str());
    } else {
        re_printf("warning", "Invalid argument: HWCORE_RSLVIDE_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

// Description: EnableStatInfo
// Ref: {HWCORE_RSLVIDE_UD_Direction_049, HWCORE_RSLVIDE_UD_Direction_044}
void HWCORE_RSLVIDE::EnableStatInfo(const std::string is_enable)
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
            re_printf("warning", "Invalid argument: HWCORE_RSLVIDE_EnableStatInfo %s\n", is_enable.c_str());
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
        re_printf("info", "HWCORE_RSLVIDE_EnableStatInfo %s\n", temp_arg.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: HWCORE_RSLVIDE_EnableStatInfo %s\n", is_enable.c_str());
    }
}//}}}

// Function     : DumpStatusInfo
// Description  : Dump buffer
// Ref: {HWCORE_RSLVIDE_UD_Direction_005}
///*********************************
void HWCORE_RSLVIDE::DumpStatInfo()
{//{{{
    
    DumpStatInfo(this->basename(), "ANGOFS", m_angofs_buff_value);
    DumpStatInfo(this->basename(), "THTRESFIX", mpHWCORE_RSLVIDEFunc->GetRegVal("THTRESFIX"));
    DumpStatInfo(this->basename(), "THTRESFIX previous value", m_thtresfix_pre_value);
    DumpStatInfo(this->basename(), "THTEFIX", mpHWCORE_RSLVIDEFunc->GetRegVal("THTEFIX"));
    DumpStatInfo(this->basename(), "THTEFIX previous value", m_thtefix_pre_value);
}//}}}

// Function     : DumpStatusInfo
// Description  : Dump buffer
// Ref: {HWCORE_RSLVIDE_UD_Direction_047}
///*********************************
void HWCORE_RSLVIDE::DumpStatInfo(std::string block, std::string reg_name, int nOpeValue)
{//{{{
    if(mEnableStatInfo != false) {
        re_printf("info", "(%s) %s: 0x%x \n", block.c_str(), reg_name.c_str(), nOpeValue);
    }
}//}}}

// Description: Command to assert reset
// Ref: {HWCORE_RSLVIDE_UD_Direction_035, HWCORE_RSLVIDE_UD_Direction_040, HWCORE_RSLVIDE_UD_Direction_034}
void HWCORE_RSLVIDE::AssertReset(const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETnId]) && (!mIsResetCmdReceive[emPRESETnId])) {
            mIsResetCmdReceive[emPRESETnId] = true;
            re_printf("info", "The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPRESETnId].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETnId] = period;
        } else {
            re_printf("warning", "The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else if (reset_name == "mip_prst_n") {
        if ((!mIsResetHardActive[emMip_prst_nId]) && (!mIsResetCmdReceive[emMip_prst_nId])) {
            mIsResetCmdReceive[emMip_prst_nId] = true;
            re_printf("info", "The model will be reset (mip_prst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emMip_prst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[emMip_prst_nId] = period;
        } else {
            re_printf("warning", "The software reset of mip_prst_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning", "The reset name (%s) is wrong. It should be PRESETn or mip_prst_n ", reset_name.c_str());
    }
}//}}}

// Description: Set clock value and clock unit
// Ref: {HWCORE_RSLVIDE_UD_Direction_032}
void HWCORE_RSLVIDE::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq(mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1 / (double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            }else{
                mPCLKPeriod = 0;
                // Prepare for next run
                CancelEvents(emMip_prst_nId);
            }
        } else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

// Description: Get clock value
// Ref: {HWCORE_RSLVIDE_UD_Direction_032, HWCORE_RSLVIDE_UD_Direction_043}
void HWCORE_RSLVIDE::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info", "PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

// Description: Force value to register
// Ref: {HWCORE_RSLVIDE_UD_Direction_032}
void HWCORE_RSLVIDE::ForceRegister(const std::string reg_name, const unsigned int reg_value)
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
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mpHWCORE_RSLVIDEFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Release forced value after forcing registers
// Ref: {HWCORE_RSLVIDE_UD_Direction_032}
void HWCORE_RSLVIDE::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHWCORE_RSLVIDEFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Write value to registers by software
// Ref: {HWCORE_RSLVIDE_UD_Direction_032}
void HWCORE_RSLVIDE::WriteRegister(const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mpHWCORE_RSLVIDEFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Read value of register by software
// Ref: {HWCORE_RSLVIDE_UD_Direction_032}
void HWCORE_RSLVIDE::ReadRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpHWCORE_RSLVIDEFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: List all registers name
// Ref: {HWCORE_RSLVIDE_UD_Direction_013}
void HWCORE_RSLVIDE::ListRegister(void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHWCORE_RSLVIDEFunc->RegisterHandler(cmd);
}//}}}

// Description: Dump help message of all parameters or commands
// Ref: {HWCORE_RSLVIDE_UD_Direction_016, HWCORE_RSLVIDE_UD_Direction_017, HWCORE_RSLVIDE_UD_Direction_028}
void HWCORE_RSLVIDE::Help(const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_MessageLevel (\"HWCORE_RSLVIDE instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_DumpRegisterRW (\"HWCORE_RSLVIDE instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_EnableRegisterMessage (\"HWCORE_RSLVIDE instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_DumpInterrupt (\"HWCORE_RSLVIDE instance\", \"true/false\")                     Enable/disable dumping interrupt value (Default: false)");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_EnableStatInfo (\"HWCORE_RSLVIDE instance\", \"true/false\")                    Enable/disable dumping the internal buffer registers' values of the HWCORE_RSLVIDE model (Default: false)");
        }else if (word_vector[0] == "commands"){
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_DumpStatInfo (\"HWCORE_RSLVIDE instance\")                                      Dump the values of the internal buffer registers of the HWCORE_RSLVIDE model");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_AssertReset (\"HWCORE_RSLVIDE instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the HWCORE_RSLVIDE model");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_SetCLKFreq (\"HWCORE_RSLVIDE instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to HWCORE_RSLVIDE model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_GetCLKFreq (\"HWCORE_RSLVIDE instance\", \"clock_name\")                        Get clock frequency value of HWCORE_RSLVIDE model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_ForceRegister (\"HWCORE_RSLVIDE instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_ReleaseRegister (\"HWCORE_RSLVIDE instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_WriteRegister (\"HWCORE_RSLVIDE instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_ReadRegister (\"HWCORE_RSLVIDE instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_RSLVIDE_ListRegister (\"HWCORE_RSLVIDE instance\")        Dump name of all registers");
            SC_REPORT_INFO(this->basename()," HWCORE_RSLVIDE_ChgProcessingTime (\"HWCORE_RSLVIDE instance\", \"func_name\", \"cycle\")       Change the processing of function.");
        }else{
            re_printf("warning", "The name (%s) of HWCORE_RSLVIDE_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    }else{
        re_printf("warning", "The name (%s) of HWCORE_RSLVIDE_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}
// Description: Dump help message of all parameters or commands
// Ref: {HWCORE_RSLVIDE_UD_Direction_033}
void HWCORE_RSLVIDE::ChgProcessingTime (const std::string func_name, const unsigned int value)
{//{{{
    if (func_name == "ang0") {
        m_ang_func_cyc[emANG00] = value;
    } else if (func_name == "ang1") {
        m_ang_func_cyc[emANG01] = value;
    } else if (func_name == "ang2") {
        m_ang_func_cyc[emANG02] = value;
    } else if (func_name == "ang3") {
        m_ang_func_cyc[emANG03] = value;
    } else if (func_name == "ang_b") {
        m_ang_ip_border_cyc = value;
    } else {
        re_printf("warning", "Invalid argument: HWCORE_RSLVIDE_ChgProcessingTime %s\n", func_name.c_str());
    }
}//}}} 
//========================================================
//============ Overwrite func of CRWInterface.h ==========
//========================================================
// Overwrite read APIs for register access
//Ref: {HWCORE_RSLVIDE_UD_Flow_004}
void HWCORE_RSLVIDE::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if(data_ptr == NULL){
        return;
    }
    mpHWCORE_RSLVIDEFunc->read(offsetAddress, data_ptr, size, debug);
}//}}}

// Overwrite write APIs for write access
//Ref: {HWCORE_RSLVIDE_UD_Flow_005}
void HWCORE_RSLVIDE::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if(data_ptr == NULL){
        return;
    }
    mpHWCORE_RSLVIDEFunc->write(offsetAddress, data_ptr, size, debug);
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
// Description: Cancel  events
//Ref: {HWCORE_RSLVIDE_UD_Flow_001}
void HWCORE_RSLVIDE::CancelEvents(const unsigned int id)
{//{{{
    // Operation events are canceled           
    for (unsigned short port_id = 0; port_id < emOUPUT_PORT_NUM; port_id ++) {
        if ((port_id == emPRESML_ANG_COMP0_EQ_SP) || (port_id == emANG_COMP1_EQ_SP)) {
            if (id == emMip_prst_nId) {
                m_update_output_port_event[port_id].cancel();
            } else  {
                continue;
            }
        } else {
            m_update_output_port_event[port_id].cancel();
        }
    }
    if (id == emMip_prst_nId) {
        for (unsigned short num = 0; num < emANG_FUNC_NUM; num ++) {
            m_ang_add_cyc_event[num].cancel();
            m_ang_start_event[num].cancel();
        }
    }
}//}}}

// Description: Handle output edge signal from RCTG model
// Ref: {HWCORE_RSLVIDE_UD_Flow_027}
void HWCORE_RSLVIDE::HandleRCTGSignalMethod(void)
{//{{{
    if (wc_em2icmp0.read() != false) {
        m_wc_em2icmp0_value = wc_em2icmp0.read();
    }
    if (wc_em2icmp1.read() != false) {
        m_wc_em2icmp1_value = wc_em2icmp1.read();
    }
    if (output_rctg_end_sp_reg.read() != false) {
        m_output_rctg_end_sp_reg_value = output_rctg_end_sp_reg.read();
    }
}//}}}
    
//========================================================
//============ Startup trigger source handler  ===========
//========================================================

// Description: Handle FUNCWAITGRPB Port
// Ref: {HWCORE_RSLVIDE_UD_Flow_013, HWCORE_RSLVIDE_UD_ATT1_005}
void HWCORE_RSLVIDE::HandleFuncwaitgrpbPortMethod()
{//{{{
    if (ModelinResetStateorZeroClock() != false) return;
    if (m_ang_status != emANG_WAIT) return;
    unsigned short start_func = FUNCWAITGRPB.read();
    switch (start_func) {
        case 0x100:
        m_ang_add_cyc_event[emANG00].notify((double)m_ang_func_cyc[emANG00] * mPCLKPeriod, mTimeResolutionUnit);
        break;
        
        case 0x101:
        m_ang_add_cyc_event[emANG01].notify((double)m_ang_func_cyc[emANG01] * mPCLKPeriod, mTimeResolutionUnit);
        break;
        
        case 0x102:
        m_ang_add_cyc_event[emANG02].notify((double)m_ang_func_cyc[emANG02] * mPCLKPeriod, mTimeResolutionUnit);
        break;
        
        case 0x103:
        m_ang_add_cyc_event[emANG03].notify((double)m_ang_func_cyc[emANG03] * mPCLKPeriod, mTimeResolutionUnit);
        break;
        
        case 0x109:
        m_ang_ip_cyc_num = 0x0;
        SetAngleState(emANG_IDLE);
        break;
        
        default:
        // Do nothing
        break;
    } 
}//}}} 

// Description: Calculation Angle Cycle
// Ref: {HWCORE_RSLVIDE_UD_Flow_014}
void HWCORE_RSLVIDE::CalAngleCycle(unsigned char ANG_ID)
{//{{{
    double wait_cyc = 0;
    unsigned int value = FUNCFLGRPB.read();
    m_ang_ip_cyc_num += m_ang_func_cyc[ANG_ID];

    if((ANG_ID == emANG03) || (((value >> (ANG_ID*2)) & 0x3) != 0)) {
        if (m_ang_ip_cyc_num < m_ang_ip_border_cyc) {
            wait_cyc = (double) (m_ang_ip_border_cyc - m_ang_ip_cyc_num) * mPCLKPeriod;
        }
    }
    m_ang_start_event[ANG_ID].notify(wait_cyc, mTimeResolutionUnit);
}//}}}

// Description: Handle phi_out_mip_clk port
// Ref: {HWCORE_RSLVIDE_UD_Flow_011, HWCORE_RSLVIDE_UD_ATT1_005}
void HWCORE_RSLVIDE::HandlePhiOutPortMethod()
{//{{{
    if (ModelinResetStateorZeroClock() != false) return;
    unsigned char ANGSEL = mpHWCORE_RSLVIDEFunc->GetRegVal("ANGCTR", "ANGSEL") & 0x3;
    if(ANGSEL == 0x0) {
        unsigned int res_value_temp = phi_out_mip_clk.read();
    
        if(m_resolver_ang_curr_value != res_value_temp) {
            m_resolver_ang_curr_value = res_value_temp;
            StartAngleIP();          
        }
    }
}//}}} 

// Description: Angle IP start operation
// Ref: {HWCORE_RSLVIDE_UD_Flow_012, HWCORE_RSLVIDE_UD_ATT1_005}
void HWCORE_RSLVIDE::StartAngleIP(void)
{//{{{
    if (ModelinResetStateorZeroClock() != false) return;
    if (m_ang_status != emANG_IDLE) return;
    // Update buffer register
    SetBuffRegister();
    unsigned short funcidlegrpb = FUNCIDLEGRPB.read();
    if (((funcidlegrpb >> 8) & 0x1) == 0) { //FUNCIDLEGRPB.IP
        m_ang_add_cyc_event[emANG00].notify((double)m_ang_func_cyc[emANG00] * mPCLKPeriod, mTimeResolutionUnit);
        SetAngleState(emANG_BUSY);
    } else if (funcidlegrpb == 0x100) {
        m_ang_add_cyc_event[emANG00].notify((double)m_ang_func_cyc[emANG00] * mPCLKPeriod, mTimeResolutionUnit);
        SetAngleState(emANG_BUSY);
    } else if (funcidlegrpb == 0x101) {
        m_ang_add_cyc_event[emANG01].notify((double)m_ang_func_cyc[emANG01] * mPCLKPeriod, mTimeResolutionUnit);
        SetAngleState(emANG_BUSY);
    } else if (funcidlegrpb == 0x102) {
        m_ang_add_cyc_event[emANG02].notify((double)m_ang_func_cyc[emANG02] * mPCLKPeriod, mTimeResolutionUnit);
        SetAngleState(emANG_BUSY);
    } else if (funcidlegrpb == 0x103) {
        m_ang_add_cyc_event[emANG03].notify((double)m_ang_func_cyc[emANG03] * mPCLKPeriod, mTimeResolutionUnit);
        SetAngleState(emANG_BUSY);
    }
    
}//}}}    
 
// Description: Angle IP start operation
// Ref: {HWCORE_RSLVIDE_UD_Flow_015}
void HWCORE_RSLVIDE::SetAngleState(unsigned char ANGLE_STATE)
{//{{{
    m_ang_status = ANGLE_STATE;
    if (ANGLE_STATE == emANG_BUSY) {
        m_output_port_value[emRSLV_FUNC_WAIT] = 0x0;
        m_update_output_port_event[emRSLV_FUNC_WAIT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
        m_output_port_value[emRSLV_FUNC_BUSY] = 0x1;
        m_update_output_port_event[emRSLV_FUNC_BUSY].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    } else if (ANGLE_STATE == emANG_WAIT) {
        m_output_port_value[emRSLV_FUNC_BUSY] = 0x0;
        m_update_output_port_event[emRSLV_FUNC_BUSY].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
        m_output_port_value[emRSLV_FUNC_WAIT] = 0x1;
        m_update_output_port_event[emRSLV_FUNC_WAIT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    } else { // emANG_IDLE     
        m_output_port_value[emRSLV_FUNC_WAIT] = 0x0;
        m_update_output_port_event[emRSLV_FUNC_WAIT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
        m_output_port_value[emRSLV_FUNC_BUSY] = 0x0;
        m_update_output_port_event[emRSLV_FUNC_BUSY].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    }
}//}}}

// Description: Update buffer register
// Ref: {HWCORE_RSLVIDE_UD_Flow_016, HWCORE_RSLVIDE_UD_Direction_048}
void HWCORE_RSLVIDE::SetBuffRegister(void)
{//{{{
    m_angofs_buff_value   = mpHWCORE_RSLVIDEFunc->GetRegVal("ANGOFS") & 0xFFFF;
    m_thtresfix_pre_value = mpHWCORE_RSLVIDEFunc->GetRegVal("THTRESFIX") & 0xFFFF;
    m_rescnt_pre_value    = mpHWCORE_RSLVIDEFunc->GetRegVal("RESCNT") & 0xFF;
    mpHWCORE_RSLVIDEFunc->SetRegVal("THTRESFIX", (unsigned int)m_resolver_ang_curr_value);
    m_resolver_ang_pre_value = mpHWCORE_RSLVIDEFunc->GetRegVal("RESTHTORG")& 0xFFFF;
    mpHWCORE_RSLVIDEFunc->SetRegVal("RESTHTORG", (unsigned int)m_resolver_ang_curr_value);
    
    m_output_port_value[emRDATA_EM2ITHTRESFIX] = m_resolver_ang_curr_value;
    m_update_output_port_event[emRDATA_EM2ITHTRESFIX].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    // Dump statistics info
    if(mEnableStatInfo != false) {
        printf("Information of buffers (start):\n");
        DumpStatInfo(this->basename(), "THTRESFIX previous value", m_thtresfix_pre_value);
        DumpStatInfo(this->basename(), "THTEFIX previous value", m_thtefix_pre_value);
        DumpStatInfo(this->basename(), "ANGOFS buffer value", m_angofs_buff_value);
    }
}//}}} 

// Description: common angle function
// Ref: {HWCORE_RSLVIDE_UD_Flow_017, HWCORE_RSLVIDE_UD_Feature_004, HWCORE_RSLVIDE_UD_Feature_005, HWCORE_RSLVIDE_UD_Feature_006, HWCORE_RSLVIDE_UD_Feature_007}
void HWCORE_RSLVIDE::GeneralAngleFunc (unsigned char ANGLE_FUNC)
{//{{{
    if (ModelinResetStateorZeroClock() != false) return;
    switch (ANGLE_FUNC) {
        case emANG00:
        // No operation
        break;
        
        case emANG01:
        // Offset Addition
        OffsetAddition();
        break;
        
        case emANG02:
        // Electrical Angle Calculation
        ElectricalAngleCalculation();
        break;
        
        default: //emANG03
        // Angle Compare Match Determination
        AngleCompareMatch();
        break;
    }
    // Common operation
    CheckNextOperation(ANGLE_FUNC);
    m_output_port_value[emRSLV_FUNC_ST] =  1 << ANGLE_FUNC;
    m_update_output_port_event[emRSLV_FUNC_ST].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
}//}}}

// Description: Offset Addition
// Ref: {HWCORE_RSLVIDE_UD_Flow_018}
void HWCORE_RSLVIDE::OffsetAddition(void)
{//{{{
    unsigned short RESTHTORG = mpHWCORE_RSLVIDEFunc->GetRegVal("RESTHTORG") & 0xFFFF;
    unsigned short ANGOFS = mpHWCORE_RSLVIDEFunc->GetRegVal("ANGOFS") & 0xFFFF;
    unsigned short THTRESFIX = (RESTHTORG + ANGOFS) & 0x0FFF;
    
    mpHWCORE_RSLVIDEFunc->SetRegVal("THTRESFIX", THTRESFIX);
    m_output_port_value[emRDATA_EM2ITHTRESFIX] = THTRESFIX;
    m_update_output_port_event[emRDATA_EM2ITHTRESFIX].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    // Dump statistics info
    if(mEnableStatInfo != false) {
        DumpStatInfo("ang1", "THTRESFIX", THTRESFIX);
    }
}//}}}

// Description: Electrical Angle Calculation
// Ref: {HWCORE_RSLVIDE_UD_Flow_019}
void HWCORE_RSLVIDE::ElectricalAngleCalculation(void)
{//{{{
    // Preprocessing
    unsigned short rthti      = mpHWCORE_RSLVIDEFunc->GetRegVal("THTRESFIX") & 0xFFFF;  //THTRESFIX current value            
    unsigned short rthti_old  = m_thtresfix_pre_value;                         //THTRESFIX [previous value]
    unsigned char  rescnti    = m_rescnt_pre_value;                            //Resolver angle cycle counter value [previous value] 
    unsigned short ethti_old  = m_thtefix_pre_value;                           //Electrical angle value [previous value] 
    unsigned short  ethto     = 0x0;
    unsigned short  ethto_old = 0x0;
    unsigned char   rescnto   = 0x0;
    // Main operation
    ang2(rthti, rthti_old, rescnti, ethti_old, &ethto, &ethto_old, &rescnto);
    // Post processing
    m_thtresfix_pre_value = rthti;
    m_thtefix_pre_value   = ethto_old;
    m_rescnt_pre_value    = rescnto;
    mpHWCORE_RSLVIDEFunc->SetRegVal("THTEFIX", ethto);
    mpHWCORE_RSLVIDEFunc->SetRegVal("RESCNT", rescnto);
    // Update output port when register value is changed
    m_output_port_value[emRDATA_EM2ITHTEFIX] = ethto;
    m_update_output_port_event[emRDATA_EM2ITHTEFIX].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    // Dump statistics info
    if(mEnableStatInfo != false) {
        DumpStatInfo("ang2", "THTRESFIX previous value", rthti);
        DumpStatInfo("ang2", "THTEFIX", ethto);
        DumpStatInfo("ang2", "THTEFIX previous value", ethto_old);
    }
}//}}}

// Description: Angle Compare Match Determination
// Ref: {HWCORE_RSLVIDE_UD_Flow_021}
void HWCORE_RSLVIDE::AngleCompareMatch(void)
{//{{{
    // Preprocessing
    unsigned short ethti     = mpHWCORE_RSLVIDEFunc->GetRegVal("THTEFIX") & 0xFFFF;  //THTRESFIX current value
    unsigned short ethti_old = m_thtefix_pre_value;
    unsigned short rthti     = mpHWCORE_RSLVIDEFunc->GetRegVal("RESTHTORG") & 0xFFFF;
    unsigned short rthti_old = m_resolver_ang_pre_value;
    unsigned short comp0i    = 0;
    unsigned short comp1i    = 0;
    bool comp0_match2        = false;
    bool recctr_slctcmp0 = em2ibrecctr_slctcmp0_reg.read();
    if((recctr_slctcmp0 == false) || (m_ang3_comp0match_check_flag == false)) { //EMU3nRECCTR.SLCTCMP0 or first round of comparison 
        comp0i = em2icmp0.read();
    } else {
        comp0i = rdata_em2iipcmp0.read();
    }
    comp1i = em2icmp1.read();
    if((recctr_slctcmp0 == false) && (m_wc_em2icmp0_value == true)) {
        m_ang3_comp0_match_flag = false;
    } else if ((recctr_slctcmp0 == true) && (m_output_rctg_end_sp_reg_value == true)) {
        m_ang3_comp0_match_flag = false;
    } else if(m_comp0_match1 == true) {
        m_ang3_comp0_match_flag = true;
        m_comp0_match1 = false;
    }
    if(m_wc_em2icmp1_value == true) {
        m_ang3_comp1_match_flag = false;
    } else if (m_comp1_match == true) {
        m_ang3_comp1_match_flag = true;
        m_comp1_match = false;
    }
    // Main operation
    // ------------------------------------------------------------
    // Angle comparison 0 match determination processing
    ang3_comp0 ( ethti, ethti_old, comp0i, m_ang3_comp0_match_flag, &m_comp0_match1, &comp0_match2);
    // ------------------------------------------------------------
    // Angle comparison 1 match determination processing
    ang3_comp1 ( rthti, rthti_old, comp1i, m_ang3_comp1_match_flag, &m_comp1_match);
    // Post processing
    if (( m_comp0_match1 == true) || (comp0_match2 == true)) {
        m_ang3_comp0match_check_flag = true;
        if ( m_comp0_match1 == true) {
            m_output_port_value[emPRESML_ANG_COMP0_EQ_SP] = true;
            m_update_output_port_event[emPRESML_ANG_COMP0_EQ_SP].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
        }
    }
    if (m_comp1_match == true) {
        m_output_port_value[emANG_COMP1_EQ_SP] = true;
        m_update_output_port_event[emANG_COMP1_EQ_SP].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    }
    // Reset variable
    m_wc_em2icmp0_value = false;
    m_wc_em2icmp1_value = false;
    m_output_rctg_end_sp_reg_value = false;
}//}}}  
  
// Description: Angle Compare match 0 function
// Ref: {HWCORE_RSLVIDE_UD_Flow_022}
void HWCORE_RSLVIDE::ang3_comp0 (unsigned short ethti, unsigned short ethti_old, unsigned short comp0i, bool comp0_match_flag, bool *comp0_match1, bool *comp0_match2 )
{//{{{
    assert(comp0_match1 != NULL);
    assert(comp0_match2 != NULL);
    
    bool max2min, min2max;
    bool comp0_match;
    unsigned short CPJUD0 = mpHWCORE_RSLVIDEFunc->GetRegVal("CPJUD0") & 0xFFFF;
    // ------------------------------------------------------------
    // Additional conditions for judging a match in comparison
    // Note: ethti_old holds the ethti value from the previous time ang2 was executed. 
    // Angle comparison 0 judgment 1 HFFF -> H0
    if( ((0xFFF - CPJUD0) < ethti_old) && (ethti < CPJUD0)) {
        max2min = true;
    } else {
        max2min = false;
    }
    // Angle comparison 0 judgment 2 H0 -> HFFF
    if( ((0xFFF - CPJUD0) < ethti) && (ethti_old < CPJUD0)) {
        min2max = true;
    } else {
        min2max = false;
    }
    // ------------------------------------------------------------
    // Angle comparison 0 match detected 
    comp0_match = false;

    if((max2min == false) && (min2max == false)){                //Judgment 1 = not met AND Judgment 2 = not met
        if ((ethti_old <  comp0i) && (comp0i <= ethti)) {       //Positive rotation
          comp0_match = true;
        } else if (ethti <= comp0i && comp0i < ethti_old) {     //Negative rotation
          comp0_match = true;
        }
    } else if (max2min == true) { //Judgment 1 = met
        if ((ethti_old <  comp0i) || (comp0i <= ethti)) {       //Positive rotation
          comp0_match = true;
        }
    } else { //Judgment 2 = met, if (min2max == true)
        if ((ethti <= comp0i) || (comp0i <  ethti_old)) {       //Negative rotation
            comp0_match = true;
        } else {
          comp0_match = false;
        }
   }
   
    // Angle comparison 0 match detected (with flag control)   
    if(comp0_match_flag == false) {
        *comp0_match1 = comp0_match;
    } else {
        *comp0_match1 = false;
    }
    // Angle comparison 0 match detected (without flag control)    
    *comp0_match2 = comp0_match;
    
}//}}}

// Description: Angle Compare match 1 function
// Ref: {HWCORE_RSLVIDE_UD_Flow_023}
void HWCORE_RSLVIDE::ang3_comp1 (unsigned short rthti, unsigned short rthti_old, unsigned short comp1i, bool comp1_match_flag, bool *comp1_match )
{//{{{
    assert (comp1_match != NULL);
    bool max2min, min2max;
    unsigned short CPJUD1 = mpHWCORE_RSLVIDEFunc->GetRegVal("CPJUD1") & 0xFFFF;
    // ------------------------------------------------------------
    // Additional conditions for judging a match in comparison
    // Note: rthti_old holds the rthti value from the previous time processing by the angle generation IP was executed. 
    // Angle comparison 1 judgment 1 HFFF -> H0
    
    if(((0x0FFF - CPJUD1) < rthti_old) && (rthti < CPJUD1 )) {
        max2min = true;
    } else {
        max2min = false;
    }
    // Angle comparison 1 judgment 2 H0 -> HFFF
    if(((0x0FFF - CPJUD1) < rthti) && (rthti_old < CPJUD1)) {
        min2max = true;
    } else {
        min2max = false;
    }
    // Angle comparison 1 match detected
    *comp1_match = false;
    
    if(comp1_match_flag == false){
        if(max2min == false && min2max == false){           //Judgment 1 = not met AND Judgment 2 = not met
            if (rthti_old < comp1i && comp1i <= rthti ) {   //Positive rotation
                *comp1_match = true;
            } else if (rthti <= comp1i && comp1i < rthti_old) { //Negative rotation
                *comp1_match = true;
            }
        } else if (max2min == true) { //Judgment 1 = met
            if (rthti_old < comp1i || comp1i <= rthti ) {   //Positive rotation
                *comp1_match = true;
            }
        } else { //Judgment 2 = met. if (min2max == true) 
            if (rthti <= comp1i || comp1i < rthti_old) {  //Negative rotation
                *comp1_match = true;
            }
        }
    } 
}//}}}

// Description: Electrical Angle Calculation(ang2) Operation
// Ref: {HWCORE_RSLVIDE_UD_Flow_019, HWCORE_RSLVIDE_UD_Flow_020}
void HWCORE_RSLVIDE::ang2(unsigned short rthti, unsigned short rthti_old, unsigned char rescnti, unsigned short ethti_old, unsigned short *ethto, unsigned short *ethto_old, unsigned char  *rescnto)
{//{{{

    assert(ethto != NULL);
    assert(ethto_old != NULL);
    assert(rescnto != NULL);
    
    //Note: The input value ethti_old is the value of ethto from the previous time ang2 was executed.
    //      The value of ethti_old is conveyed to ethto_old after execution of ang2.
    *ethto_old = ethti_old;
    //Note: rthti_old holds the rthti value from the previous time the angle generation IP was executed.
    unsigned char RESRLD = mpHWCORE_RSLVIDEFunc->GetRegVal("RESRLD") & 0x7;
    unsigned short PXR = mpHWCORE_RSLVIDEFunc->GetRegVal("PXR") & 0xFFFF;
    bool max2min, min2max;
    signed char rescnt;
    unsigned char tmp_rescnto;
    unsigned short tmp_tht;

    if((0xF00 < rthti_old) && (rthti < 0x0FF )) {
        max2min = true;
    } else {
        max2min = false;
    }
    if((0xF00 < rthti) && (rthti_old < 0x0FF )) {
        min2max = true;
    } else {
        min2max = false;
    }

    if(max2min == true) {
        rescnt = rescnti + 1;
    } else if(min2max == true) {
        rescnt = rescnti - 1;
    } else {
        rescnt = rescnti;
    }
    if( rescnt > RESRLD) {
        tmp_rescnto = 0;
    } else if (rescnt < 0) {
        tmp_rescnto = RESRLD;
    } else {
        tmp_rescnto = rescnt & 0x7;
    }

    *rescnto = tmp_rescnto;
    
    tmp_tht = (unsigned short)((unsigned short)tmp_rescnto << 12);
    tmp_tht |= rthti;

    *ethto = ( (tmp_tht * PXR) >> 8 ) & 0x0FFF ;  

}//}}}

// Description: Check Next ANGLE Operation
// Ref: {HWCORE_RSLVIDE_UD_Flow_024}
void HWCORE_RSLVIDE::CheckNextOperation(unsigned char ANGLE_FUNC)
{//{{{
    unsigned int ANG = FUNCFLGRPB.read() & 0x3FFFF;
    unsigned char next_state = (ANG >> (ANGLE_FUNC*2)) & 0x3;

    if (next_state == 0x0) {
        if (ANGLE_FUNC != emANG03) {
            SetAngleState(emANG_BUSY);
            m_ang_start_event[ANGLE_FUNC+1].notify((double)(m_ang_func_cyc[ANGLE_FUNC+1]* mPCLKPeriod), mTimeResolutionUnit);
        } else {
            SetAngleState(emANG_IDLE);
            m_ang_ip_cyc_num = 0;
        }        
    } else if (next_state == 0x1) {
        SetAngleState(emANG_IDLE);
        m_output_port_value[emRSLV_FUNC_END] = 1 << ANGLE_FUNC;
        m_update_output_port_event[emRSLV_FUNC_END].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
        m_ang_ip_cyc_num = 0;
    } else {
        SetAngleState(emANG_WAIT);
        m_output_port_value[emRSLV_FUNC_END] = 1 << ANGLE_FUNC;
        m_update_output_port_event[emRSLV_FUNC_END].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    }  
}//}}}    

// Description: Handle Output Port
// Ref: {HWCORE_RSLVIDE_UD_Direction_050}
void HWCORE_RSLVIDE::HandleOutputEdgePortMethod(const unsigned int port_id)
{//{{{    

    switch(port_id) {
    case emRSLV_FUNC_ST:
        rslv_func_st.write(m_output_port_value[port_id]);
        //Edge detection type
        if (m_output_port_value[port_id] != 0x0) {
            m_output_port_value[port_id] = 0x0;
            m_update_output_port_event[port_id].notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
        break;
    case emRSLV_FUNC_END:
        rslv_func_end.write(m_output_port_value[port_id]);
        //Edge detection type
        if (m_output_port_value[port_id] != 0x0) {
            m_output_port_value[port_id] = 0x0;
            m_update_output_port_event[port_id].notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
        break;
    case emRSLV_FUNC_WAIT:
        rslv_func_wait.write((bool) m_output_port_value[port_id]);
        break;
        
    case emRSLV_FUNC_BUSY: 
        rslv_func_busy.write((bool) m_output_port_value[port_id]);
        break;
        
    case emANGCTR:
        ANGCTR.write((unsigned char)m_output_port_value[port_id]);
        break;
        
    case emRESTHSFT:
        RESTHSFT.write(m_output_port_value[port_id]);
        break;
        
    case emRDATA_EM2ITHTRESFIX: 
        rdata_em2ithtresfix.write(m_output_port_value[port_id]);
        break;
        
    case emRDATA_EM2ITHTEFIX:
        rdata_em2ithtefix.write(m_output_port_value[port_id]);
        break;
        
    case emPRESML_ANG_COMP0_EQ_SP:
        if ((mDumpInterrupt != false) && (presml_ang_comp0_eq_sp.read() != (bool)m_output_port_value[port_id])) {
            re_printf("info","[%s] is changed to %d.\n","presml_ang_comp0_eq_sp", m_output_port_value[port_id]);
        }
        presml_ang_comp0_eq_sp.write((bool) m_output_port_value[port_id]);
        //Edge detection type
        if (m_output_port_value[port_id] == 0x1) {
            m_output_port_value[port_id] = 0x0;
            m_update_output_port_event[port_id].notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
        break;
        
    default:// emANG_COMP1_EQ_SP:  
        if ((mDumpInterrupt != false) && (ang_comp1_eq_sp.read() != (bool)m_output_port_value[port_id])) {
            re_printf("info","[%s] is changed to %d.\n","ang_comp1_eq_sp", m_output_port_value[port_id]);
        }
        ang_comp1_eq_sp.write((bool) m_output_port_value[port_id]);
        //Edge detection type
        if (m_output_port_value[port_id] == 0x1) {
            m_output_port_value[port_id] = 0x0;
            m_update_output_port_event[port_id].notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
        break;
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
// Description: Check clock period
// Ref: {HWCORE_RSLVIDE_UD_Direction_039}
bool HWCORE_RSLVIDE::CheckClockPeriod()
{//{{{
    bool clock_status = false;
    if (mPCLKFreq > 0) {
        clock_status = true;
    }else{
        re_printf("info", "PCLK frequency is zero\n");
    }
    return clock_status;
}//}}}

// Description: Get reset status
bool HWCORE_RSLVIDE::GetResetStatus()
{//{{{
    return (mIsResetHardActive[emPRESETnId] || mIsResetCmdActive[emPRESETnId] || mIsResetHardActive[emMip_prst_nId] || mIsResetCmdActive[emMip_prst_nId]);
}//}}}

// Description: Update ANGCTR Port
// Ref: {HWCORE_RSLVIDE_UD_Direction_039}
void HWCORE_RSLVIDE::UpdateANGCTRPort(unsigned char value)
{//{{{
    m_output_port_value[emANGCTR] = value;
    m_update_output_port_event[emANGCTR].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
}//}}}

// Description: Update RESTHSFT Port
// Ref: {HWCORE_RSLVIDE_UD_Direction_039}
void HWCORE_RSLVIDE::UpdateRESTHSFTPort(unsigned short value)
{//{{{
    m_output_port_value[emRESTHSFT] = value;
    m_update_output_port_event[emRESTHSFT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
}//}}}

// Description: Update THTEFIX Port
// Ref: {HWCORE_RSLVIDE_UD_Direction_039}
void HWCORE_RSLVIDE::UpdateTHTEFIXPort(unsigned short value)
{//{{{
    m_output_port_value[emRDATA_EM2ITHTEFIX] = value;
    m_update_output_port_event[emRDATA_EM2ITHTEFIX].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
}//}}}

// Description: Update THTRESFIX Port
// Ref: {HWCORE_RSLVIDE_UD_Direction_039}
void HWCORE_RSLVIDE::UpdateTHTRESFIXPort(unsigned short value)
{//{{{
    m_output_port_value[emRDATA_EM2ITHTRESFIX] = value;
    m_update_output_port_event[emRDATA_EM2ITHTRESFIX].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
}//}}}

// Description: Get previous resolver angle value
unsigned short HWCORE_RSLVIDE::GetResolverAngleValue()
{
    return m_resolver_ang_curr_value;
}

// Description: Update internal variable value
void HWCORE_RSLVIDE::UpdateVariableValue(unsigned char var, unsigned short value)
{//{{{
    if (var == emRESOLVER_CURR_VALUE){
        m_resolver_ang_curr_value = value;
    } else {
        m_thtefix_pre_value  = value;
    }
}//}}}

// vim600: set foldmethod=marker :
