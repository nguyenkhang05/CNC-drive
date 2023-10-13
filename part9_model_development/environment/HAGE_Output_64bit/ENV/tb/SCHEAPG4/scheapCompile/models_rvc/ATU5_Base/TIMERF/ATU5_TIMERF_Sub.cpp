// ---------------------------------------------------------------------
// $Id: ATU5_TIMERF_Sub.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_TIMERF_Sub.h"
#include "ATU5_TIMERF_Sub_Func.h"
#include "ATU5_TIMERF.h"
#include "ATU5_TIMERF_Func.h"
#include "ATU5_Timer.h"
#include "ATU5_TIMERA.h"

///Constructor of ATU5_TIMERF_Sub class
ATU5_TIMERF_Sub::ATU5_TIMERF_Sub (sc_module_name name,
                        ATU5_TIMERF *ATU5_TIMERF_Ptr,
                        unsigned int sub_id):
    sc_module(name),
    ATU5_TIMERF_Sub_AgentController(),
    // Input ports
    tifa("tifa"),
    tifb("tifb"),
    // Output ports
    ovffi("ovffi"),
    icffi("icffi")
{//{{{
    std::ostringstream port_name;
    for (unsigned int index = 0; index < 8; index++) {
        port_name.str("");
        port_name << "tia" << index;
        tia[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(tia[index] != NULL);
    }
    for (unsigned int index = 0; index < 2; index++) {
        port_name.str("");
        port_name << "poe_sub" << index;
        poe_sub[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(poe_sub[index] != NULL);
    }

    assert(ATU5_TIMERF_Ptr != NULL);
    mATU5_TIMERF = ATU5_TIMERF_Ptr;
    mModuleName = (std::string)name;
    mSubId = sub_id;

    mATU5_TIMERF_Sub_Func = new ATU5_TIMERF_Sub_Func((std::string)name, this);
    sc_assert(mATU5_TIMERF_Sub_Func != NULL);

    mNoiseCancelerA = new ATU5_NoiseCanceler("NoiseCancelerA", mATU5_TIMERF->mATU5);
    sc_assert(mNoiseCancelerA != NULL);

    mNoiseCancelerB = new ATU5_NoiseCanceler("NoiseCancelerB", mATU5_TIMERF->mATU5);
    sc_assert(mNoiseCancelerB != NULL);

    mCounterA = new ATU5_Timer("CounterA", mATU5_TIMERF->mATU5);
    sc_assert(mCounterA != NULL);

    mCounterC = new ATU5_Timer("CounterC", mATU5_TIMERF->mATU5);
    sc_assert(mCounterC != NULL);

    // Initialize variables & output ports
    Initialize();

    // Declare method/thread
    SC_METHOD(ClearECNTBF_isIncreasedMethod);
    dont_initialize();
    sensitive << mClearECNTBF_isIncreasedEvent;

    SC_METHOD(ClearECNTAF_isIncreasedMethod);
    dont_initialize();
    sensitive << mClearECNTAF_isIncreasedEvent;

    SC_METHOD(HandlePOE1Method);
    dont_initialize();
    sensitive << signalB;

    SC_METHOD(OutputNoiseMethodA);
    dont_initialize();
    sensitive << mNoiseCancelerA->mResultOutEvent;

    SC_METHOD(SetNCNTFAMethod);
    dont_initialize();
    sensitive << mNoiseCancelerA->mUpdateCntEvent;

    SC_METHOD(OutputNoiseMethodB);
    dont_initialize();
    sensitive << mNoiseCancelerB->mResultOutEvent;

    SC_METHOD(SetNCNTFBMethod);
    dont_initialize();
    sensitive << mNoiseCancelerB->mUpdateCntEvent;

    SC_METHOD(SlectInputAMethod);
    dont_initialize();
    sensitive << tifa;
    sensitive << (*(tia[0]));
    sensitive << (*(tia[1]));
    sensitive << (*(tia[2]));
    sensitive << (*(tia[3]));
    sensitive << (*(tia[4]));
    sensitive << (*(tia[5]));
    sensitive << (*(tia[6]));
    sensitive << (*(tia[7]));

    SC_METHOD(HandleTIFBMethod);
    dont_initialize();
    sensitive << tifb;

    SC_METHOD(SelectClockInput);
    dont_initialize();
    sensitive << mATU5_TIMERF->mClkUpdateEvent[2];
    sensitive << mATU5_TIMERF->mClkUpdateEvent[3];
    sensitive << mATU5_TIMERF->mClkUpdateEvent[4];
    sensitive << mATU5_TIMERF->mClkUpdateEvent[5];
    sensitive << mATU5_TIMERF->mClkUpdateEvent[6];
    sensitive << mATU5_TIMERF->mClkUpdateEvent[7];
    sensitive << mATU5_TIMERF->mClkZeroEvent[2];
    sensitive << mATU5_TIMERF->mClkZeroEvent[3];
    sensitive << mATU5_TIMERF->mClkZeroEvent[4];
    sensitive << mATU5_TIMERF->mClkZeroEvent[5];
    sensitive << mATU5_TIMERF->mClkZeroEvent[6];
    sensitive << mATU5_TIMERF->mClkZeroEvent[7];

    SC_METHOD(StartOperationMethod);
    dont_initialize();
    sensitive << mATU5_TIMERF->mTimerFEnableEvent;
    sensitive << mATU5_TIMERF->mSubblockEnableEvent[mSubId];
    sensitive << mWriteMDFEvent;
    sensitive << mEndEdgeCntInGivenTimeEvent;
    sensitive << mEndEdgeIntCntEvent;
    sensitive << mEndTimeDurInputLevelEvent;
    sensitive << mEndPwmWaveTimeEvent;
    sensitive << mEndRotSpeedPulseEvent;
    sensitive << mEndUpDownCntEvent;
    sensitive << mEndUpDownCntx4Event;
    sensitive << mCntCLKUpdatedEvent;
    sensitive << mECNTAFUpdatedEvent;
    sensitive << mECNTCFUpdatedEvent;
    sensitive << mGRAFUpdatedEvent;
    sensitive << mGRBFUpdatedEvent;
    sensitive << mGRDFUpdatedEvent;

    SC_METHOD(HandleCMTimerAMethod);
    dont_initialize();
    sensitive << mCounterA->mCompareMatchEvent;

    SC_METHOD(HandleCMTimerCMethod);
    dont_initialize();
    sensitive << mCounterC->mCompareMatchEvent;

    SC_METHOD(EndOperationMethod);
    dont_initialize();
    sensitive << mATU5_TIMERF->mSubblockDisabledEvent[mSubId];
    sensitive << mECNTAFCMEvent;
    sensitive << signalA_sync;
    sensitive << mCMECNTBFEvent;
    sensitive << mATU5_TIMERF->mTimerFDisableEvent;
    sensitive << mCntCLKZeroEvent;

    //first mode
    SC_METHOD(EdgeCntInGivenTimeMethod);
    dont_initialize();
    sensitive << mEdgeCntInGivenTimeEvent;

    SC_METHOD(EndEdgeCntInGivenTimeMethod);
    dont_initialize();
    sensitive << mEndEdgeCntInGivenTimeEvent;

    //second mode
    SC_METHOD(EdgeIntCntMethod);
    dont_initialize();
    sensitive << mEdgeIntCntEvent;

    SC_METHOD(EndEdgeIntCntMethod);
    dont_initialize();
    sensitive << mEndEdgeIntCntEvent;

    //third mode
    SC_METHOD(TimeDurInputLevelMethod);
    dont_initialize();
    sensitive << mTimeDurInputLevelEvent;

    SC_METHOD(EndTimeDurInputLevelMethod);
    dont_initialize();
    sensitive << mEndTimeDurInputLevelEvent;

    //fourth mode
    SC_METHOD(PwmWaveTimeMethod);
    dont_initialize();
    sensitive << mPwmWaveTimeEvent;

    SC_METHOD(EndPwmWaveTimeMethod);
    dont_initialize();
    sensitive << mEndPwmWaveTimeEvent;

    //fifth mode
    SC_METHOD(RotSpeedPulseMethod);
    dont_initialize();
    sensitive << mRotSpeedPulseEvent;

    SC_METHOD(EndRotSpeedPulseMethod);
    dont_initialize();
    sensitive << mEndRotSpeedPulseEvent;

    //sixth mode
    SC_METHOD(UpDownCntMethod);
    dont_initialize();
    sensitive << mUpDownCntEvent;

    SC_METHOD(EndUpDownCntMethod);
    dont_initialize();
    sensitive << mEndUpDownCntEvent;

    //seventh mode
    SC_METHOD(UpDownCntx4Method);
    dont_initialize();
    sensitive << mUpDownCntx4Event;

    SC_METHOD(EndUpDownCntx4Method);
    dont_initialize();
    sensitive << mEndUpDownCntx4Event;

    /// Timer Methods
    SC_METHOD(ClearECNTBFMethod);
    dont_initialize();
    sensitive << ClearECNTBFEvent;

    SC_METHOD(CompareMatchAMethod);
    dont_initialize();
    sensitive << mECNTAFCMEvent;

    SC_METHOD(CompareMatchBMethod);
    dont_initialize();
    sensitive << mCMECNTBFEvent;

    SC_METHOD(StartTimerCMethod);
    dont_initialize();
    sensitive << mStartTimerCEvent;

    SC_METHOD(StartTimerAMethod);
    dont_initialize();
    sensitive << mStartTimerAEvent;

    SC_METHOD(StopTimerAMethod);
    dont_initialize();
    sensitive << mStopTimerAEvent;

    SC_METHOD(StopTimerCMethod);
    dont_initialize();
    sensitive << mStopTimerCEvent;

    SC_METHOD(CntEdgeAMethod);
    dont_initialize();
    sensitive << signalA;

    SC_METHOD(LevelAMethod);
    dont_initialize();
    sensitive << signalA_sync;

    SC_METHOD(AssertSignalA_syncMethod);
    dont_initialize();
    sensitive << mAssertSignalA_syncEvent;

    SC_METHOD(DeassertSignalA_syncMethod);
    dont_initialize();
    sensitive << mDeassertSignalA_syncEvent;

    SC_METHOD(SignalAisChangeMethod);
    dont_initialize();
    sensitive << signalA;

    SC_METHOD(CntEdgeBMethod);
    dont_initialize();
    sensitive << signalB;

    SC_METHOD(IncreaseECNTBFMethod);
    dont_initialize();
    sensitive << mIncreaseECNTBFEvent;

    SC_METHOD(DecreaseECNTBFMethod);
    dont_initialize();
    sensitive << mDecreaseECNTBFEvent;

    ///output method
    SC_METHOD(AssertICFFIMethod);
    dont_initialize();
    sensitive << mAssertICFFIEvent;

    SC_METHOD(DeassertICFFIMethod);
    dont_initialize();
    sensitive << mDeassertICFFIEvent;

    SC_METHOD(AssertOVFFIMethod);
    dont_initialize();
    sensitive << mAssertOVFFIEvent;

    SC_METHOD(DeassertIOVFIMethod);
    dont_initialize();
    sensitive << mDeassertIOVFIEvent;

    SC_METHOD(HandleOVFbyGTimerAMethod);
    dont_initialize();
    sensitive << mCounterA->mOverflowEvent;

    SC_METHOD(OVFAMethod);
    dont_initialize();
    sensitive << mECNTAFOverFlowEvent;

    SC_METHOD(OVFBMethod);
    dont_initialize();
    sensitive << mOVFBEvent;

    SC_METHOD(HandleOVFbyGTimerCMethod);
    dont_initialize();
    sensitive << mCounterC->mOverflowEvent;

    SC_METHOD(OVFCMethod);
    dont_initialize();
    sensitive << mECNTCFOverFlowEvent;
    sensitive << mECNTCFCMEvent;

    SC_METHOD(HandlePOE0Method);
    dont_initialize();
    sensitive << signalA;
}//}}}

///Destructor of ATU5_TIMERF_Sub class
ATU5_TIMERF_Sub::~ATU5_TIMERF_Sub (void)
{//{{{
    delete mATU5_TIMERF_Sub_Func;
    delete mNoiseCancelerA;
    delete mNoiseCancelerB;
    delete mCounterA;
    delete mCounterC;
}//}}}

/// Cancel operation events
void ATU5_TIMERF_Sub::CancelEvents (void)
{//{{{
    mAssertSignalA_syncEvent.cancel();
    mDeassertSignalA_syncEvent.cancel();

    mECNTAFCMEvent.cancel();
    mECNTCFCMEvent.cancel();
    mECNTAFOverFlowEvent.cancel();
    mECNTCFOverFlowEvent.cancel();
    ClearECNTBFEvent.cancel();
    mStartTimerCEvent.cancel();
    mStartTimerAEvent.cancel();
    mStopTimerAEvent.cancel();
    mStopTimerCEvent.cancel();
    mIncreaseECNTBFEvent.cancel();
    mDecreaseECNTBFEvent.cancel();
    mCMECNTBFEvent.cancel();

    mWriteMDFEvent.cancel();
    mOVFBEvent.cancel();
    mCntCLKZeroEvent.cancel();
    mClearECNTBF_isIncreasedEvent.cancel();
    mClearECNTAF_isIncreasedEvent.cancel();

    mCntCLKUpdatedEvent.cancel();
    mECNTAFUpdatedEvent.cancel();
    mECNTCFUpdatedEvent.cancel();
    mGRAFUpdatedEvent.cancel();
    mGRBFUpdatedEvent.cancel();
    mGRDFUpdatedEvent.cancel();

    mEndEdgeCntInGivenTimeEvent.cancel();
    mEndEdgeIntCntEvent.cancel();
    mEndTimeDurInputLevelEvent.cancel();
    mEndPwmWaveTimeEvent.cancel();
    mEndRotSpeedPulseEvent.cancel();
    mEndUpDownCntEvent.cancel();
    mEndUpDownCntx4Event.cancel();
    mEdgeCntInGivenTimeEvent.cancel();
    mEdgeIntCntEvent.cancel();
    mTimeDurInputLevelEvent.cancel();
    mPwmWaveTimeEvent.cancel();
    mRotSpeedPulseEvent.cancel();
    mUpDownCntEvent.cancel();
    mUpDownCntx4Event.cancel();
    mAssertOVFFIEvent.cancel();
    mDeassertIOVFIEvent.cancel();
    mAssertICFFIEvent.cancel();
    mDeassertICFFIEvent.cancel();
}//}}}

/// Initialize variables & output ports
void ATU5_TIMERF_Sub::Initialize (void)
{//{{{
    // Initialize variables
    mGRCF              = (unsigned int)0xFFFFFFFFU;
    mGRAF              = (unsigned int)0xFFFFFFFFU;
    mGRDF              = (unsigned int)0xFFFFFFFFU;
    mCntCLKUpdated     = false;
    mECNTAFUpdated     = false;
    mECNTCFUpdated     = false;
    mGRAFUpdated       = false;
    mGRBFUpdated       = false;
    mGRDFUpdated       = false;

    mCntCLKPeriod      = 0;
    mTimeCLKChangePoint= 0;
    mCntCLKFreq        = 0;
    mCntCLKOrgFreq     = 0;
    mCntCLKFreqUnit    = "Hz";
    mClkName           = "";
    mTimeCLKChangePoint= 0;
    mTimeCLK_NOISEChangePoint = 0;

    mECNTBF =0;
    signalA.write(false);
    signalB.write(false);
    signalA_sync.write(false);

    mIsTimerARunning = false;
    mIsTimerCRunning = false;
    mECNTCFisCM = false;
    mIsCMA = false;
    mIsCMB = false;
    mECNTBF_isIncreased = false;
    mECNTAF_isIncreased = false;
    mCmpATime = 0;
    mCmpCTime = 0;
    mIsCmpA = false;
    mIsCmpC = false;

    mATU5_TIMERF_Sub_Func->Initialize();

    mIsEdgeCntInGivenTimeRunning = false;
    mIsEdgeIntCntRunning         = false;
    mIsTimeDurInputLevelRunning  = false;
    mIsPwmWaveTimeRunning        = false;
    mIsRotSpeedPulseRunning      = false;
    mIsUpDownCntRunning          = false;
    mIsUpDownCntx4Running        = false;

    //General timer setting
    mCounterA->SetupGeneralTimer(true, true);
    mCounterA->SetCounterMode(true);//Free-running mode
    mCounterA->SetTimerSize(32);

    mCounterC->SetupGeneralTimer(false, true);
    mCounterC->SetCounterMode(true);//Free-running mode
    mCounterC->SetTimerSize(32);

    // Initialize output ports
    ovffi.initialize(false);
    icffi.initialize(false);
    (*poe_sub[0]).initialize(false);
    (*poe_sub[1]).initialize(false);
}//}}}

void ATU5_TIMERF_Sub::HandlePOE0Method(void) {
    poe_sub[0]->write(signalA.read());
}

void ATU5_TIMERF_Sub::HandlePOE1Method(void) {
    poe_sub[1]->write(signalB.read());
}

void ATU5_TIMERF_Sub::CompareMatchAMethod (void) {
    mIsCMA = true;
}

void ATU5_TIMERF_Sub::CompareMatchBMethod (void) {
    mIsCMB = true;
}

/// Start Timer
void ATU5_TIMERF_Sub::StartTimerAMethod(void) {
    if(mIsTimeDurInputLevelRunning && mECNTAF_isIncreased) {

        double current_time = sc_time_stamp().to_double();

        unsigned int counter_value = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTAF") + 1;
        unsigned int cm_value = 0xFFFFFFFF;
        SetupCounter ("counterA", counter_value, cm_value, (double)mCntCLKPeriod);
        //
        //Edge A counter
        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
        if(counter_value==0){
            mECNTAFOverFlowEvent.notify(SC_ZERO_TIME);
        }
        re_printf("info","ATU5_TIMERF_Sub(496) TimerA is about started due to Level of TIFA ECNTAF=%d\n", (unsigned int)counter_value);
        mCounterA->StartStop(true);
        mIsTimerARunning = true;
    }
    else if(mIsPwmWaveTimeRunning && mECNTAF_isIncreased) {
        double current_time = sc_time_stamp().to_double();
        unsigned int counter_valueA = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTAF") + 1;
        unsigned int cm_valueA = 0xFFFFFFFF;
        SetupCounter ("counterA", counter_valueA, cm_valueA, (double)mCntCLKPeriod);
        if(counter_valueA==0){
            mECNTAFOverFlowEvent.notify(SC_ZERO_TIME);
        }
        mCounterA->StartStop(true);
        mIsTimerARunning = true;
    }
    else if(mIsRotSpeedPulseRunning && mECNTAF_isIncreased) {
        double current_time = sc_time_stamp().to_double();
        unsigned int counter_valueA = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTAF") + 1;
        unsigned int cm_valueA = 0xFFFFFFFF;
        SetupCounter ("counterA", counter_valueA, cm_valueA, (double)mCntCLKPeriod);
        if(counter_valueA==0){
            mECNTAFOverFlowEvent.notify(SC_ZERO_TIME);
        }
        mCounterA->StartStop(true);
        mIsTimerARunning = true;
    }
    else if(mIsEdgeCntInGivenTimeRunning || mIsEdgeIntCntRunning || mIsUpDownCntRunning || mIsUpDownCntx4Running) {
        unsigned int counter_value = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTAF") + 1;
        unsigned int cm_value;
        if(mIsEdgeCntInGivenTimeRunning || mIsUpDownCntRunning || mIsUpDownCntx4Running) {
            cm_value = mGRAF;
        }
        else if(mIsEdgeIntCntRunning) {
            cm_value = 0xFFFFFFFF;
        }
        SetupCounter ("counterA", counter_value, cm_value, (double)mCntCLKPeriod);
        if(counter_value==0){
            mECNTAFOverFlowEvent.notify(SC_ZERO_TIME);
        }
//         if(counter_value==cm_value) {
//             mECNTAFCMEvent.notify(SC_ZERO_TIME);
//         }
        mCounterA->StartStop(true);
        mIsTimerARunning = true;
    }
}

void ATU5_TIMERF_Sub::StartTimerCMethod(void) {
    if(mIsRotSpeedPulseRunning || mIsPwmWaveTimeRunning) {
        unsigned int counter_valueC = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTCF") + 1;
        unsigned int cm_valueC;
        if(mIsRotSpeedPulseRunning) {
            cm_valueC = mATU5_TIMERF_Sub_Func->GetRegValue("GRBF")*65536;
        }
        else if(mIsPwmWaveTimeRunning) {
            cm_valueC = 0xFFFFFFFF;
            if(mATU5_TIMERF->mATU5_TIMERF_Func->GRDFCMEnabled()){
                cm_valueC = mGRDF;
            }
        }
        SetupCounter ("counterC", counter_valueC, cm_valueC, (double)mCntCLKPeriod);
        if(counter_valueC==0){
            mECNTCFOverFlowEvent.notify(SC_ZERO_TIME);
        }
    }

    mCounterC->StartStop(true);
    mIsTimerCRunning = true;
}

void ATU5_TIMERF_Sub::StopTimerCMethod(void) {
    mCounterC->StartStop(false);
    mIsTimerCRunning = false;
}

/// Stop Timer
void ATU5_TIMERF_Sub::StopTimerAMethod(void) {
    if(mIsTimerARunning) {
        if((mIsTimeDurInputLevelRunning || mIsPwmWaveTimeRunning || mIsRotSpeedPulseRunning)&&!mECNTAFUpdated) {
            unsigned int counter_value;
            counter_value = mCounterA->GetCounterValue();
            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",counter_value);
        }
        mCounterA->StartStop(false);
        mIsTimerARunning = false;
    }
}

/// Setup Counter
void ATU5_TIMERF_Sub::SetupCounter (const std::string counter_name, unsigned int counter_value,unsigned int cm_value, double period) {
    if(counter_name=="counterA") {
        mCounterA->SetupGeneralTimer(false, true);
        mCounterA->SetCounterMode(true);//Free-running mode
        mCounterA->SetTimerSize(32);

        mCounterA->SetCounterInitialValue(counter_value);        //set counter register
        mCounterA->SetCompareMatchValue(cm_value);              // set compare match register
        mCounterA->SetCounterPeriod(period);                    //set clock count period
    }
    if(counter_name=="counterC") {
        mCounterC->SetupGeneralTimer(false, true);
        mCounterC->SetCounterMode(true);//Free-running mode
        mCounterC->SetTimerSize(32);

        mCounterC->SetCounterInitialValue(counter_value);        //set counter register
        mCounterC->SetCompareMatchValue(cm_value);              // set compare match register
        mCounterC->SetCounterPeriod(period);                    //set clock count period
    }
}

/// Start Operation Method
void ATU5_TIMERF_Sub::StartOperationMethod (void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) && mATU5_TIMERF->mIsTimerFEnabled) {
            switch(mATU5_TIMERF_Sub_Func->OperationMode()) {
                case 0:
                    if(!mIsEdgeCntInGivenTimeRunning) {
                        mIsEdgeCntInGivenTimeRunning = true;
                        double current_time = sc_time_stamp().to_double();
                        mEdgeCntInGivenTimeEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
                        re_printf("info"," ATU5_TIMERF_Sub(413) EdgeCntInGivenTime is started\n");
                    }
                    else if(mCntCLKUpdated || mECNTAFUpdated || mGRAFUpdated) {
                        re_printf("info"," ATU5_TIMERF_Sub(490) EdgeCntInGivenTime is updated\n");
                        if(!mECNTAFUpdated && mIsTimerARunning) {
                            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                        }
                        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                        mStopTimerAEvent.notify(SC_ZERO_TIME);
                        double current_time = sc_time_stamp().to_double();
                        mEdgeCntInGivenTimeEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                    }
                    else {
                        re_printf("info"," ATU5_TIMERF_Sub(416) EdgeCntInGivenTime is already started\n");
                    }
                    break;
                case 1:
                    if(!mIsEdgeIntCntRunning) {
                        mIsEdgeIntCntRunning = true;
                        double current_time = sc_time_stamp().to_double();
                        mEdgeIntCntEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
                        re_printf("info"," ATU5_TIMERF_Sub(415) EdgeIntCnt is started\n");
                    }
                    else if(mCntCLKUpdated || mECNTAFUpdated) {
                        if(!mECNTAFUpdated && mIsTimerARunning) {
                            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                        }
                        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                        mStopTimerAEvent.notify(SC_ZERO_TIME);
                        double current_time = sc_time_stamp().to_double();
                        mEdgeIntCntEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                    }
                    break;
                case 2:
                    if(!mIsTimeDurInputLevelRunning) {
                        mIsTimeDurInputLevelRunning = true;
                        mTimeDurInputLevelEvent.notify(SC_ZERO_TIME);
                        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
                        re_printf("info"," ATU5_TIMERF_Sub(419) TimeDurInputLevel is started\n");
                    }
                    else if(mCntCLKUpdated || mECNTAFUpdated) {
                        if(!mECNTAFUpdated && mIsTimerARunning) {
                            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                        }
                        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                        mStopTimerAEvent.notify(SC_ZERO_TIME);
                        double current_time = sc_time_stamp().to_double();
                        mTimeDurInputLevelEvent.notify(SC_ZERO_TIME);
                    }
                    break;
                case 4:
                    if(!mIsPwmWaveTimeRunning) {
                        mIsPwmWaveTimeRunning = true;
                        double current_time = sc_time_stamp().to_double();
                        mPwmWaveTimeEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
                        re_printf("info"," ATU5_TIMERF_Sub(423) PwmWaveTime is started\n");
                    }
                    else if(mCntCLKUpdated || mECNTAFUpdated || mECNTCFUpdated) {
                        if(!mECNTAFUpdated && mIsTimerARunning) {
                            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                        }
                        if(!mECNTCFUpdated && mIsTimerCRunning) {
                            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTCF",mCounterC->GetCounterValue());
                        }
                        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                        mStopTimerCEvent.notify(SC_ZERO_TIME);
                        mStopTimerAEvent.notify(SC_ZERO_TIME);
                        double current_time = sc_time_stamp().to_double();
                        mPwmWaveTimeEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                    }
                    break;
                case 5:
                    if(!mIsRotSpeedPulseRunning) {
                        mIsRotSpeedPulseRunning = true;
                        double current_time = sc_time_stamp().to_double();
                        mRotSpeedPulseEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
                        re_printf("info"," ATU5_TIMERF_Sub(427) RotSpeedPulse is started\n");
                    }
                    else if(mCntCLKUpdated || mECNTAFUpdated || mECNTCFUpdated || mGRBFUpdated || mGRDFUpdated) {
                        if(!mECNTAFUpdated && mIsTimerARunning) {
                            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                        }
                        if(!mECNTCFUpdated && mIsTimerCRunning) {
                            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTCF",mCounterC->GetCounterValue());
                        }
                        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                        mStopTimerCEvent.notify(SC_ZERO_TIME);
                        mStopTimerAEvent.notify(SC_ZERO_TIME);
                        double current_time = sc_time_stamp().to_double();
                        mRotSpeedPulseEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                    }
                    break;
                case 6:
                    if(!mIsUpDownCntRunning && mSubId<3) {
                        mIsUpDownCntRunning = true;
                        double current_time = sc_time_stamp().to_double();
                        mUpDownCntEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
                        re_printf("info"," ATU5_TIMERF_Sub(431) UpDownCnt is started\n");
                    }
                    else if((mCntCLKUpdated || mECNTAFUpdated || mGRAFUpdated)  && mSubId<3) {
                        if(!mECNTAFUpdated && mIsTimerARunning) {
                            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                        }
                        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                        mStopTimerAEvent.notify(SC_ZERO_TIME);
                        double current_time = sc_time_stamp().to_double();
                        mUpDownCntEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                    }
                    break;
                case 7:
                    if(!mIsUpDownCntx4Running && mSubId<3) {
                        mIsUpDownCntx4Running = true;
                        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
                        double current_time = sc_time_stamp().to_double();
                        mUpDownCntx4Event.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
                        re_printf("info"," ATU5_TIMERF_Sub(435) UpDownCntx4 is started\n");
                    }
                    else if((mCntCLKUpdated || mECNTAFUpdated || mGRAFUpdated)  && mSubId<3) {
                        if(!mECNTAFUpdated && mIsTimerARunning) {
                            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                        }
                        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                        mStopTimerAEvent.notify(SC_ZERO_TIME);
                        double current_time = sc_time_stamp().to_double();
                        mUpDownCntx4Event.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

///Clear ECNTBF Method
void ATU5_TIMERF_Sub::ClearECNTBFMethod (void) {
    mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",0);
    mECNTBF = 0;
}

/// End Operation Method
void ATU5_TIMERF_Sub::EndOperationMethod(void) {
    double current_time = sc_time_stamp().to_double();
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(mIsEdgeCntInGivenTimeRunning && (mIsCMA || mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled))) {
            re_printf("info"," ATU5_TIMERF_Sub(450) EdgeCntInGivenTime is stopped\n");
            if(mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled)) {
                if(mIsTimerARunning) {
                    mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                }
                mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                mIsEdgeCntInGivenTimeRunning = false;
                mStopTimerAEvent.notify(SC_ZERO_TIME);
            }
            else {
                mEndEdgeCntInGivenTimeEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time, mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0]) + mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                mIsCMA = false;
            }
        }
        else if(mIsEdgeIntCntRunning && (mIsCMB  || mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled))) {
            if(mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled)) {
                if(mIsTimerARunning) {
                    mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                }
                mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                mIsEdgeIntCntRunning = false;
                mStopTimerAEvent.notify(SC_ZERO_TIME);
            }
            else {
                mEndEdgeIntCntEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                ClearECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time, mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0]) + mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                mIsCMB = false;
            }
        }
        else if(mIsTimeDurInputLevelRunning && (mIsCMB || mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled))) {
            if(mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled)) {
                mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                mIsTimeDurInputLevelRunning = false;
                mStopTimerAEvent.notify(SC_ZERO_TIME);
            }
            else {
                mEndTimeDurInputLevelEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                ClearECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time, mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0]) + mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                mIsCMB = false;
            }
        }
        else if(mIsPwmWaveTimeRunning && (mIsCMB || mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled))) {
            if(mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled)) {
                mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                if(mIsTimerCRunning) {
                    mATU5_TIMERF_Sub_Func->SetRegValue("ECNTCF",mCounterC->GetCounterValue());
                }
                mIsPwmWaveTimeRunning = false;
                mStopTimerAEvent.notify(SC_ZERO_TIME);
                mStopTimerCEvent.notify(SC_ZERO_TIME);
            }
            else {
                mEndPwmWaveTimeEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                ClearECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time, mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0]) + mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                mIsCMB = false;
            }
        }
        else if(mIsRotSpeedPulseRunning) {
            re_printf("info","ATU5_TIMERF_Sub(896) Rotation is stopped\n");
            if(((mATU5_TIMERF_Sub_Func->EdgeSelect()==1 && signalA_sync.read()) ||
                            (mATU5_TIMERF_Sub_Func->EdgeSelect()==2 && !signalA_sync.read()) ||
                             mATU5_TIMERF_Sub_Func->EdgeSelect()==3) || mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled)) {
                if(mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled)) {
                    mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                    if(mIsTimerCRunning) {
                        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTCF",mCounterC->GetCounterValue());
                    }
                    mIsRotSpeedPulseRunning = false;
                    mStopTimerAEvent.notify(SC_ZERO_TIME);
                    mStopTimerCEvent.notify(SC_ZERO_TIME);
                }
                else{
                    mEndRotSpeedPulseEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                }
            }
        }
        else if(mIsUpDownCntRunning && (mIsCMA || mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled))) {
            if(mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled)) {
                if(mIsTimerARunning) {
                    mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                }
                mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                mIsUpDownCntRunning = false;
                mStopTimerAEvent.notify(SC_ZERO_TIME);
            }
            else {
                mEndUpDownCntEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0]) + mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                mIsCMA = false;
            }
        }
        else if(mIsUpDownCntx4Running && (mIsCMA || mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled))) {
            if(mCntCLKPeriod==0 || (!mATU5_TIMERF->mATU5_TIMERF_Func->SubblockIsEnable(mSubId) || !mATU5_TIMERF->mIsTimerFEnabled)) {
                if(mIsTimerARunning) {
                    mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",mCounterA->GetCounterValue());
                }
                mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
                mIsUpDownCntx4Running = false;
                mStopTimerAEvent.notify(SC_ZERO_TIME);
            }
            else {
                mEndUpDownCntx4Event.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0]) + mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                mIsCMA = false;
            }
        }
    }
}

