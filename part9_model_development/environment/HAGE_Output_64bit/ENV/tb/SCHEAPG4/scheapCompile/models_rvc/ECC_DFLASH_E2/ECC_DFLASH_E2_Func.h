// ---------------------------------------------------------------------
// $Id: ECC_DFLASH_E2_Func.h,v 1.4 2019/09/06 08:48:49 landang Exp $
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_DFLASH_E2_FUNC_H__
#define __ECC_DFLASH_E2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "ecc_dflash_e2_regif.h"
#include "ECC_DFLASH_E2_AgentController.h"
#include "global.h"

class ECC_DFLASH_E2_Func: public Cecc_dflash_e2_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (ECC_DFLASH_E2_Func);
    ECC_DFLASH_E2_Func(std::string name, ECC_DFLASH_E2_AgentController* ECC_DFLASH_E2AgentController);
    ~ECC_DFLASH_E2_Func();

    void EnableReset(const bool is_active);
    void SetSEDIEInitVal(const bool is_one);
    void IsAddrCaptureSupport(const bool is_support);
    void RegisterHandler(const std::vector<std::string> &args);
    void ErrorProcess(const unsigned int error_type, const unsigned int error_addr);
    void GetPAWIDTHValue(const unsigned int pawidth);
    void DumpStatusInfo (void);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    // APIs

private:
    enum eOutputPort {
        emEccDisable = 0,
        emSecDisable = 1,
        emEccSed     = 2,
        emEccDed     = 3,
        emErrOvf     = 4
    };

    enum eSecurity {
        emKeyCode = 0xA5A5A500
    };

    // Variables
    ECC_DFLASH_E2_AgentController   *mpECC_DFLASH_E2AgentController;    // ECC_DFLASH_E2 Agent controller pointer
    std::string mModuleName;    // Module name
    bool mEnableWrProtReg; // Unlocks protection register
    unsigned int mPAWIDTH;
    bool mIsAddrCaptureSupport;

    // Functions
    void Initialize();

    // Callback
    void cb_DFECCCTL_ECCDIS(RegCBstr str);
    void cb_DFERSTC_ERRCLR(RegCBstr str);
    void cb_DFOVFSTC_ERROVFCLR(RegCBstr str);
    void cb_DFKCPROT_KCE(RegCBstr str);
    void cb_DFERRINT_EOVFIE(RegCBstr str);

};
#endif //__ECC_DFLASH_E2_FUNC_H__
