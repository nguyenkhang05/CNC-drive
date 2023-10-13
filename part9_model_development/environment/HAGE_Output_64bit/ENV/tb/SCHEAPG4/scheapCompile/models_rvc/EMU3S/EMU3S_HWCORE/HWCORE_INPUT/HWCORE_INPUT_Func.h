// -----------------------------------------------------------------------------
// $Id: HWCORE_INPUT_Func.h,v 1.3 2020/10/21 04:27:23 quynhnhunguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
// Description: Copyright of HWCORE_INPUT_Func.h
// Ref: {HWCORE_INPUT_UD_Copyright_001}
#ifndef __HWCORE_INPUT_FUNC_H__
#define __HWCORE_INPUT_FUNC_H__

#include "hwcore_input_regif.h"

class HWCORE_INPUT_AgentController;
class HWCORE_INPUT_Func : public Chwcore_input_regif
{
public:
   HWCORE_INPUT_Func(std::string name, HWCORE_INPUT_AgentController* AgentController);
   ~HWCORE_INPUT_Func();

   std::map<std::string, vpcl::re_register*> mRegisterMap;        // Map value of registers
   //user function here
   void RegisterHandler(const std::vector<std::string>& args);
   void EnableReset(const bool is_active);
   //
   // Virtual functions of BusSlaveFuncIf class
   void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
   void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

   unsigned int GetRegUnsignedVal(std::string RegName);
   signed int GetRegSignedVal(std::string RegName);
   void SetRegUnsignedVal(std::string RegName, unsigned int val);
   void SetRegSignedVal(std::string RegName, signed int val);

private:

   HWCORE_INPUT_AgentController     *mpAgentController;       // Agent controller pointer
   std::string                 mModuleName;              // Module name
   
   void Initialize(void);
   void cb_CTRINMD_INSTCTR(RegCBstr str);
   void cb_THTESEL_DATA(RegCBstr str);
   void cb_AD0IBUF_DATA(RegCBstr str);
   void cb_AD1IBUF_DATA(RegCBstr str);
   void cb_AD2IBUF_DATA(RegCBstr str);
   void cb_IUFIX_DATA(RegCBstr str);
   void cb_IVFIX_DATA(RegCBstr str);
   void cb_IWFIX_DATA(RegCBstr str);
   void cb_KCLSTATC_CLR(RegCBstr str);
   void cb_IDFIXOBUF_DATA(RegCBstr str);
   void cb_IQFIXOBUF_DATA(RegCBstr str);
};
#endif //__HWCORE_INPUT_Func_H__
