// ---------------------------------------------------------------------
// $Id: PEG_AgentController.h,v 1.2 2014/03/12 09:59:30 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __PEG_AGENT_CONTROLLER__
#define __PEG_AGENT_CONTROLLER__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class PEG_AgentController
{
#include "PEG_cmdif.h"
public:
    PEG_AgentController()
    {
        ;
    }
    virtual ~PEG_AgentController()
    {
        ;
    }
    virtual bool CheckAccessPermission(TlmBasicPayload &trans) = 0;
    virtual bool CheckClock(const std::string clock_name) = 0;
    virtual bool GetResetStatus(void) = 0;
};
#endif
