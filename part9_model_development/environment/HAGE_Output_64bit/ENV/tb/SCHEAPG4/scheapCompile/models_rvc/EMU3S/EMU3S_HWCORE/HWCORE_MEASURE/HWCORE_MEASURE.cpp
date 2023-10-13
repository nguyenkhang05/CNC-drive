// ---------------------------------------------------------------------
// $Id: HWCORE_MEASURE.cpp,v 1.8 2020/11/09 06:18:43 phuongtran Exp $
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
// Description: Copyright of HWCORE_MEASURE.cpp
// Ref: {HWCORE_MEASURE_UD_Copyright_001}
// Description: About the include header files of HWCORE_MEASURE.cpp
// Ref: {HWCORE_MEASURE_UD_ATT1_002}
#include "re_define.h"
#include "HWCORE_MEASURE.h"
#include "HWCORE_MEASURE_Func.h"
// Description: Constructor of HWCORE_MEASURE class
// Ref: {HWCORE_MEASURE_UD_Summary_001, HWCORE_MEASURE_UD_ATT1_004}
HWCORE_MEASURE::HWCORE_MEASURE(sc_module_name name)
    :sc_module(name)
    , HWCORE_MEASURE_AgentController()
    , CRWInterface()
    , rvc_common_model()
    , PCLK("PCLK")
    , CCLK("CCLK")
    , PRESETn("PRESETn")
    , mip_crst_n("mip_crst_n")
    , mip_prst_n("mip_prst_n")
    //Input ports
    , CTR("CTR")
    , rdc_z("rdc_z")
    , rdc_phi("rdc_phi")
    , ANGCTR("ANGCTR")
    , RESTHSFT("RESTHSFT")
    //Output ports
    , pmt2cp("pmt2cp")
    , pmt2of("pmt2of")
    , phi_out("phi_out")
{//{{{ 
    // Configure Slave socket
    mpHWCORE_MEASUREFunc = new HWCORE_MEASURE_Func((std::string)name, this);
    sc_assert(mpHWCORE_MEASUREFunc != NULL);

    // Initialize variables
    mPCLKPeriod =           0;
    mPCLKOrgFreq =          0;
    mPCLKFreq =             0;
    mPCLKFreqUnit =         "Hz";
    mCCLKPeriod =           0;
    mCCLKOrgFreq =          0;
    mCCLKFreq =             0;
    mCCLKFreqUnit =         "Hz";
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }
    mTimeResolutionValue =  1;
    mTimeResolutionUnit =   SC_NS;
    mStartCCLKTime = 0;
    mStartPCLKTime = 0;
    mEnableRegisterMessage = true;
    mDumpInterrupt =        false;

    pmt2_invl_div[0x00] = 0x0001;
    pmt2_invl_div[0x01] = 0x0002;
    pmt2_invl_div[0x02] = 0x0004;
    pmt2_invl_div[0x03] = 0x0008;
    pmt2_invl_div[0x04] = 0x0010;
    pmt2_invl_div[0x05] = 0x0020;
    pmt2_invl_div[0x06] = 0x0040;
    pmt2_invl_div[0x07] = 0x0080;
    pmt2_invl_div[0x08] = 0x0100;
    pmt2_invl_div[0x09] = 0x0200;
    pmt2_invl_div[0x0A] = 0x0400;
    pmt2_invl_div[0x0B] = 0x0800;
    pmt2_invl_div[0x10] = 0x1000;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    Initialize_SSCG();
    Initialize_CLEAN();
    Initialize_OutputPort();

    // SC_THREAD/METHOD
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleCCLKSignalMethod);
    dont_initialize();
    sensitive << CCLK;

    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;

    SC_METHOD(HandleMip_crst_nSignalMethod);
    sensitive << mip_crst_n;

    SC_METHOD(HandleMip_prst_nSignalMethod);
    sensitive << mip_prst_n;

    SC_METHOD(HandleCTRSignalMethod);
    sensitive << CTR;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_MEASURE::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_MEASURE::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_MEASURE::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    //For input part
    SC_METHOD(rdc_phi_procMethod);
    sensitive << rdc_phi;
    dont_initialize();

    SC_METHOD(rdc_z_pos_procMethod);
    sensitive << rdc_z.pos();
    dont_initialize();

    SC_METHOD(RESTHSFT_procMethod);
    sensitive << RESTHSFT;
    dont_initialize();

    //For internal part
    SC_METHOD(pmt2_over_flowMethod);
    sensitive << m_pmt2_over_flowEvent;
    dont_initialize();

    //For interrupt part
    SC_METHOD(AssertPmt2cp_intCLEANMethod);
    dont_initialize();
    sensitive << mAssertPmt2cp_intCLEANEvent;

    SC_METHOD(AssertPmt2cp_intSSCGMethod);
    dont_initialize();
    sensitive << mAssertPmt2cp_intSSCGEvent;

    SC_METHOD(DeassertPmt2cp_intMethod);
    dont_initialize();
    sensitive << mDeassertPmt2cp_intEvent;

    SC_METHOD(WritePmt2cp_intMethod);
    dont_initialize();
    sensitive << mWritePmt2cp_intEvent;

    SC_METHOD(AssertPmt2of_intCLEANMethod);
    dont_initialize();
    sensitive << mAssertPmt2of_intCLEANEvent;

    SC_METHOD(AssertPmt2of_intSSCGMethod);
    dont_initialize();
    sensitive << mAssertPmt2of_intSSCGEvent;

    SC_METHOD(DeassertPmt2of_intMethod);
    dont_initialize();
    sensitive << mDeassertPmt2of_intEvent;

    SC_METHOD(WritePmt2of_intMethod);
    dont_initialize();
    sensitive << mWritePmt2of_intEvent;

    //For output data
    SC_METHOD(WritePhi_outCLEANMethod);
    dont_initialize();
    sensitive << mWritePhi_outCLEANEvent;

    SC_METHOD(WritePhi_outSSCGMethod);
    dont_initialize();
    sensitive << mWritePhi_outSSCGEvent;

}//}}}

