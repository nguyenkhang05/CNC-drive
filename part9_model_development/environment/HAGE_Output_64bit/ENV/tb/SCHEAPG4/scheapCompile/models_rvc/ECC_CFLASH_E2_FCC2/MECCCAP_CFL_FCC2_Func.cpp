// ---------------------------------------------------------------------
// $Id: MECCCAP_CFL_FCC2_Func.cpp,v 1.7 2019/09/26 10:07:02 landang Exp $
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
#include "MECCCAP_CFL_FCC2_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
MECCCAP_CFL_FCC2_Func::MECCCAP_CFL_FCC2_Func (std::string name, ECC_CFLASH_E2_FCC2_AgentController* CFLASHAgentController):
                Cmecccap_cfl_fcc2_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(CFLASHAgentController != NULL);
    mpCFLASHAgentController = CFLASHAgentController;
    mpCFLASHAgentController->CommandInit(name);
    mModuleName = name;

    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x00, "CF_ERRINT"));
    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x60, "CF_ERMSA0"));
    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x64, "CF_ERMEA0"));
    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x68, "CF_ERMSA1"));
    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x6C, "CF_ERMEA1"));

    Cmecccap_cfl_fcc2_regif::set_instance_name(name);
}//}}}

/// Destructor
MECCCAP_CFL_FCC2_Func::~MECCCAP_CFL_FCC2_Func()
{//{{{
}//}}}

/// Reset Func and registers
void MECCCAP_CFL_FCC2_Func::EnableReset(const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "sys_resetz"){
        Cmecccap_cfl_fcc2_regif::EnableReset(is_active);
    }
}//}}}

