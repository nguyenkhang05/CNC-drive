// ---------------------------------------------------------------------
// $Id: PSIS011.cpp,v 1.7 2018/07/12 06:20:36 chanle Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "PSIS011.h"
#include "PSIS011_Func.h"

//=============================================
//============= Class Csync_pulse =============
//=============================================
Cwdt::Cwdt(sc_module_name name)
    :sc_module(name)
    ,wdt_error("wdt_error")
{//{{{
    wdt_error.initialize(0);
    Initialize();
    
    SC_METHOD(WriteWDTErrorMethod);
    dont_initialize();
    sensitive << mWriteWDTErrorEvent;

    SC_METHOD(NegateWDTErrorMethod);
    dont_initialize();
    sensitive << mNegateWDTErrorEvent;
}//}}}

/// Destructor
Cwdt::~Cwdt()
{//{{{ 
}//}}}

/// Initialize variable when reset
void Cwdt::Initialize()
{//{{{
    mPrescaler = 0;
    mExpired = 0;
    mWDTError = 0;
    mClockPeriod = 0;
    mTimeResolutionUnit = SC_NS;
}//}}}

/// Set psis_clk period
void Cwdt::ConfigWDTClock(sc_dt::uint64 period, sc_time_unit time_unit)
{//{{{
    mClockPeriod = period;
    mTimeResolutionUnit = time_unit;
}//}}} 

/// Config WDT counter: scale, expire value
void Cwdt::ConfigWDTCounter(unsigned int prescaler, unsigned int expired)
{//{{{
    mPrescaler = prescaler;
    mExpired = expired;
}//}}} 

/// Start WDT counter
void Cwdt::StartWDT()
{//{{{
    if (mPrescaler > 0){
        double expired = (double)mClockPeriod * mPrescaler * mExpired;
        mWDTError = 1;
        mWriteWDTErrorEvent.cancel();
        mWriteWDTErrorEvent.notify(expired, mTimeResolutionUnit);
    }
}//}}}

/// Stop WDT counter
void Cwdt::StopWDT()
{//{{{
    mWriteWDTErrorEvent.cancel();
}//}}}

/// Write WDT error wdt_error
void Cwdt::WriteWDTErrorMethod()
{//{{{
    wdt_error.write(mWDTError);
    if (mWDTError == 1){
        mNegateWDTErrorEvent.notify((double)mClockPeriod, mTimeResolutionUnit);
    }
}//}}}

/// Negate WDT error wdt_error
void Cwdt::NegateWDTErrorMethod()
{//{{{
    mWDTError = 0;
    mWriteWDTErrorEvent.notify();
}//}}}

/// Process when reset is activated or negated
void Cwdt::EnableReset (const bool is_active)
{//{{{
    StopWDT();
}//}}}

//=============================================
//============= Class Csync_pulse =============
//=============================================
/// Constructor and Destructor
Csync_pulse::Csync_pulse(sc_module_name name, PSIS011 *psis011)
        :sc_module(name)
        ,psis_trg_sync_chn("psis_trg_sync_chn")
        ,sync_pulse("sync_pulse")
        ,inner_sync_pulse_sig("inner_sync_pulse_sig")
{//{{{
    sc_assert (psis011 != NULL);
    mpPSIS011 = psis011;
    sync_pulse.initialize(0);

    Initialize();
    mPCLKPeriod = 0;
    mTimeResolutionUnit = SC_NS;
    
    SC_THREAD(GenInnerSyncPulseThread);
    dont_initialize();
    sensitive << mGenInnerSyncPulseEvent;

    SC_METHOD(IssueSelectedSyncPulseMethod);
    dont_initialize();
    sensitive << psis_trg_sync_chn;
    sensitive << inner_sync_pulse_sig;
}//}}}

/// Destructor
Csync_pulse::~Csync_pulse()
{//{{{ 
}//}}}

/// Process when reset assert or negate
void Csync_pulse::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        StopGenInnerSyncPulse();
        Initialize();
    }
}//}}}

/// Initialize variable
void Csync_pulse::Initialize()
{//{{{
    mEnableGenInnerSyncPulse = false;
    mSyncPulseSelect = 0;
    mPrescaler = 0;
    mExpired = 0;
}//}}}

/// Config clock for Sync Pulse
void Csync_pulse::ConfigSyncPulseClock(sc_dt::uint64 pclk_period, sc_time_unit time_unit)
{//{{{
    mPCLKPeriod = pclk_period;
    mTimeResolutionUnit = time_unit;
}//}}}

/// Start generating internal sync pulse
void Csync_pulse::StartGenInnerSyncPulse()
{//{{{
    mEnableGenInnerSyncPulse = true;
    mGenInnerSyncPulseEvent.notify();
}//}}}

/// Stop generating internal sync pulse
void Csync_pulse::StopGenInnerSyncPulse()
{//{{{
    mEnableGenInnerSyncPulse = false;
}//}}}

/// Config internal sync pulse parameter (prescaler, expired value)
void Csync_pulse::ConfigInnerSyncPulse(unsigned int select, unsigned int prescaler, unsigned int expired)
{//{{{
    mSyncPulseSelect = select;
    mPrescaler = prescaler;
    mExpired = expired;
}//}}}

/// Generate internal sync pulse based on setting in registers
void Csync_pulse::GenInnerSyncPulseThread()
{//{{{
    while(true){
        if ((mEnableGenInnerSyncPulse == false) || (mpPSIS011->CheckClockPeriod("psis_mult_clk") == false)){
            wait();
            continue;
        }
        double wait_time = mpPSIS011->CalculateCLKPosEdge("PCLK", false, sc_time_stamp().to_double()) + (mPrescaler + 1)*(double)mPCLKPeriod*mExpired;
        if (wait_time == 0){
            wait();
            continue;
        }
        wait (wait_time, mTimeResolutionUnit);
        inner_sync_pulse_sig.write(1);
        wait ((double)mPCLKPeriod, mTimeResolutionUnit);
        inner_sync_pulse_sig.write(0);
    }
}//}}}

/// Issue sync pulse
void Csync_pulse::IssueSelectedSyncPulseMethod()
{//{{{
    // If select GTM
    if (mSyncPulseSelect == 1){
        sync_pulse.write(psis_trg_sync_chn.read());

    // If select inner sync pulse
    }else{
        sync_pulse.write(inner_sync_pulse_sig.read());
    }
}//}}}

//=============================================
//============= Class Ctimestamp ==============
//=============================================
/// Constructor and Destructor
Ctimestamp::Ctimestamp(sc_module_name name)
        :sc_module(name)
        ,psis_clk_timestamp("psis_clk_timestamp")
        ,psis_clr_timestamp("psis_clr_timestamp")
        ,psis_stsp_timestamp("psis_stsp_timestamp")
        ,internal_clock("internal_clock")
{//{{{
    mIsReset = false;
    Initialize();

    SC_METHOD(ClearTimestampMethod);
    dont_initialize();
    sensitive << psis_clr_timestamp;
    sensitive << mClearTimestampEvent;

    SC_METHOD(MonitorGTMClockMethod);
    dont_initialize();
    sensitive << psis_clk_timestamp;

    SC_METHOD(MonitorInternalClockMethod);
    dont_initialize();
    sensitive << internal_clock;
}//}}}

/// Destructor
Ctimestamp::~Ctimestamp()
{//{{{ 
}//}}}

/// Process when reset assert or negate
void Ctimestamp::EnableReset(const bool is_active)
{//{{{
    mIsReset = is_active;
    if (is_active){
        Initialize();
    }
}//}}}

/// Get timestamp value
unsigned int Ctimestamp::GetTimestamp()
{//{{{
    return mCounter;
}//}}}

/// Config timestamp
void Ctimestamp::ConfigTimestamp(bool clear_select_gtm, bool enable_select_gtm, bool clock_select_gtm)
{//{{{
    mClearSelectGTM = clear_select_gtm;
    mEnableSelectGTM = enable_select_gtm;
    mClockSelectGTM = clock_select_gtm;
}//}}}

/// Set timestamp enable or not via setting of inner register
void Ctimestamp::SetTSInnerEnable(bool ts_inner_enable)
{//{{{
    mTSInnerEnable = ts_inner_enable;
}//}}}

/// Notify clear timestamp (called from Internal register)
void Ctimestamp::NotifyClearTimestamp()
{//{{{
    mClearTimestampEvent.notify();
}//}}}

/// Clear timestamp counter
void Ctimestamp::ClearTimestampMethod()
{//{{{
    // Not process if reset activated
    if (mIsReset){
        return;
    }

    // If GTM is selected
    if (mClockSelectGTM){
        if (psis_clr_timestamp.read() == 1){
            mCounter = 0;
        }

    // If internal timing is select 
    }else{
        mCounter = 0;
    }
}//}}}

/// Counter base on the clock input GTM
void Ctimestamp::MonitorGTMClockMethod()
{//{{{
    // Not process if reset activated
    if (mIsReset){
        return;
    }

    // Count up based on GTM clock if timestamp is enabled
    if ((psis_clk_timestamp.read() == 1) && (CheckTimestampEnable()) && (mClockSelectGTM)){
        mCounter++;
    }
}//}}}

/// Counter base on the clock input GTM
void Ctimestamp::MonitorInternalClockMethod()
{//{{{
    // Not check: reset activated or not here, because the interal clock is not generated in reset period.

    // Count up based on internal clock if timestamp is enabled
    if ((internal_clock.read()) && (CheckTimestampEnable()) && (mClockSelectGTM == false)){
        mCounter++;
    }
}//}}}

/// Check timestamp is enable or not
bool Ctimestamp::CheckTimestampEnable()
{//{{{
    bool ret = false;
    if (mEnableSelectGTM){
        ret = psis_stsp_timestamp.read();
    }else{
        ret = mTSInnerEnable;
    }
    return ret;
}//}}}

/// Initialize variable
void Ctimestamp::Initialize()
{//{{{
    mCounter = 0;
    mClearSelectGTM = false;
    mEnableSelectGTM = false;
    mClockSelectGTM = false;
    mTSInnerEnable = false;
}//}}}

