// ---------------------------------------------------------------------
// $Id: DSADC.h 1398 2019-11-26 06:16:27Z chuonghoangle $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __DSADC_H__
#define __DSADC_H__

#include "BusSlaveBase.h"
#include <map>
#include "DSADC_AgentController.h"
#include "rvc_common_model.h"

class DSADC_Func;

class DSADC: public sc_module,
            public DSADC_AgentController,
            public rvc_common_model,
            public BusSlaveBase<32,1>
{

private:
    
    DSADC_Func *mDSADC_Func;                    /// DSADC_Func class instance
                                                    
    /// Declare variables                           
    sc_dt::uint64 mCLK_HSBFreq;                 /// Frequency value of CLK_HSB clock
    sc_dt::uint64 mCLK_HSBOrgFreq;              /// Original frequency value of CLK_HSB clock
    std::string mCLK_HSBFreqUnit;               /// Frequency unit of CLK_HSB clock
    sc_dt::uint64 mCLK_LSBFreq;                 /// Frequency value of CLK_LSB clock
    sc_dt::uint64 mCLK_LSBOrgFreq;              /// Original frequency value of CLK_LSB clock
    std::string mCLK_LSBFreqUnit;               /// Frequency unit of CLK_LSB clock                                              
    double mCLK_LSBSyncTPoint;                  /// The time point at which active edge of clock happens
    double mCLK_HSBSyncTPoint;                  /// The time point at which active edge of clock happens
    double mCLK_FosSyncTPoint;                  /// The time point at which active edge of clock happens                                                                                                
    sc_dt::uint64 mFosOrgFreq;                      
    std::string mFosFreqUnit;                       
    sc_dt::uint64 mTimeResolutionValue;         /// Time resolution value of simulation                                            
    bool mResetCurVal[emResetSignalNum];        /// Store current value of PRESETn or resetad_n
    bool mResetPreVal[emResetSignalNum];        /// Store previous value of PRESETn or resetad_n
    bool mIsResetHardActive[emResetSignalNum];  /// Reset operation status of PRESETn or resetad_n signal
    bool mIsResetCmdReceive[emResetSignalNum];  /// Receive status of AssertReset command of PRESETn or resetad_n
    bool mIsResetCmdActive[emResetSignalNum];   /// Active status of AssertReset command of PRESETn or resetad_n
    double mResetCmdPeriod[emResetSignalNum];   /// Period of AssertReset command of PRESETn or resetad_n    
    std::map<std::string, unsigned int> mAdIdMap;

public:
    /// Declare input signals
    sc_in<sc_dt::uint64> CLK_HSB;
    sc_in<bool> resetad_n;
    sc_in<double> ADSVREFH;
    sc_in<double> ADSVREFL;

    sc_in<double> DSAN000P;
    sc_in<double> DSAN000N;
    sc_in<double> DSAN001P;
    sc_in<double> DSAN001N;
    sc_in<double> DSAN002P;
    sc_in<double> DSAN002N;
    sc_in<double> DSAN003P;
    sc_in<double> DSAN003N;

    sc_in<double> DSAN100P;
    sc_in<double> DSAN100N;
    sc_in<double> DSAN101P;
    sc_in<double> DSAN101N;

    sc_in<double> DSAN200P;
    sc_in<double> DSAN200N;

    sc_in<double> DSAN120P;
    sc_in<double> DSAN120N;
    sc_in<double> DSAN121P;
    sc_in<double> DSAN121N;

    sc_in<double> DSAN130P;
    sc_in<double> DSAN130N;
    sc_in<double> DSAN131P;
    sc_in<double> DSAN131N;

    sc_in<double> DSAN110P;
    sc_in<double> DSAN110N;
    sc_in<double> DSAN111P;
    sc_in<double> DSAN111N;
    
    sc_in<double> DSAN150P;
    sc_in<double> DSAN150N;
    sc_in<double> DSAN151P;
    sc_in<double> DSAN151N;

    sc_in<double> DSAN140P;
    sc_in<double> DSAN140N;
    sc_in<double> DSAN141P;
    sc_in<double> DSAN141N;
    sc_in<double> DSAN142P;
    sc_in<double> DSAN142N;
    sc_in<double> DSAN143P;
    sc_in<double> DSAN143N;

    sc_in<double> DSAN220P;
    sc_in<double> DSAN220N;

    sc_in<double> DSAN210P;
    sc_in<double> DSAN210N;

    sc_in<bool> DSADTRG00;
    sc_in<bool> DSADTRG10;
    sc_in<bool> DSADTRG20;
    sc_in<bool> DSADTRG12;
    sc_in<bool> DSADTRG13;
    sc_in<bool> DSADTRG11;
    sc_in<bool> DSADTRG15;
    sc_in<bool> DSADTRG14;
    sc_in<bool> DSADTRG22;
    sc_in<bool> DSADTRG21;
    
    sc_in<bool> DSADRGT00;
    sc_in<bool> DSADRGT10;
    sc_in<bool> DSADRGT20;
    sc_in<bool> DSADRGT12;
    sc_in<bool> DSADRGT13;
    sc_in<bool> DSADRGT11;
    sc_in<bool> DSADRGT15;
    sc_in<bool> DSADRGT14;
    sc_in<bool> DSADRGT22;
    sc_in<bool> DSADRGT21;

    /// Declare output signals
    sc_out<bool> DSADEND00;
    sc_out<bool> DSADEND10;
    sc_out<bool> DSADEND20;
    sc_out<bool> DSADEND12;
    sc_out<bool> DSADEND13;
    sc_out<bool> DSADEND11;
    sc_out<bool> DSADEND15;
    sc_out<bool> DSADEND14;
    sc_out<bool> DSADEND22;
    sc_out<bool> DSADEND21;
    
    sc_out<bool> DSADI00;
    sc_out<bool> DSADI10;
    sc_out<bool> DSADI20;
    sc_out<bool> DSADI12;
    sc_out<bool> DSADI13;
    sc_out<bool> DSADI11;
    sc_out<bool> DSADI15;
    sc_out<bool> DSADI14;
    sc_out<bool> DSADI22;
    sc_out<bool> DSADI21;
    
    sc_out<bool> DSADE00;
    sc_out<bool> DSADE10;
    sc_out<bool> DSADE20;
    sc_out<bool> DSADE12;
    sc_out<bool> DSADE13;
    sc_out<bool> DSADE11;
    sc_out<bool> DSADE15;
    sc_out<bool> DSADE14;
    sc_out<bool> DSADE22;
    sc_out<bool> DSADE21;
    
    sc_out<bool> DSADPE00;
    sc_out<bool> DSADPE10;
    sc_out<bool> DSADPE20;
    sc_out<bool> DSADPE12;
    sc_out<bool> DSADPE13;
    sc_out<bool> DSADPE11;
    sc_out<bool> DSADPE15;
    sc_out<bool> DSADPE14;
    sc_out<bool> DSADPE22;
    sc_out<bool> DSADPE21;
    
    sc_out<bool> DSADDFREQ000;
    sc_out<bool> DSADDFREQ010;
    sc_out<bool> DSADDFREQ020;
    sc_out<bool> DSADDFREQ012;
    sc_out<bool> DSADDFREQ013;
    sc_out<bool> DSADDFREQ011;
    sc_out<bool> DSADDFREQ015;
    sc_out<bool> DSADDFREQ014;
    sc_out<bool> DSADDFREQ022;
    sc_out<bool> DSADDFREQ021;
    
    sc_out<bool> DSADDFREQ100;
    sc_out<bool> DSADDFREQ110;
    sc_out<bool> DSADDFREQ120;
    sc_out<bool> DSADDFREQ112;
    sc_out<bool> DSADDFREQ113;
    sc_out<bool> DSADDFREQ111;
    sc_out<bool> DSADDFREQ115;
    sc_out<bool> DSADDFREQ114;
    sc_out<bool> DSADDFREQ122;
    sc_out<bool> DSADDFREQ121;
    
    sc_out<sc_uint<4> > DSADDFTAG00;
    sc_out<sc_uint<4> > DSADDFTAG10;
    sc_out<sc_uint<4> > DSADDFTAG20;
    sc_out<sc_uint<4> > DSADDFTAG12;
    sc_out<sc_uint<4> > DSADDFTAG13;
    sc_out<sc_uint<4> > DSADDFTAG11;
    sc_out<sc_uint<4> > DSADDFTAG15;
    sc_out<sc_uint<4> > DSADDFTAG14;
    sc_out<sc_uint<4> > DSADDFTAG22;
    sc_out<sc_uint<4> > DSADDFTAG21;
    
    sc_out<sc_uint<16> > DSADDFDATA00;
    sc_out<sc_uint<16> > DSADDFDATA10;
    sc_out<sc_uint<16> > DSADDFDATA20;
    sc_out<sc_uint<16> > DSADDFDATA12;
    sc_out<sc_uint<16> > DSADDFDATA13;
    sc_out<sc_uint<16> > DSADDFDATA11;
    sc_out<sc_uint<16> > DSADDFDATA15;
    sc_out<sc_uint<16> > DSADDFDATA14;
    sc_out<sc_uint<16> > DSADDFDATA22;
    sc_out<sc_uint<16> > DSADDFDATA21;
    
    sc_out<bool> DSADUE00;
    sc_out<bool> DSADUE10;
    sc_out<bool> DSADUE20;
    sc_out<bool> DSADUE12;
    sc_out<bool> DSADUE13;
    sc_out<bool> DSADUE11;
    sc_out<bool> DSADUE15;
    sc_out<bool> DSADUE14;
    sc_out<bool> DSADUE22;
    sc_out<bool> DSADUE21;
    
    sc_out<bool> DSADLE00;
    sc_out<bool> DSADLE10;
    sc_out<bool> DSADLE20;
    sc_out<bool> DSADLE12;
    sc_out<bool> DSADLE13;
    sc_out<bool> DSADLE11;
    sc_out<bool> DSADLE15;
    sc_out<bool> DSADLE14;
    sc_out<bool> DSADLE22;
    sc_out<bool> DSADLE21;
    
    sc_out<bool> *DSADVCI00[emVCNum];
    sc_out<bool> *DSADVCI10[emVCNum];
    sc_out<bool> *DSADVCI20[emVCNum];
    sc_out<bool> *DSADVCI12[emVCNum];
    sc_out<bool> *DSADVCI13[emVCNum];
    sc_out<bool> *DSADVCI11[emVCNum];
    sc_out<bool> *DSADVCI15[emVCNum];
    sc_out<bool> *DSADVCI14[emVCNum];
    sc_out<bool> *DSADVCI22[emVCNum];
    sc_out<bool> *DSADVCI21[emVCNum];

    /// Socket declaration
    TlmTargetSocket<32> *ts;            /// For DSADC's registers

    /// Add target socket for each sub-block
    TlmTargetSocket<32> *ts_AD00;
    TlmTargetSocket<32> *ts_AD10;
    TlmTargetSocket<32> *ts_AD20;
    TlmTargetSocket<32> *ts_AD12;
    TlmTargetSocket<32> *ts_AD13;
    TlmTargetSocket<32> *ts_AD11;
    TlmTargetSocket<32> *ts_AD14;
    TlmTargetSocket<32> *ts_AD15;
    TlmTargetSocket<32> *ts_AD22;
    TlmTargetSocket<32> *ts_AD21;

    SC_HAS_PROCESS (DSADC);
    DSADC (sc_module_name name,
          unsigned int rLatency,
          unsigned int wLatency);
    ~DSADC (void);

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
    void DumpStatusInfo( );                           /// Dump the status information of model
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
    double CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point);
    void CancelEvents (void);
    void Initialize (void);
    void RegisterHandlerWrapper (const bool is_com_reg, const std::string ad_id, const std::vector<std::string> cmd);
    
    /// Virtual functions of DSADC_AgentController class
    bool GetResetStatus (const std::string reset_name);
    bool CheckClockPeriod (const std::string clock_name);

    /// Virtual fuctions of DSADC_Base 
    unsigned int GetCheckDisconnectDetectSetup(void);     /// Get DSADCADGCR.ODDE/ODE value
    bool GetIsUnsignConfig(void);                         /// Get DSADCADGCR.UNSND value
    bool GetIsPinDiagEnable(void);                        /// Get DSADCTDCR.TDE value

};
#endif
