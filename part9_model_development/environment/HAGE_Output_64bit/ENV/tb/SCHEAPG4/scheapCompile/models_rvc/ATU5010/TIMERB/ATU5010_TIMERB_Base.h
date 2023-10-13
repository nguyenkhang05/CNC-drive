// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERB_Base.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#ifndef __ATU5010_TIMERB_BASE_H__
#define __ATU5010_TIMERB_BASE_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ATU5010;
class ATU5010_TIMERB_Base
{
#include "ATU5010_TIMERB_cmdif.h"
public:
    enum eCLKID {    // Define clock index
        emCLKBUSnum = 4
    };

    ATU5010                            *mATU5010;

    ATU5010_TIMERB_Base(void){;}
    virtual ~ATU5010_TIMERB_Base(void){;}
    virtual bool IsTCNTB0Running (void) = 0;
    virtual unsigned int GetCounterValue (const std::string cnt_name) = 0;
    virtual void SetTCNTB0CORVal (const unsigned int cor_val) = 0;
};
#endif
