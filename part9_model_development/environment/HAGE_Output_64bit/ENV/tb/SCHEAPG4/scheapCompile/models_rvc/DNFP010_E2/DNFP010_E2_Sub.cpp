// ---------------------------------------------------------------------
// $Id: DNFP010_E2_Sub.cpp 5605 2017-03-31 06:00:48Z ChinhTD $
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
#include "DNFP010_E2_Sub.h"
#include "DNFP010_E2_Sub_Func.h"
#include "DNFP010_E2_AgentController.h"


///Constructor of DNFP010_E2_Sub class
DNFP010_E2_Sub::DNFP010_E2_Sub (sc_module_name name,
                        DNFP010_E2_AgentController *DNFP010_E2_AgentController_Ptr,
                        unsigned int ch_id):
    sc_module(name),
    DNFP010_E2_Sub_AgentController(),
    // Input ports
    DNFIN("DNFIN"),

    // Output ports
    DNFFSOUT("DNFFSOUT"),
    DNFFAOUT("DNFFAOUT")
{//{{{

    assert(DNFP010_E2_AgentController_Ptr != NULL);
    mDNFP010_E2_AgentController = DNFP010_E2_AgentController_Ptr;
    mModuleName = (std::string)name;
    mChId = ch_id;

    mDNFP010_E2_Sub_Func = new DNFP010_E2_Sub_Func((std::string)name, this);
    sc_assert(mDNFP010_E2_Sub_Func != NULL);



    // Initialize variables & output ports
    mTimePRSChangePoint = 0;
    Initialize();

    // Declare method/thread
    ///Input Handle Methods
    SC_METHOD(HandleDNFINMethod);
    dont_initialize();
    sensitive << DNFIN;

    SC_METHOD(AssertDNFIN_sync0Method);
    dont_initialize();
    sensitive << mAssertDNFIN_sync0Event;

    SC_METHOD(DeassertDNFIN_sync0Method);
    dont_initialize();
    sensitive << mDeassertDNFIN_sync0Event;

    SC_METHOD(AssertDNFIN_syncMethod);
    dont_initialize();
    sensitive << mAssertDNFIN_syncEvent;

    SC_METHOD(DeassertDNFIN_syncMethod);
    dont_initialize();
    sensitive << mDeassertDNFIN_syncEvent;

    SC_METHOD(Assertnoise_eliminated_sigMethod);
    dont_initialize();
    sensitive << mAssertnoise_eliminated_sigEvent;

    SC_METHOD(Deassertnoise_eliminated_sigMethod);
    dont_initialize();
    sensitive << mDeassertnoise_eliminated_sigEvent;

    SC_METHOD(WriteDNFFSOUTMethod);
    dont_initialize();
    sensitive << mWriteDNFFSOUTEvent;

    SC_METHOD(WriteDNFFAOUTMethod);
    dont_initialize();
    sensitive << mWriteDNFFAOUTEvent;

    SC_METHOD(WriteDNFIN_syncMethod);
    dont_initialize();
    sensitive << mWriteDNFIN_sync_sigEvent;

    SC_METHOD(Writenoise_eliminated_sigMethod);
    dont_initialize();
    sensitive << mWritenoise_eliminated_sigEvent;

    SC_METHOD(HandleNFENchangedMethod);
    dont_initialize();
    sensitive << mNFENchangedEvent;

    SC_METHOD(CheckDNFIN_syncLevelInActiveClockMethod);
    dont_initialize();
    sensitive << mCheckDNFIN_syncLevelInActiveClockEvent;

    SC_METHOD(CheckChangeStateToIdleMethod);
    dont_initialize();
    sensitive << mCheckChangeStateToIdleEvent;

    SC_METHOD(NRMStateMachineMethod);
    dont_initialize();
    sensitive << DNFIN_sync_sig;
}//}}}

///Destructor of DNFP010_E2_Sub class
DNFP010_E2_Sub::~DNFP010_E2_Sub (void)
{//{{{
    delete mDNFP010_E2_Sub_Func;
}//}}}


