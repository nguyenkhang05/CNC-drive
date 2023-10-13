// ---------------------------------------------------------------------
// $Id: DNFP010_E2_AgentController.h 2179 2017-03-02 04:57:12Z ChinhTD $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DNFP010_E2_AGENT_CONTROLLER_H__
#define __DNFP010_E2_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class DNFP010_E2_AgentController
class DNFP010_E2_AgentController
{
#include "DNFP010_E2_cmdif.h"
public:
    // variables
    sc_dt::uint64 mdnfp01tsmpclkPeriod;          //Period value of clock
    double mTimeCLKChangePoint[2];              //The time clock is changed its value (0: PCLK, 1: dnfp01tsmpclk)
    sc_time_unit mTimeResolutionUnit;      //Time resolution unit of simulation
    // Constructor/Destructor
    DNFP010_E2_AgentController(void) {;}
    virtual ~DNFP010_E2_AgentController(void) {;}

    // Interface (APIs)
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual double CalculateNextClockEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time) = 0;
};
#endif
