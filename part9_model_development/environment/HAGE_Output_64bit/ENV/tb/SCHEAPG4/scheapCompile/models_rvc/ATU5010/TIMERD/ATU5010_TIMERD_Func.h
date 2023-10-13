// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERD_Func.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#ifndef __ATU5010_TIMERD_FUNC_H__
#define __ATU5010_TIMERD_FUNC_H__
#include "atu5010_timerd_regif.h"
#include "ATU5010_TIMERD_AgentController.h"

class ATU5010_TIMERD_Func: public Catu5010_timerd_regif
{
public:
    ATU5010_TIMERD_Func (std::string name, 
                ATU5010_TIMERD_AgentController *TIMERDAgentPtr);
    ~ATU5010_TIMERD_Func (void);

    void EnableReset (const bool is_active);

    //Virtual functions 
    void RegisterHandler(const bool is_common, const unsigned int sub_id ,const std::vector<std::string>& arg);

    //Virtual functions 
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void TimerDStatus(const bool is_active);
    bool mTimerDStatus;
private:
    ATU5010_TIMERD_AgentController    *mTIMERDAgent;
    std::string             mModuleName;

    void cb_TSTRD_STRD0(RegCBstr str);
    
public:
    bool Get_CCAPSELD_CCAPSEL(void);
    void Set_CICRD_CICRD(unsigned int index, unsigned int value);

};
#endif
