// ---------------------------------------------------------------------
// $Id: HWCORE_RCTG.cpp,v 1.7 2020/11/09 08:01:40 quanganhtran Exp $
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
// Description: Copyright of HWCORE_RCTG.cpp
// Ref: {HWCORE_RCTG_UD_Copyright_001}
// Description: About the include header files of HWCORE_RCTG.cpp
// Ref: {HWCORE_RCTG_UD_ATT1_002, HWCORE_RCTG_UD_Direction_028}
#include "re_define.h"
#include "HWCORE_RCTG.h"
#include "HWCORE_RCTG_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
///*********************************
/// Function     : HWCORE_RCTG 
/// Description  : Constructor of HWCORE_RCTG class
/// Parameters   : 
///                sc_module_name name: name of model
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Summary_001, HWCORE_RCTG_UD_Summary_002, HWCORE_RCTG_UD_ATT1_004, HWCORE_RCTG_UD_Direction_038, HWCORE_RCTG_UD_Direction_039, HWCORE_RCTG_UD_ATT1_009}
// Ref: {HWCORE_RCTG_UD_Direction_030}
HWCORE_RCTG::HWCORE_RCTG(sc_module_name name)
    : sc_module(name),
    HWCORE_RCTG_AgentController(),
    CRWInterface(),
    PCLK("PCLK"),
    PRESETn("PRESETn"),
    rst_n("rst_n"),
    //Input ports
    ang_comp0_eq_sp_80m("ang_comp0_eq_sp_80m"),
    output_rctg_start_sp("output_rctg_start_sp"),
    //Output ports
    rdata_em2iipcmp0("rdata_em2iipcmp0"),
    em2icmp0("em2icmp0"),
    em2icmp1("em2icmp1"),
    em2ibrecctr_slctcmp0_reg("em2ibrecctr_slctcmp0_reg"),
    rctg_u_80m_reg("rctg_u_80m_reg"),
    rctg_v_80m_reg("rctg_v_80m_reg"),
    rctg_w_80m_reg("rctg_w_80m_reg"),
    //Output ports
    output_rctg_end_sp_reg("output_rctg_end_sp_reg"),
    wc_em2icmp0("wc_em2icmp0"),
    wc_em2icmp1("wc_em2icmp1")
{//{{{ 
    mpHWCORE_RCTG_Func = new HWCORE_RCTG_Func((std::string)name, this);
    if (mpHWCORE_RCTG_Func == NULL) {
        return;
    }

    // Initialize clock/reset variables and parameters
    mPCLKPeriod =           0;
    mPCLKOrgFreq =          0;
    mPCLKFreq =             0;
    mPCLKFreqUnit =         "Hz";
    mStartClkTime =         0;
    for (int i = 0; i < emNumReset; i++) {
        mResetCurVal[i] = true;
        mResetPreVal[i] = true;
        mIsResetHardActive[i] = false;
        mIsResetCmdReceive[i] = false;
        mIsResetCmdActive[i] = false;
        mResetCmdPeriod[i] = 0;
    }
    mTimeResolutionValue =  1;
    mTimeResolutionUnit =   SC_NS;
    mEnableRegisterMessage = true;
    mDumpInterrupt = true;
    mProcessingTime = DEFAULT_PROC_TIME;
    mDumpStatInfo = false;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    //Create map for interrupt ports
    mMapIntr[emOUTPUT_RCTG_END_SP_REG] = &output_rctg_end_sp_reg;
    mMapIntr[emWC_EM2ICMP0] = &wc_em2icmp0;
    mMapIntr[emWC_EM2ICMP1] = &wc_em2icmp1;

    //Create map for unsigned short output ports
    mMapOut_UShort[emRDATA_EM2IICMP0] = &rdata_em2iipcmp0;
    mMapOut_UShort[emEM2ICMP0] = &em2icmp0;
    mMapOut_UShort[emEM2ICMP1] = &em2icmp1;
    //Create map for bool output ports
    mMapOut_Bool[emEM2IBRECCTR_SCLTCMP0_REG] = &em2ibrecctr_slctcmp0_reg;
    mMapOut_Bool[emRCTG_U_80M_REG] = &rctg_u_80m_reg;
    mMapOut_Bool[emRCTG_V_80M_REG] = &rctg_v_80m_reg;
    mMapOut_Bool[emRCTG_W_80M_REG] = &rctg_w_80m_reg;

    // Methods for monitoring Clock port
    SC_METHOD(HandlePCLKSignalMethod);
    sensitive << PCLK;

    // Methods for monitoring PRESETn port
    SC_METHOD(HandleResetSignalMethod_PRESETn);
    sensitive << PRESETn;

    // Methods for monitoring rst_n port
    SC_METHOD(HandleResetSignalMethod_rst_n);
    sensitive << rst_n;

    // Methods for handling hard reset events
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RCTG::HandleResetHardMethod, this, reset_id),
            sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    // Methods for handling command reset events
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RCTG::HandleResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    // Methods for canceling command reset events
    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RCTG::CancelResetCmdMethod, this, reset_id),
            sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    // Methods to process after PCLK is changed
    SC_METHOD(ProcessAfterPCLKChangeMethod);
    dont_initialize();
    sensitive << mProcessAfterPCLKChangeEvent;

    // Methods for monitoring input trigger ports
    SC_METHOD(HandleRectIPTriggerPortMethod);
    dont_initialize();
    sensitive << output_rctg_start_sp;

    SC_METHOD(HandleBRectIPTriggerPortMethod);
    dont_initialize();
    sensitive << ang_comp0_eq_sp_80m;

    // Methods for triggering rectangle IP and batch rectangle IP
    SC_METHOD(TriggerRectIPMethod);
    dont_initialize();
    sensitive << mTriggerRectIPEvent;

    SC_METHOD(TriggerBRectIPMethod);
    dont_initialize();
    sensitive << mTriggerBRectIPEvent;

    // Methods for handling process after rectangle IP triggered
    SC_METHOD(RectangleIPProcessMethod);
    dont_initialize();
    sensitive << mProcessRectangleIPEvent;

    // Methods for handling process after batch rectangle IP triggered
    SC_METHOD(BRectangleIPProcessMethod);
    dont_initialize();
    sensitive << mProcessBatchRectangleIPEvent;

    // Methods for assert/de-assert interrupt ports
    for (unsigned int int_idx = 0; int_idx < emINTERRUPT_NUM ; int_idx++){
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssert_DeassertInterruptEvent[int_idx]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RCTG::Assert_DeassertInterruptMethod, this, int_idx),
        sc_core::sc_gen_unique_name("Assert_DeassertInterruptMethod"), &opt);
    }

    // Methods for update output ports
    for (unsigned int out_idx = 0; out_idx < emOUTPUT_PORTS_NUM; out_idx++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mUpdateOutputEvent[out_idx]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_RCTG::UpdateOutputMethod, this, out_idx),
            sc_core::sc_gen_unique_name("UpdateOutputMethod"), &opt);
    }

    // Initialize output port
    rdata_em2iipcmp0.initialize(0x0);
    output_rctg_end_sp_reg.initialize(false);
    em2icmp0.initialize(0x0);
    wc_em2icmp0.initialize(false);
    em2icmp1.initialize(0x0);
    wc_em2icmp1.initialize(false);
    em2ibrecctr_slctcmp0_reg.initialize(false);
    rctg_u_80m_reg.initialize(false);
    rctg_v_80m_reg.initialize(false);
    rctg_w_80m_reg.initialize(false);

    // Initialize member variables
    Initialize();
}//}}}

