// ---------------------------------------------------------------------
// $Id: DNFP020_E2_Func.cpp 2489 2017-03-24 03:06:15Z ChinhTD $
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
#include "DNFP020_E2_Func.h"
#include "DNFP020_E2_AgentController.h"

///Constructor of DNFP020_E2_Func class
DNFP020_E2_Func::DNFP020_E2_Func (std::string name, DNFP020_E2_AgentController *DNFP020_E2_AgentControllerPtr):
                Cdnfp020_e2_regif(name, 32)
{//{{{
    assert(DNFP020_E2_AgentControllerPtr != NULL);
    mDNFP020_E2_AgentController = DNFP020_E2_AgentControllerPtr;
    mDNFP020_E2_AgentController->CommandInit(name);
    mModuleName = name;
}//}}}

///Destructor of DNFP020_E2_Func class
DNFP020_E2_Func::~DNFP020_E2_Func (void)
{//{{{
}//}}}

///Register handler command to CDNFP020_E2_regif class
void DNFP020_E2_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cdnfp020_e2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Cdnfp020_e2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void DNFP020_E2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = ((unsigned int)trans.get_address() & 0xFF);
    if(addr < 4) {
        memset(trans.get_data_ptr(), 0, trans.get_data_length());
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
    else {
        unsigned int ch_id = addr >> 3;
        unsigned int reffer_add = ((addr&0xF8) + 4);
        if(addr<reffer_add) {
            ch_id = (addr >> 3) - 1;
        }
        mDNFP020_E2_AgentController->ReadSub(ch_id,offsetAddress, trans, t, debug);
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void DNFP020_E2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = ((unsigned int)trans.get_address() & 0xFF);
    if(addr < 4) {
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
    else {
        unsigned int ch_id = addr >> 3;
        unsigned int reffer_add = ((addr&0xF8) + 4);
        if(addr<reffer_add) {
            ch_id = (addr >> 3) - 1;
        }
        mDNFP020_E2_AgentController->WriteSub(ch_id, offsetAddress, trans, t, debug);
    }
}//}}}
unsigned int DNFP020_E2_Func::GetSLST(void)
{//{{{
    if((*DNFP02CTL)["SLST"]==0) {
        re_printf("info"," Sampling count = 2\n");
        return 2;
    }
    else if((*DNFP02CTL)["SLST"]==1) {
        re_printf("info"," Sampling count = 3\n");
        return 3;
    }
    else if((*DNFP02CTL)["SLST"]==2) {
        re_printf("info"," Sampling count = 4\n");
        return 4;
    }
    else {
        re_printf("info"," Sampling count = 5\n");
        return 5;
    }
}//}}}

unsigned int DNFP020_E2_Func::GetPRS(void)
{//{{{
    if((*DNFP02CTL)["PRS"]==0) {
        re_printf("info","DNF input clock is divided by 1\n");
        return 1;
    }
    else if((*DNFP02CTL)["PRS"]==1) {
        re_printf("info","DNF input clock is divided by 2\n");
        return 2;
    }
    else if((*DNFP02CTL)["PRS"]==2) {
        re_printf("info","DNF input clock is divided by 4\n");
        return 4;
    }
    else if((*DNFP02CTL)["PRS"]==3) {
        re_printf("info","DNF input clock is divided by 8\n");
        return 8;
    }
    else if((*DNFP02CTL)["PRS"]==4) {
        re_printf("info","DNF input clock is divided by 16\n");
        return 16;
    }
    else {
        re_printf("info","Setting of PRS is prohibited\n");
        return 0;
    }
}//}}}

/// Virtual function of DNFP01CTL register in Catu5_timerf_sub_regif class
void DNFP020_E2_Func::cb_DNFP02CTL_SLST (RegCBstr str)
{//{{{
    unsigned int pre_data = (str.pre_data >> 5) & 0x3;
    if((pre_data != ((*DNFP02CTL)["SLST"])) && mDNFP020_E2_AgentController->GetSubNFEN()) {
        re_printf("warning","SLST cannot be changed while operating\n");
        ((*DNFP02CTL)["SLST"]) = pre_data;
    }
}//}}}

void DNFP020_E2_Func::cb_DNFP02CTL_PRS (RegCBstr str)
{//{{{
    unsigned int pre_data = (str.pre_data) & 0x7;
    if((pre_data != ((*DNFP02CTL)["PRS"])) && mDNFP020_E2_AgentController->GetSubNFEN()) {
        re_printf("warning","PRS cannot be changed while operating\n");
        ((*DNFP02CTL)["PRS"]) = pre_data;
    }
}//}}}

///Reset operation of DNFP020_E2_Func class
void DNFP020_E2_Func::Reset (const bool is_active)
{//{{{
    Cdnfp020_e2_regif::EnableReset(is_active);
}//}}}
// vim600: set foldmethod=marker :
