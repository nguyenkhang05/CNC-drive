// ---------------------------------------------------------------------
// $Id: ATU5_TIMERF_Func.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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

#include "ATU5_TIMERF_Func.h"
#include "ATU5_TIMERF_AgentController.h"

///Constructor of ATU5_TIMERF_Func class
ATU5_TIMERF_Func::ATU5_TIMERF_Func (std::string name, ATU5_TIMERF_AgentController *ATU5_TIMERF_AgentControllerPtr , unsigned int subblock):
                Catu5_timerf_regif(name, 32 , subblock)
{//{{{
    msbF = subblock;
    assert(ATU5_TIMERF_AgentControllerPtr != NULL);
    mATU5_TIMERF_AgentController = ATU5_TIMERF_AgentControllerPtr;
    mATU5_TIMERF_AgentController->CommandInit(name);
    Catu5_timerf_regif::set_instance_name(name);
    mModuleName = name;

    mPVFCRF01writable = false;
}//}}}

///Destructor of ATU5_TIMERF_Func class
ATU5_TIMERF_Func::~ATU5_TIMERF_Func (void)
{//{{{
}//}}}

///Reset operation of ATU5_TIMERF_Func class
void ATU5_TIMERF_Func::Reset (const bool is_active)
{//{{{
    Catu5_timerf_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ATU5_TIMERF_Func::RegisterHandler (const bool is_common, const unsigned int sub_id ,const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
        for (unsigned int index = 0; index < msbF; index++) {//Dump Reg of all sub
            mATU5_TIMERF_AgentController->mTimerF_Sub[index]->mATU5_TIMERF_Sub_Func->RegisterHandler(args);
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Catu5_timerf_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
            for (unsigned int index = 0; index < msbF; index++) {//Set Message level to all sub
                mATU5_TIMERF_AgentController->mTimerF_Sub[index]->mATU5_TIMERF_Sub_Func->RegisterHandler(args);
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Catu5_timerf_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                mATU5_TIMERF_AgentController->mTimerF_Sub[sub_id]->mATU5_TIMERF_Sub_Func->RegisterHandler(args);
        } else {
            if (is_common) {
                msg_return = Catu5_timerf_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
            } else {
                mATU5_TIMERF_AgentController->mTimerF_Sub[sub_id]->mATU5_TIMERF_Sub_Func->RegisterHandler(args);
            }
        }
    }
}//}}}
///Virtual function of BusSlaveFuncIf class
void ATU5_TIMERF_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());

    if ((ATU5_TIMERF_AgentController::eComStart <= addr) && (addr <= ATU5_TIMERF_AgentController::eComEnd)) {
        // Read from Common registers
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } 
    for(unsigned int i = 0 ; i <   msbF ; i++)
    {
        if ((0x5040 + i * 0x40 <= addr) && (addr < 0x5040 + (i +1)*0x40)) {
            mATU5_TIMERF_AgentController->mTimerF_Sub[i]->mATU5_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
        }
    }

}//}}}

///Virtual function of BusSlaveFuncIf class
void ATU5_TIMERF_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFF;

    if ((ATU5_TIMERF_AgentController::eComStart <= addr) && (addr <= ATU5_TIMERF_AgentController::eComEnd)) {
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } 

    for(unsigned int i = 0 ; i <   msbF ; i++)
    {
        if ((0x5040 + i * 0x40 <= addr) && (addr < 0x5040 + (i +1)*0x40)) {
            mATU5_TIMERF_AgentController->mTimerF_Sub[i]->mATU5_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
        }
    }

}//}}}

/// Virtual function of PVFCRF register in Catu5_timerf_regif class
void ATU5_TIMERF_Func::cb_PVFCRF_PVFCRKEY (RegCBstr str)
{//{{{
    if ((!mATU5_TIMERF_AgentController->mATU5->GetResetStatus("presetz")) && (!mATU5_TIMERF_AgentController->mATU5->GetResetStatus("presetz_bif")) && 
            mATU5_TIMERF_AgentController->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_AgentController->mATU5->CheckClockPeriod("CLK_HSB")) {
        if ((unsigned int)(*PVFCRF)["PVFCRKEY"] == 0xC9) {
            mPVFCRF01writable = true;
        }
        else {
            (*PVFCRF) = str.pre_data;
        }
    }
    (*PVFCRF)["PVFCRKEY"] = 0;
}//}}}

void ATU5_TIMERF_Func::cb_TSTRF_STRF (RegCBstr str)
{//{{{
    unsigned int pre_value = str.pre_data;
    if ((!mATU5_TIMERF_AgentController->mATU5->GetResetStatus("presetz")) && (!mATU5_TIMERF_AgentController->mATU5->GetResetStatus("presetz_bif")) && 
	    mATU5_TIMERF_AgentController->mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIMERF_AgentController->mATU5->CheckClockPeriod("CLK_HSB")) {
        for (unsigned int i = 0; i < msbF; i++) {
            if ((((unsigned int)((*TSTRF) >> i)&0x1) != 0) && (((pre_value >>i)&0x1) == 0)) {
                mATU5_TIMERF_AgentController->NotifyEvent(i,1);
            } else if ((((unsigned int)((*TSTRF) >> i)&0x1) == 0) && (((pre_value >>i)&0x1) != 0)) {
                mATU5_TIMERF_AgentController->NotifyEvent(i,0);
            }   
        }
    }
}//}}}

bool ATU5_TIMERF_Func::SubblockIsEnable(const unsigned int sub_id) {
    return (((unsigned int)((*TSTRF) >> sub_id)&0x1) != 0);
}

bool ATU5_TIMERF_Func::GRDFCMEnabled (void) {
    return (bool)(*PVFCRF)["GRDFCMEN"];
}

bool ATU5_TIMERF_Func::BKCRWENEnabled (void) {
    return (bool)(*PVFCRF)["BKCRWEN"];
}

bool ATU5_TIMERF_Func::IsNoiseCancelEn(const unsigned int sub)
{//{{{
    return (bool)((unsigned int)(*NCCRF)["NCEF"] & (1<<sub));
}//}}}

unsigned int ATU5_TIMERF_Func::GetNoiseCancelMode (const unsigned int sub)
{//{{{
    unsigned int ret = 0;
    if ((bool)((unsigned int)(*NCMCR1F)["NCM1F"] & (1<<sub))){
        if ((bool)((unsigned int)(*NCMCR2F)["NCM2F"] & (1<<sub))) {
            ret = 2;
        } else {
            ret = 1;
        }
    }
    return ret;
}//}}}
