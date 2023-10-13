// ---------------------------------------------------------------------
// $Id: ECC_CRAM_E2_AgentController.h,v 1.1 2019/09/26 10:07:06 landang Exp $
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
#ifndef __ECC_CRAM_E2_AGENT_CONTROLLER_H__
#define __ECC_CRAM_E2_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class ECC_CRAM_E2_AgentController
class ECC_CRAM_E2_AgentController
{
#include "ECC_CRAM_E2_cmdif.h"
public:
    // Constructor/Destructor
    ECC_CRAM_E2_AgentController(void) {;}
    virtual ~ECC_CRAM_E2_AgentController(void) {;}

    // Interface (APIs)
    virtual void WriteDisPort_CRC(unsigned int pe_id, unsigned int value) = 0;
    virtual void WriteDisPort_CRA(unsigned int value) = 0;

    virtual unsigned int GetCapDepth() = 0;
    virtual void AssertERROVFOUT0_CR() = 0;
    virtual void AssertERROVFOUT1_CR() = 0;

    virtual bool GetKeyOpen() = 0;
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual unsigned int GetAWIDTH_MSB() = 0;
};
#endif //__ECC_CRAM_E2_AGENT_CONTROLLER_H__
