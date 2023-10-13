// ---------------------------------------------------------------------
// $Id: HI20FSGD2_ERRSLV_AgentController.h,v 1.1 2019/11/26 04:05:44 hadinhtran Exp $
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __HI20FSGD2_ERRSLV_AGENT_H__
#define __HI20FSGD2_ERRSLV_AGENT_H__

#include <systemc.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class HI20FSGD2_ERRSLV;

class HI20FSGD2_ERRSLV_AgentController {
#include "HI20FSGD2_ERRSLV_cmdif.h"
public:
  enum eContanstValue{
        emZero = 0,
        emOne = 1,
        emTwo = 2
    };

  HI20FSGD2_ERRSLV_AgentController(void) {;}
  virtual ~HI20FSGD2_ERRSLV_AgentController(void) {;}

  virtual bool GetResetStatus(void) = 0;
  virtual bool CheckClockPeriod(void) = 0;
  virtual void SetKceBit(const bool value) = 0;
};

#endif//__HI20FSGD2_ERRSLV_AGENT_H__
