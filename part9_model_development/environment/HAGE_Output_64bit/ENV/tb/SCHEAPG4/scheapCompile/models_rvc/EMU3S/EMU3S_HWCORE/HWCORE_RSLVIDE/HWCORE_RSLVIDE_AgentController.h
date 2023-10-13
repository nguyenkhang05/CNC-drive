// ---------------------------------------------------------------------
// $Id: HWCORE_RSLVIDE_AgentController.h,v 1.2 2020/10/22 04:06:16 ngathunguyen Exp $
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
// Description: Copyright of HWCORE_RSLVIDE_AgentController.h
// Ref: {HWCORE_RSLVIDE_UD_Copyright_001}
#ifndef __HWCORE_RSLVIDE_AGENT_CONTROLLER_H__
#define __HWCORE_RSLVIDE_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "global.h"
#include "BusMasterBase.h"

// Description: Declaration of HWCORE_RSLVIDE_AgentController class
// Ref: {HWCORE_RSLVIDE_UD_ATT1_004}
class HWCORE_RSLVIDE_AgentController
{
#include "HWCORE_RSLVIDE_cmdif.h"
public:
    // Constructor/Destructor
    HWCORE_RSLVIDE_AgentController(void) { ; }
    virtual ~HWCORE_RSLVIDE_AgentController(void) { ; }

    // Interface (APIs)
    virtual bool GetResetStatus() = 0;
    virtual bool CheckClockPeriod() = 0;
    virtual void StartAngleIP(void) = 0;
    virtual void UpdateANGCTRPort(unsigned char value) = 0;
    virtual void UpdateRESTHSFTPort(unsigned short value) = 0;
    virtual void UpdateTHTEFIXPort(unsigned short value) = 0;
    virtual void UpdateTHTRESFIXPort(unsigned short value) = 0;
    virtual void UpdateVariableValue(unsigned char var, unsigned short value) = 0;
    virtual unsigned short GetResolverAngleValue() = 0;
};
#endif //__HWCORE_RSLVIDE_AGENT_CONTROLLER_H__
