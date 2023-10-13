// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERC_SUB_Func.cpp,v 1.0 2016/07/12 01:45:48 quangthinhnguyen Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "ATU5010_TIMERC_SUB_Func.h"
#include "ATU5010_TIMERC_SUB_AgentController.h"
#include "ATU5010_TIMERC.h"
#include "ATU5010_TIMERC_Func.h"

///*********************************
/// Function     : ATU5010_TIMERC_SUB
/// Description  : Constructor of ATU5010_TIMERC_SUB_Func class
///*********************************
ATU5010_TIMERC_SUB_Func::ATU5010_TIMERC_SUB_Func (std::string name, 
                       ATU5010_TIMERC_SUB_AgentController *TIMERC_SUBAgentPtr, unsigned int sub_id):
                       Catu5010_timerc_sub_regif(name, 32, sub_id)
{//{{{ 
    assert(TIMERC_SUBAgentPtr != NULL);
    mTIMERC_SUBAgent = TIMERC_SUBAgentPtr;
    mTIMERC_SUBAgent->CommandInit(name);
    Catu5010_timerc_sub_regif::set_instance_name(name);
    mModuleName = name;
    mIsCounterWorking = false;
    mSub_id = sub_id;
    mPre_CntClk = 0;
    mPre_PWMbit = 0;
    mPre_TCRC_CLRC = false;
}//}}} 

///*********************************
/// Function     : ~ATU5010_TIMERC_SUB_Func
/// Description  : Destructor of ATU5010_TIMERC_SUB_Func class
///*********************************
ATU5010_TIMERC_SUB_Func::~ATU5010_TIMERC_SUB_Func (void)
{//{{{
}//}}}

