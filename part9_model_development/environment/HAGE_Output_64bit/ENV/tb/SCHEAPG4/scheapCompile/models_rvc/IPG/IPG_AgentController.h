// ---------------------------------------------------------------------
// $Id: IPG_AgentController.h,v 1.2 2014/03/13 01:08:13 dungvannguyen Exp $
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
#ifndef __IPG_AgentController_H__
#define __IPG_AgentController_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

struct Csyserripg;

class IPG_AgentController
{
#include "IPG_cmdif.h"
public:
    IPG_AgentController(void) {;}
    virtual ~IPG_AgentController(void) {;}
    virtual bool CheckClockPeriod (const std::string clock_name)=0;
    virtual bool GetResetStatus (void)=0;
    virtual bool CheckAccessPermission (const bool is_ifu, TlmBasicPayload& trans)=0;
    virtual void SendOutputErrorPort (const Csyserripg err_port_value)=0;
};
#endif
