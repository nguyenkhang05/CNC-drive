// ---------------------------------------------------------------------
// $Id: EMU3SSS.cpp,v 1.11 2020/11/21 03:47:59 thiepnguyen Exp $
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
// Description: Copyright of EMU3SSS.cpp
// Description: About the include header files of EMU3SSS.cpp
#include "re_define.h"
#include "EMU3SSS.h"

// Description: Constructor of EMU3SSS class
// Ref: {EMU3SSS_UD_ReferenceDoc_001, EMU3SSS_UD_BlockDiagram_001, EMU3SSS_UD_Summary_001, EMU3SSS_UD_Feature_001}
EMU3SSS::EMU3SSS(sc_module_name name, uint32_t rLatency, uint32_t wLatency, SIM_MODE_T simmode )
    :sc_module(name)
    ,rvc_common_model()
    ,tsp_cpu("tsp_cpu")
    ,ish("ish")
    ,PCLK("PCLK")
    ,CCLK("CCLK")
    ,PRESETn("PRESETn")
    ,CRESETn("CRESETn")
    ,tstpektim("tstpektim")
    ,tstvlytim("tstvlytim")
    ,tstadt0("tstadt0")
    ,tstadt1("tstadt1")
    ,tstint3("tstint3")
    ,tstint4("tstint4")
    ,tstwdcmp0("tstwdcmp0")
    ,tstwdcmpu("tstwdcmpu")
    ,tstwdcmpv("tstwdcmpv")
    ,tstwdcmpw("tstwdcmpw")
    ,tstwecmp("tstwecmp")
    ,tstiup("tstiup")
    ,tstivp("tstivp")
    ,tstiwp("tstiwp")
    ,adc_start("adc_start")
    ,adc_grpend("adc_grpend")
    ,adc_ch0end("adc_ch0end")
    ,adc_ch0dat("adc_ch0dat")
    ,adc_ch1end("adc_ch1end")
    ,adc_ch1dat("adc_ch1dat")
    ,adc_ch2end("adc_ch2end")
    ,adc_ch2dat("adc_ch2dat")
    ,adc1_start("adc1_start")
    ,adc1_grpend("adc1_grpend")
    ,adc1_ch0end("adc1_ch0end")
    ,adc1_ch0dat("adc1_ch0dat")
    ,adc1_ch1end("adc1_ch1end")
    ,adc1_ch1dat("adc1_ch1dat")
    ,adc1_ch2end("adc1_ch2end")
    ,adc1_ch2dat("adc1_ch2dat")
    ,rdc_phi("rdc_phi")
    ,rdc_z("rdc_z")
    ,rdc_high_omega("rdc_high_omega")
    ,rdc1_phi("rdc1_phi")
    ,rdc1_z("rdc1_z")
    ,rdc1_high_omega("rdc1_high_omega")
    ,emu3_stop_req("emu3_stop_req")
    ,emu3_stop_ack("emu3_stop_ack")
    ,EMU3_LCSTERR_P("EMU3_LCSTERR_P")
    ,EMU3_LCSTERR_C("EMU3_LCSTERR_C")
    ,EMU3_EDCERR_P("EMU3_EDCERR_P")
    ,EMU3_EDCERR_C("EMU3_EDCERR_C")
    ,sferc_sig("sferc_sig")
    ,emu3_stop_ack_ahbarb_sig("emu3_stop_ack_ahbarb_sig")
    ,emu3_stop_ack_apbdec_sig("emu3_stop_ack_apbdec_sig")
    ,edcerr_sed_pclk_sig("edcerr_sed_pclk_sig")
    ,edcerr_ded_pclk_sig("edcerr_ded_pclk_sig")
    ,edcerr_sed_cclk_sig("edcerr_sed_cclk_sig")
    ,edcerr_ded_cclk_sig("edcerr_ded_cclk_sig")
{//{{{
    
    // Create objects
    mEMU3S_AHB_ARB = new EMU3S_AHB_ARB("EMU3S_AHB_ARB", simmode);
    if(mEMU3S_AHB_ARB == NULL){
        return;

    }
    mEMU3S_APB_DEC = new EMU3S_APB_DEC("EMU3S_APB_DEC", simmode);
    if(mEMU3S_APB_DEC == NULL){
        return;
    }
    
    mEMU3S_HWCORE = new EMU3S_HWCORE("EMU3S_HWCORE", rLatency, wLatency, simmode);
    if(mEMU3S_HWCORE == NULL){
        return;
    }

    for (unsigned int i = 0; i < emINTERRUPT_NUM; i++)
    {
        std::ostringstream port_name;
        port_name.str("");
        port_name << "emu_int_" << i;
        emu_int[i] = new sc_out<bool>(port_name.str().c_str());
    }

    // Initialize mode
    Initialize(emINITIALIZE);

    // Create connections
    CreateConnections();
    
    //Initialize variables
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
    mTimePCLKChangePoint    = 0;
    mTimeCCLKChangePoint    = 0;
    mDumpInterrupt          = false;

    mIsPRESETnActive        = false;
    mIsPRESETnHardActive    = false;
    mIsPRESETnCmdReceive    = false;
    mIsPRESETnCmdActive     = false;
    mPRESETnCmdPeriod       = 0;

    mIsCRESETnActive        = false;
    mIsCRESETnHardActive    = false;
    mIsCRESETnCmdReceive    = false;
    mIsCRESETnCmdActive     = false;
    mCRESETnCmdPeriod       = 0;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    // Clock/Reset
    SC_METHOD(MonitorPCLKMethod);
    sensitive << PCLK;

    SC_METHOD(MonitorCCLKMethod);
    sensitive << CCLK;

    SC_METHOD(MonitorPRESETnMethod);
    sensitive << PRESETn;

    SC_METHOD(MonitorCRESETnMethod);
    sensitive << CRESETn;

    SC_METHOD(HandlePRESETnMethod);
    dont_initialize();
    sensitive << mPRESETnResetEvent;

    SC_METHOD(HandleCRESETnMethod);
    dont_initialize();
    sensitive << mCRESETnResetEvent;

    // Processes supports python cmd
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

    //FUSA Python commands
    SC_METHOD(Write_EDCERR_P_Method);
    dont_initialize();
    sensitive << mWrite_EDCERR_P_Event;

    SC_METHOD(Assert_EDCERR_P_Method);
    dont_initialize();
    sensitive << mAssert_EDCERR_P_Event;

    SC_METHOD(Deassert_EDCERR_P_Method);
    dont_initialize();
    sensitive << mDeassert_EDCERR_P_Event;

    SC_METHOD(Write_EDCERR_C_Method);
    dont_initialize();
    sensitive << mWrite_EDCERR_C_Event;

    SC_METHOD(Assert_EDCERR_C_Method);
    dont_initialize();
    sensitive << mAssert_EDCERR_C_Event;

    SC_METHOD(Deassert_EDCERR_C_Method);
    dont_initialize();
    sensitive << mDeassert_EDCERR_C_Event;

    SC_METHOD(Write_LCSTERR_P_Method);
    dont_initialize();
    sensitive << mWrite_LCSTERR_P_Event;

    SC_METHOD(Assert_LCSTERR_P_Method);
    dont_initialize();
    sensitive << mAssert_LCSTERR_P_Event;

    SC_METHOD(Deassert_LCSTERR_P_Method);
    dont_initialize();
    sensitive << mDeassert_LCSTERR_P_Event;

    SC_METHOD(Write_LCSTERR_C_Method);
    dont_initialize();
    sensitive << mWrite_LCSTERR_C_Event;

    SC_METHOD(Assert_LCSTERR_C_Method);
    dont_initialize();
    sensitive << mAssert_LCSTERR_C_Event;

    SC_METHOD(Deassert_LCSTERR_C_Method);
    dont_initialize();
    sensitive << mDeassert_LCSTERR_C_Event;

    SC_METHOD(Handle_EDC_C_Error_Method)
    dont_initialize();
    sensitive << edcerr_sed_cclk_sig;
    sensitive << edcerr_ded_cclk_sig;

    SC_METHOD(Handle_EDC_P_Error_Method)
    dont_initialize();
    sensitive << edcerr_sed_pclk_sig;
    sensitive << edcerr_ded_pclk_sig;

    SC_METHOD(Monitor_STOP_REQ_Method);
    sensitive << emu3_stop_req;

    SC_METHOD(Monitor_STOP_ACK_Method);
    dont_initialize();
    sensitive << emu3_stop_ack_ahbarb_sig;
    sensitive << emu3_stop_ack_apbdec_sig;

    SC_METHOD(Handle_STOP_ACK_Method);
    sensitive << mWrite_STOP_ACK_Event;

}//}}

