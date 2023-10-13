// ---------------------------------------------------------------------
// $Id: ATU5_TIMERF_Sub_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_TIMERF_Sub_Func.h"
#include "ATU5_TIMERF_Sub_AgentController.h"
#include "ATU5_TIMERF.h"
#include "ATU5_TIMERF_Func.h"

#include "ATU5_Timer.h"

///Constructor of ATU5_TIMERF_Sub_Func class
ATU5_TIMERF_Sub_Func::ATU5_TIMERF_Sub_Func (std::string name, ATU5_TIMERF_Sub_AgentController *ATU5_TIMERF_Sub_AgentControllerPtr):
                Catu5_timerf_sub_regif(name, 32)
{//{{{
    assert(ATU5_TIMERF_Sub_AgentControllerPtr != NULL);
    mATU5_TIMERF_Sub_AgentController = ATU5_TIMERF_Sub_AgentControllerPtr;
    mATU5_TIMERF_Sub_AgentController->CommandInit(name);
    Catu5_timerf_sub_regif::set_instance_name(name);
    mModuleName = name;

    // Initialize variables
    Initialize();

}//}}}

///Destructor of ATU5_TIMERF_Sub_Func class
ATU5_TIMERF_Sub_Func::~ATU5_TIMERF_Sub_Func (void)
{//{{{
}//}}}
/// Initialize variables
void ATU5_TIMERF_Sub_Func::Initialize (void)
{//{{{
    mBGRAF  = (unsigned int)0xFFFFFFFFU;
    mBGRCF  = (unsigned int)0xFFFFFFFFU;
    mBGRDF  = (unsigned int)0xFFFFFFFFU;
}//}}}

void ATU5_TIMERF_Sub_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{

    unsigned int sub_id = mATU5_TIMERF_Sub_AgentController->mSubId;
    unsigned int addr = (unsigned int)(trans.get_address()-0x5000) & 0x3F;
    bool EdgeCntInGivenTimeRunning  = mATU5_TIMERF_Sub_AgentController->mIsEdgeCntInGivenTimeRunning;
    bool EdgeIntCntRunning          = mATU5_TIMERF_Sub_AgentController->mIsEdgeIntCntRunning;
    bool TimeDurInputLevelRunning   = mATU5_TIMERF_Sub_AgentController->mIsTimeDurInputLevelRunning;
    bool PwmWaveTimeRunning         = mATU5_TIMERF_Sub_AgentController->mIsPwmWaveTimeRunning;
    bool RotSpeedPulseRunning       = mATU5_TIMERF_Sub_AgentController->mIsRotSpeedPulseRunning;
    bool UpDownCntRunning           = mATU5_TIMERF_Sub_AgentController->mIsUpDownCntRunning;
    bool UpDownCntx4Running         = mATU5_TIMERF_Sub_AgentController->mIsUpDownCntx4Running;
    if(addr==0x10 && mATU5_TIMERF_Sub_AgentController->mIsTimerARunning) {
        if(!mATU5_TIMERF_Sub_AgentController->mECNTAFUpdated){
            unsigned int counter_value;
            counter_value = mATU5_TIMERF_Sub_AgentController->mCounterA->GetCounterValue();
            (*ECNTAF) = counter_value;
        }
    }
    else if(addr==0x18 && (EdgeCntInGivenTimeRunning || EdgeIntCntRunning || TimeDurInputLevelRunning || PwmWaveTimeRunning ||
        RotSpeedPulseRunning || UpDownCntRunning || UpDownCntx4Running)) {
        (*ECNTBF) = mATU5_TIMERF_Sub_AgentController->mECNTBF & 0xFFFF;
    }
    else if(addr==0x20 && mATU5_TIMERF_Sub_AgentController->mIsTimerCRunning) {
        if(!mATU5_TIMERF_Sub_AgentController->mECNTCFUpdated) {
            unsigned int counter_value;
            counter_value = mATU5_TIMERF_Sub_AgentController->mCounterC->GetCounterValue();
            (*ECNTCF) = counter_value;
        }
    }
    else if(addr==0x24){
        if ((bool)(*BKCRF)["ARSWCF"]) {
            (*GRCF) = mBGRCF;
        }
        else {
            (*GRCF) = mATU5_TIMERF_Sub_AgentController->mGRCF;
        }
    }
    else if(addr==0x14){
        if ((bool)(*BKCRF)["ARSWAF"] && sub_id>2) {
            (*GRAF) = mBGRAF;
        }
        else {
            (*GRAF) = mATU5_TIMERF_Sub_AgentController->mGRAF;
        }
    }
    else if(addr==0x28){
        if ((bool)(*BKCRF)["ARSWDF"] && sub_id>2) {
            (*GRDF) = mBGRDF;
        }
        else {
            (*GRDF) = mATU5_TIMERF_Sub_AgentController->mGRDF;
        }
    }
    else if(addr==0x0C && sub_id>2) {
        (*NCNTFB) = 0;
    }
    else if(addr==0x0E && sub_id>2) {
        (*NCRFB) = 0;
    }
    else if(addr==0x08 && mATU5_TIMERF_Sub_AgentController->IsNoiseCancelDurationA()) {
        SetNCNTFA(mATU5_TIMERF_Sub_AgentController->GetNoiseCounterValueA());
    }
    else if(addr==0x0C && mATU5_TIMERF_Sub_AgentController->IsNoiseCancelDurationB()) {
        SetNCNTFB(mATU5_TIMERF_Sub_AgentController->GetNoiseCounterValueB());
    }
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());

        unsigned int addr = trans.get_address() & 0xFFFF;
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

