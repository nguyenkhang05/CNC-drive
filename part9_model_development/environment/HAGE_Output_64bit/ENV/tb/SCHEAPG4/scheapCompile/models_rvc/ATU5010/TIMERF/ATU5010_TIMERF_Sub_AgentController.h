// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERF_Sub_AgentController.h,v 1.0 2016/03/16 09:36:51 binhnguyen Exp $
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
#ifndef __ATU5010_TIMERF_SUB_AGENTCONTROLLER_H__
#define __ATU5010_TIMERF_SUB_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ATU5010_TIMERF;
class ATU5010_Timer;
class ATU5010_TIMERF_Sub_AgentController
{
#include "ATU5010_TIMERF_Sub_cmdif.h"

public:
	unsigned int   mGRCF;                   //
    unsigned int   mGRAF;                   //
    unsigned int   mGRDF;                   //
    unsigned int   mECNTBF;
    ATU5010_Timer           *mCounterA;                   // Timer for ECNTAF
    ATU5010_Timer           *mCounterC;                   // Timer for ECNTCF
    bool  mCntCLKUpdated;
    bool  mECNTAFUpdated;
    bool  mECNTCFUpdated;
    bool  mGRAFUpdated;
    bool  mGRBFUpdated;
    bool  mGRDFUpdated;
    bool    mIsTimerARunning;
    bool    mIsTimerCRunning;
    bool mIsEdgeCntInGivenTimeRunning;      //EdgeCntInGivenTime processing
    bool mIsEdgeIntCntRunning;              //EdgeIntCnt processing
    bool mIsTimeDurInputLevelRunning;       //TimeDurInputLevel processing
    bool mIsPwmWaveTimeRunning;             //PwmWaveTime processing
    bool mIsRotSpeedPulseRunning;           //RotSpeedPulse processing
    bool mIsUpDownCntRunning;               //UpDownCnt processing
    bool mIsUpDownCntx4Running;             //UpDownCntx4 processing
    sc_event mWriteMDFEvent;                //Write to TCR1F.MDF event
    enum eEvent {    // Define ATU5010_TIMERF_Sub events
        emECNTAFUpdated,
        emECNTCFUpdated,
        emGRAFUpdated,
        emGRBFUpdated,
        emGRDFUpdated,
        emTCR1FUpdated

    };
    virtual unsigned int GetNoiseCounterValueA (void) =0;
    virtual bool IsNoiseCancelDurationA(void) =0;
    virtual unsigned int GetNoiseCounterValueB (void) =0;
    virtual bool IsNoiseCancelDurationB(void) =0;
    ATU5010_TIMERF_Sub_AgentController(void) {;}
    virtual ~ATU5010_TIMERF_Sub_AgentController(void) {;}
    virtual void NotifyEvent (const unsigned int event) = 0;
    virtual void SetNoiseCORValB (void)= 0;
    virtual void SetNoiseCounterValueB (void)= 0;
    virtual void SetNoiseCORValA (void)= 0;
    virtual void SetNoiseCounterValueA (void)= 0;

    ATU5010_TIMERF *mATU5010_TIMERF;                   // ATU5010_TIMERF class pointer
    unsigned int mSubId;
};
#endif
