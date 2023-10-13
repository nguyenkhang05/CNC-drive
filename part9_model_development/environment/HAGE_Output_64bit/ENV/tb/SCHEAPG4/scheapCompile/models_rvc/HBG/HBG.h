// ---------------------------------------------------------------------
// $Id: HBG.h,v 1.11 2020/02/25 02:15:23 huyquocpham Exp $
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

#ifndef __HBG_H__
#define __HBG_H__

#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"
#include "HBG_AgentController.h"
#include "rvc_common_model.h"


template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;
class HBG_Func;
class HBGERRSLV_Func;
class HBG_Common_Guard;
class HBG:  public HBG_AgentController,
    public BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_AXI>,
    public BusBridgeSlaveBase<BUS_WIDTH_AXI,1>,
    public BusSlaveBase<BUS_WIDTH_APB, 2>,
    public BusMasterBase<BUS_WIDTH_AXI, 1>,
    public rvc_common_model

{
private:
    enum eResetActiveLevel {    // Define active level of reset signal
        emResetActive = 0
    };

    HBG_Func        *mHBG_Func;                 //HBG_Func class instance
    HBGERRSLV_Func  *mHBGERRSLV_Func;           //HBGERRSLV_Func class instance
    HBG_Common_Guard    *mHBG_DataHandler;      //HBG_Common_Guard class instance

    sc_dt::uint64   mPCLKPeriod;                //Period value of clock
    sc_dt::uint64   mPCLKFreq;                  //Frequency value of clock
    sc_dt::uint64   mPCLKOrgFreq;               //Original frequency of clock
    std::string     mPCLKFreqUnit;              //Frequency unit of clock

    sc_dt::uint64   mACLKPeriod;                //Period value of APCLK clock
    sc_dt::uint64   mACLKFreq;                  //Frequency value of APCLK clock
    sc_dt::uint64   mACLKOrgFreq;               //Original frequency of APCLK clock
    std::string     mACLKFreqUnit;              //Frequency unit of APCLK clock

    sc_dt::uint64   mTimeResolutionValue;       //Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;        //Time resolution unit of simulation

    bool            mResetCurVal;               //Store current value of reset signals
    bool            mResetPreVal;               //Store previous value of reset signals
    bool            mIsResetHardActive;         //Reset operation status of reset signals
    bool            mIsResetCmdReceive;         //Receive status of AssertReset command of reset signals
    bool            mIsResetCmdActive;          //Active status of AssertReset command of reset signals
    double          mResetCmdPeriod;            //Period of AssertReset command of reset signals
    double          mTimeCLKChangePoint;        //The time clock is changed its value
    bool            mDumpInterrupt;
    bool            mEnableRegisterMessage;
    bool            mAXI_GRERR_value;           //Value of AXI_GRERRR output
    unsigned int    mLock;                      //Store the value which is set from Python
    unsigned int    mSec;                       //Store the value which is set from Python

    /// Declare events
    sc_event mResetHardEvent;                   //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent;                    //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent;              //Call CancelResetCmdMethod when AssertReset is over
    sc_event mAssertAXI_GRERREvent;             //Assert AXI_GRERR output
    sc_event mDeassertAXI_GRERREvent;           //Deassert AXI_GRERR output
    sc_event mWriteAXI_GRERREvent;              //Write to AXI_GRERR output

public:
    SC_HAS_PROCESS(HBG);
    HBG(sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency,
            SIM_MODE_T simmode);
    ~HBG(void);

    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> ACLK;
    sc_out<bool> AXI_GRERR;

    //Socket declaration
    TlmTargetSocket<BUS_WIDTH_APB> *tsp1;
    TlmTargetSocket<BUS_WIDTH_APB> *tsp2;
    TlmTargetSocket<BUS_WIDTH_AXI> *as_ts;
    TlmInitiatorSocket<BUS_WIDTH_AXI> *am_is;

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
    void SetAreaAddress(const unsigned int start_addr, const unsigned int size, const unsigned int channel);

    void SetAreaAddress (const std::string group_id, const unsigned int start_addr, const unsigned int size);
    void LOCK_SUPPORT(unsigned int value);      //Set mLock
    unsigned int GetLOCK_SUPPORT();            //Get mLock
    void SEC_DIS(unsigned int value);          //Set mSec
    void RESET_VALUE(unsigned int value);      //Set mReset_Value
    void SPID_RESET_VALUE(unsigned int value); //Set mSPID_Reset_Value
    unsigned int Get_KCE();                    //Get HBGPROT0_KCE bit Value


private:
    /// Declare methods
    void HandleErrorMethod (void);
    void HandlePCLKSignalMethod (void);
    void HandleACLKSignalMethod (void);

    void HandleResetPortSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);

    void AssertAXI_GRERRMethod (void);
    void DeassertAXI_GRERRMethod (void);
    void WriteAXI_GRERRMethod (void);

    /// Internal functions
    void DumpInterruptMsg (const std::string intr_name, const bool value);              //Dump interrupt message
    void EnableReset (const std::string reset_name, const bool is_active);
    double CalculateNextRisingEdge (const std::string clock_name,const double time_point,const double time_change_point);

    void CancelEvents (void);
    void RegisterHandler (const std::vector<std::string> cmd, const std::string reg_name);
    void PCLKUpdate (void);
    void ACLKUpdate (void);

    bool CheckClockPeriod (const std::string clock_name);
    void SetHBGPROT1(const unsigned int spid);
    void SetHBGPROT0(const unsigned int gen, const unsigned int dbg, const unsigned int um, const unsigned int wg, const unsigned int rg);

};
#endif //__HBG_H__
//
