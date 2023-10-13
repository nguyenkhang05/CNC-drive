// ---------------------------------------------------------------------
// $Id: HWCORE_VRFBUF_Func.cpp,v 1.3 2020/11/17 04:52:39 huyquocpham Exp $
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
// Description: Copyright of HWCORE_VRFBUF_Func.cpp
// Ref: {EMU3S_HWCORE_UD_Copyright_001}
#include "HWCORE_VRFBUF_Func.h"
#include "global.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
///*********************************
/// Function   : HWCORE_VRFBUF_Func
/// Description: Constructor HWCORE_VRFBUF_Func
/// Ref: {EMU3S_HWCORE_UD_Feature_028}
///*********************************
HWCORE_VRFBUF_Func::HWCORE_VRFBUF_Func(std::string name, HWCORE_INT_AgentController* HWCORE_INTAgentControllerPtr) :
    Chwcore_vrfbuf_regif(name, 32)
{//{{{
    assert(HWCORE_INTAgentControllerPtr != NULL);
    mpHWCORE_INTAgentController = HWCORE_INTAgentControllerPtr;
    mpHWCORE_INTAgentController->CommandInit(name);
    mModuleName = name;

    Chwcore_vrfbuf_regif::set_instance_name(name);

    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBCTR0"            ,(CBCTR0)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBTIM"             ,(CBTIM)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBAD0"             ,(CBAD[0])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBAD1"             ,(CBAD[1])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBAD2"             ,(CBAD[2])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBTHTRESFIXIN"     ,(CBTHTRESFIXIN)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBIDFIX"           ,(CBIDFIX)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBIQFIX"           ,(CBIQFIX)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBPWMUIP"          ,(CBPWMUIP)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBPWMVIP"          ,(CBPWMVIP)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBPWMWIP"          ,(CBPWMWIP)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBBREC"            ,(CBBREC)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CBIREC3"           ,(CBIREC3)));
}//}}}

///*********************************
/// Function     : ~HWCORE_VRFBUF_Func
/// Description  : Destructor of HWCORE_VRFBUF_Func
///*********************************
HWCORE_VRFBUF_Func::~HWCORE_VRFBUF_Func()
{//{{{
    mRegisterMap.clear();
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Reset HWCORE_VRFBUF_Func and hwcore_vrfbuf_regif
///*********************************
void HWCORE_VRFBUF_Func::EnableReset(const bool is_active)
{//{{{
    Chwcore_vrfbuf_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function   : RegisterHandler
/// Description: Handler for register IF
/// Ref: {EMU3S_HWCORE_UD_Direction_024}
///*********************************
void HWCORE_VRFBUF_Func::RegisterHandler(const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) 
        {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    }
    else 
    {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) 
            {
                cmd = args;
                cmd.erase(cmd.begin() + 1);
                cmd.insert(cmd.begin() + 1, reg_p->name());
                msg_return = Chwcore_vrfbuf_regif::reg_handle_command(cmd);
                if (msg_return != "") 
                {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Cspid_regif class
            msg_return = Chwcore_vrfbuf_regif::reg_handle_command(args);
            if (msg_return != "") 
            {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///*********************************
/// Function   : read 
/// Description: read register
/// Ref: {EMU3S_HWCORE_UD_Direction_047, EMU3S_HWCORE_UD_Direction_048, EMU3S_HWCORE_UD_Direction_049}
/// Ref: {EMU3S_HWCORE_UD_Direction_051, EMU3S_HWCORE_UD_Direction_052, EMU3S_HWCORE_UD_Direction_075}
/// Ref: {EMU3S_HWCORE_UD_Flow_054}
///*********************************
void HWCORE_VRFBUF_Func::read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    assert(data_ptr != NULL);
    if (!debug) 
    {
        reg_rd(offsetAddress, data_ptr, size);
    } 
    else 
    {
        reg_rd_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

///*********************************
/// Function     : write 
/// Description: write register
/// Ref: {EMU3S_HWCORE_UD_Direction_047, EMU3S_HWCORE_UD_Direction_048, EMU3S_HWCORE_UD_Direction_050}
/// Ref: {EMU3S_HWCORE_UD_Direction_053, EMU3S_HWCORE_UD_Direction_056, EMU3S_HWCORE_UD_Direction_075}
/// Ref: {EMU3S_HWCORE_UD_Registers_048, EMU3S_HWCORE_UD_Registers_049}
/// Ref: {EMU3S_HWCORE_UD_Registers_050, EMU3S_HWCORE_UD_Registers_051, EMU3S_HWCORE_UD_Registers_052}
/// Ref: {EMU3S_HWCORE_UD_Registers_053, EMU3S_HWCORE_UD_Registers_054, EMU3S_HWCORE_UD_Registers_055}
/// Ref: {EMU3S_HWCORE_UD_Registers_056, EMU3S_HWCORE_UD_Registers_057, EMU3S_HWCORE_UD_Registers_058}
/// Ref: {EMU3S_HWCORE_UD_Flow_055}
///*********************************
void HWCORE_VRFBUF_Func::write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    assert(data_ptr != NULL);
    if (!debug) 
    {
        reg_wr(offsetAddress, data_ptr, size);
    } 
    else 
    {
        reg_wr_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

///*********************************
/// Function     : SetRegVal
/// Description  : Write value to register from core
///*********************************
void HWCORE_VRFBUF_Func::SetRegVal(std::string RegName, unsigned int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()])) = val;
}//}}}

///*********************************
/// Function     : SetRegVal
/// Description  : Write value to register bit field from core
///*********************************
void HWCORE_VRFBUF_Func::SetRegVal(std::string RegName, std::string BitName, unsigned int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()]))[BitName.c_str()] = val;
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : Read value from register from core
///*********************************
unsigned int HWCORE_VRFBUF_Func::GetRegVal(std::string RegName)
{//{{{
    return ((unsigned int)(*(mRegisterMap[RegName.c_str()])));
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : Read value from register bit field from core
///*********************************
unsigned int HWCORE_VRFBUF_Func::GetRegVal(std::string RegName, std::string BitName)
{//{{{
    return ((unsigned int)((*(mRegisterMap[RegName.c_str()]))[BitName.c_str()]));
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
///*********************************
/// Function   : cb_CBCTR0_CBEN0
/// Description: Write callback function of CBCTR0 register
/// Ref: {EMU3S_HWCORE_UD_Flow_056}
///*********************************
void HWCORE_VRFBUF_Func::cb_CBCTR0_CBEN0(RegCBstr str)
{//{{{
    unsigned int reg_val = 0;
    unsigned int CBEN0 = (str.data & 0x1);
    unsigned int CBEN1 = ((str.data >> 1) & 0x1);
    unsigned int CBMON = ((str.pre_data >> 2) & 0x1);
    reg_val = (CBMON << 2) | (CBEN1 << 1); // Clear CBEN0 and Update CBEN1
    if ((CBEN0 == 1 || CBEN1 == 1) && (CBMON == 0))
    {
        reg_val |= 0x04; // Set CBMON
    }
    else if ((CBEN1 == 0) && (CBMON == 1))
    {
        unsigned int CBEN1_pre = ((str.pre_data >> 1) & 0x1);
        if ((CBEN1_pre == 1) && (CBEN0 == 0))
        {
            reg_val &= 0x03; // Clear CBMON
        }
    }
    *CBCTR0 = reg_val;
}//}}}