///*********************************
/// Function     : ~HWCORE_RCTG 
/// Description  : Destructor of HWCORE_RCTG class
/// Parameters   : None
/// Return value : None
///*********************************
HWCORE_RCTG::~HWCORE_RCTG()
{//{{{
    delete mpHWCORE_RCTG_Func;
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize member variables when reset is activated
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG::Initialize(void)
{//{{{
    // Initialize member variables
    mCurRectIPState = emRCTG_IDLE;
    for(int out_idx = 0; out_idx < emOUTPUT_PORTS_NUM; out_idx++){
        mOutputValue[out_idx] = 0;
    }
}//}}}

///*********************************
/// Function     : CancelEvents
/// Description  : Cancel operation events when clock is zero or reset is asserted
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG::CancelEvents(void)
{//{{{
    // Operation events are canceled
    for(int idx = 0; idx < emINTERRUPT_NUM; idx++){
        mAssert_DeassertInterruptEvent[idx].cancel();
    }
    for(int idx = 0; idx < emOUTPUT_PORTS_NUM; idx++){
        mUpdateOutputEvent[idx].cancel();
    }
    mProcessRectangleIPEvent.cancel();
    mProcessBatchRectangleIPEvent.cancel();
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
///*********************************
/// Function     : HandleResetSignalMethod_PRESETn
/// Description  : Monitor PRESETn reset port
/// Parameters   : None
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Flow_001}
void HWCORE_RCTG::HandleResetSignalMethod_PRESETn(void)
{//{{{
    mResetCurVal[emPRESETnId] = PRESETn.read();
    if (mResetCurVal[emPRESETnId] != mResetPreVal[emPRESETnId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[emPRESETnId] = mResetCurVal[emPRESETnId];
        // Reset signals are asynchronous
        mResetHardEvent[emPRESETnId].notify(SC_ZERO_TIME);
    }
}//}}}

///*********************************
/// Function     : HandleResetSignalMethod_rst_n
/// Description  : Monitor rst_n reset port
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG::HandleResetSignalMethod_rst_n(void)
{//{{{
    mResetCurVal[emrst_nId] = rst_n.read();
    mResetPreVal[emrst_nId] = mResetCurVal[emrst_nId];
    // Reset signals are asynchronous
    mResetHardEvent[emrst_nId].notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : HandlePCLKSignalMethod
/// Description  : Monitor PCLK clock port
/// Parameters   : None
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Feature_001, HWCORE_RCTG_UD_Flow_002}
void HWCORE_RCTG::HandlePCLKSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleResetHardMethod
/// Description  : Process reset function when reset port is active
/// Parameters   : 
///                const unsigned int reset_id: reset id (0: PRESETn reset port | 1: rst_n reset port)
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_047, HWCORE_RCTG_UD_Direction_048, HWCORE_RCTG_UD_Flow_017}
void HWCORE_RCTG::HandleResetHardMethod(const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info", "The reset port PRESETn is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
        else {
            re_printf("info", "The reset port rst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
    }
    else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) {
            re_printf("info", "The reset port PRESETn is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id]);
        }
        else {
            re_printf("info", "The reset port rst_n is de-asserted.\n");
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
/// Parameters   : 
///                const unsigned int reset_id: reset id (0: PRESETn reset port | 1: rst_n reset port)
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_045}
void HWCORE_RCTG::HandleResetCmdMethod(const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    }
    else { //reset_id == emrst_nId
        reset_name = "rst_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info", "The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///*********************************
/// Function     : CancelResetCmdMethod
/// Description  : Cancel reset function when reset command is active
/// Parameters   : 
///                const unsigned int reset_id: reset id (0: PRESETn reset port | 1: rst_n reset port)
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_044}
void HWCORE_RCTG::CancelResetCmdMethod(const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    }
    else {
        reset_name = "rst_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info", "Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id]);
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Reset model and its sub-instance if reset is actived
/// Parameters   : 
///                const bool is_active (0: de-assert reset | 1: assert reset)
/// Return value : None
///*********************************
void HWCORE_RCTG::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        if (GetResetStatus("rst_n") == true){
            // Cancel events
            CancelEvents();
            // Initialize member variables
            Initialize();
            // Write output ports zero
            for (int idx = 0; idx < emOUTPUT_PORTS_NUM; idx++) {
                mUpdateOutputEvent[idx].notify(SC_ZERO_TIME);
            }
            //Clear interrupts
            for (int idx = 0; idx < emINTERRUPT_NUM; idx++) {
                //Already low, do not proceed
                
                if (mMapIntr[idx]->read() == false) {
                    continue;
                }
                //Proceed to clear when interrupts are currently high
                mAssert_DeassertInterruptEvent[idx].notify(SC_ZERO_TIME);
            }
        }
        //Only reset port related to registers
        else{
            mOutputValue[emEM2ICMP0] = 0;
            mOutputValue[emEM2ICMP1] = 0;
            mOutputValue[emEM2IBRECCTR_SCLTCMP0_REG] = 0;            
            // Write output ports zero
            mUpdateOutputEvent[emEM2ICMP0].notify(SC_ZERO_TIME);
            mUpdateOutputEvent[emEM2ICMP1].notify(SC_ZERO_TIME);
            mUpdateOutputEvent[emEM2IBRECCTR_SCLTCMP0_REG].notify(SC_ZERO_TIME);                     
        }
    }
    mpHWCORE_RCTG_Func->EnableReset(is_active);
}//}}}

//========================================================
//============ Input triggers   ==========================
//========================================================
///*********************************
/// Function     : HandleRectIPTriggerPortMethod
/// Description  : handle changes of port output_rctg_start_sp
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG::HandleRectIPTriggerPortMethod() {
    bool cur_val = false;

    cur_val = (bool)output_rctg_start_sp.read();

    // Port changed from 1 to 0 (falling edge)
    if (cur_val == 0) {
        return;
    }
    mTriggerRectIPEvent.notify(SC_ZERO_TIME);
}

///*********************************
/// Function     : HandleBRectIPTriggerPortMethod
/// Description  : handle changes of port ang_comp0_eq_sp_80m
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG::HandleBRectIPTriggerPortMethod() {
    bool cur_val = false;

    cur_val = (bool)ang_comp0_eq_sp_80m.read();

    // Port changed from 1 to 0 (falling edge)
    if (cur_val == 0) {
        return;
    }
    mTriggerBRectIPEvent.notify(SC_ZERO_TIME);
}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
///*********************************
/// Function     : CheckClockPeriod
/// Description  : Check clock period
/// Parameters   : None
/// Return value : bool ret: check result (true: clock > 0 | false: clock <=0)
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_043}
bool HWCORE_RCTG::CheckClockPeriod(void)
{//{{{
    bool clock_status = false;
    if (mPCLKFreq > 0) {
        clock_status = true;
    }
    else {
        re_printf("info", "PCLK frequency is zero\n");
    }
    return clock_status;
}//}}}

///*********************************
/// Function     : GetResetStatus
/// Description  : Get reset status
/// Parameters   :
///                const std::string reset_name: name of reset to be status checked.
/// Return value : 
///                bool ret: check result (true: reset is asserted | false: reset is not asserted)
///*********************************
bool HWCORE_RCTG::GetResetStatus(const std::string reset_name)
{//{{{
    bool reset_status = false;
    if (reset_name == "PRESETn") {
        if (mIsResetHardActive[emPRESETnId] || mIsResetCmdActive[emPRESETnId]) {
            reset_status = true;
        }
    }
    else if (reset_name == "rst_n") {
        if (mIsResetHardActive[emrst_nId] || mIsResetCmdActive[emrst_nId]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///*********************************
/// Function     : NotifyAssert_DeassertInterrupt
/// Description  : method for receive notification to assert/de-assert interrupt from function block 
/// Parameters   : 
///                int InterruptIndex: index of interrupts
/// Return value : None
///*********************************
void HWCORE_RCTG::NotifyAssert_DeassertInterrupt(unsigned int InterruptIndex)
{//{{{
    double current_time = sc_time_stamp().to_double();
    mAssert_DeassertInterruptEvent[InterruptIndex].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartClkTime), mTimeResolutionUnit);
}//}}}

 ///*********************************
 /// Function     : NotifyUpdateOutputPorts
 /// Description  : method for receive notification to update outputs from function block 
 /// Parameters   : 
 ///                int OutputIndex    : index of output
 ///                unsigned int value : value to be update to specified output port
 /// Return value : None
 ///*********************************
void HWCORE_RCTG::NotifyUpdateOutputPorts(unsigned int OutputIndex, unsigned int value)
{//{{{
    mOutputValue[OutputIndex] = value;
    mUpdateOutputEvent[OutputIndex].notify(SC_ZERO_TIME);
}//}}}

 ///*********************************
 /// Function     : ProcessAfterPCLKChangeMethod
 /// Description  : method for receive notification to update outputs from function block 
 /// Parameters   : 
 ///                int OutputIndex    : index of output
 ///                unsigned int value : value to be update to specified output port
 /// Return value : None
 ///*********************************
void HWCORE_RCTG::ProcessAfterPCLKChangeMethod()
{//{{{
    double current_time = sc_time_stamp().to_double();

    // Clear status flag to idle, wait for another trigger
    mCurRectIPState = emRCTG_IDLE;

    // Interrupt is raise but not pull low
    for (int idx = 0; idx < emINTERRUPT_NUM; idx++) {
        //Already low, do not proceed
        if (mMapIntr[idx]->read() == false) {
            continue;
        }
        //Proceed to clear at next positive edge when interrupts are currently high
        mAssert_DeassertInterruptEvent[idx].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartClkTime), mTimeResolutionUnit);
    }
}//}}}

