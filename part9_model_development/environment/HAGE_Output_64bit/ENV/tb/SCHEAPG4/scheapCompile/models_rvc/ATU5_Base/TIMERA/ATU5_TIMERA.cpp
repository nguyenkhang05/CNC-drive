// ---------------------------------------------------------------------
// $Id: ATU5_TIMERA.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <string>
#include "ATU5_Base.h"
#include "ATU5_TIMERA.h"
#include "ATU5_TIMERA_Func.h"
#include "ATU5_Timer.h"
#include "ATU5_TIMERD.h" 

/*********************************
// Function     : ATU5_TIMERA
// Description  : Constructor of ATU5_TIMERA class
// Parameter    :
//      name            Module name
//      ATU5_Ptr        ATU5_Base pointer
// Return value : None
**********************************/
ATU5_TIMERA::ATU5_TIMERA(sc_module_name name, ATU5_Base *ATU5_Ptr):
                   sc_module(name),
                   ATU5_TIMERA_Base(),
                   clkbus_0("clkbus_0"),
                   clkbus_1("clkbus_1"),
                   clkbus_2("clkbus_2"),
                   clkbus_3("clkbus_3"),
                   clkbus_4("clkbus_4"),
                   clkbus_5("clkbus_5"),
                   nc_clk_1("nc_clk_1"),
                   nc_clk_2("nc_clk_2"),
                   TCNTB6CM("TCNTB6CM"),
                   ev_out_1_0("ev_out_1_0"),
                   ev_out_1_1("ev_out_1_1"),
                   ev_out_1_2("ev_out_1_2"),
                   ev_out_2A("ev_out_2A"),
                   ev_out_2B("ev_out_2B"),
                   ev_out_1B("ev_out_1B"),
                   ev_out_1C("ev_out_1C"),
                   ev_out_1D("ev_out_1D"),
                   ev_out_1E("ev_out_1E"),
                   ev_out_1F("ev_out_1F"),
                   ev_out_1G("ev_out_1G"),
                   ev_out_1H("ev_out_1H"),
                   ev_out_1I("ev_out_1I"),
                   OVIA("OVIA")
{//{{{
    assert(ATU5_Ptr != NULL);
    mATU5 = ATU5_Ptr;

    mFreeRunCnt = new ATU5_Timer("mFreeRunCnt", mATU5);
    sc_assert(mFreeRunCnt != NULL);
    mFreeRunCnt->SetupGeneralTimer(true, true);
    mFreeRunCnt->SetCounterMode(true);//Free-running mode

    std::ostringstream port_name;
    for (unsigned int index = 0; index < ATU5_Base::emTIAnum; index++) {
        port_name.str("");
        port_name << "mNoiseCanceler" << index;
        mNoiseCanceler[index] = new ATU5_NoiseCanceler(port_name.str().c_str(), mATU5);
        sc_assert(mNoiseCanceler[index] != NULL);
        mNoiseCanceler[index]->SetNoiseCORVal(0);
    }

    mATU5_TIA_Func = new ATU5_TIMERA_Func((std::string)name, this);
    sc_assert(mATU5_TIA_Func != NULL);
    //Initial value of the ports----------------
    for (unsigned int index = 0; index < ATU5_Base::emTIAnum; index++) {
        port_name.str("");
        port_name << "TIA" << index;
        TIA[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TIA[index] != NULL);
        port_name.str("");
        port_name << "ICIA" << index;
        ICIA[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(ICIA[index] != NULL);
        ICIA[index]->initialize(false);
        port_name.str("");
        port_name << "TILOA" << index;
        TILOA[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TILOA[index] != NULL);
        TILOA[index]->initialize(false);
    }
    ev_out_1_0.initialize(false);
    ev_out_1_1.initialize(false);
    ev_out_1_2.initialize(false);
    ev_out_2A.initialize(false);
    ev_out_2B.initialize(false);
    ev_out_1B.initialize(false);
    ev_out_1C.initialize(false);
    ev_out_1D.initialize(false);
    ev_out_1E.initialize(false);
    ev_out_1F.initialize(false);
    ev_out_1G.initialize(false);
    ev_out_1H.initialize(false);
    ev_out_1I.initialize(false);
    OVIA.initialize(false);
    //Initial variables/objects-----------------
    Initialize();
    mCLKNOISE_nPeriod[0] = 0; // move here from Initialize() to fix bug Redmine #84445
    mCLKNOISE_nPeriod[1] = 0; // move here from Initialize() to fix bug Redmine #84445
    //------------------------------------------
    SC_METHOD(HandleCntClkInputMethod);
    dont_initialize();
    sensitive << clkbus_0; // divided from CLK_LSB
    sensitive << clkbus_1; // divided from CLK_LSB
    sensitive << clkbus_2; // divided from CLK_LSB
    sensitive << clkbus_3; // divided from CLK_LSB
    sensitive << clkbus_4; // TCLKA
    sensitive << clkbus_5; // TCLKB | AGCKM from timer B
    sensitive << nc_clk_1; // CLK_LSB | CLK_LSB / 128
    sensitive << nc_clk_2; // AGCK1 from timer B

    for (unsigned int index = 0; index < ATU5_Base::emTIAnum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(TIA[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERA::HandleTIAInputMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleTIAInputMethod"), &opt);
    }

    for (unsigned int index = 0; index < ATU5_Base::emTIAnum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mTIAtoEVOEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERA::HandleTIAtoEVOMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleTIAtoEVOMethod"), &opt);
    }

    for (unsigned int index = 0; index < ATU5_Base::emTIAnum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNoiseCanceler[index]->mResultOutEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERA::OutputNoiseMethod, this, index),
                          sc_core::sc_gen_unique_name("OutputNoiseMethod"), &opt);
    }

    for (unsigned int index = 0; index < ATU5_Base::emTIAnum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNoiseCanceler[index]->mUpdateCntEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERA::SetNCNTAMethod, this, index),
                          sc_core::sc_gen_unique_name("SetNCNTAMethod"), &opt);
    }

    for (unsigned int index = 0; index < emEventNum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERA::AssertEventMethod, this, index),
                          sc_core::sc_gen_unique_name("AssertEventMethod"), &opt);
    }

    for (unsigned int index = 0; index < emEventNum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeAssertEvent[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERA::DeAssertEventMethod, this, index),
                          sc_core::sc_gen_unique_name("DeAssertEventMethod"), &opt);
    }

    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mIntrActiveEvent[interrupt_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERA::AssertIntrMethod, this, interrupt_id),
                          sc_core::sc_gen_unique_name("AssertIntrMethod"), &opt);
    }

    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mIntrInActiveEvent[interrupt_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERA::DeAssertIntrMethod, this, interrupt_id),
                          sc_core::sc_gen_unique_name("DeAssertIntrMethod"), &opt);
    }

    SC_METHOD(StartTimerMethod);
    dont_initialize();
    sensitive << mStartTimerEvent;

    SC_METHOD(HandleTimerCompareMatchMethod);
    dont_initialize();
    sensitive << mFreeRunCnt->mCompareMatchEvent;
    sensitive << mOverflowEvent;

    SC_THREAD(FreeRunCntThread);
    dont_initialize();
    sensitive << mFreeRunCntEvent;
    
    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCaptureTCNTDEvent[interrupt_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERA::CaptureTCNTDMethod, this, interrupt_id),
                          sc_core::sc_gen_unique_name("CaptureTCNTDMethod"), &opt);
    }
}//}}}

