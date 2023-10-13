// ---------------------------------------------------------------------
// $Id: MSPITG_Func.h,v 1.3 2020/02/08 08:01:54 ducla Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of MSPITG_Func.h
// Ref: {MSPITG_DD_Copyright_001}
#ifndef __MSPITG_FUNC_H__
#define __MSPITG_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "mspitg_regif.h"
#include "global.h"

class MSPITG_AgentController;
class MSPITG_Func : public Cmspitg_regif
   , public BusSlaveFuncIf
{
public:
   MSPITG_Func(std::string name, MSPITG_AgentController* AgentController);
   ~MSPITG_Func();

   //user function here
   void RegisterHandler(const std::vector<std::string>& args);
   void EnableReset(const bool is_active);
   //
   // Virtual functions of BusSlaveFuncIf class
   void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
   void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
private:

   MSPITG_AgentController     *mpAgentController;       // Agent controller pointer
   std::string                 mModuleName;              // Module name
   
   void Initialize(void);
   void cb_MSPITGCTLn_TRGSEL0(RegCBstr str);
   void cb_MSPITGDMAALT_DMAAS02(RegCBstr str);
   void cb_MSPITGDTSALT_DTSAS06(RegCBstr str);
};
#endif //__MSPITG_Func_H__
