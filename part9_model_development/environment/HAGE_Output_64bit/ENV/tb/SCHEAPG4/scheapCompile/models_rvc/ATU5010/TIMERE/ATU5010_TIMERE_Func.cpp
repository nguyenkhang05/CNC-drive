// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERE_Func.cpp,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#include "ATU5010_TIMERE_Func.h"
#include "atu5010_timere_regif.h"
#include "ATU5010_TIMERE_AgentController.h"
#include "ATU5010_TIMERE_SUB.h"
#include "ATU5010_TIMERE_SUB_Func.h"

///*********************************
/// Function     : ATU5010_TIMERE
/// Description  : Constructor of ATU5010_TIMERE_Func class
///*********************************
ATU5010_TIMERE_Func::ATU5010_TIMERE_Func (std::string name, 
                       ATU5010_TIMERE_AgentController *TIMEREAgentPtr):
                       Catu5010_timere_regif(name, 32)
                       //BusSlaveFuncIf()
{//{{{ 
    assert(TIMEREAgentPtr != NULL);
    mModuleName = name;
    mTIMEREAgent = TIMEREAgentPtr;
    mTIMEREAgent->CommandInit(name);
    Catu5010_timere_regif::set_instance_name(name);
    mTimerEStatus = false;
}//}}} 

///*********************************
/// Function     : ~ATU5010_TIMERE_Func
/// Description  : Destructor of ATU5010_TIMERE_Func class
///*********************************
ATU5010_TIMERE_Func::~ATU5010_TIMERE_Func (void)
{//{{{
}//}}}

///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5010_TIMERE_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
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
    } else if ((0x4100  <= addr) && (addr < 0x4198)) { //sub0
        mTIMEREAgent->mTIMERE_SUB[0]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    }  else if ((0x4200 <= addr) && (addr < 0x4298)) {//sub1
        mTIMEREAgent->mTIMERE_SUB[1]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    }  else if ((0x4300 <= addr) && (addr < 0x4398)) {//sub2
        mTIMEREAgent->mTIMERE_SUB[2]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    }  else if ((0x4400 <= addr) && (addr < 0x4498)) {//sub3
        mTIMEREAgent->mTIMERE_SUB[3]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    }  else if ((0x4500 <= addr) && (addr < 0x4598)) {//sub4
        mTIMEREAgent->mTIMERE_SUB[4]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    }  else if ((0x4600 <= addr) && (addr < 0x4698)) {//sub5
        mTIMEREAgent->mTIMERE_SUB[5]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    }  else if ((0x4700 <= addr) && (addr < 0x4798)) {//sub6
        mTIMEREAgent->mTIMERE_SUB[6]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    }  else if ((0x4800 <= addr) && (addr < 0x4898)) {//sub7
        mTIMEREAgent->mTIMERE_SUB[7]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    }  else if ((0x4900 <= addr) && (addr < 0x4998)) {//sub8
        mTIMEREAgent->mTIMERE_SUB[8]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    }  else if ((0x4a00 <= addr) && (addr < 0x4a98)) {//sub9
        mTIMEREAgent->mTIMERE_SUB[9]->mTIE_SUBFunc->read(offsetfw,trans,t,debug);
    } 
}//}}}

