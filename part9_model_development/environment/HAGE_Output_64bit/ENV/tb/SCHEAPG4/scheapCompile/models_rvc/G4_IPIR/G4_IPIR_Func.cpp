// ---------------------------------------------------------------------
// $Id: G4_IPIR_Func.cpp,v 1.11 2019/07/12 08:25:52 nhuvo Exp $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "G4_IPIR_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
G4_IPIR_Func::G4_IPIR_Func (std::string name, G4_IPIR_AgentController* IPIRAgentController):
                Cg4_ipir_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(IPIRAgentController != NULL);
    mpIPIRAgentController = IPIRAgentController;
    mpIPIRAgentController->CommandInit(name);
    mModuleName = name;
    Cg4_ipir_regif::set_instance_name(name);
    // Initialize
    Initialize();
}//}}}

/// Destructor
G4_IPIR_Func::~G4_IPIR_Func()
{//{{{
}//}}}

/// Initialize variables
void G4_IPIR_Func::Initialize(void)
{//{{{
    for (int i = 0; i < PE_MAX_NUM; i++){
        for (int j = 0; j < INTC_IPIR_CH_NUM; j++){
            maInterrupt[i][j] = false;
        }
    }
}//}}}

/// Reset Func and registers
void G4_IPIR_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        Initialize();
    }
    Cg4_ipir_regif::EnableReset(is_active);
}//}}}

/// Read API (target socket)
void G4_IPIR_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    bool isAccess = false;
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (pExtInfo != NULL) {
        // Get master info
        unsigned int mPEID = pExtInfo->getPeId();
        // Clear data buffer
        memset(trans.get_data_ptr(), 0, trans.get_data_length());
        // Zero clock or reset active

        if ((0 == mpIPIRAgentController->GetClockPeriod()) || (true == mpIPIRAgentController->GetResetStatus())) {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == mPEID)) {
                isAccess = true;
            } else {
                re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            }
        } else {
            isAccess = true;
        }

        if (mpIPIRAgentController->GetExtErr()){
            re_printf("warning", "Cannot read registers during external error.\n");
            return;
        }

        // Convert address (if any)
        unsigned int converted_address = 0;
        if (ConvertAddress(trans, converted_address) && isAccess == true){
            if (!debug) {
                reg_rd(converted_address, trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_rd_dbg(converted_address, trans.get_data_ptr(), trans.get_data_length());
            }
        }
    }
}//}}}

/// Write API (target socket)
void G4_IPIR_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    bool isAccess = false;
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (pExtInfo != NULL) {
        // Get master info
        unsigned int mPEID = pExtInfo->getPeId();
        // Zero clock
        if (0 == mpIPIRAgentController->GetClockPeriod()) {
            // Software debugger
            if ((true == debug) && (DEBUG_MASTER_BMID == mPEID)) {
                isAccess = true;
            } else {
                re_printf("warning", "Cannot write register when clock is 0.\n");
            }
        } else {
            isAccess = true;
        }

        if (mpIPIRAgentController->GetExtErr()){
            re_printf("warning", "Cannot write registers during external error.\n");
            return;
        }
        // Convert address (if any)
        unsigned int converted_address = 0;
        if (ConvertAddress(trans, converted_address) && isAccess == true){
            if (!debug) {
                reg_wr(converted_address, trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_wr_dbg(converted_address, trans.get_data_ptr(), trans.get_data_length());
            }
        }
    } 
}//}}}

/// Convert input address to physical address
bool G4_IPIR_Func::ConvertAddress(TlmBasicPayload& trans, unsigned int& converted_address)
{//{{{
    unsigned int address = (unsigned int)trans.get_address();
    unsigned int masked_addr = address & SELF_MASK_ADDRESS;
    if ((masked_addr < START_OFFSET_SELF_AREA + SELF_AREA_SIZE)){
        TlmG3mExtension *g3mExt = (TlmG3mExtension *)0;
        trans.get_extension(g3mExt);
        sc_assert(g3mExt != NULL);
        unsigned int bmid = (unsigned int)(g3mExt->getPeId());// getPeId() of extension getting BMID
        unsigned int peid = 0;
        if (mpIPIRAgentController->GetPeNum(bmid, peid)){// found bmid
            converted_address = address + SELF_AREA_SIZE + SIZE_PE_SELF_REG*peid; // Convert to physical address
            return true;
        }
    }else{
        converted_address = address; // Not convert
        return true;
    }
    return false;
}//}}}

/// Handler for register IF
void G4_IPIR_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cg4_ipir_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cg4_ipir_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

