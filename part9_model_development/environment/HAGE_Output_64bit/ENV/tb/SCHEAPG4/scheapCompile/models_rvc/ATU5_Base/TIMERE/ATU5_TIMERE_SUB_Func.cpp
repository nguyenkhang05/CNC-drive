// ---------------------------------------------------------------------
// $Id: ATU5_TIMERE_SUB_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_TIMERE_SUB_Func.h"
#include "ATU5_TIMERE_SUB_AgentController.h"
#include "ATU5_TIMERE.h"
#include "ATU5_TIMERE_Func.h"
#include "ATU5_Base.h"
///*********************************
/// Function     : ATU5_TIMERE_SUB
/// Description  : Constructor of ATU5_TIMERE_SUB_Func class
///*********************************
ATU5_TIMERE_SUB_Func::ATU5_TIMERE_SUB_Func (std::string name, 
                       ATU5_TIMERE_SUB_AgentController *mTIMERE_SUBAgentPtr):
                       Catu5_timere_sub_regif(name, 32)
{//{{{ 
    assert(mTIMERE_SUBAgentPtr != NULL);
    mModuleName = name;
    mTIMERE_SUBAgent = mTIMERE_SUBAgentPtr;
    mTIMERE_SUBAgent->CommandInit(name);
    Catu5_timere_sub_regif::set_instance_name(name);
    mIsShutOffEnableSub = true;
}//}}} 

///*********************************
/// Function     : ~ATU5_TIMERE_SUB_Func
/// Description  : Destructor of ATU5_TIMERE_SUB_Func class
///*********************************
ATU5_TIMERE_SUB_Func::~ATU5_TIMERE_SUB_Func (void)
{//{{{
}//}}}
///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5_TIMERE_SUB_Func::EnableReset (const bool is_active)
{//{{{ 
    Catu5_timere_sub_regif::EnableReset(is_active);
}//}}}
///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5_TIMERE_SUB_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{

    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    unsigned int addr = (unsigned int)(trans.get_address() & 0xFF);

    if (addr == 0x24 && mTIMERE_SUBAgent->mIsTimerRunning[ATU5_TIMERE_SUB_AgentController::emChannel0]) {
       mTIMERE_SUBAgent->UpdateTCNTERegValue(ATU5_TIMERE_SUB_AgentController::emChannel0);
    } else if (addr == 0x44 && mTIMERE_SUBAgent->mIsTimerRunning[ATU5_TIMERE_SUB_AgentController::emChannel1]) {
       mTIMERE_SUBAgent->UpdateTCNTERegValue(ATU5_TIMERE_SUB_AgentController::emChannel1);
    } else if (addr == 0x64 && mTIMERE_SUBAgent->mIsTimerRunning[ATU5_TIMERE_SUB_AgentController::emChannel2]) {
       mTIMERE_SUBAgent->UpdateTCNTERegValue(ATU5_TIMERE_SUB_AgentController::emChannel2);
    } else if (addr == 0x84 && mTIMERE_SUBAgent->mIsTimerRunning[ATU5_TIMERE_SUB_AgentController::emChannel3]) {
       mTIMERE_SUBAgent->UpdateTCNTERegValue(ATU5_TIMERE_SUB_AgentController::emChannel3);
    }
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///*********************************
/// Function     : write
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5_TIMERE_SUB_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}
///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ATU5_TIMERE_SUB_Func::RegisterHandler (const std::vector<std::string>& args)
{//{{{ 
    std::string msg_return = "";
    if (args.size() == 1) {
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
                msg_return = Catu5_timere_sub_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Catu5_timere_sub_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///*********************************
/// Function     : GetDMFE
/// Description  : Get the value of TSRE.DMFE bit
///*********************************
unsigned int ATU5_TIMERE_SUB_Func::GetDMFE(const unsigned int channel)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "DMFE" << channel;
    unsigned int temp = (*TSRE)[bit_name.str().c_str()];
    return temp;
}//}}}

///*********************************
/// Function     : GetCMFE
/// Description  : Get the value of TSRE.CMFE bit
///*********************************
unsigned int ATU5_TIMERE_SUB_Func::GetCMFE(const unsigned int channel)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "CMFE" << channel;
    unsigned int temp = (*TSRE)[bit_name.str().c_str()];
    return temp;
}//}}}

