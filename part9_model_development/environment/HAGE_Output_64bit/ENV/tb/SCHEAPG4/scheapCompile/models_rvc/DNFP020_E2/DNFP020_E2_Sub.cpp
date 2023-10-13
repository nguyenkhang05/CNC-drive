// ---------------------------------------------------------------------
// $Id: DNFP020_E2_Sub.cpp 2617 2017-03-31 06:00:48Z ChinhTD $
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
#include "DNFP020_E2_Sub.h"
#include "DNFP020_E2_Sub_Func.h"
#include "DNFP020_E2_AgentController.h"


///Constructor of DNFP020_E2_Sub class
DNFP020_E2_Sub::DNFP020_E2_Sub (sc_module_name name,
                        DNFP020_E2_AgentController *DNFP020_E2_AgentController_Ptr,
                        unsigned int ch_id):
    sc_module(name),
    DNFP020_E2_Sub_AgentController(),
    // Input ports
    DNFEDGEIN("DNFEDGEIN")

{//{{{
    assert(DNFP020_E2_AgentController_Ptr != NULL);
    mDNFP020_E2_AgentController = DNFP020_E2_AgentController_Ptr;
    //mDNFP020_E2_AgentController->CommandInit((std::string)name);
    mModuleName = (std::string)name;
    mChId = ch_id;

    mDNFP020_E2_Sub_Func = new DNFP020_E2_Sub_Func((std::string)name, this);
    sc_assert(mDNFP020_E2_Sub_Func != NULL);



    // Initialize variables & output ports
    mTimePRSChangePoint = 0;
    Initialize();

    // Declare method/thread
    ///Input Handle Methods
    SC_METHOD(Writeedge_detecting_signalMethod);
    dont_initialize();
    sensitive << medge_detecting_signalEvent;

    SC_METHOD(Handleedge_detecting_signalMethod);
    dont_initialize();
    sensitive << edge_detecting_signal;

    SC_METHOD(HandleDNFEDGEINMethod);
    dont_initialize();
    sensitive << DNFEDGEIN;

    SC_METHOD(AssertDNFEDGEIN_sync0Method);
    dont_initialize();
    sensitive << mAssertDNFEDGEIN_sync0Event;

    SC_METHOD(DeassertDNFEDGEIN_sync0Method);
    dont_initialize();
    sensitive << mDeassertDNFEDGEIN_sync0Event;

    SC_METHOD(AssertDNFEDGEIN_syncMethod);
    dont_initialize();
    sensitive << mAssertDNFEDGEIN_syncEvent;

    SC_METHOD(DeassertDNFEDGEIN_syncMethod);
    dont_initialize();
    sensitive << mDeassertDNFEDGEIN_syncEvent;

    SC_METHOD(Assertnoise_eliminated_sigMethod);
    dont_initialize();
    sensitive << mAssertnoise_eliminated_sigEvent;

    SC_METHOD(Deassertnoise_eliminated_sigMethod);
    dont_initialize();
    sensitive << mDeassertnoise_eliminated_sigEvent;

    SC_METHOD(WriteDNFEDGEIN_syncMethod);
    dont_initialize();
    sensitive << mWriteDNFEDGEIN_sync_sigEvent;

    SC_METHOD(Writenoise_eliminated_sigMethod);
    dont_initialize();
    sensitive << mWritenoise_eliminated_sigEvent;

    SC_METHOD(HandleNFENchangedMethod);
    dont_initialize();
    sensitive << mNFENchangedEvent;

    SC_METHOD(CheckDNFEDGEIN_syncLevelInActiveClockMethod);
    dont_initialize();
    sensitive << mCheckDNFEDGEIN_syncLevelInActiveClockEvent;

    SC_METHOD(CheckChangeStateToIdleMethod);
    dont_initialize();
    sensitive << mCheckChangeStateToIdleEvent;

    SC_METHOD(NRMStateMachineMethod);
    dont_initialize();
    sensitive << DNFEDGEIN_sync_sig;

    SC_METHOD(SetEDFMethod);
    dont_initialize();
    sensitive << mUpdateEDFRegEvent;

}//}}}

///Destructor of DNFP020_E2_Sub class
DNFP020_E2_Sub::~DNFP020_E2_Sub (void)
{//{{{
    delete mDNFP020_E2_Sub_Func;
}//}}}