///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5010_TIMERC_SUB_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    unsigned int addr = (unsigned int)(trans.get_address() & 0xFF);
    if ((((0x38 <= addr) && (addr < 0x3C)) ||( (0xB8 <= addr) && (addr < 0xBC))) && mIsCounterWorking) {
        SetTCNTC(mTIMERC_SUBAgent->GetCounterValue());
    } else if ((((0x70 <= addr) && (addr < 0x72)) || (((0xF0 <= addr) && (addr < 0xF2)))) && mTIMERC_SUBAgent->mTIMERC->mTimerC_Func->IsNoiseCancelEn(mSub_id, 0)) {
        SetNCNTC(0,mTIMERC_SUBAgent->GetNoiseCounterValue(0));
    } else if ((((0x74 <= addr) && (addr < 0x76)) || (((0xF4 <= addr) && (addr < 0xF6)))) && mTIMERC_SUBAgent->mTIMERC->mTimerC_Func->IsNoiseCancelEn(mSub_id, 1)) {
        SetNCNTC(1,mTIMERC_SUBAgent->GetNoiseCounterValue(1));
    } else if ((((0x78 <= addr) && (addr < 0x7A)) || (((0xF8 <= addr) && (addr < 0xFA)))) && mTIMERC_SUBAgent->mTIMERC->mTimerC_Func->IsNoiseCancelEn(mSub_id, 2)) {
        SetNCNTC(2,mTIMERC_SUBAgent->GetNoiseCounterValue(2));
    } else if ((((0x7C <= addr) && (addr < 0x7E)) || (((0xFC <= addr) && (addr < 0xFE)))) && mTIMERC_SUBAgent->mTIMERC->mTimerC_Func->IsNoiseCancelEn(mSub_id, 3)) {
        SetNCNTC(3,mTIMERC_SUBAgent->GetNoiseCounterValue(3));
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
void ATU5010_TIMERC_SUB_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int )(trans.get_address() & 0xFF);
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        if (((0x22 <=addr) && (addr < 0x24)) || ((0xA2 <= addr) && (addr < 0xA4))) {
            //not retained value in TSCRC
            Callback_TSCRC();
        } else if (((0x24 <= addr) && (addr < 0x26)) ||((0xA4 <= addr) && (addr < 0xA6))) {
            Callback_TCRC();
        }
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        unsigned int temp =  (unsigned int)(trans.get_data_length());
        if ((((addr == 0x21) && (temp == 4)) || 
            ((addr == 0x22) && ((temp == 3) || (temp == 4))) ||
            ((addr == 0x23) && ((2 <= temp)) && (temp < 5)) ||
            ((0x24 <= addr) && (addr < 0x26))) || //sub 0x2X and sub 0xAX
            (((addr == 0xA1) && (temp == 4)) ||
            ((addr == 0xA2) && ((temp == 3) || (temp == 4))) ||
            ((addr == 0xA3) && ((2 <= temp) && (temp < 5))) ||
            ((0xA4 <= addr) && (addr < 0xA6)))) {
            Callback_TCRC();
        }
        if ((((0x22 <= addr) && (addr < 0x24)) ||
            ((addr == 0x20) && ((temp == 3) || (temp == 4))) ||
            ((addr == 0x21) && ((1 < temp) && (temp <5)))) ||
            (((0xA2 <= addr) && (addr < 0xA4)) ||
            ((addr == 0xA0) && ((temp == 3) || (temp == 4))) ||
            ((addr == 0xA1) && ((1 < temp) && (temp <5))) ||
            (((addr == 0x1F) || (addr == 0x9F)) && (temp == 4)))) {
            Callback_TSCRC();
        }
    }

    if (((0x70 <= addr) && (addr < 0x72)) || ((0xF0 <= addr) && (addr < 0xF2))) {
        Callback_NCNTCn(0);
    } else if (((0x74 <= addr) && (addr < 0x76)) || ((0xF4 <= addr) && (addr < 0xF6))) {
        Callback_NCNTCn(1);
    } else if (((0x78 <= addr) && (addr < 0x7A)) || ((0xF8 <= addr) && (addr < 0xFA))) {
        Callback_NCNTCn(2); 
    } else if (((0x7C <= addr) && (addr < 0x7E)) || ((0xFC <= addr) && (addr < 0xFE))) {
        Callback_NCNTCn(3); 
    } else if (((0x72 <= addr) && (addr < 0x74)) || ((0xF2 <= addr) && (addr < 0xF4))) {
        Callback_NCRCn(0);
    } else if (((0x76 <= addr) && (addr < 0x78)) || ((0xF6 <= addr) && (addr < 0xF8))) {
        Callback_NCRCn(1);
    } else if (((0x7A <= addr) && (addr < 0x7C)) || ((0xFA <= addr) && (addr < 0xFC))) {
        Callback_NCRCn(2);
    } else if (((0x7E <= addr) && (addr < 0x80)) || ((0xFE <= addr) && (addr < 0x100))) {
        Callback_NCRCn(3);
    }

        if (((0x40 <= addr) && (addr < 0x44)) || ((0xC0 <= addr) && (addr < 0xC4))) {
            Callback_GRC(0);
        } else if (((0x44 <= addr) && (addr < 0x48)) || ((0xC4 <= addr) && (addr < 0xC8))) {
            Callback_GRC(1);
        } else if (((0x48 <= addr) && (addr < 0x4C)) || ((0xC8 <= addr) && (addr < 0xCC))) {
            Callback_GRC(2);
        } else if (((0x4C <= addr) && (addr < 0x50)) || ((0xCC <= addr) && (addr < 0xD0))) {
            Callback_GRC(3);
        } else if (((0x50 <= addr) && (addr < 0x54)) || ((0xD0 <= addr) && (addr < 0xD4))) {
            Callback_OCRC(0);
        } else if (((0x54 <= addr) && (addr < 0x58)) || ((0xD4 <= addr) && (addr < 0xD8))) {
            Callback_OCRC(1);
        } else if (((0x58 <= addr) && (addr < 0x5C)) || ((0xD8 <= addr) && (addr < 0xDC))) {
            Callback_OCRC(2);
        } else if (((0x5C <= addr) && (addr < 0x60)) || ((0xDC <= addr) && (addr < 0xE0))) {
            Callback_OCRC(3);
        }
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ATU5010_TIMERC_SUB_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Catu5010_timerc_sub_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Catu5010_timerc_sub_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///*********************************
/// Function     : SetTCNTC
/// Description  : Set the value to TCNTC
///*********************************
void ATU5010_TIMERC_SUB_Func::SetTCNTC(const unsigned int value)
{//{{{
    (*TCNTC) = value;
}//}}}

