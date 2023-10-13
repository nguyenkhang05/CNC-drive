// ---------------------------------------------------------------------
// $Id: DMAG_U2_AgentController.h,v 1.4 2020/02/14 06:25:20 hadinhtran Exp $
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

#ifndef __DMAG_U2_AGENT_H__
#define __DMAG_U2_AGENT_H__

#include <systemc.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class DMAG_U2;

class DMAG_U2_AgentController {
#include "DMAG_U2_cmdif.h"
public:
  enum eMaxChanNum {      // Define default channel number
    emNUM_MAX_ChanNum = 18
  };
  unsigned int mChannelNum;
  DMAG_U2_AgentController(void) {;}
  virtual ~DMAG_U2_AgentController(void) {;}

  virtual void NotifyTransErrorEvent(void) = 0;
  virtual bool GetResetStatus(void) = 0;
  virtual bool CheckClockPeriod(void) = 0;
  virtual unsigned int getPermittedSPID(unsigned char channel_id) = 0;
  virtual void getDMAGPROT_n(unsigned char ch_id, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg) = 0;
};

#endif//__DMAG_U2_AGENT_H__
