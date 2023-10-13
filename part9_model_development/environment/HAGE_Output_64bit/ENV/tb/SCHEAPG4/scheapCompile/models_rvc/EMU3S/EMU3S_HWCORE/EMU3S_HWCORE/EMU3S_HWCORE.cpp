// ---------------------------------------------------------------------
// $Id: EMU3S_HWCORE.cpp,v 1.11 2020/11/18 01:21:30 huyquocpham Exp $
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
// Description: Copyright of EMU3S_HWCORE.cpp
// Ref: {EMU3S_HWCORE_UD_Copyright_001}

//--------------------------------------
// EMU3S_HWCORE model implimentation file
//--------------------------------------
// Description: Rule about the include header files.
// Ref: {EMU3S_HWCORE_UD_ATT1_001}
#include "re_define.h"
#include "EMU3S_HWCORE.h"
#include "EMU3S_HWCORE_Func.h"
#include "HWCORE_HANDSHAKE_Func.h"
#include "HWCORE_INPUT.h"
#include "HWCORE_IIR.h"
#include "HWCORE_MEASURE.h"
#include "HWCORE_RCTG.h"
#include "HWCORE_RSLVIDE.h"
#include "HWCORE_ADTRG.h"
#include "HWCORE_PI.h"
#include "HWCORE_PWM.h"
#include "HWCORE_IRCTG3.h"
#include "HWCORE_INT.h"

using namespace std;
using namespace CmErrMsg;

extern sc_time_unit glb_resolution_unit;
extern void CmErrMsg::printErrorMessage(const char* a, const char* b, const char* c);

// Description: Constructor of EMU3S_HWCORE model
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_001}
EMU3S_HWCORE::EMU3S_HWCORE(sc_module_name name, uint32_t rLatency, uint32_t wLatency, SIM_MODE_T simmode)
    :sc_module(name),
    EMU3S_HWCORE_AgentController(),
    BusSlaveBase<BUS_WIDTH_APB, 1>(),
    PCLK("PCLK"),
    CCLK("CCLK"),
    PRESETn("PRESETn"),
    CRESETn("CRESETn"),
    ish("ish"),
    SFERC("SFERC"),
    edcerr_sed_pclk("edcerr_sed_pclk"),
    edcerr_ded_pclk("edcerr_ded_pclk"),
    edcerr_sed_cclk("edcerr_sed_cclk"),
    edcerr_ded_cclk("edcerr_ded_cclk"),
    adc_start("adc_start"),
    adc_grpend("adc_grpend"),
    adc_ch0end("adc_ch0end"),
    adc_ch0dat("adc_ch0dat"),
    adc_ch1end("adc_ch1end"),
    adc_ch1dat("adc_ch1dat"),
    adc_ch2end("adc_ch2end"),
    adc_ch2dat("adc_ch2dat"),
    adc1_start("adc1_start"),
    adc1_grpend("adc1_grpend"),
    adc1_ch0end("adc1_ch0end"),
    adc1_ch0dat("adc1_ch0dat"),
    adc1_ch1end("adc1_ch1end"),
    adc1_ch1dat("adc1_ch1dat"),
    adc1_ch2end("adc1_ch2end"),
    adc1_ch2dat("adc1_ch2dat"),
    rdc_phi("rdc_phi"),
    rdc_z("rdc_z"),
    rdc_high_omega("rdc_high_omega"),
    rdc1_phi("rdc1_phi"),
    rdc1_z("rdc1_z"),
    rdc1_high_omega("rdc1_high_omega"),
    tstpektim("tstpektim"),
    tstvlytim("tstvlytim"),
    tstadt0("tstadt0"),
    tstadt1("tstadt1"),
    tstint3("tstint3"),
    tstint4("tstint4"),
    tstwdcmp0("tstwdcmp0"),
    tstwdcmpu("tstwdcmpu"),
    tstwdcmpv("tstwdcmpv"),
    tstwdcmpw("tstwdcmpw"),
    tstwecmp("tstwecmp"),
    tstiup("tstiup"),
    tstivp("tstivp"),
    tstiwp("tstiwp"),
    PCLK_sig("PCLK_sig"),
    CCLK_sig("CCLK_sig"),
    PRESETn_sig("PRESETn_sig"),
    rst_n_sig("rst_n_sig"),
    rst_clean_n_sig("rst_clean_n_sig"),
    rdc_out_z_sig("rdc_out_z_sig"),
    rdc_out_phi_sig("rdc_out_phi_sig"),
    rdc_out_high_omega_sig("rdc_out_high_omega_sig"),
    adc_in_start_sig("adc_in_start_sig"),
    adc_out_grpend_sig("adc_out_grpend_sig"),
    adc_out_ch0end_sig("adc_out_ch0end_sig"),
    adc_out_ch1end_sig("adc_out_ch1end_sig"),
    adc_out_ch2end_sig("adc_out_ch2end_sig"),
    adc_out_ch0dat_sig("adc_out_ch0dat_sig"),
    adc_out_ch1dat_sig("adc_out_ch1dat_sig"),
    adc_out_ch2dat_sig("adc_out_ch2dat_sig"),
    tstwecmp_sig("tstwecmp_sig"),
    tstwdcmp0_sig("tstwdcmp0_sig"),
    tstwdcmpu_sig("tstwdcmpu_sig"),
    tstwdcmpv_sig("tstwdcmpv_sig"),
    tstwdcmpw_sig("tstwdcmpw_sig"),
    tstpektim_sig("tstpektim_sig"),
    tstvlytim_sig("tstvlytim_sig"),
    tstadt0_sig("tstadt0_sig"),
    tstadt1_sig("tstadt1_sig"),
    tstint3_sig("tstint3_sig"),
    tstint4_sig("tstint4_sig"),
    input_start_sp_sig("input_start_sp_sig"),
    FUNCIDLEGRPA0_sig("FUNCIDLEGRPA0_sig"),
    FUNCWAITGRPA_sig("FUNCWAITGRPA_sig"),
    FUNCFLGRPA0_sig("FUNCFLGRPA0_sig"),
    input_func_end_sig("input_func_end_sig"),
    input_func_st_sig("input_func_st_sig"),
    input_func_wait_sig("input_func_wait_sig"),
    input_func_busy_sig("input_func_busy_sig"),
    CTR_sig("CTR_sig"),
    output_rctg_start_sp_sig("output_rctg_start_sp_sig"),
    FUNCIDLEGRPB_sig("FUNCIDLEGRPB_sig"),
    FUNCWAITGRPB_sig("FUNCWAITGRPB_sig"),
    FUNCFLGRPB_sig("FUNCFLGRPB_sig"),
    rslv_func_st_sig("rslv_func_st_sig"),
    rslv_func_end_sig("rslv_func_end_sig"),
    rslv_func_wait_sig("rslv_func_wait_sig"),
    rslv_func_busy_sig("rslv_func_busy_sig"),
    output_pwm_start_sp_sig("output_pwm_start_sp_sig"),
    FUNCIDLEGRPA2_sig("FUNCIDLEGRPA2_sig"),
    FUNCFLGRPA2_sig("FUNCFLGRPA2_sig"),
    REFCTR_sig("REFCTR_sig"),
    pwm_func_end_sig("pwm_func_end_sig"),
    pwm_func_st_sig("pwm_func_st_sig"),
    pwm_func_wait_sig("pwm_func_wait_sig"),
    pwm_func_busy_sig("pwm_func_busy_sig"),
    ADSEL_sig("ADSEL_sig"),
    IPTRGCTR_sig("IPTRGCTR_sig"),
    IPTRGNUM_sig("IPTRGNUM_sig"),
    TBLADR_sig("TBLADR_sig"),
    AHBPRT_sig("AHBPRT_sig"),
    ADENDCTR_sig("ADENDCTR_sig"),
    IPTRGMON_ADTRGNUM_sig("IPTRGMON_ADTRGNUM_sig"),
    pi_start_sp_sig("pi_start_sp_sig"),
    FUNCIDLEGRPA1_sig("FUNCIDLEGRPA1_sig"),
    FUNCFLGRPA1_sig("FUNCFLGRPA1_sig"),
    IPTRG_sig("IPTRG_sig"),
    pi_func_end_sig("pi_func_end_sig"),
    pi_func_st_sig("pi_func_st_sig"),
    pi_func_wait_sig("pi_func_wait_sig"),
    pi_func_busy_sig("pi_func_busy_sig"),
    funca_wait_int_sig("funca_wait_int_sig"),
    funcb_wait_int_sig("funcb_wait_int_sig"),
    adtrg_ch0_dat_sig("adtrg_ch0_dat_sig"),
    adtrg_ch1_dat_sig("adtrg_ch1_dat_sig"),
    adtrg_ch2_dat_sig("adtrg_ch2_dat_sig"),
    adtrg_thte_sig("adtrg_thte_sig"),
    adtrg_thtr_sig("adtrg_thtr_sig"),
    input_end_sp_reg_sig("input_end_sp_reg_sig"),
    em2ictrinmd_instctr_sig("em2ictrinmd_instctr_sig"),
    theta_e_sel_sig("theta_e_sel_sig"),
    ia_v_lo_fix_reg_sig("ia_v_lo_fix_reg_sig"),
    ia_w_lo_fix_reg_sig("ia_w_lo_fix_reg_sig"),
    ia_u_lo_fix_reg_sig("ia_u_lo_fix_reg_sig"),
    rdata_em2iidfixobuf_sig("rdata_em2iidfixobuf_sig"),
    rdata_em2iiqfixobuf_sig("rdata_em2iiqfixobuf_sig"),
    em3ictrinmd_iirdq_sig("em3ictrinmd_iirdq_sig"),
    rdata_em2ithtrefixin_sig("rdata_em2ithtrefixin_sig"),
    kcl_int_sig("kcl_int_sig"),
    in_ovfl_out_sig("in_ovfl_out_sig"),
    in_ovfl_func_sig("in_ovfl_func_sig"),
    in_zdfl_out_sig("in_zdfl_out_sig"),
    ia_1ch_lo_org_sel_sig("ia_1ch_lo_org_sel_sig"),
    ia_2ch_lo_org_sel_sig("ia_2ch_lo_org_sel_sig"),
    ia_3ch_lo_org_sel_sig("ia_3ch_lo_org_sel_sig"),
    rdata_em3iad0_sig("rdata_em3iad0_sig"),
    rdata_em3iad1_sig("rdata_em3iad1_sig"),
    rdata_em3iad2_sig("rdata_em3iad2_sig"),
    adtrg_end_trig_sig("adtrg_end_trig_sig"),
    iir_end0_sig("iir_end0_sig"),
    iir_end1_sig("iir_end1_sig"),
    iir_end2_sig("iir_end2_sig"),
    iir_ovfl_out_sig("iir_ovfl_out_sig"),
    iir_ovfl_func_sig("iir_ovfl_func_sig"),
    iir_output0_sig("iir_output0_sig"),
    iir_output1_sig("iir_output1_sig"),
    iir_output2_sig("iir_output2_sig"),
    presml_ang_comp0_eq_sp_sig("presml_ang_comp0_eq_sp_sig"),
    rdata_em2iipcmp0_sig("rdata_em2iipcmp0_sig"),
    em2ibrecctr_slctcmp0_reg_sig("em2ibrecctr_slctcmp0_reg_sig"),
    wc_em2icmp0_sig("wc_em2icmp0_sig"),
    em2icmp0_sig("em2icmp0_sig"),
    wc_em2icmp1_sig("wc_em2icmp1_sig"),
    em2icmp1_sig("em2icmp1_sig"),
    output_rctg_end_sp_reg_sig("output_rctg_end_sp_reg_sig"),
    rctg_u_80m_reg_sig("rctg_u_80m_reg_sig"),
    rctg_v_80m_reg_sig("rctg_v_80m_reg_sig"),
    rctg_w_80m_reg_sig("rctg_w_80m_reg_sig"),
    pmt2cp_sig("pmt2cp_sig"),
    pmt2of_sig("pmt2of_sig"),
    phi_out_sig("phi_out_sig"),
    ANGCTR_sig("ANGCTR_sig"),
    RESTHSFT_sig("RESTHSFT_sig"),
    rdata_em2ithtresfix_sig("rdata_em2ithtresfix_sig"),
    rdata_em2ithtefix_sig("rdata_em2ithtefix_sig"),
    ang_comp1_eq_sp_sig("ang_comp1_eq_sp_sig"),
    ir3_adc_u_r_sig("ir3_adc_u_r_sig"),
    ir3_adc_v_r_sig("ir3_adc_v_r_sig"),
    ir3_adc_w_r_sig("ir3_adc_w_r_sig"),
    ad_end_trig_sp_sig("ad_end_trig_sp_sig"),
    adtrg_mvint1_sig("adtrg_mvint1_sig"),
    adtrg_mvint0_sig("adtrg_mvint0_sig"),
    adtrg_mem_rg_chg_sig("adtrg_mem_rg_chg_sig"),
    adtrg_smerr_sig("adtrg_smerr_sig"),
    adtrg_amerr_sig("adtrg_amerr_sig"),
    ad_start_mou_sp_80m_sig("ad_start_mou_sp_80m_sig"),
    ad_start_vall_sp_80m_sig("ad_start_vall_sp_80m_sig"),
    id_sel_out_sig("id_sel_out_sig"),
    iq_sel_out_sig("iq_sel_out_sig"),
    id_dir_reg_sig("id_dir_reg_sig"),
    iq_dir_reg_sig("iq_dir_reg_sig"),
    id_reg_sig("id_reg_sig"),
    iq_reg_sig("iq_reg_sig"),
    rdata_em2ivd_sig("rdata_em2ivd_sig"),
    rdata_em2ivq_sig("rdata_em2ivq_sig"),
    output_pwm_end_sp_reg_sig("output_pwm_end_sp_reg_sig"),
    tstwdcmpu_pclk_sig("tstwdcmpu_pclk_sig"),
    tstwdcmpv_pclk_sig("tstwdcmpv_pclk_sig"),
    tstwdcmpw_pclk_sig("tstwdcmpw_pclk_sig"),
    pwm_ovfl_out_sig("pwm_ovfl_out_sig"),
    pwm_ovfl_func_sig("pwm_ovfl_func_sig"),
    pwm_zdfl_out_sig("pwm_zdfl_out_sig"),
    pi_end_sp_reg_sig("pi_end_sp_reg_sig"),
    pi_ovfl_out_sig("pi_ovfl_out_sig"),
    pi_ovfl_func_sig("pi_ovfl_func_sig"),
    ir3_int_err_r_sig("ir3_int_err_r_sig"),
    ir3_int_u_r_sig("ir3_int_u_r_sig"),
    ir3_int_v_r_sig("ir3_int_v_r_sig"),
    ir3_int_w_r_sig("ir3_int_w_r_sig"),
    ir3_calc_u_r_sig("ir3_calc_u_r_sig"),
    ir3_calc_v_r_sig("ir3_calc_v_r_sig"),
    ir3_calc_w_r_sig("ir3_calc_w_r_sig"),
    ir3_pat_u_r_sig("ir3_pat_u_r_sig"),
    ir3_pat_v_r_sig("ir3_pat_v_r_sig"),
    ir3_pat_w_r_sig("ir3_pat_w_r_sig")
{ 
    /* BusSlave */
    setSlaveResetPort32(&PRESETn);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32((char*)"tsp_cpu");

    tsp_cpu = tSocket32[0];

    mpEMU3S_HWCORE_Func = new EMU3S_HWCORE_Func((std::string)name, this);
    mpHWCORE_HANDSHAKE_Func = new HWCORE_HANDSHAKE_Func("HWCORE_HANDSHAKE", this);
    mpINPUT      = new HWCORE_INPUT("INPUT");
    mpIIR        = new HWCORE_IIR("IIR");
    mpMEASURE    = new HWCORE_MEASURE("MEASURE");
    mpRCTG       = new HWCORE_RCTG("RCTG");
    mpRSLVIDE    = new HWCORE_RSLVIDE("RSLVIDE");
    mpADTRG      = new HWCORE_ADTRG("ADTRG", simmode);
    mpPI         = new HWCORE_PI("PI");
    mpPWM        = new HWCORE_PWM("PWM");
    mpIRCTG3     = new HWCORE_IRCTG3("IRCTG3");
    mpINT        = new HWCORE_INT("INT");
    
    mBusSlaveIf32[0]->setFuncModulePtr((BusSlaveFuncIf *)mpEMU3S_HWCORE_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    for (unsigned int i = 0; i < emINTERRUPT_NUM; i++)
    {
        std::ostringstream port_name;
        port_name.str("");
        port_name << "emu_int_" << i;
        emu_int[i] = new sc_out<bool>(port_name.str().c_str()); 
    }
    
    // Initialize clock/reset variables
    mPCLKPeriod = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreq = 0;
    mPCLKFreqUnit = "Hz";
    mCCLKPeriod = 0;
    mCCLKOrgFreq = 0;
    mCCLKFreq = 0;
    mCCLKFreqUnit = "Hz";

    mIsPRESETnActive = false;
    mIsPRESETnHardActive = false;
    mIsPRESETnCmdReceive = false;
    mIsPRESETnCmdActive = false;
    mPRESETnCmdPeriod = 0;

    mIsCRESETnActive = false;
    mIsCRESETnHardActive = false;
    mIsCRESETnCmdReceive = false;
    mIsCRESETnCmdActive = false;
    mCRESETnCmdPeriod = 0;

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mTimePCLKChangePoint = 0;
    mTimeCCLKChangePoint = 0;
    
    IsSWReset = true;

    mEnableRegisterMessage = true;
    mDumpInterrupt = false;
    all_sub_ip = "ALL";

    CreateConnections();
    
    Initialize();
    rdc_out_phi_sig.write(0);
    rdc_out_z_sig.write(false);
    rdc_out_high_omega_sig.write(0);
    adc_out_grpend_sig.write(false);
    adc_out_ch0end_sig.write(false);
    adc_out_ch1end_sig.write(false);
    adc_out_ch2end_sig.write(false);
    adc_out_ch0dat_sig.write(0);
    adc_out_ch0dat_sig.write(0);
    adc_out_ch0dat_sig.write(0);
    InitPort();
    InitProcess();
}

// Description: Create Connections inside EMU3S_HWCORE
// Ref: {}
void EMU3S_HWCORE::CreateConnections()
{
    ConnectResetSig();
    ConnectClockSig();
    ConnectTopSig();
    ConnectInternalSig();
}

// Description: Connect reset signals
// Ref: {}
void EMU3S_HWCORE::ConnectResetSig()
{
    mpINPUT->PRESETn(PRESETn_sig);
    mpIIR->PRESETn(PRESETn_sig);
    mpMEASURE->PRESETn(PRESETn_sig);
    mpRCTG->PRESETn(PRESETn_sig);
    mpRSLVIDE->PRESETn(PRESETn_sig);
    mpADTRG->PRESETn(PRESETn_sig);
    mpPI->PRESETn(PRESETn_sig);
    mpPWM->PRESETn(PRESETn_sig);
    mpIRCTG3->PRESETn(PRESETn_sig);
    mpINT->PRESETn(PRESETn_sig);
    
    mpINPUT->rst_n(rst_n_sig);
    mpIIR->rst_n(rst_n_sig);
    mpMEASURE->mip_prst_n(rst_n_sig);
    mpRCTG->rst_n(rst_n_sig);
    mpRSLVIDE->mip_prst_n(rst_n_sig);
    mpADTRG->emu3ictr_emust(rst_n_sig);
    mpPI->rst_n(rst_n_sig);
    mpPWM->rst_n(rst_n_sig);
    mpIRCTG3->mip_prst_n(rst_n_sig);
    mpINT->rst_n(rst_n_sig);

    mpMEASURE->mip_crst_n(rst_clean_n_sig);
    mpADTRG->crst_n(rst_clean_n_sig);
    mpPWM->crst_n(rst_clean_n_sig);
}

// Description: Connect clock signals
// Ref: {}
void EMU3S_HWCORE::ConnectClockSig()
{
    mpINPUT->PCLK(PCLK_sig);
    mpIIR->PCLK(PCLK_sig);
    mpMEASURE->PCLK(PCLK_sig);
    mpRCTG->PCLK(PCLK_sig);
    mpRSLVIDE->PCLK(PCLK_sig);
    mpADTRG->PCLK(PCLK_sig);
    mpPI->PCLK(PCLK_sig);
    mpPWM->PCLK(PCLK_sig);
    mpIRCTG3->PCLK(PCLK_sig);
    mpINT->PCLK(PCLK_sig);

    mpMEASURE->CCLK(CCLK_sig);
    mpADTRG->CCLK(CCLK_sig);
    mpPWM->CCLK(CCLK_sig);
}

// Description: Connect Top signals to Sub-IPs
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_036, EMU3S_HWCORE_UD_Flow_039}
void EMU3S_HWCORE::ConnectTopSig()
{
    (*mpADTRG->is)(ish);
    
    // Output from Top
    
    mpMEASURE->rdc_z(rdc_out_z_sig);
    mpMEASURE->rdc_phi(rdc_out_phi_sig);
    mpPWM->rdc_high_omega_pclk(rdc_out_high_omega_sig);
    
    mpADTRG->adc_grpend(adc_out_grpend_sig);
    mpADTRG->adc_ch0end(adc_out_ch0end_sig);
    mpADTRG->adc_ch0_dat(adc_out_ch0dat_sig);
    mpADTRG->adc_ch1end(adc_out_ch1end_sig);
    mpADTRG->adc_ch1_dat(adc_out_ch1dat_sig);
    mpADTRG->adc_ch2end(adc_out_ch2end_sig);
    mpADTRG->adc_ch2_dat(adc_out_ch2dat_sig);

    mpADTRG->tstpektim(tstpektim_sig);
    mpADTRG->tstvlytim(tstvlytim_sig);
    mpADTRG->tstadt0(tstadt0_sig);
    mpADTRG->tstadt1(tstadt1_sig);
    mpADTRG->tstint3(tstint3_sig);
    mpADTRG->tstint4(tstint4_sig);

    mpINPUT->input_start_sp(input_start_sp_sig);
    mpINPUT->FUNCIDLEGRPA0(FUNCIDLEGRPA0_sig);
    mpINPUT->FUNCWAITGRPA(FUNCWAITGRPA_sig);
    mpINPUT->FUNCFLGRPA0(FUNCFLGRPA0_sig);
    
    mpMEASURE->CTR(CTR_sig);
    
    mpRCTG->output_rctg_start_sp(output_rctg_start_sp_sig);

    mpRSLVIDE->FUNCFLGRPB(FUNCFLGRPB_sig);
    mpRSLVIDE->FUNCIDLEGRPB(FUNCIDLEGRPB_sig);
    mpRSLVIDE->FUNCWAITGRPB(FUNCWAITGRPB_sig);

    mpPWM->output_pwm_start_sp(output_pwm_start_sp_sig);
    mpPWM->FUNCIDLEGRPA2(FUNCIDLEGRPA2_sig);
    mpPWM->FUNCWAITGRPA(FUNCWAITGRPA_sig);
    mpPWM->FUNCFLGRPA2(FUNCFLGRPA2_sig);
    mpPWM->REFCTR(REFCTR_sig);

    mpADTRG->em3iadsel(ADSEL_sig);
    mpADTRG->emu3iptrgctr(IPTRGCTR_sig);
    mpADTRG->emu3iptrgnum(IPTRGNUM_sig);
    mpADTRG->emu3tbladr(TBLADR_sig);
    mpADTRG->ahbprt(AHBPRT_sig);

    mpPI->pi_start_sp(pi_start_sp_sig);
    mpPI->FUNCIDLEGRPA1(FUNCIDLEGRPA1_sig);
    mpPI->FUNCWAITGRPA(FUNCWAITGRPA_sig);
    mpPI->FUNCFLGRPA1(FUNCFLGRPA1_sig);
    mpPI->IPTRG(IPTRG_sig);

    mpINT->funca_wait_int(funca_wait_int_sig);
    mpINT->funcb_wait_int(funcb_wait_int_sig);

    // Output from Sub-IPs
    mpPWM->tstwdcmp0(tstwdcmp0_sig);
    mpPWM->tstwdcmpu(tstwdcmpu_sig);
    mpPWM->tstwdcmpv(tstwdcmpv_sig);
    mpPWM->tstwdcmpw(tstwdcmpw_sig);
    mpPWM->tstwecmp(tstwecmp_sig);

    for (unsigned int i = 0; i < emINTERRUPT_NUM; i++)
    {
        (*mpINT->emu_int[i])(*emu_int[i]);
    }
    
    mpADTRG->adc_start(adc_in_start_sig);

    //mpINPUT->input_func_end(input_func_end_sig);
    //mpINPUT->input_func_st(input_func_st_sig);
    mpINPUT->input_func_end(input_func_st_sig);       // Work around for input_func_st/end issue (Limitation No.8 in REQ of EMU3S_HWCORE)
    mpINPUT->input_func_st(input_func_end_sig);       // Work around for input_func_st/end issue (Limitation No.8 in REQ of EMU3S_HWCORE)
    mpINPUT->input_func_wait(input_func_wait_sig);
    mpINPUT->input_func_busy(input_func_busy_sig);

    mpRSLVIDE->rslv_func_st(rslv_func_st_sig);
    mpRSLVIDE->rslv_func_end(rslv_func_end_sig);
    mpRSLVIDE->rslv_func_wait(rslv_func_wait_sig);
    mpRSLVIDE->rslv_func_busy(rslv_func_busy_sig);

    mpPWM->pwm_func_end(pwm_func_end_sig);
    mpPWM->pwm_func_st(pwm_func_st_sig);
    mpPWM->pwm_func_wait(pwm_func_wait_sig);
    mpPWM->pwm_func_busy(pwm_func_busy_sig);

    mpADTRG->em3iadctr_adend(ADENDCTR_sig);
    mpADTRG->emu3_iptrgmon_adtrgnum(IPTRGMON_ADTRGNUM_sig);

    mpPI->pi_func_end(pi_func_end_sig);
    mpPI->pi_func_st(pi_func_st_sig);
    mpPI->pi_func_wait(pi_func_wait_sig);
    mpPI->pi_func_busy(pi_func_busy_sig);
}

