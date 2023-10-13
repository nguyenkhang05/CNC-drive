// ---------------------------------------------------------------------
// $Id: HWCORE_ADTRG.cpp,v 1.14 2020/10/24 07:16:55 huepham Exp $
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
// Description: Copyright of HWCORE_ADTRG.cpp
// Ref: {HWCORE_ADTRG_UD_Copyright_001}
// Description: About the include header files of HWCORE_ADTRG.cpp
// Ref: {HWCORE_ADTRG_UD_ATT1_002, HWCORE_ADTRG_UD_Direction_028}
#include "re_define.h"
#include "HWCORE_ADTRG.h"
#include "HWCORE_ADTRG_Func.h"
// Description: Constructor of HWCORE_ADTRG class
// Ref: {HWCORE_ADTRG_UD_Summary_001, HWCORE_ADTRG_UD_ATT1_004}
HWCORE_ADTRG::HWCORE_ADTRG(sc_module_name name, SIM_MODE_T simmode )
    :sc_module(name)
    ,HWCORE_ADTRG_AgentController()
    ,CRWInterface()
    ,BusMasterBase<BUS_WIDTH_AHB, 1>()
    ,rvc_common_model()
    ,PCLK("PCLK")
    ,PRESETn("PRESETn")
    ,CCLK("CCLK")
    ,crst_n("crst_n")
    //HWCORE_RSLVIDE
    ,theta_res_fix_reg("theta_res_fix_reg")
    ,theta_e_fix_reg("theta_e_fix_reg")
    ,ang_comp0_eq_sp("ang_comp0_eq_sp")
    //HWCORE_IRCTG3
    ,irctg3_compmatch_u("irctg3_compmatch_u")
    ,irctg3_compmatch_v("irctg3_compmatch_v")
    ,irctg3_compmatch_w("irctg3_compmatch_w")
    //HWCORE_INPUT
    ,em2ictrinmd_instctr("em2ictrinmd_instctr")
    //EMU3S_HWCORE
    ,emu3ictr_emust("emu3ictr_emust")
    ,em3iadsel("em3iadsel")
    ,emu3iptrgctr("emu3iptrgctr")
    ,emu3iptrgnum("emu3iptrgnum")
    ,adc_grpend("adc_grpend")
    ,adc_ch0end("adc_ch0end")
    ,adc_ch1end("adc_ch1end")
    ,adc_ch2end("adc_ch2end")
    ,adc_ch0_dat("adc_ch0_dat")
    ,adc_ch1_dat("adc_ch1_dat")
    ,adc_ch2_dat("adc_ch2_dat")
    ,tstpektim("tstpektim")
    ,tstvlytim("tstvlytim")
    ,tstadt0("tstadt0")
    ,tstadt1("tstadt1")
    ,tstint3("tstint3")
    ,tstint4("tstint4")
    ,emu3tbladr("emu3tbladr")
    ,ahbprt("ahbprt")
    //EMU3S_IIR
    ,iiroutdat0("iiroutdat0")
    ,iiroutdat1("iiroutdat1")
    ,iiroutdat2("iiroutdat2")
    //EMU3S_HWCORE
    ,adc_start("adc_start" )
    ,em3iadctr_adend("em3iadctr_adend")
    ,emu3_iptrgmon_adtrgnum ("emu3_iptrgmon_adtrgnum")
    //HWCORE_IIR Filter
    ,rdata_em3iad0("rdata_em3iad0")
    ,rdata_em3iad1("rdata_em3iad1")
    ,rdata_em3iad2("rdata_em3iad2")
    //HWCORE_INT
    ,adtrg_end_trig("adtrg_end_trig")
    ,ad_end_trig_sp("ad_end_trig_sp")
    ,adtrg_mvint0("adtrg_mvint0")
    ,adtrg_mvint1("adtrg_mvint1")
    ,adtrg_mem_rg_chg("adtrg_mem_rg_chg")
    ,adtrg_smerr("adtrg_smerr")
    ,adtrg_amerr("adtrg_amerr")
    ,ad_start_mou_sp_80m("ad_start_mou_sp_80m")
    ,ad_start_vall_sp_80m("ad_start_vall_sp_80m")
    //HWCORE_Input IP
    ,adtrg_ch0_dat("adtrg_ch0_dat")
    ,adtrg_ch1_dat("adtrg_ch1_dat")
    ,adtrg_ch2_dat("adtrg_ch2_dat")
    ,adtrg_thte("adtrg_thte")
    ,adtrg_thtr("adtrg_thtr")
{//{{{

    mpHWCORE_ADTRGFunc = new HWCORE_ADTRG_Func((std::string)name, this);
    sc_assert(mpHWCORE_ADTRGFunc != NULL);

    BusMasterBase<BUS_WIDTH_AHB,1>::setMasterResetPort32(&PRESETn);
    BusMasterBase<BUS_WIDTH_AHB,1>::setMasterFreqPort32(&PCLK);
    BusMasterBase<BUS_WIDTH_AHB,1>::setInitiatorSocket32((char*)"is");

    is = BusMasterBase<BUS_WIDTH_AHB, 1>::iSocket32[0];

    mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)mpHWCORE_ADTRGFunc);
    mBusMasterIf32[0]->setBusProtocol(BUS_AHB);
    mBusMasterIf32[0]->setTransNmbLmt(0xFFFFFFFF);
    mBusMasterIf32[0]->setTlmTransType(simmode);
    mSimMode = simmode;

    /* AHB master */
    mAhbMasterIf              = mBusMasterIf32[0];
    
    mAHBTrans               = new TlmBasicPayload;

    mAHBTransG3Ext          = new TlmG3mExtension;
    mAHBTransExt            = new TlmAhbExtension;

    mAHBTrans->set_write();
    mAHBTrans->set_extension(mAHBTransG3Ext);
    mAHBTrans->set_extension(mAHBTransExt);
    mAHBTrans->set_data_length(4);
    
    mpData                  = new unsigned int [2];
    memset(mpData, 0x00, 2 * 4);
    mSPID                   = 0;

    // Initialize variables
    mPCLKPeriod             = 0;
    mPCLKOrgFreq            = 0;
    mPCLKFreq               = 0;
    mPCLKFreqUnit           = "Hz";
    mStartPCLKTime          = 0;

    mCCLKPeriod             = 0;
    mCCLKOrgFreq            = 0;
    mCCLKFreq               = 0;
    mCCLKFreqUnit           = "Hz";
    mStartCCLKTime          = 0;

    mTimeResolutionValue    = 1;
    mTimeResolutionUnit     = SC_NS;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;

        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    };

    mDumpInterrupt          = false;
    mEnableRegisterMessage  = true;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    // Initialize output port
    InitializeSSCG(emINITIALIZE);
    InitializeCLEAN(emINITIALIZE);

    // SC_THREAD/METHOD
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleCCLKSignalMethod);
    dont_initialize();
    sensitive << CCLK;

    SC_METHOD(HandlePRESETNSignalMethod);
    sensitive << PRESETn;

    SC_METHOD(HandleCRSTNSignalMethod);
    sensitive << crst_n;

    SC_METHOD(HandleEMUSTSignalMethod);
    sensitive << emu3ictr_emust;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_ADTRG::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_ADTRG::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_ADTRG::CancelResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    for (unsigned int port_id = 0; port_id < emOUPUT_PORT_EDGE_NUM; port_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&m_update_output_edge_event[port_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_ADTRG::HandleOutputEdgePortMethod, this, port_id),
            sc_core::sc_gen_unique_name("HandleOutputEdgePortMethod"), &opt);
    }
    for (unsigned short port_id = 0; port_id < emOUPUT_PORT_EDGE_NUM; port_id++) {
        m_output_port_edge_value[port_id]  = false;
    }

    for (unsigned int port_id = 0; port_id < emOUPUT_PORT_DATA_NUM; port_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&m_update_output_data_event[port_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_ADTRG::HandleOutputDataPortMethod, this, port_id),
            sc_core::sc_gen_unique_name("HandleOutputDataPortMethod"), &opt);
    }
    for (unsigned short port_id = 0; port_id < emOUPUT_PORT_DATA_NUM; port_id++) {
        m_output_port_data_value[port_id]  = 0;
    }
    
    SC_METHOD(HandleADSelectSignalMethod);
    dont_initialize();
    sensitive << em3iadsel;

    SC_METHOD(HandleCarrierPeakMethod);
    dont_initialize();
    sensitive << tstadt0.pos();
    sensitive << tstpektim.pos();

    SC_METHOD(HandleCarrierTroughMethod);
    dont_initialize();
    sensitive << tstadt1.pos();
    sensitive << tstvlytim.pos();

    SC_METHOD(HandleADStartTriggerMethod);
    dont_initialize();
    sensitive << m_ad_start_trg_event;

    SC_METHOD(HandleADSamplingPeakMethod);
    dont_initialize();
    sensitive << m_ad_sampling_mou_trg_event_queue;

    SC_METHOD(HandleADSamplingTroughMethod);
    dont_initialize();
    sensitive << m_ad_sampling_vall_trg_event_queue;

    SC_METHOD(HandleAngLatchingRegisterMethod);
    dont_initialize();
    sensitive << m_rd_trg_event;

    SC_METHOD(HandleAngLatchingPortMethod);
    dont_initialize();
    sensitive << m_ang_latch_event;

    SC_METHOD(HandleAngComp0Method);
    dont_initialize();
    sensitive << ang_comp0_eq_sp.pos();

    SC_METHOD(HandleIR3UPhaseMethod);
    dont_initialize();
    sensitive << irctg3_compmatch_u.pos();

    SC_METHOD(HandleIR3VPhaseMethod);
    dont_initialize();
    sensitive << irctg3_compmatch_v.pos();

    SC_METHOD(HandleIR3WPhaseMethod);
    dont_initialize();
    sensitive << irctg3_compmatch_w.pos();


    SC_METHOD(HandleSkipStartupTriggerCounterMethod);
    dont_initialize();
    sensitive << m_emu3_iptrgmon_adtrgnum_event;

    SC_METHOD(HandleCompletionSignalfromADCMethod);
    dont_initialize();
    sensitive << adc_ch0end.pos();
    sensitive << adc_ch1end.pos();
    sensitive << adc_ch2end.pos();
    sensitive << adc_grpend.pos();


    SC_METHOD(HandleCompletionSignalfromTSG3Method);
    dont_initialize();
    sensitive << tstint3.pos();
    sensitive << tstint4.pos();

    SC_METHOD(HandleADCompletionMethod);
    dont_initialize();
    sensitive << m_adc_completion_event;

    SC_METHOD(HandleIIRCompletionMethod);
    dont_initialize();
    sensitive << iiroutdat0;
    sensitive << iiroutdat1;
    sensitive << iiroutdat2;
    
    SC_THREAD(ProcessWriteMemoryThread);
    sensitive << m_issue_write;
}//}}}

