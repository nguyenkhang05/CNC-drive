// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERC.cpp,v 1.1 2017/05/26 10:15:45 binhnguyen Exp $
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
#include "ATU5010_TIMERC.h"
#include "ATU5010_TIMERC_Func.h"
#include "ATU5010_TIMERC_SUB.h"
#include "ATU5010_TIMERC_SUB_Func.h"
#include "ATU5010.h"
#include "ATU5010_Func.h"

///*********************************
/// Function     : ATU5010_TIMERC
/// Description  : Constructor of ATU5010_TIMERC class
///*********************************
ATU5010_TIMERC::ATU5010_TIMERC (sc_module_name name, ATU5010 *ATU5010_Ptr):
                sc_module(name),
                ATU5010_TIMERC_AgentController(),
                PCLK("PCLK"),
                nc_clk("nc_clk"),
                i_event_1_0("i_event_1_0"),
                i_event_2A("i_event_2A"),
                i_event_2B("i_event_2B")
{//{{{
    assert(ATU5010_Ptr != NULL);
    mATU5010 = ATU5010_Ptr;
    mModuleName = (std::string)name;
    //Bind clock, reset signal and target socket to Bus IF
    //Initialize variables - for clock, time resolution, reset
    mPCLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz";
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    Initialize();
    mTimerC_Func = new ATU5010_TIMERC_Func((std::string)name, this);
    sc_assert(mTimerC_Func != NULL);

    std::ostringstream port;
    for (unsigned int sub = 0; sub < emSub; sub++) {
        port.str(""); port << "TIOC0" << sub;
        TIOC0[sub] = new sc_inout<bool>(port.str().c_str());
        sc_assert (TIOC0[sub] != NULL);
        port.str(""); port << "TIOC1" << sub;
        TIOC1[sub] = new sc_inout<bool>(port.str().c_str());
        sc_assert (TIOC1[sub] != NULL);
        port.str(""); port << "TIOC2" << sub;
        TIOC2[sub] = new sc_inout<bool>(port.str().c_str());
        sc_assert (TIOC2[sub] != NULL);
        port.str(""); port << "TIOC3" << sub;
        TIOC3[sub] = new sc_inout<bool>(port.str().c_str());
        sc_assert (TIOC3[sub] != NULL);
        port.str(""); port << "INT_OVI" << sub;
        INT_OVI[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OVI[sub] != NULL);
        port.str(""); port << "INT_GRC0" << sub;
        INT_GRC0[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_GRC0[sub] != NULL);
        port.str(""); port << "INT_GRC1" << sub;
        INT_GRC1[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_GRC1[sub] != NULL);
        port.str(""); port << "INT_GRC2" << sub;
        INT_GRC2[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_GRC2[sub] != NULL);
        port.str(""); port << "INT_GRC3" << sub;
        INT_GRC3[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_GRC3[sub] != NULL);
        port.str(""); port << "INT_OCRC0" << sub;
        INT_OCRC0[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OCRC0[sub] != NULL);
        port.str(""); port << "INT_OCRC1" << sub;
        INT_OCRC1[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OCRC1[sub] != NULL);
        port.str(""); port << "INT_OCRC2" << sub;
        INT_OCRC2[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OCRC2[sub] != NULL);
        port.str(""); port << "INT_OCRC3" << sub;
        INT_OCRC3[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OCRC3[sub] != NULL);
        //add for updating
        port.str("");port << "TOCE0" << sub;
        TOCE0[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOCE0[sub] != NULL);
        port.str("");port << "TOCE1" << sub;
        TOCE1[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOCE1[sub] != NULL);
        port.str("");port << "TOCE2" << sub;
        TOCE2[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOCE2[sub] != NULL);
        port.str("");port << "TOCE3" << sub;
        TOCE3[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOCE3[sub] != NULL);
 
        port.str("");port << "TOC0" << sub;
        TOC0[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOC0[sub] != NULL);
        port.str("");port << "TOC1" << sub;
        TOC1[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOC1[sub] != NULL);
        port.str("");port << "TOC2" << sub;
        TOC2[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOC2[sub] != NULL);
        port.str("");port << "TOC3" << sub;
        TOC3[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOC3[sub] != NULL);
   }
    for (unsigned int num = 0; num < 6; num++) {
        port.str(""); port << "clock_bus" << num;
        clock_bus[num] = new sc_in<sc_dt::uint64> (port.str().c_str());
        sc_assert (clock_bus[num] != NULL);
    }

    mTIMERC_SUB[emSub0] = new ATU5010_TIMERC_SUB("Sub0", this, emSub0);
    mTIMERC_SUB[emSub1] = new ATU5010_TIMERC_SUB("Sub1", this, emSub1);
    mTIMERC_SUB[emSub2] = new ATU5010_TIMERC_SUB("Sub2", this, emSub2);
    mTIMERC_SUB[emSub3] = new ATU5010_TIMERC_SUB("Sub3", this, emSub3);
    mTIMERC_SUB[emSub4] = new ATU5010_TIMERC_SUB("Sub4", this, emSub4);
    mTIMERC_SUB[emSub5] = new ATU5010_TIMERC_SUB("Sub5", this, emSub5);
    mTIMERC_SUB[emSub6] = new ATU5010_TIMERC_SUB("Sub6", this, emSub6);
    mTIMERC_SUB[emSub7] = new ATU5010_TIMERC_SUB("Sub7", this, emSub7);
    mTIMERC_SUB[emSub8] = new ATU5010_TIMERC_SUB("Sub8", this, emSub8);
    mTIMERC_SUB[emSub9] = new ATU5010_TIMERC_SUB("Sub9", this, emSub9);
    mTIMERC_SUB[emSub10] = new ATU5010_TIMERC_SUB("Sub10", this, emSub10);
    mTIMERC_SUB[emSub11] = new ATU5010_TIMERC_SUB("Sub11", this, emSub11);
    mTIMERC_SUB[emSub12] = new ATU5010_TIMERC_SUB("Sub12", this, emSub12);
    mTIMERC_SUB[emSub13] = new ATU5010_TIMERC_SUB("Sub13", this, emSub13);
    for (unsigned int i = 0; i < emSub; i++) {
        sc_assert (mTIMERC_SUB[i] != NULL);
    }

    for (unsigned int i = 0; i < emSub; i++) {
        (mTIMERC_SUB[i]->PCLK)(PCLK);
        (mTIMERC_SUB[i]->nc_clk)(nc_clk);
        (mTIMERC_SUB[i]->i_event_1_0)(i_event_1_0);
        (mTIMERC_SUB[i]->i_event_2A)(i_event_2A);
        (mTIMERC_SUB[i]->i_event_2B)(i_event_2B);
        ((mTIMERC_SUB[i]->clock_bus0))(*(clock_bus[0]));
        ((mTIMERC_SUB[i]->clock_bus1))(*(clock_bus[1]));
        ((mTIMERC_SUB[i]->clock_bus2))(*(clock_bus[2]));
        ((mTIMERC_SUB[i]->clock_bus3))(*(clock_bus[3]));
        ((mTIMERC_SUB[i]->clock_bus4))(*(clock_bus[4]));
        ((mTIMERC_SUB[i]->clock_bus5))(*(clock_bus[5]));
        (mTIMERC_SUB[i]->TIOC3)(*(TIOC3[i]));
        (mTIMERC_SUB[i]->TIOC2)(*(TIOC2[i]));
        (mTIMERC_SUB[i]->TIOC1)(*(TIOC1[i]));
        (mTIMERC_SUB[i]->TIOC0)(*(TIOC0[i]));
        (mTIMERC_SUB[i]->INT_OVI)(*(INT_OVI[i]));
        (*(mTIMERC_SUB[i]->INT_GRC[0]))(*(INT_GRC0[i]));
        (*(mTIMERC_SUB[i]->INT_GRC[1]))(*(INT_GRC1[i]));
        (*(mTIMERC_SUB[i]->INT_GRC[2]))(*(INT_GRC2[i]));
        (*(mTIMERC_SUB[i]->INT_GRC[3]))(*(INT_GRC3[i]));
        (*(mTIMERC_SUB[i]->INT_OCRC[0]))(*(INT_OCRC0[i]));
        (*(mTIMERC_SUB[i]->INT_OCRC[1]))(*(INT_OCRC1[i]));
        (*(mTIMERC_SUB[i]->INT_OCRC[2]))(*(INT_OCRC2[i]));
        (*(mTIMERC_SUB[i]->INT_OCRC[3]))(*(INT_OCRC3[i]));
        //add for updating
        (mTIMERC_SUB[i]->TOCE3)(*(TOCE3[i]));
        (mTIMERC_SUB[i]->TOCE2)(*(TOCE2[i]));
        (mTIMERC_SUB[i]->TOCE1)(*(TOCE1[i]));
        (mTIMERC_SUB[i]->TOCE0)(*(TOCE0[i]));

        (mTIMERC_SUB[i]->TOC3)(*(TOC3[i]));
        (mTIMERC_SUB[i]->TOC2)(*(TOC2[i]));
        (mTIMERC_SUB[i]->TOC1)(*(TOC1[i]));
        (mTIMERC_SUB[i]->TOC0)(*(TOC0[i]));

    }
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;
 }//}}} 
