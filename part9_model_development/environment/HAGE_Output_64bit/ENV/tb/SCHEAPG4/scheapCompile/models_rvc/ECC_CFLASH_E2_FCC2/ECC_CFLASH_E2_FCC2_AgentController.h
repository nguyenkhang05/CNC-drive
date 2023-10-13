// ---------------------------------------------------------------------
// $Id: ECC_CFLASH_E2_FCC2_AgentController.h,v 1.3 2019/08/20 11:55:08 tientran2 Exp $
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
#ifndef __ECC_CFLASH_E2_FCC2_AGENT_CONTROLLER_H__
#define __ECC_CFLASH_E2_FCC2_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ECC_CFLASH_E2_FCC2_AgentController
{
#include "ECC_CFLASH_E2_FCC2_cmdif.h"
public:
    // Constructor/Destructor
    ECC_CFLASH_E2_FCC2_AgentController(void) {;}
    virtual ~ECC_CFLASH_E2_FCC2_AgentController(void) {;}

    // Interface (APIs)
    virtual void WriteDisPort_CFP(unsigned int pe_id, unsigned int value) = 0;
    virtual void WriteDisPort_CFS(unsigned int value) = 0;
    virtual void WriteDisPort_CFC(unsigned int cfc_id, unsigned int value) = 0;

    virtual unsigned int GetCapDepth() = 0;
    virtual void AssertERROVFOUT0_CF() = 0;
    virtual void AssertERROVFOUT1_CF() = 0;

    virtual bool GetKeyOpen() = 0;
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual unsigned int GetAWIDTH_MSB() = 0;
};
#endif //__ECC_CFLASH_E2_FCC2_AGENT_CONTROLLER_H__
