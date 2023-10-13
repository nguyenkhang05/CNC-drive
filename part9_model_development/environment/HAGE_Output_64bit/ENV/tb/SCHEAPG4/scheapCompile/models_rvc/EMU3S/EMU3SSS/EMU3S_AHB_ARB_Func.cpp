// ---------------------------------------------------------------------
// $Id: EMU3S_AHB_ARB_Func.cpp,v 1.6 2020/11/18 18:52:19 thiepnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <cstdarg>
#include "EMU3S_AHB_ARB_Func.h"
#include "BusMasterIf.h"
#include <iomanip>
#include "EMU3S_AHB_ARB.h"

// Constructor of EMU3S_AHB_ARB_Func class
EMU3S_AHB_ARB_Func::EMU3S_AHB_ARB_Func(std::string name,
                         BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB> *Parent,
                         BusBridgeSlaveIf<BUS_WIDTH_AHB> *SlaveIf,
                         BusMasterIf<BUS_WIDTH_AHB> *MasterIf,
                         SIM_MODE_T simmode):
                         BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>(Parent,SlaveIf,MasterIf)
{//{{{
    if((MasterIf == NULL) || (SlaveIf == NULL)){
        return;
    }
    BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>::setTlmTransType(simmode);
    mMasterIf               = MasterIf;
    mSlaveIf                = SlaveIf;

    Initialize();
    mTransTime              = SC_ZERO_TIME;
    mTrans                  = (TlmBasicPayload*)0;
    mIsDebug                = false;
    mbus_stop_ack           = false;
    mrequest_write_number   = 0;
}//}}}

// Description: Destructor of EMU3S_AHB_ARB_Func class
EMU3S_AHB_ARB_Func::~EMU3S_AHB_ARB_Func( void )
{
    deleteTrans();
}

// Description: Notify transaction completion in AT mode at abnormal state
void EMU3S_AHB_ARB_Func::TransferComplete(void)
{//{{{
    if ((BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>::mTransType == AT) && (!mIsDebug)) {//AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}

// Description: Set transaction common infor
void EMU3S_AHB_ARB_Func::setTransBasicEmu3s( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  outTrans.set_address( inTrans.get_address() );
  outTrans.set_data_ptr( inTrans.get_data_ptr() );
  outTrans.set_data_length( inTrans.get_data_length() );
}

// Description: Set AHB extension for transaction
void EMU3S_AHB_ARB_Func::setTransAhb(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmAhbExtension *inAhbExt  = (TlmAhbExtension *)0;
    TlmAhbExtension *outAhbExt = new TlmAhbExtension;
    inTrans.get_extension(inAhbExt);

    if((inAhbExt == NULL) || (outAhbExt == NULL)){
        return;
    }

    outAhbExt->setBurstType(inAhbExt->getBurstType());
    outAhbExt->setLock(inAhbExt->isLock());
    outAhbExt->setCachable(inAhbExt->isCachable());
    outAhbExt->setBufferable(inAhbExt->isBufferable());
    outAhbExt->setPrivilege(inAhbExt->isPrivilege());
    outAhbExt->setDataOrOp(inAhbExt->getDataOrOp());
    outTrans.set_extension(outAhbExt);
}//}}}

// Description: Set G3m extension for transaction
void EMU3S_AHB_ARB_Func::setTransG3mEmu3s( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
    TlmG3mExtension *inG3mExt  = (TlmG3mExtension *)0;
    TlmG3mExtension *outG3mExt = new TlmG3mExtension;
    inTrans.get_extension( inG3mExt );

    if((inG3mExt == NULL) || (outG3mExt == NULL)){
        return;
    }

    outG3mExt->setDmaAccess( inG3mExt->isDmaAccess() );
    outG3mExt->setVmId( inG3mExt->getVmId() );
    outG3mExt->setTcId( inG3mExt->getTcId() );
    outG3mExt->setPeId( inG3mExt->getPeId() );
    outG3mExt->setSpId( inG3mExt->getSpId() );
    outG3mExt->setUserMode( inG3mExt->isUserMode() );
    outG3mExt->setVirtualMode( inG3mExt->isVirtualMode() );
    outG3mExt->setSpcLd( inG3mExt->isSpcLd() );
    outG3mExt->setSecure( inG3mExt->isSecure() );
    outTrans.set_extension( outG3mExt );
}

// Description: Overwrite virtual functions of BusBridgeCoreBase class
void EMU3S_AHB_ARB_Func::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    mTrans = &trans;
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if((pExtInfo == NULL) || (t == NULL)){
        return;
    }
    
    mTransTime = *t;
    mIsDebug = debug;

    if (mbus_stop_ack == false || (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
        if (debug || (BusBridgeCoreBase<32,32>::mTransType == LT)) {// LT mode
            mMasterIf->write(trans,debug,*t,false);
        } else {//AT mode
            TlmBasicPayload *sendTrans = new TlmBasicPayload;
            this->setTransBasicEmu3s(trans, *sendTrans );
            this->setTransG3mEmu3s(trans, *sendTrans );
            this->setTransAhb(trans, *sendTrans );
            mRequestMap[sendTrans] = &trans;
            mMasterIf->write(*sendTrans,debug,*t,false);
            mis_bus_busy            = true;
            mrequest_write_number   = mrequest_write_number + 1;
        }
    } else {
        TransferComplete();
    }  
}//}}}

// Description: Overwrite virtual functions of BusBridgeCoreBase class
void EMU3S_AHB_ARB_Func::deleteTrans(void)
{//{{{
    TlmBasicPayload *trans;
    TlmG3mExtension *mAHBTransG3Ext;
    TlmAhbExtension *mAHBTransExt;
    while(!mClearTransFifo.empty()){
        trans = mClearTransFifo.front();

        if(trans == NULL){
            return;
        }

        mAHBTransExt   = (TlmAhbExtension *)0;
        mAHBTransG3Ext = (TlmG3mExtension *)0;
        trans->get_extension(mAHBTransExt);

        if(mAHBTransExt == NULL){
            return;
        }

        trans->release_extension(mAHBTransExt);
        trans->get_extension(mAHBTransG3Ext);

        if(mAHBTransG3Ext == NULL){
            return;
        }

        trans->release_extension(mAHBTransG3Ext);
        mRequestMap.erase(trans);
        mClearTransFifo.pop();
        delete trans;
    }
}//}}}

// Description: Overwrite virtual functions of BusBridgeCoreBase class
void EMU3S_AHB_ARB_Func::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
        if(mRequestMap[&trans] == NULL){
            return;
        }

        mSlaveIf->notifyTransComp(*mRequestMap[&trans],t);
        deleteTrans();
        mClearTransFifo.push(&trans);
        if(mrequest_write_number == 2){
            m_bus_competion_event.notify(t);
            mrequest_write_number = 0;
        }
}//}}}

// Description: Operate reset
void EMU3S_AHB_ARB_Func::EnableReset (const bool is_active)
{//{{{
    if (is_active) {
        CancelEvents();
        Initialize();
    }
}//}}}

// Description: Cancel operation events
void EMU3S_AHB_ARB_Func::CancelEvents (void)
{//{{{
    m_bus_competion_event.cancel();
}//}}}

// Description: Initialize internal variables
void EMU3S_AHB_ARB_Func::Initialize (void)
{//{{{
    mis_bus_busy = false;
}//}}}

// vim600: set foldmethod=marker :