/// Read API (target socket)
void MECCCAP_CFL_FCC2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    unsigned int size = (unsigned int)trans.get_data_length();
    unsigned int address = (unsigned int)trans.get_address();
    memset(trans.get_data_ptr(), 0, size);
    /// get status of reset
    bool mResetStatus = mpCFLASHAgentController->GetResetStatus("presetn") | mpCFLASHAgentController->GetResetStatus("sys_resetz"); 
    /// When clock == 0 or assert reset, only bebug master read.
    if (!mpCFLASHAgentController->CheckClockPeriod("pclk") || mResetStatus) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId( ) != DEBUG_MASTER_BMID)) {
            if (mResetStatus) { /// during reset
                re_printf("warning", "Cannot access register during reset period \n");
            } else {
                re_printf("warning", "Cannot access register when clock is 0.\n");
            }
            return;
        }
    }
    // Prevent access to "reserved area" (define by CAP_DEPTH)
    unsigned int masked_addr = address & 0xFF; // Mask to get last 8 bits
    unsigned int cap_depth = mpCFLASHAgentController->GetCapDepth();
    unsigned int start_reserved_seadr = 0x70 + 0x4*cap_depth; // start reserved area of nSEADR registers
    unsigned int end_reserved_seadr = 0xF0-1; // end reserved area of nSEADR registers (=offset DEADR - 1)
    if ((start_reserved_seadr <= masked_addr) && (masked_addr <= end_reserved_seadr)){
        re_printf("warning", "Cannot access to reserved area (%08X).\n", address);
        return;
    }
    if (!debug) {
        reg_rd(address, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg(address, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Write API (target socket)
void MECCCAP_CFL_FCC2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (mpCFLASHAgentController->GetResetStatus("presetn") || mpCFLASHAgentController->GetResetStatus("sys_resetz")){
        re_printf("warning", "Cannot access register during reset period.\n");
        return;
    }
    // Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);

        // Zero clock
    if (false == mpCFLASHAgentController->CheckClockPeriod("pclk")) {
        // Software debugger
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }

    unsigned int address = (unsigned int)trans.get_address();
    unsigned int masked_addr = address & 0xFF; // Mask to get last 8 bits
    unsigned int cap_depth = mpCFLASHAgentController->GetCapDepth();
    unsigned int start_reserved_seadr = 0x70 + 0x4*cap_depth; // start reserved area of nSEADR registers
    unsigned int end_reserved_seadr = 0xF0-1; // end reserved area of nSEADR registers (=offset DEADR - 1)
    if ((start_reserved_seadr <= masked_addr) && (masked_addr <= end_reserved_seadr)){
        re_printf("warning", "Cannot access to reserved area (%08X).\n", address);
    }else{

        // Check protected or not
        bool keyopen = mpCFLASHAgentController->GetKeyOpen();
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
void MECCCAP_CFL_FCC2_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cmecccap_cfl_fcc2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_fcc2_regif class
            msg_return = Cmecccap_cfl_fcc2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Set status bit and address if there is room
bool MECCCAP_CFL_FCC2_Func::Set1bitStatusAndAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    unsigned int cap_depth = mpCFLASHAgentController->GetCapDepth();
    unsigned int status = (unsigned int)(*CF_SERSTR);
    bool is_find_room = false;
    // Store if any
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((status >> i) & 0x1) == 0){
            // Update SEADR register according
            (*CF_nSEADR[i])["SEDL"] = edl_code;
            (*CF_nSEADR[i])["SEADR0"] = mod_address;
            // Set according bit in SERINF register
            unsigned int serinf = (unsigned int)(*CF_SERINF);
            serinf |= (1 << edl_code);
            (*CF_SERINF) = serinf;
            // Set according bit in SERSTR register
            status |= (1 << i);
            (*CF_SERSTR) = status;
            is_find_room = true;
            break;
        }
    }
    return is_find_room;
}//}}}

/// Check has room to capture 1bit error
bool MECCCAP_CFL_FCC2_Func::CheckHasRoomFor1bit()
{//{{{
    unsigned int cap_depth = mpCFLASHAgentController->GetCapDepth();
    unsigned int status = (unsigned int)(*CF_SERSTR);
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
void MECCCAP_CFL_FCC2_Func::SetFIFOOverflow ()
{//{{{
    unsigned int pre_serrovf1 = (unsigned int)(*CF_OVFSTR)["SERROVF1"];
    if ((pre_serrovf1 == 0)  && ((unsigned int)(*CF_ERRINT)["SEOVFIE"] == 1)){
        mpCFLASHAgentController->AssertERROVFOUT1_CF();
    }
    if (pre_serrovf1 == 0) {
    (*CF_OVFSTR)["SERROVF1"] = 1;
    }
}//}}}
 /// Set Adderss buffer overflow
void MECCCAP_CFL_FCC2_Func::SetFlagAddressBufferOverflow()
{//{{{
    unsigned int pre_serrovf0 = (unsigned int)(*CF_OVFSTR)["SERROVF0"];
    if ((pre_serrovf0 == 0) && ((unsigned int)(*CF_ERRINT)["SEOVFIE"] == 1)){
        mpCFLASHAgentController->AssertERROVFOUT0_CF();
    }
    if (pre_serrovf0 == 0) {
        (*CF_OVFSTR)["SERROVF0"] = 1;
    }
}//}}}

/// Get SEDL bit
unsigned int MECCCAP_CFL_FCC2_Func::GetSEDLBit(unsigned int index)
{//{{{
    return (unsigned int)(*CF_nSEADR[index])["SEDL"];
}//}}}

/// Get SEADR bit
unsigned int MECCCAP_CFL_FCC2_Func::GetSEADRBit(unsigned int index)
{//{{{
    return (unsigned int)(*CF_nSEADR[index])["SEADR0"];
}//}}}

/// Get DEDL bit
unsigned int MECCCAP_CFL_FCC2_Func::GetDEDLBit()
{//{{{
    return (unsigned int)(*CF_00DEADR)["DEDL"];
}//}}}

/// Get DEADR bit
unsigned int MECCCAP_CFL_FCC2_Func::GetDEADRBit()
{//{{{
    return (unsigned int)(*CF_00DEADR)["DEADR0"];
}//}}}

/// Get Start mask address
unsigned int MECCCAP_CFL_FCC2_Func::GetERMSA(unsigned int index)
{//{{{
    unsigned int ret_val = 0;
    if (index == 0){
        ret_val = (unsigned int)(*CF_ERMSA0)["ERMSA"];
    }else{// index == 1
        ret_val = (unsigned int)(*CF_ERMSA1)["ERMSA"];
    }
    return ret_val;
}//}}}

/// Get End mask address
unsigned int MECCCAP_CFL_FCC2_Func::GetERMEA(unsigned int index)
{//{{{
    unsigned int ret_val = 0;
    if (index == 0){
        ret_val = (unsigned int)(*CF_ERMEA0)["ERMEA"];
    }else{// index == 1
        ret_val = (unsigned int)(*CF_ERMEA1)["ERMEA"];
    }
    return ret_val;
}//}}}

/// Get mask address enable
unsigned int MECCCAP_CFL_FCC2_Func::GetERM_EN(unsigned int index)
{//{{{
    unsigned int ret_val = 0;
    if (index == 0){
        ret_val = (unsigned int)(*CF_ERMSA0)["EN"];
    }else{// index == 1
        ret_val = (unsigned int)(*CF_ERMSA1)["EN"];
    }
    return ret_val;
}//}}}

/// Check no fatal status
bool MECCCAP_CFL_FCC2_Func::CheckNoFatalStatus()
{//{{{
    return ((unsigned int)(*CF_DERSTR) == 0);
}//}}}

/// Set 2bit error status
void MECCCAP_CFL_FCC2_Func::Set2bitErrorStatus()
{//{{{
    (*CF_DERSTR)["DEDF00"] = 1;
}//}}}

/// Set address parity error status
void MECCCAP_CFL_FCC2_Func::SetAddrParityErrorStatus()
{//{{{
    (*CF_DERSTR)["APEF00"] = 1;
}//}}}

/// Set fatal error address and edl code according
void MECCCAP_CFL_FCC2_Func::SetFatalAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    (*CF_00DEADR)["DEDL"] = edl_code;
    (*CF_00DEADR)["DEADR0"] = mod_address;
}//}}}

/// Get value of CF_SERSTR register
unsigned int MECCCAP_CFL_FCC2_Func::GetCF_SERSTR()
{//{{{
    return (unsigned int)(*CF_SERSTR);
}//}}}

/// Get value of CF_DERSTR register
unsigned int MECCCAP_CFL_FCC2_Func::GetCF_DERSTR()
{//{{{
    return (unsigned int)(*CF_DERSTR);
}//}}}

/// Get value of CF_OVFSTR register
unsigned int MECCCAP_CFL_FCC2_Func::GetCF_OVFSTR()
{//{{{
    return (unsigned int)(*CF_OVFSTR);
}//}}}

/// Write ERRINT_INIT to ERRINT reg after reset
void MECCCAP_CFL_FCC2_Func::WriteERRINT_INIT(unsigned int value)
{//{{{
    CF_ERRINT->UpdateBitInitValue("SEDIE", (value>>0)&0x1);
    CF_ERRINT->UpdateBitInitValue("DEDIE", (value>>1)&0x1);
    CF_ERRINT->UpdateBitInitValue("APEIE", (value>>2)&0x1);
    CF_ERRINT->UpdateBitInitValue("SEOVFIE", (value>>7)&0x1);
}//}}}

/// Get ERRINT value
unsigned int MECCCAP_CFL_FCC2_Func::GetERRINT()
{//{{{
    return (unsigned int)(*CF_ERRINT);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of CF_SSTCLR_CF_SSTCLR bit
void MECCCAP_CFL_FCC2_Func::cb_CF_SSTCLR_CF_SSTCLR(RegCBstr str)
{//{{{ 
    // clear CF_SERSTR according
    unsigned int val = (unsigned int)(*CF_SSTCLR);
    unsigned int inf = (unsigned int)(*CF_SERINF);
    unsigned int flg = (unsigned int)(*CF_SERSTR);
    unsigned int cap_depth = mpCFLASHAgentController->GetCapDepth();
    unsigned int reg32_mask = 0xFFFFFFFF;
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((val >> i) & 0x1) == 1){
            // Clear status flag
            flg = flg & (reg32_mask ^ (1 << i));
            // Clear SERINF bit
            unsigned int sedl = (unsigned int)(*CF_nSEADR[i])["SEDL"];
            inf = inf & (reg32_mask ^ (1 << sedl));
        }
    }
    *CF_SERSTR = flg;
    *CF_SERINF = inf;
    (*CF_SSTCLR) = 0;
}//}}} 

