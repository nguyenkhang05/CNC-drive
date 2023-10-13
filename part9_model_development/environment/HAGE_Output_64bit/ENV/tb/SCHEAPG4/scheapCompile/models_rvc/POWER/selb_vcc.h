// -----------------------------------------------------------------------------
// $Id: selb_vcc.h,v 1.8 2012/12/13 08:45:22 trongtruong Exp $
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

#ifndef __SELB_VCC_H__
#define __SELB_VCC_H__
#include "re_define.h"
#include "vccdhold.h"
#include "general_reset.h"
#include <cstdarg>
#include <map>
#include <sstream>
class Cselb_vcc: public Cgeneral_reset
{//{{{
#include "selb_vcc_cmdif.h"
    public:
        SC_HAS_PROCESS(Cselb_vcc);
        Cselb_vcc(sc_module_name name);
        ~Cselb_vcc();
        //Ports declaration
        sc_in<sc_dt::uint64> clk;
        sc_in<bool> pocdet_n;
        sc_in<bool> cvm2hdeten;
        sc_in<bool> cvm2ldeten;
        sc_in<bool> cvm2hdetlvl0;
        sc_in<bool> cvm2hdetlvl1;
        sc_in<bool> cvm2ldetlvl0;
        sc_in<bool> cvm2ldetlvl1;
        sc_in<bool> cvm_res_msk;
        sc_in<bool> cvm_resreq_msk;
        sc_in<bool> cvm_diag_mask;
        sc_in<bool> pdcvm2bgr;
        sc_in<bool> pdcvm2bufo;
        sc_in<bool> errcvm2isoh;
        sc_in<bool> errcvm2isol;
        sc_in<bool> errcvm2awoh;
        sc_in<bool> errcvm2awol;
        sc_in<bool> cvmtrim0;
        sc_in<bool> cvmtrim1;
        sc_in<bool> cvmtrim2;
        sc_in<bool> cvmtrim3;
        sc_in<bool> cvmtrim4;
        sc_in<bool> cvm_disable;
        sc_in<bool> cvm_latchen;
        sc_in<bool> cvmde_latchen;
        sc_in<bool> cvmdmask1_latchen;
        sc_in<bool> cvmdmask2_latchen;
        sc_in<bool> cvmtrim_latchen;
        sc_in<bool> cvmen_select;
        sc_in<bool> cvmmsk_cold;
        sc_in<bool> mode_trans;
        sc_in<bool> test_cvm2mask;
        sc_in<bool> pdcvm2isoh_t;
        sc_in<bool> pdcvm2isol_t;
        sc_in<bool> pdcvm2awoh_t;
        sc_in<bool> pdcvm2awol_t;
        sc_in<bool> cvm2isohdet_v50;
        sc_in<bool> cvm2isoldet_v50;
        sc_in<bool> cvm2awohdet_v50;
        sc_in<bool> cvm2awoldet_v50;
        sc_in<bool> clunc_and_v50;
        sc_in<bool> clunc_or_v50;
        sc_out<bool> selcvm2refh0_v50;
        sc_out<bool> selcvm2refh1_v50;
        sc_out<bool> selcvm2refl0_v50;
        sc_out<bool> selcvm2refl1_v50;
        sc_out<bool> errcvm2isoh_v50;
        sc_out<bool> errcvm2isol_v50;
        sc_out<bool> errcvm2awoh_v50;
        sc_out<bool> errcvm2awol_v50;
        sc_out<bool> pdcvm2isoh_v50;
        sc_out<bool> pdcvm2isol_v50;
        sc_out<bool> pdcvm2awoh_v50;
        sc_out<bool> pdcvm2awol_v50;
        sc_out<bool> pdcvm2bgr_v50;
        sc_out<bool> pdcvm2bufo_v50;
        sc_out<bool> sel_cvmtrim0_v50;
        sc_out<bool> sel_cvmtrim1_v50;
        sc_out<bool> sel_cvmtrim2_v50;
        sc_out<bool> sel_cvmtrim3_v50;
        sc_out<bool> sel_cvmtrim4_v50;
        sc_out<bool> cvm2hdet_v50;
        sc_out<bool> cvm2ldet_v50;
        sc_out<bool> reset_req_cvm_v50;
        sc_out<bool> dis_ldo_iso_v50;
        sc_out<bool> cvm2isoldet_flg_v50;
        sc_out<bool> cvm2awoldet_flg_v50;
        sc_out<bool> cvm_reset_v50;
        sc_out<bool> ei_cvmout_v50;
    private:
        //Signal
        sc_signal<bool> mSignalCvm_latchen_v50;
        sc_signal<bool> mSignalCvmde_latchen_v50;
        sc_signal<bool> mSignalCvmdmask1_latchen_v50;
        sc_signal<bool> mSignalCvmdmask2_latchen_v50;
        sc_signal<bool> mSignalCvmtrim_latchen_v50;
        sc_signal<bool> mSignalCvmmsk_cold_v50;
        sc_signal<bool> mSignalCvm_resreq_msk_v50;
        sc_signal<bool> mSignalPocdet_n_v50;
        sc_signal<bool> mSignalCvm2hdeten_v50;
        sc_signal<bool> mSignalCvm2ldeten_v50;
        sc_signal<bool> mSignalCvm2hdetlvl0_v50;
        sc_signal<bool> mSignalCvm2hdetlvl1_v50;
        sc_signal<bool> mSignalCvm2ldetlvl0_v50;
        sc_signal<bool> mSignalCvm2ldetlvl1_v50;
        sc_signal<bool> mSignalCvm_res_msk_v50;
        sc_signal<bool> mSignalCvm_diag_mask_v50;
        sc_signal<bool> mSignalPdcvm2bgr_v50;
        sc_signal<bool> mSignalPdcvm2bufo_v50;
        sc_signal<bool> mSignalCvmtrim0_v50;
        sc_signal<bool> mSignalCvmtrim1_v50;
        sc_signal<bool> mSignalCvmtrim2_v50;
        sc_signal<bool> mSignalCvmtrim3_v50;
        sc_signal<bool> mSignalCvmtrim4_v50;
        sc_signal<bool> mSignalCvm_disable_v50;
        sc_signal<bool> mSignalCvmen_select_v50;
        sc_signal<bool> mSignalMode_trans_v50;
        sc_signal<bool> mSignalTest_cvm2mask_v50;
        sc_signal<bool> mSignalPdcvm2isoh_t_v50;
        sc_signal<bool> mSignalPdcvm2isol_t_v50;
        sc_signal<bool> mSignalPdcvm2awoh_t_v50;
        sc_signal<bool> mSignalPdcvm2awol_t_v50;
        sc_signal<bool> mSignalVccdholdOut0;
        sc_signal<bool> mSignalVccdholdOut1;
        sc_signal<bool> mSignalVccdholdOut8;
        sc_signal<bool> mSignalVccdholdOut9;
        //Enum
        enum eCONSTANCE{
            emOutputUpdateCycle = 19
           ,emDFFOfMux = 17
           ,emNanoSecond = 1000000000
        };
        //Variable declaration
        double mCLKPeriod;
        bool mCvm2hdetArr[18];
        bool mCvm2ldetArr[18];
        bool mCvm2hdet_dnf;
        bool mPreCvm2hdet_dnf;
        bool mCvm2ldet_dnf;
        bool mPreCvm2ldet_dnf;
        bool mPocdetRstFlag;
        bool mResetFlag;
        Cvccdhold *mVccdhold[15];
        //Events
        sc_event mPocdetRstEvent;
        sc_event mResetEvent;
        sc_event mCvm2hdet_dnfEvent;
        sc_event mCvm2ldet_dnfEvent;
        sc_event mReadPortEvent;
        //Functions
        void Initialize();
        double CalculateRisingEdgeTime();
        void Cvm2det_dnfFunc(unsigned int id, bool *arr, bool &preiso, bool iso, bool &preawo, bool awo, unsigned int &RemainUpdateCycle);
        bool Cvm2det_v50Func(bool condition1, bool condition2, bool signal);
        bool Pdcvm2_v50Func(bool cvm2deten);
        void AssertReset(const double delay, const double period);
        void SetCLKfreq(const std::string clk_name, const double clk_freq);
        void EnableReset(const bool is_active);
        //Methods
        void Clunc_or_v50Method();
        void Clunc_and_v50Method();
        void Pdcvm2h_v50Method();
        void Pdcvm2l_v50Method();
        void Pocdet_n_v50Method();
        void Cvm2hdet_dnfThread();
        void Cvm2ldet_dnfThread();
        void Cvm2hdet_v50Method();
        void Cvm2ldet_v50Method();
        void Dis_ldo_iso_v50Method();
        void Reset_req_cvm_v50Method();
        void Cvm_reset_v50Method();
        void Clk_v50Method();
};//}}}

#endif //__SELB_VCC_H__
// vim600: set foldmethod=marker :
