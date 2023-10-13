// ---------------------------------------------------------------------
// $Id: ATU5_Timer.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_Timer_H__
#define __ATU5_Timer_H__
#include "Cgeneral_timer.h"

class ATU5_Base;

class ATU5_Timer:public Cgeneral_timer
{

public:
    unsigned int                    mCmpMatchVal;
    ATU5_Base                       *mATU5;
    sc_event                        mCompareMatchEvent;
    sc_event                        mOverflowEvent;
    sc_event                        mUnderflowEvent;

    ATU5_Timer (std::string name, ATU5_Base *ATU5_Ptr);
    ~ATU5_Timer (void);
    void EnableReset (const bool is_active);
    void StartStop (const bool is_start);
    void SetCompareMatchValue (const unsigned int value);
    unsigned int GetCompareMatchValue (void);
    void SetCounterInitialValue (const unsigned int value);
    void SetCounterPeriod (const double counter_period);
    void SetCounterMode (const bool is_freerun);
    unsigned int GetCounterValue (void);
    void SetupGeneralTimer (const bool is_clearmode, const bool is_cntup);
    void SetTimerSize(const unsigned int value);
    // added by Timer E
    void ClearBitOVF(void);
    void ClearBitCMF(void);
    void ClearBitUNF(void);
    // added by Timer C
    bool GetBitOVF (void);
    bool GetBitCMF (void);
private:
    //Declare virtual function of Cgeneral_timer class
    void cmpMatchOutputHandling (bool value);
    void interruptHandling();
};
#endif
