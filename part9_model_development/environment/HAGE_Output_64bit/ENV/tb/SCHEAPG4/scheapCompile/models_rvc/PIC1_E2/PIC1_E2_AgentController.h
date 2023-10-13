// ---------------------------------------------------------------------
// $Id: PIC1_E2_AgentController.h 3395 2017-06-29 08:02:34Z ChinhTD $
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
#ifndef __PIC1_E2_AGENT__
#define __PIC1_E2_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class PIC1_E2_AgentController
{
#include "PIC1_E2_cmdif.h"
public:
    double mTimeCLKChangePoint;              //The time clock is changed its value
    PIC1_E2_AgentController(void) {;}
    virtual ~PIC1_E2_AgentController(void) {;}
    virtual void StartSimultaneous (const std::string signal_name) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
};
#endif
