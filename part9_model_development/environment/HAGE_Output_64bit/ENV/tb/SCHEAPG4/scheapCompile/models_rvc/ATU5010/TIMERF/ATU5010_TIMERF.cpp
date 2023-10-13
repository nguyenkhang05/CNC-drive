// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERF.cpp,v 1.2 2017/04/12 10:15:45 HuyDT1 Exp $
//
// Copyright(c) 2016-2017 Renesas Electronics Corporation
// Copyright(c) 2016-2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <stdint.h>
#include "ATU5010_TIMERF.h"
#include "ATU5010_TIMERF_Func.h"
#include "ATU5010_TIMERF_Sub.h"
#include "ATU5010_TIMERF_Sub_Func.h"

///Constructor of ATU5010_TIMERF class
ATU5010_TIMERF::ATU5010_TIMERF (sc_module_name name, ATU5010 *ATU5010_Ptr):
    sc_module(name),
    ATU5010_TIMERF_AgentController(),
    nc_clk("nc_clk")
{//{{{
    assert(ATU5010_Ptr != NULL);
    mATU5010 = ATU5010_Ptr;

    mATU5010_TIMERF_Func = new ATU5010_TIMERF_Func((std::string)name, this);
    sc_assert(mATU5010_TIMERF_Func != NULL);

    std::ostringstream port_name;
    for (unsigned int index = 0; index < emSubNum; index++) {
        port_name.str("");
        port_name << "TIFA" << index;
        TIFA[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TIFA[index] != NULL);

        port_name.str("");
        port_name << "OVFFI" << index;
        OVFFI[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(OVFFI[index] != NULL);

        port_name.str("");
        port_name << "ICFFI" << index;
        ICFFI[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(ICFFI[index] != NULL);
    }
    for (unsigned int index = 0; index < emClkNum-3; index++) {
        port_name.str("");
        port_name << "CLKBUS" << index;
        CLKBUS[index] = new sc_in<sc_dt::uint64>(port_name.str().c_str());
        sc_assert(CLKBUS[index] != NULL);
    }
    for (unsigned int index = 0; index < 3; index++) {
        port_name.str("");
        port_name << "TIFB" << index;
        TIFB[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TIFB[index] != NULL);
    }

    for (unsigned int index = 0; index < 8; index++) {
        port_name.str("");
        port_name << "TIA" << index;
        TIA[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(TIA[index] != NULL);
    }
    for (unsigned int index = 0; index < 6; index++) {
        port_name.str("");
        port_name << "POE" << index;
        POE[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(POE[index] != NULL);
    }

    for (unsigned int index = 0; index < (emSubNum-3); index++) {
        port_name.str("");
        port_name << "dummy_TIFB_sig" << index;
        dummy_TIFB_sig[index] = new sc_signal<bool>(port_name.str().c_str());
        sc_assert(dummy_TIFB_sig[index] != NULL);
    }
    for (unsigned int index = 0; index < (2*emSubNum-6); index++) {
        port_name.str("");
        port_name << "dummy_POE_sig" << index;
        dummy_POE_sig[index] = new SC_SIGNAL(bool)(port_name.str().c_str());
        sc_assert(dummy_POE_sig[index] != NULL);
    }


    mTimerF_Sub[emSub0]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub0" , this, emSub0 );
    mTimerF_Sub[emSub1]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub1" , this, emSub1 );
    mTimerF_Sub[emSub2]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub2" , this, emSub2 );
    mTimerF_Sub[emSub3]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub3" , this, emSub3 );
    mTimerF_Sub[emSub4]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub4" , this, emSub4 );
    mTimerF_Sub[emSub5]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub5" , this, emSub5 );
    mTimerF_Sub[emSub6]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub6" , this, emSub6 );
    mTimerF_Sub[emSub7]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub7" , this, emSub7 );
    mTimerF_Sub[emSub8]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub8" , this, emSub8 );
    mTimerF_Sub[emSub9]  = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub9" , this, emSub9 );
    mTimerF_Sub[emSub10] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub10", this, emSub10);
    mTimerF_Sub[emSub11] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub11", this, emSub11);
    mTimerF_Sub[emSub12] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub12", this, emSub12);
    mTimerF_Sub[emSub13] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub13", this, emSub13);
    mTimerF_Sub[emSub14] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub14", this, emSub14);
    mTimerF_Sub[emSub15] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub15", this, emSub15);
  	mTimerF_Sub[emSub16] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub16", this, emSub16);
  	mTimerF_Sub[emSub17] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub17", this, emSub17);
  	mTimerF_Sub[emSub18] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub18", this, emSub18);
  	mTimerF_Sub[emSub19] = new ATU5010_TIMERF_Sub("ATU5010_TIMERF_Sub19", this, emSub19);

    for (unsigned int i = 0; i < emSubNum; i++) {
        sc_assert(mTimerF_Sub[i] != NULL);
    }

    // Port connection

    for (unsigned int i = 0; i < emSubNum; i++) {
        ((mTimerF_Sub[i]->tifa))(*(TIFA[i]));
        ((mTimerF_Sub[i]->ovffi))(*(OVFFI[i]));
        ((mTimerF_Sub[i]->icffi))(*(ICFFI[i]));
        for (unsigned int j = 0; j < 8; j++) {
            (*(mTimerF_Sub[i]->tia[j]))(*(TIA[j]));
        }
        if (i < 3) {
            ((mTimerF_Sub[i]->tifb))(*(TIFB[i]));
        } else {
            ((mTimerF_Sub[i]->tifb))(*(dummy_TIFB_sig[i-3]));
        }
        if (i < 3) {
            (*(mTimerF_Sub[i]->poe_sub[0]))(*(POE[i]));
            (*(mTimerF_Sub[i]->poe_sub[1]))(*(POE[i+3]));
        } else {
            (*(mTimerF_Sub[i]->poe_sub[0]))(*(dummy_POE_sig[2*i-6]));
            (*(mTimerF_Sub[i]->poe_sub[1]))(*(dummy_POE_sig[2*i-5]));
        }
    }

    //Initialize variables
    for (unsigned int i = 0; i < emClkNum; i++) {
        mCLKPeriod[i]   = 0;
        mTimeCLKChangePoint[i] = 0;
        mCLKFreq[i]     = 0;
        mCLKOrgFreq[i]  = 0;
        mCLKFreqUnit[i] = "Hz";
    }
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mIsTimerFEnabled = false;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }
    for(unsigned int i=0; i<emSubNum; i++) {
        mSubblockEnableEvent[i].cancel();
    }

    SC_METHOD(HandleCLKBUS_0SignalMethod);
    dont_initialize();
    sensitive << (*CLKBUS[0]);

    SC_METHOD(HandleCLKBUS_1SignalMethod);
    dont_initialize();
    sensitive << (*CLKBUS[1]);

    SC_METHOD(HandleCLKBUS_2SignalMethod);
    dont_initialize();
    sensitive << (*CLKBUS[2]);

    SC_METHOD(HandleCLKBUS_3SignalMethod);
    dont_initialize();
    sensitive << (*CLKBUS[3]);

    SC_METHOD(HandleCLKBUS_4SignalMethod);
    dont_initialize();
    sensitive << (*CLKBUS[4]);

    SC_METHOD(HandleCLKBUS_5SignalMethod);
    dont_initialize();
    sensitive << (*CLKBUS[5]);

    SC_METHOD(Handlenc_clkSignalMethod);
    dont_initialize();
    sensitive << nc_clk;



}//}}}

///Destructor of ATU5010_TIMERF class
ATU5010_TIMERF::~ATU5010_TIMERF (void)
{//{{{
     delete mATU5010_TIMERF_Func;
    for (unsigned int i = 0; i < emSubNum; i++) {
        delete mTimerF_Sub[i];
    }
}//}}}

///Set clock value and clock unit
void ATU5010_TIMERF::SetCLKPeriod (const std::string clock_name, const sc_dt::uint64 period)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if(word_vector[0] == "CLKBUS_0") {
            mCLKPeriod[CLKBUS_0] = period;
            mTimeCLKChangePoint[CLKBUS_0]=(sc_dt::uint64)mATU5010->mClkBusStartTime[0];
            if (mCLKPeriod[CLKBUS_0] > 0) {
                mClkUpdateEvent[CLKBUS_0].notify(SC_ZERO_TIME);
            }
            else {
                mCLKPeriod[CLKBUS_0] = 0;
                // Notify clock is zero
                mClkZeroEvent[CLKBUS_0].notify(SC_ZERO_TIME);
            }
        }
        else if(word_vector[0] == "CLKBUS_1") {
            mCLKPeriod[CLKBUS_1] = period;
            mTimeCLKChangePoint[CLKBUS_1]=(sc_dt::uint64)mATU5010->mClkBusStartTime[1];
            if (mCLKPeriod[CLKBUS_1] > 0) {
                mClkUpdateEvent[CLKBUS_1].notify(SC_ZERO_TIME);
            }
            else {
                mCLKPeriod[CLKBUS_1] = 0;
                // Notify clock is zero
                mClkZeroEvent[CLKBUS_1].notify(SC_ZERO_TIME);
            }
        }
        else if(word_vector[0] == "CLKBUS_2") {
            mCLKPeriod[CLKBUS_2] = period;
            mTimeCLKChangePoint[CLKBUS_2]=(sc_dt::uint64)mATU5010->mClkBusStartTime[2];
            if (mCLKPeriod[CLKBUS_2] > 0) {
                mClkUpdateEvent[CLKBUS_2].notify(SC_ZERO_TIME);
            }
            else {
                mCLKPeriod[CLKBUS_2] = 0;
                // Notify clock is zero
                mClkZeroEvent[CLKBUS_2].notify(SC_ZERO_TIME);
            }
        }
        else if(word_vector[0] == "CLKBUS_3") {
            mCLKPeriod[CLKBUS_3] = period;
            mTimeCLKChangePoint[CLKBUS_3]=(sc_dt::uint64)mATU5010->mClkBusStartTime[3];
            if (mCLKPeriod[CLKBUS_3] > 0) {
                mClkUpdateEvent[CLKBUS_3].notify(SC_ZERO_TIME);
            }
            else {
                mCLKPeriod[CLKBUS_3] = 0;
                // Notify clock is zero
                mClkZeroEvent[CLKBUS_3].notify(SC_ZERO_TIME);
            }
        }
        else if(word_vector[0] == "CLKBUS_4") {
            mCLKPeriod[CLKBUS_4] = period;
            mTimeCLKChangePoint[CLKBUS_4]=(sc_dt::uint64)mATU5010->mClkBusStartTime[4];
            if (mCLKPeriod[CLKBUS_4] > 0) {
                mClkUpdateEvent[CLKBUS_4].notify(SC_ZERO_TIME);
            }
            else {
                mCLKPeriod[CLKBUS_4] = 0;
                // Notify clock is zero
                mClkZeroEvent[CLKBUS_4].notify(SC_ZERO_TIME);
            }
        }
        else if(word_vector[0] == "CLKBUS_5") {
            mCLKPeriod[CLKBUS_5] = period;
            mTimeCLKChangePoint[CLKBUS_5]=(sc_dt::uint64)mATU5010->mClkBusStartTime[5];
            if (mCLKPeriod[CLKBUS_5] > 0) {
                mClkUpdateEvent[CLKBUS_5].notify(SC_ZERO_TIME);
            }
            else {
                mCLKPeriod[CLKBUS_5] = 0;
                // Notify clock is zero
                mClkZeroEvent[CLKBUS_5].notify(SC_ZERO_TIME);
            }
        }
        else if(word_vector[0] == "CLK_NOISE") {
            mCLKPeriod[CLK_NOISE] = period;
            if (mCLKPeriod[CLK_NOISE] > 0) {
                mClkUpdateEvent[CLK_NOISE].notify(SC_ZERO_TIME);
            }
            else {
                mCLKPeriod[CLK_NOISE] = 0;
                // Notify clock is zero
                mClkZeroEvent[CLK_NOISE].notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

///Separate some words from a string to store a vector
void ATU5010_TIMERF::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

///Get time resolution
void ATU5010_TIMERF::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
       resolution_value = 1000000000000000LL;
       resolution_unit = SC_FS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        resolution_value = 1000000000000LL;
        resolution_unit = SC_PS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
       resolution_value = 1000000000;
       resolution_unit = SC_NS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
       resolution_value = 1000000;
       resolution_unit = SC_US;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
       resolution_value = 1000;
       resolution_unit = SC_MS;
    } else {
       resolution_value = 1;
       resolution_unit = SC_SEC;
    }
}//}}}

///Calculate synchronous time
double ATU5010_TIMERF::CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point,const double time_change_point)
{//{{{

    double clock_period;
    if(clock_name == "CLK_LSB") {
        clock_period = (double)mATU5010->mCLK_LSBPeriod;
    }
    else if(clock_name == "CLKBUS_0") {
        clock_period = (double)mCLKPeriod[CLKBUS_0];
    }
    else if(clock_name == "CLKBUS_1") {
        clock_period = (double)mCLKPeriod[CLKBUS_1];
    }
    else if(clock_name == "CLKBUS_2") {
        clock_period = (double)mCLKPeriod[CLKBUS_2];
    }
    else if(clock_name == "CLKBUS_3") {
        clock_period = (double)mCLKPeriod[CLKBUS_3];
    }
    else if(clock_name == "CLKBUS_4") {
        clock_period = (double)mCLKPeriod[CLKBUS_4];
    }
    else if(clock_name == "CLKBUS_5") {
        clock_period = (double)mCLKPeriod[CLKBUS_5];
    }
    double period_num = 0;
    if(clock_period!=0) {
        period_num = (time_point - time_change_point) / clock_period;
    }
    else {
        period_num = 0;
    }
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (is_pos) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        if (((double)mod_period_num + 0.5) < period_num) {
            mod_period_num++;
        }
    }
    double edge_point = 0, mod_period_num_db = 0;
    if (is_pos) {
        mod_period_num_db = (double)mod_period_num;
    } else {
        mod_period_num_db = (double)mod_period_num + 0.5;
    }
    if(clock_period!=0) {
        edge_point = mod_period_num_db * clock_period - (time_point - time_change_point);
    }
    else {
        edge_point = 0;
    }
    return edge_point;
}//}}}

///Check clock period value
bool ATU5010_TIMERF::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    if(clock_name == "CLKBUS_0") {
        clock_period = mCLKPeriod[CLKBUS_0];
    }
    else if(clock_name == "CLKBUS_1") {
        clock_period = mCLKPeriod[CLKBUS_1];
    }
    else if(clock_name == "CLKBUS_2") {
        clock_period = mCLKPeriod[CLKBUS_2];
    }
    else if(clock_name == "CLKBUS_3") {
        clock_period = mCLKPeriod[CLKBUS_3];
    }
    else if(clock_name == "CLKBUS_4") {
        clock_period = mCLKPeriod[CLKBUS_4];
    }
    else if(clock_name == "CLKBUS_5") {
        clock_period = mCLKPeriod[CLKBUS_5];
    }
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Handle nc_clk signal
void ATU5010_TIMERF::Handlenc_clkSignalMethod (void)
{//{{{
    sc_dt::uint64 period = nc_clk.read();
    SetCLKPeriod("CLK_NOISE", period);
    for (unsigned int i = 0; i < emSubNum; i++) {
        mTimerF_Sub[i]->SetCLKNoisePeriod();
    }
}//}}}

///Handle CLKBUS_0 signal
void ATU5010_TIMERF::HandleCLKBUS_0SignalMethod (void)
{//{{{
    sc_dt::uint64 period = (*CLKBUS[0]).read();
    SetCLKPeriod("CLKBUS_0", period);
}//}}}

///Handle CLKBUS_1 signal
void ATU5010_TIMERF::HandleCLKBUS_1SignalMethod (void)
{//{{{
    sc_dt::uint64 period = (*CLKBUS[1]).read();
    SetCLKPeriod("CLKBUS_1", period);
}//}}}

///Handle CLKBUS_2 signal
void ATU5010_TIMERF::HandleCLKBUS_2SignalMethod (void)
{//{{{
    sc_dt::uint64 period = (*CLKBUS[2]).read();
    SetCLKPeriod("CLKBUS_2", period);
}//}}}

///Handle CLKBUS_3 signal
void ATU5010_TIMERF::HandleCLKBUS_3SignalMethod (void)
{//{{{
    sc_dt::uint64 period = (*CLKBUS[3]).read();
    SetCLKPeriod("CLKBUS_3", period);
}//}}}

///Handle CLKBUS_4 signal
void ATU5010_TIMERF::HandleCLKBUS_4SignalMethod (void)
{//{{{
    sc_dt::uint64 period = (*CLKBUS[4]).read();
    SetCLKPeriod("CLKBUS_4", period);
}//}}}

///Handle CLKBUS_5 signal
void ATU5010_TIMERF::HandleCLKBUS_5SignalMethod (void)
{//{{{
    sc_dt::uint64 period = (*CLKBUS[5]).read();
    SetCLKPeriod("CLKBUS_5", period);
}//}}}

/// Cancel operation events
void ATU5010_TIMERF::CancelEvents (void)
{//{{{
    for (unsigned int i = 0; i < emSubNum; i++) {
        mTimerF_Sub[i]->CancelEvents();
    }
}//}}}

/// Initialize internal variables
void ATU5010_TIMERF::Initialize (void)
{//{{{
    for (unsigned int i = 0; i < emSubNum; i++) {
        mTimerF_Sub[i]->Initialize();
    }
}//}}}

void ATU5010_TIMERF::NotifyEvent(const unsigned int event, bool enable) {
    switch(event) {
        case emStartSubblock0:
            if(enable) {
                mSubblockEnableEvent[0].notify(SC_ZERO_TIME);
            }
            else {
                mSubblockDisabledEvent[0].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock1:
            if(enable) {
                mSubblockEnableEvent[1].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[1].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock2:
            if(enable) {
                mSubblockEnableEvent[2].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[2].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock3:
            if(enable) {
                mSubblockEnableEvent[3].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[3].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock4:
            if(enable) {
                mSubblockEnableEvent[4].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[4].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock5:
            if(enable) {
                mSubblockEnableEvent[5].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[5].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock6:
            if(enable) {
                mSubblockEnableEvent[6].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[6].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock7:
            if(enable) {
                mSubblockEnableEvent[7].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[7].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock8:
            if(enable) {
                mSubblockEnableEvent[8].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[8].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock9:
            if(enable) {
                mSubblockEnableEvent[9].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[9].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock10:
            if(enable) {
                mSubblockEnableEvent[10].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[10].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock11:
            if(enable) {
                mSubblockEnableEvent[11].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[11].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock12:
            if(enable) {
                mSubblockEnableEvent[12].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[12].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock13:
            if(enable) {
                mSubblockEnableEvent[13].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[13].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock14:
            if(enable) {
                mSubblockEnableEvent[14].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[14].notify(SC_ZERO_TIME);
            }
            break;
        case emStartSubblock15:
            if(enable) {
                mSubblockEnableEvent[15].notify(SC_ZERO_TIME);
            }
             else {
                mSubblockDisabledEvent[15].notify(SC_ZERO_TIME);
            }
            break;
		    case emStartSubblock16:
      			if (enable) {
      				mSubblockEnableEvent[16].notify(SC_ZERO_TIME);
      			}
      			else {
      				mSubblockDisabledEvent[16].notify(SC_ZERO_TIME);
      			}
      			break;
		    case emStartSubblock17:
      			if (enable) {
      				mSubblockEnableEvent[17].notify(SC_ZERO_TIME);
      			}
      			else {
      				mSubblockDisabledEvent[17].notify(SC_ZERO_TIME);
      			}
      			break;
  		  case emStartSubblock18:
      			if (enable) {
      				mSubblockEnableEvent[18].notify(SC_ZERO_TIME);
      			}
      			else {
      				mSubblockDisabledEvent[18].notify(SC_ZERO_TIME);
      			}
      			break;
  		  case emStartSubblock19:
      			if (enable) {
      				mSubblockEnableEvent[19].notify(SC_ZERO_TIME);
      			}
      			else {
      				mSubblockDisabledEvent[19].notify(SC_ZERO_TIME);
      			}
      			break;
        default:
            break;
    }
}

void ATU5010_TIMERF::EnableTimerF(const bool is_start) {
    if(is_start) {
        mTimerFEnableEvent.notify(SC_ZERO_TIME);
        mIsTimerFEnabled = true;
    }
    else {
        mTimerFDisableEvent.notify(SC_ZERO_TIME);
        mIsTimerFEnabled = false;
    }
}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5010_TIMERF::EnableReset (const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
        CancelEvents();
    }
    for (unsigned int sub = 0; sub < emSubNum; sub++) {
        mTimerF_Sub[sub]->EnableReset(is_active);
    }
    mATU5010_TIMERF_Func->Reset(is_active);
}//}}}

// vim600: set foldmethod=marker :
