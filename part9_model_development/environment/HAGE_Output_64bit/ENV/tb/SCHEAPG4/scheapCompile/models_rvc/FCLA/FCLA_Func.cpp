// ---------------------------------------------------------------------
// $Id: FCLA_Func.cpp,v 1.5 2014/10/20 06:20:51 dungvannguyen Exp $
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
#include "FCLA_Func.h"
#include "FCLA_AgentController.h"

///Constructor of FCLA_Func class
FCLA_Func::FCLA_Func (std::string name, FCLA_AgentController *FCLA_AgentControllerPtr)
                    :Cfcla_regif(name, 32), BusSlaveFuncIf()
{//{{{
    assert(FCLA_AgentControllerPtr != NULL);
    mFCLA_AgentController = FCLA_AgentControllerPtr;
    mFCLA_AgentController->CommandInit(name);
    Cfcla_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of FCLA_Func class
FCLA_Func::~FCLA_Func (void)
{//{{{
}//}}}

///Reset operation of FCLA_Func class
void FCLA_Func::Reset (const bool is_active)
{//{{{
    Cfcla_regif::EnableReset(is_active);
}//}}}

///Register handler command to fcla_regif class
void FCLA_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cfcla_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Cfcla_regif class
            msg_return = Cfcla_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Get filter bypass status
bool FCLA_Func::IsFilterBypassEnable (const unsigned int filter_no)
{//{{{
    return ((bool)((*FCLA0CTL[filter_no])["FCLA0BYPS"]));
}//}}}

///Get level detector status
bool FCLA_Func::IsLevelDetectorEnable (const unsigned int filter_no)
{//{{{
    return ((bool)((*FCLA0CTL[filter_no])["FCLA0INTL"]));
}//}}}

///Get rising detect level status
bool FCLA_Func::IsRisingDetectLevel (const unsigned int filter_no)
{//{{{
    return ((bool)((*FCLA0CTL[filter_no])["FCLA0INTR"]));
}//}}}

///Get falling detect level status
bool FCLA_Func::IsFallingDetectLevel (const unsigned int filter_no)
{//{{{
    return ((bool)((*FCLA0CTL[filter_no])["FCLA0INTF"]));
}//}}}

///Virtual function of BusSlaveFuncIf class
void FCLA_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void FCLA_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of register FCLAFLAG in fcla_regif class
void FCLA_Func::cb_FCLA0CTL_FCLA0INTR (RegCBstr str)
{//{{{
    if (!(mFCLA_AgentController->GetResetStatus("SMPRESZ"))) {
        mFCLA_AgentController->ControlEdgeSelectPorts(str.channel, (bool)(str.data&0x1), (bool)((str.data>>1)&0x1));
        mFCLA_AgentController->ControlLevelDetector(str.channel, (bool)((str.data>>2)&0x1));
        mFCLA_AgentController->ControlFilterBypass(str.channel, (bool)((str.data>>7)&0x1));
    } else {
        mFCLA_AgentController->print_msg("warning","FCLA model does not operate when reset of SMPRESZ is active\n");
    }
}//}}}
// vim600: set foldmethod=marker :
