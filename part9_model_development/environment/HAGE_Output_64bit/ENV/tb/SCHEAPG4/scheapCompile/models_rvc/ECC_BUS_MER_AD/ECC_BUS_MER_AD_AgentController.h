// ---------------------------------------------------------------------
// $Id: ECC_BUS_MER_AD_AgentController.h,v 1.3 2020/04/01 06:41:19 thunguyen3 Exp $
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
#ifndef __ECC_BUS_MER_AD_AGENT_CONTROLLER_H__
#define __ECC_BUS_MER_AD_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class ECC_BUS_MER_AD_AgentController
class ECC_BUS_MER_AD_AgentController
{
#include "ECC_BUS_MER_AD_cmdif.h"
public:
    // Constructor/Destructor
    ECC_BUS_MER_AD_AgentController(void) {;}
    virtual ~ECC_BUS_MER_AD_AgentController(void) {;}

    // Interface (APIs)
    virtual bool CheckClockPeriod() = 0;
    virtual bool GetResetStatus() = 0;
    virtual void WriteDisablePort(unsigned int dis_value) = 0;
    virtual void AssertInterrupt(std::string int_name) = 0;
    virtual void UpdateKeyCodeEnable(bool key_code_enable) = 0;
};
#endif //__ECC_BUS_MER_AD_AGENT_CONTROLLER_H__
