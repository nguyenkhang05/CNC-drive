// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
#ifndef __ASF_H__
#define __ASF_H__
#include "systemc.h"
#include <iterator>
#include "asf_regif.h"
#include "tlm_tgt_if.h"

/// ASF model class
class Casf: public sc_module
            , public vpcl::tlm_tgt_if<32>
            , public Casf_regif
{
#include "asf_cmdif.h"
private:
    // Declare enum type
    enum eConstant {
        emChannelNum = 16  
        ,emTgtId     = 0      //  Target socket ID
        ,emNanoSecond    = 1000000000
    };
    // Declare event
    sc_event    mAccEvent;
    sc_event    mUpdateDRegEvent;
    sc_event    mGenINTEvent;
    sc_event    mUpdateInterValEvent;
    sc_event    mResetAccEvent;
    sc_event    mCmdResetEvent;
    sc_event    mCancelCmdResetEvent;
    sc_event    mASREQEvent;

    // Variable
    bool mRTDBit;
    bool mSTBit;
    bool mPortReset;
    bool mCmdReset;
    bool mIsCmdResetStatus;
    bool mIsInitialize;
    bool mIsRTDOperate;

    double        mFreqPCLK;
    double        mPCLKPeriod;
    double        mResetPeriod; // handleCommand Reset period time
    unsigned int  mAccSelect;
    unsigned int  mAccCTAG;
    unsigned int  mDFData;
    unsigned int  mDFTag;
    unsigned int  mAccCNT;
    unsigned int  mDataBitNum;
    
    typedef std::map<unsigned int, unsigned int> MapType;
    MapType       mINTEnable;
    MapType       mChannelEnable;
    MapType       mCMPMatch;
    MapType       mAccVal;

    // call back function
    void cb_CMP_CMP(RegCBstr str);
    void cb_RTDR_RTCNT(RegCBstr str);
    void cb_CTL0_CTAG(RegCBstr str);
    void cb_CTL1_ST(RegCBstr str);
    void cb_CNT_CNT(RegCBstr str);

    // Method
    void PCLKPeriodMethod(void);
    void PortResetMethod(void);
    void DFTAGInputMethod(void);
    void DFDATAInputMethod(void);
    void AccumulationMethod(void);
    void UpdateDataRegMethod(void);
    void GenINTThread(void);
    void UpdateInterValMethod(void);
    void ResetAccumulationMethod(void);
    void CmdResetMethod(void);
    void CancelCmdResetMethod(void);
    void AccumulatorProcessMethod(void);
    void ASREQMethod(void);

    // tlm_tgt_if api functions
    void tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans);

    // Functions
    void Initialize(void);
    void DumpStatInfo(void);
    double GetTimeResolution(void);
    void SetCLKfreq(std::string clk_name, double clk_freq);
    void AssertReset(double delay, double period);
    void EnableReset(const bool is_active);
    unsigned int ModifyAccData(void);
    void Count(void);
    void DataCond(void);
    void SetLatency_TLM(const unsigned int id, const double pclk_period, const bool is_constructor);

public:
    // Port declaration
    sc_in<sc_dt::uint64>   pclk;
    sc_in<bool>            preset_n;
    sc_in<bool>            ASREQ;
    sc_in<sc_uint<4> >     DFTAG;
    sc_in<sc_uint<16> >    DFDATA;
    sc_out<sc_uint<16> >   INT_ACE;

    SC_HAS_PROCESS(Casf);
    Casf(sc_module_name name);
    ~Casf();

};

#endif //__ASF_H__
