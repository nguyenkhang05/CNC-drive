// ---------------------------------------------------------------------
// $Id: PIC1A_Func.cpp,v 1.2 2014/12/31 06:33:44 dungvannguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "PIC1A_Func.h"
#include "PIC1A_AgentController.h"

///Constructor of PIC1A_Func class
PIC1A_Func::PIC1A_Func (std::string name, PIC1A_AgentController *PIC1A_AgentControllerPtr)
                    :Cpic1a_regif(name, 32), BusSlaveFuncIf()
{//{{{
    assert(PIC1A_AgentControllerPtr != NULL);
    mPIC1A_AgentController = PIC1A_AgentControllerPtr;
    mPIC1A_AgentController->CommandInit(name);
    Cpic1a_regif::set_instance_name(name);
    mModuleName = name;
    
    // Initialize Internal variables
    Initialize();
}//}}}

///Destructor of PIC1A_Func class
PIC1A_Func::~PIC1A_Func (void)
{//{{{
}//}}}

///Reset operation of PIC1A_Func class
void PIC1A_Func::Reset (const bool is_active)
{//{{{
    Cpic1a_regif::EnableReset(is_active);
    
    if (is_active) {
        // Initialize Internal variables
        Initialize();
    }
}//}}}

///Register handler command to pic1a_regif class
void PIC1A_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cpic1a_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Cpic1a_regif class
            msg_return = Cpic1a_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void PIC1A_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFFFFFC;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd(addr, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void PIC1A_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFFFFFC;
    unsigned int addr_temp = addr & 0xFF;
    if ((0x48 <= addr_temp) && (addr_temp <= 0x4F)) {
        // Write to PIC0WNGFn
        if ((*PIC0EN)["PIC0EN0"] == 1) {
            mPIC1A_AgentController->print_msg("warning","Cannot write to read-only registers\n");
            return;
        }
    }
    if (!debug) {
        reg_wr(addr, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Virtual function of register cb_PIC0EN in pic1a_regif class
void PIC1A_Func::cb_PIC0EN_PIC0EN0 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            if (((*PIC0EN)["PIC0EN0"] == 1) && (str.pre_data == 0)) {
                // Clear PIC0WNGFnm flags
                (*PIC0WNGF[str.channel]) = 0;
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0SST in pic1a_regif class
void PIC1A_Func::cb_PIC0SST_SYNCTRG (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            if ((*PIC0SST)["SYNCTRG"] == 1) {
                for (unsigned int i = 0; i < 16; i++) {
                    if ((bool)(((unsigned int)(*PIC0SSER0) >> i) & 0x1)) {
                        mPIC1A_AgentController->TriggerSyncStart((unsigned int)(PIC1A_AgentController::emTAUD0SST0 + i));
                    }
                    if ((bool)(((unsigned int)(*PIC0SSER1) >> i) & 0x1)) {
                        mPIC1A_AgentController->TriggerSyncStart((unsigned int)(PIC1A_AgentController::emTAUD1SST0 + i));
                    }
                    if ((bool)(((unsigned int)(*PIC0SSER2) >> i) & 0x1)) {
                        if (i <= 3) {
                            mPIC1A_AgentController->TriggerSyncStart((unsigned int)(PIC1A_AgentController::emTAUJ0TSST0 + i));
                        } else if (i <= 7) {
                            mPIC1A_AgentController->TriggerSyncStart((unsigned int)(PIC1A_AgentController::emTAUJ1TSST0 + (i - 4)));
                        } else if (i <= 9) {
                            mPIC1A_AgentController->TriggerSyncStart((unsigned int)(PIC1A_AgentController::emTSG0TSST + (i - 8)));
                        } else if (i <= 11) {
                            mPIC1A_AgentController->TriggerSyncStart((unsigned int)(PIC1A_AgentController::emTPB0TSST + (i - 10)));
                        } else {    // 12 <= i <= 15
                            mPIC1A_AgentController->TriggerSyncStart((unsigned int)(PIC1A_AgentController::emENCAT0TSST + (i - 12)));
                        }
                    }
                    if ((bool)(((unsigned int)(*PIC0SSER3) >> i) & 0x1)) {
                        if (i <= 3) {
                            mPIC1A_AgentController->TriggerSyncStart((unsigned int)(PIC1A_AgentController::emOST0TSST + i));
                        }
                    }
                }
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
    (*PIC0SST)["SYNCTRG"] = 0;
}//}}}

/// Virtual function of register PIC0INI0n in pic1a_regif class
void PIC1A_Func::cb_PIC0INI0_PIC0INI00 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < 8; i++) {
                if ((bool)(((unsigned int)(*PIC0INI0[str.channel]) >> i) & 0x1)) {
                    mPIC1A_AgentController->ResetRS(str.channel*8 + i);
                }
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
    (*PIC0INI0[str.channel]) = 0;
}//}}}

/// Virtual function of register PIC0INI1n in pic1a_regif class
void PIC1A_Func::cb_PIC0INI1_PIC0INI100 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < 3; i++) {
                if ((bool)(((unsigned int)(*PIC0INI1[str.channel]) >> i) & 0x1)) {
                    mPIC1A_AgentController->ResetDT(str.channel*3 + i);
                }
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
    (*PIC0INI1[str.channel]) = 0;
}//}}}

/// Virtual function of register PIC0INI2n in pic1a_regif class
void PIC1A_Func::cb_PIC0INI2_PIC0INI20 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < PIC1A_AgentController::emRTOnum; i++) {
                if ((bool)(((unsigned int)(*PIC0INI2[str.channel]) >> i) & 0x1)) {
                    mPIC1A_AgentController->ResetRTO(str.channel, i*2);
                    mPIC1A_AgentController->ResetRTO(str.channel, i*2 + 1);
                }
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
    (*PIC0INI2[str.channel]) = 0;
}//}}}

/// Virtual function of register PIC0WNGMKn in pic1a_regif class
void PIC1A_Func::cb_PIC0WNGMK_WNGMK0 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            if ((*PIC0EN)["PIC0EN0"] == 0) {
                for (unsigned int i = 0; i < PIC1A_AgentController::emWNGnum; i++) {
                    ProcessWNG(str.channel, i, PIC1A_AgentController::emWNGMK);
                }
            } else {
                mPIC1A_AgentController->print_msg("warning","Cannot change PIC0WNGMK%d setting when PIC0EN.PIC0EN0 is 1\n", str.channel);
                (*PIC0WNGMK[str.channel]) = str.pre_data;
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0WNGSn in pic1a_regif class
void PIC1A_Func::cb_PIC0WNGS_PIC0WNGS0 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < PIC1A_AgentController::emWNGnum; i++) {
                ProcessWNG(str.channel, i, PIC1A_AgentController::emWNGS);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0WNGFCLR in pic1a_regif class
void PIC1A_Func::cb_PIC0WNGFCLR_WNGFCLR0 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < PIC1A_AgentController::emWNGnum; i++) {
                if ((bool)(((unsigned int)(*PIC0WNGFCLR[str.channel]) >> i) & 0x1)) {
                    if ((*PIC0EN)["PIC0EN0"] == 1) {
                        // Clear corresponding PIC0WNGFnm
                        (*PIC0WNGF[str.channel]) &= (~((unsigned int)1 << i));
                    }
                }
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
    (*PIC0WNGFCLR[str.channel]) = 0;
}//}}}

/// Virtual function of register PIC0LHSEL in pic1a_regif class
void PIC1A_Func::cb_PIC0LHSEL_PIC0LHSEL1 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < PIC1A_AgentController::emFFnum; i++) {
                CalcMUX_PIC0TSGOUTCTR_Other(str.channel, i);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0TSGOUTCTR in pic1a_regif class
void PIC1A_Func::cb_PIC0TSGOUTCTR_PIC0TSGOUTCTR0 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < PIC1A_AgentController::emFFnum; i++) {
                CalcMUX_PIC0TSGOUTCTR_0(str.channel, i);
                CalcMUX_PIC0TSGOUTCTR_Other(str.channel, i);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0HALLSEL in pic1a_regif class
void PIC1A_Func::cb_PIC0HALLSEL_PIC0HALLSEL0 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < PIC1A_AgentController::emInum; i++) {
                CalcMUX_PIC0HALLSEL(str.channel, i);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0TAUDSEL in pic1a_regif class
void PIC1A_Func::cb_PIC0TAUDSEL_PIC0TAUDIN00 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < PIC1A_AgentController::emInum; i++) {
                if (str.channel == 0) {
                    CalcMUX_PIC0TAUD0SEL(i);
                } else {
                    CalcMUX_PIC0TAUD1SEL(i);
                }
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0HIZCEN in pic1a_regif class
void PIC1A_Func::cb_PIC0HIZCEN_PIC0HIZCEN0 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            CalcMUX_PIC0HIZCEN(str.channel);
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0SST in pic1a_regif class
void PIC1A_Func::cb_PIC0ENCSEL40_PIC0ENCSEL403_0 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            CalcMUX_PIC0ENCSEL40_0003(str.channel);
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG20 in pic1a_regif class
void PIC1A_Func::cb_PIC0REG20_PIC0REG2000 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            if (((*PIC0REG20[str.channel])["PIC0REG2013_12"] == 0x3) ||
            ((*PIC0REG20[str.channel])["PIC0REG2011_10"] == 0x3) ||
            ((*PIC0REG20[str.channel])["PIC0REG2009_08"] == 0x3)) {
                mPIC1A_AgentController->print_msg("warning","The setting value is prohibited.\n");
            }
            
            CalcMUX_PIC0REG20_00(str.channel);
            CalcMUX_PIC0REG20_01(str.channel);
            CalcMUX_PIC0REG20_02(str.channel);
            CalcMUX_PIC0REG20_03(str.channel);
            CalcMUX_PIC0REG20_05(str.channel);
            CalcMUX_PIC0REG20_17(str.channel);
            CalcTRG(str.channel);
            for (unsigned int i = 0; i < PIC1A_AgentController::emInum; i++) {
                CalcMUX_PIC0REG20_28(str.channel, i);
            }
            if (str.channel == 0) {
                CalcMUX_PIC0REG200_04();
                CalcMUX_PIC0REG200_0809();
                CalcMUX_PIC0REG200_1011();
                CalcMUX_PIC0REG200_1213();
                CalcMUX_PIC0REG200_16();
                CalcMUX_PIC0REG200_18();
                CalcMUX_PIC0REG200_2425(PIC1A_AgentController::emTAPATSIM0);
                CalcMUX_PIC0REG200_2425(PIC1A_AgentController::emTAPATUDCM0);
            } else {
                CalcMUX_PIC0REG210_04();
                CalcMUX_PIC0REG210_0809();
                CalcMUX_PIC0REG210_1011();
                CalcMUX_PIC0REG210_1213();
                CalcMUX_PIC0REG210_16();
                CalcMUX_PIC0REG210_18();
                CalcMUX_PIC0REG210_2425(PIC1A_AgentController::emTAPATSIM0);
                CalcMUX_PIC0REG210_2425(PIC1A_AgentController::emTAPATUDCM0);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG21 in pic1a_regif class
void PIC1A_Func::cb_PIC0REG21_PIC0REG2101_00 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            if (((*PIC0REG21[str.channel])["PIC0REG2127_26"] == 0x1) ||
            ((*PIC0REG21[str.channel])["PIC0REG2125_24"] == 0x1) ||
            ((*PIC0REG21[str.channel])["PIC0REG2123_22"] == 0x1) ||
            ((*PIC0REG21[str.channel])["PIC0REG2121_20"] == 0x1) ||
            ((*PIC0REG21[str.channel])["PIC0REG2119_18"] == 0x1) ||
            ((*PIC0REG21[str.channel])["PIC0REG2117_16"] == 0x1) ||
            ((*PIC0REG21[str.channel])["PIC0REG2111_10"] == 0x3) ||
            ((*PIC0REG21[str.channel])["PIC0REG2109_08"] == 0x3) ||
            ((*PIC0REG21[str.channel])["PIC0REG2103_02"] == 0x3) ||
            ((*PIC0REG21[str.channel])["PIC0REG2101_00"] == 0x3)) {
                mPIC1A_AgentController->print_msg("warning","The setting value is prohibited.\n");
            }
            
            if (str.channel == 0) {
                CalcMUX_PIC0REG201_0001();
                CalcMUX_PIC0REG201_0203();
                CalcMUX_PIC0REG201_0405();
                CalcMUX_PIC0REG201_0607();
                CalcMUX_PIC0REG201_0809();
                CalcMUX_PIC0REG201_1011();
            } else {
                CalcMUX_PIC0REG211_0001();
                CalcMUX_PIC0REG211_0203();
                CalcMUX_PIC0REG211_0405();
                CalcMUX_PIC0REG211_0607();
                CalcMUX_PIC0REG211_0809();
                CalcMUX_PIC0REG211_1011();
            }
            for (unsigned int i = 0; i < PIC1A_AgentController::emPFNnum; i++) {
                CalcPFN(str.channel, i, PIC1A_AgentController::emPFNWrReg);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG22 in pic1a_regif class
void PIC1A_Func::cb_PIC0REG22_PIC0REG2200 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            if (((*PIC0REG22[str.channel])["PIC0REG2227_26"] == 0x3) ||
            ((*PIC0REG22[str.channel])["PIC0REG2225_24"] == 0x3) ||
            ((*PIC0REG22[str.channel])["PIC0REG2223_22"] == 0x3) ||
            ((*PIC0REG22[str.channel])["PIC0REG2221_20"] == 0x3) ||
            ((*PIC0REG22[str.channel])["PIC0REG2219_18"] == 0x3) ||
            ((*PIC0REG22[str.channel])["PIC0REG2217_16"] == 0x3)) {
                mPIC1A_AgentController->print_msg("warning","The setting value is prohibited.\n");
            }
            
            if (str.channel == 0) {
                CalcMUX_PIC0REG202_02();
                CalcMUX_PIC0REG202_03();
                CalcMUX_PIC0REG202_04();
                CalcMUX_PIC0REG202_05();
                CalcMUX_PIC0REG202_06();
                CalcMUX_PIC0REG202_07();
                CalcMUX_PIC0REG202_29();
            } else {
                CalcMUX_PIC0REG212_02();
                CalcMUX_PIC0REG212_03();
                CalcMUX_PIC0REG212_04();
                CalcMUX_PIC0REG212_05();
                CalcMUX_PIC0REG212_06();
                CalcMUX_PIC0REG212_07();
                CalcMUX_PIC0REG212_29();
            }
            CalcMUX_PIC0REG22_08(str.channel);
            CalcMUX_PIC0REG22_09(str.channel);
            CalcMUX_PIC0REG22_10(str.channel);
            CalcMUX_PIC0REG22_11(str.channel);
            CalcMUX_PIC0REG22_12(str.channel);
            CalcMUX_PIC0REG22_13(str.channel);
            CalcMUX_PIC0REG22_1617(str.channel);
            CalcMUX_PIC0REG22_1819(str.channel);
            CalcMUX_PIC0REG22_2021(str.channel);
            CalcMUX_PIC0REG22_2223(str.channel);
            CalcMUX_PIC0REG22_2425(str.channel);
            CalcMUX_PIC0REG22_2627(str.channel);
            CalcMUX_PIC0REG22_28(str.channel);
            CalcMUX_PIC0REG22_30(str.channel);
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG23 in pic1a_regif class
void PIC1A_Func::cb_PIC0REG23_PIC0REG2302_00 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < PIC1A_AgentController::emFNnum; i++) {
                CalcFNOutput(str.channel, i);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG24 in pic1a_regif class
void PIC1A_Func::cb_PIC0REG24_PIC0REG2400 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            for (unsigned int i = 0; i < PIC1A_AgentController::emMUX_PIC0REG2n4num; i++) {
                CalcMUX_PIC0REG24(str.channel, i);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG30 in pic1a_regif class
void PIC1A_Func::cb_PIC0REG30_PIC0REG3000 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            if (((*PIC0REG30)["PIC0REG3020_19"] == 0x3) ||
            ((*PIC0REG30)["PIC0REG3017_16"] == 0x3) ||
            (0xB < (*PIC0REG30)["PIC0REG3015_12"]) ||
            (0xB < (*PIC0REG30)["PIC0REG3005_02"])) {
                mPIC1A_AgentController->print_msg("warning","The setting value is prohibited.\n");
            }
            
            CalcMUX_PIC0REG30_00();
            CalcMUX_PIC0REG30_01();
            CalcMUX_PIC0REG30_0205();
            CalcMUX_PIC0REG30_0607();
            CalcMUX_PIC0REG30_0809();
            CalcMUX_PIC0REG30_1011();
            CalcMUX_PIC0REG30_1215();
            CalcMUX_PIC0REG30_18();
            CalcMUX_PIC0REG30_21();
            CalcMUX_PIC0REG30_23();
            CalcMUX_PIC0REG30_24();
            for (unsigned int i = 0; i < PIC1A_AgentController::emGnum; i++) {
                CalcMUX_PIC0REG30_1617(i);
                CalcMUX_PIC0REG30_1920(i);
                CalcMUX_PIC0REG30_22(i);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG31 in pic1a_regif class
void PIC1A_Func::cb_PIC0REG31_PIC0REG3100 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            if (((*PIC0REG31)["PIC0REG3122_21"] == 0x3) ||
            (0x4 < (*PIC0REG31)["PIC0REG3117_15"]) ||
            ((*PIC0REG31)["PIC0REG3113_12"] == 0x3) ||
            (0x4 < (*PIC0REG31)["PIC0REG3108_06"]) ||
            ((*PIC0REG31)["PIC0REG3105_04"] == 0x3) ||
            ((*PIC0REG31)["PIC0REG3102_01"] == 0x3)) {
                mPIC1A_AgentController->print_msg("warning","The setting value is prohibited.\n");
            }
            
            CalcMUX_PIC0REG31_00();
            CalcMUX_PIC0REG31_0102();
            CalcMUX_PIC0REG31_03();
            CalcMUX_PIC0REG31_0405();
            CalcMUX_PIC0REG31_0608();
            CalcMUX_PIC0REG31_0910();
            CalcMUX_PIC0REG31_11();
            CalcMUX_PIC0REG31_1213();
            CalcMUX_PIC0REG31_14();
            CalcMUX_PIC0REG31_1517();
            CalcMUX_PIC0REG31_1819();
            CalcMUX_PIC0REG31_20();
            CalcMUX_PIC0REG31_2122();
            CalcMUX_PIC0REG31_23();
            CalcMUX_PIC0REG31_24();
            CalcMUX_PIC0REG31_25();
            CalcMUX_PIC0REG31_26();
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG32 in pic1a_regif class
void PIC1A_Func::cb_PIC0REG32_PIC0REG3200 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            CalcMUX_PIC0REG32_00_08(PIC1A_AgentController::emENCAT2AIN);
            CalcMUX_PIC0REG32_00_08(PIC1A_AgentController::emENCAT3AIN);
            CalcMUX_PIC0REG32_01_09(PIC1A_AgentController::emENCAT2BIN);
            CalcMUX_PIC0REG32_01_09(PIC1A_AgentController::emENCAT3BIN);
            CalcMUX_PIC0REG32_0203();
            CalcMUX_PIC0REG32_04();
            CalcMUX_PIC0REG32_05();
            CalcMUX_PIC0REG32_1011();
            CalcMUX_PIC0REG32_12();
            CalcMUX_PIC0REG32_13();
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG33 in pic1a_regif class
void PIC1A_Func::cb_PIC0REG33_PIC0REG3300 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            CalcMUX_PIC0REG33(str.channel);
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Virtual function of register PIC0REG5n in pic1a_regif class
void PIC1A_Func::cb_PIC0REG5_PIC0REG500 (RegCBstr str)
{//{{{
    if (!mPIC1A_AgentController->GetResetStatus()) {
        if (mPIC1A_AgentController->CheckClockPeriod("PCLK")) {
            if ((*PIC0REG5[str.channel])["PIC0REG502_01"] == 0x3) {
                mPIC1A_AgentController->print_msg("warning","The setting value is prohibited.\n");
            }
            
            if (str.channel == 0) {
                CalcMUX_PIC0REG50_0102();
            } else {
                CalcMUX_PIC0REG51_0102();
            }
            CalcMUX_PIC0REG5_04(str.channel);
            CalcMUX_PIC0REG5_0506(str.channel);
            CalcMUX_PIC0REG5_07(str.channel);
            CalcMUX_PIC0REG5_08(str.channel);
            CalcMUX_PIC0REG5_10(str.channel);
            for (unsigned int i = 0; i < PIC1A_AgentController::emInum; i++) {
                CalcMUX_PIC0REG5_00(str.channel, i);
                CalcMUX_PIC0REG5_12(str.channel, i);
            }
            for (unsigned int i = 0; i < 6; i++) {
                CalcMUX_PIC0REG5_03(str.channel, i);
                CalcMUX_PIC0REG5_13(str.channel, i);
                CalcMUX_PIC0REG5_14(str.channel, i);
            }
        } else {
            mPIC1A_AgentController->print_msg("warning","PIC model does not operate when PCLK is invalid\n");
        }
    } else {
        mPIC1A_AgentController->print_msg("warning","PIC model does not operate when reset is active\n");
    }
}//}}}

/// Calculate output value of MUX(PIC0TAUD0SEL)
void PIC1A_Func::CalcMUX_PIC0TAUD0SEL (const unsigned int index)
{//{{{
    unsigned int temp = index / 2;
    unsigned int port_index = 2 * temp;
    unsigned int bit_index = 2 * port_index;
    bool out_val = false;
    
    switch (((unsigned int)(*PIC0TAUDSEL[0]) >> bit_index) & 0x3) {
        case 1:
            out_val = mMUX_PIC0REG24Output[0][port_index + 1];
            break;
        case 2:
            out_val = mMUX_PIC0REG24Output[1][port_index];
            break;
        case 3:
            out_val = mMUX_PIC0REG24Output[1][port_index + 1];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG24Output[0][port_index];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTAUD0TIN0 + port_index, out_val);
    
    switch (((unsigned int)(*PIC0TAUDSEL[0]) >> (bit_index + 2)) & 0x3) {
        case 1:
            out_val = mMUX_PIC0REG24Output[0][port_index];
            break;
        case 2:
            out_val = mMUX_PIC0REG24Output[1][port_index + 1];
            break;
        case 3:
            out_val = mMUX_PIC0REG24Output[1][port_index];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG24Output[0][port_index + 1];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTAUD0TIN0 + (port_index + 1), out_val);
}//}}}

/// Calculate output value of MUX(PIC0TAUD1SEL)
void PIC1A_Func::CalcMUX_PIC0TAUD1SEL (const unsigned int index)
{//{{{
    unsigned int temp = index / 2;
    unsigned int port_index = 2 * temp;
    unsigned int bit_index = 2 * port_index;
    bool out_val = false;
    
    switch (((unsigned int)(*PIC0TAUDSEL[1]) >> bit_index) & 0x3) {
        case 1:
            out_val = mMUX_PIC0REG24Output[1][port_index + 1];
            break;
        case 2:
            out_val = mMUX_PIC0REG24Output[0][port_index];
            break;
        case 3:
            out_val = mMUX_PIC0REG24Output[0][port_index + 1];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG24Output[1][port_index];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTAUD1TIN0 + port_index, out_val);
    
    switch (((unsigned int)(*PIC0TAUDSEL[1]) >> (bit_index + 2)) & 0x3) {
        case 1:
            out_val = mMUX_PIC0REG24Output[1][port_index];
            break;
        case 2:
            out_val = mMUX_PIC0REG24Output[0][port_index + 1];
            break;
        case 3:
            out_val = mMUX_PIC0REG24Output[0][port_index];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG24Output[1][port_index + 1];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTAUD1TIN0 + (port_index + 1), out_val);
}//}}}

/// Calculate output value of MUX(PIC0TSGOUTCTR[0])
void PIC1A_Func::CalcMUX_PIC0TSGOUTCTR_0 (const unsigned int module, const unsigned int index)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0TSGOUTCTR[module])["PIC0TSGOUTCTR0"]) {
        case 1:
            out_val = mTSGOUTCTR_FFOutput[module][index];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG5_14Output[module][index];
            break;
    }
    // Update output port
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP2U + index, out_val);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP3U + index, out_val);
    }
}//}}}

