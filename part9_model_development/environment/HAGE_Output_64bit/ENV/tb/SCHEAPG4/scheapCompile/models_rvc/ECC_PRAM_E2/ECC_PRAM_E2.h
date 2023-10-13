// ---------------------------------------------------------------------
// $Id: ECC_PRAM_E2.h,v 1.5 2019/09/26 10:07:42 landang Exp $
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
#ifndef __ECC_PRAM_E2_H__
#define __ECC_PRAM_E2_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "ECC_PRAM_E2_AgentController.h"
#include "ECC_def.h"
#include "rvc_common_model.h"

#define DEFAULT_WORD        31
#define MIN_WORD            1
#define MAX_WORD            31
#define DEFAULT_ERR_STAG    1
#define MIN_ERR_STAG        0
#define MAX_ERR_STAG        8

class ECC_PRAM_E2_Func;

class ECC_PRAM_E2: public sc_module
            ,public ECC_PRAM_E2_AgentController
            ,public BusSlaveBase<32, 1>
            ,public rvc_common_model
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    pclk;           // operating clock
    sc_in<bool>             presetn;        // reset

    sc_in<sc_dt::uint64>    ram_clk;        // RAM clock

    sc_in<bool>             ecterve;        // Access enable signal of error judgment enable bit
    sc_in<bool>             ectere;         // 2bit error judgment enable signal

    sc_out<bool>            ectiovf;        // Overflow interrupt
    sc_out<bool>            ectie2;         // 2bit error interrupt
    sc_out<bool>            ectie1;         // 1bit error interrupt
    sc_out<bool>            ectpref;        // indicate there is 2 bit error (not through FF)

    TlmTargetSocket<32>     *ts;    // target sockets 

    ECC_PRAM_E2_Func        *mpECC_PRAM_E2Func;    // Pointer of ECC_PRAM_E2_Func

private:
    // Enums
    enum eClockInfo{
        empclkId        = 0
        ,emram_clkId    = 1
        ,emNumClk       = 2
    };

    // Variables
    sc_dt::uint64   mClkPeriod[emNumClk];       // Period of clock
    sc_dt::uint64   mClkOrgFreq[emNumClk];      // Previous frequency of clock
    sc_dt::uint64   mClkFreq[emNumClk];         // Frequency of clock
    std::string     mClkFreqUnit[emNumClk];     // Frequency unit of clock
    double          mStartClkTime[emNumClk];    // Start time of clock

    bool            mIspresetnInit;     // Is PRESET initialized
    bool            mIsResetHardActive; // Is reset by port
    bool            mIsResetCmdReceive; // Is receive cmd reset
    bool            mIsResetCmdActive;  // Is reset by cmd
    double          mResetCmdPeriod;    // Period of cmd reset
    bool            mEnableRegisterMessage; // Is enable regisyer message or not
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    unsigned int    mWORD;          // Store WORD parameter
    unsigned int    mERR_STAG;      // Store ERR_STAG parameter
    bool            mDumpInterrupt; // Enable dump interrupt info or not
    
    bool            mECTIOVF;   // Store value of ectiovf
    bool            mECTIE2;    // Store value of ectie2
    bool            mECTIE1;    // Store value of ectie1
    bool            mECTPREF;   // Store value of ectpref
    bool            mHasFatalError; // Indicate has 2bit error 

    bool            mIsAddrConfig;  // Indicate Error Address is configured from Python IF or not
    unsigned int    mErrorAddress;  // Error address
    bool            mECTERVESig;    // Store value of ecterve port    
    bool            mECTERESig;     // Store value of ectere port

    ErrorInfo       m1bitError;  // List of 1bit error input
    ErrorInfo       mFatalError; // List of fatal error input

    // Events
    sc_event mClkZeroEvent[emNumClk];   // Notify clock is zero

    sc_event mResetCmdEvent;        // Event to reset by command 
    sc_event mResetCmdCancelEvent;  // Event to cancel reset by command

    sc_event mWriteECTIOVFEvent;    // Trigger WriteectiovfMethod
    sc_event mWriteECTIE2Event;     // Trigger Writeectie2Method
    sc_event mWriteECTIE1Event;     // Trigger Writeectie1Method
    sc_event mNegateECTIE1Event;    // Trigger NegateECTIE1Method
    sc_event mNegateECTIE2Event;    // Trigger NegateECTIE2Method
    sc_event mNegateECTIOVFEvent;   // Trigger NegateECTIOVFMethod
    sc_event mWriteECTPREFEvent;    // Trigger WriteectprefMethod
    sc_event mNegateECTPREFEvent;   // Trigger NegateECTPREFMethod
    sc_event mClearECEMFEvent;      // Trigger ClearECEMFMethod
    sc_event mProcess1bitErrorEvent;    // Trigger Process1bitErrorMethod
    sc_event mProcessFatalErrorEvent;   // Trigger ProcessFatalErrorMethod


    // Threads/Methods
    void MonitorpclkMethod ();
    void Monitorram_clkMethod ();

    void presetnMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    void MonitorecterveMethod();
    void MonitorectereMethod();
    void WriteECTIE1Method();
    void WriteECTIE2Method();
    void NegateECTIE1Method();
    void NegateECTIE2Method();
    void WriteECTIOVFMethod();
    void NegateECTIOVFMethod();
    void WriteECTPREFMethod();
    void NegateECTPREFMethod();
    void ClearECEMFMethod();
    void Process1bitErrorMethod();
    void ProcessFatalErrorMethod();

    // Functions
    void Initialize (void);
    void EnableReset (const bool is_active);
    void CancelEvents();


    // Overwrite firtual functions of ECC_PRAM_E2_AgentController
    unsigned int GetERR_STAG();
    bool GetECTERVESig();
    unsigned int GetWORD();
    void AssertECTIOVFPort();
    void AssertECTIE1Port();
    void AssertECTIE2Port();
    bool GetResetStatus();
    bool CheckClockPeriod(const std::string clock_name);
    void DriveECTPREFPort();

public:
    SC_HAS_PROCESS (ECC_PRAM_E2);
    ECC_PRAM_E2 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~ECC_PRAM_E2 ();

    // PythonIF
    void EnableRegisterMessage(const std::string is_enable);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void WORD (const unsigned int value);
    void ERR_STAG (const unsigned int value);
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
#endif //__ECC_PRAM_E2_H__