// Description: Destructor of EMU3SSS class
EMU3SSS::~EMU3SSS(void)
{//{{{
    for (unsigned int i = 0; i < emINTERRUPT_NUM; i++)
    {
        delete emu_int[i];
    }
    delete mEMU3S_AHB_ARB;
    delete mEMU3S_APB_DEC;
    delete mEMU3S_HWCORE;
}//}}}

// Description: Initialize members of model
// Ref: {EMU3SSS_UD_Ports_001, EMU3SSS_UD_Ports_008, EMU3SSS_UD_Flow_001}
void EMU3SSS::Initialize(eRESET_CONTROL emType)
{
    //Initialize variables
    mEDCERR_P       = false;
    mEDCERR_C       = false;
    mLCSTERR_P      = false;
    mLCSTERR_C      = false;

    if(emType == emINITIALIZE)
    {
        memu3_stop_ack  = false;

        for (unsigned int i = 0; i < emINTERRUPT_NUM; i++)
        {
            emu_int[i]->initialize(false);
        }

        tstwdcmp0.initialize(false);
        tstwdcmpu.initialize(false);
        tstwdcmpv.initialize(false);
        tstwdcmpw.initialize(false);
        tstwecmp.initialize(false);
        tstiup.initialize(false);
        tstivp.initialize(false);
        tstiwp.initialize(false);
        adc_start.initialize(false);
        adc1_start.initialize(false);
        emu3_stop_ack.initialize(false);
        EMU3_LCSTERR_P.initialize(false);
        EMU3_LCSTERR_C.initialize(false);
        EMU3_EDCERR_P.initialize(false);
        EMU3_EDCERR_C.initialize(false);

        //Initialize signal
        sferc_sig.write(0);
        emu3_stop_ack_ahbarb_sig.write(false);
        emu3_stop_ack_apbdec_sig.write(false);
        edcerr_sed_pclk_sig.write(false);
        edcerr_ded_pclk_sig.write(false);
        edcerr_sed_cclk_sig.write(false);
        edcerr_ded_cclk_sig.write(false);
        emu3_stop_req_sig.write(false);
        PCLK_sig.write(false);
        CCLK_sig.write(false);
        PRESETn_sig.write(true);
        CRESETn_sig.write(true);
    }
    else if (emType == emRESET_CLEAN)
    {   
        //Cancel events
        mWrite_EDCERR_C_Event.cancel();
        mAssert_EDCERR_C_Event.cancel();
        mDeassert_EDCERR_C_Event.cancel();
        mWrite_LCSTERR_C_Event.cancel();
        mAssert_LCSTERR_C_Event.cancel();
        mDeassert_LCSTERR_C_Event.cancel();

        //Reset ports
        mWrite_LCSTERR_C_Event.notify(SC_ZERO_TIME);
        mWrite_EDCERR_C_Event.notify(SC_ZERO_TIME);
    }
    else    //(emType == emRESET_SSCG)
    {
        //Cancel events
        mWrite_EDCERR_P_Event.cancel();
        mAssert_EDCERR_P_Event.cancel();
        mDeassert_EDCERR_P_Event.cancel();
        mWrite_LCSTERR_P_Event.cancel();
        mAssert_LCSTERR_P_Event.cancel();
        mDeassert_LCSTERR_P_Event.cancel();
        mWrite_STOP_ACK_Event.cancel();

        //Reset ports
        mWrite_EDCERR_P_Event.notify(SC_ZERO_TIME);
        mWrite_LCSTERR_P_Event.notify(SC_ZERO_TIME);
    }
}

