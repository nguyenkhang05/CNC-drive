// ---------------------------------------------------------------------
// $Id: RHSB_XBAR_AgentController.h 1339 2019-11-14 09:01:36Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __RHSB_XBAR_AGENT__
#define __RHSB_XBAR_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class RHSB_XBAR_AgentController
{
#include "RHSB_XBAR_cmdif.h"
public:
    RHSB_XBAR_AgentController(void) {;}
    virtual ~RHSB_XBAR_AgentController(void) {;}
    virtual bool CheckClockPeriod (void) = 0;
    virtual void SelectforOutput (const unsigned int pos_out, const unsigned int pos_in ,const std::string from_input) = 0;
    virtual void WriteOutput (void) = 0;
    virtual bool GetResetStatus(void) = 0;
};
#endif