// Description: Destructor
HWCORE_MEASURE::~HWCORE_MEASURE()
{//{{{
    delete mpHWCORE_MEASUREFunc;
}//}}}

// Define read APIs
void HWCORE_MEASURE::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if(data_ptr == NULL){
        return;
    }
    mpHWCORE_MEASUREFunc->read(offsetAddress, data_ptr, size, debug);
}//}}}

// Define write APIs
void HWCORE_MEASURE::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if(data_ptr == NULL){
        return;
    }
    mpHWCORE_MEASUREFunc->write(offsetAddress, data_ptr, size, debug);
}//}}}

// Description: Initialize sync with PCLK clock when reset activated
void HWCORE_MEASURE::Initialize_SSCG(void)
{//{{{
    // Initialize variable
    mPmt2cp_int = false;
    mPmt2of_int = false;
    mPhi_out = 0;
    // Initialize output port
    mWritePmt2cp_intEvent.notify();
    mWritePmt2of_intEvent.notify();
    mWritePhi_outSSCGEvent.notify();
}//}}}
// Description: Initialize sync with CCLK clock when reset activated
void HWCORE_MEASURE::Initialize_CLEAN(void)
{//{{{
    // Initialize variable
    pmt2_last_proc_time = 0;
    pmt2_stop_time = 0;
    pmt2_past_angle = 0;
    g_capture_rotation_pos = false;
    g_pmt2_compare_cnt = 0;
    res_value_temp = 0;
    res_value_temp_old = 0;
    mDeassertPmt2cp_intEvent.notify();
    mDeassertPmt2of_intEvent.notify();
    mWritePhi_outCLEANEvent.notify();
    // Initialize output port
}//}}}
// Description: Initialize output ports
void HWCORE_MEASURE::Initialize_OutputPort(void)
{//{{{
    pmt2cp.initialize(false);
    pmt2of.initialize(false);
    phi_out.initialize(0x0);
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
// Description: Check clock period
// Ref: {HWCORE_MEASURE_UD_Direction_036}
bool HWCORE_MEASURE::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector[0] == "PCLK") {
        clock_period = mPCLKPeriod;
    } else {
        clock_period = mCCLKPeriod;
    }

    if (clock_period > 0) { 
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

// Description: Get reset status
bool HWCORE_MEASURE::GetResetStatus(std::string domain)
{//{{{
    if (domain == "SSCG") {
        return (mIsResetHardActive[emPRESETn] || mIsResetCmdActive[emPRESETn] || mIsResetHardActive[emCTR] || mIsResetCmdActive[emCTR] || mIsResetHardActive[emMip_prst_n] || mIsResetCmdActive[emMip_prst_n]);
    } else {
        return (mIsResetHardActive[emCTR] || mIsResetCmdActive[emCTR] || mIsResetHardActive[emMip_crst_n] || mIsResetCmdActive[emMip_crst_n]);
    }    
}//}}}


//========================================================
//============ Reset and clock  ==========================
//========================================================
// Description: Monitor PRESETn port
void HWCORE_MEASURE::HandlePRESETnSignalMethod(void)
{//{{{
    mResetCurVal[emPRESETn] = PRESETn.read();
    mResetHardEvent[emPRESETn].notify(SC_ZERO_TIME);
}//}}}

// Description: Monitor mip_crst_n port
void HWCORE_MEASURE::HandleMip_crst_nSignalMethod(void)
{//{{{
    double pmt2_cnt;
    mResetCurVal[emMip_crst_n] = mip_crst_n.read();
    if (mResetCurVal[emMip_crst_n] && ((mpHWCORE_MEASUREFunc->GetRegVal("PMT2CTR") & 0x01) == 0x01)) {
        pmt2_last_proc_time = sc_time_stamp().to_double();           // get time
        pmt2_cnt = (double)(mpHWCORE_MEASUREFunc->GetRegVal("PMT2CNT"));
        NextOverFlow(pmt2_cnt); 
    }
    mResetHardEvent[emMip_crst_n].notify(SC_ZERO_TIME);
}//}}}

// Description: Monitor mip_prst_n port
void HWCORE_MEASURE::HandleMip_prst_nSignalMethod(void)
{//{{{
    mResetCurVal[emMip_prst_n] = mip_prst_n.read();
    mResetHardEvent[emMip_prst_n].notify(SC_ZERO_TIME);
}//}}}

// Description: Monitor CTR port
void HWCORE_MEASURE::HandleCTRSignalMethod(void)
{//{{{
    mResetCurVal[emCTR] = (bool)(CTR.read() & 0x01);
    mResetHardEvent[emCTR].notify(SC_ZERO_TIME);
}//}}}

// Description: Monitor PCLK clock
// Ref: {HWCORE_MEASURE_UD_Feature_001, HWCORE_MEASURE_UD_Flow_002}
void HWCORE_MEASURE::HandlePCLKSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

// Description: Monitor CCLK clock
// Ref: {HWCORE_MEASURE_UD_Feature_001, HWCORE_MEASURE_UD_Flow_002}
void HWCORE_MEASURE::HandleCCLKSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = CCLK.read();
    SetCLKFreq("CCLK", freq_value, "Hz");
}//}}}

