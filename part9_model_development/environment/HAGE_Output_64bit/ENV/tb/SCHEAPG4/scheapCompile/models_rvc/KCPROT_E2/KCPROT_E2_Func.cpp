// ---------------------------------------------------------------------
// $Id: KCPROT_E2_Func.cpp,v 1.5 2019/09/26 10:07:47 landang Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "KCPROT_E2_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
KCPROT_E2_Func::KCPROT_E2_Func (std::string name, KCPROT_E2_AgentController* KCPROTAgentController):
                Ckcprot_e2_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(KCPROTAgentController != NULL);
    mpKCPROTAgentController = KCPROTAgentController;
    mpKCPROTAgentController->CommandInit(name);
    mModuleName = name;
    Ckcprot_e2_regif::set_instance_name(name);
}//}}}

/// Destructor
KCPROT_E2_Func::~KCPROT_E2_Func()
{//{{{
}//}}}

/// Reset Func and registers
void KCPROT_E2_Func::EnableReset(const bool is_active)
{//{{{
    Ckcprot_e2_regif::EnableReset(is_active);
}//}}}

/// Read API (target socket)
void KCPROT_E2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int size = (unsigned int)trans.get_data_length();
    memset(trans.get_data_ptr(), 0, size);
    /// Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    /// get status of reset
    bool mResetStatus = mpKCPROTAgentController->GetResetStatus(); 
    /// When clock == 0 or assert reset, only bebug master read.
    if (! (mpKCPROTAgentController->CheckClockPeriod()) || mResetStatus) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId( ) != DEBUG_MASTER_BMID)) {
            if (mResetStatus) { /// during reset
                re_printf("warning", "Cannot read during reset period\n");
            } else {
                re_printf("warning", "Cannot access register when clock is 0.\n");
            }
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
void KCPROT_E2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (mpKCPROTAgentController->GetResetStatus()){
        re_printf("warning", "Cannot access register during reset period.\n");
        return;
    }
    /// Get extension infor
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    
    /// When clock = 0, only bebug master write.
    if (! (mpKCPROTAgentController->CheckClockPeriod())) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Handler for register IF
void KCPROT_E2_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Ckcprot_e2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Ckcprot_e2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of IPInFCLRm register
void KCPROT_E2_Func::cb_KCPROT_KCE(RegCBstr str)
{//{{{ 
    unsigned int written_value = (unsigned int)(*KCPROT)["KCPROT"] << 1;// left shift to same with KEYCODE
    unsigned int keycode = mpKCPROTAgentController->GetKeyCode();
    if (written_value == keycode){
        unsigned int kce_val = (unsigned int)(*KCPROT)["KCE"];
        mpKCPROTAgentController->UpdateKeyCodeEnable((bool)kce_val);
    }else{
        (*KCPROT) = str.pre_data;
    }
    (*KCPROT)["KCPROT"] = 0;
}//}}}
// vim600: set foldmethod=marker :
