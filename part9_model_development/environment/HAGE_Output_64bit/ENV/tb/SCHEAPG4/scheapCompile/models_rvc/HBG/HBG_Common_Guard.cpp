// ---------------------------------------------------------------------
// $Id: HBG_Common_Guard.cpp,v 1.14 2020/03/08 06:28:17 huyquocpham Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <cstdarg>
#include "HBG_Common_Guard.h"
#include "BusMasterIf.h"
#include <iomanip>
#include "HBG.h"

///Constructor of HBG_Common_Guard class
HBG_Common_Guard::HBG_Common_Guard(std::string name,
                         BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_AXI> *Parent,
                         BusBridgeSlaveIf<BUS_WIDTH_AXI> *SlaveIf,
                         BusMasterIf<BUS_WIDTH_AXI> *MasterIf,
                         SIM_MODE_T simmode):
                         //sc_module(name),
                         BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AXI>(Parent,SlaveIf,MasterIf)
{//{{{
    assert((MasterIf != NULL) && (SlaveIf != NULL));
    BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AXI>::setTlmTransType(simmode);
    mMasterIf = MasterIf;
    mSlaveIf = SlaveIf;
    mTransTime = SC_ZERO_TIME;
    mTrans = (TlmBasicPayload*)0;
    mIsDebug = false;

    mRstState = false;
    mCLKPeriod = 0;
    Initialize();
}//}}}

