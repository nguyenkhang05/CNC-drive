// ---------------------------------------------------------------------
// $Id: HWCORE_IIR_AgentController.h,v 1.1 2020/08/18 07:30:11 minhquocha Exp $
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
// Description: Copyright of HWCORE_IIR_AgentController.h
// Ref: {HWCORE_IIR_UD_Copyright_001}
#ifndef __HWCORE_IIR_AGENT_CONTROLLER_H__
#define __HWCORE_IIR_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Description: Declaration of HWCORE_IIR_AgentController class
// Ref: {HWCORE_IIR_UD_ATT1_004}
class HWCORE_IIR_AgentController
{
#include "HWCORE_IIR_cmdif.h"
public:
    // Constructor/Destructor
    HWCORE_IIR_AgentController(void) { ; }
    virtual ~HWCORE_IIR_AgentController(void) { ; }

    // Interface (APIs)
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod(void) = 0;
    virtual void callback_IIRSFT_TRG(void) = 0;
    virtual void callback_IIRRLD(void) = 0;
    virtual void callback_IIRRSL(void) = 0;
};
#endif //__HWCORE_IIR_AGENT_CONTROLLER_H__
