// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012-2015 Renesas System Design Co., Ltd.
// Copyright(c) 2012-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#include "re_define.h"
#include "ifc.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//Constructor of SAR AD class
Cifc::Cifc(sc_module_name name): sc_module (name)
        ,vpcl::tlm_tgt_if<32>(name)
        ,Cifc_regif((std::string)name, 32)

        ,pclk("pclk")
        ,preset_n("preset_n")
        ,IFVCH("IFVCH")
        ,IFRD0("IFRD0")
        ,IFRD1("IFRD1")
        ,IFRE("IFRE")
        ,ADC0_IFDATA("ADC0_IFDATA")
        ,ADC1_IFDATA("ADC1_IFDATA")
{//{{{

    //Initalze handleCommand  
    CommandInit(this->name());
    Cifc_regif::set_instance_name(this->name());

    //Initialze variable
    mResetPeriod      = 0;
    mIsCmdResetFlag   = false;
    mIFCPortResetFlag = false;  
    mIFCCmdResetFlag  = false;
    mIsInitialize = true;
    mIsStartSim = false;
    mPCLKPeriod = 0;
    mIFRD0Val = false;
    mIFRD1Val = false;
    mIFREVal = false;
    mCurVCNum = 0;
    //Initialize output port
    IFVCH.initialize(0);
    IFRD0.initialize(0);
    IFRD1.initialize(0);
    IFRE.initialize(0);

    //Reset Method
    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << preset_n;

    SC_METHOD(AssertResetMethod);
    dont_initialize();
    sensitive << mCmdResetEvent;

    SC_METHOD(DeAssertResetMethod);
    dont_initialize();
    sensitive << mCmdCancelResetEvent;

    SC_METHOD(UpdateIFVCHMethod);
    dont_initialize();
    sensitive << mUpdateIFVCHEvent;

    SC_METHOD(UpdateIFRD0Method);
    dont_initialize();
    sensitive << mUpdateIFRD0Event;

    SC_METHOD(UpdateIFRD1Method);
    dont_initialize();
    sensitive << mUpdateIFRD1Event;

    SC_METHOD(UpdateIFREMethod);
    dont_initialize();
    sensitive << mUpdateIFREEvent;

    //Clock Method
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << pclk;
    
    SC_METHOD(InitializeMethod);
    SetLatency_TLM (0, true);
}//}}}

//destructor of Cifc class
Cifc::~Cifc()
{//{{{
}//}}}

void Cifc::PCLKMethod(void)  
{//{{{ 
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Cifc::ResetMethod(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(preset_n.read() == vpcl::emResetActive){
        mIFCPortResetFlag = true;
    }else{
        mIFCPortResetFlag = false;
    }
    if(mIFCPortResetFlag){
        mIsInitialize = false;
        re_printf("info","Reset signal is asserted.\n"); 
        this->EnableReset(mIFCPortResetFlag);
        is_cancel_assertreset = true;
    }else{
        if(mIsInitialize){
            mIsInitialize = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info","Reset signal is negated.\n");  
            this->EnableReset(mIFCPortResetFlag);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdResetEvent.cancel();
        mCmdCancelResetEvent.cancel();
        mIsCmdResetFlag = false;
        mIFCCmdResetFlag = false;
    }

}//}}}

void Cifc::AssertResetMethod(void)
{//{{{
    mIFCCmdResetFlag = true;
    this->EnableReset(mIFCCmdResetFlag);
    mCmdCancelResetEvent.notify((unsigned int)mResetPeriod, SC_NS);
}//}}}

void Cifc::DeAssertResetMethod(void)
{//{{{ 
    mIFCCmdResetFlag = false;
    mIsCmdResetFlag = false;
    this->EnableReset(mIFCCmdResetFlag);
}//}}}

void Cifc::InitializeMethod(void)
{//{{{
    mIsInitialize = true;    
    mIsStartSim = true;    
}//}}}

//Function
//tlm_tgt_if function

void Cifc::tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t)
{ //{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr    = 0;
    unsigned char *p_data  = NULL;
    unsigned int size = 0; 
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);
    if(!status){
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(p_data != NULL);

    // Callback method
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        unsigned int adc_num = ProcessReadAccess((unsigned int)addr);
        wait(mPCLKPeriod, SC_NS);
        if (adc_num != emMaxADCNum) {
            UpdateDRValue(adc_num, mCurVCNum);
        }
        status = this->reg_rd((unsigned int) addr, p_data, size);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr((unsigned int) addr, p_data, size);
    }
    // Be necessarily TLM_IGNORE_COMMAND
    else {
        status = true;
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
} //}}}

unsigned int Cifc::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
{//{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, true);
    sc_assert(p_data != NULL);

    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        unsigned int adc_num = ProcessReadAccess((unsigned int)addr);
        if (adc_num != emMaxADCNum) {
            UpdateDRValue(adc_num, mCurVCNum);
        }
        status = reg_rd_dbg((unsigned int) addr, p_data, size);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr_dbg((unsigned int) addr, p_data, size);
    }
    // Be necessarily TLM_IGNORE_COMMAND
    else {
        status = true;
        size = 0;
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status){
        return size;
    } else {
        return 0;
    }
}//}}}

