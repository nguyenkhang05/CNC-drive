// ---------------------------------------------------------------------
// $Id: ECC_BUS_SEP_AD.h,v 1.5 2020/03/17 02:37:23 thunguyen3 Exp $
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
#ifndef __ECC_BUS_SEP_AD_H__
#define __ECC_BUS_SEP_AD_H__

#include "global.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "ECC_def.h"
#include "ECC_BUS_SEP_AD_AgentController.h"

class ECC_BUS_AECCCTL_Func;
class ECC_BUS_DECCCTL_Func;
class ECC_BUS_ECCCAP_Func;
template class BusSlaveBase<32 ,1>;
template class BusSlaveBase<32 ,2>;
template class BusSlaveBase<32 ,3>;

class ECC_BUS_SEP_AD: public sc_module
            ,public ECC_BUS_SEP_AD_AgentController
{
public:
    // Clock/Reset
    sc_in<sc_dt::uint64>    pclk;      // clock
    sc_in<bool>             presetn;   // reset

    // AECCTL
    sc_out<bool>            *aeccdisable;// Disable/Enable error detection in address

    // DECCTL
    sc_out<bool>            *secdisable; // Disable/Enable error correction for 1 bit error in data
    sc_out<bool>            *eccdisable; // Disable/Enable error detection in data

    // ECCCAP
    sc_out<bool>            *adederrout; // 2bit error in address
    sc_out<bool>            *asederrout; // 1bit error in address
    sc_out<bool>            *dederrout;  // 2bit error in data
    sc_out<bool>            *sederrout;  // 1bit error in data

    // Key open
    sc_in<bool>             keyopen;    // keyopen to write protected registers

    TlmTargetSocket<32>     *ts_aeccctl; // target sockets to ECC_BUS_AECCCTL_Func
    TlmTargetSocket<32>     *ts_deccctl; // target sockets to ECC_BUS_DECCCTL_Func
    TlmTargetSocket<32>     *ts_ecccap;  // target sockets to ECC_BUS_ECCCAP_Func

    ECC_BUS_AECCCTL_Func    *mpAECCCTL_Func;   // Pointer of ECC_BUS_AECCCTL_Func
    ECC_BUS_DECCCTL_Func    *mpDECCCTL_Func;   // Pointer of ECC_BUS_DECCCTL_Func
    ECC_BUS_ECCCAP_Func     *mpECCCAP_Func;   // Pointer of ECC_BUS_ECCCAP_Func
    BusSlaveBase<32, 1>     *mpBusSlaveBase1;
    BusSlaveBase<32, 2>     *mpBusSlaveBase2;
    BusSlaveBase<32, 3>     *mpBusSlaveBase3;

private:
    // Enums
    enum eGeneralInfo{
        emSizeofStatusReg = 32  // status register has 32bit for 32 error detection locations
    };

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

    bool            mDumpInterrupt; // Dump interrupt info or not
    bool            mEnableRegisterMessage;

    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation

    bool            mENA_AECCCTL;
    bool            mENA_DECCCTL;
    bool            mENA_ECCCAP;

    bool            mAECCDIS;   // Store value of aeccdisable
    bool            mDSECDIS;    // Store value of secdisable
    bool            mDECCDIS;    // Store value of eccdisable

    bool            mKeyOpen;    // Store value of keyopen port
    bool            mADEDERROUT; // Store value of adederrout port
    bool            mASEDERROUT; // Store value of asederrout port
    bool            mDEDERROUT;  // Store value of dederrout port
    bool            mSEDERROUT;  // Store value of sederrout port

    bool            mIsEDLConfig;   // Indicate EDL is configured from Python IF or not
    unsigned int    mErrorEDL;      // Error EDL
    ErrorInfo       mDataError;     // Data error (1bit or 2bit)
    ErrorInfo       mAddressError;  // Address error (1bit or 2bit)

    std::string     mErr_Kind;      // Kind of error
    unsigned int    mEna_Module;    // Config which module is enable
    unsigned int    mNum_Module;    // Number of enable modules
    std::vector<char*> mArr_Module; // Vector contain module name
    // Events
    sc_event mClkZeroEvent;         // Notify clock is zero

    sc_event mResetCmdEvent;        // Event to reset by command 
    sc_event mResetCmdCancelEvent;  // Event to cancel reset by command

    sc_event mWriteADEDERROUTEvent;     // Trigger WriteADEDERROUTMethod
    sc_event mNegateADEDERROUTEvent;    // Trigger NegateADEDERROUTMethod
    sc_event mWriteASEDERROUTEvent;     // Trigger WriteASEDERROUTMethod
    sc_event mNegateASEDERROUTEvent;    // Trigger NegateASEDERROUTMethod
    sc_event mWriteDEDERROUTEvent;      // Trigger WriteDEDERROUTMethod
    sc_event mNegateDEDERROUTEvent;     // Trigger NegateDEDERROUTMethod
    sc_event mWriteSEDERROUTEvent;      // Trigger WriteSEDERROUTMethod
    sc_event mNegateSEDERROUTEvent;     // Trigger NegateSEDERROUTMethod
    sc_event mWriteDisablePortEvent;    // Trigger WriteDisablePortMethod
    sc_event mProcessDataErrorEvent;    // Trigger ProcessDataErrorMethod
    sc_event mProcessAddressErrorEvent; // Trigger ProcessAddressErrorMethod


    // Threads/Methods
    void MonitorpclkMethod ();

    void presetnMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    void MonitorkeyopenMethod();
    void WriteDisablePortMethod();
    void WriteADEDERROUTMethod();
    void NegateADEDERROUTMethod();
    void WriteASEDERROUTMethod();
    void NegateASEDERROUTMethod();
    void WriteDEDERROUTMethod();
    void NegateDEDERROUTMethod();
    void WriteSEDERROUTMethod();
    void NegateSEDERROUTMethod();
    void ProcessAddressErrorMethod();
    void ProcessDataErrorMethod();

    // Functions
    void Initialize (void);
    void EnableReset (const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    double CalculateCLKPosEdge (const bool add_period, const double time_point);
    void CancelEvents();

    // Overwrite firtual functions of ECC_BUS_SEP_AD_AgentController
    void WriteDisPortAECCTL(unsigned int value);
    void WriteDisPortDECCTL(unsigned int value);
    bool GetKeyOpen();
    bool GetResetStatus ();
    bool CheckClockPeriod();
    void AssertInterrupt(std::string int_name);

public:
    SC_HAS_PROCESS (ECC_BUS_SEP_AD);
    ECC_BUS_SEP_AD (sc_module_name name, std::string err_kind, const unsigned int rLatency, const unsigned int wLatency, const unsigned int ena_module);
    ~ECC_BUS_SEP_AD ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void IssueError (const unsigned int errortype);
    void SetEDL (const unsigned int edl);
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
#endif //__ECC_BUS_SEP_AD_H__
