// ---------------------------------------------------------------------
// $Id: APBAgentController_UIMPL.h,v 1.0 2013/11/19 09:21:57 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __APB_AGENT_CONTROLLER_UIMPL__
#define __APB_AGENT_CONTROLLER_UIMPL__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class APBAgentController_UIMPL
{
#include "APB_UIMPL_cmdif.h"
public:
    APBAgentController_UIMPL()
    {
        ;
    }
    virtual ~APBAgentController_UIMPL()
    {
        ;
    }
    virtual bool CheckClockPeriod (void) = 0;
    virtual void ControlIntr (void) = 0;
    virtual void TransferTransInfo (TlmBasicPayload &trans) = 0;
    virtual bool GetResetStatus (void) = 0;
};
#endif //__APB_AGENT_CONTROLLER_UIMPL__
