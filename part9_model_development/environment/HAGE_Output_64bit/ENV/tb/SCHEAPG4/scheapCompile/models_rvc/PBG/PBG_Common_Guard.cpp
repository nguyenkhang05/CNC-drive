// ---------------------------------------------------------------------
// $Id: PBG_Common_Guard.cpp,v 1.2 2019/06/16 06:02:16 nhuvo Exp $
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

#include <cstdarg>
#include "PBG_Common_Guard.h"
#include "BusMasterIf.h"
#include <iomanip>

///Constructor of PBG_Common_Guard class
PBG_Common_Guard::PBG_Common_Guard(std::string name,
                         BusBridgeModuleBase<32,32> *Parent,
                         BusBridgeSlaveIf<32> *SlaveIf,
                         BusMasterIf<32> *MasterIf,
                         SIM_MODE_T simmode,
                         unsigned int ch_num):
                         //sc_module(name),
                         BusBridgeCoreBase<32,32>(Parent,SlaveIf,MasterIf)
{//{{{
    assert((MasterIf != NULL) && (SlaveIf != NULL));
    BusBridgeCoreBase<32,32>::setTlmTransType(simmode);
    mMasterIf = MasterIf;
    mSlaveIf = SlaveIf;
    mTransTime = SC_ZERO_TIME;
    mTrans = (TlmBasicPayload*)0;
    mIsDebug = false;
    mChannelNum = ch_num;
    mIsDisableSec  = true;

    mRstState = false;
    mCLKPeriod = 0;
    Initialize();
}//}}}

