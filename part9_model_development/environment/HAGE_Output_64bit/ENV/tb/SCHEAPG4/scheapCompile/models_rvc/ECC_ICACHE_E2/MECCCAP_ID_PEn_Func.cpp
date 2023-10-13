// ---------------------------------------------------------------------
// $Id: MECCCAP_ID_PEn_Func.cpp,v 1.1 2019/09/26 10:07:26 landang Exp $
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
#include "MECCCAP_ID_PEn_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
MECCCAP_ID_PEn_Func::MECCCAP_ID_PEn_Func (std::string name, unsigned int pe_id, ECC_ICACHE_E2_AgentController* ICACHEAgentController):
                Cmecccap_id_pen_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(ICACHEAgentController != NULL);
    mpICACHEAgentController = ICACHEAgentController;
    mpICACHEAgentController->CommandInit(name);
    mModuleName = name;
    mPeId = pe_id;

    mProtectedRegMap.insert(std::pair<unsigned int, std::string>(0x00, "ID_ERRINT"));

    Cmecccap_id_pen_regif::set_instance_name(name);
}//}}}

/// Destructor
MECCCAP_ID_PEn_Func::~MECCCAP_ID_PEn_Func()
{//{{{
}//}}}

/// Reset Func and registers
void MECCCAP_ID_PEn_Func::EnableReset(const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "sys_resetz"){
        Cmecccap_id_pen_regif::EnableReset(is_active);
    }
}//}}}

/// Read API (target socket)
void MECCCAP_ID_PEn_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int size = (unsigned int)trans.get_data_length();
    unsigned int address = (unsigned int)trans.get_address();
    memset(trans.get_data_ptr(), 0, size);

    /// Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    /// get status of reset
    bool mResetStatus = mpICACHEAgentController->GetResetStatus("presetn") | mpICACHEAgentController->GetResetStatus("sys_resetz"); 
    /// When clock == 0 or assert reset, only bebug master read.
    if (!mpICACHEAgentController->CheckClockPeriod("pclk") || mResetStatus) {
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
    unsigned int cap_depth = mpICACHEAgentController->GetCapDepth("ID");
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
void MECCCAP_ID_PEn_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (mpICACHEAgentController->GetResetStatus("presetn") || mpICACHEAgentController->GetResetStatus("sys_resetz")){
        re_printf("warning", "Cannot access register during reset period.\n");
        return;
    }
    /// Get extension infor
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    
    /// When clock = 0, only bebug master write.
    if (!mpICACHEAgentController->CheckClockPeriod("pclk")) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
            re_printf("warning", "Cannot access register when clock is 0. \n");
            return;
        }
    }
    unsigned int address = (unsigned int)trans.get_address();
    unsigned int masked_addr = address & 0xFF; // Mask to get last 8 bits
    unsigned int cap_depth = mpICACHEAgentController->GetCapDepth("ID");
    unsigned int start_reserved_seadr = 0x70 + 0x4*cap_depth; // start reserved area of nSEADR registers
    unsigned int end_reserved_seadr = 0xF0-1; // end reserved area of nSEADR registers (=offset DEADR - 1)
    if ((start_reserved_seadr <= masked_addr) && (masked_addr <= end_reserved_seadr)){
        re_printf("warning", "Cannot access to reserved area (%08X).\n", address);
    }else{

        // Check protected or not
        bool keyopen = mpICACHEAgentController->GetKeyOpen();
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
void MECCCAP_ID_PEn_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cmecccap_id_pen_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cmecccap_id_pen_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Set status bit and address if there is room
bool MECCCAP_ID_PEn_Func::Set1bitStatusAndAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    unsigned int cap_depth = mpICACHEAgentController->GetCapDepth("ID");
    unsigned int status = (unsigned int)(*ID_SERSTR);
    bool is_find_room = false;
    // Store if any
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((status >> i) & 0x1) == 0){
            // Update SEADR register according
            (*ID_nSEADR[i])["SEDL"] = edl_code;
            (*ID_nSEADR[i])["SEADR0"] = mod_address;
            // Set according bit in SERINF register
            unsigned int serinf = (unsigned int)(*ID_SERINF);
            serinf |= (1 << edl_code);
            (*ID_SERINF) = serinf;
            // Set according bit in SERSTR register
            status |= (1 << i);
            (*ID_SERSTR) = status;
            is_find_room = true;
            break;
        }
    }
    return is_find_room;
}//}}}

/// Set Adderss buffer overflow
void MECCCAP_ID_PEn_Func::SetFlagAddressBufferOverflow()
{//{{{
    unsigned int pre_serrovf0 = (unsigned int)(*ID_OVFSTR)["SERROVF0"];
    if (pre_serrovf0 == 0){
        mpICACHEAgentController->AssertERROVFOUT0_ID(mPeId);
    }
    (*ID_OVFSTR)["SERROVF0"] = 1;
}//}}}

