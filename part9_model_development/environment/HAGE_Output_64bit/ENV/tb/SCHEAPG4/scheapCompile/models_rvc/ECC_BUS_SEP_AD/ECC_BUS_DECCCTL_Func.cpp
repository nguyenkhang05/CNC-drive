// ---------------------------------------------------------------------
// $Id: ECC_BUS_DECCCTL_Func.cpp,v 1.5 2020/03/30 10:53:33 thunguyen3 Exp $
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
#include "ECC_BUS_DECCCTL_Func.h"
#include "ECC_BUS_SEP_AD_AgentController.h"
#ifndef func_re_printf
#define func_re_printf mpECC_BUS_SEP_ADAgentController->get_fileline(__FILE__, __LINE__); mpECC_BUS_SEP_ADAgentController->_re_printf
#endif//func_re_printf
//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
ECC_BUS_DECCCTL_Func::ECC_BUS_DECCCTL_Func (std::string name, ECC_BUS_SEP_AD_AgentController* ECC_BUS_SEP_ADAgentController, std::string err_kind):
                Cecc_bus_deccctl_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(ECC_BUS_SEP_ADAgentController != NULL);
    mpECC_BUS_SEP_ADAgentController = ECC_BUS_SEP_ADAgentController;
    mpECC_BUS_SEP_ADAgentController->CommandInit(name);
    mModuleName = name;
    mErr_Kind = err_kind;
    Cecc_bus_deccctl_regif::set_instance_name(name);
}//}}}

/// Destructor
ECC_BUS_DECCCTL_Func::~ECC_BUS_DECCCTL_Func()
{//{{{
}//}}}

/// Reset Func and registers
void ECC_BUS_DECCCTL_Func::EnableReset(const bool is_active)
{//{{{
    if (mErr_Kind != "AA") {
        Cecc_bus_deccctl_regif::EnableReset(is_active);
    }
}//}}}

/// Read API (target socket)
void ECC_BUS_DECCCTL_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    // Zero clock or reset active
    if ((!mpECC_BUS_SEP_ADAgentController->CheckClockPeriod()) || (mpECC_BUS_SEP_ADAgentController->GetResetStatus())) {
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
    if (mErr_Kind != "AA") {
        unsigned int size = (unsigned int)trans.get_data_length();
        memset(trans.get_data_ptr(), 0, size);
        
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

/// Write API (target socket)
void ECC_BUS_DECCCTL_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (!mpECC_BUS_SEP_ADAgentController->CheckClockPeriod()) {
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
    if (mErr_Kind != "AA") {
        unsigned int address = (unsigned int)trans.get_address();

        if (!debug) {
            reg_wr(address, trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg(address, trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

/// Handler for register IF
void ECC_BUS_DECCCTL_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    if (mErr_Kind != "AA") {
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
                    msg_return = Cecc_bus_deccctl_regif::reg_handle_command(cmd);
                    if (msg_return != "") {
                        SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                    }
                    cmd.clear();
                    reg_p = next_reg_object();
                }
            } else {//Set handle command to Catu5_timera_fcc2_regif class
                msg_return = Cecc_bus_deccctl_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
            }
        }
    }
}//}}}

/// Get DECCCTL.ECCDIS value
unsigned int ECC_BUS_DECCCTL_Func::GetDECCDIS()
{//{{{
    return (unsigned int)(*DECCCTL)["ECCDIS"];
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of cb_DECCCTL_ECCDIS
void ECC_BUS_DECCCTL_Func::cb_DECCCTL_ECCDIS(RegCBstr str)
{//{{{ 
    if (mpECC_BUS_SEP_ADAgentController->GetKeyOpen()){
        mpECC_BUS_SEP_ADAgentController->WriteDisPortDECCTL((unsigned int)(*DECCCTL));
    } else {
        (*DECCCTL) = str.pre_data;
    }
}//}}}

/// Callback function of cb_DECCTSTCTL_DATSEL
void ECC_BUS_DECCCTL_Func::cb_DECCTSTCTL_DATSEL(RegCBstr str)
{//{{{ 
    if (!(mpECC_BUS_SEP_ADAgentController->GetKeyOpen())){
        (*DECCTSTCTL) = str.pre_data;
    }
}//}}}

/// Callback function of cb_DECCTSTDIN0_DATA
void ECC_BUS_DECCCTL_Func::cb_DECCTSTDIN0_DATA(RegCBstr str)
{//{{{
    if (!(mpECC_BUS_SEP_ADAgentController->GetKeyOpen())){
        (*DECCTSTDIN0) = str.pre_data;
    }
}//}}}

/// Callback function of cb_DECCTSTDIN1_DATA
void ECC_BUS_DECCCTL_Func::cb_DECCTSTDIN1_DATA(RegCBstr str)
{//{{{
    if (!(mpECC_BUS_SEP_ADAgentController->GetKeyOpen())){
        (*DECCTSTDIN1) = str.pre_data;
    }
}//}}}

/// Callback function of cb_DECCTSTEIN_ECC
void ECC_BUS_DECCCTL_Func::cb_DECCTSTEIN_ECC(RegCBstr str)
{//{{{
    if (!(mpECC_BUS_SEP_ADAgentController->GetKeyOpen())){
        (*DECCTSTEIN) = str.pre_data;
    }
}//}}}

// vim600: set foldmethod=marker :
