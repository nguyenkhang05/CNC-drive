// ---------------------------------------------------------------------
// $Id: HI20FSGD2_Common_Guard.cpp,v 1.6 2019/12/09 06:37:20 hadinhtran Exp $
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
#include "HI20FSGD2_Common_Guard.h"
#include "BusMasterIf.h"
#include <iomanip>
#include "HI20FSGD2.h"

///Constructor of HI20FSGD2_Common_Guard class
HI20FSGD2_Common_Guard::HI20FSGD2_Common_Guard(std::string name,
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

    mRstState = false;
    mCLKPeriod = 0;
    mSECPara = true; // P_SEC_DIS default equal 1
    Initialize();
}//}}}

///Notify transaction completion in AT mode
void HI20FSGD2_Common_Guard::TransferComplete(void)
{//{{{
    if ((BusBridgeCoreBase<32,32>::mTransType == AT) && (!mIsDebug)) {//AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}

///Set transaction basic info (address, data, length)
void HI20FSGD2_Common_Guard::SetTransBasicModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    outTrans.set_address(inTrans.get_address());
    outTrans.set_data_ptr(inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}

///Set transaction G3M extension
void HI20FSGD2_Common_Guard::SetTransG3mModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmG3mExtension *inG3mExt  = (TlmG3mExtension *)0;
    TlmG3mExtension *outG3mExt = new TlmG3mExtension;
    sc_assert(inG3mExt != NULL);
    sc_assert(outG3mExt != NULL);
    inTrans.get_extension(inG3mExt);
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
void HI20FSGD2_Common_Guard::read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    mTrans = &trans;
    sc_assert(t != NULL);
    mTransTime = *t;
    mIsDebug = debug;
    bool is_continue = false;
    
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Zero clock or reset active
    if ((0 == mCLKPeriod) || (true == mRstState)){
        if (pExtInfo != NULL) {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == pExtInfo->getPeId())) {
                is_continue = true;
            } else {
                TransferComplete();
                SC_REPORT_WARNING("HI20FSGD2_Common_Guard", "Cannot access register when clock is 0 or reset port is asserted.\n");
            }
        }
    } else {
        is_continue = true;
    }

    if (is_continue) {
            if (CheckAccessibility(trans,debug)) {
                if (debug || (BusBridgeCoreBase<32,32>::mTransType == LT)) {// LT mode
                    mMasterIf->read(trans,debug,*t,false);
                } else {//AT mode
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SetTransBasicModel(trans, *sendTrans );
                    SetTransG3mModel(trans, *sendTrans );
                    mRequestMap[sendTrans] = &trans;
                    mMasterIf->read(*sendTrans,debug,*t,false);
                }
            } else {
                TransferComplete();
            }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void HI20FSGD2_Common_Guard::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    mTrans = &trans;
    sc_assert(t != NULL);
    mTransTime = *t;
    mIsDebug = debug;
    bool is_continue = false;
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Zero clock or reset active
    if ((0 == mCLKPeriod) || (true == mRstState)){
        if (pExtInfo != NULL) {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == pExtInfo->getPeId())) {
                is_continue = true;
            } else {
                TransferComplete();
                SC_REPORT_WARNING("HI20FSGD2_Common_Guard", "Cannot access register when clock is 0 or reset port is asserted.\n");
            }
        }
    } else {
        is_continue = true;
    }

    if (is_continue) {
        if (CheckAccessibility(trans,debug)) {
            if (debug || (BusBridgeCoreBase<32,32>::mTransType == LT)) {// LT mode
                mMasterIf->write(trans,debug,*t,false);
            } else {//AT mode
                TlmBasicPayload *sendTrans = new TlmBasicPayload;
                SetTransBasicModel(trans, *sendTrans );
                SetTransG3mModel(trans, *sendTrans );
                mRequestMap[sendTrans] = &trans;
                mMasterIf->write(*sendTrans,debug,*t,false);
            }
        } else {
            TransferComplete();
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void HI20FSGD2_Common_Guard::deleteTrans(void)
{//{{{
    TlmBasicPayload *trans;
    TlmG3mExtension *G3mExt;
    while(!mClearTransFifo.empty()){
        trans = mClearTransFifo.front();
        sc_assert(trans != NULL);
        G3mExt = (TlmG3mExtension *)0;
        trans->get_extension(G3mExt);
        sc_assert(G3mExt != NULL);
        trans->release_extension(G3mExt);
        mRequestMap.erase(trans);
        mClearTransFifo.pop();
        delete trans;
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void HI20FSGD2_Common_Guard::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
    if(!mRequestMap.empty()){
        mSlaveIf->notifyTransComp(*mRequestMap[&trans],t);
        deleteTrans();
        mClearTransFifo.push(&trans);
    }
}//}}}

///Check access ability to peripherals
bool HI20FSGD2_Common_Guard::CheckAccessibility(TlmBasicPayload &trans, bool debug)
{//{{{
    bool status = true;
    // bool is_pe_trans = true;
    bool is_read = trans.is_read();
    bool is_write = trans.is_write();
    bool trans_type;
    // g3 extension variable
    bool um = false;
    bool sm = false;
    unsigned char spid = 0x0;
    if ((is_read) && (!is_write)){
        trans_type = false; // read trans
    }
    else {
        trans_type = true; // write trans
    }

    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    if (ExtensionInfo != NULL){
        um = ExtensionInfo->isUserMode();
        sm = ExtensionInfo->isSecure();
        spid = ExtensionInfo->getSpId();
    }

    unsigned int addr = (unsigned int)trans.get_address();
    unsigned int ch_id;

    bool is_pbg_trans = CheckTransAddress(addr,ch_id);
    if(is_pbg_trans && ch_id<mChannelNum) {
        bool is_spid_enable = false;
        bool is_write_enable = false;
        bool is_read_enable = false;
        if((ExtensionInfo != NULL) && (((unsigned int)(mSPID[ch_id]>>(spid))&0x1) == 1)){
            is_spid_enable = true;
        }
        if( mWG[ch_id] || is_spid_enable) {
            is_write_enable = true;
        }
        if( mRG[ch_id] || is_spid_enable) {
            is_read_enable = true;
        }

        if( mGEN[ch_id] ) {
            if (ExtensionInfo != NULL){
                if(!sm && !mSEC[ch_id] && !mSECPara) {
                    status = false;
                }
                else if(debug && mDBG[ch_id] && ExtensionInfo->getPeId() == 0xFF) {
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
            status = true;
        }
    }

    if(ch_id>=mChannelNum) {
        std::stringstream stream;
        stream<<"the address 0x"<< std::hex <<addr<< std::dec <<" refer to channel "<<ch_id<< " is exceeded the number channel of this group";
        SC_REPORT_WARNING("HI20FSGD2_Common_Guard",stream.str().c_str());
    }

    if(!status) {
        mTypeADDR = addr;
        mTypeDBG = (unsigned int)debug;
        mTypeUM = (unsigned int)um;
        mTypeSEC = (unsigned int)sm;
        mTypeSPID = spid;
        mTypeWRITE = trans_type;
        mTransErrorEvent.notify(SC_ZERO_TIME);
    }
    return status;
}//}}}

///Set start address and size for each guarded area
bool HI20FSGD2_Common_Guard::SetAreaAddr(const std::string name, const unsigned int start_addr, const unsigned int size, const unsigned int ch_id)
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
        SC_REPORT_WARNING("HI20FSGD2_Common_Guard",stream.str().c_str());
    }
    return is_valid;
}//}}}

///Check whether the address of transaction in guarded areas
bool HI20FSGD2_Common_Guard::CheckTransAddress(const unsigned int trans_addr, unsigned int &ch_id)
{//{{{
    bool result = false;
    std::string name_of_area;
    for (unsigned int i=0; i<mAreaInfo.size(); i++) {
        if ((trans_addr >= mAreaInfo[i].start_addr) && (trans_addr <= mAreaInfo[i].end_addr)) {
            name_of_area = mAreaInfo[i].area_name;
            ch_id = mAreaInfo[i].ch_id;
            std::stringstream stream;
            stream<<"Access address 0x"<<std::hex<<trans_addr<<std::dec<<" is inside  "<<name_of_area<<" area";
            SC_REPORT_INFO("HI20FSGD2_Common_Guard",stream.str().c_str());
            result = true;
            break;
        }
    }
    if (!result) {
        ch_id = 0xFFFFFFFF;
        std::stringstream stream;
        stream<<"Access address 0x"<<std::hex<<trans_addr<<" is outside of protected area";
        SC_REPORT_INFO("HI20FSGD2_Common_Guard",stream.str().c_str());
    }
    return result;
}//}}}

///Enable Reset
void HI20FSGD2_Common_Guard::EnableReset (const bool is_active)
{//{{{
    mRstState = is_active;
    if (is_active) {
        CancelEvents();
        Initialize();
    }
}//}}}

/// Cancel operation events
void HI20FSGD2_Common_Guard::CancelEvents (void)
{//{{{
    mTransErrorEvent.cancel();
}//}}}

/// Initialize internal variables
void HI20FSGD2_Common_Guard::Initialize (void)
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
    mTypeSEC  = false;
    mTypeSPID = 0;
    mTypeWRITE = false;
}//}}}

/// Set secure function exist or not
void HI20FSGD2_Common_Guard::SetSecureFunc(bool sec)
{
    mSECPara = sec;
}

/// Set clock period
void HI20FSGD2_Common_Guard::SetCLKPeriod (const sc_dt::uint64 clk_period)
{//{{{
    mCLKPeriod = clk_period;
}//}}}

/// Set access variables
void HI20FSGD2_Common_Guard::SetVAR(const std::string name, const unsigned int val, const unsigned int ch_id) {
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
    else if(name=="SEC") {
        mSEC[ch_id] = val;
    }
    else if(name=="WG") {
        mWG[ch_id] = val;
    }
    else //(name=="RG")
    {
        mRG[ch_id] = val;
    }
}

/// Get access variables
unsigned int HI20FSGD2_Common_Guard::GetVAR(const std::string name) {
    if(name=="ADDR") {
        return mTypeADDR;
    }
    else if(name=="DBG") {
        return (unsigned int)mTypeDBG;
    }
    else if(name=="UM") {
        return (unsigned int)mTypeUM;
    }
    else if(name=="SEC") {
        return (unsigned int)mTypeSEC;
    }
    else if(name=="SPID") {
        return mTypeSPID;
    }
    else //(name=="WRITE") 
    {
        return (unsigned int)mTypeWRITE;
    }
}

// vim600: set foldmethod=marker :
