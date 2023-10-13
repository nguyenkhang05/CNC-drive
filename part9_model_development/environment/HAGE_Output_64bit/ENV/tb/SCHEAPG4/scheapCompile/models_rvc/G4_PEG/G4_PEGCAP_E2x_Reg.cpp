// ---------------------------------------------------------------------
// $Id: G4_PEGCAP_E2x_Reg.cpp,v 1.5 2020/01/14 02:39:30 nhutminhdang Exp $
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
#include "G4_PEGCAP_E2x_Reg.h"
#include "G4_PEGCAP_Func.h"

/*********************************
// Function     : G4_PEGCAP_E2x_Reg
// Description  : Constructor of G4_PEGCAP_E2x_Reg class
// Parameter    : None
// Return value : None
**********************************/
G4_PEGCAP_E2x_Reg::G4_PEGCAP_E2x_Reg (std::string name)
                                     :Cg4_pegcap_e2x_regif(name, 32)
{//{{{
    Cg4_pegcap_e2x_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

/*********************************
// Function     : ~G4_PEGCAP_E2x_Reg
// Description  : Destructor of G4_PEGCAP_E2x_Reg class
// Parameter    : None
// Return value : None
**********************************/
G4_PEGCAP_E2x_Reg::~G4_PEGCAP_E2x_Reg()
{//{{{
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Enable/ Disable reset
// Parameter    : 
//    is_active   true: reset, false: not reset
// Return value : None
**********************************/
void G4_PEGCAP_E2x_Reg::EnableReset(const bool is_active)
{//{{{
    Cg4_pegcap_e2x_regif::EnableReset(is_active);
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
void G4_PEGCAP_E2x_Reg::read (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
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
void G4_PEGCAP_E2x_Reg::write (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
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
void G4_PEGCAP_E2x_Reg::RegisterHandler (const std::vector<std::string> &args)
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
                cmd.erase(cmd.begin() + 1);
                cmd.insert(cmd.begin() + 1, reg_p->name());
                msg_return = Cg4_pegcap_e2x_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else { // Set handle command to regif class
            msg_return = Cg4_pegcap_e2x_regif::reg_handle_command(args);
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
void G4_PEGCAP_E2x_Reg::SetERR_INFO(G4_PEG_Cerrinfo err_info)
{//{{{
    if((*PEGERRSTAT)["ERR"] == 1) {
        (*PEGERRSTAT)["OVF"] = 1;
    }
    else {
        (*PEGERRSTAT)["ERR"] = 1;
        (*PEGERRTYPE)["SEC"] = err_info.SEC;
        (*PEGERRTYPE)["DBG"] = err_info.DBG;
        (*PEGERRTYPE)["UM"] = err_info.UM;
        (*PEGERRTYPE)["SPID"] = err_info.SPID;
        (*PEGERRTYPE)["WRITE"] = err_info.PEGWrERR;
        (*PEGERRADDR) = err_info.ErrAddr;
    }
}//}}}

/*********************************
// Function     : cb_PEGERRCLR_CLRO
// Description  : Callback function of PEGERRCLR register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void G4_PEGCAP_E2x_Reg::cb_PEGERRCLR_CLRO(RegCBstr str)
{//{{{
    if((unsigned int)(*PEGERRCLR)["CLRE"] == 0x1) {
        (*PEGERRSTAT)["ERR"] = 0;
    } 
    if((unsigned int)(*PEGERRCLR)["CLRO"] == 0x1){
        (*PEGERRSTAT)["OVF"] = 0;
    }
    (*PEGERRCLR) = 0;
}//}}}
// vim600: set foldmethod=marker :