// Description: Connect Sub-IPs by internal signals
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_003}
void EMU3S_HWCORE::ConnectInternalSig()
{
    mpADTRG->adtrg_ch0_dat(adtrg_ch0_dat_sig);
    mpINPUT->adtrg_ch0_dat(adtrg_ch0_dat_sig);

    mpADTRG->adtrg_ch1_dat(adtrg_ch1_dat_sig);
    mpINPUT->adtrg_ch1_dat(adtrg_ch1_dat_sig);

    mpADTRG->adtrg_ch2_dat(adtrg_ch2_dat_sig);
    mpINPUT->adtrg_ch2_dat(adtrg_ch2_dat_sig);

    mpADTRG->adtrg_thte(adtrg_thte_sig);
    mpINPUT->adtrg_thte(adtrg_thte_sig);

    mpADTRG->adtrg_thtr(adtrg_thtr_sig);
    mpINPUT->adtrg_thtr(adtrg_thtr_sig);

    mpINPUT->input_end_sp_reg(input_end_sp_reg_sig);
    mpIIR->input_end_sp_reg(input_end_sp_reg_sig);
    mpINT->input_end_sp_reg(input_end_sp_reg_sig);

    mpINPUT->em2ictrinmd_instctr(em2ictrinmd_instctr_sig);
    mpADTRG->em2ictrinmd_instctr(em2ictrinmd_instctr_sig);

    mpINPUT->theta_e_sel(theta_e_sel_sig);
    mpPWM->theta_e_reg(theta_e_sel_sig);

    mpINPUT->ia_v_lo_fix_reg(ia_v_lo_fix_reg_sig);
    mpPWM->ia_v_lo_fix_reg(ia_v_lo_fix_reg_sig);

    mpINPUT->ia_w_lo_fix_reg(ia_w_lo_fix_reg_sig);
    mpPWM->ia_w_lo_fix_reg(ia_w_lo_fix_reg_sig);

    mpINPUT->ia_u_lo_fix_reg(ia_u_lo_fix_reg_sig);
    mpPWM->ia_u_lo_fix_reg(ia_u_lo_fix_reg_sig);

    mpINPUT->rdata_em2iidfixobuf(rdata_em2iidfixobuf_sig);
    mpIIR->rdata_em2iidfixobuf(rdata_em2iidfixobuf_sig);
    mpPI->rdata_em2iidfixobuf(rdata_em2iidfixobuf_sig);
    mpINT->s32or_id_lo(rdata_em2iidfixobuf_sig);

    mpINPUT->rdata_em2iiqfixobuf(rdata_em2iiqfixobuf_sig);
    mpIIR->rdata_em2iiqfixobuf(rdata_em2iiqfixobuf_sig);
    mpPI->rdata_em2iiqfixobuf(rdata_em2iiqfixobuf_sig);
    mpINT->s32or_iq_lo(rdata_em2iiqfixobuf_sig);

    mpINPUT->em3ictrinmd_iirdq(em3ictrinmd_iirdq_sig);
    mpIIR->em3ictrinmd_iirdq(em3ictrinmd_iirdq_sig);

    mpINPUT->rdata_em2ithtrefixin(rdata_em2ithtrefixin_sig);
    mpINT->u16or_res_angle(rdata_em2ithtrefixin_sig);

    mpINPUT->kcl_int(kcl_int_sig);
    mpINT->kcl_int(kcl_int_sig);

    mpINPUT->in_ovfl_out(in_ovfl_out_sig);
    mpINT->in_ovfl_in(in_ovfl_out_sig);

    mpINPUT->in_ovfl_func(in_ovfl_func_sig);
    mpINT->in_ovfl_func(in_ovfl_func_sig);

    mpINPUT->in_zdfl_out(in_zdfl_out_sig);
    mpINT->in_zdfl_in(in_zdfl_out_sig);

    mpINPUT->ia_1ch_lo_org_sel(ia_1ch_lo_org_sel_sig);
    mpINT->ia_1ch_lo_org_sel(ia_1ch_lo_org_sel_sig);

    mpINPUT->ia_2ch_lo_org_sel(ia_2ch_lo_org_sel_sig);
    mpINT->ia_2ch_lo_org_sel(ia_2ch_lo_org_sel_sig);

    mpINPUT->ia_3ch_lo_org_sel(ia_3ch_lo_org_sel_sig);
    mpINT->ia_3ch_lo_org_sel(ia_3ch_lo_org_sel_sig);

    mpADTRG->rdata_em3iad0(rdata_em3iad0_sig);
    mpIIR->em2iadi0(rdata_em3iad0_sig);

    mpADTRG->rdata_em3iad1(rdata_em3iad1_sig);
    mpIIR->em2iadi1(rdata_em3iad1_sig);

    mpADTRG->rdata_em3iad2(rdata_em3iad2_sig);
    mpIIR->em2iadi2(rdata_em3iad2_sig);

    mpADTRG->adtrg_end_trig(adtrg_end_trig_sig);
    mpIIR->adtrg_end_trig(adtrg_end_trig_sig);
    mpINT->ins_trig(adtrg_end_trig_sig);

    mpIIR->iir_end0(iir_end0_sig);
    mpINT->iir_end0(iir_end0_sig);

    mpIIR->iir_end1(iir_end1_sig);
    mpINT->iir_end1(iir_end1_sig);

    mpIIR->iir_end2(iir_end2_sig);
    mpINT->iir_end2(iir_end2_sig);

    mpIIR->iir_ovfl_out(iir_ovfl_out_sig);
    mpINT->iir_ovfl_in(iir_ovfl_out_sig);

    mpIIR->iir_ovfl_func(iir_ovfl_func_sig);
    mpINT->iir_ovfl_func(iir_ovfl_func_sig);

    mpIIR->iir_output0(iir_output0_sig);
    mpADTRG->iiroutdat0(iir_output0_sig);
    mpPI->rdata_iiroutdat0(iir_output0_sig);

    mpIIR->iir_output1(iir_output1_sig);
    mpADTRG->iiroutdat1(iir_output1_sig);
    mpPI->rdata_iiroutdat1(iir_output1_sig);

    mpIIR->iir_output2(iir_output2_sig);
    mpADTRG->iiroutdat2(iir_output2_sig);

    mpRSLVIDE->presml_ang_comp0_eq_sp(presml_ang_comp0_eq_sp_sig);
    mpRCTG->ang_comp0_eq_sp_80m(presml_ang_comp0_eq_sp_sig);
    mpADTRG->ang_comp0_eq_sp(presml_ang_comp0_eq_sp_sig);
    mpINT->ang_comp0_eq_sp(presml_ang_comp0_eq_sp_sig);

    mpRCTG->rdata_em2iipcmp0(rdata_em2iipcmp0_sig);
    mpRSLVIDE->rdata_em2iipcmp0(rdata_em2iipcmp0_sig);

    mpRCTG->em2ibrecctr_slctcmp0_reg(em2ibrecctr_slctcmp0_reg_sig);
    mpRSLVIDE->em2ibrecctr_slctcmp0_reg(em2ibrecctr_slctcmp0_reg_sig);

    mpRCTG->wc_em2icmp0(wc_em2icmp0_sig);
    mpRSLVIDE->wc_em2icmp0(wc_em2icmp0_sig);

    mpRCTG->em2icmp0(em2icmp0_sig);
    mpRSLVIDE->em2icmp0(em2icmp0_sig);

    mpRCTG->wc_em2icmp1(wc_em2icmp1_sig);
    mpRSLVIDE->wc_em2icmp1(wc_em2icmp1_sig);

    mpRCTG->em2icmp1(em2icmp1_sig);
    mpRSLVIDE->em2icmp1(em2icmp1_sig);

    mpRCTG->output_rctg_end_sp_reg(output_rctg_end_sp_reg_sig);
    mpRSLVIDE->output_rctg_end_sp_reg(output_rctg_end_sp_reg_sig);
    mpINT->output_rctg_end_sp_reg(output_rctg_end_sp_reg_sig);

    mpRCTG->rctg_u_80m_reg(rctg_u_80m_reg_sig);
    mpINT->rctg_u_80m_reg(rctg_u_80m_reg_sig);

    mpRCTG->rctg_v_80m_reg(rctg_v_80m_reg_sig);
    mpINT->rctg_v_80m_reg(rctg_v_80m_reg_sig);

    mpRCTG->rctg_w_80m_reg(rctg_w_80m_reg_sig);
    mpINT->rctg_w_80m_reg(rctg_w_80m_reg_sig);

    mpMEASURE->pmt2cp(pmt2cp_sig);
    mpINT->pmt2cp(pmt2cp_sig);

    mpMEASURE->pmt2of(pmt2of_sig);
    mpINT->pmt2of(pmt2of_sig);

    mpMEASURE->phi_out(phi_out_sig);
    mpRSLVIDE->phi_out_mip_clk(phi_out_sig);

    mpRSLVIDE->ANGCTR(ANGCTR_sig);
    mpMEASURE->ANGCTR(ANGCTR_sig);

    mpRSLVIDE->RESTHSFT(RESTHSFT_sig);
    mpMEASURE->RESTHSFT(RESTHSFT_sig);

    mpRSLVIDE->rdata_em2ithtresfix(rdata_em2ithtresfix_sig);
    mpADTRG->theta_res_fix_reg(rdata_em2ithtresfix_sig);

    mpRSLVIDE->rdata_em2ithtefix(rdata_em2ithtefix_sig);
    mpADTRG->theta_e_fix_reg(rdata_em2ithtefix_sig);
    mpPWM->theta_e_fix_reg(rdata_em2ithtefix_sig);
    mpIRCTG3->e_angle(rdata_em2ithtefix_sig);

    mpRSLVIDE->ang_comp1_eq_sp(ang_comp1_eq_sp_sig);
    mpINT->ang_comp1_eq_sp(ang_comp1_eq_sp_sig);

    mpIRCTG3->ir3_adc_u_r(ir3_adc_u_r_sig);
    mpADTRG->irctg3_compmatch_u(ir3_adc_u_r_sig);

    mpIRCTG3->ir3_adc_v_r(ir3_adc_v_r_sig);
    mpADTRG->irctg3_compmatch_v(ir3_adc_v_r_sig);

    mpIRCTG3->ir3_adc_w_r(ir3_adc_w_r_sig);
    mpADTRG->irctg3_compmatch_w(ir3_adc_w_r_sig);

    mpADTRG->ad_end_trig_sp(ad_end_trig_sp_sig);
    mpINT->adc_trig(ad_end_trig_sp_sig);

    mpADTRG->adtrg_mvint1(adtrg_mvint1_sig);
    mpINT->adtrg_mvint1(adtrg_mvint1_sig);

    mpADTRG->adtrg_mvint0(adtrg_mvint0_sig);
    mpINT->adtrg_mvint0(adtrg_mvint0_sig);

    mpADTRG->adtrg_mem_rg_chg(adtrg_mem_rg_chg_sig);
    mpINT->adtrg_mem_rg_chg(adtrg_mem_rg_chg_sig);

    mpADTRG->adtrg_smerr(adtrg_smerr_sig);
    mpINT->adtrg_smerr(adtrg_smerr_sig);

    mpADTRG->adtrg_amerr(adtrg_amerr_sig);
    mpINT->adtrg_amerr(adtrg_amerr_sig);

    mpADTRG->ad_start_mou_sp_80m(ad_start_mou_sp_80m_sig);
    mpINT->tstpektim(ad_start_mou_sp_80m_sig);

    mpADTRG->ad_start_vall_sp_80m(ad_start_vall_sp_80m_sig);
    mpINT->tstvlytim(ad_start_vall_sp_80m_sig);

    mpPI->id_sel_out(id_sel_out_sig);
    mpPWM->id_sel_out(id_sel_out_sig);

    mpPI->iq_sel_out(iq_sel_out_sig);
    mpPWM->iq_sel_out(iq_sel_out_sig);

    mpPI->id_dir_reg(id_dir_reg_sig);
    mpPWM->id_dir_reg(id_dir_reg_sig);

    mpPI->iq_dir_reg(iq_dir_reg_sig);
    mpPWM->iq_dir_reg(iq_dir_reg_sig);

    mpPI->id_reg(id_reg_sig);
    mpPWM->id_reg(id_reg_sig);

    mpPI->iq_reg(iq_reg_sig);
    mpPWM->iq_reg(iq_reg_sig);

    mpPI->rdata_em2ivd(rdata_em2ivd_sig);
    mpPWM->rdata_em2ivd(rdata_em2ivd_sig);

    mpPI->rdata_em2ivq(rdata_em2ivq_sig);
    mpPWM->rdata_em2ivq(rdata_em2ivq_sig);

    mpPWM->output_pwm_end_sp_reg(output_pwm_end_sp_reg_sig);
    mpINT->output_pwm_end_sp_reg(output_pwm_end_sp_reg_sig);

    mpPWM->tstwdcmpu_pclk(tstwdcmpu_pclk_sig);
    mpINT->tstwdcmpu(tstwdcmpu_pclk_sig);

    mpPWM->tstwdcmpv_pclk(tstwdcmpv_pclk_sig);
    mpINT->tstwdcmpv(tstwdcmpv_pclk_sig);

    mpPWM->tstwdcmpw_pclk(tstwdcmpw_pclk_sig);
    mpINT->tstwdcmpw(tstwdcmpw_pclk_sig);

    mpPWM->pwm_ovfl_out(pwm_ovfl_out_sig);
    mpINT->pwm_ovfl_in(pwm_ovfl_out_sig);

    mpPWM->pwm_ovfl_func(pwm_ovfl_func_sig);
    mpINT->pwm_ovfl_func(pwm_ovfl_func_sig);

    mpPWM->pwm_zdfl_out(pwm_zdfl_out_sig);
    mpINT->pwm_zdfl_in(pwm_zdfl_out_sig);

    mpPI->pi_end_sp_reg(pi_end_sp_reg_sig);
    mpINT->pi_end_sp_reg(pi_end_sp_reg_sig);

    mpPI->pi_ovfl_out(pi_ovfl_out_sig);
    mpINT->pi_ovfl_in(pi_ovfl_out_sig);

    mpPI->pi_ovfl_func(pi_ovfl_func_sig);
    mpINT->pi_ovfl_func(pi_ovfl_func_sig);

    mpIRCTG3->ir3_int_err_r(ir3_int_err_r_sig);
    mpINT->irctg3_err(ir3_int_err_r_sig);

    mpIRCTG3->ir3_int_u_r(ir3_int_u_r_sig);
    mpINT->irctg3_u_int(ir3_int_u_r_sig);

    mpIRCTG3->ir3_int_v_r(ir3_int_v_r_sig);
    mpINT->irctg3_v_int(ir3_int_v_r_sig);

    mpIRCTG3->ir3_int_w_r(ir3_int_w_r_sig);
    mpINT->irctg3_w_int(ir3_int_w_r_sig);

    mpIRCTG3->ir3_calc_u_r(ir3_calc_u_r_sig);
    mpINT->irctg3_ucmp_trig(ir3_calc_u_r_sig);

    mpIRCTG3->ir3_calc_v_r(ir3_calc_v_r_sig);
    mpINT->irctg3_vcmp_trig(ir3_calc_v_r_sig);

    mpIRCTG3->ir3_calc_w_r(ir3_calc_w_r_sig);
    mpINT->irctg3_wcmp_trig(ir3_calc_w_r_sig);

    mpIRCTG3->ir3_pat_u_r(ir3_pat_u_r_sig);
    mpINT->irctg3_upatout_reg(ir3_pat_u_r_sig);

    mpIRCTG3->ir3_pat_v_r(ir3_pat_v_r_sig);
    mpINT->irctg3_vpatout_reg(ir3_pat_v_r_sig);

    mpIRCTG3->ir3_pat_w_r(ir3_pat_w_r_sig);
    mpINT->irctg3_wpatout_reg(ir3_pat_w_r_sig);
}

// Description: Destructor of EMU3S_HWCORE model
// Ref: {}
EMU3S_HWCORE::~EMU3S_HWCORE()
{
    for (unsigned int i = 0; i < emINTERRUPT_NUM; i++)
    {
        delete emu_int[i];
    }
    delete mpEMU3S_HWCORE_Func;
    delete mpHWCORE_HANDSHAKE_Func;
    delete mpINPUT;
    delete mpIIR;
    delete mpMEASURE;
    delete mpRCTG;
    delete mpRSLVIDE;
    delete mpADTRG;
    delete mpPI;
    delete mpPWM;
    delete mpIRCTG3;
    delete mpINT;
    mSubIPMap.clear();
    mFuncMaxMap.clear();
    mPower2Map.clear();
    mStateMap.clear();
}

// Description: Initialize Maps
// Ref: {}
void EMU3S_HWCORE::Initialize_Maps(void)
{
    mSubIPMap.insert(std::pair<std::string, unsigned int>("EMU3S_HWCORE", emEMU3S_HWCORE));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("INPUT", emINPUT));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("IIR", emIIR));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("MEASURE", emMEASURE));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("RCTG", emRCTG));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("HANDSHAKE", emHANDSHAKE));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("RSLVIDE", emRSLVIDE));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("ADTRG", emADTRG));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("PI", emPI));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("PWM", emPWM));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("IRCTG3", emIRCTG3));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("INT", emINT));
    mSubIPMap.insert(std::pair<std::string, unsigned int>("VRFBUF", emVRFBUF));

    mFuncMaxMap.insert(std::pair<unsigned int, unsigned int>(emINPUT, 4));
    mFuncMaxMap.insert(std::pair<unsigned int, unsigned int>(emPI, 2));
    mFuncMaxMap.insert(std::pair<unsigned int, unsigned int>(emPWM, 11));
    mFuncMaxMap.insert(std::pair<unsigned int, unsigned int>(emRSLVIDE, 4));

    mPower2Map.insert(std::pair<unsigned int, unsigned int>(1, 0));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(2, 1));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(4, 2));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(8, 3));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(16, 4));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(32, 5));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(64, 6));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(128, 7));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(256, 8));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(512, 9));
    mPower2Map.insert(std::pair<unsigned int, unsigned int>(1024, 10));

    mStateMap.insert(std::pair<unsigned int, unsigned int>(emINPUT, emIDLE));
    mStateMap.insert(std::pair<unsigned int, unsigned int>(emPI, emIDLE));
    mStateMap.insert(std::pair<unsigned int, unsigned int>(emPWM, emIDLE));
    mStateMap.insert(std::pair<unsigned int, unsigned int>(emRSLVIDE, emIDLE));
}

