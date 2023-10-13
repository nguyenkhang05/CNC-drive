// ---------------------------------------------------------------------
// $Id: TSN_Func.cpp,v 1.1 2014/12/10 01:14:39 binhnguyen Exp $
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
#include "TSN_Func.h"
#include "TSN_AgentController.h"

///Constructor of TSN_Func class
TSN_Func::TSN_Func (std::string name, TSN_AgentController *TSN_AgentControllerPtr)
                    :Ctsn_regif(name, 32), BusSlaveFuncIf()
{//{{{
    assert(TSN_AgentControllerPtr != NULL);
    mTSN_AgentController = TSN_AgentControllerPtr;
    mTSN_AgentController->CommandInit(name);
    Ctsn_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of TSN_Func class
TSN_Func::~TSN_Func (void)
{//{{{
}//}}}

///Reset operation of TSN_Func class
void TSN_Func::Reset (const bool is_active)
{//{{{
    Ctsn_regif::EnableReset(is_active);
}//}}}

///Register handler command to tsn_regif class
void TSN_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Ctsn_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Ctsn_regif class
            msg_return = Ctsn_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Dump information of TSN
void TSN_Func::DumpStatusInfo (const unsigned int state)
{//{{{
    std::string state_str = "";
    switch (state) {
        case TSN_AgentController::emRESET:
            state_str = "RESET";
            break;
        case TSN_AgentController::emENABLE:
            state_str = "ENABLE";
            break;
        case TSN_AgentController::emREADY:
            state_str = "READY";
            break;
        case TSN_AgentController::emDISCHARGE:
            state_str = "DISCHARGE";
            break;
        case TSN_AgentController::emCONVERSION:
            state_str = "CONVERSION";
            break;
        case TSN_AgentController::emCONV_DONE:
            state_str = "CONV_DONE";
            break;
        default: // emDISABLE
            state_str = "DISABLE";
            break;
    }
    
    mTSN_AgentController->print_msg("info","TSN status:\n");
    mTSN_AgentController->print_msg("info","TSNSTAT = 0x%X (TSN is in %s state)\n", (unsigned int)(*TSNSTAT), state_str.c_str());
}//}}}

///Virtual function of BusSlaveFuncIf class
void TSN_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void TSN_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Virtual function of register TSNCR in tsn_regif class
void TSN_Func::cb_TSNCR_TSNEN (RegCBstr str)
{//{{{
    if ((!mTSN_AgentController->GetResetStatus("ADRESZ")) && (!mTSN_AgentController->GetResetStatus("presetz"))) {
        if (mTSN_AgentController->CheckClockPeriod("ADCLK")) {
            TSN_AgentController::eState cur_state = mTSN_AgentController->GetState();
            if (((*TSNCR)["TSNEN"] == 1) && (str.pre_data == 0)) {
                if (cur_state == TSN_AgentController::emDISABLE) {
                    mTSN_AgentController->EnableSetting(true);
                }
            } else if (((*TSNCR)["TSNEN"] == 0) && (str.pre_data == 1)) {
                mTSN_AgentController->EnableSetting(false);
                // Clear status flag
                SetStatusFlag(false);
            }
        } else {
            mTSN_AgentController->print_msg("warning","TSN model does not operate when ADCLK is invalid\n");
        }
    } else {
        mTSN_AgentController->print_msg("warning","TSN model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register TSNDIAG in tsn_regif class
void TSN_Func::cb_TSNDIAG_TSNSELFDIAG (RegCBstr str)
{//{{{
    if ((!mTSN_AgentController->GetResetStatus("ADRESZ")) && (!mTSN_AgentController->GetResetStatus("presetz"))) {
        if (!IsTSNEnable()) {
            if ((*TSNDIAG)["TSNSELFDIAG"] == 1) {
                mTSN_AgentController->SelfDiagSetting(true);
            } else {
                mTSN_AgentController->SelfDiagSetting(false);
            }
        } else {
            mTSN_AgentController->print_msg("warning","Cannot change setting for Self-diagnosis function when TSNCR.TSNEN is 1\n");
        }
    } else {
        mTSN_AgentController->print_msg("warning","TSN model does not operate when reset is active\n");
    }
}//}}}

/// Set the status flag
void TSN_Func::SetStatusFlag (const bool is_true)
{//{{{
    (*TSNSTAT)["TSNST"] = (unsigned int)is_true;
}//}}}

/// Check value of TSNCR register
bool TSN_Func::IsTSNEnable (void)
{//{{{
    return (bool)(*TSNCR)["TSNEN"];
}//}}}

// vim600: set foldmethod=marker :