///*********************************
/// Function     : GetTCNTC
/// Description  : Get The value of TCNTC
///*********************************
unsigned int ATU5010_TIMERC_SUB_Func::GetTCNTC(void)
{//{{{
    return ((unsigned int)(*TCNTC));
}//}}}

///*********************************
/// Function     : SetCounterStatus
/// Description  : Set counter Status
///*********************************
void ATU5010_TIMERC_SUB_Func::SetCounterStatus(const bool flag)
{//{{{
    mIsCounterWorking = flag;
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5010_TIMERC_SUB_Func::EnableReset (const bool is_active)
{//{{{ 
    Catu5010_timerc_sub_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : UpdateGRCReg
/// Description  : Update the Capture Counter to GRC
///*********************************
void ATU5010_TIMERC_SUB_Func::UpdateGRCReg(const unsigned int value, const unsigned int channel)
{//{{{
    (*GRCn[channel]) = value & 0xFFFFFFFF;
    (*GMRCn[channel]) = value & 0xFFFFFFFF;
}//}}}

///*********************************
/// Function     : UpdateOCRCReg
/// Description  : Update the Capture Counter to OCRC
///*********************************
void ATU5010_TIMERC_SUB_Func::UpdateOCRCReg(const unsigned int value, const unsigned int channel)
{//{{{
    (*OCRCn[channel]) = value & 0xFFFFFFFF;
    (*OCMRCn[channel]) = value & 0xFFFFFFFF;
}//}}} 

///*********************************
/// Function     : TurnOVFFlag
/// Description  : Turn the flag of OVF
///*********************************
void ATU5010_TIMERC_SUB_Func::TurnOVFFlag(void)
{//{{{
    (*TSRC)["OVFC"] = 1;
}//}}}

///*********************************
/// Function     : TurnGRCFlag
/// Description  : Turn the flag of GRC
///*********************************
void ATU5010_TIMERC_SUB_Func::TurnGRCFlag(const unsigned int channel, const unsigned int value)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "IMFC" << channel;
    (*TSRC)[bit_name.str().c_str()] = value;
}//}}}

///*********************************
/// Function     : TurnOCRCFlag
/// Description  : Turn the flag of OCRC
///*********************************
void ATU5010_TIMERC_SUB_Func::TurnOCRCFlag(const unsigned int channel, const unsigned int value)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "OCMFC" << channel;
    (*TSRC)[bit_name.str().c_str()] = value;
}//}}}

///*********************************
/// Function     : IsInterruptOVF
/// Description  : Check the interrupt setting
///*********************************
bool ATU5010_TIMERC_SUB_Func::IsInterruptOVF(void)
{//{{{
    return (bool)((*TIERC)["OVCE"]);
}//}}}

///*********************************
/// Function     : IsInterruptGRC
/// Description  : Check the interrupt setting
///*********************************
bool ATU5010_TIMERC_SUB_Func::IsInterruptGRC(const unsigned int channel)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "GRCE" << channel;
    return (bool)((*TIERC)[bit_name.str().c_str()]);
}//}}}

///*********************************
/// Function     : IsInterruptOCRC
/// Description  : Check the interrupt setting
///*********************************
bool ATU5010_TIMERC_SUB_Func::IsInterruptOCRC(const unsigned int channel)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "OCRCE" << channel;
    return (bool)((*TIERC)[bit_name.str().c_str()]);
}//}}}