/// Select input
void ATU5_TIMERF_Sub::SlectInputAMethod (void) {
    if(mATU5_TIMERF_Sub_Func->SelectAinput()) {
        if(mATU5_TIMERF->mATU5_TIMERF_Func->IsNoiseCancelEn(mSubId)) {
            if(!mNoiseCancelerA->IsZeroNoiseCLk()) {
                bool inputval = (bool)(tifa.read());
                StartNoiseCancellerA(inputval);
            }
        }
        else {
            signalA.write(tifa.read());
        }
    }
    else {
        unsigned int x = mATU5_TIMERF_Sub_Func->SelectTIAinput();
        signalA.write((*(tia[x])).read());
    }
}

void ATU5_TIMERF_Sub::HandleTIFBMethod (void) {
    if(mATU5_TIMERF->mATU5_TIMERF_Func->IsNoiseCancelEn(mSubId) && (mSubId<3)) {
        if(!mNoiseCancelerA->IsZeroNoiseCLk()) {
            bool inputval = (bool)(tifb.read());
            StartNoiseCancellerB(inputval);
        }
    }
    else {
        signalB.write(tifb.read());
    }
}

//***********Start Noise Canceller processing**********************
/*********************************
// Function     : GetNoiseCounterValueB
// Description  : Get current counter value of NCNTFB
// Parameter    :
// Return value : Current counter value
**********************************/
unsigned int ATU5_TIMERF_Sub::GetNoiseCounterValueB (void)
{//{{{
    unsigned int reg_val = mATU5_TIMERF_Sub_Func->GetNCNTFB();
    return (mNoiseCancelerB->GetNoiseCounterValue(reg_val));
}//}}}

