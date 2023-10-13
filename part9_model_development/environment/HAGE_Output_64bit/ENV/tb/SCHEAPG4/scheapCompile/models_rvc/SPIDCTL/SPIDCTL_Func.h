// ---------------------------------------------------------------------
// $Id: SPIDCTL_Func.h,v 1.1 2020/01/21 08:14:34 khoaanhnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of SPIDCTL_Func.h
// Ref: {SPIDCTL_DD_Copyright_001}
#ifndef __SPIDCTL_FUNC_H__
#define __SPIDCTL_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "spidctl_regif.h"
#include "SPIDCTL_AgentController.h"

#define VALID_KCPROT_VALUE    0xA5A5A500    //Valid value when writing SPIDKCPROT

// Description: Declaration of SPIDCTL_Func class
// Ref: {SPIDCTL_DD_ATT1_004}
class SPIDCTL_Func : public Cspidctl_regif
    , public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS(SPIDCTL_Func);
    SPIDCTL_Func(std::string name, SPIDCTL_AgentController* SPIDCTLAgentControllerPtr);
    ~SPIDCTL_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void DumpStatusInfo(void);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    unsigned int GetSPIDReg_SPIDBit(unsigned int index);
    unsigned int GetSPIDMskReg(unsigned int index);

private:

    // Variables
    SPIDCTL_AgentController * mpSPIDCTLAgentController;    // SPIDCTL Agent controller pointer
    std::string     mModuleName;                           // Module name

    // Function
    void Initialize(void);
    // Callback
    void cb_BMSPID_SPID(RegCBstr str);
    void cb_BMSPIDMSK_SPIDMSK(RegCBstr str);
    void cb_BMSPIDMSKLOCK_LOCK(RegCBstr str);
    void cb_SPIDKCPROT_KCE(RegCBstr str);
};

#endif // __SPIDCTL_FUNC_H__
