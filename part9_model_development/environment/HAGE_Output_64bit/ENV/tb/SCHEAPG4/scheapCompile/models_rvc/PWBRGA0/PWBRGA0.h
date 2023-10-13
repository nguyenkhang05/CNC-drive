// ---------------------------------------------------------------------
// $Id: PWBRGA0.h,v 1.8 2020/01/15 13:23:55 landang Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PWBRGA0_H__
#define __PWBRGA0_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "PWBRGA0_AgentController.h"
#include "rvc_common_model.h"
#include <list>

class PWBRGA0_Func;

class PWBRGA0: public sc_module
            ,public PWBRGA0_AgentController
            ,public BusSlaveBase<32, 1>
            ,public rvc_common_model
{
private:
    // Enums

public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    PCLK;               // Clock for APB
    sc_in<sc_dt::uint64>    PCLKRW;             // Clock for register 
    sc_in<bool>             PRESETZ;            // Reset of domain PCLK

    // APB I/F
    TlmTargetSocket<32>     *ts; // target sockets 

    // Port related to test function
    sc_in<bool>             SVSTOP;             // Supervisor mode signal

    // Output clock
    sc_out<sc_dt::uint64>   *PWBA_CKEN[emNumClkOut];

private:
    // Instance
    PWBRGA0_Func*   mpPWBRGA0Func;

    // Variables
    bool            mIsPRESETZInit;     // Is PRESET initialized
    sc_dt::uint64   mPCLKPeriod;        // Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;       // Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;          // Frequency of PCLK
    std::string     mPCLKFreqUnit;      // Frequency unit of PCLK
    sc_dt::uint64   mPCLKRWPeriod;      // Period of PCLKRW
    sc_dt::uint64   mPCLKRWOrgFreq;     // Previous frequency of PCLKRW
    sc_dt::uint64   mPCLKRWFreq;        // Frequency of PCLKRW
    std::string     mPCLKRWFreqUnit;    // Frequency unit of PCLKRW
    bool            mIsResetHardActive; // Is reset by port
    bool            mIsResetCmdReceive; // Is receive cmd reset
    bool            mIsResetCmdActive;  // Is reset by cmd
    double          mResetCmdPeriod;    // Period of cmd reset
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation
    bool            mEnableRegisterMessage; // Enable/Disable info/warning/error message of register IF
    double          mStartPCLKTime;     // Start time of PCLK
    sc_dt::uint64   mCKEN[emNumClkOut]; // Store value to write to PWBA_CKEN port
    unsigned int    mTE;                // Store value to write to TE bits
    unsigned int    mPendCountNum[emNumClkOut]; // Store pending value of each counter
    double          mStartClockTime[emNumClkOut];   // Store time of rising edge of clock
    bool            mIsSVSTOP;

    // Events
    sc_event mWriteCKENEvent[emNumClkOut];       // Event to write PWBA_CKEN port
    sc_event mWriteTEBitEvent;      // Event to write TE bit status

    sc_event mResetCmdEvent;        // Event to reset by command 
    sc_event mResetCmdCancelEvent;  // Event to cancel reset by command
    sc_event mClkZeroEvent;         // Event to inform that clock is zero

    // Threads/Methods
    void MonitorPCLKMethod ();
    void MonitorPCLKRWMethod ();

    void MonitorPRESETZMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    void WriteTEBitMethod();
    void WriteCKENMethod(unsigned int index);
    void MonitorSVSTOPMethod();

    // Functions
    void Initialize (void);
    void EnableReset (const bool is_active);
    void CancelEvents();
    unsigned int GetCounter(unsigned int index);

    // Overwrite firtual functions of PWBRGA0_AgentController
    bool GetResetStatus();
    bool CheckClockPeriod(std::string clock_name);
    void StartCounter(unsigned int next_te_value);
    void StopCounter(unsigned int next_te_value);
    void UpdateEmulationStatus();

public:
    SC_HAS_PROCESS (PWBRGA0);
    PWBRGA0 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~PWBRGA0 ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
};
#endif //__PWBRGA0_H__
