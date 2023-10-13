// ---------------------------------------------------------------------
// $Id: HWCORE_HANDSHAKE_Func.cpp,v 1.6 2020/11/17 04:55:45 huyquocpham Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of .HWCORE_HANDSHAKE_Func.cpp
// Ref: {EMU3S_HWCORE_UD_Copyright_001}

#include "HWCORE_HANDSHAKE_Func.h"
#include "EMU3S_HWCORE_AgentController.h"
#ifndef func_re_printf
#define func_re_printf mpEMU3S_HWCORE_AgentController->get_fileline(__FILE__, __LINE__); mpEMU3S_HWCORE_AgentController->_re_printf
#endif//func_re_printff
//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
// Description: Constructor of HWCORE_HANDSHAKE_Func class
// Ref: {}
HWCORE_HANDSHAKE_Func::HWCORE_HANDSHAKE_Func(std::string name, EMU3S_HWCORE_AgentController* pEMU3S_HWCORE_AgentController) :
            Chwcore_handshake_regif(name, 32)
            ,CRWInterface()
{
    if (pEMU3S_HWCORE_AgentController == NULL)
    {
        return;
    }
    mpEMU3S_HWCORE_AgentController = pEMU3S_HWCORE_AgentController;
    mModuleName = name;
    Chwcore_handshake_regif::set_instance_name(name);
    // Initialize
    DATSETBUF = new unsigned int [24];
    Initialize();
}

// Description: Destructor of HWCORE_HANDSHAKE_Func class
// Ref: {}
HWCORE_HANDSHAKE_Func::~HWCORE_HANDSHAKE_Func()
{
    delete [] DATSETBUF;
}

// Description: Initialize variables
// Ref: {}
void HWCORE_HANDSHAKE_Func::Initialize(void)
{
    unsigned int i = 0;
    for (i = 0; i < 24; i++)
    {
        DATSETBUF[i] = 0;
    }
}

// Description: Reset Func and registers
// Ref: {EMU3S_HWCORE_UD_Direction_059}
void HWCORE_HANDSHAKE_Func::EnableReset(const bool is_active)
{
    if (is_active) 
    {
        Initialize();
    }
    Chwcore_handshake_regif::EnableReset(is_active);
}

// Description: Read API
// Ref: {}
void HWCORE_HANDSHAKE_Func::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{
    assert(data_ptr != NULL);
    if (!debug) 
    {
        reg_rd(offsetAddress, data_ptr, size);
    } 
    else 
    {
        reg_rd_dbg(offsetAddress, data_ptr, size);
    }
}

// Description: Write API
// Ref: {EMU3S_HWCORE_UD_Registers_059, EMU3S_HWCORE_UD_Registers_060, EMU3S_HWCORE_UD_Registers_061}
// Ref: {EMU3S_HWCORE_UD_Registers_062, EMU3S_HWCORE_UD_Registers_063, EMU3S_HWCORE_UD_Registers_064}
// Ref: {EMU3S_HWCORE_UD_Registers_065, EMU3S_HWCORE_UD_Registers_066, EMU3S_HWCORE_UD_Registers_067}
// Ref: {EMU3S_HWCORE_UD_Registers_068, EMU3S_HWCORE_UD_Registers_069, EMU3S_HWCORE_UD_Registers_070}
// Ref: {EMU3S_HWCORE_UD_Registers_071, EMU3S_HWCORE_UD_Registers_072}
void HWCORE_HANDSHAKE_Func::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{
    assert(data_ptr != NULL);
    if (!debug) 
    {
        reg_wr(offsetAddress, data_ptr, size);
    } 
    else 
    {
        reg_wr_dbg(offsetAddress, data_ptr, size);
    }
}

