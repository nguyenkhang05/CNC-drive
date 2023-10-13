// ---------------------------------------------------------------------
// $Id: ATU5_TIMERE_Func.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERE_FUNC_H__
#define __ATU5_TIMERE_FUNC_H__
#include "atu5_timere_regif.h"
#include "ATU5_TIMERE_AgentController.h"


class ATU5_TIMERE_Func: public Catu5_timere_regif
{
friend class ATU5_TIMERE;
public:
    ATU5_TIMERE_Func (std::string name, 
                ATU5_TIMERE_AgentController *TIMEREAgentPtr, unsigned int subblock);
    ~ATU5_TIMERE_Func (void);

    
    void TimerEStatus(const bool is_active);
    void RegisterHandler(const bool is_common, const unsigned int sub_id ,const std::vector<std::string>& arg);

    //Virtual functions
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void CheckStartCondition(void);
private:
    unsigned int msbE; // Number of subblocks of Timer E
    ATU5_TIMERE_AgentController    *mTIMEREAgent;
    std::string             mModuleName;

    bool    mTimerEStatus;
    
    void cb_TSTRE_STRE(RegCBstr str);
    void cb_SBRLENE_SBRLDENE(RegCBstr str);
    void EnableReset (const bool is_active);
};
#endif