//Reset function
void Cifc::EnableReset(const bool is_active)
{//{{{
    if(is_active){
        re_printf("info","The IFC is reset.\n");

        // Reset all variables
        mIFRD0Val = false;
        mIFRD1Val = false;
        mIFREVal = false;
        mCurVCNum = 0;
        
        // Cancel all events
        mUpdateIFRD0Event.cancel();
        mUpdateIFRD1Event.cancel();
        mUpdateIFVCHEvent.cancel();
        mUpdateIFREEvent.cancel();

        // Reset output ports
        mUpdateIFRD0Event.notify();
        mUpdateIFRD1Event.notify();
        mUpdateIFVCHEvent.notify();
        mUpdateIFREEvent.notify();
    }

    Cifc_regif::EnableReset(is_active);
}//}}}

/// Function is used to notify event when register is read
unsigned int Cifc::ProcessReadAccess(unsigned int addr)
{//{{{
    if (mPCLKPeriod == 0) {
        re_printf("warning","IFC stops operation when the pclk period is set to 0.\n");
    } else {
        if (addr % 4 == 0) {
            mCurVCNum = (addr&emADC0AddrMask)/4;
            if (mCurVCNum < emMaxVirChannel) {
                if ((addr & emADC1AddrMask) == (addr & emADC0AddrMask)) { // ADC0
                    mUpdateIFVCHEvent.notify();
                    mUpdateIFRD0Event.notify();
                    mUpdateIFREEvent.notify();
                    return emADC0Num;
                } else { // ADC1
                    mUpdateIFVCHEvent.notify();
                    mUpdateIFRD1Event.notify();
                    mUpdateIFREEvent.notify();
                    return emADC1Num;
                }
            } else {
                mCurVCNum = 0;
            }
        }
    }
    return emMaxADCNum;
}//}}}

/// Function is used to update the input value into coressponding DR register
void Cifc::UpdateDRValue(const unsigned int adc_num, const unsigned int vc_num)
{//{{{
    unsigned int dr_val = 0;
    if (adc_num == emADC0Num) { // ADC0
        dr_val = (unsigned int) ADC0_IFDATA.read();
    } else { // ADC1
        dr_val = (unsigned int) ADC1_IFDATA.read();
    }
    dr_val = SFPtoFPConvert(dr_val);
    if (adc_num == emADC0Num) {
        (*DR0[vc_num]) = dr_val;
    } else {
        (*DR1[vc_num]) = dr_val;
    }
}//}}}

/// Function is used to convert from Signed Fixed-Point format to Floating-Point format
unsigned int Cifc::SFPtoFPConvert(unsigned int value)
{//{{{
    unsigned int converted_val = 0;
    unsigned int temp_val = (value & (~emDataSignedMask))&emDataMask;
    unsigned int tmp_exp = 0;
    unsigned int exp_val = 0; 
    unsigned int fraction_val = 0;
    // Process signed bit
    if ((value & emDataSignedMask) == emDataSignedMask) {
        converted_val = emSignedMask;
    }
    
    if (temp_val != 0) {
        // Process Exponent // tmp_exp = (EXP - 127) + 15 ==> EXP = exp + 127 - 15
        for (unsigned int i=0; i<=emMaxDRBitVal; i++) {
            if (((unsigned int)(1<<i)) < temp_val) {
                tmp_exp = i;
            } else {
                break;
            }
        }
        exp_val = tmp_exp+127-15;
        converted_val = converted_val | ((exp_val<<23)&emExpMask);

        // Process Fraction // temp_val = 2^tmp_exp + ((FRACTION * 2^-23) * 2^tmp_exp) ==> FRACTION = (temp_val - 2^tmp_exp)*(2^(23-tmp_exp))
        fraction_val = (temp_val-(unsigned int)(1<<tmp_exp))*(unsigned int)(1<<(23 - tmp_exp));
        converted_val = converted_val | (fraction_val&emFractionMask);
    }
    return converted_val;
}//}}}