//========================================================
//========= Overwrite func. of CRWInterface ==============
//========================================================
///*********************************
/// Function     : read
/// Description  : public API for reading registers of HWCORE_RCTG model
/// Parameters   : 
///                unsigned int offset   : address offset of register to be read
///                   unsigned char* p_data : pointer to contain read data of register
///                   unsigned int size     : length of data to be read
///                   bool debug            : debug flag (0: normal read | 1: debug read)
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Flow_004}
void HWCORE_RCTG::read(unsigned int offset, unsigned char* p_data, unsigned int size, bool debug)
{//{{{
    if (p_data == NULL) {
        return;
    }
    mpHWCORE_RCTG_Func->read(offset, p_data, size, debug);
}//}}}

///*********************************
/// Function     : write
/// Description  : public API for writing registers of HWCORE_RCTG model
/// Parameters   : 
///                unsigned int offset   : address offset of register to be written
///                   unsigned char* p_data : pointer to data to be written to register
///                   unsigned int size     : length of data to be written
///                   bool debug            : debug flag (0: normal write | 1: debug write)
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Flow_005}
void HWCORE_RCTG::write(unsigned int offset, unsigned char* p_data, unsigned int size, bool debug)
{//{{{
    if (p_data == NULL) {
        return;
    }
    mpHWCORE_RCTG_Func->write(offset, p_data, size, debug);
}//}}}

