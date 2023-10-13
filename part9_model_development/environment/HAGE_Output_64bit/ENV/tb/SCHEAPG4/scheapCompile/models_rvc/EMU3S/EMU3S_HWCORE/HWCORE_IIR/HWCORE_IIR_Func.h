// ---------------------------------------------------------------------
// $Id: HWCORE_IIR_Func.h,v 1.1 2020/08/18 07:30:11 minhquocha Exp $
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
// Description: Copyright of HWCORE_IIR_Func.h
// Ref: {HWCORE_IIR_UD_Copyright_001}
#ifndef __HWCORE_IIR_FUNC_H__
#define __HWCORE_IIR_FUNC_H__
#include "hwcore_iir_regif.h"
#include "HWCORE_IIR_AgentController.h"

// Description: Declaration of HWCORE_IIR_Func class
// Ref: {HWCORE_IIR_UD_ATT1_004}
class HWCORE_IIR_Func : public Chwcore_iir_regif
{
public:
    SC_HAS_PROCESS(HWCORE_IIR_Func);
    HWCORE_IIR_Func(std::string name, HWCORE_IIR_AgentController* HWCORE_IIRAgentControllerPtr);
    ~HWCORE_IIR_Func();

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
    HWCORE_IIR_AgentController * mpHWCORE_IIRAgentController;    // HWCORE_IIR Agent controller pointer
    std::string                  mModuleName;                    // Module name

    // Function
    void Initialize(void);
    // Callback
    void cb_IIRINIT_INIT0(RegCBstr str);
    void cb_IIRSFT_TRG(RegCBstr str);
    void cb_IIRRLD_RLD0(RegCBstr str);
    void cb_IIRRSL_RSL(RegCBstr str);

public:
    std::map<std::string, vpcl::re_register*> mRegisterMap;       // Map value of registers
};

#endif // __HWCORE_IIR_FUNC_H__
