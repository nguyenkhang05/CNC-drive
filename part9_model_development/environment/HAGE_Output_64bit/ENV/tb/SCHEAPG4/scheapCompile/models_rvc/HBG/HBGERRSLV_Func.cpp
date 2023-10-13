// ---------------------------------------------------------------------
// $Id: HBGERRSLV_Func.cpp,v 1.11 2020/03/08 06:28:17 huyquocpham Exp $
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

#include "HBGERRSLV_Func.h"
#include "HBG_AgentController.h"
#include "HBG_Func.h"

///Constructor of HBGERRSLV_Func class
HBGERRSLV_Func::HBGERRSLV_Func (std::string name, HBG_AgentController *HBG_AgentControllerPtr):
    Chbgerrslv_regif(name, 32)
{//{{{
    assert(HBG_AgentControllerPtr != NULL);
    mHBG_AgentController = HBG_AgentControllerPtr;
    mHBG_AgentController->CommandInit(name);
    Chbgerrslv_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of HBGERRSLV_Func class
HBGERRSLV_Func::~HBGERRSLV_Func (void)
{//{{{
}//}}}

///Reset operation of HBGERRSLV_Func class
void HBGERRSLV_Func::Reset (const bool is_active)
{//{{{
    Chbgerrslv_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void HBGERRSLV_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Chbgerrslv_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else if (args[1] == "DumpRegisterRW") {
            msg_return = Chbgerrslv_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        } else {
            msg_return = Chbgerrslv_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void HBGERRSLV_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
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
void HBGERRSLV_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
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

void HBGERRSLV_Func::cb_HBGOVFCLR_CLRO(RegCBstr str)
{//{{{
    unsigned int CLRO_cur = bit_select(str.data, (*HBGOVFCLR)["CLRO"].mStartAddr, (*HBGOVFCLR)["CLRO"].mEndAddr);
    if (1 == CLRO_cur) {
        (*HBGOVFSTAT)["OVF"] = 0;
    }
    (*HBGOVFCLR)["CLRO"] = 0;
}//}}}

void HBGERRSLV_Func::SetVar(const std::string name, const unsigned int val)
{//{{{

    uint32_t SPIDERR_cur = (uint32_t)(*HBGSPIDERRSTAT)["SPIDERR"]; 
    if((unsigned int) ((*HBGSPIDERRSTAT)["SPIDERR"]) == 0) {
        if(name == "DBG") {
            (*HBGERRTYPE)["DBG"] = val;
        }
        else if(name == "UM") {
            (*HBGERRTYPE)["UM"] = val;
        }
        else if(name == "SPID") {
            (*HBGERRTYPE)["SPID"] = val;
        }
        else if(name == "WRITE") {
            (*HBGERRTYPE)["WRITE"] = val;
        }
        else if(name == "ADDR") {
            (*HBGERRADDR) = val;
        } 
        else if(name == "SEC") {
            (*HBGERRTYPE)["SEC"] = val;
        }
    } else if(name == "SPIDERR") {
        (*HBGOVFSTAT)["OVF"] = 0x1;
    }
    if(name == "SPIDERR") {
        (*HBGSPIDERRSTAT)["SPIDERR"] = SPIDERR_cur | val;
    }

}//}}}

unsigned int HBGERRSLV_Func::Get_SPIDERR(void)
{//{{{
    return (unsigned int)(*HBGSPIDERRSTAT)["SPIDERR"];
}//}}}

unsigned int HBGERRSLV_Func::Get_KCE(void)
{//{{{
    return (unsigned int)(*HBGKCPROT)["KCE"];
}//}}}

void HBGERRSLV_Func::cb_HBGSPIDERRCLR_SPIDCLR(RegCBstr str)
{//{{{
    unsigned int SPIDCLR_cur = (unsigned int) str.data;
    (*HBGSPIDERRSTAT)["SPIDERR"] = (~SPIDCLR_cur) & ((unsigned int) (*HBGSPIDERRSTAT)["SPIDERR"]);
    (*HBGSPIDERRCLR )["SPIDCLR"] = 0;
}//}}}

void HBGERRSLV_Func::cb_HBGKCPROT_KCPROT(RegCBstr str)
{//{{{
    if (!(((unsigned int) (*HBGKCPROT)["KCPROT"]) == (0xA5A5A500 >> 1))) {
        (*HBGKCPROT) = str.pre_data;
    }
    (*HBGKCPROT)["KCPROT"] = 0;
}//}}}

// vim600: set foldmethod=marker :