///*********************************
/// Function     : UpdateDTREReg
/// Description  : Update the Duty value
///*********************************
void ATU5_TIMERE_SUB_Func::UpdateDTREReg(unsigned int channel)
{//{{{
    if ((channel >= 0) && (channel < 4)) {
        (*DTRE[channel])["DTRE"] = (unsigned int)(*DRLDE[channel])["DRLDE"];
        mTIMERE_SUBAgent->mDTREValue[channel] = (unsigned int)(*DRLDE[channel])["DRLDE"];
   }
}//}}}

///*********************************
/// Function     : UpdateCYLREReg
/// Description  : Update the Cycle value
///*********************************
void ATU5_TIMERE_SUB_Func::UpdateCYLREReg(unsigned int channel)
{//{{{
    if ((channel >= 0) && (channel < 4)) {
        if((unsigned int)(*CRLDE[channel])["CRLDE"]==0){
            re_printf("warning","The value in cycle-setting ranges from 0x000001 to 0xFFFFFF \n");
        }else{
            (*CYLRE[channel])["CYLRE"] = (unsigned int)(*CRLDE[channel])["CRLDE"];
            mTIMERE_SUBAgent->mCYLREValue[channel] = (unsigned int)(*CRLDE[channel])["CRLDE"];
        }
    }
}//}}}


///*********************************
/// Function     : IsInterruptCMEE
/// Description  : Check the interrupt setting
///*********************************
bool ATU5_TIMERE_SUB_Func::IsInterruptCMEE(const unsigned int channel)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "CMEE" << channel;
    unsigned int temp = (*TIERE)[bit_name.str().c_str()];
    if (temp == 1) {
        return true;
    } else {
        return false;
    }
}//}}}

///*********************************
/// Function     : IsInterruptDMEE
/// Description  : Check the interrupt setting
///*********************************
bool ATU5_TIMERE_SUB_Func::IsInterruptDMEE(const unsigned int channel)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "DMEE" << channel;
    unsigned int temp = (*TIERE)[bit_name.str().c_str()];
    if (temp == 1) {
        return true;
    } else {
        return false;
    }
}//}}}
///*********************************
/// Function     : IsReloadEnable
/// Description  : Check the reload enable setting
/// Return       : bool
///*********************************
bool ATU5_TIMERE_SUB_Func::IsReloadEnable(const unsigned int channel)
{//{{{
    std::ostringstream channel_name;
    channel_name.str("");channel_name << "RLDENE" << channel;
    bool channel_en = (bool)(*RLDCRE)[channel_name.str().c_str()];

    if (channel_en) {
        return true;
    } else {
        return false;
    }
}//}}}

///*********************************
/// Function     : IsShutOffEnable
/// Description  : Check the ShutOff Enable
/// Return       : bool
///*********************************
bool ATU5_TIMERE_SUB_Func::IsShutOffEnable(unsigned int channel)
{//{{{
    std::ostringstream unit_name;
    unit_name.str("");unit_name << "POEEN" << channel;
    bool enable = (bool)(*POECRE)[unit_name.str().c_str()] ;
    if (enable) {
        if(!mIsShutOffEnableSub){
            re_printf("warning", "Output shutoff is not supported for subblocks 6 to 8\n");
            return false;
        }else {
            return true;
        }
    } else {
        return false;
    }
}//}}}
///*********************************
/// Function     : GetShutOffActiveInputLevel
/// Description  : Get the ShutOff Active Input Level
/// Return       : bool
///*********************************
bool ATU5_TIMERE_SUB_Func::GetShutOffActiveInputLevel()
{//{{{
    return (bool)(*POECRE)["POEPOL"] ;
    
}//}}}

///*********************************
/// Function     : GetShutOffOutputLevel
/// Description  : Get the ShutOff Output Level
/// Return       : bool
///*********************************
bool ATU5_TIMERE_SUB_Func::GetShutOffOutputLevel(unsigned int channel)
{//{{{
    std::ostringstream unit_name;
    unit_name.str("");unit_name << "PWMSLV" << channel;
    bool enable = (bool)(*SOLVLE)[unit_name.str().c_str()] ;
    
    if (enable) {
        return true;
    } else {
        return false;
    }
}//}}}
///*********************************
/// Function     : CheckTOEInversionSetting
/// Description  : Check TOE Inversion Output Setting
/// Return       : bool
///*********************************
bool ATU5_TIMERE_SUB_Func::CheckTOEInversionSetting(unsigned int channel)
{//{{{
    std::ostringstream unit_name;
    unit_name.str("");unit_name << "TONEE" << channel;
    bool enable = (*TOCRE)[unit_name.str().c_str()];
    
    if (enable) {
        return true;
    } else {
        return false;
    }
}//}}}

