// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERF_Func.cpp,v 1.1 2016/04/25 08:21:27 binhnguyen Exp $
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

#include "ATU5010_TIMERF_Func.h"
#include "ATU5010_TIMERF_AgentController.h"

///Constructor of ATU5010_TIMERF_Func class
ATU5010_TIMERF_Func::ATU5010_TIMERF_Func (std::string name, ATU5010_TIMERF_AgentController *ATU5010_TIMERF_AgentControllerPtr):
                Catu5010_timerf_regif(name, 32)
{//{{{
    assert(ATU5010_TIMERF_AgentControllerPtr != NULL);
    mATU5010_TIMERF_AgentController = ATU5010_TIMERF_AgentControllerPtr;
    mATU5010_TIMERF_AgentController->CommandInit(name);
    Catu5010_timerf_regif::set_instance_name(name);
    mModuleName = name;

    mPVFCRF01writable = false;
}//}}}

///Destructor of ATU5010_TIMERF_Func class
ATU5010_TIMERF_Func::~ATU5010_TIMERF_Func (void)
{//{{{
}//}}}

///Reset operation of ATU5010_TIMERF_Func class
void ATU5010_TIMERF_Func::Reset (const bool is_active)
{//{{{
    Catu5010_timerf_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void ATU5010_TIMERF_Func::RegisterHandler (const bool is_common, const unsigned int sub_id ,const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
        for (unsigned int index = 0; index < ATU5010_TIMERF_AgentController::emSubNum; index++) {//Dump Reg of all sub
            mATU5010_TIMERF_AgentController->mTimerF_Sub[index]->mATU5010_TIMERF_Sub_Func->RegisterHandler(args);
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Catu5010_timerf_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
            for (unsigned int index = 0; index < ATU5010_TIMERF_AgentController::emSubNum; index++) {//Set Message level to all sub
                mATU5010_TIMERF_AgentController->mTimerF_Sub[index]->mATU5010_TIMERF_Sub_Func->RegisterHandler(args);
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Catu5010_timerf_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                mATU5010_TIMERF_AgentController->mTimerF_Sub[sub_id]->mATU5010_TIMERF_Sub_Func->RegisterHandler(args);
        } else {
            if (is_common) {
                msg_return = Catu5010_timerf_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
            } else {
                mATU5010_TIMERF_AgentController->mTimerF_Sub[sub_id]->mATU5010_TIMERF_Sub_Func->RegisterHandler(args);
            }
        }
    }
}//}}}
///Virtual function of BusSlaveFuncIf class
void ATU5010_TIMERF_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFF;
    memset(trans.get_data_ptr(), 0, trans.get_data_length());

    if ((ATU5010_TIMERF_AgentController::eComStart <= addr) && (addr <= ATU5010_TIMERF_AgentController::eComEnd)) {
        // Read from Common registers
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else if ((ATU5010_TIMERF_AgentController::eSub0Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub0End)) {
        // Read from TimerF subblock 0
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub0]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub1Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub1End)) {
        // Read from TimerF subblock 1
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub1]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub2Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub2End)) {
        // Read from TimerF subblock 2
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub2]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub3Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub3End)) {
        // Read from TimerF subblock 3
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub3]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub4Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub4End)) {
        // Read from TimerF subblock 4
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub4]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub5Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub5End)) {
        // Read from TimerF subblock 5
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub5]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub6Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub6End)) {
        // Read from TimerF subblock 6
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub6]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub7Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub7End)) {
        // Read from TimerF subblock 7
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub7]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub8Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub8End)) {
        // Read from TimerF subblock 8
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub8]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub9Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub9End)) {
        // Read from TimerF subblock 9
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub9]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub10Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub10End)) {
        // Read from TimerF subblock 10
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub10]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub11Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub11End)) {
        // Read from TimerF subblock 11
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub11]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub12Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub12End)) {
        // Read from TimerF subblock 12
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub12]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub13Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub13End)) {
        // Read from TimerF subblock 13
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub13]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub14Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub14End)) {
        // Read from TimerF subblock 14
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub14]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub15Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub15End)) {
        // Read from TimerF subblock 15
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub15]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
    }
    else if ((ATU5010_TIMERF_AgentController::eSub16Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub16End)) {
    	 // Read from TimerF subblock 16
    	 mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub16]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
  	} else if ((ATU5010_TIMERF_AgentController::eSub17Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub17End)) {
    		// Read from TimerF subblock 17
    		mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub17]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
  	} else if ((ATU5010_TIMERF_AgentController::eSub18Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub18End)) {
    		// Read from TimerF subblock 18
    		mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub18]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
  	} else if ((ATU5010_TIMERF_AgentController::eSub19Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub19End)) {
    		// Read from TimerF subblock 19
    		mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub19]->mATU5010_TIMERF_Sub_Func->read(offsetAddress, trans, t, debug);
  	}

}//}}}

