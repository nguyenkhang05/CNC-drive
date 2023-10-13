// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERC_SUB.cpp,v 1.3 2018/03/06 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <stdint.h>
#include "ATU5010_TIMERC.h"
#include "ATU5010_TIMERC_Func.h"
#include "ATU5010_TIMERC_SUB.h"
#include "ATU5010_TIMERC_SUB_Func.h"
#include "ATU5010_Timer.h"
#include "ATU5010.h"

///*********************************
/// Function     : ATU5010_TIMERC_SUB
/// Description  : Constructor of ATU5010_TIMERC_SUB class
///*********************************
ATU5010_TIMERC_SUB::ATU5010_TIMERC_SUB (sc_module_name name,
            ATU5010_TIMERC *ATU5010_TIMERC_Ptr,
            unsigned int sub_id):
                sc_module(name),
                ATU5010_TIMERC_SUB_AgentController(),
                PCLK("PCLK"),
                nc_clk("nc_clk"),
                clock_bus0("clock_bus0"),
                clock_bus1("clock_bus1"),
                clock_bus2("clock_bus2"),
                clock_bus3("clock_bus3"),
                clock_bus4("clock_bus4"),
                clock_bus5("clock_bus5"),
                TIOC0("TIOC0"),
                TIOC1("TIOC1"),
                TIOC2("TIOC2"),
                TIOC3("TIOC3"),
                TOCE0("TOCE0"),
                TOCE1("TOCE1"),
                TOCE2("TOCE2"),
                TOCE3("TOCE3"),
                TOC0("TOC0"),
                TOC1("TOC1"),
                TOC2("TOC2"),
                TOC3("TOC3"),
                i_event_1_0("i_event_1_0"),
                i_event_2A("i_event_2A"),
                i_event_2B("i_event_2B"),
                INT_OVI("INT_OVI")
{//{{{
    assert(ATU5010_TIMERC_Ptr != NULL);
    mTIMERC = ATU5010_TIMERC_Ptr;
    mModuleName = (std::string) name;
    mSub_id = sub_id;
    //Initialize variables - for clock, time resolution, reset
    //Bind clock, reset signal and target socket to Bus IF
    mTIC_SUBFunc = new ATU5010_TIMERC_SUB_Func((std::string)name, this, sub_id);
    sc_assert(mTIC_SUBFunc != NULL);
    INT_OVI.initialize(false);
    Initialize();
    std::ostringstream port;
    for (unsigned int channel = 0; channel < emChannel; channel++) {
        port.str(""); port << "INT_GRC" << channel;
        INT_GRC[channel] = new sc_out<bool>(port.str().c_str());
        sc_assert(INT_GRC[channel] != NULL);
        INT_GRC[channel]->initialize(false);
        port.str(""); port << "INT_OCRC" << channel;
        INT_OCRC[channel] = new sc_out<bool>(port.str().c_str());
        sc_assert(INT_OCRC[channel] != NULL);
        INT_OCRC[channel]->initialize(false);
    }
    //add for update - Initialize the TOCE ports
    TOCE0.initialize(true);
    TOCE1.initialize(true);
    TOCE2.initialize(true);
    TOCE3.initialize(true);
    TOC0.initialize(false);
    TOC1.initialize(false);
    TOC2.initialize(false);
    TOC3.initialize(false);

    ////////////////////////////////////////////
    mFreeRunCnt = new ATU5010_Timer("mFreeRunCnt",mTIMERC->mATU5010);
    sc_assert(mFreeRunCnt != NULL);
    mFreeRunCnt->SetCounterMode(true);
    mFreeRunCnt->SetupGeneralTimer(false, true);

    // Instance the vice counter for each channel
    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream name;
        name.str("");name << "mRunCntGRC" << index;
        mRunCntGRC[index] = new ATU5010_Timer(name.str().c_str(),mTIMERC->mATU5010);
        mRunCntGRC[index]->SetCounterMode(true);// Set count-up
        mRunCntGRC[index]->SetupGeneralTimer(false,true); //Edit 0808
    } //instance the Counter for Compare match with GRC

    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream name;
        name.str("");name << "mRunCntOCRC" << index;
        mRunCntOCRC[index] = new ATU5010_Timer(name.str().c_str(),mTIMERC->mATU5010);
        mRunCntOCRC[index]->SetCounterMode(true);// Set count-up
        mRunCntOCRC[index]->SetupGeneralTimer(false,true); //Edit 0808
    } //instance the Counter for Compare match with OCRC

    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream port_name;
        port_name.str("");
        port_name << "mNoiseCanceler" << index;
        mNoiseCanceler[index] = new ATU5010_NoiseCanceler(port_name.str().c_str(), mTIMERC->mATU5010);
        sc_assert(mNoiseCanceler[index] != NULL);
        mNoiseCanceler[index]->SetNoiseCORVal(0);
    } //

    // Define Method
    //For Noise Canceler
    for (unsigned int index = 0; index < ATU5010_TIMERC_SUB_AgentController::emChannel; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNoiseCanceler[index]->mResultOutEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::OutputNoiseMethod, this, index),
                          sc_core::sc_gen_unique_name("OutputNoiseMethod"), &opt);
    }

    for (unsigned int index = 0; index < ATU5010_TIMERC_SUB_AgentController::emChannel; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNoiseCanceler[index]->mUpdateCntEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::SetNCNTCMethod, this, index),
                          sc_core::sc_gen_unique_name("SetNCNTCMethod"), &opt);
    }
    //************************************************************************************

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCaptureOCRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::CaptureOCRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("CaptureOCRCMethod"), &opt);
    } 

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCaptureGRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::CaptureGRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("CaptureGRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteTIOCOutputEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::WriteTIOCOutputMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("WriteTIOCOutputMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRisingInterruptGRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::RisingInterruptGRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("RisingInterruptGRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRisingInterruptOCRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::RisingInterruptOCRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("RisingInterruptOCRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFallingInterruptGRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::FallingInterruptGRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("FallingInterruptGRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFallingInterruptOCRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::FallingInterruptOCRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("FallingInterruptOCRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteTOCEPortEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::WriteTOCEPortMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("WriteTOCEPortMethod"), &opt);
    }

    SC_METHOD (RisingInterruptOVFMethod);
    dont_initialize();
    sensitive << mRisingInterruptOVFEvent;

    SC_METHOD (FallingInterruptOVFMethod);
    dont_initialize();
    sensitive << mFallingInterruptOVFEvent;

    SC_METHOD(HandleCmpOvfMethod);
    dont_initialize();
    sensitive << mFreeRunCnt->mCompareMatchEvent;
    sensitive << mHandleCmpOvfEvent;
    sensitive << mFreeRunCnt->mOverflowEvent;

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRunCntGRC[signal_id]->mCompareMatchEvent);
        opt.set_sensitivity(&mHandleCmpGRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::HandleCmpGRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleCmpGRCMethod"), &opt);
    }
    
    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRunCntGRC[signal_id]->mOverflowEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::HandleOvfGRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleOvfGRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRunCntOCRC[signal_id]->mCompareMatchEvent);
        opt.set_sensitivity(&mHandleCmpOCRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::HandleCmpOCRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleCmpOCRCMethod"), &opt);
    }
    
    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRunCntOCRC[signal_id]->mOverflowEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::HandleOvfOCRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleOvfOCRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHandleFlagAndInterruptOCRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::HandleFlagAndInterruptOCRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleFlagAndInterruptOCRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHandleFlagAndInterruptGRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::HandleFlagAndInterruptGRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleFlagAndInterruptGRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStartCntGRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::StartCntGRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StartCntGRCMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStartCntOCRCEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERC_SUB::StartCntOCRCMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StartCntOCRCMethod"), &opt);
    }

    SC_METHOD(StartTimerMethod);
    dont_initialize();
    sensitive << mStartTimerEvent;

    SC_METHOD(HandleInputChannel0Method);
    dont_initialize();
    sensitive << i_event_1_0;
    sensitive << i_event_2A;
    sensitive << i_event_2B;
    sensitive << mHandleInputChannel0Event;

    SC_METHOD(HandleInputChannel1Method);
    dont_initialize();
    sensitive << i_event_1_0;
    sensitive << i_event_2A;
    sensitive << i_event_2B;
    sensitive << mHandleInputChannel1Event;

    SC_METHOD(HandleInputChannel2Method);
    dont_initialize();
    sensitive << i_event_1_0;
    sensitive << i_event_2A;
    sensitive << i_event_2B;
    sensitive << mHandleInputChannel2Event;
    
    SC_METHOD(HandleInputChannel3Method);
    dont_initialize();
    sensitive << i_event_1_0;
    sensitive << i_event_2A;
    sensitive << i_event_2B;
    sensitive << mHandleInputChannel3Event;

    SC_METHOD(HandleTIOC0Method);
    dont_initialize();
    sensitive << TIOC0;

    SC_METHOD(HandleTIOC1Method);
    dont_initialize();
    sensitive << TIOC1;

    SC_METHOD(HandleTIOC2Method);
    dont_initialize();
    sensitive << TIOC2;

    SC_METHOD(HandleTIOC3Method);
    dont_initialize();
    sensitive << TIOC3;

    SC_METHOD(HandlePWMMethod);
    dont_initialize();
    sensitive << mHandlePWMEvent;

    SC_METHOD(HandleCntClkInputMethod);
    dont_initialize();
    sensitive << clock_bus0;
    sensitive << clock_bus1;
    sensitive << clock_bus2;
    sensitive << clock_bus3;
    sensitive << clock_bus4;
    sensitive << clock_bus5;
    sensitive << nc_clk;

    SC_METHOD(ClearTimeMethod);
    dont_initialize();
    sensitive << mClearTimeEvent;

    SC_METHOD(InterruptWithPCLKMethod);
    dont_initialize();
    sensitive << mInterruptWithPCLKEvent;

    SC_METHOD(RestartTimerMethod);
    dont_initialize();
    sensitive << mRestartTimerEvent;
    
    SC_METHOD(RestartCUCRCMethod);
    dont_initialize();
    sensitive << mRestartCUCRCEvent;
}//}}}