///*********************************
/// Function     : GetTCNTERegValue
/// Description  : Get TCNTE register value
/// Return       : unsigned int
///*********************************
unsigned int ATU5_TIMERE_SUB_Func::GetTCNTERegValue(unsigned int channel)
{//{{{
    unsigned int value = (unsigned int)(*TCNTE[channel])["TCNTE"];
    
    return value;

}//}}}

///*********************************
/// Function     : SetTCNTERegValue
/// Description  : Set TCNTE register value
/// Return       : none
///*********************************
void ATU5_TIMERE_SUB_Func::SetTCNTERegValue(unsigned int channel,unsigned int value)
{//{{{
    (*TCNTE[channel])["TCNTE"] = value & 0xFFFFFFFF;
}//}}}

///*********************************
/// Function     : TurnCMFEFlag
/// Description  : Turn CMFE flag
/// Return       : unsigned int
///*********************************
void ATU5_TIMERE_SUB_Func::TurnCMFEFlag(unsigned int channel)
{//{{{
    std::ostringstream unit_name;
    unit_name.str("");unit_name << "CMFE" << channel;
    (*TSRE)[unit_name.str().c_str()] = 0x1;

}//}}}
///*********************************
/// Function     : TurnOVFEFlag
/// Description  : Turn OVFE flag
/// Return       : unsigned int
///*********************************
void ATU5_TIMERE_SUB_Func::TurnOVFEFlag(unsigned int channel)
{//{{{
    std::ostringstream unit_name;
    unit_name.str("");unit_name << "OVFE" << channel;
    (*TSRE)[unit_name.str().c_str()] = 0x1;

}//}}}

///*********************************
/// Function     : TurnDMFEFlag
/// Description  : Turn DMFE flag
/// Return       : unsigned int
///*********************************
void ATU5_TIMERE_SUB_Func::TurnDMFEFlag(unsigned int channel)
{//{{{
    std::ostringstream unit_name;
    unit_name.str("");unit_name << "DMFE" << channel;
    (*TSRE)[unit_name.str().c_str()] = 0x1;

}//}}}
///*********************************
/// Function     : GetChannelClockRatio
/// Description  : Get Channel Clock ratio
/// Return       : unsigned int
///*********************************
unsigned int ATU5_TIMERE_SUB_Func::GetChannelClockRatio(unsigned int channel)
{//{{{
    unsigned int value = 1;
    if ((bool)(*TCRE)["PSCSEL"] == 0) {
        value = (unsigned int)(*PSCRE)["PSCE"] + 1;
    } else {
        value = (unsigned int)(*PSCCRE[channel])["PSCCE"] + 1;
    }
    return value;
}//}}}
///*********************************
/// Function     : GetSubBlockClockRatio
/// Description  : Get SubBlock Clock ratio
/// Return       : unsigned int
///*********************************
unsigned int ATU5_TIMERE_SUB_Func::GetSubBlockClockRatio()
{//{{{
    unsigned int value = 1;
    value = (unsigned int)(*TCRE)["CKSELE"] ;
    return value;
}//}}}
///*********************************
/// Function     : Call backs
/// Description  : 
/// Return       : 
///*********************************
void ATU5_TIMERE_SUB_Func::cb_DTRE_DTRE(RegCBstr str)
{//{{{
    mTIMERE_SUBAgent->mDTREValue[str.channel] = (unsigned int)(*DTRE[str.channel])["DTRE"];
    if (str.data != str.pre_data) {
        mTIMERE_SUBAgent->mChangeDTREValue[str.channel] = true;
        if(mTIMERE_SUBAgent->mIsCounterRunningEnable[str.channel]) {  // counter is running
            mTIMERE_SUBAgent->NotifyRewriteCounterEvent(str.channel);
        }
    } 

}//}}}

