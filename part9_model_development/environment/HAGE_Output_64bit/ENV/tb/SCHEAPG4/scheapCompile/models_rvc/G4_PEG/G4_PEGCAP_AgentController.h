// ---------------------------------------------------------------------
// $Id: G4_PEGCAP_AgentController.h,v 1.8 2020/01/14 02:39:30 nhutminhdang Exp $
//
// Copyright(c) 2016-2020 Renesas Electronics Corporation
// Copyright(c) 2016-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __G4_PEGCAP_AGENT_H__
#define __G4_PEGCAP_AGENT_H__

#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include <sstream>
class G4_PEGCAP;

class G4_PEGCAP_AgentController {
#include "G4_PEGCAP_cmdif.h"

public:
    /// Constructor & Deconstructor
    G4_PEGCAP_AgentController(void) {;}
    virtual ~G4_PEGCAP_AgentController(void) {;}
    
    /// Virtual functions
    virtual bool CheckClockPeriod() = 0;
    virtual bool GetResetStatus() = 0;
    virtual void NotifyClearPEGSPIDERRCLREvent() = 0;
};
#endif//__G4_PEGCAP_AGENT_H__