/*********************************
// Function     : ~ATU5_TIMERA
// Description  : Destructor of ATU5_TIMERA class
// Parameter    : None
// Return value : None
**********************************/
ATU5_TIMERA::~ATU5_TIMERA(void)
{//{{{
    delete mATU5_TIA_Func;
    for (unsigned int index = 0; index < ATU5_Base::emTIAnum; index++) {
        delete TIA[index];
        delete ICIA[index];
        delete TILOA[index];
        delete mNoiseCanceler[index];
    }
    delete mFreeRunCnt;
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initializes internal variables & output ports
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERA::Initialize (void)
{//{{{
    mCLKBUSPeriod = 0;
    for (unsigned int i = 0; i < emCLKBUSnum; i++) {
        mCLKBUS_nPeriod[i] = 0;
    }
    mIsTimerRunning = false;
    mIsTimerCorMatch = false;
    mIsCLKBusUpdated = false;
    mIsTCNTAUpdated = false;
    mTimeTick = 0;

    for (unsigned int i = 0; i < ATU5_Base::emTIAnum; i++) {
        mCurTIA[i] = false;
    }
    mTimeCLKChangePoint = 0;
    mIsPreCLKBUSzero = true;
    mIsOVFInt = false;
    mIsFreeRunCLKfromB = 0;
    mTCNTD0_val = 0;
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel all operation events
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERA::CancelEvents (void)
{//{{{
    for (unsigned int i = 0; i < ATU5_Base::emTIAnum; i++) {
        mTIAtoEVOEvent[i].cancel();
    }
    for (unsigned int i = 0; i < emEventNum; i++) {
        mAssertEvent[i].cancel();
        mDeAssertEvent[i].cancel();
    }
    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        mIntrActiveEvent[interrupt_id].cancel();
        mIntrInActiveEvent[interrupt_id].cancel();
    }
    mStartTimerEvent.cancel();
    mFreeRunCntEvent.cancel();
    mOverflowEvent.cancel();
}//}}}

/*********************************
// Function     : EnableTIA
// Description  : Enable/Disable Timer A
// Parameter    :
//      is_start            Start/Stop trigger
// Return value : None
**********************************/
void ATU5_TIMERA::EnableTIA (const bool is_start)
{//{{{
    if (is_start) {
        if (!mIsTimerRunning) {
            //Start TCNTA
            SetCLKBusPeriod(mATU5_TIA_Func->GetCLKBusSel());
            if (mIsFreeRunCLKfromB){
                mFreeRunCnt->SetCounterPeriod((double)mCLKBUSPeriod*98/100);
            }else{
                mFreeRunCnt->SetCounterPeriod((double)mCLKBUSPeriod);
            }
            mFreeRunCnt->SetCompareMatchValue(0xFFFFFFFF);
            mFreeRunCnt->SetCounterInitialValue(mATU5_TIA_Func->GetTCNTA()); // set counter initial value from TCNTA register
            StartStopTimer(true);
        }
    } else {
        if (mIsTimerRunning) {
            StartStopTimer(false);
        }
    }
}//}}}

/*********************************
// Function     : RegisterHandlerTIA
// Description  : Handle accessing registers
// Parameter    :
//      cmd                 Register command
// Return value : None
**********************************/
void ATU5_TIMERA::RegisterHandlerTIA (const std::vector<std::string> cmd)
{//{{{
    mATU5_TIA_Func->RegisterHandler(cmd);
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Process reset function
// Parameter    :
//      is_active           Reset is active
// Return value : None
**********************************/
void ATU5_TIMERA::EnableReset (const bool is_active)
{//{{{
    mATU5_TIA_Func->EnableReset(is_active);
    mFreeRunCnt->EnableReset(is_active);
    for (unsigned int i = 0; i < ATU5_Base::emTIAnum; i++) {
        mNoiseCanceler[i]->EnableReset(is_active);
    }
    if (is_active) {
        mRstStateEvent.notify(SC_ZERO_TIME);
        CancelEvents();
        ResetOutputPort();
        Initialize();
        StartStopTimer(false);
    }
}//}}}

/*********************************
// Function     : ResetOutputPort
// Description  : Reset output port when reset is active
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERA::ResetOutputPort (void)
{//{{{
    for (unsigned int event_id = 0; event_id < emEventNum; event_id++) {
        ControlEvents(event_id, false);
    }
    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        ControlInterruptPorts(interrupt_id, false);
    }
    for (unsigned int chan_num = 0; chan_num < ATU5_Base::emTIAnum; chan_num++) {
        TILOA[chan_num]->write(false);
    }
}//}}}

/*********************************
// Function     : HandleCntClkInputMethod
// Description  : Handle the change of clkbus_n clock (n:0->5) and nc_clk_n clock (n:1 or 2)
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERA::HandleCntClkInputMethod (void)
{//{{{
    //Handle the change of clkbus_n clock (n:0->5)
    mCLKBUS_nPeriod[0] = clkbus_0.read(); // divided from CLK_LSB
    mCLKBUS_nPeriod[1] = clkbus_1.read(); // divided from CLK_LSB
    mCLKBUS_nPeriod[2] = clkbus_2.read(); // divided from CLK_LSB
    mCLKBUS_nPeriod[3] = clkbus_3.read(); // divided from CLK_LSB
    mCLKBUS_nPeriod[4] = clkbus_4.read(); // TCLKA
    mCLKBUS_nPeriod[5] = clkbus_5.read(); // TCCLKB | AGCKM from timer B

    if (mCLKBUSPeriod != mCLKBUS_nPeriod[mATU5_TIA_Func->GetCLKBusSel()]) {
        mTimeCLKChangePoint = mATU5->mClkBusStartTime[mATU5_TIA_Func->GetCLKBusSel()];
    }
    if (mIsTimerRunning && (mCLKBUSPeriod != mCLKBUS_nPeriod[mATU5_TIA_Func->GetCLKBusSel()])){
        if (mCLKBUS_nPeriod[mATU5_TIA_Func->GetCLKBusSel()] > 0) {
            if (mCLKBUSPeriod == 0) {
                mIsPreCLKBUSzero = true;
            } else {
                mIsPreCLKBUSzero = false;
            }
            mIsCLKBusUpdated = true;
            SetCLKBusPeriod(mATU5_TIA_Func->GetCLKBusSel());
            mTimeCLKChangePoint = mATU5->mClkBusStartTime[mATU5_TIA_Func->GetCLKBusSel()];
            re_printf("warning","TCNTA clock is changed during Timer A is enabled\n");
            if (mIsFreeRunCLKfromB && mIsTimerCorMatch){
                mIsTimerCorMatch = false;
            }
            mFreeRunCntEvent.notify(SC_ZERO_TIME);
        } else { // stop free-running counter when bus clock used = 0
            mCLKBUSPeriod = mCLKBUS_nPeriod[mATU5_TIA_Func->GetCLKBusSel()];
            mFreeRunCnt->StartStop(false);
            if (!(mIsFreeRunCLKfromB && mIsTimerCorMatch)){
                unsigned int cnt_val = mFreeRunCnt->GetCounterValue();
                mATU5_TIA_Func->SetTCNTA(cnt_val);
            }
        }
    }
    //Update the frequencies of nc_clk_n clock (n:1 or 2)
    if (nc_clk_1.read() != mCLKNOISE_nPeriod[0]) {
        mCLKNOISE_nPeriod[0] = nc_clk_1.read();
    }
    if (nc_clk_2.read() != mCLKNOISE_nPeriod[1]) {
        mCLKNOISE_nPeriod[1] = nc_clk_2.read();
    }

    // When clkbus_5, and/or nc_clk_1, and/or nc_clk_2 are changed
    // timer A's noise cancellation clock should be updated again
    for (unsigned int i = 0; i < ATU5_Base::emTIAnum; i++) {
        SetCLKNoisePeriod(i);
    }
}//}}}

/*********************************
// Function     : HandleTIAInputMethod
// Description  : Handle the change in TIAx input port
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5_TIMERA::HandleTIAInputMethod (const unsigned int chan_num)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        SetCLKNoisePeriod(chan_num);
        if (mNoiseCanceler[chan_num]->IsZeroNoiseCLk()) {
            re_printf("warning","Noise cancellation cannot start due to zero clock\n");
        }
        mCurTIA[chan_num] = TIA[chan_num]->read();
        if (mATU5_TIA_Func->IsNoiseCancelEn(chan_num) && (!mNoiseCanceler[chan_num]->IsZeroNoiseCLk())) {
            mNoiseCanceler[chan_num]->NotifyNoiseCancellation(true, true, mCurTIA[chan_num]);
        } else {
            NotifyTIAChange(chan_num);
            mNoiseCanceler[chan_num]->NotifyNoiseCancellation(false, true, mCurTIA[chan_num]);
        }
    }
}//}}}

/*********************************
// Function     : HandleTIAtoEVOMethod
// Description  : Handle the change in TIAx input port (after noise cancellation-if enabled) syn with CLK_LSB
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5_TIMERA::HandleTIAtoEVOMethod (const unsigned int chan_num)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        //assert events
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1_0",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1_0].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1_1",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1_1].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1_2",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1_2].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_2A",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO2A].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_2B",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO2B].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1B",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1B].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1C",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1C].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1D",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1D].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1E",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1E].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1F",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1F].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1G",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1G].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1H",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1H].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
        if (mATU5_TIA_Func->IsOutEvent("ev_out_1I",chan_num, mCurTIA[chan_num])) {
            mAssertEvent[emEVO1I].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
        }
    }
}//}}}

/*********************************
// Function     : OutputNoiseMethod
// Description  : Handle noise cancellation output
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5_TIMERA::OutputNoiseMethod (const unsigned int chan_num)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        if (mATU5_TIA_Func->IsNoiseCancelEn(chan_num) && (!mNoiseCanceler[chan_num]->IsZeroNoiseCLk())) {
            mCurTIA[chan_num] = mNoiseCanceler[chan_num]->GetCurOutval();
        } else {
            mCurTIA[chan_num] = TIA[chan_num]->read();
        }
        NotifyTIAChange(chan_num);
    }
}//}}}

/*********************************
// Function     : SetNCNTAMethod
// Description  : Handle noise cancellation counter output
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5_TIMERA::SetNCNTAMethod (const unsigned int chan_num)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        unsigned int cnt_val = mNoiseCanceler[chan_num]->GetUpdatedCntValue();
        mATU5_TIA_Func->SetNCNTA(cnt_val, chan_num);
    }
}//}}}

/*********************************
// Function     : AssertEventMethod
// Description  : Set event to 1
// Parameter    :
//      event_id            Event ID
// Return value : None
**********************************/
void ATU5_TIMERA::AssertEventMethod (const unsigned int event_id)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        ControlEvents(event_id, true);
        mDeAssertEvent[event_id].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : DeAssertEventMethod
// Description  : Clear event to 0
// Parameter    :
//      event_id            Event ID
// Return value : None
**********************************/
void ATU5_TIMERA::DeAssertEventMethod (const unsigned int event_id)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        ControlEvents(event_id, false);
    }
}//}}}

