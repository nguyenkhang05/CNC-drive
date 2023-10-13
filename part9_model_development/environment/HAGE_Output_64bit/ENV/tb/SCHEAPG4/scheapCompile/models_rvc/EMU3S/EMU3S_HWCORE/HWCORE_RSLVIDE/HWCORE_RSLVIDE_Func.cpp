// ---------------------------------------------------------------------
// $Id: HWCORE_RSLVIDE_Func.cpp,v 1.2 2020/10/22 04:06:16 ngathunguyen Exp $
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
// Description: Copyright of HWCORE_RSLVIDE_Func.cpp
// Ref: {HWCORE_RSLVIDE_UD_Copyright_001}
// Description: About the include header files of HWCORE_RSLVIDE_Func.cpp
// Ref: {HWCORE_RSLVIDE_UD_ATT1_002}
#include "HWCORE_RSLVIDE_Func.h"
#include "global.h"

#ifndef func_re_printf
#define func_re_printf get_fileline(__FILE__, __LINE__); mpHWCORE_RSLVIDEAgentController->_re_printf
#endif//func_re_printf

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
// Description: Constructor
// Ref: {HWCORE_RSLVIDE_UD_ATT1_004}
HWCORE_RSLVIDE_Func::HWCORE_RSLVIDE_Func(std::string name, HWCORE_RSLVIDE_AgentController* HWCORE_RSLVIDEAgentControllerPtr) :
    Chwcore_rslvide_regif(name, 32)
{//{{{
    assert(HWCORE_RSLVIDEAgentControllerPtr != NULL);
    mpHWCORE_RSLVIDEAgentController = HWCORE_RSLVIDEAgentControllerPtr;
    mpHWCORE_RSLVIDEAgentController->CommandInit(name);
    mModuleName = name;

    Chwcore_rslvide_regif::set_instance_name(name);
    Initialize();

    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ANGCTR"         ,(ANGCTR)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CPJUD0"         ,(CPJUD0)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("CPJUD1"         ,(CPJUD1)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("RESTHSFT"       ,(RESTHSFT)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ANGOFS"         ,(ANGOFS)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PXR"            ,(PXR)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("RESTHTORG"      ,(RESTHTORG)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTEFIX"        ,(THTEFIX)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("RESRLD"         ,(RESRLD)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("RESCNT"         ,(RESCNT)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTRESFIX"      ,(THTRESFIX)));        
}//}}}

// Description: Destructor
HWCORE_RSLVIDE_Func::~HWCORE_RSLVIDE_Func()
{//{{{
}//}}}

// Description: Reset Func and registers
//Ref: {HWCORE_RSLVIDE_UD_Flow_025}
void HWCORE_RSLVIDE_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    Chwcore_rslvide_regif::EnableReset(is_active);
}//}}}

// Description: Initialize internal variables
void HWCORE_RSLVIDE_Func::Initialize(void)
{//{{{
}//}}}

// Description: Read API (target socket)
// Ref: {HWCORE_RSLVIDE_UD_ATT1_006, HWCORE_RSLVIDE_UD_ReferenceDoc_001, HWCORE_RSLVIDE_UD_Flow_004}
void HWCORE_RSLVIDE_Func::read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if(data_ptr == NULL){
        return;
    }
    if (!debug) {
        reg_rd(offsetAddress, data_ptr, size);
    } else {
        reg_rd_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

// Description: Write API (target socket)
// Ref: {HWCORE_RSLVIDE_UD_ATT1_006, HWCORE_RSLVIDE_UD_ReferenceDoc_001, HWCORE_RSLVIDE_UD_Flow_005}
void HWCORE_RSLVIDE_Func::write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if(data_ptr == NULL){
        return;
    }
    if (!debug) {
        reg_wr(offsetAddress, data_ptr, size);
    } else {
        reg_wr_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

// Description: Handler for register IF
void HWCORE_RSLVIDE_Func::RegisterHandler(const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    }
    else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin() + 1);
                cmd.insert(cmd.begin() + 1, reg_p->name());
                msg_return = Chwcore_rslvide_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Cspid_regif class
            msg_return = Chwcore_rslvide_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Write value to register from core
void HWCORE_RSLVIDE_Func::SetRegVal(std::string RegName, unsigned int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()])) = val;
    
}//}}}

