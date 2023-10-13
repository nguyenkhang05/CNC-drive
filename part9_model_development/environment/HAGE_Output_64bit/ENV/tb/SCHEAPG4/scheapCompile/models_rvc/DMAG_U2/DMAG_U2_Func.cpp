// ---------------------------------------------------------------------
// $Id: DMAG_U2_Func.cpp,v 1.4 2020/02/14 06:25:20 hadinhtran Exp $
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

#include "DMAG_U2_Func.h"
#include "DMAG_U2_AgentController.h"

///Constructor of DMAG_U2_Func class
DMAG_U2_Func::DMAG_U2_Func(std::string name, DMAG_U2_AgentController *DMAG_U2_AgentControllerPtr, unsigned int ch_num):
                Cdmag_u2_regif(name, 32, ch_num)
{//{{{
    assert(DMAG_U2_AgentControllerPtr != NULL);
    mDMAG_U2_AgentController = DMAG_U2_AgentControllerPtr;
    mDMAG_U2_AgentController->CommandInit(name);
    Cdmag_u2_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of DMAG_U2_Func class
DMAG_U2_Func::~DMAG_U2_Func(void)
{//{{{
}//}}}

///Reset operation of DMAG_U2_Func class
void DMAG_U2_Func::EnableReset (const bool is_active)
{//{{{
    Cdmag_u2_regif::EnableReset(is_active);
}//}}}

///Handle commands of register IF class
void DMAG_U2_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cdmag_u2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Cdmag_u2_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
        } else {
            msg_return = Cdmag_u2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void DMAG_U2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // When clock = 0, or reset state, only debug master can read register 
    if ( (!mDMAG_U2_AgentController->CheckClockPeriod()) || (mDMAG_U2_AgentController->GetResetStatus()) ){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void DMAG_U2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only debug master write
    if (!mDMAG_U2_AgentController->CheckClockPeriod()){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }

    // write register if possible
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Set initial value for DMAGMPIDm registers
void DMAG_U2_Func::DMAG_U2_P_RESET_VALUE0_m(unsigned int value, unsigned int index)
{//{{{
    // Set initial value for DMAGMPIDm registers
    if ((index < emNUM_MAX_DMAGMPID) && (value <= emNum_5BitMax)){
        DMAGMPID[index]->UpdateBitInitValue("SPID", value&0x1F);
    }
    else {
        re_printf("warning", "Channel index argument or value argument is wrong.\n");
    }
}//}}}

///Set initial value for DMAGPROT_n registers
void DMAG_U2_Func::DMAG_U2_P_RESET_VALUE1_n(unsigned int value, unsigned int index)
{//{{{
    // get value of bits
    unsigned int mpid = value >> emNum_BitPosition_Base_MPID & 0xFF;
    unsigned int gen = value>> emNUM_BitPosition_GEN &0x1;
    unsigned int dbg = value >> emNUM_BitPosition_DBG & 0x1;
    unsigned int um = value >> emNUM_BitPosition_UM & 0x1;
    unsigned int wg = value >> emNUM_BitPosition_WG & 0x1;
    unsigned int rg = value >> emNUM_BitPosition_RG & 0x1;

    std::ostringstream str_tmp;
    // Set initial value for DMAGPROT_n registers
    if ((mChNum - 2U) == index) {
        DMAGPROT_GR->UpdateBitInitValue("MPID", mpid);
        DMAGPROT_GR->UpdateBitInitValue("GEN", gen);
        DMAGPROT_GR->UpdateBitInitValue("DBG", dbg);
        DMAGPROT_GR->UpdateBitInitValue("UM", um);
        DMAGPROT_GR->UpdateBitInitValue("WG", wg);
        DMAGPROT_GR->UpdateBitInitValue("RG", rg);
    }
    else if ((mChNum - 1U) == index) {
        DMAGPROT_GR->UpdateBitInitValue("MPID", mpid);
        DMAGPROT_DP->UpdateBitInitValue("GEN", gen);
        DMAGPROT_DP->UpdateBitInitValue("DBG", dbg);
        DMAGPROT_DP->UpdateBitInitValue("UM", um);
        DMAGPROT_DP->UpdateBitInitValue("WG", wg);
        DMAGPROT_DP->UpdateBitInitValue("RG", rg);
    }
    else if (index < (mChNum - 2U)) {
        DMAGPROT_GR->UpdateBitInitValue("MPID", mpid);
        DMAGPROT_[index]->UpdateBitInitValue("GEN", gen);
        DMAGPROT_[index]->UpdateBitInitValue("DBG", dbg);
        DMAGPROT_[index]->UpdateBitInitValue("UM", um);
        DMAGPROT_[index]->UpdateBitInitValue("WG", wg);
        DMAGPROT_[index]->UpdateBitInitValue("RG", rg);
    }
    else {
        re_printf("warning", "Channel index argument is wrong.\n");
    }
}//}}}

///Set value to error info registers
void DMAG_U2_Func::SetVar(const std::string name, const unsigned int val)
{//{{{
    if (name == "SEC") {
        (*DMAGERRTYPE)["SEC"] = val & 0x1;
    }
    else if (name == "DBG") {
        (*DMAGERRTYPE)["DBG"] = val & 0x1;
    }
    else if (name == "UM") {
        (*DMAGERRTYPE)["UM"] = val & 0x1;
    }
    else if (name == "SPID") {
        (*DMAGERRTYPE)["SPID"] = val & 0x1F;
    }
    else if (name == "WRITE") {
        (*DMAGERRTYPE)["WRITE"] = val & 0x1;
    }
    else if (name == "ADDR") {
        (*DMAGERRADDR) = val;
    }
    else { // name is "SPIDERR"
        // Store SPID error into DMAGSPIDERRSTAT register
        (*DMAGSPIDERRSTAT)["SPIDERR"] = ((unsigned int)(*DMAGSPIDERRSTAT)["SPIDERR"]) | ((unsigned int)(1U << val));
    }
}//}}}

///Set overflow bit to DMAGOVFSTAT register
void DMAG_U2_Func::SetOverFlowBit()
{//{{{
    (*DMAGOVFSTAT)["OVF"] = 0x1;
}//}}}

///Check if first time error occurs
bool DMAG_U2_Func::IsTheFirstERR(void)
{//{{{
    if ((unsigned int)(*DMAGSPIDERRSTAT)["SPIDERR"] == 0) {
        return true;
    }
    else {
        return false;
    }
}//}}}

///Check if spid error is the same with previous spid errors
bool DMAG_U2_Func::IsTheSameSPID(unsigned int spid_err)
{//{{{
    if (1 == ((unsigned int)(*DMAGSPIDERRSTAT)["SPIDERR"] >> spid_err & 0x1)) {
        return true;
    }
    else {
        return false;
    }
}//}}}

///Get permitted SPID from channel setting registers
unsigned int DMAG_U2_Func::GetPermittedSPID(unsigned char channel_id)
{//{{{
    unsigned int SPID_pos_bit = 0;
    unsigned int mpid[emNUM_MAX_DMAGMPID] = { 0 };
    std::ostringstream str_tmp;
    if ((mChNum - 2U) == channel_id) {
        for (unsigned int i = 0; i < emNUM_MAX_DMAGMPID; i++) {
            if (((unsigned int)(*DMAGPROT_GR)["MPID"] >> i) & 0x1 != 0){
                mpid[i] = (unsigned int)(*DMAGMPID[i])["SPID"];
            } else {
                mpid[i] = 0xFF;
            }
        }
    }
    else if ((mChNum - 1U) == channel_id) {
        for (unsigned int i = 0; i < emNUM_MAX_DMAGMPID; i++) {
            if (((unsigned int)(*DMAGPROT_DP)["MPID"] >> i) & 0x1 != 0){
                mpid[i] = (unsigned int)(*DMAGMPID[i])["SPID"];
            } else {
                mpid[i] = 0xFF;
            }
        }
    }
    else { // channel_id < (mChNum - 2U)
        for (unsigned int i = 0; i < emNUM_MAX_DMAGMPID; i++) {
            if (((unsigned int)(*DMAGPROT_[channel_id])["MPID"] >> i) & 0x1 != 0){
                mpid[i] = (unsigned int)(*DMAGMPID[i])["SPID"];
            } else {
                mpid[i] = 0xFF;
            }
        }
    }
    unsigned int temp = 0;
    for (unsigned int i = 0; i < emNUM_MAX_DMAGMPID; i++) {
        temp = 0;
        if (32U > mpid[i]) {
            temp = 1 << (mpid[i] & 0x1F);
            SPID_pos_bit |= temp;
        }
    }
    return SPID_pos_bit;
}//}}}

///Get protection setting info from channel setting registers
void DMAG_U2_Func::GetDMAGPROT_n(unsigned char channel_id, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg)
{//{{{
    if ((mChNum - 2U) == channel_id) {
        gen = (bool)((unsigned int)(*DMAGPROT_GR)["GEN"]);
        dbg = (bool)((unsigned int)(*DMAGPROT_GR)["DBG"]);
        um = (bool)((unsigned int)(*DMAGPROT_GR)["UM"]);
        wg = (bool)((unsigned int)(*DMAGPROT_GR)["WG"]);
        rg = (bool)((unsigned int)(*DMAGPROT_GR)["RG"]);
    }
    else if ((mChNum - 1U) == channel_id) {
        gen = (bool)((unsigned int)(*DMAGPROT_DP)["GEN"]);
        dbg = (bool)((unsigned int)(*DMAGPROT_DP)["DBG"]);
        um = (bool)((unsigned int)(*DMAGPROT_DP)["UM"]);
        wg = (bool)((unsigned int)(*DMAGPROT_DP)["WG"]);
        rg = (bool)((unsigned int)(*DMAGPROT_DP)["RG"]);
    }
    else{ // channel_id < (mChNum - 2U)
        gen = (bool)((unsigned int)(*DMAGPROT_[channel_id])["GEN"]);
        dbg = (bool)((unsigned int)(*DMAGPROT_[channel_id])["DBG"]);
        um = (bool)((unsigned int)(*DMAGPROT_[channel_id])["UM"]);
        wg = (bool)((unsigned int)(*DMAGPROT_[channel_id])["WG"]);
        rg = (bool)((unsigned int)(*DMAGPROT_[channel_id])["RG"]);
    }
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================

///Callback function of DMAGOVFCLR_CLRO bit
void DMAG_U2_Func::cb_DMAGOVFCLR_CLRO(RegCBstr str)
{//{{{
    // Proceed if writing 1 to this register
    if (1 == (unsigned int) (*DMAGOVFCLR)["CLRO"]) {
        // Clear overflow status bit
        (*DMAGOVFSTAT)["OVF"] = 0;
    }
    // Clear CLRO bit
    (*DMAGOVFCLR)["CLRO"] = 0;
}//}}}

///Callback function of DMAGSPIDERRCLR_SPIDCLR bit
void DMAG_U2_Func::cb_DMAGSPIDERRCLR_SPIDCLR(RegCBstr str)
{//{{{
    // Get number of SPID error
    unsigned int temp = (unsigned int) ((*DMAGSPIDERRSTAT)["SPIDERR"]) & (0xFFFFFFFF);
    // Proceed if temp is not 0
    if (0 != temp) {
        // Clear error status bit
        (*DMAGSPIDERRSTAT)["SPIDERR"] = (((*DMAGSPIDERRCLR)["SPIDCLR"])&temp)^temp;
    }
    // Clear SPIDCLR bit
    (*DMAGSPIDERRCLR)["SPIDCLR"] = 0;
}//}}}

///Callback function of DMAGKCPROT_KCPROT bit
void DMAG_U2_Func::cb_DMAGKCPROT_KCPROT(RegCBstr str)
{//{{{
    // Proceed if writing 0xA5A5A501 into DMAGKCPROT register
    if (0xA5A5A501 == (unsigned int)(*DMAGKCPROT)){
        // Enable Key Code bit
        (*DMAGKCPROT)["KCE"] = 1;
    }
    // Proceed if writing 0xA5A5A500 into DMAGKCPROT register
    else if (0xA5A5A500 == (unsigned int)(*DMAGKCPROT)){
        // Disable Key Code bit
        (*DMAGKCPROT)["KCE"] = 0;
    }
    else{
        // Reverse previous data 
        (*DMAGKCPROT)["KCE"] = str.pre_data;
    }
    // Clear protection bit 
    (*DMAGKCPROT)["KCPROT"] = 0;
}//}}}

///Callback function of DMAGMPID_SPID bit
void DMAG_U2_Func::cb_DMAGMPID_SPID(RegCBstr str)
{//{{{
    // Proceed when key code bit is disable
    if ((unsigned int)(*DMAGKCPROT)["KCE"] == 0) {
        (*DMAGMPID[str.channel]) = str.pre_data;
    }
}//}}}

///Callback function of DMAGPROT_GR_GEN bit
void DMAG_U2_Func::cb_DMAGPROT_GR_GEN(RegCBstr str)
{//{{{
    // Proceed when key code bit is disable
    if ((unsigned int)(*DMAGKCPROT)["KCE"] == 0) {
        (*DMAGPROT_GR) = str.pre_data;
    }
    
}//}}}

///Callback function of DMAGPROT_DP_GEN bit
void DMAG_U2_Func::cb_DMAGPROT_DP_GEN(RegCBstr str)
{//{{{
    // Proceed when key code bit is disable
    if ((unsigned int)(*DMAGKCPROT)["KCE"] == 0) {
        (*DMAGPROT_DP) = str.pre_data;
    }
}//}}}

///Callback function of DMAGPROT__GEN bit
void DMAG_U2_Func::cb_DMAGPROT__GEN(RegCBstr str)
{//{{{
    // Proceed when key code bit is disable
    if ((unsigned int)(*DMAGKCPROT)["KCE"] == 0) {
        (*DMAGPROT_[str.channel]) = str.pre_data;
    }
}//}}}

