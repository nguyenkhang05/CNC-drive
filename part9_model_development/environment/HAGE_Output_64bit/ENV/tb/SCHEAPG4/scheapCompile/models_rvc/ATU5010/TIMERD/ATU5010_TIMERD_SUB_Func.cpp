// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERD_SUB_Func.cpp,v 1.1 2018/03/14 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "ATU5010_TIMERD_SUB_Func.h"

///*********************************
/// Function     : ATU5010_TIMERD_SUB
/// Description  : Constructor of ATU5010_TIMERD_SUB_Func class
///*********************************
ATU5010_TIMERD_SUB_Func::ATU5010_TIMERD_SUB_Func (std::string name,
                       ATU5010_TIMERD_SUB_AgentController *mTIMERD_SUBAgentPtr):
                       Catu5010_timerd_sub_regif(name, 32)
{//{{{
    assert(mTIMERD_SUBAgentPtr != NULL);
    mModuleName = name;
    mTIMERD_SUBAgent = mTIMERD_SUBAgentPtr;
    mTIMERD_SUBAgent->CommandInit(name);
    Catu5010_timerd_sub_regif::set_instance_name(name);
}//}}}

///*********************************
/// Function     : ~ATU5010_TIMERD_SUB_Func
/// Description  : Destructor of ATU5010_TIMERD_SUB_Func class
///*********************************
ATU5010_TIMERD_SUB_Func::~ATU5010_TIMERD_SUB_Func (void)
{//{{{
}//}}}
///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5010_TIMERD_SUB_Func::EnableReset (const bool is_active)
{//{{{
    Catu5010_timerd_sub_regif::EnableReset(is_active);
}//}}}
///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5010_TIMERD_SUB_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    unsigned int addr = (unsigned int)(trans.get_address() & 0x1FF);
    unsigned int channel = (addr>=0x1C0)?3:((addr>=0x180)?2:((addr>=0x140)?1:0));
    if (addr == 0x20 && mTIMERD_SUBAgent->mIsTimer1Running) {
       SetTCNT1DRegVal(mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emCounter1,0));
    } else if (addr == 0x24 && mTIMERD_SUBAgent->mIsTimer2Running){
       SetTCNT2DRegVal(mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emCounter2,0));
    } else if (addr == 0x58 && mTIMERD_SUBAgent->mIsDcntRunning[0] && !mTIMERD_SUBAgent->mIsDCNTUpdated[0]) {
       (*DCNTD[0])["CNTD"] = mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emDownCounter,0);
    } else if (addr == 0x78 && mTIMERD_SUBAgent->mIsDcntRunning[1] && !mTIMERD_SUBAgent->mIsDCNTUpdated[1]) {
       (*DCNTD[1])["CNTD"] = mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emDownCounter,1);
    } else if (addr == 0x98 && mTIMERD_SUBAgent->mIsDcntRunning[2] && !mTIMERD_SUBAgent->mIsDCNTUpdated[2]) {
       (*DCNTD[2])["CNTD"] = mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emDownCounter,2);
    } else if (addr == 0xB8 && mTIMERD_SUBAgent->mIsDcntRunning[3] && !mTIMERD_SUBAgent->mIsDCNTUpdated[3]) {
       (*DCNTD[3])["CNTD"] = mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emDownCounter,3);
    } else if ((addr == 0x100 || addr == 0x140 || addr == 0x180 || addr == 0x1C0) && mTIMERD_SUBAgent->mIsTimerOFMICNTDRunning[channel]) {
       SetOFMICNTDRegValue(channel, mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emOFMICNTDCounter,channel));
    } else if ((addr == 0x104 || addr == 0x144 || addr == 0x184 || addr == 0x1C4) && mTIMERD_SUBAgent->mIsTimerONMICNTDRunning[channel]) {
       SetONMICNTDRegValue(channel, mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emONMICNTDCounter,channel));
    } else if ((addr == 0x108 || addr == 0x148 || addr == 0x188 || addr == 0x1C8) && mTIMERD_SUBAgent->mIsTimerOTONMICNTDRunning[channel]) {//0x108
       SetOTONMICNTDRegValue(channel, mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emOTOMICNTDCounter,channel));
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
void ATU5010_TIMERD_SUB_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int )(trans.get_address() & 0x1FF);
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
    if(addr==0x008){
        unsigned int ch_id  = 0;
        if((bool)(*DSTRD)["DSTD0"]) {
            ch_id = 0;
            mTIMERD_SUBAgent->StartDCNTDReg(ch_id);
        }
        else if((bool)(*DSTRD)["DSTD1"]) {
            ch_id = 1;
            mTIMERD_SUBAgent->StartDCNTDReg(ch_id);
        }
        else if((bool)(*DSTRD)["DSTD2"]) {
            ch_id = 2;
            mTIMERD_SUBAgent->StartDCNTDReg(ch_id);
        }
        else if((bool)(*DSTRD)["DSTD3"]) {
            ch_id = 3;
            mTIMERD_SUBAgent->StartDCNTDReg(ch_id);
        }
        (*DSTRD) = 0x00;
    }
    else if (addr == 0x58 && (mTIMERD_SUBAgent->mIsDcntRunning[0] || mTIMERD_SUBAgent->mIsDcntinUdfPeriod[0])) {
       mTIMERD_SUBAgent->mUpdateDCNTDEvent[0].notify(SC_ZERO_TIME);
    } else if (addr == 0x78 && (mTIMERD_SUBAgent->mIsDcntRunning[1] || mTIMERD_SUBAgent->mIsDcntinUdfPeriod[1])) {
       mTIMERD_SUBAgent->mUpdateDCNTDEvent[1].notify(SC_ZERO_TIME);
    } else if (addr == 0x98 && (mTIMERD_SUBAgent->mIsDcntRunning[2] || mTIMERD_SUBAgent->mIsDcntinUdfPeriod[2])) {
       mTIMERD_SUBAgent->mUpdateDCNTDEvent[2].notify(SC_ZERO_TIME);
    } else if (addr == 0xB8 && (mTIMERD_SUBAgent->mIsDcntRunning[3] || mTIMERD_SUBAgent->mIsDcntinUdfPeriod[3])) {
       mTIMERD_SUBAgent->mUpdateDCNTDEvent[3].notify(SC_ZERO_TIME);
    }
}//}}}
///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ATU5010_TIMERD_SUB_Func::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Catu5010_timerd_sub_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Catu5010_timerd_sub_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}
// set overflow flags
void ATU5010_TIMERD_SUB_Func::TurnOVF1Flag(void)
{//{{{
    (*TSRD)["OVF1D"] = 1;
}//}}}

