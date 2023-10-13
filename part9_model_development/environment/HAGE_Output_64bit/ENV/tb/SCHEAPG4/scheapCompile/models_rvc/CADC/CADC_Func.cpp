// ---------------------------------------------------------------------
// $Id: CADC_Func.cpp,v 1.1 2018/11/19 02:50:58 chanle Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "CADC_Func.h"
#include "CADC_AgentController.h"
#include "CADC_UNIT.h"
#include "CADC_UNIT_Func.h"

///Constructor of CADC_Func class
CADC_Func::CADC_Func (std::string name, CADC_AgentController *CADC_AgentControllerPtr):
                Ccadc_regif(name, 32),
                BusSlaveFuncIf()
{//{{{
    assert(CADC_AgentControllerPtr != NULL);
    mCADC_AgentController = CADC_AgentControllerPtr;
    mCADC_AgentController->CommandInit(name);
    Ccadc_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of CADC_Func class
CADC_Func::~CADC_Func (void)
{//{{{
}//}}}

///Reset operation of CADC_Func class
void CADC_Func::Reset (const bool is_active)
{//{{{
    Ccadc_regif::EnableReset(is_active);
}//}}}

///Register handler command to Ccadc_regif class
void CADC_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Ccadc_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Ccadc_regif class
            msg_return = Ccadc_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void CADC_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    if ((0x0000 <= addr) && (addr <= 0x000F)) {
        // Read from Common registers
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else if ((0x1000 <= addr) && (addr <= 0x10FF)) {
        // Read from CADC00
        mCADC_AgentController->mAD_Unit[CADC_AgentController::emAD00]->mCADC_UNIT_Func->read(offsetAddress, trans, t, debug);
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void CADC_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0x1FFF;
    
    if ((0x0000 <= addr) && (addr <= 0x000F)) {
        // Write to Common registers
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
        
        bool is_operating = false;
        for (unsigned int i = 0; i < CADC_AgentController::emADNum; i++) {
            if (mCADC_AgentController->mAD_Unit[i]->mCADC_UNIT_Func->GetStatusFlag(true)) {
                is_operating = true;
            }
        }
        if (is_operating) {
            if ((0x0004 <= addr) && (addr <= 0x0007)) {
                // Write to CADCADGCR
                mCADC_AgentController->print_msg("warning","CADCADGCR is written while CADC is converting.\n");
            } else if ((0x000C <= addr) && (addr <= 0x000F)) {
                // Write to CADCTDCR
                mCADC_AgentController->print_msg("warning","CADCTDCR is written while CADC is converting.\n");
            }
        }
    } else if ((0x1000 <= addr) && (addr <= 0x10FF)) {
        // Write to CADC00
        mCADC_AgentController->mAD_Unit[CADC_AgentController::emAD00]->mCADC_UNIT_Func->write(offsetAddress, trans, t, debug);
    }
}//}}}

/// Get Get CADCADGCR.ODDE/ODE value (Public function)
unsigned int CADC_Func::CheckDisconnectDetectSetup (void)
{//{{{
    return (unsigned int)((*CADCADGCR) >> 4);
}//}}}

/// Get CADCADGCR.CADCUNSND value (Public function)
bool CADC_Func::IsUnsignConfig (void)
{//{{{
    return (bool)(*CADCADGCR)["CADCUNSND"];
}//}}}

/// Get CADCTDCR.CADCTDE value (Public function)
bool CADC_Func::IsPinDiagEnable (void)
{//{{{
    return (bool)(*CADCTDCR)["CADCTDE"];
}//}}}

/// Get CADCADGCR/ODE value (Public function)
bool CADC_Func::ValueODE (void)
{//{{{
    return (bool)(*CADCADGCR)["CADCODE"];
}//}}}

/// Virtual function of CADCnADSTCR register in Ccadc_regif class
void CADC_Func::cb_CADCTDCR_CADCTDE (RegCBstr str)
{//{{{
    if ((IsPinDiagEnable()) && (!(bool)(*CADCADGCR)["CADCODDE"])) {
        mCADC_AgentController->print_msg("warning","When doing pin level self-diagnosis, set CADCODDE to 1 before  setting CADCTDE to 1.\n");
    }
}//}}}

// vim600: set foldmethod=marker :