/*********************************
// Function     : AssertIntrMethod
// Description  : Set interrupt port to 1
// Parameter    :
//      interrupt_id        Interrupt ID
// Return value : None
**********************************/
void ATU5_TIMERA::AssertIntrMethod (const unsigned int interrupt_id)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        if (interrupt_id == emOVIA) {
            mATU5_TIA_Func->UpdateTimerStatus(false,0);
        } else {
            mATU5_TIA_Func->UpdateTimerStatus(true,interrupt_id-1);
            unsigned int cap_val = mFreeRunCnt->GetCounterValue();
            if (CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint) == 0){
                cap_val = cap_val - 1;
            } else {
                cap_val = GetCounterValue();
            }
            mATU5_TIA_Func->UpdateICRAVal(cap_val,interrupt_id-1,false);
            mATU5_TIA_Func->UpdateICRAVal(mTCNTD0_val,interrupt_id-1,true);
        }
        ControlInterruptPorts(interrupt_id, true);
        mIntrInActiveEvent[interrupt_id].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : DeAssertIntrMethod
// Description  : Clear interrupt port to 0
// Parameter    :
//      interrupt_id        Interrupt ID
// Return value : None
**********************************/
void ATU5_TIMERA::DeAssertIntrMethod (const unsigned int interrupt_id)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        ControlInterruptPorts(interrupt_id, false);
    }
}//}}}

/*********************************
// Function     : StartTimerMethod
// Description  : Start counter of TCNTA
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERA::StartTimerMethod (void)
{//{{{
    unsigned int cnt_val = GetCounterValue();
    if (mIsFreeRunCLKfromB){
        if (mCLKBUSPeriod > 0){
            mFreeRunCnt->SetCounterInitialValue(mATU5_TIA_Func->GetTCNTA());
            mFreeRunCnt->StartStop(true);
            mTimeTick = GetCurTime();
            mIsTimerCorMatch = false;
        }else{
            mATU5_TIA_Func->SetTCNTA(cnt_val);
        }
    }else{
        if (!mATU5->CheckClockPeriod("CLK_LSB") && (mCLKBUSPeriod == 0)) {
            mIsTimerRunning = false;
        }
        mFreeRunCnt->StartStop(mIsTimerRunning);
        if (mIsTimerRunning) {
            mFreeRunCnt->SetCounterInitialValue(mATU5_TIA_Func->GetTCNTA()+1);
            mTimeTick = GetCurTime();
            mIsTimerCorMatch = false;
        } else {
            mATU5_TIA_Func->SetTCNTA(cnt_val);
        }
    }
}//}}}

/*********************************
// Function     : HandleTimerCompareMatchMethod
// Description  : Handle compare match event of TCNTA
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERA::HandleTimerCompareMatchMethod (void)
{//{{{
    if ((mATU5->CheckClockPeriod("CLK_LSB") && mCLKBUSPeriod != 0)) {
        mTimeTick = GetCurTime();
        mIsTimerCorMatch = true;
        mIsOVFInt = true;
        mFreeRunCntEvent.notify(SC_ZERO_TIME);
        if (mIsTCNTAUpdated) {
            mIsTCNTAUpdated = false;
        }
    }
}//}}}

/*********************************
// Function     : FreeRunCntThread
// Description  : Handle the free-running counter of TCNTA
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERA::FreeRunCntThread (void)
{//{{{
    for(;;) {
        if(!IsTimerRunning() || IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB")) { // don't check clock = 0 when TCLKA, TCLKB, or AGCKM is using
            wait();
            continue;
        }
        try {
            while (true) {
                if ((mTimeTick == GetCurTime())&&(mIsTimerCorMatch)) {
                    mFreeRunCnt->StartStop(false);
                    mATU5_TIA_Func->SetTCNTA(0xFFFFFFFF);
                    if (!mIsFreeRunCLKfromB){
                        wait((double)mCLKBUSPeriod, mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                        if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB")) { // don't check clock = 0 when TCLKA, TCLKB, or AGCKM is using
                            throw vpcl::emRstException;
                        }
                    }
                    if (mIsCLKBusUpdated) {
                        mIsCLKBusUpdated = false;
                        mFreeRunCnt->SetCounterPeriod((double)mCLKBUSPeriod);
                        wait(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint), mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                        if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB")) { // don't check clock = 0 when TCLKA, TCLKB, or AGCKM is using
                            throw vpcl::emRstException;
                        }
                    }
                    if (!mIsFreeRunCLKfromB){
                        mIsTimerCorMatch = false;
                        mFreeRunCnt->StartStop(true);
                        mFreeRunCnt->SetCounterInitialValue(0);
                    }
                    mIsTCNTAUpdated = false;
                    if (mATU5_TIA_Func->GetTCNTA() != 0xFFFFFFFF) {
                        mFreeRunCnt->SetCounterInitialValue(1 + mATU5_TIA_Func->GetTCNTA());
                    }
                    if (mIsOVFInt) {
                        mIntrActiveEvent[emOVIA].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
                        mIsOVFInt = false;
                    }
                } else {
                    unsigned int cnt_val = mFreeRunCnt->GetCounterValue();
                    mFreeRunCnt->StartStop(false);
                    if (mIsCLKBusUpdated) {
                        if (mIsFreeRunCLKfromB){
                            mFreeRunCnt->SetCounterPeriod((double)mCLKBUSPeriod*98/100);
                        }else{
                            mFreeRunCnt->SetCounterPeriod((double)mCLKBUSPeriod);
                        }
                    }
                    if (!mIsFreeRunCLKfromB){// only wait for normal case, not wait for clock from B
                        wait(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint), mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                        if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB")) { // don't check clock = 0 when TCLKA, TCLKB, or AGCKM is using
                            throw vpcl::emRstException;
                        }
                    }
                    mFreeRunCnt->StartStop(true);
                    if (mIsTCNTAUpdated) { // user writes TCNTA
                        mIsTCNTAUpdated = false;
                        cnt_val = mATU5_TIA_Func->GetTCNTA();

                        // If AGCKM is used & set to 0, counter should not be started (fix #79779 bug)
                        if (mIsFreeRunCLKfromB && !mCLKBUSPeriod) {
                            mFreeRunCnt->StartStop(false);
                        }
                    }
                    if ((!mIsCLKBusUpdated) || (mIsPreCLKBUSzero)) {
                        if (mIsFreeRunCLKfromB){
                            mFreeRunCnt->SetCounterInitialValue(cnt_val);
                        }else{
                            mFreeRunCnt->SetCounterInitialValue(cnt_val+1);
                        }
                    }
                    mIsCLKBusUpdated = false;
                }
                wait();
            }
        }
        catch (vpcl::eResetException) {
            mIsTimerCorMatch = false;
            mIsCLKBusUpdated = false;
            mIsTCNTAUpdated = false;
        }
    }
}//}}}

/*********************************
// Function     : ControlEvents
// Description  : Control event output
// Parameter    :
//      event_id            Event ID
//      set_val             Setting value
// Return value : None
**********************************/
void ATU5_TIMERA::ControlEvents (const unsigned int event_id, const bool set_val)
{//{{{
    switch (event_id) {
        case emEVO1_0:
            ev_out_1_0.write(set_val);
            break;
        case emEVO1_1:
            ev_out_1_1.write(set_val);
            break;
        case emEVO1_2:
            ev_out_1_2.write(set_val);
            break;
        case emEVO2A:
            ev_out_2A.write(set_val);
            break;
        case emEVO2B:
            ev_out_2B.write(set_val);
            break;
        case emEVO1B:
            ev_out_1B.write(set_val);
            break;
        case emEVO1C:
            ev_out_1C.write(set_val);
            break;
        case emEVO1D:
            ev_out_1D.write(set_val);
            break;
        case emEVO1E:
            ev_out_1E.write(set_val);
            break;
        case emEVO1F:
            ev_out_1F.write(set_val);
            break;
        case emEVO1G:
            ev_out_1G.write(set_val);
            break;
        case emEVO1H:
            ev_out_1H.write(set_val);
            break;
        default://emEVO1I
            ev_out_1I.write(set_val);
            break;
    }
}//}}}

/*********************************
// Function     : DumpInterruptMsg
// Description  : Dump interrupt message
// Parameter    :
//      intr_name           Interrupt name
//      value               true: active level; false: de-active level
// Return value : None
**********************************/
void ATU5_TIMERA::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mATU5->mDumpInterrupt) {
        if (value) {
            re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
        } else {
            re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
        }
   }
}//}}}

