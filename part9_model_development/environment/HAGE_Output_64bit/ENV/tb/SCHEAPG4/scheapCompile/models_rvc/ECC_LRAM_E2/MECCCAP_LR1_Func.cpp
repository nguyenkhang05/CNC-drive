// ---------------------------------------------------------------------
// $Id: MECCCAP_LR1_Func.cpp,v 1.1 2019/09/26 10:07:33 landang Exp $
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
#include "MECCCAP_LR1_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
MECCCAP_LR1_Func::MECCCAP_LR1_Func (std::string name, ECC_LRAM_E2_AgentController* LRAMAgentController):
                Cmecccap_lr1_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(LRAMAgentController != NULL);
    mpLRAMAgentController = LRAMAgentController;
    mpLRAMAgentController->CommandInit(name);
    mModuleName = name;

    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x00, "LR1_ERRINT"));
    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x60, "LR1_ERMSA0"));
    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x64, "LR1_ERMEA0"));
    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x68, "LR1_ERMSA1"));
    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x6C, "LR1_ERMEA1"));

    Cmecccap_lr1_regif::set_instance_name(name);
}//}}}

/// Destructor
MECCCAP_LR1_Func::~MECCCAP_LR1_Func()
{//{{{
}//}}}

/// Reset Func and registers
void MECCCAP_LR1_Func::EnableReset(const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "sys_resetz"){
        Cmecccap_lr1_regif::EnableReset(is_active);
    }
}//}}}

