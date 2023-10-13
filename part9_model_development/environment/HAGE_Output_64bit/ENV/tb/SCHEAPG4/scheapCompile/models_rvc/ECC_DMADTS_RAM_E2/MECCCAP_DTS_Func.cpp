// ---------------------------------------------------------------------
// $Id: MECCCAP_DTS_Func.cpp,v 1.8 2019/09/26 10:07:20 landang Exp $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "MECCCAP_DTS_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
MECCCAP_DTS_Func::MECCCAP_DTS_Func (std::string name, ECC_DMADTS_RAM_E2_AgentController* DMADTS_RAMAgentController):
                Cmecccap_dts_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(DMADTS_RAMAgentController != NULL);
    mpDMADTS_RAMAgentController = DMADTS_RAMAgentController;
    mpDMADTS_RAMAgentController->CommandInit(name);
    mModuleName = name;

    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x00, "DR_ERRINT"));

    Cmecccap_dts_regif::set_instance_name(name);
}//}}}

/// Destructor
MECCCAP_DTS_Func::~MECCCAP_DTS_Func()
{//{{{
}//}}}

/// Reset Func and registers
void MECCCAP_DTS_Func::EnableReset(const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "sys_resetz"){
        Cmecccap_dts_regif::EnableReset(is_active);
    }
}//}}}

/// Read API (target socket)
void MECCCAP_DTS_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int size = (unsigned int)trans.get_data_length();
    unsigned int address = (unsigned int)trans.get_address();
    memset(trans.get_data_ptr(), 0, size);
    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    // Prevent access to "reserved area" (define by CAP_DEPTH)
    unsigned int masked_addr = address & 0xFF; // Mask to get last 8 bits
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DR");
    unsigned int start_reserved_seadr = 0x70 + 0x4*cap_depth; // start reserved area of nSEADR registers
    unsigned int end_reserved_seadr = 0xF0-1; // end reserved area of nSEADR registers (=offset DEADR - 1)
    if ((start_reserved_seadr <= masked_addr) && (masked_addr <= end_reserved_seadr)){
        re_printf("warning", "Cannot access to reserved area (%08X).\n", address);
    }else{
        // Zero clock or reset active
        if ((false == mpDMADTS_RAMAgentController->CheckClockPeriod("pclk"))
            || (true == (mpDMADTS_RAMAgentController->GetResetStatus("presetn")))
            || (true == (mpDMADTS_RAMAgentController->GetResetStatus("sys_resetz")))) {
        // Software debugger
            if ((true == debug) && (extension != NULL) && (DEBUG_MASTER_BMID == extension->getPeId())) {
                reg_rd_dbg((unsigned int) trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            } else {
                re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            }
        } else {
            if (!debug) {
                reg_rd(address, trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_rd_dbg(address, trans.get_data_ptr(), trans.get_data_length());
            }
        }
    }
}//}}}

/// Write API (target socket)
void MECCCAP_DTS_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (mpDMADTS_RAMAgentController->GetResetStatus("presetn") || mpDMADTS_RAMAgentController->GetResetStatus("sys_resetz")){
        re_printf("warning", "Cannot access register during reset period.\n");
        return;
    }
    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);

        // Zero clock
    if (false == mpDMADTS_RAMAgentController->CheckClockPeriod("pclk")) {
        // Software debugger
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }

    unsigned int address = (unsigned int)trans.get_address();
    unsigned int masked_addr = address & 0xFF; // Mask to get last 8 bits
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DR");
    unsigned int start_reserved_seadr = 0x70 + 0x4*cap_depth; // start reserved area of nSEADR registers
    unsigned int end_reserved_seadr = 0xF0-1; // end reserved area of nSEADR registers (=offset DEADR - 1)
    if ((start_reserved_seadr <= masked_addr) && (masked_addr <= end_reserved_seadr)){
        re_printf("warning", "Cannot access to reserved area (%08X).\n", address);
    }else{

        // Check protected or not
        bool keyopen = mpDMADTS_RAMAgentController->GetKeyOpen();
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
    }
}//}}}

