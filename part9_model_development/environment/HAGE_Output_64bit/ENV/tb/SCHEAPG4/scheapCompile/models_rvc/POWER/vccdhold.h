// -----------------------------------------------------------------------------
// $Id: vccdhold.h,v 1.2 2012/11/20 04:14:41 trongtruong Exp $
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

#ifndef __VCCDHOLD_H__
#define __VCCDHOLD_H__
#include "re_define.h"
#include "systemc.h"

/// VCCDHOLD class
class Cvccdhold: public sc_module
{//{{{
public:
    SC_HAS_PROCESS(Cvccdhold);
    Cvccdhold(sc_module_name name);
    ~Cvccdhold();

    double mCLKPeriod;
    //Ports declaration
    sc_in<bool> in;
    sc_in<bool> en;
    sc_in<bool> rst;
    sc_out<bool> out;
    
    void AssertResetFunc(bool is_assert_reset);
    
private:
    //Events
    sc_event mOutEvent;
    sc_event mReadPortEvent;
    //Variable
    bool mAssertResetFlag;
    //Functions
    double CalculateRisingEdgeTime();
    //Methods
    void InMethod(void);
    void OutMethod(void);
};//}}}

#endif //__VCCDHOLD_H__
