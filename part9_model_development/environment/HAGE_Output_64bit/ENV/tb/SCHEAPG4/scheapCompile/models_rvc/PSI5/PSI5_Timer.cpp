// ---------------------------------------------------------------------
// $Id: PSI5_Timer.cpp,v 1.7 2020/04/23 07:12:11 hadinhtran Exp $
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
#include "PSI5_Timer.h"

///*********************************
/// Function     : PSI5_Timer
/// Description  : Constructor of PSI5_Timer class
///*********************************
PSI5_Timer::PSI5_Timer (std::string name):Cgeneral_timer(name.c_str())
{//{{{
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : ~PSI5_Timer 
/// Description  : Destructor of PSI5_Timer class
///*********************************
PSI5_Timer::~PSI5_Timer (void)
{//{{{
}//}}}

///*********************************
/// Function     : EnableReset 
/// Description  : Reset general timer
///*********************************
void PSI5_Timer::EnableReset (const bool is_active)
{//{{{
    Cgeneral_timer::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : StartStop 
/// Description  : Start/stop general timer
///*********************************
void PSI5_Timer::StartStop (const bool is_start)
{//{{{
    Cgeneral_timer::setSTR(is_start);
}//}}}

///*********************************
/// Function     : SetCompareMatchValue 
/// Description  : Set compare match value of general timer
///*********************************
void PSI5_Timer::SetCompareMatchValue (const unsigned int value)
{//{{{
    Cgeneral_timer::setCOR(value);
}//}}}

///*********************************
/// Function     : SetCounterIntialValue 
/// Description  : Set counter initial value of general timer
///*********************************
void PSI5_Timer::SetCounterIntialValue (const unsigned int value)
{//{{{
     Cgeneral_timer::setCNT(value);
}//}}}

///*********************************
/// Function     : SetCounterPeriod 
/// Description  : Set counter period of general timer
///*********************************
void PSI5_Timer::SetCounterPeriod (const double counter_period)
{//{{{
    Cgeneral_timer::setClkCountPeriod(counter_period);
}//}}}

///*********************************
/// Function     : SetCounterMode 
/// Description  : Set counter mode of general timer
///*********************************
void PSI5_Timer::SetCounterMode (const bool is_freerun)
{//{{{
    if (is_freerun) {
        Cgeneral_timer::setCMM(emFreeRun);//Free-running mode
    } else {
        Cgeneral_timer::setCMM(emOneShot);//One-shot mode
    }
}//}}}

///*********************************
/// Function     : SetupGeneralTimer 
/// Description  : Setup General Timer
///*********************************
void PSI5_Timer::SetupGeneralTimer ()
{//{{{
    Cgeneral_timer::setCCLR(emInFactor);//Clear mode
    Cgeneral_timer::setCounterDirection(emCountUp);//Count up
    Cgeneral_timer::setTimeUnit(mTimeResolutionUnit);//Time unit
    Cgeneral_timer::setCMS(32);//Counter size
    Cgeneral_timer::setOVIE(true);
}//}}}

///*********************************
/// Function     : GetCounterValue 
/// Description  : Get counter value of general timer
///*********************************
unsigned int PSI5_Timer::GetCounterValue ()
{//{{{
    return Cgeneral_timer::getCNT();
}//}}}

///*********************************
/// Function     : GetTimeResolution 
/// Description  : Get time resolution unit
///*********************************
void PSI5_Timer::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        resolution_value = 1000000000000000LL;
        resolution_unit = SC_FS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        resolution_value = 1000000000000LL;
        resolution_unit = SC_PS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        resolution_value = 1000000000;
        resolution_unit = SC_NS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        resolution_value = 1000000;
        resolution_unit = SC_US;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        resolution_value = 1000;
        resolution_unit = SC_MS;
    } else {
        resolution_value = 1;
        resolution_unit = SC_SEC;
    }
}//}}}

///*********************************
/// Function     : cmpMatchOutputHandling 
/// Description  : virtual function of Cgeneral_timer class
///*********************************
void PSI5_Timer::cmpMatchOutputHandling (bool value)
{//{{{
    mCompareMatchEvent.notify(SC_ZERO_TIME);
}//}}}
/*********************************
// Function     : interruptHandling
// Description  : Virtual function of Cgeneral_timer class
// Parameter    :
//      value               None
// Return value : None
**********************************/
void PSI5_Timer::interruptHandling ()
{//{{{
    StatusRegister  status = Cgeneral_timer::getSR();
    if(status.gtOVF) {
        mOverflowEvent.notify(SC_ZERO_TIME);
    }
}//}}}
// vim600: set foldmethod=marker :
