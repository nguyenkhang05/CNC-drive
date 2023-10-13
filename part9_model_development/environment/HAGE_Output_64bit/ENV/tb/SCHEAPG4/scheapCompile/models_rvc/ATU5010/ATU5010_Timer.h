// ---------------------------------------------------------------------
// $Id: ATU5010_Timer.h,v 1.1 2017/06/28 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_Timer_H__
#define __ATU5010_Timer_H__
#include "Cgeneral_timer.h"

class ATU5010;

class ATU5010_Timer:public Cgeneral_timer
{
friend class ATU5010;
public:
    unsigned int                    mCmpMatchVal;
    ATU5010                            *mATU5010;
    sc_event                        mCompareMatchEvent;
    sc_event                        mOverflowEvent;
    sc_event                        mUnderflowEvent;

    ATU5010_Timer (std::string name, ATU5010 *ATU5010_Ptr);
    ~ATU5010_Timer (void);
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
