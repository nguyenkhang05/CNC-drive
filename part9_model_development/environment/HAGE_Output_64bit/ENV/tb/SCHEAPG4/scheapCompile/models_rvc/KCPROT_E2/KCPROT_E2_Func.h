// ---------------------------------------------------------------------
// $Id: KCPROT_E2_Func.h,v 1.4 2019/09/06 08:50:02 landang Exp $
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
#ifndef __KCPROT_E2_FUNC_H__
#define __KCPROT_E2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "kcprot_e2_regif.h"
#include "KCPROT_E2_AgentController.h"
#include "global.h"

class KCPROT_E2_Func: public Ckcprot_e2_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (KCPROT_E2_Func);
    KCPROT_E2_Func(std::string name, KCPROT_E2_AgentController* KCPROTAgentController);
    ~KCPROT_E2_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    KCPROT_E2_AgentController   *mpKCPROTAgentController;    // KCPROT Agent controller pointer
    std::string mModuleName;    // Module name

    // Function

    // Callback
    void cb_KCPROT_KCE(RegCBstr str);

};
#endif //__KCPROT_E2_FUNC_H__
