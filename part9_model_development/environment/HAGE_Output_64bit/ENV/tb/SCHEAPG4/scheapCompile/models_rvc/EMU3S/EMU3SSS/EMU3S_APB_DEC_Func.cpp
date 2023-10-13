// ---------------------------------------------------------------------
// $Id: EMU3S_APB_DEC_Func.cpp,v 1.8 2020/11/18 19:43:48 thiepnguyen Exp $
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
#include "EMU3S_APB_DEC_Func.h"
#include "BusMasterIf.h"
#include <iomanip>
#include "EMU3S_APB_DEC.h"

// Constructor of EMU3S_APB_DEC_Func class
EMU3S_APB_DEC_Func::EMU3S_APB_DEC_Func(std::string name,
                         BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB> *Parent,
                         BusBridgeSlaveIf<BUS_WIDTH_APB> *SlaveIf,
                         BusMasterIf<BUS_WIDTH_APB> *MasterIf,
                         SIM_MODE_T simmode):
                         BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>(Parent,SlaveIf,MasterIf)
{//{{{
    if((MasterIf == NULL) || (SlaveIf == NULL)){
        return;
    }
    BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::setTlmTransType(simmode);
    mMasterIf       = MasterIf;
    mSlaveIf        = SlaveIf;

    Initialize();
    mTransTime      = SC_ZERO_TIME;
    mTrans          = (TlmBasicPayload*)0;
    mIsDebug        = false;
    mbus_stop_ack   = false;
}//}}}

// Destructor of EMU3S_APB_DEC_Func class
EMU3S_APB_DEC_Func::~EMU3S_APB_DEC_Func( void )
{
    deleteTrans();
}

// Description: Notify transaction completion in AT mode at abnormal state
void EMU3S_APB_DEC_Func::TransferComplete(void)
{//{{{
    if ((BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::mTransType == AT) && (!mIsDebug)) {//AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}

// Description: Set APB extension for transaction
void EMU3S_APB_DEC_Func::setTransApb(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmApbExtension *inAPBExt = (TlmApbExtension *)0;
    inTrans.get_extension(inAPBExt);

    if( inAPBExt == NULL ){
        return;
    }

    TlmApbExtension *outAPBExt = new TlmApbExtension;
    if( outAPBExt == NULL ){
        return;
    }

    outAPBExt->setLock(inAPBExt->isLock());
    outAPBExt->setExclusion(inAPBExt->isExclusion());
    outTrans.set_extension(outAPBExt);

}//}}}

// Description: Set transaction common infor
void EMU3S_APB_DEC_Func::setTransBasicEmu3s( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  outTrans.set_address( inTrans.get_address() );
  outTrans.set_data_ptr( inTrans.get_data_ptr() );
  outTrans.set_data_length( inTrans.get_data_length() );
}

// Description: Set G3m extension for transaction
void EMU3S_APB_DEC_Func::setTransG3mEmu3s( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
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
void EMU3S_APB_DEC_Func::read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    bool is_debug_master_access = false;
    mTrans = &trans;
    mIsDebug = debug;

    if (t == NULL){
        return;
    }
    mTransTime = *t;

    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if(debug){
        if( pExtInfo == NULL ){
            pExtInfo = new TlmG3mExtension;
            trans.set_extension( pExtInfo );
            pExtInfo->setPeId( 0xFF );
            pExtInfo->setSecure( true );
            is_debug_master_access = true;
        }
        else{
            if(pExtInfo->getPeId() == DEBUG_MASTER_BMID){
                is_debug_master_access = true;
            }
        }
    }

    if (mbus_stop_ack == false || is_debug_master_access ) {
        if (debug || (BusBridgeCoreBase<32,32>::mTransType == LT)) {// LT mode
            mMasterIf->read(trans,debug,*t,false);
        } else {//AT mode
            TlmBasicPayload *sendTrans = new TlmBasicPayload;
            this->setTransBasicEmu3s(trans, *sendTrans );
            this->setTransApb(trans, *sendTrans );
            this->setTransG3mEmu3s(trans, *sendTrans );
            mRequestMap[sendTrans] = &trans;
            mMasterIf->read(*sendTrans,debug,*t,false);
            mis_bus_busy = true;
        }
    } else {
        TransferComplete();
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void EMU3S_APB_DEC_Func::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    bool is_debug_master_access = false;
    mTrans = &trans;
    mIsDebug = debug;

    if (t == NULL){
        return;
    }
    mTransTime = *t;

    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if(debug){
        if( pExtInfo == NULL ){
            pExtInfo = new TlmG3mExtension;
            trans.set_extension( pExtInfo );
            pExtInfo->setPeId( 0xFF );
            pExtInfo->setSecure( true );
            is_debug_master_access = true;
        }
        else{
            if(pExtInfo->getPeId() == DEBUG_MASTER_BMID){
                is_debug_master_access = true;
            }
        }
    }

    if (mbus_stop_ack == false || is_debug_master_access ) {
        if (debug || (BusBridgeCoreBase<32,32>::mTransType == LT)) {// LT mode
            mMasterIf->write(trans,debug,*t,false);
        } else {//AT mode
            TlmBasicPayload *sendTrans = new TlmBasicPayload;
            this->setTransBasicEmu3s(trans, *sendTrans );
            this->setTransG3mEmu3s(trans, *sendTrans );
            mRequestMap[sendTrans] = &trans;
            mMasterIf->write(*sendTrans,debug,*t,false);
            mis_bus_busy = true;
        }
    } else {
        TransferComplete();
    }  
}//}}}

// Description: Overwrite virtual functions of BusBridgeCoreBase class
void EMU3S_APB_DEC_Func::deleteTrans(void)
{//{{{
    TlmBasicPayload *trans;
    TlmG3mExtension *mAPBTransG3Ext;

    while(!mClearTransFifo.empty()){
        trans = mClearTransFifo.front();

        if(trans == NULL){
            return;
        }

        mAPBTransG3Ext = (TlmG3mExtension *)0;
        trans->get_extension(mAPBTransG3Ext);

        if(mAPBTransG3Ext == NULL){
            return;
        }

        trans->release_extension(mAPBTransG3Ext);

        mRequestMap.erase(trans);
        mClearTransFifo.pop();
        delete trans;
    }
}//}}}

// Description: Overwrite virtual functions of BusBridgeCoreBase class
void EMU3S_APB_DEC_Func::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
        if(mRequestMap[&trans] == NULL){
            return;
        }

        mSlaveIf->notifyTransComp(*mRequestMap[&trans],t);
        deleteTrans();
        mClearTransFifo.push(&trans);
        m_bus_competion_event.notify(t);
}//}}}

// Description: Operate reset
void EMU3S_APB_DEC_Func::EnableReset (const bool is_active)
{//{{{
    if (is_active) {
        CancelEvents();
        Initialize();
    }
}//}}}

//Description: Cancel operation events
void EMU3S_APB_DEC_Func::CancelEvents (void)
{//{{{
    m_bus_competion_event.cancel();
}//}}}

// Description: Initialize internal variables
void EMU3S_APB_DEC_Func::Initialize (void)
{//{{{
    mis_bus_busy = false;
}//}}}
// vim600: set foldmethod=marker :
