// ---------------------------------------------------------------------
// $Id: ECC_ETHERNET_E2.h,v 1.6 2019/09/26 10:07:23 landang Exp $
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_ETHERNET_E2_H__
#define __ECC_ETHERNET_E2_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "ECC_ETHERNET_E2_AgentController.h"
#include "ECC_def.h"
#include "rvc_common_model.h"

class ECC_ETHERNET_E2_Func;

class ECC_ETHERNET_E2: public sc_module
            ,public ECC_ETHERNET_E2_AgentController
            ,public BusSlaveBase<32, 1>
            ,public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    pclk;           // operating clock
    sc_in<bool>             presetn;        // reset

    sc_out<bool>            secdisable_rx;  // 1bit error correction prohibition (for RX RAM)
    sc_out<bool>            secdisable_tx;  // 1bit error correction prohibition (for TX RAM)
    sc_out<bool>            dederrout;      // 2bit error interrupt
    sc_out<bool>            sederrout;      // 1bit error interrupt

    TlmTargetSocket<32>     *ts;    // target sockets 

    ECC_ETHERNET_E2_Func    *mpECC_ETHERNET_E2Func;    // Pointer of ECC_ETHERNET_E2_Func

private:
    // Enums

    // Variables
    sc_dt::uint64   mPCLKPeriod;        // Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;       // Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;          // Frequency of PCLK
    std::string     mPCLKFreqUnit;      // Frequency unit of PCLK
    double          mStartPCLKTime;     // Start time of PCLK

    bool            mIspresetnInit;     // Is PRESET initialized
    bool            mIsResetHardActive; // Is reset by port
    bool            mIsResetCmdReceive; // Is receive cmd reset
    bool            mIsResetCmdActive;  // Is reset by cmd
    double          mResetCmdPeriod;    // Period of cmd reset

    bool            mEnableRegisterMessage; // Is enable regisyer message or not
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    bool            mDumpInterrupt; // Enable dump interrupt info or not
    
    bool            mSECDISABLE_RX; // Store value of secdisable_rx
    bool            mSECDISABLE_TX; // Store value of secdisable_tx
    bool            mDEDERROUT;     // Store value of dederrout
    bool            mSEDERROUT;     // Store value of sederrout

    // Events
    sc_event mClkZeroEvent;         // Notify clock is zero

    sc_event mResetCmdEvent;        // Event to reset by command 
    sc_event mResetCmdCancelEvent;  // Event to cancel reset by command

    sc_event mAssertDEDERROUTEvent;     // Trigger AssertDEDERROUTMethod
    sc_event mAssertSEDERROUTEvent;     // Trigger AssertSEDERROUTMethod
    sc_event mWriteDEDERROUTEvent;      // Trigger WriteDEDERROUTMethod
    sc_event mWriteSEDERROUTEvent;      // Trigger WriteSEDERROUTMethod
    sc_event mNegateSEDERROUTEvent;     // Trigger NegateSEDERROUTMethod
    sc_event mNegateDEDERROUTEvent;     // Trigger NegateDEDERROUTMethod
    sc_event mWriteSECDISABLE_TXEvent;  // Trigger WriteSECDISABLE_TXMethod
    sc_event mWriteSECDISABLE_RXEvent;  // Trigger WriteSECDISABLE_RXMethod

    // Threads/Methods
    void MonitorpclkMethod ();

    void presetnMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    void WriteSEDERROUTMethod();
    void WriteDEDERROUTMethod();
    void NegateSEDERROUTMethod();
    void NegateDEDERROUTMethod();
    void AssertDEDERROUTMethod();
    void AssertSEDERROUTMethod();
    void WriteSECDISABLE_TXMethod();
    void WriteSECDISABLE_RXMethod();

    // Functions
    void Initialize (void);
    void EnableReset (const bool is_active);
    void CancelEvents();

    // Overwrite firtual functions of ECC_ETHERNET_E2_AgentController
    void WriteTXDisPort(bool value);
    void WriteRXDisPort(bool value);
    bool GetResetStatus();
    bool CheckClockPeriod();

public:
    SC_HAS_PROCESS (ECC_ETHERNET_E2);
    ECC_ETHERNET_E2 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~ECC_ETHERNET_E2 ();

    // PythonIF
    void EnableRegisterMessage(const std::string is_enable);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void IssueError (const unsigned int errortype);
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
#endif //__ECC_ETHERNET_E2_H__
