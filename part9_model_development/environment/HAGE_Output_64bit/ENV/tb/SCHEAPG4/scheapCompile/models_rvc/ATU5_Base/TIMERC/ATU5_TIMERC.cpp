// ---------------------------------------------------------------------
// $Id: ATU5_TIMERC.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "ATU5_TIMERC.h"
#include "ATU5_TIMERC_Func.h"
#include "ATU5_TIMERC_SUB.h"
#include "ATU5_TIMERC_SUB_Func.h"
#include "ATU5_Base.h"


///*********************************
/// Function     : ATU5_TIMERC
/// Description  : Constructor of ATU5_TIMERC class
///*********************************
ATU5_TIMERC::ATU5_TIMERC (sc_module_name name, ATU5_Base *ATU5_Ptr, unsigned int subblock):
                sc_module(name),
                ATU5_TIMERC_AgentController(),
                PCLK("PCLK"),
                nc_clk("nc_clk"),
                i_event_1_0("i_event_1_0"),
                i_event_2A("i_event_2A"),
                i_event_2B("i_event_2B")
{//{{{
    assert(ATU5_Ptr != NULL) ;
    mATU5 = ATU5_Ptr;
    mModuleName = (std::string)name;
    //Bind clock, reset signal and target socket to Bus IF
    //Initialize variables - for clock, time resolution, reset
    mPCLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz";
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    msbC = subblock;
    Initialize();
    mTimerC_Func = new ATU5_TIMERC_Func((std::string)name, this , msbC);
    sc_assert(mTimerC_Func != NULL);

    std::ostringstream port;

    TIOC0 = new sc_inout<bool>*[msbC];
    sc_assert(TIOC0 != NULL);
    TIOC1 = new sc_inout<bool>*[msbC];
    sc_assert(TIOC1 != NULL);
    TIOC2 = new sc_inout<bool>*[msbC];
    sc_assert(TIOC2 != NULL);
    TIOC3 = new sc_inout<bool>*[msbC];
    sc_assert(TIOC3 != NULL);
    INT_OVI = new sc_out<bool>*[msbC];
    sc_assert(INT_OVI != NULL);
    INT_GRC0 = new sc_out<bool>*[msbC];
    sc_assert(INT_GRC0 != NULL);
    INT_GRC1 = new sc_out<bool>*[msbC];
    sc_assert(INT_GRC1 != NULL);
    INT_GRC2 = new sc_out<bool>*[msbC];
    sc_assert(INT_GRC2 != NULL);
    INT_GRC3 = new sc_out<bool>*[msbC];
    sc_assert(INT_GRC3 != NULL);
    INT_OCRC0 = new sc_out<bool>*[msbC];
    sc_assert(INT_OCRC0 != NULL);
    INT_OCRC1 = new sc_out<bool>*[msbC];
    sc_assert(INT_OCRC1 != NULL);
    INT_OCRC2 = new sc_out<bool>*[msbC];
    sc_assert(INT_OCRC2 != NULL);
    INT_OCRC3 = new sc_out<bool>*[msbC];
    sc_assert(INT_OCRC3 != NULL);
    TOCE0 = new sc_out<bool>*[msbC];
    sc_assert(TOCE0 != NULL);
    TOCE1 = new sc_out<bool>*[msbC];
    sc_assert(TOCE1 != NULL);
    TOCE2 = new sc_out<bool>*[msbC];
    sc_assert(TOCE2 != NULL);
    TOCE3 = new sc_out<bool>*[msbC];
    sc_assert(TOCE3 != NULL);
    TOC0 = new sc_out<bool>*[msbC];
    sc_assert(TOC0 != NULL);
    TOC1 = new sc_out<bool>*[msbC];
    sc_assert(TOC1 != NULL);
    TOC2 = new sc_out<bool>*[msbC];
    sc_assert(TOC2 != NULL);
    TOC3 = new sc_out<bool>*[msbC];
    sc_assert(TOC3 != NULL);
    for (unsigned int sub = 0; sub < msbC; sub++) {
        port.str(""); port << "TIOC0" << sub;
        TIOC0[sub] = new sc_inout<bool>(port.str().c_str());
        sc_assert (TIOC0[sub] != NULL);
        TIOC0[sub]->initialize(false);
        
        port.str(""); port << "TIOC1" << sub;
        TIOC1[sub] = new sc_inout<bool>(port.str().c_str());
        sc_assert (TIOC1[sub] != NULL);
        TIOC1[sub]->initialize(false);
        
        port.str(""); port << "TIOC2" << sub;
        TIOC2[sub] = new sc_inout<bool>(port.str().c_str());
        sc_assert (TIOC2[sub] != NULL);
        TIOC2[sub]->initialize(false);
        
        port.str(""); port << "TIOC3" << sub;
        TIOC3[sub] = new sc_inout<bool>(port.str().c_str());
        sc_assert (TIOC3[sub] != NULL);
        TIOC3[sub]->initialize(false);
        
        port.str(""); port << "INT_OVI" << sub;
        INT_OVI[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OVI[sub] != NULL);
        INT_OVI[sub]->initialize(false);
        
        port.str(""); port << "INT_GRC0" << sub;
        INT_GRC0[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_GRC0[sub] != NULL);
        INT_GRC0[sub]->initialize(false);
        
        port.str(""); port << "INT_GRC1" << sub;
        INT_GRC1[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_GRC1[sub] != NULL);
        INT_GRC1[sub]->initialize(false);
        
        port.str(""); port << "INT_GRC2" << sub;
        INT_GRC2[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_GRC2[sub] != NULL);
        INT_GRC2[sub]->initialize(false);
        
        port.str(""); port << "INT_GRC3" << sub;
        INT_GRC3[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_GRC3[sub] != NULL);
        INT_GRC3[sub]->initialize(false);
        
        port.str(""); port << "INT_OCRC0" << sub;
        INT_OCRC0[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OCRC0[sub] != NULL);
        INT_OCRC0[sub]->initialize(false);
        
        port.str(""); port << "INT_OCRC1" << sub;
        INT_OCRC1[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OCRC1[sub] != NULL);
        INT_OCRC1[sub]->initialize(false);
        
        port.str(""); port << "INT_OCRC2" << sub;
        INT_OCRC2[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OCRC2[sub] != NULL);
        INT_OCRC2[sub]->initialize(false);
        
        port.str(""); port << "INT_OCRC3" << sub;
        INT_OCRC3[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (INT_OCRC3[sub] != NULL);
        INT_OCRC3[sub]->initialize(false);
        
        //add for updating
        port.str("");port << "TOCE0" << sub;
        TOCE0[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOCE0[sub] != NULL);
        TOCE0[sub]->initialize(false);
        
        port.str("");port << "TOCE1" << sub;
        TOCE1[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOCE1[sub] != NULL);
        TOCE1[sub]->initialize(false);
        
        port.str("");port << "TOCE2" << sub;
        TOCE2[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOCE2[sub] != NULL);
        TOCE2[sub]->initialize(false);
        
        port.str("");port << "TOCE3" << sub;
        TOCE3[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOCE3[sub] != NULL);
        TOCE3[sub]->initialize(false);
 
        port.str("");port << "TOC0" << sub;
        TOC0[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOC0[sub] != NULL);
        TOC0[sub]->initialize(false);
        
        port.str("");port << "TOC1" << sub;
        TOC1[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOC1[sub] != NULL);
        TOC1[sub]->initialize(false);
        
        port.str("");port << "TOC2" << sub;
        TOC2[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOC2[sub] != NULL);
        TOC2[sub]->initialize(false);
        
        port.str("");port << "TOC3" << sub;
        TOC3[sub] = new sc_out<bool>(port.str().c_str());
        sc_assert (TOC3[sub] != NULL);
        TOC3[sub]->initialize(false);
        
    }
    for (unsigned int num = 0; num < 6; num++) {
        port.str(""); port << "clock_bus" << num;
        clock_bus[num] = new sc_in<sc_dt::uint64> (port.str().c_str());
        sc_assert (clock_bus[num] != NULL);
    }

    mTIMERC_SUB = new ATU5_TIMERC_SUB*[msbC] ;
    sc_assert (mTIMERC_SUB != NULL);
    for( unsigned int sub = 0; sub < msbC ; sub ++ )
    {
        std::ostringstream inst_Sub_name;
        inst_Sub_name << "Sub" << sub ;
        mTIMERC_SUB[sub] = new ATU5_TIMERC_SUB((const char*) inst_Sub_name.str().c_str(), this, sub);
        sc_assert (mTIMERC_SUB[sub] != NULL);
    }

    for (unsigned int i = 0; i < msbC; i++) {
        sc_assert (mTIMERC_SUB[i] != NULL);
    }

    for (unsigned int i = 0; i < msbC; i++) {
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
/// Function     : ~ATU5_TIMERC
// Description  : Destructor of ATU5_TIMERC class
///*********************************
ATU5_TIMERC::~ATU5_TIMERC (void)
{//{{{ 
    delete mTimerC_Func;
    for (unsigned int i = 0; i < msbC; i++) {
        delete mTIMERC_SUB[i];
    }
    delete [] mTIMERC_SUB;

    for (unsigned int i = 0; i < msbC; i++) {
        delete  TIOC0[i];
    }
    delete [] TIOC0;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TIOC1[i];
    }
    delete [] TIOC1;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TIOC2[i];
    }
    delete [] TIOC2;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TIOC3[i];
    }
    delete [] TIOC3;
    for (unsigned int i = 0; i < msbC; i++) {
        delete INT_OVI[i];
    }
    delete [] INT_OVI;
    for (unsigned int i = 0; i < msbC; i++) {
        delete INT_GRC0[i];
    }
    delete [] INT_GRC0;
    for (unsigned int i = 0; i < msbC; i++) {
        delete INT_GRC1[i];
    }
    delete [] INT_GRC1;
    for (unsigned int i = 0; i < msbC; i++) {
        delete INT_GRC2[i];
    }
    delete [] INT_GRC2;
    for (unsigned int i = 0; i < msbC; i++) {
        delete INT_GRC3[i];
    }
    delete [] INT_GRC3;
    for (unsigned int i = 0; i < msbC; i++) {
        delete INT_OCRC0[i];
    }
    delete [] INT_OCRC0;
    for (unsigned int i = 0; i < msbC; i++) {
        delete INT_OCRC1[i];
    }
    delete [] INT_OCRC1;
    for (unsigned int i = 0; i < msbC; i++) {
        delete INT_OCRC2[i];
    }
    delete [] INT_OCRC2;
    for (unsigned int i = 0; i < msbC; i++) {
        delete INT_OCRC3[i];
    }
    delete [] INT_OCRC3;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TOCE0[i];
    }
    delete [] TOCE0;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TOCE1[i];
    }
    delete [] TOCE1;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TOCE2[i];
    }
    delete [] TOCE2;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TOCE3[i];
    }
    delete [] TOCE3;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TOC0[i];
    }
    delete [] TOC0;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TOC1[i];
    }
    delete [] TOC1;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TOC2[i];
    }
    delete [] TOC2;
    for (unsigned int i = 0; i < msbC; i++) {
        delete TOC3[i];
    }
    delete [] TOC3;
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Check the status of timer C from Wrapper
///*********************************
void ATU5_TIMERC::Initialize()
{//{{{
    mStatusC = false;
}//}}}

