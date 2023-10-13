// ---------------------------------------------------------------------
// $Id: SEG_AgentController.h,v 1.2 2014/03/13 07:21:49 dungvannguyen Exp $
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
#ifndef __SEG_AGENTCONTROLLER_H__
#define __SEG_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class SEG_AgentController
{
#include "SEG_cmdif.h"
public:
    SEG_AgentController(void) {;}
    virtual ~SEG_AgentController(void) {;}
    virtual bool GetResetStatus (const std::string reset_name)=0;
    virtual bool CheckClockPeriod (const std::string clock_name)=0;
    virtual void NotifyErrorOutput (const unsigned char index, const bool is_err_input_signal)=0;
    virtual void ArbitrateErrors (void)=0;
};
#endif
