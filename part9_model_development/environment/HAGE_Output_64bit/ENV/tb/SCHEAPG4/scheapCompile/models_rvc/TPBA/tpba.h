// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
#ifndef __TPBA_H__
#define __TPBA_H__
#include "systemc.h"
#include "tpba_regif.h"
#include "tlm_tgt_if.h"
#include "Cgeneral_timer.h"
#include <iterator>
//TPBA couter class
class Ctpba_counter: public Cgeneral_timer
{
public:
    sc_signal<bool> mCompareMatch;
    SC_HAS_PROCESS(Ctpba_counter);
    Ctpba_counter(sc_module_name name);
    ~Ctpba_counter();
    void StartStopCounter (const bool start, const double clk_period, const unsigned int clk_divide);
    void SetCompareMatchValue (const unsigned int value);
    void Initialize();
    unsigned int GetCNTValue(void);
    //double GetClkPeriod (void);
private:
    sc_event mWriteCmpSignalEvent;
    bool mCmpValue;
    void cmpMatchOutputHandling (bool value);
    void WriteCmpSignalMethod (void);
};

//TPBA model class
class Ctpba:public sc_module
            ,public Ctpba_regif
            ,public vpcl::tlm_tgt_if<32>
{
#include "tpba_cmdif.h"
public:
//Ports declaration
    sc_in <sc_dt::uint64 > pclk;
    sc_in  <bool> preset_n;
    sc_in  <bool> TPBATSST;
    sc_out <bool> TPBATINTPRD;
    sc_out <bool> TPBATINTDTY;
    sc_out <bool> TPBATINTPAT;
    sc_out <bool> TPBATTOUT;
    SC_HAS_PROCESS(Ctpba);
    Ctpba(sc_module_name name);
    ~Ctpba();
private:
    enum ePeriodUnit {
        emNanoSecond = 1000000000
    };
    enum eStatus {
        emIdle     = 0,
        emCounting = 1,
        emReset    = 2
    };
//Events declaration
    sc_event mAssertResetEvent;  
    sc_event mCancelAssertResetEvent;
    sc_event mWaitingEvent;
    sc_event mWriteTOUTEvent;
    sc_event mWritePRDEvent;
    sc_event mTSEvent;
    sc_event mTTEvent;
    sc_event mTSSTEvent;

    sc_event mPRDPATEvent;
    sc_event mDTYEvent;
//Declare counter
    Ctpba_counter *mPeriodCounter;
    Ctpba_counter *mDutyCounter;
//Variable
    unsigned int mStatus;
    unsigned int mPrdCount;
    unsigned int mDtyCount;
    unsigned int mPatCount;
    unsigned int mTPBA0CB0;//the buffer that stores value of TPBA0CMP0 register
    unsigned int mTPBA0CB1;//the buffer that stores value of TPBA0CMP1 register
    unsigned int mTPBA0CB2;//the buffer that stores value of TPBA0BUF register
    unsigned int mPointerBuf;
    unsigned int mPATCounter;//this variable helps to determine the level of PAT
    double mStopTime;
    double mClkPeriod;
    double mPeriodTime;
    bool mTPBA0TOLB;
    bool mWriteTOUTValue;
    bool mPATFlag;
    bool mTSSTFlag;
    bool mCmdIfRstFlag;
    bool mPRDValue;
    bool mPATValue;
    bool mDTYValue;
    bool mPrePRDValue;
    bool mPrePATValue;
    bool mPreDTYValue;
    bool mIsReset;
    bool mWriteTOUTByPRD;
    double freq_PCLK;
    bool mIsDeassertIntr;
    bool mPATMatching;
    bool mPRDForceStop;
    bool mDTYForceStop;
    bool mIsInitialize;
    bool mIsStartCounting;
    bool mAssertDTY;
    

    //tlm function
    void tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(tlm::tlm_generic_payload &trans);

    bool ChkAccess(const bool is_wr,const std::string channel_name,const std::string register_name,const std::string bit_name);
    void PCLKPeriodMethod (void);
    void TSSTMethod (void);
    void StartMethod (void);
    void StopMethod(void);
    void MatchMethod (void);
    void WaitingMethod (void);
    void WriteTOUTMethod (void);
    void PRDPATMethod (void);
    void DTYMethod (void);
    void DumpStatInfo (void); 
    void DumpInfo(const char *type, const char *message, ...);
    void DumpTransInfo(std::string status);
    void DumpIntInfo(std::string operation, std::string intr_name);
    std::string SelectStatus (const unsigned int status);
    void ResetMethod (void);
    void EnableReset (const bool is_active);
    void AssertReset (double wait_time, double period_time);
    void AssertResetMethod (void);
    void ReInitialize (const bool is_constructor = false);
    void CancelAssertResetMethod (void);
    void UpdateRegisters (void);
    double GetTimeResolution(void);
    void SetLatency_TLM(const double pclk_period, const bool is_constructor);
    void SetCLKfreq(std::string clk_name, double clk_freq);

    void cb_TPBA0CMP1_TPBA0CMP1(RegCBstr str);
    void cb_TPBA0CNT0_TPBA0CNT0(RegCBstr str);
    void cbDBG_TPBA0CNT0_TPBA0CNT0(RegCBstr str);
    void cb_TPBA0RDT_TPBA0RDT0(RegCBstr str);
    void cb_TPBA0TOE_TPBA0TOE0(RegCBstr str);
    void cb_TPBA0TO_TPBA0TO0(RegCBstr str);
    void cb_TPBA0TS_TPBA0TS0(RegCBstr str);
    void cb_TPBA0TT_TPBA0TT0(RegCBstr str);
};

#endif //__TPBA_H__
