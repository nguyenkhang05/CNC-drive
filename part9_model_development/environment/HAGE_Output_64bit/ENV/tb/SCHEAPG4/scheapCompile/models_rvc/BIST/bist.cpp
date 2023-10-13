// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------


#include "bist.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

/// Constructor of BIST class
Cbist::Cbist(sc_module_name name): Cgeneral_reset(name)
                                  ,vpcl::tlm_tgt_if<32>(name)
                                  ,Cbist_regif((std::string)name, 32)
                                  ,pclk("pclk")
                                  ,preset_n("preset_n")
                                  // Input ports
                                  ,start_bist("start_bist")
                                  // Output ports
                                  ,end_bist("end_bist")
{//{{{
    // Initialize handleCommand  
    CommandInit(this->name());
    Cbist_regif::set_instance_name(this->name());

    // Initialize variables
    mEnd_bistValue = true;
    mIsZeroClock = true;
    mCmdReset = false;

    // Config register values
    mRegValue.push_back(stRegValue(LBISTREF1, 0x000A5A5A, 0x000A5A5A));
    mRegValue.push_back(stRegValue(LBISTREF2, 0x0005A5A5, 0x0005A5A5));
    mRegValue.push_back(stRegValue(MBISTREF1, 0x000AA55A, 0x000AA55A));
    mRegValue.push_back(stRegValue(MBISTREF2, 0x00055AA5, 0x00055AA5));
    mRegValue.push_back(stRegValue(LBISTSIG1, 0x0005A5A5, 0x000A5A5A));
    mRegValue.push_back(stRegValue(LBISTSIG2, 0x000A5A5A, 0x0005A5A5));
    mRegValue.push_back(stRegValue(MBISTSIG1, 0x00055AA5, 0x000AA55A));
    mRegValue.push_back(stRegValue(MBISTSIG2, 0x000AA55A, 0x00055AA5));
    mRegValue.push_back(stRegValue(MBISTFTAGL1, 0xFFFFFFFF, 0x00000000));
    mRegValue.push_back(stRegValue(MBISTFTAGH1, 0xFFFFFFFF, 0x00000000));
    mRegValue.push_back(stRegValue(MBISTFTAGL2, 0xFFFFFFFF, 0x00000000));
    mRegValue.push_back(stRegValue(MBISTFTAGH2, 0xFFFFFFFF, 0x00000000));
    mRegValue.push_back(stRegValue(BSEQ0ST, 0x00000001, 0x00000002));
    mRegValue.push_back(stRegValue(BSEQ0STB, 0x00000002, 0x00000001));
    mRegValue.push_back(stRegValue(BISTST, 0x0000000F, 0x00000000));

    // Initialize output ports
    end_bist.initialize(true);

    // Reset Method
    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << preset_n;

    // Clock Method
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << pclk;
    sensitive << mReadPortEvent;

    // Operation Method
    SC_METHOD(start_bistChangeMethod);
    dont_initialize();
    sensitive << start_bist;
    sensitive << mReadPortEvent;

    SC_METHOD(ProcessFinishMethod);
    dont_initialize();
    sensitive << mProcessFinishEvent;

    // Update Output ports Method
    SC_METHOD(WriteOutputPortMethod);
    dont_initialize();
    sensitive << mWriteOutputPortEvent;

    // TLM AT, LT
    SetTlmParameter(false, 0);
}//}}}


// Destructor of Cbist class
Cbist::~Cbist()
{//{{{
}//}}}


// Process debug transport request
unsigned int Cbist::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
{//{{{
    // Get Generic Payload Attributes
    tlm::tlm_command command = tlm::TLM_IGNORE_COMMAND;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, true);
    sc_assert(data_ptr != NULL);

    if (command == tlm::TLM_READ_COMMAND) {
        memset(data_ptr, 0, data_length);
        status = reg_rd_dbg((unsigned int)address, data_ptr, data_length);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr_dbg((unsigned int)address, data_ptr, data_length);
    } else {
        // TLM_IGNORE_COMMAND
        status = true;
        data_length = 0;
    }

    // Set response status & return value
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status) {
        return data_length;
    } else {
        return 0;
    }
}//}}}


// Process transport request
void Cbist::tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    if(Cgeneral_reset::IsResetPeriod()) {
        re_printf("error","Read/Write access is forbidden while resetting.\n");
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return;
    }

    // Get Generic Payload Attributes
    tlm::tlm_command command = tlm::TLM_IGNORE_COMMAND;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, false);

    if (!status) {
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return;
    }
    sc_assert(data_ptr != NULL);

    if (command == tlm::TLM_READ_COMMAND) {
        memset(data_ptr, 0, data_length);
        reg_rd((unsigned int)address, data_ptr, data_length);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        reg_wr((unsigned int)address, data_ptr, data_length);
    }

    // Set response status
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}//}}}


// Output new value to outuput ports
void Cbist::WriteOutputPortMethod(void)
{//{{{
    if (Cgeneral_reset::IsResetPeriod()) {
        // Initialize output ports in reset period
        end_bist.write(1);
    } else {
        end_bist.write(mEnd_bistValue);
    }
}//}}}


