// ---------------------------------------------------------------------
// $Id: DFE0051_AgentController.h,v 1.2 2017/07/21 08:48:22 chuonghoangle Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DFE0051_AGENT_CONTROLLER_H__
#define __DFE0051_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class Cdfe0051_agent_controller
class Cdfe0051_agent_controller
{
#include "DFE0051_cmdif.h"
public:
    // Enums
    
    // Constructor/Destructor
    Cdfe0051_agent_controller(void) {;}
    virtual ~Cdfe0051_agent_controller(void) {;}

    // Interface (APIs)
    virtual void StartSWInput() = 0;
    virtual void IssueSWTrigger() = 0;
    virtual void UpdateChannelEnable(unsigned int channel, bool enable) = 0;
    virtual void UpdateCaptureEnable(unsigned int capid, bool enable) = 0;
    virtual void AutoInitialize(unsigned int channel) = 0;
    virtual void RaiseINTDFE0051ERR() = 0;
    virtual void DisableSubtractionFlag(unsigned int subid)= 0;
};
#endif
