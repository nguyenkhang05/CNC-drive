// ---------------------------------------------------------------------
// $Id: SEG_LT.h,v 1.9 2015/06/12 08:20:40 dungvannguyen Exp $
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
#ifndef __SEG_LT_H__
#define __SEG_LT_H__
#include "BusSlaveBase.h"
#include "SEG_AgentController.h"
#include "SysErrVCI.h"
#include "SysErrTCM.h"
#include "SysErrIPG.h"

class SEG_Func;

class SEG: public sc_module,
           public SEG_AgentController,
           public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {//Define active level of reset signal
        empresetzActive = 0,
        emresetActive = 1
    };

    SEG_Func *mSEG_Func;//SEG_Func class instance
   
    //Declare variables
    std::string mSEGVersion;//Version of SEG model
    sc_dt::uint64 mpclkPeriod;//Period value of pclk clock
    sc_dt::uint64 mclkPeriod;//Period value of clk clock
    sc_dt::uint64 mpclkFreq;//Frequency value of pclk clock
    sc_dt::uint64 mclkFreq;//Frequency value of clk clock
    sc_dt::uint64 mpclkOrgFreq;//Orginal frequency value of pclk clock
    sc_dt::uint64 mclkOrgFreq;//Orginal frequency value of clk clock
    std::string mpclkFreqUnit;//Frequency unit of pclk clock
    std::string mclkFreqUnit;//Frequency unit of clk clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation

    bool mpresetzSignalCurrentValue;//Store current value of presetz signal
    bool mpresetzSignalPreviousValue;//Store previous value of presetz signal
    bool mIspresetzHardActive;//Reset operation status of presetz signal
    bool mIspresetzCmdReceive;//Receive status of AssertReset command of presetz signal
    bool mIspresetzCmdActive;//Active status of AssertReset command of presetz signal
    double mpresetzCmdPeriod;//Period of AssertReset command of presetz signal

    bool mresetSignalCurrentValue;//Store current value of reset signal
    bool mresetSignalPreviousValue;//Store previous value of reset signal
    bool mIsresetHardActive;//Reset operation status of reset signal
    bool mIsresetCmdReceive;//Receive status of AssertReset command of reset signal
    bool mIsresetCmdActive;//Active status of AssertReset command of reset signal
    double mresetCmdPeriod;//Period of AssertReset command of reset signal

    Csyserrvci mVciErrInfoValue;//Error info from VCI
    Csyserrtcm mTcmErrInfoValue;//Error info from TCM
    Csyserripg mIpgErrInfoValue;//Error info from IPG
    unsigned char mErrFactorOutputSignalValue;// The value of error factor
    bool mErrRequestOutputSignalValue;//The value of error request output signal
    bool mIsErrRequestOutputSignal;//Whether error request output signal is controlled
    bool mIsErrFactorOutputSignal;//Whether error factor output signal is controlled

    //Declare events
    sc_event mpresetzHardEvent;//Call HandlepresetzHardMethod() when presetz is active
    sc_event mpresetzCmdEvent;//Call HandlepresetzCmdMethod() when AssertReset of presetz is called
    sc_event mpresetzCmdCancelEvent;//Call CancelpresetzCmdMethod() when AssertReset of presetz is over

    sc_event mresetHardEvent;//Call HandleresetHardMethod() when reset is active
    sc_event mresetCmdEvent;//Call HandleresetCmdMethod() when AssertReset of reset is called
    sc_event mresetCmdCancelEvent;//Call CancelresetCmdMethod() when AssertReset of reset is over

    sc_event mVciErrInfoEvent; //Call HandleVciErrInfoMethod()
    sc_event mTcmErrInfoEvent; //Call HandleTcmErrInfoMethod()
    sc_event mIpgErrInfoEvent; //Call HandleIpgErrInfoMethod()
    sc_event mg1nnSyserrReqNmEvent; //Call Handlecmt1cmSyserrAckNmMethod()
    sc_event mErrorOutputEvent; //Call HanldeErrorOutputMethod()
    sc_event mArbitrateErrorsEvent; //Call ArbitrateErrorsMethod()

public:
    //Declare input signals
    sc_in<sc_dt::uint64> pclk;
    sc_in<sc_dt::uint64> clk;
    sc_in<bool> presetz;
    sc_in<bool> reset;
    sc_in<Csyserrvci> VciErrInfo;
    sc_in<Csyserrtcm> TcmErrInfo;
    sc_in<Csyserripg> IpgErrInfo;
    sc_in<bool> cmt1cmSyserrAckNm;

    //Declare output signals
    sc_out<bool> g1nnSyserrReqNm;
    sc_out<sc_uint<4> > g1nnSyserrCodeNm;
    sc_out<bool> g1nnSyserrEbvNm;

    //Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (SEG);
    SEG (sc_module_name name, 
        unsigned int rLatency,
        unsigned int wLatency,
        std::string SEG_VERSION = "G3M_V30");
    ~SEG (void);
    void start_of_simulation (void);
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
    void SetSEGCONTInitialValue (const unsigned int value);

private:
    void HandlepclkSignalMethod (void);
    void HandleclkSignalMethod (void);
    void HandlepresetzSignalMethod (void);
    void HandleresetSignalMethod (void);
    void HandlepresetzHardMethod (void);
    void HandleresetHardMethod (void);
    void HandlepresetzCmdMethod (void);
    void HandleresetCmdMethod (void);
    void CancelpresetzCmdMethod (void);
    void CancelresetCmdMethod (void);

    void ReceiveVciErrInfoSignalMethod (void);
    void ReceiveTcmErrInfoSignalMethod (void);
    void ReceiveIpgErrInfoSignalMethod (void);
    void Receivecmt1cmSyserrAckNmSignalMethod (void);
    void HandleVciErrInfoMethod (void);
    void HandleTcmErrInfoMethod (void);
    void HandleIpgErrInfoMethod (void);
    void Handlecmt1cmSyserrAckNmMethod (void);
    void HanldeErrorOutputMethod (void);
    void ArbitrateErrorsMethod (void);

    void WriteErrorOutputSignal (const bool is_err_request_output_signal, const bool is_err_factor_output_signal);
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const std::string clock_name, const double time_point);
    bool GetResetStatus (const std::string reset_name);
    //Virtual functions of SEG_AgentController class
    bool CheckClockPeriod (const std::string clock_name);
    void NotifyErrorOutput (const unsigned char index, const bool is_err_request_output_signal);
    void ArbitrateErrors (void);
};
#endif
