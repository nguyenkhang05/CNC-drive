// ---------------------------------------------------------------------
// $Id: APBDataHandler_UIMPL.cpp,v 1.1 2013/11/26 03:42:43 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "APBDataHandler_UIMPL.h"

///Constructor of APBDataHandler_UIMPL class
APBDataHandler_UIMPL::APBDataHandler_UIMPL (std::string name, APBAgentController_UIMPL *AgentControllerPtr): 
                      BusSlaveFuncIf()
{//{{{
    assert(AgentControllerPtr != NULL);
    mAgentController = AgentControllerPtr;
    mAgentController->CommandInit(name);
}//}}}

///Access read to registers
void APBDataHandler_UIMPL::read (unsigned int offsetAddress, 
                                 TlmBasicPayload& trans, 
                                 BusTime_t* t, 
                                 bool debug)
{//{{{
    assert(t != NULL);
    if (mAgentController->GetResetStatus() == false) {
        if (mAgentController->CheckClockPeriod()) {
            mAgentController->print_msg("info","Receive the transaction from APB bus\n");
            mAgentController->TransferTransInfo(trans);
        }
    } else {
        mAgentController->print_msg("error","The reading transaction is fail because the reset is active\n");
    }
}//}}}

///Access write to registers
void APBDataHandler_UIMPL::write (unsigned int offsetAddress, 
                                  TlmBasicPayload& trans, 
                                  BusTime_t* t, 
                                  bool debug)
{//{{{
    assert(t != NULL);
    if (mAgentController->GetResetStatus() == false) {
        if (mAgentController->CheckClockPeriod()) {
            mAgentController->print_msg("info","Receive the transaction from APB bus\n");
            mAgentController->TransferTransInfo(trans);
        }
    } else {
        mAgentController->print_msg("error","The writing transaction is fail because the reset is active\n");
    }
}//}}}
// vim600: set foldmethod=marker :
