// ---------------------------------------------------------------------
// $Id: DSADC_E2XFCC1_2ND_Func.cpp 1415 2019-11-29 09:08:35Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "DSADC_E2XFCC1_2ND_Func.h"
#include "DSADC_E2XFCC1_2ND_AgentController.h"
#include "DSADC_UNIT.h"
#include "DSADC_UNIT_Func.h"

/// Constructor of DSADC_E2XFCC1_2ND_Func class
DSADC_E2XFCC1_2ND_Func::DSADC_E2XFCC1_2ND_Func (std::string name, DSADC_E2XFCC1_2ND_AgentController *DSADC_E2XFCC1_2ND_AgentControllerPtr):
                Cdsadc_e2xfcc1_2nd_regif(name, 32),
                BusSlaveFuncIf()
{//{{{
    assert(DSADC_E2XFCC1_2ND_AgentControllerPtr != NULL);
    mDSADC_E2XFCC1_2ND_AgentController = DSADC_E2XFCC1_2ND_AgentControllerPtr;
    mDSADC_E2XFCC1_2ND_AgentController->CommandInit(name);
    Cdsadc_e2xfcc1_2nd_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

/// Destructor of DSADC_E2XFCC1_2ND_Func class
DSADC_E2XFCC1_2ND_Func::~DSADC_E2XFCC1_2ND_Func (void)
{//{{{
}//}}}

/// Reset operation of DSADC_E2XFCC1_2ND_Func class
void DSADC_E2XFCC1_2ND_Func::Reset (const bool is_active)
{//{{{
    Cdsadc_e2xfcc1_2nd_regif::EnableReset(is_active);
}//}}}

/// Register handler command to Cdsadc_e2xfcc1_2nd_regif class
void DSADC_E2XFCC1_2ND_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {     /// Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {   /// Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Cdsadc_e2xfcc1_2nd_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {    /// Set handle command to Cdsadc_e2xfcc1_2nd_regif class
            msg_return = Cdsadc_e2xfcc1_2nd_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Virtual function of BusSlaveFuncIf class
void DSADC_E2XFCC1_2ND_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    bool resetStatus = mDSADC_E2XFCC1_2ND_AgentController->GetResetStatus("PRESETn");
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    bool is_debug_master_access = false;
    if (extension != NULL){
        is_debug_master_access = (extension->getPeId() == DEBUG_MASTER_BMID);
    }
    /// When clock == 0 or assert reset, only bebug master read.
    if (!mDSADC_E2XFCC1_2ND_AgentController->CheckClockPeriod("CLK_LSB") || resetStatus) {
        if (!debug || (debug && !is_debug_master_access)) {
            if (resetStatus) {
                re_printf("warning", "Cannot read during reset period\n");
            } else {
                re_printf("warning", "Cannot access register when clock is 0.\n");
            }
            return;
        } else {
            /// do nothing
        }
    }

    if (addr <= 0x0F) {
        /// Read from Common registers
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

/// Virtual function of BusSlaveFuncIf class
void DSADC_E2XFCC1_2ND_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFF;
    TlmG3mExtension *extension;
    trans.get_extension(extension);
    bool is_debug_master_access = false;
    if (extension != NULL){
        is_debug_master_access = (extension->getPeId() == DEBUG_MASTER_BMID);
    }
    /// When clock = 0, only bebug master write.
    if (!mDSADC_E2XFCC1_2ND_AgentController->CheckClockPeriod("CLK_LSB")) {
        if (!debug || (debug && !is_debug_master_access)) {
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        } else {
            /// do nothing
        }
    }
    /// Access write to register
    if (addr <= 0x0F) {
        /// Write to Common registers
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
        
    bool is_operating = false;
    for (unsigned int i = 0; i < DSADC_E2XFCC1_2ND_AgentController::emADNum; i++) {
        if (mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->GetStatusFlag(mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->emADCT)
                || mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->GetStatusFlag(mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->emdCLBACT)
                || mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->GetStatusExterTrigger(mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->emSTTRGE)
                || mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->GetStatusExterTrigger(mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->emENDTRGE)){
            is_operating = true;
        }
    }
    if (is_operating) {
        if ((0x04 <= addr) && (addr <= 0x07)) {
            /// Write to DSADCADGCR
            mDSADC_E2XFCC1_2ND_AgentController->print_msg("warning","DSADCADGCR is written while ADACT/CLBACT/STTRGE/ENDTRGE is set.\n");
        } else if ((0x0C <= addr) && (addr <= 0x0F)) {
            /// Write to DSADCTDCR
            mDSADC_E2XFCC1_2ND_AgentController->print_msg("warning","DSADCTDCR is written while ADACT/CLBACT/STTRGE/ENDTRGE is set.\n");
        }
    }
}//}}}

/// Virtual function of DSADCSYNSTCR register in Cdsadc_e2xfcc1_2nd_regif class
void DSADC_E2XFCC1_2ND_Func::cb_DSADCSYNSTCR_ADSTART (RegCBstr str)
{//{{{
    if (!mDSADC_E2XFCC1_2ND_AgentController->GetResetStatus("resetad_n") && mDSADC_E2XFCC1_2ND_AgentController->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*DSADCSYNSTCR)["ADSTART"] == 1) {
                for (unsigned int i = 0; i < DSADC_E2XFCC1_2ND_AgentController::emADNum; i++) {
                    if ((mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->IsSyncStartEnable()) &&
                    (!mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->GetStatusFlag(mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->emADCT))) {
                        mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mStartTriggerEvent.notify(SC_ZERO_TIME);
                        if (mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->GetStatusExterTrigger(mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->emSTTRGE) 
                            || mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->GetStatusExterTrigger(mDSADC_E2XFCC1_2ND_AgentController->mAD_Unit[i]->mDSADC_UNIT_Func->emENDTRGE))
                        {
                            mDSADC_E2XFCC1_2ND_AgentController->print_msg("warning","It isn't recommend to set ADSTART when STTRGE/ENDTRGE is active.\n");
                        }
                    }
                }
        }
    }
    (*DSADCSYNSTCR) = 0;
}//}}}

/// Get Get DSADCADGCR.ODDE/ODE value (Public function)
unsigned int DSADC_E2XFCC1_2ND_Func::CheckDisconnectDetectSetup (void)
{//{{{
    return (unsigned int)((*DSADCADGCR) >> 4);
}//}}}

/// Get DSADCADGCR.UNSND value (Public function)
bool DSADC_E2XFCC1_2ND_Func::IsUnsignConfig (void)
{//{{{
    return (bool)(*DSADCADGCR)["UNSND"];
}//}}}

/// Get DSADCTDCR.TDE value (Public function)
bool DSADC_E2XFCC1_2ND_Func::IsPinDiagEnable (void)
{//{{{
    return (bool)(*DSADCTDCR)["TDE"];
}//}}}

/// Virtual function of DSADCTDCR register in Cdsadc_e2xfcc1_2nd_regif class
void DSADC_E2XFCC1_2ND_Func::cb_DSADCTDCR_TDE (RegCBstr str)
{//{{{
    if (!mDSADC_E2XFCC1_2ND_AgentController->GetResetStatus("resetad_n") && mDSADC_E2XFCC1_2ND_AgentController->CheckClockPeriod("CLK_HSB")) {
        if ((str.pre_data == 0) && ((bool)(*DSADCTDCR)["TDE"] == 1) && ((bool)(*DSADCADGCR)["ODDE"] == 0)) {
            mDSADC_E2XFCC1_2ND_AgentController->print_msg("warning","For pin level self-diagnosis, set ODDE to 1 before setting TDE to 1.\n");
        }
    }
}//}}}

// vim600: set foldmethod=marker :
