// ---------------------------------------------------------------------
// $Id: PSI5_Timer.h,v 1.7 2020/04/23 07:12:11 hadinhtran Exp $
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
#ifndef __PSI5_Timer_H__
#define __PSI5_Timer_H__
#include "Cgeneral_timer.h"

class PSI5_Timer:public Cgeneral_timer
{
public:
    sc_event   mCompareMatchEvent;
    sc_event   mOverflowEvent;

    PSI5_Timer (std::string name);
    ~PSI5_Timer (void);
    void EnableReset (const bool is_active);
    void StartStop (const bool is_start);
    void SetCompareMatchValue (const unsigned int value);
    void SetCounterIntialValue (const unsigned int value);
    void SetCounterPeriod (const double counter_period);
    void SetCounterMode (const bool is_freerun);
    void SetupGeneralTimer ();
    unsigned int GetCounterValue ();
private:
    sc_dt::uint64 mTimeResolutionValue;   //Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;     //Time resolution unit of simulation
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    //Declare virtual function of Cgeneral_timer class
    void cmpMatchOutputHandling (bool value);
    void interruptHandling();
};
#endif
