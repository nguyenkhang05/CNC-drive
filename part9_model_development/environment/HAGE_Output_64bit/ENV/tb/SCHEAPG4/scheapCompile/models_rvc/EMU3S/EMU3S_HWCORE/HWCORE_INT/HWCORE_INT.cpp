// ---------------------------------------------------------------------
// $Id: HWCORE_INT.cpp,v 1.3 2020/11/17 04:52:39 huyquocpham Exp $
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
// Description: Copyright of HWCORE_INT.cpp
// Ref: {EMU3S_HWCORE_UD_Copyright_001}
// Description: About the include header files of HWCORE_INT.cpp
// Ref: {EMU3S_HWCORE_UD_ATT1_001}
#include "re_define.h"
#include "HWCORE_INT.h"
#include "HWCORE_INT_Func.h"
#include "HWCORE_VRFBUF_Func.h"
///*********************************
/// Function   : HWCORE_INT
/// Description: Constructor of HWCORE_INT class
/// Ref: {EMU3S_HWCORE_UD_BlockDiagram_004}
///*********************************
HWCORE_INT::HWCORE_INT(sc_module_name name)
    :sc_module(name)
    , HWCORE_INT_AgentController()
    , CRWInterface()
    , rvc_common_model()
    , PCLK("PCLK")
    , PRESETn("PRESETn")
    , funca_wait_int("funca_wait_int")
    , funcb_wait_int("funcb_wait_int")
    , iir_end0("iir_end0")
    , iir_end1("iir_end1")
    , iir_end2("iir_end2")
    , iir_ovfl_in("iir_ovfl_in")
    , iir_ovfl_func("iir_ovfl_func")
    , input_end_sp_reg("input_end_sp_reg")
    , kcl_int("kcl_int")
    , in_ovfl_in("in_ovfl_in")
    , in_ovfl_func("in_ovfl_func")
    , in_zdfl_in("in_zdfl_in")
    , ia_1ch_lo_org_sel("ia_1ch_lo_org_sel")
    , ia_2ch_lo_org_sel("ia_2ch_lo_org_sel")
    , ia_3ch_lo_org_sel("ia_3ch_lo_org_sel")
    , u16or_res_angle("u16or_res_angle")
    , s32or_id_lo("s32or_id_lo")
    , s32or_iq_lo("s32or_iq_lo")
    , pi_end_sp_reg("pi_end_sp_reg")
    , pi_ovfl_in("pi_ovfl_in")
    , pi_ovfl_func("pi_ovfl_func")
    , output_pwm_end_sp_reg("output_pwm_end_sp_reg")
    , tstwdcmpu("tstwdcmpu")
    , tstwdcmpv("tstwdcmpv")
    , tstwdcmpw("tstwdcmpw")
    , pwm_ovfl_in("pwm_ovfl_in")
    , pwm_ovfl_func("pwm_ovfl_func")
    , pwm_zdfl_in("pwm_zdfl_in")
    , output_rctg_end_sp_reg("output_rctg_end_sp_reg")
    , rctg_u_80m_reg("rctg_u_80m_reg")
    , rctg_v_80m_reg("rctg_v_80m_reg")
    , rctg_w_80m_reg("rctg_w_80m_reg")
    , ang_comp0_eq_sp("ang_comp0_eq_sp")
    , ang_comp1_eq_sp("ang_comp1_eq_sp")
    , irctg3_err("irctg3_err")
    , irctg3_u_int("irctg3_u_int")
    , irctg3_v_int("irctg3_v_int")
    , irctg3_w_int("irctg3_w_int")
    , irctg3_ucmp_trig("irctg3_ucmp_trig")
    , irctg3_vcmp_trig("irctg3_vcmp_trig")
    , irctg3_wcmp_trig("irctg3_wcmp_trig")
    , irctg3_upatout_reg("irctg3_upatout_reg")
    , irctg3_vpatout_reg("irctg3_vpatout_reg")
    , irctg3_wpatout_reg("irctg3_wpatout_reg")
    , tstpektim("tstpektim")
    , tstvlytim("tstvlytim")
    , adc_trig("adc_trig")
    , ins_trig("ins_trig")
    , adtrg_mvint0("adtrg_mvint0")
    , adtrg_mvint1("adtrg_mvint1")
    , adtrg_mem_rg_chg("adtrg_mem_rg_chg")
    , adtrg_smerr("adtrg_smerr")
    , adtrg_amerr("adtrg_amerr")
    , pmt2of("pmt2of")
    , pmt2cp("pmt2cp")
{//{{{

    
    mpHWCORE_INTFunc = new HWCORE_INT_Func((std::string)name, this);
    mpHWCORE_VRFBUFFunc = new HWCORE_VRFBUF_Func((std::string)name, this);
    sc_assert(mpHWCORE_INTFunc != NULL);
    sc_assert(mpHWCORE_VRFBUFFunc != NULL);

    // Initialize variables
    mPCLKPeriod =           0;
    mPCLKOrgFreq =          0;
    mPCLKFreq =             0;
    mPCLKFreqUnit =         "Hz";
    
    // Initialize reset flags
    mResetCurVal[emPRESETnId] = true;
    mResetPreVal[emPRESETnId] = true;
    mIsResetHardActive[emPRESETnId] = false;

    mResetCurVal[emrst_nId] = false;
    mResetPreVal[emrst_nId] = false;
    mIsResetHardActive[emrst_nId] = true;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] = 0;
    }
    
    // Initialize time variables
    mTimeResolutionValue =      1;
    mTimeResolutionUnit =   SC_NS;
    
    // Initialize Python parameters
    mEnableRegisterMessage = true;
    mDumpInterrupt         = false;
    mTimePCLKChangePoint   = 0;
    mINT_cyc = 0x1;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    // Initialize output port
    for (unsigned int i = 0; i < emINT_NUM; i++)
    {
        std::ostringstream port_name;
        port_name.str("");
        port_name << "emu_int_" << i;
        emu_int[i] = new sc_out<bool>(port_name.str().c_str());
        emu_int[i]->initialize(false);
    }    
    // Initialize internal variables
    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(HandlePCLKSignalMethod);
    sensitive << PCLK;

    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;

    SC_METHOD(Handlerst_nSignalMethod);
    sensitive << rst_n;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_INT::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_INT::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_INT::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(ResetPortsMethod);
    dont_initialize();
    sensitive << mResetPortsEvent;
    
    // Monitor Interrupt Sources
    SC_METHOD(MonitorInputEndSPMethod);
    dont_initialize();
    sensitive << input_end_sp_reg;

    SC_METHOD(MonitorPIEndSPMethod);
    dont_initialize();
    sensitive << pi_end_sp_reg;

    SC_METHOD(MonitorPWMEndSPMethod);
    dont_initialize();
    sensitive << output_pwm_end_sp_reg;

    SC_METHOD(MonitorRCTGEndSPMethod);
    dont_initialize();
    sensitive << output_rctg_end_sp_reg;

    SC_METHOD(MonitorTstvlytimMethod);
    dont_initialize();
    sensitive << tstvlytim;

    SC_METHOD(MonitorTstpektimMethod);
    dont_initialize();
    sensitive << tstpektim;

    SC_METHOD(MonitorAngCmp0SPMethod);
    dont_initialize();
    sensitive << ang_comp0_eq_sp;

    SC_METHOD(MonitorAngCmp1SPMethod);
    dont_initialize();
    sensitive << ang_comp1_eq_sp;

    SC_METHOD(MonitorInsTrigMethod);
    dont_initialize();
    sensitive << ins_trig;

    SC_METHOD(MonitorPmt2CpMethod);
    dont_initialize();
    sensitive << pmt2cp;

    SC_METHOD(MonitorADCTrigMethod);
    dont_initialize();
    sensitive << adc_trig;

    SC_METHOD(MonitorADTRGMvint0Method);
    dont_initialize();
    sensitive << adtrg_mvint0;

    SC_METHOD(MonitorADTRGMvint1Method);
    dont_initialize();
    sensitive << adtrg_mvint1;

    SC_METHOD(MonitorADTRGMemMethod);
    dont_initialize();
    sensitive << adtrg_mem_rg_chg;

    SC_METHOD(MonitorIIREnd0Method);
    dont_initialize();
    sensitive << iir_end0;

    SC_METHOD(MonitorIIREnd1Method);
    dont_initialize();
    sensitive << iir_end1;

    SC_METHOD(MonitorIIREnd2Method);
    dont_initialize();
    sensitive << iir_end2;

    SC_METHOD(MonitorIRCTG3UIntMethod);
    dont_initialize();
    sensitive << irctg3_u_int;

    SC_METHOD(MonitorIRCTG3VIntMethod);
    dont_initialize();
    sensitive << irctg3_v_int;

    SC_METHOD(MonitorIRCTG3WIntMethod);
    dont_initialize();
    sensitive << irctg3_w_int;

    SC_METHOD(MonitorFuncAWaitMethod);
    dont_initialize();
    sensitive << funca_wait_int;

    SC_METHOD(MonitorFuncBWaitMethod);
    dont_initialize();
    sensitive << funcb_wait_int;

    SC_METHOD(MonitorIRCTG3UCmpMethod);
    dont_initialize();
    sensitive << irctg3_ucmp_trig;

    SC_METHOD(MonitorIRCTG3VCmpMethod);
    dont_initialize();
    sensitive << irctg3_vcmp_trig;

    SC_METHOD(MonitorIRCTG3WCmpMethod);
    dont_initialize();
    sensitive << irctg3_wcmp_trig;


    // Monitor Error Sources
    SC_METHOD(MonitorINPUTOvfMethod);
    dont_initialize();
    sensitive << in_ovfl_in;

    SC_METHOD(MonitorIIROvfMethod);
    dont_initialize();
    sensitive << iir_ovfl_in;

    SC_METHOD(MonitorPIOvfMethod);
    dont_initialize();
    sensitive << pi_ovfl_in;

    SC_METHOD(MonitorPWMOvfMethod);
    dont_initialize();
    sensitive << pwm_ovfl_in;

    SC_METHOD(MonitorINPUTDivByZeroMethod);
    dont_initialize();
    sensitive << in_zdfl_in;

    SC_METHOD(MonitorPWMDivByZeroMethod);
    dont_initialize();
    sensitive << pwm_zdfl_in;

    SC_METHOD(MonitorTimerOvfMethod);
    dont_initialize();
    sensitive << pmt2of;

    SC_METHOD(MonitorKirLawViolateMethod);
    dont_initialize();
    sensitive << kcl_int;

    SC_METHOD(MonitorOverlapStartADMethod);
    dont_initialize();
    sensitive << adtrg_amerr;

    SC_METHOD(MonitorOverlapWrADSamplMethod);
    dont_initialize();
    sensitive << adtrg_smerr;

    SC_METHOD(MonitorIRCTG3ErrorMethod);
    dont_initialize();
    sensitive << irctg3_err;

    // Assert/Deassert Interrupt 
    for (unsigned int int_id = 0; int_id < emINT_NUM; int_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertEmuIntEvent[int_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_INT::AssertEmuIntMethod, this, int_id),
            sc_core::sc_gen_unique_name("AssertEmuIntMethod"), &opt);
    }

    for (unsigned int int_id = 0; int_id < emINT_NUM; int_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeassertEmuIntEvent[int_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_INT::DeassertEmuIntMethod, this, int_id),
            sc_core::sc_gen_unique_name("DeassertEmuIntMethod"), &opt);
    }

    for (unsigned int int_id = 0; int_id < emINT_NUM; int_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteEmuIntEvent[int_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_INT::WriteEmuIntMethod, this, int_id),
            sc_core::sc_gen_unique_name("WriteEmuIntMethod"), &opt);
    }

    // Trigger Buffer Registers
    SC_METHOD(TriggerBufferRegisters);
    dont_initialize();
    sensitive << mBufferRegistersEvent;
}//}}}

