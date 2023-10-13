// ---------------------------------------------------------------------
// $Id: SFMA_ExternalRead.cpp,v 1.4 2020/02/10 09:39:57 huepham Exp $
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
#include "global.h"
#include "SFMA_ExternalRead.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
SFMA_ExternalRead::SFMA_ExternalRead (std::string name, SFMA_AgentController* SFMAAgentController):
                                      BusSlaveFuncIf()
{//{{{
    assert(SFMAAgentController != NULL);
    mpSFMAAgentController = SFMAAgentController;

    Initialize();
}//}}}

/// Destructor
SFMA_ExternalRead::~SFMA_ExternalRead()
{//{{{
}//}}}

/// Reset ExternalRead
void SFMA_ExternalRead::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
}//}}}

/// Initialize internal variables
void SFMA_ExternalRead::Initialize(void)
{//{{{
}//}}}

/// Read API (target socket)
void SFMA_ExternalRead::read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (!(mpSFMAAgentController->CheckClockPeriod())
         || (true == mpSFMAAgentController->GetResetStatus())) {
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                mpSFMAAgentController->PrintMsg("warning", "External Read: Cannot access to flash memory when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            mpSFMAAgentController->PrintMsg("warning", "External Read: Cannot access to flash memory when clock is 0 or in reset state.\n");
            return;
        }
    }

    ExtAddrSpaceReadMode((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length(), t);
}//}}}

/// Write API (target socket)
void SFMA_ExternalRead::write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
}//}}}

/// External address space read mode
void SFMA_ExternalRead::ExtAddrSpaceReadMode(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t)
{//{{{
    // Wrong size
    if(0 == size) {
        mpSFMAAgentController->PrintMsg("error", "External Read: Cannot read flash memory with size = 0\n");
        return;
    } else if(0 != (address % size)) {
        std::stringstream stream;
        stream << "External Read: Invalid access address 0x" << std::hex << address << " with access size " << std::dec << size << " bytes\n";

        mpSFMAAgentController->PrintMsg("error", stream.str().c_str());
        return;
    }

    // External address space read mode
    if(false == mpSFMAAgentController->IsExtReadSettingProhibited()) {
        // Burst read
        assert(pData != NULL);
        assert(t != NULL);
        if(emReadBurstEnable == mpSFMAAgentController->GetBitField("SFMADRCR", "RBE")){
            BurstReadOperation(address, pData, size, t);
        } else { // Normal read
            NormalReadOperation(address, pData, size, t);
        }
    }
}//}}}

/// Burst Read Operation
void SFMA_ExternalRead::BurstReadOperation(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t)
{//{{{
    // Access wrong size
    if ((1 != size) && (2 != size) && (4 != size) && (8 != size)) {
        std::stringstream stream;
        stream << "Burst read: Access wrong size: " << std::dec << size << " bytes\n";

        mpSFMAAgentController->PrintMsg("error", stream.str().c_str());
        return;
    }

    assert(pData != NULL);
    assert(t != NULL);
    mpSFMAAgentController->BurstReadCommunicate(address, pData, size, t);
}//}}}

/// Normal Read Operation
void SFMA_ExternalRead::NormalReadOperation(unsigned int address, unsigned char *pData, unsigned int size, BusTime_t* t)
{//{{{
    // Access wrong size
    if ((1 != size) && (2 != size) && (4 != size)) {
        std::stringstream stream;
        stream << "Normal read: Access wrong size: " << std::dec << size << " bytes\n";

        mpSFMAAgentController->PrintMsg("error", stream.str().c_str());
        return;
    }

    assert(pData != NULL);
    assert(t != NULL);
    mpSFMAAgentController->NormalReadCommunicate(address, pData, size, t);
}//}}}
// vim600: set foldmethod=marker :
