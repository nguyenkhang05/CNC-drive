// ---------------------------------------------------------------------
// $Id: PSIS011_AgentController.h,v 1.4 2017/12/07 02:00:02 chanle Exp $
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
#ifndef __PSIS011_AGENT_CONTROLLER_H__
#define __PSIS011_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class PSIS011_AgentController
class PSIS011_AgentController
{
#include "PSIS011_cmdif.h"
public:
    // Constructor/Destructor
    PSIS011_AgentController(void) {;}
    virtual ~PSIS011_AgentController(void) {;}

    // Interface (APIs)
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual void SoftwareReset() = 0;
    virtual sc_dt::uint64 GetPCLKFreq() = 0;
    virtual void ClearTimestamp(std::string timestamp_name) = 0;
    virtual void ConfigTimestamp(std::string timestamp_name, bool clear_select_gtm, bool enable_select_gtm, bool clock_select_gtm) = 0;
    virtual void SetTSInnerEnable(std::string timestamp_name, bool ts_inner_enable) = 0;
    virtual void SetTSInnerCycle(unsigned int inner_cycle) = 0;
    virtual void InformChangeMode(unsigned int open, unsigned int opmd) = 0;
    virtual void NotifySendReqTxToArbiter(unsigned int reqid) = 0;
    virtual void NotifyTransUART() = 0;
    virtual void AssertInterrupt(unsigned int intrid) = 0;
    virtual void AssertDMARequestRX(unsigned int dmarxid) = 0;
    virtual void AssertDMARequestTX(unsigned int dmatxid) = 0;
    virtual void ChangeWDTEnableChannel(unsigned int chid, unsigned int enable) = 0;
    virtual void InformChangeSCKENBit(unsigned int value) = 0;
    virtual void StopTransDDSR(unsigned int txreqid) = 0;
};
#endif //__PSIS011_AGENT_CONTROLLER_H__
