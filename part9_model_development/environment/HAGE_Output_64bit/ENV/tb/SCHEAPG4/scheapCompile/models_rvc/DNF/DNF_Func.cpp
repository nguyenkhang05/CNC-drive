// ---------------------------------------------------------------------
// $Id: DNF_Func.cpp,v 1.3 2020/02/05 04:31:04 ngathunguyen Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <cstdarg>
#include "DNF_Func.h"
#include "DNF_AgentController.h"

///Constructor of DNF_Func class
DNF_Func::DNF_Func(std::string name,
                 const unsigned int channel_num,   
                 DNF_AgentController *AgentControllerPtr):
                 Cdnf_regif(name,32),
                 BusSlaveFuncIf()
{//{{{
    assert(AgentControllerPtr != NULL);
    mModuleName = name;
    mAgentController = AgentControllerPtr;
    mAgentController->CommandInit(name);
    Cdnf_regif::set_instance_name(name);
    mChannelNum = channel_num;
    mChannelInfo = new ChannelInfo[mChannelNum];
}//}}}

///Destructor of DNF_Func class
DNF_Func::~DNF_Func(void)
{//{{{
    delete[] mChannelInfo;
}//}}}

///Reset registers and variables
void DNF_Func::Reset(const std::string rst_name, const bool is_active)
{//{{{
    if (rst_name == "PRESETZ") {
        Cdnf_regif::EnableReset(is_active);
    } else {
        if (!is_active) {
            ControlChannel();
        }
    }
    if (is_active) {
        for (unsigned int i=0; i<mChannelNum; i++) {
            DisableChannel(i);
        }
    }
}//}}}

///Set parameters to apberrslv_uimpl_regif class
void DNF_Func::RegisterHandler(const std::vector<std::string>& args)
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
                msg_return = Cdnf_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Cdnf_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

bool DNF_Func::IsChannelEnabled(const unsigned int channel_id)
{//{{{
    return mChannelInfo[channel_id].CurrentStatus();
}//}}}

void DNF_Func::HandleInputSample(const unsigned int channel_id, const bool sample)
{//{{{
    bool is_counter_reset = mChannelInfo[channel_id].UpdateSampleVal(sample);
    CountUpSamplingTime(channel_id, is_counter_reset);
}//}}}

void DNF_Func::CountUpSamplingTime(const unsigned int channel_id, const bool is_counter_reset)
{//{{{
    if (is_counter_reset) {
        mChannelInfo[channel_id].ResetCounter();
    }
    mChannelInfo[channel_id].CountUp();
    CheckSamplingTime(channel_id);
}//}}}

void DNF_Func::CheckSamplingTime(const unsigned int channel_id)
{//{{{
    if (mChannelInfo[channel_id].CompareWithThreshold((unsigned int)(*DNFACTL)["NFSTS"] + 2)) {
        mChannelInfo[channel_id].ResetCounter();
        // Call function to output DNFATNFOUT
        mAgentController->OutputDNFATNFOUT(channel_id, mChannelInfo[channel_id].GetCurrentSampleVal());
    } else {
        mAgentController->GetNextSample(channel_id);
    }
}//}}}

unsigned int DNF_Func::GetClockPrescaler(void)
{//{{{
    return (1 << (unsigned int)(*DNFACTL)["PRS"]);
}//}}}

///Access read to registers
void DNF_Func::read(unsigned int offsetAddress, 
                            TlmBasicPayload& trans, 
                            BusTime_t* t, 
                            bool debug)
{//{{{
     // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // When clock = 0, or reset state, only Software debbugger can read register 
    if ( (!mAgentController->CheckClock("PCLK")) || mAgentController->GetResetStatus("PRESETZ") ){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }
    
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Access write to registers
void DNF_Func::write(unsigned int offsetAddress, 
                             TlmBasicPayload& trans, 
                             BusTime_t* t, 
                             bool debug)
{//{{{
     // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    
    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only Software debbugger write
    if (!mAgentController->CheckClock("PCLK")){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }
    
    if (!debug) {
         reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
         reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Callback function of DNFAEN register
void DNF_Func::cb_DNFAEN_NFENL(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock("PCLK")&&mAgentController->CheckClock("DNFATCKI")) {
        ControlChannel();
    }
}//}}}

///Callback function of DNFAENH register
void DNF_Func::cb_DNFAENH_NFENH(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock("PCLK")&&mAgentController->CheckClock("DNFATCKI")) {
        (*DNFAEN) = ((unsigned int)(*DNFAEN) & 0x00FF) | ((unsigned int)(*DNFAENH) << 8);
        ControlChannel();
    }
}//}}}

///Callback function of DNFAENL register
void DNF_Func::cb_DNFAENL_NFENL(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock("PCLK")&&mAgentController->CheckClock("DNFATCKI")) {
        (*DNFAEN) = ((unsigned int)(*DNFAEN) & 0xFF00) | (unsigned int)(*DNFAENL);
        ControlChannel();
    }
}//}}}

///Enable/disable channel
void DNF_Func::ControlChannel(void)
{//{{{
    for (unsigned int i=0; i<mChannelNum; i++) {
        if (mChannelInfo[i].UpdateStt((bool)(((*DNFAEN) >> i) & 0x1))) {
            if (mChannelInfo[i].CurrentStatus()) {
                mAgentController->print_msg("info","Channel %d is enabled.\n", i);
                EnableChannel(i);
            } else {
                mAgentController->print_msg("info","Channel %d is disabled.\n", i);
                DisableChannel(i);
            }
        }
    }
}//}}}

///Enable channel of DNF
void DNF_Func::EnableChannel(const unsigned int channel_id)
{//{{{
    // Get sample from input GINn -> call API of DNF
    if (!(mAgentController->GetResetStatus("DNFATRESZ"))) {
        mAgentController->SwitchToEnableMode(channel_id);
    }
}//}}}

//Disable channel of DNF
void DNF_Func::DisableChannel(const unsigned int channel_id)
{//{{{
    mChannelInfo[channel_id].Init();
    // Output value of other input -> call API of DNF
    if (!(mAgentController->GetResetStatus("DNFATRESZ"))) {
        mAgentController->SwitchToDisableMode(channel_id);
    }
}//}}}

// vim600: set foldmethod=marker :
