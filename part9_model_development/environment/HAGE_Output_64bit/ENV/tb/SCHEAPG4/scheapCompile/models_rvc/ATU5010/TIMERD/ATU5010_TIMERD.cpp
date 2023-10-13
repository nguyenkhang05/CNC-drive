// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERD.cpp,v 1.2 2017/05/26 10:15:45 HuyDT1 Exp $
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
#include "ATU5010_TIMERD.h"
#include "ATU5010_TIMERD_Func.h"
#include "ATU5010_TIMERD_SUB.h"
#include "ATU5010_TIMERD_SUB_Func.h"
#include "ATU5010.h"
///*********************************
/// Function     : ATU5010_TIMERD
/// Description  : Constructor of ATU5010_TIMERD class
///*********************************
ATU5010_TIMERD::ATU5010_TIMERD (sc_module_name name,
            ATU5010 *ATU5010_Ptr):
                sc_module(name),
                ATU5010_TIMERD_AgentController(),
                PCLK("PCLK"),
                clock_bus0("clock_bus0"),
                clock_bus1("clock_bus1"),
                clock_bus2("clock_bus2"),
                clock_bus3("clock_bus3"),
                clock_bus4("clock_bus4"),
                clock_bus5("clock_bus5"),
                CLRB("CLRB"),
                TAEO2A("TAEO2A"),
                TAEO2B("TAEO2B")
{//{{{
    assert(ATU5010_Ptr != NULL);
    mATU5010 = ATU5010_Ptr;
    mPCLKPeriod = 0;
    Initialize();
    //Initialize variables - for clock, time resolution, reset
    //Bind clock, reset signal and target socket to Bus IF
    std::ostringstream port_name;
    for (unsigned int index = 0; index < ATU5010_TIMERD_AgentController::emSub; index++) {
        port_name.str("");
        port_name << "INT_OVF1" << index;
        INT_OVF1[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_OVF1[index] != NULL);

        port_name.str("");
        port_name << "INT_OVF2" << index;
        INT_OVF2[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_OVF2[index] != NULL);
    }
    for (unsigned int index = 0; index < ATU5010_TIMERD_AgentController::emSub; index++) {
        for (unsigned int index2 = 0; index2 < ATU5010_TIMERD_AgentController::emChannel; index2++) {
        port_name.str("");
        port_name << "TODA" << index << index2;
        TODA[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TODA[index][index2] != NULL);

        port_name.str("");
        port_name << "TODB" << index << index2;
        TODB[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TODB[index][index2] != NULL);
        
        port_name.str("");
        port_name << "TODMIG" << index << index2;
        TODMIG[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TODMIG[index][index2] != NULL);

        port_name.str("");
        port_name << "INT_UDF" << index << index2;
        INT_UDF[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_UDF[index][index2] != NULL);

        port_name.str("");
        port_name << "INT_CMPA" << index << index2;
        INT_CMPA[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_CMPA[index][index2] != NULL);

        port_name.str("");
        port_name << "INT_CMPB" << index << index2;
        INT_CMPB[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_CMPB[index][index2] != NULL);

        port_name.str("");
        port_name << "INT_FEIED" << index << index2;
        INT_FEIED[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_FEIED[index][index2] != NULL);

        port_name.str("");
        port_name << "INT_REIED" << index << index2;
        INT_REIED[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_REIED[index][index2] != NULL);
        }
    }
     for (unsigned int index = 0; index < ATU5010_TIMERD_AgentController::emSub; index++) {
        for (unsigned int index2 = 0; index2 < 8; index2++) {
        port_name.str("");
        port_name << "PHUDFE" << index << index2;
        PHUDFE[index][index2] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(PHUDFE[index][index2] != NULL);
        }
    }

    mATU5010_TIMERDFunc = new ATU5010_TIMERD_Func((std::string)name, this);
    sc_assert(mATU5010_TIMERDFunc != NULL);

    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub0] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB0", this, ATU5010_TIMERD_AgentController::emSub0);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub1] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB1", this, ATU5010_TIMERD_AgentController::emSub1);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub2] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB2", this, ATU5010_TIMERD_AgentController::emSub2);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub3] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB3", this, ATU5010_TIMERD_AgentController::emSub3);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub4] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB4", this, ATU5010_TIMERD_AgentController::emSub4);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub5] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB5", this, ATU5010_TIMERD_AgentController::emSub5);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub6] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB6", this, ATU5010_TIMERD_AgentController::emSub6);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub7] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB7", this, ATU5010_TIMERD_AgentController::emSub7);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub8] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB8", this, ATU5010_TIMERD_AgentController::emSub8);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub9] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB9", this, ATU5010_TIMERD_AgentController::emSub9);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub10] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB10", this, ATU5010_TIMERD_AgentController::emSub10);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub11] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB11", this, ATU5010_TIMERD_AgentController::emSub11);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub12] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB12", this, ATU5010_TIMERD_AgentController::emSub12);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub13] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB13", this, ATU5010_TIMERD_AgentController::emSub13);
    mTIMERD_SUB[ATU5010_TIMERD_AgentController::emSub14] = new ATU5010_TIMERD_SUB("ATU5010_TIMERD_SUB14", this, ATU5010_TIMERD_AgentController::emSub14);

    for (unsigned int i = 0; i < ATU5010_TIMERD_AgentController::emSub; i++) {
        sc_assert (mTIMERD_SUB[i] != NULL);
    }
    //port connection
    for (unsigned int index = 0; index < ATU5010_TIMERD_AgentController::emSub; index++) {
        (mTIMERD_SUB[index]->TAEO2A)(TAEO2A);
        (mTIMERD_SUB[index]->TAEO2B)(TAEO2B);
        (mTIMERD_SUB[index]->CLRB)(CLRB);
        (mTIMERD_SUB[index]->INT_OVF1)(*INT_OVF1[index]);
        (mTIMERD_SUB[index]->INT_OVF2)(*INT_OVF2[index]);
        (mTIMERD_SUB[index]->PHU0DFE)(*(PHUDFE[index][0]));
        (mTIMERD_SUB[index]->PHU1DFE)(*(PHUDFE[index][1]));
        for (unsigned int i = 0; i < ATU5010_TIMERD_AgentController::emChannel; i++){
            (*(mTIMERD_SUB[index]->TODA[i]))(*(TODA[index][i]));
            (*(mTIMERD_SUB[index]->TODB[i]))(*(TODB[index][i]));
            (*(mTIMERD_SUB[index]->TODMIG[i]))(*(TODMIG[index][i]));
            (*(mTIMERD_SUB[index]->INT_CMPA[i]))(*(INT_CMPA[index][i]));
            (*(mTIMERD_SUB[index]->INT_CMPB[i]))(*(INT_CMPB[index][i]));
            (*(mTIMERD_SUB[index]->INT_FEIED[i]))(*(INT_FEIED[index][i]));
            (*(mTIMERD_SUB[index]->INT_REIED[i]))(*(INT_REIED[index][i]));
            (*(mTIMERD_SUB[index]->INT_UDF[i]))(*(INT_UDF[index][i]));
        }
    }
    // Declare method
     SC_METHOD(HandleClockInputMethod);
     dont_initialize();
     sensitive << PCLK
               << clock_bus0
               << clock_bus1
               << clock_bus2
               << clock_bus3
               << clock_bus4
               << clock_bus5;

    SC_METHOD(HandleInputCaptureMethod);
    dont_initialize();
    sensitive << (*PHUDFE[0][0])
              << (*PHUDFE[0][1])
              << (*PHUDFE[0][2])
              << (*PHUDFE[0][3])
              << (*PHUDFE[0][4])
              << (*PHUDFE[0][5])
              << (*PHUDFE[0][6])
              << (*PHUDFE[0][7]);
              
    SC_METHOD(InputCaptureWriteMethod);
    dont_initialize();
    sensitive << mInputCaptureEvent;
    
    for (unsigned int index = 0; index < 8; index++) {
        PHUDFE_flag[index] = false;
    }
 }//}}}
