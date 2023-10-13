// $Id$
// -----------------------------------------------------------------------------
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// Copyright(c) 2014 Renesas Electronics Corporation
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// This file is generated by FSM generator version 1.0
// Input file : sarad113x_fsm.csv
//              sarad113x_tha_fsm.csv
//              sarad113x_thb_fsm.csv
////////////////////////////////////////////////////////////////////////////////

#include "sarad113x_fsm.h"
#include "sarad113x.h"

Csarad113x_thb_conv_fsm::Csarad113x_thb_conv_fsm(Csarad113x *_parent, std::string upper_state)
    :Cfsm_base(_parent, upper_state, emStNA, emEvtWOE+1)
{
    mStateNameStr[emStTHB_IDLE               ] = mStateNamePrefix + "THB_IDLE";
    mStateNameStr[emStTHB_SAMPLING           ] = mStateNamePrefix + "THB_SAMPLING";
    mStateNameStr[emStTHB_HOLDING            ] = mStateNamePrefix + "THB_HOLDING";
    mStateNameStr[emStTHB_DELAY_HOLDING      ] = mStateNamePrefix + "THB_DELAY_HOLDING";
    mStateNameStr[emStTHB_SCANNING           ] = mStateNamePrefix + "THB_SCANNING";
    mStateNameStr[emStTHB_SCANNING_END       ] = mStateNamePrefix + "THB_SCANNING_END";
    mStateNameStr[emStTHB_VC_CONV            ] = mStateNamePrefix + "THB_VC_CONV";
    mStateNameStr[emStTHB_VC_CONV_END        ] = mStateNamePrefix + "THB_VC_CONV_END";
    mStateNameStr[emStTHB_SUSPEND            ] = mStateNamePrefix + "THB_SUSPEND";
    mStateNameStr[emStTHB_WAIT_SCANNING_START] = mStateNamePrefix + "THB_WAIT_SCANNING_START";
    
    mCurrentState = emStTHB_IDLE; // Initial state


    // State transition table construction
    mNextStateList[emStTHB_IDLE               ][emEvtTHBStartSampling     ] = emStNA + 1;
    
    mNextStateList[emStTHB_SAMPLING           ][emEvtEnd                  ] = emStTHB_IDLE;
    mNextStateList[emStTHB_SAMPLING           ][emEvtTHBHWTrigger         ] = emStNA + 2;
    mNextStateList[emStTHB_SAMPLING           ][emEvtTHBSWTrigger         ] = emStNA + 3;
    mNextStateList[emStTHB_SAMPLING           ][emEvtTHBHoldStart         ] = emStNA + 4;
    
    mNextStateList[emStTHB_HOLDING            ][emEvtEnd                  ] = emStTHB_IDLE;
    mNextStateList[emStTHB_HOLDING            ][emEvtTHBHWTrigger         ] = emStNA + 5;
    mNextStateList[emStTHB_HOLDING            ][emEvtTHBSWTrigger         ] = emStNA + 6;
    mNextStateList[emStTHB_HOLDING            ][emEvtTHBHoldComplete      ] = emStNA + 7;
    
    mNextStateList[emStTHB_DELAY_HOLDING      ][emEvtEnd                  ] = emStTHB_IDLE;
    mNextStateList[emStTHB_DELAY_HOLDING      ][emEvtTHBEndHolding        ] = emStNA + 8;
    
    mNextStateList[emStTHB_SCANNING           ][emEvtWOE                  ] = emStNA + 9;
    
    mNextStateList[emStTHB_SCANNING_END       ][emEvtWOE                  ] = emStNA + 10;
    
    mNextStateList[emStTHB_VC_CONV            ][emEvtEnd                  ] = emStTHB_IDLE;
    mNextStateList[emStTHB_VC_CONV            ][emEvtTHBFinishVCConversion] = emStTHB_VC_CONV_END;
    mNextStateList[emStTHB_VC_CONV            ][emEvtTHBSuspend           ] = emStTHB_SUSPEND;
    
    mNextStateList[emStTHB_VC_CONV_END        ][emEvtWOE                  ] = emStNA + 11;
    
    mNextStateList[emStTHB_SUSPEND            ][emEvtWOE                  ] = emStTHB_WAIT_SCANNING_START;
    
    mNextStateList[emStTHB_WAIT_SCANNING_START][emEvtEnd                  ] = emStTHB_IDLE;
    mNextStateList[emStTHB_WAIT_SCANNING_START][emEvtTHBResume            ] = emStTHB_SCANNING;
    
}

Csarad113x_thb_conv_fsm::~Csarad113x_thb_conv_fsm(void)
{
}

void Csarad113x_thb_conv_fsm::End(void)
{
    Event(emEvtEnd);
}

void Csarad113x_thb_conv_fsm::Event(unsigned int event)
{
    sc_assert(event != emEvtWOE);

    bool state_next = StateTransition(event);
    while (state_next) state_next = StateTransition(emEvtWOE);
}