///*********************************
/// Function     : ~ATU5010_TIMERC_SUB
/// Description  : Destructor of ATU5010_TIMERC_SUB class
///*********************************
ATU5010_TIMERC_SUB::~ATU5010_TIMERC_SUB (void)
{//{{{
    delete mTIC_SUBFunc;
    for (unsigned int index = 0; index < emChannel; index++) {
        delete mNoiseCanceler[index];
        delete mRunCntOCRC[index];
        delete mRunCntGRC[index];
        delete INT_GRC[index];
        delete INT_OCRC[index];
    }
    delete mFreeRunCnt;
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize value
///*********************************
void ATU5010_TIMERC_SUB::Initialize(void)
{//{{{
    mTIOC1 = false;
    mTIOC2 = false;
    mTIOC3 = false;
    mTIOC0 = false;
    mIA1 = false;
    mIA2A = false;
    mIA2B = false;
    mIsOvf = false;
    mIsOVFPWM = false;
    mCLKBUSPeriod = 0;
    mPre_CntClkPeriod = 0;
    mPCLKPeriod = 0;
    for (unsigned int i = 0; i < 6;i++) {
        mCLKBUS_nPeriod[i] = 0;
    }
    mLimitValue = 0xFFFFFFFF;
    mIs_Clear = false;
    mIs_Upper_Limit = false;
    mIsTSTRCTrue = false;

    mIsTimerRunning = false;
    mTimeCLKChangePoint = 0;
    for (unsigned int i = 0; i < emChannel; i++) {
        mRangeR2C[i] = 0;
        mRangeR1C[i] = 0;
        mTimeCLKStart[i] = 0;
        mCurCLKCntPeriod[i] = 0;
        mPre_OCRC[i] = 0xFFFFFFFF;

        mTIOC[i] = false;
        mCurTIC[i] = false;
        mIsOneShotRunning[i] = false;
        mIsCntGRCWorking[i] = false;
        mIsGRCWaitOvf[i] = false;
        mIsWaitOCRCMatching[i] = false;
        mIsWaitGRCMatching[i] = false;
        mIsGRCWaitStart[i] = false;
        mIsOCRCWaitStart[i] = false;
        //
        mIsOneshot[i] = false;
        mIsForcedOCRC[i] = false;
        mIsForcedGRC[i] = false;
        mChannelMode[i] = 0;
        mChannelValue[i] = 0;
        mIsUpdateGRC[i] = false;
        //
        mGRCValue[i] = 0xFFFFFFFF;
        mOCRCValue[i] = 0xFFFFFFFF;
        //update for TOCE
        mCurTOCE[i] = true;
        mTOCE[i] = true;
        mGRCMatchTime[i] = 0;
        mOCRCMatchTime[i] = 0;
        mIsGRC1stMatch[i] = false;
        mIsOCRC1stMatch[i] = false;
    }
    for (unsigned int i = 0; i < 2; i++) {
        mCLKBUS_nPeriod[i] = 0;
    }

}//}}}

/***********************************
Function        : EnableReset
Description     : Reset is called
***********************************/
void ATU5010_TIMERC_SUB::EnableReset(const bool is_active)
{//{{{
    mTIC_SUBFunc->EnableReset(is_active);
    mFreeRunCnt->EnableReset(is_active);
    Initialize();
    CancelEvent();
    for (unsigned int channel = 0; channel < 4; channel++) {
        mRunCntOCRC[channel]->EnableReset(is_active);
        mRunCntGRC[channel]->EnableReset(is_active);
        mNoiseCanceler[channel]->EnableReset(is_active);
        INT_OCRC[channel]->write(false);
        INT_GRC[channel]->write(false);
    }
    TIOC0.write(false);
    TIOC1.write(false);
    TIOC2.write(false);
    TIOC3.write(false);
    INT_OVI.write(false);
}//}}}

/***********************************
Function        : CancelEvent
Description     : Cancel Event in Reset state
***********************************/
void ATU5010_TIMERC_SUB::CancelEvent(void)
{//{{{
    for (unsigned channel = 0; channel < emChannel; channel++) {
        mNoiseCntEvent[channel].cancel();
        mCaptureOCRCEvent[channel].cancel();
        mCaptureGRCEvent[channel].cancel();
        mWriteTIOCOutputEvent[channel].cancel();
        mNoiseCancellationEvent[channel].cancel();
        mRisingInterruptOCRCEvent[channel].cancel();
        mRisingInterruptGRCEvent[channel].cancel();
        mFallingInterruptOCRCEvent[channel].cancel();
        mFallingInterruptGRCEvent[channel].cancel();
        mHandleFlagAndInterruptGRCEvent[channel].cancel();
        mHandleFlagAndInterruptOCRCEvent[channel].cancel();
        mCompareZeroMatchEvent[channel].cancel();
        mClkZeroEvent[channel].cancel();
        mHandleCmpOCRCEvent[channel].cancel();
        mHandleCmpGRCEvent[channel].cancel();
        mStartCntOCRCEvent[channel].cancel();
        mStartCntGRCEvent[channel].cancel();
        mOneShotProcessEvent[channel].cancel();
    }
    mHandleInputChannel0Event.cancel();
    mHandleInputChannel1Event.cancel();
    mHandleInputChannel2Event.cancel();
    mHandleInputChannel3Event.cancel();
    mRisingInterruptOVFEvent.cancel();
    mFallingInterruptOVFEvent.cancel();
    mWritetoTIOC3Event.cancel();
    mWritetoTIOC2Event.cancel();
    mWritetoTIOC1Event.cancel();
    mStartTimerEvent.cancel();
    mHandlePWMEvent.cancel();
    mRstStateEvent.cancel();
    mInterruptWithPCLKEvent.cancel();
    mRestartTimerEvent.cancel();
    mClearTimeEvent.cancel();
    mHandleCmpOvfEvent.cancel();
    mRestartCUCRCEvent.cancel();
}//}}}

/***********************************
Function        : IsModelResetActive
Description     : Check the reset of model
Return value    : true/false
***********************************/
bool ATU5010_TIMERC_SUB::IsModelResetActive(void)
{//{{{
    bool ret = false;
    if (mTIMERC->IsModelResetActive()) {
        ret = true;
    }
    return ret;
}//}}}

/***********************************
Function        : RunningCounter
Description     : TSTRC set counter start
***********************************/
void ATU5010_TIMERC_SUB::RunningCounter(const bool is_run)
{//{{{
      mTimeCLKChangePoint = mTimeCLKStart[mTIC_SUBFunc->GetCLKBusSel()];
    if (is_run) {
        re_printf ("info","Sub %i is active.\n", mSub_id);
        StartStopTimer(true);
    } else {
        re_printf("info","Stop Counter - Sub %i.\n", mSub_id);
        StartStopTimer(false);
    }
}//}}}

/***********************************
Function        : CalculateClockEdge
Description     : Calculate the Clock
***********************************/
double ATU5010_TIMERC_SUB::CalculateClockEdge (const std::string clock_name, const bool is_pos, const bool add_period, const double time_point, const double time_change_point)
{//{{{
	double period_num = 0;
    if ( (clock_name == "TCNTCCLK") && ((double)mCLKBUSPeriod != 0) ) {
    	period_num = (time_point - time_change_point) / (double)mCLKBUSPeriod;
    } else if ((clock_name == "PCLK") && ((double)mTIMERC->mPCLKPeriod != 0) ) {
        period_num = (time_point - time_change_point) / (double)mTIMERC->mPCLKPeriod;
    } else {
    	return 0;
    }
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
    if (clock_name == "TCNTCCLK") {
        edge_point = mod_period_num_db * (double)mCLKBUSPeriod - (time_point - time_change_point);
    } else if (clock_name == "PCLK") {
        edge_point = mod_period_num_db * (double)mTIMERC->mPCLKPeriod - (time_point - time_change_point);
    }
    return edge_point;
}//}}}

/***********************************
Function        : SetCounterPeriod
Description     : TSTRC set counter start
***********************************/
void ATU5010_TIMERC_SUB::SetCounterPeriod(const double period)
{//{{{
    if (period != 0) {
        mFreeRunCnt->SetCounterPeriod(period);
        for (unsigned int channel = 0; channel < 4; channel++) {
            mRunCntGRC[channel]->SetCounterPeriod(period);
            mRunCntOCRC[channel]->SetCounterPeriod(period);
        }
    }
}//}}}

/*********************************
// Function     : GetCounterValue
// Description  : Get the current value of TCNTC
// Return value : Current counter value
**********************************/
unsigned int ATU5010_TIMERC_SUB::GetCounterValue(void)
{//{{{
    unsigned int value = 0;
    value = mFreeRunCnt->GetCounterValue();
    return value;
}//}}}

/***********************************
Function        : StartStopTimer
Description     : Call to start or stop counter
***********************************/
void ATU5010_TIMERC_SUB::StartStopTimer (const bool is_start)
{//{{{
    mIsTSTRCTrue = is_start;
    bool temp = is_start;
    mCLKBUSPeriod = mCLKBUS_nPeriod[mTIC_SUBFunc->GetCLKBusSel()];
    mTimeCLKChangePoint = mTimeCLKStart[mTIC_SUBFunc->GetCLKBusSel()];
    SetCounterPeriod(mCLKBUSPeriod);//Set Counter clock
    if (temp && (mCLKBUSPeriod == 0)) {
        temp = false;
    }
    if (temp) { //Start Counter 
        if (mChannelMode[0] == emPWM) { //PWM mode
            re_printf("info","PWM mode.\n");
            mLimitValue = mGRCValue[0];
            mFreeRunCnt->SetCompareMatchValue(mLimitValue);
        } else { //Other mode
            if (mIs_Upper_Limit) { //add 0818
                mLimitValue = (unsigned int)(mTIC_SUBFunc->GetCUCRC());
            }
            mFreeRunCnt->SetCompareMatchValue(mLimitValue);
        }
        for (unsigned int channel = 0; channel <= 3; channel++) {//Set virtual counter for GRC and OCRC
            if (mChannelMode[channel] != emOneshot) {
                SetCmpOCRCValue(channel,mOCRCValue[channel]);
                SetCmpGRCValue(channel,mGRCValue[channel]);
            }
        }
        mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK", true,false, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
    } else { //Stop Counter
        if (!mIsTimerRunning && !mIsTSTRCTrue) {
            mStartTimerEvent.cancel();
            mRestartTimerEvent.cancel();
            mTIC_SUBFunc->SetCounterStatus(false);
        } else {
            mTIC_SUBFunc->SetTCNTC(GetCounterValue()); //Stop counter and update the cur value to register
            mStartTimerEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

/*********************************
// Function     : StartTimerMethod
// Description  : Start timer
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::StartTimerMethod (void)
{//{{{
    mIsTimerRunning = mIsTSTRCTrue; //add 0811
    if (mIsTimerRunning && (mCLKBUSPeriod == 0)) {
        mIsTimerRunning = false;
    }
    mCLKBUSPeriod = mCLKBUS_nPeriod[mTIC_SUBFunc->GetCLKBusSel()];
    mTimeCLKChangePoint = mTimeCLKStart[mTIC_SUBFunc->GetCLKBusSel()];
    if (mCLKBUSPeriod == 0) {
        mIsTimerRunning = false;
    }
    mFreeRunCnt->SetCounterPeriod(mCLKBUSPeriod);
    if (mChannelMode[0] == emPWM) { //PWM mode
        mLimitValue = mGRCValue[0];
    } else { //Other mode
        if (mIs_Upper_Limit) { //add 0818
            mLimitValue = (unsigned int)(mTIC_SUBFunc->GetCUCRC());
        }
    }
    mFreeRunCnt->SetCompareMatchValue(mLimitValue);
    if (mIsTimerRunning) {
        if (mIsOvf) {
            //-------Turn Flag at Restart time------------------
            mTIC_SUBFunc->TurnOVFFlag();
            //------------------------------------------------
            mIsOvf = false;
            re_printf("info","Recount after Overflow.\n");
            /////Add new for CUCRC overflow
            SetCounterValue(0);
        } else {
            mIsOVFPWM = false;
            SetCounterValue(1+(unsigned int)mTIC_SUBFunc->GetTCNTC()); //Set in 1st count to 1
        }
        if (mLimitValue == (unsigned int)(1+mTIC_SUBFunc->GetTCNTC())) {
            CheckCmpGRCOCRC();//Add 0805
            mHandleCmpOvfEvent.notify(SC_ZERO_TIME);
        } else {
            mFreeRunCnt->StartStop(mIsTimerRunning);
            for (unsigned int channel = 0; channel <= 3; channel++) { //Start the virtual counter
                if ((mChannelValue[channel] == 7) || mIsOCRCWaitStart[channel] ||
                    ((0 <= mChannelValue[channel]) && (mChannelValue[channel] <= 4)) || mIsWaitOCRCMatching[channel] ||
                    ((12 <= mChannelValue[channel]) && (mChannelValue[channel] <= 15))) {
                    if ((mChannelMode[channel] == emOneshot) && mIsWaitOCRCMatching[channel] && (mPre_OCRC[channel] == (1 + mTIC_SUBFunc->GetTCNTC()))) { // add 0819
                        if (mPre_OCRC[channel] == mGRCValue[channel]) {
                            mHandleCmpGRCEvent[channel].notify(SC_ZERO_TIME);
                        } else {
                            mHandleCmpOCRCEvent[channel].notify(SC_ZERO_TIME);
                        }
                    } else {
                        mRunCntOCRC[channel]->SetCounterPeriod(mCLKBUSPeriod);
                        mRunCntOCRC[channel]->StartStop(true);
                    }
                    mIsWaitOCRCMatching[channel] = false;
                    mIsOCRCWaitStart[channel] = false;
                } //Start counter of OCRC

                if (((mChannelMode[channel] == emComp) && (0 < mChannelValue[channel]) && (mChannelValue[channel] < 4)) || 
                    mIsGRCWaitOvf[channel] || mIsGRCWaitStart[channel] || mIsWaitGRCMatching[channel] || //add mIsWaitGRCMatching 0818
                    ((mChannelMode[channel] == emPWM) && (channel != 0) && (mChannelValue[channel] != 0))) {
                    if ((mChannelMode[channel] == emOneshot) && (mIsGRCWaitOvf[channel] || mIsGRCWaitStart[channel] || mIsWaitGRCMatching[channel]) && 
                        (mGRCValue[channel] == (1 + mTIC_SUBFunc->GetTCNTC())) && (mGRCValue[channel] != mPre_OCRC[channel])) { //add 0819
                        mHandleCmpGRCEvent[channel].notify(SC_ZERO_TIME);
                    } else {
                        mRunCntGRC[channel]->SetCounterPeriod(mCLKBUSPeriod); //close for tp03_06_05
                        mRunCntGRC[channel]->StartStop(true);
                    }
                    mIsGRCWaitStart[channel] = false;
                    mIsGRCWaitOvf[channel] = false;
                    mIsWaitGRCMatching[channel] = false;
                }// Start Counter of GRC
            }
        }
    } else { // Stop the counter
        mFreeRunCnt->StartStop(mIsTimerRunning);
        mRestartTimerEvent.cancel();
        mClearTimeEvent.cancel();
        mRestartCUCRCEvent.cancel();
        for (unsigned int channel = 0; channel < emChannel; channel++) {
            mRunCntGRC[channel]->StartStop(false);
            mRunCntOCRC[channel]->StartStop(false);
            mStartCntOCRCEvent[channel].cancel();
            mStartCntGRCEvent[channel].cancel();
        }
    }
    mTIC_SUBFunc->SetCounterStatus(mIsTimerRunning);
}//}}}

/*********************************
// Function     : RangeCmpOCRC
// Description  : Check Range Cmp of OCRC
**********************************/
void ATU5010_TIMERC_SUB::RangeCmpOCRC(const double range_value, const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        double temp = GetCounterValue();
        if ((mOCRCValue[channel] <= temp) && (temp <= (mOCRCValue[channel] + range_value))) {
            mHandleCmpOCRCEvent[channel].notify(CalculateClockEdge("PCLK", true, true, GetCurTime(),  mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
        }
    }
}//}}}

/*********************************
// Function     : RangeCmpGRC
// Description  : Check Range Cmp of GRC
**********************************/
void ATU5010_TIMERC_SUB::RangeCmpGRC(const double range_value, const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        double temp = GetCounterValue();
        if ((mGRCValue[channel] <= temp) && (temp <= (mGRCValue[channel] + range_value))) {
            mHandleCmpGRCEvent[channel].notify(CalculateClockEdge("PCLK", true, true, GetCurTime(),  mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
        }
    }
}//}}}

/*********************************
// Function     : RestartTimerMethod
// Description  : Used for Restart timer in case of setting clock_bus or set GRC0 in PWM
**********************************/
void ATU5010_TIMERC_SUB::RestartTimerMethod(void)
{//{{{
    mIsTimerRunning = mIsTSTRCTrue;
    if (mIsTimerRunning &&  (mCLKBUSPeriod == 0)) {
        mIsTimerRunning = false;
    }
    mCLKBUSPeriod = mCLKBUS_nPeriod[mTIC_SUBFunc->GetCLKBusSel()];
    mTimeCLKChangePoint = mTimeCLKStart[mTIC_SUBFunc->GetCLKBusSel()];
    SetCounterPeriod(mCLKBUSPeriod);//Set Counter clock
    mFreeRunCnt->StartStop(mIsTimerRunning);
    mTIC_SUBFunc->SetCounterStatus(true);
    if (mIsTimerRunning) {
        SetCounterValue(1 + (unsigned int) mTIC_SUBFunc->GetTCNTC());
        for (unsigned int channel = 0; channel <= 3; channel++) { //Start the virtual counter
            if ((mChannelValue[channel] == 7) || mIsOCRCWaitStart[channel] ||
                ((0 <= mChannelValue[channel]) && (mChannelValue[channel] <= 4)) || mIsWaitOCRCMatching[channel] ||
                ((12 <= mChannelValue[channel]) && (mChannelValue[channel] <= 15))) {
                mIsOCRCWaitStart[channel] = false;
                mIsWaitOCRCMatching[channel] = false;
                mRunCntOCRC[channel]->SetCounterPeriod(mCLKBUSPeriod);
                mRunCntOCRC[channel]->StartStop(mIsTimerRunning);
            }
            if (((mChannelMode[channel] == emComp) && (0 < mChannelValue[channel]) && (mChannelValue[channel] < 4)) || 
                mIsGRCWaitOvf[channel] || mIsGRCWaitStart[channel] ||
                ((mChannelMode[channel] == emPWM) && (channel != 0) && (mChannelValue[channel] != 0))) {
                mIsGRCWaitStart[channel] = false;
                mIsGRCWaitOvf[channel] = false;
                mRunCntGRC[channel]->SetCounterPeriod(mCLKBUSPeriod); //close for tp03_06_05
                mRunCntGRC[channel]->StartStop(mIsTimerRunning);
            }
        }
    }
}//}}}

/*********************************
// Function     : StartCntOCRCMethod
// Description  : Used to start Cnt OCRC in "1 shot pulse"
// NOte         : This method is called to load the value of FreeRunCnt when Cnt is running
**********************************/
void ATU5010_TIMERC_SUB::StartCntOCRCMethod(const unsigned int channel)
{//{{{
    mIsTimerRunning = mIsTSTRCTrue;
    mCLKBUSPeriod = mCLKBUS_nPeriod[mTIC_SUBFunc->GetCLKBusSel()];
    mTimeCLKChangePoint = mTimeCLKStart[mTIC_SUBFunc->GetCLKBusSel()];
    SetCounterPeriod(mCLKBUSPeriod);//Set Counter clock
    mRunCntOCRC[channel]->SetCounterInitialValue(1+(unsigned int) mTIC_SUBFunc->GetTCNTC()); // close 0811
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB") && (mTIMERC->mPCLKPeriod != 0)) {
        if (mChannelMode[channel] == emOneshot) {
            mIsOneShotRunning[channel] = true;
            mIsWaitOCRCMatching[channel] = true; //add 0811
            mIsOCRCWaitStart[channel] = false;
            if (mOCRCValue[channel] == (1 + (unsigned int)mTIC_SUBFunc->GetTCNTC())) {//change 0809
                mHandleCmpOCRCEvent[channel].notify(SC_ZERO_TIME); //add 0818
            } else {
                mRunCntOCRC[channel]->SetCompareMatchValue(mOCRCValue[channel]); //add 0811
                mRunCntOCRC[channel]->StartStop(true);
            }
            if (mGRCValue[channel] > mOCRCValue[channel]) {
                if (mOCRCValue[channel] >=  (1+mTIC_SUBFunc->GetTCNTC())) { //change 0818 change >= 
                mRunCntGRC[channel]->SetCounterInitialValue(1+(unsigned int) mTIC_SUBFunc->GetTCNTC());
                mRunCntGRC[channel]->SetCompareMatchValue(mGRCValue[channel]);
                mRunCntGRC[channel]->StartStop(true);
                mIsWaitGRCMatching[channel] = true; //add 0818
                }
            }
        } else {
                mRunCntOCRC[channel]->StartStop(true);

        }
    }
}//}}}

/*********************************
// Function     : StartCntGRCMethod
// Description  : Used to start Cnt GRC in "1 shot pulse"
// NOte         : This method is called to load the value of FreeRunCnt when Cnt is running
**********************************/
void ATU5010_TIMERC_SUB::StartCntGRCMethod(const unsigned int channel)
{//{{{
    mIsTimerRunning = mIsTSTRCTrue;
    mIsWaitGRCMatching[channel] = true; //add 0818
    mCLKBUSPeriod = mCLKBUS_nPeriod[mTIC_SUBFunc->GetCLKBusSel()];
    mTimeCLKChangePoint = mTimeCLKStart[mTIC_SUBFunc->GetCLKBusSel()];
    SetCounterPeriod(mCLKBUSPeriod);//Set Counter clock
    mRunCntGRC[channel]->SetCounterPeriod(mCLKBUSPeriod);
    mRunCntGRC[channel]->SetCounterInitialValue(1+(unsigned int) mTIC_SUBFunc->GetTCNTC());
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB") && (mTIMERC->mPCLKPeriod != 0)) {
        if (mChannelMode[channel] == emOneshot) {
            mIsCntGRCWorking[channel] = true;
            if (mGRCValue[channel] == (1 + (unsigned int)mTIC_SUBFunc->GetTCNTC())) {
                mHandleCmpGRCEvent[channel].notify(SC_ZERO_TIME);
            } else {
                mRunCntGRC[channel]->StartStop(true);
            }
        } else {
                mRunCntGRC[channel]->StartStop(true);
        }
    }
}//}}}


/*********************************
// Function     : SetCounterValue
// Description  : Set the initial counter value in break state
**********************************/
void ATU5010_TIMERC_SUB::SetCounterValue(const unsigned int count)
{//{{{
    mFreeRunCnt->SetCounterInitialValue(count);
    for (unsigned int channel = 0; channel < 4; channel++) {
        mRunCntGRC[channel]->SetCounterInitialValue(count);
        mRunCntOCRC[channel]->SetCounterInitialValue(count);
    }
}//}}}

/*********************************
// Function     : UpdatingCntClock
// Description  : The Cnt clock is updated while counting
**********************************/
void ATU5010_TIMERC_SUB::UpdatingCntClock(void)
{//{{{
    mFreeRunCnt->StartStop(false);
    for (unsigned int i = 0; i < emChannel; i++) {
        mRunCntGRC[i]->StartStop(false);
        mRunCntOCRC[i]->StartStop(false);
    }
    mTIC_SUBFunc->SetTCNTC(GetCounterValue()); //0812
    mIsTimerRunning = false;
    mPre_CntClkPeriod = mCLKBUSPeriod;
    UpdateInRunning("clock");
}//}}}

/*********************************
// Function     : UpdateInRunning
// Description  : In Counting state, update clock, TCNTC ..
**********************************/
void ATU5010_TIMERC_SUB::UpdateInRunning(const std::string name)
{//{{{
    if (name == "TCNTC") {
        mTIC_SUBFunc->SetCounterStatus(false);
        mUpdatedTCNTC = mTIC_SUBFunc->GetTCNTC(); 
        SetCounterValue(1 + mUpdatedTCNTC);
        re_printf("info", "Counter is counted with updated value.\n");
        mIsTimerRunning = false; //edit 0814 true -> false
        mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK", true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit); // add 0818
    } else if (name == "clock") {
            mCLKBUSPeriod = mCLKBUS_nPeriod[mTIC_SUBFunc->GetCLKBusSel()];
            SetCounterPeriod(mCLKBUSPeriod);
            mTimeCLKChangePoint = mTIMERC->mATU5010->mClkBusStartTime[mTIC_SUBFunc->GetCLKBusSel()];
            //Restart counter with updated setting
        if (mCLKBUSPeriod != 0) {
            re_printf("info", "Counter is counted with updated clock.\n");
            mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK", true,false, GetCurTime(), mTimeCLKChangePoint), mTIMERC->mATU5010->mTimeResolutionUnit);//edited 0813 true -> false
        }
    }
}//}}}

/*********************************
// Function     : SetCmpOCRCValue
// Description  : Handle Compare match OCRC
**********************************/
void ATU5010_TIMERC_SUB::SetCmpOCRCValue(const unsigned int channel, const unsigned int value)
{//{{{
    if (mIsTSTRCTrue) {
        if (((0 <= mChannelValue[channel]) && (mChannelValue[channel] <= 4)) || (mChannelValue[channel] == 7) ||
            (mChannelValue[channel] == 10) || (mChannelValue[channel] == 11) ||
            ((mChannelValue[channel] <=12) && (mChannelValue[channel] <= 15))) {
            if ((value == (1+(unsigned int)mFreeRunCnt->GetCounterValue())) && mIsTSTRCTrue && (mChannelMode[channel] != emOneshot)) { //add if 0809
                mHandleCmpOCRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK", true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
            } else {
                mRunCntOCRC[channel]->SetCompareMatchValue(value);//OCRC_value from call-back OCRC
            }
        }
    }
}//}}}

/*********************************
// Function     : SetCmpGRCValue
// Description  : Handle Compare match GRC
**********************************/
void ATU5010_TIMERC_SUB::SetCmpGRCValue(const unsigned int channel, const unsigned int value)
{//{{{
    if (mIsTSTRCTrue) {
        if (((0 < mChannelValue[channel]) && (mChannelValue[channel] <= 3)) ||
            ((mChannelMode[channel] == emPWM) && (channel != 0)) ||
            ((8 <= mChannelValue[channel]) && (mChannelValue[channel] <= 11))) {
            if ((value == (1+(unsigned int)mTIC_SUBFunc->GetTCNTC())) && mIsTSTRCTrue && (mChannelMode[channel] != emOneshot) && (mChannelMode[channel] != emPWM)) { //add if 0809
                mHandleCmpGRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK", true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
            } else {
                mRunCntGRC[channel]->SetCompareMatchValue(mGRCValue[channel]);
            }
        }
    }
}//}}}

/*********************************
// Function     : HandleCntClkInputMethod
// Description  : Handle Count clock
**********************************/
void ATU5010_TIMERC_SUB::HandleCntClkInputMethod(void)
{//{{{
    if (!IsModelResetActive()) {
        //Handle the change of clkbus_n clock (n:0->5)
        mCLKBUS_nPeriod[0] = clock_bus0.read();
        mCLKBUS_nPeriod[1] = clock_bus1.read();
        mCLKBUS_nPeriod[2] = clock_bus2.read();
        mCLKBUS_nPeriod[3] = clock_bus3.read();
        mCLKBUS_nPeriod[4] = clock_bus4.read();
        mCLKBUS_nPeriod[5] = clock_bus5.read();
        for (unsigned int i = 0; i < 4; i++) {
            if (mCurCLKCntPeriod[i] != mCLKBUS_nPeriod[i]) {
                mTimeCLKStart[i] = GetCurTime();
            }
        }
        if (mIsTSTRCTrue && (mCLKBUSPeriod != mCLKBUS_nPeriod[mTIC_SUBFunc->GetCLKBusSel()])) {
            mPre_CntClkPeriod = mCLKBUSPeriod;
            mCLKBUSPeriod = mCLKBUS_nPeriod[mTIC_SUBFunc->GetCLKBusSel()];
            mTimeCLKChangePoint = mTIMERC->mATU5010->mClkBusStartTime[mTIC_SUBFunc->GetCLKBusSel()];
            mIsTimerRunning = false;
            mFreeRunCnt->StartStop(false);
            for (unsigned int i = 0; i < emChannel; i++) {
                mRunCntGRC[i]->StartStop(false);
                mRunCntOCRC[i]->StartStop(false);
            }
            if (mPre_CntClkPeriod != 0) {
                mTIC_SUBFunc->SetTCNTC(GetCounterValue());
            }
            mTimeCLKChangePoint = mTimeCLKStart[mTIC_SUBFunc->GetCLKBusSel()];
            if (mPre_CntClkPeriod == 0) {
                UpdateInRunning("clock");
            } else {
                mRestartTimerEvent.notify(mCLKBUSPeriod,mTIMERC->mATU5010->mTimeResolutionUnit);
            }
            re_printf("warning","TCNTC clock is changed during Timer C is enabled\n");
        }
        //Handle the change of nc_clk_n clock (n:1 or 2)
        mCLKNOISE_nPeriod[0] = nc_clk.read();
        mCLKNOISE_nPeriod[1] = clock_bus5.read();
        for (unsigned int i = 0; i < emChannel; i++) {
            SetCLKNoisePeriod(i);
        }
    }
}//}}}

/*********************************
// Function     : ClearTimeMethod
// Description  : Clear counter value as clock for CUCRC with PCLK
**********************************/
void ATU5010_TIMERC_SUB::ClearTimeMethod(void)
{//{{{
    ////Fix for CUCRC overflow
    unsigned int ratio = mTIMERC->GetRatioValue(mTIC_SUBFunc->GetCLKBusSel());
    if ((mCLKBUSPeriod != 0) && mIsTSTRCTrue) {
        if (ratio == 0) {
            mTIC_SUBFunc->SetTCNTC(1);
            SetCounterValue(1);
        } else {
            mTIC_SUBFunc->SetTCNTC(0);
            SetCounterValue(0);
        }
        mRestartCUCRCEvent.notify(CalculateClockEdge("TCNTCCLK",true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit); //edit 0811 for tp03_01_005
    }
}//}}}

/*********************************
// Function     : RestartCUCRCMethod
// Description  : restart the counter with CUCRC and PCLK clear
**********************************/
void ATU5010_TIMERC_SUB::RestartCUCRCMethod(void)
{//{{{
    mIsTimerRunning = true;
    unsigned int ratio = mTIMERC->GetRatioValue(mTIC_SUBFunc->GetCLKBusSel());
    SetCounterValue(1+(unsigned int)mTIC_SUBFunc->GetTCNTC());
    mLimitValue = mTIC_SUBFunc->GetCUCRC(); //add 0818
    mFreeRunCnt->SetCompareMatchValue(mLimitValue); //add 0816
    mFreeRunCnt->StartStop(true);
    for (unsigned int channel = 0; channel <= 3; channel++) { //Start the virtual counter
        if ((mChannelValue[channel] == 7) || mIsOCRCWaitStart[channel] ||
            ((0 <= mChannelValue[channel]) && (mChannelValue[channel] <= 4)) || mIsWaitOCRCMatching[channel] ||
            ((12 <= mChannelValue[channel]) && (mChannelValue[channel] <= 15))) {
            if ((mChannelMode[channel] == emOneshot) && mIsWaitOCRCMatching[channel] && (mPre_OCRC[channel] == (1 + mTIC_SUBFunc->GetTCNTC()))) { // add 0819
                if (mPre_OCRC[channel] == mGRCValue[channel]) {
                    mHandleCmpGRCEvent[channel].notify(SC_ZERO_TIME);
                } else {
                    mHandleCmpOCRCEvent[channel].notify(SC_ZERO_TIME);
                }
            } else {
                mRunCntOCRC[channel]->SetCounterPeriod(mCLKBUSPeriod);
                mRunCntOCRC[channel]->StartStop(true);
            }
            mIsOCRCWaitStart[channel] = false;
            mIsWaitOCRCMatching[channel] = false; //add 0820
        } //Start counter of OCRC

        if (((mChannelMode[channel] == emComp) && (0 < mChannelValue[channel]) && (mChannelValue[channel] < 4)) || 
            mIsGRCWaitOvf[channel] || mIsGRCWaitStart[channel] || mIsWaitGRCMatching[channel] || //add mIsWaitGRCMatching 0818
            ((mChannelMode[channel] == emPWM) && (channel != 0) && (mChannelValue[channel] != 0))) {
            if ((mChannelMode[channel] == emOneshot) && (mIsGRCWaitOvf[channel] || mIsGRCWaitStart[channel] || mIsWaitGRCMatching[channel]) && 
                (mGRCValue[channel] == (1 + mTIC_SUBFunc->GetTCNTC())) && (mGRCValue[channel] != mPre_OCRC[channel])) { //add 0819
                mHandleCmpGRCEvent[channel].notify(SC_ZERO_TIME);
            } else {
                mRunCntGRC[channel]->SetCounterPeriod(mCLKBUSPeriod); //close for tp03_06_05
                mRunCntGRC[channel]->StartStop(true);
            }
            mIsGRCWaitStart[channel] = false;
            mIsGRCWaitOvf[channel] = false;
            mIsWaitGRCMatching[channel] = false;
        }// Start Counter of GRC
    }
    mTIC_SUBFunc->SetCounterStatus(true);
}//}}}

/*********************************
// Function     : HandleCmpOvfMethod
// Description  : Handle Compare Event
**********************************/
void ATU5010_TIMERC_SUB::HandleCmpOvfMethod(void)
{//{{{
    if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB") && !IsModelResetActive() && (mCLKBUSPeriod != 0)) { //Check Clock & reset
        mIsTimerRunning = false;
        //Stop temporaly counter and off status flag in Func
        mTIC_SUBFunc->SetCounterStatus(false);
        mFreeRunCnt->StartStop(false); //Stop The counter
        for (unsigned int channel = 0; channel < emChannel; channel++) {
            mRunCntGRC[channel]->StartStop(false);
            mRunCntOCRC[channel]->StartStop(false);
        }
        //Check Mode to Analyze Ovf
        if (mChannelMode[0] == emPWM) {
          mIsOVFPWM = true;//Overflow of PWM
            mIsOvf = true;
            mIsTimerRunning = false; //add 0818 for tp03_05_22
            mTIC_SUBFunc->SetTCNTC(mGRCValue[0]);
            //add 0815
            mTIC_SUBFunc->SetCounterStatus(false);
            //
            mHandlePWMEvent.notify(mTIMERC->mPCLKPeriod,mTIMERC->mATU5010->mTimeResolutionUnit);
        } else {
            bool temp = mTIC_SUBFunc->TimingClearCounter();
            mIsTimerRunning = false; //edit 0814 true -> false
            if (mIs_Upper_Limit) {
                if (mFreeRunCnt->GetBitOVF()) {
                    mTIC_SUBFunc->SetTCNTC(0xFFFFFFFF);
                } else {
                    mTIC_SUBFunc->SetTCNTC(mLimitValue);
                }
                mFreeRunCnt->ClearBitOVF();
                //Interrupt after 1 PCLK---------------------------------------------
                mInterruptWithPCLKEvent.notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]) + mTIMERC->mPCLKPeriod, mTIMERC->mATU5010->mTimeResolutionUnit);
                //-------------------------------------------------------------------
                if (!temp) {//Clear with PCLK
                    mClearTimeEvent.notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]) + mTIMERC->mPCLKPeriod, mTIMERC->mATU5010->mTimeResolutionUnit);
                } else { //Clear with CLKBUS
                        mIsOvf = true;
                        mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK",true,false, GetCurTime(), mTimeCLKChangePoint) + mCLKBUSPeriod ,mTIMERC->mATU5010->mTimeResolutionUnit);
                }
            } else { //----OVERFLOW AT 0xFFFFFFFF ------------------------
                if (mTIC_SUBFunc->IsInterruptOVF()) {
                    mRisingInterruptOVFEvent.notify(CalculateClockEdge("TCNTCCLK",true,false, GetCurTime(), mTimeCLKChangePoint) + mCLKBUSPeriod ,mTIMERC->mATU5010->mTimeResolutionUnit);// syn with CLKBUS;
                }
                //-----------------------------------
                mIsOvf = true;
                mTIC_SUBFunc->SetTCNTC((unsigned int)(0xFFFFFFFF));
                mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK",true,false, GetCurTime(), mTimeCLKChangePoint) + mCLKBUSPeriod ,mTIMERC->mATU5010->mTimeResolutionUnit);
            }
        }
    }
    mFreeRunCnt->ClearBitCMF();
}//}}}

