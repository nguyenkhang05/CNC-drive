// ---------------------------------------------------------------------
// $Id: G4_PEG_Func.cpp,v 1.8 2020/01/14 02:39:30 nhutminhdang Exp $
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

#include "G4_PEG_Func.h"
#include "G4_PEG_E2x_Reg.h"
#include "G4_PEG_U2A_Reg.h"
#include "G4_PEG_AgentController.h"
#ifndef func_re_printf
#define func_re_printf mG4_PEG_AgentController->get_fileline(__FILE__, __LINE__); mG4_PEG_AgentController->_re_printf
#endif//func_re_printf

/*********************************
// Function     : G4_PEG_Func
// Description  : Constructor of G4_PEG_Func class
// Parameter    : None
// Return value : None
**********************************/
G4_PEG_Func::G4_PEG_Func (std::string name, std::string kind, G4_PEG_AgentController *G4_PEG_AgentControllerPtr, unsigned int PEG_ID)
{//{{{
    assert(G4_PEG_AgentControllerPtr != NULL);
    mG4_PEG_AgentController = G4_PEG_AgentControllerPtr;
    mG4_PEG_AgentController->CommandInit(name);
 
    mModuleName = name;
    mPEID = PEG_ID;
    mKind = kind;
    
    if (mKind == "U2A"){
        mpG4_PEG_U2A_RegBlock = new G4_PEG_U2A_Reg(name);
    }else{
        mpG4_PEG_E2x_RegBlock = new G4_PEG_E2x_Reg(name);
    }
}//}}}