///Notify transaction completion in AT mode
void PBG_Common_Guard::TransferComplete(void)
{//{{{
    if ((BusBridgeCoreBase<32,32>::mTransType == AT) && (!mIsDebug)) {//AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}

/// Set transaction basic info (address, data, length)
void PBG_Common_Guard::SettingTransBasic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    outTrans.set_address(inTrans.get_address());
    outTrans.set_data_ptr(inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}

/// Set transaction G3M extension
void PBG_Common_Guard::SettingTransG3m(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmG3mExtension *inG3mExt  = (TlmG3mExtension *)0;
    TlmG3mExtension *outG3mExt = new TlmG3mExtension;
    inTrans.get_extension(inG3mExt);
    if (inG3mExt == NULL || outG3mExt == NULL) {
        return;
    }
    outG3mExt->setDmaAccess(inG3mExt->isDmaAccess());
    outG3mExt->setVmId(inG3mExt->getVmId());
    outG3mExt->setTcId(inG3mExt->getTcId());
    outG3mExt->setPeId(inG3mExt->getPeId());
    outG3mExt->setSpId(inG3mExt->getSpId());
    outG3mExt->setUserMode(inG3mExt->isUserMode());
    outG3mExt->setVirtualMode(inG3mExt->isVirtualMode());
    outG3mExt->setSecure(inG3mExt->isSecure());
    outTrans.set_extension(outG3mExt);
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void PBG_Common_Guard::read(unsigned int offsetAddress,TlmBasicPayload& trans, BusTime_t* t,bool debug)
{//{{{
    if(t == NULL) {
        return;
    }

    mTrans = &trans;
    mTransTime = *t;
    mIsDebug = debug;
    bool is_continue = false;
    if (mCLKPeriod != 0) {
        if (debug) {
            is_continue = true;
        } else {
            if (mRstState == true) {
                TransferComplete();
                SC_REPORT_WARNING("PBG_CommonGuard","The reading transaction is fail because the reset is active");
            } else {
                is_continue = true;
            }
        }
        if (is_continue) {
            if (CheckAccessibility(trans,debug)) {
                if (debug || (BusBridgeCoreBase<32,32>::mTransType == LT)) {// LT mode
                    mMasterIf->read(trans,debug,*t,false);
                } else {//AT mode
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SettingTransBasic(trans, *sendTrans );
                    SettingTransG3m(trans, *sendTrans );
                    mRequestMap[sendTrans] = &trans;
                    mMasterIf->read(*sendTrans,debug,*t,false);
                }
            } else {
                TransferComplete();
            }
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void PBG_Common_Guard::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    if(t == NULL) {
        return;
    }
    
    mTrans = &trans;
    mTransTime = *t;
    mIsDebug = debug;
    bool is_continue = false;
    if ((mCLKPeriod != 0)) {
        if (debug) {
            is_continue = true;
        } else {
            if (mRstState == true) {
                TransferComplete();
                SC_REPORT_WARNING("PBG_CommonGuard","The writing transaction is fail because the reset is active");
            } else {
                is_continue = true;
            }
        }
        if (is_continue) {
            if (CheckAccessibility(trans,debug)) {
                if (debug || (BusBridgeCoreBase<32,32>::mTransType == LT)) {// LT mode
                    mMasterIf->write(trans,debug,*t,false);
                } else {//AT mode
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SettingTransBasic(trans, *sendTrans );
                    SettingTransG3m(trans, *sendTrans );
                    mRequestMap[sendTrans] = &trans;
                    mMasterIf->write(*sendTrans,debug,*t,false);
                }
            } else {
                TransferComplete();
            }
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void PBG_Common_Guard::deleteTrans(void)
{//{{{
    while(!mClearTransFifo.empty()){
        mClearTransFifo.front()->free_all_extensions(); 
        mRequestMap.erase(mClearTransFifo.front());
        mClearTransFifo.pop();
    }
}//}}}


///Overwrite virtual functions of BusBridgeCoreBase class
void PBG_Common_Guard::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
    if(mRequestMap.empty()){
        return;
    }
    mSlaveIf->notifyTransComp(*mRequestMap[&trans],t);
    deleteTrans();
    mClearTransFifo.push(&trans);
    
}//}}}

///Check access ability to peripherals
bool PBG_Common_Guard::CheckAccessibility(TlmBasicPayload &trans, bool debug)
{//{{{
    bool status = true;
    bool is_read = trans.is_read();
    bool is_write = trans.is_write();
    bool trans_type = false;
    if ((is_read) && (!is_write)){
        trans_type = false; // read trans
    }
    else {
        trans_type = true; // write trans
    }

    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool um = ExtensionInfo->isUserMode();
    bool sm = ExtensionInfo->isSecure();
    unsigned char spid = ExtensionInfo->getSpId();
    unsigned int addr = (unsigned int)trans.get_address();
    unsigned int ch_id = 0;

    bool is_pbg_trans = CheckTransAddress(addr,ch_id);
    if(is_pbg_trans && ch_id<mChannelNum) {
        bool is_spid_enable = false;
        bool is_write_enable = false;
        bool is_read_enable = false;
        if( ((mSPID[ch_id] >> spid) & 0x1) == 0x1 ){
            is_spid_enable = true;
        }
        if( mWG[ch_id] || is_spid_enable) {
            is_write_enable = true;
        }
        if( mRG[ch_id] || is_spid_enable) {
            is_read_enable = true;
        }


        if( mGEN[ch_id] ) {
            if (mIsDisableSec
            ||((!mIsDisableSec)&&(sm || mSEC[ch_id]))) {
                if(debug && mDBG[ch_id] && (ExtensionInfo->getPeId() == DEBUG_MASTER_BMID)) {
                    status = true;
                }
                else if(um && !mUM[ch_id]) {
                    status = false;
                }
                else {
                    if ((trans_type && !is_write_enable) || (!trans_type && !is_read_enable)) {
                        status = false;
                    }
                    else {
                        status = true;
                    }
                }
            } else {
                status = false;
            }
        }
        else {
            status = true;
        }
    }

    if(ch_id >= mChannelNum) {
        std::stringstream stream;
        stream<<"the address 0x"<< std::hex <<addr<< std::dec <<" refer to channel "<<ch_id<< " is exceeded the number channel of this group";
        SC_REPORT_WARNING("PBG_CommonGuard",stream.str().c_str());
    }

    if(!status) {
        mTypeADDR = addr;
        mTypeDBG = (unsigned int)debug;
        mTypeUM = (unsigned int)um;
        mTypeSPID = spid;
        mTypeWRITE = trans_type;
        mTypeSEC = (unsigned int)sm;
        mTransErrorEvent.notify(SC_ZERO_TIME);
    }
    return status;
}//}}}

///Set start address and size for each guarded area
bool PBG_Common_Guard::SetAreaAddr(const std::string name, const unsigned int start_addr, const unsigned int size, const unsigned int ch_id)
{//{{{
    unsigned int start_address = start_addr;
    unsigned int end_address = start_addr + size - 1;
    AreaInformation temp;
    temp.area_name = name;
    temp.ch_id = ch_id;
    temp.start_addr = start_address;
    temp.end_addr = end_address;
    bool is_valid = true;
    
    for (unsigned int i=0; i<mAreaInfo.size(); i++) {
        if ((start_address > mAreaInfo[i].end_addr) || (end_address < mAreaInfo[i].start_addr)) {
            is_valid = true;
        } else {
            is_valid = false;
            break;
        }
    }
    if (is_valid) {
        mAreaInfo.push_back(temp);
    } else {
        std::stringstream stream;
        stream<<"Address 0x"<< std::hex <<start_address<< std::dec <<" and size "<<size<<" are invalid. They are overlapped with another area";
        SC_REPORT_WARNING("PBG_CommonGuard",stream.str().c_str());
    }
    return is_valid;
}//}}}

///Check whether the address of transaction in guarded areas
bool PBG_Common_Guard::CheckTransAddress(const unsigned int trans_addr, unsigned int &ch_id)
{//{{{
    bool result = false;
    std::string name_of_area;
    for (unsigned int i = 0; i < mAreaInfo.size(); i++) {
        if ((trans_addr >= mAreaInfo[i].start_addr) && (trans_addr <= mAreaInfo[i].end_addr)) {
            name_of_area = mAreaInfo[i].area_name;
            ch_id = mAreaInfo[i].ch_id;
            std::stringstream stream;
            stream<<"Access address 0x"<<std::hex<<trans_addr<<std::dec<<" is inside  "<<name_of_area<<" area";
            SC_REPORT_INFO("PBG_CommonGuard",stream.str().c_str());
            result = true;
            break;
        }
    }
    if (!result) {
        ch_id = 0xFFFFFFFF;
        std::stringstream stream;
        stream<<"Access address 0x"<<std::hex<<trans_addr<<" is outside  "<<name_of_area<<" area";
        SC_REPORT_WARNING("PBG_CommonGuard",stream.str().c_str());
    }
    return result;
}//}}}

void PBG_Common_Guard::EnableReset (const bool is_active)
{//{{{
    mRstState = is_active;
    if (is_active) {
        CancelEvents();
        Initialize();
    }
}//}}}

/// Cancel operation events
void PBG_Common_Guard::CancelEvents (void)
{//{{{
    mTransErrorEvent.cancel();
}//}}}

/// Initialize internal variables
void PBG_Common_Guard::Initialize (void)
{//{{{
    for (unsigned int i = 0; i < emChannelNum; i++) {
        mSPID[i] = 0;
        mGEN[i] = false;
        mDBG[i] = false;
        mUM[i] = false;
        mWG[i] = false;
        mRG[i] = false;
        mSEC[i] = false;
    }
    mTypeADDR    = 0;
    mTypeDBG = false;
    mTypeUM  = false;
    mTypeSPID = 0;
    mTypeWRITE = false;
    mTypeSEC = false;
}//}}}

void PBG_Common_Guard::SetCLKPeriod (const sc_dt::uint64 clk_period)
{//{{{
    mCLKPeriod = clk_period;
}//}}}

void PBG_Common_Guard::SetVAR(const std::string name, const unsigned int val, const unsigned int ch_id) 
{//{{{
    if(name=="SPID") {
        mSPID[ch_id] = val;
    }
    else if(name=="GEN") {
        mGEN[ch_id] = val;
    }
    else if(name=="DBG") {
        mDBG[ch_id] = val;
    }
    else if(name=="UM") {
        mUM[ch_id] = val;
    }
    else if(name=="WG") {
        mWG[ch_id] = val;
    }
    else if(name=="RG") {
        mRG[ch_id] = val;
    }
    else { // (name=="SEC") 
        mSEC[ch_id] = val;
    }
}//}}}

unsigned int PBG_Common_Guard::GetVAR(const std::string name) 
{//{{{
    if(name=="ADDR") {
        return mTypeADDR;
    }
    else if(name=="DBG") {
        return (unsigned int)mTypeDBG;
    }
    else if(name=="UM") {
        return (unsigned int)mTypeUM;
    }
    else if(name=="SPID") {
        return mTypeSPID;
    }
    else if(name=="WRITE") {
        return (unsigned int)mTypeWRITE;
    }
    else { // name=="SEC"
        return (unsigned int)mTypeSEC;
    }
}//}}}

void PBG_Common_Guard::SetSecOperation(bool isDisable)
{
    mIsDisableSec = isDisable;
}
// vim600: set foldmethod=marker :
