// ---------------------------------------------------------------------
// $Id: PIC1_E2_Func.h 3395 2017-06-29 08:02:34Z ChinhTD $
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
#ifndef __PIC1_E2_FUNC_H__
#define __PIC1_E2_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "pic1_e2_regif.h"
#include "PIC1_E2_AgentController.h"

class PIC1_E2_Func: public Cpic1_e2_regif
                 ,public BusSlaveFuncIf
{
public:
    PIC1_E2_Func (std::string name, 
                PIC1_E2_AgentController *PIC1AgentPtr);
    ~PIC1_E2_Func (void);

    void RegisterHandler (const std::vector<std::string>& args);
    void EnableReset (const bool is_active);

    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:

    PIC1_E2_AgentController    *mPIC1Agent;
    std::string             mModuleName;
    //Declare virtual function of piccom_regif class
    void cb_PIC1SST_SYNCTRG(RegCBstr str);
};
#endif
