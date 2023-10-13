// ---------------------------------------------------------------------
// $Id: KCPROT_E2.h,v 1.4 2019/09/06 08:50:02 landang Exp $
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
#ifndef __KCPROT_E2_H__
#define __KCPROT_E2_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "KCPROT_E2_AgentController.h"
#include "rvc_common_model.h"

class KCPROT_E2_Func;

class KCPROT_E2: public sc_module
            ,public KCPROT_E2_AgentController
            ,public BusSlaveBase<32,1>
            ,public rvc_common_model
{
public:
    // Ports
    sc_in<sc_dt::uint64>    PCLK;           // Clock
    sc_in<bool>             PRESETn;        // Reset
    sc_in<bool>             ICUM_CFAPT_lock;// Security lock request
    
    sc_out<bool>            keyopen;        // Safety unlock status
    sc_out<bool>            keysecopen;     // Safety and Security unlock status

    KCPROT_E2_Func          *mpKCPROT_E2Func;  // Pointer of KCPROT Func

    TlmTargetSocket<32>     *ts;            // Target socket to access register
private:
    // Enums
    enum eKeyCode{
        emDefaultKeyCode = 0xA5A5A500
    };

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
    bool            mEnableRegisterMessage; // Is enable regisyer message or not
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation
    double          mStartPCLKTime;     // Start time of PCLK

    unsigned int    mKeyCode;           // Store KEYCODE parameter
    bool            mKeyOpen;           // Store value writing to keyopen port
    bool            mICUMCFAPTlock;     // Store value of ICUM_CFAPT_lock port


    // Events
    sc_event mResetCmdEvent;        // Event to reset by command 
    sc_event mResetCmdCancelEvent;  // Event to cancel reset by command
    
    sc_event mWriteKeyOpenEvent;    // Event to write keyopen port
    sc_event mWriteKeySecOpenEvent; // Event to write keysecopen port


    // Threads/Methods
    void PCLKMethod();
    void PRESETnMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    void ICUM_CFAPT_lockMethod();
    void WriteKeyOpenMethod();
    void WriteKeySecOpenMethod();

    // Functions
    void Initialize(void);
    void EnableReset(const bool is_active);
    double NextRisingEdge();

    // Overwrite firtual functions of KCPROT_E2_AgentController
    void UpdateKeyCodeEnable(bool key_code_enable);
    unsigned int GetKeyCode();
    bool GetResetStatus ();
    bool CheckClockPeriod ();

public:
    SC_HAS_PROCESS(KCPROT_E2);
    KCPROT_E2(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~KCPROT_E2();

    // PythonIF
    void EnableRegisterMessage(const std::string is_enable);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void KEYCODE(const unsigned int keycode);
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
#endif //__KCPROT_E2_H__