/*********************************
// Function     : HandleCmpGRCMethod
// Description  : Handle Compare Event
**********************************/
void ATU5010_TIMERC_SUB::HandleCmpGRCMethod(const unsigned int channel)
{//{{{
    if ((mGRCMatchTime[channel] == sc_time_stamp().to_double()) && (mIsGRC1stMatch[channel])) {
        return;
    } else {
        mGRCMatchTime[channel] = sc_time_stamp().to_double();
        mIsGRC1stMatch[channel] = true;
        if (mChannelMode[channel] == emComp) {
            if (mRunCntGRC[channel]->GetBitOVF() && (mGRCValue[channel] == 0)) {
                mRunCntGRC[channel]->StartStop(false);
                    mHandleCmpGRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK", true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
            } else {
                if (mChannelValue[channel] == 1) {
                    mTIOC[channel] = false;
                } else if (mChannelValue[channel] == 2) {
                    mTIOC[channel] = true;
                } else if (mChannelValue[channel] == 3) {
                    mTIOC[channel] = (bool)(!mTIOC[channel]);
                }
                mWriteTIOCOutputEvent[channel].notify((CalculateClockEdge("PCLK", true, false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]) + mTIMERC->mPCLKPeriod),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit);//syn with clock
                mHandleFlagAndInterruptGRCEvent[channel].notify((CalculateClockEdge("PCLK", true, false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]) + mTIMERC->mPCLKPeriod),
                                                                    mTIMERC->mATU5010->mTimeResolutionUnit);//syn with clock
            }
            mRunCntGRC[channel]->ClearBitOVF();
        } else if (mChannelMode[channel] == emOneshot) {
            mIsOneShotRunning[channel] = false;
            mIsGRCWaitOvf[channel] = false;
            mIsWaitGRCMatching[channel] = false;
            switch (mChannelValue[channel]) {
                case 8:
                    mTIOC[channel] = 0; break;
                case 9:
                    mTIOC[channel] = 1; break;
                case 10:
                    mTIOC[channel] = 0; 
                    mHandleFlagAndInterruptGRCEvent[channel].notify(mTIMERC->mPCLKPeriod,mTIMERC->mATU5010->mTimeResolutionUnit);
                    break;
                case 11:
                    mTIOC[channel] = 1; 
                    mHandleFlagAndInterruptGRCEvent[channel].notify(mTIMERC->mPCLKPeriod,mTIMERC->mATU5010->mTimeResolutionUnit);
                    break;
                default: break;
            }
            mRunCntGRC[channel]->StartStop(false);
            mWriteTIOCOutputEvent[channel].notify(mTIMERC->mPCLKPeriod,mTIMERC->mATU5010->mTimeResolutionUnit);
        } else if (mChannelMode[channel] == emPWM) {
            if (mIsOVFPWM) {
            // -- New ---------- PWM
            //The signal duty cycle is output
                if ((mChannelValue[0] == 0) || (mChannelValue[0] == 8)) {
                    mTIOC[channel] = mTIOC[channel];
                } else if ((mChannelValue[0] == 1) || (mChannelValue[0] == 9)) {
                    mTIOC[channel] = false;
                } else if ((mChannelValue[0] == 2) || (mChannelValue[0] == 10)) {
                    mTIOC[channel] = true;
                } else {
                    mTIOC[channel] = (bool)(!mTIOC[channel]);
                }
                mWriteTIOCOutputEvent[channel].notify(SC_ZERO_TIME);//syn with clock
                //The flag of GRC0 is called in HandlePWM Method
            } else {
                mRunCntGRC[channel]->StartStop(false); //add 0810 for tp03_05_14
                    if ((mChannelValue[channel] == 0) || (mChannelValue[channel] == 8)) {
                        mTIOC[channel] = mTIOC[channel];
                    } else if ((mChannelValue[channel] == 1) || (mChannelValue[channel] == 9)){
                        mTIOC[channel] = false;
                    } else if ((mChannelValue[channel] == 2) || (mChannelValue[channel] == 10)) {
                        mTIOC[channel] = true;
                    } else {
                        mTIOC[channel] = (bool)(!mTIOC[channel]);
                    }
                    mWriteTIOCOutputEvent[channel].notify(CalculateClockEdge("PCLK", true, true, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), 
                                                            mTIMERC->mATU5010->mTimeResolutionUnit);//syn with clock
                    if ((mChannelValue[channel] != 8) && (mChannelValue[channel] != 0)) {
                        mHandleFlagAndInterruptGRCEvent[channel].notify(CalculateClockEdge("PCLK", true, true, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), 
                                                            mTIMERC->mATU5010->mTimeResolutionUnit);//syn with clock
                    }
            }
        }
        mRunCntGRC[channel]->ClearBitCMF();
    }
}//}}}

