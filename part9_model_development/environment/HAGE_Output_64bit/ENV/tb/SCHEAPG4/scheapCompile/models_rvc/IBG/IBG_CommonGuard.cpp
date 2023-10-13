// ---------------------------------------------------------------------
// $Id: IBG_CommonGuard.cpp,v 1.7 2019/05/30 01:49:32 nhuvo Exp $
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

#include "IBG_CommonGuard.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <string>
using std::string;
#include "OSCI2.h" // copy attributes

//------------------------------------------------------------------------------
/**@brief the less-than comparator defines the order of address ranges, decides
*        which is less address range
* @param[in] lhs the left hand side operand
* @param[in] rhs the right hand side operand
* @return true if the left hand side operand is less than the right hand side operand
*/
bool IBG_CommonGuard_AddRngLessThanCompare::operator() (const IBG_CommonGuard_AddRng& lhs, const IBG_CommonGuard_AddRng& rhs) const {//{{{
    return lhs.first < rhs.second; // higher address of lhs is less than lower address of rhs
}//}}}
//------------------------------------------------------------------------------
/**@brief Constructor of the bus guard
*/
IBG_CommonGuard::IBG_CommonGuard(std::string                       name
                                 ,IbgCommonGuardBusBridgeModule*    Parent
                                 ,IbgCommonGuardBusBridgeSlaveIf*   SlaveIf
                                 ,IbgCommonGuardBusMasterIf*        MasterIf
                                 ,SIM_MODE_T                        simmode
                                 ,IBG_CommonGuardAgentControllerIf* AgentController_ptr
                                 )
                                 : IbgCommonGuardBusBridgeCore(Parent,SlaveIf,MasterIf)
                                 , mSlaveIf         (SlaveIf)
                                 , mMasterIf        (MasterIf)
                                 , mAgentController (AgentController_ptr)
{//{{{
    sc_assert((mAgentController != NULL) && (mMasterIf != NULL) && (mSlaveIf != NULL));
    IbgCommonGuardBusBridgeCore::setTlmTransType(simmode);
    mTransTime = SC_ZERO_TIME;
    mTrans = (TlmBasicPayload*)0;
    mIsDebug = false;

    mAgentController->printMsg("info","finish instantiating the common guard\n");
}//}}}
//------------------------------------------------------------------------------
/**@brief Destructor
*/
IBG_CommonGuard::~IBG_CommonGuard() {}
//------------------------------------------------------------------------------
/**@brief Notify transaction completion in AT mode
*/
void IBG_CommonGuard::TransferComplete (void) 
{//{{{
    if ((IbgCommonGuardBusBridgeCore::mTransType == AT) && (!mIsDebug)) {
        //AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Copy basic info (address, data, length)from the input transaction into the output transaction
*/
void IBG_CommonGuard::SettingTransBasic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    outTrans.set_address(inTrans.get_address());
    outTrans.set_data_ptr(inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}

//------------------------------------------------------------------------------
/**@brief Copy extension information from the input transaction into the output transaction
*/
void IBG_CommonGuard::SetTransExt0(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans) 
{//{{{
    TlmG3mExtension* mG3mExt = (TlmG3mExtension*)0;
    inTrans.get_extension(mG3mExt);
    if (mG3mExt != NULL) {
        TlmG3mExtension* mG3mExtOut = static_cast<TlmG3mExtension*>(mG3mExt->clone());
        outTrans.set_extension(mG3mExtOut);
        mAgentController->printMsg("info", "copy G3m extension side-band info\n");
    }

    TlmVciExtension* mVciExt = (TlmVciExtension*)0;
    inTrans.get_extension(mVciExt);
    if (mVciExt != NULL) {
        TlmVciExtension* mVciExtOut = static_cast<TlmVciExtension*>(mVciExt->clone());
        outTrans.set_extension(mVciExtOut);
        mAgentController->printMsg("info", "copy Vci extension side-band info\n");
    }
}//}}}
//##############################################################################
// Overwrite virtual functions of BusBridgeCoreBase class
//##############################################################################
//------------------------------------------------------------------------------
/**@brief Overwrite virtual functions of BusBridgeCoreBase class
* @param[in] offsetAddress offset address
* @param[inout] trans the transaction / payload
* @param[inout] t timestamp associated to the transaction
* @param[in] debug indicates that whether the transaction is debug transaction or not
*/
void IBG_CommonGuard::read (unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug) 
{//{{{
    sc_assert(t != NULL);

    mAgentController->printMsg("info","start processing a read transaction \n");

    mTrans     = &trans;
    mTransTime = *t;
    mIsDebug   = debug;

    bool is_valid_trans = false;
    bool is_continue = false;
    if (mAgentController->CheckGuardClock()) {
        if (debug) {
            is_continue = true;
        } else {
            if (mAgentController->GetGuardResetStatus()) {
                TransferComplete();
                mAgentController->printMsg("error","The reading transaction is fail because the reset is active\n");
            } else {
                is_continue = true;
            }
        }
        if (is_continue) {
            if (this->CheckAccessPermission(trans,debug,is_valid_trans)) {
                mAgentController->printMsg("info","forward the read transaction\n");
                if (debug || (IbgCommonGuardBusBridgeCore::mTransType == LT)) {// LT mode
                    mAgentController->printMsg("info","forward a read transaction in LT mode (or debug transactions)\n");
                    mMasterIf->read(trans,debug,*t,false);
                } else {//AT mode
                    mAgentController->printMsg("info","forward a read transaction in AT mode (or non-debug transactions)\n");
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SettingTransBasic(trans, *sendTrans );
                    SetTransExt0 (trans, *sendTrans );
                    //sendTrans->update_extensions_from(trans);
                    mRequestMap[sendTrans] = &trans;
                    mMasterIf->read(*sendTrans,debug,*t,false);
                }
            } else {
                mAgentController->printMsg("info","discard the read transaction\n");
                TransferComplete();
            }

        }
    }

    mAgentController->printMsg("info","finish processing a read transaction \n");

}//}}}
//------------------------------------------------------------------------------
/**@brief Overwrite virtual functions of BusBridgeCoreBase class
*/
void IBG_CommonGuard::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug) 
{//{{{
    assert(t != NULL);

    mAgentController->printMsg("info","start processing a write transaction\n");

    mTrans = &trans;
    mTransTime = *t;
    bool is_valid_trans = false;
    mIsDebug = debug;
    bool is_continue = false;
    if (mAgentController->CheckGuardClock()) {
        if (debug) {
            is_continue = true;
        } else {
            if (mAgentController->GetGuardResetStatus()) {
                TransferComplete();
                mAgentController->printMsg("error","The writing transaction is fail because the reset is active\n");
            } else {
                is_continue = true;
            }
        }
        if (is_continue) {
            if (this->CheckAccessPermission(trans,debug,is_valid_trans)) {
                mAgentController->printMsg("info","forward the write transaction\n");
                if (debug || (IbgCommonGuardBusBridgeCore::mTransType == LT)) {// LT mode
                    mAgentController->printMsg("info","forward a write transaction in LT mode (or debug transactions)\n");
                    mMasterIf->write(trans,debug,*t,false);
                } else {//AT mode
                    mAgentController->printMsg("info","forward a write transaction in AT mode (or non-debug transactions)\n");
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SettingTransBasic(trans, *sendTrans );
                    SetTransExt0 (trans, *sendTrans );
                    //sendTrans->update_extensions_from(trans);
                    mRequestMap[sendTrans] = &trans;
                    mMasterIf->write(*sendTrans,debug,*t,false);
                }
            } else {
                mAgentController->printMsg("info","discard the write transaction\n");
                TransferComplete();
            }

        }
    }

    mAgentController->printMsg("info","finish processing a write transaction\n");

}//}}}
//------------------------------------------------------------------------------
/**@brief Overwrite virtual functions of BusBridgeCoreBase class
*/
void IBG_CommonGuard::deleteTrans(void) 
{//{{{
    TlmBasicPayload *trans;

    while(!mClearTransFifo.empty()){
        trans = mClearTransFifo.front();
        if(trans!=NULL) {
            trans->free_all_extensions(); 
            mRequestMap.erase(trans);
            mClearTransFifo.pop();
            delete trans;
        }
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Overwrite virtual functions of BusBridgeCoreBase class
*/
void IBG_CommonGuard::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t) {//{{{
    if(!mRequestMap.empty()){
        mSlaveIf->notifyTransComp(*mRequestMap[&trans],t);
        deleteTrans();
        mClearTransFifo.push(&trans);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief add a address range into the map of protected range.
* @details
*  The address range is specified by the start address and a size.
*  An address range is belong to a channel of a group of the protected
*  peripheral.
*  This method is to add an address range into the protected address map.
*  The new range must not overlap any range which has been added before.
* @param[in] groupName name of the group to which the specified address range belongs
* @param[in] ch_id id of the channel to which the specified address range belongs
* @param[in] start_addr start address of the desired address range
* @param[in] size size of the desired address range
* @return true if the desired address does not overlap any address range
*         which has already exist in the map. Otherwise, the new address wont
*         be added into the map.
*/
bool IBG_CommonGuard::SetAreaAddr(const std::string groupName
                                  ,const uint32_t    channelId
                                  ,const uint32_t    start_addr
                                  ,const uint32_t    size) 
{//{{{
    /// -# validate the given address range
    uint32_t lower_addr  = start_addr;
    uint32_t higher_addr = start_addr + size - 1;
    if (lower_addr > higher_addr) {
        mAgentController->printMsg("warning", "the address range is invalid\n");
        return false;
    }

    /// -# ensure that the given address range does not overlaps any range already inserted.
    IBG_CommonGuard_AddRng key = IBG_CommonGuard_AddRng(higher_addr, lower_addr);
    if (addrMap.find(key) != addrMap.end()) {
        mAgentController->printMsg("warning", "the address range overlapped with the previous defined one\n");
        return false;
    }

    /// -# add the range into the address map
    IBG_CommonGuard_RngId value = IBG_CommonGuard_RngId(groupName, channelId);
    addrMap[key] = value;

    mAgentController->printMsg("info", "a new address range is added\n");
    return true;
}//}}}
//------------------------------------------------------------------------------
/**@brief Check access ability to peripherals
* @details the method validates legality of the transaction before it is forward
*          to the protected address range or discarded.
* @param[in] trans the received transaction to be checked
* @param[in] debug indicate whether the transaction under check is for debug or not
* @param[out] is_legal_trans stores result of the validation process
* @return true if the received transaction is legal
*/
bool IBG_CommonGuard::CheckAccessPermission(TlmBasicPayload &trans, const bool debug, bool &is_legal_trans) 
{//{{{
    mAgentController->printMsg("info", "start check access permission\n");

    /// -# get G3m extended information of the receive transaction
    TlmG3mExtension* ext = (TlmG3mExtension*)0;
    mAgentController->printMsg("info", "try to get G3m extension side-band info\n");
    trans.get_extension(ext);
    assert(ext != NULL);
    mAgentController->printMsg("info", "try to get address info\n");
    uint32_t addr = (uint32_t)trans.get_address();

    mAgentController->printMsg("info", "validate address of the received transaction\n");
    /// -# validate address of the received transaction,
    IBG_CommonGuard_AddrMap::iterator result = addrMap.find(IBG_CommonGuard_AddRng(addr,addr));
    if (addrMap.end() == result) {
        mAgentController->printMsg("warning", "the address of the received transaction does not belong to any protected address range\n");
        return true;
    }

    mAgentController->printMsg("info", "from the accessed address range, get validating settings\n");
    /// -# from the accessed address range, get validating settings
    bool GEN, DBG, UM, WG, RG;
    uint32_t SPID;
    string groupName   = result->second.first;
    uint32_t channelId = result->second.second;
    mAgentController->GetGuardSettings(groupName, channelId, GEN, DBG, UM, WG, RG, SPID);
    mAgentController->printMsg("info", "check if the IBG is enable or not\n");
    /// -# check if the IBG is enable or not
    if (false == GEN) {
        mAgentController->printMsg("info", "the bus guard is disabled\n");
        return true;
    }

    mAgentController->printMsg("info", "check if the received transaction is debug and debug-enable mode is disable or not\n");
    /// -# check if the received transaction is debug and debug-enable mode is disable or not
    if (debug && DBG) {
        if ((ext != NULL) && (ext->getPeId() == 0xFF)) {
            mAgentController->printMsg("info", "the guard forwarded the debug transaction because of debug enable feature\n");
            return true;
        }
    }

    mAgentController->printMsg("info", "check if the the received transaction is in user mode and user mode disable is on or not\n");
    /// -# check if the the received transaction is in user mode and user mode disable is on or not
    if (ext->isUserMode() && !UM) {
        mAgentController->printMsg("info", "the guard discarded the user-mode transaction because the user-mode enable is off\n");
        mAgentController->TransferErrInfo(groupName, channelId, addr, debug, ext->isUserMode(), trans.is_write(), ext->getSpId());
        return false;
    }

    mAgentController->printMsg("info", "check if the global-read is enabled for all read transactions or not\n");
    /// -# check if the global-read is enabled for all read transactions or not
    if (trans.is_read() && RG) {
        mAgentController->printMsg("info", "the guard forwarded the read transaction due to global-read feature");
        return true;
    }

    mAgentController->printMsg("info", "check if the global-write is enabled for all write transactions or not\n");
    /// -# check if the global-write is enabled for all write transactions or not
    if (trans.is_write() && WG) {
        mAgentController->printMsg("info", "the guard forwarded the write transaction due to global-write feature\n");
        return true;
    }

    mAgentController->printMsg("info", "check if the SPID field of the transaction is allowed or not\n");
    /// -# check if the SPID field of the transaction is allowed or not
    if (((SPID >> ext->getSpId()) & 0x1) == 0x1) {
        mAgentController->printMsg("info", "the guard forwarded the transaction due to the matched SPID\n");
        return true;
    }

    mAgentController->printMsg("info", "default result is false, the guard blocks all others\n");
    /// -# default result is false, the guard blocks all others.
    mAgentController->printMsg("info", "the guard discarded the transaction due to the unmatched SPID\n");
    mAgentController->TransferErrInfo(groupName, channelId, addr, debug, ext->isUserMode(), trans.is_write(), ext->getSpId());
    return false;
}//}}}
// vim600: set foldmethod=marker :
//##############################################################################
// EOF
//##############################################################################
