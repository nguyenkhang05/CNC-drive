// ---------------------------------------------------------------------
// $Id: PWBRGA0_AgentController.h,v 1.4 2018/08/23 06:24:32 chanle Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PWBRGA0_AGENT_CONTROLLER_H__
#define __PWBRGA0_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class PWBRGA0_AgentController
class PWBRGA0_AgentController
{
#include "PWBRGA0_cmdif.h"
public:
    // Enums
    enum eGeneral{
        emNumClkOut = 4
    };

    // Constructor/Destructor
    PWBRGA0_AgentController(void) {;}
    virtual ~PWBRGA0_AgentController(void) {;}

    // Interface (APIs)
    virtual bool GetResetStatus() = 0;
    virtual bool CheckClockPeriod(std::string clock_name) = 0;
    virtual void StartCounter(unsigned int next_te_value) = 0;
    virtual void StopCounter(unsigned int next_te_value) = 0;
    virtual void UpdateEmulationStatus() = 0;
};
#endif //__PWBRGA0_AGENT_CONTROLLER_H__