/*********************************
// Function     : IsNoiseCancelDuration
// Description  : Check Noise cancellation is in process or not
// Parameter    :
// Return value :
//      true                In noise cancellation duration (NCNTFBx is counting)
//      false               NCNTFBx became 0x0
**********************************/
bool ATU5_TIMERF_Sub::IsNoiseCancelDurationB(void)
{//{{{
    return mNoiseCancelerB->IsNoiseCancelDuration();
}//}}}

void ATU5_TIMERF_Sub::StartNoiseCancellerB (bool inputval) {
    unsigned int mode = mATU5_TIMERF->mATU5->GetNoiseCancelMode("Timer F");
    if (mode == 0) {
        mode = mATU5_TIMERF->mATU5_TIMERF_Func->GetNoiseCancelMode(mSubId);
    }
    mNoiseCancelerB->SetNoiseCancelMode(mode);

    if(!IsNoiseCancelDurationB()) {
        SetNoiseCounterValueB();
        SetNoiseCORValB();
    }
    mNoiseCancelerB->Enable(true);
    mNoiseCancelerB->NotifyNoiseCancellation(true,true,inputval);
}

void ATU5_TIMERF_Sub::SetNoiseCORValB (void) {
    mNoiseCancelerB->SetNoiseCORVal(mATU5_TIMERF_Sub_Func->GetNCRFB());
}