// Description: Destructor
HWCORE_ADTRG::~HWCORE_ADTRG(void)
{//{{{
    
    mAHBTrans->get_extension(mAHBTransG3Ext);
    mAHBTrans->release_extension(mAHBTransG3Ext);
    mAHBTrans->get_extension(mAHBTransExt);
    mAHBTrans->release_extension(mAHBTransExt);

    delete mAHBTrans;
    delete mpHWCORE_ADTRGFunc;
    delete[] mpData;
}//}}}

// Description: Initialize output port in CLEAN domain when crst_n port activated
void HWCORE_ADTRG::InitializeCLEAN(eRESET_CONTROL emType)
{//{{{
    //----Initialize variable--------
    //EMU3S_HWCORE
    m_adtrg_att                       = 0;
    m_ad_trig_count                   = 0;
    m_ad_samp_mou_count               = 0;
    m_ad_samp_vall_count              = 0;
    m_ang_samp_vall_lat_count         = 0;
    m_ang_samp_mou_lat_count          = 0;
    m_ad_start_trg_delay              = 0;
    m_ang_start_trg_delay             = 0;
    m_adc_status                      = emAD_IDLE;
    m_emu3_iptrgmon_adtrgnum_value    = 0;
    m_adtrg_mem_write_time            = 0;
    m_ADTRG0                          = false;
    m_ADTRG1                          = false;
    m_adtrg_in_delay_period           = false;

    //----Initialize output port---------
    // EMU3S_HWCORE
    if(emType == emINITIALIZE){
        adc_start.initialize(false);
    }
    else{
        m_output_port_edge_value[emADC_START_PORT] = false;
        m_update_output_edge_event[emADC_START_PORT].notify(SC_ZERO_TIME);
    }
}//}}}

// Description: Initialize output port in SSCG domain when PRESETn port activated
void HWCORE_ADTRG::InitializeSSCG(eRESET_CONTROL emType)
{//{{{
    //----Initialize variable--------
    m_em3iadsel_pre_val = false;
    m_adtrg0_skip_count = 0;
    m_adtrg1_skip_count = 0;
    m_Data0 = 0;
    m_Data1 = 0;
    m_pAddr = 0;
    m_mnnum = 0;
    m_switching_area = false;

    //----Initialize output port---------
    if(emType == emINITIALIZE){
        // EMU3S_HWCORE
        em3iadctr_adend.initialize(0);
        emu3_iptrgmon_adtrgnum.initialize(0);
        // HWCORE_IIR Filter
        rdata_em3iad0.initialize(0);
        rdata_em3iad1.initialize(0);
        rdata_em3iad2.initialize(0);
        // HWCORE_Input IP
        adtrg_ch0_dat.initialize(0);
        adtrg_ch1_dat.initialize(0);
        adtrg_ch2_dat.initialize(0);
        adtrg_thte.initialize(0);
        adtrg_thtr.initialize(0);
        // HWCORE_INT
        adtrg_end_trig.initialize(false);
        ad_end_trig_sp.initialize(false);
        adtrg_mvint0.initialize(false);
        adtrg_mvint1.initialize(false);
        adtrg_mem_rg_chg.initialize(false);
        adtrg_smerr.initialize(false);
        adtrg_amerr.initialize(false);
        ad_start_mou_sp_80m.initialize(false);
        ad_start_vall_sp_80m.initialize(false);
    }
    else{
        for (unsigned short port_id = 0; port_id < emOUPUT_PORT_DATA_NUM; port_id++) {
            m_output_port_data_value[port_id]  = 0;
            m_update_output_data_event[port_id].notify(SC_ZERO_TIME);
        }
        for (unsigned short port_id = 1; port_id < emOUPUT_PORT_EDGE_NUM; port_id++) {
            m_output_port_edge_value[port_id]  = false;
            m_update_output_edge_event[port_id].notify(SC_ZERO_TIME);
        }
    }

    
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
// Description: Monitor PRESETN port
// Ref: {HWCORE_ADTRG_UD_Ports_004}
void HWCORE_ADTRG::HandlePRESETNSignalMethod(void)
{//{{{
    mResetCurVal[emPRESETnId] = PRESETn.read();
    // Reset signals are asynchronous
    mResetHardEvent[emPRESETnId].notify(SC_ZERO_TIME);
}//}}}

// Description: Monitor crst_n port
// Ref: {HWCORE_ADTRG_UD_Ports_002}
void HWCORE_ADTRG::HandleCRSTNSignalMethod()
{//{{{
    mResetCurVal[emcrst_nId] = crst_n.read();
    // Reset signals are asynchronous
    mResetHardEvent[emcrst_nId].notify(SC_ZERO_TIME);
}//}}}

// Description: Monitor emust port
// Ref: {HWCORE_ADTRG_UD_Ports_013}
void HWCORE_ADTRG::HandleEMUSTSignalMethod ()
{//{{{
    mResetCurVal[emctr_emustId] = emu3ictr_emust.read();
    // Reset signals are asynchronous
    mResetHardEvent[emctr_emustId].notify(SC_ZERO_TIME);
}//}}}

// Description: Monitor PCLK clock
// Ref: {HWCORE_ADTRG_UD_Ports_003}
void HWCORE_ADTRG::HandlePCLKSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

// Description: Monitor CCLK clock
// Ref: {HWCORE_ADTRG_UD_Ports_001}
void HWCORE_ADTRG::HandleCCLKSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = CCLK.read();
    SetCLKFreq("CCLK", freq_value, "Hz");
}//}}}

