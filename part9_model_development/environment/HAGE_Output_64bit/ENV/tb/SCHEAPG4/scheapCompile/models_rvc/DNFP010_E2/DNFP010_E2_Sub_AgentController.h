// ---------------------------------------------------------------------
// $Id: DNFP010_E2_Sub_AgentController.h 5605 2017-03-27 03:23:26Z ChinhTD $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DNFP010_E2_SUB_AGENTCONTROLLER_H__
#define __DNFP010_E2_SUB_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class DNFP010_E2_AgentController;
class DNFP010_E2_Sub_AgentController
{
#include "DNFP010_E2_Sub_cmdif.h"

public:
    DNFP010_E2_Sub_AgentController(void) {;}
    virtual ~DNFP010_E2_Sub_AgentController(void) {;}
    DNFP010_E2_AgentController *mDNFP010_E2_AgentController;                   // DNFP010_E2 class pointer                  // DNFP010_E2_Sub class pointer
    unsigned int mChId;
    virtual void SetTimePRSChangePoint (void) = 0;
    virtual void NotifyEvent (void) = 0;
};
#endif
