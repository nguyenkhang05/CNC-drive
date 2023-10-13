// ---------------------------------------------------------------------
// $Id: ECC_DATAFLASH_Func.cpp,v 1.6 2015/09/10 10:58:44 huepham Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <cstdarg>
#include "ECC_DATAFLASH_Func.h"
#include "ECC_DATAFLASH_AgentController.h"

///Constructor of ECC_DATAFLASH_Func class
ECC_DATAFLASH_Func::ECC_DATAFLASH_Func (std::string name,
                 const bool is_addcap_enable,
                 unsigned int addr_pawidth,
                 ECC_DATAFLASH_AgentController *AgentControllerPtr,
                 std::string ECC_DATAFLASH_VERSION):
                 Cecc_dataflash_regif(name,32,is_addcap_enable, addr_pawidth, ECC_DATAFLASH_VERSION),
                 BusSlaveFuncIf()
{//{{{
    assert(AgentControllerPtr != NULL);
    mModuleName = name;
    mAgentController = AgentControllerPtr;
    mAgentController->CommandInit(name);
    Cecc_dataflash_regif::set_instance_name(name);
    mIsAddrCaptureEnable = is_addcap_enable;
    mPawidth = addr_pawidth;
    mVersion = ECC_DATAFLASH_VERSION;
    if (mVersion == "G3M_V30") {
        mDFERRINTInitValue = 0x07;
        SetDFERRINTInitValue(mDFERRINTInitValue);
    } else if (mVersion == "G3KH") {
        mDFERRINTInitValue = 0x02;
        SetDFERRINTInitValue(mDFERRINTInitValue);
    }
}//}}}

///Destructor of ECC_DATAFLASH_Func class
ECC_DATAFLASH_Func::~ECC_DATAFLASH_Func (void)
{//{{{
}//}}}

///Reset registers and variables
void ECC_DATAFLASH_Func::Reset (const bool is_active)
{//{{{
    Cecc_dataflash_regif::EnableReset(is_active);
}//}}}

///Dump information of registers
void ECC_DATAFLASH_Func::DumpStatusInfo (void)
{//{{{
    mAgentController->print_msg("info","Error status\n");
    mAgentController->print_msg("info","SEDF    : 0x%X\n", (unsigned int)(*DFERSTR)["SEDF"]);
    mAgentController->print_msg("info","DEDF    : 0x%X\n", (unsigned int)(*DFERSTR)["DEDF"]);
    if (mIsAddrCaptureEnable) {
        mAgentController->print_msg("info","ERROVF  : 0x%X\n", (unsigned int)(*DFOVFSTR)["ERROVF"]);
    }
}//}}}

///Set parameters to ecc_dataflash_regif class
void ECC_DATAFLASH_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cecc_dataflash_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Cecc_dataflash_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

void ECC_DATAFLASH_Func::SetSEDIEInitValue(const bool value, const bool is_update_now)
{//{{{
    DFERRINT->UpdateBitInitValue("SEDIE", (unsigned int)value) ;
    if (is_update_now) {
        (*DFERRINT)["SEDIE"] = (unsigned int)value;
    }
}//}}}

void ECC_DATAFLASH_Func::SetDFERRINTInitValue(const unsigned int value)
{//{{{
    if (mVersion == "G3M_V30") {
        DFERRINT->UpdateBitInitValue("SEDIE", value&0x1);
        DFERRINT->UpdateBitInitValue("DEDIE", (value>>1)&0x1);
        if (mIsAddrCaptureEnable) {
            DFERRINT->UpdateBitInitValue("EOVFIE", (value>>2)&0x1);
        }
    }else if (mVersion == "G3KH"){
        DFERRINT->UpdateBitInitValue("SEDIE", value&0x0);
        DFERRINT->UpdateBitInitValue("DEDIE", (value>>1)&0x1);
    }
}//}}}

void ECC_DATAFLASH_Func::RecordErrInfo(const unsigned int err_type, const unsigned int err_addr)
{//{{{
    if (CheckEccStatus()) {
        bool is_err_updated = CheckUpdateErrSttCondition();
        unsigned int mod_error_address = (err_addr >> 2) & (~(0xFFFFFFFF<<mPawidth)>>2);
        //Update error stt
        if (err_type == em1BitError) {
            TriggerOffOutputSignal(emECCSED);
        } else {
            TriggerOffOutputSignal(emECCDED);
        }
        if (is_err_updated) {
            UpdateErrorStatus(err_type, mod_error_address);
        } else {
            if (CheckUpdateOvfSttCondition(err_type, mod_error_address)) {
                //Update overflow stt
                (*DFOVFSTR)["ERROVF"] = 1;
                if (mVersion == "G3M_V30") {
                    TriggerOffOutputSignal(emECCOVF);
                } 
            }
        }
    } else {
        mAgentController->print_msg("info","Error information is not recorded because ECC_DATAFLASH is disabled.\n");
    }
}//}}}

