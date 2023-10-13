// ---------------------------------------------------------------------
// $Id: G4_PEGCAP_U2A_Reg.cpp,v 1.6 2020/01/14 02:39:30 nhutminhdang Exp $
//
// Copyright(c) 2016-2020 Renesas Electronics Corporation
// Copyright(c) 2016-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "G4_PEGCAP_U2A_Reg.h"

/*********************************
// Function     : G4_PEGCAP_U2A_Reg
// Description  : Constructor of G4_PEGCAP_U2A_Reg class
// Parameter    : None
// Return value : None
**********************************/
G4_PEGCAP_U2A_Reg::G4_PEGCAP_U2A_Reg (std::string name, G4_PEGCAP_Func* parent_func)
                                     :Cg4_pegcap_u2a_regif(name, 32)
{//{{{
    assert(parent_func != NULL);
    mpParentFunc = parent_func;
    
    Cg4_pegcap_u2a_regif::set_instance_name(name);
    mModuleName = name;
     
    Initialize();
}//}}}

/*********************************
// Function     : ~G4_PEGCAP_U2A_Reg
// Description  : Destructor of G4_PEGCAP_U2A_Reg class
// Parameter    : None
// Return value : None
**********************************/
G4_PEGCAP_U2A_Reg::~G4_PEGCAP_U2A_Reg()
{//{{{
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initialize when reset activated
// Parameter    : None
// Return value : None
**********************************/
void G4_PEGCAP_U2A_Reg::Initialize() 
{//{{{
    /// Initialize variables
    mIsClearFlag          = false;
    mIsDetectSameSPIDFlag = false;
    mSpidclr              = 0;
    mErrorSPID            = 0;
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Enable/ Disable reset
// Parameter    : 
//    is_active   true: reset, false: not reset
// Return value : None
**********************************/
void G4_PEGCAP_U2A_Reg::EnableReset(const bool is_active)
{//{{{
    if(is_active) {
        Initialize();
    }
    Cg4_pegcap_u2a_regif::EnableReset(is_active);
}//}}}

/*********************************
// Function     : read
// Description  : Read API (target socket)
// Parameter    : 
//    address     read address
//    p_data      transaction data pointer
//    size        transaction data length
//    debug       true: debug mode; false: normal mode   
// Return value : None
**********************************/
void G4_PEGCAP_U2A_Reg::read (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
{//{{{
    if (p_data == NULL) {
        return;
    }
    
    if (!debug) {
        reg_rd(address, p_data, size);
    } else {
        reg_rd_dbg(address, p_data, size);
    }
}//}}}

/*********************************
// Function     : write
// Description  : Write API (target socket)
// Parameter    : 
//    address           read address
//    p_data            transaction data pointer
//    size              transaction data length
//    debug             true: debug mode; false: normal mode   
// Return value : None
**********************************/
void G4_PEGCAP_U2A_Reg::write (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
{//{{{
    if (p_data == NULL) {
        return;
    }
    
    if (!debug) {
        reg_wr(address, p_data, size);
    } else {
        reg_wr_dbg(address, p_data, size);
    }
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Handler for register IF
// Parameter    : 
//    args        input arguments 
// Return value : None
**********************************/
void G4_PEGCAP_U2A_Reg::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) { // Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") { // Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Cg4_pegcap_u2a_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else { // Set handle command to regif class
            msg_return = Cg4_pegcap_u2a_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            } else {
                /// There is no error occurs
            }
        }
    }
}//}}}

/*********************************
// Function     : SetERR_INFO
// Description  : Record error info
// Parameter    : 
//    err_info    error info
// Return value : None
**********************************/
void G4_PEGCAP_U2A_Reg::SetERR_INFO(G4_PEG_Cerrinfo err_info, bool same_spid)
{//{{{
    unsigned int spiderr_val = (*PEGSPIDERRSTAT);
    if(spiderr_val != 0) {
        (*PEGOVFSTAT)["OVF"] = 1;
        if(!same_spid) {
            (*PEGSPIDERRSTAT) = spiderr_val | (1 << err_info.SPID);
        } else {
               /// Don't update value for PEGSPIDERRSTAT register
            mErrorSPID = err_info.SPID;
            mIsDetectSameSPIDFlag = true;
            mpParentFunc->NotifyClearPEGSPIDERRCLREvent();
        }
    } else {
        (*PEGERRTYPE)["SEC"] = err_info.SEC;
        (*PEGERRTYPE)["DBG"] = err_info.DBG;
        (*PEGERRTYPE)["UM"] = err_info.UM;
        (*PEGERRTYPE)["SPID"] = err_info.SPID;
        (*PEGERRTYPE)["WRITE"] = err_info.PEGWrERR;
        (*PEGERRADDR) = err_info.ErrAddr;
        (*PEGSPIDERRSTAT) = spiderr_val | (1 << err_info.SPID);
    }
}//}}}

/*********************************
// Function     : IsSameSPID
// Description  : Judge current error SPID is recored or not.
// Parameter    : 
//    cur_SPID    current error SPID
// Return value : true: recorded; false: not recorded yet.
**********************************/
bool G4_PEGCAP_U2A_Reg::IsSameSPID(unsigned int cur_SPID)
{//{{{
    unsigned int spiderr_val = (*PEGSPIDERRSTAT);
    return (bool)((spiderr_val >> cur_SPID) & 1);
}//}}}

/*********************************
// Function     : cb_PEGOVFCLR_CLRO
// Description  : Callback function of PEGOVFCLR register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void G4_PEGCAP_U2A_Reg::cb_PEGOVFCLR_CLRO(RegCBstr str) 
{//{{{
    if((unsigned int)(*PEGOVFCLR)["CLRO"] == 0x1){
        (*PEGOVFSTAT)["OVF"] = 0;
    }
    (*PEGOVFCLR) = 0;
}//}}}

/*********************************
// Function     : cb_PEGSPIDERRCLR_SPIDCLR
// Description  : Callback function of PEGSPIDERRCLR register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void G4_PEGCAP_U2A_Reg::cb_PEGSPIDERRCLR_SPIDCLR(RegCBstr str)
{//{{{
    mSpidclr = str.data;
    mIsClearFlag = true;
    mpParentFunc->NotifyClearPEGSPIDERRCLREvent();
}//}}}

/*********************************
// Function     : ClearPEGSPIDERRCLR
// Description  : Clear PEGSPIDERRCLR register
// Parameter    : None
// Return value : Value of mSpidclr variable
**********************************/
void G4_PEGCAP_U2A_Reg::ClearPEGSPIDERRCLR()
{//{{{
    /// Detect SPIDERR and clear SPIDCLR not in the same time.
    /// Clear flag is true
    if(!mIsDetectSameSPIDFlag && mIsClearFlag) {
        unsigned int pre_SPIDERR = (*PEGSPIDERRSTAT);
        (*PEGSPIDERRSTAT) = pre_SPIDERR & (~mSpidclr);
        (*PEGSPIDERRCLR) = 0;
    } 
    
    /// Detect SPIDERR and clear SPIDCLR in the same time.
    if(mIsDetectSameSPIDFlag && mIsClearFlag) {
        unsigned int pre_SPIDERR = (*PEGSPIDERRSTAT);
        
        /// Detect same SPIDERR and clear the same SPIDCLR in the same time.
        /// Do not clear that SPID
        if(((mSpidclr >> mErrorSPID) & 1) != 0) {    
            mSpidclr = mSpidclr & (~(1 << mErrorSPID));
        }
        (*PEGSPIDERRSTAT) = pre_SPIDERR & (~mSpidclr);
        (*PEGSPIDERRCLR) = 0;
    }
    
    mIsClearFlag = false;
    mIsDetectSameSPIDFlag = false;
}//}}}

// vim600: set foldmethod=marker :
