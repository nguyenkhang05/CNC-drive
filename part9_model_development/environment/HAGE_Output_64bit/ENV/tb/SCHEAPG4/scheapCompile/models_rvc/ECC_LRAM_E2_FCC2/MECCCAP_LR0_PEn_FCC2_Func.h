// ---------------------------------------------------------------------
// $Id: MECCCAP_LR0_PEn_FCC2_Func.h,v 1.5 2019/09/06 08:49:32 landang Exp $
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
#ifndef __MECCCAP_LR0_PEN_FCC2_FUNC_H__
#define __MECCCAP_LR0_PEN_FCC2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "mecccap_lr0_pen_fcc2_regif.h"
#include "ECC_LRAM_E2_FCC2_AgentController.h"
#include "global.h"

class MECCCAP_LR0_PEn_FCC2_Func: public Cmecccap_lr0_pen_fcc2_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (MECCCAP_LR0_PEn_FCC2_Func);
    MECCCAP_LR0_PEn_FCC2_Func(std::string name, unsigned int pe_id, ECC_LRAM_E2_FCC2_AgentController* LRAMAgentController);
    ~MECCCAP_LR0_PEn_FCC2_Func();

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
    void SetAddrFeedbackErrorStatus();
    void SetFatalAddress(unsigned int edl_code, unsigned int mod_address);
    unsigned int GetLR0_SERSTR();
    unsigned int GetLR0_DERSTR();
    unsigned int GetLR0_OVFSTR();
    void WriteERRINT_INIT(unsigned int value);
    unsigned int GetERRINT();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    ECC_LRAM_E2_FCC2_AgentController   *mpLRAMAgentController;    // KCPROT Agent controller pointer
    std::string     mModuleName;// Module name
    unsigned int    mPeId;      // PE index
    std::map<unsigned int, std::string> mProtectedRegMap;   // Protected registers map   

    // Callback
    void cb_LR0_SSTCLR_LR0_SSTCLR(RegCBstr str);
    void cb_LR0_DSTCLR_DSTCLR00(RegCBstr str);
    void cb_LR0_OVFCLR_SERROVFCLR0(RegCBstr str);
    void cb_LR0_ERMSA0_ERMSA(RegCBstr str);
    void cb_LR0_ERMEA0_ERMEA(RegCBstr str);
    void cb_LR0_ERMSA1_ERMSA(RegCBstr str);
    void cb_LR0_ERMEA1_ERMEA(RegCBstr str);

};
#endif //__MECCCAP_LR0_PEN_FCC2_FUNC_H__
