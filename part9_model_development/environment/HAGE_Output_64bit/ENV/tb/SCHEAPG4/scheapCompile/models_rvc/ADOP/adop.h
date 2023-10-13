// ---------------------------------------------------------------------
// $Id: adop.h,v 1.1 2015/08/19 03:59:38 sontran Exp $
//
// Copyright(c) 2015 Renesas Electronics Corporation
// Copyright(c) 2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __ADOP_H__
#define __ADOP_H__
#include "systemc.h"
#include <iterator>
#include <map>
#include <cstdarg>

/// ADOP model class
class Cadop: public sc_module
{
#include "adop_cmdif.h"
public:    
    // Input ports
    sc_in <sc_dt::uint64> pclk;
    sc_in <bool> preset_n;
    sc_in < sc_uint<32> > PWDATA;
    sc_in <bool> ADCATPSEL;
    sc_in <bool> ADCATWEN;
    sc_in <bool> ADCATTIN00;
    sc_in <bool> ADCATTIN01;
    sc_in <bool> ADCATTIN02;
    sc_in <bool> ADCATTIN03;
    sc_in <bool> ADCATTIN04;
    sc_in <bool> ADCATTIN05;
    sc_in <bool> ADCATTIN06;
    sc_in <bool> ADCATTIN07;
    sc_in <bool> ADCATTIN08;
    sc_in <bool> ADCATTIN09;
    sc_in <bool> ADCATTIN10;
    sc_in <bool> ADCATTIN11;
    sc_in <bool> ADCATTIN12;
    sc_in <bool> ADCATTIN13;
    sc_in <bool> ADCATTIN14;
    sc_in <bool> ADCATTIN15;

    // Output ports
    sc_out < sc_uint<16> > ADCATSDATA;
    sc_out <bool> ADCATTOUT;

    SC_HAS_PROCESS(Cadop);
    Cadop(sc_module_name name);
    ~Cadop();

private:
    //Enum
    enum eADOP_CONSTANCE {
        emNanoSecond = 1000000000
       ,emPortNum    = 16
    };
    //Variable 
    bool mIsInitialize;
    unsigned int mResetPeriod;
    bool mIsCmdResetFlag;
    bool mADOPPortResetFlag;
    bool mADOPCmdResetFlag;
    bool mIsOperating;
    unsigned int mADCA0OPAIOC;

    //Event
    sc_event mCmdResetEvent;
    sc_event mCmdCancelResetEvent;
    sc_event mAssertResetEvent;
    sc_event mUpdateOutputEvent;
    sc_event mUpdateRegisterEvent;

    //Parameter and Information
    double mPCLKPeriod;

    double GetTimeResolution(void);
    void SetCLKfreq(std::string clk_name, double clk_freq);

    //handle_command function
    void AssertReset(const double start_time, const double period);

    //Normal function
    bool IsReset(void);
    void Initialize(void);
    void EnableReset(const bool is_active);

    //method
    void PCLKMethod(void);
    void ResetMethod(void);
    void AssertResetMethod(void);
    void DeAssertResetMethod(void);
    void UpdateOutputMethod(void);
    void TWENMethod(void);
};

#endif //__ADOP_H__
