// -----------------------------------------------------------------------------
// $Id: HWCORE_INPUT_AgentController.h,v 1.3 2020/10/21 04:27:23 quynhnhunguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// Description: Copyright of HWCORE_INPUT_AgentController.h
// Ref: {HWCORE_INPUT_UD_Copyright_001}
#ifndef __HWCORE_INPUT_AGENT_CONTROLLER_H__
#define __HWCORE_INPUT_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class HWCORE_INPUT_AgentController
class HWCORE_INPUT_AgentController
{
#include "HWCORE_INPUT_cmdif.h"
public:
    // Constructor/Destructor
    HWCORE_INPUT_AgentController(void) {;}
    virtual ~HWCORE_INPUT_AgentController(void) {;}

    // Interface (APIs)
    //user function here
    virtual bool CheckClockPeriod(void) = 0;
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual void NotifyUpdateOutputPorts(const std::string port_name, unsigned int value) = 0;
};
#endif //__HWCORE_INPUT_AGENT_CONTROLLER_H__
