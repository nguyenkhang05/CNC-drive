// ---------------------------------------------------------------------
// $Id: HWCORE_PI_Func.h,v 1.3 2020/11/06 06:44:20 minhquocha Exp $
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
// Description: Copyright of HWCORE_PI_Func.h
// Ref: {HWCORE_PI_UD_Copyright_001}
#ifndef __HWCORE_PI_FUNC_H__
#define __HWCORE_PI_FUNC_H__
#include "hwcore_pi_regif.h"

// Description: Declaration of HWCORE_PI_Func class
// Ref: {HWCORE_PI_UD_ATT1_004}
class HWCORE_PI_Func : public Chwcore_pi_regif
{
public:
    SC_HAS_PROCESS(HWCORE_PI_Func);
    HWCORE_PI_Func(std::string name, HWCORE_PI_AgentController* HWCORE_PIAgentControllerPtr);
    ~HWCORE_PI_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void SetRegVal(std::string RegName, unsigned int val);
    void SetRegVal(std::string RegName, std::string BitName, unsigned int val);
    unsigned int GetRegVal(std::string RegName);
    unsigned int GetRegVal(std::string RegName, std::string BitName);

private:

    // Variables
    HWCORE_PI_AgentController * mpHWCORE_PIAgentController;    // HWCORE_PI Agent controller pointer
    std::string                  mModuleName;                    // Module name

    // Function
    void Initialize(void);
    void cb_IDIN_DATA(RegCBstr str);
    void cb_IQIN_DATA(RegCBstr str);
    void cb_ID_DATA(RegCBstr str);
    void cb_IQ_DATA(RegCBstr str);
    void cb_VD_DATA(RegCBstr str);
    void cb_VQ_DATA(RegCBstr str);
public:
    std::map<std::string, vpcl::re_register*> mRegisterMap;       // Map value of registers
};

#endif // __HWCORE_PI_FUNC_H__
