// ---------------------------------------------------------------------
// $Id: HWCORE_MEASURE_Func.h,v 1.2 2020/09/01 10:06:12 phuongtran Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of HWCORE_MEASURE_Func.h
// Ref: {HWCORE_MEASURE_UD_Copyright_001}
#ifndef __HWCORE_MEASURE_FUNC_H__
#define __HWCORE_MEASURE_FUNC_H__
#include "hwcore_measure_regif.h"
#include "HWCORE_MEASURE_AgentController.h"

// Description: Declaration of HWCORE_MEASURE_Func class
// Ref: {HWCORE_MEASURE_UD_ATT1_004}
class HWCORE_MEASURE_Func : public Chwcore_measure_regif
{
public:
    SC_HAS_PROCESS(HWCORE_MEASURE_Func);
    HWCORE_MEASURE_Func(std::string name, HWCORE_MEASURE_AgentController* HWCORE_MEASUREAgentControllerPtr);
    ~HWCORE_MEASURE_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void DumpStatInfo(void);

    void read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug);
    void SetRegVal(std::string RegName, unsigned int val);
    unsigned int GetRegVal(std::string RegName);
    unsigned int GetRegVal(std::string RegName, std::string BitName);
    unsigned char GetCTRPreVal();

private:

    // Variables
    HWCORE_MEASURE_AgentController * mpHWCORE_MEASUREAgentController;    // HWCORE_MEASURE Agent controller pointer
    std::string     mModuleName;                           // Module name

    // Function
    void Initialize(void);
    // Callback
    void cb_PMT2CTR_EN(RegCBstr str);
    void cb_PMT2SFT_SCAPTRG(RegCBstr str);
    void cb_PMT2CNT_DATA(RegCBstr str);
    void cbDBG_PMT2CNT_DATA(RegCBstr str);
    void cb_PMT2INVL_DATA(RegCBstr str);

    std::map<std::string, vpcl::re_register*> mRegisterMap;        // Map value of registers
    unsigned char mPMT2CTR_pre_value;
};

#endif // __HWCORE_MEASURE_FUNC_H__
