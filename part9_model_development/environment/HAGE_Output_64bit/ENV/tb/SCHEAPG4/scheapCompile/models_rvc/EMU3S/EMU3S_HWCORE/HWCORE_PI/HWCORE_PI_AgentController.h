// ---------------------------------------------------------------------
// $Id: HWCORE_PI_AgentController.h,v 1.3 2020/11/08 05:38:30 minhquocha Exp $
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
// Description: Copyright of HWCORE_PI_AgentController.h
// Ref: {HWCORE_PI_UD_Copyright_001}
#ifndef __HWCORE_PI_AGENT_CONTROLLER_H__
#define __HWCORE_PI_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Description: Declaration of HWCORE_PI_AgentController class
// Ref: {HWCORE_PI_UD_ATT1_004}
class HWCORE_PI_AgentController
{
#include "HWCORE_PI_cmdif.h"
public:
    // Constructor/Destructor
    HWCORE_PI_AgentController(void) { ; }
    virtual ~HWCORE_PI_AgentController(void) { ; }

    virtual void Callback_IDIN(void) = 0;
    virtual void Callback_IQIN(void) = 0;
    virtual void Callback_ID(void) = 0;
    virtual void Callback_IQ(void) = 0;
    virtual void Callback_VD(void) = 0;
    virtual void Callback_VQ(void) = 0;
};
#endif //__HWCORE_PI_AGENT_CONTROLLER_H__
