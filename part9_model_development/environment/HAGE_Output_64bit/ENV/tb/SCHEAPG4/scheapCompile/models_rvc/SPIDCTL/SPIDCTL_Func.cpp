// ---------------------------------------------------------------------
// $Id: SPIDCTL_Func.cpp,v 1.2 2020/02/05 09:26:00 khoaanhnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of SPIDCTL_Func.cpp
// Ref: {SPIDCTL_DD_Copyright_001}
// Description: About the include header files of SPIDCTL_Func.cpp
// Ref: {SPIDCTL_DD_ATT1_002}
#include "SPIDCTL_Func.h"
#include "global.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
// Description: Constructor
// Ref: {SPIDCTL_DD_ATT1_004}
SPIDCTL_Func::SPIDCTL_Func(std::string name, SPIDCTL_AgentController* SPIDCTLAgentControllerPtr) :
    Cspidctl_regif(name, 32)
    , BusSlaveFuncIf()
{//{{{
    assert(SPIDCTLAgentControllerPtr != NULL);
    mpSPIDCTLAgentController = SPIDCTLAgentControllerPtr;
    mpSPIDCTLAgentController->CommandInit(name);
    mModuleName = name;

    Cspidctl_regif::set_instance_name(name);
    Initialize();
}//}}}

// Description: Destructor
SPIDCTL_Func::~SPIDCTL_Func()
{//{{{
}//}}}

// Description: Reset Func and registers
void SPIDCTL_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    Cspidctl_regif::EnableReset(is_active);
}//}}}

// Description: Initialize internal variables
void SPIDCTL_Func::Initialize(void)
{//{{{
}//}}}

// Description: Read API (target socket)
// Ref: {SPIDCTL_DD_ATT1_006, SPIDCTL_DD_ReferenceDoc_001, SPIDCTL_DD_Direction_030, SPIDCTL_DD_Flow_004}
void SPIDCTL_Func::read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());

    // When clock = 0, or reset state, only debug master can read register
    if ((!mpSPIDCTLAgentController->CheckClockPeriod()) || (mpSPIDCTLAgentController->GetResetStatus())) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else {
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
    else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

// Description: Write API (target socket)
// Ref: {SPIDCTL_DD_ATT1_006, SPIDCTL_DD_ReferenceDoc_001, SPIDCTL_DD_Direction_029, SPIDCTL_DD_Flow_005}
void SPIDCTL_Func::write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only debug master write
    if (!(mpSPIDCTLAgentController->CheckClockPeriod())) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot access register when clock is 0.\n");
                return;
            }
        }else {
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }

    // Write register if possible
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
    else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

// Description: Handler for register IF
void SPIDCTL_Func::RegisterHandler(const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    }
    else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin() + 1);
                cmd.insert(cmd.begin() + 1, reg_p->name());
                msg_return = Cspidctl_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Cspid_regif class
            msg_return = Cspidctl_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

// Description: Dump value of status registers
void SPIDCTL_Func::DumpStatusInfo(void)
{//{{{ 
    printf("Status information of registers: \n");
    for (unsigned int i = 0; i < emNUM_BMSPID; i++) {
        printf("    BMSPID%u               = 0x%08X\n", i, (unsigned int)(*BMSPID[i])["SPID"]);
        printf("    BMSPIDMSK%u            = 0x%08X\n", i, (unsigned int)(*BMSPIDMSK[i])["SPIDMSK"]);
        printf("    BMSPIDMSKLOCK%u.LOCK   = 0x%08X\n", i, (unsigned int)(*BMSPIDMSKLOCK[i])["LOCK"]);
    }
    printf("    SPIDKCPROT.KCPROT   = 0x%08X\n", (unsigned int)(*SPIDKCPROT)["KCPROT"]);
    printf("    SPIDKCPROT.KCE      = 0x%08X\n", (unsigned int)(*SPIDKCPROT)["KCE"]);
}//}}}

