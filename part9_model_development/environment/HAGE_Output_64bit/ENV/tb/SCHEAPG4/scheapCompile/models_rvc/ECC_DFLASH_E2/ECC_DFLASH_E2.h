// ---------------------------------------------------------------------
// $Id: ECC_DFLASH_E2.h,v 1.4 2019/09/06 08:48:49 landang Exp $
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_DFLASH_E2_H__
#define __ECC_DFLASH_E2_H__

#include "rvc_common_model.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "ECC_DFLASH_E2_AgentController.h"

class ECC_DFLASH_E2_Func;

class ECC_DFLASH_E2: public sc_module,
            public ECC_DFLASH_E2_AgentController,
            public BusSlaveBase<32,1>,
            public rvc_common_model
{
public:
    // Ports
    sc_in<sc_dt::uint64>    PCLK;    // Clock
    sc_in<bool>             PRESETn; // Reset
    sc_out<bool>            eccdisable; // Disable ECC
    sc_out<bool>            secdisable; // Disable 1-bit error correction
    sc_out<bool>            eccsed; // ECC 1-bit error interrupt
    sc_out<bool>            eccded; // ECC 2-bit error interrupt
    sc_out<bool>            eccovf; // ECC error overflow interrupt

    ECC_DFLASH_E2_Func  *mpECC_DFLASH_E2Func; // Pointer of ECC_DFLASH_E2 Func
    // Target sockets
    TlmTargetSocket<32> *ts; // Target socket to access register


private:
    // Enums
    enum eOutputPort {
        emEccDisable = 0,
        emSecDisable = 1,
        emEccSed     = 2,
        emEccDed     = 3,
        emErrOvf     = 4
    };

    // Struct

    // Variables
    bool            mIsPRESETnInit;     // Is PRESETn initialized
    sc_dt::uint64   mPCLKPeriod;        // Period of clk
    sc_dt::uint64   mclkOrgFreq;        // Previous frequency of clk
    sc_dt::uint64   mclkFreq;           // Frequency of clk
    std::string     mclkFreqUnit;       // Frequency unit of clk
    bool            mIsResetHardActive; // Is reset by port
    bool            mIsResetCmdReceive; // Is receive cmd reset
    bool            mIsResetCmdActive;  // Is reset by cmd
    double          mResetCmdPeriod;    // Period of cmd reset
    bool            mEnableRegisterMessage; // Is enable regisyer message or not
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation
    double          mStartPCLKTime;     // Start time of PCLK

    bool            mDumpInterrupt; // Enable dump interrupt or not
    bool            mEccDisableVal;
    bool            mSecDisableVal;
    bool            mEccSedVal;
    bool            mEccDedVal;
    bool            mEccOvfVal;
    bool            mIsErrAddrSet;
    unsigned int    mErrorAddr;
    bool            mSEDIEInitVal;
    bool            mIsSupport;

    // Events
    sc_event meHandleResetCmdEvent;  // Event to reset by command 
    sc_event meCancelResetCmdEvent;  // Event to cancel reset by command

    sc_event meRaiseEccSedInterruptEvent;
    sc_event meNegateEccSedInterruptEvent;
    sc_event meDriveEccSedInterruptEvent;
    sc_event meRaiseEccDedInterruptEvent;
    sc_event meNegateEccDedInterruptEvent;
    sc_event meDriveEccDedInterruptEvent;
    sc_event meRaiseErrOvfInterruptEvent;
    sc_event meNegateErrOvfInterruptEvent;
    sc_event meDriveErrOvfInterruptEvent;
    sc_event meWriteEccDisableEvent;
    sc_event meWriteSecDisableEvent;

    // Threads/Methods
    void RaiseEccSedInterruptMethod();
    void NegateEccSedInterruptMethod();
    void DriveEccSedInterruptMethod();
    void RaiseEccDedInterruptMethod();
    void NegateEccDedInterruptMethod();
    void DriveEccDedInterruptMethod();
    void RaiseErrOvfInterruptMethod();
    void NegateErrOvfInterruptMethod();
    void DriveErrOvfInterruptMethod();
    void WriteEccDisableMethod();
    void WriteSecDisableMethod();

    void PCLKMethod();
    void PRESETnMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    // Functions
    void Initialize(void);
    void EnableReset(const bool is_active);

    // Overwrite virtual functions of ECC_DFLASH_E2_AgentController
    void DriveInterrupt(const unsigned int port_id);
    sc_dt::uint64 GetClockPeriod();
    bool GetResetStatus();
    void NotifyEccStatus(const unsigned int port_id, const bool output_value);

public:
    SC_HAS_PROCESS(ECC_DFLASH_E2);
    ECC_DFLASH_E2(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~ECC_DFLASH_E2();

    // PythonIF
    void EnableRegisterMessage(const std::string is_enable);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void SetSEDIEInitVal (const std::string is_one);
    void SetADDCAPValue (const std::string is_support);
    void SetPAWIDTHValue (const unsigned int pawidth);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void IssueError (const unsigned int error_type);
    void SetErrorAddr (const unsigned int error_addr);
    void DumpStatusInfo (void);
    
};
#endif //__ECC_DFLASH_E2_H__

// vim600: set foldmethod=marker :