/// Callback function of CF_DSTCLR_DSTCLR00 bit
void MECCCAP_CFL_FCC2_Func::cb_CF_DSTCLR_DSTCLR00(RegCBstr str)
{//{{{
    // clear APE, DED.
    if ((unsigned int)(*CF_DSTCLR)["DSTCLR00"] == 1){
        (*CF_DERSTR)["APEF00"] = 0;
        (*CF_DERSTR)["DEDF00"] = 0;
    }
    (*CF_DSTCLR) = 0;
}//}}}

/// Callback function of CF_OVFCLR_SERROVF0 bit
void MECCCAP_CFL_FCC2_Func::cb_CF_OVFCLR_SERROVFCLR0(RegCBstr str)
{//{{{ 
    // clear overflow error status
    if ((unsigned int)(*CF_OVFCLR)["SERROVFCLR0"] == 1){
        (*CF_OVFSTR)["SERROVF0"] = 0;
    }
    if ((unsigned int)(*CF_OVFCLR)["SERROVFCLR1"] == 1){
        (*CF_OVFSTR)["SERROVF1"] = 0;
    }
    (*CF_OVFCLR) = 0;
}//}}}

/// Callback function of CF_ERMSA0
void MECCCAP_CFL_FCC2_Func::cb_CF_ERMSA0_ERMSA(RegCBstr str)
{//{{{
    unsigned int value = (unsigned int)(*CF_ERMSA0)["ERMSA"];
    unsigned int awidth_msb = mpCFLASHAgentController->GetAWIDTH_MSB();
    (*CF_ERMSA0)["ERMSA"] = (((unsigned int)(1 << (awidth_msb + 1)) - 1) >> 12) & value;
}//}}}

