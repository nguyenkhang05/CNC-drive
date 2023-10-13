// ---------------------------------------------------------------------
// $Id: ATU5_TIMERE.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include "ATU5_TIMERE.h"
#include "ATU5_TIMERE_Func.h"
#include "ATU5_TIMERE_SUB.h"
#include "ATU5_TIMERE_SUB_Func.h"
#include "ATU5_Base.h"
///*********************************
/// Function     : ATU5_TIMERE
/// Description  : Constructor of ATU5_TIMERE class
///*********************************
ATU5_TIMERE::ATU5_TIMERE (sc_module_name name,
            ATU5_Base *ATU5_Ptr, unsigned int subblock):
                msbE(subblock),
                sc_module(name),
                ATU5_TIMERE_AgentController(),
                PCLK("PCLK"),
                clock_bus0("clock_bus0"),
                clock_bus1("clock_bus1"),
                clock_bus2("clock_bus2"),
                clock_bus3("clock_bus3"),
                clock_bus4("clock_bus4"),
                clock_bus5("clock_bus5")
                
{//{{{

    //Initialize variables - for clock, time resolution, reset
    assert(ATU5_Ptr != NULL);
    mATU5 = ATU5_Ptr;    

    //Bind clock, reset signal and target socket to Bus IF
    std::ostringstream port_name;
    for (unsigned int index = 0; index < 6; index++) {
        port_name.str("");
        port_name << "POE" << index;
        POE[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(POE[index] != NULL);
    }

    TOE = new sc_out<bool>**[msbE];
    sc_assert(TOE != NULL);
    for (unsigned int index = 0; index < msbE; index++) {
        TOE[index] = new sc_out<bool>*[ATU5_TIMERE_AgentController::emChannel];
        sc_assert(TOE[index] != NULL);

        for (unsigned int index2 = 0; index2 < ATU5_TIMERE_AgentController::emChannel; index2++) {
            port_name.str("");
            port_name << "TOE" << index << index2;
            TOE[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(TOE[index][index2] != NULL);
            TOE[index][index2]->initialize(false);
        }
    }

    INTATUECMIE = new sc_out<bool>**[msbE];
    sc_assert(INTATUECMIE != NULL);
    for (unsigned int index = 0; index < msbE; index++) {
        INTATUECMIE[index] = new sc_out<bool>*[ATU5_TIMERE_AgentController::emChannel];
        sc_assert(INTATUECMIE[index] != NULL);

        for (unsigned int index2 = 0; index2 < ATU5_TIMERE_AgentController::emChannel; index2++) {
            port_name.str("");
            port_name << "INTATUECMIE" << index << index2;
            INTATUECMIE[index][index2] = new sc_out<bool>(port_name.str().c_str());
            sc_assert(INTATUECMIE[index][index2] != NULL);
            INTATUECMIE[index][index2]->initialize(false);
        }
    }


    dummy_POE_sig = new sc_signal<bool>* [msbE - 6];
    sc_assert(dummy_POE_sig != NULL);
    for (unsigned int index = 0; index < (msbE-6); index++) {
        port_name.str("");
        port_name << "dummy_POE_sig" << index;
        dummy_POE_sig[index] = new sc_signal<bool>(port_name.str().c_str());
        sc_assert(dummy_POE_sig[index] != NULL);
    }
    
    mATU5_TIMEREFunc = new ATU5_TIMERE_Func((std::string)name, this, msbE);
    sc_assert(mATU5_TIMEREFunc != NULL);
    
    mTIMERE_SUB = new ATU5_TIMERE_SUB*[msbE];
    sc_assert(mTIMERE_SUB != NULL);
    for (unsigned int i = 0; i < msbE; i++)
    {
        std::ostringstream SubName;
        SubName.str("");
        SubName << "ATU5_TIMERE_SUB" << i;
        mTIMERE_SUB[i] = new ATU5_TIMERE_SUB(SubName.str().c_str(), this, i);
        sc_assert(mTIMERE_SUB[i]!= NULL);
    }

    //bind ports
    for (unsigned int i = 0; i < msbE; i++) {
        sc_assert (mTIMERE_SUB[i] != NULL);
        (mTIMERE_SUB[i]->PCLK)(PCLK);
        (mTIMERE_SUB[i]->clock_bus0)(clock_bus0);
        (mTIMERE_SUB[i]->clock_bus1)(clock_bus1);
        (mTIMERE_SUB[i]->clock_bus2)(clock_bus2);
        (mTIMERE_SUB[i]->clock_bus3)(clock_bus3);
        (mTIMERE_SUB[i]->clock_bus4)(clock_bus4);
        (mTIMERE_SUB[i]->clock_bus5)(clock_bus5);
        if (i < 6) {
            (mTIMERE_SUB[i]->POE)(*(POE[i]));
            mTIMERE_SUB[i]->mTIE_SUBFunc->mIsShutOffEnableSub=true;
        } else {
            (mTIMERE_SUB[i]->POE)(*(dummy_POE_sig[i-6]));
            mTIMERE_SUB[i]->mTIE_SUBFunc->mIsShutOffEnableSub=false;
        }
        for (unsigned int k = 0; k < emChannel; k++) {
            (*(mTIMERE_SUB[i]->TOE[k]))(*(TOE[i][k]));
            (*(mTIMERE_SUB[i]->INTATUECMIE[k]))(*(INTATUECMIE[i][k]));
        }
    }

 }//}}}
///*********************************
/// Function     : ~ATU5_TIMERE
/// Description  : Destructor of ATU5_TIMERE class
///*********************************
ATU5_TIMERE::~ATU5_TIMERE (void)
{//{{{ 
    delete mATU5_TIMEREFunc;

    for (unsigned int i = 0; i < msbE; i++) {
        delete mTIMERE_SUB[i];
    }
    delete[]mTIMERE_SUB;

    for (unsigned int i = 0; i < msbE; i++) {
        for (unsigned int j = 0; j < emChannel; j++){
            delete INTATUECMIE[i][j];
            delete TOE[i][j];
        }
        delete[]TOE[i];
        delete[]INTATUECMIE[i];
    }
    delete[]TOE;
    delete[]INTATUECMIE;

    for (unsigned int i = 0; i < msbE - 6; i++){
        delete dummy_POE_sig[i];
    }
    delete[]dummy_POE_sig;
}//}}}

///*********************************
/// Function     : EnableTIE
/// Description  : Wrapper enable timerE
///*********************************
void ATU5_TIMERE::EnableTIE(const bool is_start)
{//{{{
    if (is_start && (!(mATU5->GetResetStatus("presetz"))) && (!(mATU5->GetResetStatus("presetz_bif"))) && mATU5->CheckClockPeriod("CLK_LSB")&& mATU5->CheckClockPeriod("CLK_HSB")) {
        mATU5_TIMEREFunc->TimerEStatus(true);
    } else {
        mATU5_TIMEREFunc->TimerEStatus(false);
    }
}//}}}
///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5_TIMERE::EnableReset (const bool is_active)
{//{{{
    for (unsigned int sub = 0; sub < msbE; sub++) {
       mTIMERE_SUB[sub]->EnableReset(is_active);
    }
    mATU5_TIMEREFunc->EnableReset(is_active);

}//}}}

// vim600: set foldmethod=marker :
