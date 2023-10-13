// ---------------------------------------------------------------------
// $Id: ATU5_GLUE_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "ATU5_GLUE_Func.h"
#include "ATU5_GLUE_AgentController.h"

///Constructor of ATU5_GLUE_Func class
ATU5_GLUE_Func::ATU5_GLUE_Func (std::string name, ATU5_GLUE_AgentController *ATU5_GLUE_AgentControllerPtr):
                Catu5_glue_regif(name, 32),
                BusSlaveFuncIf()
{//{{{
    assert(ATU5_GLUE_AgentControllerPtr != NULL);
    mATU5_GLUE_AgentController = ATU5_GLUE_AgentControllerPtr;
    mATU5_GLUE_AgentController->CommandInit(name);
    Catu5_glue_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of ATU5_GLUE_Func class
ATU5_GLUE_Func::~ATU5_GLUE_Func (void)
{//{{{
}//}}}

///Reset operation of ATU5_GLUE_Func class
void ATU5_GLUE_Func::Reset (const bool is_active)
{//{{{
    Catu5_glue_regif::EnableReset(is_active);
}//}}}

///Register handler command to Catu5_glue_regif class
void ATU5_GLUE_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Catu5_glue_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_glue_regif class
            msg_return = Catu5_glue_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void ATU5_GLUE_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void ATU5_GLUE_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")){
        return;
    }
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUINTSELA_ATU_INTSEL_A0 (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emINTSELA, 0);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUINTSELD_ATU_INTSEL_D00 (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emINTSELD0, str.channel);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUINTSELD4_ATU_INTSEL_D00 (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emINTSELD0, 4);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUDMASELB_ATU_DMASEL_B0 (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emDMASELB, 0);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUDMASELCD_ATU_DMASEL_CD0 (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emDMASELCD0, str.channel);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUDMASELE_ATU_DMASEL_E (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emDMASELE, 0);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUDFEENTQ_ATU_DFEEN_TQ (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emDFEENTQ0, str.channel);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUDFESEL_ATU_DFESEL_D00 (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emDFESEL0, str.channel);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUDFESELD1T_ATU_DFESEL_D1T (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emDFESELD1T, 0);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUDSSELDSTS_ATU_DSSEL_DSTS0 (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emDSSELDSTS, 0);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUCASELCATS_ATU_CASEL_CATS0 (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emCASELCATS, 0);
    }
}//}}}

/// Call back of register
void ATU5_GLUE_Func::cb_ATUP5SSEL_ATU_P5SSEL_TGC1 (RegCBstr str)
{//{{{
    if (!mATU5_GLUE_AgentController->GetResetStatus("PRESETZ") && mATU5_GLUE_AgentController->CheckClockPeriod("PCLK")) {
        mATU5_GLUE_AgentController->NotifyEvent(ATU5_GLUE_AgentController::emP5SSEL, 0);
    }
}//}}}

unsigned int ATU5_GLUE_Func::GetRegSelectSetting (const unsigned int type)
{//{{{
    unsigned int ret = 0;
    switch (type) {
        case ATU5_GLUE_AgentController::emINTSELA:
            ret = (unsigned int)(*ATUINTSELA);
            break;
        case ATU5_GLUE_AgentController::emINTSELD0:
            ret = (unsigned int)(*ATUINTSELD[0]);
            break;
        case ATU5_GLUE_AgentController::emINTSELD1:
            ret = (unsigned int)(*ATUINTSELD[1]);
            break;
        case ATU5_GLUE_AgentController::emINTSELD2:
            ret = (unsigned int)(*ATUINTSELD[2]);
            break;
        case ATU5_GLUE_AgentController::emINTSELD3:
            ret = (unsigned int)(*ATUINTSELD[3]);
            break;
        case ATU5_GLUE_AgentController::emINTSELD4:
            ret = (unsigned int)(*ATUINTSELD4);
            break;
        case ATU5_GLUE_AgentController::emDMASELB:
            ret = (unsigned int)(*ATUDMASELB);
            break;
        case ATU5_GLUE_AgentController::emDMASELCD0:
            ret = (unsigned int)(*ATUDMASELCD[0]);
            break;
        case ATU5_GLUE_AgentController::emDMASELCD1:
            ret = (unsigned int)(*ATUDMASELCD[1]);
            break;
        case ATU5_GLUE_AgentController::emDMASELCD2:
            ret = (unsigned int)(*ATUDMASELCD[2]);
            break;
        case ATU5_GLUE_AgentController::emDMASELCD3:
            ret = (unsigned int)(*ATUDMASELCD[3]);
            break;
        case ATU5_GLUE_AgentController::emDMASELCD4:
            ret = (unsigned int)(*ATUDMASELCD[4]);
            break;
        case ATU5_GLUE_AgentController::emDMASELCD5:
            ret = (unsigned int)(*ATUDMASELCD[5]);
            break;
        case ATU5_GLUE_AgentController::emDMASELCD6:
            ret = (unsigned int)(*ATUDMASELCD[6]);
            break;
        case ATU5_GLUE_AgentController::emDMASELCD7:
            ret = (unsigned int)(*ATUDMASELCD[7]);
            break;
        case ATU5_GLUE_AgentController::emDFEENTQ0:
            ret = (unsigned int)(*ATUDFEENTQ[0]);
            break;
        case ATU5_GLUE_AgentController::emDFEENTQ1:
            ret = (unsigned int)(*ATUDFEENTQ[1]);
            break;
        case ATU5_GLUE_AgentController::emDFEENTQ2:
            ret = (unsigned int)(*ATUDFEENTQ[2]);
            break;
        case ATU5_GLUE_AgentController::emDFESEL0:
            ret = (unsigned int)(*ATUDFESEL[0]);
            break;
        case ATU5_GLUE_AgentController::emDFESEL1:
            ret = (unsigned int)(*ATUDFESEL[1]);
            break;
        case ATU5_GLUE_AgentController::emDFESEL2:
            ret = (unsigned int)(*ATUDFESEL[2]);
            break;
        case ATU5_GLUE_AgentController::emDFESELD1T:
            ret = (unsigned int)(*ATUDFESELD1T);
            break;
        case ATU5_GLUE_AgentController::emDSSELDSTS:
            ret = (unsigned int)(*ATUDSSELDSTS);
            break;
        case ATU5_GLUE_AgentController::emCASELCATS:
            ret = (unsigned int)(*ATUCASELCATS);
            break;
        case ATU5_GLUE_AgentController::emP5SSEL:
            ret = (unsigned int)(*ATUP5SSEL);
            break;
        default:        // emDMASELE
            ret = (unsigned int)(*ATUDMASELE);
            break;
    }
    return ret;
}//}}}

// vim600: set foldmethod=marker :
