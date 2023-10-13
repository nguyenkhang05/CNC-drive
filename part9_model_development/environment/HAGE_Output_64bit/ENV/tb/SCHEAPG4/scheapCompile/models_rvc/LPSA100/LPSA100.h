// ---------------------------------------------------------------------
// $Id: LPSA100.h,v 1.10 2020/05/29 01:25:39 hadinhtran Exp $
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
#ifndef __LPSA100_H__
#define __LPSA100_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "LPSA100_AgentController.h"
#include "LPS_def.h"
#include "rvc_common_model.h"

class LPSA100_Func;

class LPSA100: public sc_module
          ,public LPSA100_AgentController
          ,public BusSlaveBase<32, 1>
          ,public rvc_common_model
{    
private:
    /// LPSA100_Func pointer
    LPSA100_Func        *mpLPSA100Func;

    /// Variables
    bool            mDumpInterrupt;                           // Dump interrupts information when interrupts are notify
    bool            mEnableRegisterMessage;                   // Enable/disable info/warning/error message of register IF
    bool            mIsPRESETnInit;                           // Is PRESET initialized
    bool            mIsResetHardActive;                       // Is reset by port
    bool            mIsResetCmdReceive;                       // Is receive cmd reset
    bool            mIsResetCmdActive;                        // Is reset by cmd
    double          mResetCmdPeriod;                          // Period of cmd reset

    sc_dt::uint64   mClkPeriod[emNumClk];                     // Period of clock
    sc_dt::uint64   mClkOrgFreq[emNumClk];                    // Previous frequency of clock
    sc_dt::uint64   mClkFreq[emNumClk];                       // Frequency of clock
    std::string     mClkFreqUnit[emNumClk];                   // Frequency unit of clock
    double          mStartClkTime[emNumClk];                  // Start time of clock

    sc_dt::uint64   mTimeResolutionValue;                     // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;                      // Time resolution unit of simulation

    bool            mAPO;                                     // Value to write to mAPO;     
    bool            mDPO;                                     // Value to write to mDPO;     
    unsigned char   mDPSEL;                                   // Value to write to DPSEL;
    bool            mSEQADTRG;                                // Value to write to SEQADTRG;
    bool            mINTCWEND;                                // Value to write to INTCWEND;
    bool            mWUTRG0;                                  // Value to write to WUTRG0;  
    bool            mWUTRG1;                                  // Value to write to WUTRG1;
    bool            mINTDPE;                                  // Value to write to INTDPE;  
    uint32_t        mDPIN;                                    // Value read from DPIN;
    unsigned char   mUle_lps;                                 // Value read from ule_lps;    
    unsigned int    mNUMDP;                                   // Number of times the port is read in digital input mode
    bool            mINTTAUJyIx[emNumINTTAUy][emNumINTTAUx];

    sc_dt::uint64   mfRH;                                     // Value of fRH: HS IntOSC frequency
    bool            mDigital_IsProcessing;                    // Digital input mode is processing or not
    bool            mDigital_IsEnabled;                       // Digital input mode is enabled or not
    bool            mDigital_IsStopped;                       // Digital input mode is stopped or not
    sc_dt::uint64   mDigital_SignalSourceStabilizationTime;   // Signal source stabilization time
    sc_dt::uint64   mDigital_IncreDPSELTime;                  // Time period of one time increment DPSEL output port
    sc_dt::uint64   mDigital_ResetDPSELTime;                  // Time period of reset DPSEL output port after finish read digital input port
    bool            mDigital_IsBreak;                         // Operaion is breaked because operation clock is zero or reset in digital input mode
    sc_dt::uint64   mDigital_WaitTime;                        // Waiting Time in digital mode
    sc_time_unit    mDigital_WaitTimeUnit;                    // Time resolution unit of simulation

    bool            mAnalog_IsEnabled;                        // Analog input mode is enabled or not
    bool            mAnalog_IsProcessing;                     // Analog input mode is processing or not
    bool            mAnalog_IsStopped;                        // Analog input mode is stopped or not
    bool            mAnalog_IsError;                          // Flag A/D conversion is occurred error or not
    sc_dt::uint64   mAnalog_StabilizationTime;                // Stabilization time in analog input mode
    sc_dt::uint64   mAnalog_WaitTime;                         // Waiting Time in analog mode
    sc_time_unit    mAnalog_WaitTimeUnit;                     // Time resolution unit of Waiting Time in analog mode
    bool            mAnalog_IsAssertINT;                      // Port INT_AD interrupt is asserted or not

    /// Events
    sc_event mResetCmdEvent;                                  // Event to reset by command 
    sc_event mResetCmdCancelEvent;                            // Event to cancel reset by command
    sc_event mResetHardEvent;
    
    sc_event mClkZeroEvent;                                   // Event to notify when operation clock is zero

    sc_event mWriteAPOEvent;                                  // Trigger WriteAPOMethod
    sc_event mNegateAPOEvent;                                 // Trigger NegateAPOMethod
    sc_event mWriteDPOEvent;                                  // Trigger WriteDPOMethod
    sc_event mNegateDPOEvent;                                 // Trigger NegateDPOMethod
    sc_event mWriteINTCWENDEvent;                             // Trigger WriteINTCWENDMethod
    sc_event mNegateINTCWENDEvent;                            // Trigger NegateINTCWENDMethod
    sc_event mWriteWUTRG0Event;                               // Trigger WriteWUTRG0Method
    sc_event mNegateWUTRG0Event;                              // Trigger NegateWUTRG0Method
    sc_event mWriteWUTRG1Event;                               // Trigger WriteWUTRG1Method
    sc_event mNegateWUTRG1Event;                              // Trigger NegateWUTRG1Method
    sc_event mWriteINTDPEEvent;                               // Trigger WriteINTDPEMethod
    sc_event mWriteSEQADTRGEvent;                             // Trigger WriteSEQADTRGMethod
    sc_event mNegateSEQADTRGEvent;                            // Trigger NegateSEQADTRGMethod
    sc_event mWriteDPSELEvent;                                // Trigger WriteDPSELMethod

    sc_event mProcessDigitalInputModeEvent;                   // Trigger ProcessDigitalInputModeThread
    sc_event mProcessAnalogInputModeEvent;                    // Trigger ProcessAnalogInputModeThread
    sc_event mContinueAnalogThreadEvent;                      // Trigger continue ProcessAnalogInputModeThread
    sc_event mEventFlagClearedEvent;                          // Trigger notified by NotifyEventFlagCleared function
    sc_event mADErrorClearedEvent;                            // Trigger MonitorULE_LPSMethod
    
    /// Methods
    void MonitorCLK_LSBMethod();
    void MonitorCLKA_LPSMethod();

    void MonitorPRESETnMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();

    void WriteAPOMethod();
    void NegateAPOMethod();
    void WriteDPOMethod();
    void NegateDPOMethod();
    void WriteINTCWENDMethod();
    void NegateINTCWENDMethod();
    void WriteWUTRG0Method();
    void NegateWUTRG0Method();
    void WriteWUTRG1Method();
    void NegateWUTRG1Method();
    void WriteINTDPEMethod();
    void WriteSEQADTRGMethod();
    void NegateSEQADTRGMethod();
    void WriteDPSELMethod();
    
    void MonitorStartTriggerMethod();
    void MonitorULE_LPSMethod();
    void MonitorINT_ADMethod();

    /// Threads
    void ProcessDigitalInputModeThread();
    void ProcessAnalogInputModeThread();

    /// Internal Functions
    void Initialize (void);
    void EnableReset (const bool is_active);
    void CancelEvents();

    bool CalculateTiming();
    void StoreDPIN();
    bool CheckDigitalInputPort();
    bool CheckStartLPSOperation();
    bool CheckStopLPSOperation();
    void ResetDigitalInputMode();
    void ResetAnalogInputMode();

    /// Overwrite firtual functions of LPSA100_AgentController
    bool CheckClockPeriod(const std::string clock_name);
    bool GetResetStatus();
    void NotifyEventFlagCleared();
    void EnableDigitalInputMode();
    void StopDigitalInputMode();
    void EnableAnalogInputMode();
    void StopAnalogInputMode();

public:
    SC_HAS_PROCESS (LPSA100);

    /// Constructor & Destructor
    LPSA100 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~LPSA100 ();

    /// Clock/ Reset
    sc_in<sc_dt::uint64>   CLK_LSB;                                  // Register access clock
    sc_in<sc_dt::uint64>   CLKA_LPS;                                 // Register access clock, Operating clock
    sc_in<bool>            PRESETn;                                  // APB reset

    /// Input/ Output ports
    sc_in<bool>            *DPIN[emNumDPIN];                         // Digital port input signal
    sc_in<bool>            *INTTAUJyIx[emNumINTTAUy][emNumINTTAUx];  // Start triggers
    sc_in<unsigned char>   ule_lps;                                  // ADC upper/lower limit error output signal
    sc_in<bool>            *INT_AD[emNumINT_AD];                     // A/D conversion end interrupt signal

    sc_out<bool>           APO;                                      // Port output signal for analog input
    sc_out<bool>           DPO;                                      // Port output signal for digital input
    sc_out<bool>           *DPSEL[emNumDPSEL];                       // External multiplexer select output signal for digital port

    /// Interrupt
    sc_out<bool>           INTCWEND;                                 // Port polling end interrupt
    sc_out<bool>           WUTRG0;                                   // LPSA100 wake-up factor output signal for digital input
    sc_out<bool>           WUTRG1;                                   // LPSA100 wake-up factor output signal for analog input
    sc_out<bool>           SEQADTRG;                                 // A/D conversion trigger
    sc_out<bool>           INTDPE;                                   // Digital port error interrupt (LPS)

    /// Socket declaration
    TlmTargetSocket<32>    *ts;                                      // APB target socket to access register

    /// PythonIF functions
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void SetDigitalWaitTime(const unsigned int time_value, const std::string unit);
    void SetAnalogWaitTime(const unsigned int time_value, const std::string unit);
    void DumpInterrupt (const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
};
#endif //__LPSA100_H__
