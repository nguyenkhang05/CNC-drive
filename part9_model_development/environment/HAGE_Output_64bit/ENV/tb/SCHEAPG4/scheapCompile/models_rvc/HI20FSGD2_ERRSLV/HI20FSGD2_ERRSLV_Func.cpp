// ---------------------------------------------------------------------
// $Id: HI20FSGD2_ERRSLV_Func.cpp,v 1.4 2019/12/06 07:20:20 hadinhtran Exp $
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "global.h"
#include "HI20FSGD2_ERRSLV_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
///Constructor of HI20FSGD2_ERRSLV_Func class
HI20FSGD2_ERRSLV_Func::HI20FSGD2_ERRSLV_Func(std::string name, HI20FSGD2_ERRSLV_AgentController* HI20FSGD2_ERRSLV_AgentControllerPtr):
                Chi20fsgd2_errslv_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(HI20FSGD2_ERRSLV_AgentControllerPtr != NULL);
    mpHI20FSGD2_ERRSLV_AgentController = HI20FSGD2_ERRSLV_AgentControllerPtr;
    mpHI20FSGD2_ERRSLV_AgentController->CommandInit(name);
    mModuleName = name;

    Chi20fsgd2_errslv_regif::set_instance_name(name);
    Initialize();
}//}}}

///Destructor of HI20FSGD2_ERRSLV_Func class
HI20FSGD2_ERRSLV_Func::~HI20FSGD2_ERRSLV_Func(void)
{//{{{
}//}}}

///Reset Func and registers
void HI20FSGD2_ERRSLV_Func::EnableReset (const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    Chi20fsgd2_errslv_regif::EnableReset(is_active);
}//}}}

/// Initialize internal variables
void HI20FSGD2_ERRSLV_Func::Initialize(void)
{//{{{
    mTypeADDR   = 0;
    mTypeSEC    = false;
    mTypeDBG    = false;
    mTypeUM     = false;
    mTypeSPID   = 0;
    mTypeWRITE  = false;
}//}}}

