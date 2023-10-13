// ---------------------------------------------------------------------
// $Id: DMAG_U2.h,v 1.4 2020/02/14 06:25:20 hadinhtran Exp $
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

#ifndef __DMAG_U2_H__
#define __DMAG_U2_H__

#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"
#include "DMAG_U2_AgentController.h"
#include "rvc_common_model.h"


template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;
class DMAG_U2_Func;
class DMAG_U2_Common_Guard;
class DMAG_U2:  public DMAG_U2_AgentController,
public BusBridgeModuleBase<32,32>,
public BusBridgeSlaveBase<32,1>,
public BusMasterBase<32,1>,
public BusSlaveBase<32,1>,
public rvc_common_model
{
private:
    enum eResetActiveLevel {    // Define active level of reset signal
        emResetActive = 0
    };
    DMAG_U2_Func            *mDMAG_U2_Func;             //DMAG_U2_Func class instance
    DMAG_U2_Common_Guard    *mDMAG_U2_Common_Guard;     //DMAG_U2_Common_Guard class instance

    sc_dt::uint64   mPCLKPeriod;                //Period value of clock
    sc_dt::uint64   mPCLKFreq;                  //Frequency value of clock
    sc_dt::uint64   mPCLKOrgFreq;               //Original frequency of clock
    std::string     mPCLKFreqUnit;              //Frequency unit of clock

    sc_dt::uint64   mTimeResolutionValue;       //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;        //Time resolution unit of simulation

    bool            mResetCurVal;               //Store current value of reset signals
    bool            mResetPreVal;               //Store previous value of reset signals
    bool            mIsResetHardActive;         //Reset operation status of reset signals
    bool            mIsResetCmdReceive;         //Receive status of AssertReset command of reset signals
    bool            mIsResetCmdActive;          //Active status of AssertReset command of reset signals
    double          mResetCmdPeriod;            //Period of AssertReset command of reset signals
    double          mTimeCLKChangePoint;        //The time clock is changed its value

    bool            mDumpInterrupt;             // Enable/Disable dumping the interrupt info
    bool            mEnableRegisterMessage;     // Enable/Disable info/warning/error message of register IF
    /// Declare events
    sc_event mResetHardEvent;                   //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent;                    //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent;              //Call CancelResetCmdMethod when AssertReset is over
    sc_event mClkZeroEvent;                     //Notify clock is zero

    sc_event mTransErrorEvent;                  //Notify guard error has occurred

    sc_event mAssert_DMAGUARD_ERROR_Event;      //Assert dmaguard_erro output
    sc_event mDeassert_DMAGUARD_ERROR_Event;    //Deassert dmaguard_erro output

    sc_event mWrite_DMAGUARD_ERROR_Event;       //Write to dmaguard_erro output
    bool     mDMAGUARD_ERROR_value;             //Value of dmaguard_error output


public:
    SC_HAS_PROCESS(DMAG_U2);
    DMAG_U2(sc_module_name name,
            unsigned int iCHAN_NUM,
            unsigned int rLatency,
            unsigned int wLatency,
            SIM_MODE_T simmode);
    ~DMAG_U2(void);

    sc_in<sc_dt::uint64> PCLK;
    sc_out<bool> dmaguard_error;

    //Socket declaration
    TlmTargetSocket<32> *ts;
    TlmTargetSocket<32> *tsp;
    TlmInitiatorSocket<32> *isp;

    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void DumpInterrupt(const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void DMAG_U2_P_RESET_VALUE0_m(const unsigned int value, unsigned int index);
    void DMAG_U2_P_RESET_VALUE1_n(const unsigned int value, unsigned int index);

    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetAreaAddress(const unsigned int ch_id, const unsigned int start_addr, const unsigned int size);

private:
    /// Declare methods
    void HandleErrorMethod (void);
    void HandlePCLKSignalMethod (void);

    void HandleResetPortSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);

    void Assert_DMAGUARD_ERROR_Method(void);
    void Deassert_DMAGUARD_ERROR_Method(void);
    void Write_DMAGUARD_ERROR_Method(void);

    /// Internal functions
    void DumpInterruptMsg (const std::string intr_name, const bool value);              //Dump interrupt message
    void EnableReset (const bool is_active);

    void CancelEvents (void);
    void RegisterHandler (const std::vector<std::string> cmd);
    void PCLKUpdate (void);

    // Override function of AgentController class
    void NotifyTransErrorEvent(void);
    bool GetResetStatus(void);
    bool CheckClockPeriod(void);
    unsigned int getPermittedSPID(unsigned char channel_id);
    void getDMAGPROT_n(unsigned char ch_id, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg);
};
#endif //__DMAG_U2_H__//
