// ---------------------------------------------------------------------
// $Id: ECC_APB_Func.cpp,v 1.8 2018/02/08 04:09:17 chanle Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "ECC_APB_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
ECC_APB_Func::ECC_APB_Func (std::string name, std::string kind, ECC_APB_AgentController* APBAgentController):
                Cecc_apb_regif(name, kind, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(APBAgentController != NULL);
    assert ((kind == "CF_A")
        || (kind == "IFU_D") 
        || (kind == "LSUS_DA") || (kind == "LSUM_D") 
        || (kind == "SGn_D") || (kind == "SG7_DA") 
        || (kind == "PDMA_D") 
        || (kind == "APEC0_DA") || (kind == "APECn_DA"));
    mpECCAPBAgentController = APBAgentController;
    mpECCAPBAgentController->CommandInit(name);
    mModuleName = name;
    mKind = kind;

    Cecc_apb_regif::set_instance_name(name);
}//}}}

/// Destructor
ECC_APB_Func::~ECC_APB_Func()
{//{{{
}//}}}

/// Reset Func and registers
void ECC_APB_Func::EnableReset(const bool is_active)
{//{{{
    Cecc_apb_regif::EnableReset(is_active);
}//}}}

/// Read API (target socket)
void ECC_APB_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int size = (unsigned int)trans.get_data_length();
    unsigned int address = (unsigned int)trans.get_address();
    memset(trans.get_data_ptr(), 0, size);

    if (!debug) {
        reg_rd(address, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg(address, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Write API (target socket)
void ECC_APB_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (! (mpECCAPBAgentController->CheckClockPeriod()) ){
        re_printf("warning", "Cannot access register when clock is 0.\n");
        return;
    }

    unsigned int address = (unsigned int)(trans.get_address());

    if (!debug) {
        reg_wr(address, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg(address, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Handler for register IF
void ECC_APB_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cecc_apb_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cecc_apb_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Process error of data
void ECC_APB_Func::ProcessDataError(ErrorInfo data_error)
{//{{{
    unsigned int erstr = (unsigned int)(*ERSTR);
    unsigned int sedf = erstr & 0x1;
    unsigned int dedf = (erstr >> 1) & 0x1;
    unsigned int asedf = (erstr >> 6) & 0x1;
    unsigned int adedf = (erstr >> 7) & 0x1;
    unsigned int captured_address = (unsigned int)(*EADR);
    unsigned int pre_ovf = (unsigned int)(*OVFSTR)["ERROVF"];
    // 1 bit error
    if (data_error.type == em1bitError){
        // Update overflow bit if different address or different error detect
        if ( (captured_address != data_error.address) && (captured_address != 0) ){
            (*OVFSTR)["ERROVF"] = 1;
        }
        if ( (dedf == 1) || (asedf == 1) || (adedf == 1) ){
            (*OVFSTR)["ERROVF"] = 1;
        }
        // Store address if no any error before
        if ( erstr == 0 ){
            (*EADR) = data_error.address;
        }
        // Update status bit
        unsigned int pre_sedf = (unsigned int)(*ERSTR)["SEDF"];
        (*ERSTR)["SEDF"] = 1;
        // Assert interrupt if enabled
        if ( (pre_sedf == 0) && ((unsigned int)(*ERRINT)["SEDIE"] == 1) ){
            mpECCAPBAgentController->AssertInterrupt("SEDF");
        }

    // 2 bits error
    }else{
        // Update overflow bit if different address or different error detect
        if ( (captured_address != data_error.address) && (captured_address != 0) ){
            (*OVFSTR)["ERROVF"] = 1;
        }
        if ( (sedf == 1) || (asedf == 1) || (adedf == 1) ){
            (*OVFSTR)["ERROVF"] = 1;
        }
        // Store address if no 2bit error before
        if ( (dedf == 0) && (adedf == 0) ){
            (*EADR) = data_error.address;
        }
        // Update status bit
        unsigned int pre_dedf = (unsigned int)(*ERSTR)["DEDF"];
        (*ERSTR)["DEDF"] = 1;
        // Assert interrupt if enabled
        if ( (pre_dedf == 0) && ((unsigned int)(*ERRINT)["DEDIE"] == 1) ){
            mpECCAPBAgentController->AssertInterrupt("DEDF");
        }
    }
    // Process interrupt for overflow
    if ( (pre_ovf == 0) && ((unsigned int)(*OVFSTR)["ERROVF"] == 1) ){
        mpECCAPBAgentController->AssertInterrupt("ERROVF");
    }
}//}}}

/// Process error of address
void ECC_APB_Func::ProcessAddressError(ErrorInfo address_error)
{//{{{ 
    unsigned int erstr = (unsigned int)(*ERSTR);
    unsigned int sedf = erstr & 0x1;        // NOTE: not get value from bit ex: (*ERSTR)["SEDF"] because this bit exists or not based on the ECC_APB kind
    unsigned int dedf = (erstr >> 1) & 0x1; // Same NOTE above
    unsigned int asedf = (erstr >> 6) & 0x1;// Same NOTE above
    unsigned int adedf = (erstr >> 7) & 0x1;// Same NOTE above
    unsigned int captured_address = (unsigned int)(*EADR);
    unsigned int pre_ovf = (unsigned int)(*OVFSTR)["ERROVF"];
    // 1 bit error
    if (address_error.type == em1bitAddressError){
        // Update overflow bit if different address or different error detect
        if ( (captured_address != address_error.address) && (captured_address != 0) ){
            (*OVFSTR)["ERROVF"] = 1;
        }
        if ( (adedf == 1) || (sedf == 1) || (dedf == 1) ){
            (*OVFSTR)["ERROVF"] = 1;
        }
        // Store address if no any error before
        if ( erstr == 0 ){
            (*EADR) = address_error.address;
        }
        // Update status bit
        unsigned int pre_asedf = (unsigned int)(*ERSTR)["ASEDF"];
        (*ERSTR)["ASEDF"] = 1;
        // Assert interrupt if enabled
        if ( (pre_asedf == 0) && ((unsigned int)(*ERRINT)["ASEDIE"] == 1) ){
            mpECCAPBAgentController->AssertInterrupt("ASEDF");
        }

    // 2 bits error
    }else{
        // Update overflow bit if different address or different error detect
        if ( (captured_address != address_error.address) && (captured_address != 0) ){
            (*OVFSTR)["ERROVF"] = 1;
        }
        if ( (asedf == 1) || (sedf == 1) || (dedf == 1) ){
            (*OVFSTR)["ERROVF"] = 1;
        }
        // Store address if no 2bit error before
        if ( (dedf == 0) && (adedf == 0) ){
            (*EADR) = address_error.address;
        }
        // Update status bit
        unsigned int pre_adedf = (unsigned int)(*ERSTR)["ADEDF"];
        (*ERSTR)["ADEDF"] = 1;
        // Assert interrupt if enabled
        if ( (pre_adedf == 0) && ((unsigned int)(*ERRINT)["ADEDIE"] == 1) ){
            mpECCAPBAgentController->AssertInterrupt("ADEDF");
        }
    }
    // Process interrupt for overflow
    if ( (pre_ovf == 0) && ((unsigned int)(*OVFSTR)["ERROVF"] == 1) ){
        mpECCAPBAgentController->AssertInterrupt("ERROVF");
    }
}//}}}

void ECC_APB_Func::DumpStatusInfo()
{//{{{
    printf("Status info:\n");
    printf("    OVFSTR = %08X\n", (unsigned int)(*OVFSTR));
    printf("    ERSTR  = %08X\n", (unsigned int)(*ERSTR));
    printf("    EADR   = %08X\n", (unsigned int)(*EADR));
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of cb_ECCCTL_PROT
void ECC_APB_Func::cb_ECCCTL_PROT(RegCBstr str)
{//{{{ 
    if ((unsigned int)(*ECCCTL)["PROT"] == 1){
        (*ECCCTL)["PROT"] = 0;
        if (mKind == "LSUS_DA"){
            unsigned int eccdis = (unsigned int)(*ECCCTL)["ECCDIS"] & 0x1;
            unsigned int secdis = (unsigned int)(*ECCCTL)["SECDIS"] & 0x1;
            unsigned int disval = (unsigned int)(*ECCCTL) | (secdis << 3) | (eccdis << 2);
            mpECCAPBAgentController->WriteDisablePort(disval);
        }else{
            mpECCAPBAgentController->WriteDisablePort((unsigned int)(*ECCCTL));
        }
    }else{
        (*ECCCTL) = str.pre_data;
    }
}//}}} 

/// Callback function of cb_ERSTCLR_SSTCLR
void ECC_APB_Func::cb_ERSTCLR_SSTCLR(RegCBstr str)
{//{{{
    // Clear all status, address
    if ((unsigned int)(*ERSTCLR)["SSTCLR"] == 1){
        (*ERSTR) = 0;
        (*OVFSTR) = 0;
        (*EADR) = 0;
    }

    // Always read 0
    (*ERSTCLR) = 0;
}//}}}

// vim600: set foldmethod=marker :
