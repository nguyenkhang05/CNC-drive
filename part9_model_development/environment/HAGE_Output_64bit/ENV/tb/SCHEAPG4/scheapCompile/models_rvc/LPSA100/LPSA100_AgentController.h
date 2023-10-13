// ---------------------------------------------------------------------
// $Id: LPSA100_AgentController.h,v 1.2 2020/01/13 02:56:08 hadinhtran Exp $
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
#ifndef __LPSA100_AGENT_CONTROLLER_H__
#define __LPSA100_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>    
#include <stdarg.h>

// Class LPSA100_AgentController
class LPSA100_AgentController
{
#include "LPSA100_cmdif.h"
public:  
    // Constructor/Destructor
    LPSA100_AgentController(void) {;}
    virtual ~LPSA100_AgentController(void) {;}

    // Interface (APIs)
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual bool GetResetStatus() = 0;
    virtual void NotifyEventFlagCleared() = 0;
    virtual void EnableDigitalInputMode() = 0;
    virtual void StopDigitalInputMode() = 0;
    virtual void EnableAnalogInputMode() = 0;
    virtual void StopAnalogInputMode() = 0;
};
#endif //__LPSA100_AGENT_CONTROLLER_H__
