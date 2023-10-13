// ---------------------------------------------------------------------
// $Id: HSIF020_AgentController.h,v 1.7 2020/04/07 03:56:04 quynhnhunguyen Exp $
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

#ifndef __HSIF020_AGENTCONTROLLER_H__
#define __HSIF020_AGENTCONTROLLER_H__
#include "HSIF_Common.h"

class HSIF020_UNIT;

class HSIF020_AgentController
{
#include "HSIF020_cmdif.h"

public:
    HSIF020_AgentController(void) {;}
    virtual ~HSIF020_AgentController(void) {;}
    virtual bool    CheckResetStatus () = 0;
    virtual bool    CheckClockPeriod (const std::string clock_name) = 0;
    virtual bool    isInitialSetting(void) = 0;
    virtual void    NotifyAxiTransferTransaction(void) = 0;

    virtual uint8_t GetThisIpSpidInfo (void) = 0;
    virtual void    SetAuthenIdStatus(bool isPass) = 0;
    virtual void    SetAuthenCRStatus(bool isPass) = 0;
    virtual void    SetAuthenIdLock(void) = 0;
    virtual void    SetAuthenCRLock(void) = 0;

    virtual void    TxContinueGetData2Fifo(void) = 0;
    virtual void    RxContinueWriteData2Mem(void) = 0;
    virtual void    TxStreamNotifyGetData2Fifo(void) = 0;
    virtual void    RxStreamNotifyWriteData2Mem(void) = 0;
    virtual uint8_t GetTxStreamOutstandingNumb(void) = 0;
    virtual bool    IsAxiError(void) = 0;
    virtual void    NotifyTxTransStreamCmd(void) = 0;
    virtual bool    IsPassedAuthent(void) = 0;
    virtual bool    IsHitMemWindow(uint32_t startAddr, uint32_t endAddr, bool isWrite) = 0;
    virtual void    IssueInterrupt(std::string signal_name, ChId_t ch_id, bool type) = 0;
    virtual void    TxStreamStopTimeOutEvent(void) = 0;
    virtual void    SetInitialSettingPhase(bool isInitialSettingPhase) = 0;

    HSIF020_UNIT    *mHSIF020_Unit[emCHNum];               // AD unit instance
};
#endif //__HSIF020_AGENTCONTROLLER_H__