///Notify transaction completion in AT mode
void HBG_Common_Guard::TransferComplete(void)
{//{{{
    if ((BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AXI>::mTransType == AT) && (!mIsDebug)) {//AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}

///Set transaction basic info (address, data, length)
void HBG_Common_Guard::SetTrans_Basic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    outTrans.set_address(inTrans.get_address());
    outTrans.set_data_ptr(inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}

////Set transaction Axi extension
void HBG_Common_Guard::SetTrans_Axi(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmAxiExtension *inAxiExt  = (TlmAxiExtension *)0;
    TlmAxiExtension *outAxiExt = new TlmAxiExtension;
    inTrans.get_extension(inAxiExt);
    assert(inAxiExt != NULL);
    assert(outAxiExt != NULL);
    outAxiExt->setTransId(inAxiExt->getTransId());
    outTrans.set_extension(outAxiExt);
}//}}}//Set transaction Axi extension

void HBG_Common_Guard::SetTrans_G3m(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmG3mExtension *inG3mExt  = (TlmG3mExtension *)0;
    TlmG3mExtension *outG3mExt = new TlmG3mExtension;
    inTrans.get_extension(inG3mExt);
    assert(inG3mExt != NULL);
    assert(outG3mExt != NULL);
    outG3mExt->setVmId(inG3mExt->getVmId());
    outG3mExt->setTcId(inG3mExt->getTcId());
    outG3mExt->setPeId(inG3mExt->getPeId());
    outG3mExt->setSpId(inG3mExt->getSpId());
    outG3mExt->setUserMode(inG3mExt->isUserMode());
    outG3mExt->setVirtualMode(inG3mExt->isVirtualMode());
    outG3mExt->setSecure(inG3mExt->isSecure());
    outTrans.set_extension(outG3mExt);
}//}}}

// Set Secure function enable/disable
void HBG_Common_Guard::SEC_DIS(bool value)
{//{{{
    mSEC = value;
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void HBG_Common_Guard::read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    mTrans = &trans;
    assert(t != NULL);
    mTransTime = *t;
    mIsDebug = debug;
    bool is_continue = false;
    if (mCLKPeriod!=0) {
        if (debug) {
            is_continue = true;
        } else {
            if (mRstState == true) {
                TransferComplete();
                SC_REPORT_WARNING("HBG_CommonGuard","The reading transaction is fail because the reset is active");
            } else {
                is_continue = true;
            }
        }
        if (is_continue) {
            if (CheckAccessibility(trans,debug)) {
                if (debug || (BusBridgeCoreBase<64,64>::mTransType == LT)) {// LT mode
                    mMasterIf->read(trans,debug,*t,false);
                } else {//AT mode
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SetTrans_Basic(trans, *sendTrans );
                    SetTrans_Axi(trans, *sendTrans );
                    SetTrans_G3m(trans, *sendTrans );
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
void HBG_Common_Guard::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    mTrans = &trans;
    assert(t != NULL);
    mTransTime = *t;
    mIsDebug = debug;
    bool is_continue = false;
    if ((mCLKPeriod!=0)) {
        if (debug) {
            is_continue = true;
        } else {
            if (mRstState == true) {
                TransferComplete();
                SC_REPORT_WARNING("HBG_CommonGuard","The writing transaction is fail because the reset is active");
            } else {
                is_continue = true;
            }
        }
        if (is_continue) {
            if (CheckAccessibility(trans,debug)) {
                if (debug || (BusBridgeCoreBase<64,64>::mTransType == LT)) {// LT mode
                    mMasterIf->write(trans,debug,*t,false);
                } else {//AT mode
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SetTrans_Basic(trans, *sendTrans );
                    SetTrans_Axi(trans, *sendTrans );
                    SetTrans_G3m(trans, *sendTrans );
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
void HBG_Common_Guard::deleteTrans(void)
{//{{{
    TlmBasicPayload *trans;
    TlmG3mExtension *G3mExt;
    TlmAxiExtension *AxiExt;
    while(!mClearTransFifo.empty()){
        trans = mClearTransFifo.front();
        assert(trans != NULL);
        AxiExt = (TlmAxiExtension *)0;
        G3mExt = (TlmG3mExtension *)0;
        trans->get_extension(AxiExt);
        assert(AxiExt != NULL);
        trans->release_extension(AxiExt);
        trans->get_extension(G3mExt);
        assert(G3mExt != NULL);
        trans->release_extension(G3mExt);
        mRequestMap.erase(trans);
        mClearTransFifo.pop();
        delete trans;
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void HBG_Common_Guard::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
    if(!mRequestMap.empty()){
        mSlaveIf->notifyTransComp(*mRequestMap[&trans],t);
        deleteTrans();
        mClearTransFifo.push(&trans);
    }
}//}}}

///Check access ability to peripherals
bool HBG_Common_Guard::CheckAccessibility(TlmBasicPayload &trans, bool debug)
{//{{{
    bool status = true;
    bool is_read = trans.is_read();
    bool is_write = trans.is_write();
    bool trans_type;
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
    bool is_hbg_trans = CheckTransAddress(addr);
    if(is_hbg_trans ) {
        bool is_spid_enable = false;
        bool is_write_enable = false;
        bool is_read_enable = false;
        if( ((mSPID>>(spid))&0x1) == 1 ){
            is_spid_enable = true;
        }
        if( mWG || is_spid_enable) {
            is_write_enable = true;
        }
        if( mRG || is_spid_enable) {
            is_read_enable = true;
        }

        if( mGEN ) {
            if(!sm && !mSEC) {
                status = false;
            }
            else if(debug && mDBG && (ExtensionInfo->getPeId() == DEBUG_MASTER_BMID)) {
                status = true;
            }
            else if(um && !mUM) {
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
            status = true;
        }
    }

    if(!status) {
        mTypeADDR = addr;
        mTypeDBG = debug;
        mTypeUM = um;
        mTypeSEC = sm;
        mTypeSPID = spid;
        mTypeWRITE = trans_type;
        mTransErrorEvent.notify(SC_ZERO_TIME);
    }
    return status;
}//}}}

///Set start address and size for each guarded area
bool HBG_Common_Guard::SetAreaAddr(const std::string name, const unsigned int start_addr, const unsigned int size)
{//{{{
    unsigned int start_address = start_addr;
    unsigned int end_address = start_addr + size - 1;
    AreaInformation temp;
    temp.area_name = name;
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
        SC_REPORT_WARNING("HBG_CommonGuard",stream.str().c_str());
    }
    return is_valid;
}//}}}

///Check whether the address of transaction in guarded areas
bool HBG_Common_Guard::CheckTransAddress(const unsigned int trans_addr)
{//{{{
    bool result = false;
    std::string name_of_area;
    for (unsigned int i=0; i<mAreaInfo.size(); i++) {
        if ((trans_addr >= mAreaInfo[i].start_addr) && (trans_addr <= mAreaInfo[i].end_addr)) {
            name_of_area = mAreaInfo[i].area_name;
            std::stringstream stream;
            stream<<"Access address 0x"<<std::hex<<trans_addr<<std::dec<<" is inside  "<<name_of_area<<" area";
            SC_REPORT_INFO("HBG_CommonGuard",stream.str().c_str());
            result = true;
            break;
        }
    }
    if (!result) {
        std::stringstream stream;
        stream<<"Access address 0x"<<std::hex<<trans_addr<<" is outside of protected area";
        SC_REPORT_INFO("HBG_CommonGuard",stream.str().c_str());
    }
    return result;
}//}}}

void HBG_Common_Guard::EnableReset (const bool is_active)
{//{{{
    mRstState = is_active;
    if (is_active) {
        CancelEvents();
        Initialize();
    }
}//}}}

/// Cancel operation events
void HBG_Common_Guard::CancelEvents (void)
{//{{{
    mTransErrorEvent.cancel();
}//}}}

/// Initialize internal variables
void HBG_Common_Guard::Initialize (void)
{//{{{

    mSPID = 0;
    mGEN = false;
    mDBG = false;
    mUM = false;
    mWG = false;
    mRG = false;
    mSEC = true;
    
    mTypeADDR    = 0;
    mTypeDBG = false;
    mTypeUM  = false;
    mTypeSEC  = false;
    mTypeSPID = 0;
    mTypeWRITE = false;
}//}}}

void HBG_Common_Guard::SetCLKPeriod (const sc_dt::uint64 clk_period)
{//{{{
    mCLKPeriod = clk_period;
}//}}}

void HBG_Common_Guard::SetVAR(const std::string name, const unsigned int val) 
{//{{{
    if(name=="SPID") {
        mSPID = val;
    }
    else if(name=="GEN") {
        mGEN = val;
    }
    else if(name=="DBG") {
        mDBG = val;
    }
    else if(name=="UM") {
       mUM = val;
    }
    else if(name=="WG") {
        mWG = val;
    }
    else if(name=="RG") {
        mRG = val;
    }
}//}}}

unsigned int HBG_Common_Guard::GetVAR(const std::string name) 
{//{{{
    if(name=="ADDR") {
        return (unsigned int)mTypeADDR;
    }
    else if(name=="DBG") {
        return (unsigned int)mTypeDBG;
    }
    else if(name=="UM") {
        return (unsigned int)mTypeUM;
    }
    else if(name=="SPID") {
        return (unsigned int)mTypeSPID;
    }
    else if(name=="WRITE") {
        return (unsigned int)mTypeWRITE;
    }
    else if(name=="SEC") {
        return (unsigned int)mTypeSEC;
    }
    else
    {
        return 0;
    }
}//}}}

// vim600: set foldmethod=marker :
