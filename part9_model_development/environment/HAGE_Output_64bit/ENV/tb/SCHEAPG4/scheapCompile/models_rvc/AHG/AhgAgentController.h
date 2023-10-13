// ---------------------------------------------------------------------
// $Id: AhgAgentController.h,v 1.2 2014/03/03 06:28:41 dungvannguyen Exp $
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

#ifndef __AHG_AGENT_CONTROLLER__
#define __AHG_AGENT_CONTROLLER__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class AhgAgentController
{
#include "AHG_cmdif.h"
public:
    AhgAgentController()
    {
        ;
    }
    virtual ~AhgAgentController()
    {
        ;
    }
    virtual bool CheckClock(std::string clock_name) = 0;
    virtual bool CheckAccessPermission(TlmBasicPayload &trans, const bool debug, bool &is_legal_trans) = 0;
    virtual void TransferErrInfo(TlmBasicPayload &trans) = 0;
    virtual void WaitTimeToClearErrorFlags(bool ovf_flag) = 0;
    virtual bool GetResetStatus(void) = 0;
    virtual void NotifyTransComp(void) = 0;
};
#endif
