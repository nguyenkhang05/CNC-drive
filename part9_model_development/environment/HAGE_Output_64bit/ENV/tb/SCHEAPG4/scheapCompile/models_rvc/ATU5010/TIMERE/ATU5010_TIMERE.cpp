// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERE.cpp,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#include "ATU5010_TIMERE.h"
#include "ATU5010_TIMERE_Func.h"
#include "ATU5010_TIMERE_SUB.h"
#include "ATU5010_TIMERE_SUB_Func.h"
#include "ATU5010.h"
///*********************************
/// Function     : ATU5010_TIMERE
/// Description  : Constructor of ATU5010_TIMERE class
///*********************************
ATU5010_TIMERE::ATU5010_TIMERE (sc_module_name name,
            ATU5010 *ATU5010_Ptr):
                sc_module(name),
                ATU5010_TIMERE_AgentController(),
                PCLK("PCLK"),
                clock_bus0("clock_bus0"),
                clock_bus1("clock_bus1"),
                clock_bus2("clock_bus2"),
                clock_bus3("clock_bus3"),
                clock_bus4("clock_bus4"),
                clock_bus5("clock_bus5")
                
{//{{{
    //Initialize variables - for clock, time resolution, reset
    assert(mATU5010 != NULL);
    mATU5010 = ATU5010_Ptr;    

    //Bind clock, reset signal and target socket to Bus IF
    std::ostringstream port_name;
    for (unsigned int index = 0; index < 6; index++) {
        port_name.str("");
        port_name << "POE" << index;
        POE[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(POE[index] != NULL);
    }
    for (unsigned int index = 0; index < ATU5010_TIMERE_AgentController::emSub; index++) {
        for (unsigned int index2 = 0; index2 < ATU5010_TIMERE_AgentController::emChannel; index2++) {
        port_name.str("");
        port_name << "TOE" << index << index2;
        TOE[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TOE[index][index2] != NULL);

        port_name.str("");
        port_name << "INTATUECMIE" << index << index2;
        INTATUECMIE[index][index2] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INTATUECMIE[index][index2] != NULL);
        }
    }
    
    for (unsigned int index = 0; index < (ATU5010_TIMERE_AgentController::emSub-6); index++) {
        port_name.str("");
        port_name << "dummy_POE_sig" << index;
        dummy_POE_sig[index] = new sc_signal<bool>(port_name.str().c_str());
        sc_assert(dummy_POE_sig[index] != NULL);
    }
    
    mATU5010_TIMEREFunc = new ATU5010_TIMERE_Func((std::string)name, this);
    sc_assert(mATU5010_TIMEREFunc != NULL);
    
    mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub0] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB0", this, ATU5010_TIMERE_AgentController::emSub0);
    mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub1] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB1", this, ATU5010_TIMERE_AgentController::emSub1);
    mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub2] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB2", this, ATU5010_TIMERE_AgentController::emSub2);
    mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub3] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB3", this, ATU5010_TIMERE_AgentController::emSub3);
    mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub4] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB4", this, ATU5010_TIMERE_AgentController::emSub4);
    mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub5] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB5", this, ATU5010_TIMERE_AgentController::emSub5);
    mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub6] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB6", this, ATU5010_TIMERE_AgentController::emSub6);
    mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub7] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB7", this, ATU5010_TIMERE_AgentController::emSub7);
    mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub8] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB8", this, ATU5010_TIMERE_AgentController::emSub8);
	mTIMERE_SUB[ATU5010_TIMERE_AgentController::emSub9] = new ATU5010_TIMERE_SUB("ATU5010_TIMERE_SUB9", this, ATU5010_TIMERE_AgentController::emSub9);
    for (unsigned int i = 0; i < ATU5010_TIMERE_AgentController::emSub; i++) {
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
        } else {
            (mTIMERE_SUB[i]->POE)(*(dummy_POE_sig[i-6]));
        }
        for (unsigned int k = 0; k < emChannel; k++) {
            (*(mTIMERE_SUB[i]->TOE[k]))(*(TOE[i][k]));
            (*(mTIMERE_SUB[i]->INTATUECMIE[k]))(*(INTATUECMIE[i][k]));
        }
    }

 }//}}}
///*********************************
/// Function     : ~ATU5010_TIMERE
/// Description  : Destructor of ATU5010_TIMERE class
///*********************************
ATU5010_TIMERE::~ATU5010_TIMERE (void)
{//{{{ 
    delete mATU5010_TIMEREFunc;
    for (unsigned int i = 0; i < ATU5010_TIMERE_AgentController::emSub; i++) {
        delete mTIMERE_SUB[i];
    }
}//}}}

///*********************************
/// Function     : EnableTIE
/// Description  : Wrapper enable timerE
///*********************************
void ATU5010_TIMERE::EnableTIE(const bool is_start)
{//{{{
    if (is_start && (!(mATU5010->GetResetStatus("presetz"))) && (!(mATU5010->GetResetStatus("presetz_bif"))) && mATU5010->CheckClockPeriod("CLK_LSB")&& mATU5010->CheckClockPeriod("CLK_HSB")) {
        mATU5010_TIMEREFunc->TimerEStatus(true);
    } else {
        mATU5010_TIMEREFunc->TimerEStatus(false);
    }
}//}}}
///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ATU5010_TIMERE::EnableReset (const bool is_active)
{//{{{
    for (unsigned int sub = 0; sub < ATU5010_TIMERE_AgentController::emSub; sub++) {
       mTIMERE_SUB[sub]->EnableReset(is_active);
    }
    mATU5010_TIMEREFunc->EnableReset(is_active);

}//}}}

// vim600: set foldmethod=marker :
