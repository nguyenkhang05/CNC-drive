// ---------------------------------------------------------------------
// $Id: ATU5_TIMERD.h 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#ifndef __ATU5_TIMERD_H__
#define __ATU5_TIMERD_H__

#include "ATU5_TIMERD_AgentController.h"
#include "ATU5_Base.h"

class ATU5_TIMERD_Func;
class ATU5_TIMERD: public sc_module,
              public ATU5_TIMERD_AgentController
{
friend class ATU5_Base;

friend class ATU5_TIMERD_SUB;
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
    sc_in<bool> ***PHUDFE;
    // Declare output:
    sc_out<bool> ***TODA;
    sc_out<bool> ***TODB;
    sc_out<bool> ***TODMIG;
    sc_out<bool> ***INT_UDF;
    sc_out<bool> **INT_OVF1;
    sc_out<bool> **INT_OVF2;
    sc_out<bool> ***INT_CMPA;
    sc_out<bool> ***INT_CMPB;
    sc_out<bool> ***INT_FEIED;
    sc_out<bool> ***INT_REIED;
    SC_HAS_PROCESS (ATU5_TIMERD);

    ATU5_TIMERD (sc_module_name name, 
           ATU5_Base   *ATU5_Ptr, unsigned int subblock);
    ~ATU5_TIMERD (void);

    //Declare public functions
    void EnableTID(const bool is_start);
    void EnableReset (const bool is_active);
    void HandleClockInputMethod (void);
    void HandleInputCaptureMethod(void);
    void InputCaptureWriteMethod(void);
    void Initialize (void);
    unsigned int GetTCNTDVal (const std::string counter_name);
    double mInputClockStartPoint[ATU5_TIMERD_AgentController::emAllClockIndex];
    ATU5_TIMERD_Func  *mATU5_TIMERDFunc;   //ATU5_TIMERD_Func class instance
private:  
    sc_dt::uint64   mPCLKPeriod;
    sc_dt::uint64   mCLKBUS0Period;
    sc_dt::uint64   mCLKBUS1Period;
    sc_dt::uint64   mCLKBUS2Period;
    sc_dt::uint64   mCLKBUS3Period;
    sc_dt::uint64   mCLKBUS4Period;
    sc_dt::uint64   mCLKBUS5Period;
    
    bool PHUDFE_flag[8];
    unsigned int msbD; //Number of subblocks of timer D
    sc_event mInputCaptureEvent;

    //Declare virtual functions of ATU5_TIMERD_AgentController class
};
#endif