//========================================================
//==================== Outputs/Interrupts ================
//========================================================
///*********************************
/// Function     : Assert_DeassertInterruptMethod
/// Description  : method for interrupt Assert/De-assert 
/// Parameters   : 
///                int InterruptIndex: index of interrupts
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_053, HWCORE_RCTG_UD_Flow_011}
void HWCORE_RCTG::Assert_DeassertInterruptMethod(unsigned int InterruptIndex)
{//{{{
    bool cur_val = false;
    bool set_val = false;
    double current_time = sc_time_stamp().to_double();

    switch (InterruptIndex){
        case emOUTPUT_RCTG_END_SP_REG:     //output_rctg_end_sp_reg
        case emWC_EM2ICMP0:                //wc_em2icmp0
        case emWC_EM2ICMP1:                //wc_em2icmp1
            cur_val = mMapIntr[InterruptIndex]->read();
            //Set inverted value
             set_val = !cur_val;
            mMapIntr[InterruptIndex]->write(set_val);

            //Just been raised
            if (set_val == true){
                //Clear at next positive edge
                mAssert_DeassertInterruptEvent[InterruptIndex].notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartClkTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
                
                //Process when rectangle IP done processing
                if(InterruptIndex == emOUTPUT_RCTG_END_SP_REG){
                    mCurRectIPState = emRCTG_IDLE;
                    //Dump statistical information when rectangle IP end
                    if (mDumpStatInfo) {
                        re_printf("info", "Information of buffers (end): \n");
                        DumpStatInfo();
                    }
                    re_printf("info","Rectangle IP is end.\n");
                }

                //Dump interrupt information when it is asserted
                if(mDumpInterrupt == true){
                    re_printf("info","[INT][%s] Assert\n",mMapIntr[InterruptIndex]->name());
                }
            }else{
                //Dump interrupt information when it is de-asserted
                if(mDumpInterrupt == true){
                    re_printf("info","[INT][%s] De-assert\n",mMapIntr[InterruptIndex]->name());
                }
            }
            break;
        default:                          //Invalid interrupt ports   
            break;
    }
}//}}}

