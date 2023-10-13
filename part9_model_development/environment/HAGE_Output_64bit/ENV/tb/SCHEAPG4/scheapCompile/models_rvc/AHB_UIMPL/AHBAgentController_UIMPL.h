// ---------------------------------------------------------------------
// $Id: AHBAgentController_UIMPL.h,v 1.1 2013/11/26 03:52:54 dungvannguyen Exp $
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
#ifndef __AHB_AGENT_CONTROLLER_UIMPL__
#define __AHB_AGENT_CONTROLLER_UIMPL__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class AHBAgentController_UIMPL
{
#include "AHB_UIMPL_cmdif.h"
public:
    AHBAgentController_UIMPL()
    {
        ;
    }
    virtual ~AHBAgentController_UIMPL()
    {
        ;
    }
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    virtual void ControlIntr (void) = 0;
    virtual void TransferTransInfo (TlmBasicPayload &trans) = 0;
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual void WaitTimeToClearErrorFlags (bool ovf_flag) = 0;
};
#endif //__AHB_AGENT_CONTROLLER_UIMPL__
