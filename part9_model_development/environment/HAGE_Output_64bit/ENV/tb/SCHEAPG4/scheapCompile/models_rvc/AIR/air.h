// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2016 - 2017 Renesas Electronics Corporation
// Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
#ifndef __AIR_H__
#define __AIR_H__
#include "systemc.h"
#include "tlm_tgt_if.h"
#include "air_core.h"
#include <iterator>

/// AIR model class
class Cair: public sc_module
            , public vpcl::tlm_tgt_if<32>
{
#include "air_cmdif.h"
private:
    enum eAirConstant {
        emNumOfOutputPort = 64,
        emNumOfInputPort = 114
    };
    enum eAirState{
        emResetState = 0,
        emOperateState = 1
    };
    enum eTimeUnit{
        emNanoSecond = 1000000000
    };

    sc_in<bool>  mInputPort[emNumOfInputPort];

    Cair_core   *pAirCore;
    sc_event    mCmdResetEvent;
    sc_event    mCancelCmdResetEvent;
    sc_event    mWriteINTAIRINTREQEvent;
    sc_event    mWriteINTAIRDMAREQEvent;

    bool         mPortReset;
    bool         mCmdReset;
    bool         mIsCmdResetStatus;
    bool         mIsInitialize;
    unsigned int mState;
    double       mPCLKPeriod;
    double       mResetPeriod; // handleCommand Reset period time
    bool         mINTOut[emNumOfOutputPort];
    bool         mDMAOut[emNumOfOutputPort];
    bool         mPreINTOut[emNumOfOutputPort];
    bool         mPreDMAOut[emNumOfOutputPort];
    std::string  mInputPortName[emNumOfInputPort];
    unsigned int mNumINTReq[emNumOfOutputPort];
    unsigned int mNumDMAReq[emNumOfOutputPort];

    // tlm_tgt_if api functions
    void tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t);
    void tgt_acc( tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans);
    unsigned int tgt_acc_dbg( tlm::tlm_generic_payload &trans);

    // Functions
    void Initialize(void);
    double GetTimeResolution(void);
    void SetLatency_TLM(const double pclk_period, const bool is_constructor);
    void SetCLKfreq(std::string clk_name, double clk_freq);
    void AssertReset(double delay, double period);
    void EnableReset(const bool is_active);
    void WriteINTAIRDMAREQMethod(void);
    void WriteINTAIRINTREQMethod(void);
    void ReceiveInputMethod(const unsigned int i);
    void PCLKPeriodMethod(void);
    void PortResetMethod(void);
    void CmdResetMethod(void);
    void CancelCmdResetMethod(void);
    void DumpStatInfo(void);

public:
    // Port declaration
    sc_in<sc_dt::uint64>   pclk;
    sc_in<bool>            presetn;

    sc_in<bool> ADMPXI0;
    sc_in<bool> ADMPXI1;
    sc_in<bool> ADMPXI2;
    sc_in<bool> ADMPXI3;
    sc_in<bool> ADI00;
    sc_in<bool> ADI01;
    sc_in<bool> ADI02;
    sc_in<bool> ADI03;
    sc_in<bool> ADI04;
    sc_in<bool> ADI10;
    sc_in<bool> ADI11;
    sc_in<bool> ADI12;
    sc_in<bool> ADI13;
    sc_in<bool> ADI14;
    sc_in<bool> ADI20;
    sc_in<bool> ADI21;
    sc_in<bool> ADI22;
    sc_in<bool> ADI23;
    sc_in<bool> ADI24;
    sc_in<bool> ADI30;
    sc_in<bool> ADI31;
    sc_in<bool> ADI32;
    sc_in<bool> ADI33;
    sc_in<bool> ADI34;
    sc_in<bool> ADE0;
    sc_in<bool> ADE1;
    sc_in<bool> ADE2;
    sc_in<bool> ADE3;
    sc_in<bool> DSADE00;
    sc_in<bool> DSADE10;
    sc_in<bool> DSADE20;
    sc_in<bool> DSADE11;
    sc_in<bool> DSADE12;
    sc_in<bool> DSADE13;
    sc_in<bool> DSADI00;
    sc_in<bool> DSADI10;
    sc_in<bool> DSADI20;
    sc_in<bool> DSADI11;
    sc_in<bool> DSADI12;
    sc_in<bool> DSADI13;
    sc_in<bool> CADE00;
    sc_in<bool> CADI00;
    sc_in<bool> ASI0;
    sc_in<bool> ASI1;
    sc_in<bool> ASI2;
    sc_in<bool> ASI3;
    sc_in<bool> ASI4;
    sc_in<bool> ASI5;
    sc_in<bool> ASI6;
    sc_in<bool> ASI7;
    sc_in<bool> ASI8;
    sc_in<bool> ASI9;
    sc_in<bool> ASI10;
    sc_in<bool> ASI11;
    sc_in<bool> ASI12;
    sc_in<bool> ASI13;
    sc_in<bool> ASI14;
    sc_in<bool> ASI15;
    sc_in<bool> BFP0;
    sc_in<bool> BFP1;
    sc_in<bool> BFP2;
    sc_in<bool> BFP3;
    sc_in<bool> BFP4;
    sc_in<bool> BFP5;
    sc_in<bool> BFP6;
    sc_in<bool> BFP7;
    sc_in<bool> BFP8;
    sc_in<bool> BFP9;
    sc_in<bool> BFP10;
    sc_in<bool> BFP11;
    sc_in<bool> BFP12;
    sc_in<bool> BFP13;
    sc_in<bool> BFP14;
    sc_in<bool> BFP15;
    sc_in<bool> BFP16;
    sc_in<bool> BFP17;
    sc_in<bool> BFP18;
    sc_in<bool> BFP19;
    sc_in<bool> BFP20;
    sc_in<bool> BFP21;
    sc_in<bool> BFP22;
    sc_in<bool> BFP23;
    sc_in<bool> BFP24;
    sc_in<bool> BFP25;
    sc_in<bool> BFP26;
    sc_in<bool> BFP27;
    sc_in<bool> BFP28;
    sc_in<bool> BFP29;
    sc_in<bool> BFP30;
    sc_in<bool> BFP31;
    sc_in<bool> BFP32;
    sc_in<bool> BFP33;
    sc_in<bool> BFP34;
    sc_in<bool> BFP35;
    sc_in<bool> BFP36;
    sc_in<bool> BFP37;
    sc_in<bool> BFP38;
    sc_in<bool> BFP39;
    //Supported for E2x_FCC2
    sc_in<bool> DSADE14;
    sc_in<bool> DSADE15;
    sc_in<bool> DSADE21;
    sc_in<bool> DSADE22;
    sc_in<bool> DSADI14;
    sc_in<bool> DSADI15;
    sc_in<bool> DSADI21;
    sc_in<bool> DSADI22;
    sc_in<bool> BFP40;
    sc_in<bool> BFP41;
    sc_in<bool> BFP42;
    sc_in<bool> BFP43;
    sc_in<bool> BFP44;
    sc_in<bool> BFP45;
    sc_in<bool> BFP46;
    sc_in<bool> BFP47;

    sc_out<bool> INTAIRINTREQ[emNumOfOutputPort];
    sc_out<bool> INTAIRDMAREQ[emNumOfOutputPort]; 

    SC_HAS_PROCESS(Cair);
    Cair(sc_module_name name);
    ~Cair();
    void WriteINTPortOut(unsigned int index, bool is_active);
    void WriteDMAPortOut(unsigned int index, bool is_active);
};

#endif //__AIR_H__