void ATU5_TIMERE_SUB_Func::cb_CYLRE_CYLRE(RegCBstr str)
{//{{{
    if((unsigned int)((*CYLRE[str.channel])["CYLRE"])==0){
        re_printf("warning","The value in cycle-setting ranges from 0x000001 to 0xFFFFFF \n");
        mTIMERE_SUBAgent->mCYLREValue[str.channel]=str.pre_data;
    }else{
        mTIMERE_SUBAgent->mCYLREValue[str.channel] = (unsigned int)(*CYLRE[str.channel])["CYLRE"];
    }
    if (str.data != str.pre_data) {
        mTIMERE_SUBAgent->mChangeCYLRValue[str.channel] = true;
        if(mTIMERE_SUBAgent->mIsCounterRunningEnable[str.channel]) {  // counter is running
            mTIMERE_SUBAgent->NotifyRewriteCounterEvent(str.channel);
        }
    } 
}//}}}

void ATU5_TIMERE_SUB_Func::cb_SSTRE_SSTRE0(RegCBstr str)
{//{{{
    for (unsigned int index = 0; index < 4; index++) {
        std::ostringstream unit_name;
        unit_name.str("");unit_name << "SSTRE" << index;
        if ((bool)(*SSTRE)[unit_name.str().c_str()] == 0x1) {
            mTIMERE_SUBAgent->mChannelEnable[index] = true;
        } else {
            mTIMERE_SUBAgent->mChannelEnable[index] = false;
        }
        mTIMERE_SUBAgent->NotifyPreStartTimerEvent(index);
    }
    
}//}}}

void ATU5_TIMERE_SUB_Func::cb_RLDCRE_RLDENE0(RegCBstr str)
{//{{{
}//}}}

