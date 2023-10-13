// ---------------------------------------------------------------------
// $Id: HI20FSGD2_Func.cpp,v 1.5 2019/12/06 07:19:03 hadinhtran Exp $
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

#include "HI20FSGD2_Func.h"
#include "HI20FSGD2_AgentController.h"


///Constructor of HI20FSGD2_Func class
HI20FSGD2_Func::HI20FSGD2_Func (std::string name, HI20FSGD2_AgentController *HI20FSGD2_AgentControllerPtr, unsigned int ch_num):
                Chi20fsgd2_regif(name, 32, ch_num)
{//{{{
    assert(HI20FSGD2_AgentControllerPtr != NULL);
    mHI20FSGD2_AgentController = HI20FSGD2_AgentControllerPtr;
    mHI20FSGD2_AgentController->CommandInit(name);
    Chi20fsgd2_regif::set_instance_name(name);
    mModuleName = name;
    Initialize();
}//}}}

///Destructor of HI20FSGD2_Func class
HI20FSGD2_Func::~HI20FSGD2_Func (void)
{//{{{
}//}}}

///Reset operation of HI20FSGD2_Func class
void HI20FSGD2_Func::Reset (const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    Chi20fsgd2_regif::EnableReset(is_active);
}//}}}

/// Initialize internal variables
void HI20FSGD2_Func::Initialize(void)
{//{{{
    mSecurity = false;
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void HI20FSGD2_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Chi20fsgd2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Chi20fsgd2_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
        } else {
            msg_return = Chi20fsgd2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void HI20FSGD2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (pExtInfo != NULL){
        mSecurity = pExtInfo->isSecure();
    }

    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // When clock = 0, or reset state, only debug master can read register 
    if ( (!mHI20FSGD2_AgentController->CheckClockPeriod()) || (mHI20FSGD2_AgentController->GetResetStatus()) ){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void HI20FSGD2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (pExtInfo != NULL){
        mSecurity = pExtInfo->isSecure();
    }

    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only debug master write
    if (!mHI20FSGD2_AgentController->CheckClockPeriod()){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }

    // write register if possible
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Set initial value for register
void HI20FSGD2_Func::SetInitValReg (const std::string reg_name, const unsigned int ch_id, unsigned int value)
{//{{{
    if ((reg_name == "PBGPROT0") && (ch_id < mHI20FSGD2_AgentController->mChannelNum))
    {
        //Check valid initial value of PBGPROT0 register
        if ((value == emPROT0InitVal0) || (value == emPROT0InitVal1) || (value == emPROT0InitVal2)){
            unsigned int val_RG =   value & 0x1;
            unsigned int val_WG =   (value >> 1) & 0x1;
            unsigned int val_UM =   (value >> 4) & 0x1;
            unsigned int val_DBG =  (value >> 6) & 0x1;
            unsigned int val_SEC =  (value >> 7) & 0x1;
            unsigned int val_GEN =  (value >> 8) & 0x1;
            unsigned int val_LOCK = (value >> 31) & 0x1;
            
            PBGPROT0[ch_id]->UpdateBitInitValue("RG", val_RG);
            PBGPROT0[ch_id]->UpdateBitInitValue("WG", val_WG);
            PBGPROT0[ch_id]->UpdateBitInitValue("UM", val_UM);
            PBGPROT0[ch_id]->UpdateBitInitValue("DBG", val_DBG);
            PBGPROT0[ch_id]->UpdateBitInitValue("SEC", val_SEC);
            PBGPROT0[ch_id]->UpdateBitInitValue("GEN", val_GEN);
            PBGPROT0[ch_id]->UpdateBitInitValue("LCOK", val_LOCK);
        }
    }

    if ((reg_name == "PBGPROT1") && (ch_id < mHI20FSGD2_AgentController->mChannelNum))
    {
        //Check valid initial value of PBGPROT1 register
        if ((value == emPROT1InitVal0) || (value == emPROT1InitVal1)){
            PBGPROT1[ch_id]->UpdateBitInitValue("SPID", value);
        }
    }
}//}}}

/// Callback function of PBGPROT0_LOCK bit
void HI20FSGD2_Func::cb_PBGPROT0_LOCK(RegCBstr str)
{//{{{
    if (mHI20FSGD2_AgentController->CheckKceBitStatus()){
        if ((!(mHI20FSGD2_AgentController->mICUMHA)) || ((mHI20FSGD2_AgentController->mICUMHA) && (mSecurity))){
            if((bool)((str.pre_data>>31)&0x1) && (1 == mHI20FSGD2_AgentController->GetLOCK_SUPPORT())){
                (*PBGPROT0[str.channel]) = str.pre_data;
            }
            else {
                mHI20FSGD2_AgentController->SetPBGPROT0((*PBGPROT0[str.channel])["SEC"],(*PBGPROT0[str.channel])["GEN"], (*PBGPROT0[str.channel])["DBG"], (*PBGPROT0[str.channel])["UM"], (*PBGPROT0[str.channel])["WG"], (*PBGPROT0[str.channel])["RG"],str.channel);
            }
        }else {
            (*PBGPROT0[str.channel]) = str.pre_data;
        }
    }else {
        (*PBGPROT0[str.channel]) = str.pre_data;
    }
}//}}}

/// Callback function of PBGPROT1_SPID bit
void HI20FSGD2_Func::cb_PBGPROT1_SPID(RegCBstr str)
{//{{{
    if((1 == (unsigned int)(*PBGPROT0[str.channel])["LOCK"]) && (1 == mHI20FSGD2_AgentController->GetLOCK_SUPPORT())
        || (!(mHI20FSGD2_AgentController->CheckKceBitStatus())))
    {
        (*PBGPROT1[str.channel]) = str.pre_data;
    }
    else {
        mHI20FSGD2_AgentController->SetPBGPROT1((*PBGPROT1[str.channel]),str.channel);
    }
}//}}}

