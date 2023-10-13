// ---------------------------------------------------------------------
// $Id: DNFP020_E2_Sub_AgentController.h 2520 2017-03-27 03:23:26Z ChinhTD $
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
#ifndef __DNFP020_E2_SUB_AGENTCONTROLLER_H__
#define __DNFP020_E2_SUB_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class DNFP020_E2_AgentController;
class DNFP020_E2_Sub_AgentController
{
#include "DNFP020_E2_Sub_cmdif.h"

public:
    DNFP020_E2_Sub_AgentController(void) {;}
    virtual ~DNFP020_E2_Sub_AgentController(void) {;}
    DNFP020_E2_AgentController *mDNFP020_E2_AgentController;                   // DNFP020_E2 class pointer                  // DNFP020_E2_Sub class pointer
    unsigned int mChId;
    enum eEventType {
        emNFENchanged,
        emEDFcleared
    };
    virtual void SetTimePRSChangePoint (void) = 0;
    virtual void NotifyEvent (const unsigned int event) = 0;
};
#endif