///*********************************
/// Function     : EnableTIC
/// Description  : Wrapper enable timerC
///*********************************
void ATU5_TIMERC::EnableTIC(const bool is_start)
{//{{{
    bool status =  is_start;
    bool mStatusTSTRC = false;    // add to fix bug #50806
    if (status != mStatusC) {
        mStatusC = is_start;
        for (unsigned int sub = 0; sub < msbC; sub++) {
            if (status == true) {
                mStatusTSTRC = (bool) (mTimerC_Func->CheckSubblock(sub));
            }
            mTIMERC_SUB[sub]->RunningCounter(mStatusTSTRC);
	    mStatusTSTRC = false;
        }
    }
}//}}}

///*********************************
/// Function     : StartStopSubblock
/// Description  : StartSubblock
///*********************************
void ATU5_TIMERC::StartStopSubblock(const unsigned int sub, const bool is_start)
{//{{{
    if ((!IsModelResetActive()) && mStatusC) {
        mTIMERC_SUB[sub]->RunningCounter(is_start);
    } 
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5_TIMERC::EnableReset (const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    for (unsigned int sub = 0; sub < msbC; sub++) {
        mTIMERC_SUB[sub]->EnableReset(is_active);
    }
    mTimerC_Func->EnableReset(is_active);
}//}}}

///*********************************
/// Function     : IsModelResetActive
/// Description  : Check the status reset of model
///*********************************
bool ATU5_TIMERC::IsModelResetActive(void)
{//{{{
    bool ret = false;
    if ((mATU5->mIsResetHardActive[1]) || (mATU5->mIsResetCmdReceive[1])) {
        ret =true;
    }
    return ret;
}//}}}

///*********************************
/// Function     : HandlePCLKSignalMethod
/// Description  : Handle PCLK signal
///*********************************
void ATU5_TIMERC::HandlePCLKSignalMethod (void)
{//{{{
    mPCLKPeriod = (double) PCLK.read();
    if (mPCLKPeriod == 0) {
        re_printf("warning","PCLK is 0");
    } 
}//}}}

///*********************************
/// Function     : SetNoiseCancelMode
/// Description  : Set the noise cancel mode
///*********************************
void ATU5_TIMERC::SetNoiseCancelMode(const unsigned int sub_id)
{//{{{
    unsigned int mode = mATU5->GetNoiseCancelMode("Timer C");
        if (mode == 0) {
            mode = mTimerC_Func->GetNoiseCancelMode(sub_id);
        }
    mTIMERC_SUB[sub_id]->SetNoiseCancelMode(mode);
}//}}}

///*********************************
/// Function     : SetNoiseEachCancelMode
/// Description  : Set the noise cancel mode // add 0820
///*********************************
void ATU5_TIMERC::SetNoiseEachCancelMode(const unsigned int sub_id, const unsigned int channel)
{//{{{
    unsigned int mode = mATU5->GetNoiseCancelMode("Timer C");
    if (mode == 0) {
            mode = mTimerC_Func->GetNoiseCancelMode(sub_id);
        }
    mTIMERC_SUB[sub_id]->SetNoiseEachCancelMode(channel,mode);
}//}}}

// vim600: set foldmethod=marker :
