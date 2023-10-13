// ---------------------------------------------------------------------
// $Id: ATU5_AgentController.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_AGENTCONTROLLER_H__
#define __ATU5_AGENTCONTROLLER_H__
//#include <systemc.h>
//#include <iostream>
//#include <cstring>
//#include <iterator>
//#include <map>
//#include <stdarg.h>
#include "ATU5_Base.h"

//class ATU5_TIMERA;
//class ATU5_TIMERB;
//class ATU5_TIMERC;
//class ATU5_TIMERD;
//class ATU5_TIMERE;
//class ATU5_TIMERF;
//class ATU5_TIMERG;
//class ATU5_Base;

class ATU5_AgentController: public ATU5_Base
{
#include "ATU5_cmdif.h"
public:
    enum eTimerChNum {    // Define total channel number
        emTimerA_SUBnum = 8
      , emTimerC_SUBnum = 11
      , emTimerC_SUB_CHnum = 4
      , emTimerD_SUBnum = 9
      , emTimerD_SUB_CHnum = 4
      , emTimerE_SUBnum = 9
      , emTimerE_SUB_CHnum = 4
      , emTimerF_SUBnum = 16
      , emTimerNum = 7
      , emClkBusPSCNum = 4
    };
    enum eTIMER_ID {    // Define timer ID
        emTimerA
      , emTimerB
      , emTimerC
      , emTimerD
      , emTimerE
      , emTimerF
      , emTimerG
    };
    enum eTimerPortNum {
        emTimerA_TILOnum = 8
      , emTIAnum = 8
      , emTimerEF_POEnum = 6
      , emWrapper_DMAnum = 4
    };
    enum eClkEdgeType {
        emNoneEdge
      , emRisingEdge
      , emFallingEdge
      , emBothEdge
    };
    enum eTCLKtype {
        emTCLKA
      , emTCLKB
      , emTCLKnum
    };
    enum eATUENR {
        emPSCE = 0
    };
    enum eINTSELtype {
        emTRGSELDMA00
      , emTRGSELDMA01
      , emTRGSELDMA10
      , emTRGSELDMA11
      , emTRGSELAD
    };

    virtual unsigned int GetClkBus5Setting() { return 0; }

    /// PythonIF
    virtual void AssertReset (const std::string reset_name, const double start_time, const double period) {;}
    virtual void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit) {;}
    virtual void GetCLKFreq (const std::string clock_name) {;}
    virtual void ForceRegister (const std::string module_id, const std::string block_id, const std::string reg_name, const unsigned int reg_value) {;}
    virtual void ReleaseRegister (const std::string module_id, const std::string block_id, const std::string reg_name) {;}
    virtual void WriteRegister (const std::string module_id, const std::string block_id, const std::string reg_name, const unsigned int reg_value) {;}
    virtual void ReadRegister (const std::string module_id, const std::string block_id, const std::string reg_name) {;}
    virtual void ListRegister (void) {;}
    virtual void Help (const std::string type) {;}
    virtual void SetMessageLevel (const std::string msg_lv) {;}
    virtual void DumpRegisterRW (const std::string is_enable){;}
    virtual void DumpInterrupt (const std::string is_enable) {;}

    /// Declare methods
    virtual void HandleCLK_HSBSignalMethod (void) {;}
    virtual void HandleCLK_LSBSignalMethod (void) {;}

    virtual void HandlePresetz_bifSignalMethod (void) {;}
    virtual void HandlePresetzSignalMethod (void) {;}
    virtual void HandleResetHardMethod (const unsigned int reset_id) {;}
    virtual void HandleResetCmdMethod (const unsigned int reset_id) {;}
    virtual void CancelResetCmdMethod (const unsigned int reset_id) {;}

    virtual void StartPrescalerMethod (unsigned int ch_id) {;}
    virtual void StopPrescalerMethod (unsigned int ch_id) {;}
    virtual void WriteClockBusMethod (unsigned int ch_id) {;}
    virtual void WriteNoiseCancelClkMethod (void) {;}
    
    virtual void HandleTCLKASignalMethod (void) {;}
    virtual void HandleTCLKBSignalMethod (void) {;}
    virtual void TCLKUpdateMethod (unsigned int type) {;}
    
    virtual void EnableTimerABMethod (void) {;}
    
    virtual void HandleAGCK1SignalMethod (void) {;}
    virtual void HandleAGCK1AssertMethod (void) {;}
    virtual void HandleAGCK1DeassertMethod (void) {;}
    
    virtual void HandleDMATrgSelectChangeMethod (void) {;}
    virtual void UpdateDMATrgSelectMethod (void) {;}
    virtual void HandleDMA0TriggerInputMethod (void) {;}
    virtual void HandleDMA1TriggerInputMethod (void) {;}
    virtual void Writeat_saradtrgMethod(void) {;}
    virtual void WriteTrgDMAReqMethod (unsigned int id) {;}
    virtual void HandleTRGSELDMA0regChangeMethod (unsigned int id) {;}
    virtual void HandleTRGSELDMA1regChangeMethod (unsigned int id) {;}
    virtual void HandleSARADTriggerInputMethod (void) {;}
    
    virtual void HandleAGCKMSignalMethod (void) {;}
    
    /// Internal functions
    virtual void EnableReset (const std::string reset_name, const bool is_active) {;}
    virtual void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in) {;}
    virtual void SeparateString (std::vector<std::string> &vtr, const std::string msg) {;}
    virtual void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit) {;}
    virtual double CalculateNextClockEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time) { return 0;}

    virtual void CancelEvents (void) {;}
    virtual void Initialize (void) {;}
    virtual void RegisterHandlerWrapper (const std::string module_id, const std::string block_id, const std::vector<std::string> cmd) {;}
    virtual unsigned int GetNoiseCancelMode (const std::string timer_name) { return 0;}
    virtual void CLK_LSBUpdate (void) {;}
    
    /// Virtual functions of ATU5_AgentController class
    virtual bool GetResetStatus (const std::string reset_name) {return true;}
    virtual bool CheckClockPeriod (const std::string clock_name) { return true;}
    virtual void EnablePrescaler (const bool is_enable) {;}
    virtual void PrescalerSetupChange (const unsigned int ch_id) {;}
    virtual void ClkBusSetupChange (void) {;}
    virtual void NCClkSetupChange (void) {;}
    virtual void CheckClkBusSel (const unsigned int timer_id) {;}
    virtual void NotifyEvent (const unsigned int event, const unsigned int channel) {;}

    ATU5_AgentController(void) {;}
    virtual ~ATU5_AgentController(void) {;}
};
#endif