/// Calculate output value of MUX(PIC0TSGOUTCTR[1-6])
void PIC1A_Func::CalcMUX_PIC0TSGOUTCTR_Other (const unsigned int module, const unsigned int index)
{//{{{
    switch (((unsigned int)(*PIC0TSGOUTCTR[module]) >> (index + 1)) & 0x1) {
        case 1:
            mTSGOUTCTR_FFInput[module][index] = (bool)(((unsigned int)(*PIC0LHSEL[module]) >> (index + 1)) & 0x1);
            break;
        default:    // 0
            mTSGOUTCTR_FFInput[module][index] = mMUX_PIC0REG5_14Output[module][index];
            break;
    }
    mPIC1A_AgentController->TSGOUTCTR_FFChange(module, index);
}//}}}

/// Calculate output value of MUX(PIC0ENCSEL4n0[0-3])
void PIC1A_Func::CalcMUX_PIC0ENCSEL40_0003 (const unsigned int module)
{//{{{
    unsigned int index = (unsigned int)(*PIC0ENCSEL40[module])["PIC0ENCSEL403_0"];
    if (module == 0) {
        mMUX_PIC0ENCSEL40_0003Output[module] = mInputValue[PIC1A_AgentController::emTAUD0INT0 + index];
    } else {
        mMUX_PIC0ENCSEL40_0003Output[module] = mInputValue[PIC1A_AgentController::emTAUD1INT0 + index];
    }
    CalcMUX_PIC0ENCSEL40_07(module);
}//}}}

/// Calculate output value of MUX(PIC0ENCSEL4n0[7])
void PIC1A_Func::CalcMUX_PIC0ENCSEL40_07 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0ENCSEL40[module])["PIC0ENCSEL407"]) {
        case 1:
            mMUX_PIC0ENCSEL40_07Output[module] = mMUX_PIC0ENCSEL40_0003Output[module];
            break;
        default:    // 0
            mMUX_PIC0ENCSEL40_07Output[module] = false;
            break;
    }
    if (module == 0) {
        CalcMUX_PIC0REG30_18();
        CalcMUX_PIC0REG32_04();
    } else {
        CalcMUX_PIC0REG30_21();
        CalcMUX_PIC0REG32_12();
    }
}//}}}

/// Calculate output value of MUX(PIC0HIZCENn)
void PIC1A_Func::CalcMUX_PIC0HIZCEN (const unsigned int index)
{//{{{
    bool and_0 = (bool)(*PIC0HIZCEN[index])["PIC0HIZCEN0"] && mInputValue[PIC1A_AgentController::emANFESO0 + index];
    bool and_1 = (bool)(*PIC0HIZCEN[index])["PIC0HIZCEN1"] && (mInputValue[PIC1A_AgentController::emRSVHIZIN0] ||
                                                               mInputValue[PIC1A_AgentController::emRSVHIZIN1] ||
                                                               mInputValue[PIC1A_AgentController::emRSVHIZIN2]);
    bool and_2 = (bool)(*PIC0HIZCEN[index])["PIC0HIZCEN2"] && mInputValue[PIC1A_AgentController::emRSVHIZIN3];
    bool and_3 = (bool)(*PIC0HIZCEN[index])["PIC0HIZCEN3"] && mInputValue[PIC1A_AgentController::emTSG0TSTIER];
    bool and_4 = (bool)(*PIC0HIZCEN[index])["PIC0HIZCEN4"] && mInputValue[PIC1A_AgentController::emTSG1TSTIER];
    bool and_5 = (bool)(*PIC0HIZCEN[index])["PIC0HIZCEN5"] && (!mInputValue[PIC1A_AgentController::emERROROUTZ]);
    bool and_6 = (bool)(*PIC0HIZCEN[index])["PIC0HIZCEN6"] && mInputValue[PIC1A_AgentController::emRSVHIZIN4];
    bool and_7 = (bool)(*PIC0HIZCEN[index])["PIC0HIZCEN7"] && mInputValue[PIC1A_AgentController::emRSVHIZIN5];
    
    bool out_val = and_0 || and_1 || and_2 || and_3 || and_4 || and_5 || and_6 || and_7;
    switch (index) {
        case 1:
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP1TAPATHASIN, out_val);
            break;
        case 2:
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP2TAPATHASIN, out_val);
            break;
        case 3:
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP3TAPATHASIN, out_val);
            break;
        default:    // 0
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP0TAPATHASIN, out_val);
            break;
    }
}//}}}
/// Calculate output value of MUX(PIC0HALLSEL)
void PIC1A_Func::CalcMUX_PIC0HALLSEL (const unsigned int module, const unsigned int index)
{//{{{
    bool bit_value = (bool)(((unsigned int)(*PIC0HALLSEL) >> module) & 0x1);
    bool out_val = false;
    
    if (bit_value) {
        out_val = mMUX_PIC0REG5_00Output[module][index];
    } else {
        if ( module == 0) {
            out_val = mInputValue[PIC1A_AgentController::emPORTTSG0TSTAPT0 + index];
        } else {
            out_val = mInputValue[PIC1A_AgentController::emPORTTSG1TSTAPT0 + index];
        }
    }
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTAPT0 + index, out_val);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG1TSTAPT0 + index, out_val);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG2n0[0])
void PIC1A_Func::CalcMUX_PIC0REG20_00 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG20[module])["PIC0REG2000"]) {
        case 1:
            mMUX_PIC0REG20_00Output[module] = mMUX_PIC0REG20_0809Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG20_00Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN4];
            } else {
                mMUX_PIC0REG20_00Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN4];
            }
            break;
    }
    CalcMUX_PIC0REG20_28(module, PIC1A_AgentController::emI4);
}//}}}

/// Calculate output value of MUX(PIC0REG2n0[1])
void PIC1A_Func::CalcMUX_PIC0REG20_01 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG20[module])["PIC0REG2001"]) {
        case 1:
            mMUX_PIC0REG20_01Output[module] = mMUX_PIC0REG20_0809Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG20_01Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN5];
            } else {
                mMUX_PIC0REG20_01Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN5];
            }
            break;
    }
    CalcMUX_PIC0REG20_28(module, PIC1A_AgentController::emI5);
}//}}}

/// Calculate output value of MUX(PIC0REG2n0[2])
void PIC1A_Func::CalcMUX_PIC0REG20_02 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG20[module])["PIC0REG2002"]) {
        case 1:
            mMUX_PIC0REG20_02Output[module] = mMUX_PIC0REG20_1011Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG20_02Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN6];
            } else {
                mMUX_PIC0REG20_02Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN6];
            }
            break;
    }
    CalcMUX_PIC0REG20_28(module, PIC1A_AgentController::emI6);
    CalcMUX_PIC0REG22_28(module);
}//}}}

/// Calculate output value of MUX(PIC0REG2n0[3])
void PIC1A_Func::CalcMUX_PIC0REG20_03 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG20[module])["PIC0REG2003"]) {
        case 1:
            mMUX_PIC0REG20_03Output[module] = mMUX_PIC0REG20_1011Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG20_03Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN7];
            } else {
                mMUX_PIC0REG20_03Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN7];
            }
            break;
    }
    CalcMUX_PIC0REG22_28(module);
}//}}}

/// Calculate output value of MUX(PIC0REG200[4])
void PIC1A_Func::CalcMUX_PIC0REG200_04 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[0])["PIC0REG2004"]) {
        case 1:
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT08] = mMUX_PIC0REG20_1213Output[0];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT08] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN8];
            break;
    }
    CalcMUX_PIC0REG202_29();
    CalcMUX_PIC0REG24(0, PIC1A_AgentController::emT08);
}//}}}

