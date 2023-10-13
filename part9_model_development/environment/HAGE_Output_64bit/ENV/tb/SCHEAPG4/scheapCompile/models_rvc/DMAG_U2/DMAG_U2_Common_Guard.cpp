// ---------------------------------------------------------------------
// $Id: DMAG_U2_Common_Guard.cpp,v 1.11 2020/02/20 06:05:55 hadinhtran Exp $
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
#include "DMAG_U2_AgentController.h"
#include "DMAG_U2_Common_Guard.h"
#include "BusMasterIf.h"
#include <iomanip>

///Constructor of DMAG_U2_Common_Guard class
DMAG_U2_Common_Guard::DMAG_U2_Common_Guard(std::string name,
                         DMAG_U2_AgentController* DMAG_U2_AgentControllerPtr,
                         BusBridgeModuleBase<32,32> *Parent,
                         BusBridgeSlaveIf<32> *SlaveIf,
                         BusMasterIf<32> *MasterIf,
                         SIM_MODE_T simmode):
                         BusBridgeCoreBase<32,32>(Parent,SlaveIf,MasterIf)
{//{{{
    assert((DMAG_U2_AgentControllerPtr != NULL) && (MasterIf != NULL) && (SlaveIf != NULL));
    mDMAG_U2_AgentController = DMAG_U2_AgentControllerPtr;
    BusBridgeCoreBase<32,32>::setTlmTransType(simmode);
    mMasterIf = MasterIf;
    mSlaveIf = SlaveIf;
    mTransTime = SC_ZERO_TIME;
    mTrans = (TlmBasicPayload*)0;
    mIsDebug = false;
    mResetState = false;
    Initialize();
}//}}}

///Notify transaction completion in AT mode
void DMAG_U2_Common_Guard::TransferComplete(void)
{//{{{
    if ((BusBridgeCoreBase<32,32>::mTransType == AT) && (!mIsDebug)) {//AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}

///Set transaction basic info (address, data, length)
void DMAG_U2_Common_Guard::SetTransBasicModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    outTrans.set_address(inTrans.get_address());
    outTrans.set_data_ptr(inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}

///Set transaction G3M extension
void DMAG_U2_Common_Guard::SetTransG3mModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
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
void DMAG_U2_Common_Guard::read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
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
    if ( (!mDMAG_U2_AgentController->CheckClockPeriod()) || (mDMAG_U2_AgentController->GetResetStatus()) ){
        if (pExtInfo != NULL) {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == pExtInfo->getPeId())) {
                is_continue = true;
            } else {
                TransferComplete();
                SC_REPORT_WARNING("DMAG_U2_Common_Guard", "Cannot access register when clock is 0 or reset port is asserted.\n");
            }
        }
    } else {
        is_continue = true;
    }

    if (is_continue) {
            if (AccessJudgement(trans,debug)) {
                SC_REPORT_INFO("DMAG_U2_Common_Guard", "Access is permitted.\n");
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
                SC_REPORT_INFO("DMAG_U2_Common_Guard", "Access is prohibited.\n");
            }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void DMAG_U2_Common_Guard::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
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
    if ( (!mDMAG_U2_AgentController->CheckClockPeriod()) || (mDMAG_U2_AgentController->GetResetStatus()) ){
        if (pExtInfo != NULL) {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == pExtInfo->getPeId())) {
                is_continue = true;
            } else {
                TransferComplete();
                SC_REPORT_WARNING("DMAG_U2_Common_Guard", "Cannot access register when clock is 0 or reset port is asserted.\n");
            }
        }
    } else {
        is_continue = true;
    }

    if (is_continue) {
        if (AccessJudgement(trans,debug)) {
            SC_REPORT_INFO("DMAG_U2_Common_Guard", "Access is permitted.\n");
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
            SC_REPORT_INFO("DMAG_U2_Common_Guard", "Access is prohibited.\n");
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void DMAG_U2_Common_Guard::deleteTrans(void)
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
void DMAG_U2_Common_Guard::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
    if(!mRequestMap.empty()){
        mSlaveIf->notifyTransComp(*mRequestMap[&trans],t);
        deleteTrans();
        mClearTransFifo.push(&trans);
    }
}//}}}

///Check access ability 
bool DMAG_U2_Common_Guard::AccessJudgement(TlmBasicPayload &trans, bool debug)
{//{{{
    bool is_access_permitted = true;
    // bool is_pe_trans = true;
    bool is_read = trans.is_read();
    bool is_write = trans.is_write();
    bool access_type;
    if ((is_read) && (!is_write)){
        access_type = false; // read trans
    }
    else {
        access_type = true; // write trans
    }

    bool access_um = true;
    bool access_sm = false;
    unsigned int access_spid = 0x0;
    unsigned int access_peid = 0x0;
    /*Extract extension info of transaction */
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);

    if(ExtensionInfo != NULL) {
        access_um = ExtensionInfo->isUserMode();
        access_spid = ExtensionInfo->getSpId();
        access_peid = ExtensionInfo->getPeId();
        access_sm = ExtensionInfo->isSecure();
    }
    unsigned int access_addr = (unsigned int)trans.get_address();
    unsigned int ch_id;

    /** Check if accesss target address is in protected area and channel id is valid */
    bool is_dma_trans = CheckTransAddress(access_addr, ch_id);
    if(is_dma_trans && (ch_id < mDMAG_U2_AgentController->mChannelNum)) {
        /* Checking channel permission */
        bool gen_bit = false;
        bool dbg_bit = false;
        bool um_bit = false;
        bool wg_bit = false;
        bool rg_bit = false;
        unsigned int spid_value = mDMAG_U2_AgentController->getPermittedSPID(ch_id);
        mDMAG_U2_AgentController->getDMAGPROT_n(ch_id, gen_bit, dbg_bit, um_bit, wg_bit, rg_bit);

        if (gen_bit) {
            if (debug && dbg_bit && (access_peid == DEBUG_MASTER_BMID)) {
                is_access_permitted = true;
            }
            else if (access_um && !um_bit) {
                is_access_permitted = false;
            }
            else if (1 == ((spid_value>>access_spid) & 0x1)) {
                is_access_permitted = true;
            }
            else {
                if ((is_write && wg_bit) || (is_read && rg_bit)) {
                    is_access_permitted = true;
                }
                else {
                    is_access_permitted = false;
                }
            }
        }
    }

    /* If guard error */
    if(!is_access_permitted) {
        /* Update informations access to local vars*/
        mTypeADDR = access_addr;
        mTypeSEC = access_sm;
        mTypeDBG = debug;
        mTypeUM = access_um;
        mTypeSPID = access_spid;
        mTypeWRITE = access_type;
        // Notify transaction error event 
        mDMAG_U2_AgentController->NotifyTransErrorEvent();
    }
    return is_access_permitted;
}//}}}

