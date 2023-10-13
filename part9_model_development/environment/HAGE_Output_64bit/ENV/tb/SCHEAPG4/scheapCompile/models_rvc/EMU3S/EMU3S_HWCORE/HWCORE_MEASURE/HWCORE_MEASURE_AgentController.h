// ---------------------------------------------------------------------
// $Id: HWCORE_MEASURE_AgentController.h,v 1.2 2020/09/01 10:06:12 phuongtran Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of HWCORE_MEASURE_AgentController.h
// Ref: {HWCORE_MEASURE_UD_Copyright_001}
#ifndef __HWCORE_MEASURE_AGENT_CONTROLLER_H__
#define __HWCORE_MEASURE_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Description: Declaration of HWCORE_MEASURE_AgentController class
// Ref: {HWCORE_MEASURE_UD_ATT1_004}
class HWCORE_MEASURE_AgentController
{
#include "HWCORE_MEASURE_cmdif.h"
public:
    // Constructor/Destructor
    HWCORE_MEASURE_AgentController(void) { ; }
    virtual ~HWCORE_MEASURE_AgentController(void) { ; }

    // Interface (APIs)
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual void callback_PMT2CTR_EN(void) = 0;
    virtual void callback_PMT2SFT_SCAPTRG(void) = 0;
    virtual void callback_PMT2CNT_DATA_write(void) = 0;
    virtual void callback_PMT2CNT_DATA_read(void) = 0;
};
#endif //__HWCORE_MEASURE_AGENT_CONTROLLER_H__
