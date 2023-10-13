// ---------------------------------------------------------------------
// $Id: IBG_CommonGuard.h,v 1.5 2019/05/30 01:49:32 nhuvo Exp $
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

#ifndef IBG_IBG_COMMONGUARD_H_
#define IBG_IBG_COMMONGUARD_H_

#include "IBG_CommonGuardAgentControllerIf.h"
#include "BusMasterIf.h"
#include "BusBridgeCoreBase.h"
#include "CmErrMsg.h" // for WinMsg
#include "Common.h"
#include "global.h" // define bus widths BUS_WIDTH_VCI32
#include <stdint.h>

/**@typedef IBG_CommonGuard_RngId range identity of a protected address range,
* the first of the two is a string which is the name of the protected address,
* the second of the two is the id of the channel. A protected address range
* belongs to a group specified by a name and to a channel in that group specified
* by an index
*/
typedef std::pair<std::string,uint32_t>  IBG_CommonGuard_RngId;
/**@typedef IBG_CommonGuard_AddRng an address range which is determined by the
* lower limit (or start address range) and higher limit (or end address range).
* the first of the two is the higher address while the second of the two is
* the lower address.
*/
typedef std::pair<uint32_t,uint32_t>     IBG_CommonGuard_AddRng;
/**@brief a functor defines a comparison between two given address range. The
* address map requires the keys stored inside it are unique, then it requires
* the keys has to be equipped an operator.
*/
struct IBG_CommonGuard_AddRngLessThanCompare {
    bool operator() (const IBG_CommonGuard_AddRng& lhs,const IBG_CommonGuard_AddRng& rhs) const;
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
typedef std::map<IBG_CommonGuard_AddRng
    ,IBG_CommonGuard_RngId
    ,IBG_CommonGuard_AddRngLessThanCompare
>  IBG_CommonGuard_AddrMap;

///@brief realize the template classes to the more detailed ones
typedef BusBridgeCoreBase  <BUS_WIDTH_VCI32,BUS_WIDTH_VCI32> IbgCommonGuardBusBridgeCore;
typedef BusBridgeModuleBase<BUS_WIDTH_VCI32,BUS_WIDTH_VCI32> IbgCommonGuardBusBridgeModule;
typedef BusBridgeSlaveIf   <BUS_WIDTH_VCI32>                 IbgCommonGuardBusBridgeSlaveIf;
typedef BusMasterIf        <BUS_WIDTH_VCI32>                 IbgCommonGuardBusMasterIf;

/**@brief the class implements the core function of the bus guard. This receives
* transactions, validates them before sending them to the target protected
* address ranges.
*/
class IBG_CommonGuard : public IbgCommonGuardBusBridgeCore {
public:
    IBG_CommonGuard(std::string                       name
        ,IbgCommonGuardBusBridgeModule*    Parent
        ,IbgCommonGuardBusBridgeSlaveIf*   SlaveIf
        ,IbgCommonGuardBusMasterIf*        MasterIf
        ,SIM_MODE_T                        simmode
        ,IBG_CommonGuardAgentControllerIf* AgentController_ptr
        );
    virtual ~IBG_CommonGuard();

private:
    IbgCommonGuardBusBridgeSlaveIf*    mSlaveIf;         ///< the bus master cooperates with the bus guard. The bus guard notifies completions of received transactions via this interface
    IbgCommonGuardBusMasterIf*         mMasterIf;        ///< the bus master cooperates with the bus guard. The bus guard forwards read and write transactions via this interface role
    IBG_CommonGuardAgentControllerIf*  mAgentController; ///< relationship with the agent controller

    std::map<TlmBasicPayload*
        ,TlmBasicPayload*
    >                     mRequestMap;     ///< Bus request map for AT mode
    std::queue<TlmBasicPayload*>  mClearTransFifo; ///< Clear transaction list for AT mode
    TlmBasicPayload*              mTrans;          ///< Transaction info

    BusTime_t mTransTime;///< Internal time
    bool      mIsDebug;  ///< Is debug transaction

    void SettingTransBasic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void SetTransExt0 (TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void TransferComplete(void);
    //--------------------------------------------------------------------------
    // Implement virtual functions of BusSlaveFuncIf class
    //--------------------------------------------------------------------------
    void read (unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    //--------------------------------------------------------------------------
    // Overwrite virtual functions of BusBridgeCoreBase class
    //--------------------------------------------------------------------------
    void deleteTrans(void);
    void notifyTransComp(TlmBasicPayload &trans, BusTime_t &t);


    //--------------------------------------------------------------------------
    // For decode address into group, channel id and check accessibility
    //--------------------------------------------------------------------------
private:
    IBG_CommonGuard_AddrMap addrMap; ///< map with the keys are address ranges and values are range id of these address
public:
    bool SetAreaAddr(const std::string groupName
        ,const uint32_t    channelId
        ,const uint32_t    start_addr
        ,const uint32_t    size);
private:
    bool CheckAccessPermission(TlmBasicPayload &trans
        ,const bool debug
        ,bool &is_legal_trans);

};

#endif /* IBG_IBG_COMMONGUARD_H_ */
