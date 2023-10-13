// ---------------------------------------------------------------------
// $Id: PBG_AgentController.h,v 1.8 2019/06/16 06:02:16 nhuvo Exp $
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

#ifndef __PBG_AGENT_H__
#define __PBG_AGENT_H__

#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class PBG;

class PBG_AgentController {
#include "PBG_cmdif.h"

public:
  enum ePBGGroupId {
    emPBG00=0,
    emPBG10,
    emPBG2l0,
    emPBG2l1,
    emPBG2h0,
    emPBG2h1,
    emPBG30,
    emPBG31,
    emPBG32,
    emPBG40,
    emPBG50,
    emPBG60,
    emPBG61,
    emPBG62,
    emPBG63,
    emPBG64,
    emPBG65,
    emPBG66,
    emPBG67,
    emPBG68,
    emPBG69,
    emPBG70,
    emPBG80,
    emPBG90,
    emPBGGroupNum
  };
  // std::string mGroupId;
  unsigned int mChannelNum;
  PBG_AgentController(void) {;}
  virtual ~PBG_AgentController(void) {;}

  virtual bool CheckClockPeriod(const std::string clock_name) = 0;
  virtual bool GetResetStatus() = 0;
  virtual void SetPBGPROT1(const unsigned int spid, const unsigned int ch_id) = 0;
  virtual void SetPBGPROT0(const unsigned int gen, const unsigned int dbg, const unsigned int um, const unsigned int wg, const unsigned int rg, const unsigned int sec, const unsigned int ch_id) = 0;
  virtual bool GetLOCK_SUPPORT() = 0;
  virtual bool GetSEC_DISABLED() = 0;
};


#endif//__PBG_AGENT_H__