/// Cancel operation events
void DNFP020_E2_Sub::CancelEvents (void)
{//{{{
    medge_detecting_signalEvent.cancel();
    mAssertDNFEDGEIN_sync0Event.cancel();
    mDeassertDNFEDGEIN_sync0Event.cancel();
    mAssertDNFEDGEIN_syncEvent.cancel();
    mDeassertDNFEDGEIN_syncEvent.cancel();

    mAssertnoise_eliminated_sigEvent.cancel();
    mDeassertnoise_eliminated_sigEvent.cancel();

    mWriteDNFEDGEIN_sync_sigEvent.cancel();
    mWritenoise_eliminated_sigEvent.cancel();

    mNFENchangedEvent.cancel();
    mCheckDNFEDGEIN_syncLevelInActiveClockEvent.cancel();

    mAssertINTEvent.cancel();
    mDeassertINTEvent.cancel();

    mCheckChangeStateToIdleEvent.cancel();
    mUpdateEDFRegEvent.cancel();

}//}}}

/// Initialize variables & output ports
void DNFP020_E2_Sub::Initialize (void)
{//{{{
    //Initialize variables
    medge_detecting_signal_value = false;
    mDNFEDGEIN_value = false;
    mDNFEDGEIN_sync_sig_value = false;
    mnoise_eliminated_sig_value = false;
    medge_detecting_signalEvent.notify(SC_ZERO_TIME);
    mWriteDNFEDGEIN_sync_sigEvent.notify(SC_ZERO_TIME);
    mWritenoise_eliminated_sigEvent.notify(SC_ZERO_TIME);

    mNRMstate = 0;
    mTimeAssertNRMPoint = 0;
    mTimeDeassertNRMPoint = 0;
    mTimeAssertDNFEDGEIN_syncPoint = 0;
    mTimeDeassertDNFEDGEIN_syncPoint = 0;
}//}}}

/***********************************
Function        : EnableReset
Description     : Reset is called
***********************************/
void DNFP020_E2_Sub::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if(is_active) {
        Initialize();
        CancelEvents();
    }
    mDNFP020_E2_Sub_Func->Reset(is_active);
}//}}}

void DNFP020_E2_Sub::HandleDNFEDGEINMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK")) {
        if(mDNFEDGEIN_value != DNFEDGEIN.read()) {
            mDNFEDGEIN_value = DNFEDGEIN.read();
            double cur_timer = sc_time_stamp().to_double();
            double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge((double)(mDNFP020_E2_AgentController->mPCLKPeriod),true,cur_timer,mDNFP020_E2_AgentController->mTimeCLKChangePoint);
            if(mDNFEDGEIN_value) {
                mAssertDNFEDGEIN_sync0Event.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
            }
            else {
                mDeassertDNFEDGEIN_sync0Event.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
            }
        }
    }
}//}}}

void DNFP020_E2_Sub::AssertDNFEDGEIN_sync0Method (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK")) {
        if(mDNFEDGEIN_value) {
            double cur_timer = sc_time_stamp().to_double();
            double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge((double)(mDNFP020_E2_AgentController->mPCLKPeriod),true,cur_timer,mDNFP020_E2_AgentController->mTimeCLKChangePoint)+(double)(mDNFP020_E2_AgentController->mPCLKPeriod);
            mTimeAssertDNFEDGEIN_syncPoint = cur_timer + syn_time;
            mAssertDNFEDGEIN_syncEvent.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
        }
    }
}//}}}

void DNFP020_E2_Sub::DeassertDNFEDGEIN_sync0Method (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK")) {
        if(!mDNFEDGEIN_value) {
            double cur_timer = sc_time_stamp().to_double();
            double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge((double)(mDNFP020_E2_AgentController->mPCLKPeriod),true,cur_timer,mDNFP020_E2_AgentController->mTimeCLKChangePoint)+(double)(mDNFP020_E2_AgentController->mPCLKPeriod);
            mTimeDeassertDNFEDGEIN_syncPoint = cur_timer + syn_time;
            mDeassertDNFEDGEIN_syncEvent.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
        }
    }
}//}}}

void DNFP020_E2_Sub::AssertDNFEDGEIN_syncMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK")) {
        mDNFEDGEIN_sync_sig_value = true;
        mWriteDNFEDGEIN_sync_sigEvent.notify(SC_ZERO_TIME);
        mTimeAssertDNFEDGEIN_syncPoint = 0;
    }
}//}}}

void DNFP020_E2_Sub::DeassertDNFEDGEIN_syncMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK")) {
        mDNFEDGEIN_sync_sig_value = false;
        mWriteDNFEDGEIN_sync_sigEvent.notify(SC_ZERO_TIME);
        mTimeDeassertDNFEDGEIN_syncPoint = 0;
    }
}//}}}

