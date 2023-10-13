// ---------------------------------------------------------------------
// $Id: ECC_ETHERNET_E2_Func.h,v 1.4 2019/09/06 08:49:18 landang Exp $
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
#ifndef __ECC_ETHERNET_E2_FUNC_H__
#define __ECC_ETHERNET_E2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ecc_ethernet_e2_regif.h"
#include "ECC_ETHERNET_E2_AgentController.h"
#include "global.h"

class ECC_ETHERNET_E2_Func: public Cecc_ethernet_e2_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECC_ETHERNET_E2_Func);
    ECC_ETHERNET_E2_Func(std::string name, ECC_ETHERNET_E2_AgentController* ETHERNETAgentController);
    ~ECC_ETHERNET_E2_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // enum

    // Variables
    ECC_ETHERNET_E2_AgentController   *mpETHERNETAgentController;    // KCPROT Agent controller pointer
    std::string     mModuleName;// Module name

    // Callback
    void cb_ETCTXECCCTL_EC1ECP(RegCBstr str);
    void cb_ETCRXECCCTL_EC1ECP(RegCBstr str);

};
#endif //__ECC_ETHERNET_E2_FUNC_H__
