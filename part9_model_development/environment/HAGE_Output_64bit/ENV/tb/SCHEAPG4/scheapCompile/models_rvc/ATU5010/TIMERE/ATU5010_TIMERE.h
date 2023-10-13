// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERE.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#ifndef __ATU5010_TIMERE_H__
#define __ATU5010_TIMERE_H__

#include "ATU5010_TIMERE_AgentController.h"
#include "ATU5010_AgentController.h"

class ATU5010_TIMERE_Func;
class ATU5010_TIMERE: public sc_module,
              public ATU5010_TIMERE_AgentController
{
friend class ATU5010;
friend class ATU5010_Func;
friend class ATU5010_TIMERE_SUB;
private:
    enum eResetActiveLevel {
        emResetActive               = 0,
        emResetSignalNum            = 1
    };//Define active level of reset signal
public:
    //Declare input signals
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> clock_bus0;
    sc_in<sc_dt::uint64> clock_bus1;
    sc_in<sc_dt::uint64> clock_bus2;
    sc_in<sc_dt::uint64> clock_bus3;
    sc_in<sc_dt::uint64> clock_bus4;
    sc_in<sc_dt::uint64> clock_bus5;
    sc_in<bool> *POE[6];
    //Declare output signals: emChannel = 4; emSub = 10
    sc_out<bool> *TOE[ATU5010_TIMERE_AgentController::emSub][ATU5010_TIMERE_AgentController::emChannel];
    sc_out<bool> *INTATUECMIE[ATU5010_TIMERE_AgentController::emSub][ATU5010_TIMERE_AgentController::emChannel];
    SC_HAS_PROCESS (ATU5010_TIMERE);

    ATU5010_TIMERE (sc_module_name name, 
           ATU5010   *ATU5010_Ptr);
    ~ATU5010_TIMERE (void);

    //Declare public functions
    void EnableTIE(const bool is_start);
private:
    ATU5010_TIMERE_Func  *mATU5010_TIMEREFunc;   //ATU5010_TIMERE_Func class instance

    sc_signal<bool> *dummy_POE_sig[ATU5010_TIMERE_AgentController::emSub-6];
    
    double GetCurTime(void);

    void StartTimerMethod(void);
    void StopTimerMethod(void);
    void EnableReset (const bool is_active);
};
#endif