/// Cancel operation events
void DNFP010_E2_Sub::CancelEvents (void)
{//{{{
    mAssertDNFIN_sync0Event.cancel();
    mDeassertDNFIN_sync0Event.cancel();
    mAssertDNFIN_syncEvent.cancel();
    mDeassertDNFIN_syncEvent.cancel();

    mAssertnoise_eliminated_sigEvent.cancel();
    mDeassertnoise_eliminated_sigEvent.cancel();

    mWriteDNFFSOUTEvent.cancel();
    mWriteDNFFAOUTEvent.cancel();

    mWritenoise_eliminated_sigEvent.cancel();
    mWriteDNFIN_sync_sigEvent.cancel();

    mNFENchangedEvent.cancel();
    mCheckDNFIN_syncLevelInActiveClockEvent.cancel();
    mCheckChangeStateToIdleEvent.cancel();
}//}}}

/// Initialize variables & output ports
void DNFP010_E2_Sub::Initialize (void)
{//{{{
    //Initialize variables
    mNRMstate = 0;
    mTimeAssertNRMPoint = 0;
    mTimeDeassertNRMPoint = 0;
    mTimeAssertDNFIN_syncPoint = 0;
    mTimeDeassertDNFIN_syncPoint = 0;
    mDNFIN_value = false;
    mDNFIN_sync_sig_value = false;
    mnoise_eliminated_sig_value = false;
    mWriteDNFIN_sync_sigEvent.notify(SC_ZERO_TIME);
    mWritenoise_eliminated_sigEvent.notify(SC_ZERO_TIME);

    mDNFFSOUT_value = false;
    mDNFFAOUT_value = false;

    // Initialize output ports
    mWriteDNFFSOUTEvent.notify(SC_ZERO_TIME);
    mWriteDNFFAOUTEvent.notify(SC_ZERO_TIME);
}//}}}

/***********************************
Function        : EnableReset
Description     : Reset is called
***********************************/
void DNFP010_E2_Sub::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if(reset_name=="PRESETn") {
        mDNFP010_E2_Sub_Func->Reset(is_active);
    }
    else if(is_active) {
        Initialize();
        CancelEvents();
    }
}//}}}

void DNFP010_E2_Sub::HandleDNFINMethod (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk")) {
        if(mDNFIN_value != DNFIN.read()) {
            mDNFIN_value = DNFIN.read();
            double cur_timer = sc_time_stamp().to_double();
            double syn_time = mDNFP010_E2_AgentController->CalculateNextClockEdge((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod),true,cur_timer,mDNFP010_E2_AgentController->mTimeCLKChangePoint[1]);
            if(mDNFIN_value) {
                mAssertDNFIN_sync0Event.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
            }
            else {
                mDeassertDNFIN_sync0Event.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
            }
            if(!GetNFEN()){
                mDNFFAOUT_value = DNFIN.read();
                mWriteDNFFAOUTEvent.notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

void DNFP010_E2_Sub::AssertDNFIN_sync0Method (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk")) {
        if(mDNFIN_value) {
            double cur_timer = sc_time_stamp().to_double();
            double syn_time = mDNFP010_E2_AgentController->CalculateNextClockEdge((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod),true,cur_timer,mDNFP010_E2_AgentController->mTimeCLKChangePoint[1])+(double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod);
            mTimeAssertDNFIN_syncPoint = cur_timer + syn_time;
            mAssertDNFIN_syncEvent.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
        }
    }
}//}}}

void DNFP010_E2_Sub::DeassertDNFIN_sync0Method (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk")) {
        if(!mDNFIN_value) {
            double cur_timer = sc_time_stamp().to_double();
            double syn_time = mDNFP010_E2_AgentController->CalculateNextClockEdge((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod),true,cur_timer,mDNFP010_E2_AgentController->mTimeCLKChangePoint[1])+(double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod);
            mTimeDeassertDNFIN_syncPoint = cur_timer + syn_time;
            mDeassertDNFIN_syncEvent.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
        }
    }
}//}}}

void DNFP010_E2_Sub::AssertDNFIN_syncMethod (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk")) {
        mDNFIN_sync_sig_value = true;
        mWriteDNFIN_sync_sigEvent.notify(SC_ZERO_TIME);
        mTimeAssertDNFIN_syncPoint = 0;
    }
}//}}}

void DNFP010_E2_Sub::DeassertDNFIN_syncMethod (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk")) {
        mDNFIN_sync_sig_value = false;
        mWriteDNFIN_sync_sigEvent.notify(SC_ZERO_TIME);
        mTimeDeassertDNFIN_syncPoint = 0;
    }
}//}}}