// Description: Initialize variables related to SSCG domain
// Ref: {}
void EMU3S_HWCORE::InitVarSSCG(void)
{
    mEDCErrSedPclk = false;
    mEDCErrDedPclk = false;

    mRCTGStartSP = false;
    mPWMStartSP = false;
    mPIStartSP = false;
    mINPUTStartSP = false;

    mFuncAWait = false;
    mFuncBWait = false;
    
    mRCTGEndSP = false;
    mINPUTEndSP = false;
    
    mPWMTRGNUMCnt = 0;
    mPITRGNUMCnt = 0;
    mINTRGNUMCnt = 0;
    mADTRGNUMCnt = 0;

    mStateMap[emINPUT] = emIDLE;
    mStateMap[emPI] = emIDLE;
    mStateMap[emPWM] = emIDLE;
    mStateMap[emRSLVIDE] = emIDLE;

    mPWMFuncEnd_1Hot = 0;
    mPIFuncEnd_1Hot = 0;
    mINPUTFuncEnd_1Hot = 0;
    mRSLVIDEFuncEnd_1Hot = 0;
    
    mAngComp0Active = false;
    mADEndTrigActive = false;
    mIIRCh0EndActive = false;
    mIIRCh1EndActive = false;
    mIIRCh2EndActive = false;

    FUNCIDLEGRPA0 = 0;
    FUNCIDLEGRPA1 = 0;
    FUNCIDLEGRPA2 = 0;
    FUNCWAITGRPA = 0;
    FUNCWAITGRPB = 0;
}

// Description: Initialize variables related to CLEAN domain
// Ref: {}
void EMU3S_HWCORE::InitVarCLEAN(void)
{
    mEDCErrSedCclk = false;
    mEDCErrDedCclk = false;
    
    mADCStart = false;

    mTstiup = false;
    mTstivp = false;
    mTstiwp = false;

    mTstwecmp = false;
    mTstwdcmp0 = 0;
    mTstwdcmpu = 0;
    mTstwdcmpv = 0;
    mTstwdcmpw = 0;
}

// Description: Initialize variables 
// Ref: {}
void EMU3S_HWCORE::Initialize(void)
{
    Initialize_Maps();
    InitVarSSCG();
    InitVarCLEAN();
}

// Description: Initialize ports/signals related to SSCG domain
// Ref: {}
void EMU3S_HWCORE::InitSSCGPort(void)
{
    // External
    SFERC.initialize(0x33);
    edcerr_sed_pclk.initialize(false);
    edcerr_ded_pclk.initialize(false);
    for (unsigned int i = 0; i < emINTERRUPT_NUM; i++)
    {
        emu_int[i]->initialize(false);
    }

    // Internal
    output_rctg_start_sp_sig.write(false);
    pi_start_sp_sig.write(false);
    output_pwm_start_sp_sig.write(false);
    input_start_sp_sig.write(false);
    funca_wait_int_sig.write(false);
    funcb_wait_int_sig.write(false);
    CTR_sig.write(0);
    REFCTR_sig.write(0);
    AHBPRT_sig.write(0x0D);
    TBLADR_sig.write(0xFFFFFFFC);
    IPTRGCTR_sig.write(0);
    IPTRGNUM_sig.write(0);
    FUNCIDLEGRPA0_sig.write(0);
    FUNCIDLEGRPA1_sig.write(0);
    FUNCIDLEGRPA2_sig.write(0);
    FUNCIDLEGRPB_sig.write(0);
    FUNCWAITGRPA_sig.write(0);
    FUNCWAITGRPB_sig.write(0);
    FUNCFLGRPA0_sig.write(0);
    FUNCFLGRPA1_sig.write(0);
    FUNCFLGRPA2_sig.write(0);
    FUNCFLGRPB_sig.write(0);
    ADSEL_sig.write(0);
}

// Description: Initialize ports/signals related to CLEAN domain
// Ref: {}
void EMU3S_HWCORE::InitCLEANPort(void)
{
    // External
    edcerr_sed_cclk.initialize(false);
    edcerr_ded_cclk.initialize(false);

    adc_start.initialize(false);
    adc1_start.initialize(false);

    tstwdcmp0.initialize(0);
    tstwdcmpu.initialize(0);
    tstwdcmpv.initialize(0);
    tstwdcmpw.initialize(0);
    tstwecmp.initialize(false);
    tstiup.initialize(false);
    tstivp.initialize(false);
    tstiwp.initialize(false);

    // Internal
}

// Description: Initialize ports/signals
// Ref: {}
void EMU3S_HWCORE::InitPort(void)
{
    InitSSCGPort();
    InitCLEANPort();
    PCLK_sig.write(0);
    CCLK_sig.write(0);
    PRESETn_sig.write(true);
    rst_n_sig.write(false);
    rst_clean_n_sig.write(false);
}

// Description: Initialize SystemC Process
// Ref: {}
void EMU3S_HWCORE::InitProcess(void)
{
    // Clock/Reset
    SC_METHOD(MonitorPCLKMethod);
    sensitive << PCLK;

    SC_METHOD(MonitorCCLKMethod);
    sensitive << CCLK;

    SC_METHOD(MonitorPRESETnMethod);
    sensitive << PRESETn;

    SC_METHOD(MonitorCRESETnMethod);
    sensitive << CRESETn;

    SC_METHOD(HandlePRESETnCmdMethod);
    dont_initialize();
    sensitive << mPRESETnCmdEvent;

    SC_METHOD(CancelPRESETnCmdMethod);
    dont_initialize();
    sensitive << mPRESETnCmdCancelEvent;

    SC_METHOD(HandleCRESETnCmdMethod);
    dont_initialize();
    sensitive << mCRESETnCmdEvent;

    SC_METHOD(CancelCRESETnCmdMethod);
    dont_initialize();
    sensitive << mCRESETnCmdCancelEvent;

    SC_METHOD(PRESETnSigWriteMethod);
    dont_initialize();
    sensitive << mPRESETnSigWriteEvent;

    SC_METHOD(CRESETnResetMethod);
    dont_initialize();
    sensitive << mCRESETnResetEvent;

    SC_METHOD(WriteSSCGSWResetMethod);
    dont_initialize();
    sensitive << mWriteSSCGSWResetEvent;
    
    SC_METHOD(WriteCleanSWResetMethod);
    dont_initialize();
    sensitive << mWriteCleanSWResetEvent;
    
    // RDC Selector    
    SC_METHOD(MonitorRdcZMethod);
    sensitive << rdc_z;
    sensitive << rdc1_z;
    
    SC_METHOD(MonitorRdcPhiMethod);
    sensitive << rdc_phi;
    sensitive << rdc1_phi;
    
    SC_METHOD(MonitorRdcHighOmegaMethod);
    sensitive << rdc_high_omega;
    sensitive << rdc1_high_omega;
    
    // ADC Selector
    SC_METHOD(MonitorADCStartMethod);
    dont_initialize();
    sensitive << adc_in_start_sig;

    SC_METHOD(MonitorADCGrEndMethod);
    sensitive << adc_grpend;
    sensitive << adc1_grpend;

    SC_METHOD(MonitorADCCh0EndMethod);
    sensitive << adc_ch0end;
    sensitive << adc1_ch0end;

    SC_METHOD(MonitorADCCh0DatMethod);
    sensitive << adc_ch0dat;
    sensitive << adc1_ch0dat;

    SC_METHOD(MonitorADCCh1EndMethod);
    sensitive << adc_ch1end;
    sensitive << adc1_ch1end;

    SC_METHOD(MonitorADCCh1DatMethod);
    sensitive << adc_ch1dat;
    sensitive << adc1_ch1dat;

    SC_METHOD(MonitorADCCh2EndMethod);
    sensitive << adc_ch2end;
    sensitive << adc1_ch2end;

    SC_METHOD(MonitorADCCh2DatMethod);
    sensitive << adc_ch2dat;
    sensitive << adc1_ch2dat;

    SC_METHOD(WriteADC0StartMethod);
    dont_initialize();
    sensitive << mWriteADC0StartEvent;

    SC_METHOD(WriteADC1StartMethod);
    dont_initialize();
    sensitive << mWriteADC1StartEvent;

    SC_METHOD(MonitorTSG3InputMethod);
    dont_initialize();
    sensitive << tstpektim;
    sensitive << tstvlytim;
    sensitive << tstadt0;
    sensitive << tstadt1;
    sensitive << tstint3;
    sensitive << tstint4;

    // Rectangle Wave Selection
    SC_METHOD(MonitorRecPatUMethod);
    dont_initialize();
    sensitive << rctg_u_80m_reg_sig;
    sensitive << ir3_pat_u_r_sig;

    SC_METHOD(MonitorRecPatVMethod);
    dont_initialize();
    sensitive << rctg_v_80m_reg_sig;
    sensitive << ir3_pat_v_r_sig;

    SC_METHOD(MonitorRecPatWMethod);
    dont_initialize();
    sensitive << rctg_w_80m_reg_sig;
    sensitive << ir3_pat_w_r_sig;

    SC_METHOD(WriteTstiupMethod);
    dont_initialize();
    sensitive << mWriteTstiupEvent;

    SC_METHOD(WriteTstivpMethod);
    dont_initialize();
    sensitive << mWriteTstivpEvent;

    SC_METHOD(WriteTstiwpMethod);
    dont_initialize();
    sensitive << mWriteTstiwpEvent;

    // PWM Output
    SC_METHOD(MonitorTstwecmpMethod);
    dont_initialize();
    sensitive << tstwecmp_sig;

    SC_METHOD(MonitorTstwdcmp0Method);
    dont_initialize();
    sensitive << tstwdcmp0_sig;

    SC_METHOD(MonitorTstwdcmpuMethod);
    dont_initialize();
    sensitive << tstwdcmpu_sig;

    SC_METHOD(MonitorTstwdcmpvMethod);
    dont_initialize();
    sensitive << tstwdcmpv_sig;

    SC_METHOD(MonitorTstwdcmpwMethod);
    dont_initialize();
    sensitive << tstwdcmpw_sig;

    SC_METHOD(WriteTstwecmpMethod);
    dont_initialize();
    sensitive << mWriteTstwecmpEvent;
    
    SC_METHOD(WriteTstwdcmp0Method);
    dont_initialize();
    sensitive << mWriteTstwdcmp0Event;

    SC_METHOD(WriteTstwdcmpuMethod);
    dont_initialize();
    sensitive << mWriteTstwdcmpuEvent;

    SC_METHOD(WriteTstwdcmpvMethod);
    dont_initialize();
    sensitive << mWriteTstwdcmpvEvent;

    SC_METHOD(WriteTstwdcmpwMethod);
    dont_initialize();
    sensitive << mWriteTstwdcmpwEvent;

    // EDC Errors
    SC_METHOD(AssertEDCErrSedPclkMethod);
    dont_initialize();
    sensitive << mAssertEdcErrSedPclkEvent;
    
    SC_METHOD(DeassertEDCErrSedPclkMethod);
    dont_initialize();
    sensitive << mDeassertEdcErrSedPclkEvent;
    
    SC_METHOD(WriteEDCErrSedPclkMethod);
    dont_initialize();
    sensitive << mWriteEdcErrSedPclkEvent;
    
    SC_METHOD(AssertEDCErrDedPclkMethod);
    dont_initialize();
    sensitive << mAssertEdcErrDedPclkEvent;
    
    SC_METHOD(DeassertEDCErrDedPclkMethod);
    dont_initialize();
    sensitive << mDeassertEdcErrDedPclkEvent;

    SC_METHOD(WriteEDCErrDedPclkMethod);
    dont_initialize();
    sensitive << mWriteEdcErrDedPclkEvent;
    
    SC_METHOD(AssertEDCErrSedCclkMethod);
    dont_initialize();
    sensitive << mAssertEdcErrSedCclkEvent;

    SC_METHOD(DeassertEDCErrSedCclkMethod);
    dont_initialize();
    sensitive << mDeassertEdcErrSedCclkEvent;
    
    SC_METHOD(WriteEDCErrSedCclkMethod);
    dont_initialize();
    sensitive << mWriteEdcErrSedCclkEvent;
    
    SC_METHOD(AssertEDCErrDedCclkMethod);
    dont_initialize();
    sensitive << mAssertEdcErrDedCclkEvent;

    SC_METHOD(DeassertEDCErrDedCclkMethod);
    dont_initialize();
    sensitive << mDeassertEdcErrDedCclkEvent;
    
    SC_METHOD(WriteEDCErrDedCclkMethod);
    dont_initialize();
    sensitive << mWriteEdcErrDedCclkEvent;
    
    // RCTG Start Trigger
    SC_METHOD(MonitorAngComp0SPMethod);
    dont_initialize();
    sensitive << presml_ang_comp0_eq_sp_sig;
    
    SC_METHOD(MonitorRCTGEndSPMethod);
    dont_initialize();
    sensitive << output_rctg_end_sp_reg_sig;
    
    SC_METHOD(AssertRCTGStartSPMethod);
    dont_initialize();
    sensitive << mAssertRCTGStartSPEvent;

    SC_METHOD(DeassertRCTGStartSPMethod);
    dont_initialize();
    sensitive << mDeassertRCTGStartSPEvent;
    
    SC_METHOD(WriteRCTGStartSPMethod);
    dont_initialize();
    sensitive << mWriteRCTGStartSPEvent;

    // PWM Start Trigger
    SC_METHOD(AssertPWMStartSPMethod);
    dont_initialize();
    sensitive << mAssertPWMStartSPEvent;

    SC_METHOD(DeassertPWMStartSPMethod);
    dont_initialize();
    sensitive << mDeassertPWMStartSPEvent;

    SC_METHOD(WritePWMStartSPMethod);
    dont_initialize();
    sensitive << mWritePWMStartSPEvent;

    // PI Start Trigger
    SC_METHOD(AssertPIStartSPMethod);
    dont_initialize();
    sensitive << mAssertPIStartSPEvent;

    SC_METHOD(DeassertPIStartSPMethod);
    dont_initialize();
    sensitive << mDeassertPIStartSPEvent;

    SC_METHOD(WritePIStartSPMethod);
    dont_initialize();
    sensitive << mWritePIStartSPEvent;

    // Input Start Trigger
    SC_METHOD(MonitorADTRGEndTrigMethod);
    dont_initialize();
    sensitive << adtrg_end_trig_sig;
    
    SC_METHOD(MonitorIIRCh0EndMethod);
    dont_initialize();
    sensitive << iir_end0_sig;
    
    SC_METHOD(MonitorIIRCh1EndMethod);
    dont_initialize();
    sensitive << iir_end1_sig;
    
    SC_METHOD(MonitorIIRCh2EndMethod);
    dont_initialize();
    sensitive << iir_end2_sig;
    
    SC_METHOD(AssertINPUTStartSPMethod);
    dont_initialize();
    sensitive << mAssertINPUTStartSPEvent;

    SC_METHOD(DeassertINPUTStartSPMethod);
    dont_initialize();
    sensitive << mDeassertINPUTStartSPEvent;
    
    SC_METHOD(WriteINPUTStartSPMethod);
    dont_initialize();
    sensitive << mWriteINPUTStartSPEvent;
    
    // Input State
    SC_METHOD(MonitorINPUTEndSPMethod);
    dont_initialize();
    sensitive << input_end_sp_reg_sig;

    SC_METHOD(MonitorINPUTFuncStMethod);
    dont_initialize();
    sensitive << input_func_st_sig;
    
    SC_METHOD(MonitorINPUTFuncEndMethod);
    dont_initialize();
    sensitive << input_func_end_sig;
    
    SC_METHOD(MonitorINPUTFuncWaitMethod);
    dont_initialize();
    sensitive << input_func_wait_sig;
    
    SC_METHOD(MonitorINPUTFuncBusyMethod);
    dont_initialize();
    sensitive << input_func_busy_sig;

    // PI State
    SC_METHOD(MonitorPIEndSPMethod);
    dont_initialize();
    sensitive << pi_end_sp_reg_sig;

    SC_METHOD(MonitorPIFuncStMethod);
    dont_initialize();
    sensitive << pi_func_st_sig;

    SC_METHOD(MonitorPIFuncEndMethod);
    dont_initialize();
    sensitive << pi_func_end_sig;

    SC_METHOD(MonitorPIFuncWaitMethod);
    dont_initialize();
    sensitive << pi_func_wait_sig;

    SC_METHOD(MonitorPIFuncBusyMethod);
    dont_initialize();
    sensitive << pi_func_busy_sig;

    // PWM State
    SC_METHOD(MonitorPWMEndSPMethod);
    dont_initialize();
    sensitive << output_pwm_end_sp_reg_sig;

    SC_METHOD(MonitorPWMFuncStMethod);
    dont_initialize();
    sensitive << pwm_func_st_sig;

    SC_METHOD(MonitorPWMFuncEndMethod);
    dont_initialize();
    sensitive << pwm_func_end_sig;

    SC_METHOD(MonitorPWMFuncWaitMethod);
    dont_initialize();
    sensitive << pwm_func_wait_sig;

    SC_METHOD(MonitorPWMFuncBusyMethod);
    dont_initialize();
    sensitive << pwm_func_busy_sig;

    // RSLVIDE State
    SC_METHOD(MonitorRSLVIDEFuncStMethod);
    dont_initialize();
    sensitive << rslv_func_st_sig;

    SC_METHOD(MonitorRSLVIDEFuncEndMethod);
    dont_initialize();
    sensitive << rslv_func_end_sig;

    SC_METHOD(MonitorRSLVIDEFuncWaitMethod);
    dont_initialize();
    sensitive << rslv_func_wait_sig;

    SC_METHOD(MonitorRSLVIDEFuncBusyMethod);
    dont_initialize();
    sensitive << rslv_func_busy_sig;

    // Assert/Deassert Func A/B Wait Int
    SC_METHOD(AssertFuncAWaitMethod);
    dont_initialize();
    sensitive << mAssertFuncAWaitEvent;

    SC_METHOD(DeassertFuncAWaitMethod);
    dont_initialize();
    sensitive << mDeassertFuncAWaitEvent;

    SC_METHOD(WriteFuncAWaitMethod);
    dont_initialize();
    sensitive << mWriteFuncAWaitEvent;

    SC_METHOD(AssertFuncBWaitMethod);
    dont_initialize();
    sensitive << mAssertFuncBWaitEvent;

    SC_METHOD(DeassertFuncBWaitMethod);
    dont_initialize();
    sensitive << mDeassertFuncBWaitEvent;

    SC_METHOD(WriteFuncBWaitMethod);
    dont_initialize();
    sensitive << mWriteFuncBWaitEvent;

    // Montior Other Signals
    SC_METHOD(MonitorIptrgMonAdtrgNumMethod);
    dont_initialize();
    sensitive << IPTRGMON_ADTRGNUM_sig;
    
    // Write Other Internal Signals
    SC_METHOD(WriteCTRMethod);
    dont_initialize();
    sensitive << mWriteCTREvent;
    
    SC_METHOD(WriteREFCTRMethod);
    dont_initialize();
    sensitive << mWriteREFCTREvent;

    SC_METHOD(WriteAHBPRTMethod);
    dont_initialize();
    sensitive << mWriteAHBPRTEvent;

    SC_METHOD(WriteTBLADRMethod);
    dont_initialize();
    sensitive << mWriteTBLADREvent;

    SC_METHOD(WriteIPTRGCTRMethod);
    dont_initialize();
    sensitive << mWriteIPTRGCTREvent;
    
    SC_METHOD(WriteIPTRGNUMMethod);
    dont_initialize();
    sensitive << mWriteIPTRGNUMEvent;

    SC_METHOD(WriteFUNCIDLEGRPA0Method);
    dont_initialize();
    sensitive << mWriteFUNCIDLEGRPA0Event;
    
    SC_METHOD(WriteFUNCIDLEGRPA1Method);
    dont_initialize();
    sensitive << mWriteFUNCIDLEGRPA1Event;

    SC_METHOD(WriteFUNCIDLEGRPA2Method);
    dont_initialize();
    sensitive << mWriteFUNCIDLEGRPA2Event;

    SC_METHOD(WriteFUNCIDLEGRPBMethod);
    dont_initialize();
    sensitive << mWriteFUNCIDLEGRPBEvent;

    SC_METHOD(WriteFUNCWAITGRPAMethod);
    dont_initialize();
    sensitive << mWriteFUNCWAITGRPAEvent;

    SC_METHOD(WriteFUNCWAITGRPBMethod);
    dont_initialize();
    sensitive << mWriteFUNCWAITGRPBEvent;

    SC_METHOD(WriteFUNCFLGRPA0Method);
    dont_initialize();
    sensitive << mWriteFUNCFLGRPA0Event;

    SC_METHOD(WriteFUNCFLGRPA1Method);
    dont_initialize();
    sensitive << mWriteFUNCFLGRPA1Event;

    SC_METHOD(WriteFUNCFLGRPA2Method);
    dont_initialize();
    sensitive << mWriteFUNCFLGRPA2Event;

    SC_METHOD(WriteFUNCFLGRPBMethod);
    dont_initialize();
    sensitive << mWriteFUNCFLGRPBEvent;

    SC_METHOD(WriteADSELMethod);
    dont_initialize();
    sensitive << mWriteADSELEvent;

    SC_METHOD(WriteSFERCMethod);
    dont_initialize();
    sensitive << mWriteSFERCEvent;
}

