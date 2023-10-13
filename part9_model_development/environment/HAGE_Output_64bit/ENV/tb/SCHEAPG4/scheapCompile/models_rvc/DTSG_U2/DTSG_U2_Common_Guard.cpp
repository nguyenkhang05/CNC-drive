// ---------------------------------------------------------------------
// $Id: DTSG_U2_Common_Guard.cpp,v 1.14 2020/01/09 09:00:01 synguyen Exp $
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
#include "DTSG_U2_Common_Guard.h"
#include "BusMasterIf.h"
#include <iomanip>
#include "DTSG_U2.h"

///Constructor of DTSG_U2_Common_Guard class
DTSG_U2_Common_Guard::DTSG_U2_Common_Guard(std::string name,
                         DTSG_U2_AgentController* ptr_DTSG_U2_AgentController,
                         BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB> *Parent,
                         BusBridgeSlaveIf<BUS_WIDTH_APB> *SlaveIf,
                         BusMasterIf<BUS_WIDTH_APB> *MasterIf,
                         unsigned int numOfCh,
                         SIM_MODE_T simmode):
                         mAreaInfo(),
                         BusBridgeCoreBase<BUS_WIDTH_APB, BUS_WIDTH_APB>(Parent,SlaveIf, MasterIf)
{//{{{
    assert((ptr_DTSG_U2_AgentController != NULL) && (MasterIf != NULL) && (SlaveIf != NULL));
    mDTSG_U2_AgentController = ptr_DTSG_U2_AgentController;
    BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::setTlmTransType(simmode);
    mMasterIf = MasterIf;
    mSlaveIf = SlaveIf;
    mTransTime = SC_ZERO_TIME;
    mTrans = (TlmBasicPayload*)0;
    mIsDebug = false;
    mNumOfChannel = numOfCh;
    mRstState = false;
    mCLKPeriod = 0;
    Initialize();
}//}}}

/// DTSG_U2_Common_Guard functional destructor
DTSG_U2_Common_Guard::~DTSG_U2_Common_Guard()
{
    deleteTrans();
}
///Notify transaction completion in AT mode
void DTSG_U2_Common_Guard::TransferComplete(void)
{//{{{
    if ((BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::mTransType == AT) && (!mIsDebug)) {//AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans, mTransTime);
    }
}//}}}

///Set transaction basic info (address, data, length)
void DTSG_U2_Common_Guard::SetTransBasicModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    outTrans.set_address(inTrans.get_address());
    outTrans.set_data_ptr(inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}

