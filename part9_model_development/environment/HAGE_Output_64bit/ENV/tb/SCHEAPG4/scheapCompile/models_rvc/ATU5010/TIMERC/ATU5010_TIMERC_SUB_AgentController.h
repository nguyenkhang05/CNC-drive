// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERC_SUB_AgentController.h,v 1.0 2016/07/12 01:45:48 quangthinhnguyen Exp $
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
#ifndef __ATU5010_TIMERC_SUB_AGENT__
#define __ATU5010_TIMERC_SUB_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ATU5010_TIMERC;
class ATU5010_Timer;
class ATU5010_NoiseCanceler;
class ATU5010_TIMERC_SUB_AgentController
{
#include "ATU5010_TIMERC_SUB_cmdif.h"
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
    ATU5010_TIMERC *mTIMERC;
    ATU5010_TIMERC_SUB_AgentController(void) {;}
    bool mIs_Clear;
    bool mIs_Upper_Limit;
    bool mIsOneShotRunning[4];
    bool mIsOneshot[4]; // user for Oneshot mode
    bool mIsForcedOCRC[4];
    bool mIsForcedGRC[4];
    bool mIsTSTRCTrue;

    unsigned int mChannelMode[4];
    unsigned int mChannelValue[4];
    unsigned int mOCRCValue[4];
    unsigned int mGRCValue[4];
    double mRangeR1C[4];
    double mRangeR2C[4];
    unsigned int mUpdatedTCNTC;
    virtual ~ATU5010_TIMERC_SUB_AgentController(void) {;}
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
};
#endif