/*********************************
// Function     : ControlInterruptPorts
// Description  : Control interrupt ports
// Parameter    :
//      interrupt_id        Interrupt ID
//      set_val             Setting value
// Return value : None
**********************************/
void ATU5_TIMERA::ControlInterruptPorts (const unsigned int interrupt_id, const bool set_val)
{//{{{
    std::string intr_name = "";
    bool pre_value = false;
    bool written = false;
    switch (interrupt_id) {
        case emOVIA:
            intr_name = "OVIA";
            pre_value = OVIA.read();
            if (!set_val || (mATU5_TIA_Func->IsInterruptEnable("OVEA")&& set_val)) {
                OVIA.write(set_val);
                written = true;
            }
            break;
        case emICIA0:
            intr_name = "ICIA0";
            pre_value = ICIA[0]->read();
            if (!set_val || (mATU5_TIA_Func->IsInterruptEnable("ICIEA0") && set_val)) {
                ICIA[0]->write(set_val);
                written = true;
            }
            break;
        case emICIA1:
            intr_name = "ICIA1";
            pre_value = ICIA[1]->read();
            if (!set_val || (mATU5_TIA_Func->IsInterruptEnable("ICIEA1") && set_val)) {
                ICIA[1]->write(set_val);
                written = true;
            }
            break;
        case emICIA2:
            intr_name = "ICIA2";
            pre_value = ICIA[2]->read();
            if (!set_val || (mATU5_TIA_Func->IsInterruptEnable("ICIEA2") && set_val)) {
                ICIA[2]->write(set_val);
                written = true;
            }
            break;
        case emICIA3:
            intr_name = "ICIA3";
            pre_value = ICIA[3]->read();
            if (!set_val || (mATU5_TIA_Func->IsInterruptEnable("ICIEA3") && set_val)) {
                ICIA[3]->write(set_val);
                written = true;
            }
            break;
        case emICIA4:
            intr_name = "ICIA4";
            pre_value = ICIA[4]->read();
            if (!set_val || (mATU5_TIA_Func->IsInterruptEnable("ICIEA4") && set_val)) {
                ICIA[4]->write(set_val);
                written = true;
            }
            break;
        case emICIA5:
            intr_name = "ICIA5";
            pre_value = ICIA[5]->read();
            if (!set_val || (mATU5_TIA_Func->IsInterruptEnable("ICIEA5") && set_val)) {
                ICIA[5]->write(set_val);
                written = true;
            }
            break;
        case emICIA6:
            intr_name = "ICIA6";
            pre_value = ICIA[6]->read();
            if (!set_val || (mATU5_TIA_Func->IsInterruptEnable("ICIEA6") && set_val)) {
                ICIA[6]->write(set_val);
                written = true;
            }
            break;
        default://emICIA7
            intr_name = "ICIA7";
            pre_value = ICIA[7]->read();
            if (!set_val || (mATU5_TIA_Func->IsInterruptEnable("ICIEA7") && set_val)) {
                ICIA[7]->write(set_val);
                written = true;
            }
            break;
    }
    if ((pre_value != set_val) && written) {
        DumpInterruptMsg(intr_name,set_val);
    }
}//}}}

/*********************************
// Function     : StartStopTimer
// Description  : Start/stop free-running counter A
// Parameter    :
//      is_start            Start/Stop trigger
// Return value : None
**********************************/
void ATU5_TIMERA::StartStopTimer (const bool is_start)
{//{{{
    if (mIsFreeRunCLKfromB){// for clock from B (update due to updated code of B)
        mIsTimerRunning = is_start;
        mStartTimerEvent.notify(SC_ZERO_TIME);
    }else{
        mIsTimerRunning = is_start;
        if (mIsTimerRunning && (!mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0))) {
            mIsTimerRunning = false;
        }
        if (mIsTimerRunning) {
            mStartTimerEvent.notify(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint),mATU5->mTimeResolutionUnit);
        } else {
            mStartTimerEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

/*********************************
// Function     : SetCLKBusPeriod
// Description  : Set TCNTA clock period
// Parameter    :
//      clkbus_sel          Clock bus selection
// Return value : None
**********************************/
void ATU5_TIMERA::SetCLKBusPeriod (const unsigned int clkbus_sel)
{//{{{
    mCLKBUSPeriod = mCLKBUS_nPeriod[clkbus_sel];
    if (mCLKBUSPeriod == 0){
        mClkZeroEvent.notify(SC_ZERO_TIME);
    }
    mIsFreeRunCLKfromB = ((clkbus_sel == 5) && (mATU5->GetClkBus5Setting() == 1));
}//}}}

/*********************************
// Function     : SetCLKNoisePeriod
// Description  : Set NCNTA clock period
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5_TIMERA::SetCLKNoisePeriod (const unsigned int chan_num)
{//{{{
    unsigned int noiseclk_sel = mATU5_TIA_Func->GetNoiseCLKSel(chan_num);
    sc_dt::uint64 clk_period = 0;
    double start_point = 0; // revised by chuongle Jul/28/2018: GetCurTime() -> 0 
    if (noiseclk_sel == 0) {
        clk_period = mCLKNOISE_nPeriod[1];
        start_point = mATU5->mAGCK1StartTime; // start time of timerB AGCK1 as nc_clk_2 is divided from it
    } else if (noiseclk_sel == 1) {
        clk_period = mCLKNOISE_nPeriod[0];
        start_point = mATU5->mTimeCLKChangePoint[0]; // start time of CLK_LSB as nc_clk_1 is divided from it
    } else if (noiseclk_sel == 2) { // start time of clkbus_5
        clk_period = mCLKBUS_nPeriod[5];
        start_point = mATU5->mClkBusStartTime[5];
    }
    bool is_clk_from_B = ((noiseclk_sel == 0) || ( (noiseclk_sel == 2) && (mATU5->GetClkBus5Setting() == 1) ) );
    mNoiseCanceler[chan_num]->SetCLKNoisePeriod(clk_period, start_point, is_clk_from_B);
}//}}}

/*********************************
// Function     : NotifyTIAChange
// Description  : Notify the change of TIA after noise cancellation(if any)
// Parameter    :
//      chan_num            Channel number 0 -> 7
//      tia_val             TIA value after noise cancellation(if any)
// Return value : None
**********************************/
void ATU5_TIMERA::NotifyTIAChange (const unsigned int chan_num)
{//{{{
    bool curOutTia = (bool)(TILOA[chan_num]->read());
    if (curOutTia != mCurTIA[chan_num]) {
        TILOA[chan_num]->write(mCurTIA[chan_num]);
        mTIAtoEVOEvent[chan_num].cancel();
        mTIAtoEVOEvent[chan_num].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
        if (mATU5_TIA_Func->IsOutEvent("intr_icia",chan_num, mCurTIA[chan_num])) {
            double sync_time = CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]) + (2*mATU5->mCLK_LSBPeriod);
            double sync_time2 = CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]) + (mATU5->mCLK_LSBPeriod);
            mIntrActiveEvent[chan_num+1].notify(sync_time, mATU5->mTimeResolutionUnit);
            mCaptureTCNTDEvent[chan_num+1].notify(sync_time2, mATU5->mTimeResolutionUnit);
        }
    }
}//}}}