///Virtual function of BusSlaveFuncIf class
void ATU5010_TIMERF_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int addr = (unsigned int)trans.get_address() & 0xFFFF;

    if ((ATU5010_TIMERF_AgentController::eComStart <= addr) && (addr <= ATU5010_TIMERF_AgentController::eComEnd)) {
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else if ((ATU5010_TIMERF_AgentController::eSub0Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub0End)) {
        // write from TimerF subblock 0
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub0]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub1Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub1End)) {
        // write from TimerF subblock 1
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub1]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub2Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub2End)) {
        // write from TimerF subblock 2
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub2]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub3Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub3End)) {
        // write from TimerF subblock 3
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub3]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub4Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub4End)) {
        // write from TimerF subblock 4
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub4]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub5Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub5End)) {
        // write from TimerF subblock 5
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub5]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub6Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub6End)) {
        // write from TimerF subblock 6
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub6]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub7Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub7End)) {
        // write from TimerF subblock 7
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub7]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub8Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub8End)) {
        // write from TimerF subblock 8
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub8]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub9Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub9End)) {
        // write from TimerF subblock 9
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub9]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub10Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub10End)) {
        // write from TimerF subblock 10
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub10]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub11Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub11End)) {
        // write from TimerF subblock 11
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub11]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub12Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub12End)) {
        // write from TimerF subblock 12
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub12]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub13Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub13End)) {
        // write from TimerF subblock 13
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub13]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub14Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub14End)) {
        // write from TimerF subblock 14
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub14]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
    } else if ((ATU5010_TIMERF_AgentController::eSub15Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub15End)) {
        // write from TimerF subblock 15
        mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub15]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
  	}    
      else if ((ATU5010_TIMERF_AgentController::eSub16Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub16End)) {
    		// write from TimerF subblock 16
    		mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub16]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
  	} else if ((ATU5010_TIMERF_AgentController::eSub17Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub17End)) {
    		// write from TimerF subblock 17
    		mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub17]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
  	} else if ((ATU5010_TIMERF_AgentController::eSub18Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub18End)) {
    		// write from TimerF subblock 18
    		mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub18]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
  	} else if ((ATU5010_TIMERF_AgentController::eSub19Start <= addr) && (addr <= ATU5010_TIMERF_AgentController::eSub19End)) {
    		// write from TimerF subblock 19
    		mATU5010_TIMERF_AgentController->mTimerF_Sub[ATU5010_TIMERF_AgentController::emSub19]->mATU5010_TIMERF_Sub_Func->write(offsetAddress, trans, t, debug);
  	}

}//}}}

/// Virtual function of PVFCRF register in Catu5010_timerf_regif class
void ATU5010_TIMERF_Func::cb_PVFCRF_PVFCRKEY (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((*PVFCRF)["PVFCRKEY"] == 0xC9) {
            mPVFCRF01writable = true;
        }
        else {
            (*PVFCRF) = str.pre_data;
        }
    }
    (*PVFCRF)["PVFCRKEY"] = 0;
}//}}}

