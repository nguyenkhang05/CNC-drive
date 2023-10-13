// ---------------------------------------------------------------------
// $Id: ATU5_TIMERD.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "ATU5_TIMERD.h"
#include "ATU5_TIMERD_Func.h"
#include "ATU5_TIMERD_SUB.h"
#include "ATU5_TIMERD_SUB_Func.h"
#include "ATU5_Base.h"
///*********************************
/// Function     : ATU5_TIMERD
/// Description  : Constructor of ATU5_TIMERD class
///*********************************
ATU5_TIMERD::ATU5_TIMERD (sc_module_name name,
            ATU5_Base *ATU5_Ptr, unsigned int subblock):
                msbD(subblock),
                sc_module(name),
                ATU5_TIMERD_AgentController(),
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
    assert(ATU5_Ptr != NULL);
    mATU5 = ATU5_Ptr;
    mPCLKPeriod = 0;
    Initialize();
    //Initialize variables - for clock, time resolution, reset
    //Bind clock, reset signal and target socket to Bus IF
    std::ostringstream port_name;
    INT_OVF1 = new sc_out<bool>*[msbD];
    sc_assert(INT_OVF1 != NULL);

    INT_OVF2 = new sc_out<bool>*[msbD];
    sc_assert(INT_OVF2 != NULL);

    for (unsigned int index = 0; index < msbD; index++) {
        port_name.str("");
        port_name << "INT_OVF1" << index;
        INT_OVF1[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_OVF1[index] != NULL);
        INT_OVF1[index]->initialize(false);

        port_name.str("");
        port_name << "INT_OVF2" << index;
        INT_OVF2[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_OVF2[index] != NULL);
        INT_OVF2[index]->initialize(false);
    }

    TODA = new sc_out<bool>**[msbD];
    sc_assert(TODA != NULL);

    TODB = new sc_out<bool>**[msbD];
    sc_assert(TODB != NULL);

    TODMIG = new sc_out<bool>**[msbD];
    sc_assert(TODMIG != NULL);

    INT_UDF = new sc_out<bool>**[msbD];
    sc_assert(INT_UDF != NULL);

    INT_CMPA = new sc_out<bool>**[msbD];
    sc_assert(INT_CMPA != NULL);

    INT_CMPB = new sc_out<bool>**[msbD];
    sc_assert(INT_CMPB != NULL);

    INT_FEIED = new sc_out<bool>**[msbD];
    sc_assert(INT_FEIED != NULL);

    INT_REIED = new sc_out<bool>**[msbD];
    sc_assert(INT_REIED != NULL);

    for (unsigned int index = 0; index < msbD; index++) {
        TODA[index] = new sc_out<bool>*[ATU5_TIMERD_AgentController::emChannel];
        sc_assert(TODA[index] != NULL);

        TODB[index] = new sc_out<bool>*[ATU5_TIMERD_AgentController::emChannel];
        sc_assert(TODB[index] != NULL);

        TODMIG[index] = new sc_out<bool>*[ATU5_TIMERD_AgentController::emChannel];
        sc_assert(TODMIG[index] != NULL);

        INT_UDF[index] = new sc_out<bool>*[ATU5_TIMERD_AgentController::emChannel];
        sc_assert(INT_UDF[index] != NULL);

        INT_CMPA[index] = new sc_out<bool>*[ATU5_TIMERD_AgentController::emChannel];
        sc_assert(INT_CMPA[index] != NULL);

        INT_CMPB[index] = new sc_out<bool>*[ATU5_TIMERD_AgentController::emChannel];
        sc_assert(INT_CMPB[index] != NULL);

        INT_FEIED[index] = new sc_out<bool>*[ATU5_TIMERD_AgentController::emChannel];
        sc_assert(INT_FEIED[index] != NULL);

        INT_REIED[index] = new sc_out<bool>*[ATU5_TIMERD_AgentController::emChannel];
        sc_assert(INT_REIED[index] != NULL);

        for (unsigned int index2 = 0; index2 < ATU5_TIMERD_AgentController::emChannel; index2++) {

            port_name.str("");
            port_name << "TODA" << index << index2;
            TODA[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(TODA[index][index2] != NULL);
            TODA[index][index2]->initialize(false);

            port_name.str("");
            port_name << "TODB" << index << index2;
            TODB[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(TODB[index][index2] != NULL);
            TODB[index][index2]->initialize(false);

            port_name.str("");
            port_name << "TODMIG" << index << index2;
            TODMIG[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(TODMIG[index][index2] != NULL);
            TODMIG[index][index2]->initialize(false);

            port_name.str("");
            port_name << "INT_UDF" << index << index2;
            INT_UDF[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(INT_UDF[index][index2] != NULL);
            INT_UDF[index][index2]->initialize(false);

            port_name.str("");
            port_name << "INT_CMPA" << index << index2;
            INT_CMPA[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(INT_CMPA[index][index2] != NULL);
            INT_CMPA[index][index2]->initialize(false);

            port_name.str("");
            port_name << "INT_CMPB" << index << index2;
            INT_CMPB[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(INT_CMPB[index][index2] != NULL);
            INT_CMPB[index][index2]->initialize(false);

            port_name.str("");
            port_name << "INT_FEIED" << index << index2;
            INT_FEIED[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(INT_FEIED[index][index2] != NULL);
            INT_FEIED[index][index2]->initialize(false);

            port_name.str("");
            port_name << "INT_REIED" << index << index2;
            INT_REIED[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(INT_REIED[index][index2] != NULL);
            INT_REIED[index][index2]->initialize(false);
        }
    }

    PHUDFE = new sc_in<bool>**[msbD];
    sc_assert(PHUDFE != NULL);
    for (unsigned int index = 0; index < msbD; index++) {
        PHUDFE[index] = new sc_in<bool>*[8];
        sc_assert(PHUDFE[index] != NULL);

        for (unsigned int index2 = 0; index2 < 8; index2++) {
            port_name.str("");
            port_name << "PHUDFE" << index << index2;
            PHUDFE[index][index2] = new sc_in<bool>(port_name.str().c_str());
            sc_assert(PHUDFE[index][index2] != NULL);
        }
    }


    mATU5_TIMERDFunc = new ATU5_TIMERD_Func((std::string)name, this, msbD);
    sc_assert(mATU5_TIMERDFunc != NULL);
    std::ostringstream SubName;
    mTIMERD_SUB = new ATU5_TIMERD_SUB*[msbD];
    sc_assert(mTIMERD_SUB != NULL);
    for (unsigned int i = 0; i < msbD; i++)
    {
        SubName.str("");
        SubName << "ATU5_TIMERD_SUB" << i;
        mTIMERD_SUB[i] = new ATU5_TIMERD_SUB(SubName.str().c_str(), this, i);
        sc_assert(mTIMERD_SUB[i] != NULL);
    }
    
    //port connection
    for (unsigned int index = 0; index < msbD; index++) {
        (mTIMERD_SUB[index]->TAEO2A)(TAEO2A);
        (mTIMERD_SUB[index]->TAEO2B)(TAEO2B);
        (mTIMERD_SUB[index]->CLRB)(CLRB);
        (mTIMERD_SUB[index]->INT_OVF1)(*INT_OVF1[index]);
        (mTIMERD_SUB[index]->INT_OVF2)(*INT_OVF2[index]);
        (mTIMERD_SUB[index]->PHU0DFE)(*(PHUDFE[index][0]));
        (mTIMERD_SUB[index]->PHU1DFE)(*(PHUDFE[index][1]));
        for (unsigned int i = 0; i < ATU5_TIMERD_AgentController::emChannel; i++){
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
/// Function     : ~ATU5_TIMERD
/// Description  : Destructor of ATU5_TIMERD class
///*********************************
ATU5_TIMERD::~ATU5_TIMERD (void)
{//{{{
    delete mATU5_TIMERDFunc;
    for (unsigned int i = 0; i < msbD; i++) {
        delete mTIMERD_SUB[i];
    }
    delete[]mTIMERD_SUB;

    for (unsigned int i = 0; i < msbD; i++) {
        for (unsigned int j = 0; j < emChannel; j++) {
            delete TODA[i][j];
            delete TODB[i][j];
            delete TODMIG[i][j];
            delete INT_UDF[i][j];
            delete INT_CMPA[i][j];
            delete INT_CMPB[i][j];
            delete INT_FEIED[i][j];
            delete INT_REIED[i][j];
        }
    }
    for (unsigned int i = 0; i < msbD; i++) {
        delete[]TODA[i];
        delete[]TODB[i];
        delete[]TODMIG[i];
        delete[]INT_UDF[i];
        delete[]INT_CMPA[i];
        delete[]INT_CMPB[i];
        delete[]INT_FEIED[i];
        delete[]INT_REIED[i];
    }
    delete[]TODA;
    delete[]TODB;
    delete[]TODMIG;
    delete[]INT_UDF;
    delete[]INT_CMPA;
    delete[]INT_CMPB;
    delete[]INT_FEIED;
    delete[]INT_REIED;

    for (unsigned int i = 0; i < msbD; i++) {
        for (unsigned int j = 0; j < 8; j++) {
            delete PHUDFE[i][j];
        }
    }
    for (unsigned int i = 0; i < msbD; i++) {
        delete[]PHUDFE[i];
    }
    delete[]PHUDFE;

    for (unsigned int i = 0; i < msbD; i++) {
        delete INT_OVF1[i];
        delete INT_OVF2[i];
    }
    delete[]INT_OVF1;
    delete[]INT_OVF2;
}//}}}
///*********************************
/// Function     : EnableTID
/// Description  : Wrapper enable timerD
///*********************************
void ATU5_TIMERD::EnableTID(const bool is_start)
{//{{{
    if (is_start) {
        mATU5_TIMERDFunc->TimerDStatus(true);
    } else {
        mATU5_TIMERDFunc->TimerDStatus(false);
    }
}//}}}

void ATU5_TIMERD::HandleClockInputMethod (void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if (mPCLKPeriod != (sc_dt::uint64)PCLK.read()) {
        mPCLKPeriod = (sc_dt::uint64)PCLK.read();
        mInputClockStartPoint[ATU5_TIMERD_AgentController::emPCLKClockIndex] = current_time;
    }
    if (mCLKBUS0Period != (sc_dt::uint64)clock_bus0.read()) {
        mCLKBUS0Period = (sc_dt::uint64)clock_bus0.read();
        mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS0ClockIndex] = current_time;
    }
    if (mCLKBUS1Period != (sc_dt::uint64)clock_bus1.read()) {
        mCLKBUS1Period = (sc_dt::uint64)clock_bus1.read();
        mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS1ClockIndex] = current_time;
    }
    if (mCLKBUS2Period != (sc_dt::uint64)clock_bus2.read()) {
        mCLKBUS2Period = (sc_dt::uint64)clock_bus2.read();
        mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS2ClockIndex] = current_time;
    }
    if (mCLKBUS3Period != (sc_dt::uint64)clock_bus3.read()) {
        mCLKBUS3Period = (sc_dt::uint64)clock_bus3.read();
        mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS3ClockIndex] = current_time;
    }
    if (mCLKBUS4Period != (sc_dt::uint64)clock_bus4.read()) {
        mCLKBUS4Period = (sc_dt::uint64)clock_bus4.read();
        mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS4ClockIndex] = current_time;
    }
    if (mCLKBUS5Period != (sc_dt::uint64)clock_bus5.read()) {
        mCLKBUS5Period = (sc_dt::uint64)clock_bus5.read();
        mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS5ClockIndex] = current_time;
    }

    for (unsigned int index = 0; index < msbD; index++) {
        mTIMERD_SUB[index]->ClockHandleFunction();
    }

}//}}}


void ATU5_TIMERD::HandleInputCaptureMethod (void)
{//{{{
    if (!(mATU5->GetResetStatus("presetz") || mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5->CheckClockPeriod("CLK_LSB")))) {
        for (unsigned int index = 0; index < 8; index++) {
            if ((*PHUDFE[0][index])->read()) {
                PHUDFE_flag[index] = true;
            }
        }

        for (unsigned int index = 0; index < 8; index++) {
            if (PHUDFE_flag[index]) {
                mInputCaptureEvent.notify((double)mPCLKPeriod, mATU5->mTimeResolutionUnit);
                break;
            }
        }
    }
}//}}}

void ATU5_TIMERD::InputCaptureWriteMethod (void)
{//{{{
    if (!(mATU5->GetResetStatus("presetz") || mATU5->GetResetStatus("presetz_bif") ||
    (!mATU5->CheckClockPeriod("CLK_HSB")) || (!mATU5->CheckClockPeriod("CLK_LSB")))) {
    //    unsigned int SEL_CNT =  mATU5_TIMERDFunc->Get_CCAPSELD_CCAPSEL()?
    //                ATU5_TIMERD_SUB_AgentController::emCounter2
    //                :ATU5_TIMERD_SUB_AgentController::emCounter1;
        unsigned int SEL_CNT=0;
        if(mATU5_TIMERDFunc->Get_CCAPSELD_CCAPSEL()){
            SEL_CNT=ATU5_TIMERD_SUB_AgentController::emCounter2;
        }else{
            SEL_CNT=ATU5_TIMERD_SUB_AgentController::emCounter1;
        }  
        for (unsigned int index = 0; index < 8; index++) {
            if (PHUDFE_flag[index]){
                mATU5_TIMERDFunc->Set_CICRD_CICRD(index,
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
void ATU5_TIMERD::EnableReset (const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }
    for (unsigned int sub = 0; sub < msbD; sub++) {
        mTIMERD_SUB[sub]->EnableReset(is_active);
    }
    mATU5_TIMERDFunc->EnableReset(is_active);
}//}}}
///*********************************
/// Function     : Initialize
/// Description  : Process reset function
///*********************************
void ATU5_TIMERD::Initialize (void)
{//{{{
    mCLKBUS0Period = 0;
    mCLKBUS1Period = 0;
    mCLKBUS2Period = 0;
    mCLKBUS3Period = 0;
    mCLKBUS4Period = 0;
    mCLKBUS5Period = 0;
    for (unsigned int i = 0; i < ATU5_TIMERD_AgentController::emAllClockIndex; i++) {
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
unsigned int ATU5_TIMERD::GetTCNTDVal (const std::string counter_name)
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
