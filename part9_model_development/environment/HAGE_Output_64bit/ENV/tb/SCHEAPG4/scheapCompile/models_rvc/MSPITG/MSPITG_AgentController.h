// ---------------------------------------------------------------------
// $Id: MSPITG_AgentController.h,v 1.2 2020/02/05 05:30:17 ducla Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of MSPITG_AgentController.h
// Ref: {MSPITG_DD_Copyright_001}
#ifndef __MSPITG_AGENT_CONTROLLER_H__
#define __MSPITG_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class MSPITG_AgentController
class MSPITG_AgentController
{
#include "MSPITG_cmdif.h"
public:
    // Constructor/Destructor
    MSPITG_AgentController(void) {;}
    virtual ~MSPITG_AgentController(void) {;}

    // Interface (APIs)
    //user function here
    virtual bool CheckClockPeriod(void) = 0;
    virtual bool GetResetStatus() = 0;
    virtual void UpdateMSPITGCTLValue(unsigned int ch_id, unsigned int value)=0;
    virtual void UpdateMSPITGDMAALTValue(unsigned int value)=0;
    virtual void UpdateMSPITGDTSALTValue(unsigned int value)=0;
};
#endif //__MSPITG_AGENT_CONTROLLER_H__
