// -----------------------------------------------------------------------------
// $Id: HWCORE_PWM_Func.h,v 1.3 2020/10/06 03:56:40 ducla Exp $
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
// Description: Copyright of HWCORE_PWM_Func.h
// Ref: {HWCORE_PWM_UD_Copyright_001}
#ifndef __HWCORE_PWM_FUNC_H__
#define __HWCORE_PWM_FUNC_H__

#include "hwcore_pwm_regif.h"

class HWCORE_PWM_AgentController;
class HWCORE_PWM_Func : public Chwcore_pwm_regif
{
public:
   HWCORE_PWM_Func(std::string name, HWCORE_PWM_AgentController* AgentController);
   ~HWCORE_PWM_Func();

   std::map<std::string, vpcl::re_register*> mRegisterMap;        // Map value of registers
   //user function here
   void RegisterHandler(const std::vector<std::string>& args);
   void EnableReset(const bool is_active);
   void EnableResetRegister(const bool is_active);
   //
   // Virtual functions of BusSlaveFuncIf class
   void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
   void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

   void SetRegVal(std::string RegName, unsigned int val);
   unsigned int GetRegVal(std::string RegName);
   unsigned int GetRegVal(std::string RegName, std::string BitName);

private:
   HWCORE_PWM_AgentController     *mpAgentController;       // Agent controller pointer
   std::string                 mModuleName;              // Module name
   
   void Initialize(void);
   void cb_PWMCTR_PWMOP(RegCBstr str);
   void cb_PWMDT_PWMDT(RegCBstr str);

public:
   // buffer Register value
   unsigned int mPWMCTR_buf_value;
   unsigned int mVDCRCT_buf_value;
   unsigned int mVQCRCT_buf_value;
   unsigned int mDECVELG_buf_value;
   unsigned int mDECFLUX_buf_value;
   unsigned int mDECLD_buf_value;
   unsigned int mDECLQ_buf_value;
   unsigned int mPHI_buf_value;
   unsigned int mGTHT_buf_value;
   unsigned int mTHTFORESFT_buf_value;
   unsigned int mTHTVSFT_buf_value;
   unsigned int mVDQSFT_buf_value;
   unsigned int mTMAX_buf_value;
   unsigned int mVOLV_buf_value;
   unsigned int mVUOFS_buf_value;
   unsigned int mVVOFS_buf_value;
   unsigned int mVWOFS_buf_value;
   unsigned int mCARR_buf_value;
};
#endif //__HWCORE_PWM_Func_H__