void ATU5_TIMERF_Sub::SetNoiseCounterValueB (void) {
    mNoiseCancelerB->SetNoiseCounterValue(mATU5_TIMERF_Sub_Func->GetNCNTFB()); //Update counter value
}

/*********************************
// Function     : OutputNoiseMethodA
// Description  : Handle noise cancellation output for signal A
// Parameter    :
// Return value : None
**********************************/
void ATU5_TIMERF_Sub::OutputNoiseMethodB (void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(!mNoiseCancelerB->IsZeroNoiseCLk() && mATU5_TIMERF->mATU5_TIMERF_Func->IsNoiseCancelEn(mSubId)) {
            signalB.write(mNoiseCancelerB->GetCurOutval());
        }
    }
}

/*********************************
// Function     : SetNCNTFAMethod
// Description  : Handle noise cancellation counter output
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5_TIMERF_Sub::SetNCNTFBMethod (void)
{//{{{
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(!mNoiseCancelerB->IsZeroNoiseCLk() && mATU5_TIMERF->mATU5_TIMERF_Func->IsNoiseCancelEn(mSubId)) {
            unsigned int cnt_val = mNoiseCancelerB->GetUpdatedCntValue();
            mATU5_TIMERF_Sub_Func->SetNCNTFB(cnt_val);
        }
    }
}//}}}

/*********************************
// Function     : GetNoiseCounterValueA
// Description  : Get current counter value of NCNTFA
// Parameter    :
// Return value : Current counter value
**********************************/
unsigned int ATU5_TIMERF_Sub::GetNoiseCounterValueA (void)
{//{{{
    unsigned int reg_val = mATU5_TIMERF_Sub_Func->GetNCNTFA();
    return (mNoiseCancelerA->GetNoiseCounterValue(reg_val));
}//}}}

