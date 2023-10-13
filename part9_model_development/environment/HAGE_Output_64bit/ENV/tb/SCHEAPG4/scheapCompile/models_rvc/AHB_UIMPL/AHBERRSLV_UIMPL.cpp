// ---------------------------------------------------------------------
// $Id: AHBERRSLV_UIMPL.cpp,v 1.2 2014/09/24 01:51:01 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <cstdarg>
#include "AHBERRSLV_UIMPL.h"

///Constructor of AHBERRSLV_UIMPL class
AHBERRSLV_UIMPL::AHBERRSLV_UIMPL(std::string name, 
                 AHBAgentController_UIMPL *AgentControllerPtr,
                 unsigned int version): 
                 Cahberrslv_uimpl_regif(name,32,version),
                 BusSlaveFuncIf()
{//{{{
    assert(AgentControllerPtr != NULL);
    mModuleName = name;
    mTempTransInfo.Initialize();
    mAgentController = AgentControllerPtr;
    mAgentController->CommandInit(name);
    Cahberrslv_uimpl_regif::set_instance_name(name);
}//}}}

///Destructor of AHBERRSLV_UIMPL class
AHBERRSLV_UIMPL::~AHBERRSLV_UIMPL (void)
{//{{{
}//}}}

///Reset registers and variables
void AHBERRSLV_UIMPL::Reset (const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "PRESETZ") {
        Cahberrslv_uimpl_regif::EnableReset(is_active);
    } else {//if reset_name is HRESETZ
        if (is_active) {
            mTempTransInfo.Initialize();
        }
    }
}//}}}

///Dump information of registers
void AHBERRSLV_UIMPL::DumpStatusInfo (void)
{//{{{
    mAgentController->print_msg("info","STAT : 0x%X\n", (unsigned int)(*STAT));
    mAgentController->print_msg("info","ADDR : 0x%X\n", (unsigned int)(*ADDR));
    mAgentController->print_msg("info","TYPE : 0x%X\n", (unsigned int)(*TYPE));
}//}}}

///Set parameters to ahberrslv_uimpl_regif class
void AHBERRSLV_UIMPL::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cahberrslv_uimpl_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Cahberrslv_uimpl_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Inform transaction information
void AHBERRSLV_UIMPL::InformTransInfo (TlmBasicPayload &trans)
{//{{{
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    mTempTransInfo.addr = (unsigned int)(trans.get_address());
    mTempTransInfo.peid = (unsigned int)(ExtensionInfo->getPeId());
    mTempTransInfo.vcid = (unsigned int)(ExtensionInfo->getVmId());
    mTempTransInfo.spid = (unsigned int)(ExtensionInfo->getSpId());                                                                                          
    mTempTransInfo.vm = (unsigned int)(ExtensionInfo->isVirtualMode());
    mTempTransInfo.um = (unsigned int)(ExtensionInfo->isUserMode());
    mTempTransInfo.size = trans.get_data_length();
    bool is_read = trans.is_read();
    bool is_write = trans.is_write();
    if ((is_write) && (!is_read)) {
        mTempTransInfo.is_write = 1;
    } else {
        mTempTransInfo.is_write = 0;
    }
}//}}}

///Handle transaction information after synchonizing with PCLK
void AHBERRSLV_UIMPL::HandleTransInfo (void)
{//{{{
    if (mAgentController->GetResetStatus("PRESETZ") == false) {
        if ((unsigned int)(*STAT)["ERR"] == 1) { //ERR flag is turned on already
            (*STAT)["OVF"] = 1; //OVF flag is turned on
        } else { //Store error info and turn on ERR flag
            (*STAT)["ERR"] = 1;
            (*ADDR)["ADDR"] = mTempTransInfo.addr;
            (*TYPE)["PEID"] = mTempTransInfo.peid;
            (*TYPE)["VCID"] = mTempTransInfo.vcid;
            (*TYPE)["SPID"] = mTempTransInfo.spid;
            (*TYPE)["VM"] = mTempTransInfo.vm;
            (*TYPE)["UM"] = mTempTransInfo.um;
            (*TYPE)["SIZE"] = mTempTransInfo.size;
            (*TYPE)["WRITE"] = mTempTransInfo.is_write;
        }
    }
    mAgentController->ControlIntr();
}//}}}

//Clear error status
void AHBERRSLV_UIMPL::ClearErrorFlags (bool ovf_flag)
{//{{{
    if (ovf_flag) {//Clear only OVF bit
        (*STAT)["OVF"] = 0;
    } else {//Clear both OVF and ERR bits
        (*STAT)["ERR"] = 0;
        (*STAT)["OVF"] = 0;
    }
}//}}}

///Access read to registers
void AHBERRSLV_UIMPL::read (unsigned int offsetAddress, 
                            TlmBasicPayload& trans, 
                            BusTime_t* t, 
                            bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}//}}}

///Access write to registers
void AHBERRSLV_UIMPL::write (unsigned int offsetAddress, 
                             TlmBasicPayload& trans, 
                             BusTime_t* t, 
                             bool debug)
{//{{{
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}//}}}

///Clear error status
void AHBERRSLV_UIMPL::cb_CTL_CLRE (RegCBstr str)
{//{{{
    if ((unsigned int)(*CTL)["CLRE"] == 1) {
        mAgentController->print_msg("info","Receive the transaction to clear ERR and OVF flags\n");
        mAgentController->WaitTimeToClearErrorFlags(false);//Clear both OVF and ERR bit
    } else if (((unsigned int)(*CTL)["CLRE"] == 0) && ((unsigned int)(*CTL)["CLRO"] == 1)) {
        mAgentController->print_msg("info","Receive the transaction to clear OVF flag\n");
        mAgentController->WaitTimeToClearErrorFlags(true);//Clear only OVF bit
    }                      
    *CTL = 0;
}//}}}
// vim600: set foldmethod=marker :
