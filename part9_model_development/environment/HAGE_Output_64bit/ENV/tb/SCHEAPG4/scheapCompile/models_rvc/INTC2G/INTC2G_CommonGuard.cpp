// ---------------------------------------------------------------------
// $Id: INTC2G_CommonGuard.cpp,v 1.2 2019/07/08 10:49:10 tientran2 Exp $
//
// Copyright(c) 2017-2018 Renesas Electronics Corporation
// Copyright(c) 2017-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "CmErrMsg.h" // for WinMsg
#include "Common.h"
#include "INTC2G_CommonGuard.h"

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
bool INTC2G_CommonGuard_AddRngLessThanCompare::operator() (const INTC2G_CommonGuard_AddRng& lhs, const INTC2G_CommonGuard_AddRng& rhs) const
{//{{{
   return lhs.first < rhs.second; // higher address of lhs is less than lower address of rhs
}//}}}
//------------------------------------------------------------------------------
/**@brief Constructor of the bus guard
 */
INTC2G_CommonGuard::INTC2G_CommonGuard(std::string                       name
                                ,INTC2GCommonGuardBusBridgeModule*      Parent
                                ,INTC2GCommonGuardBusBridgeSlaveIf*     SlaveIf
                                ,INTC2GCommonGuardBusMasterIf*          MasterIf
                                ,SIM_MODE_T                             simmode
                                ,INTC2G_CommonGuardAgentControllerIf*   AgentController_ptr
                                )
: INTC2GCommonGuardBusBridgeCore(Parent,SlaveIf,MasterIf)
, mSlaveIf         (SlaveIf)
, mMasterIf        (MasterIf)
, mAgentController (AgentController_ptr)
{//{{{
    sc_assert((mAgentController != NULL) && (mMasterIf != NULL) && (mSlaveIf != NULL));
    INTC2GCommonGuardBusBridgeCore::setTlmTransType(simmode);
    mTransTime = SC_ZERO_TIME;
    mTrans = (TlmBasicPayload*)0;
    mIsDebug = false;

    mAgentController->printMsg("info","finish instantiating the common guard\n");
}//}}}
//------------------------------------------------------------------------------
/**@brief Destructor
 */
INTC2G_CommonGuard::~INTC2G_CommonGuard() {}
//------------------------------------------------------------------------------
/**@brief Notify transaction completion in AT mode
 */
