// ---------------------------------------------------------------------
// $Id: DSADC_Base.h 1398 2019-11-26 06:16:27Z chuonghoangle $
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DSADC_Base_H__
#define __DSADC_Base_H__

#include "BusSlaveBase.h"
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h> 
#include <sstream> 

class DSADC_UNIT;

class DSADC_Base
{

public:

    enum eModelNumBase {                    /// Define model's constant number
        emADNumBase = 10,
        emVCNum = 8
    };

    enum eResetActiveLevel {                /// Define active level of reset signal
        emResetSignalNum = 2
    };

    /// Input Port
    sc_in<sc_dt::uint64> CLK_LSB;
    sc_in<bool> PRESETn;

    /// Internal variable
    sc_dt::uint64 mCLK_LSBPeriod;
    sc_time_unit mTimeResolutionUnit;       /// Time resolution unit of simulation
    sc_dt::uint64 mFosPeriod;
    sc_dt::uint64 mCLK_HSBPeriod;           /// Period value of CLK_HSB clock
    sc_dt::uint64 mFosFreq;

    bool mDumpInterrupt;                    /// Enable/Disable dumping the interrupt info
    double mADSVCC;
    double mADSVSS;
    unsigned int mtWOdd;
    unsigned int mtWEven;
    unsigned int mtDF;
    unsigned int mtS;
    unsigned int mtR;
    unsigned int mtCLB;
    double mtCoeff;
    bool mParityError[emADNumBase];
    bool mIDError[emADNumBase];
    bool mDisconnectError[emADNumBase];

    /// Event
    sc_event mResetHardEvent[emResetSignalNum];                      /// Call HandleResetHardMethod when PRESETn or resetad_n is active
    sc_event mResetCmdEvent[emResetSignalNum];                       /// Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];                 /// Call CancelResetCmdMethod when AssertReset is over
    sc_event mClkZeroEvent[2];

    /// Internal functions
    virtual unsigned int GetCheckDisconnectDetectSetup(void) = 0;    /// Get DSADCADGCR.ODDE/ODE value
    virtual bool GetIsUnsignConfig(void) = 0;                        /// Get DSADCADGCR.UNSND value
    virtual bool GetIsPinDiagEnable(void) = 0;                       /// Get DSADCTDCR.TDE value
    virtual double CalculateClockEdge(const std::string clock_name, const bool is_pos, const double time_point) = 0;
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;

    DSADC_Base(void):CLK_LSB("CLK_LSB"), PRESETn("PRESETn") { ; }
    virtual ~DSADC_Base(void) { ; }
};
#endif
