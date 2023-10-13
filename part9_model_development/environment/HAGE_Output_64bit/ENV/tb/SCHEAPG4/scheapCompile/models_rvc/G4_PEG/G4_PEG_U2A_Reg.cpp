// ---------------------------------------------------------------------
// $Id: G4_PEG_U2A_Reg.cpp,v 1.5 2020/01/14 02:39:30 nhutminhdang Exp $
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
#include "G4_PEG_U2A_Reg.h"
#include "G4_PEG_Func.h"

/*********************************
// Function     : G4_PEG_U2A_Reg
// Description  : Constructor of G4_PEG_U2A_Reg class
// Parameter    : None
// Return value : None
**********************************/
G4_PEG_U2A_Reg::G4_PEG_U2A_Reg (std::string name)
                                :Cg4_peg_u2a_regif(name, 32)
{//{{{
    mModuleName = name;
    Cg4_peg_u2a_regif::set_instance_name(name);
}//}}}

/*********************************
// Function     : ~G4_PEG_U2A_Reg
// Description  : Destructor of G4_PEG_U2A_Reg class
// Parameter    : None
// Return value : None
**********************************/
G4_PEG_U2A_Reg::~G4_PEG_U2A_Reg()
{//{{{
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Enable/ Disable reset
// Parameter    : 
//    is_active   true: reset, false: not reset
// Return value : None
**********************************/
void G4_PEG_U2A_Reg::EnableReset(const bool is_active)
{//{{{
    Cg4_peg_u2a_regif::EnableReset(is_active);
}//}}}

/// Read API (target socket)
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
void G4_PEG_U2A_Reg::read (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
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

/// Read API (target socket)
/*********************************
// Function     : read
// Description  : Write API (target socket)
// Parameter    : 
//    address           read address
//    p_data            transaction data pointer
//    size              transaction data length
//    debug             true: debug mode; false: normal mode   
// Return value : None
**********************************/
void G4_PEG_U2A_Reg::write (unsigned int address, unsigned char *p_data, unsigned int size, bool debug)
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
void G4_PEG_U2A_Reg::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cg4_peg_u2a_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else { // Set handle command to regif class
            msg_return = Cg4_peg_u2a_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            } else {
                /// Theere is no error occurs
            }
        }
    }
}//}}}

/*********************************
// Function     : getPEGSPID_SPID
// Description  : Get value of PEGSPID register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of PEGSPID register
**********************************/
unsigned int G4_PEG_U2A_Reg::getPEGSPID_SPID(unsigned int channel_id)
{//{{{
    return (unsigned int)(*PEGSPID[channel_id]);
}//}}}

/*********************************
// Function     : getPEGROT_GEN
// Description  : Get value of GEN bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of GEN bit from PEGPROT register
**********************************/
bool G4_PEG_U2A_Reg::getPEGROT_GEN(unsigned int channel_id)
{//{{{
    return (bool)(*PEGPROT[channel_id])["GEN"];
}//}}}

/*********************************
// Function     : getPEGROT_DBG
// Description  : Get value of DBG bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of DBG bit from PEGPROT register
**********************************/
bool G4_PEG_U2A_Reg::getPEGROT_DBG(unsigned int channel_id)
{//{{{
    return (bool)(*PEGPROT[channel_id])["DBG"];
}//}}}

/*********************************
// Function     : getPEGROT_UM
// Description  : Get value of UM bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of UM bit from PEGPROT register
**********************************/
bool G4_PEG_U2A_Reg::getPEGROT_UM(unsigned int channel_id)
{//{{{
    return (bool)(*PEGPROT[channel_id])["UM"];
}//}}}

/*********************************
// Function     : getPEGROT_WG
// Description  : Get value of WG bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of WG bit from PEGPROT register
**********************************/
bool G4_PEG_U2A_Reg::getPEGROT_WG(unsigned int channel_id)
{//{{{
    return (bool)(*PEGPROT[channel_id])["WG"];
}//}}}

