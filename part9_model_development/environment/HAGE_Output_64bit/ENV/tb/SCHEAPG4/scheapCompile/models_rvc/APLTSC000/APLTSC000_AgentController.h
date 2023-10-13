// ---------------------------------------------------------------------
// $Id: APLTSC000_AgentController.h,v 1.2 2020/03/13 10:43:14 chuonghoangle Exp $
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
#ifndef __APLTSC000_AGENT_CONTROLLER_H__
#define __APLTSC000_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class APLTSC000_AgentController
class APLTSC000_AgentController
{
#include "APLTSC000_cmdif.h"
public:
    // Constructor/Destructor
    APLTSC000_AgentController(void) {;}
    virtual ~APLTSC000_AgentController(void) {;}

    // Interface (APIs)
    
    virtual void NotifyUpdateRMbit() = 0;
    virtual void ReturnValueCounter() = 0;
    virtual void UpdateCounterValue() = 0;
    virtual void StartCounter() = 0;
    virtual void StopCounter() = 0;
    virtual void MonitorSVSDIS() = 0;
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod() = 0;
};
#endif //__APLTSC000_AGENT_CONTROLLER_H__
