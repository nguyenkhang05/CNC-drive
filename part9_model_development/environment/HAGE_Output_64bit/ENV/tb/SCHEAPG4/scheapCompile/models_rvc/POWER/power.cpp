// -----------------------------------------------------------------------------
// $Id: power.cpp,v 1.6 2013/04/01 09:28:13 ducduong Exp $
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


#include "power.h"

Cpower::Cpower(sc_module_name name):
         cvm2hdeten("cvm2hdeten")
        ,cvm2ldeten("cvm2ldeten")
        ,cvm2hdetlvl0("cvm2hdetlvl0")
        ,cvm2hdetlvl1("cvm2hdetlvl1")
        ,cvm2ldetlvl0("cvm2ldetlvl0")
        ,cvm2ldetlvl1("cvm2ldetlvl1")
        ,cvm_res_msk("cvm_res_msk")
        ,cvm_resreq_msk("cvm_resreq_msk")
        ,cvm_diag_mask("cvm_diag_mask")
        ,pdcvm2isoh_t("pdcvm2isoh_t")
        ,pdcvm2isol_t("pdcvm2isol_t")
        ,pdcvm2awoh_t("pdcvm2awoh_t")
        ,pdcvm2awol_t("pdcvm2awol_t")
        ,clunc_and_v50("clunc_and_v50")
        ,clunc_or_v50("clunc_or_v50")
        ,cvm2hdet_v50("cvm2hdet_v50")
        ,cvm2ldet_v50("cvm2ldet_v50")
        ,reset_req_cvm_v50("reset_req_cvm_v50")
        ,dis_ldo_iso_v50("dis_ldo_iso_v50")
        ,cvm2isoldet_flg_v50("cvm2isoldet_flg_v50")
        ,cvm2awoldet_flg_v50("cvm2awoldet_flg_v50")
        ,cvm_reset_v50("cvm_reset_v50")
        ,ei_cvmout_v50("ei_cvmout_v50")
        ,cvmtrim0_in("cvmtrim0_in")
        ,cvmtrim1_in("cvmtrim1_in")
        ,cvmtrim2_in("cvmtrim2_in")
        ,cvmtrim3_in("cvmtrim3_in")
        ,cvmtrim4_in("cvmtrim4_in")
        ,en0("en0")
        ,en1("en1")
        ,en2("en2")
        ,en3("en3")
        ,en4("en4")
        ,diag_mode("diag_mode")
        ,cvmde_wen("cvmde_wen")
        ,cvmdmask1_wen("cvmdmask1_wen")
        ,cvmdmask2_wen("cvmdmask2_wen")
        ,clk("clk")
        ,clk_roscl("clk_roscl")
        ,moscen("moscen")
        ,pllen("pllen")
        ,cpuclks0("cpuclks0")
        ,cpuclks1("cpuclks1")
        ,cpuclks2("cpuclks2")
