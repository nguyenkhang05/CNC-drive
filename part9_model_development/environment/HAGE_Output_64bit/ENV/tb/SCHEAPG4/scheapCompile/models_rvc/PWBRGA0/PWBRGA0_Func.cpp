// ---------------------------------------------------------------------
// $Id: PWBRGA0_Func.cpp,v 1.7 2020/01/15 13:23:55 landang Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "global.h"
#include "PWBRGA0_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
PWBRGA0_Func::PWBRGA0_Func (std::string name, PWBRGA0_AgentController* PWBRGA0AgentController):
                Cpwbrga0_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(PWBRGA0AgentController != NULL);
    mpPWBRGA0AgentController = PWBRGA0AgentController;
    mpPWBRGA0AgentController->CommandInit(name);
    mModuleName = name;
    Cpwbrga0_regif::set_instance_name(name);
}//}}}

/// Destructor
PWBRGA0_Func::~PWBRGA0_Func()
{//{{{
}//}}}

/// Reset Func and registers
void PWBRGA0_Func::EnableReset(const bool is_active)
{//{{{
    Cpwbrga0_regif::EnableReset(is_active);
}//}}}

/// Read API (target socket)
void PWBRGA0_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
 // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    unsigned int size = (unsigned int)trans.get_data_length();
    memset(trans.get_data_ptr(), 0, size);

    if ((!mpPWBRGA0AgentController->CheckClockPeriod("PCLKRW")) || (mpPWBRGA0AgentController->GetResetStatus())) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
                return;
            }
        } else {
            re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
            return;
        }
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Write API (target socket)
void PWBRGA0_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
 // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (!mpPWBRGA0AgentController->CheckClockPeriod("PCLKRW")) {
        if (pExtInfo != NULL) {
            if (!(debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID))) {
                re_printf("warning", "Cannot access register when clock is 0.\n");
                return;
            }
        } else {
            re_printf("warning", "Cannot access register when clock is 0.\n");
            return;
        }
    }

    unsigned int address = (unsigned int)trans.get_address();

    if (!debug) {
        reg_wr(address, trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg(address, trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Handler for register IF
void PWBRGA0_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cpwbrga0_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to regif class
            msg_return = Cpwbrga0_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Write TE bit
void PWBRGA0_Func::WriteTEBit(unsigned int value)
{//{{{
    (*PWBA0TE) = value;
}//}}}

/// Get PWBA0BRSn register value
unsigned int PWBRGA0_Func::GetPWBA0BRSRegister(unsigned int index)
{//{{{
    return (unsigned int)(*PWBA0BRSn[index]);
}//}}}

/// Get TE bit at index
bool PWBRGA0_Func::GetTEBit(unsigned int index)
{//{{{
    return (bool)(((unsigned int)(*PWBA0TE)["TE"] >> index) & 0x1);
}//}}}

/// Get SVSDIS bit value
bool PWBRGA0_Func::GetSVSDISBit()
{//{{{
    return (bool)(*PWBA0EMU)["SVSDIS"];
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of cb_PWBA0BRSn_BRS
void PWBRGA0_Func::cb_PWBA0BRSn_BRS(RegCBstr str)
{//{{{
    unsigned int index = str.channel;
    unsigned int te_value = (unsigned int)(*PWBA0TE);
    if (((te_value >> index) & 0x1) == 1){
        (*PWBA0BRSn[index]) = str.pre_data;
    }
}//}}}

/// Callback function of cb_PWBA0TS_TS
void PWBRGA0_Func::cb_PWBA0TS_TS(RegCBstr str)
{//{{{
    unsigned int ts_value = (unsigned int)(*PWBA0TS);
    unsigned int te_value = (unsigned int)(*PWBA0TE);
    unsigned int next_te_value = te_value;
    for (unsigned int i = 0; i < mpPWBRGA0AgentController->emNumClkOut; i++){
        if ( (((ts_value >> i) & 0x1) == 1) && (((te_value >> i) & 0x1) == 0) ){
            next_te_value |= (1 << i);
        }
    }
    if (next_te_value != te_value){
        mpPWBRGA0AgentController->StartCounter(next_te_value);
    }
    // always read 0
    (*PWBA0TS) = 0;
}//}}}

/// Callback function of cb_PWBA0TT_TT
void PWBRGA0_Func::cb_PWBA0TT_TT(RegCBstr str)
{//{{{ 
    unsigned int tt_value = (unsigned int)(*PWBA0TT);
    unsigned int te_value = (unsigned int)(*PWBA0TE);
    unsigned int next_te_value = te_value;
    for (unsigned int i = 0; i < mpPWBRGA0AgentController->emNumClkOut; i++){
        if ( (((tt_value >> i) & 0x1) == 1) && (((te_value >> i) & 0x1) == 1) ){
            next_te_value &= ~(1 << i);
        }
    }
    if (next_te_value != te_value){
        mpPWBRGA0AgentController->StopCounter(next_te_value);
    }
    // always read 0
    (*PWBA0TT) = 0;
}//}}}

/// Callback function of cb_PWBA0EMU_SVSDIS
void PWBRGA0_Func::cb_PWBA0EMU_SVSDIS(RegCBstr str)
{//{{{
    if ((unsigned int)(*PWBA0TE) > 0){
        (*PWBA0EMU) = str.pre_data;
    }else{
        mpPWBRGA0AgentController->UpdateEmulationStatus();
    }
}//}}}

// vim600: set foldmethod=marker :
