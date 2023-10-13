// ---------------------------------------------------------------------
// $Id: ATU5_Timer.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_Timer.h"
#include "ATU5_Base.h"

/*********************************
// Function     : ATU5_Timer
// Description  : Constructor of ATU5_Timer class
// Parameter    :
//      name            Module name
// Return value : None
**********************************/
ATU5_Timer::ATU5_Timer (std::string name, ATU5_Base *ATU5_Ptr):Cgeneral_timer(name.c_str())
{//{{{
    assert(ATU5_Ptr != NULL);
    mATU5 = ATU5_Ptr;
    mCmpMatchVal = 0;
}//}}}

/*********************************
// Function     : ~ATU5_Timer
// Description  : Destructor of ATU5_Timer class
// Parameter    : None
// Return value : None
**********************************/
ATU5_Timer::~ATU5_Timer (void)
{//{{{
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Reset general timer
// Parameter    :
//      is_active           Active state of reset
// Return value : None
**********************************/
void ATU5_Timer::EnableReset (const bool is_active)
{//{{{
    Cgeneral_timer::EnableReset(is_active);
}//}}}

/*********************************
// Function     : StartStop
// Description  : Start/stop general timer
// Parameter    :
//      is_start            Start trigger
// Return value : None
**********************************/
void ATU5_Timer::StartStop (const bool is_start)
{//{{{
    Cgeneral_timer::setSTR(is_start);
}//}}}

/*********************************
// Function     : SetCompareMatchValue
// Description  : Set compare match value of general timer
// Parameter    :
//      value               Compare match value
// Return value : None
**********************************/
void ATU5_Timer::SetCompareMatchValue (const unsigned int value)
{//{{{
    Cgeneral_timer::setCOR(value);
}//}}}

/*********************************
// Function     : SetCounterInitialValue
// Description  : Set counter initial value of general timer
// Parameter    :
//      value               Initial counter value
// Return value : None
**********************************/
void ATU5_Timer::SetCounterInitialValue (const unsigned int value)
{//{{{
    Cgeneral_timer::setCNT(value);
}//}}}

/*********************************
// Function     : SetCounterPeriod
// Description  : Set counter period of general timer
// Parameter    :
//      counter_period      Counter period (ns)
// Return value : None
**********************************/
void ATU5_Timer::SetCounterPeriod (const double counter_period)
{//{{{
    Cgeneral_timer::setClkCountPeriod(counter_period);
}//}}}

/*********************************
// Function     : SetCounterMode
// Description  : Set counter mode of general timer
// Parameter    :
//      is_freerun          Counter mode (true: free-running; false: one-shot)
// Return value : None
**********************************/
void ATU5_Timer::SetCounterMode (const bool is_freerun)
{//{{{
    if (is_freerun) {
        Cgeneral_timer::setCMM(emFreeRun);//Free-running mode
    } else {
        Cgeneral_timer::setCMM(emOneShot);//One-shot mode
    }
}//}}}

/*********************************
// Function     : GetCounterValue
// Description  : Get counter value of general timer
// Parameter    : None
// Return value : Counter value
**********************************/
unsigned int ATU5_Timer::GetCounterValue (void)
{//{{{
    return Cgeneral_timer::getCNT();
}//}}}

/*********************************
// Function     : GetCompareMatchValue
// Description  : Get counter value of general timer at compare match time
// Parameter    : None
// Return value : Counter value
**********************************/
unsigned int ATU5_Timer::GetCompareMatchValue (void)
{//{{{
    return mCmpMatchVal;
}//}}}

/*********************************
// Function     : SetupGeneralTimer
// Description  : Setup General Timer
// Parameter    :
//      is_clearmode        Enable/disable clear mode
//      is_cntup            Count mode (true: count up; false: count down)
// Return value : None
**********************************/
void ATU5_Timer::SetupGeneralTimer (const bool is_clearmode, const bool is_cntup)
{//{{{
    if (is_clearmode) {
        Cgeneral_timer::setCCLR(emInFactor);//Clear mode
    } else {
        Cgeneral_timer::setCCLR(emProhibit);//Disable clear mode
    }
    if (is_cntup) {
        Cgeneral_timer::setCounterDirection(emCountUp);//Count up
    } else {
        Cgeneral_timer::setCounterDirection(emCountDown);//Count down
    }
    Cgeneral_timer::setTimeUnit(mATU5->mTimeResolutionUnit);//Time unit
    Cgeneral_timer::setCMS(32);
    Cgeneral_timer::setOVIE(true);
    Cgeneral_timer::setUNIE(true);
}//}}}

/*********************************
// Function     : cmpMatchOutputHandling
// Description  : Virtual function of Cgeneral_timer class
// Parameter    :
//      value               Compare match output
// Return value : None
**********************************/
void ATU5_Timer::cmpMatchOutputHandling (bool value)
{//{{{
    mCmpMatchVal = GetCounterValue();
    mCompareMatchEvent.notify(SC_ZERO_TIME);
}//}}}

/*********************************
// Function     : interruptHandling
// Description  : Virtual function of Cgeneral_timer class
// Parameter    :
//      value               None
// Return value : None
**********************************/
void ATU5_Timer::interruptHandling ()
{//{{{
    StatusRegister  status = Cgeneral_timer::getSR();
    if(status.gtOVF) {
        mOverflowEvent.notify(SC_ZERO_TIME);
    }
    if(status.gtUNF) {
        mUnderflowEvent.notify(SC_ZERO_TIME);
    }
}//}}}

void ATU5_Timer::SetTimerSize (const unsigned int value)
{//{{{
    Cgeneral_timer::setCMS(value);
}//}}}

// Added by Timer E

void ATU5_Timer::ClearBitOVF (void)
{//{{{
     Cgeneral_timer::clearOVF();
}//}}}

void ATU5_Timer::ClearBitCMF (void)
{//{{{
     Cgeneral_timer::clearCMF();
}//}}}

void ATU5_Timer::ClearBitUNF (void)
{//{{{
     Cgeneral_timer::clearUNF();
}//}}}

bool ATU5_Timer::GetBitOVF (void)
{//{{{
    StatusRegister  status = Cgeneral_timer::getSR();
    return status.gtOVF;
}//}}}

bool ATU5_Timer::GetBitCMF (void)
{//{{{
    StatusRegister  status = Cgeneral_timer::getSR();
    return status.gtCMF;
}//}}}

//
