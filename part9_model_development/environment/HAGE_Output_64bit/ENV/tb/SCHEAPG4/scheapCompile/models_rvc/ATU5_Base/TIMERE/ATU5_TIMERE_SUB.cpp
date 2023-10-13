// ---------------------------------------------------------------------
// $Id: ATU5_TIMERE_SUB.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_TIMERE_SUB.h"
#include "ATU5_TIMERE_SUB_Func.h"
#include "ATU5_Timer.h"
#include "ATU5_TIMERE_SUB.h"
#include "ATU5_TIMERE_SUB_Func.h"
#include "ATU5_Base.h"

///*********************************
/// Function     : ATU5_TIMERE_SUB
/// Description  : Constructor of ATU5_TIMERE_SUB class
///*********************************
ATU5_TIMERE_SUB::ATU5_TIMERE_SUB (sc_module_name name,
            ATU5_TIMERE *ATU5_TIMERE_Ptr,
            unsigned int subnum):
                sc_module(name),
                ATU5_TIMERE_SUB_AgentController(),
                PCLK("PCLK"),
                clock_bus0("clock_bus0"),
                clock_bus1("clock_bus1"),
                clock_bus2("clock_bus2"),
                clock_bus3("clock_bus3"),
                clock_bus4("clock_bus4"),
                clock_bus5("clock_bus5"),
                POE("POE")

{//{{{
    assert(ATU5_TIMERE_Ptr != NULL);
    mTIMERE = ATU5_TIMERE_Ptr;
    //Initialize variables - for clock, time resolution, reset

    std::ostringstream port_name;
    for (unsigned int index = 0; index < ATU5_TIMERE_SUB_AgentController::emChannel; index++) {
        port_name.str("");
        port_name << "TOE" << index;
        TOE[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TOE[index] != NULL);

        port_name.str("");
        port_name << "INTATUECMIE" << index;
        INTATUECMIE[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INTATUECMIE[index] != NULL);
    }
    //Bind clock, reset signal and target socket to Bus IF
   

    mTIE_SUBFunc = new ATU5_TIMERE_SUB_Func((std::string)name, this);
    sc_assert(mTIE_SUBFunc != NULL);
    
    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream name;
        name.str("");name << "mFreeRunCnt" << index;
        mFreeRunCnt[index] = new ATU5_Timer(name.str().c_str(), mTIMERE->mATU5);
        mFreeRunCnt[index]->SetCounterMode(true);// Set free-run mode
        mFreeRunCnt[index]->SetupGeneralTimer(false,true);
    }

    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream name;
        name.str("");name << "mRunCntDTRE" << index;
        mRunCntDTRE[index] = new ATU5_Timer(name.str().c_str(), mTIMERE->mATU5);
        mRunCntDTRE[index]->SetCounterMode(true);// Set free-run mode
        mRunCntDTRE[index]->SetupGeneralTimer(false,true); // not clear, count up
    }
    
    SC_METHOD(ShutOffRequestFlowMethod);
    dont_initialize();
    sensitive << POE << mShutOffRequestEvent;
    
    SC_METHOD(CheckClockInputMethod);
    dont_initialize();
    sensitive << clock_bus0
              << clock_bus1
              << clock_bus2
              << clock_bus3
              << clock_bus4
              << clock_bus5
              << PCLK
              << mChangeClockRequestEvent;
    
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mTOEChangeRequestEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::HandleTOEPortMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleTOEPortMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mPreTOEChangeRequestEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::AdjustTOEChangeEventMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("AdjustTOEChangeEventMethod"), &opt);
    }
    
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRisingInterruptEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::RisingInterruptMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("RisingInterruptMethod"), &opt);
    }
  
  
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFallingInterruptEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::FallingInterruptMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("FallingInterruptMethod"), &opt);
    }
  
  
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mFreeRunCnt[signal_id]->mOverflowEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::PreCmpOvfMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("PreCmpOvfMethod"), &opt);
    }
     for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCounterOverflowEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::HandleCmpOvfMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleCmpOvfMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mRunCntDTRE[signal_id]->mCompareMatchEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::PreCmpDTREMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("PreCmpDTREMethod"), &opt);
    }
    
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDutyMatchEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::HandleCmpDTREMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleCmpDTREMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mPostCycleMatchEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::PostCmpCYLRMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("PostCmpCYLRMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mFreeRunCnt[signal_id]->mCompareMatchEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::PreCmpCYLRMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("PreCmpCYLRMethod"), &opt);
    }
  
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCycleMatchEvent[signal_id]);
        opt.set_sensitivity(&mForceCycleMatchEvent[signal_id]);// second trigger
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::HandleCmpCYLRMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleCmpCYLRMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mPreForceCycleMatchEvent[signal_id]);// second trigger
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::AdjustForceCmpEventMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("AdjustForceCmpEventMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHandleFlagAndInterruptDTREEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::HandleFlagAndInterruptDTREMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleFlagAndInterruptDTREMethod"), &opt);
    }
 
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHandleFlagAndInterruptCYLREvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::HandleFlagAndInterruptCYLRMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleFlagAndInterruptCYLRMethod"), &opt);
    }
    
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mPreStartTimerEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::AdjustStartEventMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("AdjustStartEventMethod"), &opt);
    }
    
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStartTimerEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::StartTimerMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StartTimerMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStopTimerEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::StopTimerMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StopTimerMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRestartPWMCycleEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::RestartPWMCycleMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("RestartPWMCycleMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRewriteCounterEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::RewriteCounterMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("RewriteCounterMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcessSettingEvent[signal_id]);
        opt.set_sensitivity(&mProcessClearingEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERE_SUB::ProcessPriorityMethod, this, signal_id),
                sc_core::sc_gen_unique_name("ProcessPriorityMethod"), &opt);
    }
    
    mPCLKPeriod = 0;
    Initialize();
    CancelAllEvents();
    for (unsigned int index = 0; index < ATU5_TIMERE_SUB_AgentController::emChannel; index++) {
        TOE[index] -> initialize(false);
        INTATUECMIE[index] -> initialize(false);
    }
 }//}}}