void DNFP010_E2_Sub::WriteDNFIN_syncMethod (void)
{//{{{
    DNFIN_sync_sig.write(mDNFIN_sync_sig_value);
    if(!GetNFEN()){
        mDNFFSOUT_value = mDNFIN_sync_sig_value;
        mWriteDNFFSOUTEvent.notify(SC_ZERO_TIME);
    }
}//}}}

void DNFP010_E2_Sub::Assertnoise_eliminated_sigMethod (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk") && GetNFEN()) {
        mnoise_eliminated_sig_value = true;
        mWritenoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
        mCheckChangeStateToIdleEvent.notify(1,mDNFP010_E2_AgentController->mTimeResolutionUnit);
    }
}//}}}

void DNFP010_E2_Sub::Deassertnoise_eliminated_sigMethod (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk") && GetNFEN()) {
        mnoise_eliminated_sig_value = false;
        mWritenoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
        mCheckChangeStateToIdleEvent.notify(1,mDNFP010_E2_AgentController->mTimeResolutionUnit);
    }
}//}}}

void DNFP010_E2_Sub::Writenoise_eliminated_sigMethod (void)
{//{{{
    noise_eliminated_sig.write(mnoise_eliminated_sig_value);
    if(GetNFEN()){
        mDNFFSOUT_value = mnoise_eliminated_sig_value;
        mDNFFAOUT_value = mnoise_eliminated_sig_value;
        mWriteDNFFSOUTEvent.notify(SC_ZERO_TIME);
        mWriteDNFFAOUTEvent.notify(SC_ZERO_TIME);
    }
}//}}}

void DNFP010_E2_Sub::WriteDNFFSOUTMethod (void)
{//{{{
    DNFFSOUT.write(mDNFFSOUT_value);
}//}}}

void DNFP010_E2_Sub::WriteDNFFAOUTMethod (void)
{//{{{
    DNFFAOUT.write(mDNFFAOUT_value);
}//}}}

bool DNFP010_E2_Sub::GetNFEN(void)
{//{{{
    return mDNFP010_E2_Sub_Func->GetNFEN();
}//}}}

unsigned int DNFP010_E2_Sub::GetSLST(void)
{//{{{
    re_printf("info"," Sampling count = %d\n",mDNFP010_E2_Sub_Func->GetSLST());
    return mDNFP010_E2_Sub_Func->GetSLST();
}//}}}

unsigned int DNFP010_E2_Sub::GetPRS(void)
{//{{{
    re_printf("info","DNF input clock is divided by %d\n",mDNFP010_E2_Sub_Func->GetPRS());
    return mDNFP010_E2_Sub_Func->GetPRS();
}//}}}

void DNFP010_E2_Sub::SetTimePRSChangePoint (void)
{//{{{
    double cur_timer = sc_time_stamp().to_double();
    double time_to_next_redge = mDNFP010_E2_AgentController->CalculateNextClockEdge((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod),true,cur_timer,mDNFP010_E2_AgentController->mTimeCLKChangePoint[1]);
    if(time_to_next_redge==0){
        time_to_next_redge = (double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod);
    }
    mTimePRSChangePoint = sc_time_stamp().to_double() + time_to_next_redge;
}//}}}

void DNFP010_E2_Sub::NotifyEvent (void)
{//{{{
    mNFENchangedEvent.notify(SC_ZERO_TIME);
}//}}}

