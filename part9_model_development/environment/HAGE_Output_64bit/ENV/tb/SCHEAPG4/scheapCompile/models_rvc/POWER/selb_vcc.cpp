// -----------------------------------------------------------------------------
// $Id: selb_vcc.cpp,v 1.10 2012/12/19 09:26:08 trongtruong Exp $
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


#include "selb_vcc.h"

Cselb_vcc::Cselb_vcc(sc_module_name name): Cgeneral_reset(name)
        //Input ports
        ,clk("clk")
        ,pocdet_n("pocdet_n")
        ,cvm2hdeten("cvm2hdeten")
        ,cvm2ldeten("cvm2ldeten")
        ,cvm2hdetlvl0("cvm2hdetlvl0")
        ,cvm2hdetlvl1("cvm2hdetlvl1")
        ,cvm2ldetlvl0("cvm2ldetlvl0")
        ,cvm2ldetlvl1("cvm2ldetlvl1")
        ,cvm_res_msk("cvm_res_msk")
        ,cvm_resreq_msk("cvm_resreq_msk")
        ,cvm_diag_mask("cvm_diag_mask")
        ,pdcvm2bgr("pdcvm2bgr")
        ,pdcvm2bufo("pdcvm2bufo")
        ,errcvm2isoh("errcvm2isoh")
        ,errcvm2isol("errcvm2isol")
        ,errcvm2awoh("errcvm2awoh")
        ,errcvm2awol("errcvm2awol")
        ,cvmtrim0("cvmtrim0")
        ,cvmtrim1("cvmtrim1")
        ,cvmtrim2("cvmtrim2")
        ,cvmtrim3("cvmtrim3")
        ,cvmtrim4("cvmtrim4")
        ,cvm_disable("cvm_disable")
        ,cvm_latchen("cvm_latchen")
        ,cvmde_latchen("cvmde_latchen")
        ,cvmdmask1_latchen("cvmdmask1_latchen")
        ,cvmdmask2_latchen("cvmdmask2_latchen")
        ,cvmtrim_latchen("cvmtrim_latchen")
        ,cvmen_select("cvmen_select")
        ,cvmmsk_cold("cvmmsk_cold")
        ,mode_trans("mode_trans")
        ,test_cvm2mask("test_cvm2mask")
        ,pdcvm2isoh_t("pdcvm2isoh_t")
        ,pdcvm2isol_t("pdcvm2isol_t")
        ,pdcvm2awoh_t("pdcvm2awoh_t")
        ,pdcvm2awol_t("pdcvm2awol_t")
        ,cvm2isohdet_v50 ("cvm2isohdet_v50")
        ,cvm2isoldet_v50 ("cvm2isoldet_v50")
        ,cvm2awohdet_v50 ("cvm2awohdet_v50")
        ,cvm2awoldet_v50 ("cvm2awoldet_v50")
        ,clunc_and_v50("clunc_and_v50")
        ,clunc_or_v50("clunc_or_v50")
        //Output ports
        ,selcvm2refh0_v50("selcvm2refh0_v50")
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
        ,cvm2hdet_v50("cvm2hdet_v50")
        ,cvm2ldet_v50("cvm2ldet_v50")
        ,reset_req_cvm_v50("reset_req_cvm_v50")
        ,dis_ldo_iso_v50("dis_ldo_iso_v50")
        ,cvm2isoldet_flg_v50("cvm2isoldet_flg_v50")
        ,cvm2awoldet_flg_v50("cvm2awoldet_flg_v50")
        ,cvm_reset_v50("cvm_reset_v50")
        ,ei_cvmout_v50("ei_cvmout_v50")
        //Signal
        ,mSignalCvm_latchen_v50("mSignalCvm_latchen_v50")
        ,mSignalCvmde_latchen_v50("mSignalCvmde_latchen_v50")
        ,mSignalCvmdmask1_latchen_v50("mSignalCvmdmask1_latchen_v50")
        ,mSignalCvmdmask2_latchen_v50("mSignalCvmdmask2_latchen_v50")
        ,mSignalCvmtrim_latchen_v50("mSignalCvmtrim_latchen_v50")
        ,mSignalCvmmsk_cold_v50("mSignalCvmmsk_cold_v50")
        ,mSignalCvm_resreq_msk_v50("mSignalCvm_resreq_msk_v50")
        ,mSignalPocdet_n_v50("mSignalPocdet_n_v50")
        ,mSignalCvm2hdeten_v50("mSignalCvm2hdeten_v50")
        ,mSignalCvm2ldeten_v50("mSignalCvm2ldeten_v50")
        ,mSignalCvm2hdetlvl0_v50("mSignalCvm2hdetlvl0_v50")
        ,mSignalCvm2hdetlvl1_v50("mSignalCvm2hdetlvl1_v50")
        ,mSignalCvm2ldetlvl0_v50("mSignalCvm2ldetlvl0_v50")
        ,mSignalCvm2ldetlvl1_v50("mSignalCvm2ldetlvl1_v50")
        ,mSignalCvm_res_msk_v50("mSignalCvm_res_msk_v50")
        ,mSignalCvm_diag_mask_v50("mSignalCvm_diag_mask_v50")
        ,mSignalPdcvm2bgr_v50("mSignalPdcvm2bgr_v50")
        ,mSignalPdcvm2bufo_v50("mSignalPdcvm2bufo_v50")
        ,mSignalCvmtrim0_v50("mSignalCvmtrim0_v50")
        ,mSignalCvmtrim1_v50("mSignalCvmtrim1_v50")
        ,mSignalCvmtrim2_v50("mSignalCvmtrim2_v50")
        ,mSignalCvmtrim3_v50("mSignalCvmtrim3_v50")
        ,mSignalCvmtrim4_v50("mSignalCvmtrim4_v50")
        ,mSignalCvm_disable_v50("mSignalCvm_disable_v50")
        ,mSignalCvmen_select_v50("mSignalCvmen_select_v50")
        ,mSignalMode_trans_v50("mSignalMode_trans_v50")
        ,mSignalTest_cvm2mask_v50("mSignalTest_cvm2mask_v50")
        ,mSignalPdcvm2isoh_t_v50("mSignalPdcvm2isoh_t_v50")
        ,mSignalPdcvm2isol_t_v50("mSignalPdcvm2isol_t_v50")
        ,mSignalPdcvm2awoh_t_v50("mSignalPdcvm2awoh_t_v50")
        ,mSignalPdcvm2awol_t_v50("mSignalPdcvm2awol_t_v50")
        ,mSignalVccdholdOut0("mSignalVccdholdOut0")
        ,mSignalVccdholdOut1("mSignalVccdholdOut1")
        ,mSignalVccdholdOut8("mSignalVccdholdOut8")
        ,mSignalVccdholdOut9("mSignalVccdholdOut9")
{//{{{
    CommandInit(this->name());
    Initialize();
    mCLKPeriod = 0;
    mResetFlag = false;
    mPocdetRstFlag = true;
    for(unsigned int i=0; i<18; i++){
        mCvm2hdetArr[i] = 0;
        mCvm2ldetArr[i] = 0;
    }
    mCvm2hdet_dnf = 0;
    mCvm2ldet_dnf = 0;
    selcvm2refh0_v50.initialize(0);
    selcvm2refh1_v50.initialize(0);
    selcvm2refl0_v50.initialize(0);
    selcvm2refl1_v50.initialize(0);
    errcvm2isoh_v50.initialize(0);
    errcvm2isol_v50.initialize(0);
    errcvm2awoh_v50.initialize(0);
    errcvm2awol_v50.initialize(0);
    pdcvm2isoh_v50.initialize(1);
    pdcvm2isol_v50.initialize(1);
    pdcvm2awoh_v50.initialize(1);
    pdcvm2awol_v50.initialize(1);
    pdcvm2bgr_v50.initialize(0);
    pdcvm2bufo_v50.initialize(0);
    sel_cvmtrim0_v50.initialize(0);
    sel_cvmtrim1_v50.initialize(0);
    sel_cvmtrim2_v50.initialize(0);
    sel_cvmtrim3_v50.initialize(0);
    sel_cvmtrim4_v50.initialize(0);
    cvm2hdet_v50.initialize(0);
    cvm2ldet_v50.initialize(0);
    reset_req_cvm_v50.initialize(0);
    dis_ldo_iso_v50.initialize(0);
    cvm2isoldet_flg_v50.initialize(0);
    cvm2awoldet_flg_v50.initialize(0);
    cvm_reset_v50.initialize(0);
    ei_cvmout_v50.initialize(0);
    for(unsigned int i=0; i<15; i++){
        std::ostringstream str;
        str << "vccdhold" << i;
        mVccdhold[i] = new Cvccdhold(str.str().c_str());
    }
    //vccdhold1 connection
    mVccdhold[0]->in(mSignalCvm2hdeten_v50);
    mVccdhold[0]->en(mSignalCvmde_latchen_v50);
    mVccdhold[0]->rst(mSignalPocdet_n_v50);
    mVccdhold[0]->out(mSignalVccdholdOut0);
    //vccdhold2 connection
    mVccdhold[1]->in(mSignalCvm2ldeten_v50);
    mVccdhold[1]->en(mSignalCvmde_latchen_v50);
    mVccdhold[1]->rst(mSignalPocdet_n_v50);
    mVccdhold[1]->out(mSignalVccdholdOut1);
    //vccdhold3 connection
    mVccdhold[2]->in(mSignalPdcvm2bgr_v50);
    mVccdhold[2]->en(mSignalCvm_latchen_v50);
    mVccdhold[2]->rst(mSignalPocdet_n_v50);
    mVccdhold[2]->out(pdcvm2bgr_v50);
    //vccdhold4 connection
    mVccdhold[3]->in(mSignalPdcvm2bufo_v50);
    mVccdhold[3]->en(mSignalCvm_latchen_v50);
    mVccdhold[3]->rst(mSignalPocdet_n_v50);
    mVccdhold[3]->out(pdcvm2bufo_v50);
    //vccdhold5 connection
    mVccdhold[4]->in(mSignalCvm2ldetlvl0_v50);
    mVccdhold[4]->en(mSignalCvm_latchen_v50);
    mVccdhold[4]->rst(mSignalPocdet_n_v50);
    mVccdhold[4]->out(selcvm2refl0_v50);
    //vccdhold6 connection
    mVccdhold[5]->in(mSignalCvm2ldetlvl1_v50);
    mVccdhold[5]->en(mSignalCvm_latchen_v50);
    mVccdhold[5]->rst(mSignalPocdet_n_v50);
    mVccdhold[5]->out(selcvm2refl1_v50);
    //vccdhold7 connection
    mVccdhold[6]->in(mSignalCvm2hdetlvl0_v50);
    mVccdhold[6]->en(mSignalCvm_latchen_v50);
    mVccdhold[6]->rst(mSignalPocdet_n_v50);
    mVccdhold[6]->out(selcvm2refh0_v50);
    //vccdhold8 connection
    mVccdhold[7]->in(mSignalCvm2hdetlvl1_v50);
    mVccdhold[7]->en(mSignalCvm_latchen_v50);
    mVccdhold[7]->rst(mSignalPocdet_n_v50);
    mVccdhold[7]->out(selcvm2refh1_v50);
    //vccdhold9 connection
    mVccdhold[8]->in(mSignalCvm_resreq_msk_v50);
    mVccdhold[8]->en(mSignalCvmdmask2_latchen_v50);
    mVccdhold[8]->rst(mSignalPocdet_n_v50);
    mVccdhold[8]->out(mSignalVccdholdOut8);
    //vccdhold10 connection
    mVccdhold[9]->in(mSignalCvm_res_msk_v50);
    mVccdhold[9]->en(mSignalCvmdmask1_latchen_v50);
    mVccdhold[9]->rst(mSignalPocdet_n_v50);
    mVccdhold[9]->out(mSignalVccdholdOut9);
    //vccdhold11 connection
    mVccdhold[10]->in(mSignalCvmtrim0_v50);
    mVccdhold[10]->en(mSignalCvmtrim_latchen_v50);
    mVccdhold[10]->rst(mSignalPocdet_n_v50);
    mVccdhold[10]->out(sel_cvmtrim0_v50);
    //vccdhold12 connection
    mVccdhold[11]->in(mSignalCvmtrim1_v50);
    mVccdhold[11]->en(mSignalCvmtrim_latchen_v50);
    mVccdhold[11]->rst(mSignalPocdet_n_v50);
    mVccdhold[11]->out(sel_cvmtrim1_v50);
    //vccdhold13 connection
    mVccdhold[12]->in(mSignalCvmtrim2_v50);
    mVccdhold[12]->en(mSignalCvmtrim_latchen_v50);
    mVccdhold[12]->rst(mSignalPocdet_n_v50);
    mVccdhold[12]->out(sel_cvmtrim2_v50);
    //vccdhold14 connection
    mVccdhold[13]->in(mSignalCvmtrim3_v50);
    mVccdhold[13]->en(mSignalCvmtrim_latchen_v50);
    mVccdhold[13]->rst(mSignalPocdet_n_v50);
    mVccdhold[13]->out(sel_cvmtrim3_v50);
    //vccdhold15 connection
    mVccdhold[14]->in(mSignalCvmtrim4_v50);
    mVccdhold[14]->en(mSignalCvmtrim_latchen_v50);
    mVccdhold[14]->rst(mSignalPocdet_n_v50);
    mVccdhold[14]->out(sel_cvmtrim4_v50);

    SC_METHOD(Clunc_or_v50Method);
    dont_initialize();
    sensitive << clunc_or_v50;
    sensitive << cvm_latchen;
    sensitive << cvmde_latchen;
    sensitive << cvmdmask1_latchen;
    sensitive << cvmdmask2_latchen;
    sensitive << cvmtrim_latchen;
    sensitive << cvmmsk_cold;
    sensitive << cvm_resreq_msk;
    sensitive << mReadPortEvent;
    
    SC_METHOD(Clunc_and_v50Method);
    dont_initialize();
    sensitive << clunc_and_v50;
    sensitive << pocdet_n;
    sensitive << cvm2hdeten;
    sensitive << cvm2ldeten;
    sensitive << cvm2hdetlvl0;
    sensitive << cvm2hdetlvl1;
    sensitive << cvm2ldetlvl0;
    sensitive << cvm2ldetlvl1;
    sensitive << cvm_res_msk;
    sensitive << cvm_diag_mask;
    sensitive << pdcvm2bgr;
    sensitive << pdcvm2bufo;
    sensitive << errcvm2isoh;
    sensitive << errcvm2isol;
    sensitive << errcvm2awoh;
    sensitive << errcvm2awol;
    sensitive << cvmtrim0;
    sensitive << cvmtrim1;
    sensitive << cvmtrim2;
    sensitive << cvmtrim3;
    sensitive << cvmtrim4;
    sensitive << cvm_disable;
    sensitive << cvmen_select;
    sensitive << mode_trans;
    sensitive << test_cvm2mask;
    sensitive << pdcvm2isoh_t;
    sensitive << pdcvm2isol_t;
    sensitive << pdcvm2awoh_t;
    sensitive << pdcvm2awol_t;
    sensitive << mReadPortEvent;
    sensitive << mResetEvent;

    SC_METHOD(Pocdet_n_v50Method);
    dont_initialize();
    sensitive << mSignalPocdet_n_v50;

    SC_METHOD(Clk_v50Method);
    dont_initialize();
    sensitive << clk;
    sensitive << mReadPortEvent;

    SC_METHOD(Pdcvm2h_v50Method);
    dont_initialize();
    sensitive << mSignalPocdet_n_v50;
    sensitive << mSignalVccdholdOut0;
    sensitive << mSignalCvm_disable_v50;
    sensitive << mSignalCvmen_select_v50;
    sensitive << mResetEvent;

    SC_METHOD(Pdcvm2l_v50Method);
    dont_initialize();
    sensitive << mSignalPocdet_n_v50;
    sensitive << mSignalVccdholdOut1;
    sensitive << mSignalCvm_disable_v50;
    sensitive << mSignalCvmen_select_v50;
    sensitive << mResetEvent;
    
    SC_THREAD(Cvm2hdet_dnfThread);
    dont_initialize();
    sensitive << cvm2isohdet_v50;
    sensitive << cvm2awohdet_v50;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_THREAD(Cvm2ldet_dnfThread);
    dont_initialize();
    sensitive << cvm2isoldet_v50;
    sensitive << cvm2awoldet_v50;
    sensitive << mPocdetRstEvent;
    sensitive << mResetEvent;
    sensitive << mReadPortEvent;

    SC_METHOD(Cvm2hdet_v50Method);
    dont_initialize();
    sensitive << mSignalCvmmsk_cold_v50;
    sensitive << mSignalMode_trans_v50;
    sensitive << mCvm2hdet_dnfEvent;
    sensitive << mResetEvent;

    SC_METHOD(Cvm2ldet_v50Method);
    dont_initialize();
    sensitive << mSignalCvmmsk_cold_v50;
    sensitive << mSignalMode_trans_v50;
    sensitive << mCvm2ldet_dnfEvent;
    sensitive << mResetEvent;

    SC_METHOD(Dis_ldo_iso_v50Method);
    dont_initialize();
    sensitive << mSignalCvmmsk_cold_v50;
    sensitive << mSignalMode_trans_v50;
    sensitive << mCvm2hdet_dnfEvent;
    sensitive << mSignalCvm_diag_mask_v50;
    sensitive << mSignalTest_cvm2mask_v50;
    sensitive << mResetEvent;

    SC_METHOD(Reset_req_cvm_v50Method);
    dont_initialize();
    sensitive << mCvm2hdet_dnfEvent;
    sensitive << mCvm2ldet_dnfEvent;
    sensitive << mSignalCvmmsk_cold_v50;
    sensitive << mSignalMode_trans_v50;
    sensitive << mSignalCvm_diag_mask_v50;
    sensitive << mSignalVccdholdOut8;
    sensitive << mSignalTest_cvm2mask_v50;
    sensitive << mResetEvent;

    SC_METHOD(Cvm_reset_v50Method);
    dont_initialize();
    sensitive << mCvm2hdet_dnfEvent;
    sensitive << mCvm2ldet_dnfEvent;
    sensitive << mSignalCvmmsk_cold_v50;
    sensitive << mSignalMode_trans_v50;
    sensitive << mSignalCvm_diag_mask_v50;
    sensitive << mSignalVccdholdOut9;
    sensitive << mResetEvent;
}//}}}

