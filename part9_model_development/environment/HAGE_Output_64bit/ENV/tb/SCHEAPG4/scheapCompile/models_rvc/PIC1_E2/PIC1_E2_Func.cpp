// ---------------------------------------------------------------------
// $Id: PIC1_E2_Func.cpp 3395 2017-06-29 08:02:34Z ChinhTD $
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
#include <cstdarg>
#include "PIC1_E2_Func.h"

///*********************************
/// Function     : PIC1_E2_Func
/// Description  : Constructor of PIC1_E2_Func class
///*********************************
PIC1_E2_Func::PIC1_E2_Func (std::string name, 
                       PIC1_E2_AgentController *PIC1AgentPtr):
                       Cpic1_e2_regif(name, 32),
                       BusSlaveFuncIf()
{//{{{
    assert(PIC1AgentPtr != NULL);
    mModuleName = name;
    mPIC1Agent = PIC1AgentPtr;
    mPIC1Agent->CommandInit(name);
    Cpic1_e2_regif::set_instance_name(name);
}//}}}

///*********************************
/// Function     : ~PIC1_E2_Func
/// Description  : Destructor of PIC1_E2_Func class
///*********************************
PIC1_E2_Func::~PIC1_E2_Func (void)
{//{{{
}//}}}

///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void PIC1_E2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
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
void PIC1_E2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void PIC1_E2_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cpic1_e2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Cpic1_e2_regif::reg_handle_command(args);
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
void PIC1_E2_Func::EnableReset (const bool is_active)
{//{{{
    Cpic1_e2_regif::EnableReset(is_active);
}//}}}

///********************************
/// Function    : cb_PIC1SST_SYNCTRG(RegCBstr str)
/// Description : Virtual function of pic1_regif lass
///********************************
void PIC1_E2_Func::cb_PIC1SST_SYNCTRG (RegCBstr str)
{//{{{
    unsigned int ost0;
    unsigned int ost1;
    unsigned int ost2;
    unsigned int ost3;
    unsigned int ost4;
    unsigned int ost5;
    unsigned int ost6;

    if ((*PIC1SST)["SYNCTRG"] != 0) {
        if (mPIC1Agent->CheckClockPeriod("CLK_HSB")) {
            ost0 = (unsigned int) ((*PIC1SSER0)["PIC1SSER000"]);
            ost1 = (unsigned int) ((*PIC1SSER0)["PIC1SSER001"]);
            ost2 = (unsigned int) ((*PIC1SSER0)["PIC1SSER002"]);
            ost3 = (unsigned int) ((*PIC1SSER0)["PIC1SSER003"]);
            ost4 = (unsigned int) ((*PIC1SSER0)["PIC1SSER004"]);
            ost5 = (unsigned int) ((*PIC1SSER0)["PIC1SSER005"]);
            ost6 = (unsigned int) ((*PIC1SSER0)["PIC1SSER006"]);
            if (ost0) {
                mPIC1Agent->StartSimultaneous("OST0TSST");
            }
            if (ost1) {
                mPIC1Agent->StartSimultaneous("OST1TSST");
            }
            if (ost2) {
                mPIC1Agent->StartSimultaneous("OST2TSST");
            }
            if (ost3) {
                mPIC1Agent->StartSimultaneous("OST3TSST");
            }
            if (ost4) {
                mPIC1Agent->StartSimultaneous("OST4TSST");
            }
            if (ost5) {
                mPIC1Agent->StartSimultaneous("OST5TSST");
            }
            if (ost6) {
                mPIC1Agent->StartSimultaneous("OST6TSST");
            }
        } else {
            mPIC1Agent->print_msg("warning","Cannot start simultaneous when clock is invalid.\n");
        }
        (*PIC1SST)["SYNCTRG"] = 0;
    }
}//}}}
// vim600: set foldmethod=marker :
