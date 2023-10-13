// ---------------------------------------------------------------------
// $Id: CADC_AgentController.h,v 1.1 2018/11/19 02:50:58 chanle Exp $
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
#ifndef __CADC_AGENTCONTROLLER_H__
#define __CADC_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class CADC_UNIT;

class CADC_AgentController
{
#include "CADC_cmdif.h"

public:
    enum eModelNum {    // Define model's constant number
        emADNum = 1,
        emVCNum = 8
    };
    
    enum eAdId {    // Define model's constant number
        emAD00,
    };
    
    CADC_AgentController(void) {;}
    virtual ~CADC_AgentController(void) {;}
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;

    CADC_UNIT *mAD_Unit[emADNum];               // AD unit instance
};
#endif
