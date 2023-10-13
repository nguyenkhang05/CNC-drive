// ---------------------------------------------------------------------
// $Id: ATU5_TIMERE.h 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#ifndef __ATU5_TIMERE_H__
#define __ATU5_TIMERE_H__

#include "ATU5_TIMERE_AgentController.h"
#include "ATU5_Base.h"

class ATU5_TIMERE_Func;
class ATU5_TIMERE: public sc_module,
              public ATU5_TIMERE_AgentController
{
friend class ATU5_Base;
friend class ATU5_TIMERE_SUB;
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
    //Declare output signals
    sc_out<bool> ***TOE;
    sc_out<bool> ***INTATUECMIE;
    SC_HAS_PROCESS (ATU5_TIMERE);

    ATU5_TIMERE (sc_module_name name, 
           ATU5_Base   *ATU5_Ptr, unsigned int subblock);
    ~ATU5_TIMERE (void);

    //Declare public functions
    void EnableTIE(const bool is_start);
    void EnableReset (const bool is_active);
    ATU5_TIMERE_Func  *mATU5_TIMEREFunc;
private:

    unsigned int msbE;                        //Number of subblocks of Timer E

    sc_signal<bool> **dummy_POE_sig;
    
    double GetCurTime(void);

    void StartTimerMethod(void);
    void StopTimerMethod(void);
};
#endif
