// ---------------------------------------------------------------------
// $Id: DTSG_U2_AgentController.h,v 1.3 2020/01/07 04:00:23 synguyen Exp $
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

#ifndef __DTSG_U2_AGENT_H__
#define __DTSG_U2_AGENT_H__

#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>


class DTSG_U2_AgentController {
#include "DTSG_U2_cmdif.h"

    DTSG_U2_AgentController(void) {;}
    virtual ~DTSG_U2_AgentController(void) {;}

    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual void GetDTSGPROT(const unsigned int channel_index, unsigned int& mpid, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg) = 0;
    virtual unsigned int GetSPIDERR() = 0;
    virtual unsigned int GetDTSGMPID_SPID(const unsigned int channel_index) = 0;

};


#endif//__DTSG_U2_AGENT_H__
