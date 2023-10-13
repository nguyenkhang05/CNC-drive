// ---------------------------------------------------------------------
// $Id: IBG_CommonGuardAgentControllerIf.h,v 1.5 2019/05/30 01:49:32 nhuvo Exp $
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
#ifndef IBG_IBG_COMMONGUARDAGENTCONTROLLERIF_H_
#define IBG_IBG_COMMONGUARDAGENTCONTROLLERIF_H_

#include <stdint.h>
#include <string>
#include <OSCI2.h>

/**@brief the pure virtual class serves as an interface that defines APIs an Agent
*        Controller must full-fill in order to cooperate with the bus guard
*/
class IBG_CommonGuardAgentControllerIf {
public:
    virtual ~IBG_CommonGuardAgentControllerIf () {};

    /**@brief Check if period value of the clock clocking the bus guard is greater than 0 or not
    * @return true if the clock under check is greater than 0
    */
    virtual bool CheckGuardClock () = 0;

    /**@brief check status of the combination of hard reset and command reset
    *        that controls the bus guard is active or not
    * @return true if at least one reset source under check is active
    */
    virtual bool GetGuardResetStatus () = 0;

    /**@brief function allows the bus guard to report diagnosis message for debug
    * @param[in] severity level of severity of the message
    * @param[in] msg the message
    */
    virtual void printMsg (const char* severity
        ,const char* msg) = 0;

    /**@brief add a address range into the map of protected range. It is invoked via
    *        Python interface.
    * @details
    *  The new range must not overlap any range which has been added before.
    * @param[in] groupName name of the group to which the specified address range belongs
    * @param[in] ch_id id of the channel to which the specified address range belongs
    * @param[in] start_addr start address of the desired address range
    * @param[in] size size of the desired address range
    * @return true if the desired address does not overlap any address range
    *         which has already exist in the map. Otherwise, the new address wont
    *         be added into the map.
    */
    virtual bool SetAreaAddr(const std::string groupName
        ,const uint32_t    ch_id
        ,const uint32_t    start_addr
        ,const uint32_t    size) = 0;

    /**@brief the function API allows the bus guard to report error to the rest of
    *        system whenever it detects an illegal transaction
    * @details
    *        Whenever an illegal transaction is detect, the bus guard reports error to
    *        the EMC and logs the additional information associated with errors
    *        into a set of registers.
    * @param[in] groupName name of group of the protected address range to which the transaction is sent
    * @param[in] channelId the identifier of the channel of the protected address range to which the transaction is sent
    * @param[in] addr target address of the transaction
    * @param[in] isDebug whether the transaction is a debug transaction or not
    * @param[in] isUserMode whether the transaction is a user-mode transaction or not
    * @param[in] isWrite whether the transaction is read access or write access
    * @param[in] SPID spid field of the transaction
    */
    virtual void TransferErrInfo (const std::string  groupName
        ,const uint32_t     channelId
        ,const uint32_t     addr
        ,const bool         isDebug
        ,const bool         isUserMode
        ,const bool         isWrite
        ,const uint8_t      SPID) {};

    /**@brief the APIs allows the bus guard to get information of validation settings
    *        from registers in the register block.
    * @param[in] groupName name of group of the protected address range to which the transaction is sent
    * @param[in] channelId the identifier of the channel of the protected address range to which the transaction is sent
    * @param[out] GEN whether the bus guard is enable or not
    * @param[out] DBG whether debug transactions are always forwarded or still need further checks
    * @param[out] UM whether user-mode transactions are always discarded or passed to further checks
    * @param[out] WG whether write-transactions from any initiator are forwarded (global-write) or need to be check the initiators via SPID fields
    * @param[out] RG whether read-transactions from any initiator are forwarded (global-read) or need to be check the initiators via SPID fields
    * @param[out] SPID lists of authorized initiators
    * @return true if the specified group name and channelId are valid and the settings can be find out in the setting registers
    */
    virtual bool GetGuardSettings(const std::string  groupName
        ,const uint32_t     channelId
        ,bool&              GEN
        ,bool&              DBG
        ,bool&              UM
        ,bool&              WG
        ,bool&              RG
        ,uint32_t&          SPID
        ) = 0;
};

#endif /* IBG_IBG_COMMONGUARDAGENTCONTROLLERIF_H_ */
