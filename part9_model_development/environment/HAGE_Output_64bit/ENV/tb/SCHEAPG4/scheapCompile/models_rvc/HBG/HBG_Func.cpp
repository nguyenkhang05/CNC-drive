// ---------------------------------------------------------------------
// $Id: HBG_Func.cpp,v 1.10 2020/02/18 07:24:22 huyquocpham Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "HBG_Func.h"
#include "HBGERRSLV_Func.h"
#include "HBG_AgentController.h"

///Constructor of HBG_Func class
HBG_Func::HBG_Func (std::string name, HBG_AgentController *HBG_AgentControllerPtr):
    Chbg_regif(name, 32)

{//{{{
    assert(HBG_AgentControllerPtr != NULL);
    mHBG_AgentController = HBG_AgentControllerPtr;
    mHBG_AgentController->CommandInit(name);
    Chbg_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of HBG_Func class
HBG_Func::~HBG_Func (void)
{//{{{
}//}}}

///Reset operation of HBG_Func class
void HBG_Func::Reset (const bool is_active)
{//{{{
    Chbg_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : SetInitHBGPROT0
/// Description  : Set initial value for HBGPROT0 register
///*********************************
void HBG_Func::SetInitHBGPROT0 (const unsigned int init_val)
{//{{{
    HBGPROT0->UpdateBitInitValue("RG", init_val&0x1);
    HBGPROT0->UpdateBitInitValue("WG", (init_val>>1)&0x1);
    HBGPROT0->UpdateBitInitValue("UM", (init_val>>4)&0x1);
    HBGPROT0->UpdateBitInitValue("DBG",(init_val>>6)&0x1);
    HBGPROT0->UpdateBitInitValue("GEN", (init_val>>8)&0x1);
    HBGPROT0->UpdateBitInitValue("LOCK", (init_val>>31)&0x1);
}//}}}

///*********************************
/// Function     : SetInitHBGPROT1
/// Description  : Set initial value for HBGPROT1 register
///*********************************
void HBG_Func::SetInitHBGPROT1 (const unsigned int init_val)
{//{{{
    HBGPROT1->UpdateBitInitValue("SPID", init_val);
}//}}}


///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void HBG_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Chbg_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else if (args[1] == "DumpRegisterRW") {
            msg_return = Chbg_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        } else {
            msg_return = Chbg_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void HBG_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // When clock = 0, or reset state, only Software debbugger can read register 
    if ( (!mHBG_AgentController->CheckClockPeriod("PCLK")) || mIsReset ){
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
void HBG_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    
    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only Software debbugger write
    if (!mHBG_AgentController->CheckClockPeriod("PCLK")){
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
    
    if (!debug) {
         reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
         reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}


void HBG_Func::cb_HBGPROT0_LOCK(RegCBstr str)
{//{{{   
    if (0x1 == mHBG_AgentController->Get_KCE()) {
        // Process for LOCK bit
        if ((((str.pre_data >> 31) & 0x1) != 0) && (1 == mHBG_AgentController->GetLOCK_SUPPORT())) {
            (*HBGPROT0) = str.pre_data;
        } else {
            mHBG_AgentController->SetHBGPROT0((unsigned int) (*HBGPROT0)["GEN"],
                    (unsigned int) (*HBGPROT0)["DBG"],
                    (unsigned int) (*HBGPROT0)["UM"],
                    (unsigned int) (*HBGPROT0)["WG"],
                    (unsigned int) (*HBGPROT0)["RG"]);

            // Set HBGPROT0.LOCK bit = 0 if LOCK_SUPPORT = 0
            if (0 == mHBG_AgentController->GetLOCK_SUPPORT()) {
                (*HBGPROT0)["LOCK"] = 0;
            }
        }
    } else {
        (*HBGPROT0) = str.pre_data;
    }
}//}}}


void HBG_Func::cb_HBGPROT1_SPID(RegCBstr str)
{//{{{   
    if (mHBG_AgentController->Get_KCE() != 0) {
        if ((((unsigned int) (*HBGPROT0)["LOCK"]) != 0) && (1 == mHBG_AgentController->GetLOCK_SUPPORT())) {
            (*HBGPROT1) = str.pre_data;
        } else {
            mHBG_AgentController->SetHBGPROT1((unsigned int) (*HBGPROT1));
        }
    } else {
        (*HBGPROT1)["SPID"] = str.pre_data;
    }
}//}}}

// vim600: set foldmethod=marker :
