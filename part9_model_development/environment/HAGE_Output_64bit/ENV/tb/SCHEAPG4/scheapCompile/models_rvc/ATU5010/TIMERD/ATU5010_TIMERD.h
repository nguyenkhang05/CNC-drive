// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERD.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
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
#ifndef __ATU5010_TIMERD_H__
#define __ATU5010_TIMERD_H__

#include "ATU5010_TIMERD_AgentController.h"
#include "ATU5010_AgentController.h"

class ATU5010_TIMERD_Func;
class ATU5010_TIMERD: public sc_module,
              public ATU5010_TIMERD_AgentController
{
friend class ATU5010;
friend class ATU5010_Func;
friend class ATU5010_TIMERD_SUB;
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
    sc_in<bool> CLRB;
    sc_in<bool> TAEO2A;
    sc_in<bool> TAEO2B;
    sc_in<bool> *PHUDFE[ATU5010_TIMERD_AgentController::emSub][8];
    // Declare output: emSub = 15; emChannel = 4;
    sc_out<bool> *TODA[ATU5010_TIMERD_AgentController::emSub][ATU5010_TIMERD_AgentController::emChannel];
    sc_out<bool> *TODB[ATU5010_TIMERD_AgentController::emSub][ATU5010_TIMERD_AgentController::emChannel];
    sc_out<bool> *TODMIG[ATU5010_TIMERD_AgentController::emSub][ATU5010_TIMERD_AgentController::emChannel];
    sc_out<bool> *INT_UDF[ATU5010_TIMERD_AgentController::emSub][ATU5010_TIMERD_AgentController::emChannel];
    sc_out<bool> *INT_OVF1[ATU5010_TIMERD_AgentController::emSub];
    sc_out<bool> *INT_OVF2[ATU5010_TIMERD_AgentController::emSub];
    sc_out<bool> *INT_CMPA[ATU5010_TIMERD_AgentController::emSub][ATU5010_TIMERD_AgentController::emChannel];
    sc_out<bool> *INT_CMPB[ATU5010_TIMERD_AgentController::emSub][ATU5010_TIMERD_AgentController::emChannel];
    sc_out<bool> *INT_FEIED[ATU5010_TIMERD_AgentController::emSub][ATU5010_TIMERD_AgentController::emChannel];
    sc_out<bool> *INT_REIED[ATU5010_TIMERD_AgentController::emSub][ATU5010_TIMERD_AgentController::emChannel];
    SC_HAS_PROCESS (ATU5010_TIMERD);

    ATU5010_TIMERD (sc_module_name name, 
           ATU5010   *ATU5010_Ptr);
    ~ATU5010_TIMERD (void);

    //Declare public functions
    void EnableTID(const bool is_start);
    void EnableReset (const bool is_active);
    void HandleClockInputMethod (void);
    void HandleInputCaptureMethod(void);
    void InputCaptureWriteMethod(void);
    void Initialize (void);
    unsigned int GetTCNTDVal (const std::string counter_name);
    double mInputClockStartPoint[ATU5010_TIMERD_AgentController::emAllClockIndex];
private:
    ATU5010_TIMERD_Func  *mATU5010_TIMERDFunc;   //ATU5010_TIMERD_Func class instance
    sc_dt::uint64   mPCLKPeriod;
    sc_dt::uint64   mCLKBUS0Period;
    sc_dt::uint64   mCLKBUS1Period;
    sc_dt::uint64   mCLKBUS2Period;
    sc_dt::uint64   mCLKBUS3Period;
    sc_dt::uint64   mCLKBUS4Period;
    sc_dt::uint64   mCLKBUS5Period;
    
    bool PHUDFE_flag[8];
    sc_event mInputCaptureEvent;

    //Declare virtual functions of ATU5010_TIMERD_AgentController class
};
#endif