// Description: set base address
void EMU3SSS::start_of_simulation(void) {
    mEMU3S_HWCORE->tsp_cpu->setBaseAddressSize(mEMU3S_APB_DEC->tsp_cpu->getBaseAddress(), mEMU3S_APB_DEC->tsp_cpu->getBaseSize());
}

// Description: Create connections
// Ref: {EMU3SSS_UD_Ports_005, EMU3SSS_UD_Ports_006, EMU3SSS_UD_Ports_008}
void EMU3SSS::CreateConnections()
{
    // Initialize Interconnection
    ( tsp_cpu )(*( mEMU3S_APB_DEC->tsp_cpu ));
    (*( mEMU3S_APB_DEC->isp_cpu ))(*( mEMU3S_HWCORE->tsp_cpu ));

    ( mEMU3S_HWCORE->ish )(*( mEMU3S_AHB_ARB->tsh ));
    (*( mEMU3S_AHB_ARB->ish ))( ish );

    //Binding ports
    mEMU3S_AHB_ARB->AHBARB_STOP_ACK(emu3_stop_ack_ahbarb_sig);
    mEMU3S_AHB_ARB->PCLK(PCLK_sig);
    mEMU3S_AHB_ARB->PRESETn(PRESETn_sig);
    mEMU3S_AHB_ARB->EMU3_STOP_REQ(emu3_stop_req_sig);

    mEMU3S_APB_DEC->APBDEC_STOP_ACK(emu3_stop_ack_apbdec_sig);
    mEMU3S_APB_DEC->PCLK(PCLK_sig);
    mEMU3S_APB_DEC->PRESETn(PRESETn_sig);
    mEMU3S_APB_DEC->EMU3_STOP_REQ(emu3_stop_req_sig);

    mEMU3S_HWCORE->SFERC(sferc_sig);
    mEMU3S_HWCORE->edcerr_sed_pclk(edcerr_sed_pclk_sig);
    mEMU3S_HWCORE->edcerr_ded_pclk(edcerr_ded_pclk_sig);
    mEMU3S_HWCORE->edcerr_sed_cclk(edcerr_sed_cclk_sig);
    mEMU3S_HWCORE->edcerr_ded_cclk(edcerr_ded_cclk_sig);
    
    mEMU3S_HWCORE->PCLK(PCLK_sig);
    mEMU3S_HWCORE->CCLK(CCLK_sig);
    mEMU3S_HWCORE->PRESETn(PRESETn_sig);
    mEMU3S_HWCORE->CRESETn(CRESETn_sig);

    mEMU3S_HWCORE->adc_start(adc_start);
    mEMU3S_HWCORE->adc_grpend(adc_grpend);
    mEMU3S_HWCORE->adc_ch0end(adc_ch0end);
    mEMU3S_HWCORE->adc_ch0dat(adc_ch0dat);
    mEMU3S_HWCORE->adc_ch1end(adc_ch1end);
    mEMU3S_HWCORE->adc_ch1dat(adc_ch1dat);
    mEMU3S_HWCORE->adc_ch2end(adc_ch2end);
    mEMU3S_HWCORE->adc_ch2dat(adc_ch2dat);
    mEMU3S_HWCORE->adc1_start(adc1_start);
    mEMU3S_HWCORE->adc1_grpend(adc1_grpend);
    mEMU3S_HWCORE->adc1_ch0end(adc1_ch0end);
    mEMU3S_HWCORE->adc1_ch0dat(adc1_ch0dat);
    mEMU3S_HWCORE->adc1_ch1end(adc1_ch1end);
    mEMU3S_HWCORE->adc1_ch1dat(adc1_ch1dat);
    mEMU3S_HWCORE->adc1_ch2end(adc1_ch2end);
    mEMU3S_HWCORE->adc1_ch2dat(adc1_ch2dat);
    mEMU3S_HWCORE->rdc_phi(rdc_phi);
    mEMU3S_HWCORE->rdc_z(rdc_z);
    mEMU3S_HWCORE->rdc_high_omega(rdc_high_omega);
    mEMU3S_HWCORE->rdc1_phi(rdc1_phi);
    mEMU3S_HWCORE->rdc1_z(rdc1_z);
    mEMU3S_HWCORE->rdc1_high_omega(rdc1_high_omega);
    mEMU3S_HWCORE->tstpektim(tstpektim);
    mEMU3S_HWCORE->tstvlytim(tstvlytim);
    mEMU3S_HWCORE->tstadt0(tstadt0);
    mEMU3S_HWCORE->tstadt1(tstadt1);
    mEMU3S_HWCORE->tstint3(tstint3);
    mEMU3S_HWCORE->tstint4(tstint4);
    mEMU3S_HWCORE->tstwdcmp0(tstwdcmp0);
    mEMU3S_HWCORE->tstwdcmpu(tstwdcmpu);
    mEMU3S_HWCORE->tstwdcmpv(tstwdcmpv);
    mEMU3S_HWCORE->tstwdcmpw(tstwdcmpw);
    mEMU3S_HWCORE->tstwecmp(tstwecmp);
    mEMU3S_HWCORE->tstiup(tstiup);
    mEMU3S_HWCORE->tstivp(tstivp);
    mEMU3S_HWCORE->tstiwp(tstiwp);

    for (unsigned int i = 0; i < emINTERRUPT_NUM; i++)
    {
        (*mEMU3S_HWCORE->emu_int[i])(*emu_int[i]);
    }
}

