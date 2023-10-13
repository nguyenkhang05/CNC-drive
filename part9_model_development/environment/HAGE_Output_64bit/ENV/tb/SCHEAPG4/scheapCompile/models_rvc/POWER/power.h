// -----------------------------------------------------------------------------
// $Id: power.h,v 1.5 2013/03/20 06:18:30 ducduong Exp $
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

#ifndef __POWER_H__
#define __POWER_H__
#include "cvm2.h"
#include "cvm_awo.h"
#include "hold_cvm.h"
#include "selb_vcc.h"
#include "poc.h"
#include "systemc.h"

class Cpower: public sc_module
{//{{{
    public:
        Cpower(sc_module_name name);
        ~Cpower();
        //Model declartion
        Chold_cvm *mHold_cvm;
        Ccvm_awo *mCvm_awo;
        Cselb_vcc *mSelb_vcc;
        Ccvm2 *mCvm2;
        Cpoc *mPoc;
        //Ports declaration
        //For selb_vcc
        sc_in<bool> cvm2hdeten;
        sc_in<bool> cvm2ldeten;
        sc_in<bool> cvm2hdetlvl0;
        sc_in<bool> cvm2hdetlvl1;
        sc_in<bool> cvm2ldetlvl0;
        sc_in<bool> cvm2ldetlvl1;
        sc_in<bool> cvm_res_msk;
        sc_in<bool> cvm_resreq_msk;
        sc_in<bool> cvm_diag_mask;
        sc_in<bool> pdcvm2isoh_t;
        sc_in<bool> pdcvm2isol_t;
        sc_in<bool> pdcvm2awoh_t;
        sc_in<bool> pdcvm2awol_t;
        sc_in<bool> clunc_and_v50;
        sc_in<bool> clunc_or_v50;
        sc_out<bool> cvm2hdet_v50;
        sc_out<bool> cvm2ldet_v50;
        sc_out<bool> reset_req_cvm_v50;
        sc_out<bool> dis_ldo_iso_v50;
        sc_out<bool> cvm2isoldet_flg_v50;
        sc_out<bool> cvm2awoldet_flg_v50;
        sc_out<bool> cvm_reset_v50;
        sc_out<bool> ei_cvmout_v50;
        //For hold_cvm
        sc_in<bool> cvmtrim0_in;
        sc_in<bool> cvmtrim1_in;
        sc_in<bool> cvmtrim2_in;
        sc_in<bool> cvmtrim3_in;
        sc_in<bool> cvmtrim4_in;
        sc_in<bool> en0;
        sc_in<bool> en1;
        sc_in<bool> en2;
        sc_in<bool> en3;
        sc_in<bool> en4;
        //For cvm_awo
        sc_in<bool> diag_mode;
        sc_in<bool> cvmde_wen;
        sc_in<bool> cvmdmask1_wen;
        sc_in<bool> cvmdmask2_wen;
        sc_in<sc_dt::uint64> clk;
        sc_in<sc_dt::uint64> clk_roscl;
        sc_in<bool> moscen;
        sc_in<bool> pllen;
        sc_in<bool> cpuclks0;
        sc_in<bool> cpuclks1;
        sc_in<bool> cpuclks2;
//         sc_in<bool> pocdet_n;
        sc_in<bool> resz12;
        sc_in<bool> reset_fct;
        sc_in<bool> mstprqz;
        sc_in<bool> stop;
        sc_in<bool> off;
        sc_in<bool> cyclic;
        sc_in<bool> isolate;
        sc_in<bool> vccholdtrg;
        sc_in<bool> haltst;
        sc_in<bool> pdcvm2bgr_t;
        sc_in<bool> pdcvm2bufo_t;
        sc_in<bool> errcvm2isoh_t;
        sc_in<bool> errcvm2isol_t;
        sc_in<bool> errcvm2awoh_t;
        sc_in<bool> errcvm2awol_t;
        sc_in<bool> tapkeep;
        sc_in<bool> cvm2tmsk;
        //For POC
        sc_in<sc_dt::uint64> CLK_V00;
        sc_in<bool> CLR_POF;
        sc_out<bool> poradet_n_v50;
        sc_out<bool> pocadet_n_v50;
        sc_out<bool> pocdet_n_v50;
        sc_out<bool> det0det_v50;
        sc_out<bool> REQ_POF;
        sc_out<bool> DET0DET_MSK;
        sc_out<bool> POCADET_N_MSK;
        sc_out<bool> POCDET_N_MSK;
    private:
        //Variable declaration
        sc_signal<bool> mSignalPdcvm2bgr;
        sc_signal<bool> mSignalPdcvm2bufo;
        sc_signal<bool> mSignalErrcvm2isoh;
        sc_signal<bool> mSignalErrcvm2isol;
        sc_signal<bool> mSignalErrcvm2awoh;
        sc_signal<bool> mSignalErrcvm2awol;
        sc_signal<bool> mSignalCvmtrim0;
        sc_signal<bool> mSignalCvmtrim1;
        sc_signal<bool> mSignalCvmtrim2;
        sc_signal<bool> mSignalCvmtrim3;
        sc_signal<bool> mSignalCvmtrim4;
        sc_signal<bool> mSignalCvm_disable;
        sc_signal<bool> mSignalCvm_latchen;
        sc_signal<bool> mSignalCvmde_latchen;
        sc_signal<bool> mSignalCvmdmask1_latchen;
        sc_signal<bool> mSignalCvmdmask2_latchen;
        sc_signal<bool> mSignalCvmtrim_latchen;
        sc_signal<bool> mSignalCvmen_select;
        sc_signal<bool> mSignalCvmmsk_cold;
        sc_signal<bool> mSignalMode_trans;
        sc_signal<bool> mSignalTest_cvm2mask;
        sc_signal<bool> mSignalCvm2isohdet_v50;
        sc_signal<bool> mSignalCvm2isoldet_v50;
        sc_signal<bool> mSignalCvm2awohdet_v50;
        sc_signal<bool> mSignalCvm2awoldet_v50;

