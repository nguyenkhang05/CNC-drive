// ---------------------------------------------------------------------
// $Id: DNF_AgentController.h,v 1.3 2020/02/05 04:31:04 ngathunguyen Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DNF_AgentController__
#define __DNF_AgentController__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class DNF_AgentController
{
#include "DNF_cmdif.h"
public:
    DNF_AgentController()
    {
        ;
    }
    virtual ~DNF_AgentController()
    {
        ;
    }
    virtual bool CheckClock(const std::string clock_name) = 0;
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual void SwitchToEnableMode(const unsigned int channel_id) = 0;
    virtual void SwitchToDisableMode(const unsigned int channel_id) = 0;
    virtual void OutputDNFATNFOUT(const unsigned int channel_id, const bool value) = 0;
    virtual void GetNextSample(const unsigned int channel_id) = 0;
};
#endif //__DNF_AgentController__
