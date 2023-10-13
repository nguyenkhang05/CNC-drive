// ---------------------------------------------------------------------
// $Id: ATU5_TIMERE_AgentController.h 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#ifndef __ATU5_TIMERE_AGENT__
#define __ATU5_TIMERE_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>


class ATU5_Base;
class ATU5_TIMERE_SUB;
class ATU5_TIMERE_AgentController
{
#include "ATU5_TIMERE_cmdif.h"
public:
    ATU5_Base *mATU5;
    enum eChannel {
        emChannel0 = 0,
        emChannel1,
        emChannel2,
        emChannel3,
        emChannel
        };
    ATU5_TIMERE_AgentController(void) {;}
    virtual ~ATU5_TIMERE_AgentController(void) {;}
    ATU5_TIMERE_SUB **mTIMERE_SUB;   // timerE unit instance
    //virtual function
};
#endif
