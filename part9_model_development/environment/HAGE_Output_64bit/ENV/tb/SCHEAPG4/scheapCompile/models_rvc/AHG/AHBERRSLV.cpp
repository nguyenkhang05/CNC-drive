// ---------------------------------------------------------------------
// $Id: AHBERRSLV.cpp,v 1.5 2014/09/25 09:05:41 dungvannguyen Exp $
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
#include "AHBERRSLV.h"

///Constructor of AHBERRSLV class
AHBERRSLV::AHBERRSLV(std::string name,
                     AhgAgentController *AhgAgentController_ptr,
                     unsigned int version): 
                     Cahberrslv_regif(name, 32, version),
                     BusSlaveFuncIf()
{//{{{
    assert(AhgAgentController_ptr != NULL);
    mAgentController = AhgAgentController_ptr;
    mAgentController->CommandInit(name);
    Cahberrslv_regif::set_instance_name(name);
}//}}}

///Destructor of AHBERRSLV class
AHBERRSLV::~AHBERRSLV()
{//{{{
}//}}}

//Clear error status
void AHBERRSLV::ClearErrorFlags(bool ovf_flag)
{//{{{
    if (ovf_flag) {//Clear only OVF flag
        (*STAT)["OVF"] = 0;
    } else {//Clear both OVF and ERR flags
        (*STAT)["ERR"] = 0;
        (*STAT)["OVF"] = 0;
    }
}//}}}

///Reset all registers and variables
void AHBERRSLV::Reset(const bool is_active, const bool register_affect)
{//{{{
    if(register_affect) {
        Cahberrslv_regif::EnableReset(is_active);
    } else {
        if (is_active){
            mTempTransInfo.ResetExtInfo();
        }
    }
}//}}}

///Get error information from AhgDataHandler class
void AHBERRSLV::InformError(TlmBasicPayload &trans)
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

///Store error information in registers
void AHBERRSLV::HandleErrInfo(void)
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
        (*TYPE)["SIZE"] = mTempTransInfo.size;
        (*TYPE)["WRITE"] = mTempTransInfo.is_write;
    }
}//}}}

///Dump the information of registers
void AHBERRSLV::DumpStatusInfo(void)
{//{{{
    mAgentController->print_msg("info","AHBERRSLV status information\n");
    mAgentController->print_msg("info","STAT : 0x%X\n", (unsigned int)(*STAT));
    mAgentController->print_msg("info","ADDR : 0x%X\n", (unsigned int)(*ADDR));
    mAgentController->print_msg("info","TYPE : 0x%X\n", (unsigned int)(*TYPE));
}//}}}

///Set parameters and commands to ahberrslv_regif class
std::string AHBERRSLV::RegisterHandler(const std::vector<std::string> &args)
{//{{{
    std::string ret = "";
    if ((args.size() == 1) && (args[0] == "reg")) { // list registers
        printf("List registers of AHBERRSLV\n");
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
void AHBERRSLV::read(unsigned int offsetAddress, 
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
void AHBERRSLV::write(unsigned int offsetAddress, 
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

///Virtual function of ahberrslv_regif class
void AHBERRSLV::cb_CTL_CLRE(RegCBstr str)
{//{{{
    if ((unsigned int)(*CTL)["CLRE"] == 1) {
        mAgentController->print_msg("info","Issue transaction to clear ERR, OVF flags\n");
        mAgentController->WaitTimeToClearErrorFlags(false);//Clear both ovf and err flag
    } else if (((unsigned int)(*CTL)["CLRE"] == 0) && ((unsigned int)(*CTL)["CLRO"] == 1)) {
        mAgentController->print_msg("info","Issue transaction to clear ERR flag\n");
        mAgentController->WaitTimeToClearErrorFlags(true);//Clear only err flags
    }
    *CTL = 0;
}//}}}
// vim600: set foldmethod=marker :