///*********************************
/// Function     : ~HWCORE_INT
/// Description  : Destructor
///*********************************
HWCORE_INT::~HWCORE_INT()
{//{{{
    delete mpHWCORE_INTFunc;
    delete mpHWCORE_VRFBUFFunc;
}//}}}

// Define read APIs
// Ref: {}
void HWCORE_INT::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    assert(data_ptr != NULL);
    if (offsetAddress >= 0x780)
    {
        mpHWCORE_VRFBUFFunc->read(offsetAddress, data_ptr, size, debug);
    }
    else
    {
        mpHWCORE_INTFunc->read(offsetAddress, data_ptr, size, debug);
    }
}//}}}

// Define write APIs
// Ref: {}
void HWCORE_INT::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    assert(data_ptr != NULL);
    if (offsetAddress >= 0x780)
    {
        mpHWCORE_VRFBUFFunc->write(offsetAddress, data_ptr, size, debug);
    }
    else
    {
        mpHWCORE_INTFunc->write(offsetAddress, data_ptr, size, debug);
    }  
}//}}}

//======================================================================
//===================== Monitor Interrupt Sources  =====================
//======================================================================
///********************************************************
/// Function     : MonitorInputEndSPMethod 
/// Description  : Monitor input_end_sp_reg
/// Ref: {EMU3S_HWCORE_UD_Flow_057, EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorInputEndSPMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = input_end_sp_reg.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "INIF", 1);
            JudgeInterruptOutput("ININT");
            JudgeBufferRegisters("INBT");
        }
    }
}
///********************************************************
/// Function     : MonitorPIEndSPMethod 
/// Description  : Monitor pi_end_sp_reg
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorPIEndSPMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = pi_end_sp_reg.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "PIIF", 1);
            JudgeInterruptOutput("PIINT");
        }
    }
}
///********************************************************
/// Function     : MonitorPWMEndSPMethod 
/// Description  : Monitor output_pwm_end_sp_reg
/// Ref: {EMU3S_HWCORE_UD_Flow_057, EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorPWMEndSPMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = output_pwm_end_sp_reg.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "PWMIF", 1);
            JudgeInterruptOutput("PWMINT");
            JudgeBufferRegisters("PWMBT");
        }
    }
}
///********************************************************
/// Function     : MonitorRCTGEndSPMethod 
/// Description  : Monitor output_rctg_end_sp_reg
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorRCTGEndSPMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = output_rctg_end_sp_reg.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "RECIF", 1);
            JudgeInterruptOutput("RECINT");
        }
    }
}
///********************************************************
/// Function     : MonitorTstvlytimMethod 
/// Description  : Monitor tstvlytim
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorTstvlytimMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = tstvlytim.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "CARVIF", 1);
            JudgeInterruptOutput("CARVINT");
        }
    }
}
///********************************************************
/// Function     : MonitorTstpektimMethod 
/// Description  : Monitor tstpektim
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorTstpektimMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = tstpektim.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "CARMIF", 1);
            JudgeInterruptOutput("CARMINT");
        }
    }
}
///********************************************************
/// Function     : MonitorAngCmp0SPMethod 
/// Description  : Monitor ang_comp0_eq_sp
/// Ref: {EMU3S_HWCORE_UD_Flow_057, EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorAngCmp0SPMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = ang_comp0_eq_sp.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "CMP0IF", 1);
            JudgeInterruptOutput("CMP0INT");
            JudgeBufferRegisters("CMP0BT");
        }
    }
}
///********************************************************
/// Function     : MonitorAngCmp1SPMethod 
/// Description  : Monitor ang_comp1_eq_sp
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorAngCmp1SPMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = ang_comp1_eq_sp.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "CMP1IF", 1);
            JudgeInterruptOutput("CMP1INT");
        }
    }
}
///********************************************************
/// Function     : MonitorInsTrigMethod 
/// Description  : Monitor ins_trig
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorInsTrigMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = ins_trig.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "INSIF", 1);
            JudgeInterruptOutput("INSINT");
        }
    }
}
///********************************************************
/// Function     : MonitorPmt2CpMethod 
/// Description  : Monitor pmt2cp
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorPmt2CpMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = pmt2cp.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "PMT2CPIF", 1);
            JudgeInterruptOutput("PMT2CPINT");
        }
    }
}
///********************************************************
/// Function     : MonitorADCTrigMethod 
/// Description  : Monitor adc_trig
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorADCTrigMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = adc_trig.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "ADIF", 1);
            JudgeInterruptOutput("ADINT");
        }
    }
}
///********************************************************
/// Function     : MonitorADTRGMvint0Method 
/// Description  : Monitor adtrg_mvint0
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorADTRGMvint0Method(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = adtrg_mvint0.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "VADIF", 1);
            JudgeInterruptOutput("VADINT");
        }
    }
}
///********************************************************
/// Function     : MonitorADTRGMvint1Method 
/// Description  : Monitor adtrg_mvint1
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorADTRGMvint1Method(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = adtrg_mvint1.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "MADIF", 1);
            JudgeInterruptOutput("MADINT");
        }
    }
}
///********************************************************
/// Function     : MonitorADTRGMemMethod 
/// Description  : Monitor adtrg_mem_rg_chg
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorADTRGMemMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = adtrg_mem_rg_chg.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "SMBIF", 1);
            JudgeInterruptOutput("SMBINT");
        }
    }
}
///********************************************************
/// Function     : MonitorIIREnd0Method 
/// Description  : Monitor iir_end0
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorIIREnd0Method(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = iir_end0.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "IIR0IF", 1);
            JudgeInterruptOutput("IIR0INT");
        }
    }
}
///********************************************************
/// Function     : MonitorIIREnd1Method 
/// Description  : Monitor iir_end1
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorIIREnd1Method(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = iir_end1.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "IIR1IF", 1);
            JudgeInterruptOutput("IIR1INT");
        }
    }
}
///********************************************************
/// Function     : MonitorIIREnd2Method 
/// Description  : Monitor iir_end2
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorIIREnd2Method(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = iir_end2.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "IIR2IF", 1);
            JudgeInterruptOutput("IIR2INT");
        }
    }
}
///********************************************************
/// Function     : MonitorIRCTG3UCmpMethod 
/// Description  : Monitor irctg3_ucmp_trig
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorIRCTG3UCmpMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = irctg3_ucmp_trig.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "IREC3UIF", 1);
            JudgeInterruptOutput("IREC3UINT");
        }
    }
}
///********************************************************
/// Function     : MonitorIRCTG3VCmpMethod 
/// Description  : Monitor irctg3_vcmp_trig
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorIRCTG3VCmpMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = irctg3_vcmp_trig.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "IREC3VIF", 1);
            JudgeInterruptOutput("IREC3VINT");
        }
    }
}
///********************************************************
/// Function     : MonitorIRCTG3WCmpMethod 
/// Description  : Monitor irctg3_wcmp_trig
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorIRCTG3WCmpMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = irctg3_wcmp_trig.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "IREC3WIF", 1);
            JudgeInterruptOutput("IREC3WINT");
        }
    }
}
///********************************************************
/// Function     : MonitorFuncAWaitMethod 
/// Description  : Monitor funca_wait_int
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorFuncAWaitMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = funca_wait_int.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "FWGAIF", 1);
            JudgeInterruptOutput("FWGAINT");
        }
    }
}
///********************************************************
/// Function     : MonitorFuncBWaitMethod 
/// Description  : Monitor funcb_wait_int
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorFuncBWaitMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = funcb_wait_int.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "FWGBIF", 1);
            JudgeInterruptOutput("FWGBINT");
        }
    }
}
///********************************************************
/// Function     : MonitorIRCTG3UIntMethod 
/// Description  : Monitor irctg3_u_int
/// Ref: {EMU3S_HWCORE_UD_Flow_057}
///********************************************************
void HWCORE_INT::MonitorIRCTG3UIntMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = irctg3_u_int.read();
        if (cur_val)
        {
            JudgeBufferRegisters("IREC3UBT");
        }
    }
}
///********************************************************
/// Function     : MonitorIRCTG3VIntMethod 
/// Description  : Monitor irctg3_v_int
/// Ref: {EMU3S_HWCORE_UD_Flow_057}
///********************************************************
void HWCORE_INT::MonitorIRCTG3VIntMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = irctg3_v_int.read();
        if (cur_val)
        {
            JudgeBufferRegisters("IREC3VBT");
        }
    }
}
///********************************************************
/// Function     : MonitorIRCTG3WIntMethod 
/// Description  : Monitor irctg3_w_int
/// Ref: {EMU3S_HWCORE_UD_Flow_057}
///********************************************************
void HWCORE_INT::MonitorIRCTG3WIntMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = irctg3_w_int.read();
        if (cur_val)
        {
            JudgeBufferRegisters("IREC3WBT");
        }
    }
}