///*********************************
/// Function     : UpdateOutputMethod
/// Description  : method for Output ports update
/// Parameters   : 
///                int OutputIndex: index of outputs
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Flow_015, HWCORE_RCTG_UD_Flow_016}
void HWCORE_RCTG::UpdateOutputMethod(unsigned int OutputIndex)
{//{{{
    unsigned short val = mOutputValue[OutputIndex];
    
    switch (OutputIndex){
        // Unsigned short output ports
        case emRDATA_EM2IICMP0:             //rdata_em2iicmp0 
        case emEM2ICMP0:                    //em2icmp0
        case emEM2ICMP1:                    //em2icmp1
            mMapOut_UShort[OutputIndex]->write(val);
            break;
        // 1-bit output ports
        case emEM2IBRECCTR_SCLTCMP0_REG:    //em2ibrecctr_scltcmp0_reg
        case emRCTG_U_80M_REG:              //rctg_u_80m_reg
        case emRCTG_V_80M_REG:              //rctg_v_80m_reg
        case emRCTG_W_80M_REG:              //rctg_w_80m_reg
            mMapOut_Bool[OutputIndex]->write((bool)(val & 0x1));
            break;
        default:                           //Invalid output ports
            break;
    }
}//}}}

//========================================================
//================= Operation processes ==================
//========================================================
///*********************************
/// Function     : TriggerRectIPMethod
/// Description  : method for rectangle IP trigger process
/// Parameters   : None
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_052, HWCORE_RCTG_UD_Direction_054, HWCORE_RCTG_UD_Direction_055, HWCORE_RCTG_UD_Flow_012, HWCORE_RCTG_UD_Flow_013}
void HWCORE_RCTG::TriggerRectIPMethod()
{//{{{
    //Reset is asserted or clock is zero
    if (GetResetStatus("PRESETn") == true || GetResetStatus("rst_n") == true || CheckClockPeriod() == false) {
        re_printf("info", "Rectangle IP cannot be triggered while reset is asserted or clock is zero.\n");
        return;
    }

    //Prevent another trigger while rectangle IP in busy state
    if (mCurRectIPState == emRCTG_BUSY){
        re_printf("info", "Rectangle IP is currently busy, it cannot be triggered.\n");
        return;
    }

    //Toggle state of rectangle IP
    mCurRectIPState = emRCTG_BUSY;
    //Notify function block to update buffer values.
    mpHWCORE_RCTG_Func->UpdateRegBuffer();
    //Trigger start rectangle IP with set processing time.
    mProcessRectangleIPEvent.notify((double)(mPCLKPeriod * mProcessingTime), SC_PS);
    re_printf("info", "Rectangle IP is triggered start up.\n");
    //Dump statistical information when rectangle IP start up
    if (mDumpStatInfo) {
        re_printf("info", "Information of buffers (start): \n");
        DumpStatInfo();
    }
}//}}}

