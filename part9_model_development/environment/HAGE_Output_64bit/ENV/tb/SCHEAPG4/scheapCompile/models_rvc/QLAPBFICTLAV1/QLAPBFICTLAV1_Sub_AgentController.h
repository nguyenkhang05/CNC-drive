// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1_Sub_AgentController.h,v 1.3 2020/03/24 07:16:32 duongphan Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __QLAPBFICTLAV1_SUB_AGENTCONTROLLER_H__
#define __QLAPBFICTLAV1_SUB_AGENTCONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "QLAPBFICTLAV1.h"

class QLAPBFICTLAV1;
class QLAPBFICTLAV1_Sub_AgentController
{
#include "QLAPBFICTLAV1_Sub_cmdif.h"

public:
    enum eSyncSmpclkNumber{
        emSyncSmpclkNum = 3 // Number effe
    };

    QLAPBFICTLAV1_Sub_AgentController(void) {;}
    virtual ~QLAPBFICTLAV1_Sub_AgentController(void) {;}

    QLAPBFICTLAV1   *mQLAPBFICTLAV1;
    unsigned int    mChId;

    virtual void NotifyEffectiveBitSetting (const std::string effective_name) = 0;
    virtual void NotifyEdgeDetectorSignal (const std::string edge_name)=0;
    virtual void NotifyFilterSignal(const std::string signal_name)= 0;
    virtual void NotifyEdgeSignal(void)= 0;

};
#endif //__QLAPBFICTLAV1_SUB_AGENTCONTROLLER_H__
// vim600: set foldmethod=marker :
