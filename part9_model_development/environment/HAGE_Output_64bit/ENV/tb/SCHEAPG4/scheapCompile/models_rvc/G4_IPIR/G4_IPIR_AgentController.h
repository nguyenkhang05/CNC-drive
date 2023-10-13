// ---------------------------------------------------------------------
// $Id: G4_IPIR_AgentController.h,v 1.6 2019/07/10 11:25:13 nhuvo Exp $
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
#ifndef __G4_IPIR_AGENT_CONTROLLER_H__
#define __G4_IPIR_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "global.h"
#include "G4_intc_def.h"
#include <sstream>
// Class G4_IPIR_AgentController
class G4_IPIR_AgentController
{
#include "G4_IPIR_cmdif.h"
public:
    // Enums
    
    // Constructor/Destructor
    virtual ~G4_IPIR_AgentController(void) {;}
    
    // Interface (APIs)
    virtual void DriveInterrupt(bool int_array[PE_MAX_NUM][INTC_IPIR_CH_NUM]) = 0;
    virtual bool GetExtErr() = 0;
    virtual bool GetPeNum(unsigned int bmid, unsigned int& pe_num) = 0;
    virtual sc_dt::uint64 GetClockPeriod() = 0;
    virtual bool GetResetStatus() = 0;
};
#endif //__G4_IPIR_AGENT_CONTROLLER_H__
