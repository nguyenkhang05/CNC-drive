// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERD_AgentController.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_TIMERD_AGENT__
#define __ATU5010_TIMERD_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ATU5010;
class ATU5010_TIMERD_SUB;
class ATU5010_TIMERD_AgentController
{
#include "ATU5010_TIMERD_cmdif.h"
public:
    ATU5010 *mATU5010;
    enum eSub {
        emSub0 = 0,
        emSub1,
        emSub2,
        emSub3,
        emSub4,
        emSub5,
        emSub6,
        emSub7,
        emSub8,
		emSub9,
		emSub10,
		emSub11,
		emSub12,
		emSub13,
		emSub14,
        emSub
        };
    enum eChannel {
        emChannel0 = 0,
        emChannel1,
        emChannel2,
        emChannel3,
        emChannel
        };
    enum eClocks {
        emCLKBUS0ClockIndex = 0,
        emCLKBUS1ClockIndex,
        emCLKBUS2ClockIndex,
        emCLKBUS3ClockIndex,
        emCLKBUS4ClockIndex,
        emCLKBUS5ClockIndex,
        emPCLKClockIndex,
        emAllClockIndex
        };
    ATU5010_TIMERD_AgentController(void) {;}
    virtual ~ATU5010_TIMERD_AgentController(void) {;}
    ATU5010_TIMERD_SUB *mTIMERD_SUB[emSub];   // timerC unit instance
    //virtual function
};
#endif
