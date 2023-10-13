// ---------------------------------------------------------------------
// $Id: RHSB_XBAR.h 1339 2019-11-14 09:01:36Z chuonghoangle $
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
#ifndef __RHSB_XBAR_H__
#define __RHSB_XBAR_H__
#include "BusSlaveBase.h"
#include "RHSB_XBAR_AgentController.h"
#include "rvc_common_model.h"
#include "global.h"

class RHSB_XBAR_Func;
class RHSB_XBAR: public sc_module,
              public RHSB_XBAR_AgentController,
              public BusSlaveBase<32,1>,
              public rvc_common_model
{
private:
    enum eResetActiveLevel {
        emResetSignalNum            = 1
    };//Define active level of reset signal
public:
    //Declare input signals
    sc_in<sc_dt::uint64> CLK_LSB;
    sc_in<bool> PRESETn;
    sc_in<sc_dt::uint64> ATU_C;
    sc_in<sc_dt::uint64> ATU_D;
    sc_in<sc_dt::uint64> ATU_E;
    sc_in<sc_dt::uint64> GTM_TOM;
    sc_in<sc_dt::uint64> GTM_ATOM;
    sc_in<sc_dt::uint64> GPIO;

    //Declare output signals
    sc_out<sc_dt::uint64> tmr_out;
    //Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (RHSB_XBAR);

    RHSB_XBAR (sc_module_name name, 
        unsigned int rLatency,
        unsigned int wLatency,
        std::string user);
    ~RHSB_XBAR (void);

    //Declare public functions
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void USER (const std::string user);
    void EnableRegisterMessage(const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);

private:
    RHSB_XBAR_Func       *mRHSB_XBARFunc;                          //RHSB_XBAR_Func class instance

    //Used for clock setting
    sc_dt::uint64       mCLK_LSBPeriod;                         //Period value of CLK_LSB clock
    sc_dt::uint64       mCLK_LSBOrgFreq;                        //Original frequency value of CLK_LSB clock (Hz)
    sc_dt::uint64       mCLK_LSBFreq;
    std::string         mCLK_LSBFreqUnit;                       //Frequency unit of CLK_LSB clock
    sc_dt::uint64       mTimeResolutionValue;                   //Time resolution value of simulation
    sc_time_unit        mTimeResolutionUnit;                    //Time resolution unit of simulation
    double                mStartClkTime;                            // Time when clock is set
    //Used for parameter
    std::string       mUser;
    //Used for reset handling
    bool                mResetCurVal;         //Store current value of PRESETn
    bool                mResetPreVal;         //Store previous value of PRESETn
    bool                mIsResetHardActive;   //Reset operation status of PRESETn signal
    bool                mIsResetCmdReceive;   //Receive status of AssertReset command of PRESETn
    bool                mIsResetCmdActive;    //Active status of AssertReset command of PRESETn
    double              mResetCmdPeriod;      //Period of AssertReset command of PRESETn
    bool                mEnableRegisterMessage;
    //Used for Reset
    sc_event            mResetHardEvent;      //Call HandleResetHardMethod when sys_reset is active
    sc_event            mResetCmdEvent;       //Call HandleResetCmdMethod when AssertReset is called
    sc_event            mResetCmdCancelEvent; //Call CancelResetCmdMethod when AssertReset is over
    sc_event            mCheckSignalEvent; 
    sc_event            mAfterResetEvent;
    sc_event            mHandleOutputSignalEvent;
    //Used for others
    sc_dt::uint64       mtmr_out;
    sc_dt::uint64       mATU_C;
    sc_dt::uint64       mATU_D;
    sc_dt::uint64       mATU_E;
    sc_dt::uint64       mGTM_TOM;
    sc_dt::uint64       mGTM_ATOM;
    sc_dt::uint64       mGPIO;
    sc_dt::uint64       mtmrH;
    sc_dt::uint64       mtmrL;
   
    void EnableReset (const bool is_active);
    void UpdateValue(const bool position, const sc_dt::uint64 pos_out, const sc_dt::uint64 temp_input);

    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);
    void HandlePRESETNSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleCLK_LSBSignalMethod (void);
    void HandleInputSignalMethod (void);
    void HandleOutputSignalMethod (void);
    
    //Declare virtual functions of RHSB_XBAR_AgentController class
    bool CheckClockPeriod (void);
    void SelectforOutput (const unsigned int pos_out, const unsigned int pos_in, const std::string from_input);
    void WriteOutput(void);
    bool GetResetStatus(void);
};
#endif