///*********************************
/// Function     : ~ATU5_TIMERE_SUB
/// Description  : Destructor of ATU5_TIMERE_SUB class
///*********************************
ATU5_TIMERE_SUB::~ATU5_TIMERE_SUB (void)
{//{{{ 
    delete mTIE_SUBFunc;
    for (unsigned int index = 0; index <= 3; index++) {
        delete mFreeRunCnt[index];
        delete mRunCntDTRE[index];
    }
}//}}}
///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5_TIMERE_SUB::EnableReset (const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
        InitializePort();
        CancelAllEvents();
    }
    mTIE_SUBFunc->EnableReset(is_active);
   for (unsigned int index = 0; index <= 3; index++) {
        mFreeRunCnt[index]->EnableReset(is_active);
        mRunCntDTRE[index]->EnableReset(is_active); 
    }

}//}}}
void ATU5_TIMERE_SUB::Initialize (void)
{//{{{
    mSubBlockEnable = false;
    mSubBlockReloadEnable = true; // chuonghoangle revised 10/24/2018 for fixing APN chapter 2.14
    mCLKBUS0 = 0;
    mCLKBUS1 = 0;
    mCLKBUS2 = 0;
    mCLKBUS3 = 0;
    mCLKBUS4 = 0;
    mCLKBUS5 = 0;
    mCountClockStartPoint = 0;
    mPCLKClockStartPoint = 0;

    mIsTurnCMFE=false;
    mIsTurnOVFE=false;
    mIsTurnDMFE=false;
    for (unsigned int index = 0; index < ATU5_TIMERE_SUB_AgentController::emChannel; index++) {
        mIsTimerRunning[index] = false;

        mDTREValue[index] = 0xFFFFFF;
        mCYLREValue[index] = 0xFFFFFF;
        mTOEShutOffRequest[index] = false;
        mTOEClearRequest[index] = false;
        mTOESetRequest[index] = false;
        mTOEToggleRequest[index] = false;
        mChannelEnable[index] = false;
        
        mRegClearCounterRequest[index] = false;
        mClearCounter[index] = false;
        mCYLRCmpMatchBit[index] = false;
        mIsCounterRunningEnable[index] = false;
        mTimerEPrescaler[index] = false; 
        mChangeCountClkPeriod[index] = false;
        
        mChangeCounterValue[index] = false;
        mChangeDTREValue[index] = false;
        mChangeCYLRValue[index] = false;
        
        mFreeRunCnt[index]->StartStop(mIsTimerRunning[index]);
        mRunCntDTRE[index]->StartStop(mIsTimerRunning[index]); 
    
        mCountClkPeriod[index] = 0;
        mOvfTime[index] = 0;
        mCmpDTRETime[index] = 0;
        mIsOvf[index] = false;
        mIsCmpDTRE[index] = false;

        mIsClearDMFEStatus[index] = false;
        mIsClearCMFEStatus[index] = false;
        mIsClearOVFEStatus[index] = false;
        mIsForceCycleMatch[index] = false;
    }
    for (unsigned int index = 0; index < ATU5_TIMERE_SUB_AgentController::emAllClockBus; index++) { 
       mInputClockStartPoint[index] = 0;
    }
}//}}}
void ATU5_TIMERE_SUB::InitializePort (void)
{//{{{
    for (unsigned int index = 0; index < ATU5_TIMERE_SUB_AgentController::emChannel; index++) {
        TOE[index] -> write(false);
        INTATUECMIE[index] -> write(false);
    }
}//}}}
void ATU5_TIMERE_SUB::CancelAllEvents (void)
{//{{{
    mShutOffRequestEvent.cancel();
    mChangeClockRequestEvent.cancel();
    mProcessClearingEvent.cancel();
    for (unsigned int index = 0; index < ATU5_TIMERE_SUB_AgentController::emChannel; index++) {
        mTOEChangeRequestEvent[index].cancel();
        mRisingInterruptEvent[index].cancel();
        mFallingInterruptEvent[index].cancel();
   
        mHandleFlagAndInterruptDTREEvent[index].cancel();
        mHandleFlagAndInterruptCYLREvent[index].cancel();
        mDutyMatchEvent[index].cancel();
        mCycleMatchEvent[index].cancel();
        mCounterOverflowEvent[index].cancel();
        mPostCycleMatchEvent[index].cancel();
        
        mPreStartTimerEvent[index].cancel();
        mStartTimerEvent[index].cancel();
        mStopTimerEvent[index].cancel();
        mPreForceCycleMatchEvent[index].cancel();
        mForceCycleMatchEvent[index].cancel();
        mRewriteCounterEvent[index].cancel();
        mRestartPWMCycleEvent[index].cancel();
        mPreTOEChangeRequestEvent[index].cancel();
        mProcessSettingEvent[index].cancel();
    }
}//}}}

///*********************************
/// Function     : CheckClockInputMethod
/// Description  : Destructor of ATU5_TIMERE_SUB class
///*********************************
void ATU5_TIMERE_SUB::CheckClockInputMethod (void)
{//{{{ 
    HandleCountClockFunction();
 
}//}}}

