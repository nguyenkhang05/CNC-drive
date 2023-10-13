// ---------------------------------------------------------------------
// $Id: DNFP020_E2_Sub_Func.h 2381 2017-03-15 09:37:13Z ChinhTD $
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
#ifndef __DNFP020_E2_SUB_FUNC_H__
#define __DNFP020_E2_SUB_FUNC_H__
#include "dnfp020_e2_sub_regif.h"
#include "OSCI2.h"

class DNFP020_E2_Sub_AgentController;
//#include "DNFP020_E2_Sub_AgentController.h"

class DNFP020_E2_Sub_Func: public Cdnfp020_e2_sub_regif
{
public:
    DNFP020_E2_Sub_Func (std::string name, DNFP020_E2_Sub_AgentController *DNFP020_E2_Sub_AgentControllerPtr);
    ~DNFP020_E2_Sub_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    /// Public functions
    bool GetNFEN(void);
    void SetEDF(void);
    unsigned int GetDMD(void);
private:

    DNFP020_E2_Sub_AgentController *mDNFP020_E2_Sub_AgentController;
    std::string mModuleName;        //Module name

    void cb_DNFP02EDF_CLED(RegCBstr str);
    void cb_DNFP02EDC_NFEN(RegCBstr str);

};

#endif
