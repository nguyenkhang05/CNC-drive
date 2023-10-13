// ---------------------------------------------------------------------
// $Id: ECCUEABAgent.h,v 1.0 2013/08/07 10:02:20 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECCUEAB_AGENT__
#define __ECCUEAB_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ECCUEABAgent
{
#include "ECCUEAB_cmdif.h"
public:
    ECCUEABAgent(void) {;}
    virtual ~ECCUEABAgent(void) {;}
    virtual bool CheckClockPeriod (std::string clock_name) = 0;
};
#endif
