// ---------------------------------------------------------------------
// $Id: PEG_DataHandler.cpp,v 1.3 2014/04/18 06:47:39 duymai Exp $
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

#include <cstdarg>
#include "PEG_DataHandler.h"
#include "BusMasterIf.h"

///Constructor of PEG_DataHandler class
PEG_DataHandler::PEG_DataHandler(std::string name,
                         BusBridgeModuleBase<32,32> *Parent,
                         BusBridgeSlaveIf<32> *SlaveIf,                       
                         BusMasterIf<32> *MasterIf, 
                         PEG_AgentController *AgentController_ptr): 
                         BusBridgeCoreBase<32,32>(Parent,SlaveIf,MasterIf)
{//{{{
    assert((AgentController_ptr != NULL) && (MasterIf != NULL) && (SlaveIf != NULL));
    mAgentController = AgentController_ptr;
    mAgentController->CommandInit(name);
    mMasterIf = MasterIf;
    mSlaveIf = SlaveIf;
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void PEG_DataHandler::read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    assert(t != NULL);
    if ((mAgentController->CheckClock("pclk"))&&(mAgentController->CheckClock("lvcclk"))) {
        if ((mAgentController->GetResetStatus() == false) || (debug == true)) {
            if (mAgentController->CheckAccessPermission(trans)) {
                mMasterIf->read(trans,debug,*t,false);
            }
        } else {
            mAgentController->print_msg("error","The reading transaction is fail because the reset is active\n");
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void PEG_DataHandler::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    assert(t != NULL);
    if ((mAgentController->CheckClock("pclk"))&&(mAgentController->CheckClock("lvcclk"))) {
        if ((mAgentController->GetResetStatus() == false) || (debug == true)) {
            if (mAgentController->CheckAccessPermission(trans)) {
                mMasterIf->write(trans,debug,*t,false);
            }
        } else {
            mAgentController->print_msg("error","The writing transaction is fail because the reset is active\n");
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void PEG_DataHandler::deleteTrans( void )
{//{{{
}//}}}
// vim600: set foldmethod=marker :
