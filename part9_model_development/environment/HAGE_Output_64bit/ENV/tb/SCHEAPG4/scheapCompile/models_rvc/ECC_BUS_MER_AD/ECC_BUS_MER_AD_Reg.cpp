// ---------------------------------------------------------------------
// $Id: ECC_BUS_MER_AD_Reg.cpp,v 1.5 2020/04/01 06:41:19 thunguyen3 Exp $
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
#include "ECC_BUS_MER_AD_Reg.h"
#include "ECC_BUS_MER_AD_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
ECC_BUS_MER_AD_Reg::ECC_BUS_MER_AD_Reg (std::string name, ECC_BUS_MER_AD_Func* parent_func, ECC_BUS_MER_AD_AgentController* ECC_BUS_MER_ADAgentController):
                Cecc_bus_mer_ad_regif(name, 32)
{//{{{
    assert(ECC_BUS_MER_ADAgentController != NULL);
    mpECC_BUS_MER_ADAgentController = ECC_BUS_MER_ADAgentController;
    assert(parent_func != NULL);
    mpParentFunc = parent_func;
    mModuleName = name;

    Cecc_bus_mer_ad_regif::set_instance_name(name);
    Initialize();
}//}}}

/// Destructor
ECC_BUS_MER_AD_Reg::~ECC_BUS_MER_AD_Reg()
{//{{{
}//}}}

/// Reset Func and registers
void ECC_BUS_MER_AD_Reg::EnableReset(const bool is_active)
{//{{{
    Initialize();
    Cecc_bus_mer_ad_regif::EnableReset(is_active);
}//}}}

/// Initialize internal variables
void ECC_BUS_MER_AD_Reg::Initialize()
{//{{{
}//}}}

/// Read API (target socket)
void ECC_BUS_MER_AD_Reg::read (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
{//{{{
    if (p_data == NULL) {
        return;
    }
    if (!debug) {
        reg_rd(address, p_data, size);
    } else {
        reg_rd_dbg(address, p_data, size);
    }
}//}}}

/// Write API (target socket)
void ECC_BUS_MER_AD_Reg::write (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
{//{{{
    if (p_data == NULL) {
        return;
    }
    if (!debug) {
        reg_wr(address, p_data, size);
    } else {
        reg_wr_dbg(address, p_data, size);
    }
}//}}}

/// Handler for register IF
void ECC_BUS_MER_AD_Reg::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cecc_bus_mer_ad_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to regif class
            msg_return = Cecc_bus_mer_ad_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Process error of data
void ECC_BUS_MER_AD_Reg::ProcessDataError(ErrorInfo data_error)
{//{{{
    // 1 bit error
    if (data_error.type == em1bitError){
        // Update status bit
        unsigned int pre_sedf = (unsigned int)(*ERSTR)["SEDF0"];
        (*ERSTR)["SEDF0"] = 1;
        // Assert interrupt if enabled
        if ( (pre_sedf == 0) && ((unsigned int)(*ERRINT)["SEDIE"] == 1) ){
            mpParentFunc->AssertInterrupt("DS");
        }

    // 2 bits error
    }else{
        // Update status bit
        unsigned int pre_dedf = (unsigned int)(*ERSTR)["DEDF0"];
        (*ERSTR)["DEDF0"] = 1;
        // Assert interrupt if enabled
        if ( (pre_dedf == 0) && ((unsigned int)(*ERRINT)["DEDIE"] == 1) ){
            mpParentFunc->AssertInterrupt("DD");
        }
    }
}//}}}

/// Process error of address
void ECC_BUS_MER_AD_Reg::ProcessAddressError(ErrorInfo address_error)
{//{{{ 
    // 1 bit error
    if (address_error.type == em1bitAddressError){
        // Update status bit
        unsigned int pre_asedf = (unsigned int)(*ERSTR)["AECSEDF0"];
        (*ERSTR)["AECSEDF0"] = 1;
        // Assert interrupt if enabled
        if ( (pre_asedf == 0) && ((unsigned int)(*ERRINT)["ASEDIE"] == 1) ){
            mpParentFunc->AssertInterrupt("AS");
        }

    // 2 bits error
    }else{
        // Update status bit
        unsigned int pre_adedf = (unsigned int)(*ERSTR)["AECDEDF0"];
        (*ERSTR)["AECDEDF0"] = 1;
        // Assert interrupt if enabled
        if ( (pre_adedf == 0) && ((unsigned int)(*ERRINT)["ADEDIE"] == 1) ){
            mpParentFunc->AssertInterrupt("AD");
        }
    }
}//}}}

/// Dump status info
void ECC_BUS_MER_AD_Reg::DumpStatusInfo()
{//{{{
    printf("Status info:\n");
    printf("    ERSTR = %08X\n", (unsigned int)(*ERSTR));
}//}}}

/// Get ECCCTL register value
unsigned int ECC_BUS_MER_AD_Reg::GetECCCTLReg()
{//{{{
    return (unsigned int)(*ECCCTL);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of cb_ECCCTL_ECCDIS
void ECC_BUS_MER_AD_Reg::cb_ECCCTL_ECCDIS(RegCBstr str)
{//{{{
    if ((unsigned int)(*KCPROT)["KCE"] == 1){
        mpParentFunc->WriteDisablePort((unsigned int)(*ECCCTL));
    }else{
        (*ECCCTL) = str.pre_data;
    }
}//}}}

/// Callback function of cb_ERRINT_SEDIE
void ECC_BUS_MER_AD_Reg::cb_ERRINT_SEDIE(RegCBstr str)
{//{{{
    if ((unsigned int)(*KCPROT)["KCE"] != 1){
        (*ERRINT) = str.pre_data;
    }
}//}}}

/// Callback function of cb_STCLR_STCLR0
void ECC_BUS_MER_AD_Reg::cb_STCLR_STCLR0(RegCBstr str)
{//{{{
    if ((unsigned int)(*STCLR)["STCLR0"] == 1){
        (*ERSTR) = 0;
    }
    (*STCLR) = 0;
}//}}}

/// Callback function of cb_ECCCTL_PROT
void ECC_BUS_MER_AD_Reg::cb_KCPROT_KCE(RegCBstr str)
{//{{{
    unsigned int kcprot_val = (unsigned int)(*KCPROT)["KCPROT"];
    if ((kcprot_val << 1) == emKeyCode){
        unsigned int kce_val = (unsigned int)(*KCPROT)["KCE"];
        mpECC_BUS_MER_ADAgentController->UpdateKeyCodeEnable((bool)kce_val);
    } else {
        (*KCPROT) = str.pre_data & 0x1;
    }
    (*KCPROT)["KCPROT"] = 0;
}//}}} 

// vim600: set foldmethod=marker :
