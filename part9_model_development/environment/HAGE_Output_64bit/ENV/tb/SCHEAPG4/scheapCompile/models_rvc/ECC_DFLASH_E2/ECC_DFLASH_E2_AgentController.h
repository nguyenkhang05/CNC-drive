// ---------------------------------------------------------------------
// $Id: ECC_DFLASH_E2_AgentController.h,v 1.2 2019/03/05 04:17:09 synguyen Exp $
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __ECC_DFLASH_E2_AGENT_CONTROLLER_H__
#define __ECC_DFLASH_E2_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class ECC_DFLASH_E2_AgentController
class ECC_DFLASH_E2_AgentController
{
#include "ECC_DFLASH_E2_cmdif.h"
public:
   // Enums

   // Constructor/Destructor
   ECC_DFLASH_E2_AgentController(void) {;}
   virtual ~ECC_DFLASH_E2_AgentController(void) {;}

   // Interface (APIs)
    virtual void DriveInterrupt(const unsigned int port_id) = 0;
    virtual sc_dt::uint64 GetClockPeriod() = 0;
    virtual bool GetResetStatus() = 0;
    virtual void NotifyEccStatus(const unsigned int port_id, const bool output_value) = 0;
};
#endif //__ECC_DFLASH_E2_AGENT_CONTROLLER_H__