///*********************************
/// Function     : CheckPWM
/// Description  : Check the PWM status
///*********************************
void ATU5010_TIMERC_SUB_Func::CheckPWM(void)
{//{{{
    if ((*TCRC)["PWM0"] == 1) {
        if ((mTIMERC_SUBAgent->mChannelMode[0] != ATU5010_TIMERC_SUB_AgentController::emCapture) &&
             (mTIMERC_SUBAgent->mChannelMode[1] != ATU5010_TIMERC_SUB_AgentController::emCapture) &&
             (mTIMERC_SUBAgent->mChannelMode[2] != ATU5010_TIMERC_SUB_AgentController::emCapture) &&
             (mTIMERC_SUBAgent->mChannelMode[3] != ATU5010_TIMERC_SUB_AgentController::emCapture)) {
            mTIMERC_SUBAgent->mChannelMode[0] = ATU5010_TIMERC_SUB_AgentController::emPWM;
            mTIMERC_SUBAgent->mChannelMode[1] = ATU5010_TIMERC_SUB_AgentController::emPWM;
            mTIMERC_SUBAgent->mChannelMode[2] = ATU5010_TIMERC_SUB_AgentController::emPWM;
            mTIMERC_SUBAgent->mChannelMode[3] = ATU5010_TIMERC_SUB_AgentController::emPWM;
            mTIMERC_SUBAgent->print_msg("info","PWM is set sucessfully.\n");
        } else {
            mTIMERC_SUBAgent->print_msg("warning","PWM does not work in Input Capture Mode.\n");
        }
    } else { //Off PWM
        if (mTIMERC_SUBAgent->mChannelMode[0] == ATU5010_TIMERC_SUB_AgentController::emPWM) { //edit 0818
            for (unsigned int i = 0; i < 4; i++) {
                if ((0 <=  mTIMERC_SUBAgent->mChannelValue[i]) && (mTIMERC_SUBAgent->mChannelValue[i] <= 3)) {
                    mTIMERC_SUBAgent->mChannelMode[i] = ATU5010_TIMERC_SUB_AgentController::emComp;
                } else {
                    mTIMERC_SUBAgent->mChannelMode[i] = ATU5010_TIMERC_SUB_AgentController::emOneshot;
                }
            }
            mTIMERC_SUBAgent->OffPWM();//add 0815
        }
    }
}//}}}

///*********************************
/// Function     : TimingClearCounter
/// Description  : Clear counter with PCLK or count clk
///*********************************
bool ATU5010_TIMERC_SUB_Func::TimingClearCounter(void)
{//{{{
    return (bool)((*TCRC)["CLRCSELC"]);
}//}}}

///*********************************
/// Function     : SetMode
/// Description  : Set the operating model
///*********************************
void ATU5010_TIMERC_SUB_Func::SetModeChannel(const unsigned int channel, const unsigned int value)
{//{{{
    if (((value >= 4)&&(value <8)) ||
        ((value > 11)&&(value < 16))) {
        mTIMERC_SUBAgent->mChannelMode[channel] = ATU5010_TIMERC_SUB_AgentController::emCapture;
        mTIMERC_SUBAgent->print_msg("info","Channel%i is in Input Capture Mode", channel);
        mTIMERC_SUBAgent->ControlTOCE(channel,false); // control TOCE port
    } else if ((0 <= value) && (value <4)) {
        mTIMERC_SUBAgent->mChannelMode[channel] = ATU5010_TIMERC_SUB_AgentController::emComp;
        mTIMERC_SUBAgent->print_msg("info","Channel%i is in Compare Match Mode", channel);
        mTIMERC_SUBAgent->ControlTOCE(channel,true);// control TOCE port
    } else {
        mTIMERC_SUBAgent->mChannelMode[channel] = ATU5010_TIMERC_SUB_AgentController::emOneshot;
        mTIMERC_SUBAgent->print_msg("info","Channel%i is in OneShot Mode", channel);
        mTIMERC_SUBAgent->ControlTOCE(channel,true);// control TOCE port
    }
    CheckPWM();
}//}}}

///*********************************
/// Function     : SetModeInRunning
/// Description  : Set the operating model while counter is working
///*********************************
void ATU5010_TIMERC_SUB_Func::SetModeInRunning(const bool mode_ch0, const bool mode_ch1, const bool mode_ch2, const bool mode_ch3)
{//{{{
    if (mode_ch0) {
        mTIMERC_SUBAgent->mChannelValue[0] = (unsigned int)(*TIORC)["IOC0"];
    }
    if (mode_ch1) {
        mTIMERC_SUBAgent->mChannelValue[1] = (unsigned int)(*TIORC)["IOC1"];
    } 
    if (mode_ch2) {
        mTIMERC_SUBAgent->mChannelValue[2] = (unsigned int)(*TIORC)["IOC2"];
    }
    if (mode_ch3) {
        mTIMERC_SUBAgent->mChannelValue[3] = (unsigned int)(*TIORC)["IOC3"];
    }
    mTIMERC_SUBAgent->SetModeDuringRunning();
    mTIMERC_SUBAgent->print_msg("info","Operating Mode is changed successfully.\n");
}//}}}