// Description: Monitor PCLK clock
// Ref: {EMU3SSS_UD_Ports_002, EMU3SSS_UD_Flow_004}
void EMU3SSS::MonitorPCLKMethod()
{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz", "ALL");
}

// Description: Monitor CCLK clock
// Ref: {EMU3SSS_UD_Ports_002, EMU3SSS_UD_Flow_004}
void EMU3SSS::MonitorCCLKMethod()
{
    sc_dt::uint64 freq_value = CCLK.read();
    SetCLKFreq("CCLK", freq_value, "Hz", "ALL");
}

// Description: Monitor PRESETn port
// Ref: {EMU3SSS_UD_Ports_002, EMU3SSS_UD_Flow_001, EMU3SSS_UD_Flow_003, EMU3SSS_UD_Direction_036}
void EMU3SSS::MonitorPRESETnMethod()
{
    bool reset = PRESETn.read();
    if (reset == vpcl::emResetActive) {
        mIsPRESETnHardActive = true;
        re_printf("info", "The reset port PRESETn is asserted.\n");
        mIsPRESETnActive = true;
        mPRESETnResetEvent.notify(SC_ZERO_TIME);
    }
    else {
        mIsPRESETnHardActive = false;
        re_printf("info", "The reset port PRESETn is de-asserted.\n");
        mIsPRESETnActive = false;
        mPRESETnResetEvent.notify(SC_ZERO_TIME);
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
 // Ref: {EMU3SSS_UD_Ports_002, EMU3SSS_UD_Flow_001, EMU3SSS_UD_Direction_037}
void EMU3SSS::MonitorCRESETnMethod()
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

// Description: Write PRESETn signal according to reset status
// Ref: {EMU3SSS_UD_Ports_002, EMU3SSS_UD_Flow_001}
void EMU3SSS::HandlePRESETnMethod()
{
    // Forward reset signal to sub-ips
    PRESETn_sig.write(mIsPRESETnActive == vpcl::emResetActive);
    if (mIsPRESETnActive) 
    {
        Initialize(emRESET_SSCG);
    }
}

// Description: CRESETn reset operation
// Ref: {EMU3SSS_UD_Ports_002, EMU3SSS_UD_Flow_001}
void EMU3SSS::HandleCRESETnMethod()
{
    CRESETn_sig.write(mIsPRESETnActive == vpcl::emResetActive);
    if (mIsCRESETnActive) {
        Initialize(emRESET_CLEAN);
    }
}

// Description: PRESETn reset operation
// Ref: {EMU3SSS_UD_ATT1_005, EMU3SSS_UD_Direction_034}
void EMU3SSS::HandlePRESETnCmdMethod()
{
    mIsPRESETnCmdActive = true;
    re_printf("info", "The model is reset by AssertReset command of PRESETn.\n");
    mIsPRESETnActive = mIsPRESETnCmdActive;
    mPRESETnResetEvent.notify(SC_ZERO_TIME);
    mPRESETnCmdCancelEvent.notify(mPRESETnCmdPeriod, SC_NS);
}

// Description: CRESETn reset operation
// Ref: {EMU3SSS_UD_ATT1_005}
void EMU3SSS::HandleCRESETnCmdMethod()
{
    mIsCRESETnCmdActive = true;
    re_printf("info", "The model is reset by AssertReset command of CRESETn.\n");
    mIsCRESETnActive = mIsCRESETnCmdActive;
    mCRESETnResetEvent.notify(SC_ZERO_TIME);
    mCRESETnCmdCancelEvent.notify(mCRESETnCmdPeriod, SC_NS);
}

// Description: Dump help message of all parameters or commands
// Ref: {EMU3SSS_UD_Direction_019, EMU3SSS_UD_Direction_021, EMU3SSS_UD_Direction_022}
void EMU3SSS::Help(const std::string type)
{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") 
        {
            SC_REPORT_INFO(this->basename(), "EMU3SSS_MessageLevel          (EMU3SSS instance, fatal|error|warning|info, sub_ip_name)       Set debug message level(default: fatal|error). for 1 sub-ip default for all(default)");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_DumpRegisterRW        (EMU3SSS instance, true|false, sub_ip_name)                     Enable/disable dumping access register(default: false) in 1 sub-ip or all(default)   .");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_EnableRegisterMessage (EMU3SSS instance, true|false, sub_ip_name)                     Enable/disable info/warning/error message of register IF(default: true) in 1 sub-ip or all (default).");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_DumpInterrupt         (EMU3SSS instance, true|false, sub_ip_name)                     Enable/disable dumping interrupt information(default:false)  in 1 sub-ip or all(default).");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_EnableStatInfo        (EMU3SSS instance, true|false, sub_ip_name)                     Enable/disable dumping automatically statistical information (default:false)  in 1 sub-ip or all(default).");
        }
        else if (word_vector[0] == "commands") 
        {
            SC_REPORT_INFO(this->basename(), "EMU3SSS_AssertReset           (EMU3SSS instance, rst_name, start_time, period,sub_ip_name )   Assert and de-assert reset signal to the 1 sub-ip of EMU3SSS model or all(default).");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_SetCLKFreq            (EMU3SSS instance, clk_name, freq, unit, sub_ip_name)           Set clock frequency to 1 sub-ip of EMU3SSS model or all (default).");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_GetCLKFreq            (EMU3SSS instance, clk_name, sub_ip_name)                       Get clock frequency of 1 sub-ip of EMU3SSS model or all (default).");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_ForceRegister         (EMU3SSS instance, reg_name, value, sub_ip_name)                Force register with setting value for register of sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_ReleaseRegister       (EMU3SSS instance, reg_name, value, sub_ip_name)                Release register from force value for register of sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_WriteRegister         (EMU3SSS instance, reg_name , value, sub_ip_name)               Write a value to register for sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_ReadRegister          (EMU3SSS instance, reg_name , sub_ip_name)                      Read a value from register for sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_ListRegister          (EMU3SSS Instance, sub_ip_name)                                 Dump register names which is passed as sub_ip_name");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_DumpStatInfo          (EMU3SSS instance, sub_ip_name)                                 Dump the statistical information of sub-ip");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_ChgProcessingTime     (EMU3SSS instance, sub_ip_name, func_name, value)               Change the process time of the func_name of sub-ip.");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_SetAHBExtension       (EMU3SSS instance, value)                                       Change the extension of Sub-IP AHB transaction.");
            SC_REPORT_INFO(this->basename(), "EMU3SSS_SetFUSAError          (EMU3SSS instance, domain, error_type, status)                  Set FUSA errors.");

        }
        else 
        {
            re_printf("warning", "The name (%s) of EMU3SSS_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    }
    else 
    {
        re_printf("warning", "The name (%s) of EMU3SSS_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}

// Description: Cancel PRESETn function triggered by PRESETn command
// Ref: {EMU3SSS_UD_Flow_006, EMU3SSS_UD_Direction_033}
void EMU3SSS::CancelPRESETnCmdMethod()
{
    mIsPRESETnCmdActive = false;
    mIsPRESETnCmdReceive = false;
    re_printf("info", "Reset period of PRESETn is over.\n");
    mIsPRESETnActive = mIsPRESETnCmdActive;
    mPRESETnResetEvent.notify(SC_ZERO_TIME);
}

// Description: Cancel CRESETn function triggered by CRESETn command
// Ref: {EMU3SSS_UD_Flow_006, EMU3SSS_UD_Direction_033}
void EMU3SSS::CancelCRESETnCmdMethod()
{
    mIsCRESETnCmdActive = false;
    mIsCRESETnCmdReceive = false;
    re_printf("info", "Reset period of CRESETn is over.\n");
    mIsCRESETnActive = mIsCRESETnCmdActive;
    mCRESETnResetEvent.notify(SC_ZERO_TIME);
}

// Description: Command to assert reset
// Ref: {EMU3SSS_UD_Direction_008, EMU3SSS_UD_Flow_002, EMU3SSS_UD_Direction_029, EMU3SSS_UD_Direction_030, EMU3SSS_UD_Direction_035}
void EMU3SSS::AssertReset(const std::string reset_name, const double start_time, const double period, std::string sub_ip_name)
{
    if (sub_ip_name == "ALL")
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
    else 
    {
        mEMU3S_HWCORE->AssertReset(reset_name, start_time, period, sub_ip_name);   
    }
}

// Description: Set message level (fatal, error, warning, info)
// Ref: {EMU3SSS_UD_Direction_003, EMU3SSS_UD_Direction_040}
void EMU3SSS::SetMessageLevel(const std::string msg_lv, std::string sub_ip_name)
{
    mEMU3S_HWCORE->SetMessageLevel(msg_lv, sub_ip_name);
}

// Description: Enable/disable dumping message when users access registers
// Ref: {EMU3SSS_UD_Direction_004, EMU3SSS_UD_Direction_023}
void EMU3SSS::DumpRegisterRW(const std::string is_enable, std::string sub_ip_name)
{
    mEMU3S_HWCORE->DumpRegisterRW(is_enable, sub_ip_name);
}

// Description: Enable/disable info/warning/error message of register IF
// Ref: {EMU3SSS_UD_Direction_005}
void EMU3SSS::EnableRegisterMessage(const std::string is_enable, std::string sub_ip_name)
{
    mEMU3S_HWCORE->EnableRegisterMessage(is_enable, sub_ip_name);
}

 // Description: Enable/disable dumping message in case of interrupt request
 // Ref: {EMU3SSS_UD_Direction_006, EMU3SSS_UD_Direction_039}
void EMU3SSS::DumpInterrupt(const std::string is_enable, std::string sub_ip_name)
{
    if (sub_ip_name == "ALL")
    {
        DumpIntTop(is_enable);
        mEMU3S_HWCORE->DumpInterrupt(is_enable, "ALL");
    }
    else
    {
        mEMU3S_HWCORE->DumpInterrupt(is_enable, sub_ip_name);
    }
}

// Description: Determine whether enable or disable dumping automatically statistical
// Ref: {EMU3SSS_UD_Direction_007}
void EMU3SSS::EnableStatInfo(const std::string is_enable, std::string sub_ip_name)
{
    mEMU3S_HWCORE->EnableStatInfo(is_enable, sub_ip_name);
}

// Description: Set clock value and clock unit
// Ref: {EMU3SSS_UD_Direction_009, EMU3SSS_UD_Flow_004, EMU3SSS_UD_Flow_005, EMU3SSS_UD_Direction_026, EMU3SSS_UD_Direction_027, EMU3SSS_UD_Direction_031, EMU3SSS_UD_Direction_032}
void EMU3SSS::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit, std::string sub_ip_name)
{
    if (sub_ip_name == "ALL")
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
        mEMU3S_HWCORE->SetCLKFreq(clock_name, freq, unit, sub_ip_name);
    }
}


// Description: Get clock value
// Ref: {EMU3SSS_UD_Direction_010}
void EMU3SSS::GetCLKFreq(const std::string clock_name, std::string sub_ip_name)
{
    if (sub_ip_name == "ALL")
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
        mEMU3S_HWCORE->GetCLKFreq(clock_name, sub_ip_name);
    }
}

// Description: Command to force register of model/sub-ip
// Ref: {EMU3SSS_UD_Direction_011}
void EMU3SSS::ForceRegister(std::string reg_name, unsigned int value, std::string sub_ip_name)
{
    mEMU3S_HWCORE->ForceRegister(reg_name, value, sub_ip_name);
}

// Description: Release register of sub-ips
// Ref: {EMU3SSS_UD_Direction_012}
void EMU3SSS::ReleaseRegister(std::string reg_name, std::string sub_ip_name)
{
    mEMU3S_HWCORE->ReleaseRegister(reg_name, sub_ip_name);   
}

// Description: Write value to register of sub-ip
// Ref: {EMU3SSS_UD_Direction_013, EMU3SSS_UD_Feature_006}
void EMU3SSS::WriteRegister(std::string reg_name, unsigned int value, std::string sub_ip_name)
{
    mEMU3S_HWCORE->WriteRegister(reg_name, value, sub_ip_name);
}

// Description: Read value of register of sub-ips
// Ref: {EMU3SSS_UD_Direction_014, EMU3SSS_UD_Feature_006}
void EMU3SSS::ReadRegister(std::string reg_name, std::string sub_ip_name)
{
    mEMU3S_HWCORE->ReadRegister(reg_name, sub_ip_name);
}

// Description: List register of sub-ips
// Ref: {EMU3SSS_UD_Direction_015, EMU3SSS_UD_Feature_006}
void EMU3SSS::ListRegister(std::string sub_ip_name)
{
    mEMU3S_HWCORE->ListRegister(sub_ip_name);
}

// Description: Dump statistical info of all register of sub-ips
// Ref: {EMU3SSS_UD_Direction_016}
void EMU3SSS::DumpStatInfo(const std::string sub_ip_name)
{
    mEMU3S_HWCORE->DumpStatInfo(sub_ip_name);
}

// Description: Change the process time of sub-ips
// Ref: {EMU3SSS_UD_Direction_017}
void EMU3SSS::ChgProcessingTime(const std::string sub_ip_name, std::string func_name, unsigned int value)
{
    mEMU3S_HWCORE->ChgProcessingTime(sub_ip_name, func_name, value);
}

// Description: Change the extension of Sub-IP AHB transaction
// Ref: {EMU3SSS_UD_Direction_018}
void EMU3SSS::SetAHBExtension(unsigned int value)
{
    mEMU3S_HWCORE->SetAHBExtension(value);
}

// Description: Set Functional Safety errors
// Ref: {EMU3SSS_UD_Feature_001, EMU3SSS_UD_Direction_020, EMU3SSS_UD_Flow_012}
void EMU3SSS::SetFUSAError(unsigned char domain, unsigned char error_type, unsigned char status)
{   
    unsigned char mSFERC     = 0;
    bool  mPDNE              = false;
    bool  mCDNE              = false;

    if ((domain <= 1) && (error_type <=2) && (status <=1))
    {
        if (((domain == 0) && (!GetResetStatus("PRESETn")) && (CheckClockPeriod("PCLK")))
           || ((domain == 1) && (!GetResetStatus("CRESETn")) && (CheckClockPeriod("CCLK"))))
        {
            mSFERC = sferc_sig.read();

            mPDNE  = (bool)((mSFERC >> 4) & 0x1);
            mCDNE  = (bool)((mSFERC >> 5) & 0x1);

            if ((error_type == 0) || (error_type == 1)){
                mEMU3S_HWCORE->SetEDCError(domain, error_type, status);
            }
            else 
            { 
                if((mPDNE == true) && (domain == 0))
                {
                    mAssert_LCSTERR_P_Event.notify();
                }
                if(mCDNE == true && (domain == 1))
                {
                    mAssert_LCSTERR_C_Event.notify();
                }
            }
        }
        else
        {
            re_printf("info", "SetFUSAError command does not work when clock is 0 or reset is active\n");
        }
    }
    else
    {
        re_printf("warning", "A value of arguments of SetFUSAError command is prohibited (other than 0 and 1 and 2), the command is ignored\n");
    }
}

// Description: Handle EDC SSCG Domain Error
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Handle_EDC_P_Error_Method( void )
{
    bool  medcerr_sed_pclk   = false;
    bool  medcerr_ded_pclk   = false;
    bool  mSENE              = false;
    bool  mDENE              = false;
    unsigned char mSFERC     = 0;

    mSFERC = sferc_sig.read();
    mSENE  = (bool)((mSFERC >> 0) & 0x1);
    mDENE  = (bool)((mSFERC >> 1) & 0x1);
    medcerr_sed_pclk = edcerr_sed_pclk_sig.read();
    medcerr_ded_pclk = edcerr_ded_pclk_sig.read();

    if(((mSENE == true) && medcerr_sed_pclk) || ((mDENE == true) && medcerr_ded_pclk))
    {
        mAssert_EDCERR_P_Event.notify();
    }
}

// Description: Handle EDC CLEAN Domain Error
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Handle_EDC_C_Error_Method( void )
{
    bool  medcerr_sed_cclk   = false;
    bool  medcerr_ded_cclk   = false;
    bool  mSENE              = false;
    bool  mDENE              = false;
    unsigned char mSFERC     = 0;

    mSFERC = sferc_sig.read();
    mSENE  = (bool)((mSFERC >> 0) & 0x1);
    mDENE  = (bool)((mSFERC >> 1) & 0x1);
    medcerr_sed_cclk = edcerr_sed_cclk_sig.read();
    medcerr_ded_cclk = edcerr_ded_cclk_sig.read();

    if(((mSENE == true) && medcerr_sed_cclk) || ((mDENE == true) && medcerr_ded_cclk))
    {
        mAssert_EDCERR_C_Event.notify();
    }
}

// Description: Check Clock status
// Ref: {EMU3SSS_UD_Flow_002}
bool EMU3SSS::CheckClockPeriod(const std::string clock_name)
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
    else
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

// Description: Check reset status
// Ref: {}
bool EMU3SSS::GetResetStatus(const std::string reset_name)
{
    bool reset_status = false;
    if(reset_name == "PRESETn") 
    {
        if (mIsPRESETnHardActive || mIsPRESETnCmdActive) 
        {
            reset_status = true;
        }
    }
    else
    {
        if (mIsCRESETnHardActive || mIsCRESETnCmdActive) 
        {
            reset_status = true;
        }
    }
    
    return reset_status;
}

// Description: Operate dump interrupt message at emu3sss
// Ref: {}
void EMU3SSS::DumpIntTop(const std::string is_enable)
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
            re_printf("warning","Invalid argument: EMU3SSS_DumpInterrupt %s\n", is_enable.c_str());
        }
    } 
    else if (word_vector.size() == 0) 
    {
        std::string temp = "false";
        if (mDumpInterrupt) 
        {
            temp = "true";
        } 
        re_printf("info","EMU3SSS_DumpInterrupt %s\n",temp.c_str());
    } 
    else 
    {
        re_printf("warning","Invalid argument: EMU3SSS_DumpInterrupt %s\n", is_enable.c_str());
    }
}

