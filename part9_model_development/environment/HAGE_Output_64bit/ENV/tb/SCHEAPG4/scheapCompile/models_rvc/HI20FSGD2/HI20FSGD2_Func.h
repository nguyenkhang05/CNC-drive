// ---------------------------------------------------------------------
// $Id: HI20FSGD2_Func.h,v 1.4 2019/12/06 07:19:03 hadinhtran Exp $
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
#ifndef __HI20FSGD2_FUNC_H__
#define __HI20FSGD2_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "hi20fsgd2_regif.h"
#include "global.h"

class HI20FSGD2_AgentController;
class HI20FSGD2_Func: public Chi20fsgd2_regif,
                      public BusSlaveFuncIf
{
public:
    HI20FSGD2_Func (std::string name, HI20FSGD2_AgentController *HI20FSGD2_AgentControllerPtr, unsigned int ch_num);
    ~HI20FSGD2_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void SetInitValReg (const std::string reg_name, const unsigned int ch_id, unsigned int value);

    bool         mSecurity;

    enum ePBGPROT0InitVal{
        emPROT0InitVal0     = 0x00000141,
        emPROT0InitVal1     = 0x000001C1,
        emPROT0InitVal2     = 0x00000000
    };
    
    enum ePBGPROT1InitVal{
        emPROT1InitVal0     = 0x0000000F,
        emPROT1InitVal1     = 0x0400000F
    };

private:
    HI20FSGD2_AgentController *mHI20FSGD2_AgentController;
    std::string mModuleName;        //Module name
    
    /// Function
    void Initialize(void);
    
    /// Declare virtual functions of hi20fsgd2_regif class
    void cb_PBGPROT0_LOCK(RegCBstr str);
    void cb_PBGPROT1_SPID(RegCBstr str);
};
#endif //__HI20FSGD2_FUNC_H__