/// Check has room to capture 1bit error
bool MECCCAP_ID_PEn_Func::CheckHasRoomFor1bit()
{//{{{
    unsigned int cap_depth = mpICACHEAgentController->GetCapDepth("ID");
    unsigned int status = (unsigned int)(*ID_SERSTR);
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
void MECCCAP_ID_PEn_Func::SetFIFOOverflow ()
{//{{{
    unsigned int pre_serrovf1 = (unsigned int)(*ID_OVFSTR)["SERROVF1"];
    if (pre_serrovf1 == 0){
        mpICACHEAgentController->AssertERROVFOUT1_ID(mPeId);
    }
    (*ID_OVFSTR)["SERROVF1"] = 1;
}//}}}

/// Get SEDL bit
unsigned int MECCCAP_ID_PEn_Func::GetSEDLBit(unsigned int index)
{//{{{
    return (unsigned int)(*ID_nSEADR[index])["SEDL"];
}//}}}

/// Get SEADR bit
unsigned int MECCCAP_ID_PEn_Func::GetSEADRBit(unsigned int index)
{//{{{
    return (unsigned int)(*ID_nSEADR[index])["SEADR0"];
}//}}}

/// Get DEDL bit
unsigned int MECCCAP_ID_PEn_Func::GetDEDLBit()
{//{{{
    return (unsigned int)(*ID_00DEADR)["DEDL"];
}//}}}

/// Get DEADR bit
unsigned int MECCCAP_ID_PEn_Func::GetDEADRBit()
{//{{{
    return (unsigned int)(*ID_00DEADR)["DEADR0"];
}//}}}

/// Check no fatal status
bool MECCCAP_ID_PEn_Func::CheckNoFatalStatus()
{//{{{
    return ((unsigned int)(*ID_DERSTR) == 0);
}//}}}

/// Set 2bit error status
void MECCCAP_ID_PEn_Func::Set2bitErrorStatus()
{//{{{
    (*ID_DERSTR)["DEDF00"] = 1;
}//}}}

/// Set address feedback error status
void MECCCAP_ID_PEn_Func::SetAddrFeedbackErrorStatus()
{//{{{
    (*ID_DERSTR)["AFEF00"] = 1;
}//}}}

/// Set EDL and error address of fatal
void MECCCAP_ID_PEn_Func::SetFatalAddress(unsigned int edl_code, unsigned int mod_address)
{//{{{
    (*ID_00DEADR)["DEDL"] = edl_code;
    (*ID_00DEADR)["DEADR0"] = mod_address;
}//}}}

/// Get value of ID_SERSTR register
unsigned int MECCCAP_ID_PEn_Func::GetID_SERSTR()
{//{{{
    return (unsigned int)(*ID_SERSTR);
}//}}}

/// Get value of ID_DERSTR register
unsigned int MECCCAP_ID_PEn_Func::GetID_DERSTR()
{//{{{
    return (unsigned int)(*ID_DERSTR);
}//}}}

/// Get value of ID_OVFSTR register
unsigned int MECCCAP_ID_PEn_Func::GetID_OVFSTR()
{//{{{
    return (unsigned int)(*ID_OVFSTR);
}//}}}

/// Write ERRINT_INIT to ERRINT reg after reset
void MECCCAP_ID_PEn_Func::WriteERRINT_INIT(unsigned int value)
{//{{{
    ID_ERRINT->UpdateBitInitValue("SEDIE", (value>>0)&0x1);
    ID_ERRINT->UpdateBitInitValue("DEDIE", (value>>1)&0x1);
    ID_ERRINT->UpdateBitInitValue("AFEIE", (value>>3)&0x1);
}//}}}

/// Get ERRINT register
unsigned int MECCCAP_ID_PEn_Func::GetERRINT()
{//{{{
    return (unsigned int)(*ID_ERRINT);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of ID_SSTCLR_ID_SSTCLR bit
void MECCCAP_ID_PEn_Func::cb_ID_SSTCLR_ID_SSTCLR(RegCBstr str)
{//{{{ 
    // clear ID_SERSTR according
    unsigned int val = (unsigned int)(*ID_SSTCLR);
    unsigned int inf = (unsigned int)(*ID_SERINF);
    unsigned int flg = (unsigned int)(*ID_SERSTR);
    unsigned int cap_depth = mpICACHEAgentController->GetCapDepth("ID");
    unsigned int reg32_mask = 0xFFFFFFFF;
    for (unsigned int i = 0; i < cap_depth; i++){
        if (((val >> i) & 0x1) == 1){
            // Clear status flag
            flg = flg & (reg32_mask ^ (1 << i));
            // Clear SERINF bit
            unsigned int sedl = (unsigned int)(*ID_nSEADR[i])["SEDL"];
            inf = inf & (reg32_mask ^ (1 << sedl));
        }
    }
    *ID_SERSTR = flg;
    *ID_SERINF = inf;
    (*ID_SSTCLR) = 0;
}//}}}

/// Callback function of ID_DSTCLR_DSTCLR00 bit
void MECCCAP_ID_PEn_Func::cb_ID_DSTCLR_DSTCLR00(RegCBstr str)
{//{{{
    // clear DED, AFE.
    if ((unsigned int)(*ID_DSTCLR)["DSTCLR00"] == 1){
        (*ID_DERSTR)["AFEF00"] = 0;
        (*ID_DERSTR)["DEDF00"] = 0;
    }
    (*ID_DSTCLR) = 0;
}//}}}

/// Callback function of ID_OVFCLR_SERROVF0 bit
void MECCCAP_ID_PEn_Func::cb_ID_OVFCLR_SERROVFCLR0(RegCBstr str)
{//{{{ 
    // clear overflow error status
    if ((unsigned int)(*ID_OVFCLR)["SERROVFCLR0"] == 1){
        (*ID_OVFSTR)["SERROVF0"] = 0;
    }
    if ((unsigned int)(*ID_OVFCLR)["SERROVFCLR1"] == 1){
        (*ID_OVFSTR)["SERROVF1"] = 0;
    }
    (*ID_OVFCLR) = 0;
}//}}}
// vim600: set foldmethod=marker :
