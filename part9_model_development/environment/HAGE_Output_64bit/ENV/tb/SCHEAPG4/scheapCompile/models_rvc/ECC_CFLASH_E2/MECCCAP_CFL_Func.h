// ---------------------------------------------------------------------
// $Id: MECCCAP_CFL_Func.h,v 1.1 2019/09/26 10:06:58 landang Exp $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __MECCCAP_CFL_FUNC_H__
#define __MECCCAP_CFL_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "mecccap_cfl_regif.h"
#include "ECC_CFLASH_E2_AgentController.h"
#include "global.h"

class MECCCAP_CFL_Func: public Cmecccap_cfl_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (MECCCAP_CFL_Func);
    MECCCAP_CFL_Func(std::string name, ECC_CFLASH_E2_AgentController* CFLASHAgentController);
    ~MECCCAP_CFL_Func();

    void EnableReset(const std::string reset_name, const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    bool Set1bitStatusAndAddress(unsigned int edl_code, unsigned int mod_address);
    void SetFlagAddressBufferOverflow();
    bool CheckHasRoomFor1bit();
    void SetFIFOOverflow ();
    unsigned int GetSEDLBit(unsigned int index);
    unsigned int GetSEADRBit(unsigned int index);
    unsigned int GetDEDLBit();
    unsigned int GetDEADRBit();
    unsigned int GetERMSA(unsigned int index);
    unsigned int GetERMEA(unsigned int index);
    unsigned int GetERM_EN(unsigned int index);
    bool CheckNoFatalStatus();
    void Set2bitErrorStatus();
    void SetAddrParityErrorStatus();
    void SetFatalAddress(unsigned int edl_code, unsigned int mod_address);
    unsigned int GetCF_SERSTR();
    unsigned int GetCF_DERSTR();
    unsigned int GetCF_OVFSTR();
    void WriteERRINT_INIT(unsigned int value);
    unsigned int GetERRINT();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    ECC_CFLASH_E2_AgentController   *mpCFLASHAgentController;    // KCPROT Agent controller pointer
    std::string     mModuleName;// Module name
    std::map<unsigned int, std::string> mProtectedRegMap;   // Protected registers map   

    // Callback
    void cb_CF_SSTCLR_CF_SSTCLR(RegCBstr str);
    void cb_CF_DSTCLR_DSTCLR00(RegCBstr str);
    void cb_CF_OVFCLR_SERROVFCLR0(RegCBstr str);
    void cb_CF_ERMSA0_ERMSA(RegCBstr str);
    void cb_CF_ERMEA0_ERMEA(RegCBstr str);
    void cb_CF_ERMSA1_ERMSA(RegCBstr str);
    void cb_CF_ERMEA1_ERMEA(RegCBstr str);

};
#endif //__MECCCAP_CFL_FUNC_H__
