// ---------------------------------------------------------------------
// $Id: ECCCom.h,v 1.4 2014/05/30 04:39:24 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECCCOM_H__
#define __ECCCOM_H__
#include "BusSlaveBase.h"
#include "ECCComFunc.h" 
#include "ECCComAgent.h"

class ECCCom: public sc_module,
              public ECCComAgent,
              public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {//Define active level of reset signal
        emResetActive = 0,
        emResetSignalNum = 2
    };
    enum eBankNo {//Define bank memory number
        emBankNo = 4
    };
    enum eFifo {//Define FIFO depth
        emFifoDepth = 4
    };
    enum eErrorType {//Define error type
        emAddrECC2BitError = 0,
        emAddrECC1BitError = 1,
        emDataECC2BitError = 2,
        emDataECC1BitError = 3,
        emAddrParityErrorRead = 4,
        emAddrParityErrorWrite = 5,
        emParityBitError = 6,
        emDataECC2BitErrorExtension = 7,
        emDataECC1BitErrorExtension = 8,
        emErrorTypeNum = 9
    };
    enum eSignalId {//Define signal id
        emADEDERROUT = 0,
        emASEDERROUT = 1,
        emDEDERROUT = 2,
        emSEDERROUT = 3,
        emAPERROUT = 4,
        emBPERROUT = 5,
        emERROVFOUT = 6,
        emMaxSignalID = 7
    };

public:
    //Declare input signals
    sc_in<sc_dt::uint64> pclk;
    sc_in<sc_dt::uint64> sys_clk;
    sc_in<bool> presetz;
    sc_in<bool> sys_resetz;

    //Decalre output signals
    sc_out<bool> aeccdisable;
    sc_out<bool> asecdisable;
    sc_out<bool> eccdisable;
    sc_out<bool> secdisable;

    sc_out<bool> adederrout;
    sc_out<bool> asederrout;
    sc_out<bool> dederrout;
    sc_out<bool> sederrout;
    sc_out<bool> aperrout;
    sc_out<bool> bperrout;
    sc_out<bool> errovfout;

    sc_out<bool> *adederrout_sys[emBankNo];
    sc_out<bool> *asederrout_sys[emBankNo];
    sc_out<bool> *dederrout_sys[emBankNo];
    sc_out<bool> *sederrout_sys[emBankNo];
    sc_out<bool> *aperrout_sys[emBankNo];
    sc_out<bool> *bperrout_sys[emBankNo];
    sc_out<unsigned int> *addressout_sys[emBankNo];

    //Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (ECCCom);
    ECCCom (sc_module_name name, 
        unsigned int rLatency,
        unsigned int wLatency,
        unsigned int CapDepth = 1,
        unsigned int ECCCTLInitial = 0,
        unsigned int ERRINTInitial = 0,
        bool SupDupsed = false,
        bool UeabMode = false);
    ~ECCCom (void);

    //Declare public members
    void IssueError (const unsigned int bank_no, const unsigned int error_type);
    void SetErrorAddr (const unsigned int bank_no, const unsigned int addr_value);
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
    void DumpFileNameLineNum (const std::string is_enable);
    