/***********************************
Function        : HandleCountClockFunction
Description     : TSTRE set counter start
***********************************/
void ATU5_TIMERE_SUB::HandleCountClockFunction(void)
{//{{{
    unsigned int subblock_ratio = mTIE_SUBFunc->GetSubBlockClockRatio();
    unsigned int channel_ratio = 1;
    double current_time = GetCurTime();
    sc_dt::uint64 ClkPeriod;
    sc_dt::uint64 PreCountClkPeriod[ATU5_TIMERE_SUB_AgentController::emChannel];
    if (mPCLKPeriod != (sc_dt::uint64)PCLK.read()) {
        mPCLKPeriod = (sc_dt::uint64)PCLK.read();
        mPCLKClockStartPoint = current_time;
    }
    if (mCLKBUS0 != (sc_dt::uint64)clock_bus0.read()) {
        mCLKBUS0 = (sc_dt::uint64)clock_bus0.read();
        mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus0] = current_time;
    }
    if (mCLKBUS1 != (sc_dt::uint64)clock_bus1.read()) {
        mCLKBUS1 = (sc_dt::uint64)clock_bus1.read();
        mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus1] = current_time;
    }
    if (mCLKBUS2 != (sc_dt::uint64)clock_bus2.read()) {
        mCLKBUS2 = (sc_dt::uint64)clock_bus2.read();
        mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus2] = current_time;
    }
    if (mCLKBUS3 != (sc_dt::uint64)clock_bus3.read()) {
        mCLKBUS3 = (sc_dt::uint64)clock_bus3.read();
        mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus3] = current_time;
    }
    if (mCLKBUS4 != (sc_dt::uint64)clock_bus4.read()) {
        mCLKBUS4 = (sc_dt::uint64)clock_bus4.read();
        mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus4] = current_time;
    }
    if (mCLKBUS5 != (sc_dt::uint64)clock_bus5.read()) {
        mCLKBUS5 = (sc_dt::uint64)clock_bus5.read();
        mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus5] = current_time;
    }
    switch (subblock_ratio) {
    case 0:
        ClkPeriod = mCLKBUS0;
        mCountClockStartPoint = mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus0];
        break;
    case 1:
        ClkPeriod = mCLKBUS1;
        mCountClockStartPoint = mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus1];
        break;
    case 2:
        ClkPeriod = mCLKBUS2;
        mCountClockStartPoint = mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus2];
        break;
    case 3:
        ClkPeriod = mCLKBUS3;
        mCountClockStartPoint = mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus3];
        break;
    case 4:
        ClkPeriod = mCLKBUS4;
        mCountClockStartPoint = mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus4];
        break;
    case 5:
        ClkPeriod = mCLKBUS5;
        mCountClockStartPoint = mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus5];
        break;
    default:
        ClkPeriod = mCLKBUS0;
        mCountClockStartPoint = mInputClockStartPoint[ATU5_TIMERE_SUB_AgentController::emClockBus0];
        break;
    }
    for (unsigned int index = 0; index <= 3; index++) {
       channel_ratio = mTIE_SUBFunc->GetChannelClockRatio(index);
       PreCountClkPeriod[index] =  ClkPeriod * channel_ratio;
       if(PreCountClkPeriod[index] == 0){ //zero clock condition
          mCountClkPeriod[index] =  PreCountClkPeriod[index];
          if(mChangeCountClkPeriod[index]){
              mChangeCountClkPeriod[index] = false;
          }
          mStartTimerEvent[index].cancel();
          mCounterOverflowEvent[index].cancel();
          mDutyMatchEvent[index].cancel();
          mCycleMatchEvent[index].cancel();
          mTimerEPrescaler[index] = false;
          mPreStartTimerEvent[index].notify(SC_ZERO_TIME);
       } else if (mChangeCountClkPeriod[index]) { // change count clock by register
          mCountClkPeriod[index] =  PreCountClkPeriod[index];
          mChangeCountClkPeriod[index] = false;
          if (mIsTimerRunning[index]){
              mStartTimerEvent[index].cancel();
              mCounterOverflowEvent[index].cancel();
              mDutyMatchEvent[index].cancel();
              mCycleMatchEvent[index].cancel();
              mStopTimerEvent[index].notify(SC_ZERO_TIME);
              mStartTimerEvent[index].notify(CalculateClockEdge(index, true, true, current_time,mCountClockStartPoint),mTIMERE->mATU5->mTimeResolutionUnit);
          } else {
              mTimerEPrescaler[index] = true;
              mPreStartTimerEvent[index].notify(SC_ZERO_TIME);
          }
       } else if (PreCountClkPeriod[index] != mCountClkPeriod[index]) { // count clock change by input
          mCountClkPeriod[index] =  PreCountClkPeriod[index];
          if (mIsTimerRunning[index]){
              mStartTimerEvent[index].cancel();
              mCounterOverflowEvent[index].cancel();
              mDutyMatchEvent[index].cancel();
              mCycleMatchEvent[index].cancel();
              mStopTimerEvent[index].notify(SC_ZERO_TIME);
              mStartTimerEvent[index].notify(CalculateClockEdge(index, true, true, current_time,mCountClockStartPoint),mTIMERE->mATU5->mTimeResolutionUnit);
          } else { 
              mTimerEPrescaler[index] = true;
              mPreStartTimerEvent[index].notify(SC_ZERO_TIME);
          }
       }
    }
}//}}}