///Handle commands of register IF class
void HI20FSGD2_ERRSLV_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Chi20fsgd2_errslv_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Chi20fsgd2_errslv_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
        } else {
            msg_return = Chi20fsgd2_errslv_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Description: Read API (target socket)
void HI20FSGD2_ERRSLV_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);

    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());

    // When clock = 0, or reset state, only debug master can read register 
    if ( (!mpHI20FSGD2_ERRSLV_AgentController->CheckClockPeriod()) || (mpHI20FSGD2_ERRSLV_AgentController->GetResetStatus()) ){
        if (extension != NULL){
            if (! (debug && (extension->getPeId() == DEBUG_MASTER_BMID)) ){
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

/// Description: Write API (target socket)
void HI20FSGD2_ERRSLV_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);

    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only debug master write
    if (!mpHI20FSGD2_ERRSLV_AgentController->CheckClockPeriod()){
        if (extension != NULL){
            if (! (debug && (extension->getPeId() == DEBUG_MASTER_BMID)) ){
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

/// Description: Set value to error status registers
void HI20FSGD2_ERRSLV_Func::SetVar(const std::string name, unsigned int val)
{//{{{
    if (name == "SEC") {
        // Set access attribute of SEC
        (*PBGERRTYPE)["SEC"] = (val & 0x1);
    }
    else if (name == "DBG") {
        // Set access attribute of DBG
        (*PBGERRTYPE)["DBG"] = (val & 0x1);
    }
    else if (name == "UM"){
        // Set access attribute of UM
        (*PBGERRTYPE)["UM"] =  (val & 0x1);
    }
    else if (name == "SPID"){
        // Set access attribute of SPID
        (*PBGERRTYPE)["SPID"] = val;
    }
    else if (name == "WRITE") {
        // Set access type of WRITE
        (*PBGERRTYPE)["WRITE"] = (val & 0x1);
    }
    else if (name == "ADDR") {
        // Store error address into PBGERRADDR register
        (*PBGERRADDR)["ADDR"] = val;
    }
    else { // name == "SPIDERR"
        // Store SPID error into PBGSPIDERRSTAT register
        (*PBGSPIDERRSTAT)["SPIDERR"] = ((unsigned int)(*PBGSPIDERRSTAT)["SPIDERR"]) | ((unsigned int)(1U << val));
    }
}//}}}

/// Get value of variables which stored information when set guard error
unsigned int HI20FSGD2_ERRSLV_Func::GetAccessInfo(const std::string name, unsigned int val)
{//{{{
    if ((name == "SEC") && (val != 0)) {
        mTypeSEC = true;
        return (unsigned int)mTypeSEC;
    }
    else if((name=="DBG") && (val != 0)) {
        mTypeDBG = true;
        return (unsigned int)mTypeDBG;
    }
    else if((name=="UM") && (val != 0)) {
        mTypeUM = true;
        return (unsigned int)mTypeUM;
    }
    else if(name=="SPID") {
        mTypeSPID = val;
        return mTypeSPID;
    }
    else if((name=="WRITE") && (val != 0)){
        mTypeWRITE = true;
        return (unsigned int)mTypeWRITE;
    }
    else {
        return 0;
    }
}//}}}

///Set overflow bit to PBGOVFSTAT register
void HI20FSGD2_ERRSLV_Func::SetOverFlowBit()
{//{{{
    // Enable overflow status bit
    (*PBGOVFSTAT)["OVF"] = 0x1;
}//}}}

///Check if first time error occurs
bool HI20FSGD2_ERRSLV_Func::IsTheFirstERR(void)
{//{{{
    if ((unsigned int)(*PBGSPIDERRSTAT)["SPIDERR"] == 0) {
        return true;
    }
    else {
        return false;
    }
}//}}}

///Check if spid error is the same with previous spid errors
bool HI20FSGD2_ERRSLV_Func::IsTheSameSPID(unsigned int spid_err)
{//{{{
    if (1 == (((unsigned int)(*PBGSPIDERRSTAT)["SPIDERR"] >> spid_err) & 0x1)) {
        return true;
    }
    else {
        return false;
    }
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================

/// Description: Callback function of PBGOVFCLR_CRLO bit
void HI20FSGD2_ERRSLV_Func::cb_PBGOVFCLR_CLRO(RegCBstr str)
{//{{{
    // Proceed if writing 1 to this register
    if (1 == (unsigned int) (*PBGOVFCLR)["CLRO"]) {
        // Clear overflow status bit
        (*PBGOVFSTAT)["OVF"] = 0;
    }
    // Clear CLRO bit
    (*PBGOVFCLR)["CLRO"] = 0;
}//}}}

/// Description: Callback function of PBGSPIDERRCLR_SPIDCLR bit
void HI20FSGD2_ERRSLV_Func::cb_PBGSPIDERRCLR_SPIDCLR(RegCBstr str)
{//{{{
    // Get number of SPID error
    unsigned int temp = (unsigned int) ((*PBGSPIDERRSTAT)["SPIDERR"]) & (0xFFFFFFFF);
    // Proceed if temp is not 0
    if (0 != temp) {
        // Clear error status bit
        (*PBGSPIDERRSTAT)["SPIDERR"] = (((*PBGSPIDERRCLR)["SPIDCLR"])&temp)^temp;
    }
    // Clear SPIDCLR bit
    (*PBGSPIDERRCLR)["SPIDCLR"] = 0;
}//}}}

/// Description: Callback function of PBGGKCPROT_KCPROT bit
void HI20FSGD2_ERRSLV_Func::cb_PBGKCPROT_KCPROT(RegCBstr str)
{//{{{
    // Proceed if writing 0xA5A5A501 into PBGKCPROT register
    if (0xA5A5A501 == (unsigned int)(*PBGKCPROT)){
        // Enable Key Code bit
        (*PBGKCPROT)["KCE"] = 1;
        mpHI20FSGD2_ERRSLV_AgentController->SetKceBit(true);
    }
    // Proceed if writing 0xA5A5A500 into PBGKCPROT register
    else if (0xA5A5A500 == (unsigned int)(*PBGKCPROT)){
        // Disable Key Code bit
        (*PBGKCPROT)["KCE"] = 0;
        mpHI20FSGD2_ERRSLV_AgentController->SetKceBit(false);
    }
    else{
        // Reverse previous data 
        (*PBGKCPROT)["KCE"] = str.pre_data;
    }
    // Clear protection bit 
    (*PBGKCPROT)["KCPROT"] = 0;
}//}}}

