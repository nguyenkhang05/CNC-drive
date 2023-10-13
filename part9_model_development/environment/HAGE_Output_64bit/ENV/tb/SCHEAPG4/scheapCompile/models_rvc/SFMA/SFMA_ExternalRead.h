// ---------------------------------------------------------------------
// $Id: SFMA_ExternalRead.h,v 1.2 2020/01/30 08:01:36 huepham Exp $
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
#ifndef __SFMA_EXTERNALREAD_H__
#define __SFMA_EXTERNALREAD_H__

#include "BusSlaveFuncIf.h"
#include "SFMA_AgentController.h"

class SFMA_ExternalRead: public BusSlaveFuncIf
{
public:
    SFMA_ExternalRead(std::string name, SFMA_AgentController* SFMAAgentController);
    virtual ~SFMA_ExternalRead();

    void EnableReset(const bool is_active);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Read Burst Enable
    enum eReadBurstEnable {
        emReadBurstDisable = 0
        ,emReadBurstEnable = 1
    };

    // Variables
    SFMA_AgentController    *mpSFMAAgentController;     // SFMA Agent controller pointer

    // Function
    void Initialize(void);
    void ExtAddrSpaceReadMode(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t);
    void BurstReadOperation(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t);
    void NormalReadOperation(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t);
};
#endif //__SFMA_EXTERNALREAD_H__