// Description: Handler for register IF
// Ref: {}
void HWCORE_HANDSHAKE_Func::RegisterHandler(const std::vector<std::string> &args)
{
    std::string msg_return = "";
    if (args.size() == 1) { // Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    }
    else {
        if (args[1] == "*") { // Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin() + 1);
                cmd.insert(cmd.begin() + 1, reg_p->name());
                msg_return = Chwcore_handshake_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else { // Set handle command to regif class
            msg_return = Chwcore_handshake_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
            else {
                /// There is no error occurs
            }
        }
    }
}

//========================================================
//============ Handshake function ========================
//======================================================== 
// Description: Transfer To Buffer
// Ref: {EMU3S_HWCORE_UD_ATT1_007}
void HWCORE_HANDSHAKE_Func::TransferToBuffer(unsigned char set_index)
{
    unsigned int i = 0;
    switch (set_index)
    {
        case 0:
        {
            for (i = 0; i < 4; i++)
            {
                DATSETBUF[set_index*4 + i] = (*(DATSETW0[i]))["DATA"];
            }
            break;
        }
        case 1:
        {
            for (i = 0; i < 4; i++)
            {
                DATSETBUF[set_index*4 + i] = (*(DATSETW1[i]))["DATA"];
            }
            break;
        }
        case 2:
        {
            for (i = 0; i < 4; i++)
            {
                DATSETBUF[set_index*4 + i] = (*(DATSETW2[i]))["DATA"];
            }
            break;
        }
        case 3:
        {
            for (i = 0; i < 4; i++)
            {
                DATSETBUF[set_index*4 + i] = (*(DATSETW3[i]))["DATA"];
            }
            break;
        }
        case 4:
        {
            for (i = 0; i < 4; i++)
            {
                DATSETBUF[set_index*4 + i] = (*(DATSETW4[i]))["DATA"];
            }
            break;
        }
        default:
        {
            for (i = 0; i < 4; i++)
            {
                DATSETBUF[set_index*4 + i] = (*(DATSETW5[i]))["DATA"];
            }
        }
    }
}
// Description: Transfer From Buffer
// Ref: {}
void HWCORE_HANDSHAKE_Func::TransferFromBuffer(unsigned char set_index)
{
    unsigned int i = 0;
    switch (set_index)
    {
        case 0:
        {
            for (i = 0; i < 4; i++)
            {
                (*(DATSETR0[i]))["DATA"] = DATSETBUF[set_index*4 + i];
            }
            break;
        }
        case 1:
        {
            for (i = 0; i < 4; i++)
            {
                (*(DATSETR1[i]))["DATA"] = DATSETBUF[set_index*4 + i];
            }
            break;
        }
        case 2:
        {
            for (i = 0; i < 4; i++)
            {
                (*(DATSETR2[i]))["DATA"] = DATSETBUF[set_index*4 + i];
            }
            break;
        }
        case 3:
        {
            for (i = 0; i < 4; i++)
            {
                (*(DATSETR3[i]))["DATA"] = DATSETBUF[set_index*4 + i];
            }
            break;
        }
        case 4:
        {
            for (i = 0; i < 4; i++)
            {
                (*(DATSETR4[i]))["DATA"] = DATSETBUF[set_index*4 + i];
            }
            break;
        }
        default:
        {
            for (i = 0; i < 4; i++)
            {
                (*(DATSETR5[i]))["DATA"] = DATSETBUF[set_index*4 + i];
            }
        }
    }
}

//========================================================
//============ Callback function  ========================
//========================================================
// Description: Callback function of cb_DATSETWB
// Ref: {EMU3S_HWCORE_UD_Flow_037}
void HWCORE_HANDSHAKE_Func::cb_DATSETWB_TR0(RegCBstr str)
{
    unsigned char reg_val = (unsigned char) (str.data & 0x3F);
    unsigned int i = 0;
    for (i = 0; i < 6; i++)
    {
        unsigned char TRG_val = reg_val & 0x01;
        if (TRG_val == 0x01)
        {
             TransferToBuffer(i);
        }
        reg_val = (reg_val >> 1);
    }
    *(DATSETWB) =  0x00000000;
}
// Description: Callback function of cb_DATSETBR
// Ref: {EMU3S_HWCORE_UD_Flow_038}
void HWCORE_HANDSHAKE_Func::cb_DATSETBR_TR0(RegCBstr str)
{
    unsigned char reg_val = (unsigned char) (str.data & 0x3F);
    unsigned int i = 0;
    for (i = 0; i < 6; i++)
    {
        unsigned char TRG_val = reg_val & 0x01;
        if (TRG_val == 0x01)
        {
             TransferFromBuffer(i);
        }
        reg_val = (reg_val >> 1);
    }
    *(DATSETBR) =  0x00000000;
}
// vim600: set foldmethod=marker :
