// ---------------------------------------------------------------------
// $Id: ECC_DFLASH_E2_Func.cpp,v 1.8 2019/09/26 10:07:16 landang Exp $
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "ECC_DFLASH_E2_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
ECC_DFLASH_E2_Func::ECC_DFLASH_E2_Func (std::string name, ECC_DFLASH_E2_AgentController* ECC_DFLASH_E2AgentController):
                    Cecc_dflash_e2_regif(name, 32),
            BusSlaveFuncIf()
{//{{{
    assert(ECC_DFLASH_E2AgentController != NULL);
    mpECC_DFLASH_E2AgentController = ECC_DFLASH_E2AgentController;
    mpECC_DFLASH_E2AgentController->CommandInit(name);
    mModuleName = name;
    Cecc_dflash_e2_regif::set_instance_name(name);
    // Initialize
    Initialize();
    mIsAddrCaptureSupport = false;
    mPAWIDTH = 21;
}//}}}

/// Destructor
ECC_DFLASH_E2_Func::~ECC_DFLASH_E2_Func()
{//{{{
}//}}}

/// Initialize variables
void ECC_DFLASH_E2_Func::Initialize(void)
{//{{{
    mEnableWrProtReg = 0; // Locks protection registers
}//}}}

/// Reset Func and registers
void ECC_DFLASH_E2_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        Initialize();
    }
    Cecc_dflash_e2_regif::EnableReset(is_active);
}//}}}

/// Set SEDIE initial value
void ECC_DFLASH_E2_Func::SetSEDIEInitVal(const bool is_one)
{//{{{
    DFERRINT->UpdateBitInitValue("SEDIE", (unsigned int)is_one);
}//}}}

/// Address capture function availability
void ECC_DFLASH_E2_Func::IsAddrCaptureSupport(const bool is_support)
{//{{{
    mIsAddrCaptureSupport = is_support;
}//}}}

/// Read API (target socket)
void ECC_DFLASH_E2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{

    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);

    // Clear data buffer
    unsigned int full_addr = (unsigned int)trans.get_address();
    unsigned int addr = (unsigned int)trans.get_address() & 0x3F;
    unsigned int size = (unsigned int)trans.get_data_length();
    memset(trans.get_data_ptr(), 0, size);
    // Zero clock or reset active
    if ((false == mpECC_DFLASH_E2AgentController->GetClockPeriod())
            || (true == (mpECC_DFLASH_E2AgentController->GetResetStatus()))) {
        // Software debugger
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }

    if ((!mIsAddrCaptureSupport) && ((addr == 0x0C)     // DFOVFSTR
                                  || (addr == 0x10)     // DFOVFSTC
                                  || (addr == 0x18))) { // DFEADR
        re_printf("warning", "Should not read from the reservation area 0x%08X.\n", full_addr);
        return;
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Write API (target socket)
void ECC_DFLASH_E2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (mpECC_DFLASH_E2AgentController->GetResetStatus()){
        re_printf("warning", "Cannot access register during reset period.\n");
        return;
    }
    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);

    unsigned int full_addr = (unsigned int)trans.get_address();
    unsigned int addr = (unsigned int)trans.get_address() & 0x3F;

    // Locks protection registers
    if (((addr == 0x00) // DFECCCTL
      || (addr == 0x14) // DFERRINT
      || (addr == 0x1C) // DFTSTCTL
        ) && (!mEnableWrProtReg)) {
        re_printf("warning", "Cannot write to protection register 0x%08X when KCE is not active.\n", full_addr);
    // below registers are not supported when address capture function is turned off
    } else if ((!mIsAddrCaptureSupport) && ((addr == 0x10)     // DFOVFSTC
                                        || (addr == 0x0C)     // DFOVFSTR
                                        || (addr == 0x18))) { // DFEADR
        re_printf("warning", "Should not write to the reservation area 0x%08X.\n", full_addr);
    } else { // Normal
        // Zero clock
        if (false == mpECC_DFLASH_E2AgentController->GetClockPeriod()) {
            // Software debugger
            if ((true == debug) && (extension != NULL) && (DEBUG_MASTER_BMID == extension->getPeId())) {
                reg_wr_dbg((unsigned int) trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            } else {
                re_printf("warning", "Cannot access register when clock is 0.\n");
            }
        } else {
            if (!debug) {
                reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            }
        }
    }
}//}}}

/// Handler for register IF
void ECC_DFLASH_E2_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cecc_dflash_e2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cecc_dflash_e2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

