// ---------------------------------------------------------------------
// $Id: ATU5_TIMERF.h 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#ifndef __ATU5_TIMERF_H__
#define __ATU5_TIMERF_H__


class ATU5_TIMERF_Func;

#include "ATU5_TIMERF_AgentController.h"



class ATU5_TIMERF_Func;
class ATU5_TIMERF:  public sc_module,
                    public ATU5_TIMERF_AgentController
{
  friend class ATU5_Base;
  friend class ATU5_TIMERF_Sub;
  friend class ATU5_TIMERF_Sub_Func;

private:
  enum eResetActiveLevel {    // Define active level of reset signal
        emResetActive = 0,
        emResetSignalNum = 2
  };


    /// Declare variables
    sc_dt::uint64 mCLKPeriod[emClkNum];                 //Period value of clock
    sc_dt::uint64 mTimeCLKChangePoint[emClkNum];
    sc_dt::uint64 mCLKFreq[emClkNum];                   //Frequency value of clock
    sc_dt::uint64 mCLKOrgFreq[emClkNum];                //Original frequency value of clock
    std::string   mCLKFreqUnit[emClkNum];               //Frequency unit of clock

    sc_signal<bool> **dummy_TIFB_sig;
    SC_SIGNAL(bool) **dummy_POE_sig;

    bool mResetCurVal[emResetSignalNum];        //Store current value of PRESETn or resetad_n
    bool mResetPreVal[emResetSignalNum];        //Store previous value of PRESETn or resetad_n
    bool mIsResetHardActive[emResetSignalNum];  //Reset operation status of PRESETn or resetad_n signal
    bool mIsResetCmdReceive[emResetSignalNum];  //Receive status of AssertReset command of PRESETn or resetad_n
    bool mIsResetCmdActive[emResetSignalNum];   //Active status of AssertReset command of PRESETn or resetad_n
    double mResetCmdPeriod[emResetSignalNum];   //Period of AssertReset command of PRESETn or resetad_n

    /// Declare events
    sc_event mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when PRESETn or resetad_n is active
    sc_event mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over
    sc_event mClkZeroEvent[emClkNum];                          //Notify clock is zero
    sc_event mClkUpdateEvent[emClkNum];

public:
    ATU5_TIMERF_Func *mATU5_TIMERF_Func;                // ATU5_TIMERF_Func class instance
    sc_event mTimerFEnableEvent;                        //Event of writing to ATUENR.TFE
    sc_event *mSubblockEnableEvent;          //Event of writing to TSTRF.STRF
    sc_event *mSubblockDisabledEvent;
    sc_event mTimerFDisableEvent;
    bool mIsTimerFEnabled;
    //
    sc_in<sc_dt::uint64>               nc_clk;             //noise cancel clock
    sc_in<sc_dt::uint64>  *CLKBUS[emClkNum-3];           //Clock from prescaler
    sc_in<bool>           **TIFA;               //event input
    sc_in<bool>           *TIFB[3];                      //event input for subblock 0 to 2
    sc_in<bool>           *TIA[8];                       //noise-canceled signal from timer A
    //
    sc_out<bool>          *POE[6];                       //shutoff inputs for PEO0 to POE5 of timer E (noise canceller output of timer F)
    sc_out<bool>          **OVFFI;              //overflow interrupt request.
    sc_out<bool>          **ICFFI;              //input capture interrupt request.

    SC_HAS_PROCESS(ATU5_TIMERF);
    ATU5_TIMERF (sc_module_name name, ATU5_Base *ATU5_Ptr , unsigned int subblock);
    ~ATU5_TIMERF(void);

    void EnableTimerF(const bool is_start);

    /// PythonIF
    void SetCLKPeriod (const std::string clock_name, const sc_dt::uint64 period);
    void EnableReset (const bool is_active);
private:
    /// Declare methods
    void Handlenc_clkSignalMethod   (void);
    void HandleCLKBUS_0SignalMethod (void);
    void HandleCLKBUS_1SignalMethod (void);
    void HandleCLKBUS_2SignalMethod (void);
    void HandleCLKBUS_3SignalMethod (void);
    void HandleCLKBUS_4SignalMethod (void);
    void HandleCLKBUS_5SignalMethod (void);


    /// Internal functions
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    double CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point,const double time_change_point);

    void CancelEvents (void);
    void Initialize (void);

    /// Virtual functions of ATU5_TIMERF_AgentController class
    bool CheckClockPeriod (const std::string clock_name);
    void NotifyEvent (const unsigned int event, bool enable);
};

#endif
