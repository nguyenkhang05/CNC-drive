// ---------------------------------------------------------------------
// $Id: ATU5_TIMERD_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_TIMERD_Func.h"
#include "atu5_timerd_regif.h"
#include "ATU5_TIMERD_AgentController.h"
#include "ATU5_TIMERD_SUB.h"
#include "ATU5_TIMERD_SUB_Func.h"

///*********************************
/// Function     : ATU5_TIMERD
/// Description  : Constructor of ATU5_TIMERD_Func class
///*********************************
ATU5_TIMERD_Func::ATU5_TIMERD_Func (std::string name,
                       ATU5_TIMERD_AgentController *TIMERDAgentPtr, unsigned int subblock):
                       msbD(subblock),
                       Catu5_timerd_regif(name, 32)
{//{{{
    assert(TIMERDAgentPtr != NULL);
    mModuleName = name;
    mTIMERDAgent = TIMERDAgentPtr;
    mTIMERDAgent->CommandInit(name);
    Catu5_timerd_regif::set_instance_name(name);
    mTimerDStatus = false;
}//}}}

///*********************************
/// Function     : ~ATU5_TIMERD_Func
/// Description  : Destructor of ATU5_TIMERD_Func class
///*********************************
ATU5_TIMERD_Func::~ATU5_TIMERD_Func (void)
{//{{{
}//}}}

///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5_TIMERD_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr  = (unsigned int)trans.get_address() & 0xFFFF;
    unsigned int offsetfw = offsetAddress & 0x01FF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if ((0x2000 <= addr) && (addr < 0x2008) || (0x2020 <= addr) && (addr < 0x2040)) {
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
        else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
    else if ((0x2200 <= addr) && (addr < 0x4000)) {
        unsigned int sub_id = ((addr >> 8) - 0x22) / 2;
        if (sub_id <= msbD) {
            mTIMERDAgent->mTIMERD_SUB[sub_id]->mATU5_TIMERD_SUBFunc->read(offsetfw, trans, t, debug);
        }
    }
}//}}}

///*********************************
/// Function     : write
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5_TIMERD_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr  = (unsigned int)trans.get_address() & 0xFFFF;
    unsigned int offsetfw = offsetAddress & 0x01FF;
    if ((0x2000 <= addr) && (addr < 0x2008) || (0x2020 <= addr) && (addr<0x2040)) {
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }else if((0x2200 <= addr) && (addr < 0x4000)){
        unsigned int sub_id = ((addr >> 8) - 0x22) / 2;
        if (sub_id <= msbD) {
            mTIMERDAgent->mTIMERD_SUB[sub_id]->mATU5_TIMERD_SUBFunc->write(offsetfw, trans, t, debug);
        }
    }
}//}}}
///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ATU5_TIMERD_Func::RegisterHandler (const bool is_common, const unsigned int sub_id ,const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
        for (unsigned int index = 0; index < msbD; index++) {//Dump Reg of all sub
            mTIMERDAgent->mTIMERD_SUB[index]->mATU5_TIMERD_SUBFunc->RegisterHandler(args);
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Catu5_timerd_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
            for (unsigned int index = 0; index < msbD; index++) {//Set Message level to all sub
                mTIMERDAgent->mTIMERD_SUB[index]->mATU5_TIMERD_SUBFunc->RegisterHandler(args);
            }
        } else if  (args[1] == "DumpRegisterRW") {
                msg_return = Catu5_timerd_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                for (unsigned int index = 0; index < msbD; index++) {//Set Message level to all sub
                    mTIMERDAgent->mTIMERD_SUB[index]->mATU5_TIMERD_SUBFunc->RegisterHandler(args);
                }
        } else {
            if (is_common) {
                msg_return = Catu5_timerd_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
            } else {
                mTIMERDAgent->mTIMERD_SUB[sub_id]->mATU5_TIMERD_SUBFunc->RegisterHandler(args);
            }
        }
    }
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5_TIMERD_Func::EnableReset (const bool is_active)
{//{{{
    Catu5_timerd_regif::EnableReset(is_active);
    if(is_active){
        mTimerDStatus = false;
    }
}//}}}

///*********************************
/// Function     : TimerDStatus
/// Description  : TimerD is enabled from Wrapper
///*********************************
void ATU5_TIMERD_Func::TimerDStatus(const bool is_active)
{//{{{
    if (is_active) {
        mTimerDStatus = true;
        for (unsigned int sub= 0; sub < msbD; sub++) {
            if ((((unsigned int)((*TSTRD) >> sub)) & 0x1)!= 0) {
                if(mTIMERDAgent->mTIMERD_SUB[sub]->mCounterEnable==false){
                    mTIMERDAgent->mTIMERD_SUB[sub]->RunningCounter(true);
                }
            } 
        }
    } else {
        for (unsigned int sub = 0; sub <  msbD; sub++) {
            if(mTIMERDAgent->mTIMERD_SUB[sub]->mCounterEnable==true){
                mTIMERDAgent->mTIMERD_SUB[sub]->RunningCounter(false);
            }
        }
        mTimerDStatus = false;
    }
}//}}}


///*********************************
/// Function     : cb_TSTRD_STRD
/// Description  : Call-back function
///*********************************
void ATU5_TIMERD_Func::cb_TSTRD_STRD0(RegCBstr str)
{//{{{
    unsigned int mask_reg = (unsigned int)(0xFFFF >> (16 - msbD));
    (*TSTRD) = (unsigned int)(*TSTRD) & mask_reg;
    if (mTimerDStatus) {
        for (unsigned int sub= 0; sub < msbD; sub++) {
            if ((((unsigned int)((*TSTRD) >> sub)) & 0x1) != 0) {
               if(mTIMERDAgent->mTIMERD_SUB[sub]->mCounterEnable==false){
                    mTIMERDAgent->mTIMERD_SUB[sub]->RunningCounter(true);
               }
            } else {
                if(mTIMERDAgent->mTIMERD_SUB[sub]->mCounterEnable==true){
                    mTIMERDAgent->mTIMERD_SUB[sub]->RunningCounter(false);
                }
            }
        }
    } 
}//}}}

bool ATU5_TIMERD_Func::Get_CCAPSELD_CCAPSEL(void)
{//{{{
    return (bool)(*CCAPSELD)["CCAPSEL"];
}//}}}

void ATU5_TIMERD_Func::Set_CICRD_CICRD(unsigned int index, unsigned int value)
{//{{{
    (*CICRD[index]) = value;
}//}}}

// vim600: set fold method=marker :
