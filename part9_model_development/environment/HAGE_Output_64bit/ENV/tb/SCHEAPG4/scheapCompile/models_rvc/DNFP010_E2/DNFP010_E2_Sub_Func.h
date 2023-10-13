// ---------------------------------------------------------------------
// $Id: DNFP010_E2_Sub_Func.h 5262 2017-03-08 02:30:35Z ChinhTD $
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
#ifndef __DNFP010_E2_SUB_FUNC_H__
#define __DNFP010_E2_SUB_FUNC_H__
#include "dnfp010_e2_sub_regif.h"
#include "OSCI2.h"

class DNFP010_E2_Sub_AgentController;
//#include "DNFP010_E2_Sub_AgentController.h"

class DNFP010_E2_Sub_Func: public Cdnfp010_e2_sub_regif
{
public:
    DNFP010_E2_Sub_Func (std::string name, DNFP010_E2_Sub_AgentController *DNFP010_E2_Sub_AgentControllerPtr);
    ~DNFP010_E2_Sub_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    /// Public functions
    bool GetNFEN(void);
    unsigned int GetSLST(void);
    unsigned int GetPRS(void);
private:

    DNFP010_E2_Sub_AgentController *mDNFP010_E2_Sub_AgentController;
    std::string mModuleName;        //Module name

    void cb_DNFP01CTL_SLST(RegCBstr str);
    void cb_DNFP01CTL_PRS(RegCBstr str);
};

#endif
