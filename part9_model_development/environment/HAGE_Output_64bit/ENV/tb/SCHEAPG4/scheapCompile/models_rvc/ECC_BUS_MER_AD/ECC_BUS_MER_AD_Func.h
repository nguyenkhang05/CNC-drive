// ---------------------------------------------------------------------
// $Id: ECC_BUS_MER_AD_Func.h,v 1.4 2020/03/30 10:43:38 thunguyen3 Exp $
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
#ifndef __ECC_BUS_MER_AD_FUNC_H__
#define __ECC_BUS_MER_AD_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ECC_def.h"
#include "ECC_BUS_MER_AD_AgentController.h"
#include "global.h"

class ECC_BUS_APEC_Reg;
class ECC_BUS_MER_AD_Reg;

class ECC_BUS_MER_AD_Func: public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECC_BUS_MER_AD_Func);
    ECC_BUS_MER_AD_Func(std::string name, std::string kind, ECC_BUS_MER_AD_AgentController* ECC_BUS_MER_ADAgentController);
    virtual ~ECC_BUS_MER_AD_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void ProcessDataError(ErrorInfo data_error);
    void ProcessAddressError(ErrorInfo address_error);
    unsigned int GetECCCTLReg();
    void DumpStatusInfo();
    void AssertInterrupt(std::string int_name);
    void WriteDisablePort(unsigned int value);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // enum

    // Variables
    ECC_BUS_MER_AD_AgentController *mpECC_BUS_MER_ADAgentController;
    std::string mModuleName;// Module name
    std::string mKind;      // Kind of bus
    ECC_BUS_APEC_Reg    *mpAPECRegBlock;    // Block to process related to register of APEC kind
    ECC_BUS_MER_AD_Reg  *mpMERRegBlock;     // Block to process related to register of other (MER_AD) kind

    // Function
};
#endif //__ECC_BUS_MER_AD_FUNC_H__
