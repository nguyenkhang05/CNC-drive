// ---------------------------------------------------------------------
// $Id: PWSELD0_Func.cpp,v 1.7 2020/03/06 04:32:06 landang Exp $
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
#include "PWSELD0_Func.h"
#include "global.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
PWSELD0_Func::PWSELD0_Func (std::string name, PWSELD0_AgentController* PWSELD0AgentController):
                Cpwseld0_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(PWSELD0AgentController != NULL);
    mpPWSELD0AgentController = PWSELD0AgentController;
    mpPWSELD0AgentController->CommandInit(name);
    mModuleName = name;
    Cpwseld0_regif::set_instance_name(name);
}//}}}

/// Destructor
PWSELD0_Func::~PWSELD0_Func()
{//{{{
}//}}}

/// Reset Func and registers
void PWSELD0_Func::EnableReset(const bool is_active)
{//{{{
    Cpwseld0_regif::EnableReset(is_active);
}//}}}

/// Read API (target socket)
void PWSELD0_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
 // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    unsigned int size = (unsigned int)trans.get_data_length();
    memset(trans.get_data_ptr(), 0, size);

    if ((!mpPWSELD0AgentController->CheckClockPeriod()) || (mpPWSELD0AgentController->GetResetStatus())) {
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
void PWSELD0_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
 // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    if (!mpPWSELD0AgentController->CheckClockPeriod()) {
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
void PWSELD0_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cpwseld0_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to regif class
            msg_return = Cpwseld0_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Store PWG_TRGOUT request index to QUE0-7
bool PWSELD0_Func::StoreTriggerToQueue(unsigned int trgout_index)
{//{{{
    bool ret = false;
    // Store trgout_index to empty queue
    for (unsigned int i = 0; i < 8; i++){
        if ((unsigned int)(*PWS0QUE[i]) == mpPWSELD0AgentController->emEmpty){// Empty
            (*PWS0QUE[i]) = trgout_index;
            ret = true;
            if (i >= 1){
                (*PWS0STR)["QNE"] = 1;// only set QNE when QUEi (i >=1) has data
            }
            break;
        }
    }

    // Check status of queue
    if ((unsigned int)(*PWS0QUE[7]) != mpPWSELD0AgentController->emEmpty){// queue full
        (*PWS0STR)["QFL"] = 1;
        mpPWSELD0AgentController->AssertInterrupt();
    }
    return ret;
}//}}}

/// Get SLADx bit of a trigger channel PWG
unsigned int PWSELD0_Func::GetSLADxBitOfChannel(unsigned int trg_pwg_ch)
{//{{{
    return (unsigned int)(*PWS0PVCR[trg_pwg_ch])["SLADx"];
}//}}}

/// Get and merge value of PVCR from bits in PWS0PVCR register of a trigger channel
unsigned int PWSELD0_Func::GetPVCRValueOfChannel(unsigned int trg_pwg_ch)
{//{{{
    unsigned int vrdt_5_0 = (unsigned int)(*PWS0PVCR[trg_pwg_ch])["GCTRLx"];
    unsigned int vrdt_9_6 = (unsigned int)(*PWS0PVCR[trg_pwg_ch])["VCULLMTBSx"];
    unsigned int vrdt_12_10 = (unsigned int)(*PWS0PVCR[trg_pwg_ch])["MPXVx"];
    unsigned int vrdt_13 = (unsigned int)(*PWS0PVCR[trg_pwg_ch])["MPXEx"];
    unsigned int vrdt_17_14 = (unsigned int)(*PWS0PVCR[trg_pwg_ch])["WTTSx"];

    return ( (vrdt_17_14 << 14) | (vrdt_13 << 13) | (vrdt_12_10 << 10) | (vrdt_9_6 << 6) | vrdt_5_0 );
}//}}}

/// Get ARSE bit
unsigned int PWSELD0_Func::GetARSEBit()
{//{{{
    return (unsigned int)(*PWS0CTL)["ARSE"];
}//}}}

/// Store converted value of ADC to PWS0PWDDIR register at trigger channel, update WFLG flag, OWE flag
void PWSELD0_Func::StorePWS0PWDDIR(unsigned int trg_pwg_ch, unsigned int value)
{//{{{
    (*PWS0PWDDIR[trg_pwg_ch])["PWDDR"] = value;
    if ((unsigned int)(*PWS0PWDDIR[trg_pwg_ch])["WFLG"] == 0){
        (*PWS0PWDDIR[trg_pwg_ch])["WFLG"] = 1;
    }else{
        (*PWS0PWDDIR[trg_pwg_ch])["OWE"] = 1;
    }
}//}}}

/// Shift queue from QUE7->QUE0
void PWSELD0_Func::ShiftQueue()
{//{{{
    (*PWS0QUE[0]) = (unsigned int)(*PWS0QUE[1]);
    (*PWS0QUE[1]) = (unsigned int)(*PWS0QUE[2]);
    (*PWS0QUE[2]) = (unsigned int)(*PWS0QUE[3]);
    (*PWS0QUE[3]) = (unsigned int)(*PWS0QUE[4]);
    (*PWS0QUE[4]) = (unsigned int)(*PWS0QUE[5]);
    (*PWS0QUE[5]) = (unsigned int)(*PWS0QUE[6]);
    (*PWS0QUE[6]) = (unsigned int)(*PWS0QUE[7]);
    (*PWS0QUE[7]) = mpPWSELD0AgentController->emEmpty;

    // Clear QNE if PWS0QUE1 empty
    if ( (unsigned int)(*PWS0QUE[1]) == mpPWSELD0AgentController->emEmpty ){
        (*PWS0STR)["QNE"] = 0;
    }
}//}}}

/// Get SVSDIS bit value
unsigned int PWSELD0_Func::GetSVSDISBit()
{//{{{
    return (unsigned int)(*PWS0EMU)["SVSDIS"];
}//}}}

/// Get value of QUE0
unsigned int PWSELD0_Func::GetTopQueue()
{//{{{
    return (unsigned int)(*PWS0QUE[0]);
}//}}}

/// Clear flags in PWS0PWDDIR register
void PWSELD0_Func::ClearFlag(unsigned int channel)
{//{{{
    (*PWS0PWDDIR[channel])["WFLG"] = 0;
    (*PWS0PWDDIR[channel])["OWE"] = 0;
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================
/// Callback function of cb_PWS0CTL_ENBL
void PWSELD0_Func::cb_PWS0CTL_ENBL(RegCBstr str)
{//{{{
    if ((unsigned int)(*PWS0CTL)["ENBL"] == 0){
        // Clear status
        (*PWS0STR) = 0;
        for (unsigned int i = 0; i < 8; i++){
            (*PWS0QUE[i]) = mpPWSELD0AgentController->emEmpty;
        }
        for (unsigned int i = 0; i < mpPWSELD0AgentController->emNumConnectedPWG; i++){
            (*PWS0PWDDIR[i])["OWE"] = 0;
            (*PWS0PWDDIR[i])["WFLG"] = 0;
        }
        mpPWSELD0AgentController->EnablePWS(0);
    }else{
        mpPWSELD0AgentController->EnablePWS(1);
    }
}//}}}

/// Callback function of cb_PWS0STC_CLNE
void PWSELD0_Func::cb_PWS0STC_CLNE(RegCBstr str)
{//{{{
    if ((unsigned int)(*PWS0STC)["CLFL"] == 1){
        (*PWS0STR)["QFL"] = 0;
    }

    if ((unsigned int)(*PWS0STC)["CLNE"] == 1){
        (*PWS0STR)["QNE"] = 0;
    }

    // Always read 0
    (*PWS0STC) = 0;
}//}}}

/// Callback function of cb_PWS0EMU_SVSDIS
void PWSELD0_Func::cb_PWS0EMU_SVSDIS(RegCBstr str)
{//{{{
    if ((unsigned int)(*PWS0QUE[0]) != mpPWSELD0AgentController->emEmpty){
        re_printf("warning", "Cannot change PWS0EMU.SVSDIS bit when PWS0QUE0 is not empty.\n");
        (*PWS0EMU) = str.pre_data;
    }
}//}}}

/// Callback function of cb_PWS0PVCR_GCTRLx
void PWSELD0_Func::cb_PWS0PVCR_GCTRLx(RegCBstr str)
{//{{{
    (*PWS0PWDDIR[str.channel])["MP"] = (unsigned int)(*PWS0PVCR[str.channel])["MPXEx"];
    (*PWS0PWDDIR[str.channel])["MPXV"] = (unsigned int)(*PWS0PVCR[str.channel])["MPXVx"];
    (*PWS0PWDDIR[str.channel])["ID"] = (unsigned int)(*PWS0PVCR[str.channel])["GCTRLx"];
    if ((unsigned int)(*PWS0PVCR[str.channel])["SLADx"] == 3){
        re_printf("warning", "It is prohibited to set 0x3 to SLADx bit. Value 0x2 is used as default.\n");
        (*PWS0PVCR[str.channel])["SLADx"] = 2;
    }
}//}}}

/// Callback function of cb_PWS0PWDDIR_PWDDR
void PWSELD0_Func::cb_PWS0PWDDIR_PWDDR(RegCBstr str)
{//{{{
    mpPWSELD0AgentController->ClearFlag(str.channel);
}//}}}

// vim600: set foldmethod=marker :
