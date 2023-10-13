// ---------------------------------------------------------------------
// $Id: DSADC_UNIT_AgentController.h 1398 2019-11-26 06:16:27Z chuonghoangle $
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
#ifndef __DSADC_UNIT_AGENTCONTROLLER_H__
#define __DSADC_UNIT_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include <queue>
#include <sstream> 

class DSADC_Base;   

class DSADC_UNIT_AgentController
{
#include "DSADC_UNIT_cmdif.h"

public:
    enum eEvent {                                       /// Define DSADC_UNIT events
        emStartTrigger,
        emEndTrigger,
        emCalibStart,
        emCalibStop,
        emAssertDSADPE,
        emAssertDSADVCI,
        emAssertDSADUE,
        emAssertDSADLE
    };
    
    DSADC_UNIT_AgentController(void) {;}
    virtual ~DSADC_UNIT_AgentController(void) {;}  
    DSADC_Base *mDSADC;                                 /// DSADC_Base class pointer   
    unsigned int mAdId;
    unsigned int mChannelNum;
    virtual double GetAnalogValue (const bool is_refh, const unsigned int an_no) = 0;
    virtual void NotifyEvent (const unsigned int event, const unsigned int vc_no) = 0;
    virtual double GetCalibTimeStop(void) = 0;
    virtual void ClearCalibTimeStop(void) = 0;
};
#endif ///__DSADC_UNIT_AGENTCONTROLLER_H__
