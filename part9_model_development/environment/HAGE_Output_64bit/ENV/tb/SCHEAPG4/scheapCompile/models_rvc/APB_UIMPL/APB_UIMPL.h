// ---------------------------------------------------------------------
// $Id: APB_UIMPL.h,v 1.1 2013/11/27 01:45:19 dungvannguyen Exp $
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
#ifndef __APB_UIMPL_H__
#define __APB_UIMPL_H__
#include "BusSlaveBase.h"
#include "APBDataHandler_UIMPL.h" 
#include "APBERRSLV_UIMPL.h" 
#include "APBAgentController_UIMPL.h"

class APB_UIMPL:public sc_module,
                public APBAgentController_UIMPL,
                public BusSlaveBase<32,2>
{
private:
    enum eResetActiveLevel {//Define active level of reset signal
        emResetActive = 0
    };
    //Declare variables
    APBERRSLV_UIMPL *mAPBERRSLV_UIMPL;//Instance of APBERRSLV_UIMPL class
    APBDataHandler_UIMPL *mAPBDataHandler_UIMPL;//Instance of APBDataHandler_UIMPL class

    sc_dt::uint64 mPCLKPeriod;//Period value of PCLK clock
    sc_dt::uint64 mPCLKFreq;//Frequency value of PCLK clock
    sc_dt::uint64 mPCLKOrgFreq;//Orginal frequency value of PCLK clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    std::string mPCLKFreqUnit;//Frequency unit of PCLK clock
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation
    bool mDumpInterrupt;//Enable/disable dumping the interrupt
    bool mIntrValue;//Interrupt status

    bool mResetSignalCurrentValue;//Store current value of PRESETZ signal
    bool mResetSignalPreviousValue;//Store previous value of PRESETZ signal
    bool mIsResetHardActive;//Reset operation status of PRESETZ signal
    bool mIsResetCmdReceive;//Receive status of AssertReset command of PRESETZ signal
    bool mIsResetCmdActive;//Active status of AssertReset command of PRESETZ signal
    double mResetCmdPeriod;//Period of AssertReset command of PRESETZ signal

    sc_event mResetHardEvent;//Call HandleResetHardMethod() when PRESETZ is active
    sc_event mResetCmdEvent;//Call HandleResetCmdMethod() when AssertReset is called
    sc_event mResetCmdCancelEvent;//Call CancelResetCmdMethod() when AssertReset is over

    sc_event mHandleTransInfoEvent;//Call HandleTransInfoMethod() to handle transaction information after synchonized with PCLK
    sc_event mWriteIntrEvent;//Call WriteINTAPBMethod() to write the value to interrupt port
    sc_event mClearIntrEvent;//Call ClearINTAPBMethod() to clear interrupt port after 1 PCLK
public:
    //Declare input/output ports
    sc_in<bool> PRESETZ;
    sc_in<sc_dt::uint64> PCLK;
    sc_out<bool> INTAPB;
    //Declare target sockets
    TlmTargetSocket<32> *tse;
    TlmTargetSocket<32> *tss;

    SC_HAS_PROCESS (APB_UIMPL);
    APB_UIMPL (sc_module_name name, 
               unsigned int rLatency,
               unsigned int wLatency,
               unsigned int APB_UIMPL_VERSION = 0);
    ~APB_UIMPL (void);
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
    void HandlePRESETZSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);
    void HandleTransInfoMethod (void);
    void WriteINTAPBMethod (void);
    void ClearINTAPBMethod (void);
    void EnableReset (const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const bool add_period, const double time_point);
    //Virtual functions of APBAgentController_UIMPL class
    bool CheckClockPeriod (void);
    void ControlIntr (void);
    void TransferTransInfo (TlmBasicPayload &trans);
    bool GetResetStatus (void);
};
#endif //__APB_UIMPL_H__
// vim600: set foldmethod=marker :