///Set G3transaction info
void DTSG_U2_Common_Guard::SetTransG3mModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmG3mExtension *inG3mExt  = (TlmG3mExtension *)0;
    TlmG3mExtension *outG3mExt = new TlmG3mExtension;
    assert(outG3mExt != NULL);
    inTrans.get_extension(inG3mExt);
    if (inG3mExt != NULL) {
        outG3mExt->setVmId(inG3mExt->getVmId());
        outG3mExt->setTcId(inG3mExt->getTcId());
        outG3mExt->setPeId(inG3mExt->getPeId());
        outG3mExt->setSpId(inG3mExt->getSpId());
        outG3mExt->setUserMode(inG3mExt->isUserMode());
        outG3mExt->setVirtualMode(inG3mExt->isVirtualMode());
        outG3mExt->setSecure(inG3mExt->isSecure());
        outTrans.set_extension(outG3mExt);
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void DTSG_U2_Common_Guard::read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    mTrans           = &trans;
    assert(t != NULL);
    mTransTime       = *t;
    mIsDebug         = debug;
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
                SC_REPORT_WARNING("DTSG_U2_Common_Guard","The reading transaction is fail because the reset is active or zero clock");
            }
        }
    } else {
        is_continue = true;
    }

    if (is_continue) {
            if (CheckAccessibility(trans,debug)) {
                if (debug || (BusBridgeCoreBase<BUS_WIDTH_APB, BUS_WIDTH_APB>::mTransType == LT)) {// LT mode
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
void DTSG_U2_Common_Guard::write(unsigned int offsetAddress, TlmBasicPayload& trans,BusTime_t* t, bool debug)
{//{{{
    mTrans           = &trans;
    assert(t != NULL);
    mTransTime       = *t;
    mIsDebug         = debug;
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
                SC_REPORT_WARNING("DTSG_U2_Common_Guard","The writing transaction is fail because the reset is active or zero clock");
            }
        }
    } else {
        is_continue = true;
    }
    
    if (is_continue) {
        if (CheckAccessibility(trans, debug)) {
            if (debug || (BusBridgeCoreBase<BUS_WIDTH_APB, BUS_WIDTH_APB>::mTransType == LT)) {// LT mode
                mMasterIf->write(trans, debug, *t, false);
            } else {//AT mode
                TlmBasicPayload *sendTrans = new TlmBasicPayload;
                SetTransBasicModel(trans, *sendTrans );
                SetTransG3mModel(trans, *sendTrans );
                mRequestMap[sendTrans] = &trans;
                mMasterIf->write(*sendTrans, debug, *t, false);
            }
        } else {
            TransferComplete();
        }
    }

}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void DTSG_U2_Common_Guard::deleteTrans(void)
{//{{{
    TlmBasicPayload *trans;
    while(!mClearTransFifo.empty()){
        trans  = mClearTransFifo.front();
        assert(trans != NULL);
        trans->free_all_extensions();
        mRequestMap.erase(trans);
        mClearTransFifo.pop();
        delete trans;
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void DTSG_U2_Common_Guard::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
    if(!mRequestMap.empty()){
        mSlaveIf->notifyTransComp(*mRequestMap[&trans], t);
        deleteTrans();
        mClearTransFifo.push(&trans);
    }
}//}}}

///Check access ability to peripherals
bool DTSG_U2_Common_Guard::CheckAccessibility(TlmBasicPayload &trans, bool debug)
{//{{{
    bool status           = true;
    bool is_read          = trans.is_read();
    bool is_write         = trans.is_write();
    bool trans_type_write = false;
    unsigned int ch_id    = 0;
    
    if ((is_read) && (!is_write)){
        trans_type_write = false; // read trans
    }
    else {
        trans_type_write = true; // write trans
    }
    bool            um    = true;
    bool            sm    = false;
    unsigned int    spid  = 0x0;
    unsigned int    peid  = 0x0;
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    
    if(ExtensionInfo != NULL){
        um    = ExtensionInfo->isUserMode();
        sm    = ExtensionInfo->isSecure();
        spid  = ExtensionInfo->getSpId();
        peid  = ExtensionInfo->getPeId();
    }
    unsigned int    addr  = (unsigned int)trans.get_address();
    bool is_DTSG_U2_trans = CheckTransAddress(addr, ch_id);
    if(is_DTSG_U2_trans != false) {
        bool ch_RG  = false;
        bool ch_WG  = false;
        bool ch_UM = false;
        bool ch_DBG = false;
        bool ch_GEN = false;
        unsigned int ch_MPID = 0;
        mDTSG_U2_AgentController->GetDTSGPROT(ch_id, ch_MPID, ch_GEN, ch_DBG, ch_UM, ch_WG, ch_RG);
        if( ch_GEN != false) {
            if(debug && ch_DBG && (peid == DEBUG_MASTER_BMID)) {
                status = true;
            }
            else if(um && !ch_UM) {
                status = false;
            }
            else {
                if (ch_MPID == 0 && ch_WG == 0 && ch_RG == 0) {
                    status = false;
                } else {
                    bool is_spid_enable  = false;
                    bool is_write_enable = false;
                    bool is_read_enable  = false;
                    
                    for (unsigned char id = 0; id < 8; id ++) {
                        is_spid_enable   = false;
                        is_write_enable  = false;
                        is_read_enable   = false;
                        
                        // Enable DTSGMPID_m or not
                        if ((bool)((ch_MPID >> id) & 0x1)) {
                            unsigned int mpid_spid = 0;
                            mpid_spid = mDTSG_U2_AgentController->GetDTSGMPID_SPID(id);
                            if(mpid_spid == spid ){
                                is_spid_enable = true;
                            }
                        }   
                        if( ch_WG || is_spid_enable) {
                            is_write_enable = true;
                        }
                        if( ch_RG || is_spid_enable) {
                            is_read_enable = true;
                        }
                        if ((trans_type_write && !is_write_enable) || (!trans_type_write && !is_read_enable)) {
                            status = false;
                        } else {
                            status = true; // PASSED SPID checking
                            break;
                        }
                    }
                }
            }
        }
        else {
            status = true;
        }
    }

    if(status == false) {
        mTypeADDR  = addr;
        mTypeSPID  = spid;
        mTypeDBG   = (unsigned int)debug;
        mTypeUM    = (unsigned int)um;
        mTypeSEC   = (unsigned int)sm;
        mTypeWRITE = (unsigned int)trans_type_write;
        mTransErrorEvent.notify(SC_ZERO_TIME);
    }
    return status;
}//}}}

///Set start address and size for each guarded area
void DTSG_U2_Common_Guard::SetAreaAddr(const unsigned int start_addr, const unsigned int size, const unsigned int channel_index)
{//{{{
    unsigned int start_address = start_addr;
    unsigned int end_address   = start_addr + size - 1;
    bool replace_setting = false;
    bool valid_setting = true;
    unsigned int replace_idx = 0;
    AreaInformation AreaInput;
    
    AreaInput.start_addr  = start_address;
    AreaInput.end_addr    = end_address;
    AreaInput.channel_idx = channel_index;

    for (unsigned int i = 0; i < mAreaInfo.size(); i++) {
        if (channel_index == mAreaInfo[i].channel_idx) { // Update exist protected range for a channel
            replace_setting = true;
            replace_idx = i;
        } else if (((start_address > mAreaInfo[i].end_addr) || (end_address < mAreaInfo[i].start_addr))){
            valid_setting = true;
        } else {
            valid_setting = false;
            break;
        }
    }
    if (replace_setting != false && valid_setting != false) {
        mAreaInfo[replace_idx].start_addr = start_address;
        mAreaInfo[replace_idx].end_addr = end_address;
    } else if (valid_setting != false) {
        mAreaInfo.push_back(AreaInput);
    } else {
        std::stringstream stream;
        stream << "Address 0x" << std::hex << start_address << " and size 0x" << size << " is overlapped with another area. This setting is ignored";
        SC_REPORT_WARNING("DTSG_U2_Common_Guard", stream.str().c_str());
    }
}//}}}

///Check whether the address of transaction in guarded areas
bool DTSG_U2_Common_Guard::CheckTransAddress(const unsigned int trans_addr, unsigned int &channel_idx)
{//{{{
    bool result = false;
    std::string name_of_area;
    for (unsigned int i = 0; i<mAreaInfo.size(); i++) {
        if ((trans_addr >= mAreaInfo[i].start_addr) && (trans_addr <= mAreaInfo[i].end_addr)) {
            std::stringstream stream;
            channel_idx = mAreaInfo[i].channel_idx;
            if (channel_idx < mNumOfChannel - 1) {
                stream << "Access address 0x" << std::hex << trans_addr << std::dec << " is inside the protected area of channel " << channel_idx;
            } else {
                stream << "Access address 0x" << std::hex << trans_addr << std::dec << " is inside the protected area of channel GR";
            }
            SC_REPORT_INFO("DTSG_U2_Common_Guard",stream.str().c_str());
            result = true;
            break;
        }
    }
    if (!result) {
        std::stringstream stream;
        channel_idx = 0xFFFFFFFF;
        stream << "Access address 0x" << std::hex << trans_addr << " is outside the protected range of model ";
        SC_REPORT_WARNING("DTSG_U2_Common_Guard", stream.str().c_str());
    }
    return result;
}//}}}

///Enable reset signal
void DTSG_U2_Common_Guard::EnableReset (const bool is_active)
{//{{{
    mRstState = is_active;
    if (is_active) {
        CancelEvents();
        Initialize();
    }
}//}}}

///Cancel operation events
void DTSG_U2_Common_Guard::CancelEvents (void)
{//{{{
    mTransErrorEvent.cancel();
}//}}}

///Initialize internal variables
void DTSG_U2_Common_Guard::Initialize (void)
{//{{{
    mTypeADDR    = 0;
    mTypeDBG     = false;
    mTypeUM      = false;
    mTypeSEC     = false;
    mTypeSPID    = 0;
    mTypeWRITE   = false;
}//}}}

void DTSG_U2_Common_Guard::SetCLKPeriod (const sc_dt::uint64 clk_period)
{//{{{
    mCLKPeriod = clk_period;
}//}}}

///Get transaction extension information and address
unsigned int DTSG_U2_Common_Guard::GetVAR(const std::string name) 
{//{{{
    if(name == "ADDR") {
        return mTypeADDR;
    }
    else if(name == "SEC") {
        return (unsigned int)mTypeSEC;
    }
    else if(name == "DBG") {
        return (unsigned int)mTypeDBG;
    }
    else if(name == "UM") {
        return (unsigned int)mTypeUM;
    }
    else if(name == "SPID") {
        return mTypeSPID;
    }
    else { // WRITE
        return (unsigned int)mTypeWRITE;
    }
}//}}}

// vim600: set foldmethod=marker :
