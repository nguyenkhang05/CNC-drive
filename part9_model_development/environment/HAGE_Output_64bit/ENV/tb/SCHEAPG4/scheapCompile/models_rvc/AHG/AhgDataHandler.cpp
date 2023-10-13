// ---------------------------------------------------------------------
// $Id: AhgDataHandler.cpp,v 1.3 2014/04/11 05:42:45 duymai Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "AhgDataHandler.h"

///Constructor of AhgDataHandler class
AhgDataHandler::AhgDataHandler(std::string name,
                         BusBridgeModuleBase<32,32> *Parent,
                         BusBridgeSlaveIf<32> *SlaveIf,                       
                         BusMasterIf<32> *MasterIf, 
                         AhgAgentController *AgentController_ptr): 
                         BusBridgeCoreBase<32,32>(Parent,SlaveIf,MasterIf)
{//{{{
    assert((AgentController_ptr != NULL) && (MasterIf != NULL) && (SlaveIf != NULL));
    mAgentController = AgentController_ptr;
    mMasterIf = MasterIf;
    mSlaveIf = SlaveIf;
    mTransTime = SC_ZERO_TIME;
    mTrans = (TlmBasicPayload*)0;
    mIsDebug = false;
}//}}}

///Notify transaction completion in AT mode
void AhgDataHandler::TransferComplete(void)
{//{{{
    if ((BusBridgeCoreBase<32,32>::mTransType == AT) && (!mIsDebug)) {//AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}

///Set transaction basic info (address, data, length)
void AhgDataHandler::SetTransBasic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    outTrans.set_address(inTrans.get_address());
    outTrans.set_data_ptr(inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}

///Set transaction G3M extension
void AhgDataHandler::SetTransG3m(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmG3mExtension *inG3mExt  = (TlmG3mExtension *)0;
    TlmG3mExtension *outG3mExt = new TlmG3mExtension;
    inTrans.get_extension(inG3mExt);
    if (inG3mExt != NULL) {
        outG3mExt->setDmaAccess(inG3mExt->isDmaAccess());
        outG3mExt->setVmId(inG3mExt->getVmId());
        outG3mExt->setTcId(inG3mExt->getTcId());
        outG3mExt->setPeId(inG3mExt->getPeId());
        outG3mExt->setSpId(inG3mExt->getSpId());
        outG3mExt->setUserMode(inG3mExt->isUserMode());
        outG3mExt->setVirtualMode(inG3mExt->isVirtualMode());
        outTrans.set_extension(outG3mExt);
    }
}//}}}

///Set transaction AHB extension
void AhgDataHandler::SetTransAhb(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmAhbExtension *inAhbExt  = (TlmAhbExtension *)0;
    TlmAhbExtension *outAhbExt = new TlmAhbExtension;
    inTrans.get_extension(inAhbExt);
    if (inAhbExt != NULL) {
        outAhbExt->setBurstType(inAhbExt->getBurstType());
        outAhbExt->setCachable(inAhbExt->isCachable());
        outAhbExt->setBufferable(inAhbExt->isBufferable());
        outAhbExt->setPrivilege(inAhbExt->isPrivilege());
        outAhbExt->setDataOrOp(inAhbExt->getDataOrOp());
        outTrans.set_extension(outAhbExt);
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void AhgDataHandler::read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    mTrans = &trans;
    mTransTime = *t;
    bool is_valid_trans = false;
    mIsDebug = debug;
    bool is_continue = false;
    if ((mAgentController->CheckClock("PCLK"))&&(mAgentController->CheckClock("HCLK"))) {
        if (debug) {
            is_continue = true;
        } else {
            if (mAgentController->GetResetStatus() == true) {
                mAgentController->NotifyTransComp();
                mAgentController->print_msg("error","The reading transaction is fail because the reset is active\n");
            } else {
                is_continue = true;
            }
        }
        if (is_continue) {
            if (mAgentController->CheckAccessPermission(trans,debug,is_valid_trans)) {
                if (debug || (BusBridgeCoreBase<32,32>::mTransType == LT)) {// LT mode
                    mMasterIf->read(trans,debug,*t,false);
                } else {//AT mode
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SetTransBasic(trans, *sendTrans );
                    SetTransG3m(trans, *sendTrans );
                    SetTransAhb(trans, *sendTrans );
                    mRequestMap[sendTrans] = &trans;
                    mMasterIf->read(*sendTrans,debug,*t,false);
                }
            } else {
                if (is_valid_trans) {
                    mAgentController->TransferErrInfo(trans);
                }
                mAgentController->NotifyTransComp();
            }
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void AhgDataHandler::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    mTrans = &trans;
    mTransTime = *t;
    bool is_valid_trans = false;
    mIsDebug = debug;
    bool is_continue = false;
    if ((mAgentController->CheckClock("PCLK"))&&(mAgentController->CheckClock("HCLK"))) {
        if (debug) {
            is_continue = true;
        } else {
            if (mAgentController->GetResetStatus() == true) {
                mAgentController->NotifyTransComp();
                mAgentController->print_msg("error","The writing transaction is fail because the reset is active\n");
            } else {
                is_continue = true;
            }
        }
        if (is_continue) {
            if (mAgentController->CheckAccessPermission(trans,debug,is_valid_trans)) {
                if (debug || (BusBridgeCoreBase<32,32>::mTransType == LT)) {// LT mode
                    mMasterIf->write(trans,debug,*t,false);
                } else {//AT mode
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SetTransBasic(trans, *sendTrans );
                    SetTransG3m(trans, *sendTrans );
                    SetTransAhb(trans, *sendTrans );
                    mRequestMap[sendTrans] = &trans;
                    mMasterIf->write(*sendTrans,debug,*t,false);
                }
            } else {
                if (is_valid_trans) {
                    mAgentController->TransferErrInfo(trans);
                }
                mAgentController->NotifyTransComp();
            }
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void AhgDataHandler::deleteTrans(void)
{//{{{
    TlmBasicPayload *trans;
    TlmG3mExtension *G3mExt;
    TlmAhbExtension *AhbExt;
    while(!mClearTransFifo.empty()){
        trans = mClearTransFifo.front();
        G3mExt = (TlmG3mExtension *)0;
        AhbExt = (TlmAhbExtension *)0;
        trans->get_extension(G3mExt);
        trans->get_extension(AhbExt);
        trans->release_extension(G3mExt);
        trans->release_extension(AhbExt);
        mRequestMap.erase(trans);
        mClearTransFifo.pop();
        delete trans;
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void AhgDataHandler::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
    if(!mRequestMap.empty()){
        mSlaveIf->notifyTransComp(*mRequestMap[&trans],t);
        deleteTrans();
        mClearTransFifo.push(&trans);
    }
}//}}}
// vim600: set foldmethod=marker :
