// ---------------------------------------------------------------------
// $Id: ECCComFunc.cpp,v 1.3 2013/12/09 07:08:31 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "ECCComFunc.h"

///Constructor of ECCComFunc class
ECCComFunc::ECCComFunc (std::string name, 
                       ECCComAgent *ECCComAgentPtr,
                       unsigned int CapDepth,
                       unsigned int ECCCTLInitial,
                       unsigned int ERRINTInitial,
                       bool SupDupsed,
                       bool UeabMode):Cecccom_regif(name, 32, ECCCTLInitial, ERRINTInitial, UeabMode), BusSlaveFuncIf()
{//{{{
    assert(ECCComAgentPtr != NULL);
    mModuleName = name;
    mECCComAgent = ECCComAgentPtr;
    mECCComAgent->CommandInit(name);
    Cecccom_regif::set_instance_name(name);
    if ((CapDepth >= 1)&&(CapDepth <= emMaxCapDepth)) {
        mCapDepth = CapDepth;
    } else {
        mCapDepth = 0x1;
        mECCComAgent->print_msg("error","The capture depth is wrong, a default value (0x1) is set\n");
    }
    mSupDupsed = SupDupsed;
    mUeabMode = UeabMode;
    ERSTR[0] = S1STERSTR;
    ERSTR[1] = S2NDERSTR;
    ERSTR[2] = S3RDERSTR;
    ERSTR[3] = S4THERSTR;
    ERSTR[4] = S5THERSTR;
    ERSTR[5] = S6THERSTR;
    ERSTR[6] = S7THERSTR;
    ERSTR[7] = S8THERSTR;
    for (unsigned int i = 0; i < emBankNo; i++) {
        EADR[i][0] = S1STEADR[i];
        EADR[i][1] = S2NDEADR[i];
        EADR[i][2] = S3RDEADR[i];
        EADR[i][3] = S4THEADR[i];
        EADR[i][4] = S5THEADR[i];
        EADR[i][5] = S6THEADR[i];
        EADR[i][6] = S7THEADR[i];
        EADR[i][7] = S8THEADR[i];
    }
    Initialize();
}//}}}

///Destructor of ECCComFunc class
ECCComFunc::~ECCComFunc (void)
{//{{{
}//}}}

///Initialize variables
void ECCComFunc::Initialize (void)
{//{{{
    for (unsigned int i = 0; i < emBankNo; i++) {
        mDoubleErrorBufferStatus[i] = false;
        for (unsigned int j = 0; j < emMaxCapDepth; j++) {
            mSingleErrorBufferStatus[i][j] = false;
        }
    }
}//}}}

