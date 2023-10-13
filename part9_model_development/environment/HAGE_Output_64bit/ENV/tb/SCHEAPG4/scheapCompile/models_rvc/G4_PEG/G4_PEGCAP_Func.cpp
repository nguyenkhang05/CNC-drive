// ---------------------------------------------------------------------
// $Id: G4_PEGCAP_Func.cpp,v 1.11 2020/01/14 02:39:30 nhutminhdang Exp $
//
// Copyright(c) 2016-2020 Renesas Electronics Corporation
// Copyright(c) 2016-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "G4_PEGCAP_Func.h"
#include "G4_PEGCAP_E2x_Reg.h"
#include "G4_PEGCAP_U2A_Reg.h"
#include "G4_PEGCAP_AgentController.h"
#ifndef func_re_printf
#define func_re_printf mG4_PEGCAP_AgentController->get_fileline(__FILE__, __LINE__); mG4_PEGCAP_AgentController->_re_printf
#endif//func_re_printf

/*********************************
// Function     : G4_PEGCAP_Func
// Description  : Constructor of G4_PEGCAP_Func class
// Parameter    : None
// Return value : None
**********************************/
G4_PEGCAP_Func::G4_PEGCAP_Func (std::string name, std::string kind, G4_PEGCAP_AgentController *G4_PEGCAP_AgentControllerPtr)
{//{{{
    assert(G4_PEGCAP_AgentControllerPtr != NULL);
    mG4_PEGCAP_AgentController = G4_PEGCAP_AgentControllerPtr;
    mG4_PEGCAP_AgentController->CommandInit(name);
   
    mModuleName = name;
    mKind = kind;
    
    if (mKind == "U2A"){
        mpG4_PEGCAP_U2A_RegBlock = new G4_PEGCAP_U2A_Reg(name, this);
    }else{
        mpG4_PEGCAP_E2x_RegBlock = new G4_PEGCAP_E2x_Reg(name);
    }
}//}}}

/*********************************
// Function     : ~G4_PEGCAP_Func
// Description  : Destructor of G4_PEGCAP_Func class
// Parameter    : None
// Return value : None
**********************************/
G4_PEGCAP_Func::~G4_PEGCAP_Func (void)
{//{{{
    if (mKind == "U2A"){
        delete mpG4_PEGCAP_U2A_RegBlock;
    }else{
        delete mpG4_PEGCAP_E2x_RegBlock;
    }
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Enable/ Disable reset
// Parameter    : 
//    is_active   true: reset, false: not reset
// Return value : None
**********************************/
void G4_PEGCAP_Func::EnableReset (const bool is_active)
{//{{{
    if (mKind == "U2A"){
        mpG4_PEGCAP_U2A_RegBlock->EnableReset(is_active);
    }else{
        mpG4_PEGCAP_E2x_RegBlock->EnableReset(is_active);
    }
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Handle commands access to register.
// Parameter    : 
//     args       Please 
// Return value : None
**********************************/
void G4_PEGCAP_Func::RegisterHandler (const std::vector<std::string>& args)
{//{{{
    if (mKind == "U2A"){
        mpG4_PEGCAP_U2A_RegBlock->RegisterHandler(args);
    }else{
        mpG4_PEGCAP_E2x_RegBlock->RegisterHandler(args);
    }
}//}}}

/*********************************
// Function     : read
// Description  : Overwrite virtual functions of BusSlaveFuncIf class
// Parameter    : 
//    offsetAddress     read address
//    trans             payload of transaction
//    t                 bus time
//    debug             true: debug mode; false: normal mode   
// Return value : None
**********************************/
void G4_PEGCAP_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    // Zero clock or reset active
    if ((!mG4_PEGCAP_AgentController->CheckClockPeriod()) || (mG4_PEGCAP_AgentController->GetResetStatus())) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                func_re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
                return;
            }
        }
        else {
            func_re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            return;
        }
    }

    if (mKind == "U2A") {
        mpG4_PEGCAP_U2A_RegBlock->read(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }
    else {
        mpG4_PEGCAP_E2x_RegBlock->read(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }

}//}}}

/*********************************
// Function     : write
// Description  : Overwrite virtual functions of BusSlaveFuncIf class
// Parameter    : 
//    offsetAddress     write address
//    trans             payload of transaction
//    t                 bus time
//    debug             true: debug mode; false: normal mode   
// Return value : None
**********************************/
void G4_PEGCAP_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (!mG4_PEGCAP_AgentController->CheckClockPeriod()) {
        // Software debugger
        if ((pExtInfo != NULL)) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                func_re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }
        else {
            func_re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }

    if (mKind == "U2A") {
        mpG4_PEGCAP_U2A_RegBlock->write(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }
    else {
        mpG4_PEGCAP_E2x_RegBlock->write(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }

}//}}}

/*********************************
// Function     : SetERR_INFO
// Description  : Record error info
// Parameter    : 
//    err_info    error info
//    same_spid   error is same spid or not
// Return value : None
**********************************/
void G4_PEGCAP_Func::SetERR_INFO(G4_PEG_Cerrinfo err_info, bool same_spid) 
{//{{{
    if (mKind == "U2A"){
        mpG4_PEGCAP_U2A_RegBlock->SetERR_INFO(err_info, same_spid);
    }else{
        mpG4_PEGCAP_E2x_RegBlock->SetERR_INFO(err_info);
    }
}//}}}

/*********************************
// Function     : IsSameSPID
// Description  : Judge current error SPID is recored or not.
// Parameter    : 
//    cur_SPID    current error SPID
// Return value : true: recorded; false: not recorded yet.
**********************************/
bool G4_PEGCAP_Func::IsSameSPID(unsigned int cur_SPID)
{//{{{
    if (mKind == "U2A"){
        return mpG4_PEGCAP_U2A_RegBlock->IsSameSPID(cur_SPID);
    }else{
        return false;
    }
}//}}}

/*********************************
// Function     : NotifyClearPEGSPIDERRCLREvent
// Description  : Notify trigger of ClearPEGSPIDERRCLRMethod
// Parameter    : None
// Return value : None
**********************************/
void G4_PEGCAP_Func::NotifyClearPEGSPIDERRCLREvent()
{//{{{
    mG4_PEGCAP_AgentController->NotifyClearPEGSPIDERRCLREvent();
}//}}}

/*********************************
// Function     : ClearPEGSPIDERRCLR
// Description  : Call ClearPEGSPIDERRCLR function of G4_PEGCAP_U2A_Reg
// Parameter    : None
// Return value : None
**********************************/
void G4_PEGCAP_Func::ClearPEGSPIDERRCLR() 
{//{{{
    mpG4_PEGCAP_U2A_RegBlock->ClearPEGSPIDERRCLR();
}//}}}

// vim600: set foldmethod=marker :
