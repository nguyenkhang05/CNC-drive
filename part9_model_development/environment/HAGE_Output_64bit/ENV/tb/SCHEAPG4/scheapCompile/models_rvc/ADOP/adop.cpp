// -----------------------------------------------------------------------------
// $Id: adop.cpp,v 1.1 2015/08/19 03:59:38 sontran Exp $
//
// Copyright(c) 2015 Renesas Electronics Corporation
// Copyright(c) 2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#include "re_define.h"
#include "adop.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//Constructor of ADOP class
Cadop::Cadop(sc_module_name name):
        // Input ports
         pclk("pclk")
        ,preset_n("preset_n")
        ,PWDATA("PWDATA")
        ,ADCATPSEL("ADCATPSEL")
        ,ADCATWEN("ADCATWEN")
        ,ADCATTIN00("ADCATTIN00")
        ,ADCATTIN01("ADCATTIN01")
        ,ADCATTIN02("ADCATTIN02")
        ,ADCATTIN03("ADCATTIN03")
        ,ADCATTIN04("ADCATTIN04")
        ,ADCATTIN05("ADCATTIN05")
        ,ADCATTIN06("ADCATTIN06")
        ,ADCATTIN07("ADCATTIN07")
        ,ADCATTIN08("ADCATTIN08")
        ,ADCATTIN09("ADCATTIN09")
        ,ADCATTIN10("ADCATTIN10")
        ,ADCATTIN11("ADCATTIN11")
        ,ADCATTIN12("ADCATTIN12")
        ,ADCATTIN13("ADCATTIN13")
        ,ADCATTIN14("ADCATTIN14")
        ,ADCATTIN15("ADCATTIN15")
        // Output ports
        ,ADCATSDATA("ADCATSDATA")
        ,ADCATTOUT("ADCATTOUT")
{//{{{
    //Initalze handleCommand  
    CommandInit(this->name());

    //Initialze variable
    Initialize();
    mIsInitialize = true;
    mResetPeriod = 0;
    mIsCmdResetFlag = false;
    mADOPPortResetFlag = false;
    mADOPCmdResetFlag = false;
    
    // Initialize Parameter and Information
    mPCLKPeriod = 0;

    //Initialize output port
    ADCATSDATA.initialize(0);
    ADCATTOUT.initialize(false);

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

    //Clock Method
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << pclk;

    //Normal Method
    SC_METHOD(UpdateOutputMethod);
    dont_initialize();
    sensitive << mUpdateOutputEvent;
    sensitive << ADCATTIN00;
    sensitive << ADCATTIN01;
    sensitive << ADCATTIN02;
    sensitive << ADCATTIN03;
    sensitive << ADCATTIN04;
    sensitive << ADCATTIN05;
    sensitive << ADCATTIN06;
    sensitive << ADCATTIN07;
    sensitive << ADCATTIN08;
    sensitive << ADCATTIN09;
    sensitive << ADCATTIN10;
    sensitive << ADCATTIN11;
    sensitive << ADCATTIN12;
    sensitive << ADCATTIN13;
    sensitive << ADCATTIN14;
    sensitive << ADCATTIN15;
    sensitive << ADCATPSEL;
    sensitive << mAssertResetEvent;
    SC_METHOD(TWENMethod);
    dont_initialize();
    sensitive << ADCATWEN.pos();
    sensitive << mAssertResetEvent;

}//}}}

//destructor of Cadop class
Cadop::~Cadop()
{//{{{
}//}}}

void Cadop::Initialize()
{//{{{
    mIsOperating = false;
    mADCA0OPAIOC = 0;
}//}}}