/// Handler for register IF
void MECCCAP_DTS_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cmecccap_dts_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cmecccap_dts_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Set status bit and address if there is room
bool MECCCAP_DTS_Func::Set1bitStatusAndAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DR");
    unsigned int status = (unsigned int)(*DR_SERSTR);
    bool is_find_room = false;
    // Store if any
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((status >> i) & 0x1) == 0){
            // Update SEADR register according
            (*DR_nSEADR[i])["SEDL"] = edl_code;
            (*DR_nSEADR[i])["SEADR0"] = mod_address;
            // Set according bit in SERINF register
            unsigned int serinf = (unsigned int)(*DR_SERINF);
            serinf |= (1 << edl_code);
            (*DR_SERINF) = serinf;
            // Set according bit in SERSTR register
            status |= (1 << i);
            (*DR_SERSTR) = status;
            is_find_room = true;
            break;
        }
    }
    return is_find_room;
}//}}}

/// Set Adderss buffer overflow
void MECCCAP_DTS_Func::SetFlagAddressBufferOverflow()
{//{{{
    unsigned int pre_serrovf0 = (unsigned int)(*DR_OVFSTR)["SERROVF0"];
    if ((pre_serrovf0 == 0) && ((unsigned int)(*DR_ERRINT)["SEOVFIE"] == 1)){
        mpDMADTS_RAMAgentController->AssertERROVFOUT0_DR();
    }
    if (pre_serrovf0 == 0) {
        (*DR_OVFSTR)["SERROVF0"] = 1;
    }
}//}}}

/// Check has room to capture 1bit error
bool MECCCAP_DTS_Func::CheckHasRoomFor1bit()
{//{{{
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DR");
    unsigned int status = (unsigned int)(*DR_SERSTR);
    bool is_find_room = false;
    // Store if any
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((status >> i) & 0x1) == 0){
            is_find_room = true;
            break;
        }
    }
    return is_find_room;
}//}}}

/// Set FIFO overflow
void MECCCAP_DTS_Func::SetFIFOOverflow ()
{//{{{
    unsigned int pre_serrovf1 = (unsigned int)(*DR_OVFSTR)["SERROVF1"];
    if ((pre_serrovf1 == 0) && ((unsigned int)(*DR_ERRINT)["SEOVFIE"] == 1)){
        mpDMADTS_RAMAgentController->AssertERROVFOUT1_DR();
    }
    (*DR_OVFSTR)["SERROVF1"] = 1;
}//}}}

/// Get SEDL bit
unsigned int MECCCAP_DTS_Func::GetSEDLBit(unsigned int index)
{//{{{
    return (unsigned int)(*DR_nSEADR[index])["SEDL"];
}//}}}

/// Get SEADR bit
unsigned int MECCCAP_DTS_Func::GetSEADRBit(unsigned int index)
{//{{{
    return (unsigned int)(*DR_nSEADR[index])["SEADR0"];
}//}}}

/// Get DEDL bit
unsigned int MECCCAP_DTS_Func::GetDEDLBit()
{//{{{
    return (unsigned int)(*DR_00DEADR)["DEDL"];
}//}}}

/// Get DEADR bit
unsigned int MECCCAP_DTS_Func::GetDEADRBit()
{//{{{
    return (unsigned int)(*DR_00DEADR)["DEADR0"];
}//}}}

/// Check no fatal status
bool MECCCAP_DTS_Func::CheckNoFatalStatus()
{//{{{
    return ((unsigned int)(*DR_DERSTR) == 0);
}//}}}

/// Set 2bit error status
void MECCCAP_DTS_Func::Set2bitErrorStatus()
{//{{{
    (*DR_DERSTR)["DEDF00"] = 1;
}//}}}

