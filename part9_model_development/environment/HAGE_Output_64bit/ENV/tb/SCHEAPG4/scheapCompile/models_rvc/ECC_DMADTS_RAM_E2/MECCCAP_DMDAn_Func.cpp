// ---------------------------------------------------------------------
// $Id: MECCCAP_DMDAn_Func.cpp,v 1.8 2019/09/26 10:07:20 landang Exp $
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
#include "MECCCAP_DMDAn_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
MECCCAP_DMDAn_Func::MECCCAP_DMDAn_Func (std::string name, unsigned int sub_id, ECC_DMADTS_RAM_E2_AgentController* DMADTS_RAMAgentController):
                Cmecccap_dmdan_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(DMADTS_RAMAgentController != NULL);
    mpDMADTS_RAMAgentController = DMADTS_RAMAgentController;
    mpDMADTS_RAMAgentController->CommandInit(name);
    mModuleName = name;
    mSubId = sub_id;

    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x00, "DA_ERRINT"));

    Cmecccap_dmdan_regif::set_instance_name(name);
}//}}}

/// Destructor
MECCCAP_DMDAn_Func::~MECCCAP_DMDAn_Func()
{//{{{
}//}}}

/// Reset Func and registers
void MECCCAP_DMDAn_Func::EnableReset(const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "sys_resetz"){
        Cmecccap_dmdan_regif::EnableReset(is_active);
    }
}//}}}

/// Read API (target socket)
void MECCCAP_DMDAn_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
       // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);

    unsigned int size = (unsigned int)trans.get_data_length();
    unsigned int address = (unsigned int)trans.get_address();
    memset(trans.get_data_ptr(), 0, size);
    // Prevent access to "reserved area" (define by CAP_DEPTH)
    unsigned int masked_addr = address & 0xFF; // Mask to get last 8 bits
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DA");
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
void MECCCAP_DMDAn_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
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
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DA");
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
void MECCCAP_DMDAn_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cmecccap_dmdan_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cmecccap_dmdan_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Set status bit and address if there is room
bool MECCCAP_DMDAn_Func::Set1bitStatusAndAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DA");
    unsigned int status = (unsigned int)(*DA_SERSTR);
    bool is_find_room = false;
    // Store if any
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((status >> i) & 0x1) == 0){
            // Update SEADR register according
            (*DA_nSEADR[i])["SEDL"] = edl_code;
            (*DA_nSEADR[i])["SEADR0"] = mod_address;
            // Set according bit in SERINF register
            unsigned int serinf = (unsigned int)(*DA_SERINF);
            serinf |= (1 << edl_code);
            (*DA_SERINF) = serinf;
            // Set according bit in SERSTR register
            status |= (1 << i);
            (*DA_SERSTR) = status;
            is_find_room = true;
            break;
        }
    }
    return is_find_room;
}//}}}

/// Set Adderss buffer overflow
void MECCCAP_DMDAn_Func::SetFlagAddressBufferOverflow()
{//{{{
    unsigned int pre_serrovf0 = (unsigned int)(*DA_OVFSTR)["SERROVF0"];
    if ((pre_serrovf0 == 0) && ((unsigned int)(*DA_ERRINT)["SEOVFIE"] == 1)){
        mpDMADTS_RAMAgentController->AssertERROVFOUT0_DA(mSubId);
    }
    if (pre_serrovf0 == 0) {
        (*DA_OVFSTR)["SERROVF0"] = 1;
    }
}//}}}

/// Check has room to capture 1bit error
bool MECCCAP_DMDAn_Func::CheckHasRoomFor1bit()
{//{{{
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DA");
    unsigned int status = (unsigned int)(*DA_SERSTR);
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
void MECCCAP_DMDAn_Func::SetFIFOOverflow ()
{//{{{
    unsigned int pre_serrovf1 = (unsigned int)(*DA_OVFSTR)["SERROVF1"];
    if ((pre_serrovf1 == 0) && ((unsigned int)(*DA_ERRINT)["SEOVFIE"] == 1)){
        mpDMADTS_RAMAgentController->AssertERROVFOUT1_DA(mSubId);
    }
    (*DA_OVFSTR)["SERROVF1"] = 1;
}//}}}

/// Get SEDL bit
unsigned int MECCCAP_DMDAn_Func::GetSEDLBit(unsigned int index)
{//{{{
    return (unsigned int)(*DA_nSEADR[index])["SEDL"];
}//}}}

/// Get SEADR bit
unsigned int MECCCAP_DMDAn_Func::GetSEADRBit(unsigned int index)
{//{{{
    return (unsigned int)(*DA_nSEADR[index])["SEADR0"];
}//}}}

/// Get DEDL bit
unsigned int MECCCAP_DMDAn_Func::GetDEDLBit()
{//{{{
    return (unsigned int)(*DA_00DEADR)["DEDL"];
}//}}}

/// Get DEADR bit
unsigned int MECCCAP_DMDAn_Func::GetDEADRBit()
{//{{{
    return (unsigned int)(*DA_00DEADR)["DEADR0"];
}//}}}

/// Check no fatal status
bool MECCCAP_DMDAn_Func::CheckNoFatalStatus()
{//{{{
    return ((unsigned int)(*DA_DERSTR) == 0);
}//}}}

/// Set 2bit error status
void MECCCAP_DMDAn_Func::Set2bitErrorStatus()
{//{{{
    (*DA_DERSTR)["DEDF00"] = 1;
}//}}}

/// Set fatal address info
void MECCCAP_DMDAn_Func::SetFatalAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    (*DA_00DEADR)["DEDL"] = edl_code;
    (*DA_00DEADR)["DEADR0"] = mod_address;
}//}}}