void ATU5_TIMERF_Sub_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
    unsigned int sub_id = mATU5_TIMERF_Sub_AgentController->mSubId;
    unsigned int addr = (unsigned int)(trans.get_address()-0x5000) & 0x3F;
    bool EdgeCntInGivenTimeRunning  = mATU5_TIMERF_Sub_AgentController->mIsEdgeCntInGivenTimeRunning;
    bool EdgeIntCntRunning          = mATU5_TIMERF_Sub_AgentController->mIsEdgeIntCntRunning;
    bool TimeDurInputLevelRunning   = mATU5_TIMERF_Sub_AgentController->mIsTimeDurInputLevelRunning;
    bool PwmWaveTimeRunning         = mATU5_TIMERF_Sub_AgentController->mIsPwmWaveTimeRunning;
    bool RotSpeedPulseRunning       = mATU5_TIMERF_Sub_AgentController->mIsRotSpeedPulseRunning;
    bool UpDownCntRunning           = mATU5_TIMERF_Sub_AgentController->mIsUpDownCntRunning;
    bool UpDownCntx4Running         = mATU5_TIMERF_Sub_AgentController->mIsUpDownCntx4Running;
    if(addr==0x10 && (EdgeCntInGivenTimeRunning || EdgeIntCntRunning || TimeDurInputLevelRunning || PwmWaveTimeRunning ||
        RotSpeedPulseRunning || UpDownCntRunning || UpDownCntx4Running)) {
        mATU5_TIMERF_Sub_AgentController->NotifyEvent(ATU5_TIMERF_Sub_AgentController::emECNTAFUpdated);

    }
    else if(addr==0x18 && (EdgeCntInGivenTimeRunning || EdgeIntCntRunning || TimeDurInputLevelRunning || PwmWaveTimeRunning ||
        RotSpeedPulseRunning || UpDownCntRunning || UpDownCntx4Running)) {
        mATU5_TIMERF_Sub_AgentController->mECNTBF = (*ECNTBF);
    }
    else if(addr==0x1C) {
        if(RotSpeedPulseRunning) {
            mATU5_TIMERF_Sub_AgentController->NotifyEvent(ATU5_TIMERF_Sub_AgentController::emGRBFUpdated);
        }
    }
    else if(addr==0x20 && (TimeDurInputLevelRunning || PwmWaveTimeRunning ||
        RotSpeedPulseRunning)) {
        mATU5_TIMERF_Sub_AgentController->NotifyEvent(ATU5_TIMERF_Sub_AgentController::emECNTCFUpdated);
    }
    else if(addr==0x24 && (bool)(*BKCRF)["ARSWCF"]==0){
        mATU5_TIMERF_Sub_AgentController->mGRCF = (*GRCF);
    }
    else if(addr==0x14 && (bool)(*BKCRF)["ARSWAF"]==0){
        mATU5_TIMERF_Sub_AgentController->mGRAF = (*GRAF);
        if(EdgeCntInGivenTimeRunning || UpDownCntRunning || UpDownCntx4Running) {
            mATU5_TIMERF_Sub_AgentController->NotifyEvent(ATU5_TIMERF_Sub_AgentController::emGRAFUpdated);
        }
    }
    else if(addr==0x28 && (bool)(*BKCRF)["ARSWDF"]==0){
        mATU5_TIMERF_Sub_AgentController->mGRDF = (*GRDF);
        if(PwmWaveTimeRunning && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5_TIMERF_Func->GRDFCMEnabled()) {
            mATU5_TIMERF_Sub_AgentController->NotifyEvent(ATU5_TIMERF_Sub_AgentController::emGRDFUpdated);
        }
    }
    else if(addr==0x0){
        mATU5_TIMERF_Sub_AgentController->NotifyEvent(ATU5_TIMERF_Sub_AgentController::emTCR1FUpdated);
    }
    else if(addr==0x08 && mATU5_TIMERF_Sub_AgentController->IsNoiseCancelDurationA()) {
        mATU5_TIMERF_Sub_AgentController->SetNoiseCounterValueA();
    }
    else if(addr==0x0C && mATU5_TIMERF_Sub_AgentController->IsNoiseCancelDurationB()) {
        mATU5_TIMERF_Sub_AgentController->SetNoiseCounterValueB();
    }
    else if(addr==0x0A && mATU5_TIMERF_Sub_AgentController->IsNoiseCancelDurationA()) {
        mATU5_TIMERF_Sub_AgentController->SetNoiseCORValA();
    }
    else if(addr==0x0E && mATU5_TIMERF_Sub_AgentController->IsNoiseCancelDurationB()) {
        mATU5_TIMERF_Sub_AgentController->SetNoiseCORValB();
    }
}//}}}