Cselb_vcc::~Cselb_vcc()
{//{{{
    for(unsigned int i=0; i<15; i++){
        delete mVccdhold[i]; 
    }
}//}}}

void Cselb_vcc::Initialize()
{//{{{
    mPreCvm2hdet_dnf = 0;
    mPreCvm2ldet_dnf = 0;
}//}}}

double Cselb_vcc::CalculateRisingEdgeTime()
{//{{{
    double cur_time = sc_time_stamp().to_double();
    unsigned int index = (unsigned int)(cur_time / mCLKPeriod) + 1;
    return mCLKPeriod*index - cur_time;
}//}}}

void Cselb_vcc::Clunc_and_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        errcvm2isoh_v50.write(0);
        errcvm2isol_v50.write(0);
        errcvm2awoh_v50.write(0);
        errcvm2awol_v50.write(0);
        ei_cvmout_v50.write(0);
    } else {
        mSignalPocdet_n_v50.write(pocdet_n.read() & clunc_and_v50.read());
        mSignalCvm2hdeten_v50.write(cvm2hdeten.read() & clunc_and_v50.read());
        mSignalCvm2ldeten_v50.write(cvm2ldeten.read() & clunc_and_v50.read());
        mSignalCvm2hdetlvl0_v50.write(cvm2hdetlvl0.read() & clunc_and_v50.read());
        mSignalCvm2hdetlvl1_v50.write(cvm2hdetlvl1.read() & clunc_and_v50.read());
        mSignalCvm2ldetlvl0_v50.write(cvm2ldetlvl0.read() & clunc_and_v50.read());
        mSignalCvm2ldetlvl1_v50.write(cvm2ldetlvl1.read() & clunc_and_v50.read());
        mSignalCvm_res_msk_v50.write(cvm_res_msk.read() & clunc_and_v50.read());
        mSignalCvm_diag_mask_v50.write(cvm_diag_mask.read() & clunc_and_v50.read());
        mSignalPdcvm2bgr_v50.write(pdcvm2bgr.read() & clunc_and_v50.read());
        mSignalPdcvm2bufo_v50.write(pdcvm2bufo.read() & clunc_and_v50.read());
        errcvm2isoh_v50.write(errcvm2isoh.read() & clunc_and_v50.read());
        errcvm2isol_v50.write(errcvm2isol.read() & clunc_and_v50.read());
        errcvm2awoh_v50.write(errcvm2awoh.read() & clunc_and_v50.read());
        errcvm2awol_v50.write(errcvm2awol.read() & clunc_and_v50.read());
        ei_cvmout_v50.write(cvm_diag_mask.read() & clunc_and_v50.read());
        mSignalCvmtrim0_v50.write(cvmtrim0.read() & clunc_and_v50.read());
        mSignalCvmtrim1_v50.write(cvmtrim1.read() & clunc_and_v50.read());
        mSignalCvmtrim2_v50.write(cvmtrim2.read() & clunc_and_v50.read());
        mSignalCvmtrim3_v50.write(cvmtrim3.read() & clunc_and_v50.read());
        mSignalCvmtrim4_v50.write(cvmtrim4.read() & clunc_and_v50.read());
        mSignalCvm_disable_v50.write(cvm_disable.read() & clunc_and_v50.read());
        mSignalCvmen_select_v50.write(cvmen_select.read() & clunc_and_v50.read());
        mSignalMode_trans_v50.write(mode_trans.read() & clunc_and_v50.read());
        mSignalTest_cvm2mask_v50.write(test_cvm2mask.read() & clunc_and_v50.read());
        mSignalPdcvm2isoh_t_v50.write(pdcvm2isoh_t.read() & clunc_and_v50.read());
        mSignalPdcvm2isol_t_v50.write(pdcvm2isol_t.read() & clunc_and_v50.read());
        mSignalPdcvm2awoh_t_v50.write(pdcvm2awoh_t.read() & clunc_and_v50.read());
        mSignalPdcvm2awol_t_v50.write(pdcvm2awol_t.read() & clunc_and_v50.read());
    }
}//}}}

