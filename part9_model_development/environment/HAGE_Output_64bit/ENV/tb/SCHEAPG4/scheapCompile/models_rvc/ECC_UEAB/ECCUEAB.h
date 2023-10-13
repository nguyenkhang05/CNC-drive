// ---------------------------------------------------------------------
// $Id: ECCUEAB.h,v 1.3 2013/11/08 02:23:31 dungvannguyen Exp $
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
#ifndef __ECCUEAB_H__
#define __ECCUEAB_H__
#include "BusSlaveBase.h"
#include "ECCUEABFunc.h" 
#include "ECCUEABAgent.h"
 
class ECCUEAB: public sc_module,
               public ECCUEABAgent,
               public BusSlaveBase<32,1>
{
private:
    enum eResetActiveLevel {//Define active level of reset signal
        emResetActive = 0,
        emResetSignalNum = 2
    };
    enum eInputSignalGroup {//Define signal group
        emSynSYS_CLK = 0,
        emSynBUS_CLK = 1,
        emSignalGroupNum = 2,
        emSignalInGroupNum = 4
    };
    enum eOutputErrorSignal {//Define output error signals
        emADEDERROUT = 0,
        emASEDERROUT = 1,
        emAPEERROUT = 2,
        emDEDERROUT = 3,
        emSEDERROUT = 4,
        emOutputErrorSignalNum = 5
    };

public:
    //Declare input signals
    sc_in<sc_dt::uint64> pclk;
    sc_in<sc_dt::uint64> sys_clk;
    sc_in<sc_dt::uint64> bus_clk;
    sc_in<bool> presetz;
    sc_in<bool> sys_resetz;

    sc_in<bool> *adederrin_sys[emSignalInGroupNum];
    sc_in<bool> *asederrin_sys[emSignalInGroupNum];
    sc_in<bool> *apeerrin_sys[emSignalInGroupNum];
    sc_in<bool> *dederrin_sys[emSignalInGroupNum];
    sc_in<bool> *sederrin_sys[emSignalInGroupNum];
    sc_in<unsigned int> *addressin_sys[emSignalInGroupNum];

    sc_in<bool> *adederrin_bus[emSignalInGroupNum];
    sc_in<bool> *asederrin_bus[emSignalInGroupNum];
    sc_in<bool> *apeerrin_bus[emSignalInGroupNum];
    sc_in<bool> *dederrin_bus[emSignalInGroupNum];
    sc_in<bool> *sederrin_bus[emSignalInGroupNum];
    sc_in<unsigned int> *addressin_bus[emSignalInGroupNum];
  
    //Declare output signals  
    sc_out<bool> errovfout;
    sc_out<bool> adederrout;
    sc_out<bool> asederrout;
    sc_out<bool> apeerrout;
    sc_out<bool> dederrout;
    sc_out<bool> sederrout;

    //Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (ECCUEAB);
    ECCUEAB (sc_module_name name, 
        unsigned int rLatency,
        unsigned int wLatency,
        unsigned int CapDepth = 1,
        unsigned int ERRINTInitial = 0);
    ~ECCUEAB (void);

    //Declare public members
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
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
    ECCUEABFunc *mECCUEABFunc;
   
    sc_event mDoubleErrorCaptureEvent;//Call CaptureDoubleErrorMethod to store error information
    sc_event mEachDataECC1BitErrCaptureEvent[emSignalGroupNum][emSignalInGroupNum];//Call CaptureEachDataECC1BitErrMethod to store error information
    sc_event mAllDataECC1BitErrCaptureEvent;//Call CaptureAllDataECC1BitErrMethod to store error information
 
    sc_event mADEDERROUTReportEvent[emSignalGroupNum][emSignalInGroupNum];//Call ReportAddr2BitErrorMethod to process address 2bit error
    sc_event mASEDERROUTReportEvent[emSignalGroupNum][emSignalInGroupNum];//Call ReportAddr1BitErrorMethod to process address 1bit error
    sc_event mAPEERROUTReportEvent[emSignalGroupNum][emSignalInGroupNum];//Call ReportAddrParityErrorMethod to process address parity error
    sc_event mDEDERROUTReportEvent[emSignalGroupNum][emSignalInGroupNum];//Call ReportData2BitErrorMethod to process data 2bit error
    sc_event mSEDERROUTReportEvent[emSignalGroupNum][emSignalInGroupNum];//Call ReportData1BitErrorMethod to process data 1bit error
    sc_event mOverflowSignalReportEvent[emSignalGroupNum][emSignalInGroupNum];//Call ReportOverflowSignalMethod to report error overflow
    sc_event mOutputErrorSignalsClearEvent[emOutputErrorSignalNum];//Call ClearOutputErrorSignalsMethod to clear output signals
    sc_event mOverflowSignalClearEvent;//Call ClearOverflowSignalMethod to clear error overflow signal
    sc_event mOutputErrorSignalsWriteEvent[emOutputErrorSignalNum];//Call WriteOutputErrorSignalsMethod to write the value to output signals
    sc_event mOverflowSignalWriteEvent;//Call WriteOverflowSignalMethod to write the value to error overflow signal

    sc_event mResetHardEvent[emResetSignalNum];//Call HandleResetHardMethod when presetz or sys_resetz is active
    sc_event mResetCmdEvent[emResetSignalNum];//Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];//Call CancelResetCmdMethod when AssertReset is over

    ECCUEABFunc::ErrorInfoStructure mDoubleErrInfo[emSignalGroupNum][emSignalInGroupNum];//Double error information
    ECCUEABFunc::ErrorInfoStructure mDataECC1BitErrInfo[emSignalGroupNum][emSignalInGroupNum];//Data ECC 1bit error information
    ECCUEABFunc::ErrorInfoStructure mDoubleErrorCapturedInfo;//The captured information of double errors

    sc_dt::uint64 mPCLKPeriod; //period value of pclk clock
    sc_dt::uint64 mSYS_CLKPeriod; //Period value of sys_clk clock
    sc_dt::uint64 mBUS_CLKPeriod; //Period value of bus_clk clock
    sc_dt::uint64 mPCLKFreq; //Frequency value of pclk clock
    sc_dt::uint64 mPCLKOrgFreq; //Orginal frequency value of pclk clock
    sc_dt::uint64 mSYS_CLKFreq; //Frequency value of sys_clk clock
    sc_dt::uint64 mSYS_CLKOrgFreq; //Orginal frequency value of sys_clk clock
    sc_dt::uint64 mBUS_CLKFreq; //Frequency value of bus_clk clock
    sc_dt::uint64 mBUS_CLKOrgFreq; //Orginal frequency value of bus_clk clock
    sc_dt::uint64 mTimeResolutionValue; //Time resolution value of simulation
    std::string mPCLKFreqUnit; //Frequency unit of pclk clock
    std::string mSYS_CLKFreqUnit; //Frequency unit of sys_clk clock
    std::string mBUS_CLKFreqUnit; //Frequency unit of bus_clk clock
    sc_time_unit mTimeResolutionUnit; //Time resolution unit of simulation
    bool mIsCapturingDoubleError;//Status of double error capture

    bool mOutputErrorSignalsVal[emOutputErrorSignalNum];//The value of output signals
    bool mOverflowSignalVal;//The value of error overflow signal
    bool mDataECC1BitErrInputsInfo[emSignalGroupNum][emSignalInGroupNum];//The status of data ecc 1bit error input

    bool mResetSignalCurrentValue[emResetSignalNum];//Current value of presetz or sys_resetz
    bool mResetSignalPreviousValue[emResetSignalNum];//Previous value of presetz or sys_resetz
    bool mIsResetHardActive[emResetSignalNum];//Reset operation status of presetz or sys_resetz signal
    bool mIsResetCmdReceive[emResetSignalNum];//Receiving status of AssertReset command of presetz or sys_resetz
    bool mIsResetCmdActive[emResetSignalNum];//Active status of AssertReset command of presetz or sys_resetz
    double mResetCmdPeriod[emResetSignalNum];//Period of AssertReset command of presetz or sys_resetz

    void HandlePCLKSignalMethod (void);
    void HandleSYS_CLKSignalMethod (void);
    void HandleBUS_CLKSignalMethod (void);

    void HandleECC1BitErrorInputSignalsMethod (void);
    void HandleDoubleErrorInputSignalsMethod (void);
    void CaptureDoubleErrorMethod (void);
    void CaptureEachDataECC1BitErrMethod (const unsigned int signal_group, const unsigned int signal_id);
    void CaptureAllDataECC1BitErrMethod (void);
    void ReportAddr2BitErrorMethod (void);
    void ReportAddr1BitErrorMethod (void);
    void ReportAddrParityErrorMethod (void);
    void ReportData2BitErrorMethod (void);
    void ReportData1BitErrorMethod (void);
    void ReportOverflowSignalMethod (void);

    void ClearOutputErrorSignalsMethod (const unsigned int signal_id);
    void ClearOverflowSignalMethod (void);
    void WriteOutputErrorSignalsMethod (const unsigned int signal_id);
    void WriteOverflowSignalMethod (void);

    void HandlePRESETZSignalMethod (void);
    void HandleSYS_RESETZSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    void Initialize (void);
    void InitializeOutputSignals (void);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void CheckFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                    sc_dt::uint64 freq_in, std::string unit_in);
    double CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void EnableReset (const bool is_active, const unsigned int reset_id);

    //Declare virtual functions of ECCUEABAgent class
    bool CheckClockPeriod (std::string clock_name);
};
#endif
