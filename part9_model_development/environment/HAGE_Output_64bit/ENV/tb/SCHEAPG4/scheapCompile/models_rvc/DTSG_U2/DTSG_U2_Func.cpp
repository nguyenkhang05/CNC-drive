// ---------------------------------------------------------------------
// $Id: DTSG_U2_Func.cpp,v 1.12 2020/01/07 04:00:23 synguyen Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "DTSG_U2_Func.h"
#include "DTSG_U2_AgentController.h"

///Constructor of DTSG_U2_Func class
DTSG_U2_Func::DTSG_U2_Func (std::string name, DTSG_U2_AgentController *DTSG_U2_AgentControllerPtr, unsigned int numOfCh):
    Cdtsg_u2_regif(name, 32, numOfCh - 1)

{//{{{
    assert(DTSG_U2_AgentControllerPtr != NULL);
    mDTSG_U2_AgentController = DTSG_U2_AgentControllerPtr;
    Cdtsg_u2_regif::set_instance_name(name);
    mModuleName = name;
    mNumOfChannel = numOfCh;
    mIsReset = false;
}//}}}

///Destructor of DTSG_U2_Func class
DTSG_U2_Func::~DTSG_U2_Func (void)
{//{{{
}//}}}

///Reset operation of DTSG_U2_Func class
void DTSG_U2_Func::Reset (const bool is_active)
{//{{{
    mIsReset = is_active;
    Cdtsg_u2_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void DTSG_U2_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Cdtsg_u2_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else if (args[1] == "DumpRegisterRW") {
            msg_return = Cdtsg_u2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        } else {
            msg_return = Cdtsg_u2_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void DTSG_U2_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // When clock = 0, or reset state, only Software debbugger can read register 
    if ( (!mDTSG_U2_AgentController->CheckClockPeriod("clk")) || mIsReset ){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }
    
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void DTSG_U2_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    
    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only Software debbugger write
    if (!mDTSG_U2_AgentController->CheckClockPeriod("clk")){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }
    
    if (!debug) {
         reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
         reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Get value of DTSGPROT_[channel_index] or DTSGPROT_GR
void DTSG_U2_Func::GetDTSGPROT(const unsigned int channel_index, unsigned int& mpid, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg)
{
    //Indicating DTSGPROT_GR or DTSGPROT_m
    if (channel_index == mNumOfChannel - 1) {
        mpid = (*DTSGPROT_GR)["MPID"];
        gen  = (*DTSGPROT_GR)["GEN"];
        dbg  = (*DTSGPROT_GR)["DBG"];
        um   = (*DTSGPROT_GR)["UM" ];
        wg   = (*DTSGPROT_GR)["WG" ];
        rg   = (*DTSGPROT_GR)["RG" ];
    } else {
        mpid = (*DTSGPROT_[channel_index])["MPID"];
        gen  = (*DTSGPROT_[channel_index])["GEN"];
        dbg  = (*DTSGPROT_[channel_index])["DBG"];
        um   = (*DTSGPROT_[channel_index])["UM" ];
        wg   = (*DTSGPROT_[channel_index])["WG" ];
        rg   = (*DTSGPROT_[channel_index])["RG" ];
    }                                   
}                                       
                                        
///Get DTSGSPIDERRSTAT [SPIDERR]        
unsigned int DTSG_U2_Func::GetSPIDERR()
{
    return (unsigned int)(*DTSGSPIDERRSTAT)["SPIDERR"];
}

///Set value to specify bit
void DTSG_U2_Func::SetVar (std::string bit_name, unsigned int val)
{
    uint32_t SPIDERR_cur = (uint32_t)(*DTSGSPIDERRSTAT)["SPIDERR"]; 
    if((unsigned int) ((*DTSGSPIDERRSTAT)["SPIDERR"]) == 0) {
        if(bit_name == "DBG") {
            (*DTSGERRTYPE)["DBG"] = val;
        }
        else if(bit_name == "UM") {
            (*DTSGERRTYPE)["UM"] = val;
        }
        else if(bit_name == "SPID") {
            (*DTSGERRTYPE)["SPID"] = val;
        }
        else if(bit_name == "WRITE") {
            (*DTSGERRTYPE)["WRITE"] = val;
        }
        else if(bit_name == "ADDR") {
            (*DTSGERRADDR) = val;
        } 
    } else if(bit_name == "SPIDERR") {
        (*DTSGOVFSTAT)["OVF"] = 0x1;
    }
    if(bit_name == "SPIDERR") {
        (*DTSGSPIDERRSTAT)["SPIDERR"] = SPIDERR_cur | val;
    }
}

///Get DTSGMPID[channel_index].[SPID]
unsigned int DTSG_U2_Func::GetDTSGMPID_SPID(const unsigned int channel_index)
{
    return (unsigned int)(*DTSGMPID[channel_index]);
}

///Call back write of DTSGOVFCLR_CLRO
///The DTSGOVFSTAT.OVF bit is cleared immediately after writing 1 to this register 
void DTSG_U2_Func::cb_DTSGOVFCLR_CLRO(RegCBstr str)
{//{{{   
    unsigned int CLRO_cur = bit_select(str.data
            ,(*DTSGOVFCLR)["CLRO"].mStartAddr
            ,(*DTSGOVFCLR)["CLRO"].mEndAddr);
    if (CLRO_cur != 0){
        (*DTSGOVFSTAT)["OVF"] = 0;
        // Response data is always 0 when reading this register
        (*DTSGOVFCLR)["CLRO"] = 0;
    }   
}//}}}

///Call back write of DTSGSPIDERRCLR_SPIDCLRE
///The DTSGSPIDERRSTAT.SPIDERR bits are cleared immediately after writing 1 to this register
void DTSG_U2_Func::cb_DTSGSPIDERRCLR_SPIDCLRE(RegCBstr str)
{//{{{ 
    if(str.data != 0){
        unsigned int SPIDCLRE_cur = (unsigned int) str.data;
       (*DTSGSPIDERRSTAT)["SPIDERR"] =  (unsigned int)(*DTSGSPIDERRSTAT)["SPIDERR"] & (~SPIDCLRE_cur);
       
       // Response data is always 0 when reading this register
       (*DTSGSPIDERRCLR)["SPIDCLRE"] = 0;
    }
}//}}}

///Call back write of DTSGSPIDERRCLR_KCPROT
void DTSG_U2_Func::cb_DTSGKCPROT_KCPROT(RegCBstr str)
{//{{{  
    if((*DTSGKCPROT)["KCPROT"] != (0xA5A5A500 >> 1)) {
        (*DTSGKCPROT) = str.pre_data;
    }
    // Response data is always 0 when reading KCPROT
    (*DTSGKCPROT)["KCPROT"] = 0;
}//}}}

///Call back write of DTSGMPID_SPID
void DTSG_U2_Func::cb_DTSGMPID_SPID(RegCBstr str)
{//{{{
    if((*DTSGKCPROT)["KCE"] == 0) {
        (*DTSGMPID[str.channel]) = str.pre_data;
    }
}//}}}

///Call back write of DTSGPROT_GR_MPID
void DTSG_U2_Func::cb_DTSGPROT_GR_MPID(RegCBstr str)
{//{{{
    if((*DTSGKCPROT)["KCE"] == 0) {
        (*DTSGPROT_GR) = str.pre_data;
    }
}//}}}

///Call back write of DTSGPROT_MPID
void DTSG_U2_Func::cb_DTSGPROT__MPID(RegCBstr str)
{//{{{
    if((*DTSGKCPROT)["KCE"] == 0) {
        (*DTSGPROT_[str.channel]) = str.pre_data;
    }
}//}}}

// vim600: set foldmethod=marker :
