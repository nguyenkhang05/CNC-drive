// ---------------------------------------------------------------------
// $Id: IPG_DataHandlerLSU.cpp,v 1.3 2014/04/17 03:09:28 dungvannguyen Exp $
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
#include "BusMasterIf.h"
#include "IPG_DataHandlerLSU.h"

///Constructor of IPG_DataHandlerLSU class
IPG_DataHandlerLSU::IPG_DataHandlerLSU(std::string name,
                       BusBridgeModuleBase<32,32> *Parent,
                       BusBridgeSlaveIf<32> *SlaveIf,
                       BusMasterIf<32> *MasterIf,
                       IPG_AgentController *AgentController_ptr): 
                       BusBridgeCoreBase<32,32>(Parent,SlaveIf,MasterIf)
{//{{{
    assert((AgentController_ptr != NULL)&&(MasterIf != NULL)&&(SlaveIf != NULL));
    mIPG_AgentController = AgentController_ptr;
    mMasterIf = MasterIf;
    mSlaveIf = SlaveIf;
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void IPG_DataHandlerLSU::read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert((ExtensionInfo != NULL)&&(t != NULL));
    unsigned char vcid = ExtensionInfo->getVmId();
    bool is_continue_to_transfer = true;
    if (!debug) {//Normal transaction
        if (mIPG_AgentController->GetResetStatus()) {//lvcirreset is active
            is_continue_to_transfer = false;
            mIPG_AgentController->print_msg("error","The LSU reading transaction is fail because the reset is active\n");
        }
    }
    if (is_continue_to_transfer) {
        if ((mIPG_AgentController->CheckClockPeriod("pclk"))&&(mIPG_AgentController->CheckClockPeriod("lvcclk"))) {
            if (vcid == 0) {
                if (mIPG_AgentController->CheckAccessPermission(false, trans)) {
                    mMasterIf->read(trans,debug,*t,false);
                } else {
                    mIPG_AgentController->print_msg("info","The transaction is blocked\n");
                }
            } else {
                mMasterIf->read(trans,debug,*t,false);
                mIPG_AgentController->print_msg("info","The IPG protection function is not active with VICD is different from 0\n");
            }
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void IPG_DataHandlerLSU::write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug)
{//{{{
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert((ExtensionInfo != NULL)&&(t != NULL));
    unsigned char vcid = ExtensionInfo->getVmId();
    bool is_continue_to_transfer = true;
    if (!debug) {//Normal transaction
        if (mIPG_AgentController->GetResetStatus()) {//lvcirreset is active
            is_continue_to_transfer = false;
            mIPG_AgentController->print_msg("error","The LSU writing transaction is fail because the reset is active\n");
        }
    }
    if (is_continue_to_transfer) {
        if ((mIPG_AgentController->CheckClockPeriod("pclk"))&&(mIPG_AgentController->CheckClockPeriod("lvcclk"))) {
            if (vcid == 0) {
                if (mIPG_AgentController->CheckAccessPermission(false, trans)) {
                    mMasterIf->write(trans,debug,*t,false);
                } else {
                    mIPG_AgentController->print_msg("info","The transaction is blocked\n");
                }
            } else {
                mMasterIf->write(trans,debug,*t,false);
                mIPG_AgentController->print_msg("info","The IPG protection function is not active with VICD is different from 0\n");
            }
        }
    }
}//}}}

///Overwrite virtual functions of BusBridgeCoreBase class
void IPG_DataHandlerLSU::deleteTrans( void )
{//{{{
}//}}}
// vim600: set foldmethod=marker :
