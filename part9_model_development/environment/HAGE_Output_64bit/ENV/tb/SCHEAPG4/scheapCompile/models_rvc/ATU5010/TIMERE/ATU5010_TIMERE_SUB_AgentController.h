// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERE_SUB_AgentController.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#ifndef __ATU5010_TIMERE_SUB_AGENT__
#define __ATU5010_TIMERE_SUB_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>
class ATU5010_TIMERE;
class ATU5010_Timer;
class ATU5010_TIMERE_SUB_AgentController
{
#include "ATU5010_TIMERE_SUB_cmdif.h"
public:
    enum eChannel {
        emChannel0 = 0,
        emChannel1,
        emChannel2,
        emChannel3,
        emChannel
    };
    enum eCountClock {
        emClockBus0 = 0,
        emClockBus1,
        emClockBus2,
        emClockBus3,
        emClockBus4,
        emClockBus5,
        emPCLKClock,
        emAllClockBus
    };
    ATU5010_TIMERE *mTIMERE;
    ATU5010_TIMERE_SUB_AgentController(void) {;}
    bool     mIsCounterRunningEnable[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mTOEToggleRequest[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mChannelReloadEnable[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mChangeCounterValue[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mChangeDTREValue[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mChangeCYLRValue[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool     mSubBlockEnable;
    bool mChannelEnable[ATU5010_TIMERE_SUB_AgentController::emChannel];
    unsigned int mCYLREValue[ATU5010_TIMERE_SUB_AgentController::emChannel];
    unsigned int mDTREValue[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mIsTimerRunning[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mChangeCountClkPeriod[ATU5010_TIMERE_SUB_AgentController::emChannel];
    virtual ~ATU5010_TIMERE_SUB_AgentController(void) {;}
    virtual void UpdateTCNTERegValue(unsigned int channel) = 0;
    

    sc_event mPreStartTimerEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mStartTimerEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mStopTimerEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mPreForceCycleMatchEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mForceCycleMatchEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mRewriteCounterEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mRestartPWMCycleEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mPreTOEChangeRequestEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mChangeClockRequestEvent;

};
#endif