///*********************************
/// Function     : TriggerBRectIPMethod
/// Description  : method for batch rectangle IP trigger process
/// Parameters   : None
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_056, HWCORE_RCTG_UD_Flow_014}
void HWCORE_RCTG::TriggerBRectIPMethod()
{//{{{
    //Reset is asserted or clock is zero
    if (GetResetStatus("PRESETn") == true || GetResetStatus("rst_n") == true || CheckClockPeriod() == false) {
        re_printf("info", "Batch Rectangle IP cannot be triggered while reset is asserted or clock is zero.\n");
        return;
    }

    mProcessBatchRectangleIPEvent.notify();
}//}}}

///*********************************
/// Function     : RectangleIPProcessMethod
/// Description  : Threads/Methods for process operations after rectangle IP was triggered
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG::RectangleIPProcessMethod()
{//{{{
    //Start core-process of function block
    mpHWCORE_RCTG_Func->RectangleIPProcess();
}//}}}

///*********************************
/// Function     : BRectangleIPProcessMethod
/// Description  : Threads/Methods for process operations after batch rectangle IP was triggered
/// Parameters   : None
/// Return value : None
///*********************************
void HWCORE_RCTG::BRectangleIPProcessMethod()
{//{{{
    //Start core-process of function block
    mpHWCORE_RCTG_Func->BRectangleIPProcess();
}//}}}

//========================================================
//===================== PYTHON IF ========================
//========================================================
///*********************************
/// Function     : SetMessageLevel
/// Description  : Set message level
/// Parameters   : 
///                const std::string msg_lv : message level
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_001, HWCORE_RCTG_UD_Direction_019}
void HWCORE_RCTG::SetMessageLevel(const std::string msg_lv)
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
    mpHWCORE_RCTG_Func->RegisterHandler(cmd);

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
    mpHWCORE_RCTG_Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : DumpRegisterRW
