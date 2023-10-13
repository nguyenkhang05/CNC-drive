// ---------------------------------------------------------------------
// $Id: ATU5_TIMERC_SUB_AgentController.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERC_SUB_AGENT__
#define __ATU5_TIMERC_SUB_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ATU5_TIMERC;
class ATU5_Timer;
class ATU5_NoiseCanceler;
class ATU5_TIMERC_SUB_AgentController
{
#include "ATU5_TIMERC_SUB_cmdif.h"
public:
    enum eMode {
        emComp = 0,
        emCapture,
        emPWM,
        emOneshot
    };
    enum eChannel {
        emCh0 = 0,
        emCh1,
        emCh2,
        emCh3,
        emChannel
    };
    ATU5_TIMERC *mTIMERC;
    ATU5_TIMERC_SUB_AgentController(void) {;}
    bool mIs_Upper_Limit;
    bool mIsOneShotRunning[4];
    bool mIsOneshot[4]; // user for Oneshot mode
    bool mIsForcedOCRC[4];
    bool mIsForcedGRC[4];
    bool mIsTSTRCTrue;
    bool mIsGRCoccur[ATU5_TIMERC_SUB_AgentController::emChannel];  // added for ver 0.080  181
    bool mIsTSGRCoccur[ATU5_TIMERC_SUB_AgentController::emChannel];  // added for ver 0.080  181 
    bool mIsOCRCoccur[ATU5_TIMERC_SUB_AgentController::emChannel];  // added for ver 0.080  181
    bool mIsTSOCRCoccur[ATU5_TIMERC_SUB_AgentController::emChannel];  // added for ver 0.080  181 
    sc_event mPriorityEvent[ATU5_TIMERC_SUB_AgentController::emChannel]; // added for ver 0.080  181
    unsigned int mChannelMode[4];
    unsigned int mChannelValue[4];
    unsigned int mOCRCValue[4];
    unsigned int mGRCValue[4];
    unsigned int mRangeR1C[4]; // modified for ver 0.080
    unsigned int mRangeR2C[4];  // modified for ver 0.080
    unsigned int mUpdatedTCNTC;
    virtual ~ATU5_TIMERC_SUB_AgentController(void) {;}
    virtual void SetCounterValue(const unsigned int value) = 0;
    virtual void SetOCRCValue(const unsigned int channel, const unsigned int value) = 0;
    virtual void SetGRCValue(const unsigned int channel, const unsigned int value) = 0;
    virtual void SetUpperLimit(const unsigned int value) = 0;
    virtual void StartOneShotMethod(const unsigned int channel, const unsigned int value) = 0;
    virtual void HandleForcedCmpMatchMethod(const unsigned int channel) = 0;
    virtual void UpdateNoiseCounterValue(const unsigned int channel, const unsigned int value) = 0;
    virtual void SetNoiseCORVal(const unsigned int chan_num, const unsigned int cor_val) = 0;
    virtual void UpdateInRunning(const std::string name) = 0;
    virtual void UpdatingCntClock(void) = 0;
    virtual unsigned int GetNoiseCounterValue(const unsigned int channel) = 0;
    virtual unsigned int GetCounterValue(void) = 0;
    virtual void OffCntUpperLimit(void) = 0;
    virtual void SetModeDuringRunning(void) = 0;
    virtual void RangeCmpOCRC(const double range_value, const unsigned int channel) = 0;
    virtual void RangeCmpGRC(const double range_value, const unsigned int channel) = 0;
    virtual void OffPWM(void) = 0;//add 0815
    virtual void ControlTOCE(const unsigned int channel, const bool value) = 0;
    virtual void SetPriority(const unsigned int channel, const bool value) = 0; // ver 0.080
};
#endif