/*********************************
// Function     : IsModelResetActive
// Description  : Check active level of presetz
// Parameter    : None
// Return value : Current active level of presetz
**********************************/
bool ATU5_TIMERA::IsModelResetActive(void)
{//{{{
    bool ret = false;
    if (mATU5->mIsResetHardActive[1] || mATU5->mIsResetCmdReceive[1]) {
        ret = true;
    }
    return ret;
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time
**********************************/
double ATU5_TIMERA::GetCurTime(void)
{//{{{
    return sc_time_stamp().to_double();
}//}}}

/*********************************
// Function     : CalculateClockEdge
// Description  : Calculate synchronous time
// Parameter    :
//      clock_period        Clock period (unit depend on mATU5->mTimeResolutionValue)
//      is_pos              true: raising edge; false: falling edge
//      add_period          true: 0 < syn_time <= 1; false: 0 <= syn_time < 1
//      time_point          Time point
//      time_change_point   Time point clock is updated
// Return value : The duration from time_point to next valid clock edge
**********************************/
double ATU5_TIMERA::CalculateClockEdge (double clock_period, const bool is_pos, const bool add_period, const double time_point, const double time_change_point)
{//{{{
    if (clock_period == 0){
        return 0;
    }
    double period_num = 0;
    period_num = (time_point - time_change_point) / clock_period;
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (!add_period) {
        if (is_pos) {
            if ((double)mod_period_num < period_num) {
                mod_period_num++;
            }
        } else {
            if (((double)mod_period_num + 0.5) < period_num) {
                mod_period_num++;
            }
        }
    } else {
        mod_period_num++;
    }
    double edge_point = 0, mod_period_num_db = 0;
    if (is_pos) {
        mod_period_num_db = (double)mod_period_num;
    } else {
        mod_period_num_db = (double)mod_period_num + 0.5;
    }
    edge_point = mod_period_num_db * clock_period - (time_point - time_change_point);
    return edge_point;
}//}}}

/*********************************
// Function     : IsTimerRunning
// Description  : Check Timer A is enable or disable
// Parameter    : None
// Return value : Current status of Timer A
**********************************/
bool ATU5_TIMERA::IsTimerRunning(void)
{//{{{
    return mIsTimerRunning;
}//}}}

/*********************************
// Function     : IsNoiseCancelDuration
// Description  : Check Noise cancellation is in process or not
// Parameter    :
//      chan_num            Channel number
// Return value :
//      true                In noise cancellation duration (NCNTAx is counting)
//      false               NCNTAx became 0x0
**********************************/
bool ATU5_TIMERA::IsNoiseCancelDuration(const unsigned int chan_num)
{//{{{
    return mNoiseCanceler[chan_num]->IsNoiseCancelDuration();
}//}}}

/*********************************
// Function     : GetCounterValue
// Description  : Get current counter value of TCNTA
// Parameter    : None
// Return value : Current counter value
**********************************/
unsigned int ATU5_TIMERA::GetCounterValue (void)
{//{{{
    unsigned int value = mFreeRunCnt->GetCounterValue();
    if (mIsTCNTAUpdated || mIsTimerCorMatch) {
        value = mATU5_TIA_Func->GetTCNTA();
    }
    return value;
}//}}}

/*********************************
// Function     : GetNoiseCounterValue
// Description  : Get current counter value of NCNTA
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : Current counter value
**********************************/
unsigned int ATU5_TIMERA::GetNoiseCounterValue (const unsigned int chan_num)
{//{{{
    unsigned int reg_val = mATU5_TIA_Func->GetNCNTA(chan_num);
    return (mNoiseCanceler[chan_num]->GetNoiseCounterValue(reg_val));
}//}}}

/*********************************
// Function     : GetTILRAVal
// Description  : Get current TILRA value
// Parameter    : None
// Return value : Current counter value
**********************************/
unsigned int ATU5_TIMERA::GetTILRAVal (void)
{//{{{
    unsigned int ret = 0;
    for (unsigned int i = 0; i < 8; i++) {
        bool value = TILOA[i]->read();
        ret = ret | ((value&0x1)<<i);
    }
    return ret;
}//}}}

/*********************************
// Function     : UpdateCounterValue
// Description  : Update counter value of TCNTA when users write into TCNTA register
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERA::UpdateCounterValue (void)
{//{{{
    if (mIsTimerRunning) {
        mIsTCNTAUpdated = true;
        if (mIsOVFInt) {
            mIsOVFInt = false;
        }
        if (mATU5_TIA_Func->GetTCNTA() != 0xFFFFFFFE) {
            mFreeRunCntEvent.notify(SC_ZERO_TIME);
        } else {
            mOverflowEvent.notify(CalculateClockEdge((double)mCLKBUSPeriod, true, true, GetCurTime(), mTimeCLKChangePoint),mATU5->mTimeResolutionUnit);
        }
    }
}//}}}

/*********************************
// Function     : UpdateNoiseCounterValue
// Description  : Update counter value of NCNTA when users write into NCNTA register
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5_TIMERA::UpdateNoiseCounterValue (const unsigned int chan_num)
{//{{{
    mNoiseCanceler[chan_num]->SetNoiseCounterValue(mATU5_TIA_Func->GetNCNTA(chan_num)); //Update counter value
}//}}}

/*********************************
// Function     : SetNoiseCancelMode
// Description  : Set Noise Cancellation mode
// Parameter    :
//      chan_num            Channel number
// Return value : None
**********************************/
void ATU5_TIMERA::SetNoiseCancelMode (const unsigned int chan_num)
{//{{{
    unsigned int mode = GetNoiseCancelMode(chan_num);
    mNoiseCanceler[chan_num]->SetNoiseCancelMode(mode);
    mATU5_TIA_Func->SetNCNTA(0, chan_num);
}//}}}

/// Get noise cancellation mode
unsigned int ATU5_TIMERA::GetNoiseCancelMode(const unsigned int chan_num)
{//{{{
    unsigned int mode = mATU5->GetNoiseCancelMode("Timer A");
    if (mode == 0) {
        mode = mATU5_TIA_Func->GetNoiseCancelMode(chan_num);
    }
    return mode;
}//}}}

/*********************************
// Function     : SetNoiseCORVal
// Description  : Set Noise Cancellation compare match value
// Parameter    :
//      chan_num            Channel number
//      cor_val             Compare match value
// Return value : None
**********************************/
void ATU5_TIMERA::SetNoiseCORVal (const unsigned int chan_num, const unsigned int cor_val)
{//{{{
    mNoiseCanceler[chan_num]->SetNoiseCORVal(cor_val);
}//}}}

void ATU5_TIMERA::CaptureTCNTDMethod (const unsigned int interrupt_id)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        if (interrupt_id != emOVIA) {
            mTCNTD0_val = mATU5->mATU5_TimerD->GetTCNTDVal("TCNT1D0");
            if (mATU5_TIA_Func->GetICRA2SEL() == 1) {
                mTCNTD0_val = mATU5->mATU5_TimerD->GetTCNTDVal("TCNT2D0");
            }
        }
    }
}//}}}

void ATU5_TIMERA::EnableNoiseCancel (const unsigned int chan_num, const bool is_enable)
{//{{{
    mNoiseCanceler[chan_num]->Enable(is_enable);
}//}}}

/*********************************
// Function     : ATU5_NoiseCanceler
// Description  : Constructor of ATU5_NoiseCanceler class
// Parameter    :
//      name            Module name
// Return value : None
**********************************/
ATU5_NoiseCanceler::ATU5_NoiseCanceler(sc_module_name name, ATU5_Base *ATU5_Ptr):
                   sc_module(name)
{//{{{
    mATU5 = ATU5_Ptr;
    mNoiseCancelCnt = new ATU5_Timer("mNoiseCancelCnt", mATU5);
    sc_assert(mNoiseCancelCnt != NULL);
    mNoiseCancelCnt->SetCompareMatchValue(0);
    mNoiseCancelCnt->SetCounterMode(true);//Free-running mode
    Initialize();
    mCLKNOISEPeriod = 0; // move here from Initialize() to fix bug Redmine #84445
    mTIAVal = false;

    SC_METHOD(HandleNoiseCompareMatchMethod);
    dont_initialize();
    sensitive << mNoiseCancelCnt->mCompareMatchEvent;

    SC_METHOD(NoiseEnMethod);
    dont_initialize();
    sensitive << mNoiseEnEvent;

    SC_METHOD(NoiseCancellationMethod);
    dont_initialize();
    sensitive << mNoiseCancellationEvent;

    SC_METHOD(StartNoiseTimerMethod);
    dont_initialize();
    sensitive << mStartNoiseTimerEvent;

    SC_THREAD(NoiseCancelCntThread);
    dont_initialize();
    sensitive << mNoiseCntEvent;
    
    SC_METHOD(TIASyncMethod);
    dont_initialize();
    sensitive << mTIASyncEvent;

    SC_THREAD(ArbitrateStartStopCounterThread);
    dont_initialize();
    sensitive << mArbitrateStartStopCounterEvent;
}//}}}

/*********************************
// Function     : ~ATU5_NoiseCanceler
// Description  : Destructor of ATU5_NoiseCanceler class
// Parameter    : None
// Return value : None
**********************************/
ATU5_NoiseCanceler::~ATU5_NoiseCanceler(void)
{//{{{
    delete mNoiseCancelCnt;
}//}}}

/*********************************
// Function     : IsZeroNoiseCLk
// Description  : Check clock noise period
// Parameter    : None
// Return value : true (zero clock)/false (valid clock)
**********************************/
bool ATU5_NoiseCanceler::IsZeroNoiseCLk(void)
{//{{{
    if ((mCLKNOISEPeriod == 0) && !mIsNoiseCLKfromB) {
        return true;
    }
    return false;
}//}}}

/*********************************
// Function     : GetCurOutval
// Description  : Get current output value
// Parameter    : None
// Return value : Current output value
**********************************/
bool ATU5_NoiseCanceler::GetCurOutval(void)
{//{{{
    return mCurOutval;
}//}}}

/*********************************
// Function     : IsNoiseCancelDuration
// Description  : Check Noise cancellation is in process or not
// Parameter    : None
// Return value :
//      true                In noise cancellation duration (NCNTAx is counting)
//      false               NCNTAx became 0x0
**********************************/
bool ATU5_NoiseCanceler::IsNoiseCancelDuration(void)
{//{{{
    return mIsNoiseCancelDuration;
}//}}}

/*********************************
// Function     : GetUpdatedCntValue
// Description  : Get current counter value of NCNTA
// Parameter    : None
// Return value : Current counter value
**********************************/
unsigned int ATU5_NoiseCanceler::GetUpdatedCntValue (void)
{//{{{
    return mUpdatedCntVal;
}//}}}

