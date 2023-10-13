// ---------------------------------------------------------------------
// $Id: ECC_BUS_MER_AD_Reg.h,v 1.4 2020/04/01 06:41:19 thunguyen3 Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_BUS_MER_AD_REG_H__
#define __ECC_BUS_MER_AD_REG_H__

#include "ecc_bus_mer_ad_regif.h"
#include "ECC_BUS_MER_AD_AgentController.h"
#include "ECC_def.h"

class ECC_BUS_MER_AD_Func;

class ECC_BUS_MER_AD_Reg: public Cecc_bus_mer_ad_regif
{
public:
    SC_HAS_PROCESS (ECC_BUS_MER_AD_Reg);
    ECC_BUS_MER_AD_Reg(std::string name, ECC_BUS_MER_AD_Func* parent_func, ECC_BUS_MER_AD_AgentController* ECC_BUS_MER_ADAgentController);
    ~ECC_BUS_MER_AD_Reg();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void ProcessDataError(ErrorInfo data_error);
    void ProcessAddressError(ErrorInfo address_error);
    void DumpStatusInfo();
    unsigned int GetECCCTLReg();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int address, unsigned char *p_data, unsigned int size, bool debug);
    void write(unsigned int address, unsigned char *p_data, unsigned int size, bool debug);

private:
    ECC_BUS_MER_AD_AgentController *mpECC_BUS_MER_ADAgentController;
    // enum
    enum eRegInfo{
        emKeyCode = 0xA5A5A500
    };

    // Variables
    ECC_BUS_MER_AD_Func *mpParentFunc;
    std::string mModuleName;// Module name

    // Function
    void Initialize();

    // Callback
    void cb_ECCCTL_ECCDIS(RegCBstr str);
    void cb_ERRINT_SEDIE(RegCBstr str);
    void cb_STCLR_STCLR0(RegCBstr str);
    void cb_KCPROT_KCE(RegCBstr str);

};
#endif //__ECC_BUS_MER_AD_REG_H__
