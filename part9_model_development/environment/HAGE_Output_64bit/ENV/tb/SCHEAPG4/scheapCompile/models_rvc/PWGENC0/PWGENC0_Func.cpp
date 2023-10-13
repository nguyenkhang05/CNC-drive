// ---------------------------------------------------------------------
// $Id: PWGENC0_Func.cpp,v 1.10 2020/02/17 07:36:51 landang Exp $
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
#include "PWGENC0_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
PWGENC0_Func::PWGENC0_Func (std::string name, PWGENC0_AgentController* PWGENC0AgentController):
                Cpwgenc0_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(PWGENC0AgentController != NULL);
    mpPWGENC0AgentController = PWGENC0AgentController;
    mpPWGENC0AgentController->CommandInit(name);
    mModuleName = name;
    Cpwgenc0_regif::set_instance_name(name);
}//}}}

/// Destructor
PWGENC0_Func::~PWGENC0_Func()
{//{{{
}//}}}

/// Reset Func and registers
void PWGENC0_Func::EnableReset(const bool is_active)
{//{{{
    Cpwgenc0_regif::EnableReset(is_active);
}//}}}

/// Read API (target socket)
void PWGENC0_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
 // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    unsigned int size = (unsigned int)trans.get_data_length();
    memset(trans.get_data_ptr(), 0, size);

    if (mpPWGENC0AgentController->CheckCounterRunning()){
        (*PWG0CNT) = mpPWGENC0AgentController->GetCurrentCounter();
    }

    if ((!mpPWGENC0AgentController->CheckClockPeriod("PCLKRW")) || (mpPWGENC0AgentController->GetResetStatus())) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
                return;
            }
        } else {
            re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            return;
        }
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Write API (target socket)
void PWGENC0_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
 // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (!mpPWGENC0AgentController->CheckClockPeriod("PCLKRW")) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot access register when clock is 0.\n");
                return;
            }
        } else {
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }

    unsigned int address = (unsigned int)trans.get_address();

    if (!debug) {
        reg_wr(address, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg(address, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Handler for register IF
void PWGENC0_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Cpwgenc0_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to regif class
            msg_return = Cpwgenc0_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Update PWG0TCBR register from PWG0TCR
void PWGENC0_Func::LoadTCBR()
{//{{{
    (*PWG0TCBR) = (unsigned int)(*PWG0TCR);
}//}}}

/// Load counter value into PWG0CTBR, PWG0CSBR, PWG0CRBR
void PWGENC0_Func::LoadCounterValue()
{//{{{
    (*PWG0CTBR) = (unsigned int)(*PWG0CTDR);
    (*PWG0CSBR) = (unsigned int)(*PWG0CSDR);
    (*PWG0CRBR) = (unsigned int)(*PWG0CRDR);
}//}}}

/// Set value to status register (PWG0ST)
void PWGENC0_Func::SetPWG0STReg(unsigned int value)
{//{{{
    (*PWG0ST) = value;
}//}}}

/// Set value to TE bit
void PWGENC0_Func::UpdateTEBit(unsigned int value)
{//{{{
    (*PWG0TE)["TE"] = value;
}//}}}

/// Get value of TE bit
unsigned int PWGENC0_Func::GetTEBit()
{//{{{
    return (unsigned int)(*PWG0TE)["TE"];
}//}}}

/// Get PWG0CTL register value
unsigned int PWGENC0_Func::GetPWG0CTLReg()
{//{{{ 
    return (unsigned int)(*PWG0CTL);
}//}}}

/// Get PWG0CSBR register value
unsigned int PWGENC0_Func::GetPWG0CSBRReg()
{//{{{ 
    return (unsigned int)(*PWG0CSBR);
}//}}}

/// Get PWG0CRBR register value
unsigned int PWGENC0_Func::GetPWG0CRBRReg()
{//{{{ 
    return (unsigned int)(*PWG0CRBR);
}//}}}

/// Get PWG0CTBR register value
unsigned int PWGENC0_Func::GetPWG0CTBRReg()
{//{{{ 
    return (unsigned int)(*PWG0CTBR);
}//}}}

/// Get PWG0TCBR register value
unsigned int PWGENC0_Func::GetPWG0TCBRReg()
{//{{{ 
    return (unsigned int)(*PWG0TCBR);
}//}}}

/// Get PWG0ST register value
unsigned int PWGENC0_Func::GetPWG0STReg()
{//{{{ 
    return (unsigned int)(*PWG0ST);
}//}}}

/// Update value to RSF status
void PWGENC0_Func::UpdateRSFBit(unsigned int value)
{//{{{
    (*PWG0ST)["RSF"] = value;
}//}}}

/// Update value to RSFT status
void PWGENC0_Func::UpdateRSFTBit(unsigned int value)
{//{{{
    (*PWG0ST)["RSFT"] = value;
}//}}}

/// Update value to FOF status
void PWGENC0_Func::UpdateFOFBit(unsigned int value)
{//{{{
    (*PWG0ST)["FOF"] = value;
}//}}}

/// Set value to PWG0CNT register
void PWGENC0_Func::SetPWG0CNTReg(unsigned int value)
{//{{{
    (*PWG0CNT) = value;
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of cb_PWG0CSDR_CSDR
void PWGENC0_Func::cb_PWG0CSDR_CSDR(RegCBstr str)
{//{{{
    unsigned int pwg_period = mpPWGENC0AgentController->GetPERIOD();
    unsigned int csdr = (unsigned int)(*PWG0CSDR);
    if (csdr > pwg_period){
        re_printf("warning", "Cannot set PWG0CSDR register = (0x%X) larger than PWG_PERIOD (=0x%X).\n", csdr, pwg_period);
        (*PWG0CSDR) = str.pre_data;
    }
}//}}}

/// Callback function of cb_PWG0RDT_RDT
void PWGENC0_Func::cb_PWG0RDT_RDT(RegCBstr str)
{//{{{
    unsigned int tcut = (unsigned int)(*PWG0CTL)["TCUT"];

    // callback of RDT bit
    if ((unsigned int)(*PWG0RDT)["RDT"] == 1){
        mpPWGENC0AgentController->RequestSetRSFBit(); // request to update RSF bit as priority reset > set > clear
    }

    // callback of RDTT bit
    if ((unsigned int)(*PWG0RDT)["RDTT"] == 1){
        if ((tcut == 1) || (tcut == 2)){
            mpPWGENC0AgentController->RequestSetRSFTBit();// request to update RSFT bit as priority reset > set > clear
        }
    }

    // Read as 0
    (*PWG0RDT) = 0;
}//}}}

/// Callback function of cb_PWG0ST_RSF
void PWGENC0_Func::cb_PWG0ST_RSF(RegCBstr str)
{//{{{
    // Cannot change while counting
    if ((unsigned int)(*PWG0TE)["TE"] == 1){
        re_printf("warning", "Cannot write PWG0ST register while counting.\n");
        (*PWG0ST) = str.pre_data;
    }else{
        // Update RSF while stop counting
        if ((unsigned int)(*PWG0ST)["RSF"] == 0){
            mpPWGENC0AgentController->RequestClearRSFBit();
        }else{
            mpPWGENC0AgentController->RequestSetRSFBit();
        }
        // Update RSFT while stop counting
        if ((unsigned int)(*PWG0ST)["RSFT"] == 0){
            mpPWGENC0AgentController->RequestClearRSFTBit();
        }else{
            mpPWGENC0AgentController->RequestSetRSFTBit();
        }
        // Update FOF while stop counting
        if ((unsigned int)(*PWG0ST)["FOF"] == 0){
            mpPWGENC0AgentController->RequestClearFOFBit();
        }else{
            mpPWGENC0AgentController->RequestSetFOFBit();
        }
    }
}//}}}

/// Callback function of cb_PWG0CNT_CNT
void PWGENC0_Func::cb_PWG0CNT_CNT(RegCBstr str)
{//{{{
    // Callback write
    if ((unsigned int)(*PWG0TE)["TE"] == 0){
        mpPWGENC0AgentController->UpdateCurrentCounter((unsigned int)(*PWG0CNT));
    }else{
        re_printf("warning", "Cannot write PWG0CNT register while counting.\n");
    }
}//}}}

/// Callback function of cb_PWG0TCR_TOE
void PWGENC0_Func::cb_PWG0TCR_TOE(RegCBstr str)
{//{{{
    // Update PWG0TCBR after 1 cycle if TCUT = 3
    if ((unsigned int)(*PWG0CTL)["TCUT"] == 3){
        mpPWGENC0AgentController->LoadTCBREventAfter1PCLK();
    }
}//}}}

/// Callback function of cb_PWG0TCBR_TOBE
void PWGENC0_Func::cb_PWG0TCBR_TOBE(RegCBstr str)
{//{{{
    // Cannot change while counting
    if ((unsigned int)(*PWG0TE)["TE"] == 1){
        if ((unsigned int)(*PWG0TCBR) != str.pre_data){
            re_printf("warning", "Cannot change PWG0TCBR register while counting.\n");
            (*PWG0TCBR) = str.pre_data;
        }
    }
}//}}}

/// Callback function of cb_PWG0CTL_CKS
void PWGENC0_Func::cb_PWG0CTL_CKS(RegCBstr str)
{//{{{
    // Cannot write CKS, TCUT, OCL bit while counting
    if ((unsigned int)(*PWG0TE)["TE"] == 1){
        unsigned int cks  = (*PWG0CTL)["CKS"];
        unsigned int tcut = (*PWG0CTL)["TCUT"];
        unsigned int ocl  = (*PWG0CTL)["OCL"];
        unsigned int pre_cks  = str.pre_data & 0x3; // bit 1-0
        unsigned int pre_tcut = (str.pre_data >> 4) & 0x3; // bit 5-4
        unsigned int pre_ocl  = (str.pre_data >> 7) & 0x1; // bit 7
        if (cks != pre_cks){
            re_printf("warning", "Cannot change PWG0CTL.CKS bit while counting.\n");
            (*PWG0CTL)["CKS"] = pre_cks;
        }
        if (tcut != pre_tcut){
            re_printf("warning", "Cannot change PWG0CTL.TCUT bit while counting.\n");
            (*PWG0CTL)["TCUT"] = pre_tcut;
        }
        if (ocl != pre_ocl){
            re_printf("warning", "Cannot change PWG0CTL.OCL bit while counting.\n");
            (*PWG0CTL)["OCL"] = pre_ocl;
        }
    }
}//}}}

/// Callback function of cb_PWG0CSBR_CSBR
void PWGENC0_Func::cb_PWG0CSBR_CSBR(RegCBstr str)
{//{{{
    // Cannot change while counting
    if ((unsigned int)(*PWG0TE)["TE"] == 1){
        if ((unsigned int)(*PWG0CSBR) != str.pre_data){
            re_printf("warning", "Cannot change PWG0CSBR register while counting.\n");
            (*PWG0CSBR) = str.pre_data;
        }
    }
}//}}}

/// Callback function of cb_PWG0CRBR_CRBR
void PWGENC0_Func::cb_PWG0CRBR_CRBR(RegCBstr str)
{//{{{
    // Cannot change while counting
    if ((unsigned int)(*PWG0TE)["TE"] == 1){
        if ((unsigned int)(*PWG0CRBR) != str.pre_data){
            re_printf("warning", "Cannot change PWG0CRBR register while counting.\n");
            (*PWG0CRBR) = str.pre_data;
        }
    }
}//}}}

/// Callback function of cb_PWG0CTBR_CTBR
void PWGENC0_Func::cb_PWG0CTBR_CTBR(RegCBstr str)
{//{{{
    // Cannot change while counting
    if ((unsigned int)(*PWG0TE)["TE"] == 1){
        if ((unsigned int)(*PWG0CTBR) != str.pre_data){
            re_printf("warning", "Cannot change PWG0CTBR register while counting.\n");
            (*PWG0CTBR) = str.pre_data;
        }
    }
}//}}}

/// Callback function of cb_PWG0TS_TS
void PWGENC0_Func::cb_PWG0TS_TS(RegCBstr str)
{//{{{
    unsigned int ts = (unsigned int)(*PWG0TS)["TS"];
    unsigned int te = (unsigned int)(*PWG0TE)["TE"];
    if ((ts == 1) && (te == 0)){
        if (mpPWGENC0AgentController->GetPWG_STE() == 0){
            mpPWGENC0AgentController->EnableCounter(true);
        }
    }
    // Always read 0
    (*PWG0TS) = 0;
}//}}}

/// Callback function of cb_PWG0TT_TT
void PWGENC0_Func::cb_PWG0TT_TT(RegCBstr str)
{//{{{
    unsigned int tt = (unsigned int)(*PWG0TT)["TT"];
    unsigned int te = (unsigned int)(*PWG0TE)["TE"];
    if ((tt == 1) && (te == 1)){
        if (mpPWGENC0AgentController->GetPWG_STE() == 0){
            mpPWGENC0AgentController->EnableCounter(false);
        }
    }
    // Always read 0
    (*PWG0TT) = 0;
}//}}}

/// Callback function of cb_PWG0FOT_FOT
void PWGENC0_Func::cb_PWG0FOT_FOT(RegCBstr str)
{//{{{
    if ((unsigned int)(*PWG0FOT)["FOT"] == 1){
        mpPWGENC0AgentController->RequestSetFOFBit();
        mpPWGENC0AgentController->ForceTOUT((bool)(*PWG0CTL)["FOS"]);
    }
    // Always read 0
    (*PWG0FOT) = 0;
}//}}}

// vim600: set foldmethod=marker :
