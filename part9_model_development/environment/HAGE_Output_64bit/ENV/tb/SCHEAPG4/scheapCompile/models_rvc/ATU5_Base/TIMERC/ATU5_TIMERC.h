// ---------------------------------------------------------------------
// $Id: ATU5_TIMERC.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERC_H__
#define __ATU5_TIMERC_H__

#include "ATU5_TIMERC_AgentController.h"
#include "ATU5_Base.h"
#include "re_define.h"
#include "ATU5_Timer.h"
#include <map>

class ATU5_TIMERC_Func;
class ATU5_TIMERC: public sc_module,
              public ATU5_TIMERC_AgentController
{
friend class ATU5_Base;
friend class ATU5_TIMERC_SUB;

private:
    enum eResetActiveLevel {
        emResetActive               = 0,
        emResetSignalNum            = 1
    };//Define active level of reset signal
    double mPCLKPeriod;
public:
    //Declare input signals
    ATU5_TIMERC_Func  *mTimerC_Func;   //ATU5_TIMERC_Func class instance
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> *clock_bus[6];
    sc_in<sc_dt::uint64> nc_clk;
    //ports from timer A
    sc_in<bool> i_event_1_0;
    sc_in<bool> i_event_2A;
    sc_in<bool> i_event_2B;
    //each sub has 4 sc_inout port: TIOC0, TIOC1, TIOC2, TIOC3
    sc_inout<bool> **TIOC0;
    sc_inout<bool> **TIOC1;
    sc_inout<bool> **TIOC2;
    sc_inout<bool> **TIOC3;

    //update for new spec/rtl: add TOCE TOC
    sc_out<bool> **TOCE0;
    sc_out<bool> **TOCE1;
    sc_out<bool> **TOCE2;
    sc_out<bool> **TOCE3;

    sc_out<bool> **TOC0;
    sc_out<bool> **TOC1;
    sc_out<bool> **TOC2;
    sc_out<bool> **TOC3;

    //each sub has 1 interrupt overflow port: INT_OVI
    sc_out<bool> **INT_OVI;

    //each sub has 4 interrupt GRC port: INT_GRC0,INT_GRC1,INT_GRC2,INT_GRC3,
    sc_out<bool> **INT_GRC0;
    sc_out<bool> **INT_GRC1;
    sc_out<bool> **INT_GRC2;
    sc_out<bool> **INT_GRC3;

    //each sub has 4 interrupt OCRC port: INT_OCRC0,INT_OCRC1,INT_OCRC2,INT_OCRC3,
    sc_out<bool> **INT_OCRC0;
    sc_out<bool> **INT_OCRC1;
    sc_out<bool> **INT_OCRC2;
    sc_out<bool> **INT_OCRC3;

    //Declare public functions
private:
    sc_dt::uint64   mPCLKFreq;
    sc_dt::uint64   mPCLKOrgFreq;
    std::string     mPCLKFreqUnit;
    std::string     mModuleName;
    unsigned int    mTimeResolutionValue;
    std::string     mTimeResolutionUnit;
    bool            mStatusC;

    bool IsModelResetActive(void);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void StartStopTimer (const bool is_start);
    void HandlePCLKSignalMethod(void);
    void Initialize(void);
    void StartSubblock(const unsigned int sub); 

    void StartStopSubblock(const unsigned int sub, const bool is_start);
    void SetNoiseCancelMode(const unsigned int sub_id);
    void SetNoiseEachCancelMode(const unsigned int sub, const unsigned int channel);
public:
    void EnableTIC(const bool is_start);
    void EnableReset (const bool is_active);
    SC_HAS_PROCESS (ATU5_TIMERC);
    ATU5_TIMERC (sc_module_name name,ATU5_Base   *ATU5_Ptr ,unsigned int subblock);
    ~ATU5_TIMERC (void);
};
#endif
