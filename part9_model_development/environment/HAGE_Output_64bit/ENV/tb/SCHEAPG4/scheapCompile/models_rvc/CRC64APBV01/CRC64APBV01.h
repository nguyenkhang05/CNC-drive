// ---------------------------------------------------------------------
// $Id: CRC64APBV01.h,v 1.8 2019/12/17 01:50:15 huepham Exp $
//
// Copyright(c) 2018 - 2019 Renesas Electronics Corporation
// Copyright(c) 2018 - 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __CRC64APBV01_H__
#define __CRC64APBV01_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "CRC64APBV01_AgentController.h"
#include "rvc_common_model.h"

class CRC64APBV01_Func;

class CRC64APBV01: public sc_module
            ,public CRC64APBV01_AgentController
            ,public BusSlaveBase<32, 1>
            ,public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    PCLK;           // clock
    sc_in<bool>             PRESETn;        // reset

    // INT to CPU

    // REQ to DMAC

    TlmTargetSocket<32>     *ts; // target sockets 

    CRC64APBV01_Func        *mpCRC64APBV01Func;   // Pointer of CRC64APBV01 Func

private:
    // Enums
    
    // Variables
    bool            mIsPRESETnInit;     // Is PRESET initialized
    sc_dt::uint64   mPCLKPeriod;        // Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;       // Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;          // Frequency of PCLK
    std::string     mPCLKFreqUnit;      // Frequency unit of PCLK
    bool            mIsResetHardActive; // Is reset by port
    bool            mIsResetCmdReceive; // Is receive cmd reset
    bool            mIsResetCmdActive;  // Is reset by cmd
    double          mResetCmdPeriod;    // Period of cmd reset
    bool            mEnableRegisterMessage;         // Enable/disable info/warning/error message of register IF
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation
    double          mStartPCLKTime;     // Start time of PCLK

    //bool            mDumpInterrupt; // Indicate dump info of interrupt or not
    
    // Events
    sc_event mStopClockEvent;       // Used to inform thread that clock is stopped

    sc_event mResetCmdEvent;        // Event to reset by command 
    sc_event mResetCmdCancelEvent;  // Event to cancel reset by command
    sc_event mResetEvent;           // Used to inform thread that model is reset

    // Threads/Methods
    void MonitorPCLKMethod();
    void MonitorPRESETnMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    // Functions
    void EnableReset(const bool is_active);

    // Overwrite firtual functions of CRC64APBV01_AgentController
    bool GetResetStatus();
    bool CheckClockPeriod();

public:
    SC_HAS_PROCESS (CRC64APBV01);
    CRC64APBV01 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~CRC64APBV01 ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    //void DumpInterrupt (const std::string is_enable);
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
#endif //__CRC64APBV01_H__
