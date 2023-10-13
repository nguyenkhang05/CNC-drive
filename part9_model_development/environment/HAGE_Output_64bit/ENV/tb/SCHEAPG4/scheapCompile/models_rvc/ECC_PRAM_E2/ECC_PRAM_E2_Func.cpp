// ---------------------------------------------------------------------
// $Id: ECC_PRAM_E2_Func.cpp,v 1.7 2019/09/26 10:07:42 landang Exp $
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
#include "ECC_PRAM_E2_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
ECC_PRAM_E2_Func::ECC_PRAM_E2_Func (std::string name, ECC_PRAM_E2_AgentController* PRAMAgentController):
                Cecc_pram_e2_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(PRAMAgentController != NULL);
    mpPRAMAgentController = PRAMAgentController;
    mpPRAMAgentController->CommandInit(name);
    mModuleName = name;
    mIsErrorJudgment = true;
    Initialize();

    Cecc_pram_e2_regif::set_instance_name(name);
}//}}}

/// Destructor
ECC_PRAM_E2_Func::~ECC_PRAM_E2_Func()
{//{{{
}//}}}

/// Initialize variable
void ECC_PRAM_E2_Func::Initialize()
{//{{{
    mAddrLatch  = 0;
    mLastIndex  = 0;
}//}}}

/// Reset Func and registers
void ECC_PRAM_E2_Func::EnableReset(const bool is_active)
{//{{{
    Cecc_pram_e2_regif::EnableReset(is_active);
    if (is_active){
        Initialize();
    }
}//}}}