void Cselb_vcc::Clunc_or_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        return;
    }
    mSignalCvm_latchen_v50.write(cvm_latchen.read() | clunc_or_v50.read());
    mSignalCvmde_latchen_v50.write(cvmde_latchen.read() | clunc_or_v50.read());
    mSignalCvmdmask1_latchen_v50.write(cvmdmask1_latchen.read() | clunc_or_v50.read());
    mSignalCvmdmask2_latchen_v50.write(cvmdmask2_latchen.read() | clunc_or_v50.read());
    mSignalCvmtrim_latchen_v50.write(cvmtrim_latchen.read() | clunc_or_v50.read());
    mSignalCvmmsk_cold_v50.write(cvmmsk_cold.read() | clunc_or_v50.read());
    mSignalCvm_resreq_msk_v50.write(cvm_resreq_msk.read() | clunc_or_v50.read());
}//}}}

bool Cselb_vcc::Pdcvm2_v50Func(bool cvm2deten)
{//{{{
    bool pdcvm2 = 0;
    if(mSignalCvmen_select_v50.read() == 1){
        pdcvm2 = mSignalCvm_disable_v50.read() | (!cvm2deten);
    } else {
        pdcvm2 = mSignalCvm_disable_v50.read() | (!(mSignalPocdet_n_v50.read()));
    }
    return pdcvm2;
}//}}}

