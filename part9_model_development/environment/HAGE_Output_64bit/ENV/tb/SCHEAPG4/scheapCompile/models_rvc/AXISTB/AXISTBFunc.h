// ---------------------------------------------------------------------
// $Id: AXISTBFunc.h,v 1.1 2014/03/14 09:47:02 duymai Exp $
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
#ifndef __AXISTBFunc_H__
#define __AXISTBFunc_H__
#include "BusSlaveFuncIf.h"
#include "AXISTB_AgentController.h"
#include "axistb_regif.h"
#include <queue>

class AXISTBFunc: public Caxistb_regif
                 ,public BusSlaveFuncIf
{
public:
    AXISTBFunc (std::string name, AXISTB_AgentController *AXISTBAgentControllerPtr);
    ~AXISTBFunc (void);

    void Reset (const std::string reset_name, const bool is_active);
    void DumpStatusInfo (void);
    void RegisterHandler (const std::vector<std::string> &args);
    bool CheckHaltStatus(const std::string side_name);
    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    AXISTB_AgentController *mAXISTBAgentController;
    std::string mModuleName;

    void cb_HDMACTL_GRISRQ(RegCBstr str);
};
#endif
