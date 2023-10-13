// ---------------------------------------------------------------------
// $Id: HSIF020_UNIT_AgentController.h,v 1.5 2020/03/16 02:12:55 quynhnhunguyen Exp $
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __HSIF020_UNIT_AGENTCONTROLLER_H__
#define __HSIF020_UNIT_AGENTCONTROLLER_H__

#include "HSIF_Common.h"

class HSIF020;
class HSIF020_UNIT_AgentController
{
#include "HSIF020_UNIT_cmdif.h"

public:

    HSIF020_UNIT_AgentController(void) {;}
    virtual ~HSIF020_UNIT_AgentController(void) {;}
    
    HSIF020     *mHSIF020;
    ChId_t      mCh_id;
    
    virtual void        NotifyStartNonStreamEvent(const unsigned int cty_config) = 0;
    virtual uint8_t     GetCurrentFramInfo(void) = 0;
    virtual uint32_t    GetTimeOutSetting(void) = 0;
    virtual bool        IsChannelBusy(void) = 0;
    virtual bool        CheckClockPeriod(const std::string clock_name) = 0;
    virtual void        SetInterruptStatus(uint32_t bit_name) = 0;
    virtual void        SetInitialSettingPhase(bool isInitialSettingPhase) = 0;
};
#endif  //__HSIF020_UNIT_AGENTCONTROLLER_H__