void Cselb_vcc::Pdcvm2h_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        pdcvm2isoh_v50.write(1);
        pdcvm2awoh_v50.write(1);
    } else{
        bool pdcvm2h = Pdcvm2_v50Func(mSignalVccdholdOut0.read());
        pdcvm2isoh_v50.write(pdcvm2h);
        pdcvm2awoh_v50.write(pdcvm2h);
    }
}//}}}

void Cselb_vcc::Pdcvm2l_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        pdcvm2isol_v50.write(1);
        pdcvm2awol_v50.write(1);
    } else{
        bool pdcvm2l = Pdcvm2_v50Func(mSignalVccdholdOut1.read());
        pdcvm2isol_v50.write(pdcvm2l);
        pdcvm2awol_v50.write(pdcvm2l);
    }
}//}}}

void Cselb_vcc::Pocdet_n_v50Method()
{//{{{
    if(mSignalPocdet_n_v50.read() == 0){
        mPocdetRstFlag = true;
    } else{
        mPocdetRstFlag = false;
    }
    mPocdetRstEvent.notify();
}//}}}

void Cselb_vcc::Cvm2det_dnfFunc(unsigned int id, bool *arr, bool &preiso, bool iso, bool &preawo, bool awo, unsigned int &RemainUpdateCycle)
{//{{{
    unsigned int count0 = 0;
    for(unsigned int i=1; i<=emDFFOfMux; i++){
        if(arr[i] == 0){
            count0++;
        }
    }
    switch (id){
        case 1:
            if((count0 == emDFFOfMux) || (count0 == 0)){
                mCvm2hdet_dnf = arr[1];
                if(mCvm2hdet_dnf != mPreCvm2hdet_dnf){
                    mPreCvm2hdet_dnf = mCvm2hdet_dnf;
                    mCvm2hdet_dnfEvent.notify();
                }
            }
            break;
        case 2:
            if((count0 == emDFFOfMux) || (count0 == 0)){
                mCvm2ldet_dnf = arr[1];
                if(mCvm2ldet_dnf != mPreCvm2ldet_dnf){
                    mPreCvm2ldet_dnf = mCvm2ldet_dnf;
                    mCvm2ldet_dnfEvent.notify();
                }
            }
            break;
        default:
            break;
    }
    if((preiso != iso) || (preawo != awo)){
        preiso = iso;
        preawo = awo;
        RemainUpdateCycle = emOutputUpdateCycle + 1;
    }
    for(unsigned int i=emDFFOfMux; i>0; i--){
        arr[i] = arr[i -1];
    }
    arr[0] = (iso | awo);
}//}}}

