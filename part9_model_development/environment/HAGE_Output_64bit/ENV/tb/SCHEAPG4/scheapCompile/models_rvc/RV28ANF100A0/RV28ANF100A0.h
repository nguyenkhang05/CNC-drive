// ---------------------------------------------------------------------
// $Id: RV28ANF100A0.h,v 1.6 2018/05/16 08:19:59 chuonghoangle Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __RV28ANF100A0_H__
#define __RV28ANF100A0_H__

#include "re_define.h"
#include "BusSlaveBase.h"

class RV28ANF100A0: public sc_module
{
#include "RV28ANF100A0_cmdif.h"
public:

    // Inputs/Outputs
    sc_in<bool>             ANFIN;
    sc_in<bool>             INTR;
    sc_in<bool>             INTF;
    sc_out<bool>            ANFOUT;
    sc_out<bool>            AEDOUT;
	
private:
    
    // Variables
    sc_dt::uint64   mTimeResolutionValue;   // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;    // Time resolution unit of simulation
    unsigned int    mNoiseWidth;
    sc_time_unit    mNoiseWidthUnit;
    unsigned int    mEdgeWidth;
    sc_time_unit    mEdgeWidthUnit;

    bool            mDelay1;                     // temp value of mDelay1Signal
    bool            mDelay2;                     // temp value of mDelay2Signal
    bool            mANFOUT;                     // temp value of ANFOUT
    bool            mIsDelay1UpdateEventPending;
    bool            mIsDelay2UpdateEventPending;
    bool            mDelay3;                     // temp value of mDelay3Signal

public:
    sc_signal<bool> mInbarSignal;           // invert of ANFIN
    sc_signal<bool> mDelay1Signal;          // equal mInbarSignal delayed noisewidth/2
    sc_signal<bool> mDelay2Signal;          // equal mDelay1Signal delayed noisewidth/2
    sc_signal<bool> mResetSignal;           // if equal 0 then set ANFOUT = 1
    sc_signal<bool> mSetSignal;             // if equal 0 then set ANFOUT = 0
    sc_signal<bool> mDelay3Signal;          // equal ANFOUT delayed edgewidth
    sc_signal<bool> mIn1Signal;             // ANFOUT rising edge
    sc_signal<bool> mIn2Signal;             // ANFOUT falling edge
    sc_signal<bool> mIn3Signal;             // ANFOUT both edge

private:    
    // Events
    sc_event        mInbarEvent;
    sc_event        mDelay1UpdateEvent;
    sc_event        mDelay1Event;
    sc_event        mDelay2UpdateEvent;
    sc_event        mDelay2Event;
    sc_event        mANFOUTUpdateEvent;
    sc_event        mDelay3UpdateEvent;
    
    // Threads/Methods
    void InbarMethod();
    void Delay1Method();
    void Delay1UpdateMethod();
    void Delay2Method();
    void Delay2UpdateMethod();
    void ResetMethod();
    void SetMethod();
    void SrlatchMethod();
    void ANFOUTUpdateMethod();
    void ANFOUTInitThread();
    void Delay3Method();
    void Delay3UpdateMethod();
    void In1Method();
    void In2Method();
    void In3Method();
    void Mux4to1Method();
    
    // Function
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void DumpInfo(const char *type, const char *message, ...);

public:
    SC_HAS_PROCESS (RV28ANF100A0);
    RV28ANF100A0 (sc_module_name name, const unsigned int rLatency, const unsigned int wLatency);
    ~RV28ANF100A0 ();

    // PythonIF
    void SetMessageLevel (const std::string msg_lv);
    void SetNoiseMaxWidth (const unsigned int noisewidth, const std::string unit);
    void SetEdgeFixWidth (const unsigned int edgewidth, const std::string unit);
    void Help ();
};

#endif //__RV28ANF100A0_H__
