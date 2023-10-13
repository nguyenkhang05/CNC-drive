// ---------------------------------------------------------------------
// $Id: INTC2G_CommonGuard.h,v 1.2 2019/07/08 10:49:10 tientran2 Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef INTC2G_INTC2G_COMMONGUARD_H_
#define INTC2G_INTC2G_COMMONGUARD_H_

#include "BusMasterIf.h"
#include "BusBridgeCoreBase.h"
#include "global.h"
#include <stdint.h>

#include "INTC2G_CommonGuardAgentControllerIf.h"


/**@typedef INTC2G_CommonGuard_ChannelId identity the id of the channel. A protected
 * address range belongs to  a channel specified by an index
 */
typedef uint32_t  INTC2G_CommonGuard_ChannelId;

/**@typedef INTC2G_CommonGuard_AddRng an address range which is determined by the
 * lower limit (or start address range) and higher limit (or end address range).
 * the first of the two is the higher address while the second of the two is
 * the lower address.
 */
typedef std::pair<uint32_t,uint32_t>     INTC2G_CommonGuard_AddRng;

/**@brief a functor defines a comparison between two given address range. The
 * address map requires the keys stored inside it are unique, then it requires
 * the keys has to be equipped an operator.
 */
struct INTC2G_CommonGuard_AddRngLessThanCompare {
   bool operator() (const INTC2G_CommonGuard_AddRng& lhs,const INTC2G_CommonGuard_AddRng& rhs) const;
};
/**@typedef an address map whose keys are address range and values are
 * address range identifiers.
 * The address map allows the keys are unique, that means the address ranges
 * must not overlap each other. The map also allows to check whether an
 * given single address is in a certain address range already stored inside it
 * or not, and if it is, which is the matched address range id. For short, it
 * decodes from address of the transaction to the range id of the protected address
 * ranges.
 */
typedef std::map<INTC2G_CommonGuard_AddRng
                ,INTC2G_CommonGuard_ChannelId
                ,INTC2G_CommonGuard_AddRngLessThanCompare
                >  INTC2G_CommonGuard_AddrMap;

///@brief realize the template classes to the more detailed ones
typedef BusBridgeCoreBase  <BUS_WIDTH_APB,BUS_WIDTH_APB> INTC2GCommonGuardBusBridgeCore;
typedef BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB> INTC2GCommonGuardBusBridgeModule;
typedef BusBridgeSlaveIf   <BUS_WIDTH_APB>                 INTC2GCommonGuardBusBridgeSlaveIf;
typedef BusMasterIf        <BUS_WIDTH_APB>                 INTC2GCommonGuardBusMasterIf;

/**@brief the class implements the core function of the bus guard. This receives
 * transactions, validates them before sending them to the target protected
 * address ranges.
 */
class INTC2G_CommonGuard : public INTC2GCommonGuardBusBridgeCore {
public:
    INTC2G_CommonGuard(std::string                       name
                   ,INTC2GCommonGuardBusBridgeModule*    Parent
                   ,INTC2GCommonGuardBusBridgeSlaveIf*   SlaveIf
                   ,INTC2GCommonGuardBusMasterIf*        MasterIf
                   ,SIM_MODE_T                           simmode
                   ,INTC2G_CommonGuardAgentControllerIf* AgentController_ptr
                   );
    virtual ~INTC2G_CommonGuard();

private:
    INTC2GCommonGuardBusBridgeSlaveIf*    mSlaveIf;         ///< the bus master cooperates with the bus guard. The bus guard notifies completions of received transactions via this interface
    INTC2GCommonGuardBusMasterIf*         mMasterIf;        ///< the bus master cooperates with the bus guard. The bus guard forwards read and write transactions via this interface role
    INTC2G_CommonGuardAgentControllerIf*  mAgentController; ///< relationship with the agent controller

    std::map<TlmBasicPayload*
            ,TlmBasicPayload*
            >                     mRequestMap;     ///< Bus request map for AT mode
    std::queue<TlmBasicPayload*>  mClearTransFifo; ///< Clear transaction list for AT mode
    TlmBasicPayload*              mTrans;          ///< Transaction info

    BusTime_t mTransTime;///< Internal time
    bool      mIsDebug;  ///< Is debug transaction

    void SetTranBasic(TlmBasicPayload &inTrans
                      ,TlmBasicPayload &outTrans);
    void SetTransExt0 (TlmBasicPayload &inTrans
                      ,TlmBasicPayload &outTrans);
    void TransferComplete(void);
    //--------------------------------------------------------------------------
    // Implement virtual functions of BusBridgeCoreBase class
    //--------------------------------------------------------------------------
    void read (unsigned int offsetAddress
              ,TlmBasicPayload& trans
              ,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress
              ,TlmBasicPayload& trans
              ,BusTime_t* t,bool debug);
    //--------------------------------------------------------------------------
    // Overwrite virtual functions of BusBridgeCoreBase class
    //--------------------------------------------------------------------------
    void deleteTrans(void);
    void notifyTransComp(TlmBasicPayload &trans, BusTime_t &t);
    bool CheckAccessPermission(TlmBasicPayload &trans
                                  ,const bool debug);
    //--------------------------------------------------------------------------
    // For decode address into group, channel id and check accessibility
    //--------------------------------------------------------------------------
    INTC2G_CommonGuard_AddrMap addrMap; ///< map with the keys are address ranges and values are range id of these address
public:
    bool SetAreaAddr(const uint32_t    channelId
                    ,const uint32_t    start_addr
                    ,const uint32_t    size);
};

#endif /* INTC2G_INTC2G_COMMONGUARD_H_ */
