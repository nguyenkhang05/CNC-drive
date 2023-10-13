// ---------------------------------------------------------------------
// $Id: HI20FSGD2.h,v 1.6 2019/11/26 05:07:33 hadinhtran Exp $
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

#ifndef __HI20FSGD2_H__
#define __HI20FSGD2_H__

#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"
#include "HI20FSGD2_AgentController.h"
#include "HI20FSGD2_ERR_INFO.h"
#include "rvc_common_model.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;
class HI20FSGD2_Func;
class HI20FSGD2_Common_Guard;

class HI20FSGD2: public HI20FSGD2_AgentController,
        public BusBridgeModuleBase<32,32>,
        public BusBridgeSlaveBase<32,1>,
        public BusMasterBase<32,1>,
        public BusSlaveBase<32,1>,
        public rvc_common_model
{
public:
    /// Constructor
    SC_HAS_PROCESS(HI20FSGD2);
    HI20FSGD2(sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency,
            unsigned int iCHAN_NUM,
            SIM_MODE_T simmode);
    ~HI20FSGD2(void);

    /// Ports signal
    sc_in<sc_dt::uint64>        PCLK;
    sc_in<bool>                 KCPROT_KCE;
    sc_out<HI20FSGD2_Cerrinfo>  PSLVERR;

    /// Socket declaration
    TlmTargetSocket<32> *tsp;
    TlmTargetSocket<32> *tsv;
    TlmInitiatorSocket<32> *is;

    /// Class pointer
    HI20FSGD2_Func              *mHI20FSGD2_Func;              //HI20FSGD2_Func class instance
    HI20FSGD2_Common_Guard      *mHI20FSGD2_DataHandler;       //HI20FSGD2_Common_Guard class instance

    /// PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void SetInitialValue (const std::string reg_name, const unsigned int ch_id, unsigned int value);

    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void SetICUMHA(const std::string is_enable);

    void SetAreaAddress (const std::string group_id, const unsigned int start_addr, const unsigned int size, const unsigned int ch_id);
    void LOCK_SUPPORT(unsigned int value);
    void SEC_DIS(unsigned int value);
    unsigned int GetLOCK_SUPPORT();

private:
    /// Variables
    sc_dt::uint64                 mPCLKPeriod;                //Period value of clock
    sc_dt::uint64                 mPCLKFreq;                  //Frequency value of clock
    sc_dt::uint64                 mPCLKOrgFreq;               //Original frequency of clock
    std::string                   mPCLKFreqUnit;              //Frequency unit of clock

    sc_dt::uint64                 mTimeResolutionValue;       //Time resolution value of simulation
    sc_time_unit                  mTimeResolutionUnit;        //Time resolution unit of simulation

    bool                          mResetCurVal;               //Store current value of reset signals
    bool                          mResetPreVal;               //Store previous value of reset signals
    bool                          mIsResetHardActive;         //Reset operation status of reset signals
    bool                          mIsResetCmdReceive;         //Receive status of AssertReset command of reset signals
    bool                          mIsResetCmdActive;          //Active status of AssertReset command of reset signals
    double                        mResetCmdPeriod;            //Period of AssertReset command of reset signals
    double                        mTimeCLKChangePoint;        //The time clock is changed its value

    bool                          mDumpInterrupt;             //Enable/Disable dumping the interrupt info
    bool                          mEnableRegisterMessage;     //Enable/disable info/warning/error message of register IF
    unsigned int                  mLock;                      //Store the value which is set from Python
    bool                          mKCPROT_KCE;                //Enable/Disable target guard only ICUMHA
    bool                          mPSLVERR_asserted;          //Enable/Disable target guard only ICUMHA
    HI20FSGD2_Cerrinfo            mPSLVERR;                   // Value of PSLVERR port

    /// Declare events
    sc_event mResetHardEvent;                                 //Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent;                                  //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent;                            //Call CancelResetCmdMethod when AssertReset is over
    sc_event mAssertPSLVERREvent;                             //Assert PSLVERR output
    sc_event mDeassertPSLVERREvent;                           //Deassert PSLVERR output

    /// Declare methods
    void HandleErrorMethod (void);
    void HandlePCLKSignalMethod (void);
    void HandleKCPROT_KCESignalMethod (void);

    void HandleresetPortSignalMethod (void);
    void HandleResetHardMethod (void);
    void HandleResetCmdMethod (void);
    void CancelResetCmdMethod (void);

    void AssertPSLVERRMethod (void);
    void DeassertPSLVERRMethod (void);

    /// Internal functions
    void Initialize(void);
    void DumpInterruptMsg (const std::string intr_name, const bool value);              //Dump interrupt message
    void EnableReset (const std::string reset_name, const bool is_active);

    void CancelEvents (void);
    void RegisterHandler (const std::vector<std::string> cmd);
    void PCLKUpdate (void);

    bool CheckClockPeriod (void);
    bool GetResetStatus(void);
    bool CheckKceBitStatus(void);
    void SetPBGPROT1(const unsigned int spid, const unsigned int ch_id);
    void SetPBGPROT0(const unsigned int sec, const unsigned int gen, const unsigned int dbg, const unsigned int um, const unsigned int wg, const unsigned int rg, const unsigned int ch_id);
};
#endif //__HI20FSGD2_H__
//