//        ,pocdet_n("pocdet_n")
        ,resz12("resz12")
        ,reset_fct("reset_fct")
        ,mstprqz("mstprqz")
        ,stop("stop")
        ,off("off")
        ,cyclic("cyclic")
        ,isolate("isolate")
        ,vccholdtrg("vccholdtrg")
        ,haltst("haltst")
        ,pdcvm2bgr_t("pdcvm2bgr_t")
        ,pdcvm2bufo_t("pdcvm2bufo_t")
        ,errcvm2isoh_t("errcvm2isoh_t")
        ,errcvm2isol_t("errcvm2isol_t")
        ,errcvm2awoh_t("errcvm2awoh_t")
        ,errcvm2awol_t("errcvm2awol_t")
        ,tapkeep("tapkeep")
        ,cvm2tmsk("cvm2tmsk")

        ,CLK_V00("CLK_V00")
        ,CLR_POF("CLR_POF")
        ,poradet_n_v50("poradet_n_v50")
        ,pocadet_n_v50("pocadet_n_v50")
        ,pocdet_n_v50("pocdet_n_v50")
        ,det0det_v50("det0det_v50")
        ,REQ_POF("REQ_POF")
        ,DET0DET_MSK("DET0DET_MSK")
        ,POCADET_N_MSK("POCADET_N_MSK")
        ,POCDET_N_MSK("POCDET_N_MSK")
        //Signal
        ,mSignalPdcvm2bgr("mSignalPdcvm2bgr")
        ,mSignalPdcvm2bufo("mSignalPdcvm2bufo")
        ,mSignalErrcvm2isoh("mSignalErrcvm2isoh")
        ,mSignalErrcvm2isol("mSignalErrcvm2isol")
        ,mSignalErrcvm2awoh("mSignalErrcvm2awoh")
        ,mSignalErrcvm2awol("mSignalErrcvm2awol")
        ,mSignalCvmtrim0("mSignalCvmtrim0")
        ,mSignalCvmtrim1("mSignalCvmtrim1")
        ,mSignalCvmtrim2("mSignalCvmtrim2")
        ,mSignalCvmtrim3("mSignalCvmtrim3")
        ,mSignalCvmtrim4("mSignalCvmtrim4")
        ,mSignalCvm_disable("mSignalCvm_disable")
        ,mSignalCvm_latchen("mSignalCvm_latchen")
        ,mSignalCvmde_latchen("mSignalCvmde_latchen")
        ,mSignalCvmdmask1_latchen("mSignalCvmdmask1_latchen")
        ,mSignalCvmdmask2_latchen("mSignalCvmdmask2_latchen")
        ,mSignalCvmtrim_latchen("mSignalCvmtrim_latchen")
        ,mSignalCvmen_select("mSignalCvmen_select")
        ,mSignalCvmmsk_cold("mSignalCvmmsk_cold")
        ,mSignalMode_trans("mSignalMode_trans")
        ,mSignalTest_cvm2mask("mSignalTest_cvm2mask")
        ,mSignalCvm2isohdet_v50("mSignalCvm2isohdet_v50")
        ,mSignalCvm2isoldet_v50("mSignalCvm2isoldet_v50")
        ,mSignalCvm2awohdet_v50("mSignalCvm2awohdet_v50")
        ,mSignalCvm2awoldet_v50("mSignalCvm2awoldet_v50")
        ,mSignalSelcvm2refh0_v50("mSignalSelcvm2refh0_v50")
        ,mSignalSelcvm2refh1_v50("mSignalSelcvm2refh1_v50")
        ,mSignalSelcvm2refl0_v50("mSignalSelcvm2refl0_v50")
        ,mSignalSelcvm2refl1_v50("mSignalSelcvm2refl1_v50")
        ,mSignalErrcvm2isoh_v50("mSignalErrcvm2isoh_v50")
        ,mSignalErrcvm2isol_v50("mSignalErrcvm2isol_v50")
        ,mSignalErrcvm2awoh_v50("mSignalErrcvm2awoh_v50")
        ,mSignalErrcvm2awol_v50("mSignalErrcvm2awol_v50")
        ,mSignalPdcvm2isoh_v50("mSignalPdcvm2isoh_v50")
        ,mSignalPdcvm2isol_v50("mSignalPdcvm2isol_v50")
        ,mSignalPdcvm2awoh_v50("mSignalPdcvm2awoh_v50")
        ,mSignalPdcvm2awol_v50("mSignalPdcvm2awol_v50")
        ,mSignalPdcvm2bgr_v50("mSignalPdcvm2bgr_v50")
        ,mSignalPdcvm2bufo_v50("mSignalPdcvm2bufo_v50")
        ,mSignalSel_cvmtrim0_v50("mSignalSel_cvmtrim0_v50")
        ,mSignalSel_cvmtrim1_v50("mSignalSel_cvmtrim1_v50")
        ,mSignalSel_cvmtrim2_v50("mSignalSel_cvmtrim2_v50")
        ,mSignalSel_cvmtrim3_v50("mSignalSel_cvmtrim3_v50")
        ,mSignalSel_cvmtrim4_v50("mSignalSel_cvmtrim4_v50")
        ,mSignalHold_cvmtrim0("mSignalHold_cvmtrim0")
        ,mSignalHold_cvmtrim1("mSignalHold_cvmtrim1")
        ,mSignalHold_cvmtrim2("mSignalHold_cvmtrim2")
        ,mSignalHold_cvmtrim3("mSignalHold_cvmtrim3")
        ,mSignalHold_cvmtrim4("mSignalHold_cvmtrim4")
{//{{{
    mHold_cvm = new Chold_cvm("hold_cvm");
    sc_assert(mHold_cvm != NULL);
    mCvm_awo  = new Ccvm_awo("cvm_awo");
    sc_assert(mCvm_awo != NULL);
    mSelb_vcc = new Cselb_vcc("selb_vcc");
    sc_assert(mSelb_vcc != NULL);
    mCvm2     = new Ccvm2("cvm2");
    sc_assert(mCvm2 != NULL);
    mPoc      = new Cpoc("poc");
    sc_assert(mPoc != NULL);
    //Input ports of hold_cvm connect to outside ports
    mHold_cvm->CLK(clk);
    mHold_cvm->SIGIN0(cvmtrim0_in);
    mHold_cvm->SIGIN1(cvmtrim1_in);
    mHold_cvm->SIGIN2(cvmtrim2_in);
    mHold_cvm->SIGIN3(cvmtrim3_in);
    mHold_cvm->SIGIN4(cvmtrim4_in);
    mHold_cvm->EN0(en0);
    mHold_cvm->EN1(en1);
    mHold_cvm->EN2(en2);
    mHold_cvm->EN3(en3);
    mHold_cvm->EN4(en4);
    mHold_cvm->RESZ(resz12);
    //Outside ports and output ports of hold_cvm connect to input ports of cvm_awo
    mHold_cvm->SIGOUT0(mSignalHold_cvmtrim0);
    mHold_cvm->SIGOUT1(mSignalHold_cvmtrim1);
    mHold_cvm->SIGOUT2(mSignalHold_cvmtrim2);
    mHold_cvm->SIGOUT3(mSignalHold_cvmtrim3);
    mHold_cvm->SIGOUT4(mSignalHold_cvmtrim4);
    //input ports of cvm_awo connect to outside ports and output ports of hold_cvm
    mCvm_awo->diag_mode(diag_mode);
    mCvm_awo->cvmde_wen(cvmde_wen);
    mCvm_awo->cvmdmask1_wen(cvmdmask1_wen);
    mCvm_awo->cvmdmask2_wen(cvmdmask2_wen);
    mCvm_awo->cvmtrim0_in(cvmtrim0_in);
    mCvm_awo->cvmtrim1_in(cvmtrim1_in);
    mCvm_awo->cvmtrim2_in(cvmtrim2_in);
    mCvm_awo->cvmtrim3_in(cvmtrim3_in);
    mCvm_awo->cvmtrim4_in(cvmtrim4_in);
    mCvm_awo->hold_cvmtrim0(mSignalHold_cvmtrim0);
    mCvm_awo->hold_cvmtrim1(mSignalHold_cvmtrim1);
    mCvm_awo->hold_cvmtrim2(mSignalHold_cvmtrim2);
    mCvm_awo->hold_cvmtrim3(mSignalHold_cvmtrim3);
    mCvm_awo->hold_cvmtrim4(mSignalHold_cvmtrim4);
    mCvm_awo->clk(clk);
    mCvm_awo->clk_roscl(clk_roscl);
    mCvm_awo->moscen(moscen);
    mCvm_awo->pllen(pllen);
    mCvm_awo->cpuclks0(cpuclks0);
    mCvm_awo->cpuclks1(cpuclks1);
    mCvm_awo->cpuclks2(cpuclks2);
//    mCvm_awo->pocdet_n(pocdet_n);
    mCvm_awo->pocdet_n(POCDET_N_MSK);
    mCvm_awo->resz12(resz12);
    mCvm_awo->reset_fct(reset_fct);
    mCvm_awo->mstprqz(mstprqz);
    mCvm_awo->stop(stop);
    mCvm_awo->off(off);
    mCvm_awo->cyclic(cyclic);
    mCvm_awo->isolate(isolate);
    mCvm_awo->vccholdtrg(vccholdtrg);
    mCvm_awo->haltst(haltst);
    mCvm_awo->pdcvm2bgr_t(pdcvm2bgr_t);
    mCvm_awo->pdcvm2bufo_t(pdcvm2bufo_t);
    mCvm_awo->errcvm2isoh_t(errcvm2isoh_t);
    mCvm_awo->errcvm2isol_t(errcvm2isol_t);
    mCvm_awo->errcvm2awoh_t(errcvm2awoh_t);
    mCvm_awo->errcvm2awol_t(errcvm2awol_t);
    mCvm_awo->tapkeep(tapkeep);
    mCvm_awo->cvm2tmsk(cvm2tmsk);
    //Outside ports and outputs of cvm_awo connect to inputs of selb_vcc
    mCvm_awo->pdcvm2bgr(mSignalPdcvm2bgr);
    mCvm_awo->pdcvm2bufo(mSignalPdcvm2bufo);
    mCvm_awo->errcvm2isoh(mSignalErrcvm2isoh);
    mCvm_awo->errcvm2isol(mSignalErrcvm2isol);
    mCvm_awo->errcvm2awoh(mSignalErrcvm2awoh);
    mCvm_awo->errcvm2awol(mSignalErrcvm2awol);
    mCvm_awo->cvmtrim0(mSignalCvmtrim0);
    mCvm_awo->cvmtrim1(mSignalCvmtrim1);
    mCvm_awo->cvmtrim2(mSignalCvmtrim2);
    mCvm_awo->cvmtrim3(mSignalCvmtrim3);
    mCvm_awo->cvmtrim4(mSignalCvmtrim4);
    mCvm_awo->cvm_disable(mSignalCvm_disable);
    mCvm_awo->cvm_latchen(mSignalCvm_latchen);
    mCvm_awo->cvmde_latchen(mSignalCvmde_latchen);
    mCvm_awo->cvmdmask1_latchen(mSignalCvmdmask1_latchen);
    mCvm_awo->cvmdmask2_latchen(mSignalCvmdmask2_latchen);
    mCvm_awo->cvmtrim_latchen(mSignalCvmtrim_latchen);
    mCvm_awo->cvmen_select(mSignalCvmen_select);
    mCvm_awo->cvmmsk_cold(mSignalCvmmsk_cold);
    mCvm_awo->mode_trans(mSignalMode_trans);
    mCvm_awo->test_cvm2mask(mSignalTest_cvm2mask);
    mCvm2->cvm2isohdet_v50(mSignalCvm2isohdet_v50);
    mCvm2->cvm2isoldet_v50(mSignalCvm2isoldet_v50);
    mCvm2->cvm2awohdet_v50(mSignalCvm2awohdet_v50);
    mCvm2->cvm2awoldet_v50(mSignalCvm2awoldet_v50);
    //Input of selb_vcc connect to outside and output of cvm_awo
    mSelb_vcc->cvm2hdeten(cvm2hdeten);
    mSelb_vcc->cvm2ldeten(cvm2ldeten);
    mSelb_vcc->cvm2hdetlvl0(cvm2hdetlvl0);
    mSelb_vcc->cvm2hdetlvl1(cvm2hdetlvl1);
    mSelb_vcc->cvm2ldetlvl0(cvm2ldetlvl0);
    mSelb_vcc->cvm2ldetlvl1(cvm2ldetlvl1);
    mSelb_vcc->cvm_res_msk(cvm_res_msk);
    mSelb_vcc->cvm_resreq_msk(cvm_resreq_msk);
    mSelb_vcc->cvm_diag_mask(cvm_diag_mask);
    mSelb_vcc->pdcvm2bgr(mSignalPdcvm2bgr);
    mSelb_vcc->pdcvm2bufo(mSignalPdcvm2bufo);
    mSelb_vcc->errcvm2isoh(mSignalErrcvm2isoh);
    mSelb_vcc->errcvm2isol(mSignalErrcvm2isol);
    mSelb_vcc->errcvm2awoh(mSignalErrcvm2awoh);
    mSelb_vcc->errcvm2awol(mSignalErrcvm2awol);
    mSelb_vcc->cvmtrim0(mSignalCvmtrim0);
    mSelb_vcc->cvmtrim1(mSignalCvmtrim1);
    mSelb_vcc->cvmtrim2(mSignalCvmtrim2);
    mSelb_vcc->cvmtrim3(mSignalCvmtrim3);
    mSelb_vcc->cvmtrim4(mSignalCvmtrim4);
    mSelb_vcc->cvm_disable(mSignalCvm_disable);
    mSelb_vcc->cvm_latchen(mSignalCvm_latchen);
    mSelb_vcc->cvmde_latchen(mSignalCvmde_latchen);
    mSelb_vcc->cvmdmask1_latchen(mSignalCvmdmask1_latchen);
    mSelb_vcc->cvmdmask2_latchen(mSignalCvmdmask2_latchen);
    mSelb_vcc->cvmtrim_latchen(mSignalCvmtrim_latchen);
    mSelb_vcc->cvmen_select(mSignalCvmen_select);
    mSelb_vcc->cvmmsk_cold(mSignalCvmmsk_cold);
    mSelb_vcc->mode_trans(mSignalMode_trans);
    mSelb_vcc->test_cvm2mask(mSignalTest_cvm2mask);
    mSelb_vcc->pdcvm2isoh_t(pdcvm2isoh_t);
    mSelb_vcc->pdcvm2isol_t(pdcvm2isol_t);
    mSelb_vcc->pdcvm2awoh_t(pdcvm2awoh_t);
    mSelb_vcc->pdcvm2awol_t(pdcvm2awol_t);
    mSelb_vcc->clk(clk);
//    mSelb_vcc->pocdet_n(pocdet_n);
    mSelb_vcc->pocdet_n(POCDET_N_MSK);
    mSelb_vcc->cvm2isohdet_v50(mSignalCvm2isohdet_v50);
    mSelb_vcc->cvm2isoldet_v50(mSignalCvm2isoldet_v50);
    mSelb_vcc->cvm2awohdet_v50(mSignalCvm2awohdet_v50);
    mSelb_vcc->cvm2awoldet_v50(mSignalCvm2awoldet_v50);
    mSelb_vcc->clunc_and_v50(clunc_and_v50);
    mSelb_vcc->clunc_or_v50(clunc_or_v50);
    //Output ports of selb_vcc connect to input ports of cvm2 and outside ports
    mSelb_vcc->selcvm2refh0_v50(mSignalSelcvm2refh0_v50);
    mSelb_vcc->selcvm2refh1_v50(mSignalSelcvm2refh1_v50);
    mSelb_vcc->selcvm2refl0_v50(mSignalSelcvm2refl0_v50);
    mSelb_vcc->selcvm2refl1_v50(mSignalSelcvm2refl1_v50);
    mSelb_vcc->errcvm2isoh_v50(mSignalErrcvm2isoh_v50);
    mSelb_vcc->errcvm2isol_v50(mSignalErrcvm2isol_v50);
    mSelb_vcc->errcvm2awoh_v50(mSignalErrcvm2awoh_v50);
    mSelb_vcc->errcvm2awol_v50(mSignalErrcvm2awol_v50);
    mSelb_vcc->pdcvm2isoh_v50(mSignalPdcvm2isoh_v50);
    mSelb_vcc->pdcvm2isol_v50(mSignalPdcvm2isol_v50);
    mSelb_vcc->pdcvm2awoh_v50(mSignalPdcvm2awoh_v50);
    mSelb_vcc->pdcvm2awol_v50(mSignalPdcvm2awol_v50);
    mSelb_vcc->pdcvm2bgr_v50(mSignalPdcvm2bgr_v50);
    mSelb_vcc->pdcvm2bufo_v50(mSignalPdcvm2bufo_v50);
    mSelb_vcc->sel_cvmtrim0_v50(mSignalSel_cvmtrim0_v50);
    mSelb_vcc->sel_cvmtrim1_v50(mSignalSel_cvmtrim1_v50);
    mSelb_vcc->sel_cvmtrim2_v50(mSignalSel_cvmtrim2_v50);
    mSelb_vcc->sel_cvmtrim3_v50(mSignalSel_cvmtrim3_v50);
    mSelb_vcc->sel_cvmtrim4_v50(mSignalSel_cvmtrim4_v50);
    mSelb_vcc->cvm2hdet_v50(cvm2hdet_v50);
    mSelb_vcc->cvm2ldet_v50(cvm2ldet_v50);
    mSelb_vcc->reset_req_cvm_v50(reset_req_cvm_v50);
    mSelb_vcc->dis_ldo_iso_v50(dis_ldo_iso_v50);
    mSelb_vcc->cvm2isoldet_flg_v50(cvm2isoldet_flg_v50);
    mSelb_vcc->cvm2awoldet_flg_v50(cvm2awoldet_flg_v50);
    mSelb_vcc->cvm_reset_v50(cvm_reset_v50);
    mSelb_vcc->ei_cvmout_v50(ei_cvmout_v50);
    //Input ports of cvm2 connect to output ports of selb_vcc
    mCvm2->selcvm2refh0_v50(mSignalSelcvm2refh0_v50);
    mCvm2->selcvm2refh1_v50(mSignalSelcvm2refh1_v50);
    mCvm2->selcvm2refl0_v50(mSignalSelcvm2refl0_v50);
    mCvm2->selcvm2refl1_v50(mSignalSelcvm2refl1_v50);
    mCvm2->errcvm2isoh_v50(mSignalErrcvm2isoh_v50);
    mCvm2->errcvm2isol_v50(mSignalErrcvm2isol_v50);
    mCvm2->errcvm2awoh_v50(mSignalErrcvm2awoh_v50);
    mCvm2->errcvm2awol_v50(mSignalErrcvm2awol_v50);
    mCvm2->pdcvm2isoh_v50(mSignalPdcvm2isoh_v50);
    mCvm2->pdcvm2isol_v50(mSignalPdcvm2isol_v50);
    mCvm2->pdcvm2awoh_v50(mSignalPdcvm2awoh_v50);
    mCvm2->pdcvm2awol_v50(mSignalPdcvm2awol_v50);
    mCvm2->pdcvm2bgr_v50(mSignalPdcvm2bgr_v50);
    mCvm2->pdcvm2bufo_v50(mSignalPdcvm2bufo_v50);
    mCvm2->sel_cvmtrim0_v50(mSignalSel_cvmtrim0_v50);
    mCvm2->sel_cvmtrim1_v50(mSignalSel_cvmtrim1_v50);
    mCvm2->sel_cvmtrim2_v50(mSignalSel_cvmtrim2_v50);
    mCvm2->sel_cvmtrim3_v50(mSignalSel_cvmtrim3_v50);
    mCvm2->sel_cvmtrim4_v50(mSignalSel_cvmtrim4_v50);
    //poc connect to power
    mPoc->CLK_V00(CLK_V00);
    mPoc->CLR_POF(CLR_POF);
    mPoc->poradet_n_v50(poradet_n_v50);
    mPoc->pocadet_n_v50(pocadet_n_v50);
    mPoc->pocdet_n_v50(pocdet_n_v50);
    mPoc->det0det_v50(det0det_v50);
    mPoc->DET0DET_MSK(DET0DET_MSK);
    mPoc->POCADET_N_MSK(POCADET_N_MSK);
    mPoc->POCDET_N_MSK(POCDET_N_MSK);
    mPoc->REQ_POF(REQ_POF);
}//}}}

Cpower::~Cpower()
{//{{{
    delete mHold_cvm;
    delete mCvm_awo;
    delete mSelb_vcc;
    delete mCvm2;
    delete mPoc;
}//}}}

// vim600: set foldmethod=marker :
