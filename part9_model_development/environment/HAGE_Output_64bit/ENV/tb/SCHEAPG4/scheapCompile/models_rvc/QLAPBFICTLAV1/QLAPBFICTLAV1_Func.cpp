// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1_Func.cpp,v 1.3 2020/03/24 07:16:32 duongphan Exp $
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
#include "QLAPBFICTLAV1_Func.h"
#include "QLAPBFICTLAV1_Sub.h"
#include "QLAPBFICTLAV1_Sub_Func.h"

///Constructor of QLAPBFICTLAV1_Func class
QLAPBFICTLAV1_Func::QLAPBFICTLAV1_Func (std::string name,
        QLAPBFICTLAV1_AgentController *iQLAPBFICTLAV1_AgentController):
        BusSlaveFuncIf()

{//{{{
    assert(iQLAPBFICTLAV1_AgentController != NULL);
    mQLAPBFICTLAV1_AgentController = iQLAPBFICTLAV1_AgentController;
    mQLAPBFICTLAV1_AgentController->CommandInit((std::string)name);

}//}}}

///Destructor of QLAPBFICTLAV1_Sub class
QLAPBFICTLAV1_Func::~QLAPBFICTLAV1_Func(void)
{//{{{
}//}}}

///Virtual function of BusSlaveFuncIf class
void QLAPBFICTLAV1_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    assert(t != NULL);
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    unsigned int offset_size = ((unsigned int)trans.get_address() & 0x3);
    unsigned int ch_id = ((unsigned int)trans.get_address() & 0x3F) >> 2;

    if(ch_id >= mQLAPBFICTLAV1_AgentController->GetChId()) {
        mQLAPBFICTLAV1_AgentController->print_msg("error", "Invalid access address 0x%08X\n", (unsigned int)trans.get_address());
    }
    else if((offset_size+(unsigned int)trans.get_data_length())>4 && debug) {
        mQLAPBFICTLAV1_AgentController->print_msg("error", "Reading access size at address 0x%08X is wrong: %d byte(s).\n", (unsigned int)trans.get_address(),(unsigned int)trans.get_data_length());
    }
    else {
        mQLAPBFICTLAV1_AgentController->mQLAPBFICTLAV1_Sub[ch_id]->mQLAPBFICTLAV1_Sub_Func->read(offsetAddress, trans, t, debug);
    }
}//}}}

///Virtual function of BusSlaveFuncIf class

void QLAPBFICTLAV1_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    assert(t != NULL);
         unsigned int ch_id = ((unsigned int)trans.get_address() & 0x3F) >> 2;
         unsigned int offset_size = ((unsigned int)trans.get_address() & 0x3);
         if(ch_id >= mQLAPBFICTLAV1_AgentController->GetChId()) {
            mQLAPBFICTLAV1_AgentController->print_msg("error", "Invalid access address 0x%08X\n", (unsigned int)trans.get_address());
         }
         else if((offset_size+(unsigned int)trans.get_data_length())>4 && debug) {
           mQLAPBFICTLAV1_AgentController->print_msg("error", "Writing access size at address 0x%08X is wrong: %d byte(s).\n", (unsigned int)trans.get_address(),(unsigned int)trans.get_data_length());
          }
          else {
          mQLAPBFICTLAV1_AgentController->mQLAPBFICTLAV1_Sub[ch_id]->mQLAPBFICTLAV1_Sub_Func->write(offsetAddress, trans, t, debug);
         }

}//}}}
// vim600: set foldmethod=marker :
