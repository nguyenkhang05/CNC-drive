// ---------------------------------------------------------------------
// $Id: TSN.h,v 1.1 2014/12/10 01:14:38 binhnguyen Exp $
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
#ifndef __TSN_H__
#define __TSN_H__
#include "BusSlaveBase.h"
#include "TSN_AgentController.h"

class TSN_Func;

class TSN: public sc_module,
            public TSN_AgentController,
            public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {    //Define active level of reset signal
        emResetActive = 0,
        emResetSignalNum = 2
    };

    TSN_Func *mTSN_Func;            //TSN_Func class instance

    /// Declare variables
    sc_dt::uint64 mPCLKPeriod;          //Period value of PCLK clock
    sc_dt::uint64 mPCLKFreq;            //Frequency value of PCLK clock
    sc_dt::uint64 mPCLKOrgFreq;         //Orginal frequency value of PCLK clock
    std::string mPCLKFreqUnit;          //Frequency unit of PCLK clock
    sc_dt::uint64 mADCLKPeriod;         //Period value of ADCLK clock
    sc_dt::uint64 mADCLKFreq;           //Frequency value of ADCLK clock
    sc_dt::uint64 mADCLKOrgFreq;        //Orginal frequency value of ADCLK clock
    std::string mADCLKFreqUnit;         //Frequency unit of ADCLK clock

    sc_dt::uint64 mTimeResolutionValue;     //Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;       //Time resolution unit of simulation

    bool mResetCurVal[emResetSignalNum];        //Store current value of presetz or ADRESZ
    bool mResetPreVal[emResetSignalNum];        //Store previous value of presetz or ADRESZ
    bool mIsResetHardActive[emResetSignalNum];  //Reset operation status of presetz or ADRESZ signal
    bool mIsResetCmdReceive[emResetSignalNum];  //Receive status of AssertReset command of presetz or ADRESZ
    bool mIsResetCmdActive[emResetSignalNum];   //Active status of AssertReset command of presetz or ADRESZ
    double mResetCmdPeriod[emResetSignalNum];   //Period of AssertReset command of presetz or ADRESZ

    double mTempValue;              // Temperature value set by PythonIF
    unsigned int mErrMaskCount;     // Wait time for TSN to receive conversion error signal from ADC (number of ADCLK cycle)
    unsigned int mStartupCount;     // Start-up time for TSN to change to ready operation after enabled (number of ADCLK cycle)
    bool mDumpInterrupt;            // Enable/Disable dumping the interrupt info
    
    bool mTSNERR;           // Value of TSNERR port
    double mTSOUT;          // Value of TSOUT port
    eState mState;          // Current state of TSN model
    bool mEnableSelfDiag;   // Enable/Disable status of Self-diagnosis function

    /// Declare events
    sc_event mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when presetz or sys_reset is active
    sc_event mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over

    sc_event mUpdateTSOUTOutputEvent;       // Call UpdateTSOUTOutputMethod() to update the value of TSOUT port
    sc_event mWriteTSOUTOutputEvent;        // Call WriteTSOUTOutputMethod() to control TSOUT port
    sc_event mWriteTSNERRInterruptEvent;    // Call WriteTSNERRInterruptMethod() to control TSNERR port
    sc_event mAssertTSNERREvent;            // Call AssertTSNERRMethod() to assert TSNERR interrupt
    sc_event mDeassertTSNERREvent;          // Call DeassertTSNERRMethod() to de-assert TSNERR interrupt
    sc_event mFinishInitializeEvent;        // Call FinishInitializeMethod() to change TSN to READY state
    sc_event mFinishDischargeEvent;         // Call FinishDischargeMethod() to change TSN to CONVERSION state
    sc_event mFinishConvDoneEvent;          // Call FinishConvDoneMethod() to change TSN to READY state
    sc_event mHandleCHSLCChangeEvent;       // Call HandleCHSLCChangeMethod() to process the change of CHSLC input port
    sc_event mHandleINT_ADEChangeEvent;     // Call HandleINT_ADEChangeMethod() to process the change of INT_ADE input port

public:
    /// Declare input signals
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> ADCLK;
    sc_in<bool> presetz;
    sc_in<bool> ADRESZ;
    sc_in<bool> CHSLC;
    sc_in<bool> INT_ADE;

    /// Declare output signals
    sc_out<double> TSOUT;
    sc_out<bool> TSNERR;

    /// Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (TSN);
    TSN (sc_module_name name,
          unsigned int rLatency,
          unsigned int wLatency);
    ~TSN (void);
    
    /// PythonIF
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
    void SetTemperature (const double value);
    void SetErrmaskCount (const unsigned int value);
    void SetStartupCount (const unsigned int value);
    void DumpStatusInfo (void);

private:
    /// Declare methods
    void HandlePCLKSignalMethod (void);
    void HandleADCLKSignalMethod (void);
    
    void HandlePresetzSignalMethod (void);
    void HandleADRESZSignalMethod (void);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);
    void HandleResetHardMethod (const unsigned int reset_id);

    void HandleCHSLCInputMethod (void);
    void HandleCHSLCChangeMethod (void);
    void HandleINT_ADEInputMethod (void);
    void HandleINT_ADEChangeMethod (void);

    void UpdateTSOUTOutputMethod (void);
    void WriteTSOUTOutputMethod (void);
    void WriteTSNERRInterruptMethod (void);
    void AssertTSNERRMethod (void);
    void DeassertTSNERRMethod (void);
    void FinishInitializeMethod (void);
    void FinishDischargeMethod (void);
    void FinishConvDoneMethod (void);

    /// Internal functions
    void CancelEvents (void);

    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const std::string clock_name, const double time_point);

    /// Virtual functions of TSN_AgentController class
    bool GetResetStatus (const std::string reset_name);
    bool CheckClockPeriod (const std::string clock_name);
    eState GetState (void);
    void EnableSetting (const bool is_enable);
    void SelfDiagSetting (const bool is_enable);
};
#endif
