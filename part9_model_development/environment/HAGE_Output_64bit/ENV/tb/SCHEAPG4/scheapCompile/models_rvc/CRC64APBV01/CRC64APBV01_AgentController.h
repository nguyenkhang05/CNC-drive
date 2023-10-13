// ---------------------------------------------------------------------
// $Id: CRC64APBV01_AgentController.h,v 1.5 2019/12/07 10:28:36 ngathunguyen Exp $
//
// Copyright(c) 2018 - 2019 Renesas Electronics Corporation
// Copyright(c) 2018 - 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __CRC64APBV01_AGENT_CONTROLLER_H__
#define __CRC64APBV01_AGENT_CONTROLLER_H__
#include <systemc.h>

// Class CRC64APBV01_AgentController
class CRC64APBV01_AgentController
{
#include "CRC64APBV01_cmdif.h"
public:
    // Constructor/Destructor
    CRC64APBV01_AgentController(void) {;}
    virtual ~CRC64APBV01_AgentController(void) {;}

    // Interface (APIs)
    virtual bool GetResetStatus() = 0;
    virtual bool CheckClockPeriod() = 0;
};
#endif //__CRC64APBV01_AGENT_CONTROLLER_H__