//========================================================
//============ APIs               ========================
//========================================================
/// Clear registers of a channel
void G4_IPIR_Func::ClearRegsChannel(int channel)
{//{{{
    for (int i = 0; i < PE_MAX_NUM; i++){
        // Clear regs.
        unsigned int reg_group = get_reg_group((unsigned int) i, channel);
        *IPInREQm[reg_group] = 0;
        *IPInENm[reg_group] = 0;
        *IPInFLGm[reg_group] = 0;
        // Write false to interrupt in case level detect
        maInterrupt[i][channel] = false;
    }
    mpIPIRAgentController->DriveInterrupt(maInterrupt);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Get reg_group which is enum in reg_if file to determine index of register
unsigned int G4_IPIR_Func::get_reg_group(unsigned int pe, unsigned int ch)
{//{{{
    return (pe*INTC_IPIR_CH_NUM + ch);
}//}}}

/// Get PE index from the reg_group info
unsigned int G4_IPIR_Func::get_pe_index(unsigned int reg_group)
{//{{{
    return reg_group/INTC_IPIR_CH_NUM;
}//}}}

/// Get CH index from the reg_group info
unsigned int G4_IPIR_Func::get_ch_index(unsigned int reg_group)
{//{{{
    return reg_group%INTC_IPIR_CH_NUM;
}//}}}

/// Callback function of IPInFCLRm register
void G4_IPIR_Func::cb_IPInFCLRm_IPInFCLRm(RegCBstr str)
{//{{{
    unsigned int data = (unsigned int)(*IPInFCLRm[str.channel]);
    bool is_withdraw = false;
    for (unsigned int x = 0; x < PE_MAX_NUM; x++){
        if (((data >> x) & 0x1) == 1){
            unsigned int ch_index = get_ch_index(str.channel);
            unsigned int pe_index = get_pe_index(str.channel);
            unsigned int reg_group = get_reg_group(x, ch_index);
            // Clear IPInREQx[m]
            unsigned int req = (unsigned int)(*IPInREQm[reg_group]);
            req = req & (REG_MASK ^ (1 << pe_index));
            *IPInREQm[reg_group] = req;
            maInterrupt[pe_index][ch_index] = false;
            is_withdraw = true;
            // Clear IPInFLGm[x]
            unsigned int flg = (unsigned int)(*IPInFLGm[str.channel]);
            flg = flg & (REG_MASK ^ (1 << x));
            *IPInFLGm[str.channel] = flg;
        }
    }
    // Clear interrupt if any
    if (is_withdraw){
        mpIPIRAgentController->DriveInterrupt(maInterrupt);
    }
    *IPInFCLRm[str.channel] = 0;// Always read 0
}//}}}

/// Callback function of IPInREQm register
void G4_IPIR_Func::cb_IPInREQm_IPInREQm(RegCBstr str)
{//{{{
    bool has_int = false;
    unsigned int req = (unsigned int)(*IPInREQm[str.channel]);
    unsigned int pre_req = (unsigned int)str.pre_data;

    for (unsigned int x = 0; x < PE_MAX_NUM; x++){
        // REQ change from 0 to 1
        if ((((pre_req >> x) & 0x1) == 0) && (((req >> x) & 0x1) == 1)){
            unsigned int ch_index = get_ch_index(str.channel);
            unsigned int pe_index = get_pe_index(str.channel);
            unsigned int reg_group = get_reg_group(x, ch_index);
            unsigned int en_bit = ((unsigned int)(*IPInENm[reg_group]) >> pe_index) & 0x1;
            if (en_bit == 1){
                // Set 1 to IPInFLGx[m]
                unsigned int flg = (unsigned int)(*IPInFLGm[reg_group]);
                flg = flg | (1<<pe_index);
                *IPInFLGm[reg_group] = flg;
                // Set true to interrupt array at xth (on according channel)
                maInterrupt[x][ch_index] = true;
                has_int = true;
            }else{
                // No operation
            }
        }
    }

    // Issue interrupt if any
    if (has_int){
        mpIPIRAgentController->DriveInterrupt(maInterrupt);
    }
}//}}}

/// Callback function of IPInRCLRm register
void G4_IPIR_Func::cb_IPInRCLRm_IPInRCLRm(RegCBstr str)
{//{{{
    bool is_withdraw = false;
    unsigned int data = (unsigned int)(*IPInRCLRm[str.channel]);
    for (unsigned int x = 0; x < PE_MAX_NUM; x++){
        if (((data >> x) & 0x1) == 1){
            unsigned int ch_index = get_ch_index(str.channel);
            unsigned int pe_index = get_pe_index(str.channel);
            unsigned int reg_group = get_reg_group(x, ch_index);
            // Clear IPInREQm[x]
            unsigned int req = (unsigned int)(*IPInREQm[str.channel]);
            req = req & (REG_MASK ^ (1 << x));
            *IPInREQm[str.channel] = req;
            // Clear IPInFLGx[m], IPIRnREQx[m] if IPInENx[m] = 1
            unsigned int en_bit = ((unsigned int)(*IPInENm[reg_group]) >> pe_index) & 0x1;
            if (en_bit == 1){
                unsigned int flg = (unsigned int)(*IPInFLGm[reg_group]);
                flg = flg & (REG_MASK ^ (1 << pe_index));
                *IPInFLGm[reg_group] = flg;
                unsigned int related_req = (unsigned int)(*IPInREQm[reg_group]);
                related_req = related_req & (REG_MASK ^ (1 << pe_index));
                *IPInREQm[reg_group] = related_req;
            }
            // Set true to interrupt array at xth (on according channel)
            maInterrupt[x][ch_index] = false;
            is_withdraw = true;
        }
    }

    // Withdraw interrupt if any
    if (is_withdraw){
        mpIPIRAgentController->DriveInterrupt(maInterrupt);
    }

    *IPInRCLRm[str.channel] = 0;// Always read 0
}//}}}

// vim600: set foldmethod=marker :
