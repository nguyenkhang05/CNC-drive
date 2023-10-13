// ---------------------------------------------------------------------
// $Id: ECCCNT_DMDAn_Func.h,v 1.4 2019/09/06 08:49:03 landang Exp $
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
#ifndef __ECCCNT_DMDAn_FUNC_H__
#define __ECCCNT_DMDAn_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ecccnt_dmdan_regif.h"
#include "ECC_DMADTS_RAM_E2_AgentController.h"
#include "global.h"

class ECCCNT_DMDAn_Func: public Cecccnt_dmdan_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECCCNT_DMDAn_Func);
    ECCCNT_DMDAn_Func(std::string name, unsigned int sub_id, ECC_DMADTS_RAM_E2_AgentController* DMADTS_RAMAgentController);
    ~ECCCNT_DMDAn_Func();

    void EnableReset(const std::string reset_name, const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    unsigned int GetDAECCCTLReg();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    ECC_DMADTS_RAM_E2_AgentController   *mpDMADTS_RAMAgentController;// KCPROT Agent controller pointer
    std::string     mModuleName;// Module name
    unsigned int    mSubId;      // PE index
    std::map<unsigned int, std::string> mProtectedRegMap;   // Protected registers map   

    // Callback
    void cb_DAECCCTL_ECCDIS(RegCBstr str);

};
#endif //__ECCCNT_DMDAn_FUNC_H__