/// Calculate output value of MUX(PIC0REG210[4])
void PIC1A_Func::CalcMUX_PIC0REG210_04 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[1])["PIC0REG2004"]) {
        case 1:
            mMUX_PIC0REG210_04Output = mMUX_PIC0REG20_1213Output[1];
            break;
        default:    // 0
            mMUX_PIC0REG210_04Output = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN8];
            break;
    }
    CalcMUX_PIC0REG212_29();
    CalcMUX_PIC0REG31_24();
}//}}}

/// Calculate output value of MUX(PIC0REG2n0[5])
void PIC1A_Func::CalcMUX_PIC0REG20_05 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG20[module])["PIC0REG2005"]) {
        case 1:
            mMUX_PIC0REG20_05Output[module] = mMUX_PIC0REG20_1213Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG20_05Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN9];
            } else {
                mMUX_PIC0REG20_05Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN9];
            }
            break;
    }
    if (module == 0) {
        CalcMUX_PIC0REG202_29();
    } else {
        CalcMUX_PIC0REG212_29();
    }
}//}}}

/// Calculate output value of MUX(PIC0REG200[8-9])
void PIC1A_Func::CalcMUX_PIC0REG200_0809 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[0])["PIC0REG2009_08"]) {
        case 1:
            mMUX_PIC0REG20_0809Output[0] = mMUX_PIC0REG5_00Output[0][PIC1A_AgentController::emI1];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG20_0809Output[0] = mInputValue[PIC1A_AgentController::emTSG0TSTPTE];
            break;
        default:    // 0
            mMUX_PIC0REG20_0809Output[0] = mInputValue[PIC1A_AgentController::emDNFENCAT0AIN];
            break;
    }
    CalcMUX_PIC0REG20_00(0);
    CalcMUX_PIC0REG20_01(0);
}//}}}

/// Calculate output value of MUX(PIC0REG210[8-9])
void PIC1A_Func::CalcMUX_PIC0REG210_0809 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[1])["PIC0REG2009_08"]) {
        case 1:
            mMUX_PIC0REG20_0809Output[1] = mMUX_PIC0REG5_00Output[1][PIC1A_AgentController::emI1];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG20_0809Output[1] = mInputValue[PIC1A_AgentController::emTSG1TSTPTE];
            break;
        default:    // 0
            mMUX_PIC0REG20_0809Output[1] = mInputValue[PIC1A_AgentController::emDNFENCAT1AIN];
            break;
    }
    CalcMUX_PIC0REG20_00(1);
    CalcMUX_PIC0REG20_01(1);
}//}}}

/// Calculate output value of MUX(PIC0REG200[10-11])
void PIC1A_Func::CalcMUX_PIC0REG200_1011 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[0])["PIC0REG2011_10"]) {
        case 1:
            mMUX_PIC0REG20_1011Output[0] = mMUX_PIC0REG5_00Output[0][PIC1A_AgentController::emI2];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG20_1011Output[0] = mInputValue[PIC1A_AgentController::emTSG0TSTPTE];
            break;
        default:    // 0
            mMUX_PIC0REG20_1011Output[0] = mInputValue[PIC1A_AgentController::emDNFENCAT0BIN];
            break;
    }
    CalcMUX_PIC0REG20_02(0);
    CalcMUX_PIC0REG20_03(0);
}//}}}

/// Calculate output value of MUX(PIC0REG210[10-11])
void PIC1A_Func::CalcMUX_PIC0REG210_1011 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[1])["PIC0REG2011_10"]) {
        case 1:
            mMUX_PIC0REG20_1011Output[1] = mMUX_PIC0REG5_00Output[1][PIC1A_AgentController::emI2];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG20_1011Output[1] = mInputValue[PIC1A_AgentController::emTSG1TSTPTE];
            break;
        default:    // 0
            mMUX_PIC0REG20_1011Output[1] = mInputValue[PIC1A_AgentController::emDNFENCAT1BIN];
            break;
    }
    CalcMUX_PIC0REG20_02(1);
    CalcMUX_PIC0REG20_03(1);
}//}}}

/// Calculate output value of MUX(PIC0REG200[12-13])
void PIC1A_Func::CalcMUX_PIC0REG200_1213 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[0])["PIC0REG2013_12"]) {
        case 1:
            mMUX_PIC0REG20_1213Output[0] = mMUX_PIC0REG5_00Output[0][PIC1A_AgentController::emI3];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG20_1213Output[0] = mInputValue[PIC1A_AgentController::emTSG0TSTPTE];
            break;
        default:    // 0
            mMUX_PIC0REG20_1213Output[0] = mInputValue[PIC1A_AgentController::emDNFENCAT0ZIN];
            break;
    }
    CalcMUX_PIC0REG200_04();
    CalcMUX_PIC0REG20_05(0);
}//}}}

/// Calculate output value of MUX(PIC0REG210[12-13])
void PIC1A_Func::CalcMUX_PIC0REG210_1213 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[1])["PIC0REG2013_12"]) {
        case 1:
            mMUX_PIC0REG20_1213Output[1] = mMUX_PIC0REG5_00Output[1][PIC1A_AgentController::emI3];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG20_1213Output[1] = mInputValue[PIC1A_AgentController::emTSG1TSTPTE];
            break;
        default:    // 0
            mMUX_PIC0REG20_1213Output[1] = mInputValue[PIC1A_AgentController::emDNFENCAT1ZIN];
            break;
    }
    CalcMUX_PIC0REG210_04();
    CalcMUX_PIC0REG20_05(1);
}//}}}

/// Calculate output value of MUX(PIC0REG200[16])
void PIC1A_Func::CalcMUX_PIC0REG200_16 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[0])["PIC0REG2016"]) {
        case 1:
            mMUX_PIC0REG20_16Output[0] = mTRG_Output[1];
            break;
        default:    // 0
            mMUX_PIC0REG20_16Output[0] = mTRG_Output[0];
            break;
    }
    CalcMUX_PIC0REG20_17(0);
    CalcMUX_PIC0REG31_0910();
}//}}}

/// Calculate output value of MUX(PIC0REG210[16])
void PIC1A_Func::CalcMUX_PIC0REG210_16 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG20[1])["PIC0REG2016"]) {
        case 1:
            mMUX_PIC0REG20_16Output[1] = mTRG_Output[0];
            break;
        default:    // 0
            mMUX_PIC0REG20_16Output[1] = mTRG_Output[1];
            break;
    }
    CalcMUX_PIC0REG20_17(1);
    CalcMUX_PIC0REG31_1819();
}//}}}

/// Calculate output value of MUX(PIC0REG2n0[17])
void PIC1A_Func::CalcMUX_PIC0REG20_17 (const unsigned int module)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG20[module])["PIC0REG2017"]) {
        case 1:
            out_val = mMUX_PIC0REG20_16Output[module];
            break;
        default:    // 0
            if (module == 0) {
                out_val = mInputValue[PIC1A_AgentController::emTAUD0INT1];
            } else {
                out_val = mInputValue[PIC1A_AgentController::emTAUD1INT1];
            }
            break;
    }
    
    for (unsigned int i = 0; i < PIC1A_AgentController::emRTOnum; i++) {
        if ((!out_val) && (mRTO_TRGInput[module][i])) {
            CalcRTO(module, i, PIC1A_AgentController::emRTOTrigger);
        }
        mRTO_TRGInput[module][i] = out_val;
    }
}//}}}

/// Calculate output value of MUX(PIC0REG200[18])
void PIC1A_Func::CalcMUX_PIC0REG200_18 (void)
{//{{{  
    switch ((unsigned int)(*PIC0REG20[0])["PIC0REG2018"]) {
        case 1:
            mMUX_PIC0REG20_18Output[0] = mInputValue[PIC1A_AgentController::emTAUD0TOUT2];
            break;
        default:    // 0
            mMUX_PIC0REG20_18Output[0] = mInputValue[PIC1A_AgentController::emTAUD0UDC2];
            break;
    }
    CalcMUX_PIC0REG22_1617(0);
    CalcMUX_PIC0REG22_2021(0);
    CalcMUX_PIC0REG22_2425(0);
}//}}}

/// Calculate output value of MUX(PIC0REG210[18])
void PIC1A_Func::CalcMUX_PIC0REG210_18 (void)
{//{{{  
    switch ((unsigned int)(*PIC0REG20[1])["PIC0REG2018"]) {
        case 1:
            mMUX_PIC0REG20_18Output[1] = mInputValue[PIC1A_AgentController::emTAUD1TOUT2];
            break;
        default:    // 0
            mMUX_PIC0REG20_18Output[1] = mInputValue[PIC1A_AgentController::emTAUD1UDC2];
            break;
    }
    CalcMUX_PIC0REG22_1617(1);
    CalcMUX_PIC0REG22_2021(1);
    CalcMUX_PIC0REG22_2425(1);
}//}}}

/// Calculate output value of MUX(PIC0REG200[24-25])
void PIC1A_Func::CalcMUX_PIC0REG200_2425 (const unsigned int type)
{//{{{
    bool out_val = false;
    unsigned reg_val = (unsigned int)(*PIC0REG20[0])["PIC0REG2025_24"];
    switch (type) {
        case PIC1A_AgentController::emTAPATSIM0:
            switch (reg_val) {
                case 1:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD0INT0];
                    break;
                case 2:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD0INT2];
                    break;
                case 3:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD0INT8];
                    break;
                default:    // 0
                    out_val = false;
                    break;
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP0TAPATSIM0, out_val);
            break;
        default:    // emTAPATUDCM0
            switch (reg_val) {
                case 1:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD0UDC0];
                    break;
                case 2:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD0UDC2];
                    break;
                case 3:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD0UDC8];
                    break;
                default:    // 0
                    out_val = false;
                    break;
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP0TAPATUDCM0, out_val);
            break;
    }
}//}}}

/// Calculate output value of MUX(PIC0REG210[24-25])
void PIC1A_Func::CalcMUX_PIC0REG210_2425 (const unsigned int type)
{//{{{
    bool out_val = false;
    unsigned reg_val = (unsigned int)(*PIC0REG20[1])["PIC0REG2025_24"];
    switch (type) {
        case PIC1A_AgentController::emTAPATSIM0:
            switch (reg_val) {
                case 1:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD1INT0];
                    break;
                case 2:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD1INT2];
                    break;
                case 3:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD1INT8];
                    break;
                default:    // 0
                    out_val = false;
                    break;
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP1TAPATSIM0, out_val);
            break;
        default:    // emTAPATUDCM0
            switch (reg_val) {
                case 1:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD1UDC0];
                    break;
                case 2:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD1UDC2];
                    break;
                case 3:
                    out_val = mInputValue[PIC1A_AgentController::emTAUD1UDC8];
                    break;
                default:    // 0
                    out_val = false;
                    break;
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP1TAPATUDCM0, out_val);
            break;
    }
}//}}}

/// Calculate output value of MUX(PIC0REG2n0[28])
void PIC1A_Func::CalcMUX_PIC0REG20_28 (const unsigned int module, const unsigned int index)
{//{{{
    bool bit_value = (bool)(*PIC0REG20[module])["PIC0REG2028"];
    switch (index) {
        case PIC1A_AgentController::emI4:
            if (bit_value) {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT04] = mMUX_PIC0REG5_12Output[module][index];
            } else {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT04] = mMUX_PIC0REG20_00Output[module];
            }
            CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT04);
            break;
        case PIC1A_AgentController::emI5:
            if (bit_value) {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT05] = mMUX_PIC0REG5_12Output[module][index];
            } else {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT05] = mMUX_PIC0REG20_01Output[module];
            }
            CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT05);
            break;
        default:    // PIC1A_AgentController::emI6
            if (bit_value) {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT06] = mMUX_PIC0REG5_12Output[module][index];
            } else {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT06] = mMUX_PIC0REG20_02Output[module];
            }
            CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT06);
            break;
    }
}//}}}

/// Calculate output value of MUX(PIC0REG201[0-1])
void PIC1A_Func::CalcMUX_PIC0REG201_0001 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[0])["PIC0REG2101_00"]) {
        case 1:
            mRTO_INInput[0][PIC1A_AgentController::emRTO0] = mInputValue[PIC1A_AgentController::emTAUD0TOUT2];
            break;
        case 2:
        case 3:
            mRTO_INInput[0][PIC1A_AgentController::emRTO0] = mRS_Output[PIC1A_AgentController::emRS12];
            break;
        default:    // 0
            mRTO_INInput[0][PIC1A_AgentController::emRTO0] = mInputValue[PIC1A_AgentController::emTAUD0TOUT4];
            break;
    }
    CalcRTO(0, PIC1A_AgentController::emRTO0, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG211[0-1])
void PIC1A_Func::CalcMUX_PIC0REG211_0001 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[1])["PIC0REG2101_00"]) {
        case 1:
            mRTO_INInput[1][PIC1A_AgentController::emRTO0] = mInputValue[PIC1A_AgentController::emTAUD1TOUT2];
            break;
        case 2:
        case 3:
            mRTO_INInput[1][PIC1A_AgentController::emRTO0] = mRS_Output[PIC1A_AgentController::emRS02];
            break;
        default:    // 0
            mRTO_INInput[1][PIC1A_AgentController::emRTO0] = mInputValue[PIC1A_AgentController::emTAUD1TOUT4];
            break;
    }
    CalcRTO(1, PIC1A_AgentController::emRTO0, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG201[2-3])
void PIC1A_Func::CalcMUX_PIC0REG201_0203 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[0])["PIC0REG2103_02"]) {
        case 1:
            mRTO_INInput[0][PIC1A_AgentController::emRTO1] = mInputValue[PIC1A_AgentController::emTAUD0TOUT3];
            break;
        case 2:
        case 3:
            mRTO_INInput[0][PIC1A_AgentController::emRTO1] = mRS_Output[PIC1A_AgentController::emRS13];
            break;
        default:    // 0
            mRTO_INInput[0][PIC1A_AgentController::emRTO1] = mInputValue[PIC1A_AgentController::emTAUD0TOUT5];
            break;
    }
    CalcRTO(0, PIC1A_AgentController::emRTO1, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG211[2-3])
void PIC1A_Func::CalcMUX_PIC0REG211_0203 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[1])["PIC0REG2103_02"]) {
        case 1:
            mRTO_INInput[1][PIC1A_AgentController::emRTO1] = mInputValue[PIC1A_AgentController::emTAUD1TOUT3];
            break;
        case 2:
        case 3:
            mRTO_INInput[1][PIC1A_AgentController::emRTO1] = mRS_Output[PIC1A_AgentController::emRS03];
            break;
        default:    // 0
            mRTO_INInput[1][PIC1A_AgentController::emRTO1] = mInputValue[PIC1A_AgentController::emTAUD1TOUT5];
            break;
    }
    CalcRTO(1, PIC1A_AgentController::emRTO1, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG201[4-5])
void PIC1A_Func::CalcMUX_PIC0REG201_0405 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[0])["PIC0REG2105_04"]) {
        case 1:
            mRTO_INInput[0][PIC1A_AgentController::emRTO2] = mInputValue[PIC1A_AgentController::emTAUD0TOUT2];
            break;
        case 2:
            mRTO_INInput[0][PIC1A_AgentController::emRTO2] = mRS_Output[PIC1A_AgentController::emRS16];
            break;
        case 3:
            mRTO_INInput[0][PIC1A_AgentController::emRTO2] = mRS_Output[PIC1A_AgentController::emRS01];
            break;
        default:    // 0
            mRTO_INInput[0][PIC1A_AgentController::emRTO2] = mInputValue[PIC1A_AgentController::emTAUD0TOUT6];
            break;
    }
    CalcRTO(0, PIC1A_AgentController::emRTO2, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG211[4-5])
