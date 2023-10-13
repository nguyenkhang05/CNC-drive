// ---------------------------------------------------------------------
// $Id: ATU5_TIMERC_AgentController.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019  Renesas Electronics Corporation
// Copyright(c) 2016-2019  Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERC_AGENT__
#define __ATU5_TIMERC_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ATU5_Base;
class ATU5_TIMERC_SUB;
class ATU5_TIMERC_AgentController
{
#include "ATU5_TIMERC_cmdif.h"
public:
    ATU5_Base            *mATU5;
    unsigned int msbC ; 
    
    ATU5_TIMERC_AgentController(void) {;}
    virtual ~ATU5_TIMERC_AgentController(void) {;}
    virtual void StartStopSubblock(const unsigned int sub, const bool is_start) = 0;
    virtual void SetNoiseCancelMode(const unsigned int sub) = 0;
    virtual void SetNoiseEachCancelMode(const unsigned int sub, const unsigned int channel) = 0; //add 0820
    ATU5_TIMERC_SUB **mTIMERC_SUB ;  // timerC unit instance
    //virtual function
};
#endif