/// Read API (target socket)
void ECC_PRAM_E2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int size = (unsigned int)trans.get_data_length();
    unsigned int address = (unsigned int)trans.get_address();
    memset(trans.get_data_ptr(), 0, size);

    /// Get extension info
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    /// get status of reset
    bool mResetStatus = mpPRAMAgentController->GetResetStatus(); 
    /// When clock == 0 or assert reset, only bebug master read.
    if (!mpPRAMAgentController->CheckClockPeriod("pclk") || mResetStatus) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId( ) != DEBUG_MASTER_BMID)) {
            if (mResetStatus) { /// during reset
                re_printf("warning", "Cannot read during reset period\n");
            } else {
                re_printf("warning", "Cannot access register when clock is 0.\n");
            }
            return;
        }
    }
    // Prevent access to "reserved area" (define by CAP_DEPTH)
    unsigned int masked_addr = address & 0x1F; // Mask to get last 8 bits
    unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
    unsigned int start_reserved_seadr = 0x10 + 0x4*err_stag; // start reserved area of address register

    if (start_reserved_seadr <= masked_addr){
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
void ECC_PRAM_E2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (mpPRAMAgentController->GetResetStatus()){
        re_printf("warning", "Cannot access register during reset period.\n");
        return;
    }
    /// Get extension infor
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    
    /// When clock = 0, only bebug master write.
    if (!mpPRAMAgentController->CheckClockPeriod("pclk")) {
        if (!debug || (extension == NULL) || (debug && extension->getPeId() != DEBUG_MASTER_BMID)){
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }
    unsigned int address = (unsigned int)trans.get_address();
    unsigned int masked_addr = address & 0x1F; // Mask to get last 8 bits
    unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
    unsigned int start_reserved_seadr = 0x10 + 0x4*err_stag; // start reserved area of address registers
    if (start_reserved_seadr <= masked_addr){
        re_printf("warning", "Cannot access to reserved area (%08X).\n", address);
    }else{
        if (!debug) {
            reg_wr(address, trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg(address, trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

/// Handler for register IF
void ECC_PRAM_E2_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cecc_pram_e2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Cecc_pram_e2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Get ECDEDF bits
unsigned int ECC_PRAM_E2_Func::GetECDEDFBits()
{//{{{
    unsigned int ret = 0;
    unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
    for (unsigned int i = 0; i < err_stag; i++){
        unsigned int value = ((unsigned int)(*ExCTL) >> (17 + i)) & 0x1;
        ret |= (value << i);
    }
    return ret;
}//}}}

/// Get ECSEDF bits
unsigned int ECC_PRAM_E2_Func::GetECSEDFBits()
{//{{{
    unsigned int ret = 0;
    unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
    for (unsigned int i = 0; i < err_stag; i++){
        unsigned int value = ((unsigned int)(*ExCTL) >> (16 + i)) & 0x1;
        ret |= (value << i);
    }
    return ret;
}//}}}

/// Get ECOVFF bit
unsigned int ECC_PRAM_E2_Func::GetECOVFFBit()
{//{{{
    return (unsigned int)(*ExCTL)["ECOVFF"];
}//}}}

/// Get ExEADn register value
unsigned int ECC_PRAM_E2_Func::GetExEADn(unsigned int index)
{//{{{
    unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
    sc_assert(index < err_stag);
    return (unsigned int)(*ExEADn[index]);
}//}}}

/// Update Error Judgement
void ECC_PRAM_E2_Func::UpdateErrorJudgement()
{//{{{
    bool ecterve_sig = mpPRAMAgentController->GetECTERVESig();
    unsigned int ecthm = (unsigned int)(*ExCTL)["ECTHM"];
    unsigned int ecervf = (unsigned int)(*ExCTL)["ECERVF"];
    if (((!ecterve_sig) && (ecthm == 0))
        || (ecterve_sig && (ecthm == 0) && (ecervf == 1))){
        mIsErrorJudgment = true;
    }else{
        mIsErrorJudgment = false;
        unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
        (*ExCTL)["ECOVFF"] = 0;
        (*ExCTL)["ECER2F"] = 0;
        (*ExCTL)["ECER1F"] = 0;
        (*ExCTL)["ECEMF"] = 0;
        (*ExCTL) = (unsigned int)(*ExCTL) & 0xFFFF; // clear 16 MSB bit (status bits for 1bit error and 2bit error)
        for (unsigned int i = 0; i < err_stag; i++){
            (*ExEADn[i]) = 0;
        }
    }
    mpPRAMAgentController->DriveECTPREFPort();
}//}}}

/// Get error judgment
bool ECC_PRAM_E2_Func::CheckPermitJudgeError()
{//{{{
    return mIsErrorJudgment;
}//}}}

/// Update 1bit error status 
void ECC_PRAM_E2_Func::Process1bitError(unsigned int address)
{//{{{
    int stage = FindRoomToCapAddr(address, false);
    switch (stage){
        case emNoStage:
            Raise1bitErrorInterrupt();
            break;
        case emAlreadyCap:
            // Do nothing
            break;
        case emFull:
            (*ExCTL)["ECOVFF"] = 1;
            mpPRAMAgentController->AssertECTIOVFPort();
            break;
        default:{// has room to capture
            // Update status bit and address according
            unsigned int ctlval = (unsigned int)(*ExCTL) | (1 << (16 + stage*2));
            (*ExEADn[stage]) = ShiftAddress(address);
            mAddrLatch |= (1 << stage); // Record that this address register was used
            mLastIndex = stage;
            (*ExCTL) = ctlval;

            // Drive interrupt
            Raise1bitErrorInterrupt();
            break;
        }
    }
    (*ExCTL)["ECER1F"] = 1;
}//}}}

/// Update 2bit error status 
void ECC_PRAM_E2_Func::ProcessFatalError(unsigned int address)
{//{{{
    int stage = FindRoomToCapAddr(address, true);
    switch (stage){
        case (int)emNoStage:
            Raise2bitErrorInterrupt();
            break;
        case (int)emAlreadyCap:
            // Do nothing
            break;
        case emFull:{
            // Set overflow bit, and assert overflow interrupt
            (*ExCTL)["ECOVFF"] = 1;
            mpPRAMAgentController->AssertECTIOVFPort();
            // Overwrite address, set status, if full of 1bit error
            if (CheckFull1bitErrorStatus()){
                Capture2bitError(address, mLastIndex);
                Raise2bitErrorInterrupt();
            }
            break;
        }
        default:{// has room to capture
            Capture2bitError(address, stage);
            Raise2bitErrorInterrupt();
            break;
        }
    }
    (*ExCTL)["ECER2F"] = 1;
}//}}}

/// Check full 1bit error status
bool ECC_PRAM_E2_Func::CheckFull1bitErrorStatus()
{//{{{
    unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
    unsigned int ctlval = (unsigned int)(*ExCTL);
    for (unsigned int i = 0; i < err_stag; i++){
        if (((ctlval >> (17 + i*2)) & 0x1) == 1){
            return false;
        }
    }
    return true;
}//}}}

/// Capture 2bit error
void ECC_PRAM_E2_Func::Capture2bitError(unsigned int address, unsigned int index)
{//{{{
    // Update status bit and address according
    unsigned int ctlval = (unsigned int)(*ExCTL) | (1 << (17 + index*2));
    (*ExEADn[index]) = ShiftAddress(address); // Capture address
    mAddrLatch |= (1 << index); // Record that this address register was used
    (*ExCTL) = ctlval;
}//}}}

/// Raise 2bit error interupt
void ECC_PRAM_E2_Func::Raise2bitErrorInterrupt()
{//{{{
    // Drive interrupt
    if ((unsigned int)(*ExCTL)["ECER2F"] == 0){
        if ((unsigned int)(*ExCTL)["EC2EDIC"] == 1){
            mpPRAMAgentController->AssertECTIE2Port();
        }
    }
}//}}}

/// Raise 1bit error interrupt
void ECC_PRAM_E2_Func::Raise1bitErrorInterrupt()
{//{{{
    if (((unsigned int)(*ExCTL)["ECER1F"] == 0) && ((unsigned int)(*ExCTL)["ECER2F"] == 0)){
        if ((unsigned int)(*ExCTL)["EC1EDIC"] == 1){
            mpPRAMAgentController->AssertECTIE1Port();
        }
    }
}//}}}

/// Set value to ECEMF bit
void ECC_PRAM_E2_Func::SetECEMFbit(unsigned int value)
{//{{{
    (*ExCTL)["ECEMF"] = value;
}//}}}

/// Get ExCTL register value
unsigned int ECC_PRAM_E2_Func::GetExCTLReg()
{//{{{
    return (unsigned int)(*ExCTL);
}//}}}

/// Shift address based on WORD parameter
unsigned int ECC_PRAM_E2_Func::ShiftAddress(unsigned int address)
{//{{{
    unsigned int ret = 0;
    unsigned int wordval = mpPRAMAgentController->GetWORD();
    ret = address & (unsigned int)((1 << wordval) - 1);
    return ret;
}//}}}

/// Check address is not captured yet
int ECC_PRAM_E2_Func::FindRoomToCapAddr(unsigned int address, bool is_2bit)
{//{{{
    unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
    int ret = (int)emNoStage;
    // Case no stage
    if (err_stag == 0){
        return (int)emNoStage;
    }

    if (CheckAddressSameFactorCaptured(address, is_2bit)){
        return (int)emAlreadyCap; // Captured before
    }else{
        unsigned int mask = (1 << err_stag) - 1;
        if ((mAddrLatch & mask) == mask){
            return (int)emFull;// Case full
        }else{
            for (unsigned int i = 0; i < err_stag; i++){
                if (((mAddrLatch >> i) & 0x1) == 0){// Not capture into this register
                    ret = (int)i;// Case not full, but already captured
                    break;
                }
            }
        }
    }
    
    return ret;
}//}}}

/// Check address captured or not
bool ECC_PRAM_E2_Func::CheckAddressSameFactorCaptured(unsigned int address, bool is_2bit)
{//{{{
    unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
    for (unsigned int i = 0; i < err_stag; i++){
        if ((ShiftAddress(address) == (unsigned int)(*ExEADn[i])) && (((mAddrLatch >> i) & 0x1) == 1)){
            if (((is_2bit) && ((((unsigned int)(*ExCTL) >> (17 + i*2)) & 0x1) == 1))
            || ((!is_2bit) && ((((unsigned int)(*ExCTL) >> (16 + i*2)) & 0x1) == 1))){
                return true;
            }
        }
    }
    return false;
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of cb_ExCTL_ECER1C
void ECC_PRAM_E2_Func::cb_ExCTL_ECER1C(RegCBstr str)
{//{{{ 
    unsigned int err_stag = mpPRAMAgentController->GetERR_STAG();
    // Callback for ECER1C bit -> clear status, flag
    if ((unsigned int)(*ExCTL)["ECER1C"] == 1){
        (*ExCTL)["ECER1F"] = 0;
        (*ExCTL)["ECOVFF"] = 0;
        mAddrLatch = 0;
        mLastIndex = 0;
        (*ExCTL) = (unsigned int)(*ExCTL) & 0xFFFF; // clear 16 MSB bit (status bits for 1bit error and 2bit error)
        for (unsigned int i = 0; i < err_stag; i++){
            (*ExEADn[i]) = 0;
        }
    }

    // Callback for ECER2C bit -> clear status, flag
    if ((unsigned int)(*ExCTL)["ECER2C"] == 1){
        (*ExCTL)["ECER2F"] = 0;
        (*ExCTL)["ECOVFF"] = 0;
        mAddrLatch = 0;
        mLastIndex = 0;
        (*ExCTL) = (unsigned int)(*ExCTL) & 0xFFFF; // clear 16 MSB bit (status bits for 1bit error and 2bit error)
        for (unsigned int i = 0; i < err_stag; i++){
            (*ExEADn[i]) = 0;
        }
    }

    // Always read 0, for bits below:
    (*ExCTL)["ECER1C"] = 0;
    (*ExCTL)["ECER2C"] = 0;
}//}}} 

/// Callback function of cb_ExCTL_ECTHM
void ECC_PRAM_E2_Func::cb_ExCTL_ECTHM(RegCBstr str)
{//{{{
    bool is_write_valid = false;
    unsigned int wr_emca = (unsigned int)((str.data >> 14) & 0x3);
    if ((str.size == 2) && (wr_emca == 0x1)){ // Access 16 bits, check str.data at EMCA bit
        is_write_valid = true;
    }

    // Callback of ECTHM, only be written if EMCA = 0x1
    if (!is_write_valid){
        (*ExCTL)["ECTHM"] = (str.pre_data >> 7) & 0x1 ;// Set previous value for ECTHM
    }

    // Callback of ECERVF, only be written if EMCA = 0x1
    if (! (is_write_valid && (mpPRAMAgentController->GetECTERVESig()))){
        (*ExCTL)["ECERVF"] = (str.pre_data >> 6) & 0x1 ;// Set previous value for ECERVF
    }
    // Update error judgment when bit ECTHM, ECERVF changes
    (*ExCTL)["EMCA"] = 0;
    UpdateErrorJudgement();
}//}}}

// vim600: set foldmethod=marker :