///*********************************
/// Function     : write
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void ATU5010_TIMERE_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int offsetfw = offsetAddress & 0x00FF;
    unsigned int addr  = (unsigned int)trans.get_address() & 0xFFFF;
    if ((0x4000 <= addr) && (addr < 0x4008)) {
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
     } else if ((0x4100  <= addr) && (addr < 0x4198)) { //sub0
        mTIMEREAgent->mTIMERE_SUB[0]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
     }  else if ((0x4200 <= addr) && (addr < 0x4298)) {//sub1
        mTIMEREAgent->mTIMERE_SUB[1]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
     }  else if ((0x4300 <= addr) && (addr < 0x4398)) {//sub2
        mTIMEREAgent->mTIMERE_SUB[2]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
     }  else if ((0x4400 <= addr) && (addr < 0x4498)) {//sub3
        mTIMEREAgent->mTIMERE_SUB[3]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
     }  else if ((0x4500 <= addr) && (addr < 0x4598)) {//sub4
        mTIMEREAgent->mTIMERE_SUB[4]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
     }  else if ((0x4600 <= addr) && (addr < 0x4698)) {//sub5
        mTIMEREAgent->mTIMERE_SUB[5]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
     }  else if ((0x4700 <= addr) && (addr < 0x4798)) {//sub6
        mTIMEREAgent->mTIMERE_SUB[6]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
     }  else if ((0x4800 <= addr) && (addr < 0x4898)) {//sub7
        mTIMEREAgent->mTIMERE_SUB[7]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
     }  else if ((0x4900 <= addr) && (addr < 0x4998)) {//sub8
        mTIMEREAgent->mTIMERE_SUB[8]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
	 } else if ((0x4a00 <= addr) && (addr < 0x4a98)) {//sub9
        mTIMEREAgent->mTIMERE_SUB[9]->mTIE_SUBFunc->write(offsetfw,trans,t,debug);
	 } 
}//}}}
///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ATU5010_TIMERE_Func::RegisterHandler (const bool is_common, const unsigned int sub_id ,const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
        for (unsigned int index = 0; index < ATU5010_TIMERE_AgentController::emSub; index++) {//Dump Reg of all sub
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
                msg_return = Catu5010_timere_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
            for (unsigned int index = 0; index < ATU5010_TIMERE_AgentController::emSub; index++) {//Set Message level to all sub
                mTIMEREAgent->mTIMERE_SUB[index]->mTIE_SUBFunc->RegisterHandler(args);
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Catu5010_timere_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                for (unsigned int index = 0; index < ATU5010_TIMERE_AgentController::emSub; index++) {//Set Message level to all sub
                    mTIMEREAgent->mTIMERE_SUB[index]->mTIE_SUBFunc->RegisterHandler(args);
                }
        } else {
            if (is_common) {
                msg_return = Catu5010_timere_regif::reg_handle_command(args);
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
void ATU5010_TIMERE_Func::EnableReset (const bool is_active)
{//{{{ 
    Catu5010_timere_regif::EnableReset(is_active);
    if(is_active){
         mTimerEStatus = false;
    }
}//}}}

///*********************************
/// Function     : TimerEStatus
/// Description  : TimerE is enabled from Wrapper
///*********************************
void ATU5010_TIMERE_Func::TimerEStatus(const bool is_active)
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
void ATU5010_TIMERE_Func::cb_TSTRE_STRE0(RegCBstr str)
{//{{{
    CheckStartCondition();
}//}}}
///*********************************
/// Function     : CheckStartCondition
/// Description  : Call-back function
///*********************************
void ATU5010_TIMERE_Func::CheckStartCondition(void)
{//{{{
    if (mTimerEStatus) {
        for (unsigned int sub= 0; sub < ATU5010_TIMERE_AgentController::emSub; sub++) {
            if ((((unsigned int)((*TSTRE) >> sub)) & 0x1) != 0) {
                mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockEnable = true;
            } else {
                mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockEnable = false;
            }
            for (unsigned int index = 0; index < 4; index++) {
                   mTIMEREAgent->mTIMERE_SUB[sub]->mPreStartTimerEvent[index].notify(SC_ZERO_TIME);
            }
        }
    } else {
        for (unsigned int sub= 0; sub < ATU5010_TIMERE_AgentController::emSub; sub++) {
               mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockEnable = false;
               for (unsigned int index = 0; index < 4; index++) {
                   mTIMEREAgent->mTIMERE_SUB[sub]->mPreStartTimerEvent[index].notify(SC_ZERO_TIME);
               }
        }
    }
}//}}}
///*********************************
/// Function     : cb_SBRLENE_SBRLDENE0
/// Description  : Call-back function
///*********************************
void ATU5010_TIMERE_Func::cb_SBRLENE_SBRLDENE0(RegCBstr str)
{//{{{
    std::ostringstream bit_name;
        for (unsigned int sub= 0; sub < ATU5010_TIMERE_AgentController::emSub; sub++) {
            if ((((unsigned int)((*SBRLENE) >> sub)) & 0x1) != 0) {
                mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockReloadEnable = true;
            } else {
                mTIMEREAgent->mTIMERE_SUB[sub]->mSubBlockReloadEnable = false;
            }
        }
}//}}}

// vim600: set foldmethod=marker :
