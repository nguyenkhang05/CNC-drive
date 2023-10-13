//----------------------------------------------------------------------
//* File: DummyPeripheralRvc_Func.h
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of DummyPeripheralRvc_Func.h
// Ref: {SPIDCTL_UT_VRFCopyRight_001}
#ifndef __DummyPeripheralRvc_FUNC_H__
#define __DummyPeripheralRvc_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "dummyperipheralrvc_regif.h"
#include "DummyPeripheralRvc_Base.h"

class DummyPeripheralRvc;

class DummyPeripheralRvc_Func: public Cdummyperipheralrvc_regif, public BusSlaveFuncIf
{
public:
    DummyPeripheralRvc_Func(std::string name, DummyPeripheralRvc_Base *DummyPeripheralRvc_Base_ptr, DummyPeripheralRvc *parent);
    ~DummyPeripheralRvc_Func();
    
    //For slave - virtual functions in BusSlaveFuncIf
    void read(unsigned int offsetAddress, TlmBasicPayload &trans, sc_time *t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload &trans, sc_time *t, bool debug);


    void updateReg(std::string reg_name, unsigned int Index, unsigned int value);
    
    void cb_JUDGE_REG_JUDGE(RegCBstr str);
    void cb_RESET_REG_RESETPORT(RegCBstr str);
    void cb_PCLK_LOW_REG_CLK_LOW(RegCBstr str);
    
private:
    DummyPeripheralRvc_Base *mDummyPeripheralRvc_Base;
    unsigned int cur_CLK_LSB_L;
///    unsigned int cur_ERR_INFO_ADDR;
///    unsigned int cur_ERR_INFO_BMID;
///    unsigned int cur_ERR_INFO_OTHER;

};
#endif
