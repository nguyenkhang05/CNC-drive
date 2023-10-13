// ---------------------------------------------------------------------
// $Id: PSI5_AgentController.h,v 1.7 2020/04/23 07:12:11 hadinhtran Exp $
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
#ifndef __PSI5_AGENT__
#define __PSI5_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class PSI5_AgentController
{
#include "PSI5_cmdif.h"
public:
    PSI5_AgentController(void) {;}
    virtual ~PSI5_AgentController(void) {;}
    virtual bool GetResetStatus (void) = 0;
    virtual bool GetPresetnStatus (void) =0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    virtual sc_dt::uint64 GetCommunicationPeriod (void) = 0;
    virtual void SendSyncPulse (const bool is_sync_mode, const unsigned int sync_pulse_default_value=0) = 0;
    virtual void StartStopTimer (const unsigned char timer_type, const bool is_start) = 0;
    virtual void SetTimerCompareMatchValue (const unsigned char timer_type, const unsigned int value) = 0;
    virtual void SetTimerCounterIntialValue (const unsigned char timer_type, const unsigned int value) = 0;
    virtual void SetTimerCounterPeriod (const unsigned char timer_type, const double counter_period) = 0;
    virtual void SetTimerCounterMode (const unsigned char timer_type, const bool is_freerun) = 0;
    virtual unsigned int GetStampTimeCounterValue () = 0;  
    virtual void ControlInterruptPorts (const unsigned int interrupt_id, const bool is_active) = 0;
    virtual void SendData (const unsigned int data_format, const unsigned int data) = 0;
    virtual void Writepsi5_ts_tick_outPort (const unsigned int bitrate_value, const bool is_clear) = 0;
    virtual void Sendpsi5_ts_clr_outPort (void) = 0;
    virtual void EnableSoftwareReset (const bool is_active) = 0;
    virtual void SetCounter_period_in_master_mode(double period)= 0;
};
#endif
