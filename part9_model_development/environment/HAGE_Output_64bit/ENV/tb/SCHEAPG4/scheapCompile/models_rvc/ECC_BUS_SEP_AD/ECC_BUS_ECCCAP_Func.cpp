// ---------------------------------------------------------------------
// $Id: ECC_BUS_ECCCAP_Func.cpp,v 1.10 2020/03/30 10:53:33 thunguyen3 Exp $
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
#include "ECC_BUS_ECCCAP_Func.h"
#include "ECC_BUS_SEP_AD_AgentController.h"
#ifndef func_re_printf
#define func_re_printf mpECC_BUS_SEP_ADAgentController->get_fileline(__FILE__, __LINE__); mpECC_BUS_SEP_ADAgentController->_re_printf
#endif//func_re_printf

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
ECC_BUS_ECCCAP_Func::ECC_BUS_ECCCAP_Func (std::string name, ECC_BUS_SEP_AD_AgentController* ECC_BUS_SEP_ADAgentController, std::string err_kind):
    Cecc_bus_ecccap_regif(name, 32, err_kind)
    ,BusSlaveFuncIf()
{//{{{
    assert(ECC_BUS_SEP_ADAgentController != NULL);
    mpECC_BUS_SEP_ADAgentController = ECC_BUS_SEP_ADAgentController;
    mpECC_BUS_SEP_ADAgentController->CommandInit(name);
    mModuleName = name;
    Cecc_bus_ecccap_regif::set_instance_name(name);
    mErr_Kind = err_kind;
}//}}}

/// Destructor
ECC_BUS_ECCCAP_Func::~ECC_BUS_ECCCAP_Func()
{//{{{
}//}}}

/// Reset Func and registers
void ECC_BUS_ECCCAP_Func::EnableReset(const bool is_active)
{//{{{
    Cecc_bus_ecccap_regif::EnableReset(is_active);
}//}}}

