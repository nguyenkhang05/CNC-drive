// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERE_SUB.h,v 1.2 2018/03/06 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_TIMERE_SUB_H__
#define __ATU5010_TIMERE_SUB_H__

#include "ATU5010_TIMERE_SUB_AgentController.h"
#include "ATU5010_TIMERE.h"
#include "ATU5010_AgentController.h"
#include "ATU5010_Timer.h"
class ATU5010_TIMERE_SUB_Func;
class ATU5010_TIMERE_SUB: public sc_module,
              public ATU5010_TIMERE_SUB_AgentController
{
friend class ATU5010;
friend class ATU5010_Func;
friend class ATU5010_TIMERE;
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
    sc_in<bool> POE;
    //Declare output signals
    sc_out<bool> *TOE[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_out<bool> *INTATUECMIE[ATU5010_TIMERE_SUB_AgentController::emChannel];
    //Declare target socket

    SC_HAS_PROCESS (ATU5010_TIMERE_SUB);

    ATU5010_TIMERE_SUB (sc_module_name name, 
            ATU5010_TIMERE *ATU5010_TIMERE_Ptr,
            unsigned int subnum);
    ~ATU5010_TIMERE_SUB (void);
     ATU5010_Timer *mFreeRunCnt[ATU5010_TIMERE_SUB_AgentController::emChannel];
     ATU5010_Timer *mRunCntDTRE[ATU5010_TIMERE_SUB_AgentController::emChannel];
    //Declare public functions
     void UpdateTCNTERegValue(unsigned int channel);
     ATU5010_TIMERE_SUB_Func       *mTIE_SUBFunc;                          //ATU5010_TIMERE_SUB_Func class instance
     bool     mTimerEPrescaler[ATU5010_TIMERE_SUB_AgentController::emChannel];
     bool     mSubBlockReloadEnable;
     bool     mClearCounter[ATU5010_TIMERE_SUB_AgentController::emChannel];
     sc_dt::uint64 mCountClkPeriod[ATU5010_TIMERE_SUB_AgentController::emChannel];
     sc_dt::uint64 mPCLKPeriod;
     sc_dt::uint64 mCLKBUS0;
     sc_dt::uint64 mCLKBUS1;
     sc_dt::uint64 mCLKBUS2;
     sc_dt::uint64 mCLKBUS3;
     sc_dt::uint64 mCLKBUS4;
     sc_dt::uint64 mCLKBUS5;
     
     double mInputClockStartPoint[ATU5010_TIMERE_SUB_AgentController::emAllClockBus];
     double mCountClockStartPoint;
     double mPCLKClockStartPoint;

private:
    //Method
    void   CheckClockInputMethod(void);
    void   HandleCountClockFunction(void);
    void   HandleTOEPortMethod(const unsigned int channel);
    void   RisingInterruptMethod(const unsigned int channel);
    void   FallingInterruptMethod(const unsigned int channel);
    void   PreCmpOvfMethod(const unsigned int channel);
    void   HandleCmpOvfMethod(const unsigned int channel);
    void   HandleCmpDTREMethod(const unsigned int channel);
    void   HandleCmpCYLRMethod(const unsigned int channel);
    void   PreCmpDTREMethod(const unsigned int channel);
    void   PreCmpCYLRMethod(const unsigned int channel);
    void   HandleFlagAndInterruptDTREMethod(const unsigned int channel);
    void   HandleFlagAndInterruptCYLRMethod(const unsigned int channel);
    void   StartTimerMethod(const unsigned int channel);
    void   StopTimerMethod(const unsigned int channel);
    void   RestartPWMCycleMethod(const unsigned int channel);
    void   RewriteCounterMethod(const unsigned int channel);
    void   ShutOffRequestFlowMethod(void);
    void   AdjustForceCmpEventMethod(const unsigned int channel);
    void   AdjustStartEventMethod(const unsigned int channel);
    void   AdjustTOEChangeEventMethod(unsigned int channel);
    void   PostCmpCYLRMethod(unsigned int channel);
    void   EnableReset (const bool is_active);
    void   Initialize (void);
    void   InitializePort (void);
    void   CancelAllEvents (void);

    //variables
    bool mTOEShutOffRequest[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mTOEClearRequest[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mTOESetRequest[ATU5010_TIMERE_SUB_AgentController::emChannel];
    
    bool mRegClearCounterRequest[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool mCYLRCmpMatchBit[ATU5010_TIMERE_SUB_AgentController::emChannel];
    double      mOvfTime[ATU5010_TIMERE_SUB_AgentController::emChannel];
    double      mCmpDTRETime[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool        mIsOvf[ATU5010_TIMERE_SUB_AgentController::emChannel];
    bool        mIsCmpDTRE[ATU5010_TIMERE_SUB_AgentController::emChannel];

    unsigned int mSubNum;

    //event
    sc_event mTOEChangeRequestEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mRisingInterruptEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mFallingInterruptEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    
    sc_event mHandleFlagAndInterruptDTREEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mHandleFlagAndInterruptCYLREvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mDutyMatchEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mCycleMatchEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mCounterOverflowEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mPostCycleMatchEvent[ATU5010_TIMERE_SUB_AgentController::emChannel];
    sc_event mShutOffRequestEvent;

    double CalculateClockEdge (unsigned int channel, const bool is_pos, const bool add_period, const double time_point,const double time_change_point);
    unsigned int GetCntValue(const unsigned int channel);
    void SetCntValue(unsigned int channel, const unsigned int count);
    void SetCntDTREValue(unsigned int channel, const unsigned int count);
    void SetCntCmpValue(unsigned int channel, unsigned int value);
    void SetCntDTRECmpValue(unsigned int channel, unsigned int value);
    double GetCurTime(void);

};
#endif