void Cselb_vcc::Cvm2hdet_dnfThread()
{//{{{
    for(;;){
        if(mPocdetRstFlag || Cgeneral_reset::IsResetPeriod()){
            //Reset is active
            for(unsigned int i=0; i<18; i++){
                mCvm2hdetArr[i] = 0;
            }
            mCvm2hdet_dnf = 0;
            mCvm2hdet_dnfEvent.notify();
            wait();
        }
        try{
            while(true){
                if(mCLKPeriod != 0 && (!mPocdetRstFlag) && (!mResetFlag)){
                    bool PreCvm2isohdet_v50 = cvm2isohdet_v50.read();
                    bool PreCvm2awohdet_v50 = cvm2awohdet_v50.read();
                    for(unsigned int RemainCvm2hdetUpdateCycle=emOutputUpdateCycle; RemainCvm2hdetUpdateCycle>0; RemainCvm2hdetUpdateCycle--){
                        double syn_time = CalculateRisingEdgeTime();
                        wait((unsigned int)syn_time, SC_NS, mPocdetRstEvent | mResetEvent);
                        if(mPocdetRstFlag || mResetFlag){
                            throw vpcl::emRstException;
                        }
                        Cvm2det_dnfFunc(1, mCvm2hdetArr, PreCvm2isohdet_v50, cvm2isohdet_v50.read(), PreCvm2awohdet_v50, cvm2awohdet_v50.read(),RemainCvm2hdetUpdateCycle);
                    }
                }
                wait();
                if(mPocdetRstFlag || mResetFlag){
                    throw vpcl::emRstException;
                }
            }
        }
        catch(vpcl::eResetException){
        }
    }
}//}}}