///*********************************
/// Function     : Callback_TSCRC
/// Description  : Callback for TSCRC
///*********************************
void ATU5010_TIMERC_SUB_Func::Callback_TSCRC(void)
{//{{{
    if (((unsigned int)(*TSCRC)["OVFCC"] == 1)) {
        (*TSRC)["OVFC"] = 0;
        mTIMERC_SUBAgent->print_msg("info","Clear overflow flag.\n");
    }
    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream bit_name1;
        std::ostringstream bit_name2;
        bit_name1.str(""); bit_name1 << "OCMFCC" << index;
        bit_name2.str(""); bit_name2 << "IMFCC" << index;
        if ((unsigned int)(*TSCRC)[bit_name1.str().c_str()] == 1) {
            TurnOCRCFlag(index,0);
            mTIMERC_SUBAgent->print_msg("info","Clear input capture flag.\n");
        }
        if ((unsigned int)(*TSCRC)[bit_name2.str().c_str()] == 1) {
            TurnGRCFlag(index,0);
            mTIMERC_SUBAgent->print_msg("info","Clear output compare flag.\n");
        }
    }
    (*TSCRC) = 0;
}//}}}

///*********************************
/// Function     : Callback_TCRC
/// Description  : Callback for TCRC
///*********************************
void ATU5010_TIMERC_SUB_Func::Callback_TCRC(void)
{//{{{
    if ((((unsigned int)(*TCRC)["PWM0"] == 1) && (mPre_PWMbit == 0)) || 
        (((unsigned int)(*TCRC)["PWM0"] == 0) && (mPre_PWMbit == 1))) {
        CheckPWM();
        mPre_PWMbit = (unsigned int)(*TCRC)["PWM0"];
    } //Control PWM

    if (((unsigned int)(((*TCRC) >> 12) & 0xF) == 0) || ((unsigned int)(((*TCRC) >> 4) & 0xF) == 0)) {
        mTIMERC_SUBAgent->print_msg("warning","Cannot write 0 to this bit.\n");
    }
    for (unsigned int channel = 0; channel <= 3; channel++) {
        std::ostringstream bit_name1;
        std::ostringstream bit_name2;
        bit_name1.str("");bit_name1 << "FOCMC" << channel;
        bit_name2.str("");bit_name2 << "FCMC" << channel;
        mTIMERC_SUBAgent->mIsForcedGRC[channel] = (bool)((*TCRC)[bit_name2.str().c_str()]);
        mTIMERC_SUBAgent->mIsForcedOCRC[channel] = (bool)((*TCRC)[bit_name1.str().c_str()]);
        if (((*TCRC)[bit_name1.str().c_str()] == 1) ||
                ((*TCRC)[bit_name2.str().c_str()] == 1)) {
            mTIMERC_SUBAgent->HandleForcedCmpMatchMethod(channel);
        }
    }

    if ((unsigned int)((*TCRC)["CKSELC"]) == 6 || (unsigned int)((*TCRC)["CKSELC"]) == 7) {
        mTIMERC_SUBAgent->print_msg("info","Setting Prohibited. The clock bus 0 is set as default.\n");
    } // just dump mess

    mTIMERC_SUBAgent->mIs_Upper_Limit = (bool)(*TCRC)["CLRC"];//Set the counter upper limit
    if ((mPre_TCRC_CLRC != mTIMERC_SUBAgent->mIs_Upper_Limit) && (mTIMERC_SUBAgent->mChannelMode[0] != ATU5010_TIMERC_SUB_AgentController::emPWM)) {
        //Condition is changed in 0816 add- other PWM
        mPre_TCRC_CLRC = mTIMERC_SUBAgent->mIs_Upper_Limit;
        if ((bool)(mTIMERC_SUBAgent->mIs_Upper_Limit)) {
            mTIMERC_SUBAgent->print_msg("info","The counter upper limit function is enabled.\n");
            if ((unsigned int)(*CUCRC) == 0) {
                mTIMERC_SUBAgent->print_msg("info","Do not set CUCRC to 0x0");
            }
            mTIMERC_SUBAgent->SetUpperLimit((unsigned int)(*CUCRC));
        } else {
            mTIMERC_SUBAgent->OffCntUpperLimit();
        }
    }

    if (((unsigned int)((*TCRC)["CKSELC"])) != mPre_CntClk) {
        mIsCounterWorking = false;
        mPre_CntClk = (unsigned int)((*TCRC)["CKSELC"]);
        mTIMERC_SUBAgent->UpdatingCntClock();
        mTIMERC_SUBAgent->print_msg("info","Change the count clock setting while counter is working.\n");
    }

    (*TCRC) &= 0x30F;
}//}}}