void ATU5010_TIMERF_Func::cb_TSTRF_STRF0 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF0"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock0,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock0,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF1 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF1"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock1,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock1,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF2 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF2"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock2,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock2,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF3 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF3"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock3,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock3,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF4 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF4"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock4,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock4,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF5 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF5"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock5,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock5,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF6 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF6"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock6,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock6,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF7 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF7"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock7,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock7,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF8 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF8"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock8,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock8,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF9 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF9"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock9,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock9,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF10 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF10"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock10,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock10,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF11 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF11"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock11,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock11,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF12 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF12"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock12,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock12,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF13 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF13"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock13,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock13,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF14 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF14"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock14,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock14,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF15 (RegCBstr str)
{//{{{
    if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
        if ((bool)(*TSTRF)["STRF15"]) {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock15,1);
        }
        else {
            mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock15,0);
        }
    }
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF16(RegCBstr str)
{//{{{
  	if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
    		if ((bool)(*TSTRF)["STRF16"]) {
    			  mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock16, 1);
    		}
    		else {
    			  mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock16, 0);
    		}
  	}
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF17(RegCBstr str)
{//{{{
  	if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
    		if ((bool)(*TSTRF)["STRF17"]) {
    			  mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock17, 1);
    		}
    		else {
    			  mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock17, 0);
    		}
  	}
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF18(RegCBstr str)
{//{{{
  	if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
    		if ((bool)(*TSTRF)["STRF18"]) {
    			  mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock18, 1);
    		}
    		else {
    			  mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock18, 0);
    		}
  	}
}//}}}
void ATU5010_TIMERF_Func::cb_TSTRF_STRF19(RegCBstr str)
{//{{{
  	if (!mATU5010_TIMERF_AgentController->mATU5010->GetResetStatus("presetz_bif") && mATU5010_TIMERF_AgentController->mATU5010->CheckClockPeriod("CLK_HSB")) {
    		if ((bool)(*TSTRF)["STRF19"]) {
    			  mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock19, 1);
    		}
    		else {
    			  mATU5010_TIMERF_AgentController->NotifyEvent(ATU5010_TIMERF_AgentController::emStartSubblock19, 0);
    		}
  	}
}//}}}

bool ATU5010_TIMERF_Func::SubblockIsEnable(const unsigned int sub_id) {
    switch(sub_id) {
        case 0:
            return (bool)(*TSTRF)["STRF0"];
            break;
        case 1:
            return (bool)(*TSTRF)["STRF1"];
            break;
        case 2:
            return (bool)(*TSTRF)["STRF2"];
            break;
        case 3:
            return (bool)(*TSTRF)["STRF3"];
            break;
        case 4:
            return (bool)(*TSTRF)["STRF4"];
            break;
        case 5:
            return (bool)(*TSTRF)["STRF5"];
            break;
        case 6:
            return (bool)(*TSTRF)["STRF6"];
            break;
        case 7:
            return (bool)(*TSTRF)["STRF7"];
            break;
        case 8:
            return (bool)(*TSTRF)["STRF8"];
            break;
        case 9:
            return (bool)(*TSTRF)["STRF9"];
            break;
        case 10:
            return (bool)(*TSTRF)["STRF10"];
            break;
        case 11:
            return (bool)(*TSTRF)["STRF11"];
            break;
        case 12:
            return (bool)(*TSTRF)["STRF12"];
            break;
        case 13:
            return (bool)(*TSTRF)["STRF13"];
            break;
        case 14:
            return (bool)(*TSTRF)["STRF14"];
            break;
        case 15:
            return (bool)(*TSTRF)["STRF15"];
            break;
    		case 16:
      			return (bool)(*TSTRF)["STRF16"];
      			break;
    		case 17:
      			return (bool)(*TSTRF)["STRF17"];
      			break;
    		case 18:
      			return (bool)(*TSTRF)["STRF18"];
      			break;
    		case 19:
      			return (bool)(*TSTRF)["STRF19"];
      			break;
        default:
            return false;
            break;
    }
}

bool ATU5010_TIMERF_Func::GRDFCMEnabled (void) {
    return (bool)(*PVFCRF)["GRDFCMEN"];
}

bool ATU5010_TIMERF_Func::BKCRWENEnabled (void) {
    return (bool)(*PVFCRF)["BKCRWEN"];
}

bool ATU5010_TIMERF_Func::IsNoiseCancelEn(const unsigned int sub)
{//{{{
    return (bool)((*NCCRF)["NCEF"] & (1<<sub));
}//}}}

unsigned int ATU5010_TIMERF_Func::GetNoiseCancelMode (const unsigned int sub)
{//{{{
    unsigned int ret = 0;
    if ((bool)((*NCMCR1F)["NCM1F"] & (1<<sub))){
        if ((bool)((*NCMCR2F)["NCM2F"] & (1<<sub))) {
            ret = 2;
        } else {
            ret = 1;
        }
    }
    return ret;
}//}}}