void Cselb_vcc::Cvm2ldet_dnfThread()
{//{{{
    for(;;){
        if(mPocdetRstFlag || Cgeneral_reset::IsResetPeriod()){
            //Reset is active
            for(unsigned int i=0; i<18; i++){
                mCvm2ldetArr[i] = 0;
            }
            mCvm2ldet_dnf = 0;
            mCvm2ldet_dnfEvent.notify();
            wait();
        }
        try{
            while(true){
                if(mCLKPeriod != 0 && (!mPocdetRstFlag) && (!mResetFlag)){
                    bool PreCvm2isoldet_v50 = cvm2isoldet_v50.read();
                    bool PreCvm2awoldet_v50 = cvm2awoldet_v50.read();
                    for(unsigned int RemainCvm2ldetUpdateCycle=emOutputUpdateCycle; RemainCvm2ldetUpdateCycle>0; RemainCvm2ldetUpdateCycle--){
                        double syn_time = CalculateRisingEdgeTime();
                        wait((unsigned int)syn_time, SC_NS, mPocdetRstEvent | mResetEvent);
                        if(mPocdetRstFlag || mResetFlag){
                            throw vpcl::emRstException;
                        }
                        Cvm2det_dnfFunc(2, mCvm2ldetArr, PreCvm2isoldet_v50, cvm2isoldet_v50.read(), PreCvm2awoldet_v50, cvm2awoldet_v50.read(),RemainCvm2ldetUpdateCycle);
                    }
                }
                wait();
                if(mPocdetRstFlag || mResetFlag){
                    throw vpcl::emRstException;
                }
            }
        }
        catch(vpcl::eResetException){
        }
    }
}//}}}

