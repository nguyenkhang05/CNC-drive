// -----------------------------------------------------------------------------
// $Id: hold_cvm.cpp,v 1.6 2012/12/19 04:09:30 trongtruong Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------


#include "hold_cvm.h"
    
Chold_cvm::Chold_cvm(sc_module_name name): Cgeneral_reset(name)
        ,CLK("CLK")
        ,SIGIN0("SIGIN0")
        ,SIGIN1("SIGIN1")
        ,SIGIN2("SIGIN2")
        ,SIGIN3("SIGIN3")
        ,SIGIN4("SIGIN4")
        ,EN0("EN0")
        ,EN1("EN1")
        ,EN2("EN2")
        ,EN3("EN3")
        ,EN4("EN4")
        ,RESZ("RESZ")
        ,SIGOUT0("SIGOUT0")
        ,SIGOUT1("SIGOUT1")
        ,SIGOUT2("SIGOUT2")
        ,SIGOUT3("SIGOUT3")
        ,SIGOUT4("SIGOUT4")
{//{{{
    CommandInit(this->name());
    Initialize(true);

    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << RESZ;

    SC_METHOD(SIGINMethod);
    dont_initialize();
    sensitive << EN0;
    sensitive << EN1;
    sensitive << EN2;
    sensitive << EN3;
    sensitive << EN4;
    sensitive << SIGIN0;
    sensitive << SIGIN1;
    sensitive << SIGIN2;
    sensitive << SIGIN3;
    sensitive << SIGIN4;
    sensitive << mReadPortEvent;

    SC_METHOD(CLKMethod);
    dont_initialize();
    sensitive << CLK;
    sensitive << mReadPortEvent;

    SC_METHOD(SIGOUTMethod);
    dont_initialize();
    sensitive << mSIGOUTEvent;
}//}}}

Chold_cvm::~Chold_cvm()
{//{{{
}//}}}

void Chold_cvm::Initialize(bool Start)
{//{{{
    if(Start){
        mCLKPeriod = 0;
        SIGOUT0.initialize(0);
        SIGOUT1.initialize(0);
        SIGOUT2.initialize(0);
        SIGOUT3.initialize(0);
        SIGOUT4.initialize(0);
    } else {
        SIGOUT0.write(0);
        SIGOUT1.write(0);
        SIGOUT2.write(0);
        SIGOUT3.write(0);
        SIGOUT4.write(0);
    }
}//}}}

double Chold_cvm::CalculateRisingEdgeTime()
{//{{{
    double cur_time = sc_time_stamp().to_double();
    unsigned int index = (unsigned int)(cur_time / mCLKPeriod) + 1;
    return mCLKPeriod*index - cur_time;
}//}}}

void Chold_cvm::SIGINMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod() || mCLKPeriod == 0){
        return;
    }
    double syn_time = CalculateRisingEdgeTime();
    mSIGOUTEvent.notify(syn_time, SC_NS);
}//}}}

void Chold_cvm::SIGOUTMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        Initialize(false);
    } else{
        if(EN0.read() == 1){
            SIGOUT0.write(SIGIN0.read());
        }
        if(EN1.read() == 1){
            SIGOUT1.write(SIGIN1.read());
        }
        if(EN2.read() == 1){
            SIGOUT2.write(SIGIN2.read());
        }
        if(EN3.read() == 1){
            SIGOUT3.write(SIGIN3.read());
        }
        if(EN4.read() == 1){
            SIGOUT4.write(SIGIN4.read());
        }
    }
}//}}}

void Chold_cvm::CLKMethod()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        return;
    }
    double clk_freq = sc_dt::uint64_to_double(CLK.read());
    SetCLKfreq("CLK",clk_freq);
}//}}}

void Chold_cvm::SetCLKfreq(const std::string clk_name, const double clk_freq)
{//{{{
    if(clk_name == "CLK") {
        if(clk_freq > 0) {
            // Calculate the clock period to check the resolution
            double clk_period = (double)(emNanoSecond / clk_freq);
            double time_unit = (double)(emNanoSecond / Cgeneral_reset::GetTimeResolution());
            if(time_unit > clk_period){
                re_printf("warning","The CLK period is less than 1 unit time of system.\n");
                return;
            }
            mCLKPeriod = clk_period;
            sc_time clock_value(mCLKPeriod, SC_NS);
            re_printf("info","CLK is set with a frequency as %f.\n",clk_freq);
        }else {
            re_printf("error","Clock CLK = %f is invalid.\n", clk_freq);
        }
    }else {
        re_printf("warning","Clock name = %s is invalid.\n", clk_name.c_str());
    }
}//}}}

void Chold_cvm::ResetMethod()
{//{{{
    //Check value of RESZ signal
    bool is_PortReset = (RESZ.read() == vpcl::emResetActive) ? true : false;
    // Call reset method of common class
    Cgeneral_reset::Com_ResetMethod(is_PortReset);
}//}}}

void Chold_cvm::AssertReset(const double delay, const double period)
{//{{{
    Cgeneral_reset::Com_AssertReset(delay, period);
}//}}}

void Chold_cvm::EnableReset(const bool is_active)
{//{{{
    if(is_active){
        mSIGOUTEvent.cancel();
        mSIGOUTEvent.notify();
    } else{
        mReadPortEvent.notify();
    }
}//}}}
// vim600: set foldmethod=marker :