///*********************************
/// Function     : ~ATU5010_TIMERD
/// Description  : Destructor of ATU5010_TIMERD class
///*********************************
ATU5010_TIMERD::~ATU5010_TIMERD (void)
{//{{{
    delete mATU5010_TIMERDFunc;
    for (unsigned int i = 0; i < ATU5010_TIMERD_AgentController::emSub; i++) {
        delete mTIMERD_SUB[i];
    }
}//}}}
///*********************************
/// Function     : EnableTID
/// Description  : Wrapper enable timerD
///*********************************
void ATU5010_TIMERD::EnableTID(const bool is_start)
{//{{{
    if (is_start) {
        mATU5010_TIMERDFunc->TimerDStatus(true);
    } else {
        mATU5010_TIMERDFunc->TimerDStatus(false);
    }
}//}}}

void ATU5010_TIMERD::HandleClockInputMethod (void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if (mPCLKPeriod != (sc_dt::uint64)PCLK.read()) {
        mPCLKPeriod = (sc_dt::uint64)PCLK.read();
        mInputClockStartPoint[ATU5010_TIMERD_AgentController::emPCLKClockIndex] = current_time;
    }
    if (mCLKBUS0Period != (sc_dt::uint64)clock_bus0.read()) {
        mCLKBUS0Period = (sc_dt::uint64)clock_bus0.read();
        mInputClockStartPoint[ATU5010_TIMERD_AgentController::emCLKBUS0ClockIndex] = current_time;
    }
    if (mCLKBUS1Period != (sc_dt::uint64)clock_bus1.read()) {
        mCLKBUS1Period = (sc_dt::uint64)clock_bus1.read();
        mInputClockStartPoint[ATU5010_TIMERD_AgentController::emCLKBUS1ClockIndex] = current_time;
    }
    if (mCLKBUS2Period != (sc_dt::uint64)clock_bus2.read()) {
        mCLKBUS2Period = (sc_dt::uint64)clock_bus2.read();
        mInputClockStartPoint[ATU5010_TIMERD_AgentController::emCLKBUS2ClockIndex] = current_time;
    }
    if (mCLKBUS3Period != (sc_dt::uint64)clock_bus3.read()) {
        mCLKBUS3Period = (sc_dt::uint64)clock_bus3.read();
        mInputClockStartPoint[ATU5010_TIMERD_AgentController::emCLKBUS3ClockIndex] = current_time;
    }
    if (mCLKBUS4Period != (sc_dt::uint64)clock_bus4.read()) {
        mCLKBUS4Period = (sc_dt::uint64)clock_bus4.read();
        mInputClockStartPoint[ATU5010_TIMERD_AgentController::emCLKBUS4ClockIndex] = current_time;
    }
    if (mCLKBUS5Period != (sc_dt::uint64)clock_bus5.read()) {
        mCLKBUS5Period = (sc_dt::uint64)clock_bus5.read();
        mInputClockStartPoint[ATU5010_TIMERD_AgentController::emCLKBUS5ClockIndex] = current_time;
    }

    for (unsigned int index = 0; index < ATU5010_TIMERD_AgentController::emSub ; index++) {
        mTIMERD_SUB[index]->ClockHandleFunction();
    }

}//}}}