void PIC1A_Func::CalcMUX_PIC0REG211_0405 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[1])["PIC0REG2105_04"]) {
        case 1:
            mRTO_INInput[1][PIC1A_AgentController::emRTO2] = mInputValue[PIC1A_AgentController::emTAUD1TOUT2];
            break;
        case 2:
            mRTO_INInput[1][PIC1A_AgentController::emRTO2] = mRS_Output[PIC1A_AgentController::emRS06];
            break;
        case 3:
            mRTO_INInput[1][PIC1A_AgentController::emRTO2] = mRS_Output[PIC1A_AgentController::emRS11];
            break;
        default:    // 0
            mRTO_INInput[1][PIC1A_AgentController::emRTO2] = mInputValue[PIC1A_AgentController::emTAUD1TOUT6];
            break;
    }
    CalcRTO(1, PIC1A_AgentController::emRTO2, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG201[6-7])
void PIC1A_Func::CalcMUX_PIC0REG201_0607 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[0])["PIC0REG2107_06"]) {
        case 1:
            mRTO_INInput[0][PIC1A_AgentController::emRTO3] = mInputValue[PIC1A_AgentController::emTAUD0TOUT3];
            break;
        case 2:
            mRTO_INInput[0][PIC1A_AgentController::emRTO3] = mRS_Output[PIC1A_AgentController::emRS17];
            break;
        case 3:
            mRTO_INInput[0][PIC1A_AgentController::emRTO3] = mRS_Output[PIC1A_AgentController::emRS02];
            break;
        default:    // 0
            mRTO_INInput[0][PIC1A_AgentController::emRTO3] = mInputValue[PIC1A_AgentController::emTAUD0TOUT7];
            break;
    }
    CalcRTO(0, PIC1A_AgentController::emRTO3, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG211[6-7])
void PIC1A_Func::CalcMUX_PIC0REG211_0607 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[1])["PIC0REG2107_06"]) {
        case 1:
            mRTO_INInput[1][PIC1A_AgentController::emRTO3] = mInputValue[PIC1A_AgentController::emTAUD1TOUT3];
            break;
        case 2:
            mRTO_INInput[1][PIC1A_AgentController::emRTO3] = mRS_Output[PIC1A_AgentController::emRS07];
            break;
        case 3:
            mRTO_INInput[1][PIC1A_AgentController::emRTO3] = mRS_Output[PIC1A_AgentController::emRS12];
            break;
        default:    // 0
            mRTO_INInput[1][PIC1A_AgentController::emRTO3] = mInputValue[PIC1A_AgentController::emTAUD1TOUT7];
            break;
    }
    CalcRTO(1, PIC1A_AgentController::emRTO3, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG201[8-9])
void PIC1A_Func::CalcMUX_PIC0REG201_0809 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[0])["PIC0REG2109_08"]) {
        case 1:
            mRTO_INInput[0][PIC1A_AgentController::emRTO4] = mInputValue[PIC1A_AgentController::emTAUD0TOUT2];
            break;
        case 2:
        case 3:
            mRTO_INInput[0][PIC1A_AgentController::emRTO4] = mRS_Output[PIC1A_AgentController::emRS03];
            break;
        default:    // 0
            mRTO_INInput[0][PIC1A_AgentController::emRTO4] = mInputValue[PIC1A_AgentController::emTAUD0TOUT8];
            break;
    }
    CalcRTO(0, PIC1A_AgentController::emRTO4, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG211[8-9])
void PIC1A_Func::CalcMUX_PIC0REG211_0809 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[1])["PIC0REG2109_08"]) {
        case 1:
            mRTO_INInput[1][PIC1A_AgentController::emRTO4] = mInputValue[PIC1A_AgentController::emTAUD1TOUT2];
            break;
        case 2:
        case 3:
            mRTO_INInput[1][PIC1A_AgentController::emRTO4] = mRS_Output[PIC1A_AgentController::emRS13];
            break;
        default:    // 0
            mRTO_INInput[1][PIC1A_AgentController::emRTO4] = mInputValue[PIC1A_AgentController::emTAUD1TOUT8];
            break;
    }
    CalcRTO(1, PIC1A_AgentController::emRTO4, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG201[10-11])
void PIC1A_Func::CalcMUX_PIC0REG201_1011 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[0])["PIC0REG2111_10"]) {
        case 1:
            mRTO_INInput[0][PIC1A_AgentController::emRTO5] = mInputValue[PIC1A_AgentController::emTAUD0TOUT3];
            break;
        case 2:
        case 3:
            mRTO_INInput[0][PIC1A_AgentController::emRTO5] = mRS_Output[PIC1A_AgentController::emRS04];
            break;
        default:    // 0
            mRTO_INInput[0][PIC1A_AgentController::emRTO5] = mInputValue[PIC1A_AgentController::emTAUD0TOUT9];
            break;
    }
    CalcRTO(0, PIC1A_AgentController::emRTO5, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG211[10-11])
void PIC1A_Func::CalcMUX_PIC0REG211_1011 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG21[1])["PIC0REG2111_10"]) {
        case 1:
            mRTO_INInput[1][PIC1A_AgentController::emRTO5] = mInputValue[PIC1A_AgentController::emTAUD1TOUT3];
            break;
        case 2:
        case 3:
            mRTO_INInput[1][PIC1A_AgentController::emRTO5] = mRS_Output[PIC1A_AgentController::emRS14];
            break;
        default:    // 0
            mRTO_INInput[1][PIC1A_AgentController::emRTO5] = mInputValue[PIC1A_AgentController::emTAUD1TOUT9];
            break;
    }
    CalcRTO(1, PIC1A_AgentController::emRTO5, PIC1A_AgentController::emRTOInputChange);
}//}}}

/// Calculate output value of MUX(PIC0REG202[2])
void PIC1A_Func::CalcMUX_PIC0REG202_02 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[0])["PIC0REG2202"]) {
        case 1:
            mMUX_PIC0REG22_02Output[0] = mRS_Output[PIC1A_AgentController::emRS02];
            break;
        default:    // 0
            mMUX_PIC0REG22_02Output[0] = mInputValue[PIC1A_AgentController::emTAUD0TOUT5];
            break;
    }
    CalcMUX_PIC0REG22_1819(0);
    UpdateFNInput(0, PIC1A_AgentController::emFN1, false, mMUX_PIC0REG22_02Output[0]);
}//}}}

/// Calculate output value of MUX(PIC0REG212[2])
void PIC1A_Func::CalcMUX_PIC0REG212_02 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[1])["PIC0REG2202"]) {
        case 1:
            mMUX_PIC0REG22_02Output[1] = mRS_Output[PIC1A_AgentController::emRS12];
            break;
        default:    // 0
            mMUX_PIC0REG22_02Output[1] = mInputValue[PIC1A_AgentController::emTAUD1TOUT5];
            break;
    }
    CalcMUX_PIC0REG22_1819(1);
    UpdateFNInput(1, PIC1A_AgentController::emFN1, false, mMUX_PIC0REG22_02Output[1]);
}//}}}

/// Calculate output value of MUX(PIC0REG202[3])
void PIC1A_Func::CalcMUX_PIC0REG202_03 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[0])["PIC0REG2203"]) {
        case 1:
            mMUX_PIC0REG22_03Output[0] = mRS_Output[PIC1A_AgentController::emRS03];
            break;
        default:    // 0
            mMUX_PIC0REG22_03Output[0] = mInputValue[PIC1A_AgentController::emTAUD0TOUT7];
            break;
    }
    CalcMUX_PIC0REG22_2223(0);
    UpdateFNInput(0, PIC1A_AgentController::emFN3, false, mMUX_PIC0REG22_03Output[0]);
}//}}}

/// Calculate output value of MUX(PIC0REG212[3])
void PIC1A_Func::CalcMUX_PIC0REG212_03 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[1])["PIC0REG2203"]) {
        case 1:
            mMUX_PIC0REG22_03Output[1] = mRS_Output[PIC1A_AgentController::emRS13];
            break;
        default:    // 0
            mMUX_PIC0REG22_03Output[1] = mInputValue[PIC1A_AgentController::emTAUD1TOUT7];
            break;
    }
    CalcMUX_PIC0REG22_2223(1);
    UpdateFNInput(1, PIC1A_AgentController::emFN3, false, mMUX_PIC0REG22_03Output[1]);
}//}}}

/// Calculate output value of MUX(PIC0REG202[4])
void PIC1A_Func::CalcMUX_PIC0REG202_04 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[0])["PIC0REG2204"]) {
        case 1:
            mMUX_PIC0REG22_04Output[0] = mRS_Output[PIC1A_AgentController::emRS04];
            break;
        default:    // 0
            mMUX_PIC0REG22_04Output[0] = mInputValue[PIC1A_AgentController::emTAUD0TOUT9];
            break;
    }
    CalcMUX_PIC0REG22_2627(0);
    UpdateFNInput(0, PIC1A_AgentController::emFN5, false, mMUX_PIC0REG22_04Output[0]);
}//}}}

/// Calculate output value of MUX(PIC0REG212[4])
void PIC1A_Func::CalcMUX_PIC0REG212_04 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[1])["PIC0REG2204"]) {
        case 1:
            mMUX_PIC0REG22_04Output[1] = mRS_Output[PIC1A_AgentController::emRS14];
            break;
        default:    // 0
            mMUX_PIC0REG22_04Output[1] = mInputValue[PIC1A_AgentController::emTAUD1TOUT9];
            break;
    }
    CalcMUX_PIC0REG22_2627(1);
    UpdateFNInput(1, PIC1A_AgentController::emFN5, false, mMUX_PIC0REG22_04Output[1]);
}//}}}

/// Calculate output value of MUX(PIC0REG202[5])
void PIC1A_Func::CalcMUX_PIC0REG202_05 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[0])["PIC0REG2205"]) {
        case 1:
            mPFN_In2Input[0][PIC1A_AgentController::emPFN01] = mRS_Output[PIC1A_AgentController::emRS05];
            break;
        default:    // 0
            mPFN_In2Input[0][PIC1A_AgentController::emPFN01] = mInputValue[PIC1A_AgentController::emTAUD0TOUT11];
            break;
    }
    CalcPFN(0, PIC1A_AgentController::emPFN01, PIC1A_AgentController::emPFNIn2Input);
}//}}}

/// Calculate output value of MUX(PIC0REG212[5])
void PIC1A_Func::CalcMUX_PIC0REG212_05 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[1])["PIC0REG2205"]) {
        case 1:
            mPFN_In2Input[1][PIC1A_AgentController::emPFN01] = mRS_Output[PIC1A_AgentController::emRS15];
            break;
        default:    // 0
            mPFN_In2Input[1][PIC1A_AgentController::emPFN01] = mInputValue[PIC1A_AgentController::emTAUD1TOUT11];
            break;
    }
    CalcPFN(1, PIC1A_AgentController::emPFN01, PIC1A_AgentController::emPFNIn2Input);
}//}}}

/// Calculate output value of MUX(PIC0REG202[6])
void PIC1A_Func::CalcMUX_PIC0REG202_06 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[0])["PIC0REG2206"]) {
        case 1:
            mPFN_In2Input[0][PIC1A_AgentController::emPFN23] = mRS_Output[PIC1A_AgentController::emRS06];
            break;
        default:    // 0
            mPFN_In2Input[0][PIC1A_AgentController::emPFN23] = mInputValue[PIC1A_AgentController::emTAUD0TOUT13];
            break;
    }
    CalcPFN(0, PIC1A_AgentController::emPFN23, PIC1A_AgentController::emPFNIn2Input);
}//}}}

/// Calculate output value of MUX(PIC0REG212[6])
void PIC1A_Func::CalcMUX_PIC0REG212_06 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[1])["PIC0REG2206"]) {
        case 1:
            mPFN_In2Input[1][PIC1A_AgentController::emPFN23] = mRS_Output[PIC1A_AgentController::emRS16];
            break;
        default:    // 0
            mPFN_In2Input[1][PIC1A_AgentController::emPFN23] = mInputValue[PIC1A_AgentController::emTAUD1TOUT13];
            break;
    }
    CalcPFN(1, PIC1A_AgentController::emPFN23, PIC1A_AgentController::emPFNIn2Input);
}//}}}

/// Calculate output value of MUX(PIC0REG202[7])
void PIC1A_Func::CalcMUX_PIC0REG202_07 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[0])["PIC0REG2207"]) {
        case 1:
            mPFN_In2Input[0][PIC1A_AgentController::emPFN45] = mRS_Output[PIC1A_AgentController::emRS07];
            break;
        default:    // 0
            mPFN_In2Input[0][PIC1A_AgentController::emPFN45] = mInputValue[PIC1A_AgentController::emTAUD0TOUT15];
            break;
    }
    CalcPFN(0, PIC1A_AgentController::emPFN45, PIC1A_AgentController::emPFNIn2Input);
}//}}}

/// Calculate output value of MUX(PIC0REG212[7])
void PIC1A_Func::CalcMUX_PIC0REG212_07 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[1])["PIC0REG2207"]) {
        case 1:
            mPFN_In2Input[1][PIC1A_AgentController::emPFN45] = mRS_Output[PIC1A_AgentController::emRS17];
            break;
        default:    // 0
            mPFN_In2Input[1][PIC1A_AgentController::emPFN45] = mInputValue[PIC1A_AgentController::emTAUD1TOUT15];
            break;
    }
    CalcPFN(1, PIC1A_AgentController::emPFN45, PIC1A_AgentController::emPFNIn2Input);
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[8])
void PIC1A_Func::CalcMUX_PIC0REG22_08 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2208"]) {
        case 1:
            mMUX_PIC0REG22_08Output[module] = mWNG_gOutput[module][PIC1A_AgentController::emWNG0];
            break;
        default:    // 0
            mMUX_PIC0REG22_08Output[module] = mFN_Output[module][PIC1A_AgentController::emFN0];
            break;
    }
    CalcMUX_PIC0REG5_03(module, PIC1A_AgentController::emUS);
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP0U, mMUX_PIC0REG22_08Output[module]);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP1U, mMUX_PIC0REG22_08Output[module]);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[9])
void PIC1A_Func::CalcMUX_PIC0REG22_09 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2209"]) {
        case 1:
            mMUX_PIC0REG22_09Output[module] = mWNG_iOutput[module][PIC1A_AgentController::emWNG0];
            break;
        default:    // 0
            mMUX_PIC0REG22_09Output[module] = mFN_Output[module][PIC1A_AgentController::emFN1];
            break;
    }
    CalcMUX_PIC0REG5_03(module, PIC1A_AgentController::emUC);
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP0UB, mMUX_PIC0REG22_09Output[module]);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP1UB, mMUX_PIC0REG22_09Output[module]);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[10])
void PIC1A_Func::CalcMUX_PIC0REG22_10 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2210"]) {
        case 1:
            mMUX_PIC0REG22_10Output[module] = mWNG_gOutput[module][PIC1A_AgentController::emWNG1];
            break;
        default:    // 0
            mMUX_PIC0REG22_10Output[module] = mFN_Output[module][PIC1A_AgentController::emFN2];
            break;
    }
    CalcMUX_PIC0REG5_03(module, PIC1A_AgentController::emVS);
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP0V, mMUX_PIC0REG22_10Output[module]);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP1V, mMUX_PIC0REG22_10Output[module]);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[11])
void PIC1A_Func::CalcMUX_PIC0REG22_11 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2211"]) {
        case 1:
            mMUX_PIC0REG22_11Output[module] = mWNG_iOutput[module][PIC1A_AgentController::emWNG1];
            break;
        default:    // 0
            mMUX_PIC0REG22_11Output[module] = mFN_Output[module][PIC1A_AgentController::emFN3];
            break;
    }
    CalcMUX_PIC0REG5_03(module, PIC1A_AgentController::emVC);
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP0VB, mMUX_PIC0REG22_11Output[module]);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP1VB, mMUX_PIC0REG22_11Output[module]);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[12])
void PIC1A_Func::CalcMUX_PIC0REG22_12 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2212"]) {
        case 1:
            mMUX_PIC0REG22_12Output[module] = mWNG_gOutput[module][PIC1A_AgentController::emWNG2];
            break;
        default:    // 0
            mMUX_PIC0REG22_12Output[module] = mFN_Output[module][PIC1A_AgentController::emFN4];
            break;
    }
    CalcMUX_PIC0REG5_03(module, PIC1A_AgentController::emWS);
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP0W, mMUX_PIC0REG22_12Output[module]);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP1W, mMUX_PIC0REG22_12Output[module]);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[13])
void PIC1A_Func::CalcMUX_PIC0REG22_13 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2213"]) {
        case 1:
            mMUX_PIC0REG22_13Output[module] = mWNG_iOutput[module][PIC1A_AgentController::emWNG2];
            break;
        default:    // 0
            mMUX_PIC0REG22_13Output[module] = mFN_Output[module][PIC1A_AgentController::emFN5];
            break;
    }
    CalcMUX_PIC0REG5_03(module, PIC1A_AgentController::emWC);
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP0WB, mMUX_PIC0REG22_13Output[module]);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTOP1WB, mMUX_PIC0REG22_13Output[module]);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[16-17])
void PIC1A_Func::CalcMUX_PIC0REG22_1617 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2217_16"]) {
        case 1:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT10] = mWNG_dOutput[module][PIC1A_AgentController::emWNG0];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT10] = mMUX_PIC0REG20_18Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT10] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN10];
            } else {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT10] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN10];
            }
            break;
    }
    CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT10);
    CalcMUX_PIC0REG22_30(module);
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[18-19])
void PIC1A_Func::CalcMUX_PIC0REG22_1819 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2219_18"]) {
        case 1:
            mMUX_PIC0REG22_1819Output[module] = mWNG_eOutput[module][PIC1A_AgentController::emWNG0];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG22_1819Output[module] = mMUX_PIC0REG22_02Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG22_1819Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN11];
            } else {
                mMUX_PIC0REG22_1819Output[module] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN11];
            }
            break;
    }
    CalcMUX_PIC0REG22_30(module);
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[20-21])
void PIC1A_Func::CalcMUX_PIC0REG22_2021 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2221_20"]) {
        case 1:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT12] = mWNG_dOutput[module][PIC1A_AgentController::emWNG1];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT12] = mMUX_PIC0REG20_18Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT12] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN12];
            } else {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT12] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN12];
            }
            break;
    }
    CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT12);
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[22-23])
void PIC1A_Func::CalcMUX_PIC0REG22_2223 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2223_22"]) {
        case 1:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT13] = mWNG_eOutput[module][PIC1A_AgentController::emWNG1];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT13] = mMUX_PIC0REG22_03Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT13] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN13];
            } else {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT13] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN13];
            }
            break;
    }
    CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT13);
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[24-25])
void PIC1A_Func::CalcMUX_PIC0REG22_2425 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2225_24"]) {
        case 1:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT14] = mWNG_dOutput[module][PIC1A_AgentController::emWNG2];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT14] = mMUX_PIC0REG20_18Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT14] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN14];
            } else {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT14] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN14];
            }
            break;
    }
    CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT14);
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[26-27])
void PIC1A_Func::CalcMUX_PIC0REG22_2627 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2227_26"]) {
        case 1:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT15] = mWNG_eOutput[module][PIC1A_AgentController::emWNG2];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT15] = mMUX_PIC0REG22_04Output[module];
            break;
        default:    // 0
            if (module == 0) {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT15] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN15];
            } else {
                mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT15] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN15];
            }
            break;
    }
    CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT15);
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[28])
void PIC1A_Func::CalcMUX_PIC0REG22_28 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2228"]) {
        case 1:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT07] = mMUX_PIC0REG20_02Output[module];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT07] = mMUX_PIC0REG20_03Output[module];
            break;
    }
    CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT07);
}//}}}