//=============================================
//============= Class PSIS011 =================
//=============================================
/// Constructor and Destructor
PSIS011::PSIS011(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,PSIS011_AgentController()
        ,PCLK("PCLK")
        ,psis_clk("psis_clk")
        ,psis_mult_clk("psis_mult_clk")
        ,PRESETn("PRESETn")
        ,psis_rst_n("psis_rst_n")
        ,psis_mult_rst_n("psis_mult_rst_n")
        ,RX_DATA("RX_DATA")
        ,RX_CONTROL("RX_CONTROL")
        ,TX_DATA("TX_DATA")
        ,TX_CONTROL("TX_CONTROL")
        ,psis_clk_timestamp_a("psis_clk_timestamp_a")
        ,psis_clk_timestamp_b("psis_clk_timestamp_b")
        ,psis_clr_timestamp_a("psis_clr_timestamp_a")
        ,psis_clr_timestamp_b("psis_clr_timestamp_b")
        ,psis_stsp_timestamp_a("psis_stsp_timestamp_a")
        ,psis_stsp_timestamp_b("psis_stsp_timestamp_b")
        ,timestamp_inner_clock_sig("timestamp_inner_clock_sig")
        ,psis_clk_timestamp_a_sig("psis_clk_timestamp_a_sig")
        ,psis_clr_timestamp_a_sig("psis_clr_timestamp_a_sig")
        ,psis_stsp_timestamp_a_sig("psis_stsp_timestamp_a_sig")
        ,psis_clk_timestamp_b_sig("psis_clk_timestamp_b_sig")
        ,psis_clr_timestamp_b_sig("psis_clr_timestamp_b_sig")
        ,psis_stsp_timestamp_b_sig("psis_stsp_timestamp_b_sig")
        ,sw_rst_sig("sw_rst_sig")
{//{{{ 
    mpPSIS011Func = new PSIS011_Func((std::string)name, this);
    mpTimestampA = new Ctimestamp("timestampA");
    mpTimestampB = new Ctimestamp("timestampB");

    for (unsigned int i = 0; i < emNumChannel; i++){
        std::ostringstream  inst_name;
        inst_name << "WDT_" << i;
        mpWDTChannel[i] = new Cwdt(inst_name.str().c_str());
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        std::ostringstream  inst_name;
        inst_name << "sync_pulse_gen_" << i;
        mpSyncPulseChannel[i] = new Csync_pulse(inst_name.str().c_str(), this);
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        std::ostringstream  inst_name;
        inst_name << "int_psis_ch" << i;
        int_psis_chn[i] = new sc_out<bool>(inst_name.str().c_str());
        int_psis_chn[i]->initialize(false);
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        std::ostringstream  inst_name;
        inst_name << "dma_psis_ch" << i << "_rx";
        dma_psis_chn_rx[i] = new sc_out<bool>(inst_name.str().c_str());
        dma_psis_chn_rx[i]->initialize(false);
    }
    for (unsigned int i = 0; i < (emNumChannel - 1); i++){
        std::ostringstream  inst_name;
        inst_name << "dma_psis_ch" << i << "_tx";
        dma_psis_chn_tx[i] = new sc_out<bool>(inst_name.str().c_str());
        dma_psis_chn_tx[i]->initialize(false);
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        std::ostringstream  inst_name;
        inst_name << "psis_trg_sync_ch" << i;
        psis_trg_sync_chn[i] = new sc_in<bool>(inst_name.str().c_str());
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        std::ostringstream  inst_name;
        inst_name << "selected_sync_pulse_sig_ch" << i;
        selected_sync_pulse_sig_chn[i] = new sc_signal<bool>(inst_name.str().c_str());
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        std::ostringstream  inst_name;
        inst_name << "wdt_error_sig_ch" << i;
        wdt_error_sig_chn[i] = new sc_signal<bool>(inst_name.str().c_str());
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        std::ostringstream  inst_name;
        inst_name << "tx_req_sig" << i;
        tx_req_sig[i] = new sc_signal<bool>(inst_name.str().c_str());
        tx_req_sig[i]->write(0);
    }

    // Initialize output port
    TX_DATA.initialize(0);
    TX_CONTROL.initialize(0);
    sw_rst_sig.write(1);
    timestamp_inner_clock_sig.write(0);
    rx_control_sig.write(0);
    rx_data_sig.write(0);

    // Configure Slave socket
    setSlaveResetPort32(&PRESETn);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32("ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];

    mBusSlaveIf32[0]->setFuncModulePtr(mpPSIS011Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // connect GTM port to timestamp A/B
    (mpTimestampA->psis_clk_timestamp)(psis_clk_timestamp_a);
    (mpTimestampA->psis_clr_timestamp)(psis_clr_timestamp_a);
    (mpTimestampA->psis_stsp_timestamp)(psis_stsp_timestamp_a);
    (mpTimestampB->psis_clk_timestamp)(psis_clk_timestamp_b);
    (mpTimestampB->psis_clr_timestamp)(psis_clr_timestamp_b);
    (mpTimestampB->psis_stsp_timestamp)(psis_stsp_timestamp_b);
    mpTimestampA->internal_clock(timestamp_inner_clock_sig);
    mpTimestampB->internal_clock(timestamp_inner_clock_sig);
    // Connect Sync Pulse generator of each channel
    for (unsigned int i = 0; i < emNumChannel; i++){
        (mpSyncPulseChannel[i]->psis_trg_sync_chn)(*psis_trg_sync_chn[i]);
        (mpSyncPulseChannel[i]->sync_pulse)(*selected_sync_pulse_sig_chn[i]);
    }
    // Connect to WDT of each channel
    for (unsigned int i = 0; i < emNumChannel; i++){
        (mpWDTChannel[i]->wdt_error)(*wdt_error_sig_chn[i]);
    }

    // Initialize variables
    for (int i = 0; i < emNumClk; i++){
        mClkPeriod[i]       = 0;
        mClkOrgFreq[i]      = 0;
        mClkFreq[i]         = 0;
        mClkFreqUnit[i]     = "Hz";
        mStartClkTime[i]    = 0;
    }
    for (int i = 0; i < emNumReset; i++){
        mResetCurVal[i]         = true;
        mResetPreVal[i]         = true;
        mIsResetHardActive[i]   = false;
        mIsResetCmdReceive[i]   = false;
        mIsResetCmdActive[i]    = false;
        mResetCmdPeriod[i]      = 0;
    }
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mDumpInterrupt = false;
    mDNFDelay = 0;

    CreateValidFPKTvsFPAYLDTable();
    CreateStrobeStringMap();

    InitializeOfPRESETn();
    InitializeOfpsis_rst_n();
    InitializeOfpsis_mult_rst_n();
    mSWRST = 1;
    mWritesw_rst_sigEvent.notify();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorPCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(Monitorpsis_clkMethod);
    dont_initialize();
    sensitive << psis_clk;

    SC_METHOD(Monitorpsis_mult_clkMethod);
    dont_initialize();
    sensitive << psis_mult_clk;

    SC_METHOD(MonitorPRESETnMethod);
    sensitive << PRESETn;
    sensitive << sw_rst_sig;

    SC_METHOD(Monitorpsis_rst_nMethod);
    sensitive << psis_rst_n;
    sensitive << sw_rst_sig;

    SC_METHOD(Monitorpsis_mult_rst_nMethod);
    sensitive << psis_mult_rst_n;
    sensitive << sw_rst_sig;

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    for (unsigned int chid = 0; chid < emNumChannel; chid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(selected_sync_pulse_sig_chn[chid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::MonitorSyncPulseMethod, this, chid),
                sc_core::sc_gen_unique_name("MonitorSyncPulseMethod"), &opt);
    }

    for (unsigned int chid = 0; chid < emNumChannel; chid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(wdt_error_sig_chn[chid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::MonitorWDTErrorMethod, this, chid),
                sc_core::sc_gen_unique_name("MonitorWDTErrorMethod"), &opt);
    }

    SC_METHOD(MonitorDNFOutputMethod);
    dont_initialize();
    sensitive << rx_control_sig;

    SC_THREAD(ArbitrateTxReqThread);
    dont_initialize();
    for (unsigned int i = 0; i < emNumChannel; i++){
        sensitive << *tx_req_sig[i];
    }

    SC_METHOD(MoveToStopReceptionStateMethod);
    dont_initialize();
    sensitive << mMoveToStopReceptionStateEvent;

    SC_METHOD(MoveToIdleReceptionStateMethod);
    dont_initialize();
    sensitive << mMoveToIdleReceptionStateEvent;

    SC_METHOD(EndIdleReceptionStateMethod);
    dont_initialize();
    sensitive << mEndIdleReceptionStateEvent;

    SC_THREAD(GenClockForTimestampThread);
    dont_initialize();
    sensitive << mGenClockForTimestampEvent;

    SC_METHOD(DNFDelayMethod);
    dont_initialize();
    sensitive << RX_CONTROL;
    sensitive << RX_DATA;

    SC_METHOD(ForwardDNFUARTRxMethod);
    dont_initialize();
    sensitive << mForwardDNFUARTRxEvent;

    for (unsigned int reqid = 0; reqid < emNumChannel; reqid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteTxReqEvent[reqid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::WriteTxReqMethod, this, reqid),
                sc_core::sc_gen_unique_name("WriteTxReqMethod"), &opt);
    }

    for (unsigned int reqid = 0; reqid < emNumChannel; reqid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateTxReqEvent[reqid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::NegateTxReqMethod, this, reqid),
                sc_core::sc_gen_unique_name("NegateTxReqMethod"), &opt);
    }

    SC_THREAD(TransPSI5Thread);
    dont_initialize();
    sensitive << mTransPSI5Event;
    sensitive << mResetEvent[empsis_rst_nId];
    sensitive << mClkZeroEvent[empsis_clkId];

    SC_THREAD(TransUARTThread);
    dont_initialize();
    sensitive << mTransUARTDataEvent;
    sensitive << mResetEvent[empsis_rst_nId];
    sensitive << mClkZeroEvent[empsis_clkId];

    SC_METHOD(WriteUARTOutputMethod);
    dont_initialize();
    sensitive << mWriteUARTOutputEvent;

    SC_METHOD(ClearTXSTSBitMethod);
    dont_initialize();
    sensitive << mClearTXSTSBitEvent;

    for (unsigned int txreqid = 0; txreqid < (emNumChannel - 1); txreqid++) {// 0-6 reqid for channel 1-7
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mClearDDSRSTSBitEvent[txreqid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::ClearDDSRSTSBitMethod, this, txreqid),
                sc_core::sc_gen_unique_name("ClearDDSRSTSBitMethod"), &opt);
    }

    for (unsigned int chid = 0; chid < emNumChannel; chid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteInterruptEvent[chid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::WriteInterruptMethod, this, chid),
                sc_core::sc_gen_unique_name("WriteInterruptMethod"), &opt);
    }

    for (unsigned int chid = 0; chid < emNumChannel; chid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateInterruptEvent[chid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::NegateInterruptMethod, this, chid),
                sc_core::sc_gen_unique_name("NegateInterruptMethod"), &opt);
    }

    for (unsigned int txreqid = 0; txreqid < (emNumChannel - 1); txreqid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDMARequestTXEvent[txreqid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::WriteDMARequestTXMethod, this, txreqid),
                sc_core::sc_gen_unique_name("WriteDMARequestTXMethod"), &opt);
    }

    for (unsigned int txreqid = 0; txreqid < (emNumChannel - 1); txreqid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateDMARequestTXEvent[txreqid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::NegateDMARequestTXMethod, this, txreqid),
                sc_core::sc_gen_unique_name("NegateDMARequestTXMethod"), &opt);
    }

    for (unsigned int chid = 0; chid < emNumChannel; chid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDMARequestRXEvent[chid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::WriteDMARequestRXMethod, this, chid),
                sc_core::sc_gen_unique_name("WriteDMARequestRXMethod"), &opt);
    }

    for (unsigned int chid = 0; chid < emNumChannel; chid++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateDMARequestRXEvent[chid]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&PSIS011::NegateDMARequestRXMethod, this, chid),
                sc_core::sc_gen_unique_name("NegateDMARequestRXMethod"), &opt);
    }

    SC_METHOD(ClearUTTBBFBitMethod);
    dont_initialize();
    sensitive << mClearUTTBBFBitEvent;

    SC_METHOD(SetUTTFINBitMethod);
    dont_initialize();
    sensitive << mSetUTTFINBitEvent;

    SC_METHOD(Writesw_rst_sigMethod);
    sensitive << mWritesw_rst_sigEvent;

    SC_METHOD(Negatesw_rst_sigMethod);
    dont_initialize();
    sensitive << mNegatesw_rst_sigEvent;
}//}}}

/// Destructor
PSIS011::~PSIS011()
{//{{{
    delete mpPSIS011Func;
    delete mpTimestampA;
    delete mpTimestampB;
    for (unsigned int i = 0; i < emNumChannel; i++){
        delete mpSyncPulseChannel[i];
        delete mpWDTChannel[i];
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        delete int_psis_chn[i];
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        delete dma_psis_chn_rx[i];
    }
    for (unsigned int i = 0; i < (emNumChannel - 1); i++){
        delete dma_psis_chn_tx[i];
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        delete psis_trg_sync_chn[i];
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        delete selected_sync_pulse_sig_chn[i];
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        delete wdt_error_sig_chn[i];
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        delete tx_req_sig[i];
    }
}//}}}

/// Initialize when PRESETn activated
void PSIS011::InitializeOfPRESETn(void)
{//{{{
    for (unsigned int i = 0; i < emNumChannel; i++){
        mChannel[i].Initialize();
        mReqArray[i] = 0;
        mInterrupt[i] = 0;
        mDMARequestRX[i] = 0;
        mWriteTxReqEvent[i].notify(SC_ZERO_TIME);
    }
    for (unsigned int i = 0; i < (emNumChannel-1); i++){
        mDMARequestTX[i] = 0;
    }
    mMode = emConfigurationMode;
    mTSInnerCycle = 0;
    mMaxIdleCycle = 0;
    mURPRTY = 0;
    mUTPRTY = 0;
    mDivider = 0;
    mTxCLKDivider = 0;
    mRX_CONTROL = 0;
    mRX_DATA = 0;
    mReceptionState = emIdleReceptionState;
    mFPKT = 0;
    mSCKEN = 0;
    mNFSET = 0;
    mFPAYLD = 0;
    mRFCPS = 0;
    mUTTF = 0;
    mPSI5Packet = 0;
    mHasWriteUCTD = 0;
    mNumTXReqInFIFO = 0;
    mLoopbackEnable = false;
    mIgnorePacket = false;
    
    while (mReqFIFO.size() > 0){
        mReqFIFO.pop();
    }
}//}}}

/// Initialize when psis_rst_n activated
void PSIS011::InitializeOfpsis_rst_n(void)
{//{{{
    for (unsigned int i = 0; i < emNumChannel; i++){
        mChannel[i].Initialize();
        mReqArray[i] = 0;
    }
    for (unsigned int i = 0; i < (emNumChannel-1); i++){
        mIsStopDDSR[i] = true;
    }
    mRX_CONTROL = 0;
    mRX_DATA = 0;
    mReceptionState = emIdleReceptionState;
    mDNF_RX_CONTROL = 0;
    mDNF_RX_DATA = 0;
    mRxUARTFrmNum = 0;
    mLostUARTFrame = false;
    mIsUARTPrityError = 0;
    mIsUARTFramingError = 0;
    mCurChid = 0;
    mCurFid = 0;
    mCurHeaderStatus = 0;
    mPSI5Packet = 0;
    mTX_CONTROL = 0;
    mTX_DATA = 0;
    mNumTXReqInFIFO = 0;
    mHasPSI5ReceiveError = false;
    mIgnorePacket = false;
    
    while (mReqFIFO.size() > 0){
        mReqFIFO.pop();
    }

    // Clear trans status bit
    mClearTXSTSBitEvent.notify(SC_ZERO_TIME);
    mClearUTTBBFBitEvent.notify(SC_ZERO_TIME);
    for (unsigned int i = 0; i < (emNumChannel - 1); i++){
        mClearDDSRSTSBitEvent[i].notify(SC_ZERO_TIME);
    }
    mpPSIS011Func->UpdatePSI5SUCTMReg("UTTF", 0);
}//}}}

/// Initialize when psis_mult_rst_n activated
void PSIS011::InitializeOfpsis_mult_rst_n(void)
{//{{{
    mTSInnerCycle = 0;
}//}}}

/// Create table of valid FPKT vs FPAYLD
void PSIS011::CreateValidFPKTvsFPAYLDTable()
{//{{{
    for (unsigned int fpayld = 8; fpayld < 29; fpayld++){
        for (unsigned int rfcps = 0; rfcps < 2; rfcps++){
            unsigned int fpkt = 0;
            if (rfcps == 0){
                if ((8 <= fpayld) && (fpayld <= 9)){
                    fpkt = 3;
                }else if ((10 <= fpayld) && (fpayld <= 17)){
                    fpkt = 4;
                }else if ((18 <= fpayld) && (fpayld <= 25)){
                    fpkt = 5;
                }else {
                    fpkt = 6;
                }
            }else{// rfcps == 1
                if ((8 <= fpayld) && (fpayld <= 15)){
                    fpkt = 4;
                }else if ((16 <= fpayld) && (fpayld <= 23)){
                    fpkt = 5;
                }else {
                    fpkt = 6;
                }
            }
            mValidFPKTvsFPAYLDTable[fpayld - 8 + 21*rfcps] = (fpayld << 4) | (rfcps << 3) | fpkt;
        }
    }
}//}}}

/// Create Strobe string map
void PSIS011::CreateStrobeStringMap()
{//{{{
    mStrobeStringMap.insert(std::pair<unsigned int, std::string>(0, "IDLE"));
    mStrobeStringMap.insert(std::pair<unsigned int, std::string>(1, "START"));
    mStrobeStringMap.insert(std::pair<unsigned int, std::string>(2, "STOP"));
    mStrobeStringMap.insert(std::pair<unsigned int, std::string>(3, "ABORT"));
}//}}}

