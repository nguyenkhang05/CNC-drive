// ---------------------------------------------------------------------
// $Id: DNFP020_E2_AgentController.h 2241 2017-03-08 02:30:35Z ChinhTD $
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
#ifndef __DNFP020_E2_AGENT_CONTROLLER_H__
#define __DNFP020_E2_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
//#include "tlm.h"      // for OSCI TLM2.0
#include "OSCI2.h"    // class for User Modeling environment based on TLM2.0

// Class DNFP020_E2_AgentController
class DNFP020_E2_AgentController
{
#include "DNFP020_E2_cmdif.h"
public:
    // variables
    sc_dt::uint64 mPCLKPeriod;          //Period value of clock
    double mTimeCLKChangePoint;              //The time clock is changed its value
    sc_time_unit mTimeResolutionUnit;      //Time resolution unit of simulation
    // Constructor/Destructor
    DNFP020_E2_AgentController(void) {;}
    virtual ~DNFP020_E2_AgentController(void) {;}

    // Interface (APIs)
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual double CalculateNextClockEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time) = 0;
    virtual void ReadSub (unsigned int ch_id, unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug) = 0;
    virtual void WriteSub (unsigned int ch_id, unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug) = 0;
    virtual unsigned int GetSLST(void) = 0;
    virtual unsigned int GetPRS(void) = 0;
    virtual bool GetSubNFEN(void) = 0;
};
#endif
