// -----------------------------------------------------------------------------
// $Id: cvm2.h,v 1.1 2012/11/19 06:46:03 trongtruong Exp $
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

#ifndef __CVM2_H__
#define __CVM2_H__
#include "re_define.h"
#include "systemc.h"
class Ccvm2: public sc_module
{//{{{
    public:
        SC_HAS_PROCESS(Ccvm2);
        Ccvm2(sc_module_name name);
        ~Ccvm2();
        //Ports declaration
        sc_in<bool> selcvm2refh0_v50;
        sc_in<bool> selcvm2refh1_v50;
        sc_in<bool> selcvm2refl0_v50;
        sc_in<bool> selcvm2refl1_v50;
        sc_in<bool> errcvm2isoh_v50;
        sc_in<bool> errcvm2isol_v50;
        sc_in<bool> errcvm2awoh_v50;
        sc_in<bool> errcvm2awol_v50;
        sc_in<bool> pdcvm2isoh_v50;
        sc_in<bool> pdcvm2isol_v50;
        sc_in<bool> pdcvm2awoh_v50;
        sc_in<bool> pdcvm2awol_v50;
        sc_in<bool> pdcvm2bgr_v50;
        sc_in<bool> pdcvm2bufo_v50;
        sc_in<bool> sel_cvmtrim0_v50;
        sc_in<bool> sel_cvmtrim1_v50;
        sc_in<bool> sel_cvmtrim2_v50;
        sc_in<bool> sel_cvmtrim3_v50;
        sc_in<bool> sel_cvmtrim4_v50;
        sc_out<bool> cvm2isohdet_v50;
        sc_out<bool> cvm2isoldet_v50;
        sc_out<bool> cvm2awohdet_v50;
        sc_out<bool> cvm2awoldet_v50;
    private:
        //Events
        //Functions
        void Initialize();
        //Methods
        void Cvm2det_v50Method();
};//}}}

#endif //__CVM2_H__
