// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERC_AgentController.h,v 1.0 2016/07/12 01:45:48 quangthinhnguyen Exp $
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
#ifndef __ATU5010_TIMERC_AGENT__
#define __ATU5010_TIMERC_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ATU5010;
class ATU5010_TIMERC_SUB;
class ATU5010_TIMERC_AgentController
{
#include "ATU5010_TIMERC_cmdif.h"
public:
    ATU5010            *mATU5010;
    enum eSub {
        emSub0,
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
        emSub =  14
        };
    
    ATU5010_TIMERC_AgentController(void) {;}
    virtual ~ATU5010_TIMERC_AgentController(void) {;}
    virtual void StartStopSubblock(const unsigned int sub, const bool is_start) = 0;
    virtual void SetNoiseCancelMode(const unsigned int sub) = 0;
    virtual void SetNoiseEachCancelMode(const unsigned int sub, const unsigned int channel) = 0; //add 0820
    ATU5010_TIMERC_SUB *mTIMERC_SUB[emSub];   // timerC unit instance
    //virtual function
};
#endif
