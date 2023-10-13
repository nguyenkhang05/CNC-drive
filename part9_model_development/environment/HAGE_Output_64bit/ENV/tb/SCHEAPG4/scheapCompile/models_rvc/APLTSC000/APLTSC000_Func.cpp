// ---------------------------------------------------------------------
// $Id: APLTSC000_Func.cpp,v 1.3 2020/03/21 06:59:29 chuonghoangle Exp $
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
#include "APLTSC000_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
APLTSC000_Func::APLTSC000_Func (std::string name, APLTSC000_AgentController* APLTSC000AgentController):
                Capltsc000_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(APLTSC000AgentController != NULL);
    mpAPLTSC000AgentController = APLTSC000AgentController;
    mpAPLTSC000AgentController->CommandInit(name);
    mModuleName = name;

    Capltsc000_regif::set_instance_name(name);
    Initialize();
}//}}}

/// Destructor
APLTSC000_Func::~APLTSC000_Func()
{//{{{
}//}}}

/// Reset Func and registers
void APLTSC000_Func::EnableReset(const bool is_active)
{//{{{
    Initialize();
    Capltsc000_regif::EnableReset(is_active);
}//}}}

/// Initialize internal variables
void APLTSC000_Func::Initialize()
{//{{{
}//}}}

/// Read API (target socket)
void APLTSC000_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());

    // When clock = 0, or reset state, only Software debbugger can read register 
    if ( (!mpAPLTSC000AgentController->CheckClockPeriod()) || mpAPLTSC000AgentController->GetResetStatus("PRESETn") || mpAPLTSC000AgentController->GetResetStatus("app1_rst_n") ){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }

    // Update counter register
    mpAPLTSC000AgentController->ReturnValueCounter();

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Write API (target socket)
void APLTSC000_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
     // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    
    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only Software debbugger write
    if (! (mpAPLTSC000AgentController->CheckClockPeriod()) ){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }

    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Handler for register IF
void APLTSC000_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Capltsc000_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timera_regif class
            msg_return = Capltsc000_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Dump value of status registers
void APLTSC000_Func::DumpStatusInfo()
{//{{{ 
    printf("Status information of registers:\n");
    printf("    LTSCnCSTR = %08X\n", (unsigned int)(*LTSCnCSTR)["LTSCnCST"]);
    printf("    LTSCnCNTL = %08X\n", (unsigned int)(*LTSCnCNTL)["LTSCnCNTL"]);
    printf("    LTSCnCNTH = %08X\n", (unsigned int)(*LTSCnCNTH)["LTSCnCNTH"]);
}//}}}

/// Return LTSCnEMU register
unsigned int APLTSC000_Func::GetSVSDISReg()
{//{{{ 
    return (unsigned int)(*LTSCnEMU)["LTSCnSVSDIS"];
}//}}} 

/// Return LTSCnCSTR register
unsigned int APLTSC000_Func::GetCSTReg()
{//{{{ 
    return (unsigned int)(*LTSCnCSTR)["LTSCnCST"];
}//}}} 

/// Return LTSCnCNTH register
unsigned int APLTSC000_Func::GetCNTHReg()
{//{{{  
    return (unsigned int)(*LTSCnCNTH)["LTSCnCNTH"];
}//}}}  

/// Return LTSCnCNTL register
unsigned int APLTSC000_Func::GetCNTLReg()
{//{{{   
    return (unsigned int)(*LTSCnCNTL)["LTSCnCNTL"];
}//}}}

/// Return LTSCnRM register
unsigned int APLTSC000_Func::GetRMReg()
{//{{{   
    return (unsigned int)(*LTSCnRM)["LTSCnRM"];
}//}}}

/// Set LTSCnCNTL register
void APLTSC000_Func::SetCNTLReg(unsigned int value)
{//{{{   
    (*LTSCnCNTL)["LTSCnCNTL"] = value;
}//}}} 

/// Set LTSCnCNTH register
void APLTSC000_Func::SetCNTHReg(unsigned int value)
{//{{{  
    (*LTSCnCNTH)["LTSCnCNTH"] = value;
}//}}}  

/// Set LTSCnEMU register
void APLTSC000_Func::SetSVSDISReg(unsigned int value)
{//{{{  
    (*LTSCnEMU)["LTSCnSVSDIS"] = value;
}//}}} 

//========================================================
//============ Callback function  ========================
//========================================================

/// Callback function of LTSCnTCS_LTSCnTS bit
void APLTSC000_Func::cb_LTSCnTCS_LTSCnTS(RegCBstr str)
{//{{{ 
    // Only start the counter when it is currently stopped
    if ( ((unsigned int)(*LTSCnCSTR)["LTSCnCST"] == 0) && ((unsigned int)(*LTSCnTCS)["LTSCnTS"] == 1) ){
        (*LTSCnCSTR)["LTSCnCST"] = 1;
        mpAPLTSC000AgentController->StartCounter();
    }
    (*LTSCnTCS) = 0;
}//}}}  

/// Callback function of LTSCnTCT_LTSCnTS bit
void APLTSC000_Func::cb_LTSCnTCT_LTSCnTT(RegCBstr str)
{//{{{  
    if ((unsigned int)(*LTSCnTCT)["LTSCnTT"] == 1){
        mpAPLTSC000AgentController->ReturnValueCounter();
        (*LTSCnCSTR)["LTSCnCST"] = 0;
        mpAPLTSC000AgentController->StopCounter();
    }
    (*LTSCnTCT) = 0;
}//}}}  

/// Callback function of LTSCnRM_LTSCnRM bit
void APLTSC000_Func::cb_LTSCnRM_LTSCnRM(RegCBstr str)
{//{{{  
    if (str.pre_data != str.data){
        mpAPLTSC000AgentController->NotifyUpdateRMbit();
    }
}//}}} 

/// Callback function of LTSCnEMU_LTSCnSVSDIS bit
void APLTSC000_Func::cb_LTSCnEMU_LTSCnSVSDIS(RegCBstr str)
{//{{{
    if (str.pre_data != str.data){
        mpAPLTSC000AgentController->MonitorSVSDIS();
    }

}//}}} 

/// Callback function of LTSCnCNTL_LTSCnCNTL bit
void APLTSC000_Func::cb_LTSCnCNTL_LTSCnCNTL(RegCBstr str)
{//{{{ 
    mpAPLTSC000AgentController->UpdateCounterValue();
}//}}} 

/// Callback function of LTSCnCNTH_LTSCnCNTH bit
void APLTSC000_Func::cb_LTSCnCNTH_LTSCnCNTH(RegCBstr str)
{//{{{  
    mpAPLTSC000AgentController->UpdateCounterValue();
}//}}}  

// vim600: set foldmethod=marker :
