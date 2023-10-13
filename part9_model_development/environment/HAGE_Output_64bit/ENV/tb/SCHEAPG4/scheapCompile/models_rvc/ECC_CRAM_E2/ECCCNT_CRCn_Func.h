// ---------------------------------------------------------------------
// $Id: ECCCNT_CRCn_Func.h,v 1.1 2019/09/26 10:07:06 landang Exp $
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
#ifndef __ECCCNT_CRCn_FUNC_H__
#define __ECCCNT_CRCn_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ecccnt_crcn_regif.h"
#include "ECC_CRAM_E2_AgentController.h"
#include "global.h"

class ECCCNT_CRCn_Func: public Cecccnt_crcn_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECCCNT_CRCn_Func);
    ECCCNT_CRCn_Func(std::string name, unsigned int pe_id, ECC_CRAM_E2_AgentController* CRAMAgentController);
    ~ECCCNT_CRCn_Func();

    void EnableReset(const std::string reset_name, const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    unsigned int GetCRCAFCTLReg();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    ECC_CRAM_E2_AgentController   *mpCRAMAgentController;// KCPROT Agent controller pointer
    std::string     mModuleName;// Module name
    unsigned int    mPeId;      // PE index
    std::map<unsigned int, std::string> mProtectedRegMap;   // Protected registers map   

    // Callback
    void cb_CRCAFCTL_AFEDIS(RegCBstr str);

};
#endif //__ECCCNT_CRCn_FUNC_H__
