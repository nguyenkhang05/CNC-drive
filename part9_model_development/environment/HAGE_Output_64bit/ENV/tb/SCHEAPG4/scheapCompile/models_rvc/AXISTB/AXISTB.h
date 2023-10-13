// ---------------------------------------------------------------------
// $Id: AXISTB.h,v 1.3 2014/05/05 01:23:39 duymai Exp $
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
#ifndef __AXISTB_H__
#define __AXISTB_H__
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"
#include "AXISTBFunc.h" 
#include "AXISTB_AgentController.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;
class AXISTB_DataHandler;

class AXISTB: public sc_module,
           public AXISTB_AgentController,
           public BusBridgeSlaveBase<64,2>,
           public BusMasterBase<64,2>,
           public BusSlaveBase<32,1>
{
private:
    enum eRstActive {//Define active level of reset signal
        emRstActive = 0,
        emRstInActive = 1
    };
    enum eBufferSize {
        emDefaultBufferSize = 8
    };
    struct BufferData {
        bool is_write;
        TlmBasicPayload *trans;
        BusTime_t transtime;
        BufferData(void)
        {
            this->is_write = true;
            this->trans = NULL;
            this->transtime = SC_ZERO_TIME;
        }
    };
    std::queue<BufferData> mBufferGRI;
    std::queue<BufferData> mBufferSIC;
    unsigned int mBufferSize[2];

    unsigned int mTransLTConter[2];
    
    sc_event *mGRICompLTEvent;
    sc_event *mSICCompLTEvent;

    std::queue<sc_event* > mEventQueueGRI;
    std::queue<sc_event* > mEventQueueSIC;

    //Declare private members
    AXISTBFunc *mAXISTBFunc;//AXISTBFunc class instance
    AXISTB_DataHandler *mAXISTBDataHandlerGRI;
    AXISTB_DataHandler *mAXISTBDataHandlerSIC;
    BusBridgeModuleBase<64,64> *mBusBridgeModule[2];
    sc_signal<bool> mresetPortSig;
   
    //Declare variables
    bool mIsStartSim;
    sc_dt::uint64 mPCLKPeriod;//Period value of pclk clock
    sc_dt::uint64 mHCLKPeriod;//Period value of lvcclk clock
    sc_dt::uint64 mPCLKFreq;//Frequency value of pclk clock
    sc_dt::uint64 mHCLKFreq;//Frequency value of lvcclk clock
    sc_dt::uint64 mPCLKOrgFreq;//Orginal frequency value of pclk clock
    sc_dt::uint64 mHCLKOrgFreq;//Orginal frequency value of lvcclk clock
    std::string mPCLKFreqUnit;//Frequency unit of pclk clock
    std::string mHCLKFreqUnit;//Frequency unit of lvcclk clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation

    bool mPRESETnSignalCurrentValue;//Store current value of presetz signal
    bool mPRESETnSignalPreviousValue;//Store previous value of presetz signal
    bool mIsPRESETnHardActive;//Reset operation status of presetz signal
    bool mIsPRESETnCmdReceive;//Receive status of AssertReset command of presetz signal
    bool mIsPRESETnCmdActive;//Active status of AssertReset command of presetz signal
    double mPRESETnCmdPeriod;//Period of AssertReset command of presetz signal

    bool mHRESETnSignalCurrentValue;//Store current value of HRESETn signal
    bool mHRESETnSignalPreviousValue;//Store previous value of HRESETn signal
    bool mIsHRESETnHardActive;//Reset operation status of HRESETn signal
    bool mIsHRESETnCmdReceive;//Receive status of AssertReset command of HRESETn signal
    bool mIsHRESETnCmdActive;//Active status of AssertReset command of HRESETn signal
    double mHRESETnCmdPeriod;//Period of AssertReset command of HRESETn signal

    //Declare events
    sc_event mPRESETnHardEvent;//Call HandlepresetzHardMethod() when presetz is active
    sc_event mPRESETnCmdEvent;//Call HandlepresetzCmdMethod() when AssertReset of presetz is called
    sc_event mPRESETnCmdCancelEvent;//Call CancelpresetzCmdMethod() when AssertReset of presetz is over

    sc_event mHRESETnHardEvent;//Call HandleHRESETnHardMethod() when HRESETn is active
    sc_event mHRESETnCmdEvent;//Call HandleHRESETnCmdMethod() when AssertReset of HRESETn is called
    sc_event mHRESETnCmdCancelEvent;//Call CancelHRESETnCmdMethod() when AssertReset of HRESETn is over

    sc_event mGetDataFromBufGRIEvent;
    sc_event mGetDataFromBufSICEvent;
    sc_event mNotifyGRITransCompEvent;
    sc_event mNotifySICTransCompEvent;
    sc_event mNotifyGRITransCancelEvent;
    sc_event mNotifySICTransCancelEvent;
    sc_event mGRIContinueTransferringEvent;
    sc_event mSICContinueTransferringEvent;
    sc_event mGRITransCompEvent;
    sc_event mSICTransCompEvent;

    sc_event mHRESETnEvent;

public:
    //Declare input signals
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> HCLK;
    sc_in<bool> PRESETn;
    sc_in<bool> HRESETn;

    //Declare target socket
    TlmTargetSocket<32> *tsp;

    TlmTargetSocket<64> *tsg;
    TlmTargetSocket<64> *tss;
    TlmInitiatorSocket<64> *isg;
    TlmInitiatorSocket<64> *iss;

    SC_HAS_PROCESS (AXISTB);
    AXISTB (sc_module_name name, 
         unsigned int rLatency,
         unsigned int wLatency);
    ~AXISTB (void);
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
    void SetBufferSize(const std::string name, const int size);

    void setTlmTransType (SIM_MODE_T simMode);
private:
    void HandlePCLKSignalMethod (void);
    void HandleHCLKSignalMethod (void);
    void HandlePRESETnSignalMethod (void);
    void HandleHRESETnSignalMethod (void);
    void HandlePRESETnHardMethod (void);
    void HandleHRESETnHardMethod (void);
    void HandlePRESETnCmdMethod (void);
    void HandleHRESETnCmdMethod (void);
    void CancelPRESETnCmdMethod (void);
    void CancelHRESETnCmdMethod (void);
    void NotifyGRITransferringCompleteMethod(void);
    void NotifySICTransferringCompleteMethod(void);
    void NotifyGRITransferringCancelMethod(void);
    void NotifySICTransferringCancelMethod(void);

    void SendDataFromBufGRIThread(void);
    void SendDataFromBufSICThread(void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateCLKPosEdge (const std::string clock_name, const double time_point);
    void SendDataToDataHandler(const std::string name);
    //Virtual functions of AXISTB_AgentController class
    bool CheckClockPeriod (const std::string clock_name);
    bool GetHRESETZResetStatus (void);
    bool PushDataToBuffer(const std::string name, const bool is_write, TlmBasicPayload &trans, const BusTime_t trans_time);
    void ContinueTransferring(const std::string name);
    void NotifyTransComp(const std::string name);
    void NotifyTransCancel(const std::string name);

    void start_of_simulation(void);
    void TransferLT(const std::string name, const bool is_write, TlmBasicPayload &trans, const BusTime_t trans_time);
};
#endif