///Access read to registers
void ECC_DATAFLASH_Func::read (unsigned int offsetAddress, 
                            TlmBasicPayload& trans, 
                            BusTime_t* t, 
                            bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}//}}}

///Access write to registers
void ECC_DATAFLASH_Func::write (unsigned int offsetAddress, 
                             TlmBasicPayload& trans, 
                             BusTime_t* t, 
                             bool debug)
{//{{{
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}//}}}

void ECC_DATAFLASH_Func::cb_DFECCCTL_ECCDIS(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock()) {
        if (((*DFECCCTL)["PROT0"] == 1) && ((*DFECCCTL)["PROT1"] == 0)){
            TriggerOffOutputSignal(emEccDis);
            TriggerOffOutputSignal(emSecDis);
        } else {
            mAgentController->print_msg("warning","Cannot write to DFECCCTL register because of setting of PROT bits.\n");
            *DFECCCTL = str.pre_data;
        }
    }
    (*DFECCCTL) &= 0x3FFF;
}//}}}

void ECC_DATAFLASH_Func::cb_DFERSTC_ERRCLR(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock()) {
        if ((*DFERSTC)["ERRCLR"] == 1) {
            (*DFERSTR) = 0;
        }
    }
    (*DFERSTC) = 0;
}//}}}

void ECC_DATAFLASH_Func::cb_DFOVFSTC_ERROVFCLR(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock()) {
        if ((*DFOVFSTC)["ERROVFCLR"] == 1) {
            (*DFOVFSTR) = 0;
        }
    }
    (*DFOVFSTC) = 0;
}//}}}

void ECC_DATAFLASH_Func::cb_DFTSTCTL_ECCTST(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock()) {
        if (((*DFTSTCTL)["PROT0"] == 0) || ((*DFTSTCTL)["PROT1"] == 1)){
            mAgentController->print_msg("warning","Cannot write to DFTSTCTL register because of setting of PROT bits.\n");
            *DFTSTCTL = str.pre_data;
        }
    }
    (*DFTSTCTL) &= 0x3FFF;
}//}}}

bool ECC_DATAFLASH_Func::CheckEccStatus(void)
{//{{{
    if ((unsigned int)(*DFECCCTL)["ECCDIS"] == 1) {
        return false;
    } else {
        return true;
    }
}//}}}

bool ECC_DATAFLASH_Func::CheckUpdateErrSttCondition(void)
{//{{{
    if ((unsigned int)(*DFERSTR) != 0){
        return false;
    } else {
        return true;
    }
}//}}}

bool ECC_DATAFLASH_Func::CheckUpdateOvfSttCondition(const unsigned int err_type, const unsigned int err_addr)
{//{{{
    if (mIsAddrCaptureEnable) {
        if ((unsigned int)(*DFEADR)["DFEADR"] == err_addr) {
            if (err_type == em1BitError) {
                if ((unsigned int)(*DFERSTR)["SEDF"] == 1) {
                    return false;
                }
            } else {
                if ((unsigned int)(*DFERSTR)["DEDF"] == 1) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}//}}}

void ECC_DATAFLASH_Func::TriggerOffOutputSignal(const unsigned int output_id)
{//{{{
    switch (output_id) {
        case emECCDED:
            if ((unsigned int)(*DFERRINT)["DEDIE"] == 1) {
                mAgentController->ErrorNotification(emECCDED);
            }
            break;
        case emECCOVF:
            if ((unsigned int)(*DFERRINT)["EOVFIE"] == 1) {
                mAgentController->OverflowNotification();
            }
            break;
        case emEccDis:
            mAgentController->ECCStatusNotification(emEccDis, (bool)(*DFECCCTL)["ECCDIS"]);
            break;
        case emSecDis:
            mAgentController->ECCStatusNotification(emSecDis, (bool)(*DFECCCTL)["SEDDIS"]);
            break;
        default:
            if ((unsigned int)(*DFERRINT)["SEDIE"] == 1) {
                mAgentController->ErrorNotification(emECCSED);
            }
            break;
    }
}//}}}

void ECC_DATAFLASH_Func::UpdateErrorStatus(const unsigned int err_type, const unsigned int err_addr)
{//{{{
    if (err_type == em1BitError) {
        (*DFERSTR)["SEDF"] = 1;
    } else {
        (*DFERSTR)["DEDF"] = 1;
    }
    if (mIsAddrCaptureEnable) {
        (*DFEADR)["DFEADR"] = err_addr;
    }
}//}}}
// vim600: set foldmethod=marker :
