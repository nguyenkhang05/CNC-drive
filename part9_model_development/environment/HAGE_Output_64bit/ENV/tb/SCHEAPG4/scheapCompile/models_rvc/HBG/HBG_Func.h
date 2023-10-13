// ---------------------------------------------------------------------
// $Id: HBG_Func.h,v 1.8 2020/02/18 07:24:22 huyquocpham Exp $
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
#ifndef __HBG_FUNC_H__
#define __HBG_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "hbg_regif.h"
#include "global.h"

class HBG_AgentController;
class HBGERRSLV_Func;
class HBG_Func: public Chbg_regif,
    public BusSlaveFuncIf
{
public:
    HBG_Func (std::string name, HBG_AgentController *HBG_AgentControllerPtr);
    ~HBG_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);
    void SetInitHBGPROT0(const unsigned int init_val);
    void SetInitHBGPROT1(const unsigned int init_val);
    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    HBG_AgentController *mHBG_AgentController;
    std::string mModuleName;        //Module name

    /// Declare virtual functions of HBG_regif class
    void cb_HBGPROT0_LOCK(RegCBstr str);
    void cb_HBGPROT1_SPID(RegCBstr str);


};
#endif //__HBG_FUNC_H__

