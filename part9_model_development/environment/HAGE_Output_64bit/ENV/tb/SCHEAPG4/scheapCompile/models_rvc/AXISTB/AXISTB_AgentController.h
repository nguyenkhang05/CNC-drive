// ---------------------------------------------------------------------
// $Id: AXISTB_AgentController.h,v 1.3 2014/05/05 01:23:39 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __AXISTB_AgentController_H__
#define __AXISTB_AgentController_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class AXISTB_AgentController
{
#include "AXISTB_cmdif.h"
public:
    AXISTB_AgentController(void) {;}
    virtual ~AXISTB_AgentController(void) {;}
    virtual bool CheckClockPeriod (const std::string clock_name)=0;
    virtual bool GetHRESETZResetStatus (void)=0;
    virtual bool PushDataToBuffer(const std::string name, const bool is_write, TlmBasicPayload &trans, const BusTime_t trans_time)=0;
    virtual void ContinueTransferring(const std::string name)=0;
    virtual void NotifyTransComp(const std::string name)=0;
    virtual void NotifyTransCancel(const std::string name)=0;
    virtual void TransferLT(const std::string name, const bool is_write, TlmBasicPayload &trans, const BusTime_t trans_time)=0;
};
#endif
