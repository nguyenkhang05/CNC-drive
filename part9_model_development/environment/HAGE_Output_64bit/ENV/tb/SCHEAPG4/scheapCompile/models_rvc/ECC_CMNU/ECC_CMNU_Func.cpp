// ---------------------------------------------------------------------
// $Id: ECC_CMNU_Func.cpp,v 1.6 2015/11/06 02:01:36 ngankimtran2 Exp $
//
// Copyright(c) 2014 - 2015 Renesas Electronics Corporation
// Copyright(c) 2014 - 2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "ECC_CMNU_Func.h"

///*********************************
/// Function     : ECC_CMNU_Func
/// Description  : Constructor of ECC_CMNU_Func class
///*********************************
ECC_CMNU_Func::ECC_CMNU_Func (std::string name, 
                       ECC_CMNU_AgentController *ECCCMNUAgentPtr,
                       unsigned int ECCCTLInitial,
                       unsigned int ERRINTInitial,
                       bool SupDuperr,
                       unsigned int addr_awidth,
                       unsigned int addr_awidth_lsb,
                       std::string model_version):
                       Cecc_cmnu_regif(name, 32, addr_awidth, addr_awidth_lsb, model_version),
                       BusSlaveFuncIf()
{//{{{
    assert(ECCCMNUAgentPtr != NULL);
    mModuleName = name;
    mECCCMNUAgent = ECCCMNUAgentPtr;
    mECCCMNUAgent->CommandInit(name);
    Cecc_cmnu_regif::set_instance_name(name);
    mSupDuperr = SupDuperr;
    mAwidth = addr_awidth;
    mAwidth_lsb = addr_awidth_lsb;
    mVersion = model_version;
    SetECCCTLInitVal(ECCCTLInitial);
    SetERRINTInitVal(ERRINTInitial);
    Initialize();
}//}}}

