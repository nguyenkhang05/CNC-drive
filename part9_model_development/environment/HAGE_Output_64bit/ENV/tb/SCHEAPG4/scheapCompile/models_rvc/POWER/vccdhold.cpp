// -----------------------------------------------------------------------------
// $Id: vccdhold.cpp,v 1.4 2012/12/13 08:42:07 trongtruong Exp $
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


#include "vccdhold.h"

/// Constructor of Cvccdhold class
Cvccdhold::Cvccdhold(sc_module_name name):
          in("in")
         ,en("en")
         ,rst("rst")
         ,out("out")
{ //{{{
    //Initial output
    out.initialize(0);
    mAssertResetFlag = false;    
    //Initial variable
    mCLKPeriod = 0;

    SC_METHOD(InMethod);
    dont_initialize();
    sensitive << in;
    sensitive << en;
    sensitive << mReadPortEvent;
    sensitive << rst;
    
    SC_METHOD(OutMethod);
    dont_initialize();
    sensitive << mOutEvent;
}//}}}

// Destructor of Cvccdhold class
Cvccdhold::~Cvccdhold()
{//{{{
}//}}}

double Cvccdhold::CalculateRisingEdgeTime()
{//{{{
    double cur_time = sc_time_stamp().to_double();
    unsigned int index = (unsigned int)(cur_time / mCLKPeriod) + 1;
    return mCLKPeriod*index - cur_time;
}//}}}

void Cvccdhold::AssertResetFunc(bool is_assert_reset)
{//{{{
    if(is_assert_reset){
        mOutEvent.cancel();
        mAssertResetFlag = true;
        mOutEvent.notify();
    }else{
        mAssertResetFlag = false;
        mReadPortEvent.notify();
    }
}//}}}

void Cvccdhold::InMethod()
{//{{{
    if(mCLKPeriod != 0){
        if(rst.read() == 0){
            mOutEvent.notify();
        } else {
            double syn_time = CalculateRisingEdgeTime();
            mOutEvent.notify(syn_time, SC_NS);
        }
    }
}//}}}

void Cvccdhold::OutMethod()
{//{{{
    if((rst.read() == 0) || (mAssertResetFlag)){
        mOutEvent.cancel();
        out.write(0);
    } else if(en.read() == 1){
        out.write(in.read());
    }
}//}}}

// vim600: set foldmethod=marker :
