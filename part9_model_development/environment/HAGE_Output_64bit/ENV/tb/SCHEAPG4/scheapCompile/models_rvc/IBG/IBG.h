// ---------------------------------------------------------------------
// $Id: IBG.h,v 1.10 2019/07/15 08:43:08 nhuvo Exp $
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

#ifndef IBG_IBG_H_
#define IBG_IBG_H_

#include "global.h" // define bus widths BUS_WIDTH_VCI32
#include "re_define.h"
#include "BusSlaveBase.h"           // define BusSlaveBase interface
#include "BusBridgeModuleBase.h"    // define BusBridgeModuleBase interface
#include "BusBridgeSlaveBase.h"     // define BusBridgeSlaveBase interface
#include "BusMasterBase.h"          // define BusMasterBase interface

#include "IBG_Func.h"
#include "IBG_CommonGuard.h"
#include "IBG_CommonGuardAgentControllerIf.h"
#include "rvc_common_model.h"

#include <systemc>
using namespace sc_core;
#include <stdint.h>
#include <map>
#include <string>
using std::string;

#define CHANNEL_MAX_NUM 17
#define CHANNEL_MIN_NUM 1

///@brief realize the template classes to the more detailed ones
typedef BusSlaveBase       <BUS_WIDTH_APB,1>                 IbgBusSlave;
typedef TlmTargetSocket    <BUS_WIDTH_APB>                   IbgApbTlmTargetSocket;
typedef BusBridgeModuleBase<BUS_WIDTH_VCI32,BUS_WIDTH_VCI32> IbgBusBridgeModule;
typedef BusBridgeSlaveBase <BUS_WIDTH_VCI32,1>               IbgBusBridgeSlave;
typedef BusMasterBase      <BUS_WIDTH_VCI32,1>               IbgBusMaster;
typedef TlmTargetSocket    <BUS_WIDTH_VCI32>                 IbgVci32TlmTargetSocket;
typedef TlmInitiatorSocket <BUS_WIDTH_VCI32>                 IbgVci32TlmInitiatorSocket;

/**@brief the top-level class of Inter-cluster bus guard (IBG) model;
 *        it instantiates function models and helps the function model do timed
 *        code. It also connects these parts together.
 */