void Cifc::UpdateIFVCHMethod()
{//{{{
    if (mIFCPortResetFlag || mIFCCmdResetFlag || (mPCLKPeriod == 0)) {
        IFVCH.write(0);
    } else {
        IFVCH.write((sc_uint<6>)mCurVCNum);
    }
}//}}}

void Cifc::UpdateIFRD0Method()
{//{{{
    if (mIFCPortResetFlag || mIFCCmdResetFlag || (mPCLKPeriod == 0)) {
        IFRD0.write(0);
    } else {
        if (!mIFRD0Val) {
            mIFRD0Val = true;
            IFRD0.write(1);
            mUpdateIFRD0Event.notify(mPCLKPeriod*2, SC_NS);
        } else {
            mIFRD0Val = false;
            IFRD0.write(0);
        }
    }
}//}}}

void Cifc::UpdateIFRD1Method()
{//{{{
    if (mIFCPortResetFlag || mIFCCmdResetFlag || (mPCLKPeriod == 0)) {
        IFRD1.write(0);
    } else {
        if (!mIFRD1Val) {
            mIFRD1Val = true;
            IFRD1.write(1);
            mUpdateIFRD1Event.notify(mPCLKPeriod*2, SC_NS);
        } else {
            mIFRD1Val = false;
            IFRD1.write(0);
        }
    }
}//}}}

void Cifc::UpdateIFREMethod()
{//{{{
    if (mIFCPortResetFlag || mIFCCmdResetFlag || (mPCLKPeriod == 0)) {
        IFRE.write(0);
    } else {
        if (!mIFREVal) {
            mIFREVal = true;
            IFRE.write(1);
            mUpdateIFREEvent.notify(mPCLKPeriod, SC_NS);
        } else {
            mIFREVal = false;
            IFRE.write(0);
        }
    }
}//}}}

//handleCommand function
void Cifc::AssertReset(const double start_time, const double period)
{//{{{
    if((!mIFCPortResetFlag)&&(!mIsCmdResetFlag)) {
        mIsCmdResetFlag = true;
        re_printf("info","IFC will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) start_time, FRACTION_TO_INT(start_time));
        mCmdResetEvent.notify(start_time, SC_NS);
        mResetPeriod = (unsigned int)period;
    }else{
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

double Cifc::GetTimeResolution(void)
{//{{{
    double tu_value = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)){
        tu_value = 1000000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)){
        tu_value = 1000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)){
        tu_value = 1000000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)){
        tu_value = 1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)){
        tu_value = 1000;
    } else {
        tu_value = 1;
    }
    return tu_value;
}//}}}

void Cifc::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{
    double time_unit = 0;
    double pclk_period = 0;
    bool is_period_valid = true;

    // Setting clock values
    if (clk_name == "pclk") {
        if (clk_freq <= 0) {
            pclk_period = 0;
            if (mIsStartSim) {
                // Reset output ports when 0 is set to pclk
                mUpdateIFRD0Event.notify();
                mUpdateIFRD1Event.notify();
                mUpdateIFVCHEvent.notify();
                mUpdateIFREEvent.notify();
            }
        } else {
            pclk_period  = (double)(emNanoSecond / clk_freq);
            time_unit    = (double)(emNanoSecond / GetTimeResolution());
            if (time_unit > pclk_period) {
                is_period_valid = false;
                re_printf("warning","The pclk period is less than 1 unit time of system.\n");
            }
        }

        if (is_period_valid) {
            mPCLKPeriod = pclk_period;
            SetLatency_TLM(pclk_period, false);
            re_printf("info","The pclk is set with a frequency as %lld.%06d\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
        }
    } else {
        re_printf("error","Clock name is invalid.\n");
    }
}//}}}

void Cifc::SetLatency_TLM(const double pclk_period, const bool is_constructor)
{//{{{ 
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 1 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 1 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}
// vim600: set foldmethod=marker :