/// Calculate output value of MUX(PIC0REG202[29])
void PIC1A_Func::CalcMUX_PIC0REG202_29 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[0])["PIC0REG2229"]) {
        case 1:
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT09] = mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT08];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT09] = mMUX_PIC0REG20_05Output[0];
            break;
    }
    CalcMUX_PIC0REG24(0, PIC1A_AgentController::emT09);
}//}}}

/// Calculate output value of MUX(PIC0REG212[29])
void PIC1A_Func::CalcMUX_PIC0REG212_29 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG22[1])["PIC0REG2229"]) {
        case 1:
            mMUX_PIC0REG212_29Output = mMUX_PIC0REG210_04Output;
            break;
        default:    // 0
            mMUX_PIC0REG212_29Output = mMUX_PIC0REG20_05Output[1];
            break;
    }
    CalcMUX_PIC0REG31_25();
}//}}}

/// Calculate output value of MUX(PIC0REG2n2[30])
void PIC1A_Func::CalcMUX_PIC0REG22_30 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG22[module])["PIC0REG2230"]) {
        case 1:
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT11] = mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT10];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[module][PIC1A_AgentController::emT11] = mMUX_PIC0REG22_1819Output[module];
            break;
    }
    CalcMUX_PIC0REG24(module, PIC1A_AgentController::emT11);
}//}}}

/// Calculate output value of MUX(PIC0REG2n4)
void PIC1A_Func::CalcMUX_PIC0REG24 (const unsigned int module, const unsigned int index)
{//{{{
    if ((bool)(((unsigned int)(*PIC0REG24[module]) >> index) & 0x1)) {
        mMUX_PIC0REG24Output[module][index] = mMUX_PIC0REG33Output[module];
    } else {
        mMUX_PIC0REG24Output[module][index] = mMUX_PIC0REG24Input[module][index];
    }
    
    CalcMUX_PIC0TAUD0SEL(index);
    CalcMUX_PIC0TAUD1SEL(index);
}//}}}

/// Calculate output value of MUX(PIC0REG30[0])
void PIC1A_Func::CalcMUX_PIC0REG30_00 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3000"]) {
        case 1:
            mMUX_PIC0REG30_00Output = mInputValue[PIC1A_AgentController::emTSG0TS2PEC];
            break;
        default:    // 0
            mMUX_PIC0REG30_00Output = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG1];
            break;
    }
    CalcMUX_PIC0REG30_22(PIC1A_AgentController::emG1);
}//}}}

/// Calculate output value of MUX(PIC0REG30[1])
void PIC1A_Func::CalcMUX_PIC0REG30_01 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3001"]) {
        case 1:
            mMUX_PIC0REG30_01Output = mInputValue[PIC1A_AgentController::emTSG0TST2PUD];
            break;
        default:    // 0
            mMUX_PIC0REG30_01Output = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG2];
            break;
    }
    CalcMUX_PIC0REG30_22(PIC1A_AgentController::emG2);
}//}}}

/// Calculate output value of MUX(PIC0REG30[2-5])
void PIC1A_Func::CalcMUX_PIC0REG30_0205 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3005_02"]) {
        case 1:
            out_val = mMUX_PIC0REG30_21Output;
            break;
        case 2:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT04];
            break;
        case 3:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT03];
            break;
        case 4:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT02];
            break;
        case 5:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT01];
            break;
        case 6:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT00];
            break;
        case 7:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT14];
            break;
        case 8:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT13];
            break;
        case 9:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT12];
            break;
        case 10:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT11];
            break;
        case 11:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT10];
            break;
        default:    // 0 , 12 -> 15
            out_val = mMUX_PIC0REG30_18Output;
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT0TIN1, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG30[6-7])
void PIC1A_Func::CalcMUX_PIC0REG30_0607 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3007_06"]) {
        case 1:
            out_val = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG1];
            break;
        case 2:
            out_val = mInputValue[PIC1A_AgentController::emTSG1TS2PEC];
            break;
        case 3:
            out_val = mInputValue[PIC1A_AgentController::emENCAT0IOV];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG1];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT1AIN, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG30[8-9])
void PIC1A_Func::CalcMUX_PIC0REG30_0809 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3009_08"]) {
        case 1:
            out_val = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG2];
            break;
        case 2:
            out_val = mInputValue[PIC1A_AgentController::emTSG1TST2PUD];
            break;
        case 3:
            out_val = mInputValue[PIC1A_AgentController::emENCAT0IUD];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG2];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT1BIN, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG30[10-11])
void PIC1A_Func::CalcMUX_PIC0REG30_1011 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3011_10"]) {
        case 1:
            out_val = mInputValue[PIC1A_AgentController::emENCAT0EQ0];
            break;
        case 2:
            out_val = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG3];
            break;
        case 3:
            out_val = mInputValue[PIC1A_AgentController::emENCAT0EQ1];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG3];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT1ZIN, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG30[12-15])
void PIC1A_Func::CalcMUX_PIC0REG30_1215 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3015_12"]) {
        case 1:
            out_val = mMUX_PIC0REG30_18Output;
            break;
        case 2:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT04];
            break;
        case 3:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT03];
            break;
        case 4:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT02];
            break;
        case 5:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT01];
            break;
        case 6:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT00];
            break;
        case 7:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT14];
            break;
        case 8:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT13];
            break;
        case 9:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT12];
            break;
        case 10:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT11];
            break;
        case 11:
            out_val = mInputValue[PIC1A_AgentController::emADCATTOUT10];
            break;
        default:    // 0 , 12 -> 15
            out_val = mMUX_PIC0REG30_21Output;
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT1TIN1, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG30[16-17])
void PIC1A_Func::CalcMUX_PIC0REG30_1617 (const unsigned int type)
{//{{{
    unsigned int bit_value = (unsigned int)(*PIC0REG30)["PIC0REG3017_16"];
    switch (type) {
        case PIC1A_AgentController::emG1:
            switch (bit_value) {
                case 1:
                    mMUX_PIC0REG30_1617Output[type] = mInputValue[PIC1A_AgentController::emSAT0AOUT];
                    break;
                case 2:
                case 3:
                    mMUX_PIC0REG30_1617Output[type] = mInputValue[PIC1A_AgentController::emSAT1AOUT];
                    break;
                default:    // 0
                    mMUX_PIC0REG30_1617Output[type] = mInputValue[PIC1A_AgentController::emDNFENCAT0AIN];
                    break;
            }
            CalcMUX_PIC0REG30_00();
            CalcMUX_PIC0REG30_0607();
            CalcMUX_PIC0REG32_00_08(PIC1A_AgentController::emENCAT2AIN);
            CalcMUX_PIC0REG32_00_08(PIC1A_AgentController::emENCAT3AIN);
            break;
        case PIC1A_AgentController::emG2:
            switch (bit_value) {
                case 1:
                    mMUX_PIC0REG30_1617Output[type] = mInputValue[PIC1A_AgentController::emSAT0BOUT];
                    break;
                case 2:
                case 3:
                    mMUX_PIC0REG30_1617Output[type] = mInputValue[PIC1A_AgentController::emSAT1BOUT];
                    break;
                default:    // 0
                    mMUX_PIC0REG30_1617Output[type] = mInputValue[PIC1A_AgentController::emDNFENCAT0BIN];
                    break;
            }
            CalcMUX_PIC0REG30_01();
            CalcMUX_PIC0REG30_0809();
            CalcMUX_PIC0REG32_01_09(PIC1A_AgentController::emENCAT2BIN);
            CalcMUX_PIC0REG32_01_09(PIC1A_AgentController::emENCAT3BIN);
            break;
        default:    // PIC1A_AgentController::emG3
            switch (bit_value) {
                case 1:
                    mMUX_PIC0REG30_1617Output[type] = mInputValue[PIC1A_AgentController::emSAT0ZOUT];
                    break;
                case 2:
                case 3:
                    mMUX_PIC0REG30_1617Output[type] = mInputValue[PIC1A_AgentController::emSAT1ZOUT];
                    break;
                default:    // 0
                    mMUX_PIC0REG30_1617Output[type] = mInputValue[PIC1A_AgentController::emDNFENCAT0ZIN];
                    break;
            }
            CalcMUX_PIC0REG30_1011();
            CalcMUX_PIC0REG30_22(PIC1A_AgentController::emG3);
            CalcMUX_PIC0REG32_0203();
            CalcMUX_PIC0REG32_1011();
            break;
    }
    CalcMUX_PIC0REG31_0608();
}//}}}

/// Calculate output value of MUX(PIC0REG30[18])
void PIC1A_Func::CalcMUX_PIC0REG30_18 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3018"]) {
        case 1:
            mMUX_PIC0REG30_18Output = mMUX_PIC0ENCSEL40_07Output[0];
            break;
        default:    // 0
            mMUX_PIC0REG30_18Output = mInputValue[PIC1A_AgentController::emDNFENCAT0TIN1];
            break;
    }
    CalcMUX_PIC0REG30_0205();
    CalcMUX_PIC0REG30_1215();
}//}}}

/// Calculate output value of MUX(PIC0REG30[19-20])
void PIC1A_Func::CalcMUX_PIC0REG30_1920 (const unsigned int type)
{//{{{
    unsigned int bit_value = (unsigned int)(*PIC0REG30)["PIC0REG3020_19"];
    switch (type) {
        case PIC1A_AgentController::emG1:
            switch (bit_value) {
                case 1:
                    mMUX_PIC0REG30_1920Output[type] = mInputValue[PIC1A_AgentController::emSAT1AOUT];
                    break;
                case 2:
                case 3:
                    mMUX_PIC0REG30_1920Output[type] = mInputValue[PIC1A_AgentController::emSAT0AOUT];
                    break;
                default:    // 0
                    mMUX_PIC0REG30_1920Output[type] = mInputValue[PIC1A_AgentController::emDNFENCAT1AIN];
                    break;
            }
            CalcMUX_PIC0REG30_0607();
            CalcMUX_PIC0REG32_00_08(PIC1A_AgentController::emENCAT2AIN);
            CalcMUX_PIC0REG32_00_08(PIC1A_AgentController::emENCAT3AIN);
            break;
        case PIC1A_AgentController::emG2:
            switch (bit_value) {
                case 1:
                    mMUX_PIC0REG30_1920Output[type] = mInputValue[PIC1A_AgentController::emSAT1BOUT];
                    break;
                case 2:
                case 3:
                    mMUX_PIC0REG30_1920Output[type] = mInputValue[PIC1A_AgentController::emSAT0BOUT];
                    break;
                default:    // 0
                    mMUX_PIC0REG30_1920Output[type] = mInputValue[PIC1A_AgentController::emDNFENCAT1BIN];
                    break;
            }
            CalcMUX_PIC0REG30_0809();
            CalcMUX_PIC0REG32_01_09(PIC1A_AgentController::emENCAT2BIN);
            CalcMUX_PIC0REG32_01_09(PIC1A_AgentController::emENCAT3BIN);
            break;
        default:    // PIC1A_AgentController::emG3
            switch (bit_value) {
                case 1:
                    mMUX_PIC0REG30_1920Output[type] = mInputValue[PIC1A_AgentController::emSAT1ZOUT];
                    break;
                case 2:
                case 3:
                    mMUX_PIC0REG30_1920Output[type] = mInputValue[PIC1A_AgentController::emSAT0ZOUT];
                    break;
                default:    // 0
                    mMUX_PIC0REG30_1920Output[type] = mInputValue[PIC1A_AgentController::emDNFENCAT1ZIN];
                    break;
            }
            CalcMUX_PIC0REG30_1011();
            CalcMUX_PIC0REG32_0203();
            CalcMUX_PIC0REG32_1011();
            break;
    }
    CalcMUX_PIC0REG30_22(type);
    CalcMUX_PIC0REG31_1517();
}//}}}

/// Calculate output value of MUX(PIC0REG30[21])
void PIC1A_Func::CalcMUX_PIC0REG30_21 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3021"]) {
        case 1:
            mMUX_PIC0REG30_21Output = mMUX_PIC0ENCSEL40_07Output[1];
            break;
        default:    // 0
            mMUX_PIC0REG30_21Output = mInputValue[PIC1A_AgentController::emDNFENCAT1TIN1];
            break;
    }
    CalcMUX_PIC0REG30_0205();
    CalcMUX_PIC0REG30_1215();
}//}}}

