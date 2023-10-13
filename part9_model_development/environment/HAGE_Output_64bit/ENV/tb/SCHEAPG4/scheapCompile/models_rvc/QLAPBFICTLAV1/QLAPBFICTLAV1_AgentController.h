// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1_AgentController.h,v 1.3 2020/03/24 07:16:32 duongphan Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __QLAPBFICTLAV1_AGENT_CONTROLLER_H__
#define __QLAPBFICTLAV1_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include <string.h>

// Class _AgentController
class QLAPBFICTLAV1_Sub;
class QLAPBFICTLAV1_AgentController
{
#include "QLAPBFICTLAV1_cmdif.h"
public:

    enum eChId{
        emChNumMax = 8 // Number channel max (channel 0 -> 7)
    };
    enum eResetActiveLevel {    // Define active level of reset signal
        emResetActive = 0,
        emResetSignalNum = 2
    };

    enum mClkZero {    // Define active level of reset signal
        emClkZeroNum = 2
    };

public:
    QLAPBFICTLAV1_AgentController(void) {;}
    virtual ~QLAPBFICTLAV1_AgentController(void) {;}

    virtual unsigned int GetChId(void) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    QLAPBFICTLAV1_Sub *mQLAPBFICTLAV1_Sub[emChNumMax];                   // QLAPBFICTLAV1 class pointer
};
#endif //__QLAPBFICTLAV1_AGENT_CONTROLLER_H__
// vim600: set foldmethod=marker :
