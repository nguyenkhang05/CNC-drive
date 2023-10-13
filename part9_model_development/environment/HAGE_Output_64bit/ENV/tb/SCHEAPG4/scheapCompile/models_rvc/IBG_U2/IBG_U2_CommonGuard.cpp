// ---------------------------------------------------------------------
// $Id: IBG_U2_CommonGuard.cpp,v 1.3 2020/02/26 06:51:33 duongphan Exp $
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

#include "IBG_U2_CommonGuard.h"
#include <iostream>
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
bool IBG_U2_CommonGuard_AddRngLessThanCompare::operator() (const IBG_U2_CommonGuard_AddRng& lhs, const IBG_U2_CommonGuard_AddRng& rhs) const 
{//{{{
   return lhs.first < rhs.second; // higher address of lhs is less than lower address of rhs
}//}}}
//------------------------------------------------------------------------------
/**@brief Constructor of the bus guard
 */
IBG_U2_CommonGuard::IBG_U2_CommonGuard(std::string                       name
                                ,Ibg_u2CommonGuardBusBridgeModule*    Parent
                                ,Ibg_u2CommonGuardBusBridgeSlaveIf*   SlaveIf
                                ,Ibg_u2CommonGuardBusMasterIf*        MasterIf
                                ,SIM_MODE_T                        simmode
                                ,IBG_U2_CommonGuardAgentControllerIf* AgentController_ptr
                                )
: Ibg_u2CommonGuardBusBridgeCore(Parent,SlaveIf,MasterIf)
, mSlaveIf         (SlaveIf)
, mMasterIf        (MasterIf)
, mAgentController (AgentController_ptr)
{//{{{
    sc_assert((mAgentController != NULL) && (mMasterIf != NULL) && (mSlaveIf != NULL));
    Ibg_u2CommonGuardBusBridgeCore::setTlmTransType(simmode);
    mTransTime = SC_ZERO_TIME;
    mTrans = (TlmBasicPayload*)0;
    mIsDebug = false;
}//}}}
//------------------------------------------------------------------------------
/**@brief Destructor
 */
IBG_U2_CommonGuard::~IBG_U2_CommonGuard() 
{//{{{
}//}}}
//------------------------------------------------------------------------------
/**@brief Notify transaction completion in AT mode
 */
void IBG_U2_CommonGuard::TransferComplete (void) 
{//{{{
    if ((Ibg_u2CommonGuardBusBridgeCore::mTransType == AT) && (!mIsDebug)) {
        //AT mode and non-debug transaction
        mSlaveIf->notifyTransComp(*mTrans,mTransTime);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Copy basic info (address, data, length)from the input transaction into the output transaction
 */
 // Changed by Quang
void IBG_U2_CommonGuard::IBG_SetTransBasic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans) 
{//{{{
    outTrans.set_address    (inTrans.get_address());
    outTrans.set_data_ptr   (inTrans.get_data_ptr());
    outTrans.set_data_length(inTrans.get_data_length());
}//}}}
//------------------------------------------------------------------------------
/**@brief Copy extension information from the input transaction into the output transaction
 */
void IBG_U2_CommonGuard::SetTransExt0(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans) {
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
void IBG_U2_CommonGuard::read (unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug) 
{//{{{
    sc_assert(t != NULL);
    mTrans     = &trans;
    mTransTime = *t;
    mIsDebug   = debug;

    bool is_continue = false;

    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if ( (!mAgentController->CheckGuardClock("cpu_clk")) || (mAgentController->GetGuardResetStatus()) ) {
        if (pExtInfo != NULL) {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == pExtInfo->getPeId())) {
                is_continue = true;
            }
            else {
                TransferComplete();
                mAgentController->printMsg("warning","The reading transaction is fail because the reset is active or zero clock\n");
                }
            }
        }
        else {
                is_continue = true;
        }

        if (is_continue) {
            if (this->CheckAccessPermission(trans,debug)) {
                if (debug || (Ibg_u2CommonGuardBusBridgeCore::mTransType == LT)) {// LT mode
                    mAgentController->printMsg("info","forward a read transaction in LT mode (or debug transactions)\n");
                    mMasterIf->read(trans,debug,*t,false);
                } else {//AT mode
                    mAgentController->printMsg("info","forward a read transaction in AT mode (or non-debug transactions)\n");
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    IBG_SetTransBasic(trans, *sendTrans );
                    SetTransExt0 (trans, *sendTrans );
                    mRequestMap[sendTrans] = &trans;
                    mMasterIf->read(*sendTrans,debug,*t,false);
                }
            }

        }


}//}}}
//------------------------------------------------------------------------------
/**@brief Overwrite virtual functions of BusBridgeCoreBase class
 */
