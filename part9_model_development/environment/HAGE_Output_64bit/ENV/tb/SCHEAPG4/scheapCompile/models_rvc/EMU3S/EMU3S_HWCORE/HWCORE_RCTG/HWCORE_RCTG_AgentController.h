// ---------------------------------------------------------------------
// $Id: HWCORE_RCTG_AgentController.h,v 1.2 2020/08/20 02:54:26 khoaanhnguyen Exp $
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
// Description: Copyright of HWCORE_RCTG_AgentController.h
// Ref: {HWCORE_RCTG_UD_Copyright_001}
#ifndef __HWCORE_RCTG_AGENT_CONTROLLER_H__
#define __HWCORE_RCTG_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Description: Declaration of HWCORE_RCTG_AgentController class
class HWCORE_RCTG_AgentController
{
#include "HWCORE_RCTG_cmdif.h"
public:
    //Interrupt index
    enum eInterrupt{
        emOUTPUT_RCTG_END_SP_REG = 0,
        emWC_EM2ICMP0,
        emWC_EM2ICMP1,
        // Number of interrupts
        emINTERRUPT_NUM,
    };

    enum eOutput{
        // 12-bit output ports
        emRDATA_EM2IICMP0 = 0,
        emEM2ICMP0,
        emEM2ICMP1,
        // 1-bit output ports
        emEM2IBRECCTR_SCLTCMP0_REG,
        emRCTG_U_80M_REG,
        emRCTG_V_80M_REG,
        emRCTG_W_80M_REG,
        // Number of output ports 
        emOUTPUT_PORTS_NUM,
    };

    // Constructor/Destructor
    HWCORE_RCTG_AgentController(void) { ; }
    virtual ~HWCORE_RCTG_AgentController(void) { ; }

    // Interface (APIs)
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod(void) = 0;

    // APIs for updating ports + interrupts
    virtual void NotifyAssert_DeassertInterrupt(unsigned int InterruptIndex) = 0;
    virtual void NotifyUpdateOutputPorts(unsigned int OutputIndex, unsigned int value) = 0;
};
#endif //__HWCORE_RCTG_AGENT_CONTROLLER_H__
