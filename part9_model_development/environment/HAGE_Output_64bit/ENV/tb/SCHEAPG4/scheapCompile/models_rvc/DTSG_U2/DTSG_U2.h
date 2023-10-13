// ---------------------------------------------------------------------
// $Id: DTSG_U2.h,v 1.7 2020/01/16 02:37:09 synguyen Exp $
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

#ifndef __DTSG_U2_H__
#define __DTSG_U2_H__

#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"
#include "DTSG_U2_AgentController.h"
#include "rvc_common_model.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;
class DTSG_U2_Func;
class DTSG_U2_Common_Guard;
class DTSG_U2:  public DTSG_U2_AgentController,
    public BusBridgeModuleBase<BUS_WIDTH_APB, BUS_WIDTH_APB>,
    public BusBridgeSlaveBase<BUS_WIDTH_APB,1>,
    public BusSlaveBase<BUS_WIDTH_APB, 1>,
    public BusMasterBase<BUS_WIDTH_APB, 1>,
    public rvc_common_model
{
private:

    DTSG_U2_Func            *mDTSG_U2_Func;             //DTSG_U2_Func class instance
    DTSG_U2_Common_Guard    *mDTSG_U2_DataHandler;      //DTSG_U2_Common_Guard class instance

    sc_dt::uint64   mCLKPeriod;                        //Period value of clock
    sc_dt::uint64   mCLKFreq;                          //Frequency value of clock
    sc_dt::uint64   mCLKOrgFreq;                       //Original frequency of clock
    std::string     mCLKFreqUnit;                      //Frequency unit of clock
                                                     
    sc_dt::uint64   mTimeResolutionValue;               //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;                //Time resolution unit of simulation
                                                     
    bool            mResetCurVal;                       //Store current value of reset signals
    bool            mResetPreVal;                       //Store previous value of reset signals
    bool            mIsResetHardActive;                 //Reset operation status of reset signals
    bool            mIsResetCmdReceive;                 //Receive status of AssertReset command of reset signals
    bool            mIsResetCmdActive;                  //Active status of AssertReset command of reset signals
    double          mResetCmdPeriod;                    //Period of AssertReset command of reset signals
    double          mTimeCLKChangePoint;                //The time clock is changed its value
    bool            mDumpInterrupt;                  
    bool            mEnableRegisterMessage;                  
    bool            mDTSG_ERR_value;                    //Value of dtsg_error output
    unsigned int    mNumOfChannel;                      //Total channel in DTSG model. Default is 129
                                                     
    /// Declare events                               
    sc_event mResetHardEvent;                           //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent;                            //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent;                      //Call CancelResetCmdMethod when AssertReset is over
    sc_event mAssertDTSG_ERREvent;                   //Assert dtsg_error output
    sc_event mDeassertDTSG_ERREvent;                 //Deassert dtsg_error output
    sc_event mWriteDTSG_ERREvent;                    //Write to dtsg_error output

public:
    SC_HAS_PROCESS(DTSG_U2);
    DTSG_U2(sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency,
            unsigned int numChannel,
            SIM_MODE_T simmode);
    ~DTSG_U2(void);

    sc_in<sc_dt::uint64> clk;
    sc_out<bool> dtsguard_error;

    //Socket declaration
    TlmTargetSocket<BUS_WIDTH_APB> *tsp;
    TlmTargetSocket<BUS_WIDTH_APB> *tspa;
    TlmInitiatorSocket<BUS_WIDTH_APB> *a_is;

    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
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
    void EnableRegisterMessage(const std::string is_enable);
    void SetAreaAddress (const unsigned int start_addr, const unsigned int size, const unsigned int channel);
    void GetDTSGPROT(const unsigned int channel_index, unsigned int& mpid, bool& gen, bool& dbg, bool& um, bool& wg, bool& rg);
    unsigned int GetSPIDERR();
    unsigned int GetDTSGMPID_SPID(const unsigned int channel_index);

private:
    /// Declare methods
    void HandleErrorMethod (void);
    void HandleCLKSignalMethod (void);

    void HandleResetSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);

    void AssertDTSG_ERRMethod (void);
    void DeassertDTSG_ERRMethod (void);
    void WriteDTSG_ERRMethod (void);

    /// Internal functions
    void DumpInterruptMsg (const std::string intr_name, const bool value);              //Dump interrupt message
    void EnableReset (const std::string reset_name, const bool is_active);


    void CancelEvents (void);
    void RegisterHandler (const std::vector<std::string> cmd, const std::string reg_name);
    void CLKUpdate (void);

    bool CheckClockPeriod (const std::string clock_name);

};
#endif //__DTSG_U2_H__
//
