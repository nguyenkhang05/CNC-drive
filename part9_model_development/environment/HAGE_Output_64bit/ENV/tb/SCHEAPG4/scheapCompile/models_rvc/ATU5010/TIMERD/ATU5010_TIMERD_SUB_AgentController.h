// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERD_SUB_AgentController.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#ifndef __ATU5010_TIMERD_SUB_AGENT__
#define __ATU5010_TIMERD_SUB_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>
class ATU5010_TIMERD;
class ATU5010_Timer;
class ATU5010_TIMERD_SUB_AgentController
{
#include "ATU5010_TIMERD_SUB_cmdif.h"
public:
    enum eChannel {
        emChannel0 = 0,
        emChannel1,
        emChannel2,
        emChannel3,
        emChannel
        };
    enum eCounter {
        emCounter1 = 0,
        emCounter2,
        emDownCounter,
        emAllCounter,
        emOFMICNTDCounter,
        emONMICNTDCounter,
        emOTOMICNTDCounter
        };
    ATU5010_TIMERD *mTIMERD;
    bool        mCLRCounter1;
    bool        mCLRCounter2;
    bool        mOverflowTCNT1D;
    bool        mOverflowTCNT2D;
    bool        mIsTimer1Running;
    bool        mIsTimer2Running;
    bool        mIsDcntRunning[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mIsDcntinUdfPeriod[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mIsDcntWasRunning[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mIsTimerOFMICNTDRunning[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mIsTimerONMICNTDRunning[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mIsTimerOTONMICNTDRunning[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mCLR1DVal;
    bool        mCLR2DVal;
    bool        mOBREDVal;
    bool        mC1CEDVal;
    bool        mC2CEDVal;
    bool        mCLRSEL1DVal;
    bool        mCLRSEL2DVal;
    bool        mCMPSEL1DVal[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mCMPSEL2DVal[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mInterruptCMPAEnable[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mInterruptCMPBEnable[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mOSELADValue[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mODADValue[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mChannelAInvert[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mCompareMatchCNT1Enable[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mCompareMatchCNT2Enable[ATU5010_TIMERD_SUB_AgentController::emChannel];
    unsigned int mTIDSELDVal;
    unsigned int mCUCR1DValue;
    unsigned int mCUCR2DValue;
    unsigned int mCKSEL1DVal;
    unsigned int mCKSEL2DVal;
    unsigned int mDCSELDVal;
    unsigned int mRC1DVal;
    unsigned int mRC2DVal;
    unsigned int mOCR1DValue[ATU5010_TIMERD_SUB_AgentController::emChannel];
    unsigned int mOCR2DValue[ATU5010_TIMERD_SUB_AgentController::emChannel];
    unsigned int mOSSDValue[ATU5010_TIMERD_SUB_AgentController::emChannel];
    unsigned int mIOADValue[ATU5010_TIMERD_SUB_AgentController::emChannel];
    unsigned int mIOBDValue[ATU5010_TIMERD_SUB_AgentController::emChannel];

    unsigned int subnum;
    bool        mChangeCountClock[ATU5010_TIMERD_SUB_AgentController::emAllCounter];
    bool        mInterruptOVF1Enable;
    bool        mInterruptOVF2Enable;
    bool        mGlobalInvertASelect;
    ATU5010_TIMERD_SUB_AgentController(void) {;}
    virtual ~ATU5010_TIMERD_SUB_AgentController(void) {;}
    virtual void ClockHandleFunction (void) = 0;
    virtual void CheckRangeCompareFunction1(unsigned int counter, unsigned int channel) = 0;
    virtual void CheckRangeCompareFunction2(unsigned int counter, unsigned int channel) = 0;
    virtual unsigned int GetCounterValue(unsigned int counter, unsigned int channel) = 0;
    virtual void   StartDCNTDReg (const unsigned int channel) = 0;
    virtual void ChangeOutputPortTODA (const unsigned int channel, unsigned int port_val)=0;
    virtual void WriteTODBMethod (const unsigned int ch_id) = 0;
    virtual void ProcessMIGMethod (const unsigned int ch_id) = 0;
    virtual void WriteTODMIGMethod (const unsigned int ch_id) = 0;
    sc_event    mWriteCompareMatchValue[ATU5010_TIMERD_SUB_AgentController::emDownCounter];
    sc_event    mWriteCounterValue[ATU5010_TIMERD_SUB_AgentController::emDownCounter];
    sc_event    mUpdateDCNTDEvent[ATU5010_TIMERD_SUB_AgentController::emChannel];
    sc_event    mWriteOFMICNTDEvent[ATU5010_TIMERD_SUB_AgentController::emChannel];
    sc_event    mWriteONMICNTDEvent[ATU5010_TIMERD_SUB_AgentController::emChannel];
    sc_event    mWriteOTOMICNTDEvent[ATU5010_TIMERD_SUB_AgentController::emChannel];
    
    bool        mIsDCNTUpdated[ATU5010_TIMERD_SUB_AgentController::emChannel];
    
    bool        mInterruptUNDEDEnable[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mInterruptFEIEDEnable[ATU5010_TIMERD_SUB_AgentController::emChannel];
    bool        mInterruptREIEDEnable[ATU5010_TIMERD_SUB_AgentController::emChannel];
};
#endif
