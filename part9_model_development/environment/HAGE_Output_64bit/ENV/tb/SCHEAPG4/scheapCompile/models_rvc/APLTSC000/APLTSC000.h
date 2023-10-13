// ---------------------------------------------------------------------
// $Id: APLTSC000.h,v 1.3 2020/03/21 06:59:29 chuonghoangle Exp $
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
#ifndef __APLTSC000_H__
#define __APLTSC000_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "APLTSC000_AgentController.h"
#include "rvc_common_model.h"

class APLTSC000_Func;

class APLTSC000: public sc_module
            ,public APLTSC000_AgentController
            ,public BusSlaveBase<32, 1>
            ,rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    PCLK;           // clock
    sc_in<bool>             PRESETn;        // reset
    sc_in<bool>             app1_rst_n;     // SW reset 

    // supervisor mode
    sc_in<bool>             SVSTOP;

    TlmTargetSocket<32>     *ts; // target sockets 

    APLTSC000_Func               *mpAPLTSC000Func;   // Pointer of APLTSC000 Func

private:
    enum eInfo{
         emMax    = 0xFFFFFFFFFFFFFFFF
    };

    enum eResetInfo{
        emPRESETnId             = 0
        ,emapp1_rst_nId         = 1
        ,emNumReset             = 2
    };
    // Variables
    sc_dt::uint64   mPCLKPeriod;        // Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;       // Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;          // Frequency of PCLK
    std::string     mPCLKFreqUnit;      // Frequency unit of PCLK
    bool            mResetCurVal[emNumReset]; //Store current value of reset signals
    bool            mResetPreVal[emNumReset]; //Store previous value of reset signals
    bool            mIsResetHardActive[emNumReset]; // Is reset by port
    bool            mIsResetCmdReceive[emNumReset]; // Is receive cmd reset
    bool            mIsResetCmdActive[emNumReset];  // Is reset by cmd
    double          mResetCmdPeriod[emNumReset];    // Period of cmd reset
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    bool            mEmulationMode;
    bool            mIsCounterRunning;
    bool            mSvstop;
    sc_dt::uint64   mStartCounterValue;
    double          mStartCounterTime;
    bool            mEnableRegisterMessage;

    // Events
    sc_event mGetRMEvent;           // Call Monitorapp1_rst_nMethod when RM bit changed

    sc_event mResetHardEvent[emNumReset];       // Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emNumReset];        // Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emNumReset];  // Event to cancel reset by command

    sc_event mCancelCommandEvent;   // Used to inform thread that command is cancelled

    // Threads/Methods
    void MonitorPCLKMethod ();

    void MonitorPRESETnMethod ();
    void MonitorSVSTOPMethod ();
    void Monitorapp1_rst_nMethod ();
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    // Functions
    void Initialize (void);
    void EnableReset(const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    bool CheckClockPeriod (const std::string clock_name);
   
    void NotifyUpdateRMbit();
    void MonitorSVSDIS();
    void Emulation(bool svstop, unsigned int svsdis);
    void StartCounter();
    void StopCounter();
    void ReturnValueCounter();
    void UpdateCounterValue();
    sc_dt::uint64 GetCounterInReg();
    sc_dt::uint64 CalCounterStep();

    bool GetResetStatus(const std::string reset_name);
    bool CheckClockPeriod();

public:
    SC_HAS_PROCESS (APLTSC000);
    APLTSC000 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~APLTSC000 ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void DumpStatusInfo ();
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
#endif //__APLTSC000_H__
