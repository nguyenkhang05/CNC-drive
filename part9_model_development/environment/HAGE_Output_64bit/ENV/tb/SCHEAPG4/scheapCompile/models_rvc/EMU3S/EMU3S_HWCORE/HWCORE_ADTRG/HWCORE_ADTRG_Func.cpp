// ---------------------------------------------------------------------
// $Id: HWCORE_ADTRG_Func.cpp,v 1.4 2020/10/16 09:52:37 huepham Exp $
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
// Description: Copyright of HWCORE_ADTRG_Func.cpp
// Ref: {HWCORE_ADTRG_UD_Copyright_001}
// Description: About the include header files of HWCORE_ADTRG_Func.cpp
// Ref: {HWCORE_ADTRG_UD_ATT1_002}
#include "HWCORE_ADTRG_Func.h"
#include "global.h"

#ifndef func_re_printf
#define func_re_printf get_fileline(__FILE__, __LINE__); mpHWCORE_ADTRGAgentController->_re_printf
#endif//func_re_printf
    
//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
// Description: Constructor
// Ref: {HWCORE_ADTRG_UD_ATT1_004}
HWCORE_ADTRG_Func::HWCORE_ADTRG_Func(std::string name, HWCORE_ADTRG_AgentController* HWCORE_ADTRGAgentControllerPtr) :
    Chwcore_adtrg_regif(name, 32)
    , BusMasterFuncIf()
{//{{{
    assert(HWCORE_ADTRGAgentControllerPtr != NULL);
    mpHWCORE_ADTRGAgentController = HWCORE_ADTRGAgentControllerPtr;
    mpHWCORE_ADTRGAgentController->CommandInit(name);
    mModuleName = name;

    Chwcore_adtrg_regif::set_instance_name(name);
    Initialize();
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADCTR"           , (ADCTR   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("SMCTR"           , (SMCTR   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADSFTTRG"        , (ADSFTTRG)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADSMTRG"         , (ADSMTRG )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADSMOFS"         , (ADSMOFS )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADTRGM"          , (ADTRGM  )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADTRGCTR"        , (ADTRGCTR)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("TRGGCM"          , (TRGGCM  )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADEDM"           , (ADEDM   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADMWCTR"         , (ADMWCTR )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADMOFS2"         , (ADMOFS2 )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADMNCNT"         , (ADMNCNT )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADMNMON"         , (ADMNMON )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADBUFSEL"        , (ADBUFSEL)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADENDCTR"        , (ADENDCTR)));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADDCNT"          , (ADDCNT  )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("RDDCNT"          , (RDDCNT  )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADTRG"           , (ADTRG   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADMON"           , (ADMON   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ADMONC"          , (ADMONC  )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTE"            , (THTE    )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("THTR"            , (THTR    )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AD0"             , (AD[0]   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AD1"             , (AD[1]   )));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("AD2"             , (AD[2]   )));
    
}//}}}

// Description: Destructor
HWCORE_ADTRG_Func::~HWCORE_ADTRG_Func()
{//{{{
}//}}}

// Description: Reset Func and registers
void HWCORE_ADTRG_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    Chwcore_adtrg_regif::EnableReset(is_active);
}//}}}

// Description: Initialize internal variables
void HWCORE_ADTRG_Func::Initialize(void)
{//{{{
}//}}}

/**
* @brief Notify AHB transaction completion
* @param trans AHB transaction
* @param t transaction time
*/
void HWCORE_ADTRG_Func::notifyTransComp(TlmTransactionType &trans, BusTime_t &t)
{
    mpHWCORE_ADTRGAgentController->notifyTransComp(trans, t);
}

// Description: Read API (target socket)
// Ref: {HWCORE_ADTRG_UD_ATT1_006, HWCORE_ADTRG_UD_ReferenceDoc_001, HWCORE_ADTRG_UD_Direction_030, HWCORE_ADTRG_UD_Flow_004}
void HWCORE_ADTRG_Func::read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) {
        return;
    }
    if (!debug) {
        reg_rd(offsetAddress, data_ptr, size);
    }
    else {
        reg_rd_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

// Description: Write API (target socket)
// Ref: {HWCORE_ADTRG_UD_ATT1_006, HWCORE_ADTRG_UD_ReferenceDoc_001, HWCORE_ADTRG_UD_Direction_029, HWCORE_ADTRG_UD_Flow_005}
void HWCORE_ADTRG_Func::write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) {
        return;
    }
    if (!debug) {
        reg_wr(offsetAddress, data_ptr, size);
    }
    else {
        reg_wr_dbg(offsetAddress, data_ptr, size);
    }
}//}}}

// Description: Handler for register IF
void HWCORE_ADTRG_Func::RegisterHandler(const std::vector<std::string> &args)
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
                msg_return = Chwcore_adtrg_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Cspid_regif class
            msg_return = Chwcore_adtrg_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

// Description: Get value of register
// Ref: {HWCORE_ADTRG_UD_ATT1_007, HWCORE_ADTRG_UD_Register_001}
unsigned int HWCORE_ADTRG_Func::GetRegisterVal(std::string regname)
{//{{{
    return ((unsigned int)(*(mRegisterMap[regname.c_str()])));
}//}}}

