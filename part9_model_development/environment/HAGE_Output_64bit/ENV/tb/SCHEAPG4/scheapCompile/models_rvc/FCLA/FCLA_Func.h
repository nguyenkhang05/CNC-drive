// ---------------------------------------------------------------------
// $Id: FCLA_Func.h,v 1.1 2014/06/24 06:02:39 dungvannguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __FCLA_FUNC_H__
#define __FCLA_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "fcla_regif.h"

class FCLA_AgentController;

class FCLA_Func: public Cfcla_regif
                ,public BusSlaveFuncIf
{
public:
    FCLA_Func (std::string name,
               FCLA_AgentController *FCLA_AgentControllerPtr);
    ~FCLA_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);
    bool IsRisingDetectLevel (const unsigned int filter_no);
    bool IsFallingDetectLevel (const unsigned int filter_no);
    bool IsFilterBypassEnable (const unsigned int filter_no);
    bool IsLevelDetectorEnable (const unsigned int filter_no);
    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    FCLA_AgentController *mFCLA_AgentController;
    std::string mModuleName;//Module name
    //Declare virtual function of fcla_regif class
    void cb_FCLA0CTL_FCLA0INTR (RegCBstr str);
};
#endif