// Description: Process reset function when reset port is active
// Ref: {HWCORE_ADTRG_UD_Ports_002, HWCORE_ADTRG_UD_Ports_004, HWCORE_ADTRG_UD_Ports_013, HWCORE_ADTRG_UD_Direction_043, HWCORE_ADTRG_UD_Direction_044, HWCORE_ADTRG_UD_Flow_001, HWCORE_ADTRG_UD_Flow_002}
void HWCORE_ADTRG::HandleResetHardMethod(const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info", "The reset port PRESETn is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
            m_abnormal_event.notify(SC_ZERO_TIME);
        } else if (reset_id == emcrst_nId) {
            re_printf("info", "The reset port crst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        } else {
            re_printf("info", "The reset port emu3ictr_emust is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        }
    }
    else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) {
            re_printf("info", "The reset port PRESETn is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        }
        else if (reset_id == emcrst_nId) {
            re_printf("info", "The reset port crst_n is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        } else {
            re_printf("info", "The reset port emu3ictr_emust is de-asserted.\n");
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
// Ref: {HWCORE_ADTRG_UD_ATT1_005, HWCORE_ADTRG_UD_Direction_041}
void HWCORE_ADTRG::HandleResetCmdMethod(const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    }
    else if (reset_id == emcrst_nId) {
        reset_name = "crst_n";
    } else {
        reset_name = "emu3ictr_emust";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info", "The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

// Description: Cancel AssertReset Command Method
// Ref: {HWCORE_ADTRG_UD_Direction_032, HWCORE_ADTRG_UD_Direction_040, HWCORE_ADTRG_UD_Direction_041}
void HWCORE_ADTRG::CancelResetCmdMethod(const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    }
    else if (reset_id == emcrst_nId){
        reset_name = "crst_n";
    } else {
        reset_name = "emu3ictr_emust";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info", "Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
}//}}}

// Description: Reset model and its sub-instance if PRESETn is activated
//Ref: {HWCORE_ADTRG_UD_Flow_001, HWCORE_ADTRG_UD_Flow_002, HWCORE_ADTRG_UD_Flow_032}
void HWCORE_ADTRG::EnableReset(const bool is_active, const unsigned int reset_id)
{//{{{
    if (is_active) {
        if (reset_id == emPRESETnId) {
            // Reset register
            mpHWCORE_ADTRGFunc->EnableReset(is_active);
            // Reset port which correspond to register
            m_output_port_data_value[emRDATA_EM3IAD0_PORT] = 0;
            m_output_port_data_value[emRDATA_EM3IAD1_PORT] = 0;
            m_output_port_data_value[emRDATA_EM3IAD2_PORT] = 0;
            m_output_port_data_value[emADTRG_CH0_DAT_PORT] = 0;
            m_output_port_data_value[emADTRG_CH1_DAT_PORT] = 0;
            m_output_port_data_value[emADTRG_CH2_DAT_PORT] = 0;
            m_output_port_data_value[emEM3IADCTR_ADEND_PORT] = 0;
            m_output_port_data_value[emEMU3_IPTRGMON_ADTRGNUM_PORT] = 0;
            
            m_update_output_data_event[emRDATA_EM3IAD0_PORT].notify(SC_ZERO_TIME);
            m_update_output_data_event[emRDATA_EM3IAD1_PORT].notify(SC_ZERO_TIME);
            m_update_output_data_event[emRDATA_EM3IAD2_PORT].notify(SC_ZERO_TIME);
            m_update_output_data_event[emADTRG_CH0_DAT_PORT].notify(SC_ZERO_TIME);
            m_update_output_data_event[emADTRG_CH1_DAT_PORT].notify(SC_ZERO_TIME);
            m_update_output_data_event[emADTRG_CH2_DAT_PORT].notify(SC_ZERO_TIME);
            m_update_output_data_event[emEM3IADCTR_ADEND_PORT].notify(SC_ZERO_TIME);
            m_update_output_data_event[emEMU3_IPTRGMON_ADTRGNUM_PORT].notify(SC_ZERO_TIME);
        } else if (reset_id == emcrst_nId){
            // Cancel events
            CancelEventsCLEAN();
            // Initialize variables, output port
            InitializeCLEAN(emRESET);
        } else { //CTR_EMUST reset both SSCG and CLEAN domain
            // Cancel events
            CancelEventsSSCG();
            // Initialize variables, output port
            InitializeSSCG(emRESET);
            // Reset register
            mpHWCORE_ADTRGFunc->EnableReset(is_active);
        }
    } else if ((reset_id == emPRESETnId) || (reset_id == emctr_emustId)) {
        mpHWCORE_ADTRGFunc->EnableReset(is_active);
    }
}//}}}

// Description: Function to check reset and zero clock
bool HWCORE_ADTRG::ModelinResetStateorZeroClock(std::string domain_name)
{//{{{
    bool reset = true;
    bool clock = true;
    reset = GetResetStatus(domain_name);
    if (domain_name == "SSCG") {
        clock = CheckClockPeriod("PCLK");
    } else {
        clock = CheckClockPeriod("CCLK");
    }
    return (reset || !clock);
}//}}}
//========================================================
//============ PYTHON IF               ===================
//========================================================
// Description: Set message level (fatal, error, warning, info)
void HWCORE_ADTRG::SetMessageLevel(const std::string msg_lv)
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
    mpHWCORE_ADTRGFunc->RegisterHandler(cmd);

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
    mpHWCORE_ADTRGFunc->RegisterHandler(cmd);
}//}}}

// Description: Enable/disable dumping message when users access registers
void HWCORE_ADTRG::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_ADTRGFunc->RegisterHandler(cmd);
}//}}}

// Description: Enable/disable info/warning/error message of register IF
// Ref: {HWCORE_ADTRG_UD_Direction_035, HWCORE_ADTRG_UD_Direction_046}
void HWCORE_ADTRG::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }else if(word_vector[0] == "false"){
            mEnableRegisterMessage = false;
        }else{
            re_printf("warning", "Invalid argument: HWCORE_ADTRG_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }else if(word_vector.size() == 0){
        std::string temp = "";
        if(mEnableRegisterMessage){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info", "HWCORE_ADTRG_EnableRegisterMessage %s\n", temp.c_str());
    }else{
        re_printf("warning", "Invalid argument: HWCORE_ADTRG_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

// Description: Enable/disable dumping message when users access registers
// Ref: {HWCORE_ADTRG_UD_Direction_035}
void HWCORE_ADTRG::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_ADTRG_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string status = "true";
        if (mDumpInterrupt == false)
        {
            status = "false";
        }
        re_printf("info", "HWCORE_ADTRG_DumpInterrupt %s\n", status.c_str());
    } else {
        re_printf("warning", "Invalid argument: HWCORE_ADTRG_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

// Description: Command to assert reset
// Ref: {HWCORE_ADTRG_UD_Direction_042, HWCORE_ADTRG_UD_Direction_037, HWCORE_ADTRG_UD_Direction_036}
void HWCORE_ADTRG::AssertReset(const std::string reset_name, const double start_time, const double period)
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
    } else if (reset_name == "crst_n") {
        if ((!mIsResetHardActive[emcrst_nId]) && (!mIsResetCmdReceive[emcrst_nId])) {
            mIsResetCmdReceive[emcrst_nId] = true;
            re_printf("info", "The model will be reset (crst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emcrst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[emcrst_nId] = period;
        } else {
            re_printf("warning", "The software reset of crst_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else if(reset_name == "emu3ictr_emust") {
        if ((!mIsResetHardActive[emctr_emustId]) && (!mIsResetCmdReceive[emctr_emustId])) {
            mIsResetCmdReceive[emctr_emustId] = true;
            re_printf("info", "The model will be reset (emu3ictr_emust) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emctr_emustId].notify(start_time, SC_NS);
            mResetCmdPeriod[emctr_emustId] = period;
        } else {
            re_printf("warning", "The software reset of emu3ictr_emust is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning", "The reset name (%s) is wrong. It should be PRESETn or crst_n or emu3ictr_emust", reset_name.c_str());
    }
}//}}}

// Description: Set clock value and clock unit
// Ref: {HWCORE_ADTRG_UD_Direction_034, HWCORE_ADTRG_UD_Flow_003}
void HWCORE_ADTRG::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
                CancelEventsSSCG();
                m_abnormal_event.notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "CCLK") {
            mCCLKOrgFreq = freq;
            ConvertClockFreq(mCCLKFreq, mCCLKFreqUnit, freq, unit);
            if (mCCLKFreq > 0) {
                mCCLKPeriod = (sc_dt::uint64)(((1 / (double)mCCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartCCLKTime = sc_time_stamp().to_double();
            }else{
                mCCLKPeriod = 0;
                // Prepare for next run
                CancelEventsCLEAN();
            }
        } else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

// Description: Get clock value
// Ref: {HWCORE_ADTRG_UD_Direction_039, HWCORE_ADTRG_UD_Direction_034, HWCORE_ADTRG_UD_Direction_045}
void HWCORE_ADTRG::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info", "PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "CCLK") {
            re_printf("info", "CCLK frequency is %0.0f %s\n", (double)mCCLKOrgFreq, mCCLKFreqUnit.c_str());
        } else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

// Description: Force value to register
// Ref: {HWCORE_ADTRG_UD_Direction_026}
void HWCORE_ADTRG::ForceRegister(const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_ADTRGFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Release forced value after forcing registers
// Ref: {HWCORE_ADTRG_UD_Direction_026}
void HWCORE_ADTRG::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHWCORE_ADTRGFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Write value to registers by software
// Ref: {HWCORE_ADTRG_UD_Direction_026}
void HWCORE_ADTRG::WriteRegister(const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_ADTRGFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Read value of register by software
// Ref: {HWCORE_ADTRG_UD_Direction_026}
void HWCORE_ADTRG::ReadRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpHWCORE_ADTRGFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: List all registers name
// Ref: {HWCORE_ADTRG_UD_Direction_012}
void HWCORE_ADTRG::ListRegister(void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHWCORE_ADTRGFunc->RegisterHandler(cmd);
}//}}}

// Description: Dump help message of all parameters or commands
// Ref: {HWCORE_ADTRG_UD_Direction_013, HWCORE_ADTRG_UD_Direction_014, HWCORE_ADTRG_UD_Direction_025}
void HWCORE_ADTRG::Help(const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_MessageLevel (\"HWCORE_ADTRG instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_DumpRegisterRW (\"HWCORE_ADTRG instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_EnableRegisterMessage (\"HWCORE_ADTRG instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_DumpInterrupt (\"HWCORE_ADTRG instance\", \"true/false\")                     Enable/disable dumping interrupt value (Default: false)");
        }else if (word_vector[0] == "commands"){
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_AssertReset (\"HWCORE_ADTRG instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the HWCORE_ADTRG model");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_SetCLKFreq (\"HWCORE_ADTRG instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to HWCORE_ADTRG model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_GetCLKFreq (\"HWCORE_ADTRG instance\", \"clock_name\")                        Get clock frequency value of HWCORE_ADTRG model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_ForceRegister (\"HWCORE_ADTRG instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_ReleaseRegister (\"HWCORE_ADTRG instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_WriteRegister (\"HWCORE_ADTRG instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_ReadRegister (\"HWCORE_ADTRG instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_SetAHBExtension (\"HWCORE_ADTRG instance\", \"reg_name\")                     Set SPID value");
            SC_REPORT_INFO(this->basename(), "HWCORE_ADTRG_ListRegister (\"HWCORE_ADTRG instance\")        Dump name of all registers");
        }else{
            re_printf("warning", "The name (%s) of HWCORE_ADTRG_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    }else{
        re_printf("warning", "The name (%s) of HWCORE_ADTRG_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

//========================================================
//============ Overwrite func of CRWInterface.h ==========
//========================================================
// Overwrite read APIs for register access
//Ref: {HWCORE_ADTRG_UD_Flow_005}
void HWCORE_ADTRG::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if(data_ptr == NULL){
        return;
    }
    mpHWCORE_ADTRGFunc->read(offsetAddress, data_ptr, size, debug);
}//}}}

// Overwrite write APIs for write access
//Ref: {HWCORE_ADTRG_UD_Flow_006}
void HWCORE_ADTRG::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if(data_ptr == NULL){
        return;
    }
    mpHWCORE_ADTRGFunc->write(offsetAddress, data_ptr, size, debug);
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
// Description: Cancel SSCG domain events
//Ref: {HWCORE_ADTRG_UD_Flow_001}
void HWCORE_ADTRG::CancelEventsSSCG(void)
{//{{{
    // Operation events are canceled
    m_emu3_iptrgmon_adtrgnum_event.cancel();
    for (unsigned short port_id = 0; port_id < emOUPUT_PORT_EDGE_NUM; port_id ++) {
        if (port_id != emADC_START_PORT) {
            m_update_output_edge_event[port_id].cancel();
        }
    }
    for (unsigned short port_id = 0; port_id < emOUPUT_PORT_DATA_NUM; port_id ++) {
        m_update_output_data_event[port_id].cancel();
    }
    m_adc_completion_event.cancel();
    m_adc_data_event.cancel();
    m_ang_latch_event.cancel();

}//}}}

// Description: Cancel CLEAN domain events
// Ref: {HWCORE_ADTRG_UD_Flow_002}
void HWCORE_ADTRG::CancelEventsCLEAN(void)
{//{{{
    // Operation events are canceled
    m_rd_trg_event.cancel_all();
    m_ad_start_trg_event.cancel_all();
    m_update_output_edge_event[emADC_START_PORT].cancel();
    m_ad_sampling_mou_trg_event_queue.cancel_all();
    m_ad_sampling_vall_trg_event_queue.cancel_all();
}//}}}

///*********************************
/// Function     : DumpInterruptMsg
/// Description  : Dump Interrupt Message
///*********************************
// Description: DumpInterruptMsg
// Ref: {HWCORE_ADTRG_UD_Direction_056}
void HWCORE_ADTRG::DumpInterruptMsg (const std::string intr_name, const bool value)
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
/// Function     : SetAHBExtension
/// Description  : Set SPID value 
///*********************************
// Description: SetAHBExtension
// Ref: {HWCORE_ADTRG_UD_Direction_070}
void HWCORE_ADTRG::SetAHBExtension(unsigned int extension)
{//{{{   
    mSPID = (extension >> 8) & 0x1F;
}//}}}

//========================================================
//============ Startup trigger source handler  ===========
//========================================================
// Description: Checking whether current trigger is skip or not
// Ref: {HWCORE_ADTRG_UD_Flow_041}
bool HWCORE_ADTRG::IsSkipTrigger()
{//{{{
    bool result = false;
    bool EMU3IPTRGCTR_TNEN = emu3iptrgctr.read() & 0x1;
    if (EMU3IPTRGCTR_TNEN != 0) {
        unsigned int adtrgnum =  emu3iptrgnum.read() & 0xFF;
        if (m_ad_trig_count == adtrgnum) {
            MonitorSkipStartupTriggerSource(true); // Restart counter
            result = false;
        } else {
            result = true;
            MonitorSkipStartupTriggerSource(false); // Restart counter
        }
    } else {
        result = false;
    }
    return result;
}//}}}

// Description: Monitor Skip Startup Trigger source
// Ref: {HWCORE_ADTRG_UD_Flow_044}
void HWCORE_ADTRG::MonitorSkipStartupTriggerSource(bool reset_counter)
{//{{{
    if (reset_counter == false) {
        m_ad_trig_count++;
    } else {
        m_ad_trig_count = 0;
    }
    m_emu3_iptrgmon_adtrgnum_event.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);

}//}}}

// Description: Handle Startup Trigger Counter port emu3_iptrgmon_adtrgnum
// Ref: {HWCORE_ADTRG_UD_Flow_020}
void HWCORE_ADTRG::HandleSkipStartupTriggerCounterMethod()
{//{{{
    m_output_port_data_value[emEMU3_IPTRGMON_ADTRGNUM_PORT] = m_ad_trig_count;
    m_update_output_data_event[emEMU3_IPTRGMON_ADTRGNUM_PORT].notify(SC_ZERO_TIME);
}//}}}

// Description: Handle A/D Select Signal Method
// Ref: {HWCORE_ADTRG_UD_Flow_030}
void HWCORE_ADTRG::HandleADSelectSignalMethod()
{//{{{
    if( em3iadsel.read() != m_em3iadsel_pre_val) {
        m_em3iadsel_pre_val = em3iadsel.read();
        //Clear register, AD conversion result
        HandleADConversionResult(0x0, 0x0, 0x0);
    }
 }//}}}

// Description: Check model in time of trigger source delay or not
// Ref: {HWCORE_ADTRG_UD_Flow_012}
bool HWCORE_ADTRG::IsDelayPeriod(void)
{//{{{
    return m_adtrg_in_delay_period;
}//}}}

// Description: Handle Carrier Peak Method
// Ref: {HWCORE_ADTRG_UD_Flow_012, HWCORE_ADTRG_UD_Direction_053}
void HWCORE_ADTRG::HandleCarrierPeakMethod()
{//{{{
    if (ModelinResetStateorZeroClock("CLEAN") == true) {
        return;
    }
    else {
        if (IsDelayPeriod() != false ) {
            re_printf("warning", "Trigger A/D Converter startup during delay time of previous trigger is invalid");
            return;
        }
        unsigned int ADTRG_CMPMSL = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRG") >> 4) & 0x1;

        if ((ADTRG_CMPMSL != 0) && (tstadt0.read() != 0)) {
            m_output_port_edge_value[emAD_START_MOU_SP_80M_PORT] = true;
            m_adtrg_att = 1 << emTSTADT0_ATT;
        } else if ((ADTRG_CMPMSL == 0) && (tstpektim.read() != 0)) {
            m_output_port_edge_value[emAD_START_MOU_SP_80M_PORT] = true;
            m_adtrg_att = 1 << emTSTPEKTIM_ATT;
        } else { // Masking source trigger
            return;
        }

        m_update_output_edge_event[emAD_START_MOU_SP_80M_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
        unsigned int ADTRG_CAMOUAD = mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRG") & 0x1;
        if ((ADTRG_CAMOUAD != 0) && (IsSkipTrigger() == false)) { // A/D conversion start trigger source enable (carrier signal peak timing)
            unsigned short ADMON = mpHWCORE_ADTRGFunc->GetRegisterVal("ADMON");
            mpHWCORE_ADTRGFunc->SetRegisterVal("ADMON", (ADMON | 1 << 0));

            //A/D sampling start trigger generation
            unsigned int ADTRGM_M0 = mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRGM") & 0x1;
            unsigned int SMCTR_SMMEN = mpHWCORE_ADTRGFunc->GetRegisterVal("SMCTR")& 0x1;
            if (SMCTR_SMMEN != 0) {
                unsigned int ADSMTRG = mpHWCORE_ADTRGFunc->GetRegisterVal("ADSMTRG");
                unsigned int SMOFS = mpHWCORE_ADTRGFunc->GetRegisterVal("ADSMOFS") & 0x3FFFF;
                unsigned int SMNUM  =  (ADSMTRG >> 24) & 0x000F;
                unsigned int SMCNT = ADSMTRG & 0x3FFFF;
                double samp_time = 0;
                for (uint8_t i = 0; i <= SMNUM; i++) {
                    if(i == 0) {
                        m_ad_sampling_mou_trg_event_queue.notify(0, mTimeResolutionUnit);
                        m_ad_samp_mou_count = 0;
                        m_ang_samp_mou_lat_count = 0;
                        continue;
                    }
                    if (i == 1) {
                        samp_time += (double)(SMOFS*mCCLKPeriod);
                    }
                    samp_time += (double)(SMCNT*mCCLKPeriod);
                    m_ad_sampling_mou_trg_event_queue.notify(samp_time, mTimeResolutionUnit); // Trigger ADC every time sampling trigger or not
                }
            }
            else{ // SMCTR_SMMEN = 0
                GetDelayTimer(false);
                m_ad_start_trg_event.notify(m_ad_start_trg_delay, mTimeResolutionUnit);
                m_rd_trg_event.notify(m_ang_start_trg_delay, mTimeResolutionUnit);
                m_adtrg_in_delay_period = true;
            }
        }
    }
}//}}}

// Description: Handle Carrier Trough Method
// Ref: {HWCORE_ADTRG_UD_Flow_013}
void HWCORE_ADTRG::HandleCarrierTroughMethod()
{//{{{
    if (ModelinResetStateorZeroClock("CLEAN") == true) {
        return;
    } else {
        if (IsDelayPeriod() != false ) {
            re_printf("warning", "Trigger A/D Converter startup during delay time of previous trigger is invalid");
            return;
    }

    unsigned int ADTRG_CMPVSL = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRG") >> 5) & 0x1;

    if ((ADTRG_CMPVSL != 0) && (tstadt1.read() != 0)) {
        m_output_port_edge_value[emAD_START_VALL_SP_80M_PORT] = true;
        m_adtrg_att = 1 << emTSTADT1_ATT;
    } else if ((ADTRG_CMPVSL == 0) && (tstvlytim.read() != 0)) {
        m_output_port_edge_value[emAD_START_VALL_SP_80M_PORT] = true;
        m_adtrg_att = 1 << emTSTVLYTIM_ATT;
    } else { // Masking source trigger
        return;
    }

        m_update_output_edge_event[emAD_START_VALL_SP_80M_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);

        unsigned int ADTRG_CAVALAD = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRG") >> 1) & 0x1;
        if ((ADTRG_CAVALAD != 0) && (IsSkipTrigger() == false)) { // A/D conversion start trigger source enable (carrier signal peak timing)
            unsigned short ADMON = mpHWCORE_ADTRGFunc->GetRegisterVal("ADMON");
            mpHWCORE_ADTRGFunc->SetRegisterVal("ADMON", (ADMON | 1 << 1));

            //A/D sampling start trigger generation
            unsigned int ADTRGM_M0 = mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRGM") & 0x1;
            unsigned int SMCTR_SMVEN = mpHWCORE_ADTRGFunc->GetRegisterVal("SMCTR")& 0x2;
            if (SMCTR_SMVEN != 0) {
                unsigned int ADSMTRG = mpHWCORE_ADTRGFunc->GetRegisterVal("ADSMTRG");
                unsigned int SMOFS = mpHWCORE_ADTRGFunc->GetRegisterVal("ADSMOFS") & 0x3FFFF;
                unsigned char SMNUM  =  (ADSMTRG >> 24) & 0xF;
                unsigned int SMCNT = ADSMTRG & 0x3FFFF;
                double samp_time = 0;
                for (uint8_t i = 0; i <= SMNUM; i++) {
                    if(i == 0) {
                        m_ad_sampling_vall_trg_event_queue.notify(0, mTimeResolutionUnit);
                        m_ad_samp_vall_count = 0;
                        m_ang_samp_vall_lat_count = 0;
                        continue;
                    }
                    if (i == 1) {
                        samp_time += (double)(SMOFS*mCCLKPeriod);
                    }
                    samp_time += (double)(SMCNT*mCCLKPeriod);
                    m_ad_sampling_vall_trg_event_queue.notify(samp_time, mTimeResolutionUnit); // Trigger ADC every time sampling trigger or not
                }
            }
            else{ // SMCTR_SMMEN = 0
                GetDelayTimer(false);
                m_ad_start_trg_event.notify(m_ad_start_trg_delay, mTimeResolutionUnit);
                m_rd_trg_event.notify(m_ang_start_trg_delay, mTimeResolutionUnit);
               m_adtrg_in_delay_period = true;
            }
        }
    }
}//}}}

// Description: Handle AD Sampling Peak Method
// Ref: {HWCORE_ADTRG_UD_Flow_042}
void HWCORE_ADTRG::HandleADSamplingPeakMethod()
{//{{{
    m_adtrg_att |= 1 << emAD_SAMP_PEAK;
    GetDelayTimer(false);
    m_ad_start_trg_event.notify(m_ad_start_trg_delay, mTimeResolutionUnit);
    m_rd_trg_event.notify(m_ang_start_trg_delay, mTimeResolutionUnit);
    m_adtrg_in_delay_period = true;
}//}}}

// Description: Handle AD Sampling Trough Method
// Ref: {HWCORE_ADTRG_UD_Flow_043}
void HWCORE_ADTRG::HandleADSamplingTroughMethod()
{//{{{
     m_adtrg_att |= 1 << emAD_SAMP_TROUGH;
     GetDelayTimer(false);
     m_ad_start_trg_event.notify(m_ad_start_trg_delay, mTimeResolutionUnit);
     m_rd_trg_event.notify(m_ang_start_trg_delay, mTimeResolutionUnit);
     m_adtrg_in_delay_period = true;
}//}}}

// Description: Handle Output Edge detect Port
// Ref: {HWCORE_ADTRG_UD_Flow_010}
void HWCORE_ADTRG::HandleOutputEdgePortMethod(const unsigned int port_id)
{//{{{

    switch(port_id) {
        case emADC_START_PORT             :
            if (m_output_port_edge_value[port_id] != adc_start.read()){
                adc_start.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("adc_start", m_output_port_edge_value[port_id]);
            }
            break;

        case emADTRG_END_TRIG_PORT        :
            if (m_output_port_edge_value[port_id] != adtrg_end_trig.read()){
                adtrg_end_trig.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("adtrg_end_trig", m_output_port_edge_value[port_id]);
            }
            break;

        case emAD_END_TRIG_SP_PORT:
            if (m_output_port_edge_value[port_id] != ad_end_trig_sp.read()){
                ad_end_trig_sp.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("ad_end_trig_sp", m_output_port_edge_value[port_id]);
            }
            break;

        case emADTRG_MVINT0_PORT          :
            if (m_output_port_edge_value[port_id] != adtrg_mvint0.read()){
                adtrg_mvint0.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("adtrg_mvint0", m_output_port_edge_value[port_id]);
                //reset skipping counter
                m_adtrg0_skip_count = 0;
            }
            break;

        case emADTRG_MVINT1_PORT          :
            if (m_output_port_edge_value[port_id] != adtrg_mvint1.read()){
                adtrg_mvint1.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("adtrg_mvint1", m_output_port_edge_value[port_id]);
                //reset skipping counter
                m_adtrg1_skip_count = 0;
            }
            break;

        case emADTRG_MEM_RG_CHG_PORT      :
            if (m_output_port_edge_value[port_id] != adtrg_mem_rg_chg.read()){
                adtrg_mem_rg_chg.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("adtrg_mem_rg_chg", m_output_port_edge_value[port_id]);
            }
            break;

        case emADTRG_SMERR_PORT           :
            if (m_output_port_edge_value[port_id] != adtrg_smerr.read()){
                adtrg_smerr.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("adtrg_smerr", m_output_port_edge_value[port_id]);
            }
            break;

        case emADTRG_AMERR_PORT           :
            if (m_output_port_edge_value[port_id] != adtrg_amerr.read()){
                adtrg_amerr.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("adtrg_amerr", m_output_port_edge_value[port_id]);
            }
            break;

        case emAD_START_MOU_SP_80M_PORT   :
            if (m_output_port_edge_value[port_id] != ad_start_mou_sp_80m.read()){
                ad_start_mou_sp_80m.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("ad_start_mou_sp_80m", m_output_port_edge_value[port_id]);
            }
            break;

        default                           :
            if (m_output_port_edge_value[port_id] != ad_start_vall_sp_80m.read()){
                ad_start_vall_sp_80m.write(m_output_port_edge_value[port_id]);
                DumpInterruptMsg("ad_start_vall_sp_80m", m_output_port_edge_value[port_id]);
            }
            break;
    }

    if (m_output_port_edge_value[port_id] == true) {
        m_output_port_edge_value[port_id] = false;
        if (port_id == emADC_START_PORT) { //sync with CCLK clock
            m_adc_status = emAD_BUSY;
            m_update_output_edge_event[port_id].notify((double)mCCLKPeriod, mTimeResolutionUnit);
        } else { //sync with PCLK clock
            m_update_output_edge_event[port_id].notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

// Description: Handle Output Data detect Port
// Ref: {HWCORE_ADTRG_UD_Flow_010}
void HWCORE_ADTRG::HandleOutputDataPortMethod(const unsigned int port_id)
{//{{{

    switch(port_id) {
        case emEM3IADCTR_ADEND_PORT   :
            em3iadctr_adend.write((unsigned char)m_output_port_data_value[port_id]);
            break;

        case emEMU3_IPTRGMON_ADTRGNUM_PORT   :
            emu3_iptrgmon_adtrgnum.write((unsigned char)m_output_port_data_value[port_id]);
            break;

        case emRDATA_EM3IAD0_PORT   :
            rdata_em3iad0.write(m_output_port_data_value[port_id]);
            break;

        case emRDATA_EM3IAD1_PORT   :
            rdata_em3iad1.write(m_output_port_data_value[port_id]);
            break;

        case emRDATA_EM3IAD2_PORT   :
            rdata_em3iad2.write(m_output_port_data_value[port_id]);
            break;

        case emADTRG_CH0_DAT_PORT   :
            adtrg_ch0_dat.write(m_output_port_data_value[port_id]);
            break;

        case emADTRG_CH1_DAT_PORT   :
            adtrg_ch1_dat.write(m_output_port_data_value[port_id]);
            break;

        case emADTRG_CH2_DAT_PORT   :
            adtrg_ch2_dat.write(m_output_port_data_value[port_id]);
            break;

        case emADTRG_THTE_PORT   :
            adtrg_thte.write(m_output_port_data_value[port_id]);
            break;

        default   :
            adtrg_thtr.write(m_output_port_data_value[port_id]);
            break;
    }
    
}//}}}

// Description: Handle Angle value latching register
// Ref: {HWCORE_ADTRG_UD_Flow_011, HWCORE_ADTRG_UD_Direction_055}
void HWCORE_ADTRG::HandleAngLatchingRegisterMethod()
{//{{{

    re_printf("info","[%s] is processing...\n","Angle value latch trigger");
    if (ModelinResetStateorZeroClock("SSCG") == true) {
        return;
    }
    unsigned short thtr = theta_res_fix_reg.read() & 0xFFF;
    unsigned short thte = theta_e_fix_reg.read() & 0xFFF;

    if(((m_adtrg_att >> emAD_SAMP_TROUGH) & 0x1) == 0x1) {
        m_ang_samp_vall_lat_count ++;
    }

    if((m_adtrg_att >> emAD_SAMP_PEAK   & 0x1) == 0x1) {
        m_ang_samp_mou_lat_count ++;
    }

    //AD start trigger mask control
    if(ADTriggerMaskControl(emANG_START_TRG)) return;

    // Update register
    mpHWCORE_ADTRGFunc->SetRegisterVal("THTR", thtr);
    mpHWCORE_ADTRGFunc->SetRegisterVal("THTE", thte);
    // Update port
    m_ang_latch_event.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);

}//}}}

// Description: Handle angle latching port
// Ref: {HWCORE_ADTRG_UD_Flow_040}
void HWCORE_ADTRG:: HandleAngLatchingPortMethod()
{//{{{
    unsigned short thtr = mpHWCORE_ADTRGFunc->GetRegisterVal("THTR") & 0xFFF;
    unsigned short thte = mpHWCORE_ADTRGFunc->GetRegisterVal("THTE") & 0xFFF;
    // Update port
    m_output_port_data_value[emADTRG_THTE_PORT] = thte;
    m_output_port_data_value[emADTRG_THTR_PORT] = thtr;
    m_update_output_data_event[emADTRG_THTE_PORT].notify(SC_ZERO_TIME);
    m_update_output_data_event[emADTRG_THTR_PORT].notify(SC_ZERO_TIME);

}//}}}

// Description: Handle AD start trigger by angle compare match 0
// Ref: {HWCORE_ADTRG_UD_Flow_014}
void HWCORE_ADTRG::HandleAngComp0Method()
{//{{{
    if (ModelinResetStateorZeroClock("CLEAN") == true) {
        return;
    } else {
        unsigned int ADTRG_CMPADM = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRG") >> 2) & 0x1;
        unsigned int ADMON = mpHWCORE_ADTRGFunc->GetRegisterVal("ADMON");
        if (IsDelayPeriod() != false ) {
            re_printf("warning", "Trigger A/D Converter startup during delay time of previous trigger is invalid");
            return;
        }
        if (ADTRG_CMPADM != 0) {
            mpHWCORE_ADTRGFunc->SetRegisterVal("ADMON", (ADMON | (1 << 2)));
            m_adtrg_att = 1 << emANG_COMP0;
            GetDelayTimer(false);
            m_ad_start_trg_event.notify(m_ad_start_trg_delay, mTimeResolutionUnit);
            m_rd_trg_event.notify(m_ang_start_trg_delay, mTimeResolutionUnit);
            m_adtrg_in_delay_period = true;
        }
    }
}//}}}

// Description: Handle AD start trigger by IR3 compare macth U phase
// Ref: {HWCORE_ADTRG_UD_Flow_015}
void HWCORE_ADTRG::HandleIR3UPhaseMethod()
{//{{{
    if (ModelinResetStateorZeroClock("CLEAN") == true) {
        return;
    } else {
        unsigned int ADTRG_IR3UADM = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRG") >> 8) & 0x1;
        unsigned int ADMON = mpHWCORE_ADTRGFunc->GetRegisterVal("ADMON");
        if (IsDelayPeriod() != false ) {
            re_printf("warning", "Trigger A/D Converter startup during delay time of previous trigger is invalid");
            return;
        }
        if (ADTRG_IR3UADM != 0) {
            mpHWCORE_ADTRGFunc->SetRegisterVal("ADMON", (ADMON | (1 << 8)));
            m_adtrg_att = 1 << emIR3U;
            GetDelayTimer(false);
            m_ad_start_trg_event.notify(m_ad_start_trg_delay, mTimeResolutionUnit);
            m_rd_trg_event.notify(m_ang_start_trg_delay, mTimeResolutionUnit);
            m_adtrg_in_delay_period = true;
        }
    }
}//}}}

// Description: Handle AD start trigger by IR3 compare macth V phase
// Ref: {HWCORE_ADTRG_UD_Flow_015}
void HWCORE_ADTRG::HandleIR3VPhaseMethod()
{//{{{
    if (ModelinResetStateorZeroClock("CLEAN") == true) {
        return;
    } else {
        unsigned int ADTRG_IR3VADM = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRG") >> 9) & 0x1;
        unsigned int ADMON = mpHWCORE_ADTRGFunc->GetRegisterVal("ADMON");
        if (IsDelayPeriod() != false ) {
            re_printf("warning", "Trigger A/D Converter startup during delay time of previous trigger is invalid");
            return;
        }
        if (ADTRG_IR3VADM != 0) {
            mpHWCORE_ADTRGFunc->SetRegisterVal("ADMON", (ADMON | (1 << 9)));
            m_adtrg_att = 1 << emIR3V;
            GetDelayTimer(false);
            m_ad_start_trg_event.notify(m_ad_start_trg_delay, mTimeResolutionUnit);
            m_rd_trg_event.notify(m_ang_start_trg_delay, mTimeResolutionUnit);
            m_adtrg_in_delay_period = true;
        }
    }
}//}}}

// Description: Handle AD start trigger by IR3 compare macth W phase
// Ref: {HWCORE_ADTRG_UD_Flow_015}
void HWCORE_ADTRG::HandleIR3WPhaseMethod()
{//{{{
    if (ModelinResetStateorZeroClock("CLEAN") == true) {
        return;
    } else {
        unsigned int ADTRG_IR3WADM = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRG") >> 10) & 0x1;
        unsigned int ADMON = mpHWCORE_ADTRGFunc->GetRegisterVal("ADMON");
        if (IsDelayPeriod() != false ) {
            re_printf("warning", "Trigger A/D Converter startup during delay time of previous trigger is invalid");
            return;
        }
        if (ADTRG_IR3WADM != 0) {
            mpHWCORE_ADTRGFunc->SetRegisterVal("ADMON", (ADMON | (1 << 10)));
            m_adtrg_att = 1 << emIR3W;
            GetDelayTimer(false);
            m_ad_start_trg_event.notify(m_ad_start_trg_delay, mTimeResolutionUnit);
            m_rd_trg_event.notify(m_ang_start_trg_delay, mTimeResolutionUnit);
            m_adtrg_in_delay_period = true;
        }
    }
}//}}}

// Description: Handle Completion Signal from ADC
// Ref: {HWCORE_ADTRG_UD_Flow_022}
void HWCORE_ADTRG::HandleCompletionSignalfromADCMethod()
{//{{{
    if (ModelinResetStateorZeroClock("CLEAN") == true || ModelinResetStateorZeroClock("SSCG") == true) {
        return;
    }
    unsigned char INSTCTR = (em2ictrinmd_instctr.read() & 0x3);
    bool ADRM = mpHWCORE_ADTRGFunc->GetRegisterVal("ADCTR") & 0x1;

    if (ADRM == 0) {
        //Update register
        switch (INSTCTR) {
        case 0x0:
            if(adc_grpend.read() != 0){
                //Issue event for A/D completion
                m_output_port_edge_value[emAD_END_TRIG_SP_PORT] = true;
                m_update_output_edge_event[emAD_END_TRIG_SP_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
                // Handle AD completion
                m_adc_completion_event.notify();
            }
            break;

        case 0x1:
            if(adc_ch0end.read() != 0){
                //Issue event for A/D completion
                m_output_port_edge_value[emAD_END_TRIG_SP_PORT] = true;
                m_update_output_edge_event[emAD_END_TRIG_SP_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
                // Handle AD completion
                m_adc_completion_event.notify();
            }
            break;
        case 0x2:
            if(adc_ch1end.read() != 0){
                //Issue event for A/D completion
                m_output_port_edge_value[emAD_END_TRIG_SP_PORT] = true;
                m_update_output_edge_event[emAD_END_TRIG_SP_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
                // Handle AD completion
                m_adc_completion_event.notify();
            }
            break;
        default:
            if(adc_ch2end.read() != 0){
                //Issue event for A/D completion
                m_output_port_edge_value[emAD_END_TRIG_SP_PORT] = true;
                m_update_output_edge_event[emAD_END_TRIG_SP_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
                // Handle AD completion
                m_adc_completion_event.notify();
            }
        }
    }

}//}}}

// Description: Handle Completion Signal from TSG3
// Ref: {HWCORE_ADTRG_UD_Flow_021}
void HWCORE_ADTRG::HandleCompletionSignalfromTSG3Method()
{//{{{
    if (ModelinResetStateorZeroClock("CLEAN") == true || ModelinResetStateorZeroClock("SSCG") == true) {
        return;
    }
    unsigned char ADCTR = mpHWCORE_ADTRGFunc->GetRegisterVal("ADCTR") ;
    if ((ADCTR & 0x1) == 0x1) {

        if (((((ADCTR >> 0x1) & 0x1) == 0x1) && (tstint3.read() != false)) || ((((ADCTR >> 0x2) & 0x1) == 0x1) && (tstint4.read() != false))) {
            //Issue event for A/D completion
            m_output_port_edge_value[emAD_END_TRIG_SP_PORT] = true;
            m_update_output_edge_event[emAD_END_TRIG_SP_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
            // Handle AD completion
            m_adc_completion_event.notify(); //TODO do not update ADk
        }
    }
}//}}}

// Description: Distribution AD Completion function
// Ref: {HWCORE_ADTRG_UD_Flow_045}
void HWCORE_ADTRG::DistributionADCompletion(void)
{//{{{
    unsigned short TRGGCM = mpHWCORE_ADTRGFunc->GetRegisterVal("TRGGCM");

    if((((m_adtrg_att & (1 << emTSTPEKTIM_ATT)) >> emTSTPEKTIM_ATT) == 0x1) || ((m_adtrg_att >> emTSTADT0_ATT) == 0x1)) {
        m_ADTRG0 = true;
    } else {
        m_ADTRG0 = false;
    }
    if ((((m_adtrg_att & (1 << emTSTVLYTIM_ATT))>> emTSTVLYTIM_ATT) == 0x1) || ((m_adtrg_att >> emTSTADT1_ATT) == 0x1)) {
        m_ADTRG1 = true;
    } else {
        m_ADTRG1 = false;
    }
    if (((TRGGCM >> 8) & 0x1) == 0x1) { //Both m_ADTRG0 and m_ADTRG1 triggers which were generated through the A/D conversion completion trigger division circuit.
        m_ADTRG0 |= m_ADTRG1;
    }

}//}}}

// Description: Handle AD Completion Port to HWCORE_INT Method
// Ref: {HWCORE_ADTRG_UD_Flow_046, HWCORE_ADTRG_UD_Ports_049, HWCORE_ADTRG_UD_Ports_050, HWCORE_ADTRG_UD_Ports_051}
void HWCORE_ADTRG::HandleADConversionResult(unsigned short AD0, unsigned short AD1, unsigned short AD2)
{//{{{
    //Update register if completion trigger come from A/D Converter

    mpHWCORE_ADTRGFunc->SetRegisterVal("AD0", AD0);
    mpHWCORE_ADTRGFunc->SetRegisterVal("AD1", AD1);
    mpHWCORE_ADTRGFunc->SetRegisterVal("AD2", AD2);

    // Update output port to HWCORE_IIR
    m_output_port_data_value[emRDATA_EM3IAD0_PORT] = AD0;
    m_output_port_data_value[emRDATA_EM3IAD1_PORT] = AD1;
    m_output_port_data_value[emRDATA_EM3IAD2_PORT] = AD2;
    m_update_output_data_event[emRDATA_EM3IAD0_PORT].notify(SC_ZERO_TIME);
    m_update_output_data_event[emRDATA_EM3IAD1_PORT].notify(SC_ZERO_TIME);
    m_update_output_data_event[emRDATA_EM3IAD2_PORT].notify(SC_ZERO_TIME);
    
    // Update output port to HWCORE_INPUT
    unsigned char adbufsel = mpHWCORE_ADTRGFunc->GetRegisterVal("ADBUFSEL");

    if ((adbufsel & 0x3) == 0x0) {
        m_output_port_data_value[emADTRG_CH0_DAT_PORT] = AD0;
        m_update_output_data_event[emADTRG_CH0_DAT_PORT].notify(SC_ZERO_TIME);
    }
    if (((adbufsel >> 2) & 0x3) == 0x0) {
        m_output_port_data_value[emADTRG_CH1_DAT_PORT] = AD1;
        m_update_output_data_event[emADTRG_CH1_DAT_PORT].notify(SC_ZERO_TIME);
    }
    if (((adbufsel >> 4) & 0x3) == 0x0) {
        m_output_port_data_value[emADTRG_CH2_DAT_PORT] = AD2;
        m_update_output_data_event[emADTRG_CH2_DAT_PORT].notify(SC_ZERO_TIME);
    }

}//}}}

// Description: Skip AD Completion Trigger function
// Ref: {HWCORE_ADTRG_UD_Flow_047}
void HWCORE_ADTRG::SkipADCompletionTrigger(void)
{//{{{
    unsigned short TRGGCM = mpHWCORE_ADTRGFunc->GetRegisterVal("TRGGCM");
    bool GCMEN = mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRGCTR") & 0x1;
    unsigned char TRGGCM_TRGSEL = (TRGGCM >> 12) & 0x3;
    unsigned short ADEDM = mpHWCORE_ADTRGFunc->GetRegisterVal("ADEDM");
    double time = CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime);

    if(TRGGCM_TRGSEL == 0x2){
        m_output_port_edge_value[emADTRG_END_TRIG_PORT] = (m_ADTRG0 | m_ADTRG1);
        m_update_output_edge_event[emADTRG_END_TRIG_PORT].notify(time, mTimeResolutionUnit);
    }

    if (GCMEN == true){
        if(m_ADTRG0 == true){
            if(m_adtrg0_skip_count != (TRGGCM & 0xF)){
                m_ADTRG0 = false;
            }
            m_adtrg0_skip_count ++;
        }
        if(m_ADTRG1 == true){
            if(m_adtrg1_skip_count != (TRGGCM >> 4 & 0xF)){
                m_ADTRG1 = false;
            }
            m_adtrg1_skip_count ++;
        }
    }

    m_output_port_edge_value[emADTRG_MVINT0_PORT] = m_ADTRG0;
    m_output_port_edge_value[emADTRG_MVINT1_PORT] = m_ADTRG1;
    m_update_output_edge_event[emADTRG_MVINT0_PORT].notify(time, mTimeResolutionUnit);
    m_update_output_edge_event[emADTRG_MVINT1_PORT].notify(time, mTimeResolutionUnit);

    switch (TRGGCM_TRGSEL) {
        case 0x0: // m_ADTRG0 after skipping
            // Update adtrg_end_trig port
            m_output_port_edge_value[emADTRG_END_TRIG_PORT] = m_ADTRG0;
            // Issue event
            m_update_output_edge_event[emADTRG_END_TRIG_PORT].notify(time, mTimeResolutionUnit);
            break;
        case 0x1:// m_ADTRG1 after skipping
            // Update adtrg_end_trig port
            m_output_port_edge_value[emADTRG_END_TRIG_PORT] = m_ADTRG1;
            // Issue event
            m_update_output_edge_event[emADTRG_END_TRIG_PORT].notify(time, mTimeResolutionUnit);
            break;
        case 0x3: // An A/D conversion completion trigger which is enabled after masking specified by the EMU3nADEDM register is applied.
            m_output_port_edge_value[emADTRG_END_TRIG_PORT] = true;
            m_update_output_edge_event[emADTRG_END_TRIG_PORT].notify(time, mTimeResolutionUnit);
            break;
        default:
            break;
    }

}//}}}

// Description: Handle AD Completion Port to HWCORE_INT Method
// Ref: {HWCORE_ADTRG_UD_Flow_048, HWCORE_ADTRG_UD_Direction_054}
void HWCORE_ADTRG::HandleADCompletionMethod()
{//{{{
    // Get AD conversion result
    unsigned short AD0 = (adc_ch0_dat.read() & 0xFFF);
    unsigned short AD1 = (adc_ch1_dat.read() & 0xFFF);
    unsigned short AD2 = (adc_ch2_dat.read() & 0xFFF);
    // Write memory setting
    unsigned char ADMWCTR = mpHWCORE_ADTRGFunc->GetRegisterVal("ADMWCTR");

    bool isEnableWM = false;
    bool MTRGSEL = false;
    if((ADMWCTR & 0x1) == 0x1){
        isEnableWM = true;
    }
    else{
        isEnableWM = false;
    }

    if(((ADMWCTR >> 1) & 0x1) == 0x1){
        MTRGSEL = true;
    }
    else{
        MTRGSEL = false;
    }

    // Update ADC status
    m_adc_status = emAD_IDLE;
    // Update register, IIR port, InputIP port
    HandleADConversionResult(AD0, AD1, AD2);

    if(!isEnableWM)
    {
        SC_REPORT_INFO(this->basename(), "Disable writing to memory");
    }

    // Write to memory
    if((isEnableWM & (!MTRGSEL)) == true){
        WriteADDatatoMemory();
    }

    //A/D completion trigger mask control
    if(ADTriggerMaskControl(emAD_COMP_TRIG)) return;

    //Sinal is valid to trigger these functions below

    //Write data to memmory interface
    if((isEnableWM & MTRGSEL) == true){
        WriteADDatatoMemory();
    }
    // Distribution of the A/D conversion completion triggers function
    DistributionADCompletion();

    // Final filter for output adtrg_end_trig
    // Selects the A/D completion trigger to be output to the input IP or the IIR filter
    SkipADCompletionTrigger();

}//}}}

// Description: Write AD Data to Memory
// Ref: {HWCORE_ADTRG_UD_Flow_049}
void HWCORE_ADTRG::WriteADDatatoMemory()
{//{{{
    //Prepare address
    unsigned int address =  emu3tbladr.read();
    unsigned short ADMNMON = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADMNMON"));
    unsigned int ADMOFS2 = mpHWCORE_ADTRGFunc->GetRegisterVal("ADMOFS2") & 0xFFFC; // must multiple of 4
    unsigned short ADMNCNT = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADMNCNT") & 0xFFF);
    unsigned short ADMNMON_MNNUM = ADMNMON & 0xFFF;

    // Overlap Memory writing.
    if (m_adtrg_mem_write_time > 0) {
        m_output_port_edge_value[emADTRG_SMERR_PORT] = true;
        m_update_output_edge_event[emADTRG_SMERR_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
        return;
    }

    //Check switching memory writing
    if (ADMNCNT == ADMNMON_MNNUM) {
        m_switching_area = !m_switching_area;

        // Generate interrupt to INT
        m_output_port_edge_value[emADTRG_MEM_RG_CHG_PORT] = true;
        m_update_output_edge_event[emADTRG_MEM_RG_CHG_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);

       //Update address base and moniter register
       ADMNMON_MNNUM = 0;
       mpHWCORE_ADTRGFunc->SetRegisterVal("ADMNMON", "MNNUM", 0x0);
       if (m_switching_area) {
            mpHWCORE_ADTRGFunc->SetRegisterVal("ADMNMON", "MNSEL", 0x1);
        }
        else{
            mpHWCORE_ADTRGFunc->SetRegisterVal("ADMNMON", "MNSEL", 0x0);
        }

    }
    if(m_switching_area){
        address += ADMOFS2;
    }
    address += ADMNMON_MNNUM *8;

    //Prepare data
    unsigned char ADMWCTR = mpHWCORE_ADTRGFunc->GetRegisterVal("ADMWCTR");
    unsigned int data0 = ((mpHWCORE_ADTRGFunc->GetRegisterVal("AD1") << 16) | mpHWCORE_ADTRGFunc->GetRegisterVal("AD0"));
    unsigned int data1 = ((mpHWCORE_ADTRGFunc->GetRegisterVal("THTE") << 16) | mpHWCORE_ADTRGFunc->GetRegisterVal("AD2"));
    if (((ADMWCTR) >> 2 & 0x1) == 0x0){
        data1 = ((mpHWCORE_ADTRGFunc->GetRegisterVal("THTR") << 16) | mpHWCORE_ADTRGFunc->GetRegisterVal("AD2"));
    }
    mpData[0] = data0;
    mpData[1] = data1;
    
    // AHBExtension
    unsigned char MHPROT = ahbprt.read();
    mAHBTransExt->setDataOrOp((TlmAhbExtension::AhbDataOrOp_t)(MHPROT & 0x1));
    mAHBTransExt->setPrivilege((MHPROT >> 1) & 0x1);
    mAHBTransExt->setBufferable((MHPROT >> 2) & 0x1);
    mAHBTransExt->setCachable((MHPROT >> 3) & 0x1);
    
    m_mnnum = ADMNMON_MNNUM;
    // Get information for payload
    m_pAddr = address;
    m_issue_write.notify(SC_ZERO_TIME);
}//}}}

// Description: Issue trans to slave
// Ref: {HWCORE_ADTRG_UD_Flow_050}
void HWCORE_ADTRG::ProcessWriteMemoryThread()
{//{{{
    unsigned char pData0[4] = { 0 };
    unsigned int mAddress = 0;
    BusTime_t ahb_tran_time = SC_ZERO_TIME;
    unsigned int temp = 0;

    while(1){
        /* Cancel previous abnormal event */
        m_abnormal_event.cancel();
        /* Wait issue write event*/
        wait();
        mAddress = m_pAddr;
        for(int i = 0; i <= 1; i++){
            temp = mpData[i];
            memcpy(&pData0[0], &temp, 4);
            mAHBTrans->set_address((sc_dt::uint64)mAddress);
            mAHBTrans->set_data_ptr(&pData0[0]);
            m_adtrg_mem_write_time += 1;
            mAHBTransG3Ext->setSpId(mSPID);
            mAhbMasterIf->write(*mAHBTrans, false, ahb_tran_time, false);
            if (mSimMode == SIM_MODE_CA){
                wait(m_trans_comp_ca | m_abnormal_event);
            }
            mAddress += 4;
        }

        //Monitor write memory
       if (mSimMode == SIM_MODE_FAST) {
           m_adtrg_mem_write_time = 0;
           mpHWCORE_ADTRGFunc->SetRegisterVal("ADMNMON","MNNUM", m_mnnum + 1);
       }
    }
}//}}}


// Description: Handle IIR Completion Method
// Ref: {HWCORE_ADTRG_UD_Flow_053}
void HWCORE_ADTRG::HandleIIRCompletionMethod()
{//{{{
   unsigned char adbufsel = mpHWCORE_ADTRGFunc->GetRegisterVal("ADBUFSEL");
    if ((adbufsel & 0x3) == 0x1) {
        m_output_port_data_value[emADTRG_CH0_DAT_PORT] = iiroutdat0.read();
        m_update_output_data_event[emADTRG_CH0_DAT_PORT].notify(SC_ZERO_TIME);
    }
    if (((adbufsel >> 2) & 0x3) == 0x2) {
        m_output_port_data_value[emADTRG_CH1_DAT_PORT] = iiroutdat1.read();
        m_update_output_data_event[emADTRG_CH1_DAT_PORT].notify(SC_ZERO_TIME);
    }
    if (((adbufsel >> 4) & 0x3) == 0x3) {
        m_output_port_data_value[emADTRG_CH2_DAT_PORT] = iiroutdat2.read();
        m_update_output_data_event[emADTRG_CH2_DAT_PORT].notify(SC_ZERO_TIME);
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
// Description: Check clock period
// Ref: {HWCORE_ADTRG_UD_Direction_039}
bool HWCORE_ADTRG::CheckClockPeriod(std::string clock_name)
{//{{{
    bool clock_status = false;
    if(clock_name == "PCLK") {
        if (mPCLKFreq > 0) {
            clock_status = true;
        }else{
            re_printf("info", "PCLK frequency is zero\n");
        }
    } else {
        if (mCCLKFreq > 0) {
            clock_status = true;
        }else{
            re_printf("info", "CCLK frequency is zero\n");
        }
    }
    return clock_status;
}//}}}

// Description: Get reset status
// Ref: {HWCORE_ADTRG_UD_Flow_001, HWCORE_ADTRG_UD_Flow_002}
bool HWCORE_ADTRG::GetResetStatus(std::string domain)
{//{{{
    if (domain == "SSCG") {
        return (mIsResetHardActive[emPRESETnId] || mIsResetCmdActive[emPRESETnId] || mIsResetHardActive[emctr_emustId] || mIsResetCmdActive[emctr_emustId]);
    } else {
        return (mIsResetHardActive[emcrst_nId] || mIsResetCmdActive[emcrst_nId]);
    }
}//}}}

// Description: Handle AD/Start Trigger Method
// Ref: {HWCORE_ADTRG_UD_Flow_016}
void HWCORE_ADTRG::HandleADStartTriggerMethod()
{//{{{

    m_adtrg_in_delay_period = false;
    
    if(((m_adtrg_att >> emAD_SAMP_TROUGH) & 0x1) == 0x1) {
        m_ad_samp_vall_count++;
        if(m_ad_samp_vall_count == 1){
            m_adtrg_att |= (1 << emTSTVLYTIM_ATT);
        }
        else{
            m_adtrg_att = 1 << emAD_SAMP_TROUGH;
        }

    }
    if((m_adtrg_att >> emAD_SAMP_PEAK & 0x1) == 0x1) {
        m_ad_samp_mou_count ++;
        if(m_ad_samp_mou_count == 1){
            m_adtrg_att |= (1 << emTSTPEKTIM_ATT);
        }
        else{
            m_adtrg_att = 1 << emAD_SAMP_PEAK;
        }
    }

    //AD start trigger mask control
    if(ADTriggerMaskControl(emAD_START_TRIG)) return;

    if (m_adc_status != emAD_BUSY) {
        m_output_port_edge_value[emADC_START_PORT] = true;
        m_update_output_edge_event[emADC_START_PORT].notify(CalculateCLKPosEdge((double)mCCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    } else { // Overlap A/D startup request
        m_output_port_edge_value[emADTRG_AMERR_PORT] = true;
        m_update_output_edge_event[emADTRG_AMERR_PORT].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
    }
}//}}}

// Description: Update AD Completion Trigger control Port
// Ref: {HWCORE_ADTRG_UD_Flow_053}
void HWCORE_ADTRG::UpdateADCompletionTriggerControlPort(unsigned char adend)
{//{{{
    m_output_port_data_value[emEM3IADCTR_ADEND_PORT] = adend;
    m_update_output_data_event[emEM3IADCTR_ADEND_PORT].notify(SC_ZERO_TIME);
}//}}}

// Description: Handle Delay Timer Method
// Ref: {HWCORE_ADTRG_UD_Flow_019}
void HWCORE_ADTRG::GetDelayTimer(bool swtrg)
{//{{{
    if (swtrg == true) {
        m_ad_start_trg_delay  = CalculateCLKPosEdge((double)mCCLKPeriod, false, sc_time_stamp().to_double(), mStartCCLKTime);
        m_ang_start_trg_delay = CalculateCLKPosEdge((double)mCCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime);
    } else {
        m_ad_start_trg_delay  = (mpHWCORE_ADTRGFunc->GetRegisterVal("ADDCNT") & 0x3FFFF) * (double)mCCLKPeriod;
        m_ang_start_trg_delay = (mpHWCORE_ADTRGFunc->GetRegisterVal("RDDCNT") & 0x3FFFF) * (double)mCCLKPeriod;
    }
}//}}}

// Description: notifyTransComp
// Ref: {HWCORE_ADTRG_UD_Flow_052}
void HWCORE_ADTRG::notifyTransComp(TlmTransactionType &trans, BusTime_t &t)
{//{{{
    m_adtrg_mem_write_time--;
    mpHWCORE_ADTRGFunc->SetRegisterVal("ADMNMON","MNNUM", m_mnnum + 1);
    m_trans_comp_ca.notify(SC_ZERO_TIME);
}//}}}

// Description: Handle mask control
// Ref: {HWCORE_ADTRG_UD_Flow_012}
bool HWCORE_ADTRG::ADTriggerMaskControl(eMASK_CONTROL emType)
{//{{{
    unsigned short mask = 0;
    bool isSamplingCarrierPeak = false;
    bool isSamplingCarrierTrough = false;
    bool SMVEN = false;
    bool SMMEN = false;
       if((emType == emAD_START_TRIG) || (emType == emANG_START_TRG)){
        mask = mpHWCORE_ADTRGFunc->GetRegisterVal("ADTRGM");
    }
    else{    //Completion trigger mask control
        if((mpHWCORE_ADTRGFunc->GetRegisterVal("ADCTR") & 0x1) == 0x1){
            return false;
        }
        mask = mpHWCORE_ADTRGFunc->GetRegisterVal("ADEDM");
    }

    unsigned char SMCTR = mpHWCORE_ADTRGFunc->GetRegisterVal("SMCTR");
      if((SMCTR & 0x1) == 0x1){
        SMMEN = true;
    }
    else{
        SMMEN = false;
    }

    if(((SMCTR >> 1) & 0x1) == 0x1){
        SMVEN = true;
    }
    else{
        SMVEN = false;
    }

    if((m_adtrg_att >> emAD_SAMP_PEAK) == 0x1){
        isSamplingCarrierPeak = true;
    }
    else{
        isSamplingCarrierPeak = false;
    }

    if((m_adtrg_att >> emAD_SAMP_TROUGH) == 0x1){
        isSamplingCarrierTrough = true;
    }
    else{
        isSamplingCarrierTrough = false;
    }

    if(SMMEN){
        if(emType == emANG_START_TRG){
            if (((mask >> (m_ang_samp_mou_lat_count-1)) & 0x1) == 0x1) {
                return true;
            }
        }
        else{
               if(isSamplingCarrierPeak && (((mask >> (m_ad_samp_mou_count-1)) & 0x1) == 0x1)){
                return true;
            }
        }
    }

    if(SMVEN){
        if(emType == emANG_START_TRG){
            if (((mask >> (m_ang_samp_vall_lat_count-1)) & 0x1) == 0x1) {
                return true;
            }
        }
        else{
               if(isSamplingCarrierTrough && (((mask >> (m_ad_samp_vall_count-1)) & 0x1) == 0x1)){
                return true;
            }
        }
    }

    return false;
}//}}}

// vim600: set foldmethod=marker :

