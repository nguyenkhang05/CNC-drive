// ---------------------------------------------------------------------
// $Id: ERRCTRL.cpp,v 1.3 2014/10/08 03:05:00 binhnguyen Exp $
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

#include <cstdarg>
#include "ERRCTRL.h"
#include "PE_ERR_CTRL_AgentController.h"
#include "ERR_INFO.h"

///Constructor of ERRCTRL class
ERRCTRL::ERRCTRL (std::string name,
                 PE_ERR_CTRL_AgentController *AgentControllerPtr):
                 Cerrctrl_regif(name,32),
                 BusSlaveFuncIf()
{//{{{
    assert(AgentControllerPtr != NULL);
    mModuleName = name;
    mAgentController = AgentControllerPtr;
    mAgentController->CommandInit(name);
    Cerrctrl_regif::set_instance_name(name);
}//}}}

///Destructor of ERRCTRL class
ERRCTRL::~ERRCTRL (void)
{//{{{
}//}}}

///Reset registers and variables
void ERRCTRL::Reset (const std::string rst_name, const bool is_active)
{//{{{
    if (rst_name == "presetz") {
        Cerrctrl_regif::EnableReset(is_active);
    }
}//}}}

///Dump information of registers
void ERRCTRL::DumpStatusInfo (void)
{//{{{
    mAgentController->print_msg("info","ERR  : 0x%X\n", (unsigned int)(*PGERRSTAT)["ERR"]);
    mAgentController->print_msg("info","OVF  : 0x%X\n", (unsigned int)(*PGERRSTAT)["OVF"]);
    mAgentController->print_msg("info","SPID : 0x%X\n", (unsigned int)(*PGERRINFO)["SPID"]);
    mAgentController->print_msg("info","PEID : 0x%X\n", (unsigned int)(*PGERRINFO)["PEID"]);
}//}}}

///Set parameters to apberrslv_uimpl_regif class
void ERRCTRL::RegisterHandler (const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {
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
                msg_return = Cerrctrl_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Cerrctrl_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

void ERRCTRL::RecordErrTransInfo(const Cerrinfo err_info)
{//{{{
    if (!mAgentController->GetResetStatus("presetz")) {
        if ((unsigned int)(*PGERRSTAT)["ERR"] == 0) {
            (*PGERRSTAT)["ERR"] = 1;
            (*PGERRINFO)["SPID"] = err_info.SPID;
            (*PGERRINFO)["PEID"] = err_info.PEID; 
        } else {
            (*PGERRSTAT)["OVF"] = 1;
        }
    }
    else{
        re_printf("info","Error information is not recorded during reset period (presetz).\n");
    }
}//}}}

///Access read to registers
void ERRCTRL::read (unsigned int offsetAddress, 
                            TlmBasicPayload& trans, 
                            BusTime_t* t, 
                            bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}//}}}

///Access write to registers
void ERRCTRL::write (unsigned int offsetAddress, 
                             TlmBasicPayload& trans, 
                             BusTime_t* t, 
                             bool debug)
{//{{{
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}//}}}

///Clear error status
void ERRCTRL::cb_PGERRSTATCTL_CLRE(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock("pclk")) {
        if ((unsigned int)(*PGERRSTATCTL)["CLRE"] == 1) {
            (*PGERRSTAT)["ERR"] = 0;
        }
        if ((unsigned int)(*PGERRSTATCTL)["CLRO"] == 1) {
            (*PGERRSTAT)["OVF"] = 0;
        }
    } else {
        mAgentController->print_msg("error","pclk frequency is zero.\n");
    }
    *PGERRSTATCTL = 0;
}//}}}
// vim600: set foldmethod=marker :