// Description: Process reset function when reset port is active
// Ref: {HWCORE_MEASURE_UD_ATT1_005, HWCORE_MEASURE_UD_Direction_040, HWCORE_MEASURE_UD_Direction_041, HWCORE_MEASURE_UD_Flow_001, HWCORE_MEASURE_UD_Flow_017}
void HWCORE_MEASURE::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETn) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
        } else if (reset_id == emMip_crst_n) {
            re_printf("info","The reset port mip_crst_n is asserted.\n");
            this->EnableReset("mip_crst_n", mIsResetHardActive[reset_id]);
        } else if (reset_id == emMip_prst_n) {
            re_printf("info","The reset port mip_prst_n is asserted.\n");
            this->EnableReset("mip_prst_n", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port CTR is asserted.\n");
            this->EnableReset("CTR", mIsResetHardActive[reset_id]);
        }
    } else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETn) {
            re_printf("info","The reset port PRESETn is de-asserted.\n");
            this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
        } else if (reset_id == emMip_crst_n) {
            re_printf("info","The reset port mip_crst_n is de-asserted.\n");
            this->EnableReset("mip_crst_n", mIsResetHardActive[reset_id]);
        } else if (reset_id == emMip_prst_n) {
            re_printf("info","The reset port mip_prst_n is de-asserted.\n");
            this->EnableReset("mip_prst_n", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port CTR is de-asserted.\n");
            this->EnableReset("CTR", mIsResetHardActive[reset_id]);
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

// Description: Process reset function when reset command is active
// Ref: {HWCORE_MEASURE_UD_Direction_038}
void HWCORE_MEASURE::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETn) {
        reset_name = "PRESETn";
    } else if (reset_id == emMip_crst_n) {
        reset_name = "mip_crst_n";
    } else if (reset_id == emMip_prst_n) {
        reset_name = "mip_prst_n";
    } else { //reset_id == 3
        reset_name = "CTR";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

/// Description: Cancel reset function when reset command is active
// Ref: {HWCORE_MEASURE_UD_Direction_037}
void HWCORE_MEASURE::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETn) {
        reset_name = "PRESETn";
    } else if (reset_id == emMip_crst_n) {
        reset_name = "mip_crst_n";
    } else if (reset_id == emMip_prst_n) {
        reset_name = "mip_prst_n";
    } else { //reset_id == 3
        reset_name = "CTR";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

// Description: Reset model and its sub-instance if reset is actived
void HWCORE_MEASURE::EnableReset(const std::string reset_name, const bool is_active)
{//{{{
    if (is_active) {
        // Cancel events
        // Initialize variables
        if (reset_name == "PRESETn") {
            mpHWCORE_MEASUREFunc->EnableReset(is_active);
        } else if (reset_name == "mip_prst_n") {
            CancelEvents_SSCG();
            Initialize_SSCG();
            mpHWCORE_MEASUREFunc->EnableReset(is_active);
        } else if (reset_name == "mip_crst_n") {
            CancelEvents_CLEAN();
            Initialize_CLEAN();
        } else { // reset name = CTR will reset both PCLK and CCLK domain
            CancelEvents_CLEAN();
            Initialize_CLEAN();
            CancelEvents_SSCG();
            Initialize_SSCG();
            mpHWCORE_MEASUREFunc->EnableReset(is_active);
        }
    } else {
        if ((reset_name == "PRESETn") && (!GetResetStatus("SSCG"))) {
            mpHWCORE_MEASUREFunc->EnableReset(is_active);
        }
        if ((reset_name == "mip_prst_n") && (!GetResetStatus("SSCG"))) {
            mpHWCORE_MEASUREFunc->EnableReset(is_active);
        }
        if ((reset_name == "CTR") && (!GetResetStatus("SSCG"))) {
            mpHWCORE_MEASUREFunc->EnableReset(is_active);
        }
    }
}//}}}
//========================================================
//============ Operation               ===================
//========================================================
// Description: Cancel operation events sync with CCLK clock
void HWCORE_MEASURE::CancelEvents_CLEAN(void)
{//{{{
    // Operation events are canceled
    m_pmt2_over_flowEvent.cancel();
    mAssertPmt2cp_intCLEANEvent.cancel();
    mAssertPmt2of_intCLEANEvent.cancel();
    mWritePhi_outCLEANEvent.cancel();
}//}}}
// Description: Cancel operation events sync with PCLK clock
void HWCORE_MEASURE::CancelEvents_SSCG(void)
{//{{{
    // Operation events are canceled
    mAssertPmt2cp_intSSCGEvent.cancel();
    mDeassertPmt2cp_intEvent.cancel();
    mWritePmt2cp_intEvent.cancel();
    mAssertPmt2of_intSSCGEvent.cancel();
    mDeassertPmt2of_intEvent.cancel();
    mWritePmt2of_intEvent.cancel();
    mWritePhi_outSSCGEvent.cancel();
}//}}}

// Description: Start capture when resolver angle value change
// Ref: {HWCORE_MEASURE_UD_Flow_014, HWCORE_MEASURE_UD_Feature_007}
void HWCORE_MEASURE::rdc_phi_procMethod(void)
{
    if (CheckClockPeriod("CCLK") && (!GetResetStatus("CLEAN"))) {
        if((ANGCTR.read() & 0x01) == 0x00) { /* use rdc */
            /* ANG IP */
            res_value_temp = (unsigned short)(rdc_phi.read() >> 4);

            if(res_value_temp != res_value_temp_old) {
                judge_pmt2_capture_pre(res_value_temp);
                res_value_temp_old = res_value_temp;
            
                UpdatePhi_outVal();
            }
        }    
    }    
}
// Description: Start capture by port RESTHSFT
// Ref: {HWCORE_MEASURE_UD_Flow_015, HWCORE_MEASURE_UD_Feature_007}
void HWCORE_MEASURE::RESTHSFT_procMethod(void)
{
    if (CheckClockPeriod("CCLK") && (!GetResetStatus("CLEAN"))) {
        if((ANGCTR.read() & 0x01) == 0x01) { // use EMU3nRESTHSFT
            res_value_temp = (unsigned short)(RESTHSFT.read() & 0x0FFF);
    
            if (res_value_temp != res_value_temp_old) {
                judge_pmt2_capture_pre(res_value_temp);
                res_value_temp_old = res_value_temp;
                UpdatePhi_outVal();
            }
        }
    }
}

// Description: Start capture when Z index signal change
// Ref: {HWCORE_MEASURE_UD_Registers_005, HWCORE_MEASURE_UD_Flow_016, HWCORE_MEASURE_UD_Feature_007}
void HWCORE_MEASURE::rdc_z_pos_procMethod(void)
{
    if (CheckClockPeriod("CCLK") && (!GetResetStatus("CLEAN"))) {
        if((ANGCTR.read() & 0x01) == 0x00) {
            if ((mpHWCORE_MEASUREFunc->GetRegVal("PMT2INVL", "DATA") & 0x1F) == 0x1F) {
                pmt2_capture();
            }
        }
    }    
}

// Description: Start counter then issue interrupt when counter over flow
// Ref: {HWCORE_MEASURE_UD_Flow_007, HWCORE_MEASURE_UD_Feature_006}
void HWCORE_MEASURE::pmt2_over_flowMethod(void)
{
    pmt2_last_proc_time = sc_time_stamp().to_double();   /* get time */
    mpHWCORE_MEASUREFunc->SetRegVal("PMT2CNT", 0x00);                   /* count reset */

    NextOverFlow(0); 
    double current_time = sc_time_stamp().to_double();
    mAssertPmt2of_intCLEANEvent.notify(CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mStartCCLKTime), mTimeResolutionUnit);
}

// Description: Capture counter value from PMT2CNT reg then issue capture interruption
// Ref: {HWCORE_MEASURE_UD_ATT1_007, HWCORE_MEASURE_UD_Registers_004, HWCORE_MEASURE_UD_Flow_006, HWCORE_MEASURE_UD_Feature_009}
void HWCORE_MEASURE::pmt2_capture(void)
{
    callback_PMT2CNT_DATA_read();
   
    mpHWCORE_MEASUREFunc->SetRegVal("PMT2CAP", mpHWCORE_MEASUREFunc->GetRegVal("PMT2CNT"));

    mpHWCORE_MEASUREFunc->SetRegVal("PMT2CNT", 0x00);      /* clear count */
   
    m_pmt2_over_flowEvent.cancel();                          /* over flow event delete */
   
    if (mpHWCORE_MEASUREFunc->GetRegVal("PMT2CTR", "EN") == 0x01) {
        NextOverFlow(0); 
    }
   
    pmt2_last_proc_time = sc_time_stamp().to_double();   /* get time */
    mAssertPmt2cp_intCLEANEvent.notify(CalculateCLKPosEdge((double)mCCLKPeriod, false, pmt2_last_proc_time, mStartCCLKTime), mTimeResolutionUnit);
}

// Description: Obtain the resolver angle data from RDC
// Ref: {HWCORE_MEASURE_UD_ATT1_007, HWCORE_MEASURE_UD_Registers_005, HWCORE_MEASURE_UD_Feature_008}
void HWCORE_MEASURE::judge_pmt2_capture(unsigned short pmt2_now_angle)
{
    signed long dif_angle = 0;
    bool now_rotation_pos = false;
    bool compare_flag = false;
    unsigned long capture_angle = 0x0000;
    unsigned char pmt2invl = 0;
    unsigned short angle_big = 0;
    unsigned short angle_small = 0;

    dif_angle = pmt2_now_angle - pmt2_past_angle;
   
    if(dif_angle >= 0)                                        /* angle is positive */
    {
        /* positive rotation */
        if(dif_angle <= 2048)                                /* dif_angle smaller half rotation(2048=0x800) */
        {
            angle_big = pmt2_now_angle;                        /* angle set */
            angle_small = pmt2_past_angle;                        /* angle set */
            compare_flag = false;                            /* compare flag set (temporary) */
            now_rotation_pos = true;                            /* save rotation direction */
        }

        /* negative rotation */
        else
        {
            compare_flag = true;                            /* compare flag set */
            now_rotation_pos = false;                        /* save rotation direction */
        }
    }
    else                                                /* angle is negative */
    {
        /* negative rotation */
        if(dif_angle >= -2048)                                /* dif_angle smaller half rotation(2048=0x800) */
        {
            angle_big = pmt2_past_angle;                        /* angle set */
            angle_small = pmt2_now_angle;                        /* angle set */
            compare_flag = false;                            /* compare flag set (temporary) */
            now_rotation_pos = false;                        /* save rotation direction */
        }
        /* positive rotation */
        else
        {
            compare_flag = true;                            /* compare flag set */
            now_rotation_pos = true;                            /* save rotation direction */
        }
    }
   
    if(compare_flag == false)
    {
        pmt2invl = mpHWCORE_MEASUREFunc->GetRegVal("PMT2INVL");

        switch(pmt2invl)        /* use angle for PMT2 capture */
        {
        case 0x00:
            compare_flag = true;
            break;

        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x10:
            while(1)            /* find compare value */
            {
                if(capture_angle > angle_small)
                {
                    break;
                }
                capture_angle += pmt2_invl_div[pmt2invl];
            }

            /* judge compare */
            if((capture_angle <= angle_big) && (capture_angle > angle_small))
            {
                compare_flag = true;
            }
            else
            {
                compare_flag = false;
            }
            break;

        default:
            ;
            break;
        }
    }

    if(compare_flag == true)
    {
        if(now_rotation_pos == g_capture_rotation_pos)        /* same direction */
        {
            g_pmt2_compare_cnt++;
            if(g_pmt2_compare_cnt >= 2)                        /* privent overflow */
            {
                g_pmt2_compare_cnt = 2;
            }
        }
        else                                                /* different direction */
        {
            g_pmt2_compare_cnt = 1;
        }

        if(g_pmt2_compare_cnt >= 2)
        {
            pmt2_capture();
        }

        /* PMT2CNT register clear */
        mpHWCORE_MEASUREFunc->SetRegVal("PMT2CNT", 0x00);
        callback_PMT2CNT_DATA_write();                                /* call CB function to set event again */

        g_capture_rotation_pos = now_rotation_pos;                    /* save rotation direction when compare */

    }
    pmt2_past_angle = pmt2_now_angle;                            /* save angle */
}

// Description: Decide the capture time depend on setting in PMT2INVL
// Ref: {HWCORE_MEASURE_UD_ATT1_007, HWCORE_MEASURE_UD_Flow_008}
void HWCORE_MEASURE::judge_pmt2_capture_pre(unsigned short res_value)
{
    unsigned char pmt2invl = 0;

    pmt2invl = mpHWCORE_MEASUREFunc->GetRegVal("PMT2INVL");

    switch(pmt2invl)                        /* use angle for PMT2 capture */
    {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x08:
    case 0x09:
    case 0x0A:
    case 0x0B:
    case 0x10:
        judge_pmt2_capture(res_value);
        break;

     default: //case 0x1F z phase pulse
        ;
        break;
    }
}

// Notify event to update phi_out value
void HWCORE_MEASURE::UpdatePhi_outVal(void)
{//{{{
    mPhi_out = res_value_temp;
    double current_time = sc_time_stamp().to_double();
    mWritePhi_outCLEANEvent.notify(CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mStartCCLKTime), mTimeResolutionUnit);
}//}}}