// Description: Cancel Events related to SSCG domain
// Ref: {}
void EMU3S_HWCORE::CancelSSCGEvents(void)
{
    mAssertEdcErrSedPclkEvent.cancel();
    mDeassertEdcErrSedPclkEvent.cancel();
    mWriteEdcErrSedPclkEvent.cancel();
    mAssertEdcErrDedPclkEvent.cancel();
    mDeassertEdcErrDedPclkEvent.cancel();
    mWriteEdcErrDedPclkEvent.cancel();

    mAssertRCTGStartSPEvent.cancel();
    mDeassertRCTGStartSPEvent.cancel();
    mWriteRCTGStartSPEvent.cancel();

    mAssertPWMStartSPEvent.cancel();      
    mDeassertPWMStartSPEvent.cancel();     
    mWritePWMStartSPEvent.cancel();

    mAssertPIStartSPEvent.cancel();        
    mDeassertPIStartSPEvent.cancel();      
    mWritePIStartSPEvent.cancel();

    mAssertINPUTStartSPEvent.cancel();
    mDeassertINPUTStartSPEvent.cancel();
    mWriteINPUTStartSPEvent.cancel();
    
    mAssertFuncAWaitEvent.cancel();       
    mDeassertFuncAWaitEvent.cancel();      
    mWriteFuncAWaitEvent.cancel();

    mAssertFuncBWaitEvent.cancel();        
    mDeassertFuncBWaitEvent.cancel();      
    mWriteFuncBWaitEvent.cancel();

    mWriteCTREvent.cancel();
    mWriteREFCTREvent.cancel();
    mWriteAHBPRTEvent.cancel();
    mWriteTBLADREvent.cancel();
    mWriteIPTRGCTREvent.cancel();
    mWriteIPTRGNUMEvent.cancel();
    mWriteFUNCIDLEGRPA0Event.cancel();
    mWriteFUNCIDLEGRPA1Event.cancel();
    mWriteFUNCIDLEGRPA2Event.cancel();
    mWriteFUNCIDLEGRPBEvent.cancel();
    mWriteFUNCWAITGRPAEvent.cancel();
    mWriteFUNCWAITGRPBEvent.cancel();
    mWriteFUNCFLGRPA0Event.cancel();
    mWriteFUNCFLGRPA1Event.cancel();
    mWriteFUNCFLGRPA2Event.cancel();
    mWriteFUNCFLGRPBEvent.cancel();
    mWriteADSELEvent.cancel();
    mWriteSFERCEvent.cancel();
}

// Description: Cancel Events related to CLEAN domain
// Ref: {}
void EMU3S_HWCORE::CancelCLEANEvents(void)
{
    mAssertEdcErrSedCclkEvent.cancel();
    mDeassertEdcErrSedCclkEvent.cancel();
    mWriteEdcErrSedCclkEvent.cancel();
    mAssertEdcErrDedCclkEvent.cancel();
    mDeassertEdcErrDedCclkEvent.cancel();
    mWriteEdcErrDedCclkEvent.cancel();

    mWriteADC0StartEvent.cancel();
    mWriteADC1StartEvent.cancel();

    mWriteTstiupEvent.cancel();
    mWriteTstivpEvent.cancel();
    mWriteTstiwpEvent.cancel();

    mWriteTstwdcmp0Event.cancel();
    mWriteTstwdcmpuEvent.cancel();
    mWriteTstwdcmpvEvent.cancel();
    mWriteTstwdcmpwEvent.cancel();
}

// Description: Reset ports/signals related to SSCG domain 
// Ref: {}
void EMU3S_HWCORE::ResetSSCGPort(void)
{
    // External
    mWriteSFERCEvent.notify();
    mWriteEdcErrSedPclkEvent.notify();
    mWriteEdcErrDedPclkEvent.notify();
    // Internal
    mWriteRCTGStartSPEvent.notify();
    mWritePWMStartSPEvent.notify();
    mWritePIStartSPEvent.notify();
    mWriteINPUTStartSPEvent.notify();
    mWriteFuncAWaitEvent.notify();
    mWriteFuncBWaitEvent.notify();
    mWriteCTREvent.notify();
    mWriteREFCTREvent.notify();
    mWriteAHBPRTEvent.notify();
    mWriteTBLADREvent.notify();
    mWriteIPTRGCTREvent.notify();
    mWriteIPTRGNUMEvent.notify();
    mWriteFUNCIDLEGRPA0Event.notify();
    mWriteFUNCIDLEGRPA1Event.notify();
    mWriteFUNCIDLEGRPA2Event.notify();
    mWriteFUNCIDLEGRPBEvent.notify();
    mWriteFUNCWAITGRPAEvent.notify();
    mWriteFUNCWAITGRPBEvent.notify();
    mWriteFUNCFLGRPA0Event.notify();
    mWriteFUNCFLGRPA1Event.notify();
    mWriteFUNCFLGRPA2Event.notify();
    mWriteFUNCFLGRPBEvent.notify();
    mWriteADSELEvent.notify();
}

// Description: Reset ports/signals related to CLEAN domain 
// Ref: {}
void EMU3S_HWCORE::ResetCLEANPort(void)
{
    // External
    mWriteEdcErrSedCclkEvent.notify();
    mWriteEdcErrDedCclkEvent.notify();

    mWriteADC0StartEvent.notify();
    mWriteADC1StartEvent.notify();

    mWriteTstiupEvent.notify();
    mWriteTstivpEvent.notify();
    mWriteTstiwpEvent.notify();

    mWriteTstwecmpEvent.notify();
    mWriteTstwdcmp0Event.notify();
    mWriteTstwdcmpuEvent.notify();
    mWriteTstwdcmpvEvent.notify();
    mWriteTstwdcmpwEvent.notify();
    // Internal

}

// Description: Monitor PCLK clock
// Ref: {EMU3S_HWCORE_UD_Feature_002, EMU3S_HWCORE_UD_Flow_007}
void EMU3S_HWCORE::MonitorPCLKMethod()
{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}

// Description: Monitor CCLK clock
// Ref: {EMU3S_HWCORE_UD_Feature_002, EMU3S_HWCORE_UD_Flow_007}
void EMU3S_HWCORE::MonitorCCLKMethod()
{
    sc_dt::uint64 freq_value = CCLK.read();
    SetCLKFreq("CCLK", freq_value, "Hz");
}

// Description: Monitor PRESETn port
// Ref: {EMU3S_HWCORE_UD_Feature_031, EMU3S_HWCORE_UD_Direction_037, EMU3S_HWCORE_UD_Direction_038}
// Ref: {EMU3S_HWCORE_UD_Flow_001, EMU3S_HWCORE_UD_Flow_002, EMU3S_HWCORE_UD_Flow_003}
void EMU3S_HWCORE::MonitorPRESETnMethod()
{
    bool reset = PRESETn.read();
    if (reset == vpcl::emResetActive) {
        mIsPRESETnHardActive = true;
        re_printf("info", "The reset port PRESETn is asserted.\n");
        mIsPRESETnActive = true;
        mPRESETnSigWriteEvent.notify(SC_ZERO_TIME);
    }
    else {
        mIsPRESETnHardActive = false;
        re_printf("info", "The reset port PRESETn is de-asserted.\n");
        mIsPRESETnActive = false;
        mPRESETnSigWriteEvent.notify(SC_ZERO_TIME);
    }
    //Cancel AssertReset command when reset port is active
    if (mIsPRESETnCmdReceive && mIsPRESETnHardActive) {
        mPRESETnCmdEvent.cancel();
        mPRESETnCmdCancelEvent.cancel();
        mIsPRESETnCmdReceive = false;
        mIsPRESETnCmdActive = false;
    }
}

 // Description: Monitor CRESETn port
 // Ref: {EMU3S_HWCORE_UD_Feature_031, EMU3S_HWCORE_UD_Direction_037, EMU3S_HWCORE_UD_Direction_038}
 // Ref: {EMU3S_HWCORE_UD_Flow_004, EMU3S_HWCORE_UD_Flow_005, EMU3S_HWCORE_UD_Flow_006}
void EMU3S_HWCORE::MonitorCRESETnMethod()
{
    bool reset = CRESETn.read();
    if (reset == vpcl::emResetActive) {
        mIsCRESETnHardActive = true;
        re_printf("info", "The reset port CRESETn is asserted.\n");
        mIsCRESETnActive = true;
        mCRESETnResetEvent.notify(SC_ZERO_TIME);
    }
    else {
        mIsCRESETnHardActive = false;
        re_printf("info", "The reset port CRESETn is de-asserted.\n");
        mIsCRESETnActive = false;
        mCRESETnResetEvent.notify(SC_ZERO_TIME);
    }
    //Cancel AssertReset command when reset port is active
    if (mIsCRESETnCmdReceive && mIsCRESETnHardActive) {
        mCRESETnCmdEvent.cancel();
        mCRESETnCmdCancelEvent.cancel();
        mIsCRESETnCmdReceive = false;
        mIsCRESETnCmdActive = false;
    }
}

 // Description: Process PRESETn function when PRESETn command is active
 // Ref: {EMU3S_HWCORE_UD_Direction_035}
void EMU3S_HWCORE::HandlePRESETnCmdMethod()
{
    mIsPRESETnCmdActive = true;
    re_printf("info", "The model is reset by AssertReset command of PRESETn.\n");
    mIsPRESETnActive = mIsPRESETnCmdActive;
    mPRESETnSigWriteEvent.notify(SC_ZERO_TIME);
    mPRESETnCmdCancelEvent.notify(mPRESETnCmdPeriod, SC_NS);
}

// Description: Process CRESETn function when CRESETn command is active
// Ref: {EMU3S_HWCORE_UD_Direction_035}
void EMU3S_HWCORE::HandleCRESETnCmdMethod()
{
    mIsCRESETnCmdActive = true;
    re_printf("info", "The model is reset by AssertReset command of CRESETn.\n");
    mIsCRESETnActive = mIsCRESETnCmdActive;
    mCRESETnResetEvent.notify(SC_ZERO_TIME);
    mCRESETnCmdCancelEvent.notify(mCRESETnCmdPeriod, SC_NS);
}

// Description: Cancel PRESETn function triggered by PRESETn command
// Ref: {EMU3S_HWCORE_UD_Direction_034}
void EMU3S_HWCORE::CancelPRESETnCmdMethod()
{
    mIsPRESETnCmdActive = false;
    mIsPRESETnCmdReceive = false;
    re_printf("info", "Reset period of PRESETn is over.\n");
    mIsPRESETnActive = mIsPRESETnCmdActive;
    mPRESETnSigWriteEvent.notify(SC_ZERO_TIME);
}

// Description: Cancel CRESETn function triggered by CRESETn command
// Ref: {EMU3S_HWCORE_UD_Direction_034}
void EMU3S_HWCORE::CancelCRESETnCmdMethod()
{
    mIsCRESETnCmdActive = false;
    mIsCRESETnCmdReceive = false;
    re_printf("info", "Reset period of CRESETn is over.\n");
    mIsCRESETnActive = mIsCRESETnCmdActive;
    mCRESETnResetEvent.notify(SC_ZERO_TIME);
}

// Description: Write PRESETn signal according to reset status
// Ref: {EMU3S_HWCORE_UD_Flow_001}
void EMU3S_HWCORE::PRESETnSigWriteMethod()
{
    // Forward reset signal to sub-ips
    PRESETn_sig.write(mIsPRESETnActive == vpcl::emResetActive);
    mpEMU3S_HWCORE_Func->EnableReset(mIsPRESETnActive);
    mpHWCORE_HANDSHAKE_Func->EnableReset(mIsPRESETnActive);
    if (mIsPRESETnActive) 
    {
        // Cancel events
        CancelSSCGEvents();
        // Initialize variable
        InitVarSSCG();
        // Reset Ports/Signals
        ResetSSCGPort();
        // Issue SW Reset
        WriteSWReset(mIsPRESETnActive);
    }
}

// Description: CRESETn reset operation
// Ref: {EMU3S_HWCORE_UD_Flow_004}
void EMU3S_HWCORE::CRESETnResetMethod()
{
    if (mIsCRESETnActive) {
        // Cancel events
        CancelCLEANEvents();
        // Initialize variable
        InitVarCLEAN();
        // Reset Ports/Signals
        ResetCLEANPort();
    }
}

// Description: Trigger/Disable SW Reset
// Ref: {}
void EMU3S_HWCORE::WriteSWReset(bool is_active)
{
    IsSWReset = is_active;
    if (is_active)
    {
        CancelSSCGEvents();
        CancelCLEANEvents();
        InitVarSSCG();
        InitVarCLEAN();
    }
    mpEMU3S_HWCORE_Func->EnableSWReset(is_active);
    mpHWCORE_HANDSHAKE_Func->EnableReset(is_active);
    if (is_active)
    {
        // Reset Ports/Signals
        ResetSSCGPort();
        ResetCLEANPort();
    }

    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mTimeCCLKChangePoint);
    mWriteCleanSWResetEvent.notify(assert_time, mTimeResolutionUnit);
    assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWriteSSCGSWResetEvent.notify(assert_time, mTimeResolutionUnit);
}
// Description: Write rst_n signal according to reset status
// Ref: {}
void EMU3S_HWCORE::WriteSSCGSWResetMethod()
{
    rst_n_sig.write(!IsSWReset);
}
// Description: Write rst_clean_n signal according to reset status
// Ref: {}
void EMU3S_HWCORE::WriteCleanSWResetMethod()
{
    rst_clean_n_sig.write(!IsSWReset);
}
//========================================================
//============       RDC Selector      ===================
//========================================================
// Description: Monitor rdc_z Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_025}
void EMU3S_HWCORE::MonitorRdcZMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int RDCSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "RDCSEL");
        if (RDCSEL == 1)
        {
            rdc_out_z_sig.write(rdc1_z.read());
        }
        else
        {
            rdc_out_z_sig.write(rdc_z.read());
        }
    }
}
// Description: Monitor rdc_phi Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_025}
void EMU3S_HWCORE::MonitorRdcPhiMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int RDCSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "RDCSEL");
        if (RDCSEL == 1)
        {
            rdc_out_phi_sig.write((unsigned short)rdc1_phi.read());
        }
        else
        {
            rdc_out_phi_sig.write((unsigned short)rdc_phi.read());
        }
    }
}
// Description: Monitor rdc_high_omega Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_025}
void EMU3S_HWCORE::MonitorRdcHighOmegaMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int RDCSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "RDCSEL");
        if (RDCSEL == 1)
        {
            rdc_out_high_omega_sig.write((signed int)rdc1_high_omega.read());
        }
        else
        {
            rdc_out_high_omega_sig.write((signed int)rdc_high_omega.read());
        }
    }
}

//========================================================
//============       ADC Selector      ===================
//========================================================
// Description: Monitor adc_in_start_sig Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_047}
void EMU3S_HWCORE::MonitorADCStartMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int ADSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "ADSEL");
        mADCStart = adc_in_start_sig.read();
        if (ADSEL == 1)
        {
            mWriteADC1StartEvent.notify();
        }
        else
        {
            mWriteADC0StartEvent.notify();
        }
    }
}
// Description: Monitor adc_grpend Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_047}
void EMU3S_HWCORE::MonitorADCGrEndMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int ADSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "ADSEL");
        if (ADSEL == 1)
        {
            adc_out_grpend_sig.write(adc1_grpend.read());
        }
        else
        {
            adc_out_grpend_sig.write(adc_grpend.read());
        }
    }
}
// Description: Monitor adc_ch0end Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_047}
void EMU3S_HWCORE::MonitorADCCh0EndMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int ADSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "ADSEL");
        if (ADSEL == 1)
        {
            adc_out_ch0end_sig.write(adc1_ch0end.read());
        }
        else
        {
            adc_out_ch0end_sig.write(adc_ch0end.read());
        }
    }
}
// Description: Monitor adc_ch0dat Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_047}
void EMU3S_HWCORE::MonitorADCCh0DatMethod()
{
    unsigned int ADSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "ADSEL");
    if (ADSEL == 1)
    {
        adc_out_ch0dat_sig.write((unsigned short) adc1_ch0dat.read());
    }
    else
    {
        adc_out_ch0dat_sig.write((unsigned short) adc_ch0dat.read());
    }
}
// Description: Monitor adc_ch1end Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_047}
void EMU3S_HWCORE::MonitorADCCh1EndMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int ADSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "ADSEL");
        if (ADSEL == 1)
        {
            adc_out_ch1end_sig.write(adc1_ch1end.read());
        }
        else
        {
            adc_out_ch1end_sig.write(adc_ch1end.read());
        }
    }
}
// Description: Monitor adc_ch1dat Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_047}
void EMU3S_HWCORE::MonitorADCCh1DatMethod()
{
    unsigned int ADSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "ADSEL");
    if (ADSEL == 1)
    {
        adc_out_ch1dat_sig.write((unsigned short) adc1_ch1dat.read());
    }
    else
    {
        adc_out_ch1dat_sig.write((unsigned short) adc_ch1dat.read());
    }
}
// Description: Monitor adc_ch2end Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_047}
void EMU3S_HWCORE::MonitorADCCh2EndMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int ADSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "ADSEL");
        if (ADSEL == 1)
        {
            adc_out_ch2end_sig.write(adc1_ch2end.read());
        }
        else
        {
            adc_out_ch2end_sig.write(adc_ch2end.read());
        }
    }
}
// Description: Monitor adc_ch2dat Input
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_047}
void EMU3S_HWCORE::MonitorADCCh2DatMethod()
{
    unsigned int ADSEL = mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "ADSEL");
    if (ADSEL == 1)
    {
        adc_out_ch2dat_sig.write((unsigned short) adc1_ch2dat.read());
    }
    else
    {
        adc_out_ch2dat_sig.write((unsigned short) adc_ch2dat.read());
    }
}
// Description: Write adc_start port
// Ref: {}
void EMU3S_HWCORE::WriteADC0StartMethod()
{
    adc_start.write(mADCStart);
}
// Description: Write adc1_start port
// Ref: {}
void EMU3S_HWCORE::WriteADC1StartMethod()
{
    adc1_start.write(mADCStart);
}

// Description: Monitor TSG3 Input
// Ref: {}
void EMU3S_HWCORE::MonitorTSG3InputMethod(void)
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        tstpektim_sig.write(tstpektim.read());
        tstvlytim_sig.write(tstvlytim.read());
        tstadt0_sig.write(tstadt0.read());
        tstadt1_sig.write(tstadt1.read());
        tstint3_sig.write(tstint3.read());
        tstint4_sig.write(tstint4.read());
    }
}

//========================================================
//============   Rectangle Wave Selection    =============
//========================================================
// Description: Monitor Rectange Wave Pattern U Phase
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_050}
void EMU3S_HWCORE::MonitorRecPatUMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int RECMD = mpEMU3S_HWCORE_Func->GetRegVal("RECMD", "RECMD");
        if (RECMD == 1)
        {
            mTstiup = ir3_pat_u_r_sig.read();
        }
        else
        {
            mTstiup = rctg_u_80m_reg_sig.read();
        }
        double current_time = sc_time_stamp().to_double();
        double assert_time = CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mTimeCCLKChangePoint);
        mWriteTstiupEvent.notify(assert_time, mTimeResolutionUnit);
    }
}
// Description: Monitor Rectange Wave Pattern V Phase
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_050}
void EMU3S_HWCORE::MonitorRecPatVMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int RECMD = mpEMU3S_HWCORE_Func->GetRegVal("RECMD", "RECMD");
        if (RECMD == 1)
        {
            mTstivp = ir3_pat_v_r_sig.read();
        }
        else
        {
            mTstivp = rctg_v_80m_reg_sig.read();
        }
        double current_time = sc_time_stamp().to_double();
        double assert_time = CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mTimeCCLKChangePoint);
        mWriteTstivpEvent.notify(assert_time, mTimeResolutionUnit);
    }
}
// Description: Monitor Rectange Wave Pattern W Phase
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002, EMU3S_HWCORE_UD_Flow_050}
void EMU3S_HWCORE::MonitorRecPatWMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        unsigned int RECMD = mpEMU3S_HWCORE_Func->GetRegVal("RECMD", "RECMD");
        if (RECMD == 1)
        {
            mTstiwp = ir3_pat_w_r_sig.read();
        }
        else
        {
            mTstiwp = rctg_w_80m_reg_sig.read();
        }
        double current_time = sc_time_stamp().to_double();
        double assert_time = CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mTimeCCLKChangePoint);
        mWriteTstiwpEvent.notify(assert_time, mTimeResolutionUnit);
    }
}
// Description: Write tstiup port
// Ref: {}
void EMU3S_HWCORE::WriteTstiupMethod()
{
    tstiup.write(mTstiup);
}
// Description: Write tstivp port
// Ref: {}
void EMU3S_HWCORE::WriteTstivpMethod()
{
    tstivp.write(mTstivp);
}
// Description: Write tstiwp port
// Ref: {}
void EMU3S_HWCORE::WriteTstiwpMethod()
{
    tstiwp.write(mTstiwp);
}
//========================================================
//============       PWM Output        ===================
//========================================================
// Description: Monitor tstwecmp_sig
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002}
void EMU3S_HWCORE::MonitorTstwecmpMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        mTstwecmp = tstwecmp_sig.read();
        mWriteTstwecmpEvent.notify();
    }
}
// Description: Monitor tstwdcmp0_sig
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002}
void EMU3S_HWCORE::MonitorTstwdcmp0Method()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        mTstwdcmp0 = (sc_uint<18>)(tstwdcmp0_sig.read() & 0x3FFFF);
        mWriteTstwdcmp0Event.notify();
    }
}
// Description: Monitor tstwdcmpu_sig
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002}
void EMU3S_HWCORE::MonitorTstwdcmpuMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        mTstwdcmpu = (sc_uint<18>)(tstwdcmpu_sig.read() & 0x3FFFF);
        mWriteTstwdcmpuEvent.notify();
    }
}
// Description: Monitor tstwdcmpv_sig
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002}
void EMU3S_HWCORE::MonitorTstwdcmpvMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        mTstwdcmpv = (sc_uint<18>)(tstwdcmpv_sig.read() & 0x3FFFF);
        mWriteTstwdcmpvEvent.notify();
    }
}
// Description: Monitor tstwdcmpw_sig
// Ref: {EMU3S_HWCORE_UD_BlockDiagram_002}
void EMU3S_HWCORE::MonitorTstwdcmpwMethod()
{
    if (!(GetResetStatus("CRESETn") || GetResetStatus("EMUST")))
    {
        mTstwdcmpw = (sc_uint<18>)(tstwdcmpw_sig.read() & 0x3FFFF);
        mWriteTstwdcmpwEvent.notify();
    }
}
// Description: Write tstwecmp port
// Ref: {}
void EMU3S_HWCORE::WriteTstwecmpMethod()
{
    tstwecmp.write(mTstwecmp);
}
// Description: Write tstwdcmp0 port
// Ref: {}
void EMU3S_HWCORE::WriteTstwdcmp0Method()
{
    tstwdcmp0.write(mTstwdcmp0);
}
// Description: Write tstwdcmpu port
// Ref: {}
void EMU3S_HWCORE::WriteTstwdcmpuMethod()
{
    tstwdcmpu.write(mTstwdcmpu);
}
// Description: Write tstwdcmpv port
// Ref: {}
void EMU3S_HWCORE::WriteTstwdcmpvMethod()
{
    tstwdcmpv.write(mTstwdcmpv);
}
// Description: Write tstwdcmpw port
// Ref: {}
void EMU3S_HWCORE::WriteTstwdcmpwMethod()
{   
    tstwdcmpw.write(mTstwdcmpw);
}

