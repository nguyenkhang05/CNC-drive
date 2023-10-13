// ---------------------------------------------------------------------
// $Id: PWSELD0.h,v 1.9 2020/01/15 13:23:55 landang Exp $
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
#ifndef __PWSELD0_H__
#define __PWSELD0_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "PWSELD0_AgentController.h"
#include "rvc_common_model.h"
#include <list>

class PWSELD0_Func;

class PWSELD0: public sc_module
            ,public PWSELD0_AgentController
            ,public BusSlaveBase<32, 1>
            ,public rvc_common_model
{
private:
    // Enums

public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    PCLK;               // Clock for APB
    sc_in<bool>             PRESETZ;            // Reset of domain PCLK

    // APB I/F
    TlmTargetSocket<32>     *ts; // target sockets 

    // Test port
    sc_in<bool>             SVSTOP; // Supervisor mode signal

    // In/Out ports
    sc_in<bool>             *PWG_TRGOUT[emNumConnectedPWG];     // TRGOUT from PWGs
    sc_in<bool>             *ADC_CONV_END[emNumConnectedADC];   // Convert-end signal from ADCs
    sc_in<unsigned short>   *ADC_PWDDR[emNumConnectedADC];      // Convert value from ADCs
    sc_out<bool>            *PWS_ADTRG[emNumConnectedADC];      // Trigger signal to ADCs
    sc_out<unsigned int>    PWS_PVCR_VALUE;                     // value sent to selected ADC
    sc_out<bool>            PWS_INT_QFULL;                      // Interrupt queue full

private:
    // Instance
    PWSELD0_Func*   mpPWSELD0Func;

    // signal

    // Variables
    bool            mIsPRESETZInit;     // Is PRESET initialized
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
    bool            mDumpInterrupt;     // Control dumping interrupt message
    bool            mEnableRegisterMessage;     // Enable/Disable dumping info/warning/error message of register IF

    bool            mPWG_TRGOUT[4][emNumConnectedPWG]; // Store value of PWG_TRGOUT
    bool            mPrePWG_TRGOUT[emNumConnectedPWG]; // Store value of PWG_TRGOUT
    bool            mPWS_ADTRG[emNumConnectedADC];  // Store value of PWS_ADTRG
    unsigned int    mPWS_PVCR_VALUE;                // Store value of PWS_PVCR_VALUE
    bool            mPWS_INT_QFULL;                 // Store value of PWS_INT_QFULL
    bool            mEnablePWS;                     // Indicate ENBL bit value
    bool            mDelayedSVSTOP;                 // Store value of SVSTOP after 0.5 cycle
    bool            mSVSTOP;                        // Store value of SVSTOP
    unsigned int    mPreADC_CONV_END[emNumConnectedADC];    // Store previous value of ADC_CONV_END;
    bool            mPendingADCInput;               // Indicate there is pending ADC input not process
    unsigned int    mIndexCounter;                  // Indicate time index store PWG_TRGOUT input each clock
    bool            mIsWaitConvEnd;                 // Indicate model wait for conveted end signal from ADC

    // Events
    sc_event mResetCmdEvent;        // Event to reset by command 
    sc_event mResetCmdCancelEvent;  // Event to cancel reset by command
    sc_event mClkZeroEvent;         // Event to inform that clock is zero

    sc_event mStoreQueueEvent[4];      // Trigger StoreQueueMethod
    sc_event mIssueADTRGEvent;      // Trigger IssueADTRGMethod
    sc_event mWriteOutputToADCEvent;// Trigger WriteOutputToADCMethod
    sc_event mNegatePWS_ADTRGEvent; // Trigger NegatePWS_ADTRGMethod
    sc_event mReceiveADCEvent;      // Trigger ReceiveADCMethod
    sc_event mSetDelayedSVSTOPEvent;// Trigger SetDelayedSVSTOPMethod
    sc_event mWritePWS_INT_QFULLEvent;  // Trigger WritePWS_INT_QFULLMethod
    sc_event mClearFlagEvent[emNumConnectedPWG];// Trigger ClearFlagMethod

    // Threads/Methods
    void MonitorPCLKMethod ();

    void MonitorPRESETZMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();
    void MonitorPWG_TRGOUTMethod();
    void StoreQueueMethod(unsigned int time_index);
    void IssueADTRGMethod();
    void WriteOutputToADCMethod();
    void NegatePWS_ADTRGMethod();
    void MonitorADC_CONV_ENDMethod();
    void ReceiveADCMethod();
    void MonitorSVSTOPMethod();
    void SetDelayedSVSTOPMethod();
    void WritePWS_INT_QFULLMethod();
    void ClearFlagMethod(unsigned int channel);

    // Functions
    void Initialize (void);
    void EnableReset (const bool is_active);
    void CancelEvents();

    // Overwrite firtual functions of PWSELD0_AgentController
    bool GetResetStatus();
    bool CheckClockPeriod();
    void EnablePWS(bool is_enable);
    void AssertInterrupt();
    void ClearFlag(unsigned int channel);

public:
    SC_HAS_PROCESS (PWSELD0);
    PWSELD0 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~PWSELD0 ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void EnableRegisterMessage (const std::string is_enable);
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
#endif //__PWSELD0_H__