void Cadop::PCLKMethod(void)  
{//{{{ 
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Cadop::ResetMethod(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(preset_n.read() == vpcl::emResetActive){
        mADOPPortResetFlag = true;
    }else{
        mADOPPortResetFlag = false;
    }
    if(mADOPPortResetFlag){
        mIsInitialize = false;
        re_printf("info","Reset signal is asserted.\n"); 
        this->EnableReset(mADOPPortResetFlag);
        is_cancel_assertreset = true;
    }else{
        if(mIsInitialize){
            mIsInitialize = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info","Reset signal is negated.\n");  
            this->EnableReset(mADOPPortResetFlag);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdResetEvent.cancel();
        mCmdCancelResetEvent.cancel();
        mIsCmdResetFlag = false;
        mADOPCmdResetFlag = false;
    }
}//}}}

void Cadop::AssertResetMethod(void)
{//{{{
    mADOPCmdResetFlag = true;
    this->EnableReset(mADOPCmdResetFlag);
    mCmdCancelResetEvent.notify((unsigned int)mResetPeriod, SC_NS);
}//}}}

void Cadop::DeAssertResetMethod(void)
{//{{{ 
    mADOPCmdResetFlag = false;
    mIsCmdResetFlag = false;
    this->EnableReset(mADOPCmdResetFlag);
}//}}}

//Reset function
void Cadop::EnableReset(const bool is_active)
{//{{{ 
    if(is_active){
        //1.Global variable init
        Initialize();

        //2. Cancel all events
        mUpdateOutputEvent.cancel();
        mUpdateRegisterEvent.cancel();

        //3. Initialize output ports
        mUpdateOutputEvent.notify();
        mUpdateRegisterEvent.notify();
    }
}//}}}

double Cadop::GetTimeResolution(void)
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

void Cadop::AssertReset(const double start_time, const double period)
{//{{{
    if((!mADOPPortResetFlag)&&(!mIsCmdResetFlag)) {
        mIsCmdResetFlag = true;
        re_printf("info","ADOP will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) start_time, FRACTION_TO_INT(start_time));
        mCmdResetEvent.notify(start_time, SC_NS);
        mResetPeriod = (unsigned int)period;
    }else{
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

void Cadop::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{
    double time_unit = 0;
    double pclk_period = 0;
    bool is_period_valid = true;

    // Setting clock values
    if (clk_name == "pclk") {
        if (mIsOperating) {
            re_printf("warning","The frequency of pclk clock is changed while ADOP model is operating.\n");
        }

        if (clk_freq == 0) {
            pclk_period = 0;
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
            re_printf("info","The pclk is set with a frequency as %lld.%06d\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
        }
    } else {
        re_printf("error","Clock name is invalid.\n");
    }
}//}}}

// Check whether there is reset or not
bool Cadop::IsReset(void)
{//{{{
    if (mADOPPortResetFlag || mADOPCmdResetFlag) {
        return true;
    } else {
        return false;
    }
}//}}}

void Cadop::UpdateOutputMethod(void)
{//{{{
    if (IsReset()) {
        ADCATTOUT.write(false);
    } else {
        mIsOperating = true;
        bool adcattin [16] ;
        bool adcattout = false;
        adcattin[15] = ADCATTIN15.read();
        adcattin[14] = ADCATTIN14.read();
        adcattin[13] = ADCATTIN13.read();
        adcattin[12] = ADCATTIN12.read();
        adcattin[11] = ADCATTIN11.read();
        adcattin[10] = ADCATTIN10.read();
        adcattin[ 9] = ADCATTIN09.read();
        adcattin[ 8] = ADCATTIN08.read();
        adcattin[ 7] = ADCATTIN07.read();
        adcattin[ 6] = ADCATTIN06.read();
        adcattin[ 5] = ADCATTIN05.read();
        adcattin[ 4] = ADCATTIN04.read();
        adcattin[ 3] = ADCATTIN03.read();
        adcattin[ 2] = ADCATTIN02.read();
        adcattin[ 1] = ADCATTIN01.read();
        adcattin[ 0] = ADCATTIN00.read();
        if (!ADCATPSEL.read()) { // ADCATPSEL = 0
            adcattout = false;
            for (unsigned int i=0;i<emPortNum;i++) {
                if (adcattin[i] && (((mADCA0OPAIOC>>i)&0x1) == 1)) {
                    adcattout = true;
                    break;
                }
            }
        } else { // ADCATPSEL = 1
            adcattout = true;
            for (unsigned int i=0;i<emPortNum;i++) {
                if (!adcattin[i] && (((mADCA0OPAIOC>>i)&0x1) == 1)) {
                    adcattout = false;
                    break;
                }
            }
        }
        ADCATTOUT.write(adcattout);
    }
}//}}}

void Cadop::TWENMethod(void)
{//{{{
    if (IsReset()) {
        ADCATSDATA.write(0);
    } else {
        unsigned int data = (unsigned int)(PWDATA.read());
        mADCA0OPAIOC = data;
        ADCATSDATA.write((sc_uint<16>)data);
        mUpdateOutputEvent.notify();
    }
}//}}}

// vim600: set foldmethod=marker :
