// ---------------------------------------------------------------------
// $Id: FCLA_AgentController.h,v 1.1 2014/06/10 10:02:43 dungvannguyen Exp $
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
#ifndef __FCLA_AGENTCONTROLLER_H__
#define __FCLA_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class FCLA_AgentController
{
#include "FCLA_cmdif.h"
public:
    FCLA_AgentController(void) {;}
    virtual ~FCLA_AgentController(void) {;}
    virtual bool GetResetStatus (const std::string reset_name)=0;
    virtual bool CheckClockPeriod (const std::string clock_name)=0;
    virtual void ControlEdgeSelectPorts (const unsigned int filter_no, const bool FCLATINTR_value, const bool FCLATINTF_value)=0;
    virtual void ControlFilterBypass (const unsigned int filter_no, const bool is_enable)=0;
    virtual void ControlLevelDetector (const unsigned int filter_no, const bool is_enable)=0;

};
#endif