///Reset operation of ATU5_TIMERF_Sub_Func class
void ATU5_TIMERF_Sub_Func::Reset (const bool is_active)
{//{{{
    Catu5_timerf_sub_regif::EnableReset(is_active);
}//}}}

///Register handler command to Catu5_timerf_sub_regif class
void ATU5_TIMERF_Sub_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Catu5_timerf_sub_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Catu5_timerf_sub_regif class
            msg_return = Catu5_timerf_sub_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Virtual function of TCR1F register in Catu5_timerf_sub_regif class
void ATU5_TIMERF_Sub_Func::cb_TCR1F_MDF (RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
            mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        mATU5_TIMERF_Sub_AgentController->NotifyWriteMDFEvent();
    }
}//}}}

void ATU5_TIMERF_Sub_Func::cb_CDRF_CDRF (RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
	    mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        if((unsigned int)(*TCR1F)["MDF"]==0 || (unsigned int)(*TCR1F)["MDF"]==6 || (unsigned int)(*TCR1F)["MDF"]==7) {
            (*CDRF) = (unsigned int)(*GRBF);
        }
        else if((unsigned int)(*TCR1F)["MDF"]==1 || (unsigned int)(*TCR1F)["MDF"]==2 || (unsigned int)(*TCR1F)["MDF"]==4) {
            (*CDRF) = mATU5_TIMERF_Sub_AgentController->mGRAF;
        }
        else if((unsigned int)(*TCR1F)["MDF"]==5) {
            if(mATU5_TIMERF_Sub_AgentController->mIsRotSpeedPulseRunning) {
                (*CDRF) = mATU5_TIMERF_Sub_AgentController->mECNTBF;
            }
            else {
                (*CDRF) = (unsigned int)(*ECNTBF);
            }
        }
        if((bool)(*BKCRF)["BKENAF"]==1) {
            mBGRAF = mATU5_TIMERF_Sub_AgentController->mGRAF;
        }
        if((bool)(*BKCRF)["BKENCF"]==1) {
            mBGRCF = mATU5_TIMERF_Sub_AgentController->mGRCF;
        }
        if((bool)(*BKCRF)["BKENDF"]==1) {
            mBGRDF = mATU5_TIMERF_Sub_AgentController->mGRDF;
        }
    }
}//}}}

void ATU5_TIMERF_Sub_Func::cb_GRAF_GRAF(RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
	    mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*BKCRF)["ARSWAF"]) {
            (*GRAF) = str.pre_data;
        }
    }
}//}}}

void ATU5_TIMERF_Sub_Func::cb_GRCF_GRCF(RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
	    mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*BKCRF)["ARSWCF"]) {
            (*GRCF) = str.pre_data;
        }
    }
}//}}}

void ATU5_TIMERF_Sub_Func::cb_GRDF_GRDF(RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
	    mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*BKCRF)["ARSWDF"]) {
            (*GRDF) = str.pre_data;
        }
    }
}//}}}

void ATU5_TIMERF_Sub_Func::cb_TSCRF_ICFCF(RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
	    mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        if((bool)(*TSCRF)["ICFCF"]==1) {
            (*TSRF)["ICFF"] = 0;
        }
    }
    (*TSCRF)["ICFCF"] = 0;
}//}}}

void ATU5_TIMERF_Sub_Func::cb_TSCRF_OVFCCF (RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
	    mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        if((bool)(*TSCRF)["OVFCCF"]==1) {
            (*TSRF)["OVFCF"] = 0;
        }
    }
    (*TSCRF)["OVFCCF"] = 0;
}//}}}

void ATU5_TIMERF_Sub_Func::cb_TSCRF_OVFCBF (RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
	    mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        if((bool)(*TSCRF)["OVFCBF"]==1) {
            (*TSRF)["OVFBF"] = 0;
        }
    }
    (*TSCRF)["OVFCBF"] = 0;
}//}}}

