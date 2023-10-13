// ---------------------------------------------------------------------
// $Id: AXISTB_DataHandler.cpp,v 1.3 2014/05/05 01:23:39 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "AXISTB_DataHandler.h"

///Constructor of AXISTB_DataHandler class
AXISTB_DataHandler::AXISTB_DataHandler(std::string name,
                       std::string specified_name,
                       BusBridgeModuleBase<64,64> *Parent,
                       BusBridgeSlaveIf<64> *SlaveIf,
                       BusMasterIf<64> *MasterIf,
                       AXISTB_AgentController *AgentController_ptr): 
                       BusBridgeCoreBase<64,64>(Parent,SlaveIf,MasterIf)
{//{{{
    assert((AgentController_ptr != NULL)&&(MasterIf != NULL)&&(SlaveIf != NULL));
    mAXISTBAgentController = AgentController_ptr;
    mSpecifiedName = specified_name;
    mMasterIf = MasterIf;
    mSlaveIf = SlaveIf;
    mTransTime = SC_ZERO_TIME;
    mTrans = (TlmBasicPayload*)0;
}//}}}

///Set transaction basic info (address, data, length)
void AXISTB_DataHandler::SetTransBasic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    outTrans.set_address(inTrans.get_address());
    outTrans.set_data_ptr(inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}

///Set transaction G3M extension
void AXISTB_DataHandler::SetTransG3m(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
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

///Set transaction APB extension
void AXISTB_DataHandler::SetTransAxi(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmAxiExtension *inAxiExt  = (TlmAxiExtension *)0;
    TlmAxiExtension *outAxiExt = new TlmAxiExtension;
    inTrans.get_extension(inAxiExt);
    if (inAxiExt != NULL) {
        outAxiExt->setBurstType(inAxiExt->getBurstType());
        outAxiExt->setLock(inAxiExt->isLock());
        outAxiExt->setCachable(inAxiExt->isCachable());
        outAxiExt->setBufferable(inAxiExt->isBufferable());
        outAxiExt->setSecure(inAxiExt->getSecure());
        outAxiExt->setTransId(inAxiExt->getTransId());
        outAxiExt->setBitOpType(inAxiExt->getBitOpType());
        outAxiExt->setBitOpPos(inAxiExt->getBitOpPos());
        outTrans.set_extension(outAxiExt);
    }
}//}}}

///Transfer data to target IP
void AXISTB_DataHandler::TransferData(const bool is_write, TlmBasicPayload *trans, BusTime_t* t, bool debug)
{//{{{
    assert((t!=NULL) && (trans!=NULL));
    if ((mAXISTBAgentController->CheckClockPeriod("PCLK"))&&(mAXISTBAgentController->CheckClockPeriod("HCLK"))) {
        std::string ini_name = "iss";
        if (mSpecifiedName == "GRI") {
            ini_name = "isg";
        }
        std::string act = "";
        if (debug || (BusBridgeCoreBase<64,64>::mTransType == LT)) {// LT mode
            if (is_write) {
                mMasterIf->write(*trans,debug,*t,false);
                act = "write";
            } else {
                mMasterIf->read(*trans,debug,*t,false);
                act = "read";
            }
            if (!debug) {
                mAXISTBAgentController->NotifyTransComp(mSpecifiedName);
            }
        } else {// AT mode
            TlmBasicPayload *sendTrans = new TlmBasicPayload;
            SetTransBasic(*trans, *sendTrans );
            SetTransG3m(*trans, *sendTrans );
            SetTransAxi(*trans, *sendTrans );
            mRequestMap[sendTrans] = trans;
            if (is_write) {
                mMasterIf->write(*sendTrans,debug,*t,false);
                act = "write";
            } else {
                mMasterIf->read(*sendTrans,debug,*t,false);
                act = "read";
            }
        }
        mAXISTBAgentController->print_msg("info","A %s transaction is output via %s socket.\n", act.c_str(), ini_name.c_str());
    }
}//}}}

void AXISTB_DataHandler::CancelTransferring(void)
{//{{{
    if (BusBridgeCoreBase<64,64>::mTransType == AT) {
        assert(mTrans != NULL);
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void AXISTB_DataHandler::read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    assert(t != NULL);
    if ((mAXISTBAgentController->CheckClockPeriod("PCLK"))&&(mAXISTBAgentController->CheckClockPeriod("HCLK"))) {
        if (debug == false) {
            if (BusBridgeCoreBase<64,64>::mTransType == LT) {               
                if (mAXISTBAgentController->GetHRESETZResetStatus() == false) {
                    mAXISTBAgentController->TransferLT(mSpecifiedName, false, trans, *t);
                }
            } else {
                if (mAXISTBAgentController->GetHRESETZResetStatus() == false) {
                    mAXISTBAgentController->PushDataToBuffer(mSpecifiedName, false, trans, *t); // push data to buffer
                    mAXISTBAgentController->ContinueTransferring(mSpecifiedName);
                } else {
                    mTrans = &trans;
                    mTransTime = *t;
                    mAXISTBAgentController->print_msg("error","The transfer is fail because the reset is active\n");
                    mAXISTBAgentController->NotifyTransCancel(mSpecifiedName);
                }
            }
        } else {
            TransferData(false, &trans, t, debug);
        }
    }
}//}}}

void AXISTB_DataHandler::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    assert(t != NULL);
    if ((mAXISTBAgentController->CheckClockPeriod("PCLK"))&&(mAXISTBAgentController->CheckClockPeriod("HCLK"))) {
        if (debug == false) {
            if (BusBridgeCoreBase<64,64>::mTransType == LT) {
                if (mAXISTBAgentController->GetHRESETZResetStatus() == false) {
                    mAXISTBAgentController->TransferLT(mSpecifiedName, true, trans, *t);
                }
            } else {
                if (mAXISTBAgentController->GetHRESETZResetStatus() == false) {
                    mAXISTBAgentController->PushDataToBuffer(mSpecifiedName, true, trans, *t); // push data to buffer
                    mAXISTBAgentController->ContinueTransferring(mSpecifiedName);
                } else {
                    mTrans = &trans;
                    mTransTime = *t;
                    mAXISTBAgentController->print_msg("error","The transfer is fail because the reset is active\n");
                    mAXISTBAgentController->NotifyTransCancel(mSpecifiedName);
                }
            }
        } else {
            TransferData(true, &trans, t, debug);
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void AXISTB_DataHandler::deleteTrans( void )
{//{{{
    TlmBasicPayload *trans;
    TlmG3mExtension *G3mExt;
    TlmAxiExtension *AxiExt;
    while(!mClearTransFifo.empty()){
        trans = mClearTransFifo.front();
        G3mExt = (TlmG3mExtension *)0;
        AxiExt = (TlmAxiExtension *)0;
        trans->get_extension(G3mExt);
        trans->get_extension(AxiExt);
        trans->release_extension(G3mExt);
        trans->release_extension(AxiExt);
        mRequestMap.erase(trans);
        mClearTransFifo.pop();
        delete trans;
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void AXISTB_DataHandler::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
    if(!mRequestMap.empty()){
        mSlaveIf->notifyTransComp(*mRequestMap[&trans],t);
        mClearTransFifo.push(&trans);
        deleteTrans();
        mAXISTBAgentController->NotifyTransComp(mSpecifiedName);
    }
}//}}}
// vim600: set foldmethod=marker :
