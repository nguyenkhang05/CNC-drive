// ---------------------------------------------------------------------
// $Id: DNFP020_E2_Sub_Func.cpp 2428 2017-03-20 08:21:18Z ChinhTD $
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
#include "DNFP020_E2_Sub_Func.h"
#include "DNFP020_E2_Sub_AgentController.h"

///Constructor of DNFP020_E2_Sub_Func class
DNFP020_E2_Sub_Func::DNFP020_E2_Sub_Func (std::string name, DNFP020_E2_Sub_AgentController *DNFP020_E2_Sub_AgentControllerPtr):
                Cdnfp020_e2_sub_regif(name, 32)
{//{{{
    assert(DNFP020_E2_Sub_AgentControllerPtr != NULL);
    mDNFP020_E2_Sub_AgentController = DNFP020_E2_Sub_AgentControllerPtr;
    mDNFP020_E2_Sub_AgentController->CommandInit(name);
    Cdnfp020_e2_sub_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of DNFP020_E2_Sub_Func class
DNFP020_E2_Sub_Func::~DNFP020_E2_Sub_Func (void)
{//{{{
}//}}}
/// Initialize variables


void DNFP020_E2_Sub_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

void DNFP020_E2_Sub_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Reset operation of DNFP020_E2_Sub_Func class
void DNFP020_E2_Sub_Func::Reset (const bool is_active)
{//{{{
    Cdnfp020_e2_sub_regif::EnableReset(is_active);
}//}}}

///Register handler command to Cdnfp020_e2_sub_regif class
void DNFP020_E2_Sub_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cdnfp020_e2_sub_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Cdnfp020_e2_sub_regif class
            msg_return = Cdnfp020_e2_sub_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

bool DNFP020_E2_Sub_Func::GetNFEN(void)
{//{{{
    return (bool)((*DNFP02EDC)["NFEN"]);
}//}}}

void DNFP020_E2_Sub_Func::SetEDF(void)
{//{{{
    ((*DNFP02EDF)["EDF"]) = 1;
    re_printf("info","EDF is set.\n");
}//}}}


unsigned int DNFP020_E2_Sub_Func::GetDMD(void)
{//{{{
    return(*DNFP02EDC)["DMD"];

}//}}}

/// Virtual function of DNFP02EDF register in Catu5_timerf_sub_regif class
void DNFP020_E2_Sub_Func::cb_DNFP02EDF_CLED (RegCBstr str)
{//{{{
    ((*DNFP02EDF)["EDF"]) = 0;
    ((*DNFP02EDF)["CLED"]) = 0;
    mDNFP020_E2_Sub_AgentController->NotifyEvent(DNFP020_E2_Sub_AgentController::emEDFcleared);
    re_printf("info","EDF is cleared.\n");
}//}}}

/// Virtual function of DNFP02EDF register in Catu5_timerf_sub_regif class
void DNFP020_E2_Sub_Func::cb_DNFP02EDC_NFEN (RegCBstr str)
{//{{{
    unsigned int nfen_pre_data = str.pre_data >> 7;
    if(((unsigned int)(*DNFP02EDC)["NFEN"]==1) && (nfen_pre_data==0)){
        mDNFP020_E2_Sub_AgentController->SetTimePRSChangePoint();
    }
    if((unsigned int)(*DNFP02EDC)["NFEN"] != nfen_pre_data ) {
        mDNFP020_E2_Sub_AgentController->NotifyEvent(DNFP020_E2_Sub_AgentController::emNFENchanged);
    }
}//}}}

// vim600: set foldmethod=marker :
