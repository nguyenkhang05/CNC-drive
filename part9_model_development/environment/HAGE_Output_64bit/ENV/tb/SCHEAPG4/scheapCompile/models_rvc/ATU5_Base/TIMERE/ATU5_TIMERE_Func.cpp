// ---------------------------------------------------------------------
// $Id: ATU5_TIMERE_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include <cstdarg>
#include "ATU5_TIMERE_Func.h"
#include "atu5_timere_regif.h"
#include "ATU5_TIMERE_AgentController.h"
#include "ATU5_TIMERE_SUB.h"
#include "ATU5_TIMERE_SUB_Func.h"

///*********************************
/// Function     : ATU5_TIMERE
/// Description  : Constructor of ATU5_TIMERE_Func class
///*********************************
ATU5_TIMERE_Func::ATU5_TIMERE_Func (std::string name, 
                       ATU5_TIMERE_AgentController *TIMEREAgentPtr, unsigned int subblock):
                        msbE(subblock),
                       Catu5_timere_regif(name, 32, subblock)
                       //BusSlaveFuncIf()
{//{{{ 
    assert(TIMEREAgentPtr != NULL);
    mModuleName = name;
    mTIMEREAgent = TIMEREAgentPtr;
    mTIMEREAgent->CommandInit(name);
    Catu5_timere_regif::set_instance_name(name);
    mTimerEStatus = false;
}//}}} 

///*********************************
/// Function     : ~ATU5_TIMERE_Func
/// Description  : Destructor of ATU5_TIMERE_Func class
///*********************************
ATU5_TIMERE_Func::~ATU5_TIMERE_Func (void)
{//{{{
}//}}}

///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5_TIMERE_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr  = (unsigned int)trans.get_address() & 0xFFFF;
    unsigned int offsetfw = offsetAddress & 0x00FF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if ((0x4000 <= addr) && (addr < 0x4008)) {
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else if((addr >= 0x4100) && (addr <0x4F98)){
        if((addr & 0xFF) < 0x98 ){
            unsigned int sub_id = ((addr & 0x0F00) >> 8) - 1;
            if(sub_id <= msbE){
                mTIMEREAgent->mTIMERE_SUB[sub_id]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
            }
        }
    }
}//}}}

///*********************************
/// Function     : write
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5_TIMERE_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int offsetfw = offsetAddress & 0x00FF;
    unsigned int addr  = (unsigned int)trans.get_address() & 0xFFFF;
    if ((0x4000 <= addr) && (addr < 0x4008)) {
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
     } else if((addr >= 0x4100) && (addr <0x4F98)){
         if((addr & 0xFF) < 0x98 ){
            unsigned int sub_id = ((addr & 0x0F00) >> 8) - 1;
            if(sub_id <= msbE){
                mTIMEREAgent->mTIMERE_SUB[sub_id]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
            }
         }
     }
}//}}}
///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ATU5_TIMERE_Func::RegisterHandler (const bool is_common, const unsigned int sub_id ,const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
        for (unsigned int index = 0; index < msbE; index++) {//Dump Reg of all sub
            mTIMEREAgent->mTIMERE_SUB[index]->mTIE_SUBFunc->RegisterHandler(args);
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Catu5_timere_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
            for (unsigned int index = 0; index < msbE; index++) {//Set Message level to all sub
                mTIMEREAgent->mTIMERE_SUB[index]->mTIE_SUBFunc->RegisterHandler(args);
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Catu5_timere_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                for (unsigned int index = 0; index < msbE; index++) {//Set Message level to all sub
                    mTIMEREAgent->mTIMERE_SUB[index]->mTIE_SUBFunc->RegisterHandler(args);
                }
        } else {
            if (is_common) {
                msg_return = Catu5_timere_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
            } else {
                mTIMEREAgent->mTIMERE_SUB[sub_id]->mTIE_SUBFunc->RegisterHandler(args);
            }
        }
    }
}//}}}


///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5_TIMERE_Func::EnableReset (const bool is_active)
{//{{{ 
    Catu5_timere_regif::EnableReset(is_active);
    if(is_active){
         mTimerEStatus = false;
    }
}//}}}

///*********************************
/// Function     : TimerEStatus
/// Description  : TimerE is enabled from Wrapper
///*********************************
void ATU5_TIMERE_Func::TimerEStatus(const bool is_active)
{//{{{
    if (is_active) {
        mTimerEStatus = true;
        CheckStartCondition();
    } else {
        mTimerEStatus = false;
        CheckStartCondition();
    }
}//}}}

///*********************************
/// Function     : cb_TSTRE_STRE
/// Description  : Call-back function
///*********************************
void ATU5_TIMERE_Func::cb_TSTRE_STRE(RegCBstr str)
{//{{{

    CheckStartCondition();
}//}}}
///*********************************
/// Function     : CheckStartCondition
/// Description  : Call-back function
///*********************************
void ATU5_TIMERE_Func::CheckStartCondition(void)
{//{{{
    if (mTimerEStatus) {
        for (unsigned int sub= 0; sub < msbE; sub++) {
            if ((((unsigned int)((*TSTRE) >> sub)) & 0x1) != 0) {
                mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockEnable = true;
            } else {
                mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockEnable = false;
            }
            for (unsigned int index = 0; index < 4; index++) {
                mTIMEREAgent->mTIMERE_SUB[sub]->NotifyPreStartTimerEvent(index);
            }
        }
    } else {
        for (unsigned int sub= 0; sub < msbE; sub++) {
               mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockEnable = false;
               for (unsigned int index = 0; index < 4; index++) {
                   mTIMEREAgent->mTIMERE_SUB[sub]->NotifyPreStartTimerEvent(index);
               }
        }
    }
}//}}}
///*********************************
/// Function     : cb_SBRLENE_SBRLDENE
/// Description  : Call-back function
///*********************************
void ATU5_TIMERE_Func::cb_SBRLENE_SBRLDENE(RegCBstr str)
{//{{{
    for (unsigned int sub= 0; sub < msbE; sub++) {
        if ((((unsigned int)((*SBRLENE) >> sub)) & 0x1) != 0) {
            mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockReloadEnable = true;
        } else {
            mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockReloadEnable = false;
        }
    }
}//}}}

// vim600: set foldmethod=marker :