// Description: Dump interrupt message at emu3sss
// Ref: {EMU3SSS_UD_Direction_038}
void EMU3SSS::DumpInterruptMsg(const std::string intr_name, const bool value)
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

// Description: Assert EMU3_EDCERR_P port
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Assert_EDCERR_P_Method()
{
    mEDCERR_P = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWrite_EDCERR_P_Event.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassert_EDCERR_P_Event.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}

// Description: Deassert EMU3_EDCERR_P port
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Deassert_EDCERR_P_Method()
{
    mEDCERR_P = false;
    mWrite_EDCERR_P_Event.notify();
}

// Description: Write EMU3_EDCERR_P port
// Ref: {EMU3SSS_UD_Ports_004}
void EMU3SSS::Write_EDCERR_P_Method()
{
    bool current_value = EMU3_EDCERR_P.read();

    if (current_value != mEDCERR_P) {
        EMU3_EDCERR_P.write(mEDCERR_P);
        DumpInterruptMsg("EMU3_EDCERR_P", mEDCERR_P);
    }
}

// Description: Assert EMU3_EDCERR_C port
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Assert_EDCERR_C_Method()
{
    mEDCERR_C = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mTimeCCLKChangePoint);
    mWrite_EDCERR_C_Event.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassert_EDCERR_C_Event.notify(assert_time + (double)mCCLKPeriod, mTimeResolutionUnit);
}

