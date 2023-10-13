// ---------------------------------------------------------------------
// $Id: AHB_UIMPL.h,v 1.2 2013/11/27 01:47:07 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __AHB_UIMPL_H__
#define __AHB_UIMPL_H__
#include "BusSlaveBase.h"
#include "AHBDataHandler_UIMPL.h" 
#include "AHBERRSLV_UIMPL.h" 
#include "AHBAgentController_UIMPL.h"

class AHB_UIMPL:public sc_module,
                public AHBAgentController_UIMPL,
                public BusSlaveBase<32,2>
{
private:
    enum eResetActiveLevel {//Define active level of reset signal
        emResetActive = 0
    };
    //Declare variables
    AHBERRSLV_UIMPL *mAHBERRSLV_UIMPL;//Instance of AHBERRSLV_UIMPL class
    AHBDataHandler_UIMPL *mAHBDataHandler_UIMPL;//Instance of AHBDataHandler_UIMPL class

    sc_dt::uint64 mPCLKPeriod;//Period value of PCLK clock
    sc_dt::uint64 mHCLKPeriod;//Period value of HCLK clock
    sc_dt::uint64 mPCLKFreq;//Frequency value of PCLK clock
    sc_dt::uint64 mHCLKFreq;//Frequency value of HCLK clock
    sc_dt::uint64 mPCLKOrgFreq;//Orginal frequency value of PCLK clock
    sc_dt::uint64 mHCLKOrgFreq;//Orginal frequency value of HCLK clock
    std::string mPCLKFreqUnit;//Frequency unit of PCLK clock
    std::string mHCLKFreqUnit;//Frequency unit of HCLK clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation
    bool mDumpInterrupt;//Enable or disable dumping the interrupt
    bool mIntrValue;//The value of interrupt port
    bool mErrorAccessIgnore;//Ignore error access
    bool mStatusClearIgnore;//Ignore clearing error status
    bool mClearOnlyOVF;//Clear only OVF bit or both OVF and ERR bits

    bool mPRESETZSignalCurrentValue;//Store current value of PRESETZ signal
    bool mPRESETZSignalPreviousValue;//Store previous value of PRESETZ signal
    bool mIsPRESETZHardActive;//Reset operation status of PRESETZ signal
    bool mIsPRESETZCmdReceive;//Receive status of AssertReset command of PRESETZ signal
    bool mIsPRESETZCmdActive;//Active status of AssertReset command of PRESETZ signal
    double mPRESETZCmdPeriod;//Period of AssertReset command of PRESETZ signal

    bool mHRESETZSignalCurrentValue;//Store current value of HRESETZ signal
    bool mHRESETZSignalPreviousValue;//Store previous value of HRESETZ signal
    bool mIsHRESETZHardActive;//Reset operation status of HRESETZ signal
    bool mIsHRESETZCmdReceive;//Receive status of AssertReset command of HRESETZ signal
    bool mIsHRESETZCmdActive;//Active status of AssertReset command of HRESETZ signal
    double mHRESETZCmdPeriod;//Period of AssertReset command of HRESETZ signal

    sc_event mPRESETZHardEvent;//Call HandlePRESETZHardMethod() when PRESETZ is active
    sc_event mPRESETZCmdEvent;//Call HandlePRESETZCmdMethod() when AssertReset of PRESETZ is called
    sc_event mPRESETZCmdCancelEvent;//Call CancelPRESETZCmdMethod() when AssertReset of PRESETZ is over

    sc_event mHRESETZHardEvent;//Call HandleHRESETZHardMethod() when HRESETZ is active
    sc_event mHRESETZCmdEvent;//Call HandleHRESETZCmdMethod() when AssertReset of HRESETZ is called
    sc_event mHRESETZCmdCancelEvent;//Call CancelHRESETZCmdMethod() when AssertReset of HRESETZ is over

    sc_event mHandleTransInfoEvent;//Call HandleTransInfoMethod() to handle transaction information after synchonized with PCLK
    sc_event mWriteIntrEvent;//Call WriteINTAHBMethod() to write the value to interrupt port
    sc_event mClearIntrEvent;//Call ClearINTAHBMethod() to clear interrupt port after 1 PCLK
    sc_event mErrorAccessIgnoreEvent;//Call ErrorAccessIgnoreMethod() to cancel ignoring error access
    sc_event mStatusClearEvent;//Call StatusClearMethod() to clear error status (ERR and OVF bits)
public:
    //Declare input/output ports
    sc_in<bool> PRESETZ;
    sc_in<bool> HRESETZ;
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> HCLK;
    sc_out<bool> INTAHB;
    //Declare target sockets
    TlmTargetSocket<32> *tse;
    TlmTargetSocket<32> *tss;

    SC_HAS_PROCESS (AHB_UIMPL);
    AHB_UIMPL (sc_module_name name, 
               unsigned int rLatency,
               unsigned int wLatency,
               unsigned int AHB_UIMPL_VERSION = 0);
    ~AHB_UIMPL (void);
    //Declare public members
    void start_of_simulation (void);
    void DumpStatusInfo (void);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
private:
    void HandlePCLKSignalMethod (void);
    void HandleHCLKSignalMethod (void);
    void HandlePRESETZSignalMethod (void);
    void HandleHRESETZSignalMethod (void);
    void HandlePRESETZHardMethod (void);
    void HandleHRESETZHardMethod (void);
    void HandlePRESETZCmdMethod (void);
    void HandleHRESETZCmdMethod (void);
    void CancelPRESETZCmdMethod (void);
    void CancelHRESETZCmdMethod (void);
    void HandleTransInfoMethod (void);
    void WriteINTAHBMethod (void);
    void ClearINTAHBMethod (void);
    void ErrorAccessIgnoreMethod (void);
    void StatusClearMethod (void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point);
    //Virtual functions of AHBAgentController_UIMPL class
    bool CheckClockPeriod (const std::string clock_name);
    void ControlIntr (void);
    void TransferTransInfo (TlmBasicPayload &trans);
    bool GetResetStatus (const std::string reset_name);
    void WaitTimeToClearErrorFlags (bool ovf_flag);
};
#endif //__AHB_UIMPL_H__
// vim600: set foldmethod=marker :