///Handle commands of register IF class
void ECCComFunc::RegisterHandler (const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {
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
                msg_return = Cecccom_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Cecccom_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Process reset function
void ECCComFunc::EnableReset (const bool is_active, const std::string clock_domain)
{//{{{
    Cecccom_regif::EnableReset(is_active, clock_domain);
    if (is_active) {
        Initialize();
    }
}//}}}

///Process error input
void ECCComFunc::ProcessErrorInfo (ErrorInfoStructure error_info, const unsigned int bank_no)
{//{{{
    ErrorInfoStructure legal_info;
    ErrorInfoStructure dup_info;
    FilterCapturedError(error_info, legal_info, dup_info, bank_no);
    if (legal_info.addr_ecc_1bit_error_status || legal_info.data_ecc_1bit_error_status ||
        legal_info.parity_bit_error_status || legal_info.data_ecc_1bit_error_ext_status) { //For single error or parity bit error
        bool overflow = true;
        for (unsigned int i = 0; i < mCapDepth; i++) { // Look up empty buffer
            if (!mSingleErrorBufferStatus[bank_no][i]) { //The buffer is empty
                mSingleErrorBufferStatus[bank_no][i] = true;
                StoreErrorInfo(true, legal_info, bank_no, i);
                overflow = false;
                break;
            }
        }
        ProcessErrorOutput(true, legal_info, bank_no);
        if (overflow) {
            SetOverflow(true, bank_no);
            if ((*ERRINT)["SEOVFIE"] == 1) {
                mECCComAgent->ControlPCLKErrorOutputSignals("errovfout");
            }
        }
    }
    if (legal_info.addr_ecc_2bit_error_status || legal_info.data_ecc_2bit_error_status ||
        legal_info.addr_parity_error_read_status || legal_info.addr_parity_error_write_status ||
        legal_info.data_ecc_2bit_error_ext_status) { // For double error or address parity error
        if (!mDoubleErrorBufferStatus[bank_no]) { //The buffer is empty
            mDoubleErrorBufferStatus[bank_no] = true;
            StoreErrorInfo(false, legal_info, bank_no);
        } else { //If overflow occurs
            SetOverflow(false, bank_no); 
        }
        ProcessErrorOutput(false, legal_info, bank_no);
    }
    if (dup_info.is_ready) { //If duplicated sed/ded error occurs
        if (!mSupDupsed) { //If SupDupsed is inactive for sed errors
            ProcessErrorOutput(true, dup_info, bank_no);
        }
        ProcessErrorOutput(false, dup_info, bank_no);
    }
}//}}}

///Check whether the error is completely same error
void ECCComFunc::FilterCapturedError (ErrorInfoStructure input_info, ErrorInfoStructure &legal_info,
                                     ErrorInfoStructure &duplicated_info, const unsigned int bank_no)
{//{{{
    ErrorInfoStructure info;
    ErrorInfoStructure dup_info;
    info = input_info;
    //For single error or parity bit error
    for (unsigned int i = 0; i < emMaxCapDepth; i++) {
        if (info.error_addr == GetCapturedAddress(true, bank_no, i)) { //Check error status if error address equal to captured error address
            if (info.addr_ecc_1bit_error_status) {
                if ((GetCapturedStatus(true, bank_no, i)&0x40) == 0x40) { //Error cause is captured
                    info.addr_ecc_1bit_error_status = false;
                    dup_info.is_ready = true;
                    dup_info.addr_ecc_1bit_error_status = true;
                    dup_info.error_addr = info.error_addr;
                }
            }
            if (info.data_ecc_1bit_error_ext_status) {
                if ((GetCapturedStatus(true, bank_no, i)&0x10) == 0x10) { //Error cause is captured
                    info.data_ecc_1bit_error_ext_status = false;
                    dup_info.is_ready = true;
                    dup_info.data_ecc_1bit_error_ext_status = true;
                    dup_info.error_addr = info.error_addr;
                }
            }
            if (info.parity_bit_error_status) {
                if ((GetCapturedStatus(true, bank_no, i)&0x8) == 0x8) { //Error cause is captured
                    info.parity_bit_error_status = false;
                    dup_info.is_ready = true;
                    dup_info.parity_bit_error_status = true;
                    dup_info.error_addr = info.error_addr;
                }
            }
            if (info.data_ecc_1bit_error_status) {
                if ((GetCapturedStatus(true, bank_no, i)&0x1) == 0x1) { //Error cause is captured
                    info.data_ecc_1bit_error_status = false;
                    dup_info.is_ready = true;
                    dup_info.data_ecc_1bit_error_status = true;
                    dup_info.error_addr = info.error_addr;
                }
            }
        }
    }
    //For double error or address parity error
    if (info.error_addr == GetCapturedAddress(false, bank_no)) {
        if (info.addr_ecc_2bit_error_status) {
            if ((GetCapturedStatus(false, bank_no)&0x80) == 0x80) { //Error cause is captured
                info.addr_ecc_2bit_error_status = false;
                dup_info.is_ready = true;
                dup_info.addr_ecc_2bit_error_status = true;
                dup_info.error_addr = info.error_addr;
            }
        }
        if (info.data_ecc_2bit_error_ext_status) {
            if ((GetCapturedStatus(false, bank_no)&0x20) == 0x20) { //Error cause is captured
                info.data_ecc_2bit_error_ext_status = false;
                dup_info.is_ready = true;
                dup_info.data_ecc_2bit_error_ext_status = true;
                dup_info.error_addr = info.error_addr;
            }
        }
        if (info.addr_parity_error_read_status || info.addr_parity_error_write_status) {
            if ((GetCapturedStatus(false, bank_no)&0x4) == 0x4) { //Error cause is captured
                info.addr_parity_error_read_status = false;
                info.addr_parity_error_write_status = false;
                dup_info.is_ready = true;
                dup_info.addr_parity_error_read_status = true;
                dup_info.addr_parity_error_write_status = true;
                dup_info.error_addr = info.error_addr;
            }
        }
        if (info.data_ecc_2bit_error_status) {
            if ((GetCapturedStatus(false, bank_no)&0x2) == 0x2) { //Error cause is captured
                info.data_ecc_2bit_error_status = false;
                dup_info.is_ready = true;
                dup_info.data_ecc_2bit_error_status = true;
                dup_info.error_addr = info.error_addr;
            }
        }
    }
    legal_info = info;
    duplicated_info = dup_info;
}//}}}

///Store error information
void ECCComFunc::StoreErrorInfo (const bool is_single_error, ErrorInfoStructure error_info, const unsigned int bank_no, const unsigned int depth)
{//{{{
    std::ostringstream bit_name;
    if (is_single_error) {
        bit_name.str("");
        bit_name << "SEDF" << depth+1 << bank_no;
        (*SEDERSTR)[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_1bit_error_status;
        bit_name.str("");
        bit_name << "SEDF" << depth+1 << bank_no;
        (*ERSTR[depth])[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_1bit_error_status;
        bit_name.str("");
        bit_name << "PBEF" << depth+1 << bank_no;
        (*ERSTR[depth])[bit_name.str().c_str()] = (unsigned int)error_info.parity_bit_error_status;
        bit_name.str("");
        bit_name << "EXSEDF" << depth+1 << bank_no;
        (*ERSTR[depth])[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_1bit_error_ext_status;
        bit_name.str("");
        bit_name << "AECSEDF" << depth+1 << bank_no;
        (*ERSTR[depth])[bit_name.str().c_str()] = (unsigned int)error_info.addr_ecc_1bit_error_status;
        bit_name.str("");
        bit_name << "SEADR" << depth+1;
        (*EADR[bank_no][depth])[bit_name.str().c_str()] = error_info.error_addr;
    } else {
        bit_name.str("");
        bit_name << "DEDF" << bank_no;
        (*D1STERSTR)[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_2bit_error_status;
        bit_name.str("");
        bit_name << "APEF" << bank_no;
        (*D1STERSTR)[bit_name.str().c_str()] = (unsigned int)(error_info.addr_parity_error_read_status|error_info.addr_parity_error_write_status);
        bit_name.str("");
        bit_name << "EXDEDF" << bank_no;
        (*D1STERSTR)[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_2bit_error_ext_status;
        bit_name.str("");
        bit_name << "AECDEDF" << bank_no;
        (*D1STERSTR)[bit_name.str().c_str()] = (unsigned int)error_info.addr_ecc_2bit_error_status;
        (*D1STEADR[bank_no])["DEADR"] = error_info.error_addr;
    }
}//}}}

///Get the value of ECCCTL register
unsigned int ECCComFunc::GetECCCTLValue(void)
{//{{{
    return *ECCCTL;
}//}}}

///Get captured address
unsigned int ECCComFunc::GetCapturedAddress (const bool is_single_error, const unsigned int bank_no, const unsigned int depth)
{//{{{
    unsigned int addr = 0x0;
    if (is_single_error) {
        addr = (unsigned int)(*EADR[bank_no][depth]);
    } else {
        addr = (unsigned int)(*D1STEADR[bank_no]);
    }
    return addr;
}//}}}

///Get captured status
unsigned int ECCComFunc::GetCapturedStatus (const bool is_single_error, const unsigned int bank_no, const unsigned int depth)
{//{{{
    unsigned int captured_status = 0x0;
    if (is_single_error) {
        captured_status = (unsigned int)(((*ERSTR[depth])>>8*bank_no)&0xFF);
    } else {
        captured_status = (unsigned int)(((*D1STERSTR)>>8*bank_no)&0xFF);
    }
    return captured_status;
}//}}}

///Set error overflow flag
void ECCComFunc::SetOverflow (const bool is_single_error, const unsigned int bank_no) 
{//{{{
    std::ostringstream bit_name;
    if (is_single_error) {
        bit_name.str("");
        bit_name << "SERROVF" << bank_no;
        (*OVFSTR)[bit_name.str().c_str()] = 0x1;
    } else {
        bit_name.str("");
        bit_name << "DERROVF" << bank_no;
        (*OVFSTR)[bit_name.str().c_str()] = 0x1;
    }
}//}}}

///Control error output signals
void ECCComFunc::ProcessErrorOutput (const bool is_single_error, ErrorInfoStructure error_info, const unsigned int bank_no)
{//{{{
    if (is_single_error) {
        if ((error_info.addr_ecc_1bit_error_status) && ((*ERRINT)["ASEDIE"] == 1)) {
            mECCComAgent->ControlPCLKErrorOutputSignals("asederrout");
            mECCComAgent->ControlSYS_CLKErrorOutputSignals("asederrout", bank_no, error_info.error_addr);
        }
        if ((error_info.data_ecc_1bit_error_status||error_info.data_ecc_1bit_error_ext_status) && ((*ERRINT)["SEDIE"] == 1)) {
            mECCComAgent->ControlPCLKErrorOutputSignals("sederrout");
            mECCComAgent->ControlSYS_CLKErrorOutputSignals("sederrout", bank_no, error_info.error_addr);
        }
        if ((error_info.parity_bit_error_status) && ((*ERRINT)["PBEIE"] == 1)) {
            mECCComAgent->ControlPCLKErrorOutputSignals("bperrout");
            mECCComAgent->ControlSYS_CLKErrorOutputSignals("bperrout", bank_no, error_info.error_addr);
        }
    } else {
        if ((error_info.addr_ecc_2bit_error_status) && ((*ERRINT)["ADEDIE"] == 1)) {
            mECCComAgent->ControlPCLKErrorOutputSignals("adederrout");
            mECCComAgent->ControlSYS_CLKErrorOutputSignals("adederrout", bank_no, error_info.error_addr);
        }
        if ((error_info.data_ecc_2bit_error_status||error_info.data_ecc_2bit_error_ext_status) && ((*ERRINT)["DEDIE"] == 1)) {
            mECCComAgent->ControlPCLKErrorOutputSignals("dederrout");
            mECCComAgent->ControlSYS_CLKErrorOutputSignals("dederrout", bank_no, error_info.error_addr);
        }
        if ((error_info.addr_parity_error_read_status||error_info.addr_parity_error_write_status) && ((*ERRINT)["APEIE"] == 1)) {
            mECCComAgent->ControlPCLKErrorOutputSignals("aperrout");
            mECCComAgent->ControlSYS_CLKErrorOutputSignals("aperrout", bank_no, error_info.error_addr);
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void ECCComFunc::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void ECCComFunc::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of ecccom_regif class
void ECCComFunc::cb_ECCCTL_ECCDIS (RegCBstr str)
{//{{{
    if (((str.data>>14)&0x3) == 0x1) {//Check PROT bits value equal to 2'b01
        mECCComAgent->WaitPCLKPosEdgeToControlECCDisableSignals();
        //Clear PROT bits value
        *ECCCTL = (*ECCCTL)&0xFFFF3FFF;
    } else {
        *ECCCTL = str.pre_data;
    }
}//}}}

///Virtual function of ecccom_regif class
void ECCComFunc::cb_SSTCLR_SSTCLR10 (RegCBstr str)
{//{{{
    std::ostringstream sstclr_bit_name;
    std::ostringstream sederstr_bit_name;
    std::ostringstream serrovf_bit_name;
    std::ostringstream sedf_bit_name;
    std::ostringstream pbef_bit_name;
    std::ostringstream exsedf_bit_name;
    std::ostringstream aecsedf_bit_name;
    std::ostringstream seadr_bit_name;
    if (!mUeabMode) {
        if (*SSTCLR != 0x0) {
            if (mECCComAgent->CheckClockPeriod("sys_clk")) {
                for (unsigned int depth = 0; depth < emMaxCapDepth; depth++) { //Check bit name according to 1st error to 8th error
                    for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) { //Check bit name according to bank 0 to bank 3
                        sstclr_bit_name.str("");
                        sederstr_bit_name.str("");
                        serrovf_bit_name.str("");
                        sedf_bit_name.str("");
                        pbef_bit_name.str("");
                        exsedf_bit_name.str("");
                        aecsedf_bit_name.str("");
                        seadr_bit_name.str("");
                        sstclr_bit_name << "SSTCLR" << depth+1 << bank_no;
                        sederstr_bit_name << "SEDF" << depth+1 << bank_no;
                        serrovf_bit_name << "SERROVF" << bank_no;
                        sedf_bit_name << "SEDF" << depth+1 << bank_no;
                        pbef_bit_name << "PBEF" << depth+1 << bank_no;
                        exsedf_bit_name << "EXSEDF" << depth+1 << bank_no;
                        aecsedf_bit_name << "AECSEDF" << depth+1 << bank_no;
                        seadr_bit_name << "SEADR" << depth+1;
                        if((*SSTCLR)[sstclr_bit_name.str().c_str()] == 1) {
                            mSingleErrorBufferStatus[bank_no][depth] = false;
                            //Clear status mirror flags in SEDERSTR register
                            (*SEDERSTR)[sederstr_bit_name.str().c_str()] = 0;
                            //Clear oberflow flags in OVFSTR register
                            (*OVFSTR)[serrovf_bit_name.str().c_str()] = 0;
                            //Clear status flags
                            (*ERSTR[depth])[sedf_bit_name.str().c_str()] = 0;
                            (*ERSTR[depth])[pbef_bit_name.str().c_str()] = 0;
                            (*ERSTR[depth])[exsedf_bit_name.str().c_str()] = 0;
                            (*ERSTR[depth])[aecsedf_bit_name.str().c_str()] = 0;
                            //Clear error address
                            (*EADR[bank_no][depth])[seadr_bit_name.str().c_str()] = 0;
                        }
                    }
                }
            } else {
                mECCComAgent->print_msg("error","Can not clear error information when the sys_clk is stopped\n");
            }
        }
    } else {
        mECCComAgent->print_msg("warning","Can not write the value to SSTCLR register\n");
    }
    //Clear the value of SSTCLR register
    *SSTCLR = 0x0;
}//}}}

///Virtual function of ecccom_regif class
void ECCComFunc::cb_DSTCLR_DSTCLR0 (RegCBstr str)
{//{{{
    std::ostringstream dstclr_bit_name;
    std::ostringstream derrovf_bit_name;
    std::ostringstream aecdedf_bit_name;
    std::ostringstream exdedf_bit_name;
    std::ostringstream apef_bit_name;
    std::ostringstream dedf_bit_name;
    if (!mUeabMode) {
        if (((*DSTCLR)&0xF) != 0x0) {
            if (mECCComAgent->CheckClockPeriod("sys_clk")) {
                for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) { //Check bit name according to bank 0 to bank 3
                    dstclr_bit_name.str("");
                    derrovf_bit_name.str("");
                    aecdedf_bit_name.str("");
                    exdedf_bit_name.str("");
                    apef_bit_name.str("");
                    dedf_bit_name.str("");
                    dstclr_bit_name << "DSTCLR" << bank_no;
                    derrovf_bit_name << "DERROVF" << bank_no;
                    aecdedf_bit_name << "AECDEDF" << bank_no;
                    exdedf_bit_name << "EXDEDF" << bank_no;
                    apef_bit_name << "APEF" << bank_no;
                    dedf_bit_name << "DEDF" << bank_no;
                    if ((*DSTCLR)[dstclr_bit_name.str().c_str()] == 1) {
                        mDoubleErrorBufferStatus[bank_no] = false;
                        //Clear overflow flags in OVFSTR register
                        (*OVFSTR)[derrovf_bit_name.str().c_str()] = 0;
                        //Clear status flags in D1STERSTR register
                        (*D1STERSTR)[aecdedf_bit_name.str().c_str()] = 0;
                        (*D1STERSTR)[exdedf_bit_name.str().c_str()] = 0;
                        (*D1STERSTR)[apef_bit_name.str().c_str()] = 0;
                        (*D1STERSTR)[dedf_bit_name.str().c_str()] = 0;
                        //Clear error address in D1STEADR register
                        (*D1STEADR[bank_no])["DEADR"] = 0;
                    }
                }
            } else {
                mECCComAgent->print_msg("error","Can not clear error information when the sys_clk is stopped\n");
            }
        }
    } else {
        mECCComAgent->print_msg("warning","Can not write the value to DSTCLR register\n");
    }
    //Clear the value of DSTCLR register
    *DSTCLR = 0x0;
}//}}}

///Virtual function of ecccom_regif class
void ECCComFunc::cb_STSTCTL_ECCTST (RegCBstr str)
{//{{{
    if (((str.data>>14)&0x3) == 0x1) {//Check PROT bits value equal to 2'b01
        //Clear PROT bits value
        *STSTCTL = (*STSTCTL)&0xFFFF3FFF;
    } else {
        *STSTCTL = str.pre_data;
    }
}//}}}
// vim600: set foldmethod=marker :
