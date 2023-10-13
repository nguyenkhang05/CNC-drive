// ---------------------------------------------------------------------
// $Id: ECC_DATAFLASH.h,v 1.4 2015/08/06 11:11:14 huepham Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __ECC_DATAFLASH_H__
#define __ECC_DATAFLASH_H__
#include "BusSlaveBase.h"
#include "ECC_DATAFLASH_AgentController.h"

class ECC_DATAFLASH_Func;

class ECC_DATAFLASH: public sc_module,
                    public ECC_DATAFLASH_AgentController,
                    public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {//Define active level of reset signal
        emResetActive = 0,
        emResetInactive = 1
    };
    enum eOutputPort {
        emECCSED = 0,
        emECCDED = 1,
        emECCOVF = 2,
        emEccDis = 3,
        emSecDis = 4
    };

    //Declare variables
    ECC_DATAFLASH_Func *mECC_DATAFLASH_Func;//Instance of ECC_DATAFLASH_Func class

    sc_dt::uint64 mPclkPeriod;//Period value of Pclk clock
    sc_dt::uint64 mPclkFreq;//Frequency value of Pclk clock
    sc_dt::uint64 mPclkOrgFreq;//Orginal frequency value of Pclk clock
    std::string mPclkFreqUnit;//Frequency unit of Pclk clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation
    bool mIsStartSimulation;
    bool mIsAddressCaptureEnable;
    unsigned int mPawidth;
    bool mIsErrAddrSet;
    unsigned int mErrorAddr;
    bool mEccsedVal;
    bool mEccdedVal;
    bool mEccovfVal;
    bool mEccdisableVal;
    bool mSecdisableVal;

    bool mResetSignalCurrentValue;//Store current value of Reset signal
    bool mResetSignalPreviousValue;//Store previous value of Reset signal
    bool mIsResetHardActive;//Reset operation status of Reset signal
    bool mIsResetCmdReceive;//Receive status of AssertReset command of Reset signal
    bool mIsResetCmdActive;//Active status of AssertReset command of Reset signal
    double mResetCmdPeriod;//Period of AssertReset command of Reset signal

    sc_event mWriteEccsedEvent;
    sc_event mClearEccsedEvent;
    sc_event mWriteEccdedEvent;
    sc_event mClearEccdedEvent;
    sc_event mWriteEccovfEvent;
    sc_event mClearEccovfEvent;
    sc_event mWriteEccdisableEvent;
    sc_event mWriteSecdisableEvent;
    sc_event mResetHardEvent;//Call HandleResetHardMethod() when Reset is active
    sc_event mResetCmdEvent;//Call HandleResetCmdMethod() when AssertReset of Reset is called
    sc_event mResetCmdCancelEvent;//Call CancelResetCmdMethod() when AssertReset of Reset is over

public:
    //Declare input/output ports
    sc_in<bool> PRESETN;
    sc_in<sc_dt::uint64> PCLK;
    sc_out<bool> eccsed;
    sc_out<bool> eccded;
    sc_out<bool> eccovf;
    sc_out<bool> eccdisable;
    sc_out<bool> secdisable;

    //Declare target sockets
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (ECC_DATAFLASH);
    ECC_DATAFLASH (sc_module_name name, 
               unsigned int rLatency,
               unsigned int wLatency,
               const bool is_addr_cap = true,
               unsigned int addr_pawidth = 21,
               std::string ECC_DATAFLASH_VERSION = "G3M_V30");
    ~ECC_DATAFLASH (void);
    //Declare public members
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void SelectSEDIEInitVal (const std::string value);
    void IssueError (const unsigned int error_type);
    void SetErrorAddr (const int error_addr);
    void DumpStatusInfo (void);
    void AssertReset (const double start_time, const double period);
    void SetCLKFreq (const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (void);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
private:
    std::string mECCDATAFLASHVersion;// ECC_DATAFLASH version (G3M_V30, G3KH)
    void WriteEccsedMethod(void);
    void ClearEccsedMethod(void);
    void WriteEccdedMethod(void);
    void ClearEccdedMethod(void);
    void WriteEccovfMethod(void);
    void ClearEccovfMethod(void);
    void WriteEccdisableMethod(void);
    void WriteSecdisableMethod(void);

    void HandlePclkSignalMethod (void);
    void HandleResetSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);
    void EnableReset (const bool is_active);
    void Initialize(void);
    void StopWritingToPort(void);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const double time_point);
    bool GetResetStatus (void);
    void start_of_simulation(void);
    //Virtual functions of ECC_DATAFLASH_AgentController class
    bool CheckClock(void);
    void ErrorNotification(const unsigned int port_id);
    void OverflowNotification(void);
    void ECCStatusNotification(const unsigned int port_id, const bool output_value);
};
#endif //__ECC_DATAFLASH_H__
// vim600: set foldmethod=marker :
