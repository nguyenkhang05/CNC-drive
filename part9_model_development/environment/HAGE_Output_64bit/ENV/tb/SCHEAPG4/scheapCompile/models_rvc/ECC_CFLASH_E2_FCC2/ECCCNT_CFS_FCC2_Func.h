// ---------------------------------------------------------------------
// $Id: ECCCNT_CFS_FCC2_Func.h,v 1.5 2019/09/06 08:33:53 landang Exp $
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
#ifndef __ECCCNT_CFS_FCC2_FUNC_H__
#define __ECCCNT_CFS_FCC2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ecccnt_cfs_fcc2_regif.h"
#include "ECC_CFLASH_E2_FCC2_AgentController.h"
#include "global.h"

class ECCCNT_CFS_FCC2_Func: public Cecccnt_cfs_fcc2_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECCCNT_CFS_FCC2_Func);
    ECCCNT_CFS_FCC2_Func(std::string name, ECC_CFLASH_E2_FCC2_AgentController* CFLASHAgentController);
    ~ECCCNT_CFS_FCC2_Func();

    void EnableReset(const std::string reset_name, const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    unsigned int GetCFSECCCTLReg();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    ECC_CFLASH_E2_FCC2_AgentController   *mpCFLASHAgentController;    // KCPROT Agent controller pointer
    std::string mModuleName;    // Module name
    std::map<unsigned int, std::string> mProtectedRegMap;   // Protected registers map   

    // Callback
    void cb_CFSECCCTL_ECCDIS(RegCBstr str);

};
#endif //__ECCCNT_CFS_FCC2_FUNC_H__
