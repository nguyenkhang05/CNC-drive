// ---------------------------------------------------------------------
// $Id: ATU5011_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5011_Func.h"
#include "ATU5011_AgentController.h"
#include "ATU5_TIMERA.h"
#include "ATU5_TIMERA_Func.h"
#include "ATU5_TIMERB.h"
#include "ATU5_TIMERB_Func.h"
#include "ATU5_TIMERC.h"
#include "ATU5_TIMERC_Func.h"
#include "ATU5_TIMERD.h"
#include "ATU5_TIMERD_Func.h"
#include "ATU5_TIMERE.h"
#include "ATU5_TIMERE_Func.h"
#include "ATU5_TIMERF.h"
#include "ATU5_TIMERF_Func.h"
#include "ATU5_TIMERG.h"
#include "ATU5_TIMERG_Func.h"

///Constructor of ATU5011_Func class
ATU5011_Func::ATU5011_Func (std::string name, ATU5011_AgentController *ATU5011_AgentControllerPtr):
                Catu5011_regif(name, 32),
                BusSlaveFuncIf()
{//{{{
    assert(ATU5011_AgentControllerPtr != NULL);
    mATU5011_AgentController = ATU5011_AgentControllerPtr;
    mATU5011_AgentController->CommandInit(name);
    Catu5011_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of ATU5011_Func class
ATU5011_Func::~ATU5011_Func (void)
{//{{{
}//}}}

///Reset operation of ATU5011_Func class
void ATU5011_Func::Reset (const bool is_active)
{//{{{
    Catu5011_regif::EnableReset(is_active);
}//}}}

///Register handler command to Catu5011_regif class
void ATU5011_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Catu5011_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5011_regif class
            msg_return = Catu5011_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void ATU5011_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());

    if ((0x0 <= addr) && (addr <= 0xFF) || (0x8000 <= addr) && (addr <= 0x80FF)) { // Read from Common registers
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else if ((0x200 <= addr) && (addr <= 0x3FF)) { // Read from TimerA
        mATU5011_AgentController->mATU5_TimerA->mATU5_TIA_Func->read(offsetAddress, trans, t, debug);
    } else if ((0x400 <= addr) && (addr <= 0x5FF)) { // Read from TimerB
        mATU5011_AgentController->mATU5_TimerB->mATU5_TIB_Func->read(offsetAddress, trans, t, debug);
    } else if ((0x800 <= addr) && (addr <= 0xFFF)) { // Read from TimerC
        mATU5011_AgentController->mATU5_TimerC->mTimerC_Func->read(offsetAddress, trans, t, debug);
    } else if ((0x2000 <= addr) && (addr <= 0x3FFF)) { // Read from TimerD
        mATU5011_AgentController->mATU5_TimerD->mATU5_TIMERDFunc->read(offsetAddress, trans, t, debug);
    } else if ((0x4000 <= addr) && (addr <= 0x4FFF)) { // Read from TimerE
        mATU5011_AgentController->mATU5_TimerE->mATU5_TIMEREFunc->read(offsetAddress, trans, t, debug);
    } else if ((0x5000 <= addr) && (addr <= 0x59FF)) { // Read from TimerF
        mATU5011_AgentController->mATU5_TimerF->mATU5_TIMERF_Func->read(offsetAddress, trans, t, debug);
    } else if ((0x5C00 <= addr) && (addr <= 0x5CFF)) { // Read from TimerG
        mATU5011_AgentController->mATU5_TimerG->pFuncModel->read(offsetAddress, trans, t, debug);
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void ATU5011_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!mATU5011_AgentController->CheckClockPeriod("CLK_HSB")){
        return;
    }
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFF;

    if ((0x0 <= addr) && (addr <= 0xFF) || (0x8000 <= addr) && (addr <= 0x80FF)) { // Write from Common registers
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else if ((0x200 <= addr) && (addr <= 0x3FF)) { // Write from TimerA
        mATU5011_AgentController->mATU5_TimerA->mATU5_TIA_Func->write(offsetAddress, trans, t, debug);
    } else if ((0x400 <= addr) && (addr <= 0x5FF)) { // Write from TimerB
        mATU5011_AgentController->mATU5_TimerB->mATU5_TIB_Func->write(offsetAddress, trans, t, debug);
    } else if ((0x800 <= addr) && (addr <= 0xFFF)) { // Write from TimerC
        mATU5011_AgentController->mATU5_TimerC->mTimerC_Func->write(offsetAddress, trans, t, debug);
    } else if ((0x2000 <= addr) && (addr <= 0x3FFF)) { // Write from TimerD
        mATU5011_AgentController->mATU5_TimerD->mATU5_TIMERDFunc->write(offsetAddress, trans, t, debug);
    } else if ((0x4000 <= addr) && (addr <= 0x4FFF)) { // Write from TimerE
        mATU5011_AgentController->mATU5_TimerE->mATU5_TIMEREFunc->write(offsetAddress, trans, t, debug);
    } else if ((0x5000 <= addr) && (addr <= 0x59FF)) { // Write from TimerF
        mATU5011_AgentController->mATU5_TimerF->mATU5_TIMERF_Func->write(offsetAddress, trans, t, debug);
    } else if ((0x5C00 <= addr) && (addr <= 0x5CFF)) { // Write from TimerG
        mATU5011_AgentController->mATU5_TimerG->pFuncModel->write(offsetAddress, trans, t, debug);
    }
}//}}}

/*********************************
// Function     : GetNoiseCancelMode
// Description  : Return noise cancellation mode
// Parameter    :
//      timer_name          Timer name ("Timer A", "Timer C", "Timer F")
// Return value : noise cancellation mode
//      0                   Premature-transition cancellation mode
//      1                   Minimum time-at-level cancellation mode
//      2                   Level-accumulated cancellation mode
**********************************/
unsigned int ATU5011_Func::GetNoiseCancelMode (const std::string timer_name)
{//{{{
    unsigned int ret = 0;
    if (((timer_name == "Timer A")&&((unsigned int)(*NCMR)["NCMA"] == 1)) ||
        ((timer_name == "Timer C")&&((unsigned int)(*NCMR)["NCMC"] == 1)) ||
        ((timer_name == "Timer F")&&((unsigned int)(*NCMR)["NCMF"] == 1))){
        if ((unsigned int)(*NCMR)["NCMSEL"] == 0) {
            ret = 1;
        } else {
            ret = 2;
        }
    }
    return ret;
}//}}}

/// Call back of register
void ATU5011_Func::cb_ATUENR_PSCE (RegCBstr str)
{//{{{
    if (!mATU5011_AgentController->GetResetStatus("presetz") && mATU5011_AgentController->CheckClockPeriod("CLK_LSB")) {
        mATU5011_AgentController->EnablePrescaler((bool)(*ATUENR)["PSCE"]);
        mATU5011_AgentController->mATU5_TimerA->EnableTIA((bool)(*ATUENR)["TAE"]);
        if (((bool)(*ATUENR)["TAE"]) && (((str.pre_data >> 1) & 0x1) == 0)) {
            // TAE: 0 -> 1
            mATU5011_AgentController->CheckClkBusSel(ATU5011_AgentController::emTimerA);
        }
        mATU5011_AgentController->mATU5_TimerB->EnableTIB((bool)(*ATUENR)["TBE"]);
        if (((bool)(*ATUENR)["TBE"]) && (((str.pre_data >> 2) & 0x1) == 0)) {
            // TBE: 0 -> 1
            mATU5011_AgentController->CheckClkBusSel(ATU5011_AgentController::emTimerB);
        }
        mATU5011_AgentController->mATU5_TimerC->EnableTIC((bool)(*ATUENR)["TCE"]);
        mATU5011_AgentController->mATU5_TimerD->EnableTID((bool)(*ATUENR)["TDE"]);
        mATU5011_AgentController->mATU5_TimerE->EnableTIE((bool)(*ATUENR)["TEE"]);
        mATU5011_AgentController->mATU5_TimerF->EnableTimerF((bool)(*ATUENR)["TFE"]);
        mATU5011_AgentController->mATU5_TimerG->EnableTIG((bool)(*ATUENR)["TGE"]);
    }
}//}}}

/// Call back of register
void ATU5011_Func::cb_CBCNT_CB5EG (RegCBstr str)
{//{{{
    if (!mATU5011_AgentController->GetResetStatus("presetz") && mATU5011_AgentController->CheckClockPeriod("CLK_LSB")) {
        if (str.pre_data != str.data) {
            mATU5011_AgentController->ClkBusSetupChange();
        }
    }
}//}}}

/// Call back of register
void ATU5011_Func::cb_NCMR_NCMA (RegCBstr str)
{//{{{
    if (!mATU5011_AgentController->GetResetStatus("presetz") && mATU5011_AgentController->CheckClockPeriod("CLK_LSB")) {
        if (((str.pre_data >> 7) & 0x1) != (unsigned int)(*NCMR)["NCCSEL"]) {
            mATU5011_AgentController->NCClkSetupChange();
        }
    }
}//}}}

/// Call back of register
void ATU5011_Func::cb_PSCR_PSC (RegCBstr str)
{//{{{
    if (!mATU5011_AgentController->GetResetStatus("presetz") && mATU5011_AgentController->CheckClockPeriod("CLK_LSB")) {
        if (str.pre_data != str.data) {
            mATU5011_AgentController->PrescalerSetupChange(str.channel);
        }
    }
}//}}}

/// Call back of register
void ATU5011_Func::cb_TRGSELDMA0_INSELDMA0 (RegCBstr str)
{//{{{
    if (!mATU5011_AgentController->GetResetStatus("presetz") && mATU5011_AgentController->CheckClockPeriod("CLK_LSB")) {
        mATU5011_AgentController->NotifyEvent(ATU5011_AgentController::emTRGSELDMA00, str.channel);
    }
}//}}}

/// Call back of register
void ATU5011_Func::cb_TRGSELAD_INSELAD (RegCBstr str)
{//{{{
    if (!mATU5011_AgentController->GetResetStatus("presetz") && mATU5011_AgentController->CheckClockPeriod("CLK_LSB")) {
        mATU5011_AgentController->NotifyEvent(ATU5011_AgentController::emTRGSELAD, 0);
    }
}//}}}

/// Call back of register
void ATU5011_Func::cb_TRGSELDMA1_INSELDMA1 (RegCBstr str)
{//{{{
    if (!mATU5011_AgentController->GetResetStatus("presetz") && mATU5011_AgentController->CheckClockPeriod("CLK_LSB")) {
        mATU5011_AgentController->NotifyEvent(ATU5011_AgentController::emTRGSELDMA00, 2 + str.channel);
    }
}//}}}

unsigned int ATU5011_Func::GetPrescalerRatio (const unsigned int ch_id)
{//{{{
    return (unsigned int)(*PSCR[ch_id]);
}//}}} 

unsigned int ATU5011_Func::GetClkEdgeSetting (const bool is_TCLKA)
{//{{{
    unsigned int ret = 0;
    if (is_TCLKA) {
        ret = (unsigned int)(*CBCNT)["CB4EG"];
    } else {
        ret = (unsigned int)(*CBCNT)["CB5EG"];
    }
    return ret;
}//}}}

bool ATU5011_Func::GetClkBus5Setting (void)
{//{{{
    return (bool)(*CBCNT)["CB5SEL"];
}//}}}

bool ATU5011_Func::GetEnableStatus (const unsigned int index)
{//{{{
    return (bool)(((unsigned int)(*ATUENR) >> index) & 0x1);
}//}}}

unsigned int ATU5011_Func::GetRegSelectSetting (const unsigned int type)
{//{{{
    unsigned int ret = 0;
    switch (type) {
        case ATU5011_AgentController::emTRGSELDMA00:
            ret = (unsigned int)(*TRGSELDMA0[0]);
            break;
        case ATU5011_AgentController::emTRGSELDMA01:
            ret = (unsigned int)(*TRGSELDMA0[1]);
            break;
        case ATU5011_AgentController::emTRGSELDMA10:
            ret = (unsigned int)(*TRGSELDMA1[0]);
            break;
        case ATU5011_AgentController::emTRGSELDMA11:
            ret = (unsigned int)(*TRGSELDMA1[1]);
            break;
        default:    // emTRGSELAD:
            ret = (unsigned int)(*TRGSELAD);
            break;
    }
    return ret;
}//}}}

bool ATU5011_Func::GetNCClkSel(void)
{//{{{
    return (bool)(*NCMR)["NCCSEL"];
}//}}}

void ATU5011_Func::SetTrgStatusDMAReg (const bool is_high)
{//{{{ 
    (*TRGSRDMA[0])["OUTSRDMA"] = is_high;
    (*TRGSRDMA[1])["OUTSRDMA"] = is_high;
}//}}}

// vim600: set foldmethod=marker :
