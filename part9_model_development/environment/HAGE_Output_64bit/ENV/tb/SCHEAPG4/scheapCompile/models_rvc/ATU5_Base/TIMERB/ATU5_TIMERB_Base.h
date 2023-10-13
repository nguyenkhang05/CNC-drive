// ---------------------------------------------------------------------
// $Id: ATU5_TIMERB_Base.h 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#ifndef __ATU5_TIMERB_BASE_H__
#define __ATU5_TIMERB_BASE_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ATU5_Base;
class ATU5_TIMERB_Base
{
#include "ATU5_TIMERB_cmdif.h"
public:
    enum eCLKID {    // Define clock index
        emCLKBUSnum = 4
    };

    ATU5_Base                            *mATU5;

    ATU5_TIMERB_Base(void){;}
    virtual ~ATU5_TIMERB_Base(void){;}
    virtual bool IsTCNTB0Running (const unsigned int block_num) = 0;
    virtual unsigned int GetCounterValue (const std::string cnt_name, const unsigned int block_num) = 0;
    virtual void SetTCNTB0CORVal (const unsigned int block_num, const unsigned int cor_val) = 0;
    virtual void WriteAGCKMOut (void) = 0;
};
#endif
