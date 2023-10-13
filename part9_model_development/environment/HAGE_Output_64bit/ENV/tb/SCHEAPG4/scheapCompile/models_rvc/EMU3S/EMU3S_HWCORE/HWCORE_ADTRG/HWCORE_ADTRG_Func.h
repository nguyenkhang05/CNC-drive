// ---------------------------------------------------------------------
// $Id: HWCORE_ADTRG_Func.h,v 1.3 2020/10/14 09:55:35 huepham Exp $
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
// Description: Copyright of HWCORE_ADTRG_Func.h
// Ref: {HWCORE_ADTRG_UD_Copyright_001}
#ifndef __HWCORE_ADTRG_FUNC_H__
#define __HWCORE_ADTRG_FUNC_H__
#include "BusMasterFuncIf.h"
#include "hwcore_adtrg_regif.h"
#include "HWCORE_ADTRG_AgentController.h"

// Description: Declaration of HWCORE_ADTRG_Func class
// Ref: {HWCORE_ADTRG_UD_ATT1_004}
class HWCORE_ADTRG_Func : public Chwcore_adtrg_regif
    , public BusMasterFuncIf
{
public:
    SC_HAS_PROCESS(HWCORE_ADTRG_Func);
    HWCORE_ADTRG_Func(std::string name, HWCORE_ADTRG_AgentController* HWCORE_ADTRGAgentControllerPtr);
    ~HWCORE_ADTRG_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    unsigned int GetRegisterVal(std::string regname);
    void SetRegisterVal(std::string regname, unsigned int val);
    void SetRegisterVal(std::string RegName, std::string BitName, unsigned int val);
    // Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);

private:
    // Variables
    HWCORE_ADTRG_AgentController    *mpHWCORE_ADTRGAgentController;    // HWCORE_ADTRG Agent controller pointer
    std::string     mModuleName;                           // Module name
    std::map<std::string, vpcl::re_register*> mRegisterMap;        // Map value of registers
     /* Bus master IF functions */
    void notifyTransComp(TlmTransactionType &trans, BusTime_t &t);
    // Function
    void Initialize(void);
    // Callback
    void cb_ADSFTTRG_SFTAD(RegCBstr str);
    void cb_ADMOFS2_DATA(RegCBstr str);
    void cb_ADBUFSEL_AD0BUFSEL(RegCBstr str);
    void cb_ADENDCTR_ADEND(RegCBstr str);
    void cb_ADMONC_CAMOUADMC(RegCBstr str);
    void cb_AD_DATA(RegCBstr str);
};
#endif // __HWCORE_ADTRG_FUNC_H__