//======================================================================
//=====================    Monitor Error Sources   =====================
//======================================================================
///********************************************************
/// Function     : MonitorINPUTOvfMethod 
/// Description  : Monitor in_ovfl_in
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorINPUTOvfMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = in_ovfl_in.read();
        if (cur_val)
        {
            unsigned int OFMON = mpHWCORE_INTFunc->GetRegVal("OFMON");
            if (OFMON == 0)
            {
                mpHWCORE_INTFunc->SetRegVal("ERRSD", "OVFEF", 1);
                UpdateErrorReg("OFMON", 1, in_ovfl_func.read());
            }
            JudgeErrorOutput("OVFEN");
        }
    }
}
///********************************************************
/// Function     : MonitorIIROvfMethod 
/// Description  : Monitor iir_ovfl_in
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorIIROvfMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = iir_ovfl_in.read();
        if (cur_val)
        {
            unsigned int OFMON = mpHWCORE_INTFunc->GetRegVal("OFMON");
            if (OFMON == 0)
            {
                mpHWCORE_INTFunc->SetRegVal("ERRSD", "OVFEF", 1);
                UpdateErrorReg("OFMON", 4, iir_ovfl_func.read());
            }
            JudgeErrorOutput("OVFEN");
        }
    }
}
///********************************************************
/// Function     : MonitorPIOvfMethod 
/// Description  : Monitor pi_ovfl_in
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorPIOvfMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = pi_ovfl_in.read();
        if (cur_val)
        {
            unsigned int OFMON = mpHWCORE_INTFunc->GetRegVal("OFMON");
            if (OFMON == 0)
            {
                mpHWCORE_INTFunc->SetRegVal("ERRSD", "OVFEF", 1);
                unsigned char ovf_func = 0;
                if (pi_ovfl_func.read())
                {
                    ovf_func = 1;
                }
                UpdateErrorReg("OFMON", 2, ovf_func);
            }
            JudgeErrorOutput("OVFEN");
        }
    }
}
///********************************************************
/// Function     : MonitorPWMOvfMethod 
/// Description  : Monitor pwm_ovfl_in
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorPWMOvfMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = pwm_ovfl_in.read();
        if (cur_val)
        {
            unsigned int OFMON = mpHWCORE_INTFunc->GetRegVal("OFMON");
            if (OFMON == 0)
            {
                mpHWCORE_INTFunc->SetRegVal("ERRSD", "OVFEF", 1);
                UpdateErrorReg("OFMON", 3, pwm_ovfl_func.read());
            }
            JudgeErrorOutput("OVFEN");
        }
    }
}
///********************************************************
/// Function     : MonitorINPUTDivByZeroMethod 
/// Description  : Monitor in_zdfl_in
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorINPUTDivByZeroMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = in_zdfl_in.read();
        if (cur_val)
        {
            unsigned int ZDMON = mpHWCORE_INTFunc->GetRegVal("ZDMON");
            if (ZDMON == 0)
            {
                mpHWCORE_INTFunc->SetRegVal("ERRSD", "ZDEF", 1);
                UpdateErrorReg("ZDMON", 1, in_ovfl_func.read());
            }
            JudgeErrorOutput("ZDEN");
        }
    }
}
///********************************************************
/// Function     : MonitorPWMDivByZeroMethod 
/// Description  : Monitor pwm_zdfl_in
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorPWMDivByZeroMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = pwm_zdfl_in.read();
        if (cur_val)
        {
            unsigned int ZDMON = mpHWCORE_INTFunc->GetRegVal("ZDMON");
            if (ZDMON == 0)
            {
                mpHWCORE_INTFunc->SetRegVal("ERRSD", "ZDEF", 1);
                UpdateErrorReg("ZDMON", 3, pwm_ovfl_func.read());
            }
            JudgeErrorOutput("ZDEN");
        }
    }
}
///********************************************************
/// Function     : MonitorTimerOvfMethod 
/// Description  : Monitor pmt2of
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorTimerOvfMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = pmt2of.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("ERRSD", "PMT2OFEF", 1);
            JudgeErrorOutput("PMT2OFEN");
        }
    }
}
///********************************************************
/// Function     : MonitorKirLawViolateMethod 
/// Description  : Monitor kcl_int
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorKirLawViolateMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = kcl_int.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("ERRSD", "KCLEF", 1);
            JudgeErrorOutput("KCLEN");
        }
    }
}
///********************************************************
/// Function     : MonitorOverlapStartADMethod 
/// Description  : Monitor adtrg_amerr
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorOverlapStartADMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = adtrg_amerr.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("ERRSD", "ADMEF", 1);
            JudgeErrorOutput("ADMEN");
        }
    }
}
///********************************************************
/// Function     : MonitorOverlapWrADSamplMethod 
/// Description  : Monitor adtrg_smerr
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorOverlapWrADSamplMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = adtrg_smerr.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("ERRSD", "SMMEF", 1);
            JudgeErrorOutput("SMMEN");
        }
    }
}
///********************************************************
/// Function     : MonitorIRCTG3ErrorMethod 
/// Description  : Monitor irctg3_err
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::MonitorIRCTG3ErrorMethod(void)
{
    if (!GetResetStatus("rst_n") && !GetResetStatus("PRESETn") && CheckClockPeriod())
    {
        bool cur_val = irctg3_err.read();
        if (cur_val)
        {
            mpHWCORE_INTFunc->SetRegVal("ERRSD", "IR3EEF", 1);
            JudgeErrorOutput("IR3EEN");
        }
    }
}
//======================================================================
//===================== Assert/Deassert Interrupt  =====================
//======================================================================
///********************************************************
/// Function     : JudgeInterruptOutput 
/// Description  : Judge Interrupt Output 
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::JudgeInterruptOutput(std::string enable_bit_name)
{
    for (unsigned int i = 0; i < emINT_NUM; i++)
    {
        std::ostringstream reg_name;
        reg_name.str("");
        reg_name << "INT" << i;
        unsigned int reg_val = mpHWCORE_INTFunc->GetRegVal(reg_name.str().c_str(), enable_bit_name);
        if (reg_val == 1)
        {
            mAssertEmuIntEvent[i].notify();
        }
    }
}
///********************************************************
/// Function     : JudgeErrorOutput 
/// Description  : Judge Error Output 
/// Ref: {EMU3S_HWCORE_UD_Flow_060}
///********************************************************
void HWCORE_INT::JudgeErrorOutput(std::string enable_bit_name)
{
    for (unsigned int i = 0; i < emINT_NUM; i++)
    {
        std::ostringstream err_reg_name;
        err_reg_name.str("");
        err_reg_name << "ERR" << i;
        unsigned int err_reg_val = mpHWCORE_INTFunc->GetRegVal(err_reg_name.str().c_str(), enable_bit_name);
        if (err_reg_val == 1)
        {
            mpHWCORE_INTFunc->SetRegVal("INTSD", "ERRIF", 1);
            std::ostringstream int_reg_name;
            int_reg_name.str("");
            int_reg_name << "INT" << i;
            unsigned int int_reg_val = mpHWCORE_INTFunc->GetRegVal(int_reg_name.str().c_str(), "ERRINT");
            if (int_reg_val == 1)
            {
                mAssertEmuIntEvent[i].notify();
            }
        }
    }
}
///********************************************************
/// Function     : AssertEmuIntMethod 
/// Description  : 
/// Ref: {}
///********************************************************
void HWCORE_INT::AssertEmuIntMethod(const unsigned int int_id)
{
    mIntVal[int_id] = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWriteEmuIntEvent[int_id].notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertEmuIntEvent[int_id].notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}
