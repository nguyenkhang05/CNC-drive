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
#ifndef __DNFP020_E2_H__
#define __DNFP020_E2_H__

#include "BusSlaveBase.h"
#include "DNFP020_E2_AgentController.h"

class DNFP020_E2_Sub;
class DNFP020_E2_Func;

class DNFP020_E2: public sc_module,
            public DNFP020_E2_AgentController,
            public BusSlaveBase<32,1>
{

private:
    enum eResetActiveLevel {    // Define active level of reset signal
        emResetActive = 0,
    };
    DNFP020_E2_Func *mDNFP020_E2_Func;                // ATU5_TIMERF_Func class instance
    DNFP020_E2_Sub  **mDNFP020_E2_Sub;  // DNFP020_E2 subblock instance

    /// Declare variables
    sc_dt::uint64 mPCLKFreq;            //Frequency value of clock
    sc_dt::uint64 mPCLKOrgFreq;         //Original frequency value of clock
    std::string mPCLKFreqUnit;          //Frequency unit of clock

    sc_dt::uint64 mTimeResolutionValue;    //Time resolution value of simulation

    bool mResetCurVal;        //Store current value of reset signals
    bool mResetPreVal;        //Store previous value of reset signals
    bool mIsResetHardActive;  //Reset operation status of reset signals
    bool mIsResetCmdReceive;  //Receive status of AssertReset command of reset signals
    bool mIsResetCmdActive;   //Active status of AssertReset command of reset signals
    double mResetCmdPeriod;   //Period of AssertReset command of reset signals

    uint8_t mChanNum;         //Number of channel


    /// Declare events
    sc_event mResetHardEvent;         //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent;          //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent;    //Call CancelResetCmdMethod when AssertReset is over
    sc_event mClkZeroEvent;                          //Notify clock is zero


public:
    /// Declare signals
    // Input ports
    sc_in<sc_dt::uint64> PCLK;
    sc_in<bool> PRESETn;

    sc_in<bool>  **DNFEDGEIN;

    /// Socket declaration
    TlmTargetSocket<32> *ts;            // For DNFP020_E2's registers

    SC_HAS_PROCESS (DNFP020_E2);
    DNFP020_E2 (sc_module_name name,
		  uint8_t chanNum,
          unsigned int rLatency,
          unsigned int wLatency);
    ~DNFP020_E2 (void);

    /// PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const bool is_com_reg, const unsigned int ch_id, const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const bool is_com_reg, const unsigned int ch_id, const std::string reg_name);
    void WriteRegister (const bool is_com_reg, const unsigned int ch_id, const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const bool is_com_reg, const unsigned int ch_id, const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);

private:
    /// Declare methods
    void HandlePCLKSignalMethod (void);

    void HandlePRESETnSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);

    /// Internal functions
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);


    void CancelEvents (void);
    void RegisterHandler (const bool is_com_reg, const unsigned int ch_id, const std::vector<std::string> &args);

    /// Virtual functions of DNFP020_E2_AgentController class
    bool GetResetStatus (const std::string reset_name);
    bool CheckClockPeriod (const std::string clock_name);
    double CalculateNextClockEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time);
    void ReadSub (unsigned int ch_id, unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void WriteSub (unsigned int ch_id, unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    unsigned int GetSLST(void);
    unsigned int GetPRS(void);
    bool GetSubNFEN(void);
};
#endif
