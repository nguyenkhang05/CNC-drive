// ---------------------------------------------------------------------
// $Id: PBG_Func.cpp,v 1.9 2019/06/16 06:02:16 nhuvo Exp $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "PBG_Func.h"
#include "PBG_AgentController.h"


///Constructor of PBG_Func class
PBG_Func::PBG_Func (std::string name, PBG_AgentController *PBG_AgentControllerPtr, unsigned int ch_num):
                Cpbguard_regif(name, 32, ch_num)
{//{{{
    assert(PBG_AgentControllerPtr != NULL);
    mPBG_AgentController = PBG_AgentControllerPtr;
    mPBG_AgentController->CommandInit(name);
    Cpbguard_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of PBG_Func class
PBG_Func::~PBG_Func (void)
{//{{{
}//}}}

///Reset operation of PBG_Func class
void PBG_Func::Reset (const bool is_active)
{//{{{
    Cpbguard_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void PBG_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cpbguard_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Cpbguard_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
        } else {
            msg_return = Cpbguard_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void PBG_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (pExtInfo == NULL) {
        return;
    }

    int mPEID = pExtInfo->getPeId();
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if ((false == mPBG_AgentController->CheckClockPeriod("PCLK")) || (true == mPBG_AgentController->GetResetStatus())) {
        // Software debugger
        if ((true == debug) && (DEBUG_MASTER_BMID == mPEID)) {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
        }
    } else {
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void PBG_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if(pExtInfo == NULL) {
        return;
    }
    
    int mPEID = pExtInfo->getPeId();
    if (mPBG_AgentController->CheckClockPeriod("PCLK")){
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else {
        // Software debugger
        if ((true == debug) && (DEBUG_MASTER_BMID == mPEID)) {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            SC_REPORT_WARNING(mModuleName.c_str(), "Cannot write register when clock is 0");
        }
    }
}//}}}

void PBG_Func::cb_PBGERRCLR_CLRO(RegCBstr str)
{//{{{
    if(mPBG_AgentController->CheckClockPeriod("PCLK") && str.is_wr) {
        unsigned int CLRE_cur = bit_select(str.data
                                ,(*PBGERRCLR)["CLRE"].mStartAddr
                                ,(*PBGERRCLR)["CLRE"].mEndAddr);
        unsigned int CLRO_cur = bit_select(str.data
                                ,(*PBGERRCLR)["CLRO"].mStartAddr
                                ,(*PBGERRCLR)["CLRO"].mEndAddr);
        if (1 == CLRE_cur){
            (*PBGERRSTAT)["ERR"] = 0;
        }
        
        if(1 == CLRO_cur){
            (*PBGERRSTAT)["OVF"] = 0;
        }
    }
}//}}}
void PBG_Func::cb_PBGKCPROT_KCPROT(RegCBstr str)
{//{{{
    if(mPBG_AgentController->CheckClockPeriod("PCLK")) {
        if(!((*PBGKCPROT)["KCPROT"]==(0xA5A5A500>>1))) {
            (*PBGKCPROT)=str.pre_data;
        }
    }
    (*PBGKCPROT)["KCPROT"] = 0;
}//}}}

void PBG_Func::cb_PBGPROT0_LOCK(RegCBstr str)
{//{{{   
    if( (mPBG_AgentController->CheckClockPeriod("PCLK")) && (str.channel < mPBG_AgentController->mChannelNum)) {          
          //uint8_t pre_val_SEC = (*PBGPROT0[str.channel])["SEC"];
          unsigned int pre_val_SEC = (unsigned int) bit_select(str.pre_data
                                    ,(*PBGPROT0[str.channel])["SEC"].mStartAddr
                                    ,(*PBGPROT0[str.channel])["SEC"].mEndAddr);
          if(((((str.pre_data>>31)&0x1) && (true == mPBG_AgentController->GetLOCK_SUPPORT())) == true)
          || !((*PBGKCPROT)["KCE"])) {
              (*PBGPROT0[str.channel]) = str.pre_data;
          }
          else {
              mPBG_AgentController->SetPBGPROT0((*PBGPROT0[str.channel])["GEN"], (*PBGPROT0[str.channel])["DBG"], (*PBGPROT0[str.channel])["UM"], (*PBGPROT0[str.channel])["WG"], (*PBGPROT0[str.channel])["RG"],(*PBGPROT0[str.channel])["SEC"],str.channel);
              if (true == mPBG_AgentController->GetSEC_DISABLED()) {
                  (*PBGPROT0[str.channel])["SEC"] = pre_val_SEC;
              }
          }  
    }
}//}}}

void PBG_Func::cb_PBGPROT1_SPID(RegCBstr str)
{//{{{   
    if((mPBG_AgentController->CheckClockPeriod("PCLK"))
        && (str.channel < mPBG_AgentController->mChannelNum)) {        
          uint32_t val_LOCL_PBGPROT0 = (*PBGPROT0[str.channel]) >> 31;
          if( (((val_LOCL_PBGPROT0 & 0x1) && (true == mPBG_AgentController->GetLOCK_SUPPORT())) == true)
                 || !((*PBGKCPROT)["KCE"])) {
              (*PBGPROT1[str.channel]) = str.pre_data;
          }
          else {
              mPBG_AgentController->SetPBGPROT1((*PBGPROT1[str.channel]), str.channel);
          }  
    }
}//}}}

void PBG_Func::SetVar(const std::string name, const unsigned int val) 
{//{{{
    //Get value of ERR bit
    uint8_t ERR_val = (*PBGERRSTAT)["ERR"];
    if (ERR_val == 0) {
        if(name=="DBG") {
            (*PBGERRTYPE)["DBG"] = val;
        }
        else if(name=="UM") {
            (*PBGERRTYPE)["UM"] = val;
        }
        else if(name=="SPID") {
            (*PBGERRTYPE)["SPI"] = val;
        }
        else if(name=="WRITE") {
            (*PBGERRTYPE)["WRITE"] = val;
        }
        else if(name=="ADDR") {
            (*PBGERRADDR) = val;
        }
        else if(name=="SEC") {
            (*PBGERRTYPE)["SEC"] = val;
        }
    }
    
    if(name=="ERR") {
        if((*PBGERRSTAT)["ERR"]==1) {
            (*PBGERRSTAT)["OVF"] = 1;
        }
        else {
            (*PBGERRSTAT)["ERR"] = 1;
        }
    }
}//}}}

uint32_t PBG_Func::GetPBGERRSTAT()
{//{{{
    return (uint32_t)(*PBGERRSTAT);
}//}}}
// vim600: set foldmethod=marker :