bool Cselb_vcc::Cvm2det_v50Func(bool condition1, bool condition2, bool signal)
{//{{{
    bool cvm2det = 0;
    if((condition1 == 0) && (condition2 == 0)){
        cvm2det = signal;
    }
    return cvm2det;
}//}}}

void Cselb_vcc::Cvm2hdet_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        cvm2hdet_v50.write(0);
    } else {
        bool cvm2hdet = Cvm2det_v50Func(mSignalCvmmsk_cold_v50.read(), mSignalMode_trans_v50.read(), mCvm2hdet_dnf);
        cvm2hdet_v50.write(cvm2hdet);
    }
}//}}}

void Cselb_vcc::Cvm2ldet_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        cvm2ldet_v50.write(0);
    } else {
        bool cvm2ldet = Cvm2det_v50Func(mSignalCvmmsk_cold_v50.read(), mSignalMode_trans_v50.read(), mCvm2ldet_dnf);
        cvm2ldet_v50.write(cvm2ldet);
    }
}//}}}

void Cselb_vcc::Dis_ldo_iso_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        dis_ldo_iso_v50.write(0);
    } else {
        bool disldoiso = 0;
        if((mSignalCvmmsk_cold_v50.read() == 0) && (mSignalMode_trans_v50.read() == 0) && (mSignalCvm_diag_mask_v50.read() == 0)){
            disldoiso = (!(mSignalTest_cvm2mask_v50.read())) & mCvm2hdet_dnf;
        }
        dis_ldo_iso_v50.write(disldoiso);
    }
}//}}}