void DNFP020_E2_Sub::WriteDNFEDGEIN_syncMethod (void)
{//{{{
    DNFEDGEIN_sync_sig.write(mDNFEDGEIN_sync_sig_value);
    if(!GetNFEN()){
        medge_detecting_signal_value = mDNFEDGEIN_sync_sig_value;
        medge_detecting_signalEvent.notify(SC_ZERO_TIME);
    }
}//}}}

void DNFP020_E2_Sub::Assertnoise_eliminated_sigMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK") && GetNFEN()) {
        mnoise_eliminated_sig_value = true;
        mWritenoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
        mCheckChangeStateToIdleEvent.notify(1,mDNFP020_E2_AgentController->mTimeResolutionUnit);
    }
}//}}}

void DNFP020_E2_Sub::Deassertnoise_eliminated_sigMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK") && GetNFEN()) {
        mnoise_eliminated_sig_value = false;
        mWritenoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
        mCheckChangeStateToIdleEvent.notify(1,mDNFP020_E2_AgentController->mTimeResolutionUnit);
    }
}//}}}

void DNFP020_E2_Sub::Writenoise_eliminated_sigMethod (void)
{//{{{
    noise_eliminated_sig.write(mnoise_eliminated_sig_value);
    if(GetNFEN()){
        medge_detecting_signal_value = mnoise_eliminated_sig_value;
        medge_detecting_signalEvent.notify(SC_ZERO_TIME);
    }
}//}}}

void DNFP020_E2_Sub::Writeedge_detecting_signalMethod (void)
{//{{{
    edge_detecting_signal.write(medge_detecting_signal_value);
    //re_printf("info","edge_detecting_signal is written %d.\n",medge_detecting_signal_value);
}//}}}

void DNFP020_E2_Sub::Handleedge_detecting_signalMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK") && GetDMD()!=0) {
        if((GetDMD()==1 && edge_detecting_signal.read()==1) || (GetDMD()==2 && edge_detecting_signal.read()==0) || (GetDMD()==3)){
            double cur_timer = sc_time_stamp().to_double();
            double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge((double)(mDNFP020_E2_AgentController->mPCLKPeriod),true,cur_timer,mDNFP020_E2_AgentController->mTimeCLKChangePoint)+(double)(mDNFP020_E2_AgentController->mPCLKPeriod);
            mUpdateEDFRegEvent.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
        }
    }
}//}}}

void DNFP020_E2_Sub::SetEDFMethod(void)
{//{{{
    return mDNFP020_E2_Sub_Func->SetEDF();
}//}}}

bool DNFP020_E2_Sub::GetNFEN(void)
{//{{{
    return mDNFP020_E2_Sub_Func->GetNFEN();
}//}}}

unsigned int DNFP020_E2_Sub::GetSLST(void)
{//{{{
    return mDNFP020_E2_AgentController->GetSLST();
}//}}}

unsigned int DNFP020_E2_Sub::GetPRS(void)
{//{{{
    return mDNFP020_E2_AgentController->GetPRS();
}//}}}

unsigned int DNFP020_E2_Sub::GetDMD(void)
{//{{{
    return mDNFP020_E2_Sub_Func->GetDMD();
}//}}}

void DNFP020_E2_Sub::SetTimePRSChangePoint (void)
{//{{{
    double cur_timer = sc_time_stamp().to_double();
    double time_to_next_redge = mDNFP020_E2_AgentController->CalculateNextClockEdge((double)(mDNFP020_E2_AgentController->mPCLKPeriod),true,cur_timer,mDNFP020_E2_AgentController->mTimeCLKChangePoint);
    if(time_to_next_redge==0){
        time_to_next_redge = (double)(mDNFP020_E2_AgentController->mPCLKPeriod);
    }
    mTimePRSChangePoint = sc_time_stamp().to_double() + time_to_next_redge;
}//}}}

void DNFP020_E2_Sub::NotifyEvent (const unsigned int event)
{//{{{
    switch (event) {
        case emNFENchanged :
            mNFENchangedEvent.notify(SC_ZERO_TIME);
        default :   // emEDFcleared
            mUpdateEDFRegEvent.cancel();
            break;
    }
}//}}}