/*********************************
// Function     : HandleCmpOCRCMethod
// Description  : Handle Compare Event
**********************************/
void ATU5010_TIMERC_SUB::HandleCmpOCRCMethod(const unsigned int channel)
{//{{{
    if ((mOCRCMatchTime[channel] == sc_time_stamp().to_double()) && (mIsOCRC1stMatch[channel])) {
        return;
    } else {
        mOCRCMatchTime[channel] = sc_time_stamp().to_double();
        mIsOCRC1stMatch[channel] = true;
        if (((0 <= mChannelValue[channel]) && (mChannelValue[channel] <= 4)) || 
                (mChannelValue[channel] == 7) || ((11 < mChannelValue[channel]) && (mChannelValue[channel] <= 15))) {
            if (mRunCntOCRC[channel]->GetBitOVF() && (mOCRCValue[channel] == 0)) {
                mRunCntOCRC[channel]->StartStop(false);
                    mHandleCmpOCRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK", true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
            } else {
                mHandleFlagAndInterruptOCRCEvent[channel].notify((CalculateClockEdge("PCLK", true, false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]) + mTIMERC->mPCLKPeriod),
                                                                    mTIMERC->mATU5010->mTimeResolutionUnit);
            }
            mRunCntOCRC[channel]->ClearBitOVF();
        } else if (mChannelMode[channel] == emOneshot) {
            mTIC_SUBFunc->SetTCNTC(GetCounterValue());
            unsigned int temp = mChannelValue[channel];
            mIsWaitOCRCMatching[channel] = false;
            switch (temp) {
                case 10:
                    mTIOC[channel] = 1;
                    if (mGRCValue[channel] != mOCRCValue[channel]) {
                        mWriteTIOCOutputEvent[channel].notify(mTIMERC->mPCLKPeriod, mTIMERC->mATU5010->mTimeResolutionUnit);//syn with clock
                        mHandleFlagAndInterruptOCRCEvent[channel].notify(mTIMERC->mPCLKPeriod,mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                    break;
                case 11:
                    mTIOC[channel] = 0;
                    if (mGRCValue[channel] != mOCRCValue[channel]) {
                        mHandleFlagAndInterruptOCRCEvent[channel].notify(mTIMERC->mPCLKPeriod,mTIMERC->mATU5010->mTimeResolutionUnit);
                        mWriteTIOCOutputEvent[channel].notify(mTIMERC->mPCLKPeriod, mTIMERC->mATU5010->mTimeResolutionUnit);//syn with clock
                    }
                    break;
                default: break;
            }
            mRunCntOCRC[channel]->StartStop(false); //stop counter OCRC (1shot)
            if (mOCRCValue[channel] > mGRCValue[channel]) {
                SetCmpGRCValue(channel,mGRCValue[channel]);
                mIsGRCWaitOvf[channel] = true;
            } else if (mOCRCValue[channel] == mGRCValue[channel]) {
                mHandleFlagAndInterruptGRCEvent[channel].notify(mTIMERC->mPCLKPeriod, mTIMERC->mATU5010->mTimeResolutionUnit);//syn with clock
            }
        }
        mRunCntOCRC[channel]->ClearBitCMF();
    }
}//}}}

/*********************************
// Function     : InterruptWithPCLKMethod
// Description  : Control interrupt and flag when cmp match with CUCRC
**********************************/
void ATU5010_TIMERC_SUB::InterruptWithPCLKMethod(void)
{//{{{
    if (mTIC_SUBFunc->IsInterruptOVF()) {
        mTIC_SUBFunc->TurnOVFFlag();
        mRisingInterruptOVFEvent.notify(SC_ZERO_TIME);// syn with PCLK;
    }
}//}}}

/*********************************
// Function     : HandleFlagAndInterruptGRCMethod
// Description  : Handle Turn Flag and Interrupt of GRC
**********************************/
void ATU5010_TIMERC_SUB::HandleFlagAndInterruptGRCMethod(const unsigned int channel)
{//{{{
    bool temp = false;
    if ((mChannelValue[channel] == 0) || (mChannelValue[channel] == 8) || (mChannelValue[channel] == 9)) {
        bool temp = true;
    }
    if (!temp) {
        mTIC_SUBFunc->TurnGRCFlag(channel,1);
    }
    if (mTIC_SUBFunc->IsInterruptGRC(channel) && !temp) {
        mRisingInterruptGRCEvent[channel].notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : HandleFlagAndInterruptOCRCMethod
// Description  : Handle Turn Flag and Interrupt of OCRC
**********************************/
void ATU5010_TIMERC_SUB::HandleFlagAndInterruptOCRCMethod(const unsigned int channel)
{//{{{
    bool temp = false;
    if (!temp) {
        mTIC_SUBFunc->TurnOCRCFlag(channel,1);
    }
    if (mTIC_SUBFunc->IsInterruptOCRC(channel) && !temp)  {
        mRisingInterruptOCRCEvent[channel].notify(SC_ZERO_TIME);
    }
}//}}}

/***********************************
Function        : GetCurTime
Description     : Get the current time
***********************************/
double ATU5010_TIMERC_SUB::GetCurTime(void)
{//{{{
    return sc_time_stamp().to_double();
}//}}}

/*********************************
// Function     : WriteTIOCOutputMethod
// Description  : Write value to TIOC port
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::WriteTIOCOutputMethod(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) { //Check reset/clock
        switch (channel) {
            case 0:
                TIOC0.write(mTIOC[channel]);
                if ((mSub_id == 0) || (mSub_id == 1) || (mSub_id == 2) || (mSub_id == 3)) {
                    TOC0.write(mTIOC[channel]);
                }
                break;
            case 1:
                TIOC1.write(mTIOC[channel]);
                if ((mSub_id == 0) || (mSub_id == 1) || (mSub_id == 2) || (mSub_id == 3)) {
                    TOC1.write(mTIOC[channel]);
                }
                break;
            case 2:
                TIOC2.write(mTIOC[channel]);
                if ((mSub_id == 0) || (mSub_id == 1) || (mSub_id == 2)) {
                    TOC2.write(mTIOC[channel]);
                }
               break;
            case 3:
                TIOC3.write(mTIOC[channel]);
                if ((mSub_id == 0) || (mSub_id == 1) || (mSub_id == 2)) {
                    TOC3.write(mTIOC[channel]);
                }
               break;
            default: break;
        }
    }
}//}}}

/*********************************
// Function     : RisingInterruptGRCMethod
// Description  : Rising the interrupt
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::RisingInterruptGRCMethod(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) { //Check reset/clock
        INT_GRC[channel]->write(true);
        mFallingInterruptGRCEvent[channel].notify(mTIMERC->mPCLKPeriod, mTIMERC->mATU5010->mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : FallingInterruptGRCMethod
// Description  : Falling the interrupt
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::FallingInterruptGRCMethod(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) { //Check reset/clock
        INT_GRC[channel]->write(false);
    }
}//}}}

/*********************************
// Function     : RisingInterruptOCRCMethod
// Description  : Rising the interrupt
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::RisingInterruptOCRCMethod(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) { //Check reset/clock
        INT_OCRC[channel]->write(true);
        mFallingInterruptOCRCEvent[channel].notify(mTIMERC->mPCLKPeriod,mTIMERC->mATU5010->mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : FallingInterruptOCRCMethod
// Description  : Falling the interrupt
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::FallingInterruptOCRCMethod(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) { //Check reset/clock
        INT_OCRC[channel]->write(false);
    }
}//}}}

/*********************************
// Function     : RisingInterruptOVFMethod
// Description  : Rising the overflow interrupt
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::RisingInterruptOVFMethod(void)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")){
        INT_OVI.write(true);
        mFallingInterruptOVFEvent.notify(mTIMERC->mPCLKPeriod, mTIMERC->mATU5010->mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : FallingInterruptOVFMethod
// Description  : Falling the overflow interrupt
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::FallingInterruptOVFMethod(void)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")){
        INT_OVI.write(false);
    }
}//}}}

/*********************************
// Function     : HandleTIOC0Method
// Description  : Handle TIOC0 input
**********************************/
void ATU5010_TIMERC_SUB::HandleTIOC0Method(void)
{//{{{
    if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        SetCLKNoisePeriod(0);
        if (mNoiseCanceler[0]->IsZeroNoiseCLk()) {
            re_printf("warning","Noise cancellation cannot start due to zero clock.\n");
        }
        mCurTIC[0] = TIOC0.read();
        if ((mTIMERC->mTimerC_Func->IsNoiseCancelEn(mSub_id,0)) && (!mNoiseCanceler[0]->IsZeroNoiseCLk())) {
            mNoiseCanceler[0]->NotifyNoiseCancellation(true, true, mCurTIC[0]);
        } else {
            mHandleInputChannel0Event.notify(SC_ZERO_TIME);
            mNoiseCanceler[0]->NotifyNoiseCancellation(false, true, mCurTIC[0]);
        }
    }
}//}}}

/*********************************
// Function     : HandleTIOC1Method
// Description  : Handle TIOC1 input
**********************************/
void ATU5010_TIMERC_SUB::HandleTIOC1Method(void)
{//{{{
    if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        SetCLKNoisePeriod(1);
        if (mNoiseCanceler[1]->IsZeroNoiseCLk()) {
            re_printf("warning","Noise cancellation cannot start due to zero clock.\n");
        }
        mCurTIC[1] = TIOC1.read();
        if ((mTIMERC->mTimerC_Func->IsNoiseCancelEn(mSub_id,1)) && (!mNoiseCanceler[1]->IsZeroNoiseCLk())) {
            mNoiseCanceler[1]->NotifyNoiseCancellation(true, true, mCurTIC[1]);
        } else {
            mHandleInputChannel1Event.notify(SC_ZERO_TIME);
            mNoiseCanceler[1]->NotifyNoiseCancellation(false, true, mCurTIC[1]);
        }
    }
}//}}}

/*********************************
// Function     : HandleTIOC2Method
// Description  : Handle TIOC2 input
**********************************/
void ATU5010_TIMERC_SUB::HandleTIOC2Method(void)
{//{{{
    if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        SetCLKNoisePeriod(2);
        if (mNoiseCanceler[2]->IsZeroNoiseCLk()) {
            re_printf("warning","Noise cancellation cannot start due to zero clock.\n");
        }
        mCurTIC[2] = TIOC2.read();
        if ((mTIMERC->mTimerC_Func->IsNoiseCancelEn(mSub_id,2)) && (!mNoiseCanceler[2]->IsZeroNoiseCLk())) {
            mNoiseCanceler[2]->NotifyNoiseCancellation(true, true, mCurTIC[2]);
        } else {
            mHandleInputChannel2Event.notify(SC_ZERO_TIME);
            mNoiseCanceler[2]->NotifyNoiseCancellation(false, true, mCurTIC[2]);
        }
    }
}//}}}

