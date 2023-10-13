// ---------------------------------------------------------------------
// $Id: HWCORE_IIR_Func.cpp,v 1.4 2020/11/12 16:43:32 minhquocha Exp $
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
// Description: Copyright of HWCORE_IIR_Func.cpp
// Ref: {HWCORE_IIR_UD_Copyright_001}
// Description: About the include header files of HWCORE_IIR_Func.cpp
// Ref: {HWCORE_IIR_UD_ATT1_002}
#include "HWCORE_IIR_Func.h"
#include "global.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
///*********************************
/// Function     : HWCORE_INPUT_Func
/// Description  : Constructor HWCORE_INPUT_Func
///*********************************
HWCORE_IIR_Func::HWCORE_IIR_Func(std::string name, HWCORE_IIR_AgentController* HWCORE_IIRAgentControllerPtr) :
    Chwcore_iir_regif(name, 32)
{//{{{
    assert(HWCORE_IIRAgentControllerPtr != NULL);
    mpHWCORE_IIRAgentController = HWCORE_IIRAgentControllerPtr;
    mpHWCORE_IIRAgentController->CommandInit(name);
    mModuleName = name;

    Chwcore_iir_regif::set_instance_name(name);
    Initialize();

    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRCTR0"         ,(IIRCTR[0])         ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRCTR1"         ,(IIRCTR[1])         ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRCTR2"         ,(IIRCTR[2])         ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRINIT"         ,(IIRINIT)           ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRSFT"          ,(IIRSFT)            ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRRLD"          ,(IIRRLD)            ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRRSL"          ,(IIRRSL)            ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRSTAT"         ,(IIRSTAT)           ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRSTATC"        ,(IIRSTATC)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRICOEF"        ,(IIRICOEF)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRCOEF0"        ,(IIRCOEF[0])        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRCOEF1"        ,(IIRCOEF[1])        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRCOEF2"        ,(IIRCOEF[2])        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRCOEF3"        ,(IIRCOEF[3])        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRCOEF4"        ,(IIRCOEF[4])        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIROCOEF"        ,(IIROCOEF)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRSFTDAT0"      ,(IIRSFTDAT[0])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRSFTDAT1"      ,(IIRSFTDAT[1])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRSFTDAT2"      ,(IIRSFTDAT[2])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRZN1DAT0"      ,(IIRZN1DAT[0])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRZN1DAT1"      ,(IIRZN1DAT[1])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRZN1DAT2"      ,(IIRZN1DAT[2])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRZN2DAT0"      ,(IIRZN2DAT[0])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRZN2DAT1"      ,(IIRZN2DAT[1])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIRZN2DAT2"      ,(IIRZN2DAT[2])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIROUTDAT0"      ,(IIROUTDAT[0])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIROUTDAT1"      ,(IIROUTDAT[1])      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IIROUTDAT2"      ,(IIROUTDAT[2])      ));
}//}}}