void ATU5_TIMERF_Sub_Func::cb_BKCRF_BKENAF (RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
	    mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        if(!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5_TIMERF_Func->BKCRWENEnabled()) {
            (*BKCRF) = str.pre_data;
        }
        else {
            if(mATU5_TIMERF_Sub_AgentController->mSubId<3) {
                (*BKCRF) = (*BKCRF) & 0x22;
            }
        }
    }
}//}}}

void ATU5_TIMERF_Sub_Func::cb_TSCRF_OVFCAF (RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz"))&& (!mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->GetResetStatus("presetz_bif"))&& 
	    mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_Sub_AgentController->mATU5_TIMERF->mATU5->CheckClockPeriod("CLK_HSB")) {
        if((bool)(*TSCRF)["OVFCAF"]==1) {
            (*TSRF)["OVFAF"] = 0;
        }
    }
    (*TSCRF)["OVFCAF"] = 0;
}//}}}

bool ATU5_TIMERF_Sub_Func::SelectAinput(void) {
    if((bool)(*TCR2F)["EISELEF"]==1) {
        return false;
    }
    else {
        return true;
    }
}

unsigned int ATU5_TIMERF_Sub_Func::SelectTIAinput(void) {
    return (unsigned int)(*TCR2F)["EISELF"];
}

unsigned int ATU5_TIMERF_Sub_Func::SelectClock(void) {
    return (unsigned int)(*TCR1F)["CKSELFx"];
}


unsigned int ATU5_TIMERF_Sub_Func::OperationMode (void) {
    return (unsigned int)(*TCR1F)["MDF"];
}

unsigned int ATU5_TIMERF_Sub_Func::EdgeSelect (void){
    return (unsigned int)(*TCR1F)["EGSELF"];
}

unsigned int ATU5_TIMERF_Sub_Func::GetRegValue (const std::string name) {
    unsigned int ret =0;
    if(name=="ECNTAF") {
        ret = (unsigned int)(*ECNTAF);
    }
    else if(name=="GRBF") {
        ret = (unsigned int)(*GRBF);
    }
    else if(name=="ECNTBF") {
        ret = (unsigned int)(*ECNTBF);
    }
    else if(name=="ECNTCF") {
        ret = (unsigned int)(*ECNTCF);
    }
    return ret;
}

void ATU5_TIMERF_Sub_Func::SetRegValue (const std::string name, unsigned int value) {
    if(name=="ECNTAF") {
        (*ECNTAF) = value;
    }
    else if(name=="ECNTBF") {
        (*ECNTBF) = value;
    }
    else if(name=="ECNTCF") {
        (*ECNTCF) = value;
    }
    else if(name=="GRBF") {
        (*GRBF) = value;
    }
}

void ATU5_TIMERF_Sub_Func::SetICFF (unsigned int value) {
    (*TSRF)["ICFF"] = value;
}

bool ATU5_TIMERF_Sub_Func::ICIEnabled (void) {
    return (bool)(*TIERF)["ICIEF"];
}

bool ATU5_TIMERF_Sub_Func::OVFAEnabled (void) {
    return (bool)(*TIERF)["OVEAF"];
}
bool ATU5_TIMERF_Sub_Func::OVFBEnabled (void) {
    return (bool)(*TIERF)["OVEBF"];
}
bool ATU5_TIMERF_Sub_Func::OVFCEnabled (void) {
    return (bool)(*TIERF)["OVECF"];
}

void ATU5_TIMERF_Sub_Func::SetOVFA (unsigned int value) {
    (*TSRF)["OVFAF"] = value;
}

void ATU5_TIMERF_Sub_Func::SetOVFB ( unsigned int value) {
    (*TSRF)["OVFBF"] = value;
}

void ATU5_TIMERF_Sub_Func::SetOVFC ( unsigned int value) {
    (*TSRF)["OVFCF"] = value;
}

void ATU5_TIMERF_Sub_Func::SetNCNTFA ( unsigned int value) {
    (*NCNTFA) = value;
}

void ATU5_TIMERF_Sub_Func::SetNCNTFB ( unsigned int value) {
    (*NCNTFB) = value;
}

unsigned int ATU5_TIMERF_Sub_Func::GetNCNTFA (void) {
    return (unsigned int)(*NCNTFA);
}

unsigned int ATU5_TIMERF_Sub_Func::GetNCNTFB (void) {
    return (unsigned int)(*NCNTFB);
}

unsigned int ATU5_TIMERF_Sub_Func::GetNCRFA (void) {
    return (unsigned int)(*NCRFA);
}

unsigned int ATU5_TIMERF_Sub_Func::GetNCRFB (void) {
    return (unsigned int)(*NCRFB);
}

// vim600: set foldmethod=marker :