/// Callback function of CF_ERMEA0
void MECCCAP_CFL_FCC2_Func::cb_CF_ERMEA0_ERMEA(RegCBstr str)
{//{{{
    unsigned int value = (unsigned int)(*CF_ERMEA0)["ERMEA"];
    unsigned int awidth_msb = mpCFLASHAgentController->GetAWIDTH_MSB();
    (*CF_ERMEA0)["ERMEA"] = (((unsigned int)(1 << (awidth_msb + 1)) - 1) >> 12) & value;
}//}}}

/// Callback function of CF_ERMSA1
void MECCCAP_CFL_FCC2_Func::cb_CF_ERMSA1_ERMSA(RegCBstr str)
{//{{{
    unsigned int value = (unsigned int)(*CF_ERMSA1)["ERMSA"];
    unsigned int awidth_msb = mpCFLASHAgentController->GetAWIDTH_MSB();
    (*CF_ERMSA1)["ERMSA"] = (((unsigned int)(1 << (awidth_msb + 1)) - 1) >> 12) & value;
}//}}}

/// Callback function of CF_ERMEA1
void MECCCAP_CFL_FCC2_Func::cb_CF_ERMEA1_ERMEA(RegCBstr str)
{//{{{
    unsigned int value = (unsigned int)(*CF_ERMEA1)["ERMEA"];
    unsigned int awidth_msb = mpCFLASHAgentController->GetAWIDTH_MSB();
    (*CF_ERMEA1)["ERMEA"] = (((unsigned int)(1 << (awidth_msb + 1)) - 1) >> 12) & value;
}//}}}
// vim600: set foldmethod=marker :