/*********************************
// Function     : GetNoiseCounterValue
// Description  : Get current counter value of NCNTA
// Parameter    :
//      reg_val             Counter value is stored in count register
// Return value : Current counter value
**********************************/
unsigned int ATU5_NoiseCanceler::GetNoiseCounterValue (const unsigned int reg_val)
{//{{{
    unsigned int ret = 0;
    // read value CNT in period: compare match <= read time < compare match + 1 CLK  --->return cor val
    if ((mCompareMatchTime > 0) 
        && ((mCompareMatchTime + (double)mATU5->mCLK_LSBPeriod) > GetCurTime()) && (GetCurTime() >= mCompareMatchTime)){
        ret = mNoiseCORVal;
    }else{
        ret = mNoiseCancelCnt->GetCounterValue();
        if (mIsNCNTAUpdated || mIsNoiseTimerCorMatch) {
            ret = reg_val;
        }
    }
    return ret;
}//}}}

/*********************************
// Function     : SetNoiseCancelMode
// Description  : Set Noise Cancellation mode
// Parameter    :
//      nc_mode             Noise cancellation mode
//          0                   Premature-transition cancellation mode
//          1                   Minimum time-at-level cancellation mode
//          2                   Level-accumulated cancellation mode
// Return value : None
**********************************/
void ATU5_NoiseCanceler::SetNoiseCancelMode (const unsigned int nc_mode)
{//{{{
    if (mNoiseCancelMode != nc_mode){
        mNoiseCancelMode = nc_mode;
        mIsNoiseCancelDuration = 0;
        mNoiseCancelCnt->StartStop(false);
    }

    if ((mNoiseCancelMode == 2) && (mIsEnable)) {
        NotifyNoiseCancellation(true, false, false);
    }
}//}}}

/*********************************
// Function     : SetNoiseCORVal
// Description  : Set Noise Cancellation compare match value
// Parameter    :
//      cor_val             Compare match value
// Return value : None
**********************************/
void ATU5_NoiseCanceler::SetNoiseCORVal (const unsigned int cor_val)
{//{{{
    mNoiseCORVal = cor_val;
    mNoiseCancelCnt->SetCompareMatchValue(cor_val);
    if(mIsNoiseCancelDuration) {
        mNoiseCntEvent.notify(SC_ZERO_TIME); //Update compare match value
    }
}//}}}

/*********************************
// Function     : SetNoiseCounterValue
// Description  : Set Noise Cancellation counter value
// Parameter    :
//      nc_cnt              Noise Cancellation counter value
// Return value : None
**********************************/
void ATU5_NoiseCanceler::SetNoiseCounterValue (const unsigned int nc_cnt)
{//{{{
    if(mIsNoiseCancelDuration) {
        mIsNCNTAUpdated = true;
        mNoiseCntEvent.notify(SC_ZERO_TIME); //Update counter value
    }
    mUpdatedCntVal = nc_cnt;
}//}}}

/*********************************
// Function     : SetCLKNoisePeriod
// Description  : Set Noise Cancellation clock period
// Parameter    :
//      nc_clk              Noise Cancellation clock period
//      time_change_point   Time point clock is updated
// Return value : NoiseCancelDuration
**********************************/
void ATU5_NoiseCanceler::SetCLKNoisePeriod (const sc_dt::uint64 nc_clk, const double time_change_point, bool is_clk_from_b)
{//{{{
    mIsNoiseCLKfromB = is_clk_from_b;
    if (mCLKNOISEPeriod != nc_clk){
        mTimeCLKChangePoint = time_change_point;
        if (nc_clk > 0){
            if ((mCLKNOISEPeriod == 0) && mIsNoiseCancelDuration) {
                mIsPreCLKNOISEzero = true;
            }
            mCLKNOISEPeriod = nc_clk;
        } else {
            mCLKNOISEPeriod = 0;
            mClkZeroEvent.notify(SC_ZERO_TIME);
        }
        if (mIsNoiseCancelDuration) {
            if (mCLKNOISEPeriod != 0) {
                mIsCLKNoiseUpdated = true;
            } else { // updating to zero clock
                mNoiseCancelCnt->StartStop(false);
            }
            mNoiseCntEvent.notify(SC_ZERO_TIME); //Update count clock
            if ((mNoiseCancelMode == 2) && (mCLKNOISEPeriod == 0)) {
                mNoiseCancelCnt->SetCounterPeriod((double)mCLKNOISEPeriod);
            }
        }
    }
}//}}}

/*********************************
// Function     : NotifyNoiseCancellation
// Description  : Notify Noise Cancellation duration
// Parameter    :
//      nc_en               Noise Cancellation enable
//      input_trig          Input is changed
//      input_val           Input value if changed
// Return value : None
**********************************/
void ATU5_NoiseCanceler::NotifyNoiseCancellation (const bool nc_en, const bool input_trig, const bool input_val)
{//{{{
    if (nc_en && ((mCLKNOISEPeriod != 0) || mIsNoiseCLKfromB)) {// not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B
        if (mNoiseCancelMode != 2) {

            if (!mIsInputProcessing) { // noise counter is NOT counting
                mIsInputProcessing = true;
                mSyncTIA = input_val;
                double syn_time = CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]) + mATU5->mCLK_LSBPeriod;
                mRequestStartCounter = true;
                mArbitrateStartStopCounterEvent.notify(syn_time, mATU5->mTimeResolutionUnit);
                if ((mNoiseCancelMode == 0) && (mIsNoiseCancelDuration)) { // premature-transition cancellation mode
                    mCurTIA = mSyncTIA;
                }
            } else { // noise counter is counting
                if ((mNoiseCancelMode == 0) && (mIsNoiseCancelDuration)) { // premature-transition cancellation mode
                    mSyncTIA = input_val;
                    mCurTIA = input_val;
                }
                if (mNoiseCancelMode == 1) { // minumum time-at-level cancellation mode
                    mNoiseEnEvent.cancel();
                    mIsInputProcessing = false;
                    mSyncTIA = input_val;
                    mCurTIA = input_val;
                }
            }
        } else { // level-accumulated cancellation mode
            mIsNoiseCancelDuration = true;
            mIsLevelNoiseCancellerStart = false;
            if (input_trig) {
                if (mTIAVal != input_val) {
                    if (mTIASyncTimeVec.empty()) {
                        mTIASyncEvent.notify((double)mATU5->mCLK_LSBPeriod*2, mATU5->mTimeResolutionUnit);
                    }
                    double sync_time = sc_time_stamp().to_double() + (double)mATU5->mCLK_LSBPeriod*2;
                    mTIASyncTimeVec.push_back(sync_time);
                    mTIAVal = input_val;
                }
            }
            if (mNoiseCORVal > 0){// not start noise counter if COR = 0
                if (!mIsNoiseCLKfromB){// not apply for clock output from B due to updated code of B
                    double start_time = CalculateClockEdge((double)mCLKNOISEPeriod, true, false, GetCurTime(), mTimeCLKChangePoint);
                    mStartNoiseTimerEvent.cancel();
                    mStartNoiseTimerEvent.notify(start_time,mATU5->mTimeResolutionUnit);
                }
            }
        }
    } else {
        if (mTIAVal != input_val) {
            mTIAVal = input_val;
            mCurTIA = input_val;
        }
    }
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initializes internal variables & output ports
// Parameter    : None
// Return value : None
**********************************/
void ATU5_NoiseCanceler::Initialize (void)
{//{{{
    mIsNoiseCLKfromB = 0;
    mNoiseCancelMode = 0;
    mUpdatedCntVal = 0;
    mIsNoiseCancelDuration = false;
    mSyncTIA = false;
    mCurTIA = false;
    mCurOutval = false;
    mIsNoiseTimerCorMatch = false;
    mIsCLKNoiseUpdated = false;
    mIsNCNTAUpdated = false;
    mIsNoiseCntUp = false;
    mNoiseTimeTick = 0;
    mTimeCLKChangePoint = 0;
    mIsEnable = false;
    mNoiseCORVal = 0;
    mIsInputProcessing = false;
    mTIASyncTimeVec.clear();
    mIsPreCLKNOISEzero = false;
    mRequestStopCounter = 0;
    mRequestStartCounter = 0;
    mIsLevelNoiseCancellerStart = 0;
    mCompareMatchTime = 0;
    // add to fix bug #83786 (start)
    if (mNoiseCancelCnt != NULL) {
        mNoiseCancelCnt->SetCompareMatchValue(0);
    }
    // add to fix bug #83786 (end)
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel all operation events
// Parameter    : None
// Return value : None
**********************************/
void ATU5_NoiseCanceler::CancelEvents (void)
{//{{{
    mNoiseEnEvent.cancel();
    mNoiseCancellationEvent.cancel();
    mStartNoiseTimerEvent.cancel();
    mNoiseCntEvent.cancel();
    mTIASyncEvent.cancel();
    mArbitrateStartStopCounterEvent.cancel();
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Process reset function
// Parameter    :
//      is_active           Reset is active
// Return value : None
**********************************/
void ATU5_NoiseCanceler::EnableReset (const bool is_active)
{//{{{
    mNoiseCancelCnt->EnableReset(is_active);
    if (is_active) {
        mRstStateEvent.notify(SC_ZERO_TIME);
        CancelEvents();
        Initialize();
    }
}//}}}

/*********************************
// Function     : HandleNoiseCompareMatchMethod
// Description  : Handle compare match event of NCNTA
// Parameter    : None
// Return value : None
**********************************/
void ATU5_NoiseCanceler::HandleNoiseCompareMatchMethod (void)
{//{{{
    if ((mATU5->CheckClockPeriod("CLK_LSB")) && ((mCLKNOISEPeriod != 0) || mIsNoiseCLKfromB)) {// not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B
        mCompareMatchTime = GetCurTime();
        mNoiseTimeTick = GetCurTime();
        mIsNoiseTimerCorMatch = true;
        if (mNoiseCancelMode == 1){// chanle added 20181002 to arbitrate start/stop counter
            mRequestStopCounter = true;
            mArbitrateStartStopCounterEvent.notify();
        }else{
            mNoiseCntEvent.notify(SC_ZERO_TIME); //Compare match
        }
    }
}//}}}

/*********************************
// Function     : NoiseEnMethod
// Description  : Handle the noise cancellation
// Parameter    : None
// Return value : None
**********************************/
void ATU5_NoiseCanceler::NoiseEnMethod (void)
{//{{{
    if ((mATU5->CheckClockPeriod("CLK_LSB")) && ((mCLKNOISEPeriod != 0) || mIsNoiseCLKfromB)) {// not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B

        if (mIsInputProcessing) {
            mIsInputProcessing = false;
            if (mNoiseCancelMode == 0) {

                if (mCurTIA != mSyncTIA) {
                    mCurTIA = mSyncTIA;
                } 
                if (!mIsNoiseCancelDuration) {
                    NotifyTIAChange();
                    mNoiseCancellationEvent.notify((double)mATU5->mCLK_LSBPeriod,mATU5->mTimeResolutionUnit);
                }
            } else if (mNoiseCancelMode == 1) {
                mNoiseCancelCnt->SetCounterInitialValue(0);

                if (mCurTIA != mSyncTIA) {
                    mCurTIA = mSyncTIA;
                    mNoiseCancellationEvent.notify();
                }
            } else { //mode = 2
                mIsNoiseCancelDuration = true;
                double start_time = CalculateClockEdge((double)mCLKNOISEPeriod, true, false, GetCurTime(), mTimeCLKChangePoint);
                mStartNoiseTimerEvent.cancel();
                mStartNoiseTimerEvent.notify(start_time,mATU5->mTimeResolutionUnit);
            }
        }
    }
}//}}}

/// Arbitrate start counter for new TIA, or stop counter for previous TIA (stop higher priority than start)
void ATU5_NoiseCanceler::ArbitrateStartStopCounterThread()
{//{{{
    while(1){
        if(IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB")) {// not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B
            wait();
            continue;
        }
        wait(SC_ZERO_TIME);
        wait(SC_ZERO_TIME);
        wait(SC_ZERO_TIME);
        wait(SC_ZERO_TIME);
        wait(SC_ZERO_TIME);
        wait(SC_ZERO_TIME);
        if (mRequestStopCounter){
            mNoiseCntEvent.notify();
        }else if (mRequestStartCounter){
            mNoiseEnEvent.notify();
        }
        mRequestStartCounter = false;
        mRequestStopCounter = false;
        wait();
    }
}//}}}

/*********************************
// Function     : NoiseCancellationMethod
// Description  : Handle the noise cancellation
// Parameter    : None
// Return value : None
**********************************/
void ATU5_NoiseCanceler::NoiseCancellationMethod (void)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && ((mCLKNOISEPeriod != 0) || mIsNoiseCLKfromB)) {// not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B
        if (!mIsNoiseCancelDuration) {
            if (mNoiseCancelMode != 2) {
                mIsNoiseCancelDuration = true;
                mStartNoiseTimerEvent.notify(SC_ZERO_TIME);
            }
        } else {
            if (mNoiseCancelMode == 1) {
                mIsNoiseCancelDuration = false;
                mUpdatedCntVal = 0;
                mUpdateCntEvent.notify(SC_ZERO_TIME);
                mStartNoiseTimerEvent.notify(SC_ZERO_TIME); //stop
            }
        }
    }
}//}}}

