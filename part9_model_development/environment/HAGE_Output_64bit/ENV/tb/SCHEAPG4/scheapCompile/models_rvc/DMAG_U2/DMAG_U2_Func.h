// ---------------------------------------------------------------------
// $Id: DMAG_U2_Func.h,v 1.4 2020/02/14 06:25:20 hadinhtran Exp $
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
#ifndef __DMAG_U2_FUNC_H__
#define __DMAG_U2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "dmag_u2_regif.h"
#include "global.h"

class DMAG_U2_AgentController;
class DMAG_U2_Func: public Cdmag_u2_regif,
                public BusSlaveFuncIf
{
public:
    DMAG_U2_Func (std::string name, DMAG_U2_AgentController *DMAG_U2_AgentControllerPtr,  unsigned int ch_num);
    ~DMAG_U2_Func(void);

    void EnableReset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    void DMAG_U2_P_RESET_VALUE0_m(unsigned int value, unsigned int index);
    void DMAG_U2_P_RESET_VALUE1_n(unsigned int value, unsigned int index);

    void SetVar(const std::string name, const unsigned int val);
    void SetOverFlowBit();
    bool IsTheFirstERR(void);
    bool IsTheSameSPID(unsigned int spid_err);
    unsigned int GetPermittedSPID(unsigned char channel_id);
    void GetDMAGPROT_n(unsigned char channel_id, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg);

private:
    DMAG_U2_AgentController *mDMAG_U2_AgentController;
    std::string mModuleName;        //Module name

    /// Declare virtual functions of Cdmag_u2_regif class
    void cb_DMAGOVFCLR_CLRO(RegCBstr str);
    void cb_DMAGSPIDERRCLR_SPIDCLR(RegCBstr str);
    void cb_DMAGKCPROT_KCPROT(RegCBstr str);
    void cb_DMAGMPID_SPID(RegCBstr str);
    void cb_DMAGPROT_GR_GEN(RegCBstr str);
    void cb_DMAGPROT_DP_GEN(RegCBstr str);
    void cb_DMAGPROT__GEN(RegCBstr str);

    enum eDMAGPROT_BitPosition {
        emNum_BitPosition_Base_MPID = 16,
        emNUM_BitPosition_GEN = 0x8,
        emNUM_BitPosition_DBG = 0x6,
        emNUM_BitPosition_UM = 0x4,
        emNUM_BitPosition_WG = 0x1,
        emNUM_BitPosition_RG = 0x0
    };
    enum eMaxChanReg {
      emNUM_MAX_DMAGMPID = 8,
      emNUM_MAX_DMAGPROT = 18
    };
    enum e5BitNumValue {
        emNum_5BitMax = 31,
        emNum_5BitMin = 0
    };
};
#endif //__DMAG_U2_FUNC_H__

