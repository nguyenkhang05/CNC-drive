// ---------------------------------------------------------------------
// $Id: ECC_BUS_MER_AD.h,v 1.4 2020/04/01 06:41:19 thunguyen3 Exp $
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
#ifndef __ECC_BUS_MER_AD_H__
#define __ECC_BUS_MER_AD_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "ECC_def.h"
#include "ECC_BUS_MER_AD_AgentController.h"

class ECC_BUS_MER_AD_Func;

class ECC_BUS_MER_AD: public sc_module
            ,public ECC_BUS_MER_AD_AgentController
            ,public BusSlaveBase<32, 1>
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    pclk;           // operating clock
    sc_in<bool>             presetn;        // reset
    sc_in<bool>             ICUM_CFAPT_lock;// ECCTEST_LOCK from ICUMHASS

    sc_out<bool>            aeccdisable;    // Detection/correction prohibition for ECC error in address
    sc_out<bool>            secdisable;     // Disable/Enable correction when 1-bit error is detected in data
    sc_out<bool>            eccdisable;     // Detection/correction prohibition for ECC error in data

    sc_out<bool>            adederrout;     // 2bit error in address
    sc_out<bool>            asederrout;     // 1bit error in address
    sc_out<bool>            dederrout;      // 2bit error in data
    sc_out<bool>            sederrout;      // 1bit error in data
    sc_out<bool>            keyopen_ap;        // Safety unlock status
    sc_out<bool>            keysecopen_ap;     // Safety and Security unlock status

    TlmTargetSocket<32>     *ts;    // target sockets 

    ECC_BUS_MER_AD_Func     *mpECC_BUS_MER_ADFunc;    // Pointer of ECC_BUS_MER_AD_Func

private:
    // Enums

    // Variables
    bool            mIspresetnInit;     // Is PRESET initialized
    sc_dt::uint64   mPCLKPeriod;        // Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;       // Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;          // Frequency of PCLK
    std::string     mPCLKFreqUnit;      // Frequency unit of PCLK
    bool            mIsResetHardActive; // Is reset by port
    bool            mIsResetCmdReceive; // Is receive cmd reset
    bool            mIsResetCmdActive;  // Is reset by cmd
    double          mResetCmdPeriod;    // Period of cmd reset
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation
    double          mStartPCLKTime;     // Start time of PCLK

    bool            mDumpInterrupt;
    bool            mEnableRegisterMessage;

    bool            mECCDISABLE;        // Value to write to eccdisable
    bool            mSECDISABLE;        // Value to write to secdisable
    bool            mAECCDISABLE;       // Value to write to aeccdisable
    bool            mADEDERROUT;        // Value to write to adederrout
    bool            mASEDERROUT;        // Value to write to asederrout
    bool            mDEDERROUT;         // Value to write to dederrout
    bool            mSEDERROUT;         // Value to write to sederrout
    
    bool            mIsAddrConfig;      // Indicate address of error is set or not
    unsigned int    mErrorAddress;      // Indicate the address of error
    ErrorInfo       mDataError;         // Data error (1bit or 2bit)
    ErrorInfo       mAddressError;      // Address error (1bit or 2bit)
    bool            mKeyOpen;           // Store value writing to keyopen port
    bool            mICUMCFAPTlock;     // Store value of ICUM_CFAPT_lock port

    // Events
    sc_event mResetCmdEvent;        // Event to reset by command 
    sc_event mResetCmdCancelEvent;  // Event to cancel reset by command
    sc_event mClkZeroEvent;         // Event to inform that clock is zero

    sc_event mWriteDisablePortEvent;    // Trigger WriteDisablePortMethod
    sc_event mWriteADEDERROUTEvent;     // Trigger WriteADEDERROUTMethod
    sc_event mWriteASEDERROUTEvent;     // Trigger WriteASEDERROUTMethod
    sc_event mWriteDEDERROUTEvent;      // Trigger WriteDEDERROUTMethod
    sc_event mWriteSEDERROUTEvent;      // Trigger WriteSEDERROUTMethod
    sc_event mNegateADEDERROUTEvent;    // Trigger NegateADEDERROUTMethod
    sc_event mNegateASEDERROUTEvent;    // Trigger NegateASEDERROUTMethod
    sc_event mNegateDEDERROUTEvent;     // Trigger NegateDEDERROUTMethod
    sc_event mNegateSEDERROUTEvent;     // Trigger NegateSEDERROUTMethod
    sc_event mProcessDataErrorEvent;    // Trigger ProcessDataErrorMethod
    sc_event mProcessAddressErrorEvent; // Trigger ProcessAddressErrorMethod
    sc_event mWriteKeyOpenEvent;        // Event to write keyopen port
    sc_event mWriteKeySecOpenEvent;     // Event to write keysecopen port

   // Threads/Methods
    void MonitorpclkMethod ();

    void MonitorpresetnMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    void WriteDisablePortMethod();

    void WriteADEDERROUTMethod();
    void NegateADEDERROUTMethod();
    void WriteASEDERROUTMethod();
    void NegateASEDERROUTMethod();
    void WriteDEDERROUTMethod();
    void NegateDEDERROUTMethod();
    void WriteSEDERROUTMethod();
    void NegateSEDERROUTMethod();
    void ProcessDataErrorMethod();
    void ProcessAddressErrorMethod();

    void ICUM_CFAPT_lockMethod();
    void WriteKeyOpenMethod();
    void WriteKeySecOpenMethod();

    // Functions
    void Initialize (void);
    void EnableReset (const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    double CalculateCLKPosEdge (const bool add_period, const double time_point);
    void CancelEvents();

    // Overwrite firtual functions of ECC_BUS_MER_AD_AgentController
    bool CheckClockPeriod();
    bool GetResetStatus();
    void WriteDisablePort(unsigned int dis_value);
    void AssertInterrupt(std::string int_name);
    void UpdateKeyCodeEnable(bool key_code_enable);
    

public:
    SC_HAS_PROCESS (ECC_BUS_MER_AD);
    ECC_BUS_MER_AD (sc_module_name name, std::string kind, const unsigned int rLatency, const unsigned int wLatency);
    ~ECC_BUS_MER_AD ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void IssueError (const unsigned int errortype);
    void SetErrorAddr (const unsigned int addr_value);
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
#endif //__ECC_BUS_MER_AD_H__