/// Calculate output value of MUX(PIC0REG30[22])
void PIC1A_Func::CalcMUX_PIC0REG30_22 (const unsigned int index)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3022"]) {
        case 1:
            out_val = mMUX_PIC0REG30_1920Output[index];
            break;
        default:    // 0
            switch (index) {
                case PIC1A_AgentController::emG1:
                    out_val = mMUX_PIC0REG30_00Output;
                    break;
                case PIC1A_AgentController::emG2:
                    out_val = mMUX_PIC0REG30_01Output;
                    break;
                default:    // PIC1A_AgentController::emG3
                    out_val = mMUX_PIC0REG30_1617Output[index];
                    break;
            }
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT0AIN + index, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG30[23])
void PIC1A_Func::CalcMUX_PIC0REG30_23 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3023"]) {
        case 1:
            out_val = mInputValue[PIC1A_AgentController::emENCAT0IOV];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emENCAT0INT0];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emINTENC0CM0, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG30[24])
void PIC1A_Func::CalcMUX_PIC0REG30_24 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG30)["PIC0REG3024"]) {
        case 1:
            out_val = mInputValue[PIC1A_AgentController::emENCAT1IOV];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emENCAT1INT0];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emINTENC1CM0, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG31[0])
void PIC1A_Func::CalcMUX_PIC0REG31_00 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3100"]) {
        case 1:
            out_val = mDT_Output[PIC1A_AgentController::emDT00];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emDNFTAUJ0TIN0];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTAUJ0TIN0, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG31[1-2])
void PIC1A_Func::CalcMUX_PIC0REG31_0102 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3102_01"]) {
        case 1:
            out_val = mDT_Output[PIC1A_AgentController::emDT00];
            break;
        case 2:
        case 3:
            out_val = false;
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emDNFTAUJ0TIN1];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTAUJ0TIN1, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG31[3])
void PIC1A_Func::CalcMUX_PIC0REG31_03 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3103"]) {
        case 1:
            mMUX_PIC0REG31_03Output = mDT_Output[PIC1A_AgentController::emDT10];
            break;
        default:    // 0
            mMUX_PIC0REG31_03Output = mInputValue[PIC1A_AgentController::emDNFTAUJ0TIN2];
            break;
    }
    CalcMUX_PIC0REG31_26();
}//}}}

/// Calculate output value of MUX(PIC0REG31[4-5])
void PIC1A_Func::CalcMUX_PIC0REG31_0405 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3105_04"]) {
        case 1:
            out_val = mDT_Output[PIC1A_AgentController::emDT10];
            break;
        case 2:
        case 3:
            out_val = false;
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emDNFTAUJ0TIN3];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTAUJ0TIN3, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG31[6-8])
void PIC1A_Func::CalcMUX_PIC0REG31_0608 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3108_06"]) {
        case 1:
        case 5:
            mMUX_PIC0REG31_0608Output = mDT_Output[PIC1A_AgentController::emDT02];
            break;
        case 2:
        case 6:
            mMUX_PIC0REG31_0608Output = mInputValue[PIC1A_AgentController::emTSG0TSTPTE];
            break;
        case 3:
        case 7:
            mMUX_PIC0REG31_0608Output = (mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG1] ^
                                         mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG2]) ^
                                        mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG3];
            break;
        case 4:
            mMUX_PIC0REG31_0608Output = mDT_Output[PIC1A_AgentController::emDT00];
            break;
        default :    // 0
            mMUX_PIC0REG31_0608Output = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN0];
            break;
    }
    CalcMUX_PIC0REG31_0910();
    CalcMUX_PIC0REG31_11();
}//}}}

/// Calculate output value of MUX(PIC0REG31[9-10])
void PIC1A_Func::CalcMUX_PIC0REG31_0910 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3110_09"]) {
        case 1:
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT00] = mInputValue[PIC1A_AgentController::emTAUD0INT1];
            break;
        case 2:
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT00] = mDT_Output[PIC1A_AgentController::emDT01];
            break;
        case 3:
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT00] = mMUX_PIC0REG20_16Output[0];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT00] = mMUX_PIC0REG31_0608Output;
            break;
    }
    CalcMUX_PIC0REG24(0, PIC1A_AgentController::emT00);
}//}}}

/// Calculate output value of MUX(PIC0REG31[11])
void PIC1A_Func::CalcMUX_PIC0REG31_11 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3111"]) {
        case 1:
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT01] = mMUX_PIC0REG31_0608Output;
            break;
        default:    // 0
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT01] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN1];
            break;
    }
    CalcMUX_PIC0REG24(0, PIC1A_AgentController::emT01);
}//}}}

/// Calculate output value of MUX(PIC0REG31[12-13])
void PIC1A_Func::CalcMUX_PIC0REG31_1213 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3113_12"]) {
        case 1:
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT02] = mDT_Output[PIC1A_AgentController::emDT01];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT02] = mDT_Output[PIC1A_AgentController::emDT10];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT02] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN2];
            break;
    }
    CalcMUX_PIC0REG24(0, PIC1A_AgentController::emT02);
}//}}}

/// Calculate output value of MUX(PIC0REG31[14])
void PIC1A_Func::CalcMUX_PIC0REG31_14 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3114"]) {
        case 1:
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT03] = mDT_Output[PIC1A_AgentController::emDT10];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[0][PIC1A_AgentController::emT03] = mInputValue[PIC1A_AgentController::emDNFTAUD0TIN3];
            break;
    }
    CalcMUX_PIC0REG24(0, PIC1A_AgentController::emT03);
}//}}}

/// Calculate output value of MUX(PIC0REG31[15-17])
void PIC1A_Func::CalcMUX_PIC0REG31_1517 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3117_15"]) {
        case 1:
        case 5:
            mMUX_PIC0REG31_1517Output = mDT_Output[PIC1A_AgentController::emDT12];
            break;
        case 2:
        case 6:
            mMUX_PIC0REG31_1517Output = mInputValue[PIC1A_AgentController::emTSG1TSTPTE];
            break;
        case 3:
        case 7:
            mMUX_PIC0REG31_1517Output = (mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG1] ^
                                         mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG2]) ^
                                        mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG3];
            break;
        case 4:
            mMUX_PIC0REG31_1517Output = mDT_Output[PIC1A_AgentController::emDT10];
            break;
        default:    // 0
            mMUX_PIC0REG31_1517Output = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN0];
            break;
    }
    CalcMUX_PIC0REG31_1819();
    CalcMUX_PIC0REG31_20();
}//}}}

/// Calculate output value of MUX(PIC0REG31[18-19])
void PIC1A_Func::CalcMUX_PIC0REG31_1819 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3119_18"]) {
        case 1:
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT00] = mInputValue[PIC1A_AgentController::emTAUD1INT1];
            break;
        case 2:
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT00] = mDT_Output[PIC1A_AgentController::emDT11];
            break;
        case 3:
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT00] = mMUX_PIC0REG20_16Output[1];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT00] = mMUX_PIC0REG31_1517Output;
            break;
    }
    CalcMUX_PIC0REG24(1, PIC1A_AgentController::emT00);
}//}}}

/// Calculate output value of MUX(PIC0REG31[20])
void PIC1A_Func::CalcMUX_PIC0REG31_20 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3120"]) {
        case 1:
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT01] = mMUX_PIC0REG31_1517Output;
            break;
        default:    // 0
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT01] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN1];
            break;
    }
    CalcMUX_PIC0REG24(1, PIC1A_AgentController::emT01);
}//}}}

/// Calculate output value of MUX(PIC0REG31[21-22])
void PIC1A_Func::CalcMUX_PIC0REG31_2122 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3122_21"]) {
        case 1:
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT02] = mDT_Output[PIC1A_AgentController::emDT11];
            break;
        case 2:
        case 3:
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT02] = mDT_Output[PIC1A_AgentController::emDT00];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT02] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN2];
            break;
    }
    CalcMUX_PIC0REG24(1, PIC1A_AgentController::emT02);
}//}}}

/// Calculate output value of MUX(PIC0REG31[23])
void PIC1A_Func::CalcMUX_PIC0REG31_23 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3123"]) {
        case 1:
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT03] = mDT_Output[PIC1A_AgentController::emDT00];
            break;
        default:    // 0
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT03] = mInputValue[PIC1A_AgentController::emDNFTAUD1TIN3];
            break;
    }
    CalcMUX_PIC0REG24(1, PIC1A_AgentController::emT03);
}//}}}

/// Calculate output value of MUX(PIC0REG31[24])
void PIC1A_Func::CalcMUX_PIC0REG31_24 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3124"]) {
        case 1:
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT08] = false;
            break;
        default:    // 0
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT08] = mMUX_PIC0REG210_04Output;
            break;
    }
    CalcMUX_PIC0REG24(1, PIC1A_AgentController::emT08);
}//}}}

/// Calculate output value of MUX(PIC0REG31[25])
void PIC1A_Func::CalcMUX_PIC0REG31_25 (void)
{//{{{
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3125"]) {
        case 1:
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT09] = false;
            break;
        default:    // 0
            mMUX_PIC0REG24Input[1][PIC1A_AgentController::emT09] = mMUX_PIC0REG212_29Output;
            break;
    }
    CalcMUX_PIC0REG24(1, PIC1A_AgentController::emT09);
}//}}}

/// Calculate output value of MUX(PIC0REG31[26])
void PIC1A_Func::CalcMUX_PIC0REG31_26 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG31)["PIC0REG3126"]) {
        case 1:
            out_val = false;
            break;
        default:    // 0
            out_val = mMUX_PIC0REG31_03Output;
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTAUJ0TIN2, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG32[0]) & MUX(PIC0REG32[8])
void PIC1A_Func::CalcMUX_PIC0REG32_00_08 (const unsigned int type)
{//{{{
    bool out_val = false;
    bool bit_value = false;
    if (type == PIC1A_AgentController::emENCAT2AIN) {
        bit_value = (bool)(*PIC0REG32)["PIC0REG3200"];
        if (bit_value) {
            out_val = mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG1];
        } else {
            out_val = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG1];
        }
    } else {
        bit_value = (bool)(*PIC0REG32)["PIC0REG3208"];
        if (bit_value) {
            out_val = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG1];
        } else {
            out_val = mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG1];
        }
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(type, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG32[1]) & MUX(PIC0REG32[9])
void PIC1A_Func::CalcMUX_PIC0REG32_01_09 (const unsigned int type)
{//{{{
    bool out_val = false;
    bool bit_value = false;
    if (type == PIC1A_AgentController::emENCAT2BIN) {
        bit_value = (bool)(*PIC0REG32)["PIC0REG3201"];
        if (bit_value) {
            out_val = mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG2];
        } else {
            out_val = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG2];
        }
    } else {
        bit_value = (bool)(*PIC0REG32)["PIC0REG3209"];
        if (bit_value) {
            out_val = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG2];
        } else {
            out_val = mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG2];
        }
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(type, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG32[2-3])
void PIC1A_Func::CalcMUX_PIC0REG32_0203 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG32)["PIC0REG3203_02"]) {
        case 1:
            out_val = mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG3];
            break;
        case 2:
            out_val = mInputValue[PIC1A_AgentController::emENCAT0EQ0];
            break;
        case 3:
            out_val = mInputValue[PIC1A_AgentController::emENCAT0EQ1];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG3];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT2ZIN, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG32[4])
void PIC1A_Func::CalcMUX_PIC0REG32_04 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG32)["PIC0REG3204"]) {
        case 1:
            out_val = mMUX_PIC0ENCSEL40_07Output[0];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emDNFENCAT2TIN1];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT2TIN1, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG32[5])
void PIC1A_Func::CalcMUX_PIC0REG32_05 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG32)["PIC0REG3205"]) {
        case 1:
            out_val = mInputValue[PIC1A_AgentController::emENCAT2IOV];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emENCAT2INT0];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emINTENC2CM0, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG32[10-11])
void PIC1A_Func::CalcMUX_PIC0REG32_1011 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG32)["PIC0REG3211_10"]) {
        case 1:
            out_val = mMUX_PIC0REG30_1617Output[PIC1A_AgentController::emG3];
            break;
        case 2:
            out_val = mInputValue[PIC1A_AgentController::emENCAT1EQ0];
            break;
        case 3:
            out_val = mInputValue[PIC1A_AgentController::emENCAT1EQ1];
            break;
        default:    // 0
            out_val = mMUX_PIC0REG30_1920Output[PIC1A_AgentController::emG3];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT3ZIN, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG32[12])
void PIC1A_Func::CalcMUX_PIC0REG32_12 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG32)["PIC0REG3212"]) {
        case 1:
            out_val = mMUX_PIC0ENCSEL40_07Output[1];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emDNFENCAT3TIN1];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emENCAT3TIN1, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG32[13])
void PIC1A_Func::CalcMUX_PIC0REG32_13 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG32)["PIC0REG3213"]) {
        case 1:
            out_val = mInputValue[PIC1A_AgentController::emENCAT3IOV];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emENCAT3INT0];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emINTENC3CM0, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG33[n])
void PIC1A_Func::CalcMUX_PIC0REG33 (const unsigned int module)
{//{{{
    switch (((unsigned int)(*PIC0REG33) >> module) & 0x1) {
        case 1:
            mMUX_PIC0REG33Output[module] = mInputValue[PIC1A_AgentController::emTPB0INTPAT];
            break;
        default:    // 0
            mMUX_PIC0REG33Output[module] = mInputValue[PIC1A_AgentController::emDNFSIF0IREF + module];
            break;
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emMUX_PIC0REG2n4num; i++) {
        CalcMUX_PIC0REG24(module, i);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG5n[0])
void PIC1A_Func::CalcMUX_PIC0REG5_00 (const unsigned int module, const unsigned int index)
{//{{{
    bool bit_value = (bool)(*PIC0REG5[module])["PIC0REG500"];
    switch (index) {
        case PIC1A_AgentController::emI1:
            if (bit_value) {
                mMUX_PIC0REG5_00Output[module][index] = mInputValue[PIC1A_AgentController::emDNFENCAT1AIN];
            } else {
                mMUX_PIC0REG5_00Output[module][index] = mInputValue[PIC1A_AgentController::emDNFENCAT0AIN];
            }
            if (module == 0) {
                CalcMUX_PIC0REG200_0809();
            } else {
                CalcMUX_PIC0REG210_0809();
            }
            break;
        case PIC1A_AgentController::emI2:
            if (bit_value) {
                mMUX_PIC0REG5_00Output[module][index] = mInputValue[PIC1A_AgentController::emDNFENCAT1BIN];
            } else {
                mMUX_PIC0REG5_00Output[module][index] = mInputValue[PIC1A_AgentController::emDNFENCAT0BIN];
            }
            if (module == 0) {
                CalcMUX_PIC0REG200_1011();
            } else {
                CalcMUX_PIC0REG210_1011();
            }
            break;
        default:    // PIC1A_AgentController::emI3
            if (bit_value) {
                mMUX_PIC0REG5_00Output[module][index] = mInputValue[PIC1A_AgentController::emDNFENCAT1ZIN];
            } else {
                mMUX_PIC0REG5_00Output[module][index] = mInputValue[PIC1A_AgentController::emDNFENCAT0ZIN];
            }
            if (module == 0) {
                CalcMUX_PIC0REG200_1213();
            } else {
                CalcMUX_PIC0REG210_1213();
            }
            break;
    }
    CalcMUX_PIC0HALLSEL(module, index);
}//}}}

/// Calculate output value of MUX(PIC0REG50[1-2])
void PIC1A_Func::CalcMUX_PIC0REG50_0102 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG5[0])["PIC0REG502_01"]) {
        case 1:
            out_val = mInputValue[PIC1A_AgentController::emTAUD0TOUT3];
            break;
        case 2:
        case 3:
            out_val = mInputValue[PIC1A_AgentController::emTAUJ0TOUT3];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emDNFTSG0TSTCKE];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTCKE, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG51[1-2])
void PIC1A_Func::CalcMUX_PIC0REG51_0102 (void)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG5[1])["PIC0REG502_01"]) {
        case 1:
            out_val = mInputValue[PIC1A_AgentController::emTAUD1TOUT3];
            break;
        case 2:
        case 3:
            out_val = mInputValue[PIC1A_AgentController::emTAUJ1TOUT3];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emDNFTSG1TSTCKE];
            break;
    }
    // Update output port
    mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG1TSTCKE, out_val);
}//}}}

