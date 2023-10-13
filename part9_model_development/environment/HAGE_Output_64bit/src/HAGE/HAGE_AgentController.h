/*************************************************************************
*
*  HAGE_AgentController.h
*
* Copyright(c) 2018-2021 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
*************************************************************************/

#ifndef HAGE_AGENTCONTROLLER_H
#define HAGE_AGENTCONTROLLER_H

#include "systemc.h"
#include "global.h"
#include "rvc_common_model.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"
#include "BusSlaveFuncIf.h"

class HAGE_AgentController : public rvc_common_model
{
public:
    HAGE_AgentController(std::string name) :
        rvc_common_model(name)
    {
    };
    virtual ~HAGE_AgentController(void) {};
    virtual void StartDMA() = 0;
    virtual void StopDMA() = 0;
private:

};

#endif /* HAGE_AGENTCONTROLLER_H */