void ATU5010_TIMERD::HandleInputCaptureMethod (void)
{//{{{
    if (!(mATU5010->GetResetStatus("presetz") || mATU5010->GetResetStatus("presetz_bif") ||
    (!mATU5010->CheckClockPeriod("CLK_HSB")) || (!mATU5010->CheckClockPeriod("CLK_LSB")))) {
        for (unsigned int index = 0; index < 8; index++) {
            if ((*PHUDFE[0][index])->read()) {
                PHUDFE_flag[index] = true;
            }
        }

        for (unsigned int index = 0; index < 8; index++) {
            if (PHUDFE_flag[index]) {
                mInputCaptureEvent.notify(mPCLKPeriod, mATU5010->mTimeResolutionUnit);
                break;
            }
        }
    }
}//}}}

void ATU5010_TIMERD::InputCaptureWriteMethod (void)
{//{{{
    if (!(mATU5010->GetResetStatus("presetz") || mATU5010->GetResetStatus("presetz_bif") ||
    (!mATU5010->CheckClockPeriod("CLK_HSB")) || (!mATU5010->CheckClockPeriod("CLK_LSB")))) {
        unsigned int SEL_CNT =  mATU5010_TIMERDFunc->Get_CCAPSELD_CCAPSEL()?
                    ATU5010_TIMERD_SUB_AgentController::emCounter2
                    :ATU5010_TIMERD_SUB_AgentController::emCounter1;
        for (unsigned int index = 0; index < 8; index++) {
            if (PHUDFE_flag[index]){
                mATU5010_TIMERDFunc->Set_CICRD_CICRD(index,
                    mTIMERD_SUB[0]->GetCounterValue(SEL_CNT,0));
                PHUDFE_flag[index] = false;
            }
        }
    }
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5010_TIMERD::EnableReset (const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    for (unsigned int sub = 0; sub < ATU5010_TIMERD_AgentController::emSub; sub++) {
        mTIMERD_SUB[sub]->EnableReset(is_active);
    }
    mATU5010_TIMERDFunc->EnableReset(is_active);
}//}}}
///*********************************
/// Function     : Initialize
/// Description  : Process reset function
///*********************************
void ATU5010_TIMERD::Initialize (void)
{//{{{
    mCLKBUS0Period = 0;
    mCLKBUS1Period = 0;
    mCLKBUS2Period = 0;
    mCLKBUS3Period = 0;
    mCLKBUS4Period = 0;
    mCLKBUS5Period = 0;
    for (unsigned int i = 0; i < ATU5010_TIMERD_AgentController::emAllClockIndex; i++) {
        mInputClockStartPoint[i] = 0;
    }
}//}}}

/*********************************
// Function     : GetTCNTDVal
// Description  : Return TCNT1D0/TCNT2D0 value
// Parameter    :
//      counter_name           TCNT1D0/TCNT2D0
// Return value : TCNT1D0/TCNT2D0 value
// Note: If receive time and counter update time occur simultaneously, the counter value before update is returned. This function is used by Timer A.
**********************************/
unsigned int ATU5010_TIMERD::GetTCNTDVal (const std::string counter_name)
{
    unsigned int cur_val = 0;
    if (counter_name == "TCNT1D0") {
        if(mTIMERD_SUB[0]->mIsTimer1Running){
            cur_val = mTIMERD_SUB[0]->GetCounterValue(0,0);
        } else {
            cur_val = mTIMERD_SUB[0]->GetTCNTDRegisterVal(0);
        }
    } else { //counter_name == "TCNT2D0"
        if(mTIMERD_SUB[0]->mIsTimer2Running){
            cur_val = mTIMERD_SUB[0]->GetCounterValue(1,0);
        } else {
            cur_val = mTIMERD_SUB[0]->GetTCNTDRegisterVal(1);
        }
    }
    return cur_val;

}
// vim600: set foldmethod=marker :
