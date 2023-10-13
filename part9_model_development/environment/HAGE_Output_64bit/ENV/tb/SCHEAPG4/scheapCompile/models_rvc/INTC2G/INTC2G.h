// ---------------------------------------------------------------------
// $Id: INTC2G.h,v 1.4 2019/09/19 10:16:17 ngathunguyen Exp $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef INTC2G_INTC2G_H_
#define INTC2G_INTC2G_H_

#include "BusSlaveBase.h"                         /// define BusSlaveBase interface
#include "BusBridgeModuleBase.h"                  /// define BusBridgeModuleBase interface
#include "BusBridgeSlaveBase.h"                   /// define BusBridgeSlaveBase interface
#include "BusMasterBase.h"                        /// define BusMasterBase interface

#include "INTC2G_Func.h"
#include "INTC2G_CommonGuard.h"
#include "INTC2G_CommonGuardAgentControllerIf.h"
#include "rvc_common_model.h"

typedef BusSlaveBase       <BUS_WIDTH_APB,1>                INTC2G_BusSlave;
typedef BusMasterBase      <BUS_WIDTH_APB,1>                INTC2G_BusMaster;
typedef TlmTargetSocket    <BUS_WIDTH_APB>                  INTC2G_ApbTlmTargetSocket;
typedef TlmInitiatorSocket <BUS_WIDTH_APB>                  INTC2G_ApbTlmInitiatorSocket;
typedef BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB>    INTC2G_BusBridgeModule;
typedef BusBridgeSlaveBase <BUS_WIDTH_APB,1>                INTC2G_BusBridgeSlave;