// Description: Set value of register
// Ref: {HWCORE_ADTRG_UD_ATT1_007, HWCORE_ADTRG_UD_Register_002}
void HWCORE_ADTRG_Func::SetRegisterVal(std::string regname, unsigned int val)
{//{{{
    (*(mRegisterMap[regname.c_str()])) = val;
}//}}}

// Description: Set value of specified bit in register
// Ref: {HWCORE_ADTRG_UD_ATT1_007, HWCORE_ADTRG_UD_Register_002}
void HWCORE_ADTRG_Func::SetRegisterVal(std::string RegName, std::string BitName, unsigned int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()]))[BitName.c_str()] = val;
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
// Description: Callback when update SFTAD bits
// Ref: {HWCORE_ADTRG_UD_Flow_007}
void HWCORE_ADTRG_Func::cb_ADSFTTRG_SFTAD(RegCBstr str)
{//{{{
    if ((str.data & 0x1) != 0x0) {
        if (mpHWCORE_ADTRGAgentController->IsDelayPeriod() != false ) {
            func_re_printf("warning", "Trigger A/D Converter startup during delay time of previous trigger is invalid"); 
            return;
        }
        // Clear delay timer in case SW reset
        mpHWCORE_ADTRGAgentController->GetDelayTimer(true);
        // Update output port
        mpHWCORE_ADTRGAgentController->HandleADStartTriggerMethod();
        mpHWCORE_ADTRGAgentController->HandleAngLatchingRegisterMethod();
        // Update trigger source
        SetRegisterVal("ADMON", (GetRegisterVal("ADMON") | (1<< 3)));
        // Clear this bit
        SetRegisterVal("ADSFTTRG", 0x0);     
    }      
}//}}}

// Description: Callback when update ADMOFS2 register
// Ref: {HWCORE_ADTRG_UD_Flow_039, HWCORE_ADTRG_UD_Direction_047}
void HWCORE_ADTRG_Func::cb_ADMOFS2_DATA(RegCBstr str)
{//{{{
    if ((str.data & 0x3) != 0) {
        func_re_printf("warning", "Set ADMOFS2 register value not multiple of 4.\n"); 
    }
}//}}}

// Description: Callback when update ADMONC register
// Ref: {HWCORE_ADTRG_UD_Flow_008}
void HWCORE_ADTRG_Func::cb_ADMONC_CAMOUADMC(RegCBstr str)
{//{{{
    unsigned int ADMON = GetRegisterVal("ADMON");
    unsigned int ADMON_Clear = ADMON & (~str.data);
    // Clear ADMON register
    SetRegisterVal("ADMON", ADMON_Clear);
    // Clear this register to 0
    SetRegisterVal("ADMONC", 0x0);      
    
}//}}}

// Description: Callback when update ADENDCTR.ADEND bit
// Ref: {HWCORE_ADTRG_UD_Flow_009}
void HWCORE_ADTRG_Func::cb_ADENDCTR_ADEND(RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        mpHWCORE_ADTRGAgentController->UpdateADCompletionTriggerControlPort(str.data & 0x3);      
    }     
}//}}}

// Description: Callback when update AD register-0
// Ref: {HWCORE_ADTRG_UD_Flow_051, HWCORE_ADTRG_UD_Direction_048, HWCORE_ADTRG_UD_Direction_049, HWCORE_ADTRG_UD_Direction_050}
void HWCORE_ADTRG_Func::cb_ADBUFSEL_AD0BUFSEL(RegCBstr str)
{//{{{
    //Prohibit setting guard
    unsigned char AD0BUFSEL = str.data & 0x3;
    unsigned char AD1BUFSEL = (str.data >> 2) & 0x3;
    unsigned char AD2BUFSEL = (str.data >> 4) & 0x3;
    if ((AD0BUFSEL == 0x2) || (AD0BUFSEL == 0x3)) {
        SetRegisterVal("ADBUFSEL", "AD0BUFSEL", str.pre_data & 0x3);
        func_re_printf("warning", "Prohibit setting at ADBUFSEL_AD0BUFSEL \n"); 
    }
    if ((AD1BUFSEL == 0x1) || (AD1BUFSEL == 0x3)) {
        SetRegisterVal("ADBUFSEL", "AD1BUFSEL", ((str.pre_data >> 2) & 0x3));
        func_re_printf("warning", "Prohibit setting at ADBUFSEL_AD1BUFSEL \n"); 
    }
    if ((AD2BUFSEL == 0x1) || (AD2BUFSEL == 0x2)) {
        SetRegisterVal("ADBUFSEL", "AD2BUFSEL", ((str.pre_data >> 4) & 0x3));
        func_re_printf("warning", "Prohibit setting at ADBUFSEL_AD2BUFSEL \n"); 
    }
}//}}}

// Description: Callback when update AD register-0
// Ref: 
void HWCORE_ADTRG_Func::cb_AD_DATA(RegCBstr str){
    mpHWCORE_ADTRGAgentController->HandleADConversionResult(*AD[0], *AD[1], *AD[2]);
}

// vim600: set foldmethod=marker :
