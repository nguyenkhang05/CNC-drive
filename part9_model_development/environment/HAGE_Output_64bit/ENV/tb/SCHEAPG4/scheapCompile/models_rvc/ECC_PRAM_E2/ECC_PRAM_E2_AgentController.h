// ---------------------------------------------------------------------
// $Id: ECC_PRAM_E2_AgentController.h,v 1.2 2019/03/05 04:17:20 synguyen Exp $
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
#ifndef __ECC_PRAM_E2_AGENT_CONTROLLER_H__
#define __ECC_PRAM_E2_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class ECC_PRAM_E2_AgentController
class ECC_PRAM_E2_AgentController
{
#include "ECC_PRAM_E2_cmdif.h"
public:
    // Constructor/Destructor
    ECC_PRAM_E2_AgentController(void) {;}
    virtual ~ECC_PRAM_E2_AgentController(void) {;}

    // Interface (APIs)
    virtual unsigned int GetERR_STAG() = 0;
    virtual bool GetECTERVESig() = 0;
    virtual unsigned int GetWORD() = 0;
    virtual bool GetResetStatus() = 0;
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual void AssertECTIE1Port() = 0;
    virtual void AssertECTIE2Port() = 0;
    virtual void AssertECTIOVFPort() = 0;
    virtual void DriveECTPREFPort() = 0;
};
#endif //__ECC_PRAM_E2_AGENT_CONTROLLER_H__
