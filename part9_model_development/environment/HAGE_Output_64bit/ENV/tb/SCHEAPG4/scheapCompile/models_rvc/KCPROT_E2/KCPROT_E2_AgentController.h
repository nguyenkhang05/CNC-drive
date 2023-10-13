// ---------------------------------------------------------------------
// $Id: KCPROT_E2_AgentController.h,v 1.2 2019/03/05 04:17:23 synguyen Exp $
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
#ifndef __KCPROT_E2_AGENT_CONTROLLER_H__
#define __KCPROT_E2_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class KCPROT_E2_AgentController
class KCPROT_E2_AgentController
{
#include "KCPROT_E2_cmdif.h"
public:
    // Constructor/Destructor
    KCPROT_E2_AgentController(void) {;}
    virtual ~KCPROT_E2_AgentController(void) {;}

    // Interface (APIs)
    virtual void UpdateKeyCodeEnable(bool key_code_enable) = 0;
    virtual unsigned int GetKeyCode(void) = 0;
    virtual bool CheckClockPeriod() = 0;
    virtual bool GetResetStatus() = 0;
};
#endif //__KCPROT_E2_AGENT_CONTROLLER_H__