///*********************************
/// Function     : Callback_NCNTCn
/// Description  : User sets the counter canceler
///*********************************
void ATU5010_TIMERC_SUB_Func::Callback_NCNTCn(const unsigned int channel)
{//{{{
    mTIMERC_SUBAgent->UpdateNoiseCounterValue(channel, (unsigned int)(*NCNTCn[channel]));
}//}}}

///*********************************
/// Function     : Callback_NCRCn
/// Description  : User sets to NCRC register
///*********************************
void ATU5010_TIMERC_SUB_Func::Callback_NCRCn(const unsigned int channel)
{//{{{
    mTIMERC_SUBAgent->SetNoiseCORVal(channel, (unsigned int)(*NCRCn[channel]));
}//}}}

///*********************************
/// Function     : cb_TIORC_IOC3
/// Description  : Callback TIORC
///*********************************
void ATU5010_TIMERC_SUB_Func::cb_TIORC_IOC3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    bool Is_Changed[4] = {false, false, false, false};
    for (unsigned int index = 0; index < 4; index++) {
        bit_name.str(""); bit_name << "IOC" << index;
        unsigned int temp_pre_data = (unsigned int)(str.pre_data >>(index*4));
        unsigned int cur_data = (unsigned int)((*TIORC)[bit_name.str().c_str()]);
        if (mTIMERC_SUBAgent->mIsTSTRCTrue) {
            if ((unsigned int)((temp_pre_data >> 2)&0x3) != (unsigned int)((cur_data >> 2)&0x3)) {
                //Check condition change operating mode while counter is working
                //Change IOCxy[3:2]
                mTIMERC_SUBAgent->print_msg("warning","Cannot change operating mode while counter is working.\n");
                (*TIORC)[bit_name.str().c_str()] = (unsigned int)((str.pre_data >> (index*4)) & 0xF);
            } else {
                if (((unsigned int)(cur_data >> 2) == 1) || ((unsigned int)(cur_data >> 2) == 3)) {
                    if (((unsigned int)(temp_pre_data & 0x3) != 0) && ((unsigned int)(cur_data & 0x3) == 0)) {
                        mTIMERC_SUBAgent->print_msg("info","Channel%i: Disable Input Capture Mode.\n",index);
                        Is_Changed[index] = true;
                    } else if (((unsigned int)(temp_pre_data & 0x3) == 0) && ((unsigned int)(cur_data & 0x3) != 0)) {
                        mTIMERC_SUBAgent->print_msg("info","Channel%i:Change Input Capture Mode successfully.\n",index);
                        Is_Changed[index] = true;
                    } else if (((unsigned int)(temp_pre_data & 0x3) == 0) && ((unsigned int)(cur_data & 0x3) == 0)) {
                        mTIMERC_SUBAgent->print_msg("info","Channel%i has already disabled Input Capture Mode.\n",index);
                        Is_Changed[index] = false;
                        (*TIORC)[bit_name.str().c_str()] = (unsigned int)((str.pre_data >> (index*4)) & 0xF);
                    } else {
                        mTIMERC_SUBAgent->print_msg("warning","Channel%i Cannot change the Input Capture Mode during counter is working.\n",index);
                        Is_Changed[index] = false;
                        (*TIORC)[bit_name.str().c_str()] = (unsigned int)((str.pre_data >> (index*4)) & 0xF);
                    }
                } else if ((unsigned int)(cur_data >> 2) == 0) {
                    if (((unsigned int)(temp_pre_data & 0x3) != 0) && ((unsigned int)(cur_data & 0x3) == 0)) {
                        mTIMERC_SUBAgent->print_msg("info","Channel%i: Disable Compare Match Mode.\n",index);
                        Is_Changed[index] = true;
                    } else if (((unsigned int)(temp_pre_data & 0x3) == 0) && ((unsigned int)(cur_data & 0x3) == 0)) {
                        mTIMERC_SUBAgent->print_msg("info","Channel%i has already disabled Cmp Match Mode.\n",index);
                        Is_Changed[index] = false;
                        (*TIORC)[bit_name.str().c_str()] = (unsigned int)((str.pre_data >> (index*4)) & 0xF);
                    } else if (((unsigned int)(temp_pre_data & 0x3) == 0) && ((unsigned int)(cur_data & 0x3) != 0)) {
                        mTIMERC_SUBAgent->print_msg("info","Channel%i:Change Cmp Match Mode successfully.\n",index);
                        Is_Changed[index] = true;
                    } else {
                        mTIMERC_SUBAgent->print_msg("warning","Channel%i Cannot change the Compare Match during counter is working.\n",index);
                        Is_Changed[index] = false;
                        (*TIORC)[bit_name.str().c_str()] = (unsigned int)((str.pre_data >> (index*4)) & 0xF);
                    }
                } else {
                    if (((unsigned int)(temp_pre_data & 0x3) != (unsigned int)(cur_data & 0x3)) && (mTIMERC_SUBAgent->mChannelMode[index] == ATU5010_TIMERC_SUB_AgentController::emOneshot)) {
                        mTIMERC_SUBAgent->print_msg("warning","Channel%i Cannot change active polarity  during counter is working.\n",index);
                        Is_Changed[index] = false;
                        (*TIORC)[bit_name.str().c_str()] = (unsigned int)((str.pre_data >> (index*4)) & 0xF);
                    }
                }
            }
        } else {
            mTIMERC_SUBAgent->mChannelValue[index] = (unsigned int)(*TIORC)[bit_name.str().c_str()];
            SetModeChannel(index, (unsigned int)(*TIORC)[bit_name.str().c_str()]);
        }
    }
    if ((Is_Changed[0] || Is_Changed[1] || Is_Changed[2] || Is_Changed[3]) && mTIMERC_SUBAgent->mIsTSTRCTrue) {
        SetModeInRunning(Is_Changed[0], Is_Changed[1], Is_Changed[2], Is_Changed[3]);
    }
}//}}}