/*********************************
// Function     : StartNoiseTimerMethod
// Description  : Start noise cancellation counter
// Parameter    : None
// Return value : None
**********************************/
void ATU5_NoiseCanceler::StartNoiseTimerMethod (void)
{//{{{
    mNoiseCancelCnt->SetupGeneralTimer(true, true);
    if (mIsNoiseCLKfromB && (mCLKNOISEPeriod == 0)){// not process for case noise clock from B and clock = 0 due to updated timer B
        return;
    }

    if (!mATU5->CheckClockPeriod("CLK_LSB") || (mCLKNOISEPeriod == 0)) {// not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B
        mIsNoiseCancelDuration = false;
    }
    if (mIsNoiseCancelDuration == 1) {
        if (mNoiseCancelMode != 2) {
                if (mIsNoiseCLKfromB){// got 98% period of clock output from B due to updated code of B
                    mNoiseCancelCnt->SetCounterPeriod((double)mCLKNOISEPeriod * 98/100);
                }else{
                    mNoiseCancelCnt->SetCounterPeriod((double)mCLKNOISEPeriod);
                }
            mNoiseCancelCnt->SetupGeneralTimer(true, true);
            if (mNoiseCancelMode == 1){//minimum time-at-level cancellation mode
                mNoiseCancelCnt->SetCounterInitialValue(mUpdatedCntVal);
            }else{
                mNoiseCancelCnt->SetCounterInitialValue(mUpdatedCntVal);
            }
            mNoiseCancelCnt->StartStop(true);
            mNoiseTimeTick = GetCurTime();
        } else {
            if (mIsNoiseCntUp != mSyncTIA) {
                mNoiseCancelCnt->StartStop(false);
                unsigned int cnt_val = GetNoiseCounterValue(mUpdatedCntVal);
                mIsNoiseCntUp = mSyncTIA;
                mIsLevelNoiseCancellerStart = true;
                mNoiseCancelCnt->SetupGeneralTimer(false, mIsNoiseCntUp);
                if (mIsNoiseCLKfromB){// got 98% period of clock output from B due to updated code of B
                    mNoiseCancelCnt->SetCounterPeriod((double)mCLKNOISEPeriod * 98/100);
                    if (mIsNoiseCntUp) {
                        mNoiseCancelCnt->SetCompareMatchValue(mNoiseCORVal);
                        mNoiseCancelCnt->StartStop(true);
                        mNoiseCancelCnt->SetCounterInitialValue((cnt_val) & 0xFFFF);
                    } else {
                        mNoiseCancelCnt->SetCompareMatchValue(0);
                        mNoiseCancelCnt->StartStop(true);
                        mNoiseCancelCnt->SetCounterInitialValue((cnt_val) & 0xFFFF);
                    }
                }else{
                    mNoiseCancelCnt->SetCounterPeriod((double)mCLKNOISEPeriod);
                    if (mIsNoiseCntUp) {
                        mNoiseCancelCnt->SetCompareMatchValue(mNoiseCORVal);
                        mNoiseCancelCnt->StartStop(true);
                        mNoiseCancelCnt->SetCounterInitialValue((cnt_val+1) & 0xFFFF);
                    } else {
                        if (cnt_val == 0) {
                            mIsNoiseCancelDuration = false;
                            mUpdatedCntVal = 0;
                            mUpdateCntEvent.notify(SC_ZERO_TIME);
                            mCurTIA = 0;
                            NotifyTIAChange();
                        } else if (cnt_val == 1) {
                            mNoiseCancelCnt->SetCompareMatchValue(0);
                            mNoiseTimeTick = GetCurTime();
                            mIsNoiseTimerCorMatch = true;
                            mNoiseCntEvent.notify(SC_ZERO_TIME); //Compare match
                        } else {
                            mNoiseCancelCnt->SetCompareMatchValue(0);
                            mNoiseCancelCnt->StartStop(true);
                            mNoiseCancelCnt->SetCounterInitialValue((cnt_val-1) & 0xFFFF);
                        }
                    }
                }
            }else{
            }
        }
    } else { // mIsNoiseCancelDuration == 0
        if (mNoiseCancelMode != 2) { // premature-transition cancellation mode | minimum time-at-level cancellation mode
            mUpdatedCntVal = 0;
            if (mNoiseCancelMode == 1) {
                mNoiseCancelCnt->SetCounterInitialValue(mUpdatedCntVal);
            }
            mUpdateCntEvent.notify(SC_ZERO_TIME);
        }
        mNoiseCancelCnt->StartStop(false);
    }
}//}}}