/*********************************
// Function     : ~G4_PEG_Func
// Description  : Destructor of G4_PEG_Func class
// Parameter    : None
// Return value : None
**********************************/
G4_PEG_Func::~G4_PEG_Func (void)
{//{{{
    if (mKind == "U2A"){
        delete mpG4_PEG_U2A_RegBlock;
    }else{
        delete mpG4_PEG_E2x_RegBlock;
    }
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Enable/ Disable reset
// Parameter    : 
//    is_active   true: reset, false: not reset
// Return value : None
**********************************/
void G4_PEG_Func::EnableReset (const bool is_active)
{//{{{
    if (mKind == "U2A"){
        mpG4_PEG_U2A_RegBlock->EnableReset(is_active);
    }else{
        mpG4_PEG_E2x_RegBlock->EnableReset(is_active);
    }
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Handle commands access to register.
// Parameter    : 
//     args       Please 
// Return value : None
**********************************/
void G4_PEG_Func::RegisterHandler (const std::vector<std::string>& args)
{//{{{
    if (mKind == "U2A"){
        mpG4_PEG_U2A_RegBlock->RegisterHandler(args);
    }else{
        mpG4_PEG_E2x_RegBlock->RegisterHandler(args);
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
void G4_PEG_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    // Zero clock or reset active
    if ((!mG4_PEG_AgentController->CheckClockPeriod()) || (mG4_PEG_AgentController->GetResetStatus())) {
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
        mpG4_PEG_U2A_RegBlock->read(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }
    else {
        mpG4_PEG_E2x_RegBlock->read(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
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
void G4_PEG_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (!mG4_PEG_AgentController->CheckClockPeriod()) {
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
        mpG4_PEG_U2A_RegBlock->write(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }
    else {
        mpG4_PEG_E2x_RegBlock->write(offsetAddress, trans.get_data_ptr(), trans.get_data_length(), debug);
    }

}//}}}

/*********************************
// Function     : SetPeId
// Description  : Set value PEID for of G4_PEG
// Parameter    : 
//      peid      new value of PEID
// Return value : None
**********************************/
void G4_PEG_Func::SetPeId (unsigned int peid) {
    func_re_printf("info","PEID is set to %d\n", peid);
    mPEID = peid;
}

/*********************************
// Function     : getPEGSPID_SPID
// Description  : Get value of PEGSPID register
// Parameter    : 
//     channel_id      id of channel
// Return value : None
**********************************/
unsigned int G4_PEG_Func::getPEGSPID_SPID(unsigned int channel_id)
{//{{{
    if (mKind == "U2A"){
        return mpG4_PEG_U2A_RegBlock->getPEGSPID_SPID(channel_id);
    }else{
        return mpG4_PEG_E2x_RegBlock->getPEGSPID_SPID(channel_id);
    }
}//}}}

/*********************************
// Function     : getPEGROT_GEN
// Description  : Get value of GEN bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : None
**********************************/
bool G4_PEG_Func::getPEGROT_GEN(unsigned int channel_id)
{//{{{
    if (mKind == "U2A"){
        return mpG4_PEG_U2A_RegBlock->getPEGROT_GEN(channel_id);
    }else{
        return mpG4_PEG_E2x_RegBlock->getPEGROT_GEN(channel_id);
    }
}//}}}

/*********************************
// Function     : getPEGROT_DBG
// Description  : Get value of DBG bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : None
**********************************/
bool G4_PEG_Func::getPEGROT_DBG(unsigned int channel_id)
{//{{{
    if (mKind == "U2A"){
        return mpG4_PEG_U2A_RegBlock->getPEGROT_DBG(channel_id);
    }else{
        return mpG4_PEG_E2x_RegBlock->getPEGROT_DBG(channel_id);
    }
}//}}}

/*********************************
// Function     : getPEGROT_UM
// Description  : Get value of UM bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : None
**********************************/
bool G4_PEG_Func::getPEGROT_UM(unsigned int channel_id)
{//{{{
    if (mKind == "U2A"){
        return mpG4_PEG_U2A_RegBlock->getPEGROT_UM(channel_id);
    }else{
        return mpG4_PEG_E2x_RegBlock->getPEGROT_UM(channel_id);
    }
}//}}}

/*********************************
// Function     : getPEGROT_WG
// Description  : Get value of WG bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : None
**********************************/
bool G4_PEG_Func::getPEGROT_WG(unsigned int channel_id)
{//{{{
    if (mKind == "U2A"){
        return mpG4_PEG_U2A_RegBlock->getPEGROT_WG(channel_id);
    }else{
        return mpG4_PEG_E2x_RegBlock->getPEGROT_WG(channel_id);
    }
}//}}}

/*********************************
// Function     : getPEGROT_RG
// Description  : Get value of RG bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : None
**********************************/
bool G4_PEG_Func::getPEGROT_RG(unsigned int channel_id)
{//{{{
    if (mKind == "U2A"){
        return mpG4_PEG_U2A_RegBlock->getPEGROT_RG(channel_id);
    }else{
        return mpG4_PEG_E2x_RegBlock->getPEGROT_RG(channel_id);
    }
}//}}}

/*********************************
// Function     : getPEGBAD_BAD
// Description  : Get value of PEGBAD register
// Parameter    : 
//     channel_id      id of channel
// Return value : None
**********************************/
unsigned int G4_PEG_Func::getPEGBAD_BAD(unsigned int channel_id)
{//{{{
    if (mKind == "U2A"){
        return mpG4_PEG_U2A_RegBlock->getPEGBAD_BAD(channel_id);
    }else{
        return mpG4_PEG_E2x_RegBlock->getPEGBAD_BAD(channel_id);
    }
}//}}}

/*********************************
// Function     : getPEGADV_ADV
// Description  : Get value of PEGADV register
// Parameter    : 
//     channel_id      id of channel
// Return value : None
**********************************/
unsigned int G4_PEG_Func::getPEGADV_ADV(unsigned int channel_id)
{//{{{
    if (mKind == "U2A"){
        return mpG4_PEG_U2A_RegBlock->getPEGADV_ADV(channel_id);
    }else{
        return mpG4_PEG_E2x_RegBlock->getPEGADV_ADV(channel_id);
    }
}//}}}
// vim600: set foldmethod=marker :