/// Calculate output value of MUX(PIC0REG5n[3])
void PIC1A_Func::CalcMUX_PIC0REG5_03 (const unsigned int module, const unsigned int index)
{//{{{
    bool out_val = false;
    bool bit_value = (bool)(*PIC0REG5[module])["PIC0REG503"];
    switch (index) {
        case PIC1A_AgentController::emUS:
            if (bit_value) {
                out_val = mMUX_PIC0REG22_08Output[module];
            } else {
                out_val = mInputValue[PIC1A_AgentController::emTAUD0INT4 + (module * 54)];
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTIUS + (module * 13), out_val);
            break;
        case PIC1A_AgentController::emUC:
            if (bit_value) {
                out_val = mMUX_PIC0REG22_09Output[module];
            } else {
                out_val = mInputValue[PIC1A_AgentController::emTAUD0INT5 + (module * 54)];
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTIUC + (module * 13), out_val);
            break;
        case PIC1A_AgentController::emVS:
            if (bit_value) {
                out_val = mMUX_PIC0REG22_10Output[module];
            } else {
                out_val = mInputValue[PIC1A_AgentController::emTAUD0INT6 + (module * 54)];
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTIVS + (module * 13), out_val);
            break;
        case PIC1A_AgentController::emVC:
            if (bit_value) {
                out_val = mMUX_PIC0REG22_11Output[module];
            } else {
                out_val = mInputValue[PIC1A_AgentController::emTAUD0INT7 + (module * 54)];
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTIVC + (module * 13), out_val);
            break;
        case PIC1A_AgentController::emWS:
            if (bit_value) {
                out_val = mMUX_PIC0REG22_12Output[module];
            } else {
                out_val = mInputValue[PIC1A_AgentController::emTAUD0INT8 + (module * 54)];
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTIWS + (module * 13), out_val);
            break;
        default:    // emWC
            if (bit_value) {
                out_val = mMUX_PIC0REG22_13Output[module];
            } else {
                out_val = mInputValue[PIC1A_AgentController::emTAUD0INT9 + (module * 54)];
            }
            mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTIWC + (module * 13), out_val);
            break;
    }
}//}}}

/// Calculate output value of MUX(PIC0REG5n[4])
void PIC1A_Func::CalcMUX_PIC0REG5_04 (const unsigned int module)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG5[module])["PIC0REG504"]) {
        case 1:
            out_val = mInputValue[PIC1A_AgentController::emENCAT1ECS];
            break;
        default:    // 0
            out_val = mInputValue[PIC1A_AgentController::emENCAT0ECS];
            break;
    }
    // Update output port
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTOPS, out_val);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG1TSTOPS, out_val);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG5n[5-6])
void PIC1A_Func::CalcMUX_PIC0REG5_0506 (const unsigned int module)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG5[module])["PIC0REG506_05"]) {
        case 1:
            out_val = mMUX_PIC0REG5_08Output[module];
            break;
        case 2:
            out_val = mMUX_PIC0REG5_10Output[module];
            break;
        case 3:
            if (module == 0) {
                out_val = mInputValue[PIC1A_AgentController::emTAUD0INT5];
            } else {
                out_val = mInputValue[PIC1A_AgentController::emTAUD1INT5];
            }
            break;
        default:    // 0
            out_val = false;
            break;
    }
    // Update output port
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTOPC0, out_val);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG1TSTOPC0, out_val);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG5n[7])
void PIC1A_Func::CalcMUX_PIC0REG5_07 (const unsigned int module)
{//{{{
    bool out_val = false;
    switch ((unsigned int)(*PIC0REG5[module])["PIC0REG507"]) {
        case 1:
            out_val = false;
            break;
        default:    // 0
            if (module == 0) {
                out_val = mInputValue[PIC1A_AgentController::emTAUD0INT7];
            } else {
                out_val = mInputValue[PIC1A_AgentController::emTAUD1INT7];
            }
            break;
    }
    // Update output port
    if (module == 0) {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG0TSTOPC1, out_val);
    } else {
        mPIC1A_AgentController->UpdateOutputPort(PIC1A_AgentController::emTSG1TSTOPC1, out_val);
    }
}//}}}

/// Calculate output value of MUX(PIC0REG5n[8])
void PIC1A_Func::CalcMUX_PIC0REG5_08 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG5[module])["PIC0REG508"]) {
        case 1:
            mMUX_PIC0REG5_08Output[module] = mInputValue[PIC1A_AgentController::emENCAT2INT1];
            break;
        default:    // 0
            mMUX_PIC0REG5_08Output[module] = mInputValue[PIC1A_AgentController::emENCAT0INT1];
            break;
    }
    CalcMUX_PIC0REG5_0506(module);
}//}}}

/// Calculate output value of MUX(PIC0REG5n[10])
void PIC1A_Func::CalcMUX_PIC0REG5_10 (const unsigned int module)
{//{{{
    switch ((unsigned int)(*PIC0REG5[module])["PIC0REG510"]) {
        case 1:
            mMUX_PIC0REG5_10Output[module] = mInputValue[PIC1A_AgentController::emENCAT3INT1];
            break;
        default:    // 0
            mMUX_PIC0REG5_10Output[module] = mInputValue[PIC1A_AgentController::emENCAT1INT1];
            break;
    }
    CalcMUX_PIC0REG5_0506(module);
}//}}}

/// Calculate output value of MUX(PIC0REG5n[12])
void PIC1A_Func::CalcMUX_PIC0REG5_12 (const unsigned int module, const unsigned int index)
{//{{{
    bool bit_value = (bool)(*PIC0REG5[module])["PIC0REG512"];
    if (!bit_value) {
        if (module == 0) {
            mMUX_PIC0REG5_12Output[module][index] = mInputValue[PIC1A_AgentController::emTSG0INT0];
        } else {
            mMUX_PIC0REG5_12Output[module][index] = mInputValue[PIC1A_AgentController::emTSG1INT0];
        }
    } else {
        if (module == 0) {
            mMUX_PIC0REG5_12Output[module][index] = mInputValue[PIC1A_AgentController::emTSG0INT2 + index];
        } else {
            mMUX_PIC0REG5_12Output[module][index] = mInputValue[PIC1A_AgentController::emTSG1INT2 + index];
        }
    }
    CalcMUX_PIC0REG20_28(module, index);
}//}}}

/// Calculate output value of MUX(PIC0REG5n[13])
void PIC1A_Func::CalcMUX_PIC0REG5_13 (const unsigned int module, const unsigned int index)
{//{{{
    if ((bool)(*PIC0REG5[module])["PIC0REG513"]) {
        mMUX_PIC0REG5_13Output[module][index] = mREG513_FFOutput[module][index];
    } else {
        if (module == 0) {
            mMUX_PIC0REG5_13Output[module][index] = mInputValue[PIC1A_AgentController::emTSG0TO1 + index];
        } else {
            mMUX_PIC0REG5_13Output[module][index] = mInputValue[PIC1A_AgentController::emTSG1TO1 + index];
        }
    }
    CalcMUX_PIC0REG5_14(module, index);
    UpdateREG514_FFInput(module, index);
}//}}}

/// Calculate output value of MUX(PIC0REG5n[14])
void PIC1A_Func::CalcMUX_PIC0REG5_14 (const unsigned int module, const unsigned int index)
{//{{{
    if ((bool)(*PIC0REG5[module])["PIC0REG514"]) {
        mMUX_PIC0REG5_14Output[module][index] = mREG514_FFOutput[module][index];
    } else {
        mMUX_PIC0REG5_14Output[module][index] = mMUX_PIC0REG5_13Output[module][index];
    }
    CalcMUX_PIC0TSGOUTCTR_0(module, index);
    CalcMUX_PIC0TSGOUTCTR_Other(module, index);
}//}}}

/// Update internal input value
void PIC1A_Func::UpdateInputPort (const unsigned int index, const bool value)
{//{{{
    mInputValue[index] = value;
}//}}}

/// Calculate output value of DT sub-module
void PIC1A_Func::CalcDT (const unsigned int index, const bool is_reset)
{//{{{
    // Calculate output value
    if(is_reset) {
        mDT_Output[index] = false;
    } else {
        // Reverse output value
        mDT_Output[index] = !mDT_Output[index];
    }
    
    switch (index) {
        case PIC1A_AgentController::emDT00:
            CalcMUX_PIC0REG31_00();
            CalcMUX_PIC0REG31_0102();
            CalcMUX_PIC0REG31_0608();
            CalcMUX_PIC0REG31_2122();
            CalcMUX_PIC0REG31_23();
            break;
        case PIC1A_AgentController::emDT01:
            CalcMUX_PIC0REG31_0910();
            CalcMUX_PIC0REG31_1213();
            break;
        case PIC1A_AgentController::emDT02:
            CalcMUX_PIC0REG31_0608();
            break;
        case PIC1A_AgentController::emDT10:
            CalcMUX_PIC0REG31_03();
            CalcMUX_PIC0REG31_0405();
            CalcMUX_PIC0REG31_1213();
            CalcMUX_PIC0REG31_14();
            CalcMUX_PIC0REG31_1517();
            break;
        case PIC1A_AgentController::emDT11:
            CalcMUX_PIC0REG31_1819();
            CalcMUX_PIC0REG31_2122();
            break;
        default:    // emDT12
            CalcMUX_PIC0REG31_1517();
            break;
    }
}//}}}

/// Calculate output value of RS sub-module
void PIC1A_Func::CalcRS (const unsigned int index, const bool is_set)
{//{{{
    // Calculate output value
    mRS_Output[index] = is_set;
    
    switch (index) {
        case PIC1A_AgentController::emRS01:
            CalcMUX_PIC0REG201_0405();
            break;
        case PIC1A_AgentController::emRS02:
            CalcMUX_PIC0REG201_0607();
            CalcMUX_PIC0REG211_0001();
            CalcMUX_PIC0REG202_02();
            break;
        case PIC1A_AgentController::emRS03:
            CalcMUX_PIC0REG201_0809();
            CalcMUX_PIC0REG211_0203();
            CalcMUX_PIC0REG202_03();
            break;
        case PIC1A_AgentController::emRS04:
            CalcMUX_PIC0REG201_1011();
            CalcMUX_PIC0REG202_04();
            break;
        case PIC1A_AgentController::emRS05:
            CalcMUX_PIC0REG202_05();
            break;
        case PIC1A_AgentController::emRS06:
            CalcMUX_PIC0REG211_0405();
            CalcMUX_PIC0REG202_06();
            break;
        case PIC1A_AgentController::emRS07:
            CalcMUX_PIC0REG211_0607();
            CalcMUX_PIC0REG202_07();
            break;
        case PIC1A_AgentController::emRS11:
            CalcMUX_PIC0REG211_0405();
            break;
        case PIC1A_AgentController::emRS12:
            CalcMUX_PIC0REG211_0607();
            CalcMUX_PIC0REG201_0001();
            CalcMUX_PIC0REG212_02();
            break;
        case PIC1A_AgentController::emRS13:
            CalcMUX_PIC0REG211_0809();
            CalcMUX_PIC0REG201_0203();
            CalcMUX_PIC0REG212_03();
            break;
        case PIC1A_AgentController::emRS14:
            CalcMUX_PIC0REG211_1011();
            CalcMUX_PIC0REG212_04();
            break;
        case PIC1A_AgentController::emRS15:
            CalcMUX_PIC0REG212_05();
            break;
        case PIC1A_AgentController::emRS16:
            CalcMUX_PIC0REG201_0405();
            CalcMUX_PIC0REG212_06();
            break;
        case PIC1A_AgentController::emRS17:
            CalcMUX_PIC0REG201_0607();
            CalcMUX_PIC0REG212_07();
            break;
        default:    // emRS00, emRS10 - not used
            break;
    }
}//}}}

/// Calculate output value of RTO sub-module
void PIC1A_Func::CalcRTO (const unsigned int module, const unsigned int index, const unsigned int change_type)
{//{{{
    bool tme = (bool)(((unsigned int)(*PIC0RTO[module]) >> (index * 2)) & 0x1);
    bool tro = (bool)(((unsigned int)(*PIC0RTO[module]) >> ((index * 2) + 1)) & 0x1);
    
    switch (change_type) {
        case PIC1A_AgentController::emRTOInputChange:
            if (!tme) {
                mRTO_Output[module][index] = mRTO_INInput[module][index];
            }
            break;
        case PIC1A_AgentController::emRTOTrigger:
            if (tme) {
                mRTO_Output[module][index] = tro;
            }
            break;
        default:    // emRTOReset
            mRTO_Output[module][index] = mRTO_INInput[module][index];
            break;
    }
    ProcessWNG(module, index/2, PIC1A_AgentController::emWNGabInput);
}//}}}

/// Calculate output value of WNGnm sub-module
void PIC1A_Func::ProcessWNG (const unsigned int module, const unsigned int index, const unsigned int change_type)
{//{{{
    bool temp_ab = mRTO_Output[module][index*2] & mRTO_Output[module][index*2 + 1];
    bool temp_d = false;
    bool temp_e = false;
    bool temp_f = false;
    bool temp_h = false;

    if (mRTO_Output[module][index*2] != mRTO_Output[module][index*2 + 1]) {
        temp_d = mRTO_Output[module][index*2];
        temp_e = mRTO_Output[module][index*2 + 1];
    } else {
        temp_d = false;
        temp_e = false;
    }
    if (module == 0) {
        temp_f = mInputValue[PIC1A_AgentController::emTAUD0TOUT10 + index*2];
        temp_h = mInputValue[PIC1A_AgentController::emTAUD0TOUT11 + index*2 + 1];
    } else {
        temp_f = mInputValue[PIC1A_AgentController::emTAUD1TOUT10 + index*2];
        temp_h = mInputValue[PIC1A_AgentController::emTAUD1TOUT11 + index*2 + 1];
    }
    
    switch (change_type) {
        case PIC1A_AgentController::emWNGabInput:
            // Calculate d, e outputs
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGdOutput, temp_d, temp_e, temp_f, temp_h);
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGeOutput, temp_d, temp_e, temp_f, temp_h);
            switch (index) {
                case PIC1A_AgentController::emWNG0:
                    CalcMUX_PIC0REG22_1617(module);
                    CalcMUX_PIC0REG22_1819(module);
                    break;
                case PIC1A_AgentController::emWNG1:
                    CalcMUX_PIC0REG22_2021(module);
                    CalcMUX_PIC0REG22_2223(module);
                    break;
                default:    // emWNG2
                    CalcMUX_PIC0REG22_2425(module);
                    CalcMUX_PIC0REG22_2627(module);
                    break;
            }
            break;
        case PIC1A_AgentController::emWNGfInput:
            // Calculate i, c outputs
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGiOutput, temp_d, temp_e, temp_f, temp_h);
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGcOutput, temp_d, temp_e, temp_f, temp_h);
            break;
        case PIC1A_AgentController::emWNGhInput:
            // Calculate g, c outputs
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGgOutput, temp_d, temp_e, temp_f, temp_h);
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGcOutput, temp_d, temp_e, temp_f, temp_h);
            break;
        case PIC1A_AgentController::emWNGTrigger:
            // Update Latch outputs
            mWNG_abLatchOutput[module][index] = temp_ab;
            mWNG_dLatchOutput[module][index] = temp_d;
            mWNG_eLatchOutput[module][index] = temp_e;
            // Calculate g, i, c outputs
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGgOutput, temp_d, temp_e, temp_f, temp_h);
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGiOutput, temp_d, temp_e, temp_f, temp_h);
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGcOutput, temp_d, temp_e, temp_f, temp_h);
            break;
        case PIC1A_AgentController::emWNGMK:
            // Calculate c output
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGcOutput, temp_d, temp_e, temp_f, temp_h);
            break;
        default:    // emWNGS
            // Calculate g, i outputs
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGgOutput, temp_d, temp_e, temp_f, temp_h);
            CalcWNGOutput (module, index, PIC1A_AgentController::emWNGiOutput, temp_d, temp_e, temp_f, temp_h);
            break;
    }
}//}}}