void IBG_U2_CommonGuard::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug) 
{//{{{
    assert(t != NULL);

    mTrans = &trans;
    mTransTime = *t;
    mIsDebug = debug;
    bool is_continue = false;

    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if ( (!mAgentController->CheckGuardClock("cpu_clk")) || (mAgentController->GetGuardResetStatus()) ) {
        if (pExtInfo != NULL) {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == pExtInfo->getPeId())) {
                is_continue = true;
            }
            else {
                TransferComplete();
                mAgentController->printMsg("warning","The writing transaction is fail because the reset is active or zero clock\n");
                }
            }
        }
        else {
                is_continue = true;
        }

        if (is_continue) {
            if (this->CheckAccessPermission(trans,debug)) {
                if (debug || (Ibg_u2CommonGuardBusBridgeCore::mTransType == LT)) {// LT mode
                    mAgentController->printMsg("info","forward a write transaction in LT mode (or debug transactions)\n");
                    mMasterIf->write(trans,debug,*t,false);
                } else {//AT mode
                    mAgentController->printMsg("info","forward a write transaction in AT mode (or non-debug transactions)\n");
                    TlmBasicPayload *sendTrans = new TlmBasicPayload;
                    IBG_SetTransBasic(trans, *sendTrans );
                    SetTransExt0 (trans, *sendTrans );
                    mRequestMap[sendTrans] = &trans;
                    mMasterIf->write(*sendTrans,debug,*t,false);
                }
            }
        }

}//}}}
//------------------------------------------------------------------------------
/**@brief Overwrite virtual functions of BusBridgeCoreBase class
 */
