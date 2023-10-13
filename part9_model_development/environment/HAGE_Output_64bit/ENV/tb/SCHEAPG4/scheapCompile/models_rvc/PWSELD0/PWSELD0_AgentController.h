// ---------------------------------------------------------------------
// $Id: PWSELD0_AgentController.h,v 1.4 2020/01/15 13:23:55 landang Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PWSELD0_AGENT_CONTROLLER_H__
#define __PWSELD0_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class PWSELD0_AgentController
class PWSELD0_AgentController
{
#include "PWSELD0_cmdif.h"
public:
    // Enums
    enum eGeneral{
        emNumConnectedPWG   = 96
        ,emNumConnectedADC  = 3
        ,emEmpty            = 0x7F
    };

    // Constructor/Destructor
    PWSELD0_AgentController(void) {;}
    virtual ~PWSELD0_AgentController(void) {;}

    // Interface (APIs)
    virtual bool GetResetStatus() = 0;
    virtual bool CheckClockPeriod() = 0;
    virtual void EnablePWS(bool is_enable) = 0;
    virtual void AssertInterrupt() = 0;
    virtual void ClearFlag(unsigned int channel) = 0;
};
#endif //__PWSELD0_AGENT_CONTROLLER_H__
