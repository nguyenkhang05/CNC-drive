// ---------------------------------------------------------------------
// $Id: FCLA.h,v 1.2 2014/06/24 06:02:39 dungvannguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __FCLA_H__
#define __FCLA_H__
#include "BusSlaveBase.h"
#include "FCLA_AgentController.h"

class FCLA_Func;

class FCLA: public sc_module,
            public FCLA_AgentController,
            public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {//Define active level of PRESETZ and SMPRESZ ports
        emPRESETZActive = 0,
        emSMPRESZActive = 0
    };
    enum eFilterNum {//Define filter number
        emFilterNum = 8
    };

    FCLA_Func *mFCLA_Func;//FCLA_Func class instance
   
    //Declare variables
    sc_dt::uint64 mPCLKPeriod;//Period value of PCLK clock
    sc_dt::uint64 mSMPCLKPeriod;//Period value of SMPCLK clock
    sc_dt::uint64 mPCLKFreq;//Frequency value of PCLK clock
    sc_dt::uint64 mSMPCLKFreq;//Frequency value of SMPCLK clock
    sc_dt::uint64 mPCLKOrgFreq;//Orginal frequency value of PCLK clock
    sc_dt::uint64 mSMPCLKOrgFreq;//Orginal frequency value of SMPCLK clock
    std::string mPCLKFreqUnit;//Frequency unit of PCLK clock
    std::string mSMPCLKFreqUnit;//Frequency unit of SMPCLK clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation

    bool mPRESETZSignalCurrentValue;//Store current value of PRESETZ signal
    bool mPRESETZSignalPreviousValue;//Store previous value of PRESETZ signal
    bool mIsPRESETZHardActive;//Reset operation status of PRESETZ signal
    bool mIsPRESETZCmdReceive;//Receive status of AssertReset command of PRESETZ signal
    bool mIsPRESETZCmdActive;//Active status of AssertReset command of PRESETZ signal
    double mPRESETZCmdPeriod;//Period of AssertReset command of PRESETZ signal

    bool mSMPRESZSignalCurrentValue;//Store current value of SMPRESZ signal
    bool mSMPRESZSignalPreviousValue;//Store previous value of SMPRESZ signal
    bool mIsSMPRESZHardActive;//Reset operation status of SMPRESZ signal
    bool mIsSMPRESZCmdReceive;//Receive status of AssertReset command of SMPRESZ signal
    bool mIsSMPRESZCmdActive;//Active status of AssertReset command of SMPRESZ signal
    double mSMPRESZCmdPeriod;//Period of AssertReset command of SMPRESZ signal

    bool mFCLATINTRValue[emFilterNum];//Value of FCLATINTR ports
    bool mFCLATINTFValue[emFilterNum];//Value of FCLATINTF ports
    bool mFCLATSOValue[emFilterNum];//Value of FCLATSO ports
    bool mFCLATEDSOValue[emFilterNum];//Value of FCLATEDSO ports

    //Declare events
    sc_event mPRESETZHardEvent;//Call HandlePRESETZHardMethod() when PRESETZ is active
    sc_event mPRESETZCmdEvent;//Call HandlePRESETZCmdMethod() when AssertReset of PRESETZ is called
    sc_event mPRESETZCmdCancelEvent;//Call CancelPRESETZCmdMethod() when AssertReset of PRESETZ is over

    sc_event mSMPRESZHardEvent;//Call HandleSMPRESZHardMethod() when SMPRESZ is active
    sc_event mSMPRESZCmdEvent;//Call HandleSMPRESZCmdMethod() when AssertReset of SMPRESZ is called
    sc_event mSMPRESZCmdCancelEvent;//Call CancelSMPRESZCmdMethod() when AssertReset of SMPRESZ is over

    sc_event mEdgeSelectPortsWriteEvent[emFilterNum];//Call WriteEdgeSelectPortsMethod() to control edge select ports
    sc_event mFCLATSOPortsWriteEvent[emFilterNum];//Call WriteFCLATSOPortsMethod() to control FCLATSO ports
    sc_event mFCLATEDSOPortsWriteEvent[emFilterNum];//Call WriteFCLATEDSOPortsMethod() to control FCLATEDSO ports
    sc_event mFCLATNFSIPortValueCheckAtSyncTimeEvent[emFilterNum];//Call CheckFCLATNFSIPortValueAtSyncTimeMethod() to control FCLATEDSO ports
    sc_event mFCLATEDSOPortActivateEvent[emFilterNum];//Call ActivateFCLATEDSOPortMethod()
    sc_event mFCLATEDSOPortDeactivateEvent[emFilterNum];//Call DeactivateFCLATEDSOPortMethod()
    sc_event mFilterBypassEnableEvent[emFilterNum];//Call DetectFCLATSIPortsMethod()
    sc_event mFilterBypassDisableEvent[emFilterNum];//Call DetectFCLATNFSIPortsMethod()
    sc_event mLevelDetectorEnableEvent[emFilterNum];//Call CheckFCLATNFSIPortValueAtSyncTimeMethod()
    sc_event mLevelDetectorDisableEvent[emFilterNum];//Call DetectFCLATEDSIPortsMethod()

public:
    //Declare input signals
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> SMPCLK;
    sc_in<bool> PRESETZ;
    sc_in<bool> SMPRESZ;
    sc_in<bool> *FCLATSI[emFilterNum];
    sc_in<bool> *FCLATNFSI[emFilterNum];
    sc_in<bool> *FCLATEDSI[emFilterNum];

    //Declare output signals
    sc_out<bool> *FCLATINTR[emFilterNum];
    sc_out<bool> *FCLATINTF[emFilterNum];
    sc_out<bool> *FCLATSO[emFilterNum];
    sc_out<bool> *FCLATEDSO[emFilterNum];

    //Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (FCLA);
    FCLA (sc_module_name name, 
          unsigned int rLatency,
          unsigned int wLatency);
    ~FCLA (void);
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
    void HandlePCLKSignalMethod (void);
    void HandleSMPCLKSignalMethod (void);
    void HandlePRESETZSignalMethod (void);
    void HandleSMPRESZSignalMethod (void);
    void HandlePRESETZHardMethod (void);
    void HandleSMPRESZHardMethod (void);
    void HandlePRESETZCmdMethod (void);
    void HandleSMPRESZCmdMethod (void);
    void CancelPRESETZCmdMethod (void);
    void CancelSMPRESZCmdMethod (void);
    void DetectFCLATSIPortsMethod (const unsigned int filter_no);
    void DetectFCLATNFSIPortsMethod (const unsigned int filter_no);
    void DetectFCLATEDSIPortsMethod (const unsigned int filter_no);
    void WriteEdgeSelectPortsMethod (const unsigned int filter_no);
    void WriteFCLATSOPortsMethod (const unsigned int filter_no);
    void WriteFCLATEDSOPortsMethod (const unsigned int filter_no);
    void CheckFCLATNFSIPortValueAtSyncTimeMethod (const unsigned int filter_no);
    void ActivateFCLATEDSOPortMethod (const unsigned int filter_no);
    void DeactivateFCLATEDSOPortMethod (const unsigned int filter_no);

    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const std::string clock_name, const double time_point);
    //Virtual functions of FCLA_AgentController class
    bool GetResetStatus (const std::string reset_name);
    bool CheckClockPeriod (const std::string clock_name);
    void ControlEdgeSelectPorts (const unsigned int filter_no, const bool FCLATINTR_value, const bool FCLATINTF_value);
    void ControlFilterBypass (const unsigned int filter_no, const bool is_enable);
    void ControlLevelDetector (const unsigned int filter_no, const bool is_enable);
};
#endif