void IBG_U2_CommonGuard::deleteTrans(void) 
{//{{{
    TlmBasicPayload *trans;

    while(!mClearTransFifo.empty()){
        trans = mClearTransFifo.front();
        //Add by Quang to check validated 
        sc_assert(trans != NULL);
        //
        trans->free_all_extensions(); 
        mRequestMap.erase(trans);

        mClearTransFifo.pop();
        delete trans;
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief Overwrite virtual functions of BusBridgeCoreBase class
 */
void IBG_U2_CommonGuard::notifyTransComp(TlmBasicPayload &trans, BusTime_t &t) 
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
bool IBG_U2_CommonGuard::SetAreaAddr(const std::string groupName
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
    IBG_U2_CommonGuard_AddRng key = IBG_U2_CommonGuard_AddRng(higher_addr, lower_addr);
    if (addrMap.find(key) != addrMap.end()) {
        mAgentController->printMsg("warning", "the address range overlapped with the previous defined one\n");
        return false;
    }

    /// -# add the range into the address map
    IBG_U2_CommonGuard_RngId value = IBG_U2_CommonGuard_RngId(groupName, channelId);
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
bool IBG_U2_CommonGuard::CheckAccessPermission(TlmBasicPayload &trans, const bool debug) 
{//{{{
    /// -# get G3m extended information of the receive transaction
    TlmG3mExtension* ExtensionInfo = (TlmG3mExtension*)0;
    trans.get_extension(ExtensionInfo);
    if (ExtensionInfo == NULL) {
        mAgentController->printMsg("error", "could not get G3m extension side-band info\n");
        return false;
    }

    uint32_t addr = (uint32_t)trans.get_address();
    mAgentController->printMsg("info", "validate address of the received transaction\n");
    
    /// -# validate address of the received transaction,
    IBG_U2_CommonGuard_AddrMap::iterator result = addrMap.find(IBG_U2_CommonGuard_AddRng(addr,addr));
    bool GEN = false, DBG = false, UM = false, WG = false, RG = false, GEN_Chan_Exist = false;
    uint32_t SPID = 0;
    string groupName = mAgentController->GetGroupName();
    uint32_t channelAddressGuard = 0;
    uint32_t  channelEnableGuard = 0;

    uint8_t chanNum = mAgentController->GetChannelNum();
    
    if (groupName == "IBG_U2_TPTG") {
        chanNum = chanNum - 2;
    }
    else {
        chanNum--;
    }
    // Find channel is enabled guard
    for (unsigned int iChan = 0; iChan < chanNum; iChan++) {
        mAgentController->GetGuardSettings(groupName ,iChan ,GEN_Chan_Exist, DBG, UM, WG, RG, SPID);
        if (true == GEN_Chan_Exist) {
            channelEnableGuard = iChan;
            break;
        }
    }
    // Find common channel is enabled guard
    if (false == GEN_Chan_Exist) {
        if (groupName == "IBG_U2_IPIG") {
            mAgentController->GetGuardSettings(groupName ,8 ,GEN_Chan_Exist, DBG, UM, WG, RG, SPID);
        }
        else if (groupName == "IBG_U2_BRG") {
            mAgentController->GetGuardSettings(groupName ,16 ,GEN_Chan_Exist, DBG, UM, WG, RG, SPID);
        }
        else if (groupName == "IBG_U2_TPTG") {
            mAgentController->GetGuardSettings(groupName ,8 ,GEN_Chan_Exist, DBG, UM, WG, RG, SPID);
            if (false == GEN_Chan_Exist) {
                mAgentController->GetGuardSettings(groupName ,9 ,GEN_Chan_Exist, DBG, UM, WG, RG, SPID);
            } else {
                channelEnableGuard = 8;
            }
            if (true == GEN_Chan_Exist) {
                channelEnableGuard = 9;
            }
        }
    }
    
    // All channels are disabled guard
    if (false == GEN_Chan_Exist) {
        mAgentController->printMsg("info", "the bus guard is disabled\n");
        return true;
    }
    else {
        // Outside address and exist channel is enabled guard
        if (addrMap.end() == result) {
            printf("Warning - the address of the received transaction does not belong to any protected address range and exist the enable guard with chanel %d is set\n", channelEnableGuard);
            return true;
            // return false;//Changed by Quang bug #113877
        }
        // Inside address and exist channel is enabled guard
        else {
            channelAddressGuard = result->second.second;
            mAgentController->GetGuardSettings(groupName ,channelAddressGuard ,GEN, DBG, UM, WG, RG, SPID);
            if (false == GEN) {
                printf("Warning - the address of the received transaction is belong to protected address range but the channel index %d is not enable guard\n", channelEnableGuard);
                return true;
                //return false; //Changed by Quang bug #113877
            }
        }
    }
    
    /// -# check if the received transaction is debug and debug-enable mode is disable or not
    if (debug && DBG) {
        if ((ExtensionInfo != NULL) && (ExtensionInfo->getPeId() == 0xFF)) {
            mAgentController->printMsg("info", "the guard forwarded the debug transaction because of debug enable feature\n");
            return true;
        }
    }

    /// -# check if the the received transaction is in user mode and user mode disable is on or not
    if (ExtensionInfo->isUserMode() && !UM) {
        mAgentController->printMsg("info", "the guard discarded the user-mode transaction because the user-mode enable is off\n");
        mAgentController->TransferErrInfo(groupName, channelAddressGuard, addr, debug, ExtensionInfo->isUserMode(), trans.is_write(), ExtensionInfo->getSpId());
        return false;
    }
    
    /// -# check if the SPID field of the transaction is allowed or not
    if ((bool)((SPID >> ExtensionInfo->getSpId()) & 0x1)) {
        mAgentController->printMsg("info", "the guard forwarded the transaction due to the matched SPID\n");
        return true;
    }

    /// -# check if the global-write is enabled for all write transactions or not
    if (true == trans.is_write()) { 
        if (true == WG) {
            mAgentController->printMsg("info", "the guard forwarded the write transaction due to global-write feature\n");
            return true;
        }
        else {
            mAgentController->printMsg("info", "the guard discarded write transaction\n");
            mAgentController->TransferErrInfo(groupName, channelAddressGuard, addr, debug, ExtensionInfo->isUserMode(), trans.is_write(), ExtensionInfo->getSpId());
            return false;
        }
    }
    
    /// -# check if the global-read is enabled for all read transactions or not
    //if (trans.is_read() && RG) {
    if (true == RG) {
        mAgentController->printMsg("info", "the guard forwarded the read transaction due to global-read feature\n");
        return true;
    }
    else {
        mAgentController->printMsg("info", "the guard discarded read transaction.\n");
        mAgentController->TransferErrInfo(groupName, channelAddressGuard, addr, debug, ExtensionInfo->isUserMode(), trans.is_write(), ExtensionInfo->getSpId());
        return false;
    }

   // mAgentController->printMsg("info", "default result is false, the guard blocks all others\n");
    /// -# default result is false, the guard blocks all others.
   // mAgentController->printMsg("info", "the guard discarded the transaction due to the unmatched SPID\n");
  //  mAgentController->TransferErrInfo(groupName, channelAddressGuard, addr, debug, ExtensionInfo->isUserMode(), trans.is_write(), ExtensionInfo->getSpId());
  //  return false;
}//}}}
// vim600: set foldmethod=marker :