// Calculate next over flow time
void HWCORE_MEASURE::NextOverFlow(double pmt2cnt)
{//{{{
    double next_over_flow;
    double current_time = sc_time_stamp().to_double();
    double pmt2_start = CalculateCLKPosEdge((double)mCCLKPeriod, false, current_time, mStartCCLKTime) ;
    next_over_flow = ((((double)((unsigned long)(PMT2_CNT_MAX))) + 1) - pmt2cnt) * mCCLKPeriod + pmt2_start;
    m_pmt2_over_flowEvent.notify((double)(next_over_flow), mTimeResolutionUnit);          // over flow event set
}//}}}

// Description: pmt2cp interrupt sync to PCLK clock
// Ref: {HWCORE_MEASURE_UD_TimingChart_001}
void HWCORE_MEASURE::AssertPmt2cp_intCLEANMethod(void)
{
    double current_time = sc_time_stamp().to_double();
    mAssertPmt2cp_intSSCGEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Assert pmt2cp_mip_clk interrupt
void HWCORE_MEASURE::AssertPmt2cp_intSSCGMethod(void)
{//{{{
    mPmt2cp_int = true;
    if (mDumpInterrupt) {
        re_printf("info","INT [pmt2cp] Assert.\n");
    }
    mWritePmt2cp_intEvent.notify(SC_ZERO_TIME);
    mDeassertPmt2cp_intEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
}//}}}

// De-assert pmt2cp_mip_clk interrupt
void HWCORE_MEASURE::DeassertPmt2cp_intMethod (void)
{//{{{
    mPmt2cp_int = false;
    if (mDumpInterrupt) {
        re_printf("info","INT [pmt2cp] De-assert.\n");
    }
    mWritePmt2cp_intEvent.notify(SC_ZERO_TIME);
}//}}}

/// Write to pmt2cp_mip_clk interrupt port
void HWCORE_MEASURE::WritePmt2cp_intMethod (void)
{//{{{
    if (CheckClockPeriod("PCLK")) {
        pmt2cp.write(mPmt2cp_int);
    }
}//}}}

// Description: pmt2of interrupt sync to PCLK clock
// Ref: {HWCORE_MEASURE_UD_TimingChart_001}
void HWCORE_MEASURE::AssertPmt2of_intCLEANMethod(void)
{
    double current_time = sc_time_stamp().to_double();
    mAssertPmt2of_intSSCGEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
}
// Assert pmt2of_mip_clk interrupt
void HWCORE_MEASURE::AssertPmt2of_intSSCGMethod(void)
{//{{{
    mPmt2of_int = true;
    if (mDumpInterrupt) {
        re_printf("info","INT [pmt2of] Assert.\n");
    }
    mWritePmt2of_intEvent.notify(SC_ZERO_TIME);
    mDeassertPmt2of_intEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
}//}}}

// De-assert pmt2cp_mip_clk interrupt
void HWCORE_MEASURE::DeassertPmt2of_intMethod (void)
{//{{{
    mPmt2of_int = false;
    if (mDumpInterrupt) {
        re_printf("info","INT [pmt2of] De-assert.\n");
    }
    mWritePmt2of_intEvent.notify(SC_ZERO_TIME);
}//}}}

/// Write to pmt2cp_mip_clk interrupt port
void HWCORE_MEASURE::WritePmt2of_intMethod (void)
{//{{{
    if (CheckClockPeriod("PCLK")) {
        pmt2of.write(mPmt2of_int);
    }
}//}}}

// Description: phi_out sync to PCLK clock
// Ref: {HWCORE_MEASURE_UD_TimingChart_001}
void HWCORE_MEASURE::WritePhi_outCLEANMethod (void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mWritePhi_outSSCGEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
}//}}}
/// Write to phi_out_mip_clk port
void HWCORE_MEASURE::WritePhi_outSSCGMethod (void)
{//{{{
    if (CheckClockPeriod("PCLK")) {
        phi_out.write(mPhi_out);
    }
}//}}}

// Description: Counter start/stop 
// Ref: {HWCORE_MEASURE_UD_Feature_002}
void HWCORE_MEASURE::callback_PMT2CTR_EN(void)
{
    double pmt2_cnt;
    double pass_time;

    if (CheckClockPeriod("CCLK") && (!GetResetStatus("CLEAN"))) {
        if (((mpHWCORE_MEASUREFunc->GetRegVal("PMT2CTR") & 0x01) == 0x01)
                && (((mpHWCORE_MEASUREFunc->GetCTRPreVal()) & 0x01) == 0x00))    // count start
        {
            pmt2_last_proc_time = sc_time_stamp().to_double();           // get time
            pmt2_cnt = (double)(mpHWCORE_MEASUREFunc->GetRegVal("PMT2CNT"));
            NextOverFlow(pmt2_cnt); 
        }   
        else if (((mpHWCORE_MEASUREFunc->GetRegVal("PMT2CTR") & 0x01) == 0x00)
                && (((mpHWCORE_MEASUREFunc->GetCTRPreVal()) & 0x01) == 0x01))    // count stop
        {
            m_pmt2_over_flowEvent.cancel();                       // over flow event cancel
            pmt2_cnt = mpHWCORE_MEASUREFunc->GetRegVal("PMT2CNT");
            pass_time = (sc_time_stamp().to_double() - pmt2_last_proc_time);

            pmt2_cnt += pass_time / mCCLKPeriod;

            mpHWCORE_MEASUREFunc->SetRegVal("PMT2CNT", (unsigned int)pmt2_cnt);       // store value capture register
            pmt2_last_proc_time = sc_time_stamp().to_double();           // get time
        }
    }
}

void HWCORE_MEASURE::callback_PMT2SFT_SCAPTRG(void)
{
    if (CheckClockPeriod("CCLK") && (!GetResetStatus("CLEAN"))) {
        if (mpHWCORE_MEASUREFunc->GetRegVal("PMT2SFT") == 0x01) {
            pmt2_capture();
        }
        mpHWCORE_MEASUREFunc->SetRegVal("PMT2SFT", (unsigned int)(mpHWCORE_MEASUREFunc->GetRegVal("PMT2SFT") & 0xFE));
    }
}

// Description: Counting operation and writing
// Ref: {HWCORE_MEASURE_UD_Feature_003, HWCORE_MEASURE_UD_Feature_005}
void HWCORE_MEASURE::callback_PMT2CNT_DATA_write(void)
{
    double pmt2_cnt;

    if (CheckClockPeriod("CCLK") && (!GetResetStatus("CLEAN"))) {
        if ((mpHWCORE_MEASUREFunc->GetRegVal("PMT2CTR") & 0x01) == 0x01) {
            pmt2_last_proc_time = sc_time_stamp().to_double();           // get time
            pmt2_cnt = mpHWCORE_MEASUREFunc->GetRegVal("PMT2CNT");
            NextOverFlow(pmt2_cnt); 
        }
    }
}

// Description: Counting operation and reading
// Ref: {HWCORE_MEASURE_UD_Feature_003, HWCORE_MEASURE_UD_Feature_004}
void HWCORE_MEASURE::callback_PMT2CNT_DATA_read(void)
{
    double pmt2_cnt;
    double pass_time;

    if (CheckClockPeriod("CCLK") && (!GetResetStatus("CLEAN"))) {
        if ((mpHWCORE_MEASUREFunc->GetRegVal("PMT2CTR") & 0x01) == 0x01) {
            pmt2_cnt = (double)mpHWCORE_MEASUREFunc->GetRegVal("PMT2CNT");
            pass_time = ((double)sc_time_stamp().value()) - pmt2_last_proc_time;

            pmt2_cnt += pass_time / mCCLKPeriod;

            mpHWCORE_MEASUREFunc->SetRegVal("PMT2CNT", (unsigned int)pmt2_cnt);

            pmt2_last_proc_time = sc_time_stamp().to_double();           // get time
        }
    }
}

//========================================================
//============ PYTHON IF               ===================
//========================================================
// Description: Set message level (fatal, error, warning, info)
void HWCORE_MEASURE::SetMessageLevel(const std::string msg_lv)
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
    mpHWCORE_MEASUREFunc->RegisterHandler(cmd);

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
    mpHWCORE_MEASUREFunc->RegisterHandler(cmd);
}//}}}

