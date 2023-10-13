// -----------------------------------------------------------------------------
// $Id: hold_cvm.h,v 1.7 2012/12/13 08:45:22 trongtruong Exp $
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

#ifndef __HOLD_CVM_H__
#define __HOLD_CVM_H__
#include "re_define.h"
#include "general_reset.h"
#include <cstdarg>
#include <map>
#include <sstream>
class Chold_cvm: public Cgeneral_reset
{//{{{
#include "hold_cvm_cmdif.h"
    public:
        SC_HAS_PROCESS(Chold_cvm);
        Chold_cvm(sc_module_name name);
        ~Chold_cvm();
        //Ports declaration
        sc_in<sc_dt::uint64> CLK;
        sc_in<bool> SIGIN0;
        sc_in<bool> SIGIN1;
        sc_in<bool> SIGIN2;
        sc_in<bool> SIGIN3;
        sc_in<bool> SIGIN4;
        sc_in<bool> EN0;
        sc_in<bool> EN1;
        sc_in<bool> EN2;
        sc_in<bool> EN3;
        sc_in<bool> EN4;
        sc_in<bool> RESZ;
        sc_out<bool> SIGOUT0;
        sc_out<bool> SIGOUT1;
        sc_out<bool> SIGOUT2;
        sc_out<bool> SIGOUT3;
        sc_out<bool> SIGOUT4;
    private:
        //Enum
        enum eCONSTANCE{
            emNanoSecond = 1000000000
        };
        //Variable declaration
        double mCLKPeriod;
        //Events
        sc_event mReadPortEvent;
        sc_event mSIGOUTEvent;
        //Functions
        void Initialize(bool Start);
        double CalculateRisingEdgeTime();
        void AssertReset(const double delay, const double period);
        void EnableReset(const bool is_active);
        void SetCLKfreq(const std::string clk_name, const double clk_freq);
        //Methods
        void SIGINMethod();
        void SIGOUTMethod();
        void CLKMethod();
        void ResetMethod();
};//}}}

#endif //__HOLD_CVM_H__
// vim600: set foldmethod=marker :
