// ---------------------------------------------------------------------
// $Id: HBG_AgentController.h,v 1.10 2020/02/18 07:24:22 huyquocpham Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __HBG_AGENT_H__
#define __HBG_AGENT_H__

#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class HBG;

class HBG_AgentController {
#include "HBG_cmdif.h"

    public:
        static const int DEFAULT_LOCK = 1;
        static const int DEFAULT_SEC = 1;
        static const int MIN_LOCK = 0;
        static const int MAX_LOCK = 1;
        static const int DEFAULT_RESET_VALUE = 0x1F3;
        static const int DEFAULT_SPID_RESET_VALUE = 0xFFFFFFFF;
    public:
        HBG_AgentController(void) {;}
        virtual ~HBG_AgentController(void) {;}

        virtual bool CheckClockPeriod(const std::string clock_name) = 0;
        virtual void SetHBGPROT1(const unsigned int spid) = 0;
        virtual void SetHBGPROT0(const unsigned int gen, const unsigned int dbg, const unsigned int um, const unsigned int wg, const unsigned int rg) = 0;
        virtual unsigned int GetLOCK_SUPPORT() = 0;
        virtual void LOCK_SUPPORT(unsigned int value) = 0;
        virtual void SEC_DIS(unsigned int value) = 0;
        virtual void RESET_VALUE(unsigned int value) = 0;
        virtual void SPID_RESET_VALUE(unsigned int value) = 0;
        virtual unsigned int Get_KCE(void) = 0;
};


#endif//__HBG_AGENT_H__
