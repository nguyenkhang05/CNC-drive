// ---------------------------------------------------------------------
// $Id: HWCORE_INT_AgentController.h,v 1.1 2020/10/27 04:49:12 huyquocpham Exp $
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
// Description: Copyright of HWCORE_INT_AgentController.h
// Ref: {EMU3S_HWCORE_UD_Copyright_001}
#ifndef __HWCORE_INT_AGENT_CONTROLLER_H__
#define __HWCORE_INT_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Description: Declaration of HWCORE_INT_AgentController class
// Ref: {}
class HWCORE_INT_AgentController
{
#include "HWCORE_INT_cmdif.h"
public:
    // Constructor/Destructor
    HWCORE_INT_AgentController(void) { ; }
    virtual ~HWCORE_INT_AgentController(void) { ; }

    // Interface (APIs)
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod(void) = 0;
};
#endif //__HWCORE_INT_AGENT_CONTROLLER_H__