///*********************************
/// Function     : ~ATU5010_TIMERC
// Description  : Destructor of ATU5010_TIMERC class
///*********************************
ATU5010_TIMERC::~ATU5010_TIMERC (void)
{//{{{ 
    delete mTimerC_Func;
    for (unsigned int i = 0; i < emSub; i++) {
        delete mTIMERC_SUB[i];
    }
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Check the status of timer C from Wrapper
///*********************************
void ATU5010_TIMERC::Initialize()
{//{{{
    mStatusC = false;
}//}}}

///*********************************
/// Function     : EnableTIC
/// Description  : Wrapper enable timerC
///*********************************
void ATU5010_TIMERC::EnableTIC(const bool is_start)
{//{{{
    bool status =  is_start;
    if (status != mStatusC) {
        mStatusC = is_start;
        for (unsigned int sub = 0; sub < emSub; sub++) {
            if (status == true) {
                status = (bool) (mTimerC_Func->CheckSubblock(sub));
            }
            mTIMERC_SUB[sub]->RunningCounter(status);
        }
    }
}//}}}

///*********************************
/// Function     : StartStopSubblock
/// Description  : StartSubblock
///*********************************
void ATU5010_TIMERC::StartStopSubblock(const unsigned int sub, const bool is_start)
{//{{{
    if ((!IsModelResetActive()) && mStatusC) {
        mTIMERC_SUB[sub]->RunningCounter(is_start);
    } 
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5010_TIMERC::EnableReset (const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    for (unsigned int sub = 0; sub < emSub; sub++) {
        mTIMERC_SUB[sub]->EnableReset(is_active);
    }
    mTimerC_Func->EnableReset(is_active);
}//}}}

///*********************************
/// Function     : IsModelResetActive
/// Description  : Check the status reset of model
///*********************************
bool ATU5010_TIMERC::IsModelResetActive(void)
{//{{{
    bool ret = false;
    if ((mATU5010->mIsResetHardActive[1]) || (mATU5010->mIsResetCmdReceive[1])) {
        ret =true;
    }
    return ret;
}//}}}

///*********************************
/// Function     : HandlePCLKSignalMethod
/// Description  : Handle PCLK signal
///*********************************
void ATU5010_TIMERC::HandlePCLKSignalMethod (void)
{//{{{
    mPCLKPeriod = PCLK.read();
    if (mPCLKPeriod == 0) {
        re_printf("warning","PCLK is 0");
    } 
}//}}}

///*********************************
/// Function     : GetRatioValue
/// Description  : Get the value of Noise cancel for each sub
///*********************************
unsigned int ATU5010_TIMERC::GetRatioValue(const unsigned int value)
{//{{{
    return (unsigned int)(mATU5010->mATU5010_Func->GetPrescalerRatio(value));
}//}}}

///*********************************
/// Function     : SetNoiseCancelMode
/// Description  : Set the noise cancel mode
///*********************************
void ATU5010_TIMERC::SetNoiseCancelMode(const unsigned int sub_id)
{//{{{
    unsigned int mode = mATU5010->GetNoiseCancelMode("Timer C");
        if (mode == 0) {
            mode = mTimerC_Func->GetNoiseCancelMode(sub_id);
        }
    mTIMERC_SUB[sub_id]->SetNoiseCancelMode(mode);
}//}}}

///*********************************
/// Function     : SetNoiseEachCancelMode
/// Description  : Set the noise cancel mode // add 0820
///*********************************
void ATU5010_TIMERC::SetNoiseEachCancelMode(const unsigned int sub_id, const unsigned int channel)
{//{{{
    unsigned int mode = mATU5010->GetNoiseCancelMode("Timer C");
    if (mode == 0) {
            mode = mTimerC_Func->GetNoiseCancelMode(sub_id);
        }
    mTIMERC_SUB[sub_id]->SetNoiseEachCancelMode(channel,mode);
}//}}}

// vim600: set foldmethod=marker :
