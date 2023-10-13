// ---------------------------------------------------------------------
// $Id: DNFP010_E2_Sub_Func.cpp 5605 2017-03-15 07:40:57Z ChinhTD $
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
#include "DNFP010_E2_Sub_Func.h"
#include "DNFP010_E2_Sub_AgentController.h"

///Constructor of DNFP010_E2_Sub_Func class
DNFP010_E2_Sub_Func::DNFP010_E2_Sub_Func (std::string name, DNFP010_E2_Sub_AgentController *DNFP010_E2_Sub_AgentControllerPtr):
                Cdnfp010_e2_sub_regif(name, 32)
{//{{{
    assert(DNFP010_E2_Sub_AgentControllerPtr != NULL);
    mDNFP010_E2_Sub_AgentController = DNFP010_E2_Sub_AgentControllerPtr;
    mDNFP010_E2_Sub_AgentController->CommandInit(name);
    Cdnfp010_e2_sub_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of DNFP010_E2_Sub_Func class
DNFP010_E2_Sub_Func::~DNFP010_E2_Sub_Func (void)
{//{{{
}//}}}
/// Initialize variables


void DNFP010_E2_Sub_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

void DNFP010_E2_Sub_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Reset operation of DNFP010_E2_Sub_Func class
void DNFP010_E2_Sub_Func::Reset (const bool is_active)
{//{{{
    Cdnfp010_e2_sub_regif::EnableReset(is_active);
}//}}}

///Register handler command to Cdnfp010_e2_sub_regif class
void DNFP010_E2_Sub_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cdnfp010_e2_sub_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Cdnfp010_e2_sub_regif class
            msg_return = Cdnfp010_e2_sub_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

bool DNFP010_E2_Sub_Func::GetNFEN(void)
{//{{{
    return (bool)((*DNFP01CTL)["NFEN"]);
}//}}}

unsigned int DNFP010_E2_Sub_Func::GetSLST(void)
{//{{{
    if((*DNFP01CTL)["SLST"]==0) {
        return 2;
    }
    else if((*DNFP01CTL)["SLST"]==1) {
        return 3;
    }
    else if((*DNFP01CTL)["SLST"]==2) {
        return 4;
    }
    else {
        return 5;
    }
}//}}}

unsigned int DNFP010_E2_Sub_Func::GetPRS(void)
{//{{{
    if((*DNFP01CTL)["PRS"]==0) {
        return 1;
    }
    else if((*DNFP01CTL)["PRS"]==1) {
        return 2;
    }
    else if((*DNFP01CTL)["PRS"]==2) {
        return 4;
    }
    else if((*DNFP01CTL)["PRS"]==3) {
        return 8;
    }
    else if((*DNFP01CTL)["PRS"]==4) {
        return 16;
    }
    else {
        return 0;
    }
}//}}}
/// Virtual function of DNFP01CTL register in Catu5_timerf_sub_regif class
void DNFP010_E2_Sub_Func::cb_DNFP01CTL_SLST (RegCBstr str)
{//{{{
    unsigned int pre_data = (str.pre_data >> 5) & 0x3;
    unsigned int nfen_pre_data = str.pre_data >> 7;
    if((pre_data!=((*DNFP01CTL)["SLST"])) && (((unsigned int)(*DNFP01CTL)["NFEN"]==1) && (nfen_pre_data==1))) {
        re_printf("warning","SLST cannot be changed while operating\n");
        ((*DNFP01CTL)["SLST"]) = pre_data;
    }
}//}}}

void DNFP010_E2_Sub_Func::cb_DNFP01CTL_PRS (RegCBstr str)
{//{{{
    unsigned int pre_data = (str.pre_data) & 0x7;
    unsigned int nfen_pre_data = str.pre_data >> 7;
    if((pre_data != ((*DNFP01CTL)["PRS"])) && (((unsigned int)(*DNFP01CTL)["NFEN"]==1) && (nfen_pre_data==1))) {
        re_printf("warning","PRS cannot be changed while operating\n");
        ((*DNFP01CTL)["PRS"]) = pre_data;
    }
    else if(((unsigned int)(*DNFP01CTL)["NFEN"]==1) && (nfen_pre_data==0)){
        mDNFP010_E2_Sub_AgentController->SetTimePRSChangePoint();
    }
    if((unsigned int)(*DNFP01CTL)["NFEN"] != nfen_pre_data ) {
        mDNFP010_E2_Sub_AgentController->NotifyEvent();
    }
}//}}}

// vim600: set foldmethod=marker :