///Set start address and size for each guard channel
bool DMAG_U2_Common_Guard::SetAreaAddr(const unsigned int ch_id, const unsigned int start_addr, const unsigned int size)
{//{{{
    bool is_valid = false;
    bool is_reWrite = false;
    unsigned int reWrite_idx = 0;
    unsigned int start_address = start_addr;
    unsigned int end_address = start_addr + size - 1;
    AreaInformation temp;
    temp.ch_id = ch_id;
    temp.start_addr = start_address;
    temp.end_addr = end_address;
    is_valid = true;

    for (unsigned int i = 0; i<mAreaInfo.size(); i++) {
        if (ch_id == mAreaInfo[i].ch_id) {
            is_reWrite = true;
            reWrite_idx = i;
        }
        else if ((start_address > mAreaInfo[i].end_addr) || (end_address < mAreaInfo[i].start_addr)) {
            is_valid = true;
        }
        else {
            is_valid = false;
            break;
        }
    }

    if (is_reWrite && is_valid) {
        mAreaInfo[reWrite_idx].start_addr = temp.start_addr;
        mAreaInfo[reWrite_idx].end_addr   = temp.end_addr;
    }
    else if (is_valid) {
        mAreaInfo.push_back(temp);
    }
    else {
        std::stringstream stream;
        stream << "Address 0x" << std::hex << start_address << " and size 0x" << std::hex << size << " are invalid. They are overlapped with another area";
        SC_REPORT_WARNING("DMAG_U2_Common_Guard", stream.str().c_str());
    }
    return is_valid;
}//}}}

///Check whether the target address of transaction in guard areas
bool DMAG_U2_Common_Guard::CheckTransAddress(const unsigned int trans_addr, unsigned int &ch_id)
{//{{{
    bool result = false;
    for (unsigned int i=0; i<mAreaInfo.size(); i++) {
        if ((trans_addr >= mAreaInfo[i].start_addr) && (trans_addr <= mAreaInfo[i].end_addr)) {
            ch_id = mAreaInfo[i].ch_id;
            std::stringstream stream;
            stream << "Access address 0x" << std::hex << trans_addr << std::dec << " is inside the protected area of channel " << ch_id;
            SC_REPORT_INFO("DMAG_U2_Common_Guard",stream.str().c_str());
            result = true;
            break;
        }
    }
    if (!result) {
        ch_id = 0xFFFFFFFF;
        std::stringstream stream;
        stream << "Access address 0x" << std::hex << trans_addr << " is outside the protected area of channels";
        SC_REPORT_INFO("DMAG_U2_Common_Guard",stream.str().c_str());
    }
    return result;
}//}}}

///Reset operation of DMAG_U2_Common_Guard class
void DMAG_U2_Common_Guard::EnableReset (const bool is_active)
{//{{{
    mResetState = is_active;
    if (is_active) {
        Initialize();
    }
}//}}}

/// Initialize internal variables
void DMAG_U2_Common_Guard::Initialize (void)
{//{{{
    mTypeADDR   = 0;
    mTypeSEC    = false;
    mTypeDBG    = false;
    mTypeUM     = false;
    mTypeSPID   = 0;
    mTypeWRITE  = false;
}//}}}

/// Get value of variables which stored information of error transaction
unsigned int DMAG_U2_Common_Guard::GetVAR(const std::string name) {
    if(name=="ADDR") {
        return mTypeADDR;
    }
    else if (name == "SEC") {
        return (unsigned int)mTypeSEC;
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
    else{ // name is "WRITE"
        return (unsigned int)mTypeWRITE;
    }
}
// vim600: set foldmethod=marker :
