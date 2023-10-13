// ---------------------------------------------------------------------
// $Id: DNF.h,v 1.3 2020/02/05 04:31:04 ngathunguyen Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __DNF_H__
#define __DNF_H__
#include "BusSlaveBase.h"
#include "DNF_AgentController.h"
#include "rvc_common_model.h"

#define MAX_CHANNEL_NUM 16

class DNF_Func;

class DNF:  public sc_module,
            public DNF_AgentController,
            public BusSlaveBase<32,1>,
            public rvc_common_model
{
private:
    enum eResetActiveLevel {//Define active level of reset signal
        emResetActive = 0,
        emResetInactive = 1
    };
    enum eEdgeDetection { //Define type of edge detction
        emNoEdge = 0,
        emFallingEdge = 1,
        emRaisingEdge = 2,
        emBothEdge = 3
    };
    enum eClockInfo{
        emPCLKId                = 0
        ,emDNFATCKIId           = 1
        ,emNumClk               = 2
    };

    //Declare variables
    DNF_Func *mDNF_Func;//Instance of DNF_Func class
   
    bool mIsStartSimulation; 
    bool mIsOperating;
    unsigned int mChannelNumber;
    unsigned char* mEdgeDetection;
    bool* mNFOUTValue;
    bool* mINTValue;

    sc_dt::uint64 mPCLKPeriod;//Period value of PCLK clock
    sc_dt::uint64 mDNFATCKIPeriod;//Period value of DNFATCKI clock
    sc_dt::uint64 mPCLKFreq;//Frequency value of PCLK clock
    sc_dt::uint64 mDNFATCKIFreq;//Frequency value of DNFATCKI clock
    sc_dt::uint64 mPCLKOrgFreq;//Orginal frequency value of PCLK clock
    sc_dt::uint64 mDNFATCKIOrgFreq;//Orginal frequency value of DNFATCKI clock
    std::string mPCLKFreqUnit;//Frequency unit of PCLK clock
    std::string mDNFATCKIFreqUnit;//Frequency unit of DNFATCKI clock
    double mStartPCLKTime;// Start time of PCLK clock
    double mStartDNFATCKITime;// Start time of DNFATCKI clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation

    bool mPRESETZSignalCurrentValue;//Store current value of PRESETZ signal
    bool mPRESETZSignalPreviousValue;//Store previous value of PRESETZ signal
    bool mIsPRESETZHardActive;//Reset operation status of PRESETZ signal
    bool mIsPRESETZCmdReceive;//Receive status of AssertReset command of PRESETZ signal
    bool mIsPRESETZCmdActive;//Active status of AssertReset command of PRESETZ signal
    double mPRESETZCmdPeriod;//Period of AssertReset command of PRESETZ signal

    bool mDNFATRESZSignalCurrentValue;//Store current value of DNFATRESZ signal
    bool mDNFATRESZSignalPreviousValue;//Store previous value of DNFATRESZ signal
    bool mIsDNFATRESZHardActive;//Reset operation status of DNFATRESZ signal
    bool mIsDNFATRESZCmdReceive;//Receive status of AssertReset command of DNFATRESZ signal
    bool mIsDNFATRESZCmdActive;//Active status of AssertReset command of DNFATRESZ signal
    double mDNFATRESZCmdPeriod;//Period of AssertReset command of DNFATRESZ signal

    sc_event mPRESETZHardEvent;//Call HandlePRESETZHardMethod() when PRESETZ is active
    sc_event mPRESETZCmdEvent;//Call HandlePRESETZCmdMethod() when AssertReset of PRESETZ is called
    sc_event mPRESETZCmdCancelEvent;//Call CancelPRESETZCmdMethod() when AssertReset of PRESETZ is over

    sc_event mDNFATRESZCmdEvent;//Call HandleDNFATRESZCmdMethod() when AssertReset of DNFATRESZ is called
    sc_event mDNFATRESZCmdCancelEvent;//Call CancelDNFATRESZCmdMethod() when AssertReset of DNFATRESZ is over
    sc_event mDNFATRESZHardEvent;// Call HandleDeassertDNFATRESZHardMethod when deasserting DNFATRESZ

    sc_event mReadDNFATFINEvent[MAX_CHANNEL_NUM];
    sc_event mReadDNFATRINEvent[MAX_CHANNEL_NUM];
    sc_event mReadDNFATDGINEvent[MAX_CHANNEL_NUM];
    sc_event mReadDNFATANINEvent[MAX_CHANNEL_NUM];
    sc_event mReadDNFATAEDEvent[MAX_CHANNEL_NUM];
    sc_event mWriteDNFATINTEvent[MAX_CHANNEL_NUM];
    sc_event mClearDNFATINTEvent[MAX_CHANNEL_NUM];
    sc_event mWriteDNFATNFOUTEvent[MAX_CHANNEL_NUM];
    sc_event mSynchronizingEvent[MAX_CHANNEL_NUM];
    sc_event mGetDGINSampleEvent[MAX_CHANNEL_NUM];
    sc_event mGetNextDGINSampleEvent[MAX_CHANNEL_NUM];

public:
    //Declare input/output ports
    sc_in<bool> PRESETZ;
    sc_in<bool> DNFATRESZ;
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> DNFATCKI;
    sc_in<bool> *DNFATFIN[MAX_CHANNEL_NUM];
    sc_in<bool> *DNFATRIN[MAX_CHANNEL_NUM];
    sc_in<bool> *DNFATDGIN[MAX_CHANNEL_NUM];
    sc_in<bool> *DNFATANIN[MAX_CHANNEL_NUM];
    sc_in<bool> *DNFATAED[MAX_CHANNEL_NUM];
    sc_out<bool> *DNFATINT[MAX_CHANNEL_NUM];
    sc_out<bool> *DNFATNFOUT[MAX_CHANNEL_NUM];

    //Declare target sockets
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (DNF);
    DNF (sc_module_name name, 
               unsigned int rLatency,
               unsigned int wLatency,
               unsigned int channel_num);
    ~DNF (void);
    //Declare public members
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void EnableRegisterMessage(const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);

private:
    bool mEnableRegisterMessage;
    void HandleDNFATFINMethod(const unsigned int channel_id);
    void HandleDNFATRINMethod(const unsigned int channel_id);
    void HandleDNFATDGINMethod(const unsigned int channel_id);
    void HandleDNFATANINMethod(const unsigned int channel_id);
    void HandleDNFATAEDMethod(const unsigned int channel_id);
    void WriteDNFATINTMethod(const unsigned int channel_id);
    void ClearDNFATINTMethod(const unsigned int channel_id);
    void WriteDNFATNFOUTMethod(const unsigned int channel_id);
    void HandleSynchronizedSignalMethod(const unsigned int channel_id);
    void ProcessDGINMethod(const unsigned int channel_id);
    
    void HandlePCLKSignalMethod (void);
    void HandleDNFATCKISignalMethod (void);
    void HandlePRESETZSignalMethod (void);
    void HandleDNFATRESZSignalMethod (void);
    void HandlePRESETZHardMethod (void);
    void HandleDNFATRESZHardMethod (void);
    void HandlePRESETZCmdMethod (void);
    void HandleDNFATRESZCmdMethod (void);
    void CancelPRESETZCmdMethod (void);
    void CancelDNFATRESZCmdMethod (void);
    bool CheckBasicCondition(const unsigned int channel_id);
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    double CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point);
    void SetEdgeDetection(const unsigned int channel_id, const bool is_FIN, const bool value);
    double CalculateSamplingClock(void);
    void UpdateNFOUTValue(const unsigned int channel_id, const bool value);
    void UpdateINTValue(const unsigned int channel_id, const bool value);
    void ControlINTport(const unsigned int channel_id, const bool cur_NFOUT_val, const bool pre_val);
    void StartOperation(void);
    void StopOperation(void);
    void start_of_simulation(void);

    //Virtual functions of AHBAgentController_UIMPL class
    bool CheckClock(const std::string clock_name);
    bool GetResetStatus(const std::string reset_name);
    void SwitchToEnableMode(const unsigned int channel_id);
    void SwitchToDisableMode(const unsigned int channel_id);
    void OutputDNFATNFOUT(const unsigned int channel_id, const bool value);
    void GetNextSample(const unsigned int channel_id);
};
#endif //__DNF_H__
// vim600: set foldmethod=marker :