void Csarad113x_thb_conv_fsm::CheckCondition(const unsigned int condition_id)
{
    sc_assert(condition_id < 11);
    switch (condition_id) {
        case 0:
            if (mParent->CheckEnableTH(Csarad113x::emTHGroupB)) {
                mNextState = emStTHB_SAMPLING;
            }
            break;
        case 1:
            if ((mParent->CheckHoldStart(Csarad113x::emTHGroupB,true,Csarad113x::emHWTrigger)) &&(mParent->CheckSmpTime(Csarad113x::emTHGroupB,true))) {
                mNextState = emStTHB_HOLDING;
            }
            break;
        case 2:
            if ((mParent->CheckHoldStart(Csarad113x::emTHGroupB,true,Csarad113x::emSWTrigger)) &&(mParent->CheckSmpTime(Csarad113x::emTHGroupB,false))) {
                mNextState = emStTHB_HOLDING;
            }
            break;
        case 3:
            if ((mParent->CheckHoldStart(Csarad113x::emTHGroupB,false,Csarad113x::emOtherTrigger)) &&mParent->CheckSmpTime(Csarad113x::emTHGroupB,false)) {
                mNextState = emStTHB_HOLDING;
            }
            break;
        case 4:
            if (mParent->CheckHoldComplete(Csarad113x::emTHGroupB,Csarad113x::emHWTrigger)) {
                mNextState = emStTHB_DELAY_HOLDING;
            }
            break;
        case 5:
            if (mParent->CheckHoldComplete(Csarad113x::emTHGroupB,Csarad113x::emSWTrigger)) {
                mNextState = emStTHB_DELAY_HOLDING;
            }
            break;
        case 6:
            if (mParent->CheckHoldComplete(Csarad113x::emTHGroupB,Csarad113x::emOtherTrigger)) {
                mNextState = emStTHB_DELAY_HOLDING;
            }
            break;
        case 7:
            if (mParent->CheckTHStart(Csarad113x::emTHGroupB)) {
                mNextState = emStTHB_SCANNING;
            }
            else {
                mNextState = emStTHB_WAIT_SCANNING_START;
            }
            break;
        case 8:
            if (mParent->CheckEnableStart(mParent->mCurrentSG)) {
                mNextState = emStTHB_VC_CONV;
            }
            else {
                mNextState = emStTHB_WAIT_SCANNING_START;
            }
            break;
        case 9:
            if (mParent->IsAutoStartSampling()) {
                mNextState = emStTHB_SAMPLING;
            }
            else {
                mNextState = emStTHB_IDLE;
            }
            break;
        case 10:
            if (mParent->IsLastVC(mParent->mCurrentSG) == Csarad113x::emSuspend) {
                mNextState = emStTHB_SUSPEND;
            }
            else if (mParent->IsLastVC(mParent->mCurrentSG) == Csarad113x::emLastVC) {
                mNextState = emStTHB_SCANNING_END;
            }
            else {
                mNextState = emStTHB_VC_CONV;
            }
            break;
        default:
            break;
    }
}

void Csarad113x_thb_conv_fsm::fnEntry(void)
{
    mCurrentState = mNextState;
    switch (mCurrentState) {
        case emStTHB_HOLDING:
            mParent->StartHoldProcess(Csarad113x::emTHGroupB);
            break;
        case emStTHB_DELAY_HOLDING:
            mParent->DelayEndHolding(Csarad113x::emTHGroupB);
            break;
        case emStTHB_SCANNING:
            mParent->SetCurrentSG(Csarad113x::emTHGroupB,true);
            mParent->StartScanning(mParent->mCurrentSG,true);
            break;
        case emStTHB_VC_CONV:
            mParent->StartVCConv(mParent->mCurrentSG);
            break;
        case emStTHB_VC_CONV_END:
            mParent->FinishVCConv(mParent->mCurrentSG);
            break;
        case emStTHB_SUSPEND:
            mParent->SuspendScanning(Csarad113x::emTHGroupB,true);
            break;
        default:
            break;
    }
    mNextState = emStNA;
}

void Csarad113x_thb_conv_fsm::fnExit(void)
{
    mPreState = mCurrentState;
    switch (mCurrentState) {
        case emStTHB_HOLDING:
            mParent->EndHolding(Csarad113x::emTHGroupB);
            break;
        case emStTHB_DELAY_HOLDING:
            mParent->AssertADCATCNVTH(Csarad113x::emTHGroupB);
            break;
        case emStTHB_SCANNING_END:
            mParent->FinishScanning(mParent->mCurrentSG);
            mParent->FinishTHConversion();
            break;
        case emStTHB_VC_CONV_END:
            mParent->DumpActivity(mParent->mCurrentSG,mParent->mCurrentStartVC[mParent->mCurrentSG],mParent->mStartTimeVC);
            mParent->SetStartSmpTime(mParent->GetGCTRL(mParent->mCurrentSG, mParent->mCurrentStartVC[mParent->mCurrentSG]));
            mParent->UpdateInternalCount(mParent->mCurrentSG);
            break;
        default:
            break;
    }
    mCurrentState = emStNA;
}

void Csarad113x_thb_conv_fsm::fnDo(void)
{
}

void Csarad113x_thb_conv_fsm::DumpStateTransInfo(void)
{
    if (mDumpStateTransInfo) {
        mParent->_re_printf("info", "State changes from %s to %s\n", mStateNameStr[mCurrentState].c_str(), mStateNameStr[mNextState].c_str());
    }
}

void Csarad113x_thb_conv_fsm::EnableDumpStateTrans(bool enable)
{
    mDumpStateTransInfo = enable;
}