///*********************************
/// Function     : cb_TCNTC_CNTC
/// Description  : call back to SetCounterValue
///*********************************
void ATU5010_TIMERC_SUB_Func::cb_TCNTC_CNTC(RegCBstr str)
{//{{{
    if (!mTIMERC_SUBAgent->mIsTSTRCTrue) {
        mTIMERC_SUBAgent->SetCounterValue((unsigned int)(*TCNTC));
    } else {
        mTIMERC_SUBAgent->mUpdatedTCNTC = (unsigned int)(*TCNTC);
        mTIMERC_SUBAgent->UpdateInRunning("TCNTC");
    }
}//}}}

///*********************************
/// Function     : Callback_GRC
/// Description  : Callback GRC when write to GRCn register
///*********************************
void ATU5010_TIMERC_SUB_Func::Callback_GRC(const unsigned int channel)
{//{{{
    if (!mTIMERC_SUBAgent->mIsTSTRCTrue) {
        if ((unsigned int)(*(GRCn[channel])) == (unsigned int) (*TCNTC)) {
            mTIMERC_SUBAgent->print_msg("warning","Do not set GRC same as value of TCNTC");
        }
    }
    if (mTIMERC_SUBAgent->mChannelMode[channel] == ATU5010_TIMERC_SUB_AgentController::emPWM) {
        if ((unsigned int)(*GRCn[channel]) == 0) {
            mTIMERC_SUBAgent->print_msg("warning","Do not set GRC to 0, the output is not as expected");
        }
        if (((unsigned int)(*GRCn[channel]) == (unsigned int)(*GRCn[0])) && (channel != 0)) {
            mTIMERC_SUBAgent->print_msg("warning","Do not set GRC%i equals GRC0, the output is duty cycle of 100%",channel);
        }
        for (unsigned int i = 1; i < 3; i++) {
            if ((unsigned int)(*GRCn[0]) == (unsigned int)(*GRCn[i])) {
                mTIMERC_SUBAgent->print_msg("warning","Do not set GRC%i equals GRC0, the output is duty cycle of 100%",channel);
            }
        }
    }
    if (mTIMERC_SUBAgent->mChannelMode[channel] == ATU5010_TIMERC_SUB_AgentController::emOneshot) {
        //Start One-shot
        if (mTIMERC_SUBAgent->mIsTSTRCTrue) {
            mTIMERC_SUBAgent->mGRCValue[channel] = (unsigned int)(*(GRCn[channel]));
            mTIMERC_SUBAgent->StartOneShotMethod(channel,(unsigned int)(*(GRCn[channel])));
        } else {
            print_msg("warning","Start Counter Before Writing to GRC%i", channel);
        }
    } else {
        mTIMERC_SUBAgent->SetGRCValue(channel,(unsigned int)(*(GRCn[channel])));
    }
    unsigned int range_cmp_val = (unsigned int)(*RCR2C);
    if (range_cmp_val != 0) {
        mTIMERC_SUBAgent->mRangeR2C[channel] = (double)(*(GRCn[channel])) + (double)pow((double)2,(int)range_cmp_val);
        mTIMERC_SUBAgent->RangeCmpGRC(mTIMERC_SUBAgent->mRangeR2C[channel], channel);
    }
}//}}}

