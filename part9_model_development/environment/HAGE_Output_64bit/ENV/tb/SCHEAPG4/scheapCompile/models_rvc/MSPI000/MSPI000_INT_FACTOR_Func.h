// ---------------------------------------------------------------------
// $Id: MSPI000_INT_FACTOR_Func.h,v 1.3 2020/03/11 00:52:43 ducla Exp $
//
// Copyright(c) 2019-2020 Renesas Electronics Corporation
// Copyright(c) 2019-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __MSPI000_INT_FACTOR_Func_H__
#define __MSPI000_INT_FACTOR_Func_H__

#include "BusSlaveFuncIf.h"
#include "mspi000_int_factor_regif.h"
#include "global.h"

class MSPI000_AgentController;
class MSPI000_INT_FACTOR_Func : public Cint_factor_regif
   , public BusSlaveFuncIf
{
public:
    enum eINT_Type{
        emNumTX = 0
        ,emNumRX = 1
        ,emNumFE = 2
        ,emNumERR = 3
    };
   MSPI000_INT_FACTOR_Func(std::string name, unsigned int ch_num, MSPI000_AgentController* AgentController);
   ~MSPI000_INT_FACTOR_Func();

   //user function here
   void RegisterHandler(const std::vector<std::string>& args);
   void EnableReset(const bool is_active);
   //

   // Virtual functions of BusSlaveFuncIf class
   void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
   void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
   unsigned int GetMSPInINTFk(unsigned int chid);
   unsigned int GetMSPInINTMSKk(unsigned int chid);
   void SetMSPInINTFk(unsigned int chid, unsigned int index);


private:

   MSPI000_AgentController     *mpAgentController;       // Agent controller pointer
   std::string                 mModuleName;              // Module name
   unsigned int                mNumChannel;              // Number of channels

   void Initialize(void);
   void cb_MSPInINTMSKk_MSPInINTMSKk(RegCBstr str);
   void cb_MSPInINTFCk_MSPInINTFCk(RegCBstr str);
};
#endif //__MSPI000_INT_FACTOR_Func_H__