/*********************************
// Function     : HandleTIOC3Method
// Description  : Handle TIOC3 input
**********************************/
void ATU5010_TIMERC_SUB::HandleTIOC3Method(void)
{//{{{
    if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        SetCLKNoisePeriod(3);
        if (mNoiseCanceler[3]->IsZeroNoiseCLk()) {
            re_printf("warning","Noise cancellation cannot start due to zero clock.\n");
        }
        mCurTIC[3] = TIOC3.read();
        if ((mTIMERC->mTimerC_Func->IsNoiseCancelEn(mSub_id,3)) && (!mNoiseCanceler[3]->IsZeroNoiseCLk())) {
            mNoiseCanceler[3]->NotifyNoiseCancellation(true, true, mCurTIC[3]);
        } else {
            mHandleInputChannel3Event.notify(SC_ZERO_TIME);
            mNoiseCanceler[3]->NotifyNoiseCancellation(false, true, mCurTIC[3]);
        }
    }
}//}}}

/*********************************
// Function     : HandleInputChannel0Method
// Description  : Handle TIOC0 input
**********************************/
void ATU5010_TIMERC_SUB::HandleInputChannel0Method(void)
{//{{{
    if (!IsModelResetActive()) {//Check reset
        if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
            if ((mChannelMode[0] == emCapture)) {
                mTIOC0  = mCurTIC[0];
                mIA1    = i_event_1_0.read();
                mIA2A   = i_event_2A.read();
                mIA2B   = i_event_2B.read();
                unsigned int value = mChannelValue[0];
                if ((value == 4) || (value == 12)) {
                    //disable input capture
                    re_printf ("info","Disable Input Catpure.\n");
                } else if (value == 5) {
                    if (mTIOC0 == 1) {
                        mCaptureGRCEvent[0].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    } else {
                        mCaptureOCRCEvent[0].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    }
                } else if (value == 6) {
                    if (mTIOC0 == 1) {
                        mCaptureOCRCEvent[0].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    } else {
                        mCaptureGRCEvent[0].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    }
                } else if (value == 7) {
                    mCaptureGRCEvent[0].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                } else if (value == 13) {
                    if (mIA1 == 1) {
                        mCaptureGRCEvent[0].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                } else if (value == 14) {
                    if (mIA2A == 1) {
                        mCaptureGRCEvent[0].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                } else if (value == 15) {
                    if (mIA2B == 1) {
                        mCaptureGRCEvent[0].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : HandleInputChannel1Method
// Description  : Handle TIOC1 input
**********************************/
void ATU5010_TIMERC_SUB::HandleInputChannel1Method(void)
{//{{{
    if (!IsModelResetActive()) {//Check reset
        if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
            if (mChannelMode[1] == emCapture) {
                mTIOC1  = mCurTIC[1];
                mIA1    = i_event_1_0.read();
                mIA2A   = i_event_2A.read();
                mIA2B   = i_event_2B.read();
                unsigned int value = mChannelValue[1];
                if ((value == 4) || (value == 12)) {
                    //disable input capture
                    re_printf("info","Disable Input Catpure.\n");
                } else if (value == 5) {
                    if (mTIOC1 ==1) {
                        mCaptureGRCEvent[1].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    } else {
                        mCaptureOCRCEvent[1].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    }
                } else if (value == 6) {
                    if (mTIOC1 ==1) {
                        mCaptureOCRCEvent[1].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    } else {
                        mCaptureGRCEvent[1].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    }
                } else if (value == 7) {
                    mCaptureGRCEvent[1].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit);     //after 2 cycle
                } else if (value == 13) {
                    if (mIA1 == 1) {
                        mCaptureGRCEvent[1].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                } else if (value == 14) {
                    if (mIA2A == 1) {
                        mCaptureGRCEvent[1].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                } else if (value == 15) {
                    if (mIA2B == 1) {
                        mCaptureGRCEvent[1].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                    mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : HandleInputChannel2Method
// Description  : Handle TIOC2 input
**********************************/
void ATU5010_TIMERC_SUB::HandleInputChannel2Method(void)
{//{{{
    if (!IsModelResetActive()) {//Check reset
        if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
            if (mChannelMode[2] == emCapture) {
                mTIOC2  = mCurTIC[2];
                mIA1    = i_event_1_0.read();
                mIA2A   = i_event_2A.read();
                mIA2B   = i_event_2B.read();
                unsigned int value = mChannelValue[2];
                if ((value == 4) || (value == 12)) {
                    //disable input capture
                    re_printf ("info","Disable Input Catpure.\n");
                } else if (value == 5) {
                    if (mTIOC2 ==1) {
                        mCaptureGRCEvent[2].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    } else {
                        mCaptureOCRCEvent[2].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    }
                } else if (value == 6) {
                    if (mTIOC2 ==1) {
                        mCaptureOCRCEvent[2].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    } else {
                        mCaptureGRCEvent[2].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)), 
                                                        mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    }
                } else if (value == 7) {
                    mCaptureGRCEvent[2].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit);     //after 2 cycle
                } else if (value == 13) {
                    if (mIA1 == 1) {
                        mCaptureGRCEvent[2].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                } else if (value == 14) {
                    if (mIA2A == 1) {
                        mCaptureGRCEvent[2].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                } else if (value == 15) {
                    if (mIA2B == 1) {
                        mCaptureGRCEvent[2].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : HandleInputChannel3Method
// Description  : Handle TIOC3 input
**********************************/
void ATU5010_TIMERC_SUB::HandleInputChannel3Method(void)
{//{{{
    if (!IsModelResetActive()) {//Check reset
        if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
            if (mChannelMode[3] == emCapture) {
                mTIOC3  = mCurTIC[3];
                mIA1    = i_event_1_0.read();
                mIA2A   = i_event_2A.read();
                mIA2B   = i_event_2B.read();
                unsigned int value = mChannelValue[3];
                if ((value == 4) || (value == 12)) {
                    //disable input capture
                    re_printf ("info","Disable Input Catpure.\n");
                } else if (value == 5) {
                    if (mTIOC3 == 1) {
                        mCaptureGRCEvent[3].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    } else {
                        mCaptureOCRCEvent[3].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    }
                } else if (value == 6) {
                    if (mTIOC3 == 1) {
                        mCaptureOCRCEvent[3].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]) + (mTIMERC->mPCLKPeriod * 2))),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    } else {
                        mCaptureGRCEvent[3].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit); //after 2 cycle
                    }
                } else if (value == 7) {
                    mCaptureGRCEvent[3].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod * 2)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit);     //after 2 cycle
                } else if (value == 13) {
                    if (mIA1 == 1) {
                        mCaptureGRCEvent[3].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                } else if (value == 14) {
                    if (mIA2A == 1) {
                        mCaptureGRCEvent[3].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                } else if (value == 15) {
                    if (mIA2B == 1) {
                        mCaptureGRCEvent[3].notify(((CalculateClockEdge("PCLK", true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0])) + (mTIMERC->mPCLKPeriod)),
                                                        mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : CaptureGRCMethod
// Description  : Capture the TCNTC value to GRC0
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::CaptureGRCMethod(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        unsigned int value_cap = GetCounterValue();
        mTIC_SUBFunc->UpdateGRCReg(value_cap,channel);
        re_printf("info","Capture counter value to GRC%i and GRMC%i",channel,channel);
        mHandleFlagAndInterruptGRCEvent[channel].notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : CaptureOCRCMethod
// Description  : Capture the TCNTC value to OCRC0
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::CaptureOCRCMethod(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        unsigned int value_cap = GetCounterValue();
        mTIC_SUBFunc->UpdateOCRCReg(value_cap,channel);
        re_printf("info","Capture counter value to OCRC%i and OCMRC%i",channel,channel);
        mHandleFlagAndInterruptOCRCEvent[channel].notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : SetOCRCValue
// Description  : Set the value to mOCRCValue variable
*********************************/
void ATU5010_TIMERC_SUB::SetOCRCValue (const unsigned int channel, const unsigned int value)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        mOCRCValue[channel] = value;
        if (!mIsTimerRunning) {
            SetCmpOCRCValue(channel, value);
        } else {
            if (mChannelMode[channel] != emOneshot) {
                mTIC_SUBFunc->SetTCNTC(GetCounterValue());
                mRunCntOCRC[channel]->StartStop(false);
                SetCmpOCRCValue(channel,value);
                //----Checking Initial Value and Cmp Value are equal--------------------------
                if (mOCRCValue[channel] == (1+ (unsigned int)mRunCntOCRC[channel]->GetCounterValue())) {
                    mHandleCmpOCRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK",true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit); //add 0818
                } else {
                    mStartCntOCRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK",true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit); //edit 0818
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : SetGRCValue
// Description  : Set the value to mGRCValue variable
*********************************/
void ATU5010_TIMERC_SUB::SetGRCValue (const unsigned int channel, const unsigned int value)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        mGRCValue[channel] = value;
        if (!mIsTSTRCTrue) {
            if (mChannelMode[channel] == emComp) {
                SetCmpGRCValue(channel, value);
            }
        } else {
            if (!mIsTimerRunning) {
                SetCmpGRCValue(channel,value);
            } else {
                mTIC_SUBFunc->SetTCNTC(GetCounterValue());
                if ((channel == 0) && (mChannelMode[channel] == emPWM)) {
                    mIsTimerRunning = false;
                    mLimitValue = mGRCValue[0];
                    mFreeRunCnt->StartStop(false);
                    mTIC_SUBFunc->SetCounterStatus(false); //add 0818
                    for (unsigned int i = 0; i < emChannel; i++) {
                        mRunCntGRC[i]->StartStop(false);
                        mRunCntOCRC[i]->StartStop(false);
                    }
                    mFreeRunCnt->SetCompareMatchValue(mLimitValue);
                    mRestartTimerEvent.notify(CalculateClockEdge("TCNTCCLK",true,false, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
                } else {
                    mRunCntGRC[channel]->StartStop(false);
                    SetCmpGRCValue(channel,value);
                    //----Checking Initial Value and Cmp Value are equal--------------------------
                    if (mGRCValue[channel] == (1+ (unsigned int)mTIC_SUBFunc->GetTCNTC())) {
                        mHandleCmpGRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK",true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
                    } else {
                        mStartCntGRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK",true,false, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : OffCntUpperLimit
// Description  : Off counter upper limit 
*********************************/
void ATU5010_TIMERC_SUB::OffCntUpperLimit(void)
{//{{{
    if (mIsTimerRunning && !IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        re_printf("info","Turn off Counter upper limit.\n");
        mLimitValue = 0xFFFFFFFF;
        mIs_Upper_Limit = false;
        mFreeRunCnt->StartStop(false);
        mTIC_SUBFunc->SetCounterStatus(false); //add 0818
        mIsTimerRunning = false;
        mFreeRunCnt->SetCompareMatchValue(mLimitValue);
        mStartTimerEvent.cancel(); //add 0818
        mRestartTimerEvent.cancel();//add 0818
        mRestartCUCRCEvent.cancel();//add 0818
        for (unsigned int i = 0; i < emChannel; i++) {
            mRunCntOCRC[i]->StartStop(false);
            mRunCntGRC[i]->StartStop(false);
        }
        mTIC_SUBFunc->SetTCNTC(GetCounterValue());
        mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK",true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit); //true,true
    }
}//}}}

/*********************************
// Function     : SetModeDuringRunning
// Description  : Set the counter upper limit function 
*********************************/
void ATU5010_TIMERC_SUB::SetModeDuringRunning(void)
{//{{{
    for (unsigned int channel = 0; channel < emChannel; channel++) {
        mRunCntGRC[channel]->StartStop(false);
        mRunCntOCRC[channel]->StartStop(false);
    }
    mTIC_SUBFunc->SetTCNTC(GetCounterValue());
    mTIC_SUBFunc->SetCounterStatus(false);
    for (unsigned int channel = 0; channel <= 3; channel++) {//Set virtual counter for GRC and OCRC
        SetCmpOCRCValue(channel,mOCRCValue[channel]);
        SetCmpGRCValue(channel,mGRCValue[channel]);
    }
    mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK", true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit); //add 0818
}//}}}

/*********************************
// Function     : SetUpperLimit
// Description  : Set the counter upper limit function 
*********************************/
void ATU5010_TIMERC_SUB::SetUpperLimit (const unsigned int value)
{//{{{
    if (mIs_Upper_Limit && (mChannelMode[0] != emPWM)) { //add 0815. CUCRC not run in PWM
        mLimitValue = value;
    }
}//}}}

/*********************************
// Function     : StartOneShotMethod
// Description  : Handle Oneshot Operation
// Note         : Add 0806
*********************************/
void ATU5010_TIMERC_SUB::StartOneShotMethod(const unsigned int channel, const unsigned int value)
{//{{{
    mGRCValue[channel] = value;
    if (!mIsOneShotRunning[channel]) { //Check the OneShot has not pulsed
        re_printf("info","Oneshot is active channel%i \n",channel);
        mPre_OCRC[channel] = mOCRCValue[channel];
        if (mGRCValue[channel] >= mOCRCValue[channel]) {  //close 0813
            mIsWaitOCRCMatching[channel] = true;
        } 
        if (!mIsTimerRunning) { //Counter is still wait to started (TSTRC has been 1 already)
            if ((mChannelValue[channel] == 10) || (mChannelValue[channel] == 11)) {
                mIsOCRCWaitStart[channel] = true;
                SetCmpOCRCValue(channel,mOCRCValue[channel]);
                SetCmpGRCValue(channel,mGRCValue[channel]);
                if (mGRCValue[channel] > mOCRCValue[channel]) { //add = 0819
                    if (mOCRCValue[channel] >= (mTIC_SUBFunc->GetTCNTC())) {
                        mIsGRCWaitStart[channel] = true;
                    }
                }
                mIsOneShotRunning[channel] = true;
            } else {
                mIsGRCWaitStart[channel] = true;
                SetCmpGRCValue(channel, value); //fix 0813
            }
        } else { //Counter is counting
            mTIC_SUBFunc->SetTCNTC(GetCounterValue());
            if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB") && mIsTSTRCTrue) { //Check clock and reset
                if ((mChannelValue[channel] == 10) || (mChannelValue[channel] == 11)) {
                    SetCmpOCRCValue(channel,mOCRCValue[channel]);
                    if (mGRCValue[channel] >= mOCRCValue[channel]) {
                        if (mOCRCValue[channel] >= (mTIC_SUBFunc->GetTCNTC())) { //add 0818
                            mIsGRCWaitOvf[channel] = true;
                        }
                    }
                    mStartCntOCRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK",true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);//false ->true
                } else {
                    mIsWaitOCRCMatching[channel] = false; //add 0814
                    SetCmpGRCValue(channel,value);
                    mStartCntGRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK",true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);//Add 0806//false -> true
                }
            }
        }
    } else { //Update GRC While 1Shot pulsing
        UpdateGRCOneShot(channel);
    }
}//}}}

/*********************************
// Function     : UpdateGRCOneShot
// Description  : Update GRC During 1 shot
*********************************/
void ATU5010_TIMERC_SUB::UpdateGRCOneShot(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB") && mIsTSTRCTrue) {
        mIsUpdateGRC[channel] = true;
        mHandleCmpGRCEvent[channel].cancel();
        mWriteTIOCOutputEvent[channel].cancel();
        mStartCntGRCEvent[channel].cancel();
        if (!mIsTimerRunning) { //Update when counter is still waited to count
            SetCmpGRCValue(channel,mGRCValue[channel]);
            if (mGRCValue[channel] < mOCRCValue[channel]) {
                mIsGRCWaitOvf[channel] = true;
            } else if (mGRCValue[channel] == mOCRCValue[channel]) {
                if (!mIsWaitOCRCMatching[channel]) {
                    mIsGRCWaitOvf[channel] = true;
                }
            } else {
                mIsGRCWaitStart[channel] = true;
            }
        } else { //Update value while counter is counting
            if (!mIsWaitOCRCMatching[channel]) { //if the OCRC has matched already
                mRunCntGRC[channel]->StartStop(false);
                mTIC_SUBFunc->SetTCNTC(GetCounterValue());
                mHandleCmpGRCEvent[channel].cancel();
                mWriteTIOCOutputEvent[channel].cancel();
                SetCmpGRCValue(channel,mGRCValue[channel]);
                if (mGRCValue[channel] <= ((unsigned int)mTIC_SUBFunc->GetTCNTC())) { // change 0820
                    mIsGRCWaitOvf[channel] = true;
                } else {
                    mStartCntGRCEvent[channel].notify(CalculateClockEdge("TCNTCCLK",true,false, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : HandleForcedCmpMatchMethod
// Description  : HandleForcedCmpMatchMethod
*********************************/
void ATU5010_TIMERC_SUB::HandleForcedCmpMatchMethod(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) { //Check clock and reset
        if (mIsForcedGRC[0] && (mChannelMode[0] == emPWM) && mIsTSTRCTrue) { //add 0819 mIsTSTRCTrue
            mIsForcedGRC[0] =false;
            ////Cancel
            mStartTimerEvent.cancel();
            mHandlePWMEvent.cancel();
            re_printf("info","Counter is cleared to 0");
            mIsTimerRunning = false;
            mTIC_SUBFunc->SetCounterStatus(false);
            mIsOVFPWM = true;
            mFreeRunCnt->StartStop(false);
            for (unsigned int i = 0; i < emChannel; i++) {
                mRunCntGRC[i]->StartStop(false);
                mRunCntOCRC[i]->StartStop(false);
                mHandleCmpGRCEvent[i].cancel();
                mHandleCmpOCRCEvent[i].cancel();
                mStartCntOCRCEvent[i].cancel();
                mStartCntGRCEvent[i].cancel();
                mWriteTIOCOutputEvent[i].cancel();
            }
            mHandlePWMEvent.notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]),mTIMERC->mATU5010->mTimeResolutionUnit);
        } else {
            if (mIsForcedGRC[channel] && mIsForcedOCRC[channel] && mIsOneShotRunning[channel]) {
                mIsForcedOCRC[channel] = false;
                mIsForcedGRC[channel] = false;
                re_printf("info","Cancel 1 shot pulse.\n");
                mStartCntOCRCEvent[channel].cancel();
                mStartCntGRCEvent[channel].cancel();
                mWriteTIOCOutputEvent[channel].cancel();
                mHandleCmpGRCEvent[channel].cancel();
                mHandleCmpOCRCEvent[channel].cancel();
                mRunCntOCRC[channel]->StartStop(false);
                mRunCntGRC[channel]->StartStop(false);
                switch (mChannelValue[channel]) {
                    case 10: mTIOC[channel] = 0;
                        mHandleFlagAndInterruptGRCEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
                        mHandleFlagAndInterruptOCRCEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
                                break;
                    case 11: mTIOC[channel] = 1;
                        mHandleFlagAndInterruptGRCEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
                        mHandleFlagAndInterruptOCRCEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
                                break;
                    default: break;
                }
                mWriteTIOCOutputEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
            } else {
                if (mIsForcedGRC[channel]) {
                    mIsForcedGRC[channel] = false;
                    if (mChannelMode[channel] != emCapture) {
                        if ((mChannelValue[channel] == 1) || (mChannelValue[channel] == 8) || (mChannelValue[channel] == 10)) {
                            mTIOC[channel] = 0;
                        } else if ((mChannelValue[channel] == 2) || (mChannelValue[channel] == 11) || (mChannelValue[channel] == 9)) {
                            mTIOC[channel] = 1;
                        } else if (mChannelValue[channel] == 3) {
                            mTIOC[channel] = !mTIOC[channel];
                        }
                        mWriteTIOCOutputEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                    if (mIsTSTRCTrue) {
                        mRunCntGRC[channel]->StartStop(false);
                        if (mChannelValue[channel] != 0) {
                            mHandleFlagAndInterruptGRCEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
                        }
                    } else { //add 0810 for case While 1shot is pulse, turn off TSTRC -> dump interrupt
                        if ((mIsOneShotRunning[channel] && (mChannelMode[channel] == emOneshot)) || 
                            ((0 < mChannelValue[channel]) && (mChannelValue[channel] < 4))) {
                            mHandleFlagAndInterruptGRCEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
                        }
                    }
                }
                if (mIsForcedOCRC[channel]) {
                    mIsForcedOCRC[channel] = false;
                    mRunCntOCRC[channel]->StartStop(false);
                    if (mChannelMode[channel] == emOneshot) {
                        if (mChannelValue[channel] == 10) {
                            mTIOC[channel] = 1;
                        } else if (mChannelValue[channel] == 11) {
                            mTIOC[channel] = 0;
                        }
                        mWriteTIOCOutputEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
                    }
                    mHandleFlagAndInterruptOCRCEvent[channel].notify(CalculateClockEdge("PCLK",true,false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : HandlePWMMethod
// Description  : PWM Method
*********************************/
void ATU5010_TIMERC_SUB::HandlePWMMethod(void)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB") && mIsTSTRCTrue) {
        mTIC_SUBFunc->SetTCNTC(0);
        mIsOvf = false;
        mIsOVFPWM = true;
        mIsTimerRunning = false; //update 0814 true -> false
        mTIC_SUBFunc->SetCounterStatus(false); // open 0814
        for (unsigned int i = 0; i < emChannel; i++) {
            mHandleCmpGRCEvent[i].notify(SC_ZERO_TIME);
            //Add 0808: Update new GRC each cycle
            if (i != 0) {
                mRunCntGRC[i]->SetCompareMatchValue(mGRCValue[i]);
            }
        }
        if ((mChannelValue[0] != 0) && (mChannelValue[0] != 8)) { //add 0818
            mHandleFlagAndInterruptGRCEvent[0].notify(SC_ZERO_TIME);
        }
        //0806: Change interrupt GRC0 interrupt from cmp match to after 1 PCLK of Cmp match
        if (CalculateClockEdge("TCNTCCLK",true,false, GetCurTime(), mTimeCLKChangePoint) != 0) {
            mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK",true,false, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
        } else {
            mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK",true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
        }
    }
}//}}}

/*********************************
// Function     : HandlePWMMethod
// Description  : PWM Method
*********************************/
void ATU5010_TIMERC_SUB::OffPWM(void)
{//{{{
    if (mIs_Upper_Limit) {
        mLimitValue = mTIC_SUBFunc->GetCUCRC();
    } else {
        mLimitValue = 0xFFFFFFFF;
    }
    re_printf("info","PWM is turned off.\n");
    if (mIsTimerRunning) {
        mFreeRunCnt->StartStop(false); //add 0818
        mIsTimerRunning = false;
        mTIC_SUBFunc->SetTCNTC(GetCounterValue());
        mTIC_SUBFunc->SetCounterStatus(false);
        mFreeRunCnt->SetCompareMatchValue(mLimitValue);
        for (unsigned int channel = 0; channel < emChannel; channel++) {
            mRunCntGRC[channel]->StartStop(false);
            mRunCntOCRC[channel]->StartStop(false);
        }
        mStartTimerEvent.cancel(); //add 0816
        mHandlePWMEvent.cancel();//add 0816
        mRestartTimerEvent.cancel();//add 0816
        mStartTimerEvent.notify(CalculateClockEdge("TCNTCCLK", true,true, GetCurTime(), mTimeCLKChangePoint),mTIMERC->mATU5010->mTimeResolutionUnit);
    } else {
        mFreeRunCnt->SetCompareMatchValue(mLimitValue); //add 0819
    }
}//}}}

/*********************************
// Function     : SetCLKNoisePeriod
// Description  : Set NCNTC clock period
// Parameter    :
//      chan_num            Channel number 0 -> 3
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::SetCLKNoisePeriod (const unsigned int chan_num)
{//{{{
    unsigned int noiseclk_sel = mTIMERC->mTimerC_Func->GetNoiseCLKSel(mSub_id,chan_num);
    sc_dt::uint64 clk_period = 0;
    double start_point = GetCurTime();
    if (noiseclk_sel == 0) {
        clk_period = mCLKNOISE_nPeriod[0];
    } else { // noiseclk_sel == 1
        clk_period = mCLKNOISE_nPeriod[1];
        start_point = mTIMERC->mATU5010->mClkBusStartTime[5];
    }
    mNoiseCanceler[chan_num]->SetCLKNoisePeriod(clk_period, start_point);
}//}}}

/*********************************
// Function     : OutputNoiseMethod
// Description  : Handle noise cancellation output
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::OutputNoiseMethod(const unsigned int chan_num)
{//{{{
    if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        if (mTIMERC->mTimerC_Func->IsNoiseCancelEn(mSub_id, chan_num) && (!mNoiseCanceler[chan_num]->IsZeroNoiseCLk())) {
            mCurTIC[chan_num] = mNoiseCanceler[chan_num]->GetCurOutval();
        } else {
            if (chan_num == 0) {
                mCurTIC[chan_num] = TIOC0.read();
            } else if (chan_num == 1) {
                mCurTIC[chan_num] = TIOC1.read();
            } else if (chan_num == 2) {
                mCurTIC[chan_num] = TIOC2.read();
            } else {
                mCurTIC[chan_num] = TIOC3.read();
            }
        }
        switch (chan_num) {
            case 0:
                //edit 0816
                mHandleInputChannel0Event.notify(SC_ZERO_TIME);
                break;
            case 1:
                //edit 0816
                mHandleInputChannel1Event.notify(SC_ZERO_TIME);
                break;
            case 2:
                //edit 0816
                mHandleInputChannel2Event.notify(SC_ZERO_TIME);
                break;
            case 3:
                //edit 0816
                mHandleInputChannel3Event.notify(SC_ZERO_TIME);
                break;
            default: break;
        }
    }
}//}}}

/*********************************
// Function     : SetNCNTCMethod
// Description  : Handle noise cancellation counter output
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::SetNCNTCMethod (const unsigned int chan_num)
{//{{{
    if (mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) {
        unsigned int cnt_val = mNoiseCanceler[chan_num]->GetUpdatedCntValue();
        mTIC_SUBFunc->SetNCNTC(chan_num,cnt_val);
    }
}//}}}

/*********************************
// Function     : UpdateNoiseCounterValue
// Description  : Update counter value of NCNTC when users write into NCNTC register
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::UpdateNoiseCounterValue (const unsigned int channel, const unsigned int value)
{//{{{
    mNoiseCanceler[channel]->SetNoiseCounterValue(mTIC_SUBFunc->GetNCNTC(channel));
}//}}}

/*********************************
// Function     : SetNoiseCancelMode
// Description  : TimerC set the mode noise cancel for subblock
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::SetNoiseCancelMode (const unsigned int mode)
{//{{{
    for (unsigned int i = 0; i < 4; i++) {
        mNoiseCanceler[i]->SetNoiseCancelMode(mode);
    }
}//}}}

/*********************************
// Function     : SetNoiseCORVal
// Description  : Set Noise Cancellation compare match value
// Parameter    :
//      chan_num            Channel number
//      cor_val             Compare match value
// Return value : None
**********************************/
void ATU5010_TIMERC_SUB::SetNoiseCORVal (const unsigned int chan_num, const unsigned int value)
{//{{{
    mNoiseCanceler[chan_num]->SetNoiseCORVal(value);
}//}}}

///*******************************
/// Function    : GetNoiseCounterValue
/// Description : GetNoiseCounterValue
///******************************
unsigned int ATU5010_TIMERC_SUB::GetNoiseCounterValue(const unsigned int channel)
{//{{{ 
    //Edited 0809
    unsigned int temp = mTIC_SUBFunc->GetNCNTC(channel);
    return (unsigned int)(mNoiseCanceler[channel]->GetNoiseCounterValue(temp));
}//}}}

///*******************************
/// Function    : IsNoiseCancelDuration
/// Description : Check the status of Noise canceler
///******************************
bool ATU5010_TIMERC_SUB::IsNoiseCancelDuration(void)
{//{{{
    bool temp = false;
    for (unsigned int channel = 0; channel < 4; channel++) {
        if (mNoiseCanceler[channel]->IsNoiseCancelDuration()) {
            temp = true;
            break;
        }
    }
    return temp;
}//}}}

///*******************************
/// Function    : EnableNoiseCancel
/// Description : Enable Noise Canceller
///******************************
void ATU5010_TIMERC_SUB::EnableNoiseCancel(const unsigned int channel, const bool is_enable)
{//{{{
    mNoiseCanceler[channel]->Enable(is_enable);
}//}}}

///*******************************
/// Function    : SetNoiseEachCancelMode
/// Description : Set Mode Each Channel
///******************************
void ATU5010_TIMERC_SUB::SetNoiseEachCancelMode (const unsigned int channel, const unsigned int mode)
{//{{{
    mNoiseCanceler[channel]->SetNoiseCancelMode(mode);
}//}}}

void ATU5010_TIMERC_SUB::CheckCmpGRCOCRC(void)
{//{{{
    for (unsigned int channel = 0; channel <= 3; channel++) {
        if (((0 <= mChannelValue[channel]) && (mChannelValue[channel] <= 3)) || (mChannelValue[channel] == 4) ||
            (mChannelValue[channel] ==7) ||
            ((12 <= mChannelValue[channel]) && (mChannelValue[channel] <16))) {
            if (mLimitValue ==  mOCRCValue[channel]) {
                mHandleCmpOCRCEvent[channel].notify(SC_ZERO_TIME);
            }
        }
        if (((mChannelValue[channel] == emComp) && (0 < mChannelValue[channel]) && (mChannelValue[channel] <= 3))) {
            if (mLimitValue == mGRCValue[channel]) {
                mHandleCmpGRCEvent[channel].notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

//update TOCE port
void ATU5010_TIMERC_SUB::ControlTOCE(const unsigned int channel, const bool value)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) { //Check reset/clock
        mTOCE[channel] = value;
        if (mCurTOCE[channel] != value) {
            mWriteTOCEPortEvent[channel].notify(CalculateClockEdge("PCLK", true, false, GetCurTime(), mTIMERC->mATU5010->mTimeCLKChangePoint[0]), mTIMERC->mATU5010->mTimeResolutionUnit);
        }
    }
}//}}}

//update TOCE port
void ATU5010_TIMERC_SUB::WriteTOCEPortMethod(const unsigned int channel)
{//{{{
    if (!IsModelResetActive() && mTIMERC->mATU5010->CheckClockPeriod("CLK_LSB")) { //Check reset/clock
        switch (channel) {
            case 0:
                TOCE0.write(mTOCE[channel]);
                mCurTOCE[channel] = mTOCE[channel];
                break;
            case 1:
                TOCE1.write(mTOCE[channel]);
                mCurTOCE[channel] = mTOCE[channel];
                break;
            case 2:
                TOCE2.write(mTOCE[channel]);
                mCurTOCE[channel] = mTOCE[channel];
                break;
            case 3:
                TOCE3.write(mTOCE[channel]);
                mCurTOCE[channel] = mTOCE[channel];
                break;
            default: break;
        }
    }
}//}}}

void ATU5010_TIMERC_SUB::HandleOvfGRCMethod (const unsigned int channel)
{//{{{
    if (mRunCntGRC[channel]->GetBitOVF() && (!mRunCntGRC[channel]->GetBitCMF())) {
        mRunCntGRC[channel]->ClearBitOVF();
    }
}//}}}

void ATU5010_TIMERC_SUB::HandleOvfOCRCMethod (const unsigned int channel)
{//{{{
    if (mRunCntOCRC[channel]->GetBitOVF() && (!mRunCntOCRC[channel]->GetBitCMF())) {
        mRunCntOCRC[channel]->ClearBitOVF();
    }
}//}}}

// vim600: set foldmethod=marker :
