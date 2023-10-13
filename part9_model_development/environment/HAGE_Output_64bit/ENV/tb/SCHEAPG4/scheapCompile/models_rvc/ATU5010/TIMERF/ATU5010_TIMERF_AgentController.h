// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERF_AgentController.h,v 1.0 2017/05/29 09:36:50 FPT/TruongVV2 Exp $
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
#ifndef __ATU5010_TIMERF_AGENTCONTROLLER_H__
#define __ATU5010_TIMERF_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ATU5010_TIMERF_Sub;
class ATU5010;

#include "ATU5010_TIMERF_Sub.h"
#include "ATU5010.h"


class ATU5010_TIMERF_AgentController
{
#include "ATU5010_TIMERF_cmdif.h"

public:
    enum eModelNum {    // Define model's constant number
        emSubNum = 20,
        emClkNum = 9
    };

    enum eSubId {    // Define model's constant number
        emSub0 ,
        emSub1 ,
        emSub2 ,
        emSub3 ,
        emSub4 ,
        emSub5 ,
        emSub6 ,
        emSub7 ,
        emSub8 ,
        emSub9 ,
        emSub10,
        emSub11,
        emSub12,
        emSub13,
        emSub14,
        emSub15,
    		emSub16,
    		emSub17,
    		emSub18,
    		emSub19
    };
    enum eClkId {   // Define clock ID
        CLK_LSB,
        CLK_HSB,
        CLKBUS_0,
        CLKBUS_1,
        CLKBUS_2,
        CLKBUS_3,
        CLKBUS_4,
        CLKBUS_5,
        CLK_NOISE,
    };

    enum eFoffsetAddr {     //define offset address
        eComStart   = 0x5000,
        eComEnd     = 0x503F,
        eSub0Start  = 0x5040,
        eSub0End    = 0x507F,
        eSub1Start  = 0x5080,
        eSub1End    = 0x50BF,
        eSub2Start  = 0x50C0,
        eSub2End    = 0x50FF,
        eSub3Start  = 0x5100,
        eSub3End    = 0x513F,
        eSub4Start  = 0x5140,
        eSub4End    = 0x517F,
        eSub5Start  = 0x5180,
        eSub5End    = 0x51BF,
        eSub6Start  = 0x51C0,
        eSub6End    = 0x51FF,
        eSub7Start  = 0x5200,
        eSub7End    = 0x523F,
        eSub8Start  = 0x5240,
        eSub8End    = 0x527F,
        eSub9Start  = 0x5280,
        eSub9End    = 0x52BF,
        eSub10Start = 0x52C0,
        eSub10End   = 0x52FF,
        eSub11Start = 0x5300,
        eSub11End   = 0x533F,
        eSub12Start = 0x5340,
        eSub12End   = 0x537F,
        eSub13Start = 0x5380,
        eSub13End   = 0x53BF,
        eSub14Start = 0x53C0,
        eSub14End   = 0x53FF,
        eSub15Start = 0x5400,
        eSub15End   = 0x543F,
    		eSub16Start = 0x5440,
    		eSub16End	  = 0x547F,
    		eSub17Start = 0x5480,
    		eSub17End   = 0x54BF,
    		eSub18Start = 0x54C0,
    		eSub18End	  = 0x54FF,
    		eSub19Start = 0x5500,
    		eSub19End	  = 0x553F
    };
    enum eEvent {    // Define DSADC_UNIT events
        emStartSubblock0,
        emStartSubblock1,
        emStartSubblock2,
        emStartSubblock3,
        emStartSubblock4,
        emStartSubblock5,
        emStartSubblock6,
        emStartSubblock7,
        emStartSubblock8,
        emStartSubblock9,
        emStartSubblock10,
        emStartSubblock11,
        emStartSubblock12,
        emStartSubblock13,
        emStartSubblock14,
        emStartSubblock15,
    		emStartSubblock16,
    		emStartSubblock17,
    		emStartSubblock18,
    		emStartSubblock19
    };
    ATU5010_TIMERF_AgentController(void) {;}
    virtual ~ATU5010_TIMERF_AgentController(void) {;}
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    virtual void NotifyEvent (const unsigned int event, bool enable) = 0;
    ATU5010            *mATU5010;
    ATU5010_TIMERF_Sub *mTimerF_Sub[emSubNum];               // TimerF subblock instance
};
#endif