void ATU5010_TIMERD_SUB_Func::TurnOVF2Flag(void)
{//{{{
    (*TSRD)["OVF2D"] = 1;
}//}}}

void ATU5010_TIMERD_SUB_Func::TurnCMFBFlag(unsigned int channel)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "CMFBD" << channel;
    (*TSRD)[bit_name.str().c_str()] = 1;
}//}}}

void ATU5010_TIMERD_SUB_Func::TurnCMFAFlag(unsigned int channel)
{//{{{
    std::ostringstream bit_name;
    bit_name.str("");bit_name << "CMFAD" << channel;
    (*TSRD)[bit_name.str().c_str()] = 1;
}//}}}

// set, get counter value

void ATU5010_TIMERD_SUB_Func::SetOSBRDRegVal(unsigned int val)
{//{{{
    (*OSBRD)["OSBRD"] = val;
}//}}}
void ATU5010_TIMERD_SUB_Func::SetTCNT1DRegVal(unsigned int val)
{//{{{
    (*TCNT1D)["TCNT1D"] = val;
}//}}}

void ATU5010_TIMERD_SUB_Func::SetTCNT2DRegVal(unsigned int val)
{//{{{
    (*TCNT2D)["TCNT2D"] = val;
}//}}}

unsigned int ATU5010_TIMERD_SUB_Func::GetTCNT1DRegValue(void)
{//{{{
    unsigned int val = 0;
    val = (*TCNT1D)["TCNT1D"];
    return val;
}//}}}

unsigned int ATU5010_TIMERD_SUB_Func::GetTCNT2DRegValue(void)
{//{{{
    unsigned int val = 0;
    val = (*TCNT2D)["TCNT2D"];
    return val;
}//}}}

void ATU5010_TIMERD_SUB_Func::SetOFMICNTDRegValue(unsigned int channel, unsigned int val)
{//{{{
    (*OFMICNTD[channel])["OFMICNTD"] = val;
}//}}}

void ATU5010_TIMERD_SUB_Func::SetONMICNTDRegValue(unsigned int channel, unsigned int val)
{//{{{
    (*ONMICNTD[channel])["ONMICNTD"] = val;
}//}}}

void ATU5010_TIMERD_SUB_Func::SetOTONMICNTDRegValue(unsigned int channel, unsigned int val)
{//{{{
    (*OTONMICNTD[channel])["OTONMICNTD"] = val;
}//}}}

unsigned int ATU5010_TIMERD_SUB_Func::GetOFMINDRegValue(unsigned int channel)
{//{{{
    unsigned int val = 0;
    val = (*OFMIND[channel])["OFMIND"];
    return val;
}//}}}

unsigned int ATU5010_TIMERD_SUB_Func::GetONMINDRegValue(unsigned int channel)
{//{{{
    unsigned int val = 0;
    val = (*ONMIND[channel])["ONMIND"];
    return val;
}//}}}

unsigned int ATU5010_TIMERD_SUB_Func::GetOTOMINDRegValue(unsigned int channel)
{//{{{
    unsigned int val = 0;
    val = (*OTOMIND[channel])["OTOMIND"];
    return val;
}//}}}

unsigned int ATU5010_TIMERD_SUB_Func::GetOFMICNTDRegValue(unsigned int channel)
{//{{{
    unsigned int val = 0;
    val = (*OFMICNTD[channel])["OFMICNTD"];
    return val;
}//}}}

unsigned int ATU5010_TIMERD_SUB_Func::GetONMICNTDRegValue(unsigned int channel)
{//{{{
    unsigned int val = 0;
    val = (*ONMICNTD[channel])["ONMICNTD"];
    return val;
}//}}}

unsigned int ATU5010_TIMERD_SUB_Func::GetOTONMICNTDRegValue(unsigned int channel)
{//{{{
    unsigned int val = 0;
    val = (*OTONMICNTD[channel])["OTONMICNTD"];
    return val;
}//}}}

// set ICR1D/ICR2D reg value
void ATU5010_TIMERD_SUB_Func::SetICR1DVal(unsigned int channel, unsigned int val)
{//{{{
    (*ICR1D[channel])["ICR1D"] = val;
}//}}}

void ATU5010_TIMERD_SUB_Func::SetICR2DVal(unsigned int channel, unsigned int val)
{//{{{
    (*ICR2D[channel])["ICR2D"] = val;
}//}}}

void ATU5010_TIMERD_SUB_Func::SetOCR1DVal(unsigned int channel, unsigned int val)
{//{{{
    (*OCR1D[channel])["OC1D"] = val;
    mTIMERD_SUBAgent->mOCR1DValue[channel] = (*OCR1D[channel])["OC1D"];
}//}}}