void Cselb_vcc::Reset_req_cvm_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        reset_req_cvm_v50.write(0);
    } else {
        bool resetreqcvm = 0;
        bool thirdselect = mSignalCvm_diag_mask_v50.read() & mSignalVccdholdOut8.read();
        if((mSignalCvmmsk_cold_v50.read() == 0) && (mSignalMode_trans_v50.read() == 0) && (thirdselect == 0)){
            resetreqcvm = (!(mSignalTest_cvm2mask_v50.read())) & (mCvm2hdet_dnf | mCvm2ldet_dnf);
        }
        reset_req_cvm_v50.write(resetreqcvm);
    }
}//}}}

void Cselb_vcc::Cvm_reset_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        cvm_reset_v50.write(0);
    } else {
        bool cvmreset = 0;
        bool thirdselect = mSignalCvm_diag_mask_v50.read() & mSignalVccdholdOut9.read();
        if((mSignalCvmmsk_cold_v50.read() == 0) && (mSignalMode_trans_v50.read() == 0) && (thirdselect == 0)){
            cvmreset = mCvm2hdet_dnf | mCvm2ldet_dnf;
        } else if(mSignalCvmmsk_cold_v50.read() == 0){
            cvmreset = 1;
        }
        cvm_reset_v50.write(cvmreset);
    }
}//}}}

void Cselb_vcc::Clk_v50Method()
{//{{{
    if(Cgeneral_reset::IsResetPeriod()){
        return;
    }
    double clk_freq = sc_dt::uint64_to_double(clk.read());
    SetCLKfreq("clk_v50",clk_freq);
}//}}}

void Cselb_vcc::SetCLKfreq(const std::string clk_name, const double clk_freq)
{//{{{
    if (clk_name == "clk_v50") {
        if (clk_freq > 0) {
            // Calculate the clock period to check the resolution
            double clk_period = (double)(emNanoSecond / clk_freq);
            double time_unit = (double)(emNanoSecond / Cgeneral_reset::GetTimeResolution());
            if(time_unit > clk_period){
                re_printf("warning","The clk_v50 period is less than 1 unit time of system.\n");
                return;
            }
            mCLKPeriod = clk_period;
            sc_time clock_value(mCLKPeriod, SC_NS);
            for(unsigned int i=0; i<15; i++){
                mVccdhold[i]->mCLKPeriod = mCLKPeriod;
            }
            re_printf("info","clk_v50 is set with a frequency as %f.\n",clk_freq);
        } else {
            re_printf("error","Clock clk_v50 = %f is invalid.\n", clk_freq);
        }
    } else {
        re_printf("warning","Clock name = %s is invalid.\n", clk_name.c_str());
    }
}//}}}

void Cselb_vcc::AssertReset(const double delay, const double period)
{//{{{
    Cgeneral_reset::Com_AssertReset(delay, period);
}//}}}

void Cselb_vcc::EnableReset(const bool is_active)
{//{{{
    if(is_active){
        Initialize();
        mResetFlag = true;
        mResetEvent.notify();
    } else{
        mResetFlag = false;
        mReadPortEvent.notify();
    }
    for(unsigned int i=0; i<15; i++){
        mVccdhold[i]->AssertResetFunc(is_active);
    }
}//}}}
// vim600: set foldmethod=marker :
