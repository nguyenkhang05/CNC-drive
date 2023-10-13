// ---------------------------------------------------------------------
// $Id: HWCORE_RSLVIDE_Func.h,v 1.2 2020/10/22 04:06:16 ngathunguyen Exp $
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
// Description: Copyright of HWCORE_RSLVIDE_Func.h
// Ref: {HWCORE_RSLVIDE_UD_Copyright_001}
#ifndef __HWCORE_RSLVIDE_FUNC_H__
#define __HWCORE_RSLVIDE_FUNC_H__
#include "hwcore_rslvide_regif.h"
#include "HWCORE_RSLVIDE_AgentController.h"

// Description: Declaration of HWCORE_RSLVIDE_Func class
// Ref: {HWCORE_RSLVIDE_UD_ATT1_004}
class HWCORE_RSLVIDE_Func : public Chwcore_rslvide_regif
{
public:
    SC_HAS_PROCESS(HWCORE_RSLVIDE_Func);
    HWCORE_RSLVIDE_Func(std::string name, HWCORE_RSLVIDE_AgentController* HWCORE_RSLVIDEAgentControllerPtr);
    ~HWCORE_RSLVIDE_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);

    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void SetRegVal(std::string RegName, unsigned int val);
    unsigned int GetRegVal(std::string RegName);
    unsigned int GetRegVal(std::string RegName, std::string BitName);

private:

    enum eVARIABLE_VALUE{
        emRESOLVER_CURR_VALUE   = 0,
        emTHTEFIX_PRE_VALUE     = 1
    };
    // Variables
    HWCORE_RSLVIDE_AgentController * mpHWCORE_RSLVIDEAgentController;    // HWCORE_RSLVIDE Agent controller pointer
    std::string     mModuleName;                           // Module name

    // Function
    void Initialize(void);
    // Callback
    void cb_ANGCTR_ANGSEL(RegCBstr str);
    void cb_ANGOFS_DATA(RegCBstr str);
    void cb_RESTHSFT_DATA(RegCBstr str);
    void cb_THTEFIX_DATA(RegCBstr str);
    void cb_THTRESFIX_DATA(RegCBstr str);

    std::map<std::string, vpcl::re_register*> mRegisterMap;        // Map value of registers
        
};

#endif // __HWCORE_RSLVIDE_FUNC_H__
