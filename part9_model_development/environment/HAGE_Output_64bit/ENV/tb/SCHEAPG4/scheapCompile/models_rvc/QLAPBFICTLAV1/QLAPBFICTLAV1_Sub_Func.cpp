// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1_Sub_Func.cpp,v 1.3 2020/03/24 07:16:32 duongphan Exp $
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
#include "QLAPBFICTLAV1_Sub_Func.h"

///Constructor of FCLA_Sub_Func class
QLAPBFICTLAV1_Sub_Func::QLAPBFICTLAV1_Sub_Func (std::string name,
                                                QLAPBFICTLAV1_Sub_AgentController *iQLAPBFICTLAV1_Sub_AgentControllerPtr):
Cqlapbfictlav1_sub_regif(name, 32)
{//{{{
    assert(iQLAPBFICTLAV1_Sub_AgentControllerPtr != NULL);
    mQLAPBFICTLAV1_Sub_AgentController = iQLAPBFICTLAV1_Sub_AgentControllerPtr;
    mQLAPBFICTLAV1_Sub_AgentController->CommandInit((std::string)name);
    Cqlapbfictlav1_sub_regif::set_instance_name(name);
    mModuleName = name;
    Initialize();
}//}}}

///Destructor of FCLA_Sub_Func class
QLAPBFICTLAV1_Sub_Func::~QLAPBFICTLAV1_Sub_Func (void)
{//{{{
}//}}}
// Initialize variables

void QLAPBFICTLAV1_Sub_Func::Initialize(void){
    mActiveEffectINTL = false;
    mActiveEffectINTR = false;
    mIsReset = false;
}

void QLAPBFICTLAV1_Sub_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // When clock = 0, or reset state, only Software debbugger can read register 
    if ( (!mQLAPBFICTLAV1_Sub_AgentController->mQLAPBFICTLAV1->CheckClockPeriod("PCLK")) || mIsReset ){
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
    
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

void QLAPBFICTLAV1_Sub_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    
    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only Software debbugger write
    if (!mQLAPBFICTLAV1_Sub_AgentController->mQLAPBFICTLAV1->CheckClockPeriod("PCLK")){
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

///Reset operation of FCLA_Sub_Func class
void QLAPBFICTLAV1_Sub_Func::Reset (const bool is_active)
{//{{{
    mIsReset = is_active; 
    Cqlapbfictlav1_sub_regif::EnableReset(is_active);
}//}}}

///Register handler command to Cfcla_sub_regif class
void QLAPBFICTLAV1_Sub_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cqlapbfictlav1_sub_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Cfcla_sub_regif class
            msg_return = Cqlapbfictlav1_sub_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

// Get value BYPS
bool QLAPBFICTLAV1_Sub_Func::GetBYPS(void)
{//{{{
    return (bool)((*FCLACTL)["BYPS"]);
}//}}}

// Get value INTF

bool QLAPBFICTLAV1_Sub_Func::GetINTF(void)
{//{{{
     return (bool)((*FCLACTL)["INTF"]);
}//}}}

// Get value INTR

bool QLAPBFICTLAV1_Sub_Func::GetINTR(void)
{//{{{
     return (bool)((*FCLACTL)["INTR"]);
}//}}}

// Set enable start detector

void QLAPBFICTLAV1_Sub_Func::SetStartDetector(void)
{//{{{
     mActiveEffectINTL = true;
}//}}}

//Set level detector

void QLAPBFICTLAV1_Sub_Func::SetLevelDetector(void)
{//{{{
     mActiveEffectINTR = true;
}//}}}

//Get start detector

bool QLAPBFICTLAV1_Sub_Func::GetStartDetector(void)
{//{{{
     return mActiveEffectINTL ;
}//}}}

// Get level detector

bool QLAPBFICTLAV1_Sub_Func::GetLevelDetector(void)
{//{{{
     return mActiveEffectINTR;
}//}}}

// Get INTL bit

bool QLAPBFICTLAV1_Sub_Func::GetINTL(void)
{//{{{
     return (bool)((*FCLACTL)["INTL"]);
}//}}}

void QLAPBFICTLAV1_Sub_Func::cb_FCLACTL_INTL(RegCBstr str)
{//{{{
    if(str.is_wr
    && (((str.data >> 2) & 0x1) == 0x1)
    && (((str.data >> 2) & 0x1) != ((str.pre_data >> 2) & 0x1)) ){
         mQLAPBFICTLAV1_Sub_AgentController->NotifyEffectiveBitSetting("FCLATINTL");
    } else if (str.is_wr
            && (((str.data >> 2) & 0x1) == 0x0)
            && (((str.data >> 2) & 0x1) != ((str.pre_data >> 2) & 0x1))) {
        mActiveEffectINTL = false;
        mActiveEffectINTR = false;
        mQLAPBFICTLAV1_Sub_AgentController->NotifyEdgeSignal();
        re_printf("info","edge detector signal is enable.\n");
    }
}//}}}

void QLAPBFICTLAV1_Sub_Func::cb_FCLACTL_INTF(RegCBstr str)
{//{{{
    if(str.is_wr
   && ((*FCLACTL)["INTL"] == 0)
   && (((str.data >> 1) & 0x1) != ((str.pre_data >> 1) & 0x1)) ){
        mQLAPBFICTLAV1_Sub_AgentController->NotifyEdgeDetectorSignal("FCLATINTF");
    }
}//}}}

void QLAPBFICTLAV1_Sub_Func::cb_FCLACTL_INTR(RegCBstr str)
{//{{{
    if( str.is_wr
    && (((str.data >> 2) & 0x1) == 0)
    && (((str.data >> 0) & 0x1) != ((str.pre_data >> 0) & 0x1)) ){
        mQLAPBFICTLAV1_Sub_AgentController->NotifyEdgeDetectorSignal("FCLATINTR");
    }else if( (((str.data >> 2) & 0x1) == 1)    //INTL = 'b1
           && ((((str.data >> 0) & 0x1) != ((str.pre_data >> 0) & 0x1))
              ||(((str.data >> 2) & 0x1) != ((str.pre_data >> 2) & 0x1))) ){
        mQLAPBFICTLAV1_Sub_AgentController->NotifyEffectiveBitSetting("FCLATINTR");
    }
}//}}}

void QLAPBFICTLAV1_Sub_Func::cb_FCLACTL_BYPS(RegCBstr str){
    if ( str.is_wr
      && (((str.data >> 7) & 0x1) == 0x1)
      && (((str.data >> 7) & 0x1) != ((str.pre_data >> 7) & 0x1))){
        mQLAPBFICTLAV1_Sub_AgentController->NotifyFilterSignal("BypassSignal");
         re_printf("info","filter-bypass function is enable.\n");
    } else if ( str.is_wr
            && (((str.data >> 7) & 0x1) == 0x0)
            && (((str.data >> 7) & 0x1) != ((str.pre_data >> 7) & 0x1))){
        mQLAPBFICTLAV1_Sub_AgentController->NotifyFilterSignal("FilterSignal");
         re_printf("info","filter signal is enable.\n");
    }
}//}}}
// vim600: set foldmethod=marker :
