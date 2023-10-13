// ---------------------------------------------------------------------
// $Id: ATU5_Base.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_Base_H__
#define __ATU5_Base_H__

#include "BusSlaveBase.h"
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h> 

#  ifdef IEEE_1666_SYSTEMC
#   ifndef SC_SIGNAL
#       define SC_SIGNAL(T) sc_signal< T , SC_UNCHECKED_WRITERS>
#   endif
#  else
#   ifndef SC_SIGNAL
#       define SC_SIGNAL(T) sc_signal< T >
#   endif
#  endif

class ATU5_TIMERA;
class ATU5_TIMERB;
class ATU5_TIMERC;
class ATU5_TIMERD;
class ATU5_TIMERE;
class ATU5_TIMERF;
class ATU5_TIMERG;

class ATU5_Base 
{
//friend class ATU5_Timer;
//friend class ATU5_NoiseCanceler;
friend class ATU5_TIMERA;
friend class ATU5_TIMERA_Func;
friend class ATU5_TIMERB;
friend class ATU5_TIMERC;
friend class ATU5_TIMERC_SUB;
friend class ATU5_TIMERD;
friend class ATU5_TIMERD_SUB;
friend class ATU5_TIMERE;
friend class ATU5_TIMERE_SUB;
friend class ATU5_TIMERF;
friend class ATU5_TIMERF_Func;
friend class ATU5_TIMERF_Sub;
friend class ATU5_TIMERF_Sub_Func;
friend class ATU5_TIMERG;

public:
    enum eResetActiveLevel {    // Define active level of reset signal
        emResetActive = 0,
        emResetSignalNum = 2
    };
    enum eDefine {
        emCLK_BUSnum = 6,
        emTIAnum = 8
    };
    double mClkBusStartTime[emCLK_BUSnum];
    sc_dt::uint64 mCLK_LSBPeriod;          //Period value of clock
    sc_event mClkZeroEvent[2];                          //Notify clock is zero
    sc_event mCLKLSBUpdatedEvent;
    double mTimeCLKChangePoint[2];              //The time clock is changed its value (0: CLK_LSB, 1: CLK_HSB)
    bool mDumpInterrupt;            // Enable/Disable dumping the interrupt info
    double mAGCK1StartTime;

    sc_time_unit mTimeResolutionUnit;      //Time resolution unit of simulation
    bool mIsResetHardActive[emResetSignalNum];  //Reset operation status of reset signals
    bool mIsResetCmdReceive[emResetSignalNum];  //Receive status of AssertReset command of reset signals
    virtual unsigned int GetClkBus5Setting() = 0;

    /// PythonIF
    virtual void AssertReset (const std::string reset_name, const double start_time, const double period) = 0;
    virtual void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit) = 0;
    virtual void GetCLKFreq (const std::string clock_name) = 0;
    virtual void ForceRegister (const std::string module_id, const std::string block_id, const std::string reg_name, const unsigned int reg_value) = 0;
    virtual void ReleaseRegister (const std::string module_id, const std::string block_id, const std::string reg_name) = 0;
    virtual void WriteRegister (const std::string module_id, const std::string block_id, const std::string reg_name, const unsigned int reg_value) = 0;
    virtual void ReadRegister (const std::string module_id, const std::string block_id, const std::string reg_name) = 0;
    virtual void ListRegister (void) = 0;
    virtual void Help (const std::string type) = 0;
    virtual void SetMessageLevel (const std::string msg_lv) = 0;
    virtual void DumpRegisterRW (const std::string is_enable) = 0;
    virtual void DumpInterrupt (const std::string is_enable) = 0;

    /// Declare methods
    virtual void HandleCLK_HSBSignalMethod (void) = 0;
    virtual void HandleCLK_LSBSignalMethod (void) = 0;

    virtual void HandlePresetz_bifSignalMethod (void) = 0;
    virtual void HandlePresetzSignalMethod (void) = 0;
    virtual void HandleResetHardMethod (const unsigned int reset_id) = 0;
    virtual void HandleResetCmdMethod (const unsigned int reset_id) = 0;
    virtual void CancelResetCmdMethod (const unsigned int reset_id) = 0;

    virtual void StartPrescalerMethod (unsigned int ch_id) = 0;
    virtual void StopPrescalerMethod (unsigned int ch_id) = 0;
    virtual void WriteClockBusMethod (unsigned int ch_id) = 0;
    virtual void WriteNoiseCancelClkMethod (void) = 0;
    
    virtual void HandleTCLKASignalMethod (void) = 0;
    virtual void HandleTCLKBSignalMethod (void) = 0;
    virtual void TCLKUpdateMethod (unsigned int type) = 0;
    
    virtual void EnableTimerABMethod (void) = 0;
    
    virtual void HandleAGCK1SignalMethod (void) = 0;
    virtual void HandleAGCK1AssertMethod (void) = 0;
    virtual void HandleAGCK1DeassertMethod (void) = 0;
    
    virtual void HandleDMATrgSelectChangeMethod (void) = 0;
    virtual void UpdateDMATrgSelectMethod (void) = 0;
    virtual void HandleDMA0TriggerInputMethod (void) = 0;
    virtual void HandleDMA1TriggerInputMethod (void) = 0;
    virtual void Writeat_saradtrgMethod(void) = 0;
    virtual void WriteTrgDMAReqMethod (unsigned int id) = 0;
    virtual void HandleTRGSELDMA0regChangeMethod (unsigned int id) = 0;
    virtual void HandleTRGSELDMA1regChangeMethod (unsigned int id) = 0;
    virtual void HandleSARADTriggerInputMethod (void) = 0;
    
    virtual void HandleAGCKMSignalMethod (void) = 0;
    
    /// Internal functions
    virtual void EnableReset (const std::string reset_name, const bool is_active) = 0;
    virtual void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in) = 0;
    virtual void SeparateString (std::vector<std::string> &vtr, const std::string msg) = 0; 
    virtual void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit) = 0;
    virtual double CalculateNextClockEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time) = 0;

    virtual void CancelEvents (void) = 0;
    virtual void Initialize (void) = 0;
    virtual void RegisterHandlerWrapper (const std::string module_id, const std::string block_id, const std::vector<std::string> cmd) = 0;
    virtual unsigned int GetNoiseCancelMode (const std::string timer_name) = 0;
    virtual void CLK_LSBUpdate (void) = 0;
    
    /// Virtual functions of ATU5_AgentController class
    virtual bool GetResetStatus (const std::string reset_name) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
    virtual void EnablePrescaler (const bool is_enable) = 0;
    virtual void PrescalerSetupChange (const unsigned int ch_id) = 0;
    virtual void ClkBusSetupChange (void) = 0;
    virtual void NCClkSetupChange (void) = 0;
    virtual void CheckClkBusSel (const unsigned int timer_id) = 0;
    virtual void NotifyEvent (const unsigned int event, const unsigned int channel) = 0;
    ATU5_TIMERA *mATU5_TimerA;               // Timer A instance
    ATU5_TIMERB *mATU5_TimerB;               // Timer B instance
    ATU5_TIMERC *mATU5_TimerC;               // Timer C instance
    ATU5_TIMERD *mATU5_TimerD;               // Timer D instance
    ATU5_TIMERE *mATU5_TimerE;               // Timer E instance
    ATU5_TIMERF *mATU5_TimerF;               // Timer F instance
    ATU5_TIMERG *mATU5_TimerG;               // Timer G instance

    ATU5_Base(void){;}
    virtual ~ATU5_Base(void) {;}
};
#endif
