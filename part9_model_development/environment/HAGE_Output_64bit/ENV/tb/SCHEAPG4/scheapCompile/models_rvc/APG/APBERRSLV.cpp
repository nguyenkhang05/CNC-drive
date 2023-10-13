// ---------------------------------------------------------------------
// $Id: APBERRSLV.cpp,v 1.10 2014/09/25 08:51:11 dungvannguyen Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <cstdarg>
#include "APBERRSLV.h"

///Constructor of APBERRSLV class
APBERRSLV::APBERRSLV(std::string name, 
                     ApgAgentController *AgentController_ptr,
                     unsigned int version): 
                     Capberrslv_regif(name, 32, version),
                     BusSlaveFuncIf()
{//{{{
    assert(AgentController_ptr != NULL);
    mAgentController = AgentController_ptr;
    mAgentController->CommandInit(name);
    Capberrslv_regif::set_instance_name(name);
}//}}}

///Destructor of APBERRSLV class
APBERRSLV::~APBERRSLV()
{//{{{
}//}}}

///Reset all registers and variables
void APBERRSLV::Reset(const bool is_active)
{//{{{
    Capberrslv_regif::EnableReset(is_active);
    if (is_active){
        mTempTransInfo.ResetExtInfo();
    }
}//}}}

///Receive error information
void APBERRSLV::InformError(TlmBasicPayload &trans)
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
    
    unsigned int access_size = trans.get_data_length();
    for (unsigned int index = 0; index < 3; index++) {
        if (access_size == (1<<index)) {
            mTempTransInfo.strb = ((1<<access_size)-1) << (mTempTransInfo.addr % (5-access_size));
        }
    }
    bool is_read = trans.is_read();
    bool is_write = trans.is_write();
    if ((is_write) && (!is_read)) {
        mTempTransInfo.is_write = 1;
    } else {
        mTempTransInfo.is_write = 0;
    }
}//}}}

///Store error information in registers
void APBERRSLV::HandleErrInfo()
{//{{{
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
        (*TYPE)["STRB"] = mTempTransInfo.strb;
        (*TYPE)["WRITE"] = mTempTransInfo.is_write;
    }
}//}}}

///Dump the information of registers
void APBERRSLV::DumpStatusInfo(void)
{//{{{
    mAgentController->print_msg("info","APBERRSLV status information\n");
    mAgentController->print_msg("info","STAT : 0x%X\n", (unsigned int)(*STAT));
    mAgentController->print_msg("info","ADDR : 0x%X\n", (unsigned int)(*ADDR));
    mAgentController->print_msg("info","TYPE : 0x%X\n", (unsigned int)(*TYPE));
}//}}}

///Set parameters and commands to apberrslv_regif class
std::string APBERRSLV::RegisterHandler(const std::vector<std::string> &args)
{//{{{
    std::string ret = "";
    if ((args.size() == 1) && (args[0] == "reg")) { // list registers
        printf("List registers of APBERRSLV\n");
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            printf("%s\n", reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        bool dump_msg = true;
        for (unsigned int index = 0; index < args.size(); index++) {
            if ((args[index] == "MessageLevel") || (args[index] == "DumpFileNameLineNum") || (args[index] == "DumpRegisterRW")) {
                dump_msg = false;
            }
        }
        ret = reg_handle_command(args);
        if ((ret != "") && dump_msg) {
            printf("%s\n",ret.c_str());
        }
    }
    return ret;
}//}}}

///Virtual function of BusSlaveFuncIf class
void APBERRSLV::read(unsigned int offsetAddress, 
                     TlmBasicPayload& trans, 
                     BusTime_t* t, 
                     bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void APBERRSLV::write(unsigned int offsetAddress, 
                      TlmBasicPayload& trans, 
                      BusTime_t* t, 
                      bool debug)
{//{{{
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of apberrslv_regif class
void APBERRSLV::cb_CTL_CLRE(RegCBstr str)
{//{{{
    if ((unsigned int)(*CTL)["CLRE"] == 1) {
        (*STAT)["ERR"] = 0;
    }
    if ((unsigned int)(*CTL)["CLRO"] == 1) {
        (*STAT)["OVF"] = 0;
    }
    *CTL = 0;
}//}}}

// vim600: set foldmethod=marker :

