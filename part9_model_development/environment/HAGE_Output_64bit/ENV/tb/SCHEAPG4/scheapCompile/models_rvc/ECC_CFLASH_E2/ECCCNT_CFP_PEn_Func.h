// ---------------------------------------------------------------------
// $Id: ECCCNT_CFP_PEn_Func.h,v 1.1 2019/09/26 10:06:58 landang Exp $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECCCNT_CFP_PEn_FUNC_H__
#define __ECCCNT_CFP_PEn_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ecccnt_cfp_pen_regif.h"
#include "ECC_CFLASH_E2_AgentController.h"
#include "global.h"

class ECCCNT_CFP_PEn_Func: public Cecccnt_cfp_pen_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECCCNT_CFP_PEn_Func);
    ECCCNT_CFP_PEn_Func(std::string name, unsigned int pe_id, ECC_CFLASH_E2_AgentController* CFLASHAgentController);
    ~ECCCNT_CFP_PEn_Func();

    void EnableReset(const std::string reset_name, const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    unsigned int GetCFPECCCTLReg();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    ECC_CFLASH_E2_AgentController   *mpCFLASHAgentController;// KCPROT Agent controller pointer
    std::string     mModuleName;// Module name
    unsigned int    mPeId;      // PE index
    std::map<unsigned int, std::string> mProtectedRegMap;   // Protected registers map   

    // Callback
    void cb_CFPECCCTL_ECCDIS(RegCBstr str);

};
#endif //__ECCCNT_CFP_PEn_FUNC_H__
