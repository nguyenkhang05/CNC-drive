// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1.h,v 1.3 2020/03/24 07:16:32 duongphan Exp $
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
#ifndef __QLAPBFICTLAV1_H__
#define __QLAPBFICTLAV1_H__

#include "BusSlaveBase.h"
#include "QLAPBFICTLAV1_AgentController.h"
#include "rvc_common_model.h"
#include "global.h"
class QLAPBFICTLAV1_Func;
class QLAPBFICTLAV1: public sc_module,
                    public QLAPBFICTLAV1_AgentController,
            public BusSlaveBase<BUS_WIDTH_APB,1>,
            public rvc_common_model
{
friend class QLAPBFICTLAV1_Sub;
friend class QLAPBFICTLAV1_Sub_Func;

private:
    uint32_t mQlapbfictlav1Channel;

    QLAPBFICTLAV1_Func *mQLAPBFICTLAV1_Func;
    /// Declare variables

    sc_dt::uint64 mfclasmpclkFreq;                //Frequency value of clock
    sc_dt::uint64 mfclasmpclkOrgFreq;            //Original frequency value of clock
    std::string mfclasmpclkFreqUnit;            //Frequency unit of clock
    sc_dt::uint64 mPCLKPeriod;            //Period value of clock
    sc_dt::uint64 mPCLKFreq;            //Frequency value of clock
    sc_dt::uint64 mPCLKOrgFreq;            //Original frequency value of clock
    std::string mPCLKFreqUnit;            //Frequency unit of clock

    sc_dt::uint64 mTimeResolutionValue;            //Time resolution value of simulation
    bool mEnableRegisterMessage;                   ///< Enable/Disable info/warning/error message of register IF//
    bool mResetCurVal[emResetSignalNum];        //Store current value of reset signals
    bool mResetPreVal[emResetSignalNum];        //Store previous value of reset signals
    bool mIsResetHardActive[emResetSignalNum];    //Reset operation status of reset signals
    bool mIsResetCmdReceive[emResetSignalNum];    //Receive status of AssertReset command of reset signals
    bool mIsResetCmdActive[emResetSignalNum];    //Active status of AssertReset command of reset signals
    double mResetCmdPeriod[emResetSignalNum];    //Period of AssertReset command of reset signals


    /// Declare events
    sc_event mResetHardEvent[emResetSignalNum];            //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emResetSignalNum];            //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over
    sc_event mClkZeroEvent[2];                            //Notify clock is zero

public:
    /// Declare signals
    // Input ports
    sc_dt::uint64 mfclasmpclkPeriod;            //Period value of clock
    double mTimeCLKChangePoint[2];                //The time clock is changed its value (0: PCLK, 1: fclasmpclk)
    sc_time_unit mTimeResolutionUnit;            //Time resolution unit of simulation
    sc_in<sc_dt::uint64> SMPCLK;            //Sampling clock for level detector
    sc_in<sc_dt::uint64> PCLK;                //APB bus clock
    sc_in<bool> SMPRESZ;                    //Sampling Reset signal
    sc_in<bool> PRESETZ;                    //APB reset signal

    sc_in<bool>  *FCLATSI[emChNumMax];            //Input signal (direct signal from I/O)
    sc_in<bool>  *FCLATNFSI[emChNumMax];        //Input signal after noise filter
    sc_in<bool>  *FCLATEDSI[emChNumMax];        //Edge detect input signal
    sc_out<bool> *FCLATINTR[emChNumMax];        //Rising edge select signal
    sc_out<bool> *FCLATINTF[emChNumMax];        //Falling edge select signal
    sc_out<bool> *FCLATSO[emChNumMax];            //Filtered/bypassed output signal
    sc_out<bool> *FCLATEDSO[emChNumMax];          //Edge/Level detection signal
    /// Socket declaration
    TlmTargetSocket<BUS_WIDTH_APB> *ts;            // For 's registers

    SC_HAS_PROCESS (QLAPBFICTLAV1);
    QLAPBFICTLAV1 (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency,
            const unsigned int nChannelNumber );
    ~QLAPBFICTLAV1 (void);

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
    void EnableRegisterMessage (const std::string is_enable);//

    bool GetResetStatus (const std::string reset_name);
    bool CheckClockPeriod (const std::string clock_name);

    sc_time  CalculateClockEdge (const std::string clock_name);
    unsigned int GetChId(void);
private:
    /// Declare methods
    void HandlefclasmpclkSignalMethod (void);
    void HandlePCLKSignalMethod (void);

    void HandlePRESETZSignalMethod (void);
    void HandleSMPRESZSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleZeroCLKMethod (const unsigned int clkzero_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    /// Internal functions
    void EnableReset (const std::string reset_name, const bool is_active);
    void CancelEvents (void);
    void RegisterHandler (const unsigned int ch_id, const std::vector<std::string> &args);

    /// Virtual functions of _AgentController class


};
#endif
// vim600: set foldmethod=marker :