///********************************************************
/// Function     : DeassertEmuIntMethod 
/// Description  : 
/// Ref: {}
///********************************************************
void HWCORE_INT::DeassertEmuIntMethod(const unsigned int int_id)
{
    mIntVal[int_id] = false;
    mWriteEmuIntEvent[int_id].notify();
}
///********************************************************
/// Function     : WriteEmuIntMethod 
/// Description  : 
/// Ref: {}
///********************************************************
void HWCORE_INT::WriteEmuIntMethod(const unsigned int int_id)
{
    bool current_value = emu_int[int_id]->read();

    if (current_value != mIntVal[int_id]) 
    {
        emu_int[int_id]->write(mIntVal[int_id]);
        std::ostringstream port_name;
        port_name.str("");
        port_name << "emu_int_" << int_id;
        DumpInterruptMsg(port_name.str().c_str(), mIntVal[int_id]);
    }
}
//========================================================
//============      Update Registers      ================
//========================================================
///********************************************************
/// Function     : UpdateErrorReg 
/// Description  : 
/// Ref: {}
///********************************************************
void HWCORE_INT::UpdateErrorReg(std::string reg_name, unsigned char ip_id, unsigned char func_id)
{
    unsigned int reg_val = (unsigned int)(((unsigned int)ip_id << 4) | (unsigned int)func_id);
    mpHWCORE_INTFunc->SetRegVal(reg_name, reg_val);
}
///********************************************************
/// Function     : JudgeBufferRegisters 
/// Description  : 
/// Ref: {EMU3S_HWCORE_UD_Flow_057}
///********************************************************
void HWCORE_INT::JudgeBufferRegisters(std::string enable_bit_name)
{
    unsigned int enable_bit = mpHWCORE_VRFBUFFunc->GetRegVal("CBTIM", enable_bit_name);
    if (enable_bit == 1)
    {
        unsigned int CBCTR0 = mpHWCORE_VRFBUFFunc->GetRegVal("CBCTR0");
        unsigned int CBMON = ((CBCTR0 >> 2) & 0x1);
        if (CBMON == 1)
        {
            StartBufferRegisters();
        }
    }
}
///********************************************************
/// Function     : StartBufferRegisters 
/// Description  : 
/// Ref: {EMU3S_HWCORE_UD_ATT1_007}
///********************************************************
void HWCORE_INT::StartBufferRegisters()
{
    CBAD[0] = (unsigned int)(ia_1ch_lo_org_sel.read() & 0xFFF);
    CBAD[1] = (unsigned int)(ia_2ch_lo_org_sel.read() & 0xFFF);
    CBAD[2] = (unsigned int)(ia_3ch_lo_org_sel.read() & 0xFFF);
    CBTHTRESFIXIN = (unsigned int)(u16or_res_angle.read() & 0xFFF);
    CBIDFIX = s32or_id_lo.read();
    CBIQFIX = s32or_iq_lo.read();

    CBPWMUIP = (tstwdcmpu.read() & 0x3FFFF);
    CBPWMVIP = (tstwdcmpv.read() & 0x3FFFF);
    CBPWMWIP = (tstwdcmpw.read() & 0x3FFFF);

    CBBREC = 0;
    unsigned int CBBREC_UPTN = 0;
    if (rctg_u_80m_reg.read())
    {
        CBBREC_UPTN = 1;
    }
    unsigned int CBBREC_VPTN = 0;
    if (rctg_v_80m_reg.read())
    {
        CBBREC_VPTN = 1;
    }
    unsigned int CBBREC_WPTN = 0;
    if (rctg_w_80m_reg.read())
    {
        CBBREC_WPTN = 1;
    }
    CBBREC = ((CBBREC_WPTN << 2) | (CBBREC_VPTN << 1) | CBBREC_UPTN);

    CBIREC3 = 0;
    unsigned int CBIREC3_UPTN = 0;
    if (irctg3_upatout_reg.read())
    {
        CBIREC3_UPTN = 1;
    }
    unsigned int CBIREC3_VPTN = 0;
    if (irctg3_vpatout_reg.read())
    {
        CBIREC3_VPTN = 1;
    }
    unsigned int CBIREC3_WPTN = 0;
    if (irctg3_wpatout_reg.read())
    {
        CBIREC3_WPTN = 1;
    }
    CBIREC3 = ((CBIREC3_WPTN << 2) | (CBIREC3_VPTN << 1) | CBIREC3_UPTN);
    
    double current_time = sc_time_stamp().to_double();
    double buffer_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    buffer_time = buffer_time + (double)mPCLKPeriod*mINT_cyc;
    mBufferRegistersEvent.notify(buffer_time, mTimeResolutionUnit);
}
///********************************************************
/// Function     : TriggerBufferRegisters 
/// Description  : 
/// Ref: {}
///********************************************************
void HWCORE_INT::TriggerBufferRegisters()
{
    unsigned int CBEN1 = mpHWCORE_VRFBUFFunc->GetRegVal("CBCTR0", "CBEN1");
    if (CBEN1 == 0)
    {
        mpHWCORE_VRFBUFFunc->SetRegVal("CBCTR0", "CBMON", 0); // Clear CBMON in CBEN0 mode
    }
    mpHWCORE_VRFBUFFunc->SetRegVal("CBAD0", CBAD[0]);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBAD1", CBAD[1]);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBAD2", CBAD[2]);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBTHTRESFIXIN", CBTHTRESFIXIN);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBIDFIX", CBIDFIX);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBIQFIX", CBIQFIX);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBPWMUIP", CBPWMUIP);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBPWMVIP", CBPWMVIP);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBPWMWIP", CBPWMWIP);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBBREC", CBBREC);
    mpHWCORE_VRFBUFFunc->SetRegVal("CBIREC3", CBIREC3);
    
    mpHWCORE_INTFunc->SetRegVal("INTSD", "CBUFIF", 1);
    JudgeInterruptOutput("CBUFINT");
}
//========================================================
//============        PYTHON IF           ================
//========================================================
///*********************************
/// Function     : SetMessageLevel 
/// Description  : Set message level (fatal, error, warning, info)
/// Ref: {}
///*********************************
void HWCORE_INT::SetMessageLevel (const std::string msg_lv)
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

        mpHWCORE_INTFunc->RegisterHandler(cmd);
        mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpHWCORE_INTFunc->RegisterHandler(cmd);
        mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);
    } 
    else 
    {
    //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
    
        mpHWCORE_INTFunc->RegisterHandler(cmd);
        mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);
    
        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
    
        mpHWCORE_INTFunc->RegisterHandler(cmd);
        mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);
    }
}//}}}