///*********************************
/// Function     : ~HWCORE_INPUT_Func
/// Description  : Destructor of HWCORE_INPUT_Func
///*********************************
HWCORE_IIR_Func::~HWCORE_IIR_Func()
{//{{{
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : eset HWCORE_IIR_Func and hwcore_iir_regif
///*********************************
void HWCORE_IIR_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    Chwcore_iir_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handler for register IF
///*********************************
void HWCORE_IIR_Func::RegisterHandler(const std::vector<std::string> &args)
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
                msg_return = Chwcore_iir_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Cspid_regif class
            msg_return = Chwcore_iir_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///*********************************
/// Function     : read 
/// Description: read register
// Ref: {HWCORE_IIR_UD_ATT1_006, HWCORE_IIR_UD_Flow_004}
///*********************************
void HWCORE_IIR_Func::read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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

///*********************************
/// Function     : write
/// Description: write register
// Ref: {HWCORE_IIR_UD_ATT1_006, HWCORE_IIR_UD_Flow_005}
///*********************************
void HWCORE_IIR_Func::write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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

///*********************************
/// Function     : SetRegVal
/// Description  : Write value to register from core
///*********************************
void HWCORE_IIR_Func::SetRegVal(std::string RegName, unsigned int val)
{//{{{
    int check_exist = mRegisterMap.count(RegName);

    if (check_exist < 0) {
        return;
    }
    (*(mRegisterMap[RegName.c_str()])) = val;
}//}}}

///*********************************
/// Function     : SetRegVal
/// Description  : Write value to register bit field from core
///*********************************
void HWCORE_IIR_Func::SetRegVal(std::string RegName, std::string BitName, unsigned int val)
{//{{{
    int check_exist = mRegisterMap.count(RegName);

    if (check_exist < 0) {
        return;
    }
    (*(mRegisterMap[RegName.c_str()]))[BitName.c_str()] = val;
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : Read value from register from core
///*********************************
unsigned int HWCORE_IIR_Func::GetRegVal(std::string RegName)
{//{{{
        return ((unsigned int)(*(mRegisterMap[RegName.c_str()])));
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : Read value from register bit field from core
///*********************************
unsigned int HWCORE_IIR_Func::GetRegVal(std::string RegName, std::string BitName)
{//{{{
        return ((unsigned int)((*(mRegisterMap[RegName.c_str()]))[BitName.c_str()]));
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void HWCORE_IIR_Func::Initialize(void)
{//{{{
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
///*********************************
/// Function     : cb_IIRSFT_TRG
/// Description  : Callback function of IIRSFT.TRG
// Ref: {HWCORE_IIR_UD_Registers_003, HWCORE_IIR_UD_Flow_007}
///*********************************
void HWCORE_IIR_Func::cb_IIRSFT_TRG(RegCBstr str)
{//{{{
    if((*IIRSFT)["TRG"] == 1)
    {
        mpHWCORE_IIRAgentController->callback_IIRSFT_TRG();
    }
}//}}}

///*********************************
/// Function     : cb_IIRINIT_INIT0
/// Description  : Callback function of IIRINIT
// Ref: {HWCORE_IIR_UD_Registers_002, HWCORE_IIR_UD_Flow_006}
///*********************************
void HWCORE_IIR_Func::cb_IIRINIT_INIT0(RegCBstr str)
{//{{{
    if((*IIRINIT)["INIT0"] == 1)
    {
        /* clear pre value */
        (*IIRZN1DAT[0]) = 0x00;
        (*IIRZN2DAT[0]) = 0x00;
    }
    
    if((*IIRINIT)["INIT1"] == 1)
    {
        /* clear pre value */
        (*IIRZN1DAT[1]) = 0x00;
        (*IIRZN2DAT[1]) = 0x00;
    }
    
    if((*IIRINIT)["INIT2"] == 1)
    {
        /* clear pre value */
        (*IIRZN1DAT[2]) = 0x00;
        (*IIRZN2DAT[2]) = 0x00;
    }
    (*IIRINIT) &= 0x00;
}//}}}

///*********************************
/// Function     : cb_IIRRLD_RLD0
/// Description  : Callback function of IIRRLD
// Ref: {HWCORE_IIR_UD_Registers_004, HWCORE_IIR_UD_Flow_008}
///*********************************
void HWCORE_IIR_Func::cb_IIRRLD_RLD0(RegCBstr str)
{//{{{
    mpHWCORE_IIRAgentController->callback_IIRRLD();
}//}}}

///*********************************
/// Function     : cb_IIRRSL_RSL
/// Description  : Callback function of IIRRSL
// Ref: {HWCORE_IIR_UD_Registers_005, HWCORE_IIR_UD_Flow_009}
///*********************************
void HWCORE_IIR_Func::cb_IIRRSL_RSL(RegCBstr str)
{//{{{
    mpHWCORE_IIRAgentController->callback_IIRRSL();
}//}}}
