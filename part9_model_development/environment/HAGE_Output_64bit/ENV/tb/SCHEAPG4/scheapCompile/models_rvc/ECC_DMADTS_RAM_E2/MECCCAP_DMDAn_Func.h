// ---------------------------------------------------------------------
// $Id: MECCCAP_DMDAn_Func.h,v 1.4 2019/09/06 08:49:03 landang Exp $
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
#ifndef __MECCCAP_DMDAN_FUNC_H__
#define __MECCCAP_DMDAN_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "mecccap_dmdan_regif.h"
#include "ECC_DMADTS_RAM_E2_AgentController.h"
#include "global.h"

class MECCCAP_DMDAn_Func: public Cmecccap_dmdan_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (MECCCAP_DMDAn_Func);
    MECCCAP_DMDAn_Func(std::string name, unsigned int sub_id, ECC_DMADTS_RAM_E2_AgentController* DMADTS_RAMAgentController);
    ~MECCCAP_DMDAn_Func();

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
    void SetFatalAddress(unsigned int edl_code, unsigned int mod_address);
    unsigned int GetDA_SERSTR();
    unsigned int GetDA_DERSTR();
    unsigned int GetDA_OVFSTR();
    void WriteERRINT_INIT(unsigned int value);
    unsigned int GetERRINT();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    ECC_DMADTS_RAM_E2_AgentController   *mpDMADTS_RAMAgentController;    // KCPROT Agent controller pointer
    std::string     mModuleName;// Module name
    unsigned int    mSubId;      // PE index
    std::map<unsigned int, std::string> mProtectedRegMap;   // Protected registers map   

    // Callback
    void cb_DA_SSTCLR_SSTCLR00(RegCBstr str);
    void cb_DA_DSTCLR_DSTCLR00(RegCBstr str);
    void cb_DA_OVFCLR_SERROVFCLR0(RegCBstr str);

};
#endif //__MECCCAP_DMDAN_FUNC_H__
