// ---------------------------------------------------------------------
// $Id: HWCORE_PI_Func.cpp,v 1.4 2020/11/08 05:38:30 minhquocha Exp $
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
// Description: Copyright of HWCORE_PI_Func.cpp
// Ref: {HWCORE_PI_UD_Copyright_001}
// Description: About the include header files of HWCORE_PI_Func.cpp
// Ref: {HWCORE_PI_UD_ATT1_002}
#include "HWCORE_PI_AgentController.h"
#include "HWCORE_PI_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
///*********************************
/// Function     : HWCORE_PI_Func
/// Description  : Constructor HWCORE_PI_Func
///*********************************
HWCORE_PI_Func::HWCORE_PI_Func(std::string name, HWCORE_PI_AgentController* HWCORE_PIAgentControllerPtr) :
    Chwcore_pi_regif(name, 32)
{//{{{
    assert(HWCORE_PIAgentControllerPtr != NULL);
    mpHWCORE_PIAgentController = HWCORE_PIAgentControllerPtr;
    mpHWCORE_PIAgentController->CommandInit(name);
    mModuleName = name;

    Chwcore_pi_regif::set_instance_name(name);
    Initialize();

    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("PICTR"       ,(PICTR)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IDIN"        ,(IDIN)        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IQIN"        ,(IQIN)        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("ID"          ,(ID)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("IQ"          ,(IQ)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("GPD0"        ,(GPD0)        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("GPQ0"        ,(GPQ0)        ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("GPD"         ,(GPD)         ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("GPQ"         ,(GPQ)         ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("GID"         ,(GID)         ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("GIQ"         ,(GIQ)         ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("GIDMAX"      ,(GIDMAX)      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("GIQMAX"      ,(GIQMAX)      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("SUMID"       ,(SUMID)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("SUMIQ"       ,(SUMIQ)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("SUMIDM"      ,(SUMIDM)      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("SUMIQM"      ,(SUMIQM)      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VDMAX"       ,(VDMAX)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VQMAX"       ,(VQMAX)       ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VD"          ,(VD)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VQ"          ,(VQ)          ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VDOBUF"      ,(VDOBUF)      ));
    mRegisterMap.insert(std::pair<std::string, vpcl::re_register*>("VQOBUF"      ,(VQOBUF)      ));
}//}}}

///*********************************
/// Function     : ~HWCORE_PI_Func
/// Description  : Destructor of HWCORE_PI_Func
///*********************************
HWCORE_PI_Func::~HWCORE_PI_Func()
{//{{{
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : eset HWCORE_PI_Func and HWCORE_PI_regif
///*********************************
void HWCORE_PI_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    Chwcore_pi_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handler for register IF
///*********************************
void HWCORE_PI_Func::RegisterHandler(const std::vector<std::string> &args)
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
                msg_return = Chwcore_pi_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        }
        else {//Set handle command to Cspid_regif class
            msg_return = Chwcore_pi_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///*********************************
/// Function     : read
/// Description: read register
// Ref: {HWCORE_PI_UD_ATT1_006, HWCORE_PI_UD_Flow_004}
///*********************************
void HWCORE_PI_Func::read (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
// Ref: {HWCORE_PI_UD_ATT1_006, HWCORE_PI_UD_Flow_005}
///*********************************
void HWCORE_PI_Func::write (unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
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
void HWCORE_PI_Func::SetRegVal(std::string RegName, unsigned int val)
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
void HWCORE_PI_Func::SetRegVal(std::string RegName, std::string BitName, unsigned int val)
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
unsigned int HWCORE_PI_Func::GetRegVal(std::string RegName)
{//{{{
    return ((unsigned int)(*(mRegisterMap[RegName.c_str()])));
}//}}}

///*********************************
/// Function     : GetRegVal
/// Description  : Read value from register bit field from core
///*********************************
unsigned int HWCORE_PI_Func::GetRegVal(std::string RegName, std::string BitName)
{//{{{
    return ((unsigned int)((*(mRegisterMap[RegName.c_str()]))[BitName.c_str()]));
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void HWCORE_PI_Func::Initialize(void)
{//{{{
}//}}}

///*********************************
/// Function     : cb_IDIN_DATA
/// Description  : callback function IDIN DATA
// Ref: {HWCORE_PI_UD_Flow_013}
///*********************************
void HWCORE_PI_Func::cb_IDIN_DATA(RegCBstr str)
{//{{{
    if ((*PICTR)["FPIIBTH"] == 1) {
        mpHWCORE_PIAgentController->Callback_IDIN();
    }
}//}}}

///*********************************
/// Function     : cb_IQIN_DATA
/// Description  : callback function IQIN DATA
// Ref: {HWCORE_PI_UD_Flow_013}
///*********************************
void HWCORE_PI_Func::cb_IQIN_DATA(RegCBstr str)
{//{{{
    if ((*PICTR)["FPIIBTH"] == 1) {
        mpHWCORE_PIAgentController->Callback_IQIN();
    }
}//}}}

///*********************************
/// Function     : cb_ID_DATA
/// Description  : callback function ID DATA
// Ref: {HWCORE_PI_UD_Flow_013}
///*********************************
void HWCORE_PI_Func::cb_ID_DATA(RegCBstr str)
{//{{{
    if ((*PICTR)["FPIIBTH"] == 1) {
        mpHWCORE_PIAgentController->Callback_ID();
    }
}//}}}

///*********************************
/// Function     : cb_IQ_DATA
/// Description  : callback function IQ DATA
// Ref: {HWCORE_PI_UD_Flow_013}
///*********************************
void HWCORE_PI_Func::cb_IQ_DATA(RegCBstr str)
{//{{{
    if ((*PICTR)["FPIIBTH"] == 1) {
        mpHWCORE_PIAgentController->Callback_IQ();
    }
}//}}}

///*********************************
/// Function     : cb_VD_DATA
/// Description  : callback function VD DATA
// Ref: {HWCORE_PI_UD_Flow_014}
///*********************************
void HWCORE_PI_Func::cb_VD_DATA(RegCBstr str)
{//{{{
    mpHWCORE_PIAgentController->Callback_VD();
}//}}}

///*********************************
/// Function     : cb_VQ_DATA
/// Description  : callback function VQ DATA
// Ref: {HWCORE_PI_UD_Flow_014}
///*********************************
void HWCORE_PI_Func::cb_VQ_DATA(RegCBstr str)
{//{{{
    mpHWCORE_PIAgentController->Callback_VQ();
}//}}}