/// forward declaration of classes
class INTC2G_Func;
class INTC2G: public INTC2G_BusSlave
            , public INTC2G_BusMaster
            , public INTC2G_BusBridgeSlave
            , public INTC2G_BusBridgeModule
            , public INTC2G_CommonGuardAgentControllerIf
            , public rvc_common_model
{
public:
    #include "INTC2G_cmdif.h"
//##############################################################################
// Part: clock, reset and initialization and logging facility
//    this part is mostly reused, and changed slightly during projects
//##############################################################################
//------------------------------------------------------------------------------
// modeling clock sources
//------------------------------------------------------------------------------
public:
    sc_in<sc_dt::uint64> PCLK;      ///< positive edge active clock PCLK
private:
    sc_dt::uint64   mPCLKPeriod;            ///< Period of PCLK
    sc_dt::uint64   mPCLKOrgFreq;           ///< Previous frequency of PCLK
    sc_dt::uint64   mPCLKFreq;              ///< Frequency of PCLK
    std::string     mPCLKFreqUnit;          ///< Frequency unit of PCLK
    double          mPCLKSyncTPoint;        ///< The time point at which active edge of clock happens

    void    PCLKMethod();
    sc_time CalculateClockEdge();
    bool    CheckClockPeriod(const std::string clock_name);
//----------------------------
// modeling reset sources and initialization
//----------------------------
private:

    bool   mIsResetPortActive;  ///< Is reset by port
    bool   mIsResetCmdReceive;  ///< Is receive cmd reset
    bool   mIsResetCmdActive;   ///< Is reset by cmd
    double mResetCmdPeriod;     ///< Period of cmd reset
    bool   mEnableRegisterMessage;   ///< Is enable regisyer message or not
    sc_dt::uint64   mTimeResolutionValue;   ///< Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    ///< Time resolution unit of simulation

    sc_event mResetCmdEvent;
    sc_event mCancelResetCmdEvent;
    sc_event mClockChangeEvent;

    void resetPortHandleMethod();
    void resetCmdHandleMethod();
    void CancelResetCmdMethod();
    void EnableReset(bool isActive);
    void Initialize();
    void CancelEvents();
    void UpdateSysClockMethod();           ///< update interrupt sys new clock
public:
    bool GetResetStatus() const;
    uint32_t GetINTC2GERRSTAT();

//##############################################################################
// Part: PythonIF
//    this part is mostly reused, and changed very little during projects
//##############################################################################
public:
    void AssertReset     (const std::string reset_name
                         ,const double      start_time
                         ,const double      period);
    void SetCLKFreq      (const std::string     clock_name
                         ,const sc_dt::uint64   freq
                         ,const std::string     unit);
    void GetCLKFreq      (const std::string     clock_name);
    void ForceRegister   (const std::string     reg_name
                         ,const unsigned int    reg_value);
    void ReleaseRegister (const std::string     reg_name);
    void WriteRegister   (const std::string     reg_name
                         ,const unsigned int    reg_value);
    void ReadRegister    (const std::string reg_name);
    void ListRegister    (void);
    void Help            (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW  (const std::string is_enable);
    void DumpInterrupt   (const std::string is_enable);
    void DumpStatusInfo  ();
    void EnableRegisterMessage(const std::string is_enable);
    void SetINTC2GPROTmNum(unsigned int channelnum);
private:
    bool mDumpInterrupt;        ///< Enable/Disable dumping the interrupt info
    void DumpInterruptMsg (const std::string intr_name
                         , const bool value);

//##############################################################################
// Part: Model attribute
//##############################################################################
//----------------------------
// constructing the model
//----------------------------
private:
    const uint32_t      CHAN_NUM_START;
    uint32_t            CHAN_NUM_END;
    INTC2G_Func*        pFuncModel;     ///< Functional part
    INTC2G_CommonGuard* pIntc2Guard;    ///< Functional part of guard
    const uint8_t      MPIDBitsNumber;
    uint32_t spidDecode(uint8_t MPID_value);
public:
    SC_HAS_PROCESS(INTC2G);
    INTC2G(  sc_module_name   name
           , uint32_t         rLatency
           , uint32_t         wLatency
           , SIM_MODE_T       simmode
           );
    virtual ~INTC2G();

//------------------------------------------------------------------------------
// public interfaces:
// INTC2G Has: + a APB target socket for register access
//             + a APB target socket for receiving protected transaction
//             + a initiator socket for forwarding protected transaction
//             + a interrupt port for notifying to Error Control Module
//------------------------------------------------------------------------------
public:
    INTC2G_ApbTlmTargetSocket*          ts;
    INTC2G_ApbTlmTargetSocket*          tsp;
    INTC2G_ApbTlmInitiatorSocket*       isp;

    sc_out<bool>                        err_irq;

//------------------------------------------------------------------------------
// internal processes and associated events
//------------------------------------------------------------------------------
private:
    bool     err_irq_val;   
    sc_event mAssertErr_IrqEvent;
    sc_event mDeassertErr_IrqEvent;
    sc_event mUpdateErr_IrqEvent;

    void AssertErr_IrqMethod();
    void DeassertErr_IrqMethod();
    void UpdateErr_IrqMethod();
//------------------------------------------------------------------------------
//Virtual function of INTC2G_CommonGuardAgentControllerIf
//------------------------------------------------------------------------------
public:
    bool CheckGuardClock();
    bool GetGuardResetStatus();
    void printMsg (const char* severity
                  ,const char* msg);
    bool SetAreaAddr(const uint32_t    channelId
                    ,const uint32_t    startAddr
                    ,const uint32_t    size);
    void TransferErrInfo (const uint32_t     channelId
                         ,const uint32_t     addr
                         ,const bool         isDebug
                         ,const bool         isUserMode
                         ,const bool         isWrite
                         ,const uint8_t      SPID
                         );
    bool GetGuardSettings(const uint32_t     channelId
                         ,bool&              GEN
                         ,bool&              DBG
                         ,bool&              UM
                         ,bool&              WG
                         ,bool&              RG
                         ,uint32_t&          SPID
                         );

};

#endif /*INTC2G_INTC2G_H_*/
