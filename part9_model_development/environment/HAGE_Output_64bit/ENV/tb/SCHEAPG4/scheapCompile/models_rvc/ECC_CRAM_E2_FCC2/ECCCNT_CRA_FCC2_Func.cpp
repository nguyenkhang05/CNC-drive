// ---------------------------------------------------------------------
// $Id: ECCCNT_CRA_FCC2_Func.cpp,v 1.8 2019/09/26 10:07:11 landang Exp $
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
#include "ECCCNT_CRA_FCC2_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
ECCCNT_CRA_FCC2_Func::ECCCNT_CRA_FCC2_Func (std::string name, ECC_CRAM_E2_FCC2_AgentController* CRAMAgentController):
                Cecccnt_cra_fcc2_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(CRAMAgentController != NULL);
    mpCRAMAgentController = CRAMAgentController;
    mpCRAMAgentController->CommandInit(name);
    mModuleName = name;

    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x0, "CRAECCCTL"));

    Cecccnt_cra_fcc2_regif::set_instance_name(name);
}//}}}

/// Destructor
ECCCNT_CRA_FCC2_Func::~ECCCNT_CRA_FCC2_Func()
{//{{{
}//}}}

/// Reset Func and registers
void ECCCNT_CRA_FCC2_Func::EnableReset(const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "sys_resetz"){
        Cecccnt_cra_fcc2_regif::EnableReset(is_active);
    }
}//}}}

/// Read API (target socket)
void ECCCNT_CRA_FCC2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);

    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    // Zero clock or reset active
    if ((false == mpCRAMAgentController->CheckClockPeriod("pclk"))
            || (true == (mpCRAMAgentController->GetResetStatus("presetn")))
            || (true == (mpCRAMAgentController->GetResetStatus("sys_resetz")))) {
        // Software debugger
        if ((true == debug) && (extension != NULL) && (DEBUG_MASTER_BMID == extension->getPeId())) {
            reg_rd_dbg((unsigned int) trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
        }
    } else {
        if (!debug) {
            reg_rd((unsigned int) trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int) trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

/// Write API (target socket)
void ECCCNT_CRA_FCC2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (mpCRAMAgentController->GetResetStatus("presetn") || mpCRAMAgentController->GetResetStatus("sys_resetz")){
        re_printf("warning", "Cannot access register during reset period.\n");
        return;
    }
    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);
        // Zero clock
    if (false == mpCRAMAgentController->CheckClockPeriod("pclk")) {
        // Software debugger
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }

    unsigned int address = (unsigned int)trans.get_address();
    unsigned int masked_addr = address & 0xF;

    // Check protected or not
    bool keyopen = mpCRAMAgentController->GetKeyOpen();
    std::map<unsigned int, std::string>::iterator it;
    for (it = mProtectedRegMap.begin(); it != mProtectedRegMap.end(); it++){
        if ((masked_addr == it->first) && (!keyopen)){
            re_printf("warning", "Cannot write to %s register while keyopen is locked.\n", (it->second).c_str());
            return;
        }
    }

    if (!debug) {
        reg_wr(address, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg(address, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Handler for register IF
void ECCCNT_CRA_FCC2_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cecccnt_cra_fcc2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_fcc2_regif class
            msg_return = Cecccnt_cra_fcc2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Get value of ecc control register
unsigned int ECCCNT_CRA_FCC2_Func::GetCRAECCCTLReg()
{//{{{
    return (unsigned int)(*CRAECCCTL);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of CFPSECCTL_SECDIS bit
void ECCCNT_CRA_FCC2_Func::cb_CRAECCCTL_ECCDIS(RegCBstr str)
{//{{{ 
    mpCRAMAgentController->WriteDisPort_CRA((unsigned int)(*CRAECCCTL));
}//}}}
// vim600: set foldmethod=marker :