void ATU5_TIMERE_SUB_Func::cb_FCTRGE_FCMT0(RegCBstr str)
{//{{{
    for (unsigned int index = 0; index < 4; index++) {
        if (((str.data >> index) & 0x1) == 1){
            if(!mTIMERE_SUBAgent->mIsCounterRunningEnable[index]) {
                mTIMERE_SUBAgent->NotifyPreForceCycleMatchEvent(index);
            }
        }
    }
}//}}}
void ATU5_TIMERE_SUB_Func::cb_POECRE_POECRKEY(RegCBstr str)
{//{{{
    if ((unsigned int)(*POECRE)["POECRKEY"] == 0x9C) { //accept
        (*POECRE)["POECRKEY"] = 0x0;
    } else { // not accept, retain previous data
        (*POECRE)["POECRKEY"] = 0x0;
        (*POECRE)["POEPOL"] = (str.pre_data >> 4) & 0x1;
        (*POECRE)["POEEN3"] = (str.pre_data >> 3) & 0x1;
        (*POECRE)["POEEN2"] = (str.pre_data >> 2) & 0x1;
        (*POECRE)["POEEN1"] = (str.pre_data >> 1) & 0x1;
        (*POECRE)["POEEN0"] = (str.pre_data >> 0) & 0x1;
    } 
}//}}}
void ATU5_TIMERE_SUB_Func::cb_TCNTE_TCNTE(RegCBstr str)
{//{{{
    if(mTIMERE_SUBAgent->mIsCounterRunningEnable[str.channel]) { // counter is running
        if (str.data == 0x0) { //restart PWM cycle
            mTIMERE_SUBAgent->NotifyRestartPWMCycleEvent(str.channel);
        } else { // update counter value
            mTIMERE_SUBAgent->mChangeCounterValue[str.channel] = true;
            mTIMERE_SUBAgent->NotifyRewriteCounterEvent(str.channel);
        } 
    }
}//}}}
void ATU5_TIMERE_SUB_Func::cb_TSCRE_CMFCE0(RegCBstr str)
{//{{{
    if (((str.data >> 0) & 0x1) == 1){
        (*TSRE)["CMFE0"] = 0;
        mTIMERE_SUBAgent->mIsClearCMFEStatus[0]=true;
    }
    if (((str.data >> 1) & 0x1) == 1){
        (*TSRE)["CMFE1"] = 0;
        mTIMERE_SUBAgent->mIsClearCMFEStatus[1]=true;
    }
    if (((str.data >> 2) & 0x1) == 1){
        (*TSRE)["CMFE2"] = 0;
        mTIMERE_SUBAgent->mIsClearCMFEStatus[2]=true;
    }
    if (((str.data >> 3) & 0x1) == 1){
        (*TSRE)["CMFE3"] = 0;
        mTIMERE_SUBAgent->mIsClearCMFEStatus[3]=true;
    }
    if (((str.data >> 4) & 0x1) == 1){
        (*TSRE)["OVFE0"] = 0;
        mTIMERE_SUBAgent->mIsClearOVFEStatus[0]=true;
    }
    if (((str.data >> 5) & 0x1) == 1){
        (*TSRE)["OVFE1"] = 0;
        mTIMERE_SUBAgent->mIsClearOVFEStatus[1]=true;
    }
    if (((str.data >> 6) & 0x1) == 1){
        (*TSRE)["OVFE2"] = 0;
        mTIMERE_SUBAgent->mIsClearOVFEStatus[2]=true;
    }
    if (((str.data >> 7) & 0x1) == 1){
        (*TSRE)["OVFE3"] = 0;
        mTIMERE_SUBAgent->mIsClearOVFEStatus[3]=true;
    }
    if (((str.data >> 8) & 0x1) == 1){
        (*TSRE)["DMFE0"] = 0;
        mTIMERE_SUBAgent->mIsClearDMFEStatus[0]=true;
    }
    if (((str.data >> 9) & 0x1) == 1){
        (*TSRE)["DMFE1"] = 0;
        mTIMERE_SUBAgent->mIsClearDMFEStatus[1]=true;
    }
    if (((str.data >> 10) & 0x1) == 1){
        (*TSRE)["DMFE2"] = 0;
        mTIMERE_SUBAgent->mIsClearDMFEStatus[2]=true;
    }
    if (((str.data >> 11) & 0x1) == 1){
        (*TSRE)["DMFE3"] = 0;
        mTIMERE_SUBAgent->mIsClearDMFEStatus[3]=true;
    }
    mTIMERE_SUBAgent->NotifyProcessClearingEvent();
}//}}}
void ATU5_TIMERE_SUB_Func::cb_TOCRE_TONEE0(RegCBstr str)
{//{{{
    if (((str.data >> 0) & 0x1) != ((str.pre_data >> 0) & 0x1)){
        mTIMERE_SUBAgent->mTOEToggleRequest[0] = true;
        mTIMERE_SUBAgent->NotifyPreTOEChangeRequestEvent(0);
    }
    if (((str.data >> 1) & 0x1) != ((str.pre_data >> 1) & 0x1)){
        mTIMERE_SUBAgent->mTOEToggleRequest[1] = true;
        mTIMERE_SUBAgent->NotifyPreTOEChangeRequestEvent(1);
    }
    if (((str.data >> 2) & 0x1) != ((str.pre_data >> 2) & 0x1)){
        mTIMERE_SUBAgent->mTOEToggleRequest[2] = true;
        mTIMERE_SUBAgent->NotifyPreTOEChangeRequestEvent(2);
    }
    if (((str.data >> 3) & 0x1) != ((str.pre_data >> 3) & 0x1)){
        mTIMERE_SUBAgent->mTOEToggleRequest[3] = true;
        mTIMERE_SUBAgent->NotifyPreTOEChangeRequestEvent(3);
    }
}//}}}
void ATU5_TIMERE_SUB_Func::cb_TCRE_CKSELE(RegCBstr str)
{//{{{
    if ((((str.data >> 7) & 0x1) != ((str.pre_data >> 7) & 0x1)) || (((str.data >> 0) & 0x7) != ((str.pre_data >> 0) & 0x7))){
        for (unsigned int index = 0; index < 4; index++){
            mTIMERE_SUBAgent->mChangeCountClkPeriod[index] = true;
        }
        mTIMERE_SUBAgent->NotifyChangeClockRequestEvent();
    } 
}//}}}
void ATU5_TIMERE_SUB_Func::cb_PSCRE_PSCE(RegCBstr str)
{//{{{
    if (((bool)(*TCRE)["PSCSEL"] == 0) && (((str.data >> 0) & 0x7) != ((str.pre_data >> 0) & 0x7))){
        for (unsigned int index = 0; index < 4; index++){
            mTIMERE_SUBAgent->mChangeCountClkPeriod[index] = true;
        }
        mTIMERE_SUBAgent->NotifyChangeClockRequestEvent();
    } 
}//}}}
void ATU5_TIMERE_SUB_Func::cb_PSCCRE_PSCCE(RegCBstr str)
{//{{{
    if (((bool)(*TCRE)["PSCSEL"] == 1) && (((str.data >> 0) & 0xFF) != ((str.pre_data >> 0) & 0xFF))){
        mTIMERE_SUBAgent->mChangeCountClkPeriod[str.channel] = true;
        mTIMERE_SUBAgent->NotifyChangeClockRequestEvent();
    }
}//}}}

// vim600: set foldmethod=marker :