/*********************************
// Function     : getPEGROT_RG
// Description  : Get value of RG bit from PEGPROT register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of RG bit from PEGPROT register
**********************************/
bool G4_PEG_U2A_Reg::getPEGROT_RG(unsigned int channel_id)
{//{{{
    return (bool)(*PEGPROT[channel_id])["RG"];
}//}}}

/*********************************
// Function     : getPEGBAD_BAD
// Description  : Get value of PEGBAD register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of PEGBAD register
**********************************/
unsigned int G4_PEG_U2A_Reg::getPEGBAD_BAD(unsigned int channel_id)
{//{{{
    return (unsigned int)(*PEGBAD[channel_id]);
}//}}}

/*********************************
// Function     : getPEGADV_ADV
// Description  : Get value of PEGADV register
// Parameter    : 
//     channel_id      id of channel
// Return value : Value of PEGADV register
**********************************/
unsigned int G4_PEG_U2A_Reg::getPEGADV_ADV(unsigned int channel_id)
{//{{{
    return (unsigned int)(*PEGADV[channel_id]);
}//}}}

/*********************************
// Function     : cb_PEGKCPROT_KCPROT
// Description  : Callback fucntion of KCPROT register
// Parameter    : 
//     str        infomation of register
// Return value : 
**********************************/
void G4_PEG_U2A_Reg::cb_PEGKCPROT_KCPROT(RegCBstr str)
{//{{{
    unsigned int kcprot_val = (unsigned int)(*PEGKCPROT)["KCPROT"];
    if ((kcprot_val << 1) != emKeyCode){
        (*PEGKCPROT)["KCE"] = str.pre_data & 0x1;
    }
    (*PEGKCPROT)["KCPROT"] = 0;
}//}}}

/*********************************
// Function     : cb_PEGPROT_GEN
// Description  : Callback fucntion of KCPROT register
// Parameter    : 
//     str        infomation of register
// Return value : 
**********************************/
void G4_PEG_U2A_Reg::cb_PEGPROT_GEN(RegCBstr str)
{//{{{
    if((unsigned int)((*PEGKCPROT)["KCE"]) != 0x1) {
        (*PEGPROT[str.channel])=str.pre_data;
    }
}//}}}

/*********************************
// Function     : cb_PEGSPID_SPID
// Description  : Callback fucntion PEGSPID register
// Parameter    : 
//     str        infomation of register
// Return value : 
**********************************/
void G4_PEG_U2A_Reg::cb_PEGSPID_SPID(RegCBstr str)
{//{{{
    if((unsigned int)((*PEGKCPROT)["KCE"]) != 0x1) {
        (*PEGSPID[str.channel])=str.pre_data;
    }
}//}}}

/*********************************
// Function     : cb_PEGBAD_BAD
// Description  : Callback fucntion PEGBAD register
// Parameter    : 
//     str        infomation of register
// Return value : 
**********************************/
void G4_PEG_U2A_Reg::cb_PEGBAD_BAD(RegCBstr str)
{//{{{
    (*PEGBAD[str.channel])["BAD"] = (*PEGBAD[str.channel])["BAD"] | 0xFC000;
    if((unsigned int)((*PEGKCPROT)["KCE"]) != 0x1) {
        (*PEGBAD[str.channel])=str.pre_data;
    }
}//}}}

/*********************************
// Function     : cb_PEGADV_ADV
// Description  : Callback fucntion PEGADV register
// Parameter    : 
//     str        infomation of register
// Return value : 
**********************************/
void G4_PEG_U2A_Reg::cb_PEGADV_ADV(RegCBstr str)
{//{{{
    if((unsigned int)((*PEGKCPROT)["KCE"]) != 0x1) {
        (*PEGADV[str.channel])=str.pre_data;
    }
}//}}}
// vim600: set foldmethod=marker :
