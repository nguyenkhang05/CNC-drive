// ---------------------------------------------------------------------
// $Id: ECC_ICACHE_E2_FCC2_AgentController.h,v 1.3 2019/03/05 04:30:05 synguyen Exp $
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
#ifndef __ECC_ICACHE_E2_FCC2_AGENT_CONTROLLER_H__
#define __ECC_ICACHE_E2_FCC2_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class ECC_ICACHE_E2_FCC2_AgentController
class ECC_ICACHE_E2_FCC2_AgentController
{
#include "ECC_ICACHE_E2_FCC2_cmdif.h"
public:
    // Constructor/Destructor
    ECC_ICACHE_E2_FCC2_AgentController(void) {;}
    virtual ~ECC_ICACHE_E2_FCC2_AgentController(void) {;}

    // Interface (APIs)
    virtual void WriteDisPort_IT(unsigned int pe_id, unsigned int value) = 0;
    virtual void WriteDisPort_ID(unsigned int pe_id, unsigned int value) = 0;

    virtual unsigned int GetCapDepth(std::string sub_name) = 0;
    virtual void AssertERROVFOUT0_IT(unsigned int pe_id) = 0;
    virtual void AssertERROVFOUT1_IT(unsigned int pe_id) = 0;
    virtual void AssertERROVFOUT0_ID(unsigned int pe_id) = 0;
    virtual void AssertERROVFOUT1_ID(unsigned int pe_id) = 0;

    virtual bool GetKeyOpen() = 0;
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
};
#endif //__ECC_ICACHE_E2_FCC2_AGENT_CONTROLLER_H__