/// Read API (target socket)
void ECC_BUS_ECCCAP_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
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
    unsigned int size = (unsigned int)trans.get_data_length();
    memset(trans.get_data_ptr(), 0, size);

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Write API (target socket)
void ECC_BUS_ECCCAP_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
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
    unsigned int address = (unsigned int)trans.get_address();

    if (!debug) {
        reg_wr(address, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg(address, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Handler for register IF
void ECC_BUS_ECCCAP_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cecc_bus_ecccap_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_fcc2_regif class
            msg_return = Cecc_bus_ecccap_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Process error of data
void ECC_BUS_ECCCAP_Func::ProcessDataError(ErrorInfo data_error)
{//{{{
    unsigned int bit_index = data_error.edl_code;
    // 1 bit error
    if (data_error.type == em1bitError){
        unsigned int pre_status = (unsigned int)(*DSERSTR);
        (*DSERSTR) = pre_status | (1 << bit_index); // Set 1 at bit index (edl)
        if ( (((pre_status >> bit_index) & 0x1) == 0) && ((unsigned int)(*ERRINT)["DSEDIE"] == 1) ){ // raise interrupt if pre value is 0 and ERRINT.DSEDIE = 1
            mpECC_BUS_SEP_ADAgentController->AssertInterrupt("DS");
        }

        // 2 bits error
    }else{
        unsigned int pre_status = (unsigned int)(*DDERSTR);
        (*DDERSTR) = pre_status | (1 << bit_index); // Set 1 at bit index (edl)
        if ( (((pre_status >> bit_index) & 0x1) == 0) && ((unsigned int)(*ERRINT)["DDEDIE"] == 1) ){ // raise interrupt if pre value is 0 and ERRINT.DDEDIE = 1
            mpECC_BUS_SEP_ADAgentController->AssertInterrupt("DD");
        }
    }
}//}}}

/// Process error of address
void ECC_BUS_ECCCAP_Func::ProcessAddressError(ErrorInfo address_error)
{//{{{ 
    unsigned int bit_index = address_error.edl_code;
    // 1 bit error
    if (address_error.type == em1bitAddressError){
        unsigned int pre_status = (unsigned int)(*ASERSTR);
        (*ASERSTR) = pre_status | (1 << bit_index); // Set 1 at bit index (edl)
        if ( (((pre_status >> bit_index) & 0x1) == 0) && ((unsigned int)(*ERRINT)["ASEDIE"] == 1) ){ // raise interrupt if pre value is 0 and ERRINT.ASEDIE = 1
            mpECC_BUS_SEP_ADAgentController->AssertInterrupt("AS");
        }

        // 2 bits error
    }else{
        unsigned int pre_status = (unsigned int)(*ADERSTR);
        (*ADERSTR) = pre_status | (1 << bit_index); // Set 1 at bit index (edl)
        if ( (((pre_status >> bit_index) & 0x1) == 0) && ((unsigned int)(*ERRINT)["ADEDIE"] == 1) ){ // raise interrupt if pre value is 0 and ERRINT.ADEDIE = 1
            mpECC_BUS_SEP_ADAgentController->AssertInterrupt("AD");
        }
    }
}//}}}

/// Dump status info
void ECC_BUS_ECCCAP_Func::DumpStatusInfo()
{//{{{
    printf("Status info:\n");
    if(mErr_Kind != "DD") {
        printf("    ASERSTR = %08X\n", (unsigned int)(*ASERSTR));
        printf("    ADERSTR = %08X\n", (unsigned int)(*ADERSTR));
    }
    if(mErr_Kind != "AA") {
        printf("    DSERSTR = %08X\n", (unsigned int)(*DSERSTR));
        printf("    DDERSTR = %08X\n", (unsigned int)(*DDERSTR));
    }
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of cb_ERRINT_ADEDIE
void ECC_BUS_ECCCAP_Func::cb_ERRINT_ADEDIE(RegCBstr str) 
{//{{{ 
    if (!(mpECC_BUS_SEP_ADAgentController->GetKeyOpen())){
        (*ERRINT) = str.pre_data;
    }
}//}}}

/// Callback function of cb_ERRINT_DSEDIE
void ECC_BUS_ECCCAP_Func::cb_ERRINT_DSEDIE(RegCBstr str)
{//{{{ 
    if (!(mpECC_BUS_SEP_ADAgentController->GetKeyOpen())){
        (*ERRINT) = str.pre_data;
    } 
}//}}}

/// Callback function of cb_ASSTCLR_ASSTCLR
void ECC_BUS_ECCCAP_Func::cb_ASSTCLR_ASSTCLR(RegCBstr str)
{//{{{
    unsigned int clear_val = (unsigned int)(*ASSTCLR);
    (*ASERSTR) = ~clear_val & (unsigned int)(*ASERSTR);
    (*ASSTCLR) = 0;
}//}}}

/// Callback function of cb_ADSTCLR_ADSTCLR
void ECC_BUS_ECCCAP_Func::cb_ADSTCLR_ADSTCLR(RegCBstr str)
{//{{{ 
    unsigned int clear_val = (unsigned int)(*ADSTCLR);
    (*ADERSTR) = ~clear_val & (unsigned int)(*ADERSTR);
    (*ADSTCLR) = 0;
}//}}}

/// Callback function of cb_DSSTCLR_DSSTCLR
void ECC_BUS_ECCCAP_Func::cb_DSSTCLR_DSSTCLR(RegCBstr str)
{//{{{
    unsigned int clear_val = (unsigned int)(*DSSTCLR);
    (*DSERSTR) = ~clear_val & (unsigned int)(*DSERSTR);
    (*DSSTCLR) = 0;
}//}}}

/// Callback function of cb_DDSTCLR_DDSTCLR
void ECC_BUS_ECCCAP_Func::cb_DDSTCLR_DDSTCLR(RegCBstr str)
{//{{{
    unsigned int clear_val = (unsigned int)(*DDSTCLR);
    (*DDERSTR) = ~clear_val & (unsigned int)(*DDERSTR);
    (*DDSTCLR) = 0;
}//}}}

// vim600: set foldmethod=marker :
