// ---------------------------------------------------------------------
// $Id: ATU5_GLUE_AgentController.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_GLUE_AGENTCONTROLLER_H__
#define __ATU5_GLUE_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ATU5_GLUE_AgentController
{
#include "ATU5_GLUE_cmdif.h"
public:
    enum ePortNum {    // Define channel number
        emTQNum = 3
      , emSLICNum = 44
      , emSLIDNum = 36
      , emSLDCDNum = 64
      , emDFEFactorNum = 12
      , emDSTSNum = 10
      , emP5Num = 14
    };
    enum eINTSELtype {
        emINTSELA
      , emINTSELC0
      , emINTSELC1
      , emINTSELD0
      , emINTSELD1
      , emINTSELD2
      , emINTSELD3
      , emINTSELD4
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
    
    ATU5_GLUE_AgentController(void) {;}
    virtual ~ATU5_GLUE_AgentController(void) {;}
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    virtual void NotifyEvent (const unsigned int event, const unsigned int channel) = 0;

};
#endif