///*********************************
/// Function     : Callback_OCRC
/// Description  : Callback OCRC when write to OCRCn register
///*********************************
void ATU5010_TIMERC_SUB_Func::Callback_OCRC(const unsigned int channel)
{//{{{
    mTIMERC_SUBAgent->mOCRCValue[channel] = (unsigned int)(*(OCRCn[channel]));
    mTIMERC_SUBAgent->SetOCRCValue(channel, (unsigned int)(*(OCRCn[channel])));
    if (!mTIMERC_SUBAgent->mIsTSTRCTrue) {
        if ((unsigned int)(*(OCRCn[channel])) == (unsigned int) (*TCNTC)) {
            mTIMERC_SUBAgent->print_msg("warning","Do not set OCRC same as value of TCNTC");
        }
    }
    unsigned int range_cmp_val = (unsigned int)(*RCR1C);
    if (range_cmp_val != 0) {
        mTIMERC_SUBAgent->mRangeR1C[channel] = (double)(*(OCRCn[channel])) + (double) pow((double)2,(int)range_cmp_val);
        mTIMERC_SUBAgent->RangeCmpOCRC(mTIMERC_SUBAgent->mRangeR1C[channel], channel);
    }
}//}}}

///*********************************
/// Function     : GetNCNTC
/// Description  : Return the value of NCNTC
///*********************************
unsigned int ATU5010_TIMERC_SUB_Func::GetNCNTC(const unsigned int channel)
{//{{{
    unsigned int ret = (unsigned int)(*NCNTCn[channel]);
    return ret;
}//}}}

///*********************************
/// Function     : GetCLKBusSel
/// Description  : Get counter clock
///*********************************
unsigned int ATU5010_TIMERC_SUB_Func::GetCLKBusSel(void)
{//{{{
    unsigned int temp = (unsigned int)(*TCRC)["CKSELC"];
    if (temp == 6 || temp ==7) {
        return 0;
    }
    return temp;
}//}}}

///*********************************
/// Function     : SetNCNTC
/// Description  : Return the value of NCNTC
///*********************************
void ATU5010_TIMERC_SUB_Func::SetNCNTC(const unsigned int channel,const unsigned int value)
{//{{{
    (*NCNTCn[channel]) = value;
}//}}}

///*********************************
/// Function     : GetCUCRC
/// Description  : Retturn the value of CUCRC
///*********************************
unsigned int ATU5010_TIMERC_SUB_Func::GetCUCRC(void)
{//{{{
    return (unsigned int)(*CUCRC);
}//}}}

// vim600: set foldmethod=marker :
