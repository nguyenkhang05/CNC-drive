// ---------------------------------------------------------------------
// $Id: HWCORE_INT_Func.cpp,v 1.3 2020/11/17 04:52:39 huyquocpham Exp $
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
// Description: Copyright of HWCORE_INT_Func.cpp
// Ref: {EMU3S_HWCORE_UD_Copyright_001}
#include "HWCORE_INT_Func.h"
#include "global.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
///*********************************
/// Function   : HWCORE_INT_Func
/// Description: Constructor HWCORE_INT_Func
/// Ref: {EMU3S_HWCORE_UD_Feature_029}
///*********************************
HWCORE_INT_Func::HWCORE_INT_Func(std::string name, HWCORE_INT_AgentController* HWCORE_INTAgentControllerPtr) :
    Chwcore_int_regif(name, 32)
{//{{{
    assert(HWCORE_INTAgentControllerPtr != NULL);
    mpHWCORE_INTAgentController = HWCORE_INTAgentControllerPtr;
    mpHWCORE_INTAgentController->CommandInit(name);
    mModuleName = name;

    Chwcore_int_regif::set_instance_name(name);

    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INT0"            ,(INT[0])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INT1"            ,(INT[1])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INT2"            ,(INT[2])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INT3"            ,(INT[3])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INT4"            ,(INT[4])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INT5"            ,(INT[5])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INT6"            ,(INT[6])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INT7"            ,(INT[7])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INTSD"           ,(INTSD)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("INTSDC"          ,(INTSDC)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERR0"            ,(ERR[0])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERR1"            ,(ERR[1])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERR2"            ,(ERR[2])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERR3"            ,(ERR[3])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERR4"            ,(ERR[4])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERR5"            ,(ERR[5])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERR6"            ,(ERR[6])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERR7"            ,(ERR[7])));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERRSD"           ,(ERRSD)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ERRSDC"          ,(ERRSDC)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("OFMON"           ,(OFMON)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ZDMON"           ,(ZDMON)));
}//}}}

///*********************************
/// Function     : ~HWCORE_INT_Func
/// Description  : Destructor of HWCORE_INT_Func
///*********************************
HWCORE_INT_Func::~HWCORE_INT_Func()
{//{{{
    mRegisterMap.clear();
}//}}}

///*********************************
/// Function   : EnableReset
/// Description: Reset HWCORE_INT_Func and hwcore_int_regif
/// Ref: {EMU3S_HWCORE_UD_Direction_059} 
///*********************************
void HWCORE_INT_Func::EnableReset(const bool is_active)
{//{{{
    Chwcore_int_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function   : RegisterHandler
/// Description: Handler for register IF
/// Ref: {EMU3S_HWCORE_UD_Direction_024}
///*********************************
void HWCORE_INT_Func::RegisterHandler(const std::vector<std::string> &args)
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
                msg_return = Chwcore_int_regif::reg_handle_command(cmd);
                if (msg_return != "") 
                {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Cspid_regif class
            msg_return = Chwcore_int_regif::reg_handle_command(args);
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
void HWCORE_INT_Func::read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
/// Ref: {EMU3S_HWCORE_UD_Registers_040, EMU3S_HWCORE_UD_Registers_041, EMU3S_HWCORE_UD_Registers_042}
/// Ref: {EMU3S_HWCORE_UD_Registers_043, EMU3S_HWCORE_UD_Registers_044, EMU3S_HWCORE_UD_Registers_045}
/// Ref: {EMU3S_HWCORE_UD_Registers_046, EMU3S_HWCORE_UD_Registers_047}
/// Ref: {EMU3S_HWCORE_UD_Flow_055}
///*********************************
void HWCORE_INT_Func::write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
void HWCORE_INT_Func::SetRegVal(std::string RegName, unsigned int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()])) = val;
}//}}}

///*********************************
/// Function     : SetRegVal
/// Description  : Write value to register bit field from core
///*********************************
void HWCORE_INT_Func::SetRegVal(std::string RegName, std::string BitName, unsigned int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()]))[BitName.c_str()] = val;
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : Read value from register from core
///*********************************
unsigned int HWCORE_INT_Func::GetRegVal(std::string RegName)
{//{{{
    return ((unsigned int)(*(mRegisterMap[RegName.c_str()])));
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : Read value from register bit field from core
///*********************************
unsigned int HWCORE_INT_Func::GetRegVal(std::string RegName, std::string BitName)
{//{{{
    return ((unsigned int)((*(mRegisterMap[RegName.c_str()]))[BitName.c_str()]));
}//}}}

//=================================================================================
//=========================== Callback function  ==================================
//=================================================================================
///******************************************************************
/// Function   : cb_INTSDC_INIFC
/// Description: Write callback function of INTSDC register
/// Ref: {EMU3S_HWCORE_UD_Flow_058}
///******************************************************************
void HWCORE_INT_Func::cb_INTSDC_INIFC(RegCBstr str)
{//{{{
    if (str.data != 0)
    {
        unsigned int mask = !(str.data);
        *INTSD = (*INTSD) & mask;
        *INTSDC = 0x00000000;
    }
}//}}}
///******************************************************************
/// Function   : cb_ERRSDC_OVFEFC
/// Description: Write callback function of ERRSDC register
/// Ref: {EMU3S_HWCORE_UD_Flow_059}
///******************************************************************
void HWCORE_INT_Func::cb_ERRSDC_OVFEFC(RegCBstr str)
{//{{{
    if (str.data != 0)
    {
        unsigned int OVFEFC = (str.data & 0x1);
        unsigned int ZDEFC = ((str.data & 0x2) >> 1);
        if (OVFEFC == 1)
        {
            *OFMON = 0x00;
        }
        if (ZDEFC == 1)
        {
            *ZDMON = 0x00;
        }
        unsigned int mask = !(str.data & 0x0000FFFF);
        *ERRSD = (*ERRSD) & mask;
        *ERRSDC = 0x0000;
    }
}//}}}
