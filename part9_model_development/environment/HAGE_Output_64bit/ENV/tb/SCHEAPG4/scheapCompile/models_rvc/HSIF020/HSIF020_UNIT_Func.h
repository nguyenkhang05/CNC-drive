// ---------------------------------------------------------------------
// $Id: HSIF020_UNIT_Func.h,v 1.6 2020/04/08 07:39:39 quynhnhunguyen Exp $
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __HSIF020_UNIT_FUNC_H__
#define __HSIF020_UNIT_FUNC_H__

#include "OSCI2.h"
#include "hsif020_unit_regif.h"
#include "HSIF020_UNIT_AgentController.h"


class HSIF020_UNIT_AgentController;

class HSIF020_UNIT_Func: public Chsif020_unit_regif
{
public:
    HSIF020_UNIT_Func(std::string name
                     , HSIF020_UNIT_AgentController *iHSIF020_UNIT_AgentControllerPtr);
    ~HSIF020_UNIT_Func(void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    void read (uint32_t offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (uint32_t offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    /// Public functions
    void Initialize (void);                         /// Initialize variable
    bool IsChTxRxDis(void);                         /// Check Tx Rx is disable or not
    bool IsChRxEnable(void);                        /// Check Rx is enable or not
    bool IsChTxEnable(void);                        /// Check Tx is enable or not
    uint32_t GetRWAddr(void);                       /// Get RW address
    uint32_t GetWData(void);                        /// Get Write data

    /// Status, interrupt
    void SetChannelStatus(uint32_t bit_name);       /// Set Channel Status register
    void ClearChannelStatus(uint32_t bit_name);     /// Clear Channel Status register
    bool GetChannelStatus(uint32_t bit_name);       /// Get Channel status register
    bool GetChIntEnable(uint32_t bit_name);         /// Get channel int enable
    void SetReadData(uint32_t readData);            /// Set read data
    uint32_t GetHSIFnCST(void);                     /// Get status register
private:
    
    HSIF020_UNIT_AgentController    *mHSIF020_UNIT_AgentController;
    std::string                     mModuleName;        //Module name

    /// Virtual functions of Chsif020_unit_regif class
    void cb_HSIFnCMD_TNME(RegCBstr str);
    void cb_HSIFnCCT_CTYW(RegCBstr str);
    void cb_HSIFnCSC_CREC(RegCBstr str);
    void cb_HSIFnCIE_CREE(RegCBstr str);
    void cb_HSIFnCAR_CAR(RegCBstr str);
    void cb_HSIFnCWD_CWD(RegCBstr str);
    void cb_HSIFnCRD_CRD(RegCBstr str);
};

#endif /*__HSIF020_UNIT_FUNC_H__*/