void ATU5010_TIMERD_SUB_Func::SetOCR2DVal(unsigned int channel, unsigned int val)
{//{{{
    (*OCR2D[channel])["OC2D"] = val;
    mTIMERD_SUBAgent->mOCR2DValue[channel] = (*OCR2D[channel])["OC2D"];
}//}}}

// Get control bit value
unsigned int ATU5010_TIMERD_SUB_Func::GetTIDSELDVal(void)
{//{{{
     unsigned int val = 0;
    val = (*TICTSELD)["TIDSELD"];
    return val;
}//}}}

// call backs
void ATU5010_TIMERD_SUB_Func::cb_TCRD_OBRED(RegCBstr str)
{//{{{
   bool CallClockFunction = false;
   mTIMERD_SUBAgent->mCLR1DVal = (*TCRD)["CLR1D"];
   mTIMERD_SUBAgent->mCLR2DVal = (*TCRD)["CLR2D"];
   mTIMERD_SUBAgent->mOBREDVal = (*TCRD)["OBRED"];
   if (mTIMERD_SUBAgent->mCKSEL1DVal != (*TCRD)["CKSEL1D"]) {
      mTIMERD_SUBAgent->mCKSEL1DVal = (*TCRD)["CKSEL1D"];
      mTIMERD_SUBAgent->mChangeCountClock[ATU5010_TIMERD_SUB_AgentController::emCounter1]= true;
      CallClockFunction = true;
      if(mTIMERD_SUBAgent->mIsTimer1Running){
        mTIMERD_SUBAgent->mWriteCompareMatchValue[ATU5010_TIMERD_SUB_AgentController::emCounter1].notify(SC_ZERO_TIME);
      }
   }
   if (mTIMERD_SUBAgent->mCKSEL2DVal != (*TCRD)["CKSEL2D"]) {
      mTIMERD_SUBAgent->mCKSEL2DVal = (*TCRD)["CKSEL2D"];
      mTIMERD_SUBAgent->mChangeCountClock[ATU5010_TIMERD_SUB_AgentController::emCounter2]= true;
      CallClockFunction = true;
      if(mTIMERD_SUBAgent->mIsTimer2Running){
        mTIMERD_SUBAgent->mWriteCompareMatchValue[ATU5010_TIMERD_SUB_AgentController::emCounter2].notify(SC_ZERO_TIME);
      }
   }
   if (mTIMERD_SUBAgent->mDCSELDVal != (*TCRD)["DCSELD"]) {
      mTIMERD_SUBAgent->mDCSELDVal = (*TCRD)["DCSELD"];
      mTIMERD_SUBAgent->mChangeCountClock[ATU5010_TIMERD_SUB_AgentController::emDownCounter]= true;
      CallClockFunction = true;
   }
   mTIMERD_SUBAgent->mC1CEDVal = (*TCRD)["C1CED"];
   mTIMERD_SUBAgent->mC2CEDVal = (*TCRD)["C2CED"];
   if (CallClockFunction){
       mTIMERD_SUBAgent->ClockHandleFunction();
   }
}//}}}

void ATU5010_TIMERD_SUB_Func::cb_TCCRLRD_CLRSEL2D(RegCBstr str)
{//{{{
   mTIMERD_SUBAgent->mCLRSEL1DVal = (*TCCRLRD)["CLRSEL1D"];
   mTIMERD_SUBAgent->mCLRSEL2DVal = (*TCCRLRD)["CLRSEL2D"];
   //
    std::ostringstream bit_name;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name.str("");bit_name << "CMPSEL2D" << channel;
        mTIMERD_SUBAgent->mCMPSEL2DVal[channel] = (*TCCRLRD)[bit_name.str().c_str()];
        bit_name.str("");bit_name << "CMPSEL1D" << channel;
        mTIMERD_SUBAgent->mCMPSEL1DVal[channel] = (*TCCRLRD)[bit_name.str().c_str()];
    }
}//}}}

