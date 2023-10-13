// ---------------------------------------------------------------------
// $Id: IBG_U2.h,v 1.2 2020/02/26 06:51:33 duongphan Exp $
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

#ifndef IBG_U2_IBG_U2_H_
#define IBG_U2_IBG_U2_H_

#include "global.h" // define bus widths BUS_WIDTH_VCI32
#include "re_define.h"
#include "BusSlaveBase.h"           // define BusSlaveBase interface
#include "BusBridgeModuleBase.h"    // define BusBridgeModuleBase interface
#include "BusBridgeSlaveBase.h"     // define BusBridgeSlaveBase interface
#include "BusMasterBase.h"          // define BusMasterBase interface

#include "IBG_U2_Func.h"
#include "IBG_U2_CommonGuard.h"
#include "IBG_U2_CommonGuardAgentControllerIf.h"

//Add include "rvc_common_model" by Quang
#include "rvc_common_model.h"
//

#include <systemc>
#include <stdint.h>
#include <map>
#include <string>
using std::string;

///@brief realize the template classes to the more detailed ones
typedef BusSlaveBase       <BUS_WIDTH_APB,1>                 Ibg_u2BusSlave;
typedef TlmTargetSocket    <BUS_WIDTH_APB>                   Ibg_u2ApbTlmTargetSocket;
typedef BusBridgeModuleBase<BUS_WIDTH_VCI32,BUS_WIDTH_VCI32> Ibg_u2BusBridgeModule;
typedef BusBridgeSlaveBase <BUS_WIDTH_VCI32,1>               Ibg_u2BusBridgeSlave;
typedef BusMasterBase      <BUS_WIDTH_VCI32,1>               Ibg_u2BusMaster;
typedef TlmTargetSocket    <BUS_WIDTH_VCI32>                 Ibg_u2Vci32TlmTargetSocket;
typedef TlmInitiatorSocket <BUS_WIDTH_VCI32>                 Ibg_u2Vci32TlmInitiatorSocket;
struct Ibg_u2ChanNum {
    unsigned int chanNumMin;
    unsigned int chanNumMax;
};

/**@brief the top-level class of Inter-cluster bus guard (IBG) model;
 *        it instantiates function models and helps the function model do timed
 *        code. It also connects these parts together.
 */
class IBG_U2 : public Ibg_u2BusBridgeModule
                , public Ibg_u2BusBridgeSlave
                , public Ibg_u2BusSlave
                , public Ibg_u2BusMaster
                , public IBG_U2_CommonGuardAgentControllerIf
{
//##############################################################################
// Part 1: clock, reset and initialization and logging facility
//    this part is mostly reused, and changed slightly during projects
//##############################################################################
public:
    #include "IBG_U2_cmdif.h"
private:
    std::string mGroupName;
    uint8_t chanNum;
    std::map<std::string, Ibg_u2ChanNum> mGuardIdMap;
//----------------------------
// modeling clock sources
//----------------------------
public:
    sc_in<sc_dt::uint64> PCLK;          ///< positive edge active clock PCLK
    sc_in<sc_dt::uint64> cpu_clk;       ///< positive edge active clock cpu_clk
private:
    sc_dt::uint64 mPCLKPeriod;          ///< Period of PCLK
    sc_dt::uint64 mPCLKOrgFreq;         ///< Previous frequency of PCLK
    sc_dt::uint64 mPCLKFreq;            ///< Frequency of PCLK
    std::string   mPCLKFreqUnit;        ///< Frequency unit of PCLK
    double        mPCLKSyncTPoint;      ///< The time point at which active edge of PCLK clock happens
    
    sc_dt::uint64 mcpu_clkPeriod;       ///< Period of cpu_clk
    sc_dt::uint64 mcpu_clkOrgFreq;      ///< Previous frequency of cpu_clk
    sc_dt::uint64 mcpu_clkFreq;         ///< Frequency of cpu_clk
    std::string   mcpu_clkFreqUnit;     ///< Frequency unit of cpu_clk
    double        mcpu_clkSyncTPoint;   ///< The time point at which active edge of cpu_clk clock happens

    sc_event mPCLKZeroClkEvent;         ///< Notify the event that cpu_clk clock source becomes 0.
    sc_event mcpu_clkZeroClkEvent;      ///< Notify the event that cpu_clk clock source becomes 0.
private:
    void PCLKMethod();                  ///< Control PCLK sesitive
    void cpu_clkMethod();               ///< Control cpu_clk sesitive
private:
    bool    CheckClockPeriod   (const std::string clock_name);                            ///< Check if clock period value is greater than 0 or not
    sc_time CalculateClockEdge (const std::string clock_name);                            ///< Calculate synchronous time
    double  CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point);   ///< Calculate synchronous time
//----------------------------
// modeling reset sources and initialization
//----------------------------
public:
    sc_in<bool> PRESETn;             ///< asynchronous active-low input reset signal PRESETn
