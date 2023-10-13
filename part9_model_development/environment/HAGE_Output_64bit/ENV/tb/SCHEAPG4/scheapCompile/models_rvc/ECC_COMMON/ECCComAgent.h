// ---------------------------------------------------------------------
// $Id: ECCComAgent.h,v 1.0 2013/08/05 02:00:12 dungvannguyen Exp $
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
#ifndef __ECCCOM_AGENT__
#define __ECCCOM_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ECCComAgent
{
#include "ECCCom_cmdif.h"
public:
    ECCComAgent(void) {;}
    virtual ~ECCComAgent(void) {;}
    virtual void ControlPCLKErrorOutputSignals (const std::string signal_name) = 0;
    virtual void ControlSYS_CLKErrorOutputSignals (const std::string signal_name, const unsigned int bank_no, const unsigned int error_addr) = 0;
    virtual void WaitPCLKPosEdgeToControlECCDisableSignals (void) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
};
#endif
