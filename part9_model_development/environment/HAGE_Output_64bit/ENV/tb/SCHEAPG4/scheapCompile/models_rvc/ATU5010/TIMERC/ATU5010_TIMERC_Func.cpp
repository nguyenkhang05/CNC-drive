// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERC_Func.cpp,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#include "ATU5010_TIMERC_Func.h"
#include "ATU5010_TIMERC_AgentController.h"

///*********************************
/// Function     : ATU5010_TIMERC
/// Description  : Constructor of ATU5010_TIMERC_Func class
///*********************************
ATU5010_TIMERC_Func::ATU5010_TIMERC_Func (std::string name, 
                       ATU5010_TIMERC_AgentController *TIMERCAgentPtr):
                       Catu5010_timerc_regif(name, 32)
{//{{{ 
    assert(TIMERCAgentPtr != NULL);
    mModuleName = name;
    mTIMERCAgent = TIMERCAgentPtr;
    mTIMERCAgent->CommandInit(name);
    Catu5010_timerc_regif::set_instance_name(name);
}//}}} 

///*********************************
/// Function     : ~ATU5010_TIMERC_Func
/// Description  : Destructor of ATU5010_TIMERC_Func class
///*********************************
ATU5010_TIMERC_Func::~ATU5010_TIMERC_Func (void)
{//{{{
}//}}}

///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5010_TIMERC_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr  = (unsigned int)trans.get_address() & 0xFFF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (((0x800 <= addr) && (addr < 0x820)) || ((0xF20 <= addr) && (addr < 0xFFF))) {
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else if ((0x820 <= addr) && (addr < 0x8A0)) { //sub0
        mTIMERCAgent->mTIMERC_SUB[0]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0x8A0 <= addr) && (addr < 0x920)) {//sub1
        mTIMERCAgent->mTIMERC_SUB[1]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0x920 <= addr) && (addr < 0x9A0)) {//sub2
        mTIMERCAgent->mTIMERC_SUB[2]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0x9A0 <= addr) && (addr < 0xA20)) {//sub3
        mTIMERCAgent->mTIMERC_SUB[3]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0xA20 <= addr) && (addr < 0xAA0)) {//sub4
        mTIMERCAgent->mTIMERC_SUB[4]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0xAA0 <= addr) && (addr < 0xB20)) {//sub5
        mTIMERCAgent->mTIMERC_SUB[5]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0xB20 <= addr) && (addr < 0xBA0)) {//sub6
        mTIMERCAgent->mTIMERC_SUB[6]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0xBA0 <= addr) && (addr < 0xC20)) {//sub7
        mTIMERCAgent->mTIMERC_SUB[7]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0xC20 <= addr) && (addr < 0xCA0)) {//sub8
        mTIMERCAgent->mTIMERC_SUB[8]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0xCA0 <= addr) && (addr < 0xD20)) {//sub9
        mTIMERCAgent->mTIMERC_SUB[9]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0xD20 <= addr) && (addr < 0xDA0)) {//sub10
        mTIMERCAgent->mTIMERC_SUB[10]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0xDA0 <= addr) && (addr < 0xE20)) {//sub11
        mTIMERCAgent->mTIMERC_SUB[11]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else if ((0xE20 <= addr) && (addr < 0xEA0)) {//sub12
        mTIMERCAgent->mTIMERC_SUB[12]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    }  else {//sub13 0xEA0 <= addr < 0xF20
        mTIMERCAgent->mTIMERC_SUB[13]->mTIC_SUBFunc->read(offsetAddress,trans,t,debug);
    } 
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ATU5010_TIMERC_Func::RegisterHandler (const bool is_common, const unsigned int sub_id ,const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
        for (unsigned int index = 0; index < ATU5010_TIMERC_AgentController::emSub; index++) {//Dump Reg of all sub
            mTIMERCAgent->mTIMERC_SUB[index]->mTIC_SUBFunc->RegisterHandler(args);
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Catu5010_timerc_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
            for (unsigned int index = 0; index < ATU5010_TIMERC_AgentController::emSub; index++) {//Set Message level to all sub
                mTIMERCAgent->mTIMERC_SUB[index]->mTIC_SUBFunc->RegisterHandler(args);
            }
        } else {
            if (args[1] == "DumpRegisterRW") {
                msg_return = Catu5010_timerc_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                for (unsigned int index = 0; index < ATU5010_TIMERC_AgentController::emSub; index++) {//Set Message level to all sub
                    mTIMERCAgent->mTIMERC_SUB[index]->mTIC_SUBFunc->RegisterHandler(args);
                }
            } else {
                if (is_common) {
                    msg_return = Catu5010_timerc_regif::reg_handle_command(args);
                    if (msg_return != "") {
                        SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                    }
                } else {
                    mTIMERCAgent->mTIMERC_SUB[sub_id]->mTIC_SUBFunc->RegisterHandler(args);
                }
            }
        }
    }
}//}}}