void INTC2G_CommonGuard::TransferComplete (void)
{//{{{
    if ((INTC2GCommonGuardBusBridgeCore::mTransType == AT) && (!mIsDebug)) {
        //AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Copy basic info (address, data, length)from the input transaction into the output transaction
 */
void INTC2G_CommonGuard::SetTranBasic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    outTrans.set_address    (inTrans.get_address());
    outTrans.set_data_ptr   (inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}
//------------------------------------------------------------------------------
/**@brief Copy extension information from the input transaction into the output transaction
 */
void INTC2G_CommonGuard::SetTransExt0(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{//{{{
    TlmG3mExtension* mG3mExt = (TlmG3mExtension*)0;
    inTrans.get_extension(mG3mExt);
    if (mG3mExt != NULL) {
        TlmG3mExtension* mG3mExtOut = static_cast<TlmG3mExtension*>(mG3mExt->clone());
        outTrans.set_extension(mG3mExtOut);
        mAgentController->printMsg("info", "copy G3m extension side-band info\n");
    }

    TlmAxiExtension* mAxiExt = (TlmAxiExtension*)0;
    inTrans.get_extension(mAxiExt);
    if (mAxiExt != NULL) {
        TlmAxiExtension* mAxiExtOut = static_cast<TlmAxiExtension*>(mAxiExt->clone());
        outTrans.set_extension(mAxiExtOut);
        mAgentController->printMsg("info", "copy Axi extension side-band info\n");
    }

    TlmVpiExtension* mVpiExt = (TlmVpiExtension*)0;
    inTrans.get_extension(mVpiExt);
    if (mVpiExt != NULL) {
        TlmVpiExtension* mVpiExtOut = static_cast<TlmVpiExtension*>(mVpiExt->clone());
        outTrans.set_extension(mVpiExtOut);
        mAgentController->printMsg("info", "copy Vpi extension side-band info\n");
    }

    TlmAhbExtension* mAhbExt = (TlmAhbExtension*)0;
    inTrans.get_extension(mAhbExt);
    if (mAhbExt != NULL) {
        TlmAhbExtension* mAhbExtOut = static_cast<TlmAhbExtension*>(mAhbExt->clone());
        outTrans.set_extension(mAhbExtOut);
        mAgentController->printMsg("info", "copy Ahb extension side-band info\n");
    }

    TlmVciExtension* mVciExt = (TlmVciExtension*)0;
    inTrans.get_extension(mVciExt);
    if (mVciExt != NULL) {
        TlmVciExtension* mVciExtOut = static_cast<TlmVciExtension*>(mVciExt->clone());
        outTrans.set_extension(mVciExtOut);
        mAgentController->printMsg("info", "copy Vci extension side-band info\n");
    }

    TlmApbExtension* mApbExt = (TlmApbExtension*)0;
    inTrans.get_extension(mApbExt);
    if (mApbExt != NULL) {
        TlmApbExtension* mApbExtOut = static_cast<TlmApbExtension*>(mApbExt->clone());
        outTrans.set_extension(mApbExtOut);
        mAgentController->printMsg("info", "copy Apb extension side-band info\n");
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
void INTC2G_CommonGuard::read (unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    sc_assert(t != NULL);

    mAgentController->printMsg("info","start processing a read transaction \n");

    mTrans     = &trans;
    mTransTime = *t;
    mIsDebug   = debug;

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
            if (this->CheckAccessPermission(trans,debug)) {
                mAgentController->printMsg("info","forward the read transaction\n");
                if (debug || (INTC2GCommonGuardBusBridgeCore::mTransType == LT)) {// LT mode
                    mAgentController->printMsg("info","forward a read transaction in LT mode (or debug transactions)\n");
                    mMasterIf->read(trans,debug,*t,false);
                } else {//AT mode
                    mAgentController->printMsg("info","forward a read transaction in AT mode (or non-debug transactions)\n");
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SetTranBasic(trans, *sendTrans );
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
void INTC2G_CommonGuard::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    assert(t != NULL);

    mAgentController->printMsg("info","start processing a write transaction\n");

    mTrans = &trans;
    mTransTime = *t;
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
            if (this->CheckAccessPermission(trans,debug)) {
                mAgentController->printMsg("info","forward the write transaction\n");
                if (debug || (INTC2GCommonGuardBusBridgeCore::mTransType == LT)) {// LT mode
                    mAgentController->printMsg("info","forward a write transaction in LT mode (or debug transactions)\n");
                    mMasterIf->write(trans,debug,*t,false);
                } else {//AT mode
                    mAgentController->printMsg("info","forward a write transaction in AT mode (or non-debug transactions)\n");
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    SetTranBasic(trans, *sendTrans );
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
void INTC2G_CommonGuard::deleteTrans(void)
{//{{{
    while(!mClearTransFifo.empty()){
        TlmBasicPayload *trans;
        trans = mClearTransFifo.front();
        trans->free_all_extensions();
        mRequestMap.erase(trans);
        mClearTransFifo.pop();
        delete trans;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Overwrite virtual functions of BusBridgeCoreBase class
 */
void INTC2G_CommonGuard::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t)
{//{{{
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
 *  An address range is belong to a channel of the protected peripheral.
 *  This method is to add an address range into the protected address map.
 *  The new range must not overlap any range which has been added before.
 * @param[in] ch_id id of the channel to which the specified address range belongs
 * @param[in] start_addr start address of the desired address range
 * @param[in] size size of the desired address range
 * @return true if the desired address does not overlap any address range
 *         which has already exist in the map. Otherwise, the new address wont
 *         be added into the map.
 */
bool INTC2G_CommonGuard::SetAreaAddr(const uint32_t    channelId
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
    INTC2G_CommonGuard_AddRng key = INTC2G_CommonGuard_AddRng(higher_addr, lower_addr);
    if (addrMap.find(key) != addrMap.end()) {
        mAgentController->printMsg("warning", "the address range overlapped with the previous defined one\n");
        return false;
    }

    /// -# add the range into the address map
    addrMap[key] = channelId;

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
bool INTC2G_CommonGuard::CheckAccessPermission(TlmBasicPayload &trans, const bool debug)
{//{{{
    mAgentController->printMsg("info", "start check access permission\n");

    /// -# get G3m extended information of the receive transaction
    TlmG3mExtension* ExtensionInfo = (TlmG3mExtension*)0;
    mAgentController->printMsg("info", "try to get G3m extension side-band info\n");
    //trans.get_extension<TlmG3mExtension>(ExtensionInfo);
    trans.get_extension(ExtensionInfo);
    //ExtensionInfo = trans.get_extension<TlmG3mExtension>();
    //ExtensionInfo = trans.get_extension();
    if (ExtensionInfo == NULL) {
        mAgentController->printMsg("error", "could not get G3m extension side-band info\n");
        return false;
    }

    mAgentController->printMsg("info", "try to get address info\n");

    uint32_t lower_addr = (uint32_t)trans.get_address();
    uint32_t higher_addr = lower_addr;
    uint32_t data_length = (uint32_t)trans.get_data_length();

    if (data_length > 0 ) {
        higher_addr = lower_addr + data_length - 1;
    }

    mAgentController->printMsg("info", "validate address of the received transaction\n");
    /// -# validate address of the received transaction,
    INTC2G_CommonGuard_AddrMap::iterator result = addrMap.find(INTC2G_CommonGuard_AddRng(higher_addr, lower_addr));
    if (addrMap.end() == result) {
        mAgentController->printMsg("warning", "the address of the received transaction does not belong to any protected address range\n");
        return true;
    } else {
        if ( lower_addr < result->first.second) {
            mAgentController->printMsg("warning", "abnormal access. the address of the received transaction does not belong to protected range, but access range of transaction is belong protected range\n");
        }
    }

    mAgentController->printMsg("info", "from the accessed address range, get validating settings\n");
    /// -# from the accessed address range, get validating settings
    bool GEN, DBG, UM, WG, RG;
    uint32_t SPID;
    uint32_t channelId = result->second;
    mAgentController->GetGuardSettings(channelId,GEN, DBG, UM, WG, RG, SPID);
    mAgentController->printMsg("info", "check if the INTC2G is enable or not\n");
    /// -# check if the INTC2G is enable or not
    if (false == GEN) {
        mAgentController->printMsg("info", "the bus guard is disabled\n");
        return true;
    }

    mAgentController->printMsg("info", "check if the received transaction is debug and debug-enable mode is disable or not\n");
    /// -# check if the received transaction is debug and debug-enable mode is disable or not
    if (debug && DBG) {
        if (ExtensionInfo->getPeId() == 0xFF) {
            mAgentController->printMsg("info", "the guard forwarded the debug transaction because of debug enable feature\n");
            return true;
        }
    }

    mAgentController->printMsg("info", "check if the the received transaction is in user mode and user mode disable is on or not\n");
    /// -# check if the the received transaction is in user mode and user mode disable is on or not
    if (ExtensionInfo->isUserMode() && !UM) {
        mAgentController->printMsg("info", "the guard discarded the user-mode transaction because the user-mode enable is off\n");
        mAgentController->TransferErrInfo(channelId, lower_addr, debug, ExtensionInfo->isUserMode(), trans.is_write(), ExtensionInfo->getSpId());
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
    if (((SPID >> ExtensionInfo->getSpId()) & 0x1) == true){
        mAgentController->printMsg("info", "the guard forwarded the transaction due to the matched SPID\n");
        return true;
    }

    mAgentController->printMsg("info", "default result is false, the guard blocks all others\n");
    /// -# default result is false, the guard blocks all others.
    mAgentController->printMsg("info", "the guard discarded the transaction due to the unmatched SPID\n");
    mAgentController->TransferErrInfo(channelId, lower_addr, debug, ExtensionInfo->isUserMode(), trans.is_write(), ExtensionInfo->getSpId());
    return false;
}//}}}
// vim600: set foldmethod=marker :
