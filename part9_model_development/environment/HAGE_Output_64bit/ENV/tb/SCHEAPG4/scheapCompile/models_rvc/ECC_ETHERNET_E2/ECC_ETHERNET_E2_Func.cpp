// ---------------------------------------------------------------------
// $Id: ECC_ETHERNET_E2_Func.cpp,v 1.5 2019/09/26 10:07:23 landang Exp $
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
#include "ECC_ETHERNET_E2_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
ECC_ETHERNET_E2_Func::ECC_ETHERNET_E2_Func (std::string name, ECC_ETHERNET_E2_AgentController* ETHERNETAgentController):
                Cecc_ethernet_e2_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(ETHERNETAgentController != NULL);
    mpETHERNETAgentController = ETHERNETAgentController;
    mpETHERNETAgentController->CommandInit(name);
    mModuleName = name;

    Cecc_ethernet_e2_regif::set_instance_name(name);
}//}}}

/// Destructor
ECC_ETHERNET_E2_Func::~ECC_ETHERNET_E2_Func()
{//{{{
}//}}}

/// Reset Func and registers
void ECC_ETHERNET_E2_Func::EnableReset(const bool is_active)
{//{{{
    Cecc_ethernet_e2_regif::EnableReset(is_active);
}//}}}

/// Read API (target socket)
void ECC_ETHERNET_E2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int size = (unsigned int)trans.get_data_length();
    unsigned int address = (unsigned int)trans.get_address();
    memset(trans.get_data_ptr(), 0, size);

    /// Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    /// get status of reset
    bool mResetStatus = mpETHERNETAgentController->GetResetStatus(); 
    /// When clock == 0 or assert reset, only bebug master read.
    if (!(mpETHERNETAgentController->CheckClockPeriod()) || mResetStatus) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId( ) != DEBUG_MASTER_BMID)) {
            if (mResetStatus) { /// during reset
                re_printf("warning", "Cannot access register during reset period \n");
            } else {
                re_printf("warning", "Cannot access register when clock is 0.\n");
            }
            return;
        }
    }

    if (!debug) {
        reg_rd(address, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg(address, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Write API (target socket)
void ECC_ETHERNET_E2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (mpETHERNETAgentController->GetResetStatus()){
        re_printf("warning", "Cannot access register during reset period.\n");
        return;
    }
    /// Get extension infor
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    
    /// When clock = 0, only bebug master write.
    if (! (mpETHERNETAgentController->CheckClockPeriod())) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
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
void ECC_ETHERNET_E2_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cecc_ethernet_e2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cecc_ethernet_e2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of cb_ExCTL_ECER1C
void ECC_ETHERNET_E2_Func::cb_ETCTXECCCTL_EC1ECP(RegCBstr str)
{//{{{
    bool value = (bool)(unsigned int)(*ETCTXECCCTL)["EC1ECP"];
    mpETHERNETAgentController->WriteTXDisPort(value);
}//}}} 

/// Callback function of cb_ExCTL_ECTHM
void ECC_ETHERNET_E2_Func::cb_ETCRXECCCTL_EC1ECP(RegCBstr str)
{//{{ {
    bool value = (bool)(unsigned int)(*ETCRXECCCTL)["EC1ECP"];
    mpETHERNETAgentController->WriteRXDisPort(value);
}//}}}

// vim600: set foldmethod=marker :
