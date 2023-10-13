// ---------------------------------------------------------------------
// $Id: UIMPL_SLAVE.h,v 1.4 2020/04/07 03:39:04 huepham Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __UIMPL_SLAVE_H__
#define __UIMPL_SLAVE_H__

#include "OSCI2.h"
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <stdarg.h>
#include "global.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "rvc_common_model.h"
#include "BusSlaveFuncIf.h"
//Ref: {UIMPL_SLAVE_DD_ATT1_001, UIMPL_SLAVE_DD_Direction_020}
template<unsigned int BUS_WIDTH> 
class UIMPL_SLAVE :
    public sc_module
   ,public BusSlaveBase<BUS_WIDTH, 1>
   ,public rvc_common_model
   ,public BusSlaveFuncIf
{
//----------------------------
// constructing the model
//----------------------------
#include "UIMPL_SLAVE_cmdif.h"
public:
    TlmTargetSocket<BUS_WIDTH>      *ts;                 // target socket
    sc_in<sc_dt::uint64>            clk;                // clock port
    sc_in<bool>                     resetPort;          // reset port
    sc_out<unsigned int>            BMID;               // BMID port
    sc_out<bool>                    ERR;                // ERR port
    SC_HAS_PROCESS(UIMPL_SLAVE);
    UIMPL_SLAVE(sc_module_name name, unsigned int rLatency, unsigned int wLatency);
    ~UIMPL_SLAVE(void);
    
    
    void AssertReset     (const std::string reset_name
                         ,const double      start_time
                         ,const double      period);
    void SetMessageLevel (const std::string msg_lv);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void Help            (const std::string type);
    
private:

    sc_dt::uint64                       mclkPeriod;                    // Period of PCLK
    sc_dt::uint64                       mclkOrgFreq;                   // Previous frequency of PCLK
    sc_dt::uint64                       mclkFreq;                      // Frequency of PCLK
    std::string                         mclkFreqUnit;                  // Frequency unit of PCLK
    double                              mStartclkTime;                 // Start time of PCLK
    double                              mTimeCLKChangePoint;    //The time clock is changed its value

    sc_dt::uint64                       mTimeResolutionValue;           // Time resolution value of simulation
    sc_time_unit                        mTimeResolutionUnit;  
    
    bool                                mERRIntr;              
    unsigned int                        mBMID;
    
    sc_event                mClkZeroEvent;          // Notify clock is zero
    // Time resolution unit of simulation
    bool   mIsResetPortActive;  ///< Is reset by port
    bool   mIsResetCmdReceive;  ///< Is receive cmd reset
    bool   mIsResetCmdActive;   ///< Is reset by cmd
    double mResetCmdPeriod;     ///< Period of cmd reset
    sc_event mResetCmdEvent;
    sc_event mCancelResetCmdEvent;
    sc_event mBMIDEvent;
    sc_event mERREvent;
    sc_event mAssertERREvent;
    sc_event mDeassertERREvent;

    void HandleresetPortMethod();
    void HandleresetPortCmdMethod();
    void CancelResetCmdMethod();
    void HandleclkSignalMethod();
    void EnableReset(bool isActive);
    void CancelEvents(void);
    void Initialize();
    void DumpInterruptMsg (const std::string intr_name, const bool value);
    void WriteBMIDMethod();
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void WriteERRMethod();
    void AssertERRIntrMethod();
    void DeassertERRIntrMethod();

};

#endif /*__UIMPL_SLAVE_H__*/
