// ---------------------------------------------------------------------
// $Id: ATU5_TIMERA_Base.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERA_BASE_H__
#define __ATU5_TIMERA_BASE_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ATU5_Base;
class ATU5_TIMERA_Base
{
#include "ATU5_TIMERA_cmdif.h"
public:
    enum eCLKID {
        emCLKBUSnum = 6,
        emCLKNSnum = 2
    };

    ATU5_Base                            *mATU5;

    ATU5_TIMERA_Base(void){;}
    virtual ~ATU5_TIMERA_Base(void){;}

    virtual bool IsTimerRunning (void) = 0;
    virtual bool IsNoiseCancelDuration (const unsigned int chan_num) = 0;
    virtual unsigned int GetCounterValue (void) = 0;
    virtual unsigned int GetNoiseCounterValue (const unsigned int chan_num) = 0;
    virtual unsigned int GetTILRAVal (void) = 0;
    virtual void UpdateCounterValue (void) = 0;
    virtual void UpdateNoiseCounterValue (const unsigned int chan_num) = 0;
    virtual void SetNoiseCancelMode (const unsigned int chan_num) = 0;
    virtual unsigned int GetNoiseCancelMode (const unsigned int chan_num) = 0;
    virtual void SetNoiseCORVal (const unsigned int chan_num, const unsigned int cor_val) = 0;
    virtual void EnableNoiseCancel (const unsigned int chan_num, const bool is_enable) = 0;
};
#endif
