// ---------------------------------------------------------------------
// $Id: SPIDCTL_AgentController.h,v 1.1 2020/01/21 08:14:34 khoaanhnguyen Exp $
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
// Description: Copyright of SPIDCTL_AgentController.h
// Ref: {SPIDCTL_DD_Copyright_001}
#ifndef __SPIDCTL_AGENT_CONTROLLER_H__
#define __SPIDCTL_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Description: Declaration of SPIDCTL_AgentController class
// Ref: {SPIDCTL_DD_ATT1_004}
class SPIDCTL_AgentController
{
#include "SPIDCTL_cmdif.h"
public:
    // Constructor/Destructor
    SPIDCTL_AgentController(void) { ; }
    virtual ~SPIDCTL_AgentController(void) { ; }

    // Interface (APIs)
    virtual bool GetResetStatus(void) = 0;
    virtual bool CheckClockPeriod(void) = 0;
    virtual void NotifyUpdateSPIDValue(unsigned int Index) = 0;
    virtual bool IsKeyCodeEnable(void) = 0;
    virtual void SetKeyCodeEnable(bool isEnable) = 0;
    virtual bool IsMaskLocked(unsigned int RegIndex) = 0;
    virtual void SetMaskLocked(unsigned int RegIndex, bool isLocked) = 0;
};
#endif //__SPIDCTL_AGENT_CONTROLLER_H__