///Read value from register from core
unsigned int HWCORE_RSLVIDE_Func::GetRegVal(std::string RegName)
{//{{{
    return ((unsigned int)(*(mRegisterMap[RegName.c_str()])));
    
}//}}}

unsigned int HWCORE_RSLVIDE_Func::GetRegVal(std::string RegName, std::string BitName)
{//{{{
    return ((unsigned int)((*(mRegisterMap[RegName.c_str()]))[BitName.c_str()]));
    
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
// Description: Callback function of cb_ANGCTR_ANGSEL
// Ref: {HWCORE_RSLVIDE_UD_Registers_001, HWCORE_RSLVIDE_UD_Flow_006, HWCORE_RSLVIDE_UD_Direction_045}
void HWCORE_RSLVIDE_Func::cb_ANGCTR_ANGSEL(RegCBstr str) 
{//{{{
    //Prohibit setting
    if ((str.data & 0x3) == 0x3)
    {
        SetRegVal("ANGCTR", (str.pre_data & 0x3));
        func_re_printf("warning", "Prohibit setting at ANGCTR_ANGSEL \n"); 
    }
    else
    {
        mpHWCORE_RSLVIDEAgentController->UpdateANGCTRPort((str.data & 0x3));
    }
    
}//}}}


// Description: Callback function of cb_ANGOFS_DATA
// Ref: {HWCORE_RSLVIDE_UD_Registers_005, HWCORE_RSLVIDE_UD_Flow_007, HWCORE_RSLVIDE_UD_Direction_046}
void HWCORE_RSLVIDE_Func::cb_ANGOFS_DATA(RegCBstr str) 
{//{{{
    if ((((str.data >> 11) & 0x1F) != 0x1F) && (((str.data >> 11) & 0x1F) != 0x00))
    {
        if ((str.data & 0x800) == 0x800) {
            SetRegVal("ANGOFS", (str.data | 0xF800));
        } else {   
            SetRegVal("ANGOFS", (str.data & 0x07FF));
        }
        func_re_printf("warning", "Sign extend is needed. Model apply sign extend automatically \n"); 
    }
}//}}}

// Description: Callback function of cb_RESTHSFT_DATA
// Ref: {HWCORE_RSLVIDE_UD_Registers_004, HWCORE_RSLVIDE_UD_Flow_008}
void HWCORE_RSLVIDE_Func::cb_RESTHSFT_DATA(RegCBstr str) 
{//{{{
    mpHWCORE_RSLVIDEAgentController->UpdateRESTHSFTPort(str.data & 0xFFF);
    if (GetRegVal("ANGCTR","ANGSEL")== 0x01)
    {
        unsigned short res_value_temp = GetRegVal("RESTHSFT");
        unsigned short curr_res_value_temp = mpHWCORE_RSLVIDEAgentController->GetResolverAngleValue();

        if(curr_res_value_temp != res_value_temp)
        {
            mpHWCORE_RSLVIDEAgentController->UpdateVariableValue(emRESOLVER_CURR_VALUE, res_value_temp);
            mpHWCORE_RSLVIDEAgentController->StartAngleIP();               
        }
    }
    
}//}}}

// Description: Callback function of cb_THTEFIX_DATA
// Ref: {HWCORE_RSLVIDE_UD_Registers_008, HWCORE_RSLVIDE_UD_Flow_009}
void HWCORE_RSLVIDE_Func::cb_THTEFIX_DATA(RegCBstr str) 
{//{{{
    mpHWCORE_RSLVIDEAgentController->UpdateTHTEFIXPort(str.data & 0xFFF);
    mpHWCORE_RSLVIDEAgentController->UpdateVariableValue(emTHTEFIX_PRE_VALUE, (str.pre_data & 0xFFF));
}//}}}

// Description: Callback function of cb_THTRESFIX_DATA
// Ref: {HWCORE_RSLVIDE_UD_Registers_011, HWCORE_RSLVIDE_UD_Flow_010}
void HWCORE_RSLVIDE_Func::cb_THTRESFIX_DATA (RegCBstr str) 
{//{{{
    mpHWCORE_RSLVIDEAgentController->UpdateTHTRESFIXPort(str.data & 0xFFF);
}//}}}