/*********************************
// Function     : IsNoiseCancelDuration
// Description  : Check Noise cancellation is in process or not
// Parameter    :
// Return value :
//      true                In noise cancellation duration (NCNTFAx is counting)
//      false               NCNTFAx became 0x0
**********************************/
bool ATU5_TIMERF_Sub::IsNoiseCancelDurationA(void)
{//{{{
    return mNoiseCancelerA->IsNoiseCancelDuration();
}//}}}

void ATU5_TIMERF_Sub::StartNoiseCancellerA (bool inputval) {
    unsigned int mode = mATU5_TIMERF->mATU5->GetNoiseCancelMode("Timer F");
    if (mode == 0) {
        mode = mATU5_TIMERF->mATU5_TIMERF_Func->GetNoiseCancelMode(mSubId);
    }
    mNoiseCancelerA->SetNoiseCancelMode(mode);

    if(!IsNoiseCancelDurationA()) {
        SetNoiseCounterValueA();
        SetNoiseCORValA();
    }
    mNoiseCancelerA->Enable(true);
    mNoiseCancelerA->NotifyNoiseCancellation(true,true,inputval);
}

void ATU5_TIMERF_Sub::SetNoiseCORValA (void) {
    mNoiseCancelerA->SetNoiseCORVal(mATU5_TIMERF_Sub_Func->GetNCRFA());
}

void ATU5_TIMERF_Sub::SetNoiseCounterValueA (void) {
    mNoiseCancelerA->SetNoiseCounterValue(mATU5_TIMERF_Sub_Func->GetNCNTFA()); //Update counter value
}

/*********************************
// Function     : OutputNoiseMethodA
// Description  : Handle noise cancellation output for signal A
// Parameter    :
// Return value : None
**********************************/
void ATU5_TIMERF_Sub::OutputNoiseMethodA (void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(!mNoiseCancelerA->IsZeroNoiseCLk() && mATU5_TIMERF->mATU5_TIMERF_Func->IsNoiseCancelEn(mSubId)) {
            signalA.write(mNoiseCancelerA->GetCurOutval());
        }
    }
}

/*********************************
// Function     : SetNCNTFAMethod
// Description  : Handle noise cancellation counter output
// Parameter    :
//      chan_num            Channel number 0 -> 7
// Return value : None
**********************************/
void ATU5_TIMERF_Sub::SetNCNTFAMethod (void)
{//{{{
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(!mNoiseCancelerA->IsZeroNoiseCLk() && mATU5_TIMERF->mATU5_TIMERF_Func->IsNoiseCancelEn(mSubId)) {
            unsigned int cnt_val = mNoiseCancelerA->GetUpdatedCntValue();
            mATU5_TIMERF_Sub_Func->SetNCNTFA(cnt_val);
        }
    }
}//}}}

void ATU5_TIMERF_Sub::SetCLKNoisePeriod(void) {
    sc_dt::uint64 clk_period = mATU5_TIMERF->mCLKPeriod[8];
    double start_point = sc_time_stamp().to_double();
    mTimeCLK_NOISEChangePoint = start_point;
    mNoiseCancelerA->SetCLKNoisePeriod(clk_period,start_point, false);
    mNoiseCancelerB->SetCLKNoisePeriod(clk_period,start_point, false);
}