// Description: Enable/disable dumping message when users access registers
void HWCORE_MEASURE::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_MEASUREFunc->RegisterHandler(cmd);
}//}}}

// Description: Enable/disable info/warning/error message of register IF
// Ref: {HWCORE_MEASURE_UD_Direction_032, HWCORE_MEASURE_UD_Direction_043}
void HWCORE_MEASURE::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }else if(word_vector[0] == "false"){
            mEnableRegisterMessage = false;
        }else{
            re_printf("warning", "Invalid argument: HWCORE_MEASURE_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }else if(word_vector.size() == 0){
        std::string temp = "";
        if(mEnableRegisterMessage){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info", "HWCORE_MEASURE_EnableRegisterMessage %s\n", temp.c_str());
    }else{
        re_printf("warning", "Invalid argument: HWCORE_MEASURE_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///Enable/disable dumping interrupt information
void HWCORE_MEASURE::DumpInterrupt(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_MEASURE_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","HWCORE_MEASURE_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HWCORE_MEASURE_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

// Description: Dump status register's value
void HWCORE_MEASURE::DumpStatInfo(void)
{//{{{
    mpHWCORE_MEASUREFunc->DumpStatInfo();
}//}}}

// Description: Command to assert reset
// Ref: {HWCORE_MEASURE_UD_Direction_039, HWCORE_MEASURE_UD_Direction_034, HWCORE_MEASURE_UD_Direction_033}
void HWCORE_MEASURE::AssertReset (const std::string reset_name, const double start_time, const double period)
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
    } else if (reset_name == "mip_crst_n") {
        if ((!mIsResetHardActive[1])&&(!mIsResetCmdReceive[1])) {
            mIsResetCmdReceive[1] = true;
            re_printf("info","The model will be reset (mip_crst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[1].notify(start_time, SC_NS);
            mResetCmdPeriod[1] = period;
        } else {
            re_printf("warning","The software reset of mip_crst_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "mip_prst_n") {
        if ((!mIsResetHardActive[2])&&(!mIsResetCmdReceive[2])) {
            mIsResetCmdReceive[2] = true;
            re_printf("info","The model will be reset (mip_prst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[2].notify(start_time, SC_NS);
            mResetCmdPeriod[2] = period;
        } else {
            re_printf("warning","The software reset of mip_prst_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "CTR") {
        if ((!mIsResetHardActive[3])&&(!mIsResetCmdReceive[3])) {
            mIsResetCmdReceive[3] = true;
            re_printf("info","The model will be reset (CTR) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[3].notify(start_time, SC_NS);
            mResetCmdPeriod[3] = period;
        } else {
            re_printf("warning","The software reset of CTR is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn or mip_crst_n or mip_prst_n or CTR\n",reset_name.c_str());
    }
}//}}}

// Description: Set clock value and clock unit
// Ref: {HWCORE_MEASURE_UD_Direction_031}
void HWCORE_MEASURE::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    double pmt2_cnt;
    double pass_time;
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq(mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1 / (double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
            }else{
                mPCLKPeriod = 0;
                // Prepare for next run
                CancelEvents_SSCG();
            }
        } else if (word_vector[0] == "CCLK") {
            mCCLKOrgFreq = freq;
            ConvertClockFreq(mCCLKFreq, mCCLKFreqUnit, freq, unit);
            if (mCCLKFreq > 0) {
                mCCLKPeriod = (sc_dt::uint64)(((1 / (double)mCCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartCCLKTime = sc_time_stamp().to_double();
                pmt2_last_proc_time = sc_time_stamp().to_double() - pmt2_stop_time;           // get time
            }else{
                mCCLKPeriod = 0;
                // Prepare for next run
                CancelEvents_CLEAN();
                m_pmt2_over_flowEvent.cancel();                       // over flow event cancel
                pmt2_cnt = mpHWCORE_MEASUREFunc->GetRegVal("PMT2CNT");
                pass_time = (sc_time_stamp().to_double() - pmt2_last_proc_time);

                pmt2_cnt += pass_time / mCCLKPeriod;

                mpHWCORE_MEASUREFunc->SetRegVal("PMT2CNT", (unsigned int)pmt2_cnt);       // store value capture register
                pmt2_stop_time = pass_time;           // get time
            }
        }else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

// Description: Get clock value
// Ref: {HWCORE_MEASURE_UD_Direction_042, HWCORE_MEASURE_UD_Direction_031}
void HWCORE_MEASURE::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info", "PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "CCLK") {
            re_printf("info", "CCLK frequency is %0.0f %s\n", (double)mCCLKOrgFreq, mCCLKFreqUnit.c_str());
        }else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

// Description: Force value to register
// Ref: {HWCORE_MEASURE_UD_Direction_028}
void HWCORE_MEASURE::ForceRegister(const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_MEASUREFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Release forced value after forcing registers
// Ref: {HWCORE_MEASURE_UD_Direction_028}
void HWCORE_MEASURE::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHWCORE_MEASUREFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Write value to registers by software
// Ref: {HWCORE_MEASURE_UD_Direction_028}
void HWCORE_MEASURE::WriteRegister(const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_MEASUREFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: Read value of register by software
// Ref: {HWCORE_MEASURE_UD_Direction_028}
void HWCORE_MEASURE::ReadRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpHWCORE_MEASUREFunc->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

// Description: List all registers name
// Ref: {HWCORE_MEASURE_UD_Direction_013}
void HWCORE_MEASURE::ListRegister(void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHWCORE_MEASUREFunc->RegisterHandler(cmd);
}//}}}

// Description: Dump help message of all parameters or commands
// Ref: {HWCORE_MEASURE_UD_Direction_014, HWCORE_MEASURE_UD_Direction_015, HWCORE_MEASURE_UD_Direction_016, HWCORE_MEASURE_UD_Direction_027}
void HWCORE_MEASURE::Help(const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_MessageLevel (\"HWCORE_MEASURE instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_DumpRegisterRW (\"HWCORE_MEASURE instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_EnableRegisterMessage (\"HWCORE_MEASURE instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_DumpInterrupt (\"HWCORE_MEASURE instance\", \"true/false\")                     Enable/disable dumping interrupt information (Default: false)");
        }else if (word_vector[0] == "commands"){
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_DumpStatInfo (\"HWCORE_MEASURE instance\")                                    Dump the status info of model");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_AssertReset (\"HWCORE_MEASURE instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the HWCORE_MEASURE model");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_SetCLKFreq (\"HWCORE_MEASURE instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to HWCORE_MEASURE model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_GetCLKFreq (\"HWCORE_MEASURE instance\", \"clock_name\")                        Get clock frequency value of HWCORE_MEASURE model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_ForceRegister (\"HWCORE_MEASURE instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_ReleaseRegister (\"HWCORE_MEASURE instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_WriteRegister (\"HWCORE_MEASURE instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_ReadRegister (\"HWCORE_MEASURE instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_MEASURE_ListRegister (\"HWCORE_MEASURE instance\")                                      Dump name of all registers");
        }else{
            re_printf("warning", "The name (%s) of HWCORE_MEASURE_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    }else{
        re_printf("warning", "The name (%s) of HWCORE_MEASURE_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}
