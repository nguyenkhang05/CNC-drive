// ---------------------------------------------------------------------
// $Id: MECCCAP_IT_PEn_FCC2_Func.h,v 1.5 2019/09/06 08:49:26 landang Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __MECCCAP_IT_PEN_FCC2_FUNC_H__
#define __MECCCAP_IT_PEN_FCC2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "mecccap_it_pen_fcc2_regif.h"
#include "ECC_ICACHE_E2_FCC2_AgentController.h"
#include "global.h"

class MECCCAP_IT_PEn_FCC2_Func: public Cmecccap_it_pen_fcc2_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (MECCCAP_IT_PEn_FCC2_Func);
    MECCCAP_IT_PEn_FCC2_Func(std::string name, unsigned int pe_id, ECC_ICACHE_E2_FCC2_AgentController* ICACHEAgentController);
    ~MECCCAP_IT_PEn_FCC2_Func();

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
    bool CheckNoFatalStatus();
    void Set2bitErrorStatus();
    void SetAddrFeedbackErrorStatus();
    void SetFatalAddress(unsigned int edl_code, unsigned int mod_address);
    unsigned int GetIT_SERSTR();
    unsigned int GetIT_DERSTR();
    unsigned int GetIT_OVFSTR();
    void WriteERRINT_INIT(unsigned int value);
    unsigned int GetERRINT();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    ECC_ICACHE_E2_FCC2_AgentController   *mpICACHEAgentController;    // KCPROT Agent controller pointer
    std::string     mModuleName;// Module name
    unsigned int    mPeId;      // PE index
    std::map<unsigned int, std::string> mProtectedRegMap;   // Protected registers map   

    // Callback
    void cb_IT_SSTCLR_IT_SSTCLR(RegCBstr str);
    void cb_IT_DSTCLR_DSTCLR00(RegCBstr str);
    void cb_IT_OVFCLR_SERROVFCLR0(RegCBstr str);

};
#endif //__MECCCAP_IT_PEN_FCC2_FUNC_H__