/*********************************
// Function     : NoiseCancelCntThread
// Description  : Handle the free-running counter of TCNTA
// Parameter    : None
// Return value : None
**********************************/
void ATU5_NoiseCanceler::NoiseCancelCntThread (void)
{//{{{
    for(;;) {
        if(!mIsNoiseCancelDuration || IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB")) { // not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B
            wait();
            continue;
        }
        try {
            while (true) {
                if ((mNoiseTimeTick == GetCurTime()) && (mIsNoiseTimerCorMatch)) { // a compare match from noise counter
                    unsigned int cur_CORval = mNoiseCancelCnt->GetCompareMatchValue();
                    mNoiseCancelCnt->StartStop(false);
                    if (mNoiseCancelMode == 2) { // level-accumulated cancellation mode
                        mCurTIA = mSyncTIA;
                        mNoiseCancelCnt->SetCounterInitialValue(cur_CORval);
                    }
                    mUpdatedCntVal = cur_CORval&0xFFFF;
                    mUpdateCntEvent.notify(SC_ZERO_TIME);
                    if (mNoiseCancelMode == 1) { // time-at-level cancellation mode
                        NotifyTIAChange();
                    }

                    wait(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB")) {// not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B
                        throw vpcl::emRstException;
                    }
                    mIsNoiseTimerCorMatch = false;
                    if (mNoiseCancelMode != 2) { // premature-transition | time-at-level cancellation mode
                        mUpdatedCntVal = 0;
                        mUpdateCntEvent.notify(SC_ZERO_TIME);
                        mIsNoiseCancelDuration = false;
                    } else { // level-accumulated cancellation mode
                        NotifyTIAChange();
                    }
                    if (mNoiseCancelMode == 0) { // premature-transition cancellation mode
                        NotifyTIAChange();
                    }
                } else { // NOT a compare match from noise counter
                    unsigned int cnt_val = mNoiseCancelCnt->GetCounterValue();
                    mNoiseCancelCnt->StartStop(false);
                    if (mIsCLKNoiseUpdated) {
                        mIsCLKNoiseUpdated = false;
                        if (mCLKNOISEPeriod > 0){
                            if (mIsNoiseCLKfromB){// got 98% period of clock output from B due to updated code of B
                                mNoiseCancelCnt->SetCounterPeriod((double)mCLKNOISEPeriod * 98/100);
                            }else{
                                mNoiseCancelCnt->SetCounterPeriod((double)mCLKNOISEPeriod);
                            }
                        }
                    }
                    if (mIsNoiseCancelDuration && mIsPreCLKNOISEzero) {
                        mIsPreCLKNOISEzero = false;
                    } else {
                        wait(CalculateClockEdge((double)mCLKNOISEPeriod, true, true, GetCurTime(), mTimeCLKChangePoint), mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                        wait(SC_ZERO_TIME);// avoid racing in usk mode
                    }
                    if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB")) { // not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B
                        throw vpcl::emRstException;
                    }
                    if (mIsNCNTAUpdated) {
                        if (!mIsNoiseCLKfromB || (mCLKNOISEPeriod > 0)){
                            mIsNCNTAUpdated = false;
                            cnt_val = mUpdatedCntVal;
                        }
                    }
                    if (mNoiseCancelMode != 2) {
                        if (mCLKNOISEPeriod > 0){
                            if (mIsNoiseCLKfromB){
                                mNoiseCancelCnt->SetCounterInitialValue(cnt_val);
                            }else{
                                mNoiseCancelCnt->SetCounterInitialValue(cnt_val+1);
                            }
                            mNoiseCancelCnt->StartStop(true);
                        }
                    } else {
                        if (mIsNoiseCLKfromB){// if clock from timer B
                            if ((mCLKNOISEPeriod > 0) && mIsNoiseCancelDuration){
                                if (mIsLevelNoiseCancellerStart){// in case level-accum already start
                                    mNoiseCancelCnt->SetCounterInitialValue(cnt_val);
                                    mNoiseCancelCnt->StartStop(true);
                                }else{
                                    mStartNoiseTimerEvent.notify();
                                }
                            }
                        }else{// if normal clock
                            if (mIsNoiseCntUp && mSyncTIA) {
                                mNoiseCancelCnt->StartStop(true);
                                mNoiseCancelCnt->SetCounterInitialValue(cnt_val+1);
                            } else {
                                if (mIsLevelNoiseCancellerStart){
                                    if (cnt_val <= 1) {
                                        mIsNoiseCancelDuration = false;
                                        mUpdatedCntVal = 0;
                                        mNoiseCancelCnt->SetCounterInitialValue(mUpdatedCntVal);
                                        mUpdateCntEvent.notify(SC_ZERO_TIME);
                                        mCurTIA = 0;
                                        NotifyTIAChange();
                                    } else {
                                        mNoiseCancelCnt->StartStop(true);
                                        mNoiseCancelCnt->SetCounterInitialValue(cnt_val-1);
                                    }
                                }
                            }
                        }
                    }
                }
                wait();
                if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB")) {// not check clock = 0 for AGCKM, AGCK1 due to updated code of timer B
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
            mIsNoiseTimerCorMatch = false;
            mIsCLKNoiseUpdated = false;
            mIsNCNTAUpdated = false;
        }
    }
}//}}}

/*********************************
// Function     : NotifyTIAChange
// Description  : Notify the change of TIA after noise cancellation(if any)
// Parameter    : None
// Return value : None
**********************************/
void ATU5_NoiseCanceler::NotifyTIAChange (void)
{//{{{
    if (mCurOutval != mCurTIA) {
        mCurOutval = mCurTIA;
        mResultOutEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : IsModelResetActive
// Description  : Check active level of presetz
// Parameter    : None
// Return value : Current active level of presetz
**********************************/
bool ATU5_NoiseCanceler::IsModelResetActive(void)
{//{{{
    bool ret = false;
    if (mATU5->mIsResetHardActive[1] || mATU5->mIsResetCmdReceive[1]) {
        ret = true;
    }
    return ret;
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time
**********************************/
double ATU5_NoiseCanceler::GetCurTime(void)
{//{{{
    return sc_time_stamp().to_double();
}//}}}

/*********************************
// Function     : CalculateClockEdge
// Description  : Calculate synchronous time
// Parameter    :
//      clock_period        Clock period (unit depend on mATU5->mTimeResolutionValue)
//      is_pos              true: raising edge; false: falling edge
//      add_period          true: 0 < syn_time <= 1; false: 0 <= syn_time < 1
//      time_point          Time point
//      time_change_point   Time point clock is updated
// Return value : The duration from time_point to next valid clock edge
**********************************/
double ATU5_NoiseCanceler::CalculateClockEdge (double clock_period, const bool is_pos, const bool add_period, const double time_point, const double time_change_point)
{//{{{
    if (clock_period == 0){
        return 0;
    } 
    double period_num = 0;
    period_num = (time_point - time_change_point) / clock_period;
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (!add_period) {
        if (is_pos) {
            if ((double)mod_period_num < period_num) {
                mod_period_num++;
            }
        } else {
            if (((double)mod_period_num + 0.5) < period_num) {
                mod_period_num++;
            }
        }
    } else {
        mod_period_num++;
    }
    double edge_point = 0, mod_period_num_db = 0;
    if (is_pos) {
        mod_period_num_db = (double)mod_period_num;
    } else {
        mod_period_num_db = (double)mod_period_num + 0.5;
    }
    edge_point = mod_period_num_db * clock_period - (time_point - time_change_point);
    return edge_point;
}//}}}

void ATU5_NoiseCanceler::Enable (const bool is_enable)
{//{{{
    if ((mIsEnable) && (!is_enable) && (mNoiseCancelMode == 2)) {
        if (mIsNoiseCancelDuration) {
            mNoiseCancelCnt->StartStop(false);
            mUpdatedCntVal = mNoiseCancelCnt->GetCounterValue();
            mUpdateCntEvent.notify(SC_ZERO_TIME);
            mCurTIA = mSyncTIA;
            NotifyTIAChange();
        }
    } else if ((!mIsEnable) && (is_enable)) {
        mSyncTIA = mTIAVal;
    }
    mIsEnable = is_enable;
}//}}}

void ATU5_NoiseCanceler::TIASyncMethod (void)
{//{{{
    if ((mIsEnable) && (mNoiseCancelMode == 2)) {
        mIsNoiseCancelDuration = true;
        mSyncTIA = !mSyncTIA;
        if (mNoiseCORVal > 0){
            if (!mIsNoiseCLKfromB){
                mNoiseCancelCnt->StartStop(false);
                double start_time = CalculateClockEdge((double)mCLKNOISEPeriod, true, false, GetCurTime(), mTimeCLKChangePoint);
                mStartNoiseTimerEvent.notify(start_time,mATU5->mTimeResolutionUnit);
            }
        }else{// Write TIA, not start noise counter if COR = 0
            mCurTIA = mSyncTIA;
            NotifyTIAChange();
        }
        mTIASyncTimeVec.erase(mTIASyncTimeVec.begin());
        // Next sync time for TIA
        if (!mTIASyncTimeVec.empty()) {
            double next_time = mTIASyncTimeVec[0] - sc_time_stamp().to_double();
            mTIASyncEvent.notify(next_time, mATU5->mTimeResolutionUnit);
        }
    }
}//}}}

// vim600: set foldmethod=marker :
