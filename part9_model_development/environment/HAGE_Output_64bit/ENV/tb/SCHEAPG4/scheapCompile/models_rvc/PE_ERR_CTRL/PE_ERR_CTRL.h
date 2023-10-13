// ---------------------------------------------------------------------
// $Id: PE_ERR_CTRL.h,v 1.3 2014/03/18 01:34:44 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __PE_ERR_CTRL_H__
#define __PE_ERR_CTRL_H__
#include "BusSlaveBase.h"
#include "PE_ERR_CTRL_AgentController.h"
#include "ERR_INFO.h"

class ERRCTRL;

class PE_ERR_CTRL:  public sc_module,
                    public PE_ERR_CTRL_AgentController,
                    public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {//Define active level of reset signal
        emPresetzActive = 0,
        emPresetzInactive = 1,
        emLvcresetActive = 1,
        emLvcresetInactive = 0
    };
    //Declare variables
    ERRCTRL *mERRCTRL;//Instance of ERRCTRL class

    sc_dt::uint64 mPclkPeriod;//Period value of Pclk clock
    sc_dt::uint64 mLvcclkPeriod;//Period value of Lvcclk clock
    sc_dt::uint64 mPclkFreq;//Frequency value of Pclk clock
    sc_dt::uint64 mLvcclkFreq;//Frequency value of Lvcclk clock
    sc_dt::uint64 mPclkOrgFreq;//Orginal frequency value of Pclk clock
    sc_dt::uint64 mLvcclkOrgFreq;//Orginal frequency value of Lvcclk clock
    std::string mPclkFreqUnit;//Frequency unit of Pclk clock
    std::string mLvcclkFreqUnit;//Frequency unit of Lvcclk clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation
    bool mDumpInterrupt;//Enable or disable dumping the interrupt
    bool mIntrValue;//The value of interrupt port

    bool mPresetzSignalCurrentValue;//Store current value of Presetz signal
    bool mPresetzSignalPreviousValue;//Store previous value of Presetz signal
    bool mIsPresetzHardActive;//Reset operation status of Presetz signal
    bool mIsPresetzCmdReceive;//Receive status of AssertReset command of Presetz signal
    bool mIsPresetzCmdActive;//Active status of AssertReset command of Presetz signal
    double mPresetzCmdPeriod;//Period of AssertReset command of Presetz signal

    bool mLvcresetSignalCurrentValue;//Store current value of Lvcreset signal
    bool mLvcresetSignalPreviousValue;//Store previous value of Lvcreset signal
    bool mIsLvcresetHardActive;//Reset operation status of Lvcreset signal
    bool mIsLvcresetCmdReceive;//Receive status of AssertReset command of Lvcreset signal
    bool mIsLvcresetCmdActive;//Active status of AssertReset command of Lvcreset signal
    double mLvcresetCmdPeriod;//Period of AssertReset command of Lvcreset signal

    Cerrinfo mCurrentErrInfo;
    
    sc_event mPresetzHardEvent;//Call HandlePresetzHardMethod() when Presetz is active
    sc_event mPresetzCmdEvent;//Call HandlePresetzCmdMethod() when AssertReset of Presetz is called
    sc_event mPresetzCmdCancelEvent;//Call CancelPresetzCmdMethod() when AssertReset of Presetz is over

    sc_event mLvcresetHardEvent;//Call HandleLvcresetHardMethod() when Lvcreset is active
    sc_event mLvcresetCmdEvent;//Call HandleLvcresetCmdMethod() when AssertReset of Lvcreset is called
    sc_event mLvcresetCmdCancelEvent;//Call CancelLvcresetCmdMethod() when AssertReset of Lvcreset is over

    sc_event mRecordErrInfoEvent;//Call HandleTransInfoMethod() to handle transaction information after synchonized with lvcclk
    sc_event mWriteIntrEvent;//Call WriteINTAHBMethod() to write the value to interrupt port
    sc_event mClearIntrEvent;//Call ClearINTAHBMethod() to clear interrupt port after 1 Pclk
public:
    //Declare input/output ports
    sc_in<bool> presetz;
    sc_in<bool> lvcreset;
    sc_in<sc_dt::uint64> pclk;
    sc_in<sc_dt::uint64> lvcclk;
    sc_in<Cerrinfo> ERR_INFO;
    sc_out<bool> INTAPB;
    //Declare target sockets
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (PE_ERR_CTRL);
    PE_ERR_CTRL (sc_module_name name, 
               unsigned int rLatency,
               unsigned int wLatency);
    ~PE_ERR_CTRL (void);
    //Declare public members
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
    void HandlePclkSignalMethod (void);
    void HandleLvcclkSignalMethod (void);
    void HandlePresetzSignalMethod (void);
    void HandleLvcresetSignalMethod (void);
    void HandlePresetzHardMethod (void);
    void HandleLvcresetHardMethod (void);
    void HandlePresetzCmdMethod (void);
    void HandleLvcresetCmdMethod (void);
    void CancelPresetzCmdMethod (void);
    void CancelLvcresetCmdMethod (void);
    void HandleERR_INFOMethod(void);
    void RecordErrInfoMethod (void);
    void WriteINTAPBMethod (void);
    void ClearINTAPBMethod (void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const std::string clock_name, const double time_point);

    //Virtual functions of AHBAgentController_UIMPL class
    bool CheckClock(const std::string clock_name);
    void ControlIntr (void);
    bool GetResetStatus (const std::string reset_name);
};
#endif //__PE_ERR_CTRL_H__
// vim600: set foldmethod=marker :
