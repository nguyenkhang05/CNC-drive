// ---------------------------------------------------------------------
// $Id: DFE_AgentController.h,v 1.2 2018/09/06 10:17:08 huepham Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DFE_AGENT_CONTROLLER_H__
#define __DFE_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class Cdfe_agent_controller
class Cdfe_agent_controller
{
#include "DFE_cmdif.h"
public:
    // Constructor/Destructor
    Cdfe_agent_controller(void) {;}
    virtual ~Cdfe_agent_controller(void) {;}

    // Interface (APIs)
    virtual void StartSWInput() = 0;
    virtual void IssueSWTrigger() = 0;
    virtual void UpdateChannelEnable(unsigned int channel, bool enable) = 0;
    virtual void UpdateCaptureEnable(unsigned int capid, bool enable) = 0;
    virtual void AutoInitialize(unsigned int channel) = 0;
    virtual void RaiseINTDFEERR() = 0;
    virtual void DisableSubtractionFlag(unsigned int subid)= 0;
    virtual void SetPeakHoldInitFlag(bool iSPHEnableFlag, unsigned int iChan) = 0;
    virtual void SetAccumDeciInitFlag(bool iSAcDeciEnableFlag, unsigned int iChan) = 0;
};
#endif