class IBG : public IbgBusBridgeModule
          , public IbgBusBridgeSlave
          , public IbgBusSlave
          , public IbgBusMaster
          , public IBG_CommonGuardAgentControllerIf
          ,public rvc_common_model
{
//##############################################################################
// Part 1: clock, reset and initialization and logging facility
//    this part is mostly reused, and changed slightly during projects
//##############################################################################
public:
    #include "IBG_cmdif.h"
private:
    uint8_t CHAN_NUM;
//----------------------------
// modeling clock sources
//----------------------------
public:
    sc_in<sc_dt::uint64> PCLK;    ///< positive edge active clock PCLK
private:
    sc_dt::uint64 mPCLKPeriod;    ///< Period of HSB
    sc_dt::uint64 mPCLKOrgFreq;   ///< Previous frequency of HSB
    sc_dt::uint64 mPCLKFreq;      ///< Frequency of HSB
    std::string   mPCLKFreqUnit;  ///< Frequency unit of HSB
    double        mPCLKSyncTPoint;///< The time point at which active edge of clock happens

private:
    void PCLKMethod();
private:
    bool    CheckClockPeriod   (const std::string clock_name);
//----------------------------
// modeling reset sources and initialization
//----------------------------
public:
    sc_in<bool> PRESETn;       ///< asynchronous active-low input reset signal PRESETn
private:
    bool   mIsResetHardActive; ///< Is reset by port
    bool   mIsResetCmdReceive; ///< Is receive cmd reset
    bool   mIsResetCmdActive;  ///< Is reset by cmd
    double mResetCmdPeriod;    ///< Period of cmd reset

    sc_event mResetCmdEvent;
    sc_event mResetCmdCancelEvent;

    void resetPortMethod();
    void HandleResetCmdMethod();
    void CancelResetCmdMethod();
    void PRESETnMethod();
private:
    void Initialize(void);
    void CancelEvents(void);
    void EnableReset(const bool isActive);
//----------------------------
// time resolution
//----------------------------
private:
    sc_dt::uint64   mTimeResolutionValue;   ///< Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    ///< Time resolution unit of simulation
private:
//----------------------------
// constructing the model
//----------------------------
private:
    IBG_Func* pFuncModel; ///< instance of functional part
public:
    SC_HAS_PROCESS(IBG);
    IBG(sc_module_name iname
       ,uint8_t        iCHAN_NUM
       ,uint32_t       rLatency
       ,uint32_t       wLatency
       ,SIM_MODE_T     simmode
    );
    virtual ~IBG();

//##############################################################################
// Part 2: PythonIF
//    this part is mostly reused, and changed very little during projects
//##############################################################################
private:
    bool mDumpInterrupt; ///< Enable/Disable dumping the interrupt info
    bool mEnableRegisterMessage;         // Enable/disable info/warning/error message of register IF
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
    void EnableRegisterMessage(const std::string is_enable);
private:
    void DumpInterruptMsg (const std::string intr_name, const bool value);
//##############################################################################
// Part 3: custom code
//##############################################################################
//------------------------------------------------------------------------------
// public interfaces:
//------------------------------------------------------------------------------
public:
    IbgApbTlmTargetSocket*       ts;      ///< for register block of the IBG
    IbgVci32TlmTargetSocket*     tsp;     ///< to receive protected transactions
    IbgVci32TlmInitiatorSocket*  isp;     ///< to forward protected transactions
    sc_out<bool>                 err_irq; ///< interrupt notifies that the bus guard detected an illegal transaction
//------------------------------------------------------------------------------
// internal sub-modules, signals, variables
//------------------------------------------------------------------------------
private:
    bool                         err_irq_val; ///< value of err_irq output 
    IBG_CommonGuard* pBusGuard; ///< functional part implementing transaction exchanging and transaction validation,
//------------------------------------------------------------------------------
// internal processes and associated events
//------------------------------------------------------------------------------
private:
    sc_event mAssertErr_IrqEvent;
    sc_event mDeassertErr_IrqEvent;
    sc_event mUpdateErr_IrqEvent;
    void AssertErr_IrqMethod();
    void DeassertErr_IrqMethod();
    void UpdateErr_IrqMethod();
//------------------------------------------------------------------------------
// helper methods
//------------------------------------------------------------------------------
private:

//------------------------------------------------------------------------------
// implement common guard agent controller interface
//------------------------------------------------------------------------------
public:
    uint32_t GetGERRSTAT();
    void DumpStatusInfo();
    bool CheckGuardClock ();
    bool GetGuardResetStatus ();
    void printMsg (const char* severity
                  ,const char* msg);
    bool SetAreaAddr(const uint32_t    channelId
                    ,const uint32_t    start_addr
                    ,const uint32_t    size);
    bool SetAreaAddr(const std::string groupName
                    ,const uint32_t    channelId
                    ,const uint32_t    start_addr
                    ,const uint32_t    size);
    void TransferErrInfo (const string   groupName
                         ,const uint32_t channelId
                         ,const uint32_t  addr
                         ,const bool      isDebug
                         ,const bool      isUserMode
                         ,const bool      isWrite
                         ,const uint8_t   SPID);
    bool GetGuardSettings(const string   groupName
                         ,const uint32_t channelId
                         ,bool&     GEN
                         ,bool&     DBG
                         ,bool&     UM
                         ,bool&     WG
                         ,bool&     RG
                         ,uint32_t& SPID);
}; // end-of-class

#endif /* IBG_IBG_H_ */

//##############################################################################
// EOF
//##############################################################################