void ATU5010_TIMERD_SUB_Func::cb_TIER1D_OV2ED(RegCBstr str)
{//{{{
    mTIMERD_SUBAgent->mInterruptOVF1Enable = (bool)(*TIER1D)["OV1ED"];
    mTIMERD_SUBAgent->mInterruptOVF2Enable = (bool)(*TIER1D)["OV2ED"];
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_TIER2D_UNDED3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name.str("");bit_name << "UNDED" << channel;
        mTIMERD_SUBAgent->mInterruptUNDEDEnable[channel]= (*TIER2D)[bit_name.str().c_str()];
        bit_name.str("");bit_name << "CMPBED" << channel;
        mTIMERD_SUBAgent->mInterruptCMPBEnable[channel] = (*TIER2D)[bit_name.str().c_str()];
        bit_name.str("");bit_name << "CMPAED" << channel;
        mTIMERD_SUBAgent->mInterruptCMPAEnable[channel] = (*TIER2D)[bit_name.str().c_str()];
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_TIER3D_FEIED3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name.str("");bit_name << "FEIED" << channel;
        mTIMERD_SUBAgent->mInterruptFEIEDEnable[channel] = (*TIER3D)[bit_name.str().c_str()];
        bit_name.str("");bit_name << "REIED" << channel;
        mTIMERD_SUBAgent->mInterruptREIEDEnable[channel] = (*TIER3D)[bit_name.str().c_str()];
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_OCR1D_OC1D(RegCBstr str)
{//{{{
    bool ValIsChange = false;
    for (unsigned int index = 0; index < 4; index++) {
       if (mTIMERD_SUBAgent->mOCR1DValue[index] != (*OCR1D[index])["OC1D"]) {
          mTIMERD_SUBAgent->mOCR1DValue[index] = (*OCR1D[index])["OC1D"];
          ValIsChange = true;
       }
    }
    if(ValIsChange && mTIMERD_SUBAgent->mIsTimer1Running){
        mTIMERD_SUBAgent->mWriteCompareMatchValue[ATU5010_TIMERD_SUB_AgentController::emCounter1].notify(SC_ZERO_TIME);
    }
    if ((*RCR1D)["RC1D"]>0) {//range compare match function
        if(mTIMERD_SUBAgent->mCMPSEL1DVal[str.channel]==false) {
        mTIMERD_SUBAgent -> CheckRangeCompareFunction1(ATU5010_TIMERD_SUB_AgentController::emCounter1,str.channel);
        } else {
             mTIMERD_SUBAgent -> CheckRangeCompareFunction1(ATU5010_TIMERD_SUB_AgentController::emCounter2,str.channel);
        }
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_OCR2D_OC2D(RegCBstr str)
{//{{{
    bool ValIsChange = false;
    for (unsigned int index = 0; index < 4; index++) {
       if(mTIMERD_SUBAgent->mOCR2DValue[index] != (*OCR2D[index])["OC2D"]) {
           mTIMERD_SUBAgent->mOCR2DValue[index] = (*OCR2D[index])["OC2D"];
           ValIsChange = true;
       }
    }
    if(ValIsChange && mTIMERD_SUBAgent->mIsTimer2Running){
        mTIMERD_SUBAgent->mWriteCompareMatchValue[ATU5010_TIMERD_SUB_AgentController::emCounter2].notify(SC_ZERO_TIME);
    }
    if ((*RCR2D)["RC2D"]>0) {//range compare match function
        if(mTIMERD_SUBAgent->mCMPSEL2DVal[str.channel]==true) {
            mTIMERD_SUBAgent -> CheckRangeCompareFunction2(ATU5010_TIMERD_SUB_AgentController::emCounter1,str.channel);
        } else {
        mTIMERD_SUBAgent -> CheckRangeCompareFunction2(ATU5010_TIMERD_SUB_AgentController::emCounter2,str.channel);
    	}
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_RCR1D_RC1D(RegCBstr str)
{//{{{
       mTIMERD_SUBAgent->mRC1DVal = (*RCR1D)["RC1D"];
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_RCR2D_RC2D(RegCBstr str)
{//{{{
       mTIMERD_SUBAgent->mRC2DVal = (*RCR2D)["RC2D"];
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_TCMPED_CMPE2D3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    std::ostringstream bit_name2;
    std::ostringstream bit_name3;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        //
        bit_name.str("");bit_name << "CMPE1D" << channel;
        mTIMERD_SUBAgent->mCompareMatchCNT1Enable[channel] = (*TCMPED)[bit_name.str().c_str()];
        //
        bit_name2.str("");bit_name2 << "IO1D" << channel;
        bit_name3.str("");bit_name3 << "IOAD" << channel;
        if ((*TCMPED)[bit_name.str().c_str()] == 0) {
            (*TIOR1D)[bit_name3.str().c_str()] = 0;
            mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name3.str().c_str()];
        } else {
            switch((*TIORD)[bit_name2.str().c_str()]){
                case 1:
                    (*TIOR1D)[bit_name3.str().c_str()] = 1;
                    mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name3.str().c_str()];
                    break;
                case 2:
                    (*TIOR1D)[bit_name3.str().c_str()] = 2;
                    mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name3.str().c_str()];
                    break;
                case 3:
                    (*TIOR1D)[bit_name3.str().c_str()] = 3;
                    mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name3.str().c_str()];
                    break;
                default:
                    break;
            }
        }
        //
        bit_name.str("");bit_name << "CMPE2D" << channel;
        mTIMERD_SUBAgent->mCompareMatchCNT2Enable[channel] = (*TCMPED)[bit_name.str().c_str()];
        //
        bit_name2.str("");bit_name2 << "IO2D" << channel;
        bit_name3.str("");bit_name3 << "IOBD" << channel;
        if ((*TCMPED)[bit_name.str().c_str()] == 0) {
            (*TIOR2D)[bit_name3.str().c_str()] = 0;
            mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name3.str().c_str()];
        } else {
            switch((*TIORD)[bit_name2.str().c_str()]){
                case 1:
                    (*TIOR2D)[bit_name3.str().c_str()] = 1;
                    mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name3.str().c_str()];
                    break;
                case 2:
                    (*TIOR2D)[bit_name3.str().c_str()] = 2;
                    mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name3.str().c_str()];
                    break;
                case 3:
                    (*TIOR2D)[bit_name3.str().c_str()] = 3;
                    mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name3.str().c_str()];
                    break;
                default:
                    break;
            }
        }
    }
}//}}}

