// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERC_Func.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_TIMERC_FUNC_H__
#define __ATU5010_TIMERC_FUNC_H__
#include "atu5010_timerc_regif.h"
#include "ATU5010_TIMERC_AgentController.h"
#include "ATU5010_TIMERC_SUB.h"
#include "ATU5010_TIMERC_SUB_Func.h"

class ATU5010_TIMERC_Func: public Catu5010_timerc_regif
{
public:
    ATU5010_TIMERC_Func (std::string name, 
                ATU5010_TIMERC_AgentController *TIMERCAgentPtr);
    ~ATU5010_TIMERC_Func (void);

    void EnableReset (const bool is_active);
    bool CheckSubblock(const unsigned int sub);
    unsigned int GetNoiseCLKSel(const unsigned int sub_id,const unsigned int channel);
    unsigned int GetNoiseCancelMode(const unsigned int sub_id);
    void RegisterHandler(const bool is_common, const unsigned int sub_id ,const std::vector<std::string>& arg);
    bool IsNoiseCancelEn(const unsigned int sub_id, const unsigned int channel);

    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    ATU5010_TIMERC_AgentController    *mTIMERCAgent;
    std::string             mModuleName;

    bool    mTimeResolutionUnit;
    void cb_TSTRC_STRC(RegCBstr str);
    void cb_NCMCR1C_NCM1C(RegCBstr str);
    void cb_NCMCR2C_NCM2C(RegCBstr str);
    void cb_NCCRC_NCEC3(RegCBstr str);
};
#endif
