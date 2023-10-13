// ---------------------------------------------------------------------
// $Id: ECC_APB_Func.h,v 1.3 2018/01/18 06:29:15 chanle Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_APB_FUNC_H__
#define __ECC_APB_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ecc_apb_regif.h"
#include "ECC_APB_AgentController.h"

class ECC_APB_Func: public Cecc_apb_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECC_APB_Func);
    ECC_APB_Func(std::string name, std::string kind, ECC_APB_AgentController* APBAgentController);
    ~ECC_APB_Func();

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

    // Variables
    ECC_APB_AgentController *mpECCAPBAgentController;
    std::string mModuleName;// Module name
    std::string mKind;      // Kind of ECC_APB

    // Function

    // Callback
    void cb_ECCCTL_PROT(RegCBstr str);
    void cb_ERSTCLR_SSTCLR(RegCBstr str);

};
#endif //__ECC_APB_FUNC_H__