void ATU5010_TIMERD_SUB_Func::cb_TIOR1D_OSSD3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    std::ostringstream bit_name2;
    std::ostringstream bit_name3;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name.str("");bit_name << "OSSD" << channel;
        mTIMERD_SUBAgent->mOSSDValue[channel] = (*TIOR1D)[bit_name.str().c_str()];
        bit_name.str("");bit_name << "IOAD" << channel;
        mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name.str().c_str()];
        //
        bit_name2.str("");bit_name2 << "CMPE1D" << channel;
        bit_name3.str("");bit_name3 << "IO1D" << channel;
        switch((*TIOR1D)[bit_name.str().c_str()]){
            case 0:
                (*TCMPED)[bit_name2.str().c_str()] = 0;
                mTIMERD_SUBAgent->mCompareMatchCNT1Enable[channel] = 0;
                break;
            case 1:
                (*TCMPED)[bit_name2.str().c_str()] = 1;
                mTIMERD_SUBAgent->mCompareMatchCNT1Enable[channel] = 1;
                (*TIORD)[bit_name3.str().c_str()] = 1;
                break;
            case 2:
                (*TCMPED)[bit_name2.str().c_str()] = 1;
                mTIMERD_SUBAgent->mCompareMatchCNT1Enable[channel] = 1;
                (*TIORD)[bit_name3.str().c_str()] = 2;
                break;
            case 3:
               (*TCMPED)[bit_name2.str().c_str()] = 1;
                mTIMERD_SUBAgent->mCompareMatchCNT1Enable[channel] = 1;
                (*TIORD)[bit_name3.str().c_str()] = 3;
                break;
            default:
                break;
        }
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_TIOR2D_IOBD3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    std::ostringstream bit_name2;
    std::ostringstream bit_name3;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name.str("");bit_name << "IOBD" << channel;
        bit_name2.str("");bit_name2 << "CMPE2D" << channel;
        bit_name3.str("");bit_name3 << "IO2D" << channel;
        mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name.str().c_str()];
        //
        switch((*TIOR2D)[bit_name.str().c_str()]){
            case 0:
                (*TCMPED)[bit_name2.str().c_str()] = 0;
                mTIMERD_SUBAgent->mCompareMatchCNT2Enable[channel] = 0;
                break;
            case 1:
                (*TCMPED)[bit_name2.str().c_str()] = 1;
                mTIMERD_SUBAgent->mCompareMatchCNT2Enable[channel] = 1;
                (*TIORD)[bit_name3.str().c_str()] = 1;
                break;
            case 2:
                (*TCMPED)[bit_name2.str().c_str()] = 1;
                 mTIMERD_SUBAgent->mCompareMatchCNT2Enable[channel] = 1;
                (*TIORD)[bit_name3.str().c_str()] = 2;
                break;
            case 3:
                (*TCMPED)[bit_name2.str().c_str()] = 1;
                mTIMERD_SUBAgent->mCompareMatchCNT2Enable[channel] = 1;
                (*TIORD)[bit_name3.str().c_str()] = 3;
                break;
            default:
                (*TCMPED)[bit_name2.str().c_str()] = 0;
                mTIMERD_SUBAgent->mCompareMatchCNT2Enable[channel] = 0;
                break;
        }
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_OSELRD_OSELBD3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name.str("");bit_name << "OSELAD" << channel;
        mTIMERD_SUBAgent->mOSELADValue[channel] = (*OSELRD)[bit_name.str().c_str()];
        if(mTIMERD_SUBAgent->mOSELADValue[channel]){
            mTIMERD_SUBAgent->ChangeOutputPortTODA(channel,0);
        }
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_ODRD_ODBD3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name.str("");bit_name << "ODAD" << channel;
        mTIMERD_SUBAgent->mODADValue[channel] = (*ODRD)[bit_name.str().c_str()];
        if(mTIMERD_SUBAgent->mOSELADValue[channel]){
            mTIMERD_SUBAgent->ChangeOutputPortTODA(channel,0);
            mTIMERD_SUBAgent->WriteTODMIGMethod(channel);
        }
        if(Get_OSELRD_OSELBD(channel)) {
            mTIMERD_SUBAgent->WriteTODBMethod(channel);
            mTIMERD_SUBAgent->WriteTODMIGMethod(channel);
        }
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_TOCRD_TONEBD(RegCBstr str)
{//{{{
    mTIMERD_SUBAgent->mGlobalInvertASelect = (*TOCRD)["TONEAD"];
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_TOCCRD_TONEBD3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name.str("");bit_name << "TONEAD" << channel;
        mTIMERD_SUBAgent->mChannelAInvert[channel] = (*TOCCRD)[bit_name.str().c_str()];
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_TSCRD_OVFC2D(RegCBstr str)
{//{{{
    if (((str.data >> 0) & 0x1) == 1) {
        (*TSRD)["CMFBD0"] = 0;
    }
    if (((str.data >> 1) & 0x1) == 1) {
        (*TSRD)["CMFBD1"] = 0;
    }
    if (((str.data >> 2) & 0x1) == 1) {
        (*TSRD)["CMFBD2"] = 0;
    }
    if (((str.data >> 3) & 0x1) == 1) {
        (*TSRD)["CMFBD3"] = 0;
    }
    if (((str.data >> 4) & 0x1) == 1) {
        (*TSRD)["CMFAD0"] = 0;
    }
    if (((str.data >> 5) & 0x1) == 1) {
        (*TSRD)["CMFAD1"] = 0;
    }
    if (((str.data >> 6) & 0x1) == 1) {
        (*TSRD)["CMFAD2"] = 0;
    }
    if (((str.data >> 7) & 0x1) == 1) {
        (*TSRD)["CMFAD3"] = 0;
    }
    if (((str.data >> 8) & 0x1) == 1) {
        (*TSRD)["UDFD0"] = 0;
    }
    if (((str.data >> 9) & 0x1) == 1) {
        (*TSRD)["UDFD1"] = 0;
    }
    if (((str.data >> 10) & 0x1) == 1) {
        (*TSRD)["UDFD2"] = 0;
    }
    if (((str.data >> 11) & 0x1) == 1) {
        (*TSRD)["UDFD3"] = 0;
    }
    if (((str.data >> 12) & 0x1) == 1) {
        (*TSRD)["OVF1D"] = 0;
    }
    if (((str.data >> 13) & 0x1) == 1) {
        (*TSRD)["OVF2D"] = 0;
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_TIORD_IO2D3(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
    std::ostringstream bit_name2;
    std::ostringstream bit_name3;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name.str("");bit_name << "IO1D" << channel;
        bit_name2.str("");bit_name2 << "CMPE1D" << channel;
        bit_name3.str("");bit_name3 << "IOAD" << channel;
        if ((*TCMPED)[bit_name2.str().c_str()] == 0) {
            (*TIOR1D)[bit_name3.str().c_str()] = 0;
            mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name3.str().c_str()];
        } else {
            switch((*TIORD)[bit_name.str().c_str()]){
                case 0:
                    (*TIOR1D)[bit_name3.str().c_str()] = 3;
                    mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name3.str().c_str()];
                    break;
                case 1:
                    (*TIOR1D)[bit_name3.str().c_str()] = 1;
                    mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name3.str().c_str()];
                    break;
                case 2:
                    (*TIOR1D)[bit_name3.str().c_str()] = 2;
                    mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name3.str().c_str()];
                    break;
                case 3:
                    (*TIOR1D)[bit_name3.str().c_str()] = 3;
                    mTIMERD_SUBAgent->mIOADValue[channel] = (*TIOR1D)[bit_name3.str().c_str()];
                    break;
                default:
                    break;
            }
        }
        if (((*TIORD)[bit_name.str().c_str()]) == 0) {
            (*TIORD)[bit_name.str().c_str()] = 3;
        }
        bit_name.str("");bit_name << "IO2D" << channel;
        bit_name2.str("");bit_name2 << "CMPE2D" << channel;
        bit_name3.str("");bit_name3 << "IOBD" << channel;
        if ((*TCMPED)[bit_name2.str().c_str()] == 0) {
            (*TIOR2D)[bit_name3.str().c_str()] = 0;
            mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name3.str().c_str()];
        } else {
            switch((*TIORD)[bit_name.str().c_str()]){
                case 0:
                    (*TIOR2D)[bit_name3.str().c_str()] = 3;
                    mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name3.str().c_str()];
                    break;
                case 1:
                    (*TIOR2D)[bit_name3.str().c_str()] = 1;
                    mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name3.str().c_str()];
                    break;
                case 2:
                    (*TIOR2D)[bit_name3.str().c_str()] = 2;
                    mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name3.str().c_str()];
                    break;
                case 3:
                    (*TIOR2D)[bit_name3.str().c_str()] = 3;
                    mTIMERD_SUBAgent->mIOBDValue[channel] = (*TIOR2D)[bit_name3.str().c_str()];
                    break;
                default:
                    break;
            }
        }
        if (((*TIORD)[bit_name.str().c_str()]) == 0) {
            (*TIORD)[bit_name.str().c_str()] = 3;
        }
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_OFMICNTD_OFMICNTD(RegCBstr str)
{//{{{
    re_printf("info", "cb_OFMICNTD_OFMICNTD: %d %d\n", str.channel, mTIMERD_SUBAgent->mIsTimerOFMICNTDRunning[str.channel]);
    if (mTIMERD_SUBAgent->mIsTimerOFMICNTDRunning[str.channel]){
        re_printf("info", "cb_OFMICNTD_OFMICNTD: inside\n");
        mTIMERD_SUBAgent->mWriteOFMICNTDEvent[str.channel].notify(SC_ZERO_TIME);
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_ONMICNTD_ONMICNTD(RegCBstr str)
{//{{{
    re_printf("info", "cb_ONMICNTD_ONMICNTD: %d %d\n", str.channel, mTIMERD_SUBAgent->mIsTimerONMICNTDRunning[str.channel]);
    if (mTIMERD_SUBAgent->mIsTimerONMICNTDRunning[str.channel]){
        re_printf("info", "cb_ONMICNTD_ONMICNTD: inside\n");
        mTIMERD_SUBAgent->mWriteONMICNTDEvent[str.channel].notify(SC_ZERO_TIME);
    }
}//}}}
void ATU5010_TIMERD_SUB_Func::cb_OTONMICNTD_OTONMICNTD(RegCBstr str)
{//{{{
    re_printf("info", "cb_OTONMICNTD_OTONMICNTD: %d %d\n", str.channel, mTIMERD_SUBAgent->mIsTimerOTONMICNTDRunning[str.channel]);
    if (mTIMERD_SUBAgent->mIsTimerOTONMICNTDRunning[str.channel]){
        re_printf("info", "cb_OTONMICNTD_OTONMICNTD: inside\n");
        mTIMERD_SUBAgent->mWriteOTOMICNTDEvent[str.channel].notify(SC_ZERO_TIME);
    }
}//}}}

void ATU5010_TIMERD_SUB_Func::cb_TSCR2D_FEFCD3(RegCBstr str)
{//{{{
    std::ostringstream bit_name1;
    std::ostringstream bit_name2;
    std::ostringstream bit_name3;
    std::ostringstream bit_name4;
    for (unsigned int channel = 0; channel <4 ; channel ++) {
        bit_name1.str("");bit_name1 << "REFCD" << channel;
        bit_name3.str("");bit_name3 << "FEFCD" << channel;
        bit_name2.str("");bit_name2 << "REFD" << channel;
        bit_name4.str("");bit_name4 << "FEFD" << channel;

        if((*TSCR2D)[bit_name1.str().c_str()]){
            (*TSR2D)[bit_name2.str().c_str()] = 0;
        }
        if((*TSCR2D)[bit_name3.str().c_str()]){
            (*TSR2D)[bit_name4.str().c_str()] = 0;
        }
    }
    (*TSCR2D) = 0;
}//}}}

	unsigned int ATU5010_TIMERD_SUB_Func::GetDcntTrigger(const unsigned int ch_id) {
		switch(ch_id) {
			case 0:
				return (*DCRD)["TRGSELD0"];
				break;
			case 1:
				return (*DCRD)["TRGSELD1"];
				break;
			case 2:
				return (*DCRD)["TRGSELD2"];
				break;
			case 3:
				return (*DCRD)["TRGSELD3"];
				break;
			default:
        return 0;
				break;
		}
	}
	void ATU5010_TIMERD_SUB_Func::Set_DSR1D_DSFD (const unsigned int ch_id, unsigned int value) {
		switch(ch_id) {
			case 0:
				(*DSR1D)["DSFD0"] = value;
				break;
			case 1:
				(*DSR1D)["DSFD1"] = value;
				break;
			case 2:
				(*DSR1D)["DSFD2"] = value;
				break;
			case 3:
				(*DSR1D)["DSFD3"] = value;
				break;
			default:
				break;
		}

	}

    void ATU5010_TIMERD_SUB_Func::Set_TSR2D_FEFD (const unsigned int ch_id, unsigned int value) {
		switch(ch_id) {
			case 0:
				(*TSR2D)["FEFD0"] = value;
				break;
			case 1:
				(*TSR2D)["FEFD1"] = value;
				break;
			case 2:
				(*TSR2D)["FEFD2"] = value;
				break;
			case 3:
				(*TSR2D)["FEFD3"] = value;
				break;
			default:
				break;
		}

	}

    void ATU5010_TIMERD_SUB_Func::Set_TSR2D_REFD (const unsigned int ch_id, unsigned int value) {
		switch(ch_id) {
			case 0:
				(*TSR2D)["REFD0"] = value;
				break;
			case 1:
				(*TSR2D)["REFD1"] = value;
				break;
			case 2:
				(*TSR2D)["REFD2"] = value;
				break;
			case 3:
				(*TSR2D)["REFD3"] = value;
				break;
			default:
				break;
		}

	}

	void ATU5010_TIMERD_SUB_Func::Set_DSR1D_DWFD (const unsigned int ch_id, unsigned int value) {
		switch(ch_id) {
			case 0:
				(*DSR1D)["DWFD0"] = value;
				break;
			case 1:
				(*DSR1D)["DWFD1"] = value;
				break;
			case 2:
				(*DSR1D)["DWFD2"] = value;
				break;
			case 3:
				(*DSR1D)["DWFD3"] = value;
				break;
			default:
				break;
		}

	}
	void ATU5010_TIMERD_SUB_Func::Set_DSR2D_DWRFD (const unsigned int ch_id, unsigned int value) {
		switch(ch_id) {
			case 0:
				(*DSR2D)["DWRFD0"] = value;
				break;
			case 1:
				(*DSR2D)["DWRFD1"] = value;
				break;
			case 2:
				(*DSR2D)["DWRFD2"] = value;
				break;
			case 3:
				(*DSR2D)["DWRFD3"] = value;
				break;
			default:
				break;
		}

	}

	void ATU5010_TIMERD_SUB_Func::Set_TSRD_UDFD (const unsigned int ch_id, unsigned int value) {
		switch(ch_id) {
			case 0:
				(*TSRD)["UDFD0"] = value;
				break;
			case 1:
				(*TSRD)["UDFD1"] = value;
				break;
			case 2:
				(*TSRD)["UDFD2"] = value;
				break;
			case 3:
				(*TSRD)["UDFD3"] = value;
				break;
			default:
				break;
		}

	}

	unsigned int ATU5010_TIMERD_SUB_Func::Get_DCNTD(const unsigned int ch_id) {
		return (*DCNTD[ch_id])["CNTD"];
	}
	void ATU5010_TIMERD_SUB_Func::Set_DCNTD(const unsigned int ch_id, unsigned int value) {
		(*DCNTD[ch_id])["CNTD"] = value;
	}

	void ATU5010_TIMERD_SUB_Func::cb_DSCRD_DWRFCD0 (RegCBstr str) {
	 if(((str.data >> 0) & 0x1 )==1) {
         (*DSR2D)["DWRFD0"] = 0;
	 }
	 if(((str.data >> 1) & 0x1 )==1) {
        (*DSR2D)["DWRFD1"] = 0;
		 }
     if(((str.data >> 2) & 0x1 )==1) {
        (*DSR2D)["DWRFD2"] = 0;
		 }
     if(((str.data >> 3) & 0x1 )==1) {
	    (*DSR2D)["DWRFD3"] = 0;
		 }
     if(((str.data >> 4) & 0x1 )==1) {
	    (*DSR1D)["DWFD0"] = 0;
		 }
     if(((str.data >> 5) & 0x1 )==1) {
	    (*DSR1D)["DWFD1"] = 0;
		 }
     if(((str.data >> 6) & 0x1 )==1) {
	    (*DSR1D)["DWFD2"] = 0;
		 }
     if(((str.data >> 7) & 0x1 )==1) {
	    (*DSR1D)["DWFD3"] = 0;
		 }
	}
   void ATU5010_TIMERD_SUB_Func::cb_CUCR1D_CUCR1D (RegCBstr str){
       if (mTIMERD_SUBAgent->mCUCR1DValue != (*CUCR1D)["CUCR1D"]) {
          mTIMERD_SUBAgent->mCUCR1DValue = (*CUCR1D)["CUCR1D"];
          if(mTIMERD_SUBAgent->mIsTimer1Running){
            mTIMERD_SUBAgent->mWriteCompareMatchValue[ATU5010_TIMERD_SUB_AgentController::emCounter1].notify(SC_ZERO_TIME);
          }
      }
   }
   void ATU5010_TIMERD_SUB_Func::cb_CUCR2D_CUCR2D (RegCBstr str){
       if( mTIMERD_SUBAgent->mCUCR2DValue != (*CUCR2D)["CUCR2D"]) {
          mTIMERD_SUBAgent->mCUCR2DValue = (*CUCR2D)["CUCR2D"];
          if(mTIMERD_SUBAgent->mIsTimer2Running){
            mTIMERD_SUBAgent->mWriteCompareMatchValue[ATU5010_TIMERD_SUB_AgentController::emCounter2].notify(SC_ZERO_TIME);
          }
       }
   }
   void ATU5010_TIMERD_SUB_Func::cb_TCNT1D_TCNT1D (RegCBstr str){
      if(mTIMERD_SUBAgent->mIsTimer1Running){
          mTIMERD_SUBAgent->mWriteCounterValue[ATU5010_TIMERD_SUB_AgentController::emCounter1].notify(SC_ZERO_TIME);
      } else {
         if(mTIMERD_SUBAgent->mCLRCounter1==true){
            mTIMERD_SUBAgent->mCLRCounter1=false;
         }
         if(mTIMERD_SUBAgent->mOverflowTCNT1D==true){
            mTIMERD_SUBAgent->mOverflowTCNT1D=false;
         }
      }
   }
   void ATU5010_TIMERD_SUB_Func::cb_TCNT2D_TCNT2D (RegCBstr str){
      if(mTIMERD_SUBAgent->mIsTimer2Running){
          mTIMERD_SUBAgent->mWriteCounterValue[ATU5010_TIMERD_SUB_AgentController::emCounter2].notify(SC_ZERO_TIME);
      } else {
         if(mTIMERD_SUBAgent->mCLRCounter2==true){
            mTIMERD_SUBAgent->mCLRCounter2=false;
         }
         if(mTIMERD_SUBAgent->mOverflowTCNT2D==true){
            mTIMERD_SUBAgent->mOverflowTCNT2D=false;
         }
      }
   }
    bool ATU5010_TIMERD_SUB_Func::Get_TOCRD_TONEBD (const unsigned int ch_id) {

        return (bool)(*TOCRD)["TONEBD"];

	}
	bool ATU5010_TIMERD_SUB_Func::Get_TOCCRD_TONEBD (const unsigned int ch_id) {
		switch(ch_id) {
			case 0:
				return (bool)(*TOCCRD)["TONEBD0"];
				break;
			case 1:
				return (bool)(*TOCCRD)["TONEBD1"];
				break;
			case 2:
				return (bool)(*TOCCRD)["TONEBD2"];
				break;
			case 3:
				return (bool)(*TOCCRD)["TONEBD3"];
				break;
			default:
				return false;
				break;
		}

	}

    bool ATU5010_TIMERD_SUB_Func::Get_MIGCRD_MIGEND (const unsigned int ch_id) {
        bool ret;
        switch(ch_id) {
            case 0:
                ret = (bool)(*MIGCRD)["MIGEND0"];
                break;
            case 1:
                ret = (bool)(*MIGCRD)["MIGEND1"];
                break;
            case 2:
                ret = (bool)(*MIGCRD)["MIGEND2"];
                break;
            case 3:
                ret = (bool)(*MIGCRD)["MIGEND3"];
                break;
            default:
                break;
        }
        return ret;
    }

    bool ATU5010_TIMERD_SUB_Func::Get_MIGSELD_MIGSEL (const unsigned int ch_id) {
        bool ret;
        switch(ch_id) {
            case 0:
                ret = (bool)(*MIGSELD)["MIGSEL0"];
                break;
            case 1:
                ret = (bool)(*MIGSELD)["MIGSEL1"];
                break;
            case 2:
                ret = (bool)(*MIGSELD)["MIGSEL2"];
                break;
            case 3:
                ret = (bool)(*MIGSELD)["MIGSEL3"];
                break;
            default:
                break;
        }
        return ret;
    }

	bool ATU5010_TIMERD_SUB_Func::Get_OSELRD_OSELBD (const unsigned int ch_id) {
		switch(ch_id) {
			case 0:
				return (bool)(*OSELRD)["OSELBD0"];
				break;
			case 1:
				return (bool)(*OSELRD)["OSELBD1"];
				break;
			case 2:
				return (bool)(*OSELRD)["OSELBD2"];
				break;
			case 3:
				return (bool)(*OSELRD)["OSELBD3"];
				break;
			default:
        return false;
				break;
		}
	}

	bool ATU5010_TIMERD_SUB_Func::Get_ODRD_ODBD (const unsigned int ch_id) {
		switch(ch_id) {
			case 0:
				return (bool)(*ODRD)["ODBD0"];
				break;
			case 1:
				return (bool)(*ODRD)["ODBD1"];
				break;
			case 2:
				return (bool)(*ODRD)["ODBD2"];
				break;
			case 3:
				return (bool)(*ODRD)["ODBD3"];
				break;
			default:
        return false;
				break;
		}
	}

void ATU5010_TIMERD_SUB_Func::Set_CAPD(const std::string name, const unsigned int ch_id) {
    if(name=="OFCAPD1") {
        if(mTIMERD_SUBAgent->mIsTimer1Running) {
            (*OFCAPD1[ch_id]) = mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emCounter1,0);
        }
    }
    else if(name=="OFCAPD2") {
        if(mTIMERD_SUBAgent->mIsTimer2Running) {
            (*OFCAPD2[ch_id]) = mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emCounter2,0);
        }
    }
    else if(name=="ONCAPD1") {
        if(mTIMERD_SUBAgent->mIsTimer1Running) {
            (*ONCAPD1[ch_id]) = mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emCounter1,0);
        }
    }
    else if(name=="ONCAPD2") {
        if(mTIMERD_SUBAgent->mIsTimer2Running) {
            (*ONCAPD2[ch_id]) = mTIMERD_SUBAgent->GetCounterValue(ATU5010_TIMERD_SUB_AgentController::emCounter2,0);
        }
    }
}
// vim600: set foldmethod=marker :