///*********************************
/// Function     : write
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5010_TIMERC_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr  = (unsigned int)trans.get_address() & 0xFFF;
    if (((0x800 <= addr) && (addr < 0x820)) || ((0xF20 <= addr) && (addr < 0xFFF))) {
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else if (((0x820 <= addr) && (addr < 0x8A0))) {//sub0
        mTIMERCAgent->mTIMERC_SUB[0]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else if (((0x8A0 <= addr) && (addr < 0x920))) {//sub1
        mTIMERCAgent->mTIMERC_SUB[1]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else if (((0x920 <= addr) && (addr < 0x9A0))) {//sub2
        mTIMERCAgent->mTIMERC_SUB[2]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else if (((0x9A0 <= addr) && (addr < 0xA20))) {//sub3
        mTIMERCAgent->mTIMERC_SUB[3]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else if (((0xA20 <= addr) && (addr < 0xAA0))) {//sub4
        mTIMERCAgent->mTIMERC_SUB[4]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else if (((0xAA0 <= addr) && (addr < 0xB20))) {//sub5
        mTIMERCAgent->mTIMERC_SUB[5]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else if (((0xB20 <= addr) && (addr < 0xBA0))) {//sub6
        mTIMERCAgent->mTIMERC_SUB[6]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else if (((0xBA0 <= addr) && (addr < 0xC20))) {//sub7
        mTIMERCAgent->mTIMERC_SUB[7]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else if (((0xC20 <= addr) && (addr < 0xCA0))) {//sub8
        mTIMERCAgent->mTIMERC_SUB[8]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else if (((0xCA0 <= addr) && (addr < 0xD20))) {//sub9
        mTIMERCAgent->mTIMERC_SUB[9]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    }  else if ((0xD20 <= addr) && (addr < 0xDA0)) {//sub10
        mTIMERCAgent->mTIMERC_SUB[10]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    }  else if ((0xDA0 <= addr) && (addr < 0xE20)) {//sub11
        mTIMERCAgent->mTIMERC_SUB[11]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    }  else if ((0xE20 <= addr) && (addr < 0xEA0)) {//sub12
        mTIMERCAgent->mTIMERC_SUB[12]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    } else {//sub13
        mTIMERCAgent->mTIMERC_SUB[13]->mTIC_SUBFunc->write(offsetAddress,trans,t,debug);
    }
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5010_TIMERC_Func::EnableReset (const bool is_active)
{//{{{ 
    Catu5010_timerc_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : TimerCStatus
/// Description  : TimerC is enabled from Wrapper
///*********************************
bool ATU5010_TIMERC_Func::CheckSubblock(const unsigned int sub)
{//{{{
    bool ret = false;
    ret = (bool)(((*TSTRC) >> sub) & 0x1);
    return ret;
}//}}}

///*********************************
/// Function     : cb_TSTRC_STRC
/// Description  : Call-back function
///*********************************
void ATU5010_TIMERC_Func::cb_TSTRC_STRC(RegCBstr str)
 {//{{{
    std::ostringstream bit_name;
    unsigned int pre_value = str.pre_data;
    for (unsigned int sub= 0; sub < 14; sub++) {
        if ((((unsigned int)((*TSTRC) >> sub)&0x1) != 0) && (((pre_value >>sub)&0x1) == 0)) {
            mTIMERCAgent->StartStopSubblock(sub, true);
        } else if ((((unsigned int)((*TSTRC) >> sub)&0x1) == 0) && (((pre_value >>sub)&0x1) == 0x1)) {
            mTIMERCAgent->StartStopSubblock(sub, false);
        }
    }
}//}}}

///*********************************
/// Function     : cb_NCMCR1C_NCM1C
/// Description  : Call-back function
///*********************************
void ATU5010_TIMERC_Func::cb_NCMCR1C_NCM1C(RegCBstr str)
{//{{{
    unsigned int pre_ncm1c = str.pre_data;
    for (unsigned int sub = 0; sub < 14; sub++) {
        unsigned int cur_ncm1c = (*NCMCR1C)["NCM1C"];
        if (((pre_ncm1c>>sub)&0x1) != ((cur_ncm1c>>sub)&0x1)) {
            if (mTIMERCAgent->mTIMERC_SUB[sub]->IsNoiseCancelDuration()) {
                mTIMERCAgent->print_msg("warning","Can not change operation mode of noise canceller in sub %i.\n",sub);
                (*NCMCR1C)["NCM1C"] = (cur_ncm1c & (~((unsigned int)1 << sub)) & 0xFFFF) | (((pre_ncm1c>>sub)&0x1)<<sub);
            } else {
                mTIMERCAgent->SetNoiseCancelMode(sub);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_NCMCR2C_NCM2C
/// Description  : Call-back function when user sets Noise canceller mode
///*********************************
void ATU5010_TIMERC_Func::cb_NCMCR2C_NCM2C(RegCBstr str)
{//{{{ 
    unsigned int pre_ncm2c = str.pre_data;
    for (unsigned int sub = 0; sub < 14; sub++) {
        unsigned int cur_ncm2c = (*NCMCR2C)["NCM2C"];
        if (((pre_ncm2c>>sub)&0x1) != ((cur_ncm2c>>sub)&0x1)) {
            if (mTIMERCAgent->mTIMERC_SUB[sub]->IsNoiseCancelDuration()) {
            mTIMERCAgent->print_msg("warning","Can not change operation mode of noise canceler in sub %d.\n",sub);
            (*NCMCR2C)["NCM2C"] = (cur_ncm2c & (~((unsigned int)1 << sub)) & 0xFFFF) | (((pre_ncm2c>>sub)&0x1)<<sub);
            } else {
                mTIMERCAgent->SetNoiseCancelMode(sub);
            }
        }
    }
}//}}}

///*********************************
/// Function     : cb_NCCRC0_NCKC3
/// Description  : Call-back function when user sets Noise canceller mode
///*********************************
void ATU5010_TIMERC_Func::cb_NCCRC_NCEC3(RegCBstr str)
{//{{{

    unsigned int sub_id = (unsigned int) str.channel;
    if (sub_id < 14) {
        unsigned int pre_nckc = (unsigned int)((str.pre_data >> 4) & 0xF);
        unsigned int cur_nckc = (unsigned int)((((*NCCRC[sub_id])) >>4) & 0xF);
        for (unsigned int i =0; i < 4; i++) {
            if (((((*(NCCRC[sub_id])) >> i) & 0x1) == 0x1) && ((((str.pre_data) >> i) & 0x1) == 0x1)) {
                if (((pre_nckc >> i) & 0x1) != ((cur_nckc>>i) &0x1)) {
                    mTIMERCAgent->print_msg("warning","Cannot change noise canceller setting.\n");
                    std::ostringstream bit_name;
                    bit_name.str(""); bit_name << "NCKC" << i;
                    (*(NCCRC[sub_id]))[bit_name.str().c_str()] = pre_nckc;
                }
            } else if ((((*(NCCRC[sub_id])) >> i) & 0x1) == 0x1) {
                mTIMERCAgent->SetNoiseEachCancelMode(sub_id,i); // 0820 change
                mTIMERCAgent->mTIMERC_SUB[sub_id]->EnableNoiseCancel(i,true);
                print_msg("info","Enable Noise Canceller Sub%i - Channel%i",sub_id,i);
            } else if ((((*(NCCRC[sub_id])) >> i) & 0x1) == 0x0) {
                mTIMERCAgent->mTIMERC_SUB[sub_id]->EnableNoiseCancel(i,false);
                print_msg("info","Disable Noise Canceller Sub%i - Channel%i",sub_id,i);
            }
        }
    }
}//}}}

///*********************************
/// Function     : GetNoiseCLKSel
/// Description  : Get the clock selection for noise canceler
///*********************************
unsigned int ATU5010_TIMERC_Func::GetNoiseCLKSel(const unsigned int sub, const unsigned int channel)
{//{{{
    unsigned int ret =0;
    std::ostringstream bit;
    bit.str(""); bit << "NCKC" << channel;
    unsigned int cur_nckc = (unsigned int)(*(NCCRC[sub]))[bit.str().c_str()];
    if (cur_nckc == 0) {
        ret = 0;
    } else {
        ret = 1;
    }
    return ret;
}//}}}

///*********************************
/// Function     : GetNoiseCancelMode
/// Description  : Noise cancel mode
/// Return value : 0 premature-transition cancellation mode
///                1 Minimum time-at-level cancellation mode
///                2 Level-accumulated cancellation mode
///*********************************
unsigned int ATU5010_TIMERC_Func::GetNoiseCancelMode(const unsigned int sub_id)
{//{{{
    unsigned int ret =0;
    if ((((*NCMCR1C) >> sub_id)&0x1) == 0x1) {
        if ((((*NCMCR2C) >> sub_id)&0x1) == 0) {
            ret = 1;
        } else {
            ret = 2;
        }
    }
    return ret;
}//}}}

/*********************************
// Function     : IsNoiseCancelEn
// Description  : Return Noise canceler status
// Parameter    :
//      chan_num            Channel number
// Return value : Noise canceler status
**********************************/
bool ATU5010_TIMERC_Func::IsNoiseCancelEn(const unsigned int sub, const unsigned int channel)
{//{{{
    std::ostringstream bit;
    bit.str(""); bit << "NCEC" << channel;
    bool cur_ncec = (bool)(*(NCCRC[sub]))[bit.str().c_str()];
    return cur_ncec;
}//}}}

// vim600: set foldmethod=marker :
