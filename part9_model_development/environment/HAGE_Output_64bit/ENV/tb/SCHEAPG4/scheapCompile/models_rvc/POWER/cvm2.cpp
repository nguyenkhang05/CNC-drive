// -----------------------------------------------------------------------------
// $Id: cvm2.cpp,v 1.4 2012/11/29 02:49:49 trongtruong Exp $
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


#include "cvm2.h"

Ccvm2::Ccvm2(sc_module_name name):
         selcvm2refh0_v50("selcvm2refh0_v50")
        ,selcvm2refh1_v50("selcvm2refh1_v50")
        ,selcvm2refl0_v50("selcvm2refl0_v50")
        ,selcvm2refl1_v50("selcvm2refl1_v50")
        ,errcvm2isoh_v50("errcvm2isoh_v50")
        ,errcvm2isol_v50("errcvm2isol_v50")
        ,errcvm2awoh_v50("errcvm2awoh_v50")
        ,errcvm2awol_v50("errcvm2awol_v50")
        ,pdcvm2isoh_v50("pdcvm2isoh_v50")
        ,pdcvm2isol_v50("pdcvm2isol_v50")
        ,pdcvm2awoh_v50("pdcvm2awoh_v50")
        ,pdcvm2awol_v50("pdcvm2awol_v50")
        ,pdcvm2bgr_v50("pdcvm2bgr_v50")
        ,pdcvm2bufo_v50("pdcvm2bufo_v50")
        ,sel_cvmtrim0_v50("sel_cvmtrim0_v50")
        ,sel_cvmtrim1_v50("sel_cvmtrim1_v50")
        ,sel_cvmtrim2_v50("sel_cvmtrim2_v50")
        ,sel_cvmtrim3_v50("sel_cvmtrim3_v50")
        ,sel_cvmtrim4_v50("sel_cvmtrim4_v50")
        ,cvm2isohdet_v50("cvm2isohdet_v50")
        ,cvm2isoldet_v50("cvm2isoldet_v50")
        ,cvm2awohdet_v50("cvm2awohdet_v50")
        ,cvm2awoldet_v50("cvm2awoldet_v50")
{//{{{
    Initialize();

    SC_METHOD(Cvm2det_v50Method);
    dont_initialize();
    sensitive << errcvm2isoh_v50;
    sensitive << errcvm2isol_v50;
    sensitive << errcvm2awoh_v50;
    sensitive << errcvm2awol_v50;
    sensitive << selcvm2refh0_v50;
    sensitive << selcvm2refh1_v50;
    sensitive << selcvm2refl0_v50;
    sensitive << selcvm2refl1_v50;
    sensitive << pdcvm2isoh_v50;
    sensitive << pdcvm2isol_v50;
    sensitive << pdcvm2awoh_v50;
    sensitive << pdcvm2awol_v50;
}//}}}

Ccvm2::~Ccvm2()
{//{{{
}//}}}

void Ccvm2::Initialize()
{//{{{
    cvm2isohdet_v50.initialize(0);
    cvm2isoldet_v50.initialize(0);
    cvm2awohdet_v50.initialize(0);
    cvm2awoldet_v50.initialize(0);
}//}}}

void Ccvm2::Cvm2det_v50Method()
{//{{{
    //process for cvm2isohdet_v50 port
    if((errcvm2isoh_v50.read() == 0) && (selcvm2refh0_v50.read() == 0) && (selcvm2refh1_v50.read() == 0)){
        cvm2isohdet_v50.write(0);
    }else {
        cvm2isohdet_v50.write(!pdcvm2isoh_v50.read());
    }
    //process for cvm2isoldet_v50 port
    if((errcvm2isol_v50.read() == 0) && (selcvm2refl0_v50.read() == 0) && (selcvm2refl1_v50.read() == 0)){
        cvm2isoldet_v50.write(0);
    }else {
        cvm2isoldet_v50.write(!pdcvm2isol_v50.read());
    }
    //process for cvm2awohdet_v50 port
    if((errcvm2awoh_v50.read() == 0) && (selcvm2refh0_v50.read() == 0) && (selcvm2refh1_v50.read() == 0)){
        cvm2awohdet_v50.write(0);
    }else {
        cvm2awohdet_v50.write(!pdcvm2awoh_v50.read());
    }
    //process for cvm2awohdet_v50 port
    if((errcvm2awol_v50.read() == 0) && (selcvm2refl0_v50.read() == 0) && (selcvm2refl1_v50.read() == 0)){
        cvm2awoldet_v50.write(0);
    }else {
        cvm2awoldet_v50.write(!pdcvm2awol_v50.read());
    }
}//}}}

// vim600: set foldmethod=marker :
