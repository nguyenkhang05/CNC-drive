// ---------------------------------------------------------------------
// $Id: DNFP020_E2_Func.h 2439 2017-03-21 09:51:52Z ChinhTD $
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
#ifndef __DNFP020_E2_FUNC_H__
#define __DNFP020_E2_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "dnfp020_e2_regif.h"

class DNFP020_E2_AgentController;

class DNFP020_E2_Func: public Cdnfp020_e2_regif,
                 public BusSlaveFuncIf
{
public:
    DNFP020_E2_Func (std::string name, DNFP020_E2_AgentController *DNFP020_E2_AgentControllerPtr);
    ~DNFP020_E2_Func (void);
    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    /// Public functions
    unsigned int GetSLST(void);
    unsigned int GetPRS(void);
private:
    DNFP020_E2_AgentController *mDNFP020_E2_AgentController;
    std::string mModuleName;        //Module name

    void cb_DNFP02CTL_SLST(RegCBstr str);
    void cb_DNFP02CTL_PRS(RegCBstr str);
};

#endif
