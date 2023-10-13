// ---------------------------------------------------------------------
// $Id: CADC.h,v 1.1 2018/11/19 02:50:58 chanle Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __CADC_H__
#define __CADC_H__

#include "BusSlaveBase.h"
#include "CADC_AgentController.h"
#include <map>

class CADC_Func;

class CADC: public sc_module,
            public CADC_AgentController,
            public BusSlaveBase<32,1>
{
friend class CADC_UNIT;
friend class CADC_UNIT_Func;

private:
    enum eResetActiveLevel {    // Define active level of reset signal
        emResetActive = 0,
        emResetSignalNum = 2
    };

    CADC_Func *mCADC_Func;            // CADC_Func class instance

    /// Declare variables
    sc_dt::uint64 mCLK_HSBPeriod;          //Period value of CLK_HSB clock
    sc_dt::uint64 mCLK_HSBFreq;            //Frequency value of CLK_HSB clock
    sc_dt::uint64 mCLK_HSBOrgFreq;         //Original frequency value of CLK_HSB clock
    std::string mCLK_HSBFreqUnit;          //Frequency unit of CLK_HSB clock
    sc_dt::uint64 mCLK_LSBPeriod;           //Period value of CLK_LSB clock
    sc_dt::uint64 mCLK_LSBFreq;             //Frequency value of CLK_LSB clock
    sc_dt::uint64 mCLK_LSBOrgFreq;          //Original frequency value of CLK_LSB clock
    std::string mCLK_LSBFreqUnit;           //Frequency unit of CLK_LSB clock
    sc_dt::uint64 mM16Period;
    sc_dt::uint64 mM16Freq;
    sc_dt::uint64 mM16OrgFreq;
    std::string mM16FreqUnit;
    
    sc_dt::uint64 mTimeResolutionValue;     //Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;       //Time resolution unit of simulation

    double              mLSBSyncTPoint;       ///<The time point at which active edge of clock happens
    double              mHSBSyncTPoint;       ///<The time point at which active edge of clock happens
    double              mM16SyncTPoint;       ///<The time point at which active edge of clock happens

    bool mResetCurVal[emResetSignalNum];        //Store current value of PRESETn or resetad_n
    bool mResetPreVal[emResetSignalNum];        //Store previous value of PRESETn or resetad_n
    bool mIsResetHardActive[emResetSignalNum];  //Reset operation status of PRESETn or resetad_n signal
    bool mIsResetCmdReceive[emResetSignalNum];  //Receive status of AssertReset command of PRESETn or resetad_n
    bool mIsResetCmdActive[emResetSignalNum];   //Active status of AssertReset command of PRESETn or resetad_n
    double mResetCmdPeriod[emResetSignalNum];   //Period of AssertReset command of PRESETn or resetad_n

    bool mDumpInterrupt;            // Enable/Disable dumping the interrupt info
    double mADSVCC;
    double mADSVSS;
    double mtW;
    unsigned int mtR;
    unsigned int mtCLB;
    double mtDFAver;
    unsigned int mtSAver;
    double mtDFNotAver;
    unsigned int mtSNotAver;
    double mtCoeff;
    bool mParityError[emADNum];
    bool mIDError[emADNum];
    bool mDisconnectError[emADNum];
    unsigned int mCHNUM;
    
    std::map<std::string, unsigned int> mAdIdMap;

    /// Declare events
    sc_event mResetHardEvent[emResetSignalNum];         //Call HandleResetHardMethod when PRESETn or resetad_n is active
    sc_event mResetCmdEvent[emResetSignalNum];          //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];    //Call CancelResetCmdMethod when AssertReset is over
    sc_event mClkZeroEvent[2];                          //Notify clock is zero

public:
    /// Declare input signals
    sc_in<sc_dt::uint64> CLK_HSB;
    sc_in<sc_dt::uint64> CLK_LSB;
    sc_in<bool> PRESETn;
    sc_in<bool> resetad_n;

    sc_in<double> ADSVREFH;
    sc_in<double> ADSVREFL;
    
    sc_in<double> CAN000P;
    sc_in<double> CAN000N;
    sc_in<double> CAN001P;
    sc_in<double> CAN001N;
    sc_in<double> CAN002P;
    sc_in<double> CAN002N;
    sc_in<double> CAN003P;
    sc_in<double> CAN003N;
    
    sc_in<bool> CADTRG00;
    
    sc_in<bool> CADRGT00;
    
    /// Declare output signals
    sc_out<bool> CADEND00;
    
    sc_out<bool> CADI00;
    
    sc_out<bool> CADE00;
    
    sc_out<bool> CADPE00;
    
    sc_out<bool> CADDFREQ000;
    
    sc_out<sc_uint<4> > CADDFTAG00;
    
    sc_out<sc_uint<16> > CADDFDATA00;
    
    sc_out<bool> CADUE00;
    
    sc_out<bool> CADLE00;
    
    sc_out<bool> *CADCVCI00[emVCNum];

    /// Socket declaration
    TlmTargetSocket<32> *ts;        // For CADC's registers

    SC_HAS_PROCESS (CADC);
    CADC (sc_module_name name,
          unsigned int rLatency,
          unsigned int wLatency,
          unsigned int ch_num);
    ~CADC (void);

    /// PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name);
    void WriteRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const bool is_com_reg, const std::string ad_id, const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);

    void DumpInterrupt (const std::string is_enable);
    void SetVoltage (const std::string name, const double value);
    void SetParameter (const std::string name, const unsigned int value);
    void SetCoefficient (const double value);
    void SetError (const std::string name, const std::string ad_id, const std::string is_err);

private:
    /// Declare methods
    void HandleCLK_HSBSignalMethod (void);
    void HandleCLK_LSBSignalMethod (void);

    void HandlePRESETnSignalMethod (void);
    void HandleResetad_nSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    /// Internal functions
    void EnableReset (const std::string reset_name, const bool is_active);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           sc_dt::uint64 freq_in, std::string unit_in);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    double CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point);

    void CancelEvents (void);
    void Initialize (void);
    void RegisterHandlerWrapper (const bool is_com_reg, const std::string ad_id, const std::vector<std::string> cmd);
    
    /// Virtual functions of CADC_AgentController class
    bool GetResetStatus (const std::string reset_name);
    bool CheckClockPeriod (const std::string clock_name);
};
#endif