void DNFP010_E2_Sub::HandleNFENchangedMethod (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk")) {
        if(GetNFEN()){
            mDNFFSOUT_value = noise_eliminated_sig.read();
            mDNFFAOUT_value = noise_eliminated_sig.read();
            mWriteDNFFSOUTEvent.notify(SC_ZERO_TIME);
            mWriteDNFFAOUTEvent.notify(SC_ZERO_TIME);
        }
        else{
            mDNFFAOUT_value = DNFIN.read();
            mWriteDNFFAOUTEvent.notify(SC_ZERO_TIME);
            mDNFFSOUT_value = DNFIN_sync_sig.read();
            mWriteDNFFSOUTEvent.notify(SC_ZERO_TIME);

            mNRMstate = 0;
            mAssertnoise_eliminated_sigEvent.cancel();
            mDeassertnoise_eliminated_sigEvent.cancel();
            mnoise_eliminated_sig_value = false;
            mWritenoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

void DNFP010_E2_Sub::CheckDNFIN_syncLevelInActiveClockMethod (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk") && GetNFEN()) {
        double cur_timer = sc_time_stamp().to_double();
        if(mNRMstate==1){
            if((mTimeAssertDNFIN_syncPoint==0 && !mDNFIN_sync_sig_value) || (mTimeAssertDNFIN_syncPoint!=0 && cur_timer<mTimeAssertDNFIN_syncPoint)) {
                mAssertnoise_eliminated_sigEvent.cancel();
                mNRMstate = 0;
            }
            else if(cur_timer==mTimeAssertNRMPoint){
                mAssertnoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
            }
        }
        else if(mNRMstate==2){
            if((mTimeDeassertDNFIN_syncPoint==0 && mDNFIN_sync_sig_value) || (mTimeDeassertDNFIN_syncPoint!=0 && cur_timer<mTimeDeassertDNFIN_syncPoint)) {
                mDeassertnoise_eliminated_sigEvent.cancel();
                mNRMstate = 0;
            }
            else if(cur_timer==mTimeDeassertNRMPoint){
                mDeassertnoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

void DNFP010_E2_Sub::CheckChangeStateToIdleMethod (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk") && GetNFEN()) {
        if((DNFIN_sync_sig.read()==1 && mNRMstate==1 && noise_eliminated_sig.read()==1) || (DNFIN_sync_sig.read()==0 && mNRMstate==2 && noise_eliminated_sig.read()==0)){
            mNRMstate = 0;
        }
    }
}//}}}

void DNFP010_E2_Sub::NRMStateMachineMethod (void)
{//{{{
    if (!mDNFP010_E2_AgentController->GetResetStatus("dnfp01tsmpresn") && mDNFP010_E2_AgentController->CheckClockPeriod("dnfp01tsmpclk") && GetNFEN()) {
        if(GetPRS()!=0) {
            double cur_timer = sc_time_stamp().to_double();
            if(mNRMstate==0) {
                if(DNFIN_sync_sig.read()==1) {
                    mNRMstate = 1;
                    double syn_time = mDNFP010_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint)+((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()*(GetSLST()-1));
                    mTimeAssertNRMPoint = cur_timer + syn_time;
                    mAssertnoise_eliminated_sigEvent.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
                }
                else if(DNFIN_sync_sig.read()==0) {
                    mNRMstate = 2;
                    double syn_time = mDNFP010_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint)+((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()*(GetSLST()-1));
                    mTimeDeassertNRMPoint = cur_timer + syn_time;
                    mDeassertnoise_eliminated_sigEvent.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
                }
            }
            else if(mNRMstate==1) {
                if(DNFIN_sync_sig.read()==0) {
                    if(cur_timer==mTimeAssertNRMPoint){
                        mNRMstate = 2;
                        double syn_time = mDNFP010_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint)+((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()*(GetSLST()-1));
                        mTimeDeassertNRMPoint = cur_timer + syn_time;
                        mDeassertnoise_eliminated_sigEvent.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
                    }
                    else {
                        double syn_time = mDNFP010_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint);
                        if((cur_timer+syn_time)==mTimeAssertNRMPoint) {
                            mAssertnoise_eliminated_sigEvent.cancel();
                        }
                        if(syn_time==0) {
                            syn_time = syn_time + ((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS());
                        }
                        mCheckDNFIN_syncLevelInActiveClockEvent.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
                    }
                }
            }
            else if(mNRMstate==2) {
                if(DNFIN_sync_sig.read()==1) {
                    if(cur_timer==mTimeDeassertNRMPoint) {
                        mNRMstate = 1;
                        double syn_time = mDNFP010_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint)+((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()*(GetSLST()-1));
                        mTimeAssertNRMPoint = cur_timer + syn_time;
                        mAssertnoise_eliminated_sigEvent.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
                    }
                    else {
                        double syn_time = mDNFP010_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint);
                        if((cur_timer+syn_time)==mTimeDeassertNRMPoint) {
                            mDeassertnoise_eliminated_sigEvent.cancel();
                        }
                        if(syn_time==0) {
                            syn_time = syn_time + ((double)(mDNFP010_E2_AgentController->mdnfp01tsmpclkPeriod)*GetPRS());
                        }
                        mCheckDNFIN_syncLevelInActiveClockEvent.notify(syn_time,mDNFP010_E2_AgentController->mTimeResolutionUnit);
                    }
                }
            }
        }
        else {
            re_printf("warning","Setting of PRS bits is not support, cannot process noise elimination.\n");
        }
    }
}//}}}
// vim600: set foldmethod=marker :