//*****************END Noise Canceller processing**************
void ATU5_TIMERF_Sub::SelectClockInput (void) {
    sc_dt::uint64 mCntCLKPeriod_temp=0;                 //Period value of clock
    double timepoint=0;

    switch(mATU5_TIMERF_Sub_Func->SelectClock()) {
        case 0:
            mCntCLKPeriod_temp   =   mATU5_TIMERF->mCLKPeriod[2];
            mCntCLKFreq     =   mATU5_TIMERF->mCLKFreq[2];
            mCntCLKOrgFreq  =   mATU5_TIMERF->mCLKOrgFreq[2];
            mCntCLKFreqUnit =   mATU5_TIMERF->mCLKFreqUnit[2];
            timepoint = (double)mATU5_TIMERF->mTimeCLKChangePoint[2];
            mClkName = "CLKBUS_0";
            break;
        case 1:
            mCntCLKPeriod_temp   =   mATU5_TIMERF->mCLKPeriod[3];
            mCntCLKFreq     =   mATU5_TIMERF->mCLKFreq[3];
            mCntCLKOrgFreq  =   mATU5_TIMERF->mCLKOrgFreq[3];
            mCntCLKFreqUnit =   mATU5_TIMERF->mCLKFreqUnit[3];
            timepoint = (double)mATU5_TIMERF->mTimeCLKChangePoint[3];
            mClkName = "CLKBUS_1";
            break;
        case 2:
            mCntCLKPeriod_temp   =   mATU5_TIMERF->mCLKPeriod[4];
            mCntCLKFreq     =   mATU5_TIMERF->mCLKFreq[4];
            mCntCLKOrgFreq  =   mATU5_TIMERF->mCLKOrgFreq[4];
            mCntCLKFreqUnit =   mATU5_TIMERF->mCLKFreqUnit[4];
            timepoint = (double)mATU5_TIMERF->mTimeCLKChangePoint[4];
            mClkName = "CLKBUS_2";
            break;
        case 3:
            mCntCLKPeriod_temp   =   mATU5_TIMERF->mCLKPeriod[5];
            mCntCLKFreq     =   mATU5_TIMERF->mCLKFreq[5];
            mCntCLKOrgFreq  =   mATU5_TIMERF->mCLKOrgFreq[5];
            mCntCLKFreqUnit =   mATU5_TIMERF->mCLKFreqUnit[5];
            timepoint = (double)mATU5_TIMERF->mTimeCLKChangePoint[5];
            mClkName = "CLKBUS_3";
            break;
        case 4:
            mCntCLKPeriod_temp   =   mATU5_TIMERF->mCLKPeriod[6];
            mCntCLKFreq     =   mATU5_TIMERF->mCLKFreq[6];
            mCntCLKOrgFreq  =   mATU5_TIMERF->mCLKOrgFreq[6];
            mCntCLKFreqUnit =   mATU5_TIMERF->mCLKFreqUnit[6];
            timepoint = (double)mATU5_TIMERF->mTimeCLKChangePoint[6];
            mClkName = "CLKBUS_4";
            break;
        case 5:
            mCntCLKPeriod_temp   =   mATU5_TIMERF->mCLKPeriod[7];
            mCntCLKFreq     =   mATU5_TIMERF->mCLKFreq[7];
            mCntCLKOrgFreq  =   mATU5_TIMERF->mCLKOrgFreq[7];
            mCntCLKFreqUnit =   mATU5_TIMERF->mCLKFreqUnit[7];
            timepoint = (double)mATU5_TIMERF->mTimeCLKChangePoint[7];
            mClkName = "CLKBUS_5";
            break;
        default:
            break;
    }
    if(mCntCLKPeriod_temp!=mCntCLKPeriod && mCntCLKPeriod_temp!=0) {
        double period_num = 0;
        double current_time = sc_time_stamp().to_double();
        if(mCntCLKPeriod!=0) {
            period_num = (current_time - mTimeCLKChangePoint) / mCntCLKPeriod;
        }
        unsigned int mod_period_num = (unsigned int)period_num;
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
        double edge_point = 0, mod_period_num_db = 0;
        mod_period_num_db = (double)mod_period_num;
        if(mCntCLKPeriod!=0) {
            edge_point = mod_period_num_db * mCntCLKPeriod - (current_time - mTimeCLKChangePoint);
        }
        else {
            edge_point = 0;
        }
        mCntCLKUpdated = true;
        if((mIsTimerARunning || mIsTimerCRunning)&&edge_point==0){
            mCntCLKUpdatedEvent.notify((double)mCntCLKPeriod_temp/10,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
        }
        else {
            mCntCLKUpdatedEvent.notify(SC_ZERO_TIME);
        }
    }
    if(mCntCLKPeriod_temp!=mCntCLKPeriod && mCntCLKPeriod_temp==0) {
        mCntCLKZeroEvent.notify(SC_ZERO_TIME);
    }
    mTimeCLKChangePoint = timepoint;
    mCntCLKPeriod = mCntCLKPeriod_temp;
}

//////////////////////////////////////////////////////////////////
//Edge counting in a given time Method
//////////////////////////////////////////////////////////////////
void ATU5_TIMERF_Sub::EdgeCntInGivenTimeMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCntCLKUpdated = false;
        mECNTAFUpdated = false;
        mGRAFUpdated   = false;
        re_printf("info"," ATU5_TIMERF_Sub(1096) EdgeCntInGivenTime is started\n");
        if(mATU5_TIMERF->CheckClockPeriod(mClkName)) {
            mStartTimerAEvent.notify(SC_ZERO_TIME);
        }

        //Edge A counter
        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
    }
}

void ATU5_TIMERF_Sub::EndEdgeCntInGivenTimeMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        re_printf("info"," ATU5_TIMERF_Sub(1361) EdgeCntInGivenTime is stopped\n");
        mIsEdgeCntInGivenTimeRunning = false;
        mStopTimerAEvent.notify(SC_ZERO_TIME);
        mATU5_TIMERF_Sub_Func->SetRegValue("GRBF",mECNTBF);
        mATU5_TIMERF_Sub_Func->SetICFF(1);
        if(mATU5_TIMERF_Sub_Func->ICIEnabled()) {
            mAssertICFFIEvent.notify(SC_ZERO_TIME);
        }
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",0);
        if(mECNTBF_isIncreased) {
            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",1);
            mECNTBF = 1;
        }
        else {
            mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",0);
            mECNTBF = 0;
        }
    }
}

//////////////////////////////////////////////////////////////////
//Effective Edge interval counting Method
//////////////////////////////////////////////////////////////////
void ATU5_TIMERF_Sub::EdgeIntCntMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCntCLKUpdated = false;
        mECNTAFUpdated = false;
        if(mATU5_TIMERF->CheckClockPeriod(mClkName)) {
            mStartTimerAEvent.notify(SC_ZERO_TIME);
        }
        //Edge A counter
        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
    }
}

void ATU5_TIMERF_Sub::EndEdgeIntCntMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mIsEdgeIntCntRunning = false;
        unsigned int counter_value;
        counter_value = mCounterA->GetCounterValue();
        mStopTimerAEvent.notify(SC_ZERO_TIME);
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",0);
        mGRAF = counter_value -1;
        mATU5_TIMERF_Sub_Func->SetICFF(1);
        if(mATU5_TIMERF_Sub_Func->ICIEnabled()) {
            mAssertICFFIEvent.notify(SC_ZERO_TIME);
        }
    }
}

//////////////////////////////////////////////////////////////////
//Measurement of Time during High/Low input levels Method
//////////////////////////////////////////////////////////////////
void ATU5_TIMERF_Sub::TimeDurInputLevelMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCntCLKUpdated = false;
        mECNTAFUpdated = false;
        mECNTCFUpdated = false;
        LevelAMethod();
    }
}

void ATU5_TIMERF_Sub::EndTimeDurInputLevelMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mIsTimeDurInputLevelRunning = false;
        unsigned int counter_value;
        counter_value = mCounterA->GetCounterValue();
        mStopTimerAEvent.notify(SC_ZERO_TIME);

        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",0);
        mCounterA->SetCounterInitialValue(0);
        if(mECNTAF_isIncreased) {
            mGRAF = counter_value - 1;
        }
        else {
            mGRAF = counter_value;
        }
        mATU5_TIMERF_Sub_Func->SetICFF(1);
        if(mATU5_TIMERF_Sub_Func->ICIEnabled()) {
            mAssertICFFIEvent.notify(SC_ZERO_TIME);
        }
    }
}

//////////////////////////////////////////////////////////////////
//Measurement of PWM input waveform Method
//////////////////////////////////////////////////////////////////
void ATU5_TIMERF_Sub::PwmWaveTimeMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCntCLKUpdated = false;
        mECNTAFUpdated = false;
        mECNTCFUpdated = false;
        if(mATU5_TIMERF->CheckClockPeriod(mClkName)) {
            double current_time = sc_time_stamp().to_double();
            mStartTimerCEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
        }
        LevelAMethod();
        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
    }
}

