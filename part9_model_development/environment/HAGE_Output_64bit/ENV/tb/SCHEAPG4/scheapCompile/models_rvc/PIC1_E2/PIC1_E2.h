// ---------------------------------------------------------------------
// $Id: PIC1_E2.h 3395 2017-06-29 08:02:34Z ChinhTD $
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
#ifndef __PIC1_E2_H__
#define __PIC1_E2_H__
#include "BusSlaveBase.h"
#include "PIC1_E2_AgentController.h"

class PIC1_E2_Func;
class PIC1_E2: public sc_module,
              public PIC1_E2_AgentController,
              public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {
        emResetActive               = 0,
        emResetSignalNum            = 1
    };//Define active level of reset signal
    enum eChannel {
        emchannel0 = 0,
        emchannel1 = 1,
        emchannel2 = 2,
        emchannel3 = 3,
        emchannel4 = 4,
        emchannel5 = 5,
        emchannel6 = 6,
        emmaxchannel = 7
    };  //Define channel
public:
    //Declare input signals
    sc_in<sc_dt::uint64> CLK_HSB;
    sc_in<bool> sys_resetz;

    //Declare output signals
    sc_out<bool> OST0TSST;
    sc_out<bool> OST1TSST;
    sc_out<bool> OST2TSST;
    sc_out<bool> OST3TSST;
    sc_out<bool> OST4TSST;
    sc_out<bool> OST5TSST;
    sc_out<bool> OST6TSST;
    
    //Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (PIC1_E2);

    PIC1_E2 (sc_module_name name, 
        unsigned int rLatency,
        unsigned int wLatency);
    ~PIC1_E2 (void);

    //Declare public functions
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void SetOutputSyn (const unsigned int delay_time, const unsigned int pulse_period);
    void ListRegister (void);
    void Help (const std::string type);

private:
    PIC1_E2_Func       *mPIC1Func;                          //PIC1_E2_Func class instance

    //Used for clock setting
    sc_dt::uint64       mCLK_HSBPeriod;                         //Period value of CLK_HSB clock
    sc_dt::uint64       mCLK_HSBOrgFreq;                        //Original frequency value of CLK_HSB clock (Hz)
    sc_dt::uint64       mCLK_HSBFreq;
    std::string         mCLK_HSBFreqUnit;                          //Frequency unit of CLK_HSB clock
    sc_dt::uint64       mTimeResolutionValue;                   //Time resolution value of simulation
    sc_time_unit        mTimeResolutionUnit;                    //Time resolution unit of simulation

    //Used for reset handling
    bool                mResetCurVal;         //Store current value of sys_resetz
    bool                mResetPreVal;         //Store previous value of sys_resetz
    bool                mIsResetHardActive;   //Reset operation status of sys_resetz signal
    bool                mIsResetCmdReceive;   //Receive status of AssertReset command of sys_resetz
    bool                mIsResetCmdActive;    //Active status of AssertReset command of sys_resetz
    double              mResetCmdPeriod;      //Period of AssertReset command of sys_resetz
    //Used for Python IF
    unsigned int        mOutputDelay;
    unsigned int        mOutputPeriod;
    //Used for OSTxTSST
    bool                mOST[emmaxchannel];

    sc_event            mResetHardEvent;      //Call HandleResetHardMethod when sys_reset is active
    sc_event            mResetCmdEvent;       //Call HandleResetCmdMethod when AssertReset is called
    sc_event            mResetCmdCancelEvent; //Call CancelResetCmdMethod when AssertReset is over
    sc_event            mOutputWriteEvent[emmaxchannel];
    sc_event            mFallingOSTOutEvent[emmaxchannel];
    sc_event            mRisingOSTOutEvent[emmaxchannel];
   
    void Initialize (void);
    void InitializeOutputSignals (void);
    void CheckFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    double CalculateCLKPosEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time);

    void EnableReset (const bool is_active);

    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);
    void HandleSYS_RESETZSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleCLK_HSBSignalMethod (void);
    void RisingOSTOutSignalsMethod (const unsigned int signal_id);
    void FallingOSTOutSignalsMethod (const unsigned int signal_id);
    void WriteOutSignalsMethod (const unsigned int signal_id);

    //Declare virtual functions of PIC1_E2_AgentController class
    bool CheckClockPeriod (const std::string clock_name);
    void StartSimultaneous (const std::string output_channel);
};
#endif