/// Read API (target socket)
void MECCCAP_LR1_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int size = (unsigned int)trans.get_data_length();
    unsigned int address = (unsigned int)trans.get_address();
    memset(trans.get_data_ptr(), 0, size);

    /// Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    /// get status of reset
    bool mResetStatus = mpLRAMAgentController->GetResetStatus("presetn") | mpLRAMAgentController->GetResetStatus("sys_resetz"); 
    /// When clock == 0 or assert reset, only bebug master read.
    if (!mpLRAMAgentController->CheckClockPeriod("pclk") || mResetStatus) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId( ) != DEBUG_MASTER_BMID)) {
            if (mResetStatus) { /// during reset
                re_printf("warning", "Cannot access register during reset period.\n");
            } else {
                re_printf("warning", "Cannot access register when clock is 0.\n");
            }
            return;
        }
    }

    // Prevent access to "reserved area" (define by CAP_DEPTH)
    unsigned int masked_addr = address & 0xFF; // Mask to get last 8 bits
    unsigned int cap_depth = mpLRAMAgentController->GetCapDepth("LR1");
    unsigned int start_reserved_seadr = 0x70 + 0x4*cap_depth; // start reserved area of nSEADR registers
    unsigned int end_reserved_seadr = 0xF0-1; // end reserved area of nSEADR registers (=offset DEADR - 1)
    if ((start_reserved_seadr <= masked_addr) && (masked_addr <= end_reserved_seadr)){
        re_printf("warning", "Cannot access to reserved area (%08X).\n", address);
    }else{
        if (!debug) {
            reg_rd(address, trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg(address, trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

/// Write API (target socket)
void MECCCAP_LR1_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (mpLRAMAgentController->GetResetStatus("presetn") || mpLRAMAgentController->GetResetStatus("sys_resetz")){
        re_printf("warning", "Cannot access register during reset period.\n");
        return;
    }
    /// Get extension infor
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    
    /// When clock = 0, only bebug master write.
    if (! (mpLRAMAgentController->CheckClockPeriod("pclk") && mpLRAMAgentController->CheckClockPeriod("sys_clk")) ){
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }   
    
    unsigned int address = (unsigned int)trans.get_address();
    unsigned int masked_addr = address & 0xFF; // Mask to get last 8 bits
    unsigned int cap_depth = mpLRAMAgentController->GetCapDepth("LR1");
    unsigned int start_reserved_seadr = 0x70 + 0x4*cap_depth; // start reserved area of nSEADR registers
    unsigned int end_reserved_seadr = 0xF0-1; // end reserved area of nSEADR registers (=offset DEADR - 1)
    if ((start_reserved_seadr <= masked_addr) && (masked_addr <= end_reserved_seadr)){
        re_printf("warning", "Cannot access to reserved area (%08X).\n", address);
    }else{

        // Check protected or not
        bool keyopen = mpLRAMAgentController->GetKeyOpen();
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
void MECCCAP_LR1_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cmecccap_lr1_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cmecccap_lr1_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Set status bit and address if there is room
bool MECCCAP_LR1_Func::Set1bitStatusAndAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    unsigned int cap_depth = mpLRAMAgentController->GetCapDepth("LR1");
    unsigned int status = (unsigned int)(*LR1_SERSTR);
    bool is_find_room = false;
    // Store if any
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((status >> i) & 0x1) == 0){
            // Update SEADR register according
            (*LR1_nSEADR[i])["SEDL"] = edl_code;
            (*LR1_nSEADR[i])["SEADR0"] = mod_address;
            // Set according bit in SERINF register
            unsigned int serinf = (unsigned int)(*LR1_SERINF);
            serinf |= (1 << edl_code);
            (*LR1_SERINF) = serinf;
            // Set according bit in SERSTR register
            status |= (1 << i);
            (*LR1_SERSTR) = status;
            is_find_room = true;
            break;
        }
    }
    return is_find_room;
}//}}}

/// Set Adderss buffer overflow
void MECCCAP_LR1_Func::SetFlagAddressBufferOverflow()
{//{{{
    unsigned int pre_serrovf0 = (unsigned int)(*LR1_OVFSTR)["SERROVF0"];
    if ((pre_serrovf0 == 0) && ((unsigned int)(*LR1_ERRINT)["SEOVFIE"] == 1)){
        mpLRAMAgentController->AssertERROVFOUT0_LR1();
    }
    if (pre_serrovf0 == 0) {
        (*LR1_OVFSTR)["SERROVF0"] = 1;
    }
}//}}}

/// Check has room to capture 1bit status
bool MECCCAP_LR1_Func::CheckHasRoomFor1bit()
{//{{{
    unsigned int cap_depth = mpLRAMAgentController->GetCapDepth("LR1");
    unsigned int status = (unsigned int)(*LR1_SERSTR);
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
void MECCCAP_LR1_Func::SetFIFOOverflow ()
{//{{{
    unsigned int pre_serrovf1 = (unsigned int)(*LR1_OVFSTR)["SERROVF1"];
    if ((pre_serrovf1 == 0) && ((unsigned int)(*LR1_ERRINT)["SEOVFIE"] == 1)){
        mpLRAMAgentController->AssertERROVFOUT1_LR1();
    }
    (*LR1_OVFSTR)["SERROVF1"] = 1;
}//}}}

/// Get SEDL bit
unsigned int MECCCAP_LR1_Func::GetSEDLBit(unsigned int index)
{//{{{
    return (unsigned int)(*LR1_nSEADR[index])["SEDL"];
}//}}}

/// Get SEADR bit
unsigned int MECCCAP_LR1_Func::GetSEADRBit(unsigned int index)
{//{{{
    return (unsigned int)(*LR1_nSEADR[index])["SEADR0"];
}//}}}

/// Get DEDL bit
unsigned int MECCCAP_LR1_Func::GetDEDLBit()
{//{{{
    return (unsigned int)(*LR1_00DEADR)["DEDL"];
}//}}}

/// Get DEADR bit
unsigned int MECCCAP_LR1_Func::GetDEADRBit()
{//{{{
    return (unsigned int)(*LR1_00DEADR)["DEADR0"];
}//}}}

/// Get Start mask address
unsigned int MECCCAP_LR1_Func::GetERMSA(unsigned int index)
{//{{{
    unsigned int ret_val = 0;
    if (index == 0){
        ret_val = (unsigned int)(*LR1_ERMSA0)["ERMSA"];
    }else{// index == 1
        ret_val = (unsigned int)(*LR1_ERMSA1)["ERMSA"];
    }
    return ret_val;
}//}}}

/// Get End mask address
unsigned int MECCCAP_LR1_Func::GetERMEA(unsigned int index)
{//{{{
    unsigned int ret_val = 0;
    if (index == 0){
        ret_val = (unsigned int)(*LR1_ERMEA0)["ERMEA"];
    }else{// index == 1
        ret_val = (unsigned int)(*LR1_ERMEA1)["ERMEA"];
    }
    return ret_val;
}//}}}

/// Get mask address enable
unsigned int MECCCAP_LR1_Func::GetERM_EN(unsigned int index)
{//{{{
    unsigned int ret_val = 0;
    if (index == 0){
        ret_val = (unsigned int)(*LR1_ERMSA0)["EN"];
    }else{// index == 1
        ret_val = (unsigned int)(*LR1_ERMSA1)["EN"];
    }
    return ret_val;
}//}}}

/// Check no fatal status
bool MECCCAP_LR1_Func::CheckNoFatalStatus()
{//{{{
    return ((unsigned int)(*LR1_DERSTR) == 0);
}//}}}

/// Set 2bit error status
void MECCCAP_LR1_Func::Set2bitErrorStatus()
{//{{{
    (*LR1_DERSTR)["DEDF00"] = 1;
}//}}}

/// Set Fatal error address and edl according
void MECCCAP_LR1_Func::SetFatalAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    (*LR1_00DEADR)["DEDL"] = edl_code;
    (*LR1_00DEADR)["DEADR0"] = mod_address;
}//}}}

/// Get value of LR1_SERSTR register
unsigned int MECCCAP_LR1_Func::GetLR1_SERSTR()
{//{{{
    return (unsigned int)(*LR1_SERSTR);
}//}}}

/// Get value of LR1_DERSTR register
unsigned int MECCCAP_LR1_Func::GetLR1_DERSTR()
{//{{{
    return (unsigned int)(*LR1_DERSTR);
}//}}}

/// Get value of LR1_OVFSTR register
unsigned int MECCCAP_LR1_Func::GetLR1_OVFSTR()
{//{{{
    return (unsigned int)(*LR1_OVFSTR);
}//}}}

/// Write ERRINT_INIT to ERRINT reg after reset
void MECCCAP_LR1_Func::WriteERRINT_INIT(unsigned int value)
{//{{{
    LR1_ERRINT->UpdateBitInitValue("SEDIE", (value>>0)&0x1);
    LR1_ERRINT->UpdateBitInitValue("DEDIE", (value>>1)&0x1);
    LR1_ERRINT->UpdateBitInitValue("SEOVFIE", (value>>7)&0x1);
}//}}}

/// Get ERRINT register
unsigned int MECCCAP_LR1_Func::GetERRINT()
{//{{{
    return (unsigned int)(*LR1_ERRINT);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of LR1_SSTCLR_LR1_SSTCLR bit
void MECCCAP_LR1_Func::cb_LR1_SSTCLR_LR1_SSTCLR(RegCBstr str)
{//{{{ 
    // clear LR1_SERSTR according
    unsigned int val = (unsigned int)(*LR1_SSTCLR);
    unsigned int inf = (unsigned int)(*LR1_SERINF);
    unsigned int flg = (unsigned int)(*LR1_SERSTR);
    unsigned int cap_depth = mpLRAMAgentController->GetCapDepth("LR1");
    unsigned int reg32_mask = 0xFFFFFFFF;
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((val >> i) & 0x1) == 1){
            // Clear status flag
            flg = flg & (reg32_mask ^ (1 << i));
            // Clear SERINF bit
            unsigned int sedl = (unsigned int)(*LR1_nSEADR[i])["SEDL"];
            inf = inf & (reg32_mask ^ (1 << sedl));
        }
    }
    *LR1_SERSTR = flg;
    *LR1_SERINF = inf;
    (*LR1_SSTCLR) = 0;
}//}}}

/// Callback function of LR1_DSTCLR_DSTCLR10 bit
void MECCCAP_LR1_Func::cb_LR1_DSTCLR_DSTCLR00(RegCBstr str)
{//{{{
    // clear DED.
    if ((unsigned int)(*LR1_DSTCLR)["DSTCLR00"] == 1){
        (*LR1_DERSTR)["DEDF00"] = 0;
    }
    (*LR1_DSTCLR) = 0;
}//}}}

/// Callback function of LR1_OVFCLR_SERROVF0 bit
void MECCCAP_LR1_Func::cb_LR1_OVFCLR_SERROVFCLR0(RegCBstr str)
{//{{{ 
    // clear overflow error status
    if ((unsigned int)(*LR1_OVFCLR)["SERROVFCLR0"] == 1){
        (*LR1_OVFSTR)["SERROVF0"] = 0;
    }
    if ((unsigned int)(*LR1_OVFCLR)["SERROVFCLR1"] == 1){
        (*LR1_OVFSTR)["SERROVF1"] = 0;
    }
    (*LR1_OVFCLR) = 0;
}//}}}

/// Callback function of LR1_ERMSA0_ERMSA bit
void MECCCAP_LR1_Func::cb_LR1_ERMSA0_ERMSA(RegCBstr str)
{//{{{ 
    unsigned int value = (unsigned int)(*LR1_ERMSA0)["ERMSA"];
    unsigned int awidth_msb = mpLRAMAgentController->GetAWIDTH_MSB("LR1");
    (*LR1_ERMSA0)["ERMSA"] = (((unsigned int)(1 << (awidth_msb + 1)) - 1) >> 12) & value;
}//}}}

/// Callback function of LR1_ERMEA0_ERMEA bit
void MECCCAP_LR1_Func::cb_LR1_ERMEA0_ERMEA(RegCBstr str)
{//{{{ 
    unsigned int value = (unsigned int)(*LR1_ERMEA0)["ERMEA"];
    unsigned int awidth_msb = mpLRAMAgentController->GetAWIDTH_MSB("LR1");
    (*LR1_ERMEA0)["ERMEA"] = (((unsigned int)(1 << (awidth_msb + 1)) - 1) >> 12) & value;
}//}}}

/// Callback function of LR1_ERMSA1_ERMSA bit
void MECCCAP_LR1_Func::cb_LR1_ERMSA1_ERMSA(RegCBstr str)
{//{{{ 
    unsigned int value = (unsigned int)(*LR1_ERMSA1)["ERMSA"];
    unsigned int awidth_msb = mpLRAMAgentController->GetAWIDTH_MSB("LR1");
    (*LR1_ERMSA1)["ERMSA"] = (((unsigned int)(1 << (awidth_msb + 1)) - 1) >> 12) & value;
}//}}}

/// Callback function of LR1_ERMEA1_ERMEA bit
void MECCCAP_LR1_Func::cb_LR1_ERMEA1_ERMEA(RegCBstr str)
{//{{{ 
    unsigned int value = (unsigned int)(*LR1_ERMEA1)["ERMEA"];
    unsigned int awidth_msb = mpLRAMAgentController->GetAWIDTH_MSB("LR1");
    (*LR1_ERMEA1)["ERMEA"] = (((unsigned int)(1 << (awidth_msb + 1)) - 1) >> 12) & value;
}//}}}

// vim600: set foldmethod=marker :