// Description: Get value of register SPID
// Ref: {SPIDCTL_DD_ATT1_007, SPIDCTL_DD_Registers_001}
unsigned int SPIDCTL_Func::GetSPIDReg_SPIDBit(unsigned int index)
{//{{{
    return (unsigned int)(*BMSPID[index])["SPID"];
}//}}}

// Description: Get value of register SPID Mask
// Ref: {SPIDCTL_DD_ATT1_007, SPIDCTL_DD_Registers_002}
unsigned int SPIDCTL_Func::GetSPIDMskReg(unsigned int index)
{//{{{
    return (unsigned int)(*BMSPIDMSK[index])["SPIDMSK"];
}//}}}
//========================================================
//============ Callback function  ========================
//========================================================
// Description: Callback function of SPID bits
// Ref: {SPIDCTL_DD_Registers_001, SPIDCTL_DD_Flow_006, SPIDCTL_DD_Feature_003}
void SPIDCTL_Func::cb_BMSPID_SPID(RegCBstr str)
{//{{{
    if (str.pre_data != str.data) { // Check change data
        mpSPIDCTLAgentController->NotifyUpdateSPIDValue(str.channel);
    }
}//}}}

// Description: Callback function of SPID Mask register
// Ref: {SPIDCTL_DD_Feature_006, SPIDCTL_DD_Registers_002, SPIDCTL_DD_Flow_007, SPIDCTL_DD_Direction_047, SPIDCTL_DD_Direction_048}
void SPIDCTL_Func::cb_BMSPIDMSK_SPIDMSK(RegCBstr str)
{//{{{
    std::string register_name = "BMSPIDMSK";
    if (true == mpSPIDCTLAgentController->IsKeyCodeEnable()) { // KCE bit is 1
        if (false == mpSPIDCTLAgentController->IsMaskLocked(str.channel)) { // LOCK bit is 0
            mpSPIDCTLAgentController->NotifyUpdateSPIDValue(str.channel);
            return;
        }else{
            re_printf("warning", "Cannot write to BMSPIDMASK%02d when BMSPIDMASKLOCK%02d is locked.\n", str.channel, str.channel);
        }
    }else{
        re_printf("warning", "Cannot write to %s register while key code is disabled.\n", register_name.c_str());
    }
    (*BMSPIDMSK[str.channel])["SPIDMSK"] = str.pre_data;
}//}}}

// Description: Callback function of SPID Mask Lock bit
// Ref: {SPIDCTL_DD_Registers_003, SPIDCTL_DD_Flow_008}
void SPIDCTL_Func::cb_BMSPIDMSKLOCK_LOCK(RegCBstr str)
{//{{{
    unsigned int lock_val = (unsigned int)(*BMSPIDMSKLOCK[str.channel])["LOCK"];
    if (0 == lock_val) {
        if (1 == str.pre_data) {
            // User cannot set LOCK to 0, only reset port of model can set LOCK to 0
            (*BMSPIDMSKLOCK[str.channel])["LOCK"] = 1;
        }
    } else {
        mpSPIDCTLAgentController->SetMaskLocked(str.channel, true);
    }
}//}}}

// Description: Callback function of SPID Key Code Enable bit
// Ref: {SPIDCTL_DD_Registers_004, SPIDCTL_DD_Flow_009}
void SPIDCTL_Func::cb_SPIDKCPROT_KCE(RegCBstr str)
{//{{{
    unsigned int kcprot_val = (unsigned int)(*SPIDKCPROT)["KCPROT"] << 1;
    if (VALID_KCPROT_VALUE == kcprot_val){
        unsigned int kce_val = (unsigned int)(*SPIDKCPROT)["KCE"];
        mpSPIDCTLAgentController->SetKeyCodeEnable((bool)kce_val);
    }else{
        (*SPIDKCPROT) = str.pre_data;
    }
    (*SPIDKCPROT)["KCPROT"] = 0;
}//}}}

// vim600: set foldmethod=marker :
