// ---------------------------------------------------------------------
// $Id: HI20FSGD2_AgentController.h,v 1.6 2019/12/06 07:19:03 hadinhtran Exp $
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

#ifndef __HI20FSGD2_AGENT_H__
#define __HI20FSGD2_AGENT_H__

#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class HI20FSGD2;

class HI20FSGD2_AgentController {
#include "HI20FSGD2_cmdif.h"

public:
    static const int DEFAULT_LOCK = 1;
    static const int MAX_SEC = 1;
    static const int MIN_LOCK = 0;
    static const int MAX_LOCK = 1;

    // std::string mGroupId;
    unsigned int mChannelNum;
    bool         mICUMHA;
    HI20FSGD2_AgentController(void) {;}
    virtual ~HI20FSGD2_AgentController(void) {;}

    virtual bool CheckKceBitStatus(void) = 0;
    virtual bool GetResetStatus(void) = 0;
    virtual bool CheckClockPeriod(void) = 0;
    virtual void SetPBGPROT1(const unsigned int spid, const unsigned int ch_id) = 0;
    virtual void SetPBGPROT0(const unsigned int sec,const unsigned int gen, const unsigned int dbg, const unsigned int um, const unsigned int wg, const unsigned int rg, const unsigned int ch_id) = 0;
    virtual unsigned int GetLOCK_SUPPORT() = 0;
    virtual void LOCK_SUPPORT(unsigned int value) = 0;
    virtual void SEC_DIS(unsigned int value) = 0;
};


#endif//__HI20FSGD2_AGENT_H__