/// Description  : Enable/disable dumping message when users access registers
/// Parameters   : 
///                const std::string is_enable: string to determine whether enable or disable dumping read/write register message
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_002}
void HWCORE_RCTG::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_RCTG_Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : EnableRegisterMessage
/// Description  : Enable/disable info/warning/error message of register IF
/// Parameters   : 
///                const std::string is_enable: string to determine whether enable or disable dumping read/write register message
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_003, HWCORE_RCTG_UD_Direction_050, HWCORE_RCTG_UD_Flow_018}
void HWCORE_RCTG::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }else if(word_vector[0] == "false"){
            mEnableRegisterMessage = false;
        }else{
            re_printf("warning", "Invalid argument: HWCORE_RCTG_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }else if(word_vector.size() == 0){
        std::string temp = "";
        if(mEnableRegisterMessage){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info", "HWCORE_RCTG_EnableRegisterMessage %s\n", temp.c_str());
    }else{
        re_printf("warning", "Invalid argument: HWCORE_RCTG_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : DumpInterrupt
/// Description  : Enable/disable dumping interrupt message
/// Parameters   : 
///                const std::string is_enable: string to determine whether enable or disable dumping interrupt message
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_004}
void HWCORE_RCTG::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_enable);
    if (temp.size() == 1) {
        if (temp[0] == "true") {
            mDumpInterrupt = true;
        } else if (temp[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_RCTG_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (temp.size() == 0){
          std::string temp_arg = "";
          if(mDumpInterrupt == true){
              temp_arg = "true";
          }else{
              temp_arg = "false";
          }
        re_printf("info","HWCORE_RCTG_DumpInterrupt     %s\n",temp_arg.c_str());
    } else {
        re_printf("warning","Invalid argument: HWCORE_RCTG_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

 ///*********************************
 /// Function     : EnableStatInfo
 /// Description  : Enable/disable dumping statistical information
 /// Parameters   : 
 ///                const std::string is_enable: string to determine whether enable or disable dumping statistical information
 /// Return value : None
 ///*********************************
 // Ref: {HWCORE_RCTG_UD_Direction_005}
void HWCORE_RCTG::EnableStatInfo(const std::string is_enable)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_enable);
    if (temp.size() == 1) {
        if (temp[0] == "true") {
            mDumpStatInfo = true;
        }
        else if (temp[0] == "false") {
            mDumpStatInfo = false;
        }
        else {
            re_printf("warning", "Invalid argument: HWCORE_RCTG_EnableStatInfo %s\n", is_enable.c_str());
        }
    }
    else if (temp.size() == 0) {
        std::string temp_arg = "";
        if (mDumpStatInfo == true) {
            temp_arg = "true";
        }
        else {
            temp_arg = "false";
        }
        re_printf("info", "HWCORE_RCTG_EnableStatInfo     %s\n", temp_arg.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: HWCORE_RCTG_EnableStatInfo %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : ChgProcessingTime
/// Description  : Change processing time of rectangle IP 3
/// Parameters   : 
///                const unsigned int value: processing time of rectangle IP
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_059}
void HWCORE_RCTG::ChgProcessingTime(const unsigned int value)
{//{{{
    mProcessingTime = value;
}//}}}

///*********************************
/// Function     : DumpStatInfo
/// Description  : Dump status register's value
/// Parameters   : None
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_006}
void HWCORE_RCTG::DumpStatInfo(void)
{//{{{
    //////////////////////////////////
    /////// Buffers Information //////
    //////////////////////////////////
    //RECCTR.FDRCT buffer's information
    re_printf("info", "    Buffer RECCTR.FDRCT = 0x%08X\n", mpHWCORE_RCTG_Func->GetRegBufferVal("RECCTR.FDRCT"));
    //RECCTR.FIPPOSI buffer's information
    re_printf("info", "    Buffer RECCTR.FIPPOSI = 0x%08X\n", mpHWCORE_RCTG_Func->GetRegBufferVal("RECCTR.FIPPOSI"));
    //PHQSFT buffer's information
    re_printf("info", "    Buffer RECCTR.SLCTCMP0 = 0x%08X\n", mpHWCORE_RCTG_Func->GetRegBufferVal("PHQSFT"));
    //PSWSFT buffer's information
    re_printf("info", "    Buffer RECCTR.SETREC = 0x%08X\n", mpHWCORE_RCTG_Func->GetRegBufferVal("PSWSFT"));
}//}}}

///*********************************
/// Function     : AssertReset
/// Description  : Command to assert reset
/// Parameters   : 
///                const std::string reset_name : name of reset to be asserted by command
///                const double start_time      : time to start reset
///                const double period          : reset period
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_007, HWCORE_RCTG_UD_Direction_040, HWCORE_RCTG_UD_Direction_041, HWCORE_RCTG_UD_Direction_046}
void HWCORE_RCTG::AssertReset(const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETnId]) && (!mIsResetCmdReceive[emPRESETnId])) {
            mIsResetCmdReceive[emPRESETnId] = true;
            re_printf("info", "The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPRESETnId].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETnId] = period;
        }
        else {
            re_printf("warning", "The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    }
    else if (reset_name == "rst_n") {
        if ((!mIsResetHardActive[emrst_nId]) && (!mIsResetCmdReceive[emrst_nId])) {
            mIsResetCmdReceive[emrst_nId] = true;
            re_printf("info", "The model will be reset (rst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emrst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[emrst_nId] = period;
        }
        else {
            re_printf("warning", "The software reset of rst_n is called in the reset operation of the model. So it is ignored\n");
        }
    }
    else {
        re_printf("warning", "The reset name (%s) is wrong. It should be PRESETn or rst_n", reset_name.c_str());
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq
/// Description  : Set clock value and clock unit
/// Parameters   : 
///                const std::string clock_name : name of clock port to be set
///                const sc_dt::uint64 freq     : clock frequency to be set
///                const std::string unit       : unit of clock frequency to be set
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_008, HWCORE_RCTG_UD_ATT1_013}
void HWCORE_RCTG::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;

    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq(mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1 / (double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartClkTime = sc_time_stamp().to_double();

                //Process when PCLK change
                mProcessAfterPCLKChangeEvent.notify(SC_ZERO_TIME);
            }else{
                mPCLKPeriod = 0;
                // Prepare for next run
                CancelEvents();
            }
        }else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : GetCLKFreq
/// Description  : Dump clock's value
/// Parameters   : 
///                const std::string clock_name : name of clock port to be dump value
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_009, HWCORE_RCTG_UD_Direction_049}
void HWCORE_RCTG::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info", "PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        }else{
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }else{
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ForceRegister
/// Description  : Force value to register
/// Parameters   : 
///                const std::string reg_name   : name of register to be forced value
///                const unsigned int reg_value : value to be forced to register
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_010}
void HWCORE_RCTG::ForceRegister(const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_RCTG_Func->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReleaseRegister
/// Description  : Release forced value after forcing registers
/// Parameters   : 
///                const std::string reg_name   : name of rgister to be released
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_011}
void HWCORE_RCTG::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHWCORE_RCTG_Func->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : WriteRegister
/// Description  : Write value to registers by software
/// Parameters   : 
///                const std::string reg_name   : name of rgister to be written
///                const unsigned int reg_value : value to be written to registr
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_012}
void HWCORE_RCTG::WriteRegister(const std::string reg_name, const unsigned int reg_value)
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
            mpHWCORE_RCTG_Func->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReadRegister
/// Description  : Read value of register by software
/// Parameters   : 
///                const std::string reg_name   : name of rgister to be read
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_013}
void HWCORE_RCTG::ReadRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpHWCORE_RCTG_Func->RegisterHandler(cmd);
        }else{
            re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
        }
    }else{
        re_printf("warning", "Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ListRegister
/// Description  : List all registers name
/// Parameters   : None
/// Return value : None
///*********************************
// Ref: {HWCORE_RCTG_UD_Direction_014}
void HWCORE_RCTG::ListRegister(void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHWCORE_RCTG_Func->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : Help
/// Description  : Dump help message of all parameters or commands
/// Parameters   : 
///                const std::string type: type to be dump (parameter of command)
/// Return value : None
///*********************************
// Description: 
// Ref: {HWCORE_RCTG_UD_Direction_015, HWCORE_RCTG_UD_Direction_017, HWCORE_RCTG_UD_Direction_018, HWCORE_RCTG_UD_Direction_029}
void HWCORE_RCTG::Help(const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_MessageLevel (\"HWCORE_RCTG instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_DumpRegisterRW (\"HWCORE_RCTG instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_EnableRegisterMessage (\"HWCORE_RCTG instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_DumpInterrupt(\"HWCORE_RCTG instance\", \"true/false\")                      Enable/disable dumping interrupt information HWCORE_RCTG(Default: true)");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_EnableStatInfo(\"HWCORE_RCTG instance\", \"true/false\")                     Enable/disable dumping statistical information HWCORE_RCTG(Default: false)");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_ChgProcessingTime(\"HWCORE_RCTG instance\", process_time)                    Change processing time of rectangle IP (Default: 32)");
        }else if (word_vector[0] == "commands"){
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_DumpStatInfo (\"HWCORE_RCTG instance\")                                      Dump the statistical info of model");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_AssertReset (\"HWCORE_RCTG instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the HWCORE_RCTG model");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_SetCLKFreq (\"HWCORE_RCTG instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to HWCORE_RCTG model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_GetCLKFreq (\"HWCORE_RCTG instance\", \"clock_name\")                        Get clock frequency value of HWCORE_RCTG model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_ForceRegister (\"HWCORE_RCTG instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_ReleaseRegister (\"HWCORE_RCTG instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_WriteRegister (\"HWCORE_RCTG instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_ReadRegister (\"HWCORE_RCTG instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(), "HWCORE_RCTG_ListRegister (\"HWCORE_RCTG instance\")                                      Dump name of all registers");
        }else{
            re_printf("warning", "The name (%s) of HWCORE_RCTG_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    }else{
        re_printf("warning", "The name (%s) of HWCORE_RCTG_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
