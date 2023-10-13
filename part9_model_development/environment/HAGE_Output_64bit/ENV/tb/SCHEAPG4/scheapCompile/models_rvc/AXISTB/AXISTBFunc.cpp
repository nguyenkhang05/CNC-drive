// ---------------------------------------------------------------------
// $Id: AXISTBFunc.cpp,v 1.4 2014/10/09 08:49:59 dungvannguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "AXISTBFunc.h"

///Constructor of AXISTBFunc class
AXISTBFunc::AXISTBFunc (std::string name,
                        AXISTB_AgentController *AXISTBAgentControllerPtr):Caxistb_regif(name, 32), BusSlaveFuncIf()
{//{{{
    assert(AXISTBAgentControllerPtr != NULL);
    mAXISTBAgentController = AXISTBAgentControllerPtr;
    mAXISTBAgentController->CommandInit(name);
    Caxistb_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of AXISTBFunc class
AXISTBFunc::~AXISTBFunc (void)
{//{{{
}//}}}

///Reset operation of AXISTBFunc class
void AXISTBFunc::Reset (const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "PRESETn") {
        Caxistb_regif::EnableReset(is_active);
    }
}//}}}

///Dump information of registers AXISTBFLAG, AXISTBTYPE, AXISTBSIDE, AXISTBADDR
void AXISTBFunc::DumpStatusInfo (void)
{//{{{
    mAXISTBAgentController->print_msg("info","AXISTB status information\n");
    mAXISTBAgentController->print_msg("info","  HDMASTAT.GRISAK = 0x%X\n", (unsigned int)(*HDMASTAT)["GRISAK"]);
    mAXISTBAgentController->print_msg("info","  HDMASTAT.SICSAK = 0x%X\n", (unsigned int)(*HDMASTAT)["SICSAK"]);
}//}}}

///Register handler command to ipg_regif class
void AXISTBFunc::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Caxistb_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Caxistb_regif class
            msg_return = Caxistb_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Check halt status
bool AXISTBFunc::CheckHaltStatus(const std::string side_name)
{//{{{
    if (side_name == "GRI") {
        if ((*HDMASTAT)["GRISAK"] == 1) {
            return false;
        } else {
            return true;
        }
    } else {
        if ((*HDMASTAT)["SICSAK"] == 1) {
            return false;
        } else {
            return true;
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void AXISTBFunc::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address();
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd(addr, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void AXISTBFunc::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address();
    if (debug == false) {
        reg_wr(addr, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///virtual function of axistb_regif class
void AXISTBFunc::cb_HDMACTL_GRISRQ(RegCBstr str)
{//{{{
    if (mAXISTBAgentController->CheckClockPeriod("PCLK")) {
        if ((*HDMACTL)["GRISRQ"] == 1) {
            if ((*HDMASTAT)["GRISAK"] == 0) {
                (*HDMASTAT)["GRISAK"] = 1;
                mAXISTBAgentController->print_msg("info","The transfer via isg socket is suspended.\n");
            }
        } else {
            if ((*HDMASTAT)["GRISAK"] == 1) {
                (*HDMASTAT)["GRISAK"] = 0;
                mAXISTBAgentController->print_msg("info","The transfer via isg socket is resumed.\n");
                mAXISTBAgentController->ContinueTransferring("GRI");
            }
        }
 
        if ((*HDMACTL)["SICSRQ"] == 1) {
            if ((*HDMASTAT)["SICSAK"] == 0) {
                (*HDMASTAT)["SICSAK"] = 1;
                mAXISTBAgentController->print_msg("info","The transfer via iss socket is suspended.\n");
            }
        } else {
            if ((*HDMASTAT)["SICSAK"] == 1) {
                (*HDMASTAT)["SICSAK"] = 0;
                mAXISTBAgentController->print_msg("info","The transfer via iss socket is resumed.\n");
                mAXISTBAgentController->ContinueTransferring("SIC");
            }
        }
    }
}//}}}
// vim600: set foldmethod=marker :