// Description: Deassert EMU3_EDCERR_C port
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Deassert_EDCERR_C_Method()
{
    mEDCERR_C = false;
    mWrite_EDCERR_C_Event.notify();
}

// Description: Write EMU3_EDCERR_C port
// Ref: {EMU3SSS_UD_Ports_004}
void EMU3SSS::Write_EDCERR_C_Method()
{
    bool current_value = EMU3_EDCERR_C.read();

    if (current_value != mEDCERR_C) {
        EMU3_EDCERR_C.write(mEDCERR_C);
        DumpInterruptMsg("EMU3_EDCERR_C", mEDCERR_C);
    }
}

// Description: Assert EMU3_LCSTERR_P port
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Assert_LCSTERR_P_Method()
{
    mLCSTERR_P = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    mWrite_LCSTERR_P_Event.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassert_LCSTERR_P_Event.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
}

// Description: Deassert EMU3_LCSTERR_P port
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Deassert_LCSTERR_P_Method()
{
    mLCSTERR_P = false;
    mWrite_LCSTERR_P_Event.notify();
}

// Description: Write EMU3_LCSTERR_P port
// Ref: {EMU3SSS_UD_Ports_004}
void EMU3SSS::Write_LCSTERR_P_Method()
{
    bool current_value = EMU3_LCSTERR_P.read();

    if (current_value != mLCSTERR_P) {
        EMU3_LCSTERR_P.write(mLCSTERR_P);
        DumpInterruptMsg("EMU3_LCSTERR_P", mLCSTERR_P);
    }
}