void DNFP020_E2_Sub::HandleNFENchangedMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK")) {
        if(GetNFEN()){
            medge_detecting_signal_value = noise_eliminated_sig.read();
            medge_detecting_signalEvent.notify(SC_ZERO_TIME);
        }
        else{
            medge_detecting_signal_value = DNFEDGEIN_sync_sig.read();
            medge_detecting_signalEvent.notify(SC_ZERO_TIME);

            mNRMstate = 0;
            mAssertnoise_eliminated_sigEvent.cancel();
            mDeassertnoise_eliminated_sigEvent.cancel();
            mnoise_eliminated_sig_value = false;
            mWritenoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

void DNFP020_E2_Sub::CheckDNFEDGEIN_syncLevelInActiveClockMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK") && GetNFEN()) {
        double cur_timer = sc_time_stamp().to_double();
        if(mNRMstate==1){
            if((mTimeAssertDNFEDGEIN_syncPoint==0 && !mDNFEDGEIN_sync_sig_value) || (mTimeAssertDNFEDGEIN_syncPoint!=0 && cur_timer<mTimeAssertDNFEDGEIN_syncPoint)) {
                mAssertnoise_eliminated_sigEvent.cancel();
                mNRMstate = 0;
            }
            else if(cur_timer==mTimeAssertNRMPoint){
                mAssertnoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
            }
        }
        else if(mNRMstate==2){
            if((mTimeDeassertDNFEDGEIN_syncPoint==0 && mDNFEDGEIN_sync_sig_value) || (mTimeDeassertDNFEDGEIN_syncPoint!=0 && cur_timer<mTimeDeassertDNFEDGEIN_syncPoint)) {
                mDeassertnoise_eliminated_sigEvent.cancel();
                mNRMstate = 0;
            }
            else if(cur_timer==mTimeDeassertNRMPoint){
                mDeassertnoise_eliminated_sigEvent.notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

void DNFP020_E2_Sub::CheckChangeStateToIdleMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK") && GetNFEN()) {
        if((DNFEDGEIN_sync_sig.read()==1 && mNRMstate==1 && noise_eliminated_sig.read()==1) || (DNFEDGEIN_sync_sig.read()==0 && mNRMstate==2 && noise_eliminated_sig.read()==0)){
            mNRMstate = 0;
        }
    }
}//}}}

void DNFP020_E2_Sub::NRMStateMachineMethod (void)
{//{{{
    if (!mDNFP020_E2_AgentController->GetResetStatus("PRESETn") && mDNFP020_E2_AgentController->CheckClockPeriod("PCLK") && GetNFEN()) {
        if(GetPRS()!=0) {
            double cur_timer = sc_time_stamp().to_double();
            if(mNRMstate==0) {
                if(DNFEDGEIN_sync_sig.read()==1) {
                    mNRMstate = 1;
                    double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint)+((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()*(GetSLST()-1));
                    mTimeAssertNRMPoint = cur_timer + syn_time;
                    mAssertnoise_eliminated_sigEvent.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
                }
                else if(DNFEDGEIN_sync_sig.read()==0) {
                    mNRMstate = 2;
                    double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint)+((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()*(GetSLST()-1));
                    mTimeDeassertNRMPoint = cur_timer + syn_time;
                    mDeassertnoise_eliminated_sigEvent.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
                }
            }
            else if(mNRMstate==1) {
                if(DNFEDGEIN_sync_sig.read()==0) {
                    if(cur_timer==mTimeAssertNRMPoint){
                        mNRMstate = 2;
                        double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint)+((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()*(GetSLST()-1));
                        mTimeDeassertNRMPoint = cur_timer + syn_time;
                        mDeassertnoise_eliminated_sigEvent.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
                    }
                    else {
                        double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint);
                        if((cur_timer+syn_time)==mTimeAssertNRMPoint) {
                            mAssertnoise_eliminated_sigEvent.cancel();
                        }
                        if(syn_time==0) {
                            syn_time = syn_time + ((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS());
                        }
                        mCheckDNFEDGEIN_syncLevelInActiveClockEvent.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
                    }
                }
            }
            else if(mNRMstate==2) {
                if(DNFEDGEIN_sync_sig.read()==1) {
                    if(cur_timer==mTimeDeassertNRMPoint) {
                        mNRMstate = 1;
                        double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint)+((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()*(GetSLST()-1));
                        mTimeAssertNRMPoint = cur_timer + syn_time;
                        mAssertnoise_eliminated_sigEvent.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
                    }
                    else {
                        double syn_time = mDNFP020_E2_AgentController->CalculateNextClockEdge(((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS()),true,cur_timer,mTimePRSChangePoint);
                        if((cur_timer+syn_time)==mTimeDeassertNRMPoint) {
                            mDeassertnoise_eliminated_sigEvent.cancel();
                        }
                        if(syn_time==0) {
                            syn_time = syn_time + ((double)(mDNFP020_E2_AgentController->mPCLKPeriod)*GetPRS());
                        }
                        mCheckDNFEDGEIN_syncLevelInActiveClockEvent.notify(syn_time,mDNFP020_E2_AgentController->mTimeResolutionUnit);
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
