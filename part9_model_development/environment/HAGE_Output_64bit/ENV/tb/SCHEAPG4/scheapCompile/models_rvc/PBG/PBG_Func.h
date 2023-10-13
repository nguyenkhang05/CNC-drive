// ---------------------------------------------------------------------
// $Id: PBG_Func.h,v 1.4 2019/06/16 06:02:16 nhuvo Exp $
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
#ifndef __PBG_FUNC_H__
#define __PBG_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "pbguard_regif.h"
#include "global.h"

class PBG_AgentController;
class PBG_Func: public Cpbguard_regif,
                public BusSlaveFuncIf
{
public:
  PBG_Func (std::string name, PBG_AgentController *PBG_AgentControllerPtr, unsigned int ch_num);
  ~PBG_Func (void);

  void Reset (const bool is_active);
  void RegisterHandler (const std::vector<std::string> &args);
  void SetVar(const std::string name, const unsigned int val);

  /// Virtual functions of BusSlaveFuncIf class
  void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
  void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
  uint32_t GetPBGERRSTAT();    //Get value of PBGERRSTAT register

private:
  PBG_AgentController *mPBG_AgentController;
  std::string mModuleName;        //Module name


  /// Declare virtual functions of PBG_regif class
    void cb_PBGERRCLR_CLRO(RegCBstr str);
    void cb_PBGKCPROT_KCPROT(RegCBstr str);
    void cb_PBGPROT0_LOCK(RegCBstr str);
    void cb_PBGPROT1_SPID(RegCBstr str);
};
#endif //__PBG_FUNC_H__

