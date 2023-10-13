// ---------------------------------------------------------------------
// $Id: IPG_LT.h,v 1.4 2015/06/12 08:19:09 dungvannguyen Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __IPG_LT_H__
#define __IPG_LT_H__
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"
#include "IPG_AgentController.h"
#include "SysErrIPG.h"

template<unsigned int BUSWIDTH_M, unsigned int BUSWIDTH_S> class BusBridgeModuleBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase; 
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;
class IPG_DataHandlerLSU;
class IPG_DataHandlerIFU;
class IPG_Func;

class IPG: public sc_module,
           public IPG_AgentController,
           public BusBridgeSlaveBase<32,2>,
           public BusMasterBase<32,1>,
           public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {//Define active level of reset signal
        empresetzActive = 0,
        emlvcirresetActive = 1
    };

    //Declare private members
    IPG_Func *mIPG_Func;//IPG_Func class instance
    BusBridgeModuleBase<32,32> *mBusBridgeModule[2];
    IPG_DataHandlerLSU *mIPG_DataHandlerLSU;
    IPG_DataHandlerIFU *mIPG_DataHandlerIFU;
    sc_signal<bool> mresetPortSig;
   
    //Declare variables
    sc_dt::uint64 mpclkPeriod;//Period value of pclk clock
    sc_dt::uint64 mlvcclkPeriod;//Period value of lvcclk clock
    sc_dt::uint64 mpclkFreq;//Frequency value of pclk clock
    sc_dt::uint64 mlvcclkFreq;//Frequency value of lvcclk clock
    sc_dt::uint64 mpclkOrgFreq;//Orginal frequency value of pclk clock
    sc_dt::uint64 mlvcclkOrgFreq;//Orginal frequency value of lvcclk clock
    std::string mpclkFreqUnit;//Frequency unit of pclk clock
    std::string mlvcclkFreqUnit;//Frequency unit of lvcclk clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation

    bool mpresetzSignalCurrentValue;//Store current value of presetz signal
    bool mpresetzSignalPreviousValue;//Store previous value of presetz signal
    bool mIspresetzHardActive;//Reset operation status of presetz signal
    bool mIspresetzCmdReceive;//Receive status of AssertReset command of presetz signal
    bool mIspresetzCmdActive;//Active status of AssertReset command of presetz signal
    double mpresetzCmdPeriod;//Period of AssertReset command of presetz signal

    bool mlvcirresetSignalCurrentValue;//Store current value of lvcirreset signal
    bool mlvcirresetSignalPreviousValue;//Store previous value of lvcirreset signal
    bool mIslvcirresetHardActive;//Reset operation status of lvcirreset signal
    bool mIslvcirresetCmdReceive;//Receive status of AssertReset command of lvcirreset signal
    bool mIslvcirresetCmdActive;//Active status of AssertReset command of lvcirreset signal
    double mlvcirresetCmdPeriod;//Period of AssertReset command of lvcirreset signal

    bool mIsErrorProcessing;//Whether the mode is processing an error

    Csyserripg mIpgErrInfoValue;//Value of output port IpgErrInfo

    //Declare events
    sc_event mpresetzHardEvent;//Call HandlepresetzHardMethod() when presetz is active
    sc_event mpresetzCmdEvent;//Call HandlepresetzCmdMethod() when AssertReset of presetz is called
    sc_event mpresetzCmdCancelEvent;//Call CancelpresetzCmdMethod() when AssertReset of presetz is over

    sc_event mlvcirresetHardEvent;//Call HandlelvcirresetHardMethod() when lvcirreset is active
    sc_event mlvcirresetCmdEvent;//Call HandlelvcirresetCmdMethod() when AssertReset of lvcirreset is called
    sc_event mlvcirresetCmdCancelEvent;//Call CancellvcirresetCmdMethod() when AssertReset of lvcirreset is over

    sc_event mSendOutputErrorEvent;//Call SendOutputErrorMethod() to write the value to port IpgErrInfo
    sc_event mClearOutputErrorEvent;//Call ClearOutputErrorMethod() to write the value to port IpgErrInfo

public:
    //Declare input signals
    sc_in<sc_dt::uint64> pclk;
    sc_in<sc_dt::uint64> lvcclk;
    sc_in<bool> presetz;
    sc_in<bool> lvcirreset;
    sc_out<Csyserripg> IpgErrInfo;

    //Declare target socket
    TlmTargetSocket<32> *tsp;
    TlmTargetSocket<32> *tsl;
    TlmTargetSocket<32> *tsi;
    TlmInitiatorSocket<32> *is;

    SC_HAS_PROCESS (IPG);
    IPG (sc_module_name name, 
         unsigned int rLatency,
         unsigned int wLatency,
         std::string IPG_VERSION = "G3M_V30");
    ~IPG (void);
    void setTlmTransType (SIM_MODE_T simMode);
    void SetGuardAreas (const std::string action, const std::string area_name,
                        const unsigned int start_addr, const unsigned int size);
    void DumpStatusInfo (void);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);

private:
    void HandlepclkSignalMethod (void);
    void HandlelvcclkSignalMethod (void);
    void HandlepresetzSignalMethod (void);
    void HandlelvcirresetSignalMethod (void);
    void HandlepresetzHardMethod (void);
    void HandlelvcirresetHardMethod (void);
    void HandlepresetzCmdMethod (void);
    void HandlelvcirresetCmdMethod (void);
    void CancelpresetzCmdMethod (void);
    void CancellvcirresetCmdMethod (void);
    void SendOutputErrorMethod (void);
    void ClearOutputErrorMethod (void);

    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const std::string clock_name, const double time_point);
    ////Virtual functions of IPG_AgentController class
    bool CheckClockPeriod (const std::string clock_name);
    bool GetResetStatus (void);
    bool CheckAccessPermission (const bool is_ifu, TlmBasicPayload& trans);
    void SendOutputErrorPort (const Csyserripg err_port_value);
};
#endif
