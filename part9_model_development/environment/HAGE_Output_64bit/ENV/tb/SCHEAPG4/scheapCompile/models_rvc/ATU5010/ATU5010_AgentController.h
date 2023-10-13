// ---------------------------------------------------------------------
// $Id: ATU5010_AgentController.h,v 1.1 2017/09/15 10:15:45 binhnguyen Exp $
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
#ifndef __ATU5010_AGENTCONTROLLER_H__
#define __ATU5010_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ATU5010_TIMERA;
class ATU5010_TIMERB;
class ATU5010_TIMERC;
class ATU5010_TIMERD;
class ATU5010_TIMERE;
class ATU5010_TIMERF;
class ATU5010_TIMERG;

class ATU5010_AgentController
{
#include "ATU5010_cmdif.h"
public:
    enum eTimerChNum {    // Define total channel number
        emTimerA_SUBnum = 8
      , emTimerC_SUBnum = 14
      , emTimerC_SUB_CHnum = 4
      , emTimerD_SUBnum = 15
      , emTimerD_SUB_CHnum = 4
      , emTimerE_SUBnum = 10
      , emTimerE_SUB_CHnum = 4
      , emTimerF_SUBnum = 20
      , emTimerNum = 7
      , emCLK_BUSnum = 6
      , emClkBusPSCNum = 4
      , emNoiseCancelPCSNum = 1
      , emTimerG_SUBnum = 10
    };
    enum eTIMER_ID {    // Define timer ID
        emTimerA
      , emTimerB
      , emTimerC
      , emTimerD
      , emTimerE
      , emTimerF
      , emTimerG
    };
    enum eTimerPortNum {
        emTimerA_TILOnum = 8
      , emTIAnum = 8
      , emTimerEF_POEnum = 6
      , emWrapper_DMAnum = 4
    };
    enum eClkEdgeType {
        emNoneEdge
      , emRisingEdge
      , emFallingEdge
      , emBothEdge
    };
    enum eTCLKtype {
        emTCLKA
      , emTCLKB
      , emTCLKnum
    };
    enum eATUENR {
        emPSCE = 0
    };
    enum eINTSELtype {
        emTRGSELDMA00
      , emTRGSELDMA01
      , emTRGSELDMA10
      , emTRGSELDMA11
      , emTRGSELAD
    };
    
    ATU5010_AgentController(void) {;}
    virtual ~ATU5010_AgentController(void) {;}
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    virtual void EnablePrescaler (const bool is_enable) = 0;
    virtual void PrescalerSetupChange (const unsigned int ch_id) = 0;
    virtual void ClkBusSetupChange (void) = 0;
    virtual void NCClkSetupChange (void) = 0;
    virtual void CheckClkBusSel (const unsigned int timer_id) = 0;
    virtual void NotifyEvent (const unsigned int event, const unsigned int channel) = 0;

    ATU5010_TIMERA *mATU5010_TimerA;               // Timer A instance
    ATU5010_TIMERB *mATU5010_TimerB;               // Timer B instance
    ATU5010_TIMERC *mATU5010_TimerC;               // Timer C instance
    ATU5010_TIMERD *mATU5010_TimerD;               // Timer D instance
    ATU5010_TIMERE *mATU5010_TimerE;               // Timer E instance
    ATU5010_TIMERF *mATU5010_TimerF;               // Timer F instance
    ATU5010_TIMERG *mATU5010_TimerG;               // Timer G instance
};
#endif