//============ Overwrite func. of agent ==================
/// Get reset status
bool PSIS011::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if(reset_name == "PRESETn") {
        if (mIsResetHardActive[emPRESETnId] || mIsResetCmdActive[emPRESETnId]) {
            reset_status = true;
        }
    }else if (reset_name == "psis_rst_n"){
        if (mIsResetHardActive[empsis_rst_nId] || mIsResetCmdActive[empsis_rst_nId]) {
            reset_status = true;
        }
    }else{// psis_mult_rst_n
        if (mIsResetHardActive[empsis_mult_rst_nId] || mIsResetCmdActive[empsis_mult_rst_nId]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

/// Check clock period 
bool PSIS011::CheckClockPeriod (const std::string clock_name)
{//{{{ 
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;

    if (clock_name == "PCLK"){
        clock_period = mClkPeriod[emPCLKId];
    }else if (clock_name == "psis_clk"){
        clock_period = mClkPeriod[empsis_clkId];
    }else{// psis_mult_clk
        clock_period = mClkPeriod[empsis_mult_clkId];
    }

    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }

    return clock_status;
}//}}} 

/// Software reset
void PSIS011::SoftwareReset()
{//{{{
    re_printf("info","The Software reset is asserted.\n");
    mSWRST = 0; // active
    mWritesw_rst_sigEvent.notify();
}//}}}

/// Get PCLK frequency
sc_dt::uint64 PSIS011::GetPCLKFreq()
{//{{{
    return mClkFreq[emPCLKId];
}//}}}

/// Clear timestamp A/B
void PSIS011::ClearTimestamp(std::string timestamp_name)
{//{{{
    assert ((timestamp_name == "A") || (timestamp_name == "B"));

    if (timestamp_name == "A"){
        mpTimestampA->NotifyClearTimestamp();
    }else{ // timestamp_name == "B"
        mpTimestampB->NotifyClearTimestamp();
    }
}//}}}

/// Config timestamp A/B
void PSIS011::ConfigTimestamp(std::string timestamp_name, bool clear_select_gtm, bool enable_select_gtm, bool clock_select_gtm)
{//{{{
    assert ((timestamp_name == "A") || (timestamp_name == "B"));
    if (timestamp_name == "A"){
        mpTimestampA->ConfigTimestamp(clear_select_gtm, enable_select_gtm, clock_select_gtm);
    }else{// timestamp_name == "B"
        mpTimestampB->ConfigTimestamp(clear_select_gtm, enable_select_gtm, clock_select_gtm);
    }
}//}}}

/// Set timestamp enable or not via setting of inner register
void PSIS011::SetTSInnerEnable(std::string timestamp_name, bool ts_inner_enable)
{//{{{
    assert ((timestamp_name == "A") || (timestamp_name == "B"));
    if (timestamp_name == "A"){
        mpTimestampA->SetTSInnerEnable(ts_inner_enable);
    }else{// timestamp_name == "B"
        mpTimestampB->SetTSInnerEnable(ts_inner_enable);
    }
}//}}}

/// Set timestamp inner cycle (PCLK) -> generate inner clock for timestamp counting up
void PSIS011::SetTSInnerCycle(unsigned int inner_cycle)
{//{{{
    mTSInnerCycle = inner_cycle;
    mGenClockForTimestampEvent.notify();
}//}}}

/// Inform the next change of operation mode
void PSIS011::InformChangeMode(unsigned int open, unsigned int opmd)
{//{{{
    if (open == 0){
        re_printf("info", "Move to Configuration mode.\n");
        mMode = emConfigurationMode;
        // Clear some temp variables
        for (unsigned int i = 0; i < emNumChannel; i++){
            mChannel[i].Initialize();
            mReqArray[i] = 0;
            mInterrupt[i] = 0;
            mDMARequestRX[i] = 0;
        }
        for (unsigned int i = 0; i < (emNumChannel-1); i++){
            mDMARequestTX[i] = 0;
        }
        mReceptionState = emIdleReceptionState;
        mPSI5Packet = 0;
        mRxUARTFrmNum = 0;
        mLostUARTFrame = false;
        mHasPSI5ReceiveError = false;
        mIsUARTPrityError = 0;
        mIsUARTFramingError = 0;
        mCurChid = 0;
        mCurFid = 0;
        mCurHeaderStatus = 0;
        mHasWriteUCTD = 0;
        mNumTXReqInFIFO = 0;

        // Call sub-module to stop generating sync pulse, WDT counter
        for (unsigned int i = 1; i < emNumChannel; i++){// Not for channel 0
            mpSyncPulseChannel[i]->StopGenInnerSyncPulse();
            mpWDTChannel[i]->StopWDT();
        }
        // Clear all status, variable according operation
        while (mReqFIFO.size() > 0){ // Clear TX request FIFO
            mReqFIFO.pop();
        }
    }else{
        if (opmd == 0){
            mMode = emUARTMode;
            re_printf("info", "Move to UART mode.\n");
        }else{
            mMode = emPSI5Mode;
            re_printf("info", "Move to PSI5 mode.\n");

            // Call sub-module to config and start generating sync pulse (channel 1-7, channel 0 not transmit->not need)
            for (unsigned int i = 1; i < emNumChannel; i++){
                unsigned int select = 0;
                unsigned int prescaler = 0;
                unsigned int expired = 0;
                mpPSIS011Func->GetConfigInnerSyncPulse(i, select, prescaler, expired);
                mpSyncPulseChannel[i]->ConfigInnerSyncPulse(select, prescaler, expired);
                mpSyncPulseChannel[i]->StartGenInnerSyncPulse();
            }
            // Call sub-module to config and start WDT if any (for 8 channels)
            for (unsigned int i = 0; i < emNumChannel; i++){
                unsigned int syncmode = 0;
                unsigned int prescaler = 0;
                unsigned int expired = 0;
                mpPSIS011Func->GetConfigWDT(i, syncmode, prescaler, expired, mChannel[i].is_wdt_enabled);
                mChannel[i].is_sync = !syncmode;
                mpWDTChannel[i]->ConfigWDTCounter(prescaler, expired);
                // Start WDT if in Async mode and operate with PSI5
                if (!(mChannel[i].is_sync) && (mChannel[i].is_wdt_enabled)){
                    mpWDTChannel[i]->StartWDT();
                    mChannel[i].wdt_error = false;
                }
            }
            // Get config of timestamp
            for (unsigned int i = 0; i < emNumChannel; i++){
                mpPSIS011Func->GetConfigTimestamp(i, mChannel[i].ch_en, mChannel[i].ts_sel, mChannel[i].ts_en, mChannel[i].ts_trg_sel);
            }
            // Gen clock for internal timestamp if any
            mGenClockForTimestampEvent.notify();
            mMaxIdleCycle = mpPSIS011Func->GetMaxIdle();
        }
        unsigned int pupts = mpPSIS011Func->GetPSI5SPUPTSReg();
        mURPRTY = pupts & 0x3;
        mUTPRTY = (pupts >> 8) & 0x3;
        // Calculate time for 1 bit
        mpPSIS011Func->GetClockDivider(mDivider, mTxCLKDivider);
        // Get NFSET bit
        mNFSET = mpPSIS011Func->GetNFSETBit();
        mLoopbackEnable = mpPSIS011Func->GetLoopbackEnable();
    }
}//}}}

/// Send transmission request to Arbiter according
void PSIS011::NotifySendReqTxToArbiter(unsigned int reqid)
{//{{{
    mReqArray[reqid] = true;
    if (reqid == 7){
        mWriteTxReqEvent[reqid].notify();
    }else{
        mIsStopDDSR[reqid] = false;
    }
}//}}}

/// Notify to transmit UART frame
void PSIS011::NotifyTransUART()
{//{{{
    mHasWriteUCTD = true;
    if (mUTTF == 0){
        mTransUARTDataEvent.notify();
    }
}//}}}

/// Inform to assert interrupt according (called from Func)
void PSIS011::AssertInterrupt(unsigned int intrid)
{//{{{
    mInterrupt[intrid] = true;
    mWriteInterruptEvent[intrid].notify();
}//}}}

/// Inform to assert DMA request RX according (called from Func)
void PSIS011::AssertDMARequestRX(unsigned int dmarxid)
{//{{{
    mDMARequestRX[dmarxid] = true;
    mWriteDMARequestRXEvent[dmarxid].notify();
}//}}}

/// Inform to assert DMA request TX according (called from Func)
void PSIS011::AssertDMARequestTX(unsigned int dmatxid)
{//{{{
    mDMARequestTX[dmatxid] = true;
    mWriteDMARequestTXEvent[dmatxid].notify();
}//}}}

/// Change WDT enalbe attribute of a channel
void PSIS011::ChangeWDTEnableChannel(unsigned int chid, unsigned int enable)
{//{{{
    mChannel[chid].is_wdt_enabled = enable;
}//}}}

/// Inform change value of SCKEN bit
void PSIS011::InformChangeSCKENBit(unsigned int value)
{//{{{
    mSCKEN = value;
}//}}}

/// Stop transmit DDSR of a channel
void PSIS011::StopTransDDSR(unsigned int reqid)
{//{{{
    mIsStopDDSR[reqid] = true;
    mStopDDSREvent[reqid].notify();
}//}}}

//============ Reset and clock  ==========================
/// Monitor PRESETn port
void PSIS011::MonitorPRESETnMethod()
{//{{{
    mResetCurVal[emPRESETnId] = (PRESETn.read()) & (sw_rst_sig.read());
    if (mResetCurVal[emPRESETnId] != mResetPreVal[emPRESETnId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[emPRESETnId] = mResetCurVal[emPRESETnId];
        // Reset signals are asynchronous
        mResetHardEvent[emPRESETnId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Monitor psis_rst_n port
void PSIS011::Monitorpsis_rst_nMethod()
{//{{{
    mResetCurVal[empsis_rst_nId] = (psis_rst_n.read()) & (sw_rst_sig.read());
    if (mResetCurVal[empsis_rst_nId] != mResetPreVal[empsis_rst_nId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[empsis_rst_nId] = mResetCurVal[empsis_rst_nId];
        // Reset signals are asynchronous
        mResetHardEvent[empsis_rst_nId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Monitor psis_mult_rst_n port
void PSIS011::Monitorpsis_mult_rst_nMethod()
{//{{{
    mResetCurVal[empsis_mult_rst_nId] = (psis_mult_rst_n.read()) & (sw_rst_sig.read());
    if (mResetCurVal[empsis_mult_rst_nId] != mResetPreVal[empsis_mult_rst_nId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[empsis_mult_rst_nId] = mResetCurVal[empsis_mult_rst_nId];
        // Reset signals are asynchronous
        mResetHardEvent[empsis_mult_rst_nId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Write sw_rst_sig signal method
void PSIS011::Writesw_rst_sigMethod()
{//{{{
    sw_rst_sig.write(mSWRST);
    if (mSWRST == 0){// active => then negate
        mNegatesw_rst_sigEvent.notify(8*(double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
    }
}//}}}

/// Negate Software reset
void PSIS011::Negatesw_rst_sigMethod()
{//{{{
    re_printf("info","The Software reset is de-asserted.\n");
    mSWRST = 1;
    mWritesw_rst_sigEvent.notify();
    mpPSIS011Func->SetSWSTSBit(0);
}//}}}

/// Process reset for PRESETn, psis_rst_n, psis_mult_rst_n
void PSIS011::EnableReset(const std::string reset_name, const bool is_active)
{//{{{
    sc_assert ((reset_name == "PRESETn") || (reset_name == "psis_rst_n") || (reset_name == "psis_mult_rst_n"));
    if (reset_name == "PRESETn"){
        EnablePRESETn(is_active);
        mResetEvent[emPRESETnId].notify();
    }else if (reset_name == "psis_rst_n"){
        Enablepsis_rst_n(is_active);
        mResetEvent[empsis_rst_nId].notify();
    }else{
        Enablepsis_mult_rst_n(is_active);
        mResetEvent[empsis_mult_rst_nId].notify();
    }
}//}}}

/// Reset model and its sub-instance if PRESETn is active
void PSIS011::EnablePRESETn(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events
        CancelEventsOfPRESETn();
        // Initialize variables
        InitializeOfPRESETn();
        // Initialize output ports
        for (unsigned int i = 0; i < emNumChannel; i++){
            mWriteInterruptEvent[i].notify();
            mWriteDMARequestRXEvent[i].notify();
        }
        for (unsigned int i = 0; i < (emNumChannel - 1); i++){
            mWriteDMARequestTXEvent[i].notify();
        }
        mWriteUARTOutputEvent.notify();
    }
    mpPSIS011Func->EnableReset(is_active);
    mpTimestampA->EnableReset(is_active);
    mpTimestampB->EnableReset(is_active);
    for (unsigned int i = 0; i < emNumChannel; i++){
        mpSyncPulseChannel[i]->EnableReset(is_active);
    }
    // Keep value of SWSTS bit for case of Software reset
    if ((mSWRST == 0) && is_active){
        mpPSIS011Func->SetSWSTSBit(1);
    }
}//}}}

/// Reset model and its sub-instance if psis_rst_n is active
void PSIS011::Enablepsis_rst_n(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events
        CancelEventsOfpsis_rst_n();
        // Initialize variables
        InitializeOfpsis_rst_n();
        mWriteUARTOutputEvent.notify();
    }
    for (unsigned int i = 0; i < emNumChannel; i++){
        mpWDTChannel[i]->EnableReset(is_active);
    }
}//}}}

/// Reset model and its sub-instance if psis_mult_rst_n is active
void PSIS011::Enablepsis_mult_rst_n(const bool is_active)
{//{{{
    if (is_active){
        // Cancel events
        CancelEventsOfpsis_mult_rst_n();
        // Initialize variables
        InitializeOfpsis_mult_rst_n();
    }
    mpTimestampA->EnableReset(is_active);
    mpTimestampB->EnableReset(is_active);
    for (unsigned int i = 0; i < emNumChannel; i++){
        mpSyncPulseChannel[i]->EnableReset(is_active);
    }
}//}}}

/// Cancel event related to PRESETn domain
void PSIS011::CancelEventsOfPRESETn()
{//{{{
    mGenClockForTimestampEvent.cancel();
    mClearTXSTSBitEvent.cancel();
    mClearUTTBBFBitEvent.cancel();
    mSetUTTFINBitEvent.cancel();
    for (unsigned int i = 0; i < emNumChannel; i++){
        mWriteTxReqEvent[i].cancel();
        mNegateTxReqEvent[i].cancel();
        mWriteInterruptEvent[i].cancel();
        mNegateInterruptEvent[i].cancel();
        mWriteDMARequestRXEvent[i].cancel();
        mNegateDMARequestRXEvent[i].cancel();
    }
    for (unsigned int i = 0; i < (emNumChannel - 1); i++){
        mClearDDSRSTSBitEvent[i].cancel();
        mWriteDMARequestTXEvent[i].cancel();
        mNegateDMARequestTXEvent[i].cancel();
    }
    mWritesw_rst_sigEvent.cancel();
}//}}}

/// Cancel event related to psis_rst_n domain
void PSIS011::CancelEventsOfpsis_rst_n()
{//{{{
    mForwardDNFUARTRxEvent.cancel();
    mEndIdleReceptionStateEvent.cancel();
    mMoveToStopReceptionStateEvent.cancel();
    mMoveToIdleReceptionStateEvent.cancel();
    mTransUARTDataEvent.cancel();
    mTransPSI5Event.cancel();
    mWriteUARTOutputEvent.cancel();
    for (unsigned int i = 0; i < emNumChannel; i++){
        mWriteTxReqEvent[i].cancel();
        mNegateTxReqEvent[i].cancel();
    }
    for (unsigned int i = 0; i < (emNumChannel -1); i++){
        mStopDDSREvent[i].cancel();
    }
}//}}}

/// Cancel event related to psis_mult_rst_n domain
void PSIS011::CancelEventsOfpsis_mult_rst_n()
{//{{{ 
    mGenClockForTimestampEvent.cancel();
}//}}}

/// Monitor PCLK clock
void PSIS011::MonitorPCLKMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

/// Monitor psis_clk clock
void PSIS011::Monitorpsis_clkMethod()
{//{{{
    sc_dt::uint64 freq_value = psis_clk.read();
    SetCLKFreq("psis_clk", freq_value, "Hz");
}//}}}

/// Monitor psis_mult_clk clock
void PSIS011::Monitorpsis_mult_clkMethod()
{//{{{
    sc_dt::uint64 freq_value = psis_mult_clk.read();
    SetCLKFreq("psis_mult_clk", freq_value, "Hz");
}//}}}

/// Check frequency value and frequency unit
void PSIS011::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in)
{//{{{
    if(unit_in == "GHz"){
        freq_out = freq_in * (sc_dt::uint64)(1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

/// Process reset function when reset port is active
void PSIS011::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
        } else if (reset_id == empsis_rst_nId) {
            re_printf("info","The reset port psis_rst_n is asserted.\n");
            this->EnableReset("psis_rst_n", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port psis_mult_rst_n is asserted.\n");
            this->EnableReset("psis_mult_rst_n", mIsResetHardActive[reset_id]);
        }
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            if (reset_id == emPRESETnId) {
                re_printf("info","The reset port PRESETn is de-asserted.\n");
                this->EnableReset("PRESETn", mIsResetHardActive[reset_id]);
            } else if (reset_id == empsis_rst_nId) {
                re_printf("info","The reset port psis_rst_n is de-asserted.\n");
                this->EnableReset("psis_rst_n", mIsResetHardActive[reset_id]);
            } else {
                re_printf("info","The reset port psis_mult_rst_n is de-asserted.\n");
                this->EnableReset("psis_mult_rst_n", mIsResetHardActive[reset_id]);
            }
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

/// Process reset function when reset command is active
void PSIS011::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else if (reset_id == empsis_rst_nId) {
        reset_name = "psis_rst_n";
    } else { //reset_id == empsis_mult_rst_nId
        reset_name = "psis_mult_rst_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when reset command is active
void PSIS011::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else if (reset_id == empsis_rst_nId) {
        reset_name = "psis_rst_n";
    } else { //reset_id == empsis_mult_rst_nId
        reset_name = "psis_mult_rst_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

/// Get time resolution
void PSIS011::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        resolution_value = 1000000000000000LL;
        resolution_unit = SC_FS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        resolution_value = 1000000000000LL;
        resolution_unit = SC_PS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        resolution_value = 1000000000;
        resolution_unit = SC_NS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        resolution_value = 1000000;
        resolution_unit = SC_US;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        resolution_value = 1000;
        resolution_unit = SC_MS;
    } else {
        resolution_value = 1;
        resolution_unit = SC_SEC;
    }
}//}}}

/// Calculate the synchronous time with clock
double PSIS011::CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point)
{//{{{
    // Note: currently, this function is called for PCLK only.
    // If this function is used for other clock, please add code similar with processing for PCLK.

    double period_num = 0;
    if (clock_name == "PCLK") {
        if (mClkPeriod[emPCLKId] == 0){
            return 0;
        }
        period_num = (time_point - mStartClkTime[emPCLKId])/ (double)mClkPeriod[emPCLKId];
    }
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (!add_period) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "PCLK") {
        pos_edge_point = mStartClkTime[emPCLKId] + (double)mod_period_num * (double)mClkPeriod[emPCLKId] - time_point;
    }
    return pos_edge_point;
}//}}}

//============ Operation               ===================
/// Process counting timestamp
void PSIS011::GenClockForTimestampThread()
{//{{{
    while (true){
        // Not generate if mode is not PSI5
        if (mMode != emPSI5Mode){
            wait();
            continue;
        }

        // Gen clock based on register setting
        double wait_time = CalculateCLKPosEdge("PCLK", false, sc_time_stamp().to_double()) + mTSInnerCycle*mClkPeriod[emPCLKId];
        if (wait_time == 0){
            wait();
            continue;
        }
        wait(wait_time, mTimeResolutionUnit, mResetEvent[emPRESETnId] | mClkZeroEvent[emPCLKId]
                | mResetEvent[empsis_mult_rst_nId] | mClkZeroEvent[empsis_mult_clkId]);

        // Not operate if PRESETn active, or PCLK = 0
        if (GetResetStatus("PRESETn") || (CheckClockPeriod("PCLK") == false)){
            wait();
            continue;
        }

        // Not operate if psis_mult_rst_n active, or psis_mult_clk = 0
        if (GetResetStatus("psis_mult_rst_n") || (CheckClockPeriod("psis_mult_clk") == false)){
            wait();
            continue;
        }

        timestamp_inner_clock_sig.write(1);
        wait ((double)mClkPeriod[emPCLKId], mTimeResolutionUnit, mResetEvent[emPRESETnId] | mClkZeroEvent[emPCLKId]
                | mResetEvent[empsis_mult_rst_nId] | mClkZeroEvent[empsis_mult_clkId]);// Pulse width = 1 PCLK
        // Not operate if PRESETn active, or PCLK = 0
        if (GetResetStatus("PRESETn") || (CheckClockPeriod("PCLK") == false)){
            wait();
            continue;
        }

        // Not operate if psis_mult_rst_n active, or psis_mult_clk = 0
        if (GetResetStatus("psis_mult_rst_n") || (CheckClockPeriod("psis_mult_clk") == false)){
            wait();
            continue;
        }
        timestamp_inner_clock_sig.write(0);
    }
}//}}}

/// Monitor selected sync pulse
void PSIS011::MonitorSyncPulseMethod(unsigned int chid)
{//{{{
    // Not operate if PRESETn active, or PCLK = 0
    if (GetResetStatus("PRESETn") || (CheckClockPeriod("PCLK") == false)){
        return;
    }

    // Not operate if psis_mult_rst_n active, or psis_mult_clk = 0
    if (GetResetStatus("psis_mult_rst_n") || (CheckClockPeriod("psis_mult_clk") == false)){
        return;
    }

    if (selected_sync_pulse_sig_chn[chid]->read() == true){
        // Start WDT if channel in sync mode and WDT is enabled
        if ((mChannel[chid].is_sync) && (mChannel[chid].is_wdt_enabled)){
            mpWDTChannel[chid]->StartWDT();
            mChannel[chid].wdt_error = false;
        }

        // Capture timestamp if trigger select is sync pulse
        if ((mChannel[chid].is_sync) && (mChannel[chid].ts_trg_sel == 0)){
            unsigned int timestamp = this->GetTimestamp(chid);
            mpPSIS011Func->CaptureTimestamp(chid, timestamp);
        }

        // Clear wdt_frm_cnt of channel if channel in sync mode
        if (mChannel[chid].is_sync){
            mChannel[chid].wdt_frm_cnt = 0;
        }

        // Raise transmission request if any (in channel 1-7) in case channel in sync mode
        if ((mChannel[chid].is_sync) && (mReqArray[chid-1])){// Array index 0-6 according channel 1-7
            mWriteTxReqEvent[chid-1].notify();
        }
    }
}//}}}

/// Write Tx request to arbiter
void PSIS011::WriteTxReqMethod(unsigned int reqid)
{//{{{
    tx_req_sig[reqid]->write(mReqArray[reqid]);
    if (mReqArray[reqid]){
        if (reqid == 7){
            re_printf("info", "Issue request of CPU Tx Command to TX Request arbiter\n", reqid);
        }else {
            re_printf("info", "Issue request of channel%d to TX Request arbiter\n", reqid+1);
        }
        mNegateTxReqEvent[reqid].notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
    }
}//}}}

/// Negate Tx request
void PSIS011::NegateTxReqMethod(unsigned int reqid)
{//{{{
    mReqArray[reqid] = false;
    mWriteTxReqEvent[reqid].notify();
}//}}}

/// Issue PSI5 packet in PSI5 mode as UART frame to outside
void PSIS011::TransPSI5Thread()
{//{{{
    while (true){
        // Not operate if reset or stop clock
        if ( GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false)
            || GetResetStatus("PRESETn") || (CheckClockPeriod("PCLK") == false) ){
            wait();
            continue;
        }
        unsigned int tx_control_val = 0;
        unsigned int numbit = 0;
        while (mReqFIFO.size() > 0){
            unsigned int txid = mReqFIFO.front();
            mReqFIFO.pop();

            // == CPU Tx command == 
            if (txid == 7){
                unsigned int numfrm = 0;
                sc_dt::uint64 tx_cpu_com_data = 0;
                mpPSIS011Func->GetConfigCPUTxCom(numfrm, tx_cpu_com_data);
                numbit = 8; // defaut 8 bit (no parity)
                
                // Prepare UART frame
                for (unsigned int i = 0; i < numfrm; i++){
                    if ( GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false)
                        || GetResetStatus("PRESETn") || (CheckClockPeriod("PCLK") == false) ){
                        break;
                    }
                    mNumTXReqInFIFO --;
                    UpdateTXFIFOStatus(); // update TXFIFO status
                    unsigned int tdt = (tx_cpu_com_data >> i*8) & 0xFF; // Get 1 byte to transfer
                    unsigned int tc = 0;
                    tdt = PrepareUARTFrame(tdt, mUTPRTY, numbit);

                    // Indicate transfer complete in TX_CONTROL
                    if (i == (numfrm -1)){
                        tc = 1;
                    }

                    // Send start strobe
                    unsigned int tx_control_val = PrepareTXCONTROL(emStartStrobe, numbit, tc);
                    mTX_CONTROL = tx_control_val;
                    mTX_DATA = tdt;
                    re_printf("info", "(PSI5 mode) (CPU Tx cmd) Transmit START, TX_DATA = %X, TX_CONTROL = %X\n", tdt, tx_control_val);
                    mWriteUARTOutputEvent.notify();
                    // Set 1 to ISTCTFN bit
                    if (i == (numfrm -1)){
                        mpPSIS011Func->SetPSI5SPCISReg(0, "CTFN");// Set for channel 0 because this bit in channel 0 only
                    }

                    // Wait ((numbit+1) * divider) clock (for start bit and data bits) 
                    double wait_time = (numbit + 1) * mDivider * (double)mClkPeriod[empsis_clkId];// +1 for start bit
                    wait(wait_time, mTimeResolutionUnit, mResetEvent[empsis_rst_nId] | mClkZeroEvent[empsis_clkId]);

                    if (GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false)){
                        tx_control_val = PrepareTXCONTROL(emAbortStrobe, numbit, tc);
                        mTX_CONTROL = tx_control_val;
                        re_printf("info", "(PSI5 mode) (CPU Tx cmd) Transmit ABORT, TX_DATA = %X, TX_CONTROL = %X\n", tdt, tx_control_val);
                        mWriteUARTOutputEvent.notify();
                        break;
                    }

                    // Send stop bit
                    tx_control_val = PrepareTXCONTROL(emStopStrobe, numbit, tc);
                    mTX_CONTROL = tx_control_val;
                    re_printf("info", "(PSI5 mode) (CPU Tx cmd) Transmit STOP, TX_DATA = %X, TX_CONTROL = %X\n", tdt, tx_control_val);
                    mWriteUARTOutputEvent.notify();
                    if ((numfrm == 1) || (i == (numfrm -2))){// Update TXSTS bit at near last frame at rising edge of PCLK
                        mClearTXSTSBitEvent.notify(CalculateCLKPosEdge("PCLK", false, sc_time_stamp().to_double()), mTimeResolutionUnit);
                    }

                    // Wait 1 numbit * divider clock (1 period for stop bit)
                    wait_time = mDivider * (double)mClkPeriod[empsis_clkId];
                    wait(wait_time, mTimeResolutionUnit, mResetEvent[empsis_rst_nId] | mClkZeroEvent[empsis_clkId]);

                    if (GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false)){
                        tx_control_val = PrepareTXCONTROL(emAbortStrobe, numbit, tc);
                        mTX_CONTROL = tx_control_val;
                        re_printf("info", "(PSI5 mode) (CPU Tx cmd) Transmit ABORT, TX_DATA = %X, TX_CONTROL = %X\n", tdt, tx_control_val);
                        mWriteUARTOutputEvent.notify();
                        break;
                    }
                }
                // Send idle bit if the last byte
                if (mReqFIFO.size() == 0){
                    tx_control_val = PrepareTXCONTROL(emIdleStrobe, numbit, 0);
                    mTX_CONTROL = tx_control_val;
                    mTX_DATA = 0;
                    re_printf("info", "(PSI5 mode) (CPU Tx cmd) Transmit IDLE, TX_DATA = %X, TX_CONTROL = %X\n", 0, tx_control_val);
                    mWriteUARTOutputEvent.notify();
                }

            // == DDSR == for channel 1-7 
            }else{
                mNumTXReqInFIFO --;
                UpdateTXFIFOStatus(); // update TXFIFO status
                
                // Not run if channel is not enabled
                if (! mIsStopDDSR[txid]){
                    // Calc DDSR
                    sc_dt::uint64 ddsr = 0;
                    unsigned int length = 0;
                    numbit = 0;
                    PrepareDDSRData (txid + 1, ddsr, length);// txid 0-6 for channel 1-7

                    // Reverse all ddsr as LSB first
                    sc_dt::uint64 lsb_ddsr = Reverse(ddsr, length);

                    // Shift bit by bit, and choose the command/alter command.
                    unsigned int command_data = mpPSIS011Func->GetCommandData(txid + 1); // channel id = txid + 1
                    unsigned int tx_alt_cmd = PrepareUARTFrame((command_data >> 8) & 0xFF, mUTPRTY, numbit);
                    unsigned int tx_cmd = PrepareUARTFrame(command_data & 0xFF, mUTPRTY, numbit);
                    for (unsigned int i = 0; i < length; i++){
                        if ( GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false)
                            || GetResetStatus("PRESETn") || (CheckClockPeriod("PCLK") == false) ){
                            break;
                        }
                        unsigned int shift_bit = (unsigned int)(lsb_ddsr >> i) & 0x1;
                        unsigned int sel_tx_cmd = 0;
                        unsigned int tc = 0;
                        if (shift_bit == 0){
                            sel_tx_cmd = tx_cmd;
                        }else{
                            sel_tx_cmd = tx_alt_cmd;
                        }
                        if (i == (length -1)){
                            tc = 1;
                        }

                        // Send start strobe
                        tx_control_val = PrepareTXCONTROL(emStartStrobe, numbit, tc);
                        mTX_CONTROL = tx_control_val;
                        mTX_DATA = sel_tx_cmd;
                        re_printf("info", "(PSI5 mode) (Channel %d) Transmit START, TX_DATA = %X, TX_CONTROL = %X\n", txid + 1, sel_tx_cmd, tx_control_val);
                        mWriteUARTOutputEvent.notify();
                        if (i == (length - 1)){ // in last UART frame
                            mClearDDSRSTSBitEvent[txid].notify(CalculateCLKPosEdge("PCLK", true, sc_time_stamp().to_double()), mTimeResolutionUnit);
                        }
                        // Wait (numbit * divider) clock
                        double wait_time = (numbit+1) * mDivider * (double)mClkPeriod[empsis_clkId];
                        wait(wait_time, mTimeResolutionUnit, mResetEvent[empsis_rst_nId] | mClkZeroEvent[empsis_clkId] | mStopDDSREvent[txid]);

                        if (GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false) || mIsStopDDSR[txid]){
                            tx_control_val = PrepareTXCONTROL(emAbortStrobe, numbit, tc);
                            mTX_CONTROL = tx_control_val;
                            re_printf("info", "(PSI5 mode) (Channel %d) Transmit ABORT, TX_DATA = %X, TX_CONTROL = %X\n", txid + 1, sel_tx_cmd, tx_control_val);
                            mWriteUARTOutputEvent.notify();
                            break;
                        }

                        // Send stop bit
                        tx_control_val = PrepareTXCONTROL(emStopStrobe, numbit, tc);
                        mTX_CONTROL = tx_control_val;
                        re_printf("info", "(PSI5 mode) (Channel %d) Transmit STOP, TX_DATA = %X, TX_CONTROL = %X\n", txid + 1, sel_tx_cmd, tx_control_val);
                        mWriteUARTOutputEvent.notify();
                        // Wait 1 numbit * divider clock
                        wait_time = mDivider * (double)mClkPeriod[empsis_clkId];
                        wait(wait_time, mTimeResolutionUnit, mResetEvent[empsis_rst_nId] | mClkZeroEvent[empsis_clkId] | mStopDDSREvent[txid]);

                        if (GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false) || mIsStopDDSR[txid]){
                            tx_control_val = PrepareTXCONTROL(emAbortStrobe, numbit, tc);
                            mTX_CONTROL = tx_control_val;
                            re_printf("info", "(PSI5 mode) (Channel %d) Transmit ABORT, TX_DATA = %X, TX_CONTROL = %X\n", txid + 1, sel_tx_cmd, tx_control_val);
                            mWriteUARTOutputEvent.notify();
                            break;
                        }
                    }
                }
                // Send idle bit if the last byte
                if (mReqFIFO.size() == 0){
                    tx_control_val = PrepareTXCONTROL(emIdleStrobe, numbit, 0);
                    mTX_CONTROL = tx_control_val;
                    mTX_DATA = 0;
                    re_printf("info", "(PSI5 mode) (Channel %d) Transmit IDLE, TX_DATA = %X, TX_CONTROL = %X\n", txid + 1, 0, tx_control_val);
                    mWriteUARTOutputEvent.notify();
                }
            }
        }
        wait();
    }
}//}}}

/// Issue UART frame in UART mode to outside
void PSIS011::TransUARTThread()
{//{{{ 
    while (true){
        // Not operate if reset or stop clock
        if (GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false)){
            wait();
            continue;
        }
        while (mHasWriteUCTD){
            unsigned int numbit = 0; // 8 bits as default (no parity)
            unsigned int trans_data = mpPSIS011Func->GetUTTDTBit();
            trans_data = PrepareUARTFrame(trans_data, mUTPRTY, numbit);

            // Send start strobe
            unsigned int tx_control_val = PrepareTXCONTROL(emStartStrobe, numbit, 1);
            mTX_CONTROL = tx_control_val;
            mTX_DATA = trans_data;
            re_printf("info", "(UART mode) Transmit START, TX_DATA = %X, TX_CONTROL = %X\n", trans_data, tx_control_val);
            mWriteUARTOutputEvent.notify();
            // Update status bit according
            mpPSIS011Func->UpdatePSI5SUCTMReg("UTTF", 1);
            mUTTF = 1;
            mHasWriteUCTD = false;

            // Wait (numbit * divider) clock (for start bit and data bits)
            double wait_time = (numbit+1) * mDivider * (double)mClkPeriod[empsis_clkId];
            wait(wait_time, mTimeResolutionUnit, mResetEvent[empsis_rst_nId] | mClkZeroEvent[empsis_clkId]);

            if (GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false)){
                tx_control_val = PrepareTXCONTROL(emAbortStrobe, numbit, 1);
                mTX_CONTROL = tx_control_val;
                re_printf("info", "(UART mode) Transmit ABORT, TX_DATA = %X, TX_CONTROL = %X\n", trans_data, tx_control_val);
                mWriteUARTOutputEvent.notify();
                break;
            }
            // Update status bit according
            mClearUTTBBFBitEvent.notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
            mSetUTTFINBitEvent.notify(2*(double)mClkPeriod[emPCLKId], mTimeResolutionUnit);

            // Send stop bit
            tx_control_val = PrepareTXCONTROL(emStopStrobe, numbit, 1);
            mTX_CONTROL = tx_control_val;
            re_printf("info", "(UART mode) Transmit STOP, TX_DATA = %X, TX_CONTROL = %X\n", trans_data, tx_control_val);
            mWriteUARTOutputEvent.notify();

            // Wait 1 numbit * divider clock (1 period for stop bit)
            wait_time = mDivider * (double)mClkPeriod[empsis_clkId];
            wait(wait_time, mTimeResolutionUnit, mResetEvent[empsis_rst_nId] | mClkZeroEvent[empsis_clkId]);

            if (GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false)){
                tx_control_val = PrepareTXCONTROL(emAbortStrobe, numbit, 1);
                mTX_CONTROL = tx_control_val;
                re_printf("info", "(UART mode) Transmit ABORT, TX_DATA = %X, TX_CONTROL = %X\n", trans_data, tx_control_val);
                mWriteUARTOutputEvent.notify();
                break;
            }
            // Update status bit according
            if (!mHasWriteUCTD){
                mpPSIS011Func->UpdatePSI5SUCTMReg("UTTF", 0);
                mUTTF = 0;
                // Send idle bit if the last byte
                tx_control_val = PrepareTXCONTROL(emIdleStrobe, numbit, 1);
                mTX_CONTROL = tx_control_val;
                mTX_DATA = 0;
                re_printf("info", "(UART mode) Transmit IDLE, TX_DATA = %X, TX_CONTROL = %X\n", 0, tx_control_val);
                mWriteUARTOutputEvent.notify();
            }
        }
        wait();
    }
}//}}}

/// Write UART output port
void PSIS011::WriteUARTOutputMethod()
{//{{{
    if (mLoopbackEnable){
        mRX_CONTROL = mTX_CONTROL;
        mRX_DATA = mTX_DATA;
        mForwardDNFUARTRxEvent.notify();
    }else{
        TX_CONTROL.write(mTX_CONTROL);
        TX_DATA.write(mTX_DATA);
    }
}//}}}

/// Update TXSTS bit method
void PSIS011::ClearTXSTSBitMethod()
{//{{{
    mpPSIS011Func->SetTXSTSBit(0);
}//}}}

/// Update DDSRSTS bit method
void PSIS011::ClearDDSRSTSBitMethod(unsigned int txreqid)
{//{{{
    unsigned int chid = txreqid + 1; // reqid 0-6 for channel 1-7
    mpPSIS011Func->ClearDDSRSTSBit(chid);
    if (! (GetResetStatus("PRESETn") || (CheckClockPeriod("PCLK") == false) || GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false) )){
        mpPSIS011Func->SetPSI5SPCISReg(chid, "DDSFN");
    }
}//}}}

/// Write interrupt port int_psis_chn (channel 0-7)
void PSIS011::WriteInterruptMethod(unsigned int chid)
{//{{{
    int_psis_chn[chid]->write(mInterrupt[chid]);
    if (mInterrupt[chid]){
        if (mDumpInterrupt){
            re_printf("info", "INT [int_psis_ch%d] assert.\n", chid);
        }
        mNegateInterruptEvent[chid].notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
    }else{
        if (mDumpInterrupt){
            re_printf("info", "INT [int_psis_ch%d] de-assert.\n", chid);
        }
    }
}//}}}

/// Negate interrupt port int_psis_chn (channel 0-7)
void PSIS011::NegateInterruptMethod(unsigned int chid)
{//{{{
    mInterrupt[chid] = false;
    mWriteInterruptEvent[chid].notify();
}//}}}

/// Write DMA request TX port dma_psis_chn_tx (channel 1-7)
void PSIS011::WriteDMARequestTXMethod(unsigned int txreqid)
{//{{{
    dma_psis_chn_tx[txreqid]->write(mDMARequestTX[txreqid]);
    re_printf("info", "dma_psis_ch%d_tx is %d.\n", txreqid+1, (unsigned int)mDMARequestTX[txreqid]);
    if (mDMARequestTX[txreqid]){
        mNegateDMARequestTXEvent[txreqid].notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
    }
}//}}}

/// Negate DMA request TX port dma_psis_chn_tx (channel 1-7)
void PSIS011::NegateDMARequestTXMethod(unsigned int txreqid)
{//{{{
    mDMARequestTX[txreqid] = false;
    mWriteDMARequestTXEvent[txreqid].notify();
}//}}}

/// Write DMA request RX port dma_psis_chn_rx (channel 0-7)
void PSIS011::WriteDMARequestRXMethod(unsigned int chid)
{//{{{
    dma_psis_chn_rx[chid]->write(mDMARequestRX[chid]);
    re_printf("info", "dma_psis_ch%d_rx is %d.\n", chid, (unsigned int)mDMARequestRX[chid]);
    if (mDMARequestRX[chid]){
        mNegateDMARequestRXEvent[chid].notify((double)mClkPeriod[emPCLKId], mTimeResolutionUnit);
    }
}//}}}

/// Negate DMA request RX port dma_psis_chn_rx (channel 0-7)
void PSIS011::NegateDMARequestRXMethod(unsigned int chid)
{//{{{
    mDMARequestRX[chid] = false;
    mWriteDMARequestRXEvent[chid].notify();
}//}}}

/// Clear UTTBBF bit for next writing into UCTD register
void PSIS011::ClearUTTBBFBitMethod()
{//{{{
    mpPSIS011Func->UpdatePSI5SUCTMReg("UTTBBF", 0);
}//}}}

/// Set UTTFIN bit when finish UART trans
void PSIS011::SetUTTFINBitMethod()
{//{{{
    mpPSIS011Func->UpdateUTTFINBit(1);
}//}}}

/// Delay input RX_CONTROL and RX_DATA after a DNF delay cycle
void PSIS011::DNFDelayMethod()
{//{{{
    // Not operate when according clock is 0 or according reset 
    if (GetResetStatus("psis_rst_n") || (CheckClockPeriod("psis_clk") == false)){
        return;
    }
    mRX_CONTROL = RX_CONTROL.read();
    mRX_DATA = RX_DATA.read();

    double dnf_delay = 0;
    if (mNFSET == 1){
        dnf_delay = (double)mDNFDelay * (double)mClkPeriod[empsis_clkId];
    }
    mForwardDNFUARTRxEvent.notify(dnf_delay, mTimeResolutionUnit);
}//}}}

/// Issue UART RX signal from DNF to UART Receiver
void PSIS011::ForwardDNFUARTRxMethod()
{//{{{
    rx_control_sig.write(mRX_CONTROL);
    rx_data_sig.write(mRX_DATA);
}//}}}

/// Monitor output from DNF to receive data
void PSIS011::MonitorDNFOutputMethod()
{//{{{
    // Not need to check reset here, because the mForwardDNFUARTRxEvent is cancelled when reset

    mDNF_RX_CONTROL = rx_control_sig.read();
    mDNF_RX_DATA = rx_data_sig.read();

    switch (mReceptionState){
        case (emIdleReceptionState):
            ReceiveInIdleState();
            break;
        case (emStartReceptionState):
            ReceiveInStartState();
            break;
        default: // emStopReceptionState -> don not care
            break;
    };
}//}}}

/// Monitor tx request signals, arbitrate them and store to tx request FIFO
void PSIS011::ArbitrateTxReqThread()
{//{{{
    while (true){
        // Not operate when according clock is 0 or according reset 
        if (GetResetStatus("PRESETn") || !CheckClockPeriod("PCLK")){
            wait();
            continue;
        }
        wait(SC_ZERO_TIME); // need wait to avoid rising
        // Push to Tx Req FIFO
        for (unsigned int i = 0; i < emNumChannel; i++){
            if (i == 7){
                unsigned int numfrm = 0;
                sc_dt::uint64 tx_cpu_com_data = 0;
                mpPSIS011Func->GetConfigCPUTxCom(numfrm, tx_cpu_com_data);
                if ((mReqFIFO.size() != emFullFIFO) && (tx_req_sig[i]->read())){
                    mReqFIFO.push(i);
                    mNumTXReqInFIFO += numfrm;
                    UpdateTXFIFOStatus();
                    mTransPSI5Event.notify();
                }
            }else{
                if ((mReqFIFO.size() != emFullFIFO) && (tx_req_sig[i]->read())){
                    mReqFIFO.push(i);
                    mNumTXReqInFIFO++;
                    UpdateTXFIFOStatus();
                    mTransPSI5Event.notify();
                }
            }
        }
        wait();
    }
}//}}}

/// Monitor WDT error signal
void PSIS011::MonitorWDTErrorMethod(unsigned int chid)
{//{{{
    if (wdt_error_sig_chn[chid]->read()){
        mChannel[chid].wdt_error = true;
        // Stop WDT when counting to 0 in Sync mode
        if (mChannel[chid].is_sync){
            mpWDTChannel[chid]->StopWDT();
        // Reload and count if in Async mode
        }else{
            mpWDTChannel[chid]->StartWDT();
            mChannel[chid].wdt_error = false;
        }
        // Store un-finished PSI5 data in current receive channel
        if ((chid == mCurChid) && (mRxUARTFrmNum > 0)){
            mpPSIS011Func->SetPSI5SPCISReg(chid, "RWDT");
            re_printf("info", "WDT error in channel %d\n", chid);
            unsigned int stored_chid = 0;
            unsigned int stored_frame = 0;
            unsigned int status = 0;
            unsigned int data = 0;
            unsigned int timestamp = 0;
            GenerateMBDataForWDTError(stored_chid, stored_frame, status, data, timestamp);
            mpPSIS011Func->StoreMBData(stored_chid, stored_frame, status, data, timestamp);
            mRxUARTFrmNum = 0;
            mLostUARTFrame = false;
            mHasPSI5ReceiveError = false;
        }
    }
}//}}}

/// Receive UART in Idle Reception State
void PSIS011::ReceiveInIdleState()
{//{{{
    unsigned int strobe = (mDNF_RX_CONTROL >> 6) & 0x3;
    switch(strobe) {
        case (0x0): // Idle -> No operation
            break;
        case (0x1):{ // Start
            mEndIdleReceptionStateEvent.cancel(); // Cancel wait max idle period if receive next data
            mReceptionState = emStartReceptionState;
            // Calculate num bit
            unsigned int numbit = 8;// 1 start + 8 data
            if (mURPRTY > 0){
                numbit++;
            }
            double wait_time = (numbit + 1) * (double)mDivider * mClkPeriod[empsis_clkId]; // +1 because of start bit
            mMoveToStopReceptionStateEvent.notify(wait_time, mTimeResolutionUnit);
            break;
        }
        case (0x2):{ // Stop -> no operation
            re_printf("warning", "Invalid received \"%s\" in Idle Reception State.\n", mStrobeStringMap[strobe].c_str());
            break;
        }
        default:{ //case (0x3): // Abort -> no operation too
            re_printf("warning", "Invalid received \"%s\" in Idle Reception State.\n", mStrobeStringMap[strobe].c_str());
            break;
        }
    };
}//}}}

/// Receive UART in Start Reception State
void PSIS011::ReceiveInStartState()
{//{{{
    unsigned int strobe = (mDNF_RX_CONTROL >> 6) & 0x3;
    re_printf("warning", "Invalid received \"%s\" in Start Reception State. UART packet is aborted.\n", mStrobeStringMap[strobe].c_str());
    mLostUARTFrame = true;
    mMoveToStopReceptionStateEvent.cancel(); // Not move to Stop Reception State
    mReceptionState = emIdleReceptionState;// Change mode to Idle Reception State
}//}}}

/// Receive UART in Stop Reception State
void PSIS011::ReceiveInStopState()
{//{{{
    unsigned int strobe = (mDNF_RX_CONTROL >> 6) & 0x3;
    bool framing_error = false;

    // Check parity error and get pure data
    bool parity_error = false;
    bool calc_parity = 0;
    bool recv_parity = (bool)(mDNF_RX_DATA & 0x1);
    unsigned int pure_data = 0;
    if (mURPRTY == 0){ // Disable parity
        pure_data = mDNF_RX_DATA & 0xFF;
        parity_error = false;
    }else if (mURPRTY == 1){ // Even parity
        pure_data = (mDNF_RX_DATA >> 1) & 0xFF;
        calc_parity = GenerateParity(pure_data, 8);
        parity_error = (calc_parity != recv_parity);
    }else if (mURPRTY == 2){ // Dont care parity
        pure_data = (mDNF_RX_DATA >> 1) & 0xFF;
        parity_error = false;
    }else{ // Odd parity
        pure_data = (mDNF_RX_DATA >> 1) & 0xFF;
        calc_parity = GenerateParity(pure_data, 8);
        parity_error = (calc_parity == recv_parity);
    }
    
    // Check framing error
    unsigned int chid = mCurChid;
    if (strobe != 0x2){// Not receive Stop -> Framing error
        framing_error = true;
        // Determine chid
        if (mRxUARTFrmNum == 0){
            chid = pure_data & 0x7;
        }
        if (mChannel[chid].ch_en == 1){
            mpPSIS011Func->SetPSI5SPCISReg(0,"UTFR");
        }
        re_printf("warning", "Invalid received \"%s\" in Stop Reception State.\n", mStrobeStringMap[strobe].c_str());
    }

    // Store data to register if UART mode
    if (mMode == emUARTMode){
        mpPSIS011Func->StoreUARTData(pure_data, framing_error, parity_error);
        re_printf("info", "(UART mode) Receive UART frame = %X.\n", mDNF_RX_DATA);

    // RestorePSI5Frame if PSI5 mode
    }else{
        mRxUARTFrmNum++;
        RestorePSI5Frame(pure_data, parity_error, framing_error);
        if (mIgnorePacket){
            re_printf("info", "(PSI5 mode) Ignore UART frame no.%d = %X.\n", mRxUARTFrmNum, mDNF_RX_DATA);
        }else{
            re_printf("info", "(PSI5 mode) Receive UART frame no.%d = %X.\n", mRxUARTFrmNum, mDNF_RX_DATA);
        }
    }
    // Update status bit according
    if (parity_error && (mChannel[chid].ch_en == 1)){
        mpPSIS011Func->SetPSI5SPCISReg(0, "UTPT");
    }

}//}}}

/// Move to Stop Reception State
void PSIS011::MoveToStopReceptionStateMethod()
{//{{{
    mReceptionState = emStopReceptionState;

    // Move to Idle Reception State after 1bit * baud rate
    double wait_time = (double)mDivider * mClkPeriod[empsis_clkId];
    mMoveToIdleReceptionStateEvent.notify(wait_time, mTimeResolutionUnit);
}//}}}

/// Move to Idle Reception State
void PSIS011::MoveToIdleReceptionStateMethod()
{//{{{
    // Receive UART frame before change state to idle
    ReceiveInStopState();

    mReceptionState = emIdleReceptionState;
    // Check max idle
    if (mMode == emPSI5Mode){
        double idle_period = (double)mDivider * mClkPeriod[empsis_clkId] * (mMaxIdleCycle + 1);
        mEndIdleReceptionStateEvent.notify(idle_period, mTimeResolutionUnit);
    }
}//}}}

/// Check receive frame is lack or not when end max idle period
void PSIS011::EndIdleReceptionStateMethod()
{//{{{
    // Ignore packet in case invalid info setting about FPKT, FPAYLD
    if (mIgnorePacket){
        mIgnorePacket = false;
    }else{
        // Check lack frame in PSI5 packet -> Not store
        if (mRxUARTFrmNum < mFPKT){
            mpPSIS011Func->SetPSI5SPCISReg(mCurChid, "RFLK");

        // Generate MB data and store if enough or excess
        }else{
            if (mChannel[mCurChid].ch_en == 1){
                unsigned int chid = 0;
                unsigned int frame = 0;
                unsigned int status = 0;
                unsigned int data = 0;
                unsigned int timestamp = 0;
                // Get status, data, timestamp, allocated chid/frame into referent parameter
                GenerateMBData(chid, frame, status, data, timestamp); 
                // Store MB data to according register
                mpPSIS011Func->StoreMBData(chid, frame, status, data, timestamp);

                // Increase frame ID counter in each channel in case receive async
                if (mChannel[mCurChid].is_sync == false){
                    if (mChannel[mCurChid].async_frm_cnt == 5){
                        mChannel[mCurChid].async_frm_cnt = 0;
                    }else{
                        mChannel[mCurChid].async_frm_cnt++;
                    }
                }
            }
        }
    }

    // Clear mRxUARTFrmNum for next PSI5 packet
    mRxUARTFrmNum = 0;
    mLostUARTFrame = false;
    mHasPSI5ReceiveError = false;
}//}}}

/// Restore PSI5 frame
void PSIS011::RestorePSI5Frame(unsigned int data, bool parity_error, bool framing_error)
{//{{{
    // Not proceed PSI5 Rx if PRESETn or PCLK = 0
    if (GetResetStatus("PRESETn") || (CheckClockPeriod("PCLK") == false)){
        return;
    }

    // Update mIsUARTPrityError and mIsUARTFramingError
    mIsUARTPrityError |= parity_error;
    mIsUARTFramingError |= framing_error;
    mHasPSI5ReceiveError |= (parity_error | framing_error);

    // Ignore packet if any
    if (mIgnorePacket){
        return;
    }

    // Get Chid, Frmid from first UART frame, check validity of setting PKT, PAYLOAD, RFCPS
    if (mRxUARTFrmNum == 1){
        mCurChid = data & 0x7;
        mCurFid = (data >> 3) & 0x7;
        mCurHeaderStatus = (data >> 6) & 0x3;

        // Capture timestamp in Ch0 if any header receive
        unsigned int timestamp = this->GetTimestamp(0);
        mpPSIS011Func->CaptureTimestamp(0, timestamp);
        // Capture timestamp in Ch1-7 in sync mode and tscts = 1, or Ch1-7 in async mode
        if (mCurChid > 0){
            if (!(mChannel[mCurChid].is_sync && (mChannel[mCurChid].ts_trg_sel == 0))){
                timestamp = this->GetTimestamp(mCurChid);// capture according channel
                mpPSIS011Func->CaptureTimestamp(mCurChid, timestamp);
            }
        }

        // Check receive header info
        if (mChannel[mCurChid].is_sync){
            if (((mCurChid == 0) && (mCurFid > 0)) || ((mCurChid > 0) && (mCurFid > 5))){
                mFPKT = 6;
                mFPAYLD = 28;
                mRFCPS = mpPSIS011Func->GetRFCPSBit(mCurChid);
                mHasPSI5ReceiveError |= true;
            }else{
                mpPSIS011Func->GetConfigPSI5RxFrame(mCurChid, mCurFid, mFPKT, mFPAYLD, mRFCPS);
                if (mFPAYLD == 0){
                    mFPAYLD = 8; // Initial value "0" of FiPAYLD is treated as 8
                }
                // Check validity of setting PKT, PAYLOAD, RFCPS
                if (!CheckValidFPKTvsFPAYLD(mFPAYLD, mRFCPS, mFPKT)){
                    re_printf("warning", "Cannot restore PSI5 frame in channel %d due to invalid setting PSI5SPRCF2%d.F%dPAYLD (=%d), PSI5SPRCF1%d.RFCPS (=%d), PSI5SPRCF1%d.F%dPKT (=%d).\n", 
                            mCurChid, mCurChid, mCurFid + 1, mFPAYLD, mCurChid, mRFCPS, mCurChid, mCurFid + 1, mFPKT);
                    mIgnorePacket = true;
                    return;
                }
                mIgnorePacket = false;
            }
        }else{
            if (mCurFid > 0){
                mRFCPS = mpPSIS011Func->GetRFCPSBit(mCurChid);
                mHasPSI5ReceiveError |= true;
            }else{
                mpPSIS011Func->GetConfigPSI5RxFrame(mCurChid, mChannel[mCurChid].async_frm_cnt, mFPKT, mFPAYLD, mRFCPS);
                if (mFPAYLD == 0){
                    mFPAYLD = 8; // Initial value "0" of FiPAYLD is treated as 8
                }
                // Check validity of setting PKT, PAYLOAD, RFCPS
                if (!CheckValidFPKTvsFPAYLD(mFPAYLD, mRFCPS, mFPKT)){
                    re_printf("warning", "Cannot restore PSI5 frame in channel %d due to invalid setting PSI5SPRCF2%d.F%dPAYLD (=%d), PSI5SPRCF1%d.RFCPS (=%d), PSI5SPRCF1%d.F%dPKT (=%d).\n", 
                            mCurChid, mCurChid, mChannel[mCurChid].async_frm_cnt + 1, mFPAYLD, mCurChid, mRFCPS, mCurChid, mChannel[mCurChid].async_frm_cnt + 1, mFPKT);
                    mIgnorePacket = true;
                    return;
                }
                mIgnorePacket = false;
            }
        }
        mPSI5Packet = 0;
    }

    // Check PSI5 frame excess. If frame exesss -> update CRC, XCRC error also as HWM
    if (mRxUARTFrmNum > mFPKT){
        mpPSIS011Func->SetPSI5SPCISReg(mCurChid, "RFEX");
        mpPSIS011Func->SetPSI5SPCISReg(mCurChid, "CRC");
        mpPSIS011Func->SetPSI5SPCISReg(0, "XCRC");//channel0
        
    // Restore PSI5 packet if not excess
    }else{
        mPSI5Packet = (mPSI5Packet << 8) | (sc_dt::uint64)Reverse(data,8) ;
        // Stop WDT if receive enough specified number of UART frame in Sync mode
        if ((mRxUARTFrmNum == mFPKT) && (mChannel[mCurChid].is_sync)){
            mpWDTChannel[mCurChid]->StopWDT();
        }
    }

    // Calculate XCRC if enough UART frame => for increasing wdt_frm_cnt
    if (mRxUARTFrmNum == mFPKT){
        sc_dt::uint64 msb_psi5_packet = Reverse(mPSI5Packet, 8*mFPKT); // 8 is 8bit of 1 byte
        sc_dt::uint64 mask_crc_extra_data = ((sc_dt::uint64)1<<(mFPKT*8 - 6)) - 1;
        sc_dt::uint64 data_cal_crc_extra = msb_psi5_packet & mask_crc_extra_data;
        unsigned int recv_crc_extra = (unsigned int)((msb_psi5_packet >> (mFPKT*8 - 6)) & 0x3F);
        bool has_crc_extra_error = (GenerateCRCExtra(Reverse(data_cal_crc_extra, mFPKT*8 - 6), mFPKT*8 - 6) != recv_crc_extra);
        mHasPSI5ReceiveError |= has_crc_extra_error;
    }
    // Count wdt_frm_cnt if no UART parity/framing error, no XCRC error, valid FrmID
    if (!mHasPSI5ReceiveError){
        if (mChannel[mCurChid].wdt_frm_cnt == 5){
            mChannel[mCurChid].wdt_frm_cnt = 0;
        }else{
            mChannel[mCurChid].wdt_frm_cnt++;
        }
    }

    // Reload WDT in async mode
    if (!(mChannel[mCurChid].is_sync) && (mChannel[mCurChid].is_wdt_enabled)){
        mpWDTChannel[mCurChid]->StartWDT();
        mChannel[mCurChid].wdt_error = false;
    }
}//}}}

/// Capture timestamp from timestamp A/B
unsigned int PSIS011::GetTimestamp(unsigned int chid)
{//{{{
    unsigned int ret = 0;
    if (mChannel[chid].ts_en){
        if (mChannel[chid].ts_sel == 1){// timestamp A
            ret = mpTimestampA->GetTimestamp();
        }else{// timestamp B
            ret = mpTimestampB->GetTimestamp();
        }
    }
    return ret;
}//}}}

/// Check harmony of setting PKT, PAYLOAD, RFCPS
bool PSIS011::CheckValidFPKTvsFPAYLD(unsigned int fpayld, unsigned int rfcps, unsigned int fpkt)
{//{{{
    unsigned int bind = (fpayld << 4) | (rfcps << 3) | fpkt;

    for (unsigned int i = 0; i < emSizeOfValidFPKTvsFPAYLDTable; i++) {
        if (mValidFPKTvsFPAYLDTable[i] == bind){
            return true;
        }
    }
    return false;
}//}}}

/// Generate MailBox data in normal case
void PSIS011::GenerateMBData(unsigned int &allocated_chid, unsigned int &allocated_frame, unsigned int &status_reg_val, 
                    unsigned int &data_reg_val, unsigned int &timestamp_reg_val)
{//{{{
    // Check XCRC error, .... other error
    sc_dt::uint64 msb_psi5_packet = Reverse(mPSI5Packet, 8*mFPKT); // 8 is 8bit of 1 byte

    unsigned int mask_parity_data = ((unsigned int)1 << mFPAYLD) - 1;
    unsigned int data_cal_parity = (unsigned int)((msb_psi5_packet >> 8) & mask_parity_data);
    bool calc_parity = GenerateParity(data_cal_parity, mFPAYLD);
    unsigned int mask_payload = ((unsigned int)1 << mFPAYLD) - 1;
    unsigned int payload = (unsigned int)((msb_psi5_packet >> 8) & mask_payload);

    unsigned int recv_crc = (unsigned int)((msb_psi5_packet >> (8 + mFPAYLD)) & 0x7);
    recv_crc = (unsigned int)Reverse(recv_crc, 3);// CRC index (MSB), not LSB
    bool recv_parity = (recv_crc >> 2) & 0x1;
    bool has_psi5_parity_error = (calc_parity != recv_parity);
    bool has_crc_error = (GenerateCRC((unsigned int)Reverse(payload, mFPAYLD), mFPAYLD) != recv_crc);

    sc_dt::uint64 mask_crc_extra_data = ((sc_dt::uint64)1<<(mFPKT*8 - 6)) - 1;
    sc_dt::uint64 data_cal_crc_extra = msb_psi5_packet & mask_crc_extra_data;
    unsigned int recv_crc_extra = (unsigned int)((msb_psi5_packet >> (mFPKT*8 - 6)) & 0x3F);
    bool has_crc_extra_error = (GenerateCRCExtra(Reverse(data_cal_crc_extra, mFPKT*8 - 6), mFPKT*8 - 6) != recv_crc_extra);
    // Allocate address to store MB
    if (mHasPSI5ReceiveError){
        allocated_chid = 0;
        allocated_frame = 2;
    }else{
        if (mChannel[mCurChid].is_sync){ // Sync mode -> Note: not check case mCurFid > 5 because it checked in RestorePSI5Frame, update mHasPSI5ReceiveError = true
            allocated_chid = mCurChid;
            allocated_frame = mCurFid + 1; // 0-5 store in FRM 1-6
        }else{ // Async mode -> Note: not check case mCurFid > 0 because it checked in RestorePSI5Frame, update mHasPSI5ReceiveError = true
            allocated_chid = mCurChid;
            allocated_frame = mChannel[mCurChid].async_frm_cnt + 1;// 0-5 store in FRM 1-6
        }
    }

    // DCI, Status, data, timestamp in MB
    unsigned int dci = mChannel[allocated_chid].dci_counter;
    if (mChannel[allocated_chid].dci_counter == 15){
        mChannel[allocated_chid].dci_counter = 0;
    }else{
        mChannel[allocated_chid].dci_counter++;
    }
    // Check MB overrun, header error
    bool mb_over_run = !(mpPSIS011Func->CheckMBDataWasRead(allocated_chid, allocated_frame));
    bool header_error = (mCurHeaderStatus > 0);
    // Update bit status according
    if (header_error){
        mpPSIS011Func->SetPSI5SPCISReg(mCurChid, "TRST");
    }
    if (has_psi5_parity_error && (mRFCPS == 0)){
        mpPSIS011Func->SetPSI5SPCISReg(mCurChid, "PT");
    }
    if (has_crc_error && (mRFCPS == 1)){
        mpPSIS011Func->SetPSI5SPCISReg(mCurChid, "CRC");
    }
    if (has_crc_extra_error){
        mpPSIS011Func->SetPSI5SPCISReg(0, "XCRC");
    }

    // Update PSI5SPRES register in allocated channel/frame
    if (has_crc_extra_error || mIsUARTPrityError || mIsUARTFramingError || mb_over_run || mChannel[mCurChid].wdt_error
            || header_error || has_psi5_parity_error || has_crc_error){
        mpPSIS011Func->SetPSI5SPRESReg(allocated_chid, allocated_frame);
    }

    if (mRFCPS == 0){// parity
        status_reg_val = (dci << 28) | (allocated_chid << 22) | ((allocated_frame -1) << 19) | ((unsigned int)mb_over_run << 18) | ((unsigned int)mChannel[mCurChid].wdt_error << 17)
            | ((unsigned int)mIsUARTFramingError << 15) | ((unsigned int)mIsUARTPrityError << 14) | ((unsigned int)header_error << 13) | (mCurHeaderStatus << 11)
            | ((unsigned int)has_psi5_parity_error << 10) | ((unsigned int)recv_parity << 9) | (0 << 7) | ((unsigned int)has_crc_extra_error << 6) | recv_crc_extra;
    }else{// crc
        status_reg_val = (dci << 28) | (allocated_chid << 22) | ((allocated_frame - 1) << 19) | ((unsigned int)mb_over_run << 18) | ((unsigned int)mChannel[mCurChid].wdt_error << 17)
            | ((unsigned int)mIsUARTFramingError << 15) | ((unsigned int)mIsUARTPrityError << 14) | ((unsigned int)header_error << 13) | (mCurHeaderStatus << 11)
            | ((unsigned int)has_crc_error << 10) | (recv_crc << 7) | ((unsigned int)has_crc_extra_error << 6) | recv_crc_extra;
    }

    data_reg_val = (dci << 28) | payload;
    
    timestamp_reg_val = (dci << 28) | (mpPSIS011Func->GetCapturedTimestamp(allocated_chid));
}//}}}

/// Generate MailBox data in case WDT error
void PSIS011::GenerateMBDataForWDTError(unsigned int &allocated_chid, unsigned int &allocated_frame,
            unsigned int &status_reg_val, unsigned int &data_reg_val, unsigned int &timestamp_reg_val)
{//{{{
    // Always store into channel 0, frame 2
    allocated_chid = 0;
    allocated_frame = 2;

    // DCI, Status, data, timestamp in MB
    unsigned int dci = mChannel[allocated_chid].dci_counter;
    if (mChannel[allocated_chid].dci_counter == 15){
        mChannel[allocated_chid].dci_counter = 0;
    }else{
        mChannel[allocated_chid].dci_counter++;
    }

    // Calc for MB Status
    unsigned int mb_s_chid = mCurChid;
    unsigned int mb_s_fid = 0;
    bool mborerr = !(mpPSIS011Func->CheckMBDataWasRead(allocated_chid, allocated_frame));
    unsigned int wdterr = 1;
    bool         utfrerr = false;
    bool         utprerr = false;
    bool         headerr = false;
    unsigned int headst = 0;
    unsigned int crc = 0;
    bool         crcerr = false;
    unsigned int xcrc = 0;
    bool         xcrcerr = false;
    unsigned int payload = 0;
    bool has_psi5_parity_error = false;
    bool recv_parity = 0;
    if (mRxUARTFrmNum >= mFPKT){ // enough frame or excess
        utfrerr = mIsUARTFramingError;
        utprerr = mIsUARTPrityError;
        headerr = (mCurHeaderStatus > 0);
        headst  = mCurHeaderStatus;

        sc_dt::uint64 msb_psi5_packet = Reverse(mPSI5Packet, 8*mFPKT); // 8 is 8bit of 1 byte
        unsigned int crc = (unsigned int)((msb_psi5_packet >> (8 + mFPAYLD)) & 0x7);
        crc = (unsigned int)Reverse(crc, 3);// CRC index (MSB), not LSB
        unsigned int mask_payload = ((unsigned int)1 << mFPAYLD) - 1;
        payload = (unsigned int)((msb_psi5_packet >> 8) & mask_payload);
        unsigned int calc_crc = GenerateCRC((unsigned int)Reverse(payload, mFPAYLD), mFPAYLD);
        crcerr = (crc != calc_crc);

        unsigned int xcrc = (unsigned int)((msb_psi5_packet >> (mFPKT*8 - 6)) & 0x3F);
        xcrcerr = true;

        bool calc_parity = GenerateParity(payload, mFPAYLD);
        recv_parity = (crc >> 2) & 0x1;
        has_psi5_parity_error = (calc_parity != recv_parity);
    }else if (!mLostUARTFrame){ // lack frame not due to abort
        utfrerr = mIsUARTFramingError;
        utprerr = mIsUARTPrityError;
        headerr = (mCurHeaderStatus > 0);
        headst  = mCurHeaderStatus;
    }
    // Calc FID
    if (mChannel[mCurChid].is_sync){//sync
        if (mRxUARTFrmNum == mFPKT){
            mb_s_fid  = mCurFid;
        }else{
            mb_s_fid  = mChannel[mCurChid].wdt_frm_cnt;
        }
    }else{// FID in async
        if (mRxUARTFrmNum < mFPKT){
            mb_s_fid = 0;
        }else{
            mb_s_fid  = mCurFid;
        }
    }

    // Update bit status according
    if (headerr){
        mpPSIS011Func->SetPSI5SPCISReg(mCurChid, "TRST");
    }
    if (has_psi5_parity_error && (mRFCPS == 0)){
        mpPSIS011Func->SetPSI5SPCISReg(mCurChid, "PT");
    }
    if (crcerr){
        mpPSIS011Func->SetPSI5SPCISReg(mCurChid, "CRC");
    }
    if (xcrcerr){
        mpPSIS011Func->SetPSI5SPCISReg(0, "XCRC");
    }

    // Update PSI5SPRES register in allocated channel/frame
    mpPSIS011Func->SetPSI5SPRESReg(allocated_chid, allocated_frame);

    // Status, data, timestamp
    if (mRFCPS == 0){// parity
        status_reg_val = (dci << 28) | (mb_s_chid << 22) | ((mb_s_fid) << 19) | ((unsigned int)mborerr << 18) | (wdterr << 17)
            | ((unsigned int)utfrerr << 15) | ((unsigned int)utprerr << 14) | ((unsigned int)headerr << 13) | (headst << 11)
            | ((unsigned int)has_psi5_parity_error << 10) | ((unsigned int)recv_parity << 9) | (0 << 7) | ((unsigned int)xcrcerr << 6) | xcrc;
    }else{// crc
        status_reg_val = (dci << 28) | (mb_s_chid << 22) | ((mb_s_fid) << 19) | ((unsigned int)mborerr << 18) | (wdterr << 17)
            | ((unsigned int)utfrerr << 15) | ((unsigned int)utprerr << 14) | ((unsigned int)headerr << 13) | (headst << 11)
            | ((unsigned int)crcerr << 10) | (crc << 7) | ((unsigned int)xcrcerr << 6) | xcrc;
    }

    data_reg_val = (dci << 28) | payload;
    timestamp_reg_val = (dci << 28) | (mpPSIS011Func->GetCapturedTimestamp(allocated_chid));
}//}}}

/// Reverse a data
sc_dt::uint64 PSIS011::Reverse(sc_dt::uint64 input, unsigned int length)
{//{{{
    sc_dt::uint64 out = 0;
    for (unsigned int i = 0; i < length; i++){
        out |= ((input >> i) & 0x1) << (length - 1 - i);
    }
    return out;
}//}}}

/// Generate parity for a input data with specified length
bool PSIS011::GenerateParity(unsigned int data, unsigned int length)
{//{{{
    unsigned int cal_parity = 0;
    for (unsigned int i = 0; i < length; i++){
        cal_parity ^= (data >> i) & 0x1;
    }

    return (bool)cal_parity;
}//}}}

/// Generate CRC of PSI5 packet
unsigned int PSIS011::GenerateCRC(unsigned int data, unsigned int length)
{//{{{
    // Add 3 initial bit 3'b111 font
    data |= (emInitCRCVal << length);
    length += 3;

    // Generate CRC
    unsigned int mask_top_bit = (unsigned int)1 << (length - 1);
    unsigned int polynomial = (unsigned int)emCRCPolynomial << (length - 4);
    unsigned int remainder = data;

    for (unsigned int i = length; i > 0; i--){// not process for 3bit 000 at back
        if ((remainder & mask_top_bit) == mask_top_bit){ // topbit = 1
            remainder ^= polynomial;
        }
        remainder = remainder << 1;
    }

    return (unsigned int)(remainder >> (length - 3));
}//}}}

/// Generate CRC Extra of PSI5 packet
unsigned int PSIS011::GenerateCRCExtra(sc_dt::uint64 data, unsigned int length)
{//{{{
    // Add 6 initial bit 6'b010101 front
    data |= ((sc_dt::uint64)emInitCRCExtraVal << length);
    length += 6;

    // Generate
    sc_dt::uint64 mask_top_bit = (sc_dt::uint64)1 << (length - 1);
    sc_dt::uint64 polynomial = (sc_dt::uint64)emCRCExtraPolynomial << (length - 7);
    sc_dt::uint64 remainder = data;

    for (unsigned int i = length; i > 0; i--){
        if ((remainder & mask_top_bit) == mask_top_bit){ // topbit = 1
            remainder ^= polynomial;
        }
        remainder = remainder << 1;
    }

    return (unsigned int)(remainder >> (length - 6));
}//}}}

// Prepare UART frame
unsigned int PSIS011::PrepareUARTFrame(unsigned int data, unsigned int parity_option, unsigned int &numbit)
{//{{{
    unsigned int uart_frame = 0;
    if (parity_option == 1){// Even
        bool calc_parity = GenerateParity(data, 8);
        bool parity = calc_parity;
        uart_frame = (data << 1) | (unsigned int)parity;
        numbit = 9;
    }else if (parity_option == 2){// parity always 0
        uart_frame = data << 1;
        numbit = 9;
    }else if (parity_option == 3){//Odd
        bool calc_parity = GenerateParity(data, 8);
        bool parity = !calc_parity;
        uart_frame = (data << 1) | (unsigned int)parity;
        numbit = 9;
    }else{
        numbit = 8;
        uart_frame = data;
    }
    return uart_frame;
}//}}}

/// Prepare DDSR data
void PSIS011::PrepareDDSRData(unsigned int chid, sc_dt::uint64 &ddsr, unsigned int &length)
{//{{{
    unsigned int frame_type = 0;
    unsigned int address = 0;
    unsigned int data = 0;
    mpPSIS011Func->GetDDSRInfo(chid, frame_type, address, data); // txid 0-6 for channel 1-7
    sc_dt::uint64 data_64 = (sc_dt::uint64)data;
    if (frame_type == 0){
        length = 15;
        unsigned int data_calc_crc = ((unsigned int)Reverse(address & 0x7, 3) << 6) | ((unsigned int)Reverse(data_64 & 0x7, 3) << 3);
        ddsr = ((sc_dt::uint64)0x5 << 11) | (Reverse(address, 3) << 8) | ((sc_dt::uint64)1 << 7)
            | (Reverse(data_64, 3) << 4) | ((sc_dt::uint64)1 << 3) | GenerateCRC(data_calc_crc, 9);
    }else if (frame_type == 1){
        length = 29;
        unsigned int data_calc_crc = ((unsigned int)Reverse(address & 0x7, 3) << 16) | ((unsigned int)Reverse(data_64 & 0x1FFF, 13) << 3);
        unsigned int gen_crc = GenerateCRC(data_calc_crc, 19);
        ddsr = ((sc_dt::uint64)0x5 << 25) | (Reverse(address, 3) << 22) | ((sc_dt::uint64)1 << 21)
            | (Reverse((data_64 & 0x7), 3) << 18) | ((sc_dt::uint64)1 << 17) | (Reverse(((data_64 >> 3) & 0x7), 3) << 14) | ((sc_dt::uint64)1 << 13) 
            | (Reverse(((data_64 >> 6) & 0x7), 3) << 10) | ((sc_dt::uint64)1 << 9) | (Reverse(((data_64 >> 9) & 0x7), 3) << 6) | ((sc_dt::uint64)1 << 5)
            | (((data_64 >> 12) & 0x1) << 4) | ((((sc_dt::uint64)gen_crc >> 1)& 0x3) << 2) | ((sc_dt::uint64)1 << 1) | ((sc_dt::uint64)gen_crc & 0x1);
    }else if (frame_type == 2){
        length = 37;
        unsigned int data_calc_crc = ((unsigned int)Reverse(address & 0x7, 3) << 22) | ((unsigned int)Reverse(data_64 & 0x7FFFF, 19) << 3);
        unsigned int gen_crc = GenerateCRC(data_calc_crc, 25);
        ddsr = ((sc_dt::uint64)0x5 << 33) | ((sc_dt::uint64)Reverse(address & 0x7, 3) << 30) | ((sc_dt::uint64)1 << 29)
            | (Reverse((data_64 & 0x7), 3) << 26) | ((sc_dt::uint64)1 << 25) | (Reverse(((data_64 >> 3) & 0x7), 3) << 22) | ((sc_dt::uint64)1 << 21) 
            | (Reverse(((data_64 >> 6) & 0x7), 3) << 18) | ((sc_dt::uint64)1 << 17) | (Reverse(((data_64 >> 9) & 0x7), 3) << 14) | ((sc_dt::uint64)1 << 13)
            | (Reverse(((data_64 >> 12) & 0x7), 3) << 10) | ((sc_dt::uint64)1 << 9) | (Reverse(((data_64 >> 15) & 0x7), 3) << 6) | ((sc_dt::uint64)1 << 5)
            | (((data_64 >> 18) & 0x1) << 4) | ((((sc_dt::uint64)gen_crc >> 1)& 0x3) << 2) | ((sc_dt::uint64)1 << 1) | ((sc_dt::uint64)gen_crc & 0x1);
    }else {
        length = 43;
        unsigned int data_calc_crc = ((unsigned int)Reverse(address & 0x7, 4) << 26) | ((unsigned int)Reverse(data_64 & 0xFFFFF, 20) << 6);
        ddsr = ((sc_dt::uint64)0xFE << 34) | ((sc_dt::uint64)Reverse(address, 4) << 30) 
            | (Reverse((data_64 & 0x3), 2) << 28) | (Reverse(((data_64 >> 2) & 0x3F), 6) << 21) | (Reverse(((data_64 >> 8) & 0x3F), 6) << 14) 
            | (Reverse(((data_64 >> 14) & 0x3F), 6) << 7) | Reverse(GenerateCRCExtra(data_calc_crc, 30), 6);
    }
}//}}}

/// Prepare info in TX_CONTROL for tranfer
unsigned int PSIS011::PrepareTXCONTROL(eStrobe strobe, unsigned int numbit, unsigned int tc)
{//{{{
    unsigned int control = 0;
    unsigned int bitt = 0;
    if (mSCKEN == 1){
        bitt = (unsigned int)(mTxCLKDivider * mClkPeriod[empsis_mult_clkId] * 2 * 10000000/mTimeResolutionValue); // refer HWM page 205
    }
    if ((GetResetStatus("psis_mult_rst_n")) || (CheckClockPeriod("psis_mult_clk") == false)){ // not issue uart communication clock if psis_mult_rst_n activated
        bitt = 0;
    }
    unsigned int dir = 1; // LSB first
    control = (bitt << 16) | (tc << 14) | (dir << 8) | (strobe << 6) | numbit;
    return control;
}//}}}

/// Update TX FIFO status
void PSIS011::UpdateTXFIFOStatus()
{//{{{
    if (mNumTXReqInFIFO >= emFullFIFO){
        mpPSIS011Func->SetPSI5SPTFISReg(0x2);// Full
    }else if (mNumTXReqInFIFO == 0){
        mpPSIS011Func->SetPSI5SPTFISReg(0x1);// Empty
    }else{
        mpPSIS011Func->SetPSI5SPTFISReg(0); // Not full, not empty
    }
}//}}}

//============ PYTHON IF               ===================
/// Set message level (fatal, error, warning, info)
void PSIS011::SetMessageLevel (const std::string msg_lv)
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
    SeparateString(cmd, msg_lv);

    mpPSIS011Func->RegisterHandler("all", 0, cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    mpPSIS011Func->RegisterHandler("all", 0, cmd);
}//}}}

/// Enable/disable dumping message when users access registers
void PSIS011::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpPSIS011Func->RegisterHandler("all", 0, cmd);
}//}}}

/// Enable/disable dumping interrupt information
void PSIS011::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: PSIS011_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","PSIS011_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: PSIS011_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

// Set delay time for Noise Filter block
void PSIS011::SetDNFDelay (const unsigned int delay)
{//{{{
    mDNFDelay = delay;
}//}}}

/// Dump status register's value
void PSIS011::DumpStatusInfo ()
{//{{{
    mpPSIS011Func->DumpStatusInfo();
}//}}}

/// Command to assert reset
void PSIS011::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETnId])&&(!mIsResetCmdReceive[emPRESETnId])) {
            mIsResetCmdReceive[emPRESETnId] = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPRESETnId].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETnId] = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "psis_rst_n") {
        if ((!mIsResetHardActive[empsis_rst_nId])&&(!mIsResetCmdReceive[empsis_rst_nId])) {
            mIsResetCmdReceive[empsis_rst_nId] = true;
            re_printf("info","The model will be reset (psis_rst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[empsis_rst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[empsis_rst_nId] = period;
        } else {
            re_printf("warning","The software reset of psis_rst_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "psis_mult_rst_n") {
        if ((!mIsResetHardActive[empsis_mult_rst_nId])&&(!mIsResetCmdReceive[empsis_mult_rst_nId])) {
            mIsResetCmdReceive[empsis_mult_rst_nId] = true;
            re_printf("info","The model will be reset (psis_mult_rst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[empsis_mult_rst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[empsis_mult_rst_nId] = period;
        } else {
            re_printf("warning","The software reset of psis_mult_rst_n is called in the reset operation of the model. So it is ignored\n");
        }
    }else{
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn, or psis_rst_n, or psis_mult_rst_n.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void PSIS011::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mClkOrgFreq[emPCLKId] = freq;
            ConvertClockFreq (mClkFreq[emPCLKId], mClkFreqUnit[emPCLKId], freq, unit);
            if (mClkFreq[emPCLKId] > 0) {
                mClkPeriod[emPCLKId] = (sc_dt::uint64)(((1/(double)mClkFreq[emPCLKId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[emPCLKId] = sc_time_stamp().to_double();
                for (unsigned int i = 0; i < emNumChannel; i++){
                    mpSyncPulseChannel[i]->ConfigSyncPulseClock(mClkPeriod[emPCLKId], mTimeResolutionUnit);
                }
                mGenClockForTimestampEvent.notify(SC_ZERO_TIME);
            } else {
                mClkPeriod[emPCLKId] = 0;
                // Cancel events
                CancelEventsOfPRESETn();
                // Initialize according
                InitializeOfPRESETn();
                // Notify clock is zero
                mClkZeroEvent[emPCLKId].notify(SC_ZERO_TIME);
                // Stop gen sync pulse
                for (unsigned int i = 0; i < emNumChannel; i++){
                    mpSyncPulseChannel[i]->StopGenInnerSyncPulse();
                }
            }
        } else if (word_vector[0] == "psis_clk") {
            mClkOrgFreq[empsis_clkId] = freq;
            ConvertClockFreq (mClkFreq[empsis_clkId], mClkFreqUnit[empsis_clkId], freq, unit);
            if (mClkFreq[empsis_clkId] > 0) {
                mClkPeriod[empsis_clkId] = (sc_dt::uint64)(((1/(double)mClkFreq[empsis_clkId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[empsis_clkId] = sc_time_stamp().to_double();
                // Config clock psis_clk to WDT
                for (unsigned int i = 0; i < emNumChannel; i++){
                    mpWDTChannel[i]->ConfigWDTClock(mClkPeriod[empsis_clkId], mTimeResolutionUnit);
                }
            } else {
                mClkPeriod[empsis_clkId] = 0;
                // Stop WDT
                for (unsigned int i = 0; i < emNumChannel; i++){
                    mpWDTChannel[i]->StopWDT();
                }
                // Cancel events
                CancelEventsOfpsis_rst_n();
                // Initialize according
                InitializeOfpsis_rst_n();
                // Notify clock is zero
                mClkZeroEvent[empsis_clkId].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "psis_mult_clk") {
            mClkOrgFreq[empsis_mult_clkId] = freq;
            ConvertClockFreq (mClkFreq[empsis_mult_clkId], mClkFreqUnit[empsis_mult_clkId], freq, unit);
            if (mClkFreq[empsis_mult_clkId] > 0) {
                mClkPeriod[empsis_mult_clkId] = (sc_dt::uint64)(((1/(double)mClkFreq[empsis_mult_clkId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[empsis_mult_clkId] = sc_time_stamp().to_double();
            } else {
                mClkPeriod[empsis_mult_clkId] = 0;
                // Cancel events
                CancelEventsOfpsis_mult_rst_n();
                // Initialize according
                InitializeOfpsis_mult_rst_n();
                // Notify clock is zero
                mClkZeroEvent[empsis_mult_clkId].notify(SC_ZERO_TIME);
                // Stop gen sync pulse
                for (unsigned int i = 0; i < emNumChannel; i++){
                    mpSyncPulseChannel[i]->StopGenInnerSyncPulse();
                }
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void PSIS011::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mClkOrgFreq[emPCLKId], mClkFreqUnit[emPCLKId].c_str());
        } else if (word_vector[0] == "psis_clk") {
            re_printf("info","psis_clk frequency is %0.0f %s\n", (double)mClkOrgFreq[empsis_clkId], mClkFreqUnit[empsis_clkId].c_str());
        } else if (word_vector[0] == "psis_mult_clk") {
            re_printf("info","psis_mult_clk frequency is %0.0f %s\n", (double)mClkOrgFreq[empsis_mult_clkId], mClkFreqUnit[empsis_mult_clkId].c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void PSIS011::ForceRegister (const std::string reg_name, const unsigned int chid, const unsigned int reg_value)
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
            mpPSIS011Func->RegisterHandler(FindRegAreaFromName(reg_name), chid, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void PSIS011::ReleaseRegister (const std::string reg_name, const unsigned int chid)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpPSIS011Func->RegisterHandler(FindRegAreaFromName(reg_name), chid, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void PSIS011::WriteRegister (const std::string reg_name, const unsigned int chid, const unsigned int reg_value)
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
            mpPSIS011Func->RegisterHandler(FindRegAreaFromName(reg_name), chid, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void PSIS011::ReadRegister (const std::string reg_name, const unsigned int chid)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpPSIS011Func->RegisterHandler(FindRegAreaFromName(reg_name), chid, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void PSIS011::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpPSIS011Func->RegisterHandler("all", 0, cmd);
}//}}}

/// Separate some words from a string to store a vector
void PSIS011::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

/// Check register name and return the register's area
std::string PSIS011::FindRegAreaFromName(const std::string reg_name)
{//{{{
    std::string area = "";
    if (reg_name.find("n") == std::string::npos){
        area =  "common";
    }else{
        if (reg_name.find("PSI5SPMB") == std::string::npos){
            area =  "channel";
        }else{
            area =  "mailbox";
        }
    }
    return area;
}//}}}

/// Dump help message of all parameters or commands
void PSIS011::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"PSIS011_MessageLevel (\"PSIS011 instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"PSIS011_DumpRegisterRW (\"PSIS011 instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"PSIS011_DumpInterrupt (\"PSIS011 instance\", \"true/false\")                     Enable/disable dumping interrupt information (Default: false)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"PSIS011_SetDNFDelay (\"PSIS011 instance\", delay)                                Set delay time for Noise Filter");
            SC_REPORT_INFO(this->basename(),"PSIS011_DumpStatusInfo (\"PSIS011 instance\")                                    Dump the status info of model");
            SC_REPORT_INFO(this->basename(),"PSIS011_AssertReset (\"PSIS011 instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the PSIS011 model. Valid reset_name is PRESETn, psis_rst_n, psis_mult_rst_n");
            SC_REPORT_INFO(this->basename(),"PSIS011_SetCLKFreq (\"PSIS011 instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to PSIS011 model. Valid clock_name is PCLK, psis_clk, psis_mult_clk");
            SC_REPORT_INFO(this->basename(),"PSIS011_GetCLKFreq (\"PSIS011 instance\", \"clock_name\")                        Get clock frequency value of PSIS011 model. Valid clock_name is PCLK, psis_clk, psis_mult_clk");
            SC_REPORT_INFO(this->basename(),"PSIS011_ForceRegister (\"PSIS011 instance\", \"reg_name\", chid, reg_value)      Force register with setting value. To common register, any value is accepted for chid");
            SC_REPORT_INFO(this->basename(),"PSIS011_ReleaseRegister (\"PSIS011 instance\", \"reg_name\", chid)               Release register from force value. To common register, any value is accepted for chid");
            SC_REPORT_INFO(this->basename(),"PSIS011_WriteRegister (\"PSIS011 instance\", \"reg_name\", chid, reg_value)      Write a value to a register. To common register, any value is accepted for chid");
            SC_REPORT_INFO(this->basename(),"PSIS011_ReadRegister (\"PSIS011 instance\", \"reg_name\", chid)                  Read a value from a register. To common register, any value is accepted for chid");
            SC_REPORT_INFO(this->basename(),"PSIS011_ListRegister (\"PSIS011 instance\")                                      Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of PSIS011_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of PSIS011_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
