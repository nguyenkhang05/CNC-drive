// ---------------------------------------------------------------------
// $Id: HWCORE_MEASURE_Func.cpp,v 1.3 2020/10/29 03:24:12 phuongtran Exp $
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
// Description: Copyright of HWCORE_MEASURE_Func.cpp
// Ref: {HWCORE_MEASURE_UD_Copyright_001}
// Description: About the include header files of HWCORE_MEASURE_Func.cpp
// Ref: {HWCORE_MEASURE_UD_ATT1_002}
#include "HWCORE_MEASURE_Func.h"
#include "global.h"

#ifndef func_re_printf
#define func_re_printf get_fileline(__FILE__, __LINE__); mpHWCORE_MEASUREAgentController->_re_printf
#endif//func_re_printf

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
// Description: Constructor
// Ref: {HWCORE_MEASURE_UD_ATT1_004}
HWCORE_MEASURE_Func::HWCORE_MEASURE_Func(std::string name, HWCORE_MEASURE_AgentController* HWCORE_MEASUREAgentControllerPtr) :
    Chwcore_measure_regif(name, 32)
{//{{{
    assert(HWCORE_MEASUREAgentControllerPtr != NULL);
    mpHWCORE_MEASUREAgentController = HWCORE_MEASUREAgentControllerPtr;
    mpHWCORE_MEASUREAgentController->CommandInit(name);
    mModuleName = name;

    Chwcore_measure_regif::set_instance_name(name);
    Initialize();

    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PMT2CTR"         ,(PMT2CTR)           ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PMT2SFT"         ,(PMT2SFT)           ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PMT2CNT"         ,(PMT2CNT)           ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PMT2CAP"         ,(PMT2CAP)           ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PMT2INVL"        ,(PMT2INVL)          ));
}//}}}

// Description: Destructor
HWCORE_MEASURE_Func::~HWCORE_MEASURE_Func()
{//{{{
}//}}}

// Description: Reset Func and registers
void HWCORE_MEASURE_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    Chwcore_measure_regif::EnableReset(is_active);
}//}}}

// Description: Initialize internal variables
void HWCORE_MEASURE_Func::Initialize(void)
{//{{{
    mPMT2CTR_pre_value = 0x00;
}//}}}

// Description: Read API (target socket)
// Ref: {HWCORE_MEASURE_UD_ATT1_006, HWCORE_MEASURE_UD_ReferenceDoc_001, HWCORE_MEASURE_UD_Flow_004}
void HWCORE_MEASURE_Func::read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
// Ref: {HWCORE_MEASURE_UD_ATT1_006, HWCORE_MEASURE_UD_ReferenceDoc_001, HWCORE_MEASURE_UD_Flow_005}
void HWCORE_MEASURE_Func::write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
void HWCORE_MEASURE_Func::RegisterHandler(const std::vector<std::string> &args)
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
                msg_return = Chwcore_measure_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Cspid_regif class
            msg_return = Chwcore_measure_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Write value to register from core
void HWCORE_MEASURE_Func::SetRegVal(std::string RegName, unsigned int val)
{//{{{
    (*(mRegisterMap[RegName.c_str()])) = val;
    
}//}}}

///Read value from register from core
unsigned int HWCORE_MEASURE_Func::GetRegVal(std::string RegName)
{//{{{
    return ((unsigned int)(*(mRegisterMap[RegName.c_str()])));
    
}//}}}

unsigned int HWCORE_MEASURE_Func::GetRegVal(std::string RegName, std::string BitName)
{//{{{
    return ((unsigned int)((*(mRegisterMap[RegName.c_str()]))[BitName.c_str()]));
    
}//}}}

unsigned char HWCORE_MEASURE_Func::GetCTRPreVal()
{//{{{
    return ((unsigned char)mPMT2CTR_pre_value);
    
}//}}}

// Description: Dump value of status registers
void HWCORE_MEASURE_Func::DumpStatInfo(void)
{//{{{ 
    printf("Status information of registers: \n");
    printf("    PMT2CTR   = 0x%08X\n", (unsigned int)(*PMT2CTR)["EN"]);
    printf("    PMT2SFT   = 0x%08X\n", (unsigned int)(*PMT2SFT)["SCAPTRG"]);
    printf("    PMT2CNT   = 0x%08X\n", (unsigned int)(*PMT2CNT)["DATA"]);
    printf("    PMT2CAP   = 0x%08X\n", (unsigned int)(*PMT2CAP)["DATA"]);
    printf("    PMT2INVL  = 0x%08X\n", (unsigned int)(*PMT2INVL)["DATA"]);
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
// Description: Callback function of cb_PMT2CTR_EN
// Ref: {HWCORE_MEASURE_UD_Registers_001, HWCORE_MEASURE_UD_Flow_009}
void HWCORE_MEASURE_Func::cb_PMT2CTR_EN(RegCBstr str) 
{//{{{
    mPMT2CTR_pre_value = str.pre_data;
    mpHWCORE_MEASUREAgentController->callback_PMT2CTR_EN();
}//}}}

// Description: Callback function of cb_PMT2SFT_SCAPTRG
// Ref: {HWCORE_MEASURE_UD_Registers_002, HWCORE_MEASURE_UD_Flow_010}
void HWCORE_MEASURE_Func::cb_PMT2SFT_SCAPTRG(RegCBstr str) 
{//{{{
    mpHWCORE_MEASUREAgentController->callback_PMT2SFT_SCAPTRG();
}//}}}

// Description: Callback function of cb_PMT2CNT_DATA
// Ref: {HWCORE_MEASURE_UD_Registers_003, HWCORE_MEASURE_UD_Flow_011, HWCORE_MEASURE_UD_Flow_012}
void HWCORE_MEASURE_Func::cb_PMT2CNT_DATA(RegCBstr str) 
{//{{{
    if (str.is_wr) { //Write callback
        mpHWCORE_MEASUREAgentController->callback_PMT2CNT_DATA_write();
    }    
    else { // Read callback
        mpHWCORE_MEASUREAgentController->callback_PMT2CNT_DATA_read();
    }    
}//}}}

// Description: Callback function of cbDBG_PMT2CNT_DATA
// Ref: {HWCORE_MEASURE_UD_Registers_003, HWCORE_MEASURE_UD_Flow_011, HWCORE_MEASURE_UD_Flow_012}
void HWCORE_MEASURE_Func::cbDBG_PMT2CNT_DATA(RegCBstr str) 
{//{{{
    if (str.is_wr) { //Write callback
        mpHWCORE_MEASUREAgentController->callback_PMT2CNT_DATA_write();
    }    
    else { // Read callback
        mpHWCORE_MEASUREAgentController->callback_PMT2CNT_DATA_read();
    }    
}//}}}

// Description: Callback function of cb_PMT2INVL_DATA
// Ref: {HWCORE_MEASURE_UD_Flow_019}
void HWCORE_MEASURE_Func::cb_PMT2INVL_DATA(RegCBstr str) 
{//{{{
   if ((bool)(*PMT2CTR)["EN"]) {
       (*PMT2INVL) = str.pre_data;
   } else {
       unsigned char DATA = (str.data & 0x1F);
       if (((DATA >= 0xC &&  DATA <= 0xF)) || ((DATA >= 0x11 &&  DATA <= 0x1E))) {
           (*PMT2INVL) = str.pre_data;
           func_re_printf("warning", "invalid setting at PMT2INVL \n"); 
       }
   }
}//}}}
