// ---------------------------------------------------------------------
// $Id: AHBDataHandler_UIMPL.cpp,v 1.1 2013/11/26 03:53:26 dungvannguyen Exp $
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
#include "AHBDataHandler_UIMPL.h"

///Constructor of AHBDataHandler_UIMPL class
AHBDataHandler_UIMPL::AHBDataHandler_UIMPL (std::string name, AHBAgentController_UIMPL *AgentControllerPtr): 
                      BusSlaveFuncIf()
{//{{{
    assert(AgentControllerPtr != NULL);
    mAgentController = AgentControllerPtr;
    mAgentController->CommandInit(name);
}//}}}

///Access read to registers
void AHBDataHandler_UIMPL::read (unsigned int offsetAddress, 
                                 TlmBasicPayload& trans, 
                                 BusTime_t* t, 
                                 bool debug)
{//{{{
    assert(t != NULL);
    if (mAgentController->GetResetStatus("HRESETZ") == false) {
        if ((mAgentController->CheckClockPeriod("PCLK"))&&(mAgentController->CheckClockPeriod("HCLK"))) {
            mAgentController->print_msg("info","Receive the transaction from AHB bus\n");
            mAgentController->TransferTransInfo(trans);
        }
    } else {
        mAgentController->print_msg("error","The reading transaction is fail because the reset is active\n");
    }
}//}}}

///Access write to registers
void AHBDataHandler_UIMPL::write (unsigned int offsetAddress, 
                                  TlmBasicPayload& trans, 
                                  BusTime_t* t, 
                                  bool debug)
{//{{{
    assert(t != NULL);
    if (mAgentController->GetResetStatus("HRESETZ") == false) {
        if ((mAgentController->CheckClockPeriod("PCLK"))&&(mAgentController->CheckClockPeriod("HCLK"))) {
            mAgentController->print_msg("info","Receive the transaction from AHB bus\n");
            mAgentController->TransferTransInfo(trans);
        }
    } else {
        mAgentController->print_msg("error","The writing transaction is fail because the reset is active\n");
    }
}//}}}
// vim600: set foldmethod=marker :