//========================================================
//============ APIs               ========================
//========================================================
/// Error handler
void ECC_DFLASH_E2_Func::ErrorProcess(const unsigned int error_type, const unsigned int error_addr)
{//{{{
    unsigned int mask = ((1<<mPAWIDTH) - 1) >> 2;
    unsigned int error_addr_mod = (error_addr >> 2) & mask;
    
    unsigned int dfeadr = (unsigned int)(*DFEADR)["DFEADR"];
    unsigned int sedf = (unsigned int)(*DFERSTR)["SEDF"];
    unsigned int dedf = (unsigned int)(*DFERSTR)["DEDF"];

    bool set_ovf      = false;
    bool capture_addr = false;
    
    if (error_type == 0x0) { // ECC 1-bit error
    if ((sedf == 0) && (dedf == 0)) {
        (*DFERSTR)["SEDF"] = 1;
            capture_addr = true;
    } else if ((dedf == 1) || ((sedf == 1) && (error_addr_mod != dfeadr))) {
            set_ovf = true;
    } else {
    }

    if ((unsigned int)(*DFERRINT)["SEDIE"] == 1) {
        mpECC_DFLASH_E2AgentController->DriveInterrupt(emEccSed);
    }
    } else { // ECC 2-bit error
        if (dedf == 0) {
            (*DFERSTR)["DEDF"] = 1;
            capture_addr = true;
        if (sedf == 1) {
                set_ovf = true;
        }
    } else if (error_addr_mod != dfeadr) {
            set_ovf = true;
    } else {
    }

    if ((unsigned int)(*DFERRINT)["DEDIE"] == 1) {
        mpECC_DFLASH_E2AgentController->DriveInterrupt(emEccDed);
    }
    } 

    if (set_ovf == true) {
        (*DFOVFSTR)["ERROVF"] = 1;
        if ((unsigned int)(*DFERRINT)["EOVFIE"] == 1) {
            mpECC_DFLASH_E2AgentController->DriveInterrupt(emErrOvf);
        }
    }

    if (capture_addr == true) {
        (*DFEADR)["DFEADR"] = error_addr_mod;
    }
}//}}}

/// Get PAWIDTH value from ECC_DFLASH_E2 class
void ECC_DFLASH_E2_Func::GetPAWIDTHValue(const unsigned int pawidth)
{//{{{
    mPAWIDTH = pawidth;
}//}}}

/// Dump information of registers
void ECC_DFLASH_E2_Func::DumpStatusInfo(void)
{//{{{
    std::string type = "StatInfo";
    printf("PROFILE(%s): ECC_DFLASH_E2: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), mModuleName.c_str());
    printf("PROFILE(%s): ECC_DFLASH_E2:   SEDF: 0x%08X\n", type.c_str(), (unsigned int)(*DFERSTR)["SEDF"]);
    printf("PROFILE(%s): ECC_DFLASH_E2:   DEDF: 0x%08X\n", type.c_str(), (unsigned int)(*DFERSTR)["DEDF"]);
    if (mIsAddrCaptureSupport) {
        printf("PROFILE(%s): ECC_DFLASH_E2:   ERROVF: 0x%08X\n", type.c_str(), (unsigned int)(*DFOVFSTR)["ERROVF"]);
        printf("PROFILE(%s): ECC_DFLASH_E2:   DFEADR: 0x%08X\n", type.c_str(), (unsigned int)(*DFEADR)["DFEADR"]);
    }
}//}}}


//========================================================
//============ Callback function  ========================
//========================================================
void ECC_DFLASH_E2_Func::cb_DFECCCTL_ECCDIS(RegCBstr str)
{//{{{
    if (mpECC_DFLASH_E2AgentController->GetClockPeriod() != 0) {
        unsigned int eccdis = (unsigned int)(*DFECCCTL)["ECCDIS"];
        unsigned int seddis = (unsigned int)(*DFECCCTL)["SEDDIS"];
        unsigned int pre_data = (unsigned int)str.pre_data;
        unsigned int pre_eccdis = pre_data & 0x1;
        unsigned int pre_seddis = (pre_data >> 1) & 0x1;

        if (eccdis != pre_eccdis) {
            mpECC_DFLASH_E2AgentController->NotifyEccStatus(emEccDisable, (bool)eccdis);
        } else {
            // No operation
        }

        if (seddis != pre_seddis) {
            mpECC_DFLASH_E2AgentController->NotifyEccStatus(emSecDisable, (bool)seddis);
        } else {
            // No operation
        }
    }
}//}}}

void ECC_DFLASH_E2_Func::cb_DFERSTC_ERRCLR(RegCBstr str)
{//{{{
    if (mpECC_DFLASH_E2AgentController->GetClockPeriod() != 0) {
        if ((*DFERSTC)["ERRCLR"] == 1) {
            (*DFERSTR) = 0;
        }
    }
    (*DFERSTC) = 0; // Always read as 0
}//}}}

void ECC_DFLASH_E2_Func::cb_DFOVFSTC_ERROVFCLR(RegCBstr str)
{//{{{
    if (mpECC_DFLASH_E2AgentController->GetClockPeriod() != 0) {
        if ((*DFOVFSTC)["ERROVFCLR"] == 1) {
            (*DFOVFSTR) = 0;
        }
    }
    (*DFOVFSTC) = 0; // Always read as 0
}//}}}

void ECC_DFLASH_E2_Func::cb_DFKCPROT_KCE(RegCBstr str)
{//{{{
    unsigned int kcprot = (unsigned int)(*DFKCPROT)["KCPROT"];
    unsigned int kce = (unsigned int)(*DFKCPROT)["KCE"];
    unsigned int pre_data = (unsigned int)str.pre_data;
    unsigned int pre_kce = pre_data & 0x1;
    if ((kcprot << 1) == emKeyCode) {
        mEnableWrProtReg = (bool)kce;
    } else {
        (*DFKCPROT)["KCE"] = pre_kce; // wrong KeyCode so keep KCE
    }
    (*DFKCPROT)["KCPROT"] = 0; // KCPROT[31:1] bits are always read as 0
}//}}}

void ECC_DFLASH_E2_Func::cb_DFERRINT_EOVFIE(RegCBstr str)
{//{{{
    if (!mIsAddrCaptureSupport) {
        (*DFERRINT)["EOVFIE"] = 0;
    }
}//}}}

// vim600: set foldmethod=marker :

