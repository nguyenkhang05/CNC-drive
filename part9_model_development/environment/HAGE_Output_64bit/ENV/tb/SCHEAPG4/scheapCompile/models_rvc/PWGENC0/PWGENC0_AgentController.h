// ---------------------------------------------------------------------
// $Id: PWGENC0_AgentController.h,v 1.3 2018/08/24 06:38:28 chanle Exp $
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
#ifndef __PWGENC0_AGENT_CONTROLLER_H__
#define __PWGENC0_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class PWGENC0_AgentController
class PWGENC0_AgentController
{
#include "PWGENC0_cmdif.h"
public:
    // Enums
    enum eGeneral{
        emNumClkOut = 4
    };

    // Constructor/Destructor
    PWGENC0_AgentController(void) {;}
    virtual ~PWGENC0_AgentController(void) {;}

    // Interface (APIs)
    virtual bool GetResetStatus() = 0;
    virtual bool CheckClockPeriod(std::string clock_name) = 0;
    virtual void ClearTOUTPort() = 0;
    virtual void ForceTOUT(bool fos) = 0;
    virtual void LoadTCBREventAfter1PCLK() = 0;
    virtual void RequestSetRSFBit() = 0;
    virtual void RequestSetRSFTBit() = 0;
    virtual void RequestSetFOFBit() = 0;
    virtual void RequestClearRSFBit() = 0;
    virtual void RequestClearRSFTBit() = 0;
    virtual void RequestClearFOFBit() = 0;
    virtual void EnableCounter(bool is_enable) = 0;
    virtual unsigned int GetPERIOD() = 0;
    virtual void UpdateCurrentCounter(unsigned int value) = 0;
    virtual unsigned int GetCurrentCounter() = 0;
    virtual bool GetPWG_STE() = 0;
    virtual bool CheckCounterRunning() = 0;
};
#endif //__PWGENC0_AGENT_CONTROLLER_H__
