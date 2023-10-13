// ---------------------------------------------------------------------
// $Id: CADC_UNIT_AgentController.h,v 1.1 2018/11/19 02:50:58 chanle Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __CADC_UNIT_AGENTCONTROLLER_H__
#define __CADC_UNIT_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class CADC;

class CADC_UNIT_AgentController
{
#include "CADC_UNIT_cmdif.h"

public:
    enum eEvent {    // Define CADC_UNIT events
        emStartTrigger,
        emEndTrigger,
        emCalibStart,
        emCalibStop,
        emAssertCADPE,
        emAssertCADCVCI,
        emAssertCADUE,
        emAssertCADLE,
        emUpdateDIR,
        emCheckFinish
    };
    
    CADC_UNIT_AgentController(void) {;}
    virtual ~CADC_UNIT_AgentController(void) {;}
    
    CADC *mCADC;                   // CADC class pointer
    unsigned int mAdId;
    unsigned int mChannelNum;
    
    virtual double GetAnalogValue (const bool is_refh, const unsigned int an_no) = 0;
    virtual void NotifyEvent (const unsigned int event, const unsigned int vc_no) = 0;
};
#endif