/// Calculate output of WNGnm sub-module
void PIC1A_Func::CalcWNGOutput (const unsigned int module, const unsigned int index, const unsigned int type,
                              const bool temp_d, const bool temp_e, const bool temp_f, const bool temp_h)
{//{{{
    bool temp_c = false;
    bool temp_g = false;
    bool temp_i = false;
    switch (type) {
        case PIC1A_AgentController::emWNGdOutput:
            mWNG_dOutput[module][index] = temp_d;
            break;
        case PIC1A_AgentController::emWNGeOutput:
            mWNG_eOutput[module][index] = temp_e;
            break;
        case PIC1A_AgentController::emWNGcOutput:
            temp_c = mWNG_abLatchOutput[module][index] || (mWNG_dLatchOutput[module][index] && temp_h) || (mWNG_eLatchOutput[module][index] && temp_f);
            if ((((unsigned int)(*PIC0WNGMK[module]) >> index) & 0x1) == 0) {
                mWNG_cOutput[module][index] = temp_c;
                if ((*PIC0EN)["PIC0EN0"] == 1) {
                    mPIC1A_AgentController->TriggerSettingFlag(module, index);
                }
            } else {
                mWNG_cOutput[module][index] = false;
            }
            break;
        case PIC1A_AgentController::emWNGgOutput:
            temp_g = (!temp_h) && mWNG_dLatchOutput[module][index];
            mWNG_FFgInput[module][index] = (((unsigned int)(*PIC0WNGS[module]) >> (index*2)) & 0x1) ^ temp_g;
            mPIC1A_AgentController->WNG_FFgChange(module, index);
            break;
        default:    // emWNGiOutput
            temp_i = (!temp_f) && mWNG_eLatchOutput[module][index];
            mWNG_FFiInput[module][index] = (((unsigned int)(*PIC0WNGS[module]) >> (index*2 + 1)) & 0x1) ^ temp_i;
            mPIC1A_AgentController->WNG_FFiChange(module, index);
            break;
    }
}//}}}

/// Update WNGFn flag
void PIC1A_Func::UpdateFlag (const unsigned int module, const unsigned int index)
{//{{{
    switch (index) {
        case PIC1A_AgentController::emWNG0:
            (*PIC0WNGF[module])["PIC0WNGF0"] = mWNG_cOutput[module][index];
            break;
        case PIC1A_AgentController::emWNG1:
            (*PIC0WNGF[module])["PIC0WNGF1"] = mWNG_cOutput[module][index];
            break;
        default:    // emWNG2
            (*PIC0WNGF[module])["PIC0WNGF2"] = mWNG_cOutput[module][index];
            break;
    }
}//}}}

/// Update WNG_FFi output
void PIC1A_Func::UpdateWNG_FFiOuput (const unsigned int module, const unsigned int index)
{//{{{
    mWNG_iOutput[module][index] = mWNG_FFiInput[module][index];
    switch (index) {
        case PIC1A_AgentController::emWNG0:
            CalcMUX_PIC0REG22_09(module);
            break;
        case PIC1A_AgentController::emWNG1:
            CalcMUX_PIC0REG22_11(module);
            break;
        default:    // emWNG2
            CalcMUX_PIC0REG22_13(module);
            break;
    }
}//}}}

/// Update WNG_FFg output
void PIC1A_Func::UpdateWNG_FFgOuput (const unsigned int module, const unsigned int index)
{//{{{
    mWNG_gOutput[module][index] = mWNG_FFgInput[module][index];
    switch (index) {
        case PIC1A_AgentController::emWNG0:
            CalcMUX_PIC0REG22_08(module);
            break;
        case PIC1A_AgentController::emWNG1:
            CalcMUX_PIC0REG22_10(module);
            break;
        default:    // emWNG2
            CalcMUX_PIC0REG22_12(module);
            break;
    }
}//}}}

/// Update FN Inputs
void PIC1A_Func::UpdateFNInput (const unsigned int module, const unsigned int index, const bool is_AInput, const bool value)
{//{{{
    if (is_AInput) {
        mFN_AInput[module][index] = value;
    } else {
        mFN_BInput[module][index] = value;
    }
    CalcFNOutput(module, index);
}//}}}

/// Update FN Inputs
void PIC1A_Func::CalcFNOutput (const unsigned int module, const unsigned int index)
{//{{{
    unsigned int reg_val = ((unsigned int)(*PIC0REG23[module]) >> (index * 4)) & 0x7;
    bool a = mFN_AInput[module][index];
    bool b = mFN_BInput[module][index];
    
    switch (reg_val) {
        case 1:
            mFN_Output[module][index] = b;
            break;
        case 2:
            mFN_Output[module][index] = a ^ b;
            break;
        case 3:
            mFN_Output[module][index] = !(a || b);
            break;
        case 4:
            mFN_Output[module][index] = a && b;
            break;
        case 5:
            mFN_Output[module][index] = a || b;
            break;
        case 6:
            mFN_Output[module][index] = !a;
            break;
        case 7:
            mFN_Output[module][index] = !b;
            break;
        default:    // 0
            mFN_Output[module][index] = a;
            break;
    }
    
    switch (index) {
        case PIC1A_AgentController::emFN0:
            CalcMUX_PIC0REG22_08(module);
            break;
        case PIC1A_AgentController::emFN1:
            CalcMUX_PIC0REG22_09(module);
            break;
        case PIC1A_AgentController::emFN2:
            CalcMUX_PIC0REG22_10(module);
            break;
        case PIC1A_AgentController::emFN3:
            CalcMUX_PIC0REG22_11(module);
            break;
        case PIC1A_AgentController::emFN4:
            CalcMUX_PIC0REG22_12(module);
            break;
        default:    // emFN5
            CalcMUX_PIC0REG22_13(module);
            break;
    }
}//}}}

/// Update U1/V1/W1 Inputs of PFNnm
void PIC1A_Func::UpdatePFN_In1Input (const unsigned int module, const unsigned int index, const bool value)
{//{{{
    mPFN_In1Input[module][index] = value;
    CalcPFN(module, index, PIC1A_AgentController::emPFNIn1Input);
}//}}}

/// Calculate output value of PFN sub-module
void PIC1A_Func::CalcPFN (const unsigned int module, const unsigned int index, const unsigned int change_type)
{//{{{
    bool tout2 = false;
    if (module == 0) {
        tout2 = mInputValue[PIC1A_AgentController::emTAUD0TOUT2];
    } else {
        tout2 = mInputValue[PIC1A_AgentController::emTAUD1TOUT2];
    }
    bool in1 = mPFN_In1Input[module][index];
    bool in2 = mPFN_In2Input[module][index];
    unsigned int reg_val_1 = ((unsigned int)(*PIC0REG21[module]) >> (16 + index*4)) & 0x3;
    unsigned int reg_val_2 = ((unsigned int)(*PIC0REG21[module]) >> (18 + index*4)) & 0x3;
    bool out_val = false;
    
    switch (change_type) {
        case PIC1A_AgentController::emPFNTOUT2Input:
        case PIC1A_AgentController::emPFNIn2Input:
        case PIC1A_AgentController::emPFNWrReg:
            switch (reg_val_2) {
                case 2:
                    // !((!tout2) && (!in2)) = tout2 || in2
                    out_val = tout2 || in2;
                    break;
                case 3:
                    // (!tout2) && (!in2) = !(tout2 || in2)
                    out_val = !(tout2 || in2);
                    break;
                default:    // 0, 1
                    out_val = in2;
                    break;
            }
            UpdateFNInput(module, index*2 + 1, true, out_val);
        default:    // emPFNIn1Input
            switch (reg_val_1) {
                case 2:
                    // !(tout2 && (!in2)) = !tout2 || in2
                    out_val = (!tout2) || in2;
                    break;
                case 3:
                    out_val = tout2 && (!in2);
                    break;
                default:    // 0, 1
                    out_val = in1;
                    break;
            }
            UpdateFNInput(module, index*2, true, out_val);
            break;
    }
}//}}}

/// Update Input of FFs in PIC0REG5n[13]
void PIC1A_Func::UpdateREG513_FFInput (const unsigned int module, const unsigned int index)
{//{{{
    if (module == 0) {
        mREG513_FFInput[module][index] = mInputValue[PIC1A_AgentController::emTSG0TO1 + index];
    } else {
        mREG513_FFInput[module][index] = mInputValue[PIC1A_AgentController::emTSG1TO1 + index];
    }
    mPIC1A_AgentController->REG513_FFChange(module, index);
}//}}}

/// Update REG513_FF output
void PIC1A_Func::UpdateREG513_FFOuput (const unsigned int module, const unsigned int index)
{//{{{
    mREG513_FFOutput[module][index] = mREG513_FFInput[module][index];
    CalcMUX_PIC0REG5_13(module, index);
}//}}}

/// Update Input of FFs in PIC0REG5n[14]
void PIC1A_Func::UpdateREG514_FFInput (const unsigned int module, const unsigned int index)
{//{{{
    bool tout = false;
    if (module == 0) {
        tout = mInputValue[PIC1A_AgentController::emTAUD0TOUT4 + index/2];
    } else {
        tout = mInputValue[PIC1A_AgentController::emTAUD1TOUT4 + index/2];
    }
    
    mREG514_FFInput[module][index] = tout && mMUX_PIC0REG5_13Output[module][index];
    mPIC1A_AgentController->REG514_FFChange(module, index);
}//}}}

/// Update REG514_FF output
void PIC1A_Func::UpdateREG514_FFOuput (const unsigned int module, const unsigned int index)
{//{{{
    mREG514_FFOutput[module][index] = mREG514_FFInput[module][index];
    CalcMUX_PIC0REG5_14(module, index);
}//}}}

/// Update TSGOUTCTR_FF output
void PIC1A_Func::UpdateTSGOUTCTR_FFOuput (const unsigned int module, const unsigned int index)
{//{{{
    mTSGOUTCTR_FFOutput[module][index] = mTSGOUTCTR_FFInput[module][index];
    CalcMUX_PIC0TSGOUTCTR_0(module, index);
}//}}}

/// Calculate output value of TRGn sub-module
void PIC1A_Func::CalcTRG (const unsigned int module)
{//{{{
    bool a, b, c, d, e, f = false;
    if (module == 0) {
        a = mInputValue[PIC1A_AgentController::emTAUD0INT4];
        b = mInputValue[PIC1A_AgentController::emTAUD0INT5];
        c = mInputValue[PIC1A_AgentController::emTAUD0INT6];
        d = mInputValue[PIC1A_AgentController::emTAUD0INT7];
        e = mInputValue[PIC1A_AgentController::emTAUD0INT8];
        f = mInputValue[PIC1A_AgentController::emTAUD0INT9];
    } else {
        a = mInputValue[PIC1A_AgentController::emTAUD1INT4];
        b = mInputValue[PIC1A_AgentController::emTAUD1INT5];
        c = mInputValue[PIC1A_AgentController::emTAUD1INT6];
        d = mInputValue[PIC1A_AgentController::emTAUD1INT7];
        e = mInputValue[PIC1A_AgentController::emTAUD1INT8];
        f = mInputValue[PIC1A_AgentController::emTAUD1INT9];
    }
    
    switch ((unsigned int)(*PIC0REG20[module])["PIC0REG2020"]) {
        case 1:
            mTRG_Output[module] = a || c || e;
            break;
        default:    // 0
            mTRG_Output[module] = a || b || c || d || e || f;
            break;
    }
    CalcMUX_PIC0REG200_16();
    CalcMUX_PIC0REG210_16();
}//}}}

/// Initialize internal variables
void PIC1A_Func::Initialize (void)
{//{{{
    for (unsigned int i = 0; i < PIC1A_AgentController::emInputNum; i++) {
        mInputValue[i] = false;
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emDTnum; i++) {
        mDT_Output[i] = false;
    }
    for (unsigned int i = 0; i < PIC1A_AgentController::emRSnum; i++) {
        mRS_Output[i] = false;
    }
    for (unsigned int i = 0; i < 2; i++) {
        for (unsigned int j = 0; j < 6; j++) {
            mRTO_INInput[i][j] = false;
            mRTO_TRGInput[i][j] = false;
            mRTO_Output[i][j] = false;
            mFN_Output[i][j] = false;
            mFN_AInput[i][j] = false;
            mFN_BInput[i][j] = false;
            mREG513_FFInput[i][j] = false;
            mREG513_FFOutput[i][j] = false;
            mREG514_FFInput[i][j] = false;
            mREG514_FFOutput[i][j] = false;
            mTSGOUTCTR_FFInput[i][j] = false;
            mTSGOUTCTR_FFOutput[i][j] = false;
            mMUX_PIC0REG5_13Output[i][j] = false;
            mMUX_PIC0REG5_14Output[i][j] = false;
        }
        for (unsigned int j = 0; j < 3; j++) {
            mWNG_cOutput[i][j] = false;
            mWNG_dOutput[i][j] = false;
            mWNG_eOutput[i][j] = false;
            mWNG_gOutput[i][j] = false;
            mWNG_iOutput[i][j] = false;
            mWNG_abLatchOutput[i][j] = false;
            mWNG_dLatchOutput[i][j] = false;
            mWNG_eLatchOutput[i][j] = false;
            mWNG_FFgInput[i][j] = false;
            mWNG_FFiInput[i][j] = false;
            mPFN_In1Input[i][j] = false;
            mPFN_In2Input[i][j] = false;
            mMUX_PIC0REG5_00Output[i][j] = false;
            mMUX_PIC0REG5_12Output[i][j] = false;
        }
        for (unsigned int j = 0; j < PIC1A_AgentController::emMUX_PIC0REG2n4num; j++) {
            mMUX_PIC0REG24Input[i][j] = false;
            mMUX_PIC0REG24Output[i][j] = false;
        }
        mTRG_Output[i] = false;
        mMUX_PIC0ENCSEL40_0003Output[i] = false;
        mMUX_PIC0ENCSEL40_07Output[i] = false;
        mMUX_PIC0REG20_00Output[i] = false;
        mMUX_PIC0REG20_01Output[i] = false;
        mMUX_PIC0REG20_02Output[i] = false;
        mMUX_PIC0REG20_03Output[i] = false;
        mMUX_PIC0REG20_05Output[i] = false;
        mMUX_PIC0REG20_0809Output[i] = false;
        mMUX_PIC0REG20_1011Output[i] = false;
        mMUX_PIC0REG20_1213Output[i] = false;
        mMUX_PIC0REG20_16Output[i] = false;
        mMUX_PIC0REG20_18Output[i] = false;
        mMUX_PIC0REG22_02Output[i] = false;
        mMUX_PIC0REG22_03Output[i] = false;
        mMUX_PIC0REG22_04Output[i] = false;
        mMUX_PIC0REG22_08Output[i] = false;
        mMUX_PIC0REG22_09Output[i] = false;
        mMUX_PIC0REG22_10Output[i] = false;
        mMUX_PIC0REG22_11Output[i] = false;
        mMUX_PIC0REG22_12Output[i] = false;
        mMUX_PIC0REG22_13Output[i] = false;
        mMUX_PIC0REG22_1819Output[i] = false;
        mMUX_PIC0REG33Output[i] = false;
        mMUX_PIC0REG5_08Output[i] = false;
        mMUX_PIC0REG5_10Output[i] = false;
    }
    for (unsigned int i = 0; i < 3; i++) {
        mMUX_PIC0REG30_1617Output[i] = false;
        mMUX_PIC0REG30_1920Output[i] = false;
    }
    mMUX_PIC0REG210_04Output = false;
    mMUX_PIC0REG212_29Output = false;
    mMUX_PIC0REG30_00Output = false;
    mMUX_PIC0REG30_01Output = false;
    mMUX_PIC0REG30_18Output = false;
    mMUX_PIC0REG30_21Output = false;
    mMUX_PIC0REG31_03Output = false;
    mMUX_PIC0REG31_0608Output = false;
    mMUX_PIC0REG31_1517Output = false;
}//}}}

// vim600: set foldmethod=marker :
