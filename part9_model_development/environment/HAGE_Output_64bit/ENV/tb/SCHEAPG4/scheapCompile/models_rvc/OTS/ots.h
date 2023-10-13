// ---------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __OTS_H__
#define __OTS_H__
#include "systemc.h"
#include "tlm.h"
#include "tlm_tgt_if.h"
#include <iterator>
#include "ots_regif.h"

/// OTS model class
class Cots: public sc_module
            ,public Cots_regif
            ,public vpcl::tlm_tgt_if<32> 
{
#include "ots_cmdif.h"
public:
// Construct and deconstruct
    SC_HAS_PROCESS(Cots);
    Cots(sc_module_name name, const short coeffra, const short coeffrb, const short coeffrc);
    ~Cots();

// Port declaration
    sc_in<sc_dt::uint64> pclk;
    sc_in<sc_dt::uint64> clkot;
    sc_in<bool> preset_n;

    sc_out<bool> oti;
    sc_out<bool> otabe;
    sc_out<bool> otuli;
    sc_out<bool> ote;

    sc_core::sc_attribute<std::string> temp_file;

private:
// OTS enum
    // Interrupt ID
    enum eIntrErrKind{ 
         emIntErrKindIni = 0
        ,emOTABEErr      = 1
        ,emOTEErr        = 2
        ,emOTIIntr       = 4
        ,emOTULIIntr     = 8
    };
    // OTS phase
    enum eOTSPhase{
         emOTACTSetPhase   = 0 
        ,emCheckStopPhase  = 1
        ,emOTFSetPhase     = 2
        ,emMeasureEndPhase = 3
        ,emOTACTClearPhase = 4
    };

    enum eConstValue{
         emNormal = 0
        ,emHighB  = 1
        ,emHighA  = 2
        ,emLowA   = 3
//        ,emTempMaximum = 0x382D 
//        ,emTempMinimum = 0x1D8F
        ,emNanoSecond  = 1000000000
        ,emMaxFileLength = 255
        ,emMultiplePCLK  = 780
        ,emMultipleClkot = 72
        ,emMeasureCycleNum = 512
        ,emClockCondConst  = 106
    };

// OTS event
    sc_event mCmdResetEvent;
    sc_event mCmdCancelResetEvent;        
    sc_event mInterruptEvent;                      
    sc_event mStartEvent;        
    sc_event mOTACTSetEvent;          
    sc_event mOTACTClearEvent;        
    sc_event mOTFSetEvent;        
    sc_event mCheckStopEvent;        
    sc_event mMeasureEndEvent;        
    
// Global Variable
    std::ifstream mFileStream;
    double mPCLKPeriod;
    double mclkotPeriod;
    double mStartTime;
    double mA2Time;
    double mA3Time;
    double mA4Time;
    double mA5Time;
    double mA6Time;
    double mA7Time;
    eOTSPhase mPhase;
    unsigned int mInterruptKind;
    unsigned int mNewTempSetTemp;
    unsigned int mNewTempFile;
    unsigned int mOTIActiveNum;
    unsigned int mOTABEActiveNum;
    unsigned int mOTULIActiveNum;
    unsigned int mOTEActiveNum;
    bool mNoLineFlag;
    bool mLegalLineFlag;
    bool mIllegalLineFlag; 
    bool mEmptyFileFlag;
    bool mMeasureFlag;
    bool mStopFlag;
    bool mFirstLineFlag;

    double mResetPeriod;
    bool mIsCmdResetFlag;
    bool mOTSPortResetFlag;
    bool mOTSCmdResetFlag;
    bool mErrorFileFlag;

    double mFreqPCLK;
    double mFreqCLKOT;
    bool mIsInitialize;

// OTS Method
    void MeasureMethod (void);
    void OperationMethod (void);
    void InterruptMethod (void);
    void ResetMethod(void);
    void AssertResetMethod(void);
    void DeAssertResetMethod (void);
    void PCLKMethod (void);
    void ClkotMethod (void);

// OTS Internal Function
    bool PeriodCalculate(void); 
    void TimeCalculate(void);
    void OpenFile(void);
    void GetTemperatureLine(void);
    void TemperatureAnalysis(const bool intr_err_issue_flag);
    void EnableReset(const bool is_active);
    void GlobalInit(void);
    void DumpInterruptMsg (const std::string intr_name, const std::string intr_number, const std::string issue);
    double GetTimeResolution(void);
    void SetCLKfreq(std::string clk_name, double clk_freq);
    void SetLatency_TLM(const double pclk_period, const bool is_constructor);
    double GetCurTime(void);

// tlm_tgt_if api function
    void tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(tlm::tlm_generic_payload &trans);
    bool UnsupportAddrAccess(const unsigned int addr, const unsigned int size, unsigned char *p_data, tlm::tlm_command command, const bool dbg_mode);

// handleCommand function
    void DumpInfo(const char *type, const char *message, ... );
    void DumpStatInfo(void);
    void AssertReset(const double start_time, const double period);
    void SetTemp(const unsigned short temperature);

// overwrite function of sc_module
    void start_of_simulation();

// Register IF Callback functions
    void cb_OTSTCR_OTST(RegCBstr str);
    void cb_OTENDCR_OTEND(RegCBstr str);
    void cb_OTFCR_SDERC(RegCBstr str);
    bool ChkAccess( const bool is_wr,                  ///< [in] write/read process 
                    const std::string channel_name,    ///< [in] channel name
                    const std::string register_name,   ///< [in] register name
                    const std::string bit_name         ///< [in] bit name
                  );
};

#endif //__OTS_H__
