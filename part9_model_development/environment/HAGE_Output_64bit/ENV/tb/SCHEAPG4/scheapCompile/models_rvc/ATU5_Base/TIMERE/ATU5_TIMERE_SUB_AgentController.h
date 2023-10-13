// ---------------------------------------------------------------------
// $Id: ATU5_TIMERE_SUB_AgentController.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERE_SUB_AGENT__
#define __ATU5_TIMERE_SUB_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>


class ATU5_TIMERE;
class ATU5_Timer;
class ATU5_TIMERE_SUB_AgentController
{
#include "ATU5_TIMERE_SUB_cmdif.h"
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
    ATU5_TIMERE *mTIMERE;
    ATU5_TIMERE_SUB_AgentController(void) {;}
    bool     mIsCounterRunningEnable[ATU5_TIMERE_SUB_AgentController::emChannel];
    bool mTOEToggleRequest[ATU5_TIMERE_SUB_AgentController::emChannel];
    bool mChangeCounterValue[ATU5_TIMERE_SUB_AgentController::emChannel];
    bool mChangeDTREValue[ATU5_TIMERE_SUB_AgentController::emChannel];
    bool mChangeCYLRValue[ATU5_TIMERE_SUB_AgentController::emChannel];
    bool     mSubBlockEnable;
    bool mChannelEnable[ATU5_TIMERE_SUB_AgentController::emChannel];
    unsigned int mCYLREValue[ATU5_TIMERE_SUB_AgentController::emChannel];
    unsigned int mDTREValue[ATU5_TIMERE_SUB_AgentController::emChannel];
    bool mIsTimerRunning[ATU5_TIMERE_SUB_AgentController::emChannel];
    bool mChangeCountClkPeriod[ATU5_TIMERE_SUB_AgentController::emChannel];
    virtual ~ATU5_TIMERE_SUB_AgentController(void) {;}
    virtual void UpdateTCNTERegValue(unsigned int channel) = 0;
    
    bool     mSubBlockReloadEnable;
    //Check require clear from user
    bool mIsClearDMFEStatus[4];
    bool mIsClearOVFEStatus[4];
    bool mIsClearCMFEStatus[4];
    
    bool mIsTurnCMFE;
    bool mIsTurnOVFE;
    bool mIsTurnDMFE;
    
    sc_event mPreStartTimerEvent[ATU5_TIMERE_SUB_AgentController::emChannel];
    sc_event mStartTimerEvent[ATU5_TIMERE_SUB_AgentController::emChannel];
    sc_event mStopTimerEvent[ATU5_TIMERE_SUB_AgentController::emChannel];
    sc_event mPreForceCycleMatchEvent[ATU5_TIMERE_SUB_AgentController::emChannel];
    sc_event mForceCycleMatchEvent[ATU5_TIMERE_SUB_AgentController::emChannel];
    sc_event mRewriteCounterEvent[ATU5_TIMERE_SUB_AgentController::emChannel];
    sc_event mRestartPWMCycleEvent[ATU5_TIMERE_SUB_AgentController::emChannel];
    sc_event mPreTOEChangeRequestEvent[ATU5_TIMERE_SUB_AgentController::emChannel];
    sc_event mChangeClockRequestEvent;
    
    sc_event mProcessSettingEvent[ATU5_TIMERE_SUB_AgentController::emChannel];
    sc_event mProcessClearingEvent;
    //Notify Event Virtual function
    virtual void NotifyPreStartTimerEvent(const unsigned int index)=0;
    virtual void NotifyRewriteCounterEvent(const unsigned int index)=0;
    virtual void NotifyPreForceCycleMatchEvent(const unsigned int index)=0;
    virtual void NotifyRestartPWMCycleEvent(const unsigned int index)=0;
    virtual void NotifyPreTOEChangeRequestEvent(const unsigned int index)=0;
    virtual void NotifyChangeClockRequestEvent(void)=0;
    virtual void NotifyProcessClearingEvent(void)=0;

};
#endif