void ATU5_TIMERF_Sub::EndPwmWaveTimeMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mIsPwmWaveTimeRunning = false;
        unsigned int counter_valueA;
        counter_valueA = mCounterA->GetCounterValue();
        if(mECNTAF_isIncreased) {
            mGRAF = counter_valueA - 1;
        }
        else {
            mGRAF = counter_valueA;
        }
        mStopTimerAEvent.notify(SC_ZERO_TIME);
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",0);
        mCounterA->SetCounterInitialValue(0);
        unsigned int counter_valueC;
        mStopTimerCEvent.notify(SC_ZERO_TIME);
        counter_valueC = mCounterC->GetCounterValue();
        mGRCF = counter_valueC - 1;
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTCF",0);

        mATU5_TIMERF_Sub_Func->SetICFF(1);
        if(mATU5_TIMERF_Sub_Func->ICIEnabled()) {
            mAssertICFFIEvent.notify(SC_ZERO_TIME);
        }

    }
}

//////////////////////////////////////////////////////////////////
//Rotation Speed/Pulse Method
//////////////////////////////////////////////////////////////////
void ATU5_TIMERF_Sub::RotSpeedPulseMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCntCLKUpdated = false;
        mECNTAFUpdated = false;
        mECNTCFUpdated = false;
        mGRBFUpdated   = false;
        mGRDFUpdated   = false;
        re_printf("info","ATU5_TIMERF_Sub(1552) rotation is started\n");
        LevelAMethod();
        if(mATU5_TIMERF->CheckClockPeriod(mClkName)) {
            double current_time = sc_time_stamp().to_double();
            mStartTimerCEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
        }

        //Edge A counter
        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
    }
}

void ATU5_TIMERF_Sub::EndRotSpeedPulseMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mIsRotSpeedPulseRunning = false;
        unsigned int counter_valueA;
        counter_valueA = mCounterA->GetCounterValue();
        mGRAF = counter_valueA;
        mStopTimerAEvent.notify(SC_ZERO_TIME);
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",0);
        mCounterA->SetCounterInitialValue(0);

        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);

        unsigned int counter_valueC;
        mStopTimerCEvent.notify(SC_ZERO_TIME);
        counter_valueC = mCounterC->GetCounterValue();
        mGRCF = counter_valueC - 1;
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTCF",0);

        mGRDF = mGRDF + counter_valueC - 1;

        mATU5_TIMERF_Sub_Func->SetICFF(1);
        if(mATU5_TIMERF_Sub_Func->ICIEnabled()) {
            mAssertICFFIEvent.notify(SC_ZERO_TIME);
        }
    }
}

//////////////////////////////////////////////////////////////////
//Up/Down event count Method
//////////////////////////////////////////////////////////////////
void ATU5_TIMERF_Sub::UpDownCntMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCntCLKUpdated = false;
        mECNTAFUpdated = false;
        mGRAFUpdated   = false;
        if(mATU5_TIMERF->CheckClockPeriod(mClkName)) {
            mStartTimerAEvent.notify(SC_ZERO_TIME);
        }
        //Edge A counter
        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
    }
}

void ATU5_TIMERF_Sub::EndUpDownCntMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        re_printf("info"," ATU5_TIMERF_Sub(1096) UpDownCntMethod is stopped\n");
        mIsUpDownCntRunning = false;
        mStopTimerAEvent.notify(SC_ZERO_TIME);
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",0);
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
        mATU5_TIMERF_Sub_Func->SetRegValue("GRBF",mECNTBF);
        mATU5_TIMERF_Sub_Func->SetICFF(1);
        if(mATU5_TIMERF_Sub_Func->ICIEnabled()) {
            mAssertICFFIEvent.notify(SC_ZERO_TIME);
        }
    }
}

//////////////////////////////////////////////////////////////////
//Up/Down event count x4 Method
//////////////////////////////////////////////////////////////////
void ATU5_TIMERF_Sub::UpDownCntx4Method(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCntCLKUpdated = false;
        mECNTAFUpdated = false;
        mGRAFUpdated   = false;
        if(mATU5_TIMERF->CheckClockPeriod(mClkName)) {
            mStartTimerAEvent.notify(SC_ZERO_TIME);
        }
        //Edge A counter
        mECNTBF = mATU5_TIMERF_Sub_Func->GetRegValue("ECNTBF");
    }
}

void ATU5_TIMERF_Sub::EndUpDownCntx4Method(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mIsUpDownCntx4Running = false;
        mStopTimerAEvent.notify(SC_ZERO_TIME);
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",0);
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
        mATU5_TIMERF_Sub_Func->SetRegValue("GRBF",mECNTBF);
        mATU5_TIMERF_Sub_Func->SetICFF(1);
        if(mATU5_TIMERF_Sub_Func->ICIEnabled()) {
            mAssertICFFIEvent.notify(SC_ZERO_TIME);
        }
    }
}

///Edge A counting method
void ATU5_TIMERF_Sub::CntEdgeAMethod (void) {
    double current_time = sc_time_stamp().to_double();
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
        (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (mIsEdgeCntInGivenTimeRunning || mIsEdgeIntCntRunning) {
            if(mATU5_TIMERF_Sub_Func->EdgeSelect()==0) {
                //return
            }
            else if((mATU5_TIMERF_Sub_Func->EdgeSelect()==1 && signalA.read()) ||
                    (mATU5_TIMERF_Sub_Func->EdgeSelect()==2 && !signalA.read()) ||
                     mATU5_TIMERF_Sub_Func->EdgeSelect()==3) {
                double current_time = sc_time_stamp().to_double();
                mIncreaseECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
            }
        }
        else if(mIsTimeDurInputLevelRunning || mIsPwmWaveTimeRunning || mIsRotSpeedPulseRunning) {
            if(mATU5_TIMERF_Sub_Func->EdgeSelect()==0 || mATU5_TIMERF_Sub_Func->EdgeSelect()==3) {
                //return
            }
            else if((mATU5_TIMERF_Sub_Func->EdgeSelect()==1 && signalA.read()) ||
                    (mATU5_TIMERF_Sub_Func->EdgeSelect()==2 && !signalA.read()) ) {
                double current_time = sc_time_stamp().to_double();
                mIncreaseECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
            }
        }
        else if(mIsUpDownCntRunning) {
            if(mATU5_TIMERF_Sub_Func->EdgeSelect()==0 || mATU5_TIMERF_Sub_Func->EdgeSelect()==1 || mATU5_TIMERF_Sub_Func->EdgeSelect()==2) {
                //return
            }
            else {
                if(!signalB.read()) {
                    mIncreaseECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                }
                else {
                    mDecreaseECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                }
            }
        }
        else if(mIsUpDownCntx4Running) {
            if(mATU5_TIMERF_Sub_Func->EdgeSelect()==0 || mATU5_TIMERF_Sub_Func->EdgeSelect()==1 || mATU5_TIMERF_Sub_Func->EdgeSelect()==2) {
                //return
            }
            else {
                if(signalB.read()==!signalA.read()) {
                    mIncreaseECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                }
                else {
                    mDecreaseECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
                }
            }
        }
    }
}

void ATU5_TIMERF_Sub::CntEdgeBMethod (void) {
    double current_time = sc_time_stamp().to_double();
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
        (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (mIsUpDownCntx4Running) {
            if(signalB.read()==signalA.read()) {
                mIncreaseECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
            }
            else {
                mDecreaseECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
            }
        }
    }
}

void ATU5_TIMERF_Sub::LevelAMethod (void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
        (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")) || !mATU5_TIMERF->CheckClockPeriod(mClkName))) {
        if (mIsTimeDurInputLevelRunning || mIsPwmWaveTimeRunning || mIsRotSpeedPulseRunning) {
            if((mATU5_TIMERF_Sub_Func->EdgeSelect()==1 && !signalA_sync.read()) ||
                    (mATU5_TIMERF_Sub_Func->EdgeSelect()==2 && signalA_sync.read()) ) {
                double current_time = sc_time_stamp().to_double();
                mECNTAF_isIncreased = true;
                mStartTimerAEvent.cancel();
                mStartTimerAEvent.notify(mATU5_TIMERF->CalculateClockEdge(mClkName, true, current_time,mTimeCLKChangePoint),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
            }
            else if((mATU5_TIMERF_Sub_Func->EdgeSelect()==1 && signalA_sync.read()) ||
                    (mATU5_TIMERF_Sub_Func->EdgeSelect()==2 && !signalA_sync.read()) ) {
                double current_time = sc_time_stamp().to_double();
                mStopTimerAEvent.notify(SC_ZERO_TIME);
                mClearECNTAF_isIncreasedEvent.notify(SC_ZERO_TIME);
            }
        }
    }
}

void ATU5_TIMERF_Sub::IncreaseECNTBFMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double current_time = sc_time_stamp().to_double();
        mECNTBF_isIncreased = true;
        mClearECNTBF_isIncreasedEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0]),mATU5_TIMERF->mATU5->mTimeResolutionUnit);
        if(mECNTBF+1==mATU5_TIMERF_Sub_Func->GetRegValue("GRBF") && (mIsEdgeIntCntRunning || mIsTimeDurInputLevelRunning || mIsPwmWaveTimeRunning)) {
            mCMECNTBFEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
        }
        if(mECNTBF==0xFFFF) {
            mOVFBEvent.notify(SC_ZERO_TIME);
            mECNTBF = 0;
        }
        else {
            mECNTBF = (mECNTBF + 1) & 0xFFFF;
        }
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
    }
}