// Description: Assert EMU3_LCSTERR_C port
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Assert_LCSTERR_C_Method()
{
    mLCSTERR_C = true;
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mTimeCCLKChangePoint);
    mWrite_LCSTERR_C_Event.notify(assert_time, mTimeResolutionUnit);
    // Deassert after 1 cycle
    mDeassert_LCSTERR_C_Event.notify(assert_time + (double)mCCLKPeriod, mTimeResolutionUnit);
}

// Description: Deassert EMU3_LCSTERR_C port
// Ref: {EMU3SSS_UD_Flow_012}
void EMU3SSS::Deassert_LCSTERR_C_Method()
{
    mLCSTERR_C = false;
    mWrite_LCSTERR_C_Event.notify();
}

// Description: Write EMU3_LCSTERR_C port
// Ref: {EMU3SSS_UD_Ports_004, EMU3SSS_UD_Flow_012}
void EMU3SSS::Write_LCSTERR_C_Method()
{
    bool current_value = EMU3_LCSTERR_C.read();

    if (current_value != mLCSTERR_C) {
        EMU3_LCSTERR_C.write(mLCSTERR_C);
        DumpInterruptMsg("EMU3_LCSTERR_C", mLCSTERR_C);
    }
}

// Description: Monitor stop ack signal
// Ref: {EMU3SSS_UD_Flow_010, EMU3SSS_UD_Flow_012}
void EMU3SSS::Monitor_STOP_ACK_Method()
{
    memu3_stop_ack = emu3_stop_ack_apbdec_sig.read() & emu3_stop_ack_ahbarb_sig.read();
    double current_time = sc_time_stamp().to_double();
    double assert_time = CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint);
    
    if(memu3_stop_ack){
        mWrite_STOP_ACK_Event.notify(assert_time, mTimeResolutionUnit);
    }
    else{
        // Deassert after 1 cycle
        mWrite_STOP_ACK_Event.notify(assert_time + (double)mPCLKPeriod, mTimeResolutionUnit);
    }
}

// Description: Handle bus stop ack signal
// Ref: {EMU3SSS_UD_Flow_010, EMU3SSS_UD_Flow_012}
void EMU3SSS::Handle_STOP_ACK_Method()
{
    bool current_value = emu3_stop_ack.read();

    if (current_value != memu3_stop_ack) {
        emu3_stop_ack.write(memu3_stop_ack);
        DumpInterruptMsg("emu3_stop_ack", memu3_stop_ack);
    }
}

// Description: Monitor stop request signal
// Ref: {EMU3SSS_UD_Flow_010, EMU3SSS_UD_Flow_012}
void EMU3SSS::Monitor_STOP_REQ_Method()
{
    bool current_value = emu3_stop_req.read();
    emu3_stop_req_sig.write(current_value);
    DumpInterruptMsg("emu3_stop_req", current_value);
}