/***********************************
Function        : CalculateClockEdge
Description     : Calculate the Clock
***********************************/
double ATU5_TIMERE_SUB::CalculateClockEdge (unsigned int channel, const bool is_pos, const bool add_period, const double time_point,const double time_change_point)
{//{{{
    double period_num = 0;
    std::string clock_name="";
    switch(channel){
      case 0: clock_name = "CLOCKBUS0"; break;
      case 1: clock_name = "CLOCKBUS1"; break;
      case 2: clock_name = "CLOCKBUS2"; break;
      case 3: clock_name = "CLOCKBUS3"; break;
      case 4: clock_name = "PCLK"; break;
      default: clock_name = "CLOCKBUS0"; break;      
    }

    // Returns 0 if input clock is 0
    bool zero_clock = false;
    if ( (clock_name == "CLOCKBUS0") && (mCountClkPeriod[0] == 0) ) {
        zero_clock = true;
    } else if ( (clock_name == "CLOCKBUS1") && (mCountClkPeriod[1] == 0) ) {
        zero_clock = true;
    } else if ( (clock_name == "CLOCKBUS2") && (mCountClkPeriod[2] == 0) ) {
        zero_clock = true;
    } else if ( (clock_name == "CLOCKBUS3") && (mCountClkPeriod[3] == 0) ) {
        zero_clock = true;
    } else if ( (clock_name == "PCLK") && (mPCLKPeriod == 0) ) {
        zero_clock = true;
    }
    if (zero_clock) {
        return 0;
    }

    if (clock_name == "CLOCKBUS0") {
        period_num = (time_point - time_change_point) / (double)mCountClkPeriod[0];//temporary
    } else if (clock_name == "CLOCKBUS1") {
        period_num = (time_point - time_change_point) / (double)mCountClkPeriod[1];//temporary
    } else if (clock_name == "CLOCKBUS2") {
        period_num = (time_point - time_change_point) / (double)mCountClkPeriod[2];//temporary
    } else if (clock_name == "CLOCKBUS3") {
        period_num = (time_point - time_change_point) / (double)mCountClkPeriod[3];//temporary
    } else if (clock_name == "PCLK") {
        period_num = (time_point - time_change_point) / (double)mPCLKPeriod;//temporary
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
    if (clock_name == "CLOCKBUS0") {
        edge_point = mod_period_num_db * (double)mCountClkPeriod[0] - (time_point - time_change_point);
    } else if (clock_name == "CLOCKBUS1") {
        edge_point = mod_period_num_db * (double)mCountClkPeriod[1] - (time_point - time_change_point);
    } else if (clock_name == "CLOCKBUS2") {
        edge_point = mod_period_num_db * (double)mCountClkPeriod[2] - (time_point - time_change_point);
    } else if (clock_name == "CLOCKBUS3") {
        edge_point = mod_period_num_db * (double)mCountClkPeriod[3] - (time_point - time_change_point);
    } else if (clock_name == "PCLK") {
        edge_point = mod_period_num_db * (double)mPCLKPeriod - (time_point - time_change_point);
    }
    return edge_point;
}//}}}

/*********************************
// Function     : GetCounterValue
// Description  : Get the current value of TCNTE
// Return value : Current counter value
**********************************/
unsigned int ATU5_TIMERE_SUB::GetCntValue(const unsigned int channel)
{//{{{
    unsigned int value = 0;
    value = mFreeRunCnt[channel]->GetCounterValue();
    return value;
}//}}}

/*********************************
// Function     : AdjustStartEventMethod
// Description  : Start timer
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERE_SUB::AdjustStartEventMethod (const unsigned int channel)
{//{{{
    if (mChannelEnable[channel] && mSubBlockEnable && mTimerEPrescaler[channel]) {
        if (mIsCounterRunningEnable[channel] == false){
            mIsCounterRunningEnable[channel] = true;
        }
            double current_time = GetCurTime();
            mStartTimerEvent[channel].cancel();
            mStartTimerEvent[channel].notify(CalculateClockEdge(channel, true, false, current_time,mCountClockStartPoint),mTIMERE->mATU5->mTimeResolutionUnit);
    } else {
        if (mIsCounterRunningEnable[channel] == true){
            mIsCounterRunningEnable[channel] = false;
        }   
            mStartTimerEvent[channel].cancel();
            mStopTimerEvent[channel].notify(SC_ZERO_TIME);
    }
}//}}} 
/*********************************
// Function     : StartTimerMethod
// Description  : Start timer
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERE_SUB::StartTimerMethod (const unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {

        if (mCYLREValue[channel] < mDTREValue[channel]) { re_printf("warning", "The value in duty cycle setting must be equal to or less than the value in cycle-setting\n"); }
        unsigned int next_value = 0;
        unsigned int curr_value = 0;
        double current_time = GetCurTime();
        if (!mIsTimerRunning[channel]) {
            // set counter status bit
            mIsTimerRunning[channel] = true;
            // set count period
            mFreeRunCnt[channel]->SetCounterPeriod((double)mCountClkPeriod[channel]);
            mRunCntDTRE[channel]->SetCounterPeriod((double)mCountClkPeriod[channel]);

            //set timer size
            mFreeRunCnt[channel]->SetTimerSize(24);
            mRunCntDTRE[channel]->SetTimerSize(24);
            // set compare match value
            SetCntCmpValue(channel, mCYLREValue[channel]);
            SetCntDTRECmpValue(channel, mDTREValue[channel]);
            // set start count value
            if (mClearCounter[channel] || mCYLRCmpMatchBit[channel]) { // counter is clear by overflow
                if (mCYLRCmpMatchBit[channel]) {
                    mTIE_SUBFunc->SetTCNTERegValue(channel, 0x1);
                    mCYLRCmpMatchBit[channel] = false;
                    SetCntValue(channel, mTIE_SUBFunc->GetTCNTERegValue(channel));
                    SetCntDTREValue(channel, mTIE_SUBFunc->GetTCNTERegValue(channel));
                } else if (mClearCounter[channel]) {
                    mClearCounter[channel] = false;
                    mTIE_SUBFunc->SetTCNTERegValue(channel, 0x0);
                    SetCntValue(channel, 0x0);
                    SetCntDTREValue(channel, 0x0);
                }
                mClearCounter[channel] = false;
                //start counter
                mFreeRunCnt[channel]->StartStop(mIsTimerRunning[channel]);
                mRunCntDTRE[channel]->StartStop(mIsTimerRunning[channel]);

                if (mTIE_SUBFunc->GetTCNTERegValue(channel) == mDTREValue[channel]) {
                    mDutyMatchEvent[channel].notify((double)mCountClkPeriod[channel], mTIMERE->mATU5->mTimeResolutionUnit);
                }
                if (mTIE_SUBFunc->GetTCNTERegValue(channel) == mCYLREValue[channel]) {
                    mPostCycleMatchEvent[channel].notify(CalculateClockEdge(channel, false, false, current_time, mCountClockStartPoint), mTIMERE->mATU5->mTimeResolutionUnit);
                    mCycleMatchEvent[channel].notify((double)mCountClkPeriod[channel], mTIMERE->mATU5->mTimeResolutionUnit);
                }
            } else {
                curr_value = mTIE_SUBFunc->GetTCNTERegValue(channel);
                if (curr_value == 0xFFFFFF) {// user set FFFFFF
                    if (curr_value == mCYLREValue[channel]) {
                        next_value = 0x1;
                    } else {
                        next_value = 0x0;
                        mCounterOverflowEvent[channel].notify(SC_ZERO_TIME);
                    }
                } else {
                    next_value = mTIE_SUBFunc->GetTCNTERegValue(channel) + 1;
                }

                SetCntValue(channel, next_value);
                SetCntDTREValue(channel, next_value);
                //start counter
                mFreeRunCnt[channel]->StartStop(mIsTimerRunning[channel]);
                mRunCntDTRE[channel]->StartStop(mIsTimerRunning[channel]);

                if (next_value == mDTREValue[channel]) {
                    mDutyMatchEvent[channel].notify((double)mCountClkPeriod[channel], mTIMERE->mATU5->mTimeResolutionUnit);
                } else if (curr_value == mDTREValue[channel]) {
                    mDutyMatchEvent[channel].notify(SC_ZERO_TIME);
                }
                if (next_value == mCYLREValue[channel]) {
                    mPostCycleMatchEvent[channel].notify(CalculateClockEdge(channel, false, false, current_time, mCountClockStartPoint), mTIMERE->mATU5->mTimeResolutionUnit);
                    mCycleMatchEvent[channel].notify((double)mCountClkPeriod[channel], mTIMERE->mATU5->mTimeResolutionUnit);
                } else if (curr_value == mCYLREValue[channel]) {
                    mCycleMatchEvent[channel].notify(SC_ZERO_TIME);
                }
            }
        }
    }
}//}}} 

/*********************************
// Function     : StopTimerMethod
// Description  : Stop timer
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERE_SUB::StopTimerMethod (const unsigned int channel)
{//{{{
    if (mIsTimerRunning[channel]) {
        // set counter status bit
        mIsTimerRunning[channel] = false ;
        
        // get counter value
        mTIE_SUBFunc->SetTCNTERegValue(channel,GetCntValue(channel));
        
        // stop counter
        mFreeRunCnt[channel]->StartStop(mIsTimerRunning[channel]);
        mRunCntDTRE[channel]->StartStop(mIsTimerRunning[channel]);
    } 
}//}}} 
/*********************************
// Function     : RewriteCounterMethod
// Description  : Rewrite counter
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERE_SUB::RewriteCounterMethod (const unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
        double current_time = GetCurTime();
        if (mIsTimerRunning[channel]) {
            if (mChangeCounterValue[channel] == true) {
                // set counter status bit
                mIsTimerRunning[channel] = false;
                // stop counter
                mFreeRunCnt[channel]->StartStop(mIsTimerRunning[channel]);
                mRunCntDTRE[channel]->StartStop(mIsTimerRunning[channel]);
            } else {
                mStopTimerEvent[channel].notify(SC_ZERO_TIME);
            }
        }
        mStartTimerEvent[channel].cancel();
        mDutyMatchEvent[channel].cancel();
        mCycleMatchEvent[channel].cancel();
        if (mChangeCounterValue[channel] == true) {
            mCounterOverflowEvent[channel].cancel();
            mChangeDTREValue[channel] = false;
            mChangeCYLRValue[channel] = false;
            mCycleMatchEvent[channel].cancel();
            mDutyMatchEvent[channel].cancel();
            if (mClearCounter[channel] == true) {
                mClearCounter[channel] = false;
            }

        } else if (mChangeCounterValue[channel] == false) {
            if (mChangeDTREValue[channel]) {
                mChangeDTREValue[channel] = false;
                mDutyMatchEvent[channel].cancel();
            }
            if (mChangeCYLRValue[channel]) {
                mChangeCYLRValue[channel] = false;
                mCycleMatchEvent[channel].cancel();
            }
        }
        // start timer again
        if (mTIE_SUBFunc->GetTCNTERegValue(channel) == mCYLREValue[channel]) {
            mCounterOverflowEvent[channel].cancel();
            mClearCounter[channel] = false; // cycle match, omit ovf
        }
        if ((mChangeCounterValue[channel] == true) || (mTIE_SUBFunc->GetTCNTERegValue(channel) == mCYLREValue[channel])) {
            mChangeCounterValue[channel] = false;
            mStartTimerEvent[channel].notify(CalculateClockEdge(channel, true, false, current_time, mCountClockStartPoint), mTIMERE->mATU5->mTimeResolutionUnit);
        } else {
            mStartTimerEvent[channel].notify(CalculateClockEdge(channel, true, true, current_time, mCountClockStartPoint), mTIMERE->mATU5->mTimeResolutionUnit);
        }
    }
}//}}} 
/*********************************
// Function     : RestartPWMCycleMethod
// Description  : Restart PWM
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERE_SUB::RestartPWMCycleMethod (const unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
        double current_time = GetCurTime();
        if (mIsTimerRunning[channel]) {
            // set counter status bit
            mIsTimerRunning[channel] = false;
            // stop counter
            mFreeRunCnt[channel]->StartStop(mIsTimerRunning[channel]);
            mRunCntDTRE[channel]->StartStop(mIsTimerRunning[channel]);
        }
        if (mClearCounter[channel] == true) { //prevent clear counter by ovf
            mClearCounter[channel] = false;
        }
        mRegClearCounterRequest[channel] = true; //clear PWM cycle
        // restart cycle
        mStartTimerEvent[channel].cancel();
        mCycleMatchEvent[channel].cancel();
        mCounterOverflowEvent[channel].cancel();
        mDutyMatchEvent[channel].cancel();
        mCycleMatchEvent[channel].notify(CalculateClockEdge(channel, true, false, current_time, mCountClockStartPoint), mTIMERE->mATU5->mTimeResolutionUnit);
    }
}//}}} 
/*********************************
// Function     : Update TCNTE value
// Description  : Update TCNTE register Value
**********************************/
void ATU5_TIMERE_SUB::UpdateTCNTERegValue(unsigned int channel)
{//{{{
    mTIE_SUBFunc->SetTCNTERegValue(channel,GetCntValue(channel));
}//}}} 
/*********************************
// Function     : SetFreeRunCntValue
// Description  : Set Counter Value
**********************************/
void ATU5_TIMERE_SUB::SetCntValue(unsigned int channel, const unsigned int count)
{//{{{
    mFreeRunCnt[channel]->SetCounterInitialValue(count);
}//}}}
void ATU5_TIMERE_SUB::SetCntDTREValue(unsigned int channel, const unsigned int count)
{//{{{
    mRunCntDTRE[channel]->SetCounterInitialValue(count);
}//}}} 

/*********************************
// Function     : SetCmpMatValue
// Description  : Handle Compare match
**********************************/
void ATU5_TIMERE_SUB::SetCntCmpValue(unsigned int channel, unsigned int value)
{//{{{ 
      mFreeRunCnt[channel]->SetCompareMatchValue(value);
}//}}}
void ATU5_TIMERE_SUB::SetCntDTRECmpValue(unsigned int channel, unsigned int value)
{//{{{ 
      mRunCntDTRE[channel]->SetCompareMatchValue(value);
}//}}}

/*********************************
// Function     : PreCmpOvfMethod
// Description  : Handle Overflow
**********************************/
void ATU5_TIMERE_SUB::PreCmpOvfMethod(const unsigned int channel)
{//{{{
    if ((mOvfTime[channel] == sc_time_stamp().to_double()) && (mIsOvf[channel])) {
        return;
    } else {
        mOvfTime[channel] = sc_time_stamp().to_double();
        mIsOvf[channel] = true;
        if((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB")&& mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) { //Check Clock & reset
            mIsTimerRunning[channel] = false ;
            mFreeRunCnt[channel]->StartStop(mIsTimerRunning[channel]);
            mRunCntDTRE[channel]->StartStop(mIsTimerRunning[channel]);
            // clear counter to 1
            mTIE_SUBFunc->SetTCNTERegValue(channel,0xFFFFFF);
            mClearCounter[channel] = true;
            mStartTimerEvent[channel].cancel();
            mCounterOverflowEvent[channel].notify((double)mCountClkPeriod[channel],mTIMERE->mATU5->mTimeResolutionUnit); 
        }
    }
}//}}}
/*********************************
// Function     : HandleCmpOvfMethod
// Description  : Handle Overflow
**********************************/
void ATU5_TIMERE_SUB::HandleCmpOvfMethod(const unsigned int channel)
{//{{{
    if((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB")&& mTIMERE->mATU5->CheckClockPeriod("CLK_HSB"))   { //Check Clock & reset
        mIsTurnOVFE=true;
        mProcessSettingEvent[channel].notify(SC_ZERO_TIME);
        mTIE_SUBFunc->SetTCNTERegValue(channel,0x0);
        mStartTimerEvent[channel].cancel();
        mStartTimerEvent[channel].notify(SC_ZERO_TIME);//already align count clock
    }
}//}}}
/*********************************
// Function     : PreCmpDTREMethod
// Description  : Handle Compare Event
**********************************/
void ATU5_TIMERE_SUB::PreCmpDTREMethod(unsigned int channel)
{//{{{
    if ((mCmpDTRETime[channel] == sc_time_stamp().to_double()) && (mIsCmpDTRE[channel])) {
        return;
    } else {
        mCmpDTRETime[channel] = sc_time_stamp().to_double();
        mIsCmpDTRE[channel] = true;
        if((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB")&& mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
            mRunCntDTRE[channel]->ClearBitCMF();
            mDutyMatchEvent[channel].notify((double)mCountClkPeriod[channel],mTIMERE->mATU5->mTimeResolutionUnit); 
        }
    }
}//}}}
/*********************************
// Function     : HandleCmpDTREMethod
// Description  : Handle Compare Event
**********************************/
void ATU5_TIMERE_SUB::HandleCmpDTREMethod(unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
        // change port
        mTOEClearRequest[channel] = true;
        mTOEChangeRequestEvent[channel].notify(SC_ZERO_TIME);
        // flags and interrupts
        mHandleFlagAndInterruptDTREEvent[channel].notify(SC_ZERO_TIME);
    }
}//}}}
/*********************************
// Function     : AdjustForceCmpEventMethod
// Description  : Handle Compare Event
**********************************/
void ATU5_TIMERE_SUB::AdjustForceCmpEventMethod(unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
        double current_time = GetCurTime();
        mCounterOverflowEvent[channel].cancel();
        mClearCounter[channel] = false;
        mIsForceCycleMatch[channel] = true;
        mForceCycleMatchEvent[channel].notify(CalculateClockEdge(channel, true, false, current_time, mCountClockStartPoint), mTIMERE->mATU5->mTimeResolutionUnit);
    }
}//}}}
/*********************************
// Function     : PreCmpCYLRMethod
// Description  : Handle Compare Event
**********************************/
void ATU5_TIMERE_SUB::PreCmpCYLRMethod(unsigned int channel)
{//{{{
    if (!mFreeRunCnt[channel]->GetBitOVF() || (mCYLREValue[channel] == 0xFFFFFFFF)) {
        if((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB")&& mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
            double current_time = GetCurTime();
            mFreeRunCnt[channel]->ClearBitCMF();
            mPostCycleMatchEvent[channel].notify(CalculateClockEdge(channel, false, false, current_time,mCountClockStartPoint),mTIMERE->mATU5->mTimeResolutionUnit);
            mCycleMatchEvent[channel].notify((double)mCountClkPeriod[channel],mTIMERE->mATU5->mTimeResolutionUnit); 
        }
    }
    mFreeRunCnt[channel]->ClearBitOVF();
}//}}}
/*********************************
// Function     : PostCmpCYLRMethod
// Description  : Handle Compare Event
**********************************/
void ATU5_TIMERE_SUB::PostCmpCYLRMethod(unsigned int channel)
{//{{{
     mClearCounter[channel] = false;
     mCounterOverflowEvent[channel].cancel();
}//}}}
/*********************************
// Function     : HandleCmpCYLRMethod
// Description  : Handle Compare Event
**********************************/
void ATU5_TIMERE_SUB::HandleCmpCYLRMethod(const unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
        if (mRegClearCounterRequest[channel] == false) {//clearing by write 0 to TCNTE
            // stop counter
            if (mIsTimerRunning[channel]) {
                mIsTimerRunning[channel] = false;
                mFreeRunCnt[channel]->StartStop(mIsTimerRunning[channel]);
                mRunCntDTRE[channel]->StartStop(mIsTimerRunning[channel]);
            }
            // clear counter to 1
            mTIE_SUBFunc->SetTCNTERegValue(channel, 1);
            SetCntValue(channel, 1);
            SetCntDTREValue(channel, 1);
            // trigger flag/interrupt method
            mCYLRCmpMatchBit[channel] = true;
            mHandleFlagAndInterruptCYLREvent[channel].notify(SC_ZERO_TIME);
        }
        if (mIsForceCycleMatch[channel]) {
            if (mDTREValue[channel] != 0) {
                mTOESetRequest[channel] = true;
                mTOEChangeRequestEvent[channel].notify(SC_ZERO_TIME);
            } else {
                mTOEClearRequest[channel] = true;
                mTOEChangeRequestEvent[channel].notify(SC_ZERO_TIME);
            }
            mIsForceCycleMatch[channel] = false;
        }
        // reload DTRE, CYLR
        if (mTIE_SUBFunc->IsReloadEnable(channel) && mSubBlockReloadEnable) {
            mTIE_SUBFunc->UpdateDTREReg(channel);
            mTIE_SUBFunc->UpdateCYLREReg(channel);
        }
        // checking shutoff request
        mStartTimerEvent[channel].cancel();
        if (mSubBlockEnable && mChannelEnable[channel]) {
            bool ShutOffIn = (bool)POE.read();
            if ((ShutOffIn == mTIE_SUBFunc->GetShutOffActiveInputLevel())
                && (mTIE_SUBFunc->IsShutOffEnable(channel))
                && (mDTREValue[channel] >= 1)) {
                // call shut off method
                mShutOffRequestEvent.notify(SC_ZERO_TIME);
            } else if (mDTREValue[channel] >= 1) {
                //change port
                if (mRegClearCounterRequest[channel] == false) { //Clear TCNTE not change TOE
                    mTOESetRequest[channel] = true;
                    mTOEChangeRequestEvent[channel].notify(SC_ZERO_TIME);
                }
            }
            mStartTimerEvent[channel].notify(SC_ZERO_TIME);//already align count clock
        }
        if (mRegClearCounterRequest[channel] == true) {//clearing by write 0 to TCNTE
            mRegClearCounterRequest[channel] = false;
        }
    }
}//}}}
/*********************************
// Function     : HandleFlagAndInterruptCYLRMethod
// Description  : Handle Turn Flag and Interrupt of GRC
**********************************/
void ATU5_TIMERE_SUB::HandleFlagAndInterruptCYLRMethod(const unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
        if (mTIE_SUBFunc->IsInterruptCMEE(channel) && !(mTIE_SUBFunc->GetCMFE(channel))) {
            mRisingInterruptEvent[channel].notify(SC_ZERO_TIME);
        }
        if (!(mTIE_SUBFunc->GetCMFE(channel))) {
            mIsTurnCMFE = true;
            mProcessSettingEvent[channel].notify(SC_ZERO_TIME);
        }
    }
}//}}}

/*********************************
// Function     : HandleFlagAndInterruptDTREMethod
// Description  : Handle Turn Flag and Interrupt of OCRC
**********************************/
void ATU5_TIMERE_SUB::HandleFlagAndInterruptDTREMethod(const unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
        if (mTIE_SUBFunc->IsInterruptDMEE(channel) && !(mTIE_SUBFunc->GetDMFE(channel))) {
            mRisingInterruptEvent[channel].notify(SC_ZERO_TIME);
        }
        if (!(mTIE_SUBFunc->GetDMFE(channel))) {
            mIsTurnDMFE = true;
            mProcessSettingEvent[channel].notify(SC_ZERO_TIME);
        }
    }
}//}}}
/***********************************
Function        : GetCurTime
Description     : Get the current time
***********************************/
double ATU5_TIMERE_SUB::GetCurTime(void)
{//{{{
    return sc_time_stamp().to_double();
}//}}}