/// Get value of DA_SERSTR register
unsigned int MECCCAP_DMDAn_Func::GetDA_SERSTR()
{//{{{
    return (unsigned int)(*DA_SERSTR);
}//}}}

/// Get value of DA_DERSTR register
unsigned int MECCCAP_DMDAn_Func::GetDA_DERSTR()
{//{{{
    return (unsigned int)(*DA_DERSTR);
}//}}}

/// Get value of DA_OVFSTR register
unsigned int MECCCAP_DMDAn_Func::GetDA_OVFSTR()
{//{{{
    return (unsigned int)(*DA_OVFSTR);
}//}}}

/// Write ERRINT_INIT to ERRINT reg after reset
void MECCCAP_DMDAn_Func::WriteERRINT_INIT(unsigned int value)
{//{{{
    DA_ERRINT->UpdateBitInitValue("SEDIE", (value>>0)&0x1);
    DA_ERRINT->UpdateBitInitValue("DEDIE", (value>>1)&0x1);
    DA_ERRINT->UpdateBitInitValue("SEOVFIE", (value>>7)&0x1);
}//}}}

/// Get ERRINT register
unsigned int MECCCAP_DMDAn_Func::GetERRINT()
{//{{{
    return (unsigned int)(*DA_ERRINT);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of DA_SSTCLR_DA_SSTCLR bit
void MECCCAP_DMDAn_Func::cb_DA_SSTCLR_SSTCLR00(RegCBstr str)
{//{{{ 
    // clear DA_SERSTR according
    unsigned int val = (unsigned int)(*DA_SSTCLR);
    unsigned int inf = (unsigned int)(*DA_SERINF);
    unsigned int flg = (unsigned int)(*DA_SERSTR);
    unsigned int cap_depth = mpDMADTS_RAMAgentController->GetCapDepth("DA");
    unsigned int reg32_mask = 0xFFFFFFFF;
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((val >> i) & 0x1) == 1){
            // Clear status flag
            flg = flg & (reg32_mask ^ (1 << i));
            // Clear SERINF bit
            unsigned int sedl = (unsigned int)(*DA_nSEADR[i])["SEDL"];
            inf = inf & (reg32_mask ^ (1 << sedl));
        }
    }
    *DA_SERSTR = flg;
    *DA_SERINF = inf;
    (*DA_SSTCLR) = 0;
}//}}}

/// Callback function of DA_DSTCLR_DSTCLR00 bit
void MECCCAP_DMDAn_Func::cb_DA_DSTCLR_DSTCLR00(RegCBstr str)
{//{{{
    // clear APE, DED.
    if ((unsigned int)(*DA_DSTCLR)["DSTCLR00"] == 1){
        (*DA_DERSTR)["DEDF00"] = 0;
    }
    (*DA_DSTCLR) = 0;
}//}}}

/// Callback function of DA_OVFCLR_SERROVF0 bit
void MECCCAP_DMDAn_Func::cb_DA_OVFCLR_SERROVFCLR0(RegCBstr str)
{//{{{ 
    // clear overflow error status
    if ((unsigned int)(*DA_OVFCLR)["SERROVFCLR0"] == 1){
        (*DA_OVFSTR)["SERROVF0"] = 0;
    }
    if ((unsigned int)(*DA_OVFCLR)["SERROVFCLR1"] == 1){
        (*DA_OVFSTR)["SERROVF1"] = 0;
    }
    (*DA_OVFCLR) = 0;
}//}}}
// vim600: set foldmethod=marker :