///*********************************
/// Function     : DumpRegisterRW 
/// Description  : Enable/disable dumping message when users access registers
/// Ref: {}
///*********************************
void HWCORE_INT::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_INTFunc->RegisterHandler(cmd);
    mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : EnableRegisterMessage 
/// Description  : Enable/disable EnableRegisterMessage
// Ref: {}
///*********************************
void HWCORE_INT::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") 
        {
            mEnableRegisterMessage = true;
        } 
        else if (word_vector[0] == "false") 
        {
            mEnableRegisterMessage = false;
        } 
        else 
        {
            re_printf("warning","Invalid argument: HWCORE_INT_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } 
    else if (word_vector.size() == 0) 
    {
        if (mEnableRegisterMessage) 
        {
            re_printf("info","HWCORE_INT_EnableRegisterMessage %s\n", "true");
        } else {
            re_printf("info","HWCORE_INT_EnableRegisterMessage %s\n", "false");
        }
    } else {
        re_printf("warning","Invalid argument: HWCORE_INT_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : DumpInterrupt 
/// Description  : Enable/disable dumping interrupt information
// Ref: {}
///*********************************
void HWCORE_INT::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) 
    {
        if (word_vector[0] == "true") 
        {
            mDumpInterrupt = true;
        } 
        else if (word_vector[0] == "false") 
        {
            mDumpInterrupt = false;
        } 
        else 
        {
            re_printf("warning","Invalid argument: HWCORE_INT_DumpInterrupt %s\n", is_enable.c_str());
        }
    } 
    else if (word_vector.size() == 0)
    {
        std::string temp = "false";
        if(mDumpInterrupt)
        {
            temp = "true";
        }
        else
        {
            temp = "false";
        }
        re_printf("info","HWCORE_INT_DumpInterrupt %s\n",temp.c_str());
    } 
    else 
    {
        re_printf("warning","Invalid argument: HWCORE_INT_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : AssertReset
/// Description  : Command to assert reset
/// Ref: {EMU3S_HWCORE_UD_Direction_030, EMU3S_HWCORE_UD_Direction_031, EMU3S_HWCORE_UD_Direction_036}
///*********************************
void HWCORE_INT::AssertReset (const std::string reset_name, const double start_time, const double period)
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
        re_printf("warning","The reset name (%s) is wrong. It should be rst_n",reset_name.c_str());
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq
/// Description  : Set clock value
/// Ref: {EMU3S_HWCORE_UD_Feature_026, EMU3S_HWCORE_UD_Direction_027, EMU3S_HWCORE_UD_Direction_028, EMU3S_HWCORE_UD_Direction_033}
///*********************************
void HWCORE_INT::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) 
    {
        if (word_vector[0] == "PCLK") 
        {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) 
            {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mTimePCLKChangePoint = sc_time_stamp().to_double();
            } 
            else 
            {
                mPCLKPeriod = 0;
                re_printf("info", "(%s) frequency is zero.\n", clock_name.c_str());
                // Cancel events
                CancelEvents();
            }
        } 
        else 
        {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } 
    else 
    {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function   : GetCLKFreq 
/// Description: Get clock value
/// Ref: {EMU3S_HWCORE_UD_Direction_027, EMU3S_HWCORE_UD_Direction_032}
///*********************************
void HWCORE_INT::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) 
    {
        if (word_vector[0] == "PCLK") 
        {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } 
        else 
        {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } 
    else 
    {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ForceRegister 
/// Description  : Force value to register
/// Ref: {}
///*********************************
void HWCORE_INT::ForceRegister (const std::string ip_name, const std::string reg_name, const unsigned int reg_value)
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
            if (ip_name == "INT")
            {
                mpHWCORE_INTFunc->RegisterHandler(cmd);
            }
            else
            {
                mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);
            }
        } 
        else 
        {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } 
    else 
    {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReleaseRegister
/// Description  : Release forced value after forcing registers
/// Ref: {}
///*********************************
void HWCORE_INT::ReleaseRegister (const std::string ip_name, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") 
    {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) 
        {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            if (ip_name == "INT")
            {
                mpHWCORE_INTFunc->RegisterHandler(cmd);
            }
            else
            {
                mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);
            }
        } 
        else 
        {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } 
    else 
    {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : WriteRegister 
/// Description  : Write value to registers by software
/// Ref: {}
///*********************************
void HWCORE_INT::WriteRegister (const std::string ip_name, const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") 
    {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) 
        {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            if (ip_name == "INT")
            {
                mpHWCORE_INTFunc->RegisterHandler(cmd);
            }
            else
            {
                mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);
            }
        } 
        else 
        {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } 
    else 
    {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReadRegister
/// Description  : Read value of register by software
/// Ref: {}
///*********************************
void HWCORE_INT::ReadRegister (const std::string ip_name, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") 
    {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) 
        {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            if (ip_name == "INT")
            {
                mpHWCORE_INTFunc->RegisterHandler(cmd);
            }
            else
            {
                mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);
            }
        } 
        else 
        {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } 
    else 
    {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ListRegister 
/// Description  : List all registers name
/// Ref: {}
///*********************************
void HWCORE_INT::ListRegister (const std::string ip_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    if (ip_name == "INT")
    {
        mpHWCORE_INTFunc->RegisterHandler(cmd);
    }
    else
    {
        mpHWCORE_VRFBUFFunc->RegisterHandler(cmd);
    }
}//}}}

///*********************************
/// Function     : Help
/// Description  : Dump help message of all parameters or commands
/// Ref: {EMU3S_HWCORE_UD_Direction_029, EMU3S_HWCORE_UD_Direction_076, EMU3S_HWCORE_UD_Direction_077}
///*********************************
void HWCORE_INT::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") 
        {
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_MessageLevel (\"HWCORE_INT instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_DumpRegisterRW (\"HWCORE_INT instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_DumpInterrupt (\"HWCORE_INT instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_EnableRegisterMessage (\"HWCORE_INT instance\", \"true/false\")                               Enable/disable info/warning/error message of register IF (Default: true)");
        } 
        else if (word_vector[0] == "commands") 
        {
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_AssertReset (\"HWCORE_INT instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the HWCORE_INT model");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_SetCLKFreq (\"HWCORE_INT instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to HWCORE_INT model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_GetCLKFreq (\"HWCORE_INT instance\", \"clock_name\")                                          Get clock frequency value of HWCORE_INT model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_ForceRegister (\"HWCORE_INT instance\",\"ip_name\",\"reg_name\", reg_value)                   Force register with setting value");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_ReleaseRegister (\"HWCORE_INT instance\",\"ip_name\",\"reg_name\")                            Release register from force value");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_WriteRegister (\"HWCORE_INT instance\",\"ip_name\",\"reg_name\", reg_value)                   Write a value to a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_ReadRegister (\"HWCORE_INT instance\",\"ip_name\",\"reg_name\")                               Read a value from a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_ListRegister (\"HWCORE_INT instance\",\"ip_name\")                                            Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"HWCORE_INT_ChgProcessingTime (\"HWCORE_INT instance\", \"cycle\")                                        Change model processing time");
        } 
        else 
        {
            re_printf("warning","The name (%s) of HWCORE_INT_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } 
    else 
    {
        re_printf("warning","The name (%s) of HWCORE_INT_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///*********************************
/// Function   : ChgProcessingTime
/// Description: Change the process time
/// Ref: {EMU3S_HWCORE_UD_Direction_081}
///*********************************
void HWCORE_INT::ChgProcessingTime(const unsigned int cycle)
{//{{{
    mINT_cyc = cycle;
    re_printf("info", "Change the process time for INT to %d.\n", cycle);
}//}}}

//========================================================
//============ Method/Threads          ===================
//========================================================
///*********************************
/// Function     : HandlePCLKSignalMethod 
/// Description  : Handle PCLK signal
/// Ref: {EMU3S_HWCORE_UD_Feature_026, EMU3S_HWCORE_UD_Flow_052}
///*********************************
void HWCORE_INT::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function   : HandlePRESETnSignalMethod 
/// Description: Handle PRESETn signal
/// Ref: {EMU3S_HWCORE_UD_Feature_027, EMU3S_HWCORE_UD_Flow_051}
///*********************************
void HWCORE_INT::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[emPRESETnId] = PRESETn.read();
    if (mResetCurVal[emPRESETnId] != mResetPreVal[emPRESETnId] ) {       //mResetPreVal is "true" at initial state
        mResetPreVal[emPRESETnId]  = mResetCurVal[emPRESETnId] ;
        mResetHardEvent[emPRESETnId].notify(SC_ZERO_TIME);
    }
}//}}}

///*********************************
/// Function   : Handlerst_nSignalMethod 
/// Description: Handle rst_n signal
/// Ref: {EMU3S_HWCORE_UD_Feature_027, EMU3S_HWCORE_UD_Flow_051}
///*********************************
void HWCORE_INT::Handlerst_nSignalMethod (void)
{//{{{
    mResetCurVal[emrst_nId] = rst_n.read();
    if (mResetCurVal[emrst_nId] != mResetPreVal[emrst_nId]) {       //mResetPreVal is "false" at initial state
        mResetPreVal[emrst_nId] = mResetCurVal[emrst_nId];
        mResetHardEvent[emrst_nId].notify(SC_ZERO_TIME);
    }
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
///*********************************
/// Function   : HandleResetHardMethod
/// Description: Process reset function when reset port is active
/// Ref: {EMU3S_HWCORE_UD_Feature_027,  EMU3S_HWCORE_UD_Direction_037, EMU3S_HWCORE_UD_Direction_038}
///*********************************
void HWCORE_INT::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) 
    {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        } 
        else 
        {
            re_printf("info","The reset port rst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
    } 
    else 
    {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) 
        {
            re_printf("info","The reset port PRESETn is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        } 
        else 
        {
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
/// Function   : HandleResetCmdMethod 
/// Description: Process reset function when reset command is active
/// Ref: {EMU3S_HWCORE_UD_Direction_035}
///*********************************
void HWCORE_INT::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) 
    {
        reset_name = "PRESETn";
    } else { //reset_id == emrst_nId
        reset_name = "rst_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when reset command is active
///*********************************
/// Function   : CancelResetCmdMethod 
/// Description: Cancel ResetCmd Method
/// Ref: {EMU3S_HWCORE_UD_Direction_034}
///*********************************
void HWCORE_INT::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) 
    {
        reset_name = "PRESETn";
    } 
    else 
    {
        reset_name = "rst_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]);
}//}}}

///*********************************
/// Function   : EnableReset 
/// Description: Execute reset operation
/// Ref: {EMU3S_HWCORE_UD_Flow_061}
///*********************************
void HWCORE_INT::EnableReset (const bool is_active)
{//{{{
    mpHWCORE_INTFunc->EnableReset (is_active);
    mpHWCORE_VRFBUFFunc->EnableReset(is_active);
    if (is_active && GetResetStatus("rst_n")) {
        CancelEvents();
        Initialize();
        mResetPortsEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void HWCORE_INT::Initialize(void)
{//{{{
    for (unsigned int i = 0; i < emINT_NUM; i++)
    {
        mIntVal[i] = false;
    }
    for (unsigned int i = 0; i < 3; i++)
    {
        CBAD[i] = 0;
    }
    CBIDFIX = 0;
    CBIQFIX = 0;
    CBPWMUIP = 0;
    CBPWMVIP = 0;
    CBPWMWIP = 0;
    CBBREC = 0;
    CBIREC3 = 0;
}//}}}

///*********************************
/// Function     : Reset_Ports
/// Description  : Initialize output ports
///*********************************
void HWCORE_INT::ResetPortsMethod(void)
{//{{{
    for (unsigned int i = 0; i < emINT_NUM; i++)
    {
        mWriteEmuIntEvent[i].notify();
    }
}//}}}

///*********************************
/// Function     : CancelEvents
/// Description  : Cancel all events
///*********************************
void HWCORE_INT::CancelEvents()
{//{{{
    for (unsigned int i = 0; i < emINT_NUM; i++)
    {
        mAssertEmuIntEvent[i].cancel();
        mDeassertEmuIntEvent[i].cancel();
        mWriteEmuIntEvent[i].cancel();
    }
    mBufferRegistersEvent.cancel();
}//}}}

///*********************************
/// Function   : DumpInterruptMsg
/// Description: Dump Interrupt Message
/// Ref: {EMU3S_HWCORE_UD_Direction_046}
///*********************************
void HWCORE_INT::DumpInterruptMsg (const std::string intr_name, const bool value)
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
//============ Overwrite func. of agent ==================
//========================================================
///*********************************
/// Function     : CheckClockPeriod 
/// Description  : Check Clock Period
/// Ref: {}
///*********************************
bool HWCORE_INT::CheckClockPeriod (void)
{//{{{
    bool clock_status = false;
    if (mPCLKFreq > 0) 
    {
        clock_status = true;
    } 
    else 
    {
        clock_status = false;
        re_printf("info","PCLK frequency is zero\n");
    }
    return clock_status;
}//}}}

///*********************************
/// Function     : GetResetStatus 
/// Description  : Get Reset Status
///*********************************
bool HWCORE_INT::GetResetStatus(const std::string reset_name)
{//{{{
   bool reset_status = false;
    if(reset_name == "PRESETn") 
    {
        if (mIsResetHardActive[emPRESETnId] || mIsResetCmdActive[emPRESETnId]) {
            reset_status = true;
        }
    }
    else // rst_n reset
    {
        if (mIsResetHardActive[emrst_nId] || mIsResetCmdActive[emrst_nId]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}