/*********************************
// Function     : RisingInterruptMethod
// Description  : Rising the interrupt
// Return value : None
**********************************/
void ATU5_TIMERE_SUB::RisingInterruptMethod(const unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {      //Check reset/clock condition
        INTATUECMIE[channel]->write(true);
        mFallingInterruptEvent[channel].notify((double)mPCLKPeriod, mTIMERE->mATU5->mTimeResolutionUnit); //after 1 PCLK
    }
}//}}}

/*********************************
// Function     : FallingInterruptMethod
// Description  : Falling the interrupt
// Return value : None
**********************************/
void ATU5_TIMERE_SUB::FallingInterruptMethod(const unsigned int channel)
{//{{{
     if((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB")&& mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) { //Check reset/clock condition
        INTATUECMIE[channel]->write(false);
    }
}//}}}
/*********************************
// Function     : AdjustTOEChangeEventMethod
// Description  : Handle TOE port output
**********************************/
void ATU5_TIMERE_SUB::AdjustTOEChangeEventMethod(unsigned int channel)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
        double current_time = GetCurTime();
        mTOEChangeRequestEvent[channel].notify(CalculateClockEdge(4, true, false, current_time, mPCLKClockStartPoint), mTIMERE->mATU5->mTimeResolutionUnit);
    }
}//}}}
/*********************************
// Function     : HandleTOEPortMethod
// Description  : Handle TOE port output
**********************************/
void ATU5_TIMERE_SUB::HandleTOEPortMethod(unsigned int channel)
{//{{{
     if((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif")))&& mTIMERE->mATU5->CheckClockPeriod("CLK_LSB")&& mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {//Check reset
        bool port_curr =  TOE[channel]->read();
        if (mTOEShutOffRequest[channel]) {
            TOE[channel]->write(mTIE_SUBFunc->GetShutOffOutputLevel(channel));
        } else if (mTOESetRequest[channel]) {
            if(mTIE_SUBFunc->CheckTOEInversionSetting(channel) == false) {
                TOE[channel]->write(true);            
            } else {
                TOE[channel]->write(false);
            }
        } else if (mTOEClearRequest[channel]) {
            if(mTIE_SUBFunc->CheckTOEInversionSetting(channel) == false)  {
                TOE[channel]->write(false);            
            } else {
                TOE[channel]->write(true);
            }
        } else if (mTOEToggleRequest[channel]) {
            TOE[channel]->write(!port_curr);
        } else {
        }
       mTOEShutOffRequest[channel] = false;
       mTOEClearRequest[channel] = false;
       mTOESetRequest[channel] = false;
       mTOEToggleRequest[channel] = false;
    }
}//}}}
/*********************************
// Function     : ShutOffRequestFlowMethod
// Description  : Handle Shut Off Request
**********************************/
void ATU5_TIMERE_SUB::ShutOffRequestFlowMethod(void)
{//{{{
    if ((!(mTIMERE->mATU5->GetResetStatus("presetz"))) && (!(mTIMERE->mATU5->GetResetStatus("presetz_bif"))) && mTIMERE->mATU5->CheckClockPeriod("CLK_LSB") && mTIMERE->mATU5->CheckClockPeriod("CLK_HSB")) {
        bool ShutOffIn = (bool)POE.read();
        bool CheckCompareCounter = false;
        if (ShutOffIn == mTIE_SUBFunc->GetShutOffActiveInputLevel()) {
            for (unsigned int channel = 0; channel <= 3; channel++) {
                CheckCompareCounter = false;
                if (mIsTimerRunning[channel]) {
                    if (GetCntValue(channel) < mDTREValue[channel]) {
                        CheckCompareCounter = true;
                    }
                } else {
                    if (mTIE_SUBFunc->GetTCNTERegValue(channel) < mDTREValue[channel]) {
                        CheckCompareCounter = true;
                    }
                }
                if ((mTIE_SUBFunc->IsShutOffEnable(channel)) && mIsCounterRunningEnable[channel] && CheckCompareCounter) {
                    mTOEShutOffRequest[channel] = true;
                    mTOEChangeRequestEvent[channel].notify(SC_ZERO_TIME);
                }
            }
        }
    }
}//}}}
/*********************************
// Function     : ProcessPriorityMethod
// Description  : Handle priority when set and clear competed,
//                clear is given priority to
**********************************/
void ATU5_TIMERE_SUB::ProcessPriorityMethod (const unsigned int channel)
{//{{{
    if(!mIsClearDMFEStatus[channel] && mIsTurnDMFE){
        mTIE_SUBFunc->TurnDMFEFlag(channel);
    }
    if(!mIsClearOVFEStatus[channel] && mIsTurnOVFE){
        mTIE_SUBFunc->TurnOVFEFlag(channel);
    }
    if(!mIsClearCMFEStatus[channel] && mIsTurnCMFE){
        mTIE_SUBFunc->TurnCMFEFlag(channel);
    }
    mIsClearDMFEStatus[channel]=false;
    mIsClearOVFEStatus[channel]=false;
    mIsClearCMFEStatus[channel]=false;
    mIsTurnCMFE=false;
    mIsTurnOVFE=false;
    mIsTurnDMFE=false;
}//}}}

void ATU5_TIMERE_SUB::NotifyPreStartTimerEvent(const unsigned int index)
{
    mPreStartTimerEvent[index].notify(SC_ZERO_TIME);
}
void ATU5_TIMERE_SUB::NotifyRewriteCounterEvent(const unsigned int index)
{
    mRewriteCounterEvent[index].notify(SC_ZERO_TIME);
}
void ATU5_TIMERE_SUB::NotifyPreForceCycleMatchEvent(const unsigned int index)
{
    mPreForceCycleMatchEvent[index].notify(SC_ZERO_TIME);
}
void ATU5_TIMERE_SUB::NotifyRestartPWMCycleEvent(const unsigned int index)
{
    mRestartPWMCycleEvent[index].notify(SC_ZERO_TIME);
}
void ATU5_TIMERE_SUB::NotifyPreTOEChangeRequestEvent(const unsigned int index)
{
    mPreTOEChangeRequestEvent[index].notify(SC_ZERO_TIME);
}
void ATU5_TIMERE_SUB::NotifyChangeClockRequestEvent(void)
{
    mChangeClockRequestEvent.notify(SC_ZERO_TIME);
}
void ATU5_TIMERE_SUB::NotifyProcessClearingEvent(void)
{
    mProcessClearingEvent.notify(SC_ZERO_TIME);
}
// vim600: set foldmethod=marker :