//========================================================
//============         EDC Errors      ===================
//========================================================
// Description: Assert edcerr_sed_pclk
// Ref: {}
void EMU3S_HWCORE::AssertEDCErrSedPclkMethod()
{
    mEDCErrSedPclk = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWriteEdcErrSedPclkEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertEdcErrSedPclkEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert edcerr_sed_pclk
// Ref: {}
void EMU3S_HWCORE::DeassertEDCErrSedPclkMethod()
{
    mEDCErrSedPclk = false;
    mWriteEdcErrSedPclkEvent.notify();
}
// Description: Write edcerr_sed_pclk
// Ref: {}
void EMU3S_HWCORE::WriteEDCErrSedPclkMethod()
{
    bool current_value = edcerr_sed_pclk.read();

    if (current_value != mEDCErrSedPclk) {
        edcerr_sed_pclk.write(mEDCErrSedPclk);
        DumpInterruptMsg("edcerr_sed_pclk", mEDCErrSedPclk);
    }
}
// Description: Assert edcerr_ded_pclk
// Ref: {}
void EMU3S_HWCORE::AssertEDCErrDedPclkMethod()
{
    mEDCErrDedPclk = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWriteEdcErrDedPclkEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertEdcErrDedPclkEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert edcerr_ded_pclk
// Ref: {}
void EMU3S_HWCORE::DeassertEDCErrDedPclkMethod()
{
    mEDCErrDedPclk = false;
    mWriteEdcErrDedPclkEvent.notify();
}
// Description: Write edcerr_ded_pclk
// Ref: {}
void EMU3S_HWCORE::WriteEDCErrDedPclkMethod()
{
    bool current_value = edcerr_ded_pclk.read();

    if (current_value != mEDCErrDedPclk) {
        edcerr_ded_pclk.write(mEDCErrDedPclk);
        DumpInterruptMsg("edcerr_ded_pclk", mEDCErrDedPclk);
    }
}
// Description: Assert edcerr_sed_cclk
// Ref: {}
void EMU3S_HWCORE::AssertEDCErrSedCclkMethod()
{
    mEDCErrSedCclk = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mTimeCCLKChangePoint);
    mWriteEdcErrSedCclkEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertEdcErrSedCclkEvent.notify(assert_time + (double)mCCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert edcerr_sed_cclk
// Ref: {}
void EMU3S_HWCORE::DeassertEDCErrSedCclkMethod()
{
    mEDCErrSedCclk = false;
    mWriteEdcErrSedCclkEvent.notify();
}
// Description: Write edcerr_sed_cclk
// Ref: {}
void EMU3S_HWCORE::WriteEDCErrSedCclkMethod()
{
    bool current_value = edcerr_sed_cclk.read();

    if (current_value != mEDCErrSedCclk) {
        edcerr_sed_cclk.write(mEDCErrSedCclk);
        DumpInterruptMsg("edcerr_sed_cclk", mEDCErrSedCclk);
    }
}
// Description: Assert edcerr_ded_cclk
// Ref: {}
void EMU3S_HWCORE::AssertEDCErrDedCclkMethod()
{
    mEDCErrDedCclk = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mTimeCCLKChangePoint);
    mWriteEdcErrDedCclkEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertEdcErrDedCclkEvent.notify(assert_time + (double)mCCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert edcerr_ded_cclk
// Ref: {}
void EMU3S_HWCORE::DeassertEDCErrDedCclkMethod()
{
    mEDCErrDedCclk = false;
    mWriteEdcErrDedCclkEvent.notify();
}
// Description: Write edcerr_ded_cclk
// Ref: {}
void EMU3S_HWCORE::WriteEDCErrDedCclkMethod()
{
    bool current_value = edcerr_ded_cclk.read();

    if (current_value != mEDCErrDedCclk) {
        edcerr_ded_cclk.write(mEDCErrDedCclk);
        DumpInterruptMsg("edcerr_ded_cclk", mEDCErrDedCclk);
    }
}

//========================================================
//============    RCTG Start Trigger   ===================
//========================================================
// Description: Monitor presml_ang_comp0_eq_sp_sig
// Ref: {EMU3S_HWCORE_UD_Flow_035}
void EMU3S_HWCORE::MonitorAngComp0SPMethod()
{
    bool cur_val = presml_ang_comp0_eq_sp_sig.read();
    if (cur_val)
    {
        mAngComp0Active = true;
        INPUTStartSPAnalyze();
        unsigned int RECIPTRG = mpEMU3S_HWCORE_Func->GetRegVal("IPTRG", "RECIPTRG");
        if (RECIPTRG == 1)
        {
            AssertRCTGStartSPMethod();
        }
        mAngComp0Active = false;
    }
}

// Description: Monitor RCTGEndSP 
// Ref: {EMU3S_HWCORE_UD_Flow_026}
void EMU3S_HWCORE::MonitorRCTGEndSPMethod()
{
    bool mRCTGEndSP_cur_val = output_rctg_end_sp_reg_sig.read();
    if ((!mRCTGEndSP) && mRCTGEndSP_cur_val)
    {
        mpEMU3S_HWCORE_Func->SetRegVal("IPSFT", "RECIPSFT", 0);
    }
    mRCTGEndSP = mRCTGEndSP_cur_val;
}
// Description: Assert output_rctg_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::AssertRCTGStartSPMethod()
{
    mRCTGStartSP = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWriteRCTGStartSPEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertRCTGStartSPEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert output_rctg_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::DeassertRCTGStartSPMethod()
{
    mRCTGStartSP = false;
    mWriteRCTGStartSPEvent.notify();
}
// Description: Write output_rctg_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::WriteRCTGStartSPMethod()
{
    bool current_value = output_rctg_start_sp_sig.read();

    if (current_value != mRCTGStartSP) {
        output_rctg_start_sp_sig.write(mRCTGStartSP);
        DumpInterruptMsg("output_rctg_start_sp", mRCTGStartSP);
    }
}

//========================================================
//============    PWM Start Trigger    ===================
//========================================================
// Description: PWM Skip Trigger
// Ref: {EMU3S_HWCORE_UD_Feature_024, EMU3S_HWCORE_UD_ATT1_007}
void EMU3S_HWCORE::PWMSkipTrigger()
{
    unsigned char TNEN = (unsigned char)(mpEMU3S_HWCORE_Func->GetRegVal("IPTRGCTR", "TNEN") & 0x1);
    if (TNEN == 0)
    {
        mAssertPWMStartSPEvent.notify();
    }
    else
    {
        unsigned int PWMTRGNUM = (mpEMU3S_HWCORE_Func->GetRegVal("IPTRGNUM", "PWMTRGNUM") & 0xFF);
        mPWMTRGNUMCnt++;
        if (mPWMTRGNUMCnt == PWMTRGNUM + 1)
        {
            mAssertPWMStartSPEvent.notify();
            mPWMTRGNUMCnt = 0;
        }
    }
}
// Description: Assert output_pwm_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::AssertPWMStartSPMethod()
{
    mPWMStartSP = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWritePWMStartSPEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertPWMStartSPEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert output_pwm_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::DeassertPWMStartSPMethod()
{
    mPWMStartSP = false;
    mWritePWMStartSPEvent.notify();
}
// Description: Write output_pwm_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::WritePWMStartSPMethod()
{
    bool current_value = output_pwm_start_sp_sig.read();

    if (current_value != mPWMStartSP) {
        output_pwm_start_sp_sig.write(mPWMStartSP);
        DumpInterruptMsg("output_pwm_start_sp", mPWMStartSP);
    }
}

//========================================================
//============    PI Start Trigger    ===================
//========================================================
// Description: PI Start Trigger
// Ref: {EMU3S_HWCORE_UD_Flow_042}  
void EMU3S_HWCORE::PIStartSPAnalyze()
{
    bool PIIPTRG = (bool)(mpEMU3S_HWCORE_Func->GetRegVal("IPTRG", "PIIPTRG") & 0x1);
    bool CTRINMD_IIRDQ = em3ictrinmd_iirdq_sig.read();

    if (PIIPTRG)
    {
        if (CTRINMD_IIRDQ)
        {
            if (mIIRCh1EndActive)
            {
                PISkipTrigger();
            }
        }
        else
        {
            if (mINPUTEndSP)
            {
                PISkipTrigger();
            }
        }
    }
}
// Description: PI Skip Trigger
// Ref: {EMU3S_HWCORE_UD_Feature_024, EMU3S_HWCORE_UD_ATT1_007}
void EMU3S_HWCORE::PISkipTrigger()
{
    unsigned char TNEN = (unsigned char)(mpEMU3S_HWCORE_Func->GetRegVal("IPTRGCTR", "TNEN") & 0x1);
    if (TNEN == 0)
    {
        mAssertPIStartSPEvent.notify();
    }
    else
    {
        unsigned int PITRGNUM = (mpEMU3S_HWCORE_Func->GetRegVal("IPTRGNUM", "PITRGNUM") & 0xFF);
        mPITRGNUMCnt++;
        if (mPITRGNUMCnt == PITRGNUM + 1)
        {
            mAssertPIStartSPEvent.notify();
            mPITRGNUMCnt = 0;
        }
    }
}
// Description: Assert pi_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::AssertPIStartSPMethod()
{
    mPIStartSP = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWritePIStartSPEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertPIStartSPEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert pi_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::DeassertPIStartSPMethod()
{
    mPIStartSP = false;
    mWritePIStartSPEvent.notify();
}
// Description: Write pi_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::WritePIStartSPMethod()
{
    bool current_value = pi_start_sp_sig.read();

    if (current_value != mPIStartSP) {
        pi_start_sp_sig.write(mPIStartSP);
        DumpInterruptMsg("pi_start_sp_sig", mPIStartSP);
    }
}

//========================================================
//============    Input Start Trigger  ===================
//========================================================
// Description: Monitor adtrg_end_trig_sig
// Ref: {}
void EMU3S_HWCORE::MonitorADTRGEndTrigMethod(void)
{
    bool cur_val = adtrg_end_trig_sig.read();
    if (cur_val)
    {
        mADEndTrigActive = true;
        INPUTStartSPAnalyze();
        mADEndTrigActive = false;
    }
}
// Description: Monitor iir_end0_sig
// Ref: {}
void EMU3S_HWCORE::MonitorIIRCh0EndMethod(void)
{
    bool cur_val = iir_end0_sig.read();
    if (cur_val)
    {
        mIIRCh0EndActive = true;
        INPUTStartSPAnalyze();
        mIIRCh0EndActive = false;
    }
}
// Description: Monitor iir_end1_sig
// Ref: {}
void EMU3S_HWCORE::MonitorIIRCh1EndMethod(void)
{
    bool cur_val = iir_end1_sig.read();
    if (cur_val)
    {
        mIIRCh1EndActive = true;
        INPUTStartSPAnalyze();
        PIStartSPAnalyze();
        mIIRCh1EndActive = false;
    }
}
// Description: Monitor iir_end2_sig
// Ref: {}
void EMU3S_HWCORE::MonitorIIRCh2EndMethod(void)
{
    bool cur_val = iir_end2_sig.read();
    if (cur_val)
    {
        mIIRCh2EndActive = true;
        INPUTStartSPAnalyze();
        mIIRCh2EndActive = false;
    }
}
// Description: Check AD Completion
// Ref: {}
bool EMU3S_HWCORE::ADCompletionDetection()
{
    unsigned char ADEND = ((ADENDCTR_sig.read()) & 0x3);
    bool result = false;
    switch (ADEND)
    {
        case 0x0:
        {
            if (mADEndTrigActive)
            {
                result = true;
            }
            break;
        }
        case 0x1:
        {
            if (mIIRCh0EndActive)
            {
                result = true;
            }
            break;
        }
        case 0x2:
        {
            if (mIIRCh1EndActive)
            {
                result = true;
            }
            break;
        }
        default: // always 0x3
        {
            if (mIIRCh2EndActive)
            {
                result = true;
            }
        }
    }
    return result;
}

// Description: Input Start Trigger
// Ref: {EMU3S_HWCORE_UD_Flow_016}  
void EMU3S_HWCORE::INPUTStartSPAnalyze()
{
    unsigned char INIPTRG = (unsigned char) (mpEMU3S_HWCORE_Func->GetRegVal("IPTRG", "INIPTRG") & 0x3);
    
    switch (INIPTRG)
    {
        case 0x0:
        {
            // SW Trigger is set via callback and not go through here
            break;
        }
        case 0x1:
        {
            if (mAngComp0Active)
            {
                INPUTSkipTrigger();
            }
            break;
        }
        case 0x2:
        {
            if (ADCompletionDetection())
            {
                INPUTSkipTrigger();
            }
            break;
        }
        default: // always 0x3
        {
            if (mAngComp0Active || ADCompletionDetection())
            {
                INPUTSkipTrigger();
            }
        }
    }
}

// Description: INPUT Skip Trigger
// Ref: {EMU3S_HWCORE_UD_Feature_024, EMU3S_HWCORE_UD_ATT1_007}
void EMU3S_HWCORE::INPUTSkipTrigger()
{
    unsigned char TNEN = (unsigned char) (mpEMU3S_HWCORE_Func->GetRegVal("IPTRGCTR", "TNEN") & 0x1);
    if (TNEN == 0)
    {
        mAssertINPUTStartSPEvent.notify();
    }
    else
    {
        unsigned int INTRGNUM = (mpEMU3S_HWCORE_Func->GetRegVal("IPTRGNUM", "INTRGNUM") & 0xFF);
        mINTRGNUMCnt++;
        if (mINTRGNUMCnt == INTRGNUM + 1)
        {
            mAssertINPUTStartSPEvent.notify();
            mINTRGNUMCnt = 0;
        }
    }
}

// Description: Assert input_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::AssertINPUTStartSPMethod()
{
    mINPUTStartSP = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWriteINPUTStartSPEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertINPUTStartSPEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert input_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::DeassertINPUTStartSPMethod()
{
    mINPUTStartSP = false;
    mWriteINPUTStartSPEvent.notify();
}
// Description: Write input_start_sp_sig
// Ref: {}
void EMU3S_HWCORE::WriteINPUTStartSPMethod()
{
    bool current_value = input_start_sp_sig.read();

    if (current_value != mINPUTStartSP) {
        input_start_sp_sig.write(mINPUTStartSP);
        DumpInterruptMsg("input_start_sp", mINPUTStartSP);
    }
}



//========================================================
//=======     Common Start Trigger Function        =======
//========================================================
// Description: Get Current Triggered Number of Sub-IP (For Skip Trigger operation)
// Ref: {}
unsigned int EMU3S_HWCORE::GetTRGNUM(emSubModel emSub)
{
    if (emSub == emPWM)
    {
        return mPWMTRGNUMCnt;
    }
    else if (emSub == emPI)
    {
        return mPITRGNUMCnt;
    }
    else if (emSub == emINPUT)
    {
        return mINTRGNUMCnt;
    }
    else
    {
        return mADTRGNUMCnt;
    }
}
// Description: Notify Start Trigger by SW 
// Ref: {EMU3S_HWCORE_UD_Flow_016, EMU3S_HWCORE_UD_Flow_035, EMU3S_HWCORE_UD_Flow_042, EMU3S_HWCORE_UD_Flow_043}
void EMU3S_HWCORE::NotifySWTrigger(emSubModel emSub)
{
    if (emSub == emRCTG)
    {
        AssertRCTGStartSPMethod();
    }

    if (emSub == emPWM)
    {
        PWMSkipTrigger();
    }

    if (emSub == emPI)
    {
        PISkipTrigger();
    }

    if (emSub == emINPUT)
    {
        INPUTSkipTrigger();
    }
}

//========================================================
//============       PWM State       ===================
//========================================================
// Description: Monitor output_pwm_end_sp_reg_sig
// Ref: {EMU3S_HWCORE_UD_Flow_022, EMU3S_HWCORE_UD_Flow_023, EMU3S_HWCORE_UD_Flow_024}
// Ref: {EMU3S_HWCORE_UD_Flow_026}
void EMU3S_HWCORE::MonitorPWMEndSPMethod()
{
    bool mPWMEndSP = output_pwm_end_sp_reg_sig.read();
    if (mPWMEndSP)
    {
        mpEMU3S_HWCORE_Func->SetRegVal("IPSFT", "PWMIPSFT", 0);
        mStateMap[emPWM] = emIDLE;
        UpdateFSMSTGRPA(emPWM);
    }
}
// Description: Monitor pwm_func_st_sig
// Ref: {EMU3S_HWCORE_UD_Flow_021}
void EMU3S_HWCORE::MonitorPWMFuncStMethod()
{
    unsigned int val = (unsigned int)(pwm_func_st_sig.read() & 0xFFFF);
    if (val != 0)
    {
        UpdateFUNCSTGRPA(emPWM, val);
    }
}
// Description: Monitor pwm_func_end_sig
// Ref: {EMU3S_HWCORE_UD_Flow_023}
void EMU3S_HWCORE::MonitorPWMFuncEndMethod()
{
    unsigned int val = (unsigned int)(pwm_func_end_sig.read() & 0xFFFF);
    if (val != 0)
    {
        mPWMFuncEnd_1Hot = val;
        UpdateFUNCFINGRPA(emPWM);
    }
}

// Description: Monitor pwm_func_wait_sig
// Ref: {EMU3S_HWCORE_UD_Flow_022, EMU3S_HWCORE_UD_Flow_023, EMU3S_HWCORE_UD_Flow_024}
void EMU3S_HWCORE::MonitorPWMFuncWaitMethod()
{
    bool wait_state = pwm_func_wait_sig.read();
    if (wait_state)
    {
        mStateMap[emPWM] = emWAIT;
        UpdateFSMSTGRPA(emPWM);
    }
}
// Description: Monitor pwm_func_busy_sig
// Ref: {EMU3S_HWCORE_UD_Flow_024}
void EMU3S_HWCORE::MonitorPWMFuncBusyMethod()
{
    bool busy_state = pwm_func_busy_sig.read();
    if (busy_state)
    {
        mStateMap[emPWM] = emBUSY;
        UpdateFSMSTGRPA(emPWM);
    }
}

//========================================================
//============       PI State       ===================
//========================================================
// Description: Monitor pi_end_sp_reg_sig
// Ref: {EMU3S_HWCORE_UD_Flow_022, EMU3S_HWCORE_UD_Flow_023, EMU3S_HWCORE_UD_Flow_024}
// Ref: {EMU3S_HWCORE_UD_Flow_026, EMU3S_HWCORE_UD_Flow_043}
void EMU3S_HWCORE::MonitorPIEndSPMethod()
{
    bool mPIEndSP = pi_end_sp_reg_sig.read();
    if (mPIEndSP)
    {
        mpEMU3S_HWCORE_Func->SetRegVal("IPSFT", "PIIPSFT", 0);
        mStateMap[emPI] = emIDLE;
        UpdateFSMSTGRPA(emPI);

        unsigned int PWMIPTRG = mpEMU3S_HWCORE_Func->GetRegVal("IPTRG", "PWMIPTRG");
        if (PWMIPTRG == 1)
        {
            PWMSkipTrigger();
        }
    }
}
// Description: Monitor pi_func_st_sig
// Ref: {EMU3S_HWCORE_UD_Flow_021}
void EMU3S_HWCORE::MonitorPIFuncStMethod()
{
    unsigned int val = (unsigned int)(pi_func_st_sig.read() & 0xFF);
    if (val != 0)
    {
        UpdateFUNCSTGRPA(emPI, val);
    }
}
// Description: Monitor pi_func_end_sig
// Ref: {EMU3S_HWCORE_UD_Flow_023}
void EMU3S_HWCORE::MonitorPIFuncEndMethod()
{
    unsigned int val = (unsigned int)(pi_func_end_sig.read() & 0xFF);
    if (val != 0)
    {
        mPIFuncEnd_1Hot = val;
        UpdateFUNCFINGRPA(emPI);
    }
}

// Description: Monitor pi_func_wait_sig
// Ref: {EMU3S_HWCORE_UD_Flow_022, EMU3S_HWCORE_UD_Flow_023, EMU3S_HWCORE_UD_Flow_024}
void EMU3S_HWCORE::MonitorPIFuncWaitMethod()
{
    bool wait_state = pi_func_wait_sig.read();
    if (wait_state)
    {
        mStateMap[emPI] = emWAIT;
        UpdateFSMSTGRPA(emPI);
    }
}
// Description: Monitor pi_func_busy_sig
// Ref: {EMU3S_HWCORE_UD_Flow_024}
void EMU3S_HWCORE::MonitorPIFuncBusyMethod()
{
    bool busy_state = pi_func_busy_sig.read();
    if (busy_state)
    {
        mStateMap[emPI] = emBUSY;
        UpdateFSMSTGRPA(emPI);
    }
}

//========================================================
//============       Input State       ===================
//========================================================
// Description: Monitor input_end_sp_reg_sig
// Ref: {EMU3S_HWCORE_UD_Flow_022, EMU3S_HWCORE_UD_Flow_023, EMU3S_HWCORE_UD_Flow_024}
// Ref: {EMU3S_HWCORE_UD_Flow_026}
void EMU3S_HWCORE::MonitorINPUTEndSPMethod()
{
    mINPUTEndSP = input_end_sp_reg_sig.read();
    if (mINPUTEndSP)
    {
        mpEMU3S_HWCORE_Func->SetRegVal("IPSFT", "INIPSFT", 0);
        mStateMap[emINPUT] = emIDLE;
        UpdateFSMSTGRPA(emINPUT);
        
        PIStartSPAnalyze();
        mINPUTEndSP = false;
    }
}
// Description: Monitor input_func_st_sig
// Ref: {EMU3S_HWCORE_UD_Flow_021}
void EMU3S_HWCORE::MonitorINPUTFuncStMethod()
{
    unsigned int val = (unsigned int)(input_func_st_sig.read() & 0xFF);
    if (val != 0)
    {
        UpdateFUNCSTGRPA(emINPUT, val);
    }
}
// Description: Monitor input_func_end_sig
// Ref: {EMU3S_HWCORE_UD_Flow_023}
void EMU3S_HWCORE::MonitorINPUTFuncEndMethod()
{
    unsigned int val = (unsigned int) (input_func_end_sig.read() & 0xFF);
    if (val != 0)
    {
        mINPUTFuncEnd_1Hot = val;
        UpdateFUNCFINGRPA(emINPUT);
    }
}

// Description: Monitor input_func_wait_sig
// Ref: {EMU3S_HWCORE_UD_Flow_022, EMU3S_HWCORE_UD_Flow_023, EMU3S_HWCORE_UD_Flow_024}
void EMU3S_HWCORE::MonitorINPUTFuncWaitMethod()
{
    bool wait_state = input_func_wait_sig.read();
    if (wait_state)
    {
        mStateMap[emINPUT] = emWAIT;
        UpdateFSMSTGRPA(emINPUT);
    }
}
// Description: Monitor input_func_busy_sig
// Ref: {EMU3S_HWCORE_UD_Flow_024}
void EMU3S_HWCORE::MonitorINPUTFuncBusyMethod()
{
    bool busy_state = input_func_busy_sig.read();
    if (busy_state)
    {
        mStateMap[emINPUT] = emBUSY;
        UpdateFSMSTGRPA(emINPUT);
    }
}

//========================================================
//============       RSLVIDE State       ===================
//========================================================
// Description: Monitor rslv_func_st_sig
// Ref: {EMU3S_HWCORE_UD_Flow_021}
void EMU3S_HWCORE::MonitorRSLVIDEFuncStMethod()
{
    unsigned int val = (unsigned int)(rslv_func_st_sig.read() & 0xFFFF);
    if (val != 0)
    {
        UpdateFUNCSTGRPB(val);
    }
}
// Description: Monitor rslv_func_end_sig
// Ref: {EMU3S_HWCORE_UD_Flow_023}
void EMU3S_HWCORE::MonitorRSLVIDEFuncEndMethod()
{
    unsigned int val = (unsigned int)(rslv_func_end_sig.read() & 0xFFFF);
    if (val != 0)
    {
        mRSLVIDEFuncEnd_1Hot = val;
        UpdateFUNCFINGRPB();
    }
}

// Description: Monitor rslv_func_wait_sig
// Ref: {EMU3S_HWCORE_UD_Flow_022, EMU3S_HWCORE_UD_Flow_023, EMU3S_HWCORE_UD_Flow_024}
void EMU3S_HWCORE::MonitorRSLVIDEFuncWaitMethod()
{
    bool wait_state = rslv_func_wait_sig.read();
    bool busy_state = rslv_func_busy_sig.read();
    if (wait_state)
    {
        mStateMap[emRSLVIDE] = emWAIT;
        UpdateFSMSTGRPB();
    }
    else if (!busy_state)
    {
        mStateMap[emRSLVIDE] = emIDLE;
        UpdateFSMSTGRPB();
    }
}
// Description: Monitor rslv_func_busy_sig
// Ref: {EMU3S_HWCORE_UD_Flow_024}
void EMU3S_HWCORE::MonitorRSLVIDEFuncBusyMethod()
{
    bool busy_state = rslv_func_busy_sig.read();
    bool wait_state = rslv_func_wait_sig.read();
    if (busy_state)
    {
        mStateMap[emRSLVIDE] = emBUSY;
        UpdateFSMSTGRPB();
    }
    else if (!wait_state)
    {
        mStateMap[emRSLVIDE] = emIDLE;
        UpdateFSMSTGRPB();
    }
}

//========================================================
//================   Update Registers  ===================
//========================================================
// Description: Update FUNCSTGRPA register
// Ref: {}
void EMU3S_HWCORE::UpdateFUNCSTGRPA(emSubModel ip_id, unsigned int FuncSt_1Hot)
{
    unsigned int start_bit = 0;
    unsigned int func_id = 0;
    unsigned int mask = 1;
    if (ip_id == emPI)
    {
        start_bit = 7;
    }
    else if (ip_id == emPWM)
    {
        start_bit = 9;
    }
    func_id = mPower2Map[FuncSt_1Hot];
    mask = (mask << (start_bit + func_id));
    unsigned int reg_val = mpEMU3S_HWCORE_Func->GetRegVal("FUNCSTGRPA");
    mpEMU3S_HWCORE_Func->SetRegVal("FUNCSTGRPA", reg_val | mask);
}
// Description: Update FUNCSTGRPB register
// Ref: {}
void EMU3S_HWCORE::UpdateFUNCSTGRPB(unsigned int FuncSt_1Hot)
{
    unsigned int func_id = 0;
    unsigned int mask = 1;
    func_id = mPower2Map[FuncSt_1Hot];
    mask = (mask << func_id);
    unsigned int reg_val = mpEMU3S_HWCORE_Func->GetRegVal("FUNCSTGRPB");
    mpEMU3S_HWCORE_Func->SetRegVal("FUNCSTGRPB", reg_val | mask);
}
// Description: Update FSMSTGRPA register
// Ref: {}
void EMU3S_HWCORE::UpdateFSMSTGRPA(emSubModel ip_id)
{
    unsigned int state = mStateMap[ip_id];
    if (state)
    {
        mpEMU3S_HWCORE_Func->SetRegVal("FSMSTGRPA", (ip_id << 16) | mStateMap[ip_id]);
        if (state == emWAIT)
        {
            mAssertFuncAWaitEvent.notify();
        }
    }
    else // IDLE State
    {
        mpEMU3S_HWCORE_Func->SetRegVal("FSMSTGRPA", 0x00000000);
    }
}
// Description: Update FSMSTGRPB register
// Ref: {}
void EMU3S_HWCORE::UpdateFSMSTGRPB(void)
{
    unsigned int state = mStateMap[emRSLVIDE];
    if (state)
    {
        mpEMU3S_HWCORE_Func->SetRegVal("FSMSTGRPB", (1 << 16) | mStateMap[emRSLVIDE]);
        if (state == emWAIT)
        {
            mAssertFuncBWaitEvent.notify();
        }
    }
    else // IDLE State
    {
        mpEMU3S_HWCORE_Func->SetRegVal("FSMSTGRPB", 0x00000000);
    }
}
// Description: Update FUNCFINGRPA register
// Ref: {EMU3S_HWCORE_UD_Flow_021}
void EMU3S_HWCORE::UpdateFUNCFINGRPA(emSubModel ip_id)
{
    unsigned int func_max = mFuncMaxMap[ip_id];
    unsigned int check_exist = 0;
    unsigned int func_actual = 0;
    unsigned int FUNCFLGRPA_FuncMax = 0;
    switch (ip_id)
    {
        case emINPUT:
        {
            func_actual = GetFuncID(mINPUTFuncEnd_1Hot);
            FUNCFLGRPA_FuncMax = mpEMU3S_HWCORE_Func->GetRegVal("FUNCFLGRPA0", "INPUT3");
            break;
        }
        case emPI:
        {
            func_actual = GetFuncID(mPIFuncEnd_1Hot);
            FUNCFLGRPA_FuncMax = mpEMU3S_HWCORE_Func->GetRegVal("FUNCFLGRPA1", "PI1");
            break;
        }
        default: // emPWM
        {
            func_actual = GetFuncID(mPWMFuncEnd_1Hot);
            FUNCFLGRPA_FuncMax = mpEMU3S_HWCORE_Func->GetRegVal("FUNCFLGRPA2", "PWM10");
        }
    }
    if (func_actual <= func_max - 1)
    {
        if ((func_actual != func_max - 1) || (FUNCFLGRPA_FuncMax != 0)) // Not (Last HW block execution and transition to next HW block)
        {
            mpEMU3S_HWCORE_Func->SetRegVal("FUNCFINGRPA", (ip_id << 8) | func_actual);
        }
    }
}
// Description: Update FUNCFINGRPB register
// Ref: {}
void EMU3S_HWCORE::UpdateFUNCFINGRPB(void)
{
    unsigned int func_max = mFuncMaxMap[emRSLVIDE];
    unsigned int func_actual = GetFuncID(mRSLVIDEFuncEnd_1Hot);
    if (func_actual <= func_max - 1)
    {
        unsigned int FUNCFLGRPB_FuncMax = mpEMU3S_HWCORE_Func->GetRegVal("FUNCFLGRPB", "ANG3");
        if ((func_actual != func_max - 1) || (FUNCFLGRPB_FuncMax != 0)) // Not (Last HW block execution and transition to next HW block)
        {
            mpEMU3S_HWCORE_Func->SetRegVal("FUNCFINGRPB", (1 << 8) | func_actual);
        }
    }
}
// Description: Get HW Func ID
// Ref: {}
unsigned int EMU3S_HWCORE::GetFuncID(unsigned int FuncEnd_1Hot)
{
    unsigned int func_id = emNotExistFuncID;
    unsigned int check_exist = mPower2Map.count(FuncEnd_1Hot);
    if (check_exist)
    {
        func_id = mPower2Map[FuncEnd_1Hot];
    }
    return func_id;
}

//========================================================
//==========  Assert/Deassert FuncA/B Wait Int ===========
//========================================================
// Description: Assert funca_wait_int_sig
// Ref: {}
void EMU3S_HWCORE::AssertFuncAWaitMethod()
{
    mFuncAWait = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWriteFuncAWaitEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertFuncAWaitEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert funca_wait_int_sig
// Ref: {}
void EMU3S_HWCORE::DeassertFuncAWaitMethod()
{
    mFuncAWait = false;
    mWriteFuncAWaitEvent.notify();
}
// Description: Write funca_wait_int_sig
// Ref: {}
void EMU3S_HWCORE::WriteFuncAWaitMethod()
{
    bool current_value = funca_wait_int_sig.read();

    if (current_value != mFuncAWait) {
        funca_wait_int_sig.write(mFuncAWait);
        DumpInterruptMsg("funca_wait_int", mFuncAWait);
    }
}
// Description: Assert funcb_wait_int_sig
// Ref: {}
void EMU3S_HWCORE::AssertFuncBWaitMethod()
{
    mFuncBWait = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWriteFuncBWaitEvent.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassertFuncBWaitEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Description: De-assert funcb_wait_int_sig
// Ref: {}
void EMU3S_HWCORE::DeassertFuncBWaitMethod()
{
    mFuncBWait = false;
    mWriteFuncBWaitEvent.notify();
}
// Description: Write funcb_wait_int_sig
// Ref: {}
void EMU3S_HWCORE::WriteFuncBWaitMethod()
{
    bool current_value = funcb_wait_int_sig.read();

    if (current_value != mFuncBWait) {
        funcb_wait_int_sig.write(mFuncBWait);
        DumpInterruptMsg("funcb_wait_int", mFuncBWait);
    }
}

//========================================================
//===============  Monitor other Signals =================
//========================================================
// Description: Monitor IPTRGMON_ADTRGNUM_sig
// Ref: {EMU3S_HWCORE_UD_Flow_048}
void EMU3S_HWCORE::MonitorIptrgMonAdtrgNumMethod()
{
    mADTRGNUMCnt = (unsigned int)(IPTRGMON_ADTRGNUM_sig.read() & 0xFF);
}
//========================================================
//============        PYTHON IF        ===================
//========================================================
// Description: Set message level (fatal, error, warning, info)
// Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::SetMessageLevel(const std::string msg_lv, std::string sub_ip_name)
{
    if (sub_ip_name != all_sub_ip)
    {
        // Set message level for sub-ips
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name (%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emEMU3S_HWCORE:
                case emHANDSHAKE:
                {
                    SetMesLvlTop(msg_lv);
                    break;
                }
                case emINPUT:
                {
                    mpINPUT->SetMessageLevel(msg_lv);
                    break;
                }
                case emIIR:
                {
                    mpIIR->SetMessageLevel(msg_lv);
                    break;
                }
                case emMEASURE:
                {
                    mpMEASURE->SetMessageLevel(msg_lv);
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->SetMessageLevel(msg_lv);
                    break;
                }
                case emADTRG:
                {
                    mpADTRG->SetMessageLevel(msg_lv);
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->SetMessageLevel(msg_lv);
                    break;
                }
                case emPI:
                {
                    mpPI->SetMessageLevel(msg_lv);
                    break;
                }
                case emPWM:
                {
                    mpPWM->SetMessageLevel(msg_lv);
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->SetMessageLevel(msg_lv);
                    break;
                }
                default: // INT or VRFBUF
                {
                    mpINT->SetMessageLevel(msg_lv);
                    break;
                }
            }
        }
    }
    else
    {
        SetMesLvlTop(msg_lv);
        // Forward to sub-ips
        mpINPUT->SetMessageLevel(msg_lv);
        mpIIR->SetMessageLevel(msg_lv);
        mpMEASURE->SetMessageLevel(msg_lv);
        mpRCTG->SetMessageLevel(msg_lv);
        mpADTRG->SetMessageLevel(msg_lv);
        mpRSLVIDE->SetMessageLevel(msg_lv);
        mpPI->SetMessageLevel(msg_lv);
        mpPWM->SetMessageLevel(msg_lv);
        mpIRCTG3->SetMessageLevel(msg_lv);
        mpINT->SetMessageLevel(msg_lv);
    }

}
// Description: Set MessageLevel for EMU3S_HWCORE (Wrapper)
// Ref: {}
void EMU3S_HWCORE::SetMesLvlTop(const std::string msg_lv)
{
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

        mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
        mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
        mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
    } else {
    //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        SeparateString(cmd, msg_lv);
        mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
        mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        SeparateString(cmd, msg_lv);
        mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
        mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
    }
}

 // Description: Enable/disable dumping message when users access registers
 // Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::DumpRegisterRW(const std::string is_enable, std::string sub_ip_name)
{
    if (sub_ip_name != all_sub_ip)
    {
        // dump register rw to sub-ips
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emEMU3S_HWCORE:
                {
                    DumpRegRWTop(is_enable, emEMU3S_HWCORE);
                    break;
                }
                case emINPUT:
                {
                    mpINPUT->DumpRegisterRW(is_enable);
                    break;
                }
                case emIIR:
                {
                    mpIIR->DumpRegisterRW(is_enable);
                    break;
                }
                case emMEASURE:
                {
                    mpMEASURE->DumpRegisterRW(is_enable);
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->DumpRegisterRW(is_enable);
                    break;
                }
                case emHANDSHAKE:
                {
                    DumpRegRWTop(is_enable, emHANDSHAKE);
                }
                case emADTRG:
                {
                    mpADTRG->DumpRegisterRW(is_enable);
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->DumpRegisterRW(is_enable);
                    break;
                }
                case emPI:
                {
                    mpPI->DumpRegisterRW(is_enable);
                    break;
                }
                case emPWM:
                {
                    mpPWM->DumpRegisterRW(is_enable);
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->DumpRegisterRW(is_enable);
                    break;
                }
                default: // INT pr VRFBUF
                {
                    mpINT->DumpRegisterRW(is_enable);
                    break;
                }
            }
        }
    }
    else
    {
        DumpRegRWTop(is_enable, emEMU3S_HWCORE);
        // Forward this setting to sub-ips
        mpINPUT->DumpRegisterRW(is_enable);
        mpIIR->DumpRegisterRW(is_enable);
        mpMEASURE->DumpRegisterRW(is_enable);
        mpRCTG->DumpRegisterRW(is_enable);
        DumpRegRWTop(is_enable, emHANDSHAKE);
        mpADTRG->DumpRegisterRW(is_enable);
        mpRSLVIDE->DumpRegisterRW(is_enable);
        mpPI->DumpRegisterRW(is_enable);
        mpPWM->DumpRegisterRW(is_enable);
        mpIRCTG3->DumpRegisterRW(is_enable);
        mpINT->DumpRegisterRW(is_enable);
    }
}
// Description: Enable/disable dumping message when users access registers for EMU3S_HWCORE (Wrapper)
// Ref: {}
void EMU3S_HWCORE::DumpRegRWTop(const std::string is_enable, emSubModel ip)
{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    if (ip == emEMU3S_HWCORE)
    {
        mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
    }
    else
    {
        mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
    }
}

 // Description: Enable/disable info/warning/error message of register IF
 // Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::EnableRegisterMessage(const std::string is_enable, std::string sub_ip_name)
{
    if (sub_ip_name != all_sub_ip)
    {
        // Enable register message for sub-ips
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emEMU3S_HWCORE:
                case emHANDSHAKE:
                {
                    EnableRegMesTop(is_enable);
                    break;
                }
                case emINPUT:
                {
                    mpINPUT->EnableRegisterMessage(is_enable);
                    break;
                }
                case emIIR:
                {
                    mpIIR->EnableRegisterMessage(is_enable);
                    break;
                }
                case emMEASURE:
                {
                    mpMEASURE->EnableRegisterMessage(is_enable);
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->EnableRegisterMessage(is_enable);
                    break;
                }
                case emADTRG:
                {
                    mpADTRG->EnableRegisterMessage(is_enable);
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->EnableRegisterMessage(is_enable);
                    break;
                }
                case emPI:
                {
                    mpPI->EnableRegisterMessage(is_enable);
                    break;
                }
                case emPWM:
                {
                    mpPWM->EnableRegisterMessage(is_enable);
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->EnableRegisterMessage(is_enable);
                    break;
                }
                default: // INT or VRFBUF
                {
                    mpINT->EnableRegisterMessage(is_enable);
                    break;
                }
            }
        }
    }
    else
    {
        EnableRegMesTop(is_enable);
        // Forward this setting to sub-ips
        mpINPUT->EnableRegisterMessage(is_enable);
        mpIIR->EnableRegisterMessage(is_enable);
        mpMEASURE->EnableRegisterMessage(is_enable);
        mpRCTG->EnableRegisterMessage(is_enable);
        mpADTRG->EnableRegisterMessage(is_enable);
        mpRSLVIDE->EnableRegisterMessage(is_enable);
        mpPI->EnableRegisterMessage(is_enable);
        mpPWM->EnableRegisterMessage(is_enable);
        mpIRCTG3->EnableRegisterMessage(is_enable);
        mpINT->EnableRegisterMessage(is_enable);
    }

}
// Description: Enable/disable info/warning/error message of register IF for EMU3S_HWCORE (Wrapper)
// Ref: {}
void EMU3S_HWCORE::EnableRegMesTop(const std::string is_enable)
{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) 
    {
        if (word_vector[0] == "true") 
        {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") 
        {
            mEnableRegisterMessage = false;
        } else 
        {
            re_printf("warning","Invalid argument: EMU3S_HWCORE_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) 
    {
        if (mEnableRegisterMessage) 
        {
            re_printf("info","EMU3S_HWCORE_EnableRegisterMessage %s\n", "true"); 
        } else {
            re_printf("info","EMU3S_HWCORE_EnableRegisterMessage %s\n", "false"); 
        }

    } 
    else 
    {
        re_printf("warning","Invalid argument: EMU3S_HWCORE_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}

 // Description: Enable/disable dumping message in case of interrupt request
 // Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::DumpInterrupt(const std::string is_enable, std::string sub_ip_name)
{
    if (sub_ip_name != all_sub_ip)
    {
        // Dump interrupt to sub-ips
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emEMU3S_HWCORE:
                case emHANDSHAKE:
                {
                    DumpIntTop(is_enable);
                    break;
                }
                case emINPUT:
                {
                    mpINPUT->DumpInterrupt(is_enable);
                    break;
                }
                case emIIR:
                {
                    mpIIR->DumpInterrupt(is_enable);
                    break;
                }
                case emMEASURE:
                {
                    mpMEASURE->DumpInterrupt(is_enable);
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->DumpInterrupt(is_enable);
                    break;
                }
                case emADTRG:
                {
                    mpADTRG->DumpInterrupt(is_enable);
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->DumpInterrupt(is_enable);
                    break;
                }
                case emPI:
                {
                    mpPI->DumpInterrupt(is_enable);
                    break;
                }
                case emPWM:
                {
                    mpPWM->DumpInterrupt(is_enable);
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->DumpInterrupt(is_enable);
                    break;
                }
                default: // INT or VRFBUF
                {
                    mpINT->DumpInterrupt(is_enable);
                    break;
                }
            }
        }
    }
    else
    {
        DumpIntTop(is_enable);
        // Forward this setting to sub-ips
        mpINPUT->DumpInterrupt(is_enable);
        mpIIR->DumpInterrupt(is_enable);
        mpMEASURE->DumpInterrupt(is_enable);
        mpRCTG->DumpInterrupt(is_enable);
        mpADTRG->DumpInterrupt(is_enable);
        mpRSLVIDE->DumpInterrupt(is_enable);
        mpPI->DumpInterrupt(is_enable);
        mpPWM->DumpInterrupt(is_enable);
        mpIRCTG3->DumpInterrupt(is_enable);
        mpINT->DumpInterrupt(is_enable);
    }
}
// Description: Enable/disable dumping message in case of interrupt request for EMU3S_HWCORE (Wrapper)
// Ref: {}
void EMU3S_HWCORE::DumpIntTop(const std::string is_enable)
{
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
            re_printf("warning","Invalid argument: EMU3S_HWCORE_DumpInterrupt %s\n", is_enable.c_str());
        }
    } 
    else if (word_vector.size() == 0) 
    {
        std::string temp = "false";
        if (mDumpInterrupt) 
        {
            temp = "true";
        } 
        re_printf("info","EMU3S_HWCORE_DumpInterrupt %s\n",temp.c_str());
    } 
    else 
    {
        re_printf("warning","Invalid argument: EMU3S_HWCORE_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}

 // Description: Determine whether enable or disable dumping automatically statistical 
 // Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::EnableStatInfo(const std::string is_enable, std::string sub_ip_name)
{
    if (sub_ip_name != all_sub_ip)
    {
        // Enable register message for sub-ips
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emINPUT:
                {
                    mpINPUT->EnableStatInfo(is_enable);
                    break;
                }
                case emIIR:
                {
                    mpIIR->EnableStatInfo(is_enable);
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->EnableStatInfo(is_enable);
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->EnableStatInfo(is_enable);
                    break;
                }
                case emPI:
                {
                    mpPI->EnableStatInfo(is_enable);
                    break;
                }
                case emPWM:
                {
                    mpPWM->EnableStatInfo(is_enable);
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->EnableStatInfo(is_enable);
                    break;
                }
                default:
                {
                    re_printf("warning", "Sub-ip name(%s) is not supported this API, the command is ignored", sub_ip_name.c_str());
                    break;
                }
            }
        }
    }
    else
    {
        // Forward this setting to sub-ips
        mpINPUT->EnableStatInfo(is_enable);
        mpRCTG->EnableStatInfo(is_enable);
        mpRSLVIDE->EnableStatInfo(is_enable);
        mpPI->EnableStatInfo(is_enable);
        mpPWM->EnableStatInfo(is_enable);
        mpIRCTG3->EnableStatInfo(is_enable);
    }

}

 // Description: Command to assert reset
 // Ref: {EMU3S_HWCORE_UD_Direction_030, EMU3S_HWCORE_UD_Direction_031, EMU3S_HWCORE_UD_Direction_036}
 // Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::AssertReset(const std::string reset_name, const double start_time, const double period, std::string sub_ip_name)
{
    if (sub_ip_name == all_sub_ip)
    {
        if (reset_name == "PRESETn") {
            if ((!mIsPRESETnHardActive) && (!mIsPRESETnCmdReceive)) {
                mIsPRESETnCmdReceive = true;
                re_printf("info", "The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
                mPRESETnCmdEvent.notify(start_time, SC_NS);
                mPRESETnCmdPeriod = period;
            }
            else {
                re_printf("warning", "The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
            }
        }
        else if (reset_name == "CRESETn") {
            if ((!mIsCRESETnHardActive) && (!mIsCRESETnCmdReceive)) {
                mIsCRESETnCmdReceive = true;
                re_printf("info", "The model will be reset (CRESETn) for %f ns after %f ns.\n", period, start_time);
                mCRESETnCmdEvent.notify(start_time, SC_NS);
                mCRESETnCmdPeriod = period;
            }
            else {
                re_printf("warning", "The software reset of CRESETn is called in the reset operation of the model. So it is ignored.\n");
            }
        }
        else {
            re_printf("warning", "The reset name (%s) is wrong. It should be PRESETn or CRESETn.\n", reset_name.c_str());
        }
    }
    else {
        // Forward to sub-ip
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emINPUT:
                {
                    mpINPUT->AssertReset(reset_name, start_time, period);
                    break;
                }
                case emIIR:
                {
                    mpIIR->AssertReset(reset_name, start_time, period);
                    break;
                }
                case emMEASURE:
                {
                    mpMEASURE->AssertReset(reset_name, start_time, period);
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->AssertReset(reset_name, start_time, period);
                    break;
                }
                case emADTRG:
                {
                    mpADTRG->AssertReset(reset_name, start_time, period);
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->AssertReset(reset_name, start_time, period);
                    break;
                }
                case emPI:
                {
                    mpPI->AssertReset(reset_name, start_time, period);
                    break;
                }
                case emPWM:
                {
                    mpPWM->AssertReset(reset_name, start_time, period);
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->AssertReset(reset_name, start_time, period);
                    break;
                }
                case emINT:
                case emVRFBUF:
                {
                    mpINT->AssertReset(reset_name, start_time, period);
                    break;
                }
                default:
                {
                    re_printf("warning", "Sub-ip name(%s) is not supported this API, the command is ignored", sub_ip_name.c_str());
                    break;
                }
            }
        }
    }
}


 // Description: Set clock value and clock unit
 // Ref: {EMU3S_HWCORE_UD_Feature_002, EMU3S_HWCORE_UD_Direction_027, EMU3S_HWCORE_UD_Direction_028, EMU3S_HWCORE_UD_Direction_033} 
 // Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042, EMU3S_HWCORE_UD_Flow_008}
void EMU3S_HWCORE::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit, std::string sub_ip_name)
{
    if (sub_ip_name == all_sub_ip)
    {
        std::vector<std::string> word_vector;
        SeparateString(word_vector, clock_name);
        if (word_vector.size() == 1) 
        {
            if (word_vector[0] == "PCLK") 
            {
                mPCLKOrgFreq = freq;
                ConvertClockFreq(mPCLKFreq, mPCLKFreqUnit, freq, unit);
                if (mPCLKFreq > 0) 
                {
                    mPCLKPeriod = (sc_dt::uint64)(((1 / (double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                    mTimePCLKChangePoint = sc_time_stamp().to_double();
                }
                else 
                {
                    mPCLKPeriod = 0;
                    re_printf("info", "(%s) frequency is zero.\n", clock_name.c_str());
                    // Cancel events
                    CancelSSCGEvents();
                }
                PCLK_sig.write(mPCLKFreq);
            }
            else if (word_vector[0] == "CCLK") 
            {
                mCCLKOrgFreq = freq;
                ConvertClockFreq(mCCLKFreq, mCCLKFreqUnit, freq, unit);
                if (mCCLKFreq > 0) 
                {
                    mCCLKPeriod = (sc_dt::uint64)(((1 / (double)mCCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                    mTimeCCLKChangePoint = sc_time_stamp().to_double();
                }
                else 
                {
                    mCCLKPeriod = 0;
                    re_printf("info", "(%s) frequency is zero.\n", clock_name.c_str());
                    // Cancel events
                    CancelCLEANEvents();
                }
                CCLK_sig.write(mCCLKFreq);
            }
            else 
            {
                re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
            }
        }
        else 
        {
            re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
        }
    }
    else 
    {
        // Foward to sub-ip
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emINPUT:
                {
                    mpINPUT->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                case emIIR:
                {
                    mpIIR->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                case emMEASURE:
                {
                    mpMEASURE->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                case emADTRG:
                {
                    mpADTRG->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                case emPI:
                {
                    mpPI->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                case emPWM:
                {
                    mpPWM->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                case emINT:
                case emVRFBUF:
                {
                    mpINT->SetCLKFreq(clock_name, freq, unit);
                    break;
                }
                default:
                {
                    re_printf("warning", "Sub-ip name(%s) is not supported this API, the command is ignored", sub_ip_name.c_str());
                    break;
                }
            }
        }
    }
}


 // Description: Get clock value
 // Ref: {EMU3S_HWCORE_UD_Direction_027, EMU3S_HWCORE_UD_Direction_032, EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::GetCLKFreq(const std::string clock_name, std::string sub_ip_name)
{
    if (sub_ip_name == all_sub_ip)
    {
        std::vector<std::string> word_vector;
        SeparateString(word_vector, clock_name);
        if (word_vector.size() == 1) 
        {
            if (word_vector[0] == "PCLK") 
            {
                re_printf("info", "PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
            }
            if (word_vector[0] == "CCLK") 
            {
                re_printf("info", "CCLK frequency is %0.0f %s\n", (double)mCCLKOrgFreq, mCCLKFreqUnit.c_str());
            }
            else 
            {
                re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
            }
        }
        else 
        {
            re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
        }
    }
    else 
    {
        // Foward to sub-ip
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emINPUT:
                {
                    mpINPUT->GetCLKFreq(clock_name);
                    break;
                }
                case emIIR:
                {
                    mpIIR->GetCLKFreq(clock_name);
                    break;
                }
                case emMEASURE:
                {
                    mpMEASURE->GetCLKFreq(clock_name);
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->GetCLKFreq(clock_name);
                    break;
                }
                case emADTRG:
                {
                    mpADTRG->GetCLKFreq(clock_name);
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->GetCLKFreq(clock_name);
                    break;
                }
                case emPI:
                {
                    mpPI->GetCLKFreq(clock_name);
                    break;
                }
                case emPWM:
                {
                    mpPWM->GetCLKFreq(clock_name);
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->GetCLKFreq(clock_name);
                    break;
                }
                case emINT:
                case emVRFBUF:
                {
                    mpINT->GetCLKFreq(clock_name);
                    break;
                }
                default:
                {
                    re_printf("warning", "Sub-ip name(%s) is not supported this API, the command is ignored", sub_ip_name.c_str());
                    break;
                }
            }
        }
    }
}

 // Description: Command to force register of model/sub-ip
 // Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::ForceRegister(std::string reg_name, unsigned int value, std::string sub_ip_name)
{
    // Force register of sub-ips
    std::map<std::string, unsigned int>::iterator it;
    it = mSubIPMap.find(sub_ip_name);
    if (it == mSubIPMap.end())
    {
        re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
        return;
    }
    else
    {
        switch (mSubIPMap[sub_ip_name])
        {
            case emEMU3S_HWCORE:
            {
                ForceRegTop(reg_name, value, emEMU3S_HWCORE);
                break;
            }
            case emINPUT:
            {
                mpINPUT->ForceRegister(reg_name, value);
                break;
            }
            case emIIR:
            {
                mpIIR->ForceRegister(reg_name, value);
                break;
            }
            case emMEASURE:
            {
                mpMEASURE->ForceRegister(reg_name, value);
                break;
            }
            case emRCTG:
            {
                mpRCTG->ForceRegister(reg_name, value);
                break;
            }
            case emHANDSHAKE:
            {
                ForceRegTop(reg_name, value, emHANDSHAKE);
                break;
            }
            case emADTRG:
            {
                mpADTRG->ForceRegister(reg_name, value);
                break;
            }
            case emRSLVIDE:
            {
                mpRSLVIDE->ForceRegister(reg_name, value);
                break;
            }
            case emPI:
            {
                mpPI->ForceRegister(reg_name, value);
                break;
            }
            case emPWM:
            {
                mpPWM->ForceRegister(reg_name, value);
                break;
            }
            case emIRCTG3:
            {
                mpIRCTG3->ForceRegister(reg_name, value);
                break;
            }
            default: // INT or VRFBUF
            {
                mpINT->ForceRegister(sub_ip_name, reg_name, value);
                break;
            }
        }
    }
}
// Description: Command to force register of EMU3S_HWCORE (Wrapper)
// Ref: {}
void EMU3S_HWCORE::ForceRegTop(std::string reg_name, unsigned int value, emSubModel ip)
{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") 
    {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) 
        {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            if (ip == emEMU3S_HWCORE)
            {
                mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
            }
            else
            {
                mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
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
}

// Description: Release register of sub-ips
// Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::ReleaseRegister(std::string reg_name, std::string sub_ip_name)
{
    // Set message level for sub-ips
    std::map<std::string, unsigned int>::iterator it;
    it = mSubIPMap.find(sub_ip_name);
    if (it == mSubIPMap.end())
    {
        re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
        return;
    }
    else
    {
        switch (mSubIPMap[sub_ip_name])
        {
            case emEMU3S_HWCORE:
            {
                ReleaseRegTop(reg_name, emEMU3S_HWCORE);
                break;
            }
            case emINPUT:
            {
                mpINPUT->ReleaseRegister(reg_name);
                break;
            }
            case emIIR:
            {
                mpIIR->ReleaseRegister(reg_name);
                break;
            }
            case emMEASURE:
            {
                mpMEASURE->ReleaseRegister(reg_name);
                break;
            }
            case emRCTG:
            {
                mpRCTG->ReleaseRegister(reg_name);
                break;
            }
            case emHANDSHAKE:
            {
                ReleaseRegTop(reg_name, emHANDSHAKE);
                break;
            }
            case emADTRG:
            {
                mpADTRG->ReleaseRegister(reg_name);
                break;
            }
            case emRSLVIDE:
            {
                mpRSLVIDE->ReleaseRegister(reg_name);
                break;
            }
            case emPI:
            {
                mpPI->ReleaseRegister(reg_name);
                break;
            }
            case emPWM:
            {
                mpPWM->ReleaseRegister(reg_name);
                break;
            }
            case emIRCTG3:
            {
                mpIRCTG3->ReleaseRegister(reg_name);
                break;
            }
            default: // INT or VRFBUF
            {
                mpINT->ReleaseRegister(sub_ip_name, reg_name);
                break;
            }
        }
    }
}
// Description: Release register of EMU3S_HWCORE (Wrapper)
// Ref: {}
void EMU3S_HWCORE::ReleaseRegTop(std::string reg_name, emSubModel ip)
{
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
            if (ip == emEMU3S_HWCORE)
            {
                mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
            }
            else
            {
                mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
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
}

// Description: Write value to register of sub-ip
// Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::WriteRegister(std::string reg_name, unsigned int value, std::string sub_ip_name)
{
    // Set message level for sub-ips
    std::map<std::string, unsigned int>::iterator it;
    it = mSubIPMap.find(sub_ip_name);
    if (it == mSubIPMap.end())
    {
        re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
        return;
    }
    else
    {
        switch (mSubIPMap[sub_ip_name])
        {
            case emEMU3S_HWCORE:
            {
                WriteRegTop(reg_name, value, emEMU3S_HWCORE);
                break;
            }
            case emINPUT:
            {
                mpINPUT->WriteRegister(reg_name, value);
                break;
            }
            case emIIR:
            {
                mpIIR->WriteRegister(reg_name, value);
                break;
            }
            case emMEASURE:
            {
                mpMEASURE->WriteRegister(reg_name, value);
                break;
            }
            case emRCTG:
            {
                mpRCTG->WriteRegister(reg_name, value);
                break;
            }
            case emHANDSHAKE:
            {
                WriteRegTop(reg_name, value, emHANDSHAKE);
                break;
            }
            case emADTRG:
            {
                mpADTRG->WriteRegister(reg_name, value);
                break;
            }
            case emRSLVIDE:
            {
                mpRSLVIDE->WriteRegister(reg_name, value);
                break;
            }
            case emPI:
            {
                mpPI->WriteRegister(reg_name, value);
                break;
            }
            case emPWM:
            {
                mpPWM->WriteRegister(reg_name, value);
                break;
            }
            case emIRCTG3:
            {
                mpIRCTG3->WriteRegister(reg_name, value);
                break;
            }
            default: // INT or VRFBUF
            {
                mpINT->WriteRegister(sub_ip_name, reg_name, value);
                break;
            }
        }
    }
}
// Description: Write value to register of EMU3S_HWCORE (Wrapper)
// Ref: {}
void EMU3S_HWCORE::WriteRegTop(std::string reg_name, unsigned int value, emSubModel ip)
{
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
            reg_value_str<<"0x"<<std::hex<<value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            if (ip == emEMU3S_HWCORE)
            {
                mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
            }
            else
            {
                mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
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
}

//Description: Read value of register of sub-ips
//Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::ReadRegister(std::string reg_name, std::string sub_ip_name)
{
    // Set message level for sub-ips
    std::map<std::string, unsigned int>::iterator it;
    it = mSubIPMap.find(sub_ip_name);
    if (it == mSubIPMap.end())
    {
        re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
        return;
    }
    else
    {
        switch (mSubIPMap[sub_ip_name])
        {
            case emEMU3S_HWCORE:
            {
                ReadRegTop(reg_name, emEMU3S_HWCORE);
                break;
            }
            case emINPUT:
            {
                mpINPUT->ReadRegister(reg_name);
                break;
            }
            case emIIR:
            {
                mpIIR->ReadRegister(reg_name);
                break;
            }
            case emMEASURE:
            {
                mpMEASURE->ReadRegister(reg_name);
                break;
            }
            case emRCTG:
            {
                mpRCTG->ReadRegister(reg_name);
                break;
            }
            case emHANDSHAKE:
            {
                ReadRegTop(reg_name, emHANDSHAKE);
                break;
            }
            case emADTRG:
            {
                mpADTRG->ReadRegister(reg_name);
                break;
            }
            case emRSLVIDE:
            {
                mpRSLVIDE->ReadRegister(reg_name);
                break;
            }
            case emPI:
            {
                mpPI->ReadRegister(reg_name);
                break;
            }
            case emPWM:
            {
                mpPWM->ReadRegister(reg_name);
                break;
            }
            case emIRCTG3:
            {
                mpIRCTG3->ReadRegister(reg_name);
                break;
            }
            default: // INT or VRFBUF
            {
                mpINT->ReadRegister(sub_ip_name, reg_name);
                break;
            }
        }
    }
}
// Description: Read value of register of EMU3S_HWCORE (Wrapper)
// Ref: {}
void EMU3S_HWCORE::ReadRegTop(std::string reg_name, emSubModel ip)
{
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
            if (ip == emEMU3S_HWCORE)
            {
                mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
            }
            else
            {
                mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
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
}
//Description: List register of sub-ips
//Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::ListRegister(std::string sub_ip_name)
{
    if (sub_ip_name != all_sub_ip)
    {
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emEMU3S_HWCORE:
                {
                    std::vector<std::string> cmd;
                    cmd.push_back("reg");
                    mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
                    break;
                }
                case emINPUT:
                {
                    mpINPUT->ListRegister();
                    break;
                }
                case emIIR:
                {
                    mpIIR->ListRegister();
                    break;
                }
                case emMEASURE:
                {
                    mpMEASURE->ListRegister();
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->ListRegister();
                    break;
                }
                case emHANDSHAKE:
                {
                    std::vector<std::string> cmd;
                    cmd.push_back("reg");
                    mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
                    break;
                }
                case emADTRG:
                {
                    mpADTRG->ListRegister();
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->ListRegister();
                    break;
                }
                case emPI:
                {
                    mpPI->ListRegister();
                    break;
                }
                case emPWM:
                {
                    mpPWM->ListRegister();
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->ListRegister();
                    break;
                }
                default: // INT or VRFBUF
                {
                    mpINT->ListRegister(sub_ip_name);
                    break;
                }
            }
        }
    }
    else
    {
        std::vector<std::string> cmd;
        cmd.push_back("reg");
        mpEMU3S_HWCORE_Func->RegisterHandler(cmd);
        // Forward command to sub-ips
        mpINPUT->ListRegister();
        mpIIR->ListRegister();
        mpMEASURE->ListRegister();
        mpRCTG->ListRegister();
        mpHWCORE_HANDSHAKE_Func->RegisterHandler(cmd);
        mpADTRG->ListRegister();
        mpRSLVIDE->ListRegister();
        mpPI->ListRegister();
        mpPWM->ListRegister();
        mpIRCTG3->ListRegister();
        mpINT->ListRegister("INT");
        mpINT->ListRegister("VRFBUF");
    }
}

// Description: Dump statistical info of all register of sub-ips
// Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042}
void EMU3S_HWCORE::DumpStatInfo(const std::string sub_ip_name)
{
    if (sub_ip_name != all_sub_ip)
    {
        std::map<std::string, unsigned int>::iterator it;
        it = mSubIPMap.find(sub_ip_name);
        if (it == mSubIPMap.end())
        {
            re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
            return;
        }
        else
        {
            switch (mSubIPMap[sub_ip_name])
            {
                case emINPUT:
                {
                    mpINPUT->DumpStatInfo();
                    break;
                }
                case emMEASURE:
                {
                    mpMEASURE->DumpStatInfo();
                    break;
                }
                case emIIR:
                {
                    mpIIR->DumpStatInfo();
                    break;
                }
                case emRCTG:
                {
                    mpRCTG->DumpStatInfo();
                    break;
                }
                case emRSLVIDE:
                {
                    mpRSLVIDE->DumpStatInfo();
                    break;
                }
                case emPI:
                {
                    mpPI->DumpStatInfo();
                    break;
                }
                case emPWM:
                {
                    mpPWM->DumpStatInfo();
                    break;
                }
                case emIRCTG3:
                {
                    mpIRCTG3->DumpStatInfo();
                    break;
                }
                default:
                {
                    re_printf("warning", "Sub-ip name(%s) is not supported this API, the command is ignored", sub_ip_name.c_str());
                    break;
                }
            }
        }
    }
    else {
        // Forward this setting to sub-ips
        mpINPUT->DumpStatInfo();
        mpIIR->DumpStatInfo();
        mpMEASURE->DumpStatInfo();
        mpRCTG->DumpStatInfo();
        mpRSLVIDE->DumpStatInfo();
        mpPI->DumpStatInfo();
        mpPWM->DumpStatInfo();
        mpIRCTG3->DumpStatInfo();
    }
}

// Description: Change the process time of sub-ips
// Ref: {EMU3S_HWCORE_UD_Direction_041, EMU3S_HWCORE_UD_Direction_042, EMU3S_HWCORE_UD_Direction_043}
void EMU3S_HWCORE::ChgProcessingTime(const std::string sub_ip_name, std::string func_name, unsigned int value)
{
    std::map<std::string, unsigned int>::iterator it;
    it = mSubIPMap.find(sub_ip_name);
    if (it == mSubIPMap.end())
    {
        re_printf("warning", "Sub-ip name(%s) is invalid, the command is ignored", sub_ip_name.c_str());
        return;
    }
    else
    {
        switch (mSubIPMap[sub_ip_name])
        {
            case emINPUT:
            {
                mpINPUT->ChgProcessingTime(func_name, value);
                break;
            }
            case emIIR:
            {
                if (func_name == "NULL")
                {
                    mpIIR->ChgProcessingTime(value);
                }
                else
                {
                    re_printf("warning", "func_name should be 'NULL' for Sub-ip name(%s), the command is ignored", sub_ip_name.c_str());
                }
                break;
            }
            case emRCTG:
            {
                if (func_name == "NULL")
                {
                    mpRCTG->ChgProcessingTime(value);
                }
                else
                {
                    re_printf("warning", "func_name should be 'NULL' for Sub-ip name(%s), the command is ignored", sub_ip_name.c_str());
                }
                break;
            }
            case emRSLVIDE:
            {
                mpRSLVIDE->ChgProcessingTime(func_name, value);
                break;
            }
            case emPI:
            {
                mpPI->ChgProcessingTime(func_name, value);
                break;
            }
            case emPWM:
            {
                mpPWM->ChgProcessingTime(func_name, value);
                break;
            }
            case emINT:
            case emVRFBUF:
            {
                if (func_name == "NULL")
                {
                    mpINT->ChgProcessingTime(value);
                }
                else
                {
                    re_printf("warning", "func_name should be 'NULL' for Sub-ip name(%s), the command is ignored", sub_ip_name.c_str());
                }
                break;
            }
            default:
            {
                re_printf("warning", "Sub-ip name(%s) is not supported this API, the command is ignored", sub_ip_name.c_str());
                break;
            }
        }
    }
}

// Description: Change the extension of Sub-IP AHB transaction
// Ref: {}
void EMU3S_HWCORE::SetAHBExtension(unsigned int value)
{
    mpADTRG->SetAHBExtension(value);
}

// Description: Set EDC errors
// Ref: {EMU3S_HWCORE_UD_Feature_022, EMU3S_HWCORE_UD_Feature_023}
// Ref: {EMU3S_HWCORE_UD_Direction_044, EMU3S_HWCORE_UD_Direction_045, EMU3S_HWCORE_UD_Flow_027}
void EMU3S_HWCORE::SetEDCError(unsigned char domain, unsigned char error_type, unsigned char status)
{
    if ((domain <= 1) && (error_type <=1) && (status <=1))
    {
        if (!GetResetStatus("EMUST"))
        {
            if (((domain == 0) && (!GetResetStatus("PRESETn")) && (CheckClockPeriod("PCLK")))
                || ((domain == 1) && (!GetResetStatus("CRESETn")) && (CheckClockPeriod("CCLK"))))
            {
                unsigned int EDCDIS = mpEMU3S_HWCORE_Func->GetRegVal("EDCCTR", "EDCDIS");
                if (EDCDIS == 0)
                {
                    if (error_type == 0)
                    {
                        mpEMU3S_HWCORE_Func->SetRegVal("EDCSERR", "SES", (unsigned int)status);
                    }
                    else
                    {
                        mpEMU3S_HWCORE_Func->SetRegVal("EDCDERR", "DES", (unsigned int)status);
                    }

                    if (status == 1)
                    {
                        if ((domain == 0) && (error_type == 0))
                        {
                            mAssertEdcErrSedPclkEvent.notify();
                        }
                        else if ((domain == 0) && (error_type == 1))
                        {
                            mAssertEdcErrDedPclkEvent.notify();
                        }
                        else if ((domain == 1) && (error_type == 0))
                        {
                            mAssertEdcErrSedCclkEvent.notify();
                        }
                        else // domain = 1 and error_type = 1
                        {
                            mAssertEdcErrDedCclkEvent.notify();
                        }
                    }
                }
            }
            else
            {
                re_printf("Info", "SetEDCError command does not work when clock is 0 or reset is active\n");
            }
        }
        else
        {
            re_printf("Info", "SetEDCError command does not work when clock is 0 or reset is active\n");
        }
    }
    else
    {
        re_printf("warning", "A value of arguments of SetEDCError command is prohibited (other than 0 and 1), the command is ignored\n");
    }
}

// Description: Dump help message of all parameters or commands
// Ref: {EMU3S_HWCORE_UD_Direction_021, EMU3S_HWCORE_UD_Direction_022, EMU3S_HWCORE_UD_Direction_029}
void EMU3S_HWCORE::Help(const std::string type)
{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") 
        {
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_MessageLevel          (EMU3S_HWCORE instance, fatal|error|warning|info, sub_ip_name)   Set debug message level(default: fatal|error). for 1 sub-ip default for all(default)");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_DumpRegisterRW        (EMU3S_HWCORE instance, true|false, sub_ip_name)                 Enable/disable dumping access register(default: false) in 1 sub-ip or all(default)   .");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_EnableRegisterMessage (EMU3S_HWCORE instance, true|false, sub_ip_name)                 Enable/disable info/warning/error message of register IF(default: true) in 1 sub-ip or all (default).");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_DumpInterrupt         (EMU3S_HWCORE instance, true|false, sub_ip_name)                 Enable/disable dumping interrupt information(default:false)  in 1 sub-ip or all(default).");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_EnableStatInfo        (EMU3S_HWCORE instance, true|false, sub_ip_name)                 Enable/disable dumping automatically statistical information (default:false)  in 1 sub-ip or all(default).");
        }
        else if (word_vector[0] == "commands") 
        {
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_AssertReset        (EMU3S_HWCORE instance, rst_name, start_time, period,sub_ip_name )   Assert and de-assert reset signal to the 1 sub-ip of EMU3S_HWCORE model or all(default).");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_SetCLKFreq         (EMU3S_HWCORE instance, clk_name, freq, unit, sub_ip_name)           Set clock frequency to 1 sub-ip of EMU3S_HWCORE model or all (default).");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_GetCLKFreq         (EMU3S_HWCORE instance, clk_name, sub_ip_name)                       Get clock frequency of 1 sub-ip of EMU3S_HWCORE model or all (default).");

            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_ForceRegister      (EMU3S_HWCORE instance, reg_name, value, sub_ip_name)                Force register with setting value for register of sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_ReleaseRegister    (EMU3S_HWCORE instance, reg_name, value, sub_ip_name)                Release register from force value for register of sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_WriteRegister      (EMU3S_HWCORE instance, reg_name , value, sub_ip_name)               Write a value to register for sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_ReadRegister       (EMU3S_HWCORE instance, reg_name , value, sub_ip_name)               Read a value from register for sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_ListRegister       (EMU3S_HWCORE Instance, sub_ip_name)                                 Dump register names which is passed as sub_ip_name");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_DumpStatInfo       (EMU3S_HWCORE instance, sub_ip_name)                                 Dump the statistical information of sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_ChgProcessingTime  (EMU3S_HWCORE instance, sub_ip_name, func_name, value)               Change the process time of the func_name of sub-ip.");
            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_SetAHBExtension    (EMU3S_HWCORE instance, value)                                       Change the extension of Sub-IP AHB transaction.");

            SC_REPORT_INFO(this->basename(), "EMU3S_HWCORE_SetEDCError        (EMU3S_HWCORE instance, domain, error_type, status)                  Set EDC errors.");

        }
        else 
        {
            re_printf("warning", "The name (%s) of EMU3S_HWCORE_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    }
    else 
    {
        re_printf("warning", "The name (%s) of EMU3S_HWCORE_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
} 

//========================================================
//============    Internal Functions   ===================
//========================================================
// Description: Get Reset Status
// Ref: {}
bool EMU3S_HWCORE::GetResetStatus(const std::string reset_name)
{
    bool reset_status = false;
    if(reset_name == "PRESETn") 
    {
        if (mIsPRESETnHardActive || mIsPRESETnCmdActive) 
        {
            reset_status = true;
        }
    }
    else if (reset_name == "CRESETn")
    {
        if (mIsCRESETnHardActive || mIsCRESETnCmdActive) 
        {
            reset_status = true;
        }
    }
    else if (reset_name == "EMUST")
    {
        if (IsSWReset)
        {
            reset_status = true;
        }
    }
    
    return reset_status;
}
// Description: Check Clock Period
// Ref: {}
bool EMU3S_HWCORE::CheckClockPeriod(const std::string clock_name)
{
    bool clock_status = false;
    if(clock_name == "PCLK") 
    {
        if (mPCLKFreq > 0) {
            clock_status = true;
        } else {
            clock_status = false;
            re_printf("info","PCLK frequency is zero\n");
        }
    }
    else if (clock_name == "CCLK") 
    {
        if (mCCLKFreq > 0) {
            clock_status = true;
        } else {
            clock_status = false;
            re_printf("info","CCLK frequency is zero\n");
        }
    }
    return clock_status;
}

// Description: Get Sub-IP instance based on offsetAddress
// Ref: {EMU3S_HWCORE_UD_Registers_038, EMU3S_HWCORE_UD_Registers_039}
// Ref: {EMU3S_HWCORE_UD_Registers_073, EMU3S_HWCORE_UD_Registers_074, EMU3S_HWCORE_UD_Registers_075}
// Ref: {EMU3S_HWCORE_UD_Registers_076, EMU3S_HWCORE_UD_Registers_077, EMU3S_HWCORE_UD_Registers_078}
// Ref: {EMU3S_HWCORE_UD_Registers_079, EMU3S_HWCORE_UD_Registers_080, EMU3S_HWCORE_UD_Registers_081}
// Ref: {EMU3S_HWCORE_UD_Registers_082, EMU3S_HWCORE_UD_Registers_083, EMU3S_HWCORE_UD_Registers_084}
// Ref: {EMU3S_HWCORE_UD_Registers_085, EMU3S_HWCORE_UD_Registers_086, EMU3S_HWCORE_UD_Registers_087}
// Ref: {EMU3S_HWCORE_UD_Registers_088, EMU3S_HWCORE_UD_Registers_089, EMU3S_HWCORE_UD_Registers_090}
// Ref: {EMU3S_HWCORE_UD_Registers_091}
CRWInterface* EMU3S_HWCORE::GetInstance(unsigned int offsetAddress)
{
    // offsetAddress is always larger than 0x0B9 when this function is called
    if ((offsetAddress >= 0x100) && (offsetAddress <= 0x110 + 1)) 
    { // List of EMU3S_HWCORE Registers (Angle Generation IP)
        return mpRSLVIDE;
    }
    else if ((offsetAddress >= 0x180) && (offsetAddress <= 0x1CC + 1))
    { // List of the EMU3S_HWCORE Registers (A/D Conversion Control)
        return mpADTRG;
    }
    else if ((offsetAddress >= 0x200) && (offsetAddress <= 0x254 + 1)) 
    { // List of the EMU3S_HWCORE Registers (Input IP)
        return mpINPUT;
    }
    else if ((offsetAddress >= 0x2C0) && (offsetAddress <= 0x31C + 3))
    { // List of the EMU3S_HWCORE Registers (PI Control IP)
        return mpPI;
    }
    else if ((offsetAddress >= 0x340) && (offsetAddress <= 0x434 + 3))
    { // List of the EMU3S_HWCORE Registers (PWM IP)
        return mpPWM;
    }
    else if ((offsetAddress >= 0x4C0) && (offsetAddress <= 0x4D0 + 1)) 
    { // List of the EMU3S_HWCORE Registers (Rectangle IP/Batch Rectangle IP)
        return mpRCTG;
    }
    else if ((offsetAddress >= 0x500) && (offsetAddress <= 0x57C + 3))
    { // List of the EMU3S_HWCORE Registers (Independent Rectangle IP3)
        return mpIRCTG3;
    }
    else if ((offsetAddress >= 0x5C0) && (offsetAddress <= 0x61C + 3)) 
    { // List of the EMU3S_HWCORE Registers (IIR Filter)
        return mpIIR;
    }
    else if ((offsetAddress >= 0x6A0) && (offsetAddress <= 0x6B0)) 
    { // List of the EMU3S_HWCORE Registers (Resolver Angle Measurement Timer)
        return mpMEASURE;
    }
    else if ((offsetAddress >= 0x700) && (offsetAddress <= 0x745))
    { // List of the EMU3S_HWCORE Registers (Interrupt/Error Detection)
        return mpINT;
    }
    else if ((offsetAddress >= 0x780) && (offsetAddress <= 0x7A1))
    { // List of the EMU3S_HWCORE Registers (Verification Buffer)
        return mpINT;
    }
    else if ((offsetAddress >= 0x800) && (offsetAddress <= 0x88E + 1)) 
    { // List of the EMU3S_HWCORE Registers (Handshake Register)
        return mpHWCORE_HANDSHAKE_Func;
    }
    else
    {
        return NULL;
    }
}

// Description: Write CTR_sig
// Ref: {}
void EMU3S_HWCORE::WriteCTRMethod(void)
{
    unsigned char val = (unsigned char) (mpEMU3S_HWCORE_Func->GetRegVal("CTR") & 0xFF);
    CTR_sig.write(val);
}
// Description: Write REFCTR_sig
// Ref: {}
void EMU3S_HWCORE::WriteREFCTRMethod(void)
{
    unsigned char val = (unsigned char)(mpEMU3S_HWCORE_Func->GetRegVal("REFCTR") & 0xFF);
    REFCTR_sig.write(val);
}
// Description: Write AHBPRT_sig
// Ref: {}
void EMU3S_HWCORE::WriteAHBPRTMethod(void)
{
    unsigned char val = (unsigned char)(mpEMU3S_HWCORE_Func->GetRegVal("AHBPRT") & 0xFF);
    AHBPRT_sig.write(val);
}
// Description: Monitor IPTRGCTR register
// Ref: {}
void EMU3S_HWCORE::WriteIPTRGCTRMethod(void)
{
    unsigned char val = (unsigned char) (mpEMU3S_HWCORE_Func->GetRegVal("IPTRGCTR") & 0xFF);
    if (val == 0x0)
    {
        mINTRGNUMCnt = 0;
    }
    IPTRGCTR_sig.write(val);
}
// Description: Write IPTRGNUM_sig
// Ref: {}
void EMU3S_HWCORE::WriteIPTRGNUMMethod(void)
{
    unsigned int val = (unsigned int)(mpEMU3S_HWCORE_Func->GetRegVal("IPTRGNUM") & 0xFFFFFFFF);
    IPTRGNUM_sig.write(val);
}
// Description: Write TBLADR_sig
// Ref: {}
void EMU3S_HWCORE::WriteTBLADRMethod(void)
{
    unsigned int val = (unsigned int)(mpEMU3S_HWCORE_Func->GetRegVal("TBLADR") & 0xFFFFFFFF);
    TBLADR_sig.write(val);
}
// Description: Write FUNCIDLEGRPA0_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCIDLEGRPA0Method(void)
{
    FUNCIDLEGRPA0_sig.write((unsigned short) FUNCIDLEGRPA0);
    if ((FUNCIDLEGRPA0 >> emSFTEN_POS) & 0x1) // SFTEN = 1 // Workaround for write 1 in SFTEN multiple times continuosly
    {
        // Clear SFTEN (in signal) after 1 cycle
        FUNCIDLEGRPA0 &= emSFTEN_MASK;
    }
}
// Description: Write FUNCIDLEGRPA1_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCIDLEGRPA1Method(void)
{
    FUNCIDLEGRPA1_sig.write((unsigned short) FUNCIDLEGRPA1);
    if ((FUNCIDLEGRPA1 >> emSFTEN_POS) & 0x1) // SFTEN = 1 // Workaround for write 1 in SFTEN multiple times continuosly
    {
        // Clear SFTEN (in signal) after 1 cycle
        FUNCIDLEGRPA1 &= emSFTEN_MASK;
    }
}
// Description: Write FUNCIDLEGRPA2_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCIDLEGRPA2Method(void)
{
    FUNCIDLEGRPA2_sig.write((unsigned short) FUNCIDLEGRPA2);
    if ((FUNCIDLEGRPA2 >> emSFTEN_POS) & 0x1) // SFTEN = 1 // Workaround for write 1 in SFTEN multiple times continuosly
    {
        // Clear SFTEN (in signal) after 1 cycle
        FUNCIDLEGRPA2 &= emSFTEN_MASK;
    }
}
// Description: Write FUNCIDLEGRPB_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCIDLEGRPBMethod(void)
{
    unsigned short val = (unsigned short)(mpEMU3S_HWCORE_Func->GetRegVal("FUNCIDLEGRPB") & 0xFFFF);
    FUNCIDLEGRPB_sig.write(val);
}
// Description: Write FUNCWAITGRPA_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCWAITGRPAMethod(void)
{
    FUNCWAITGRPA_sig.write((unsigned short) FUNCWAITGRPA);
    if ((FUNCWAITGRPA >> emIP_POS) & 0x3) // IP != 0
    {
        // Clear IP (in signal) after 1 cycle // Workaround for write 1 in IP multiple times continuosly
        FUNCWAITGRPA &= emIP_MASK;
    }
}
// Description: Write FUNCWAITGRPB_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCWAITGRPBMethod(void)
{
    FUNCWAITGRPB_sig.write((unsigned short) FUNCWAITGRPB);
    if ((FUNCWAITGRPB >> emIP_POS) & 0x3) // IP != 0
    {
        // Clear IP (in signal) after 1 cycle // Workaround for write 1 in SFTEN multiple times continuosly
        FUNCWAITGRPB &= emIP_MASK;
    }
}
// Description: Write FUNCFLGRPA0_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCFLGRPA0Method(void)
{
    unsigned short val = (unsigned short) (mpEMU3S_HWCORE_Func->GetRegVal("FUNCFLGRPA0") & 0xFFFF);
    FUNCFLGRPA0_sig.write(val);
}
// Description: Write FUNCFLGRPA1_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCFLGRPA1Method(void)
{
    unsigned char val = (unsigned char)(mpEMU3S_HWCORE_Func->GetRegVal("FUNCFLGRPA1") & 0xFF);
    FUNCFLGRPA1_sig.write(val);
}
// Description: Write FUNCFLGRPA2_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCFLGRPA2Method(void)
{
    unsigned int val = (unsigned int)(mpEMU3S_HWCORE_Func->GetRegVal("FUNCFLGRPA2") & 0xFFFFFFFF);
    FUNCFLGRPA2_sig.write(val);
}
// Description: Write FUNCFLGRPB_sig
// Ref: {}
void EMU3S_HWCORE::WriteFUNCFLGRPBMethod(void)
{
    unsigned int val = (unsigned int)(mpEMU3S_HWCORE_Func->GetRegVal("FUNCFLGRPB") & 0xFFFFFFFF);
    FUNCFLGRPB_sig.write(val);
}
// Description: Write ADSEL_sig
// Ref: {}
void EMU3S_HWCORE::WriteADSELMethod(void)
{
    bool val = (bool)(mpEMU3S_HWCORE_Func->GetRegVal("ADRDCSL", "ADSEL") & 0x1);
    ADSEL_sig.write(val);
}
// Description: Write SFERC
// Ref: {}
void EMU3S_HWCORE::WriteSFERCMethod(void)
{
    unsigned char val = (unsigned char)(mpEMU3S_HWCORE_Func->GetRegVal("SFERC") & 0xFF);
    SFERC.write(val);
}

// Description: Notify Write CTR_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteCTR()
{
    mWriteCTREvent.notify();
}
// Description: Notify Write REFCTR_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteREFCTR()
{
    mWriteREFCTREvent.notify();
}
// Description: Notify Write AHBPRT_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteAHBPRT()
{
    mWriteAHBPRTEvent.notify();
}
// Description: Notify Write TBLADR_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteTBLADR()
{
    mWriteTBLADREvent.notify();
}
// Description: Notify Write IPTRGCTR_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteIPTRGCTR()
{
    mWriteIPTRGCTREvent.notify();
}
// Description: Notify Write IPTRGNUM_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteIPTRGNUM()
{
    mWriteIPTRGNUMEvent.notify();
}
// Description: Notify Write FUNCIDLEGRPA0_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCIDLEGRPA0()
{
    FUNCIDLEGRPA0 =  (mpEMU3S_HWCORE_Func->GetRegVal("FUNCIDLEGRPA0") & 0xFFFF);
    mWriteFUNCIDLEGRPA0Event.notify();
    if ((FUNCIDLEGRPA0 >> emSFTEN_POS) & 0x1) // SFTEN = 1 // Workaround for write 1 in SFTEN multiple times continuosly
    {
        // Clear SFTEN (in signal) after 1 cycle
        double current_time = sc_time_stamp().to_double();
        double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
        mWriteFUNCIDLEGRPA0Event.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
    }
}
// Description: Notify Write FUNCIDLEGRPA1_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCIDLEGRPA1()
{
    FUNCIDLEGRPA1 = (mpEMU3S_HWCORE_Func->GetRegVal("FUNCIDLEGRPA1") & 0xFFFF);
    mWriteFUNCIDLEGRPA1Event.notify();
    if ((FUNCIDLEGRPA1 >> emSFTEN_POS) & 0x1) // SFTEN = 1 // Workaround for write 1 in SFTEN multiple times continuosly
    {
        // Clear SFTEN (in signal) after 1 cycle
        double current_time = sc_time_stamp().to_double();
        double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
        mWriteFUNCIDLEGRPA1Event.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
    }
}
// Description: Notify Write FUNCIDLEGRPA2_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCIDLEGRPA2()
{
    FUNCIDLEGRPA2 = (mpEMU3S_HWCORE_Func->GetRegVal("FUNCIDLEGRPA2") & 0xFFFF);
    mWriteFUNCIDLEGRPA2Event.notify();
    if ((FUNCIDLEGRPA2 >> emSFTEN_POS) & 0x1) // SFTEN = 1 // Workaround for write 1 in SFTEN multiple times continuosly
    {
        // Clear SFTEN (in signal) after 1 cycle
        double current_time = sc_time_stamp().to_double();
        double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
        mWriteFUNCIDLEGRPA2Event.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
    }
}
// Description: Notify Write FUNCIDLEGRPB_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCIDLEGRPB()
{
    mWriteFUNCIDLEGRPBEvent.notify();
}
// Description: Notify Write FUNCWAITGRPA_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCWAITGRPA()
{
    FUNCWAITGRPA = (mpEMU3S_HWCORE_Func->GetRegVal("FUNCWAITGRPA") & 0xFFFF);
    mWriteFUNCWAITGRPAEvent.notify();
    if ((FUNCWAITGRPA >> emIP_POS) & 0x3) // IP != 0
    {
        // Clear IP (in signal) after 1 cycle // Workaround for write 1 in IP multiple times continuosly
        double current_time = sc_time_stamp().to_double();
        double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
        mWriteFUNCWAITGRPAEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
    }
}
// Description: Notify Write FUNCWAITGRPB_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCWAITGRPB()
{
    FUNCWAITGRPB = (mpEMU3S_HWCORE_Func->GetRegVal("FUNCWAITGRPB") & 0xFFFF);
    mWriteFUNCWAITGRPBEvent.notify();
    if ((FUNCWAITGRPB >> emIP_POS) & 0x3) // IP != 0
    {
        // Clear IP (in signal) after 1 cycle // Workaround for write 1 in SFTEN multiple times continuosly
        double current_time = sc_time_stamp().to_double();
        double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
        mWriteFUNCWAITGRPBEvent.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
    }
}
// Description: Notify Write FUNCFLGRPA0_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCFLGRPA0()
{
    mWriteFUNCFLGRPA0Event.notify();
}
// Description: Notify Write FUNCFLGRPA1_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCFLGRPA1()
{
    mWriteFUNCFLGRPA1Event.notify();
}
// Description: Notify Write FUNCFLGRPA2_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCFLGRPA2()
{
    mWriteFUNCFLGRPA2Event.notify();
}
// Description: Notify Write FUNCFLGRPB_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteFUNCFLGRPB()
{
    mWriteFUNCFLGRPBEvent.notify();
}
// Description: Notify Write ADSEL_sig
// Ref: {}
void EMU3S_HWCORE::NotifyWriteADSEL()
{
    mWriteADSELEvent.notify();
}
// Description: Notify Write SFERC
// Ref: {}
void EMU3S_HWCORE::NotifyWriteSFERC()
{
    mWriteSFERCEvent.notify();
}

// Description: Dump Interrupt Message
// Ref: {EMU3S_HWCORE_UD_Direction_046}
void EMU3S_HWCORE::DumpInterruptMsg (const std::string intr_name, const bool value)
{
   if (mDumpInterrupt) 
   {
        if (value) 
        {
            re_printf("info","[%s] is changed to %s.\n",intr_name.c_str(),"1");
        } 
        else 
        {
            re_printf("info","[%s] is changed to %s .\n",intr_name.c_str(),"0");
        }
   }
}