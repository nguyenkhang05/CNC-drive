// ---------------------------------------------------------------------
// $Id: SFMA_AgentController.h,v 1.3 2020/01/30 08:01:36 huepham Exp $
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
#ifndef __SFMA_AGENT_CONTROLLER_H__
#define __SFMA_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "BusSlaveBase.h"

// Class SFMA_AgentController
class SFMA_AgentController
{
#include "SFMA_cmdif.h"
public:
    // Constructor/Destructor
    SFMA_AgentController(void) {;}
    virtual ~SFMA_AgentController(void) {;}

    // Interface (APIs)
    virtual bool CheckClockPeriod(void) = 0;
    virtual bool GetResetStatus(void) = 0;
    virtual unsigned int GetBitField(const std::string regName, const std::string bitName) = 0;
    virtual void SPIModeCommunicate(void) = 0;
    virtual void FlushCache(void) = 0;
    virtual void NormalReadCommunicate(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t) = 0;
    virtual void BurstReadCommunicate(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t) = 0;
    virtual bool IsExtReadSettingProhibited(void) = 0;
    virtual bool IsSPIModeSettingProhibited(void) = 0;
    virtual void TransferEnd(void) = 0;
    virtual void PrintMsg(const char* severity, const char* msg) = 0;
};
#endif //__SFMA_AGENT_CONTROLLER_H__
