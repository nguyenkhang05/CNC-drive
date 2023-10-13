// ---------------------------------------------------------------------
// $Id: ATU5010_GLUE_AgentController.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#ifndef __ATU5010_GLUE_AGENTCONTROLLER_H__
#define __ATU5010_GLUE_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ATU5010_GLUE_AgentController
{
#include "ATU5010_GLUE_cmdif.h"
public:
    enum ePortNum {    // Define channel number
        emTQNum = 3
      , emSLIDNum = 60
      , emSLDCDNum = 64
      , emDFEFactorNum = 12
      , emDSTSNum = 10
      , emP5Num = 14
    };
    enum eINTSELtype {
        emINTSELA
      //, emINTSELC0
      //, emINTSELC1
      , emINTSELD0
      , emINTSELD1
      , emINTSELD2
      , emINTSELD3
      , emINTSELD4
	  , emINTSELD5
	  , emINTSELD6
	  , emINTSELD7
      , emDMASELB
      , emDMASELCD0
      , emDMASELCD1
      , emDMASELCD2
      , emDMASELCD3
      , emDMASELCD4
      , emDMASELCD5
      , emDMASELCD6
      , emDMASELCD7
      , emDMASELE
      , emDFEENTQ0
      , emDFEENTQ1
      , emDFEENTQ2
      , emDFESEL0
      , emDFESEL1
      , emDFESEL2
      , emDFESELD1T
      , emDSSELDSTS
      , emCASELCATS
      , emP5SSEL
    };
    
    ATU5010_GLUE_AgentController(void) {;}
    virtual ~ATU5010_GLUE_AgentController(void) {;}
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    virtual void NotifyEvent (const unsigned int event, const unsigned int channel) = 0;

};
#endif
