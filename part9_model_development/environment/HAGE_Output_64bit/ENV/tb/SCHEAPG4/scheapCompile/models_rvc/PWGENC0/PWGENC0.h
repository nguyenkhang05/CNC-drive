// ---------------------------------------------------------------------
// $Id: PWGENC0.h,v 1.8 2020/01/15 13:23:55 landang Exp $
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
#ifndef __PWGENC0_H__
#define __PWGENC0_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "PWGENC0_AgentController.h"
#include "rvc_common_model.h"
#include <list>

class PWGENC0_Func;

class PWGENC0: public sc_module
            ,public PWGENC0_AgentController
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

    // Input port 
    sc_in<sc_dt::uint64>    *PWG_CKI[emNumClkOut];   // Clock input for counter
    sc_in<bool>             PWG_STE;
    sc_in<unsigned short>   PWG_PERIOD;

    // Output
    sc_out<bool>            PWG_INTR;
    sc_out<bool>            PWG_INTF;
    sc_out<bool>            PWG_TRGOUT;
    sc_out<bool>            PWG_TOUT;

private:
    // Instance
    PWGENC0_Func*   mpPWGENC0Func;

    // signal

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
    double          mStartPCLKTime;     // Start time of PCLK
    bool            mDumpInterrupt;     // Control dumping interrupt message
    bool            mEnableRegisterMessage;     // Enable/Disable dumping info/warning/error message of register IF

    bool            mIsResetRSFBit;     // Indicate the RSF bit is request to reset
    bool            mIsResetRSFTBit;    // Indicate the RSFT bit is request to reset
    bool            mIsResetFOFBit;     // Indicate the FOF bit is request to reset
    bool            mIsSetRSFBit;       // Indicate the RSF bit is request to set
    bool            mIsSetRSFTBit;      // Indicate the RSFT bit is request to set
    bool            mIsSetFOFBit;       // Indicate the FOF bit is request to set
    bool            mIsClearRSFBit;     // Indicate the RSF bit is request to clear
    bool            mIsClearRSFTBit;    // Indicate the RSFT bit is request to clear
    bool            mIsClearFOFBit;     // Indicate the FOF bit is request to clear
    bool            mRealTOUT;          // Store real value of TOUT, not force value
    bool            mPWG_TOUT;          // Store value to write to PWG_TOUT
    bool            mPWG_TRGOUT;        // Store value to write to PWG_TRGOUT
    bool            mPWG_INTR;          // Store value to write to PWG_INTR
    bool            mPWG_INTF;          // Store value to write to PWG_INTF
    bool            mEnableCounting;    // Indicate the counter is enable
    bool            mIsSuspendCounting; // Indicate counter is in suspend state (due to clock = 0 while TE = 1)
    double          mResumeCounterTime; // Time of resume counting (suspend state and be supplied clock again)
    unsigned int    mCounter;           // Store counter value when not count (stop or suspen)
    unsigned int    mPERIOD;            // Store value of PWG_PERIOD port
    bool            mPWG_STE;           // Store value of PWG_STE port
    double          mReloadCounterTime; // Time of reload counter
    sc_dt::uint64   mPWG_CKI[emNumClkOut];  // Store time of 1 cycle of PWG_CKI[i] (with unit according time resolution)

    // Events
    sc_event mResetCmdEvent;        // Event to reset by command 
    sc_event mResetCmdCancelEvent;  // Event to cancel reset by command
   
    sc_event mCmpMatchCRBREvent;    // Trigger CmpMatchCRBRMethod
    sc_event mCmpMatchCSBREvent;    // Trigger CmpMatchCSBRMethod
    sc_event mCmpMatchCTBREvent;    // Trigger CmpMatchCTBRMethod
    sc_event mCmpMatchPERIODEvent;  // Trigger CmpMatchPERIODMethod
    sc_event mLoadTCBREvent;        // Trigger LoadTCBRMethod
    sc_event mCheckPWG_CKIEvent;    // Trigger MonitorPWG_CKIMethod
    sc_event mUpdateRSFBitEvent;    // Trigger UpdateRSFBitMethod
    sc_event mUpdateRSFTBitEvent;   // Trigger UpdateRSFTBitMethod
    sc_event mUpdateFOFBitEvent;    // Trigger UpdateFOFBitMethod
    sc_event mWritePWG_TOUTEvent;   // Trigger WritePWG_TOUTMethod
    sc_event mWritePWG_TRGOUTEvent; // Trigger WritePWG_TRGOUTMethod
    sc_event mWritePWG_INTREvent;   // Trigger WritePWG_INTRMethod
    sc_event mWritePWG_INTFEvent;   // Trigger WritePWG_INTFMethod

    // Threads/Methods
    void MonitorPCLKMethod ();
    void MonitorPCLKRWMethod ();

    void MonitorPRESETZMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();
    
    void CmpMatchCRBRMethod();
    void CmpMatchCSBRMethod();
    void CmpMatchCTBRMethod();
    void CmpMatchPERIODMethod();
    void LoadTCBRMethod();
    void MonitorPWG_STEMethod();
    void MonitorPWG_CKIMethod();
    void MonitorPWG_PERIODMethod();
    void UpdateRSFBitMethod();
    void UpdateRSFTBitMethod();
    void UpdateFOFBitMethod();
    void WritePWG_TOUTMethod();
    void WritePWG_TRGOUTMethod();
    void WritePWG_INTRMethod();
    void WritePWG_INTFMethod();

    // Functions
    void Initialize (void);
    void EnableReset (const bool is_active);
    void CancelEvents();
    void CountToPERIOD(unsigned int current_counter);

    // Overwrite firtual functions of PWGENC0_AgentController
    bool GetResetStatus();
    bool CheckClockPeriod(std::string clock_name);
    void ClearTOUTPort();
    void ForceTOUT(bool fos);
    void LoadTCBREventAfter1PCLK();
    void RequestSetRSFBit();
    void RequestSetRSFTBit();
    void RequestSetFOFBit();
    void RequestClearRSFBit();
    void RequestClearRSFTBit();
    void RequestClearFOFBit();
    void EnableCounter(bool is_enable);
    unsigned int GetPERIOD();
    void UpdateCurrentCounter(unsigned int value);
    unsigned int GetCurrentCounter();
    bool GetPWG_STE();
    bool CheckCounterRunning();

public:
    SC_HAS_PROCESS (PWGENC0);
    PWGENC0 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~PWGENC0 ();

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
#endif //__PWGENC0_H__