///*********************************
/// Function     : ~ECC_CMNU_Func
/// Description  : Destructor of ECC_CMNU_Func class
///*********************************
ECC_CMNU_Func::~ECC_CMNU_Func (void)
{//{{{
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize variables used for error capturing/reporting
///*********************************
void ECC_CMNU_Func::Initialize (void)
{//{{{
    for (unsigned int i = 0; i < emBankNo; i++) {
        mDoubleErrorBufferStatus[i] = false;
        mSingleErrorBufferStatus[i] = false;
    }
}//}}}

///*********************************
/// Function     : SetECCCTLInitVal
/// Description  : Set initial value for ECCCTL register
///*********************************
void ECC_CMNU_Func::SetECCCTLInitVal (const unsigned int init_val)
{//{{{
    ECCCTL->UpdateBitInitValue("ECCDIS", init_val&0x1);
    if (mVersion != "ICDM_G3MH" && mVersion != "ICTM_G3MH"){
        ECCCTL->UpdateBitInitValue("SECDIS", (init_val>>1)&0x1);
        if (mVersion == "G3M_V30") {
            ECCCTL->UpdateBitInitValue("AECCDIS", (init_val>>2)&0x1);
            ECCCTL->UpdateBitInitValue("ASECDIS", (init_val>>3)&0x1);
            *ECCCTL |= (init_val & 0xF);
        } else {
            *ECCCTL |= (init_val & 0x3);
        }
    } else {
        *ECCCTL |= (init_val & 0x1);
    }
}//}}}

///*********************************
/// Function     : SetERRINTInitVal
/// Description  : Set initial value for ERRINT register
///*********************************
void ECC_CMNU_Func::SetERRINTInitVal (const unsigned int init_val)
{//{{{
    ERRINT->UpdateBitInitValue("SEDIE", init_val&0x1);
    ERRINT->UpdateBitInitValue("DEDIE", (init_val>>1)&0x1);
    if (mVersion != "ICDM_G3MH" && mVersion != "ICTM_G3MH"){
        ERRINT->UpdateBitInitValue("APEIE", (init_val>>2)&0x1);
        if (mVersion == "G3M_V30") {
            ERRINT->UpdateBitInitValue("PBEIE", (init_val>>3)&0x1);
            ERRINT->UpdateBitInitValue("ASEDIE", (init_val>>4)&0x1);
            ERRINT->UpdateBitInitValue("ADEDIE", (init_val>>5)&0x1);
            ERRINT->UpdateBitInitValue("EOVFIE", (init_val>>6)&0x1);
            *ERRINT |= (init_val & 0x7F);
        } else if (mVersion == "CFB_G3KH" || mVersion == "CFB_G3MH" || mVersion == "CFB_X2X"){
            *ERRINT |= (init_val & 0x7);
        } else if (mVersion == "TCM_G3KH" || mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH"){
            ERRINT->UpdateBitInitValue("PBEIE", (init_val>>3)&0x1);
            *ERRINT |= (init_val & 0xF);
        }
    } else {
        *ERRINT |= (init_val & 0x3);
    }
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ECC_CMNU_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cecc_cmnu_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Cecc_cmnu_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ECC_CMNU_Func::EnableReset (const bool is_active, const std::string clock_domain)
{//{{{
    Cecc_cmnu_regif::EnableReset(is_active, clock_domain);
    if (is_active) {
        Initialize();
    }
}//}}}

///*********************************
/// Function     : ProcessErrorInfo
/// Description  : Control input error information
///*********************************
void ECC_CMNU_Func::ProcessErrorInfo (ECC_CMNU_AgentController::ErrorInfoStructure error_info, const unsigned int bank_no)
{//{{{
    bool is_legal = false;
    bool is_dup = false;
    FilterCapturedError(error_info, is_legal, is_dup, bank_no);

    if (is_legal){
        if ((!mSingleErrorBufferStatus[bank_no])&&(!mDoubleErrorBufferStatus[bank_no])) { //The buffers are empty (status register is empty)
            StoreErrorInfo(error_info, bank_no, false, false);
            if (IsReceivedSingleErr(error_info)) {
                mSingleErrorBufferStatus[bank_no] = true;
            }
            if (IsReceivedDoubleErr(error_info)) {
                mDoubleErrorBufferStatus[bank_no] = true;
            }
        } else {
            if ((IsReceivedDoubleErr(error_info))&&(!mDoubleErrorBufferStatus[bank_no])) { // The double error buffer is empty and double error occurred
                StoreErrorInfo(error_info, bank_no, true, false);
                mDoubleErrorBufferStatus[bank_no] = true;
            }
            if (mVersion == "G3M_V30") {
                if ((*ERRINT)["EOVFIE"] == 1) {
                    mECCCMNUAgent->ControlErrorOutputSignals("errovfout");
                }
            }
            SetOverflow(bank_no);
        }
    }
    if ((is_legal)||
        ((is_dup)&&(!mSupDuperr))) {
        ProcessErrorOutput(error_info);
    }
}//}}}

///*********************************
/// Function     : FilterCapturedError
/// Description  : Check whether error is legal error or same error
///*********************************
void ECC_CMNU_Func::FilterCapturedError (ECC_CMNU_AgentController::ErrorInfoStructure input_info, bool &is_legal,
                                        bool &is_dup, const unsigned int bank_no)
{//{{{
    ECC_CMNU_AgentController::ErrorInfoStructure info;
    unsigned int dect_sts = 0x0;//detected status
    unsigned int dect_addr = 0x0;//detected address

    info = input_info;
    if (mVersion == "G3M_V30") {
        if (info.addr_ecc_2bit_error_status) {
            dect_sts |= 0x80;
        }
        if (info.addr_ecc_1bit_error_status) {
            dect_sts |= 0x40;
        }
        if (info.data_ecc_2bit_error_ext_status) {
            dect_sts |= 0x20;
        }
        if (info.data_ecc_1bit_error_ext_status) {
            dect_sts |= 0x10;
        }
        if (info.parity_bit_error_status) {
            dect_sts |= 0x08;
        }
        if (info.addr_parity_error_read_status || info.addr_parity_error_write_status) {
            dect_sts |= 0x04;
        }
    } else if (mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH") {
        if (info.parity_bit_error_status) {
            dect_sts |= 0x08;
        }
        if (info.addr_parity_error_read_status) {
            dect_sts |= 0x04;
        }
    } else if (mVersion == "CFB_G3MH" || mVersion == "CFB_X2X") {
        if (info.addr_parity_error_read_status) {
            dect_sts |= 0x04;
        }
    }
    if (info.data_ecc_2bit_error_status) {
        dect_sts |= 0x02;
    }
    if (info.data_ecc_1bit_error_status) {
        dect_sts |= 0x01;
    }

    is_dup = false;
    is_legal = false;
    dect_addr = (info.error_addr & (~(0xFFFFFFFFULL<<mAwidth)) & (0xFFFFFFFFULL<<mAwidth_lsb));
    if ((dect_addr == GetCapturedAddress(bank_no)) && (dect_sts == GetCapturedStatus(bank_no))) { //Check same error
        is_dup = true;
    } else {
        is_legal = true;
    }
}//}}}

///*********************************
/// Function     : IsReceivedSingleErr
/// Description  : Check whether received error included single error or not
///*********************************
bool ECC_CMNU_Func::IsReceivedSingleErr (ECC_CMNU_AgentController::ErrorInfoStructure error_info)
{//{{{
    if (mVersion == "G3M_V30") {
        if (error_info.addr_ecc_1bit_error_status || error_info.data_ecc_1bit_error_status ||
            error_info.parity_bit_error_status || error_info.data_ecc_1bit_error_ext_status) {
            return true;
        }
    } else if (mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH"){
        if (error_info.data_ecc_1bit_error_status || error_info.parity_bit_error_status) {
            return true;
        }
    } else if (mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" ||
               mVersion == "CFB_G3MH" || mVersion == "CFB_X2X" ||
               mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH"){
        if (error_info.data_ecc_1bit_error_status) {
            return true;
        }
    }
    return false;
}//}}}

///*********************************
/// Function     : IsReceivedDoubleErr
/// Description  : Check whether received error included double error or not
///*********************************
bool ECC_CMNU_Func::IsReceivedDoubleErr (ECC_CMNU_AgentController::ErrorInfoStructure error_info)
{//{{{
    if (mVersion == "G3M_V30") {
        if (error_info.addr_ecc_2bit_error_status || error_info.data_ecc_2bit_error_status ||
            error_info.addr_parity_error_read_status || error_info.addr_parity_error_write_status ||
            error_info.data_ecc_2bit_error_ext_status) {
            return true;
        }
    } else if (mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH" ||
               mVersion == "CFB_G3MH" || mVersion == "CFB_X2X"){
        if (error_info.data_ecc_2bit_error_status || error_info.addr_parity_error_read_status) {
            return true;
        }
    } else if (mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" ||
               mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH"){
        if (error_info.data_ecc_2bit_error_status) {
            return true;
        }
    }
    return false;
}//}}}

///*********************************
/// Function     : StoreErrorInfo
/// Description  : Store error information
///*********************************
void ECC_CMNU_Func::StoreErrorInfo (ECC_CMNU_AgentController::ErrorInfoStructure error_info, const unsigned int bank_no, const bool sedbuf, const bool dedbuf)
{//{{{
    std::ostringstream bit_name;
    if (dedbuf == false){
        if (sedbuf == false){
            if ((mVersion == "G3M_V30") && (bank_no < emBankNo)) {
                bit_name.str(""); bit_name << "AECSEDF"<< bank_no;
                (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.addr_ecc_1bit_error_status;
                bit_name.str(""); bit_name << "EXSEDF"<< bank_no;
                (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_1bit_error_ext_status;
                bit_name.str(""); bit_name << "PBEF"<< bank_no;
                (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.parity_bit_error_status;
                bit_name.str(""); bit_name << "SEDF"<< bank_no;
                (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_1bit_error_status;
            } else if ((mVersion == "CFB_G3MH" || mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH") && (bank_no < emBank2)) {
                if (mVersion != "CFB_G3MH") {
                    bit_name.str(""); bit_name << "PBEF"<< bank_no;
                    (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.parity_bit_error_status;
                }
                bit_name.str(""); bit_name << "SEDF"<< bank_no;
                (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_1bit_error_status;
            } else if ((mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" || mVersion == "CFB_X2X" ||
                        mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH") && (bank_no < emBank1)) {
                (*ERSTR1ST)["SEDF0"] = (unsigned int)error_info.data_ecc_1bit_error_status;
            }
        }
        if ((mVersion == "G3M_V30") && (bank_no < emBankNo)) {
            bit_name.str(""); bit_name << "AECDEDF"<< bank_no;
            (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.addr_ecc_2bit_error_status;
            bit_name.str(""); bit_name << "EXDEDF"<< bank_no;
            (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_2bit_error_ext_status;
            bit_name.str(""); bit_name << "APEF"<< bank_no;
            (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)(error_info.addr_parity_error_read_status | error_info.addr_parity_error_write_status);
            bit_name.str(""); bit_name << "DEDF"<< bank_no;
            (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_2bit_error_status;
        } else if ((mVersion == "CFB_G3MH" || mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH") && (bank_no < emBank2)) {
            bit_name.str(""); bit_name << "APEF"<< bank_no;
            (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)(error_info.addr_parity_error_read_status);
            bit_name.str(""); bit_name << "DEDF"<< bank_no;
            (*ERSTR1ST)[bit_name.str().c_str()] = (unsigned int)error_info.data_ecc_2bit_error_status;
        } else if ((mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" || mVersion == "CFB_X2X" ||
                    mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH") && (bank_no < emBank1)) {
            if (mVersion == "CFB_X2X") {
                (*ERSTR1ST)["APEF0"] = (unsigned int)error_info.addr_parity_error_read_status;
            }
            (*ERSTR1ST)["DEDF0"] = (unsigned int)error_info.data_ecc_2bit_error_status;
        }
        if (mVersion == "G3M_V30") {
            if (bank_no == 0) {
                *EADR1ST0 = (error_info.error_addr & (~(0xFFFFFFFFULL<<mAwidth)) & (0xFFFFFFFFULL<<mAwidth_lsb));
            } else if (bank_no == 1) {
                *EADR1ST1 = (error_info.error_addr & (~(0xFFFFFFFFULL<<mAwidth)) & (0xFFFFFFFFULL<<mAwidth_lsb));
            } else if (bank_no == 2) {
                *EADR1ST2 = (error_info.error_addr & (~(0xFFFFFFFFULL<<mAwidth)) & (0xFFFFFFFFULL<<mAwidth_lsb));
            } else if (bank_no == 3) {
                *EADR1ST3 = (error_info.error_addr & (~(0xFFFFFFFFULL<<mAwidth)) & (0xFFFFFFFFULL<<mAwidth_lsb));
            }
        } else if (mVersion == "CFB_G3MH" || mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH"){
            if (bank_no == 0) {
                *EADR1ST0 = (error_info.error_addr & (~(0xFFFFFFFFULL<<mAwidth)) & (0xFFFFFFFFULL<<mAwidth_lsb));
            } else if (bank_no == 1) {
                *EADR1ST1 = (error_info.error_addr & (~(0xFFFFFFFFULL<<mAwidth)) & (0xFFFFFFFFULL<<mAwidth_lsb));
            }
        } else if (mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" || mVersion == "CFB_X2X" || mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH") {
            if (bank_no == 0) {
                *EADR1ST0 = (error_info.error_addr & (~(0xFFFFFFFFULL<<mAwidth)) & (0xFFFFFFFFULL<<mAwidth_lsb));
            }
        }
    }
}//}}}

///*********************************
/// Function     : GetCapturedAddress
/// Description  : Get captured error address
///*********************************
unsigned int ECC_CMNU_Func::GetCapturedAddress (const unsigned int bank_no)
{//{{{
    unsigned int addr = 0x0;
    if (mVersion == "G3M_V30") {
        if (bank_no == 0) {
            addr = (unsigned int)(*EADR1ST0);
        } else if (bank_no == 1) {
            addr = (unsigned int)(*EADR1ST1);
        } else if (bank_no == 2) {
            addr = (unsigned int)(*EADR1ST2);
        } else if (bank_no == 3) {
            addr = (unsigned int)(*EADR1ST3);
        }
    } else if (mVersion == "CFB_G3MH" || mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH"){
        if (bank_no == 0) {
            addr = (unsigned int)(*EADR1ST0);
        } else if (bank_no == 1) {
            addr = (unsigned int)(*EADR1ST1);
        }
    } else if (mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" || mVersion == "CFB_X2X" || mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH"){
        if (bank_no == 0) {
            addr = (unsigned int)(*EADR1ST0);
        }
    }
    return addr;
}//}}}

///*********************************
/// Function     : GetCapturedStatus
/// Description  : Get captured error status
///*********************************
unsigned int ECC_CMNU_Func::GetCapturedStatus (const unsigned int bank_no)
{//{{{
    unsigned int captured_status = 0x0;
    if ((mVersion == "G3M_V30") && (bank_no < emBankNo)) {
        captured_status = (unsigned int)(((*ERSTR1ST)>>8*bank_no)&0xFF);
    } else if ((mVersion == "CFB_G3MH") && (bank_no < emBank2)) {
        captured_status = (unsigned int)(((*ERSTR1ST)>>8*bank_no)&0x7);
    } else if ((mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH") && (bank_no < emBank2)) {
        captured_status = (unsigned int)(((*ERSTR1ST)>>8*bank_no)&0xF);
    } else if ((mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" || mVersion == "CFB_X2X" ||
                mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH") && (bank_no < emBank1)) {
        if (mVersion == "CFB_X2X") {
            captured_status = (unsigned int)((*ERSTR1ST)&0x7);
        } else {
            captured_status = (unsigned int)((*ERSTR1ST)&0x3);
        }
    }
    return captured_status;
}//}}}

///*********************************
/// Function     : SetOverflow
/// Description  : Set overflow flag
///*********************************
void ECC_CMNU_Func::SetOverflow (const unsigned int bank_no) 
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");
    if (((mVersion == "G3M_V30") && (bank_no < emBankNo)) ||
        ((mVersion == "CFB_G3MH" || mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH") && (bank_no < emBank2))) {
        bit_name << "ERROVF" << bank_no;
        (*OVFSTR)[bit_name.str().c_str()] = 0x1;
    } else if ((mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" || mVersion == "CFB_X2X" ||
                mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH") && (bank_no < emBank1)) {
        (*OVFSTR)["ERROVF0"] = 0x1;
    }
}//}}}

///*********************************
/// Function     : ProcessErrorOutput
/// Description  : Control error output signals
///*********************************
void ECC_CMNU_Func::ProcessErrorOutput (ECC_CMNU_AgentController::ErrorInfoStructure error_info)
{//{{{
    if (mVersion == "G3M_V30") {
        if ((error_info.addr_ecc_2bit_error_status) && ((*ERRINT)["ADEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("adederrout");
        }
        if ((error_info.addr_ecc_1bit_error_status) && ((*ERRINT)["ASEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("asederrout");
        }
        if ((error_info.parity_bit_error_status) && ((*ERRINT)["PBEIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("bperrout");
        }
        if ((error_info.addr_parity_error_read_status || error_info.addr_parity_error_write_status) &&
            ((*ERRINT)["APEIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("aperrout");
        }
        if ((error_info.data_ecc_2bit_error_status || error_info.data_ecc_2bit_error_ext_status) &&
            ((*ERRINT)["DEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("dederrout");
        }
        if ((error_info.data_ecc_1bit_error_status || error_info.data_ecc_1bit_error_ext_status) &&
            ((*ERRINT)["SEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("sederrout");
        }
    } else if (mVersion == "CFB_G3MH" || mVersion == "CFB_X2X") {
        if ((error_info.addr_parity_error_read_status) &&
            ((*ERRINT)["APEIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("aperrout");
        }
        if ((error_info.data_ecc_2bit_error_status) &&
            ((*ERRINT)["DEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("dederrout");
        }
        if ((error_info.data_ecc_1bit_error_status) &&
            ((*ERRINT)["SEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("sederrout");
        }
    } else if (mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH") {
        if ((error_info.parity_bit_error_status) && ((*ERRINT)["PBEIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("bperrout");
        }
        if ((error_info.addr_parity_error_read_status) &&
            ((*ERRINT)["APEIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("aperrout");
        }
        if ((error_info.data_ecc_2bit_error_status) &&
            ((*ERRINT)["DEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("dederrout");
        }
        if ((error_info.data_ecc_1bit_error_status) &&
            ((*ERRINT)["SEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("sederrout");
        }
    } else if (mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" ||
               mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH"){
        if (error_info.data_ecc_2bit_error_status && ((*ERRINT)["DEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("dederrout");
        }
        if (error_info.data_ecc_1bit_error_status && ((*ERRINT)["SEDIE"] == 1)) {
            mECCCMNUAgent->ControlErrorOutputSignals("sederrout");
        }
    }
}//}}}

///*********************************
/// Function     : GetECCCTLVal
/// Description  : Get ECCCTL regster's value
///*********************************
unsigned int ECC_CMNU_Func::GetECCCTLVal (void)
{//{{{
    unsigned int ret = 0;
    ret = (*ECCCTL);
    return ret;
}//}}}

///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ECC_CMNU_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///*********************************
/// Function     : write
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ECC_CMNU_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///*********************************
/// Function     : cb_ECCCTL_ECCDIS
/// Description  : Virtual function of ecccmnu_regif class
///*********************************
void ECC_CMNU_Func::cb_ECCCTL_ECCDIS (RegCBstr str)
{//{{{
    if (((str.data>>14)&0x3) == 0x1) {//Check PROT bits value equal to 2'b01
        mECCCMNUAgent->ControlECCDisableSignals();
        //Clear PROT bits value
        *ECCCTL = (*ECCCTL)&0xFFFF3FFF;
    } else {
        mECCCMNUAgent->print_msg("warning","Cannot write to ECCCTL register because of setting of PROT bits.\n");
        *ECCCTL = str.pre_data;
    }
}//}}}

///*********************************
/// Function     : cb_STCLR_STCLR0
/// Description  : Virtual function of ecccmnu_regif class
///*********************************
void ECC_CMNU_Func::cb_STCLR_STCLR0 (RegCBstr str)
{//{{{
    std::ostringstream stclr_bit_name;
    if (*STCLR != 0x0) {
        if (mECCCMNUAgent->CheckClockPeriod("sys_clk")) {
            if (mVersion == "G3M_V30") {
                for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) { //Check bit name according to bank 0 to bank 3
                    stclr_bit_name.str(""); stclr_bit_name << "STCLR" << bank_no;
                    if((*STCLR)[stclr_bit_name.str().c_str()] == 1) {
                        mSingleErrorBufferStatus[bank_no] = false;
                        mDoubleErrorBufferStatus[bank_no] = false;
                        stclr_bit_name.str(""); stclr_bit_name << "ERROVF" << bank_no;
                        (*OVFSTR)[stclr_bit_name.str().c_str()] = 0;
                        if (bank_no == 0) {
                            *ERSTR1ST = (*ERSTR1ST)&0xFFFFFF00;
                            *EADR1ST0 = 0x0;
                        } else if (bank_no == 1) {
                            *ERSTR1ST = (*ERSTR1ST)&0xFFFF00FF;
                            *EADR1ST1 = 0x0;
                        } else if (bank_no == 2) {
                            *ERSTR1ST = (*ERSTR1ST)&0xFF00FFFF;
                            *EADR1ST2 = 0x0;
                        } else { //bank_no == 3
                            *ERSTR1ST = (*ERSTR1ST)&0x00FFFFFF;
                            *EADR1ST3 = 0x0;
                        }
                    }
                }
            } else if (mVersion == "CFB_G3MH" || mVersion == "TCM_G3MH" || mVersion == "GRAM_G3MH") {
                for (unsigned int bank_no = 0; bank_no < emBank2; bank_no++) { //Check bit name according to bank 0 to bank 1
                    stclr_bit_name.str(""); stclr_bit_name << "STCLR" << bank_no;
                    if((*STCLR)[stclr_bit_name.str().c_str()] == 1) {
                        mSingleErrorBufferStatus[bank_no] = false;
                        mDoubleErrorBufferStatus[bank_no] = false;
                        stclr_bit_name.str(""); stclr_bit_name << "ERROVF" << bank_no;
                        (*OVFSTR)[stclr_bit_name.str().c_str()] = 0;
                        if (bank_no == 0) {
                            *ERSTR1ST = (*ERSTR1ST)&0xFFFFFF00;
                            *EADR1ST0 = 0x0;
                        } else { //bank_no == 1
                            *ERSTR1ST = (*ERSTR1ST)&0xFFFF00FF;
                            *EADR1ST1 = 0x0;
                        }
                    }
                }
            } else if (mVersion == "CFB_G3KH" || mVersion == "TCM_G3KH" || mVersion == "CFB_X2X" ||
                       mVersion == "ICDM_G3MH" || mVersion == "ICTM_G3MH"){
                if((*STCLR)["STCLR0"] == 1) {
                    mSingleErrorBufferStatus[0] = false;
                    mDoubleErrorBufferStatus[0] = false;
                    (*OVFSTR)["ERROVF0"] = 0;
                    *ERSTR1ST = (*ERSTR1ST)&0xFFFFFF00;
                    *EADR1ST0 = 0x0;
                }
            }
        } else {
            mECCCMNUAgent->print_msg("warning","Cannot clear error information when the sys_clk is stopped.\n");
        }
    }
    //Clear the value of STCLR register
    *STCLR = 0x0;
}//}}}

///*********************************
/// Function     : cb_STSTCTL_ECCTST
/// Description  : Virtual function of ecccmnu_regif class
///*********************************
void ECC_CMNU_Func::cb_STSTCTL_ECCTST (RegCBstr str)
{//{{{
    if (((str.data>>14)&0x3) == 0x1) {//Check PROT bits value equal to 2'b01
        //Clear PROT bits value
        *STSTCTL = (*STSTCTL)&0xFFFF3FFF;
    } else {
        mECCCMNUAgent->print_msg("warning","Cannot write to STSTCTL register because of setting of PROT bits.\n");
        *STSTCTL = str.pre_data;
    }
}//}}}
// vim600: set foldmethod=marker :
