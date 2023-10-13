// ---------------------------------------------------------------------
// $Id$
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
#ifndef __DNFP010_E2_H__
#define __DNFP010_E2_H__

#include "BusSlaveBase.h"
#include "DNFP010_E2_AgentController.h"

class DNFP010_E2_Sub;

class DNFP010_E2: public sc_module,
            	  public DNFP010_E2_AgentController,
                  public BusSlaveBase<32,1>,
                  public BusSlaveFuncIf
{

private:
    enum eResetActiveLevel {    // Define active level of reset signal
        emResetActive = 0,
        emResetSignalNum = 2
    };
    DNFP010_E2_Sub  **mDNFP010_E2_Sub;  // DNFP010_E2 subblock instance

    /// Declare variables

    sc_dt::uint64 mdnfp01tsmpclkFreq;            //Frequency value of clock
    sc_dt::uint64 mdnfp01tsmpclkOrgFreq;         //Original frequency value of clock
    std::string mdnfp01tsmpclkFreqUnit;          //Frequency unit of clock
    sc_dt::uint64 mPCLKPeriod;          //Period value of clock
    sc_dt::uint64 mPCLKFreq;            //Frequency value of clock
    sc_dt::uint64 mPCLKOrgFreq;         //Original frequency value of clock
    std::string mPCLKFreqUnit;          //Frequency unit of clock

    sc_dt::uint64 mTimeResolutionValue;    //Time resolution value of simulation

    uint8_t mChNum;


    bool mResetCurVal[emResetSignalNum];        //Store current value of reset signals
    bool mResetPreVal[emResetSignalNum];        //Store previous value of reset signals
    bool mIsResetHardActive[emResetSignalNum];  //Reset operation status of reset signals
    bool mIsResetCmdReceive[emResetSignalNum];  //Receive status of AssertReset command of reset signals
    bool mIsResetCmdActive[emResetSignalNum];   //Active status of AssertReset command of reset signals
    double mResetCmdPeriod[emResetSignalNum];   //Period of AssertReset command of reset signals


    /// Declare events
    sc_event mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over
    sc_event mClkZeroEvent[2];                          //Notify clock is zero


public:
    /// Declare signals
    // Input ports
    sc_in<sc_dt::uint64> dnfp01tsmpclk;
    sc_in<sc_dt::uint64> PCLK;
    sc_in<bool> dnfp01tsmpresn;
    sc_in<bool> PRESETn;

    sc_in<bool>  **DNFIN;
    sc_out<bool> **DNFFSOUT;
    sc_out<bool> **DNFFAOUT;

    /// Socket declaration
    TlmTargetSocket<32> *ts;            // For DNFP010_E2's registers

    SC_HAS_PROCESS (DNFP010_E2);
    DNFP010_E2 ( sc_module_name name,
    		     uint8_t	 chNum,
			     unsigned int rLatency,
			     unsigned int wLatency);
    ~DNFP010_E2 (void);

    /// PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const unsigned int ch_id, const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const unsigned int ch_id, const std::string reg_name);
    void WriteRegister (const unsigned int ch_id, const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const unsigned int ch_id, const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);

/// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug=false);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug=false);

private:
    /// Declare methods
    void Handlednfp01tsmpclkSignalMethod (void);
    void HandlePCLKSignalMethod (void);

    void HandlePRESETnSignalMethod (void);
    void Handlednfp01tsmpresnSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    /// Internal functions
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);


    void CancelEvents (void);
    void RegisterHandler (const unsigned int ch_id, const std::vector<std::string> &args);

    /// Virtual functions of DNFP010_E2_AgentController class
    bool GetResetStatus (const std::string reset_name);
    bool CheckClockPeriod (const std::string clock_name);
    double CalculateNextClockEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time);

};
#endif
