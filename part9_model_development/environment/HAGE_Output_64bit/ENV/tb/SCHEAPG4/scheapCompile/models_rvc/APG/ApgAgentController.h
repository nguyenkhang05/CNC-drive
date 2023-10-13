// ---------------------------------------------------------------------
// $Id: ApgAgentController.h,v 1.2 2014/03/03 06:33:04 dungvannguyen Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __APG_AGENT_CONTROLLER__
#define __APG_AGENT_CONTROLLER__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ApgAgentController
{
#include "APG_cmdif.h"
public:
    ApgAgentController()
    {
        ;
    }
    virtual ~ApgAgentController()
    {
        ;
    }
    virtual bool CheckPCLKClock(void) = 0;
    virtual bool CheckAccessPermission(TlmBasicPayload &trans, const bool debug, bool &is_legal_trans) = 0;
    virtual void TransferErrInfo(TlmBasicPayload &trans) = 0;
    virtual bool GetResetStatus(void) = 0;
    virtual void NotifyTransComp(void) = 0;
};
#endif