        sc_signal<bool> mSignalSelcvm2refh0_v50;
        sc_signal<bool> mSignalSelcvm2refh1_v50;
        sc_signal<bool> mSignalSelcvm2refl0_v50;
        sc_signal<bool> mSignalSelcvm2refl1_v50;
        sc_signal<bool> mSignalErrcvm2isoh_v50;
        sc_signal<bool> mSignalErrcvm2isol_v50;
        sc_signal<bool> mSignalErrcvm2awoh_v50;
        sc_signal<bool> mSignalErrcvm2awol_v50;
        sc_signal<bool> mSignalPdcvm2isoh_v50;
        sc_signal<bool> mSignalPdcvm2isol_v50;
        sc_signal<bool> mSignalPdcvm2awoh_v50;
        sc_signal<bool> mSignalPdcvm2awol_v50;
        sc_signal<bool> mSignalPdcvm2bgr_v50;
        sc_signal<bool> mSignalPdcvm2bufo_v50;
        sc_signal<bool> mSignalSel_cvmtrim0_v50;
        sc_signal<bool> mSignalSel_cvmtrim1_v50;
        sc_signal<bool> mSignalSel_cvmtrim2_v50;
        sc_signal<bool> mSignalSel_cvmtrim3_v50;
        sc_signal<bool> mSignalSel_cvmtrim4_v50;
        sc_signal<bool> mSignalHold_cvmtrim0;
        sc_signal<bool> mSignalHold_cvmtrim1;
        sc_signal<bool> mSignalHold_cvmtrim2;
        sc_signal<bool> mSignalHold_cvmtrim3;
        sc_signal<bool> mSignalHold_cvmtrim4;
        //Events
        //Functions
        //Methods
};//}}}

#endif //__POWER_H__
// vim600: set foldmethod=marker :
