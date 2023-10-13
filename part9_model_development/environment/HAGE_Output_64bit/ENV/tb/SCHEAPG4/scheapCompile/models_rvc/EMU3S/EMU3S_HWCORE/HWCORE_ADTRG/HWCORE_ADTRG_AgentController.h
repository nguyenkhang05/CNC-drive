// ---------------------------------------------------------------------
// $Id: HWCORE_ADTRG_AgentController.h,v 1.3 2020/10/14 09:55:35 huepham Exp $
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
// Description: Copyright of HWCORE_ADTRG_AgentController.h
// Ref: {HWCORE_ADTRG_UD_Copyright_001}
#ifndef __HWCORE_ADTRG_AGENT_CONTROLLER_H__
#define __HWCORE_ADTRG_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "global.h"
#include "BusMasterBase.h"

// Description: Declaration of HWCORE_ADTRG_AgentController class
// Ref: {HWCORE_ADTRG_UD_ATT1_004}
class HWCORE_ADTRG_AgentController
{
#include "HWCORE_ADTRG_cmdif.h"
public:
    // Constructor/Destructor
    HWCORE_ADTRG_AgentController(void) { ; }
    virtual ~HWCORE_ADTRG_AgentController(void) { ; }

    // Interface (APIs)
    virtual bool GetResetStatus(std::string resetname) = 0;
    virtual bool CheckClockPeriod(std::string clockname) = 0;
    virtual void HandleADStartTriggerMethod() = 0;
    virtual void HandleAngLatchingRegisterMethod() = 0;
    virtual void UpdateADCompletionTriggerControlPort(unsigned char adend) = 0;
    virtual void GetDelayTimer(bool swtrg) = 0;
    virtual void notifyTransComp(TlmTransactionType &trans, BusTime_t &t) = 0;
    virtual bool IsDelayPeriod(void) = 0;
    virtual void HandleADConversionResult(unsigned short AD0, unsigned short AD1, unsigned short AD2) = 0;
};
#endif //__HWCORE_ADTRG_AGENT_CONTROLLER_H__
