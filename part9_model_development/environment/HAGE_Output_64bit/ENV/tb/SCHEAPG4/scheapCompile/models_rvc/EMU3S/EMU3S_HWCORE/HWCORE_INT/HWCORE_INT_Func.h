// ---------------------------------------------------------------------
// $Id: HWCORE_INT_Func.h,v 1.1 2020/10/27 04:49:12 huyquocpham Exp $
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
// Description: Copyright of HWCORE_INT_Func.h
// Ref: {EMU3S_HWCORE_UD_Copyright_001}
#ifndef __HWCORE_INT_FUNC_H__
#define __HWCORE_INT_FUNC_H__
#include "hwcore_int_regif.h"
#include "HWCORE_INT_AgentController.h"

// Description: Declaration of HWCORE_INT_Func class
// Ref: {EMU3S_HWCORE_UD_ATT1_003, EMU3S_HWCORE_UD_ATT1_006, EMU3S_HWCORE_UD_ATT1_008}
class HWCORE_INT_Func : public Chwcore_int_regif
{
public:
    SC_HAS_PROCESS(HWCORE_INT_Func);
    HWCORE_INT_Func(std::string name, HWCORE_INT_AgentController* HWCORE_INTAgentControllerPtr);
    ~HWCORE_INT_Func();

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
    HWCORE_INT_AgentController * mpHWCORE_INTAgentController;    // HWCORE_INT Agent controller pointer
    std::string                  mModuleName;                    // Module name
    std::map<std::string, vpcl::re_register*> mRegisterMap;       // Map value of registers

    // Callback
    void cb_INTSDC_INIFC(RegCBstr str);
    void cb_ERRSDC_OVFEFC(RegCBstr str);


};

#endif // __HWCORE_INT_FUNC_H__
