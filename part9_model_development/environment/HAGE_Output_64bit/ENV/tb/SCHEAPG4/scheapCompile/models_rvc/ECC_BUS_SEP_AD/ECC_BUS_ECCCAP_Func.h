// ---------------------------------------------------------------------
// $Id: ECC_BUS_ECCCAP_Func.h,v 1.6 2020/03/30 10:53:33 thunguyen3 Exp $
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
#ifndef __ECC_BUS_ECCCAP_FUNC_H__
#define __ECC_BUS_ECCCAP_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ecc_bus_ecccap_regif.h"
#include "ECC_def.h"
#include "ECC_BUS_SEP_AD_AgentController.h"
#include "global.h"

class ECC_BUS_ECCCAP_Func: public Cecc_bus_ecccap_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECC_BUS_ECCCAP_Func);
    ECC_BUS_ECCCAP_Func(std::string name, ECC_BUS_SEP_AD_AgentController* ECC_BUS_SEP_ADAgentController, std::string err_kind);
    ~ECC_BUS_ECCCAP_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void ProcessDataError(ErrorInfo data_error);
    void ProcessAddressError(ErrorInfo address_error);
    void DumpStatusInfo();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // enum
    enum eRegisterInfo{
        emSizeofStatusReg = 32 // status reg has 32 bits for 32 error detection locations.
    };
    // Variables
    ECC_BUS_SEP_AD_AgentController   *mpECC_BUS_SEP_ADAgentController;
    std::string mModuleName;    // Module name
    std::string mErr_Kind;      // Kind of error

    // Functions

    // Callback
    void cb_ERRINT_ADEDIE(RegCBstr str);
    void cb_ERRINT_DSEDIE(RegCBstr str);
    void cb_ASSTCLR_ASSTCLR(RegCBstr str);
    void cb_ADSTCLR_ADSTCLR(RegCBstr str);
    void cb_DSSTCLR_DSSTCLR(RegCBstr str);
    void cb_DDSTCLR_DDSTCLR(RegCBstr str);

};
#endif //__ECC_BUS_ECCCAP_FUNC_H__