void ATU5_TIMERF_Sub::DecreaseECNTBFMethod(void) {
    if (!(mATU5_TIMERF->mATU5->GetResetStatus("presetz") || mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double current_time = sc_time_stamp().to_double();
        mECNTBF = (mECNTBF - 1) & 0xFFFF;
        if(mECNTBF==0xFFFF) {
            mOVFBEvent.notify(SC_ZERO_TIME);
        }
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTBF",mECNTBF);
    }
}

/// Assert ICFFI interrupt port
void ATU5_TIMERF_Sub::AssertICFFIMethod (void)
{//{{{
    icffi.write(true);
    mDeassertICFFIEvent.notify((double)mATU5_TIMERF->mATU5->mCLK_LSBPeriod, mATU5_TIMERF->mATU5->mTimeResolutionUnit);
}//}}}

/// Deassert ICFFI interrupt port
void ATU5_TIMERF_Sub::DeassertICFFIMethod (void)
{//{{{
    icffi.write(false);
}//}}}

/// Overflow methods
void ATU5_TIMERF_Sub::HandleOVFbyGTimerAMethod(void) {
    mCounterC->ClearBitOVF();
    mECNTAFUpdatedEvent.notify((double)mCntCLKPeriod/2, mATU5_TIMERF->mATU5->mTimeResolutionUnit);
    mECNTAFUpdated = true;
    mATU5_TIMERF_Sub_Func->SetRegValue("ECNTAF",0xFFFFFFFF);
}

void ATU5_TIMERF_Sub::HandleOVFbyGTimerCMethod(void) {
    mCounterC->ClearBitOVF();
    mECNTCFUpdatedEvent.notify((double)mCntCLKPeriod/2, mATU5_TIMERF->mATU5->mTimeResolutionUnit);
    mECNTCFUpdated = true;
    mATU5_TIMERF_Sub_Func->SetRegValue("ECNTCF",0xFFFFFFFF);
}

void ATU5_TIMERF_Sub::OVFAMethod(void) {

    mATU5_TIMERF_Sub_Func->SetOVFA(1);
    if(mATU5_TIMERF_Sub_Func->OVFAEnabled()) {
        mAssertOVFFIEvent.notify(SC_ZERO_TIME);
    }
}

void ATU5_TIMERF_Sub::OVFBMethod(void) {
    mATU5_TIMERF_Sub_Func->SetOVFB(1);
    if(mATU5_TIMERF_Sub_Func->OVFBEnabled()) {
        mAssertOVFFIEvent.notify(SC_ZERO_TIME);
    }
}

void ATU5_TIMERF_Sub::OVFCMethod(void) {
    mATU5_TIMERF_Sub_Func->SetOVFC(1);
    if(mATU5_TIMERF_Sub_Func->OVFCEnabled()) {
        mAssertOVFFIEvent.notify(SC_ZERO_TIME);
    }
    if(mECNTCFisCM){
        mECNTCFisCM = false;
        mECNTCFUpdatedEvent.notify((double)mCntCLKPeriod/2, mATU5_TIMERF->mATU5->mTimeResolutionUnit);
        mECNTCFUpdated = true;
        mATU5_TIMERF_Sub_Func->SetRegValue("ECNTCF",0x0);
    }
}

/// Assert ovffi interrupt port
void ATU5_TIMERF_Sub::AssertOVFFIMethod (void)
{//{{{
    ovffi.write(true);
    mDeassertIOVFIEvent.notify((double)mATU5_TIMERF->mATU5->mCLK_LSBPeriod, mATU5_TIMERF->mATU5->mTimeResolutionUnit);
}//}}}

/// Deassert ovffi interrupt port
void ATU5_TIMERF_Sub::DeassertIOVFIMethod (void)
{//{{{
    ovffi.write(false);
}//}}}

void ATU5_TIMERF_Sub::HandleCMTimerAMethod(void) {
    if ((mCmpATime == sc_time_stamp().to_double()) && (mIsCmpA)) {
        return;
    } else {
        mCmpATime = sc_time_stamp().to_double();
        mIsCmpA = true;
        mCounterA->ClearBitCMF();
        mECNTAFCMEvent.notify(SC_ZERO_TIME);
    }
}

void ATU5_TIMERF_Sub::HandleCMTimerCMethod(void) {
    if ((mCmpCTime == sc_time_stamp().to_double()) && (mIsCmpC)) {
        return;
    } else {
        mCmpCTime = sc_time_stamp().to_double();
        mIsCmpC = true;
        mCounterC->ClearBitCMF();
        if((mATU5_TIMERF->mATU5_TIMERF_Func->GRDFCMEnabled() && mIsPwmWaveTimeRunning) || (mIsRotSpeedPulseRunning)){
            mECNTCFisCM = true;
            mECNTCFCMEvent.notify((double)mATU5_TIMERF->mATU5->mCLK_LSBPeriod, mATU5_TIMERF->mATU5->mTimeResolutionUnit);
        }
    }
}

/***********************************
Function        : EnableReset
Description     : Reset is called
***********************************/
void ATU5_TIMERF_Sub::EnableReset(const bool is_active)
{//{{{
    mATU5_TIMERF_Sub_Func->Reset(is_active);
    mCounterC->EnableReset(is_active);
    mCounterA->EnableReset(is_active);
    mNoiseCancelerA->EnableReset(is_active);
    mNoiseCancelerB->EnableReset(is_active);

    if(is_active) {
        Initialize();
        CancelEvents();
    }
}//}}}

void ATU5_TIMERF_Sub::NotifyEvent(const unsigned int event) {
    switch(event) {
        case emECNTAFUpdated:
            mECNTAFUpdatedEvent.notify(SC_ZERO_TIME);
            mECNTAFUpdated = true;
            break;
        case emECNTCFUpdated:
            mECNTCFUpdatedEvent.notify(SC_ZERO_TIME);
            mECNTCFUpdated = true;
            break;
        case emGRAFUpdated:
            mGRAFUpdatedEvent.notify(SC_ZERO_TIME);
            mGRAFUpdated = true;
            break;
        case emGRBFUpdated:
            mGRBFUpdatedEvent.notify(SC_ZERO_TIME);
            mGRBFUpdated = true;
            break;
        case emGRDFUpdated:
            mGRDFUpdatedEvent.notify(SC_ZERO_TIME);
            mGRDFUpdated = true;
            break;
        case emTCR1FUpdated:
            SelectClockInput();
            break;
        default:
            break;
    }
}

void ATU5_TIMERF_Sub::ClearECNTBF_isIncreasedMethod(void) {
    mECNTBF_isIncreased = false;
}

void ATU5_TIMERF_Sub::ClearECNTAF_isIncreasedMethod(void) {
    mECNTAF_isIncreased = false;
}

void ATU5_TIMERF_Sub::SignalAisChangeMethod(void) {
    double current_time = sc_time_stamp().to_double();
    if(signalA.read()) {
        mAssertSignalA_syncEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
    }
    else {
        mDeassertSignalA_syncEvent.notify(mATU5_TIMERF->CalculateClockEdge("CLK_LSB", true, current_time,mATU5_TIMERF->mATU5->mTimeCLKChangePoint[0])+2*mATU5_TIMERF->mATU5->mCLK_LSBPeriod,mATU5_TIMERF->mATU5->mTimeResolutionUnit);
    }
}

void ATU5_TIMERF_Sub::AssertSignalA_syncMethod(void) {
    signalA_sync.write(true);
}

void ATU5_TIMERF_Sub::DeassertSignalA_syncMethod(void) {
    signalA_sync.write(false);
}
void ATU5_TIMERF_Sub::NotifyWriteMDFEvent(void) {
    mWriteMDFEvent.notify(SC_ZERO_TIME);
}

// vim600: set foldmethod=marker :
