// ---------------------------------------------------------------------
// $Id: TSN_AgentController.h,v 1.0 2014/11/27 06:42:34 binhnguyen Exp $
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
#ifndef __TSN_AGENTCONTROLLER_H__
#define __TSN_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class TSN_AgentController
{
#include "TSN_cmdif.h"
public:
    enum eState {       // Define operation state of TSN
        emRESET,
        emDISABLE,
        emENABLE,
        emREADY,
        emDISCHARGE,
        emCONVERSION,
        emCONV_DONE
    };
    
    TSN_AgentController(void) {;}
    virtual ~TSN_AgentController(void) {;}
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    
    virtual eState GetState (void) = 0;
    virtual void EnableSetting (const bool is_enable) = 0;
    virtual void SelfDiagSetting (const bool is_enable) = 0;
};
#endif