// start_bist change process
void Cbist::start_bistChangeMethod(void)
{//{{{
    if(Cgeneral_reset::IsResetPeriod()) {
        return;
    }

    if (mIsZeroClock) {
        re_printf("info","The pclk period is equal 0.\n");
        return;
    }

    bool is_assert = start_bist.read();

    if (is_assert) {
        re_printf("info","start_bist is asserted.\n");

        // De-assert end_bist
        mEnd_bistValue = false;
        mWriteOutputPortEvent.notify();

        // Change registers value
        for (unsigned int i = 0; i < emRegNum; i++) {
            (*mRegValue[i].reg_p) = mRegValue[i].assert_value;
        }

        // Notify to ProcessFinishMethod after processing time
        mProcessFinishEvent.notify((unsigned int)BISTProcessingTime, SC_MS);
    } else {
        // start_bist is de-asserted
        re_printf("info","start_bist is de-asserted.\n");
        // Cancel process wait time
        mProcessFinishEvent.cancel();
    }
}//}}}


// BIST Process Finish
void Cbist::ProcessFinishMethod(void)
{//{{{
    // Compare reference registers and signature registers
    if ((unsigned int)(*LBISTREF1) == (unsigned int)(*LBISTSIG1)) {
        (*BISTST)["LBIST1ST"] = 0;
    } else {
        (*BISTST)["LBIST1ST"] = 1;
    }
    if ((unsigned int)(*LBISTREF2) == (unsigned int)(*LBISTSIG2)) {
        (*BISTST)["LBIST2ST"] = 0;
    } else {
        (*BISTST)["LBIST2ST"] = 1;
    }
    if ((unsigned int)(*MBISTREF1) == (unsigned int)(*MBISTSIG1)) {
        (*BISTST)["MBIST1ST"] = 0;
    } else {
        (*BISTST)["MBIST1ST"] = 1;
    }
    if ((unsigned int)(*MBISTREF2) == (unsigned int)(*MBISTSIG2)) {
        (*BISTST)["MBIST2ST"] = 0;
    } else {
        (*BISTST)["MBIST2ST"] = 1;
    }

    // Assert end_bist
    mEnd_bistValue = true;
    re_printf("info","end_bist is asserted.\n");
    mWriteOutputPortEvent.notify();
}//}}}


// Called by PCLKMethod() or SetCLKfreq handleCommand
void Cbist::SetCLKfreq(const std::string clk_name, const double clk_freq)
{//{{{
    // Clock name must be "pclk"
    if (clk_name == "pclk") {
        double pclk_period = 0;
        if (clk_freq == 0) {
            mIsZeroClock = true;
            //cancel all events when clock change to 0
            mProcessFinishEvent.cancel();
            mWriteOutputPortEvent.cancel();
            mReadPortEvent.cancel();
        } else {
            // Calculate the clock period to check the resolution
            mIsZeroClock = false;
            pclk_period = (double)(emNanoSecond / clk_freq);
            double time_unit = (double)(emNanoSecond / Cgeneral_reset::GetTimeResolution());
            if(time_unit > pclk_period){
                re_printf("warning","The pclk period is less than 1 unit time of system.\n");
                return;
            }
        }

        // Calculate again the latency
        SetTlmParameter(true, pclk_period);
        re_printf("info","pclk is set with a frequency as %lld.%06d.\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
    } else {
        re_printf("warning","Clock name = %s is invalid.\n", clk_name.c_str());
    }
}//}}}


// Setting latency for target parameters
void Cbist::SetTlmParameter(const bool is_ClockSetting, const double pclk_period)
{//{{{
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();

    if (is_ClockSetting) {
        sc_time new_clock(pclk_period, SC_NS);
        tgt_param.bus_clk = new_clock;
    } else {
        tgt_param.fw_req_phase = tlm::END_REQ;
    }

    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}


void Cbist::PCLKMethod(void)
{//{{{ 
    if(Cgeneral_reset::IsResetPeriod()) {
        return;
    }
    
    double pclk_freq = sc_dt::uint64_to_double(pclk.read());
    // Check & update the new clock
    SetCLKfreq("pclk", pclk_freq);
}//}}}


void Cbist::ResetMethod(void)
{//{{{
    //Check value of preset_n signal
    bool is_PortReset = (preset_n.read() == vpcl::emResetActive) ? true : false;
    
    // Call reset method of common class
    Cgeneral_reset::Com_ResetMethod(is_PortReset);
}//}}}


void Cbist::AssertReset(const double delay, const double period)
{//{{{
    mCmdReset = true;
    // Call assert reset function of Cgeneral_reset class
    Cgeneral_reset::Com_AssertReset(delay, period);
}//}}}


void Cbist::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        // Iinitialize variables
        mEnd_bistValue = true;

        // Cancel events
        mProcessFinishEvent.cancel();
        mWriteOutputPortEvent.cancel();
        mReadPortEvent.cancel();

        // Notify to initialize output ports
        mWriteOutputPortEvent.notify();
    } else {
        // Reset is negated, update input ports
        if (!Cgeneral_reset::IsSimStarting()) {
            mReadPortEvent.notify();
        }
    }

    // Initialize registers
    Cbist_regif::EnableReset(is_active);
    if(mCmdReset == false){  // if reset by AssertReset commad, tgt if isn't initialized.
        this->tgt_enable_reset(is_active);
    } else {
        mCmdReset = false;
    }
}//}}}

// vim600: set foldmethod=marker :