/// Set address parity error status
void MECCCAP_DTS_Func::SetAddrFeedbackErrorStatus()
{//{{{
    (*DR_DERSTR)["AFEF00"] = 1;
}//}}}

/// Set fatal address info
void MECCCAP_DTS_Func::SetFatalAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    (*DR_00DEADR)["DEDL"] = edl_code;
    (*DR_00DEADR)["DEADR0"] = mod_address;
}//}}}

/// Get value of DR_SERSTR register
unsigned int MECCCAP_DTS_Func::GetDR_SERSTR()
{//{{{
    return (unsigned int)(*DR_SERSTR);
}//}}}

/// Get value of DR_DERSTR register
unsigned int MECCCAP_DTS_Func::GetDR_DERSTR()
{//{{{
    return (unsigned int)(*DR_DERSTR);
}//}}}

/// Get value of DR_OVFSTR register
unsigned int MECCCAP_DTS_Func::GetDR_OVFSTR()
{//{{{
    return (unsigned int)(*DR_OVFSTR);
}//}}}

/// Write ERRINT_INIT to ERRINT reg after reset
void MECCCAP_DTS_Func::WriteERRINT_INIT(unsigned int value)
{//{{{
    DR_ERRINT->UpdateBitInitValue("SEDIE", (value>>0)&0x1);
    DR_ERRINT->UpdateBitInitValue("DEDIE", (value>>1)&0x1);
    DR_ERRINT->UpdateBitInitValue("AFEIE", (value>>3)&0x1);
    DR_ERRINT->UpdateBitInitValue("SEOVFIE", (value>>7)&0x1);
}//}}}

/// Get ERRINT register
unsigned int MECCCAP_DTS_Func::GetERRINT()
{//{{{
    return (unsigned int)(*DR_ERRINT);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of DR_SSTCLR_DR_SSTCLR bit
void MECCCAP_DTS_Func::cb_DR_SSTCLR_SSTCLR00(RegCBstr str)
{//{{{ 
    // clear DR_SERSTR according
    unsigned int val = (unsigned int)(*DR_SSTCLR);
    unsigned int inf = (unsigned int)(*DR_SERINF);
    unsigned int flg = (unsigned int)(*DR_SERSTR);
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DR");
    unsigned int reg32_mask = 0xFFFFFFFF;
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((val >> i) & 0x1) == 1){
            // Clear status flag
            flg = flg & (reg32_mask ^ (1 << i));
            // Clear SERINF bit
            unsigned int sedl = (unsigned int)(*DR_nSEADR[i])["SEDL"];
            inf = inf & (reg32_mask ^ (1 << sedl));
        }
    }
    *DR_SERSTR = flg;
    *DR_SERINF = inf;
    (*DR_SSTCLR) = 0;
}//}}}

/// Callback function of DR_DSTCLR_DSTCLR00 bit
void MECCCAP_DTS_Func::cb_DR_DSTCLR_DSTCLR00(RegCBstr str)
{//{{{
    // clear AFE, DED.
    if ((unsigned int)(*DR_DSTCLR)["DSTCLR00"] == 1){
        (*DR_DERSTR)["AFEF00"] = 0;
        (*DR_DERSTR)["DEDF00"] = 0;
    }
    (*DR_DSTCLR) = 0;
}//}}}

/// Callback function of DR_OVFCLR_SERROVF0 bit
void MECCCAP_DTS_Func::cb_DR_OVFCLR_SERROVFCLR0(RegCBstr str)
{//{{{ 
    // clear overflow error status
    if ((unsigned int)(*DR_OVFCLR)["SERROVFCLR0"] == 1){
        (*DR_OVFSTR)["SERROVF0"] = 0;
    }
    if ((unsigned int)(*DR_OVFCLR)["SERROVFCLR1"] == 1){
        (*DR_OVFSTR)["SERROVF1"] = 0;
    }
    (*DR_OVFCLR) = 0;
}//}}}
// vim600: set foldmethod=marker :