private:
    //Declare private members
    ECCComFunc *mECCComFunc;//ECCComFunc class instance

    sc_event mPCLKErrorOutputActiveEvent[emMaxSignalID];//Call ActivatePCLKErrorOutputSignalsMethod to control error output signal in pclk domain
    sc_event mSYS_CLKErrorOutputActiveEvent[emMaxSignalID-1][emBankNo];//Call ActivateSYS_CLKErrorOutputSignalsMethod to control error output signal in sys_clk domain
    sc_event mPCLKErrorOutputInactiveEvent[emMaxSignalID];//Call DeactivatePCLKErrorOutputSignalsMethod to control error output signal in pclk domain
    sc_event mSYS_CLKErrorOutputInactiveEvent[emMaxSignalID-1][emBankNo];//Call DeactivateSYS_CLKErrorOutputSignalsMethod to control error output signal in sys_clk domain
    sc_event mPCLKErrorOutputWriteEvent[emMaxSignalID];//Call WritePCLKErrorOutputSignalsMethod to control error output signal in pclk domain
    sc_event mSYS_CLKErrorOutputWriteEvent[emMaxSignalID-1][emBankNo];//Call WriteSYS_CLKErrorOutputSignalsMethod to control error output signal in sys_clk domain
    sc_event mErrorAddrOutputEvent[emBankNo];//Call WriteErrorAddrOutputMethod to write error address output
    sc_event mErrorInputEvent[emBankNo];//Call CaptureErrorInputMethod when an error is detected
    sc_event mIncreaseReqIdEvent[emBankNo];//Call IncreaseReqIdMethod when error address is captured

    sc_event mResetHardEvent[emResetSignalNum];//Call HandleResetHardMethod when presetz or sys_reset is active
    sc_event mResetCmdEvent[emResetSignalNum];//Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];//Call CancelResetCmdMethod when AssertReset is over

    sc_event mECCDisableSignalsControlEvent;//Call ControlECCDisableSignalsMethod() at PCLK posedge
 
    sc_dt::uint64 mPCLKPeriod;//Period value of pclk clock
    sc_dt::uint64 mSYS_CLKPeriod;//Period value of sys_clk clock
    sc_dt::uint64 mPCLKFreq;//Frequency value of pclk clock
    sc_dt::uint64 mPCLKOrgFreq;//Orginal frequency value of pclk clock
    sc_dt::uint64 mSYS_CLKFreq;//Frequency value of sys_clk clock
    sc_dt::uint64 mSYS_CLKOrgFreq;//Orginal frequency value of sys_clk clock
    sc_dt::uint64 mTimeResolutionValue;//Time resolution value of simulation
    std::string mPCLKFreqUnit;//Frequency unit of pclk clock
    std::string mSYS_CLKFreqUnit;//Frequency unit of sys_clk clock
    sc_time_unit mTimeResolutionUnit;//Time resolution unit of simulation

    unsigned char mReqId[emBankNo];//Request ID of FIFO
    unsigned char mRespId[emBankNo];//Responsd ID of FIFO
    ECCComFunc::ErrorInfoStructure mErrorInfo[emBankNo];//Error information structure according to bank number
    unsigned int mErrorAddrInput[emBankNo][emFifoDepth];//Error address input according to bank number
    unsigned int mErrorAddrOutput[emBankNo];//Error address output according to bank number
    bool mPCLKErrorOutputVal[emMaxSignalID];//The value of error output signals in pclk domain
    bool mSYS_CLKErrorOutputVal[emMaxSignalID-1][emBankNo];//The value of error output signals in sys_clk domain

    bool mResetSignalCurrentValue[emResetSignalNum];//Store current value of presetz or sys_resetz
    bool mResetSignalPreviousValue[emResetSignalNum];//Store previous value of presetz or sys_resetz
    bool mIsResetHardActive[emResetSignalNum];//Reset operation status of presetz or sys_resetz signal
    bool mIsResetCmdReceive[emResetSignalNum];//Receive status of AssertReset command of presetz or sys_resetz
    bool mIsResetCmdActive[emResetSignalNum];//Active status of AssertReset command of presetz or sys_resetz
    double mResetCmdPeriod[emResetSignalNum];//Period of AssertReset command of presetz or sys_resetz

    void ActivatePCLKErrorOutputSignalsMethod (const unsigned int signal_id);
    void ActivateSYS_CLKErrorOutputSignalsMethod (const unsigned int signal_id, const unsigned int bank_no);
    void DeactivatePCLKErrorOutputSignalsMethod (const unsigned int signal_id);
    void DeactivateSYS_CLKErrorOutputSignalsMethod (const unsigned int signal_id, const unsigned int bank_no);
    void WritePCLKErrorOutputSignalsMethod (const unsigned int signal_id);
    void WriteSYS_CLKErrorOutputSignalsMethod (const unsigned int signal_id, const unsigned int bank_no);
    void WriteErrorAddrOutputMethod (const unsigned int bank_no);
    void CaptureErrorInputMethod (const unsigned int bank_no);
    void IncreaseReqIdMethod (const unsigned int bank_no);

    void HandlePCLKSignalMethod (void);
    void HandleSYS_CLKSignalMethod (void);

    void HandlePRESETZSignalMethod (void);
    void HandleSYS_RESETZSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    void ControlECCDisableSignalsMethod (void);

    void Initialize (void);
    void InitializeOutputSignals (void);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void CheckFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                    sc_dt::uint64 freq_in, std::string unit_in);
    bool CheckClockPeriod (const std::string clock_name);
    double CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void EnableReset (const bool is_active, const unsigned int reset_id);
    void ControlECCDisableSignals (const unsigned int eccctl_value);

    //Declare virtual functions of ECCComAgent class
    void ControlPCLKErrorOutputSignals (const std::string signal_name);
    void ControlSYS_CLKErrorOutputSignals (const std::string signal_name, const unsigned int bank_no, const unsigned int error_addr);
    void WaitPCLKPosEdgeToControlECCDisableSignals (void);
};
#endif