private:
    bool   mIsResetHardActive;       ///< Is reset by port
    bool   mIsResetCmdReceive;       ///< Is receive cmd reset
    bool   mIsResetCmdActive;        ///< Is reset by cmd
    double mResetCmdPeriod;          ///< Period of cmd reset
    
    bool   mIsPRESETnHardActive;     ///< Is PRESETn reset by port
    bool   mIsPRESETnCmdReceive;     ///< Is receive cmd PRESTn reset
    bool   mIsPRESETnCmdActive;      ///< Is PRESETn reset by cmd
    double mPRESETnCmdPeriod;        ///< Period of cmd PRESETn reset

    sc_event mResetCmdEvent;         ///< Event reset command
    sc_event mResetCmdCancelEvent;   ///< Event cancel reset command
    
    sc_event mPRESETnCmdEvent;         ///< Event PRESETn reset command
    sc_event mPRESETnCmdCancelEvent;   ///< Event PRESETn cancel reset command

    void resetPortMethod();          ///< Monitor resetPort port
    void HandleResetCmdMethod();     ///< Process reset function when reset command is active
    void CancelResetCmdMethod();     ///< Cancel reset function when reset command is active
    void PRESETnMethod();            ///< Monitor PRESETn port
    void HandlePRESETnCmdMethod();   ///< Process reset function when PRESETn reset command is active
    void CancelPRESETnCmdMethod();   ///< Cancel reset function when PRESETn reset command is active
private:
    void Initialize(void);
    void CancelEvents(void);
    void EnableReset(const bool isActive);
    void EnablePRESETnReset(const bool isActive);
    bool GetResetStatus(const std::string resetName);
//----------------------------
// time resolution
//----------------------------
private:
    sc_dt::uint64   mTimeResolutionValue;   ///< Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    ///< Time resolution unit of simulation
private:
    void GetTimeResolution(sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
//----------------------------
// constructing the model
//----------------------------
private:
    IBG_U2_Func* pFuncModel; ///< instance of functional part
public:
    SC_HAS_PROCESS(IBG_U2);
    IBG_U2(sc_module_name iname
        ,std::string    itype
        ,uint8_t        ichanNum
        ,uint32_t       rLatency
        ,uint32_t       wLatency
        ,SIM_MODE_T     simmode
    );
    virtual ~IBG_U2();

//##############################################################################
// Part 2: PythonIF
//    this part is mostly reused, and changed very little during projects
//##############################################################################
private:
    bool mDumpInterrupt; ///< Enable/Disable dumping the interrupt info
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
    void DumpStatusInfo  ();                           ///< Dump the status information of model
private:
    void SeparateString(std::vector<std::string> &vtr, const std::string msg);
    void ConvertClockFreq(sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in);
    void DumpInterruptMsg (const std::string intr_name, const bool value);
//##############################################################################
// Part 3: custom code
//##############################################################################
//------------------------------------------------------------------------------
// public interfaces:
//------------------------------------------------------------------------------
public:
    Ibg_u2ApbTlmTargetSocket*       ts;                ///< For register block of the IBG
    Ibg_u2Vci32TlmTargetSocket*     tsp;               ///< To receive protected transactions
    Ibg_u2Vci32TlmInitiatorSocket*  isp;               ///< To forward protected transactions
    sc_out<bool>                 ibgerr;                ///< Interrupt notifies that the bus guard detected an illegal transaction
//------------------------------------------------------------------------------
// internal sub-modules, signals, variables
//------------------------------------------------------------------------------
private:
    bool                         ibgerr_val;            ///< Value of ibgerr output 
    IBG_U2_CommonGuard* pBusGuard;                         ///< Functional part implementing transaction exchanging and transaction validation,
//------------------------------------------------------------------------------
// internal processes and associated events
//------------------------------------------------------------------------------
private:
    sc_event mAssertIbgErrEvent;
    sc_event mDeassertIbgErrEvent;
    sc_event mUpdateIbgErrEvent;
    void AssertIbgErrMethod();
    void DeassertIbgErrMethod();
    void UpdateIbgErrMethod();

//------------------------------------------------------------------------------
// implement common guard agent controller interface
//------------------------------------------------------------------------------
public:
    bool CheckGuardClock (std::string clock_name);
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
                         ,const uint8_t   SPID
                         );
    bool GetGuardSettings(const string   groupName
                         ,const uint32_t channelId
                         ,bool&     GEN
                         ,bool&     DBG
                         ,bool&     UM
                         ,bool&     WG
                         ,bool&     RG
                         ,uint32_t& SPID);
    void NotifyError();
    uint32_t GetGSPIDERRSTAT();
    uint32_t GetGOVFSTAT();
    uint8_t  GetChannelNum();
    std::string GetGroupName();
}; // end-of-class

#endif /* IBG_U2_IBG_U2_H_ */

//##############################################################################
// EOF
//##############################################################################
