// ---------------------------------------------------------------------
// $Id: DFE0051_FIFO.cpp,v 1.2 2017/07/21 08:48:22 chuonghoangle Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "DFE0051_FIFO.h"
#include "DFE0051_FIFO_Utils.h"
#include <iomanip>
#include <string>
#include <sstream>
using std::ostringstream;
using std::endl;
//------------------------------------------------------------------------------
/**@brief public constructor
 * @param[in] iname name of the DFE0051 module
 * @param[in] iDFE0051_MACRO_NUMB number of the DFE0051 macros, default value is 2
 * @param[in] iDFBA_CHN_NUM number of the FIFO channels of the buffer A circuit, default value is 6
 * @param[in] iDFBA_DEPTH number of stages (a.k.a depth) of a FIFO channel of the buffer A circuit, default value is 8
 * @param[in] iDFBB_DEPTH number of stages (a.k.a depth) of buffer B circuit, default value is 8
 * Detailed steps as follows:
 */
DFE0051_FIFO::DFE0051_FIFO(const sc_module_name& iname
                  ,const uint8_t&        iDFE0051_MACRO_NUMB
                  ,const uint8_t&        iDFBA_CHN_NUM
                  ,const uint8_t&        iDFBA_DEPTH
                  ,const uint8_t&        iDFBB_DEPTH
                  )
: sc_module (iname)
, CclkCtrl(1)
, CrstCtrl(2)
// constants
, DFE0051_MACRO_NUMB (iDFE0051_MACRO_NUMB)
, DFE0051_CHN_NUMB   (NULL)
, DFBA_CHN_NUM   (iDFBA_CHN_NUM)
, DFBA_DEPTH     (iDFBA_DEPTH)
, DFBB_DEPTH     (iDFBB_DEPTH)
// initialize public interfaces: in/out/inout ports, sockets, events notifying the exterior, APIs
// clock, reset and slave bus interface
, pclk      ("CLK_HSB")
, presetn   ("PRESETn")
// ports interfacing with buffer A circuit
, dfe0051_tim_ctrig ("dfe0051_tim_ctrig")
// ports interfacing with DFE0051 channels
, dfe0051_do_ctrig    (NULL)
, dfe0051_do          (NULL)
, dfe0051_do_upd      (NULL)
, dfe0051_dfifo_cack  (NULL)
// ports issues interrupt requests
, dfe0051_ba_ce_irq ("dfe0051_ba_ce_irq")
, dfe0051_bb_ce_irq ("dfe0051_bb_ce_irq")
, dfe0051_berr_irq  ("dfe0051_berr_irq")
// relationship of AgentController with Function Model
, pFuncModel        (NULL)
, pFFAs             (NULL)
// internal signals
, dfe0051_do_asel     (NULL)
, dfe0051_do_upd_asel (NULL)
, ba_di           (NULL)
, ba_wr           (NULL)
, ba_cack         (NULL)
, ba_di_wr        (NULL)
, bb_di           (0)
, bb_cack         (0)
, bb_state        (FROM_EMPTY)
, mDFE0051_DFIFO_CACK (NULL)
{
    /// constants
    DFE0051_CHN_NUMB = new uint8_t[DFE0051_MACRO_NUMB];
    sc_assert(DFE0051_CHN_NUMB != NULL);
    DFE0051_CHN_NUMB[0] = 16;
    DFE0051_CHN_NUMB[1] =  4;

    CclkCtrl::setName(0, pclk   .basename());
    CrstCtrl::setName(0, presetn.basename());
    CrstCtrl::setName(1, "PRESETnCommand");

    //-------------------------------------
    /// -# Instantiate input/output signals
    ///     dfe0051_do, dfe0051_do_upd and dfe0051_dfifo_cack
    dfe0051_do_ctrig   = new sc_in<bool>**    [DFE0051_MACRO_NUMB];
    dfe0051_do         = new sc_in<uint32_t>**[DFE0051_MACRO_NUMB];
    dfe0051_do_upd     = new sc_in<bool>**    [DFE0051_MACRO_NUMB];
    dfe0051_dfifo_cack = new sc_out<bool>**   [DFE0051_MACRO_NUMB];
    sc_assert(dfe0051_do_ctrig   != NULL);
    sc_assert(dfe0051_do         != NULL);
    sc_assert(dfe0051_do_upd     != NULL);
    sc_assert(dfe0051_dfifo_cack != NULL);
    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi) {
        dfe0051_do_ctrig  [dmi] = new sc_in<bool>*    [DFE0051_CHN_NUMB[dmi]];
        dfe0051_do        [dmi] = new sc_in<uint32_t>*[DFE0051_CHN_NUMB[dmi]];
        dfe0051_do_upd    [dmi] = new sc_in<bool>*    [DFE0051_CHN_NUMB[dmi]];
        dfe0051_dfifo_cack[dmi] = new sc_out<bool>*   [DFE0051_CHN_NUMB[dmi]];
        sc_assert(dfe0051_do_ctrig  [dmi] != NULL);
        sc_assert(dfe0051_do        [dmi] != NULL);
        sc_assert(dfe0051_do_upd    [dmi] != NULL);
        sc_assert(dfe0051_dfifo_cack[dmi] != NULL);
        for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci) {
            ostringstream ossName;

            ossName.clear();
            ossName.str("");
            ossName << "dfe0051" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_ctrig";
            dfe0051_do_ctrig[dmi][dci] = new sc_in<bool>(ossName.str().c_str());
            sc_assert(dfe0051_do_ctrig[dmi][dci] != NULL);


            ossName.clear();
            ossName.str("");
            ossName << "dfe0051" << (uint32_t)dmi << "_do" << (uint32_t)dci;
            dfe0051_do[dmi][dci] = new sc_in<uint32_t>(ossName.str().c_str());
            sc_assert(dfe0051_do[dmi][dci] != NULL);
            ossName.clear();
            ossName.str("");
            ossName << "dfe0051" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_upd";
            dfe0051_do_upd[dmi][dci] = new sc_in<bool>(ossName.str().c_str());
            sc_assert(dfe0051_do_upd[dmi][dci] != NULL);
            ossName.clear();
            ossName.str("");
            ossName << "dfe0051" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_cack";
            dfe0051_dfifo_cack[dmi][dci] = new sc_out<bool>(ossName.str().c_str());
            sc_assert(dfe0051_dfifo_cack[dmi][dci] != NULL);
        }
    }
    mDFE0051_DFIFO_CACK = new bool**   [DFE0051_MACRO_NUMB];
    sc_assert(mDFE0051_DFIFO_CACK != NULL);
    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi) {
        mDFE0051_DFIFO_CACK[dmi] = new bool*   [DFE0051_CHN_NUMB[dmi]];
        sc_assert(mDFE0051_DFIFO_CACK[dmi] != NULL);
        for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci) {
            mDFE0051_DFIFO_CACK[dmi][dci] = new bool;
            sc_assert(mDFE0051_DFIFO_CACK[dmi][dci] != NULL);
            *mDFE0051_DFIFO_CACK[dmi][dci] = false;
        }
    }

    //-------------------------------------
    /// -# instantiate the function model
    pFuncModel = new DFE0051_FIFO_Func ((string)iname, this);
    sc_assert(pFuncModel != NULL);

    //-------------------------------------
    /// -# Instantiate internal signals, sub-modules, variables
    ///    channels of the buffer A circuit
    pFFAs = new queue<uint32_t>[DFBA_CHN_NUM];
    sc_assert(pFFAs != NULL);
    ///    dfe0051_do_asel
    dfe0051_do_asel = new uint32_t[DFBA_CHN_NUM];
    sc_assert(dfe0051_do_asel != NULL);
    ///    dfe0051_do_upd_asel
    dfe0051_do_upd_asel = new bool[DFBA_CHN_NUM];
    sc_assert(dfe0051_do_upd_asel != NULL);
    ///    ba_di
    ba_di = new uint32_t[DFBA_CHN_NUM];
    sc_assert(ba_di != NULL);
    ///    ba_wr
    ba_wr = new bool[DFBA_CHN_NUM];
    sc_assert(ba_wr != NULL);
    ///    ba_cack
    ba_cack = new bool[DFBA_CHN_NUM];
    sc_assert(ba_cack != NULL);
    ///    ba_di_wr
    ba_di_wr = new uint32_t[DFBA_CHN_NUM];
    sc_assert(ba_di_wr != NULL);

    //-------------------------------------
    /// -# Instantiate events and register processes
    ba_ff_wr_scev = new sc_event[DFBA_CHN_NUM];
    sc_assert(ba_ff_wr_scev != NULL);

    dfe0051_dfifo_cack_scev = new sc_event*[DFE0051_MACRO_NUMB];
    sc_assert(dfe0051_dfifo_cack_scev != NULL);
    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi) {
        dfe0051_dfifo_cack_scev[dmi] = new sc_event[DFE0051_CHN_NUMB[dmi]];
        sc_assert(dfe0051_dfifo_cack_scev[dmi] != NULL);
    }
    //-------------------------------------
    /// -# register processes
    SC_THREAD(ba_wr_scth);
        //dont_initialize();

    for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci) {
        ostringstream ossName;
        ossName.clear(); ossName.str("");
        do {
            ossName << "ba_ff" << (uint32_t)fci << "_wr_scmt";
            sc_core::sc_spawn_options opt;
            //opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::ba_ff_wr_scth, this, fci)
                             ,ossName.str().c_str()
                             ,&opt);
        } while (0);
    }


    SC_METHOD(dfe0051_tim_ctrig_scmt);
        dont_initialize();
        sensitive << dfe0051_tim_ctrig;

    SC_THREAD(bb_wr_scth);
        //dont_initialize();

    SC_THREAD(dfe0051_ba_ce_irq_scth);
        //dont_initialize();

    SC_THREAD(dfe0051_bb_ce_irq_scth);
        //dont_initialize();

    SC_THREAD(dfe0051_berr_irq_scth);
        //dont_initialize();

    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci) {
            ostringstream ossName;
            ossName.clear();
            ossName.str("");
            ossName << "dfe0051" << (uint32_t)dmi << "_dfifo" << (uint32_t)dci << "_cack_scev";
            sc_core::sc_spawn_options opt;
            //opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::dfe0051_dfifo_cack_scth, this, dmi, dci)
                             ,ossName.str().c_str()
                             ,&opt);
        }

    SC_METHOD(reset_command_begin_scmt);
        dont_initialize();
        sensitive << reset_command_begin_scev;
    SC_METHOD(reset_command_end_scmt);
        dont_initialize();
        sensitive << reset_command_end_scev;

    SC_METHOD(WriteDFE0051_DFIFO_CACKMethod);
    dont_initialize();
    sensitive << mWriteDFE0051_DFIFO_CACKEvent;

    SC_METHOD(WriteDFE0051_BA_CE_IRQMethod);
    dont_initialize();
    sensitive << mWriteDFE0051_BA_CE_IRQEvent;

    SC_METHOD(WriteDFE0051_BB_CE_IRQMethod);
    dont_initialize();
    sensitive << mWriteDFE0051_BB_CE_IRQEvent;

    SC_METHOD(WriteDFE0051_BERR_IRQMethod);
    dont_initialize();
    sensitive << mWriteDFE0051_BERR_IRQEvent;
    //-------------------------------------
    /// -# initialize internal variables, signals, states
    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci)
            dfe0051_dfifo_cack[dmi][dci]->initialize(false);
    dfe0051_ba_ce_irq.initialize(false);
    dfe0051_bb_ce_irq.initialize(false);
    dfe0051_berr_irq.initialize(false);

    Initialize();
}
//------------------------------------------------------------------------------
/**@brief public destructor
 * Detailed steps as follows:
 */
DFE0051_FIFO::~DFE0051_FIFO() {
    if (mDFE0051_DFIFO_CACK != NULL) delete [] mDFE0051_DFIFO_CACK;
    // input/output signals
    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci) {
            if (dfe0051_do_ctrig  [dmi][dci] != NULL) delete dfe0051_do_ctrig  [dmi][dci];
            if (dfe0051_do        [dmi][dci] != NULL) delete dfe0051_do        [dmi][dci];
            if (dfe0051_do_upd    [dmi][dci] != NULL) delete dfe0051_do_upd    [dmi][dci];
            if (dfe0051_dfifo_cack[dmi][dci] != NULL) delete dfe0051_dfifo_cack[dmi][dci];
        }
    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi) {
        if (dfe0051_do_ctrig  [dmi] != NULL) delete [] dfe0051_do_ctrig  [dmi];
        if (dfe0051_do        [dmi] != NULL) delete [] dfe0051_do        [dmi];
        if (dfe0051_do_upd    [dmi] != NULL) delete [] dfe0051_do_upd    [dmi];
        if (dfe0051_dfifo_cack[dmi] != NULL) delete [] dfe0051_dfifo_cack[dmi];
    }
    if (dfe0051_do_ctrig   != NULL) delete [] dfe0051_do_ctrig;
    if (dfe0051_do         != NULL) delete [] dfe0051_do;
    if (dfe0051_do_upd     != NULL) delete [] dfe0051_do_upd;
    if (dfe0051_dfifo_cack != NULL) delete [] dfe0051_dfifo_cack;

    // the function model
    if (pFuncModel != NULL) delete pFuncModel;

    // internal signals, sub-modules, variables
    if (pFFAs           != NULL) delete [] pFFAs;
    if (dfe0051_do_asel     != NULL) delete [] dfe0051_do_asel;
    if (dfe0051_do_upd_asel != NULL) delete [] dfe0051_do_upd_asel;
    if (ba_di           != NULL) delete [] ba_di;
    if (ba_wr           != NULL) delete [] ba_wr;
    if (ba_cack         != NULL) delete [] ba_cack;
    if (ba_di_wr        != NULL) delete [] ba_di_wr;

    // events
    if (ba_ff_wr_scev != NULL) delete [] ba_ff_wr_scev;
    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi)
        if (dfe0051_dfifo_cack_scev[dmi] != NULL) delete [] dfe0051_dfifo_cack_scev[dmi];
    if (dfe0051_dfifo_cack_scev != NULL) delete [] dfe0051_dfifo_cack_scev;

    // constants
    if (DFE0051_CHN_NUMB != NULL) delete[] DFE0051_CHN_NUMB;

}

void DFE0051_FIFO::end_of_elaboration() {
    do {
        ostringstream ossName;
        ossName.clear(); ossName.str("");
        ossName << "pclk_scmt";
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
            opt.set_sensitivity(&pclk);
            opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::pclk_scmt, this)
                         ,ossName.str().c_str()
                         ,&opt);
    } while (0);

    do {
        ostringstream ossName;
        ossName.clear(); ossName.str("");
        ossName << "presetn_scmt";
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
            opt.set_sensitivity(&presetn);
            opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::presetn_scmt, this)
                         ,ossName.str().c_str()
                         ,&opt);
    } while (0);

    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci) {
            ostringstream ossName;

            do {
                ossName.clear();
                ossName.str("");
                ossName << "dfe0051" << (uint32_t)dmi << "_do_upd" << (uint32_t)dci << "_scmt";
                    sc_core::sc_spawn_options opt;
                    opt.spawn_method();
                        opt.set_sensitivity(&dfe0051_do    [dmi][dci]->value_changed_event());
                        opt.set_sensitivity(&dfe0051_do_upd[dmi][dci]->value_changed_event());
                        opt.dont_initialize();
                    sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::dfe0051_do_upd_scmt, this, dmi, dci)
                                     ,ossName.str().c_str()
                                     ,&opt);
            } while (0);

            do {
                ossName.clear();
                ossName.str("");
                ossName << "dfe0051" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_scmt";
                    sc_core::sc_spawn_options opt;
                    opt.spawn_method();
                        opt.set_sensitivity(&dfe0051_do[dmi][dci]->value_changed_event());
                        opt.dont_initialize();
                    sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::dfe0051_do_scmt, this, dmi, dci)
                                     ,ossName.str().c_str()
                                     ,&opt);
            } while (0);

            do {
                ossName.clear();
                ossName.str("");
                ossName << "dfe0051" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_ctrig_scmt";
                    sc_core::sc_spawn_options opt;
                    opt.spawn_method();
                    opt.set_sensitivity(&dfe0051_do_ctrig[dmi][dci]->posedge_event());
                        opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::dfe0051_do_ctrig_scmt, this, dmi, dci)
                                     ,ossName.str().c_str()
                                     ,&opt);
            } while (0);

            #ifdef DFE0051_FIFO_TEST
            do {
                ossName.clear(); ossName.str("");
                ossName << "log_" << dfe0051_do[dmi][dci]->basename();
                sc_core::sc_spawn_options opt;
                opt.spawn_method();
                    opt.set_sensitivity(dfe0051_do[dmi][dci]);
                    opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::log_dfe0051_do_scmt, this, dmi, dci)
                                 ,ossName.str().c_str()
                                 ,&opt);
            } while (0);
            do {
                ossName.clear(); ossName.str("");
                ossName << "log_" << dfe0051_do_upd[dmi][dci]->basename();
                sc_core::sc_spawn_options opt;
                opt.spawn_method();
                    opt.set_sensitivity(dfe0051_do_upd[dmi][dci]);
                    opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::log_dfe0051_do_upd_scmt, this, dmi, dci)
                                 ,ossName.str().c_str()
                                 ,&opt);
            } while (0);
            do {
                ossName.clear(); ossName.str("");
                ossName << "log_" << dfe0051_do_ctrig[dmi][dci]->basename();
                sc_core::sc_spawn_options opt;
                opt.spawn_method();
                    opt.set_sensitivity(dfe0051_do_ctrig[dmi][dci]);
                    opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::log_dfe0051_do_ctrig_scmt, this, dmi, dci)
                                 ,ossName.str().c_str()
                                 ,&opt);
            } while (0);
            do {
                ossName.clear(); ossName.str("");
                ossName << "log_" << dfe0051_dfifo_cack[dmi][dci]->basename();
                sc_core::sc_spawn_options opt;
                opt.spawn_method();
                    opt.set_sensitivity(dfe0051_dfifo_cack[dmi][dci]);
                    opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE0051_FIFO::log_dfe0051_dfifo_cack_scmt, this, dmi, dci)
                                 ,ossName.str().c_str()
                                 ,&opt);
            } while (0);
            #endif // DFE0051_FIFO_TEST
        }
}

DFE0051_FIFO_Func* DFE0051_FIFO::getBusSlaveFunc() {
    return pFuncModel;
}
//------------------------------------------------------------------------------
/**@brief method process monitors the clock input
 */
void DFE0051_FIFO::pclk_scmt() {
    #ifdef DFE0051_FIFO_TEST
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE0051_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE0051_FIFO]:"      << pclk.basename() << " clk "
        << " is changed to [" << pclk.read() << "]"
        << endl;
    re_printf("info", oss.str().c_str());
    #endif // DFE0051_FIFO_TEST
    setClkFreq(pclk.basename(), pclk.read());
}
//------------------------------------------------------------------------------
/**@brief method process monitors the reset input
 */
void DFE0051_FIFO::presetn_scmt() {
    #ifdef DFE0051_FIFO_TEST
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE0051_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE0051_FIFO]:"      << presetn.basename() << " reset "
        << " is changed to [" << uint32_t(presetn.read()) << "]"
        << endl;
    re_printf("info", oss.str().c_str());
    #endif // DFE0051_FIFO_TEST
    EnableRstSrc(presetn.basename(), (presetn.read() == false));
}
//  //------------------------------------------------------------------------------
//  /**@brief handle reset
//   */
//  void DFE0051_FIFO::EnableReset(const bool& isAct) {
//      EnableRstSrc(1, isAct);
//  }
//------------------------------------------------------------------------------
/**@brief handle reset
 */
void DFE0051_FIFO::HandleReset(const bool& isAct) {
    pFuncModel->EnableReset(isAct);
    if (isAct) {
        CancelEvent();
        Initialize();
        for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi) {
            for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci) {
                *mDFE0051_DFIFO_CACK[dmi][dci] = false;
            }
       }

        mDFE0051_BA_CE_IRQ = false;
        mDFE0051_BB_CE_IRQ = false;
        mDFE0051_BERR_IRQ = false;
        mWriteDFE0051_DFIFO_CACKEvent.notify();
        mWriteDFE0051_BA_CE_IRQEvent.notify();
        mWriteDFE0051_BB_CE_IRQEvent.notify();
        mWriteDFE0051_BERR_IRQEvent.notify();

    }
}
//------------------------------------------------------------------------------
/**@brief initialize outputs and internal variables, internal states,
 *        internal signals
 * @details
 * Detailed steps as follows:
 */
void DFE0051_FIFO::Initialize() {
    pFuncModel->Initialize();

    /// -# initialize buffer A circuit
    ///     -# initialize data buffers
    for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci) {
        init_FFA (fci);
    }
    /// -# initialize buffer B circuit
    init_FFB ();

    /// -# initialize buffer A circuit
    ///     -# initialize the internal variables
    for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci) {
        dfe0051_do_asel    [fci] = 0;
        dfe0051_do_upd_asel[fci] = false;
        ba_di          [fci] = 0;
        ba_wr          [fci] = false;
        ba_cack        [fci] = false;
        ba_di_wr       [fci] = 0;
    }
    /// -# initialize buffer B circuit
    ///     -# initialize the internal variables
    bb_di    = 0;
    bb_cack  = false;
    bb_state = FROM_EMPTY;
}

/// Write dfe0051_dfifo_cack
void DFE0051_FIFO::WriteDFE0051_DFIFO_CACKMethod()
{
    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci)
            dfe0051_dfifo_cack[dmi][dci]->write(*mDFE0051_DFIFO_CACK[dmi][dci]);
}

/// Write dfe0051_ba_ce_irq
void DFE0051_FIFO::WriteDFE0051_BA_CE_IRQMethod()
{
    dfe0051_ba_ce_irq.write(mDFE0051_BA_CE_IRQ);
}

/// Write dfe0051_bb_ce_irq
void DFE0051_FIFO::WriteDFE0051_BB_CE_IRQMethod()
{
    dfe0051_bb_ce_irq.write(mDFE0051_BB_CE_IRQ);
}

/// Write dfe0051_berr_irq
void DFE0051_FIFO::WriteDFE0051_BERR_IRQMethod()
{
    dfe0051_berr_irq.write(mDFE0051_BERR_IRQ);
}

//------------------------------------------------------------------------------
/**@brief initialize contents and status indicators of a FIFO channel of buffer A circuit
 * @param[in] fci FIFO channel A index
 * @details
 * Detailed steps as follows:
 */
void DFE0051_FIFO::init_FFA  (const uint8_t& fci) {
    /// -# clear contents of the FIFO core, this also updates empty flag
    while (!pFFAs[fci].empty())
        pFFAs[fci].pop();

    /// -# clear overflow status flag
    uint32_t currOVFA = (*pFuncModel->DFBFACST)["OVFA"];
    uint32_t mask     = (1 << fci);
    (*pFuncModel->DFBFACST)["OVFA"] = currOVFA & ~mask;

    /// -# flag AUN reside in data out when AUNE is enable is initialized to 1
    (*pFuncModel->DFBFADOCH[fci])["ADO"] = (1 == (*pFuncModel->DFBFACCTL)["AUNE"])
                                                ? 1
                                                : 0;
}
//------------------------------------------------------------------------------
/**@brief initialize contents and status indicators of a FIFO channel of buffer B circuit
 * @details
 * Detailed steps as follows:
 */
void DFE0051_FIFO::init_FFB () {
    /// -# clear contents of the FIFO core, this also updates empty flag
    while (!FFB.empty())
        FFB.pop();

    /// -# clear overflow status flag
    (*pFuncModel->DFBFBST)["OVFB"] = 0;
}
//------------------------------------------------------------------------------
/**@brief cancel all scheduled events when reset is activated or when clock
 *        becomes gated
 */
void DFE0051_FIFO::CancelEvent() {
    /// -# cancel events in all submodules
    pFuncModel->CancelEvent();

    /// -# cancel all events of this module
    ba_wr_calc_scev.cancel();
    for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci)
        ba_ff_wr_scev[fci].cancel();

    bb_di_ctrig_scev.cancel();

    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci)
            dfe0051_dfifo_cack_scev[dmi][dci].cancel();

    dfe0051_ba_ce_irq_scev.cancel();
    dfe0051_bb_ce_irq_scev.cancel();
    dfe0051_berr_irq_scev .cancel();

    for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci)
        ba_ff_wr_scev[fci].cancel();


    mWriteDFE0051_DFIFO_CACKEvent.cancel();
    mWriteDFE0051_BA_CE_IRQEvent.cancel();
    mWriteDFE0051_BB_CE_IRQEvent.cancel();
    mWriteDFE0051_BERR_IRQEvent.cancel();
}
//------------------------------------------------------------------------------
/**@brief handle reset
 */
void DFE0051_FIFO::HandleZeroClk (const uint8_t& clkIdx, const bool& isZeroClk) {
    if (isZeroClk) {
        pFuncModel->CancelEvent();
        CancelEvent();
    }
}
//------------------------------------------------------------------------------
/**@brief method that handle dfe0051_do inputs
 */
void DFE0051_FIFO::dfe0051_do_scmt (const uint8_t& dmi, const uint8_t& dci) {
    uint8_t dmi_bb  = (*(pFuncModel->DFBFBCTL))["BDSL"];
    uint8_t dci_bb  = (*(pFuncModel->DFBFBCTL))["BFBCH"];
    if ((dmi == dmi_bb) && (dci == dci_bb))
        calc_bb_di();
}
//------------------------------------------------------------------------------
/**@brief method that handle dfe0051_do inputs
 */
void DFE0051_FIFO::dfe0051_do_upd_scmt (const uint8_t& dmi, const uint8_t& dci) {
    for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci)
        if (    (dmi == (*(pFuncModel->DFBFACTLCH[fci]))["ADSL"])
             && (dci == (*(pFuncModel->DFBFACTLCH[fci]))["BFACH"])
           )
        {
            calc_ba_di (fci);
        }
}
//------------------------------------------------------------------------------
/**@brief group of method processes handle *dfe0051_do_ctrig* input
 * @details basing on setting of buffer B circuit, this process notify
 * @param dmi DFE0051 macro index
 * @param dci DFE0051 channel index
 */
void DFE0051_FIFO::dfe0051_do_ctrig_scmt (const uint8_t& dmi, const uint8_t& dci) {
    uint8_t dmi_bb  = (*(pFuncModel->DFBFBCTL))["BDSL"];
    uint8_t dci_bb  = (*(pFuncModel->DFBFBCTL))["BFBCH"];
    if ((dmi == dmi_bb) && (dci == dci_bb))
        if (!isRstActive() && !isZeroClk(0))
            bb_di_ctrig_scev.notify(toNextSyncPoint(0));
}
//------------------------------------------------------------------------------
/**@brief method process handles *dfe0051_tim_ctrig* input
 * @details this monitors *dfe0051_tim_ctrig* input, issue event *ba_wr_calc_scev*
 *          to write-process
 */
void DFE0051_FIFO::dfe0051_tim_ctrig_scmt() {
    if (1 == dfe0051_tim_ctrig.read())
        if (!isRstActive() && !isZeroClk(pclk.basename()))
            ba_wr_calc_scev.notify(toNextSyncPoint(pclk.basename()));
}
//------------------------------------------------------------------------------
/**@brief group of thread processes handle *dfe0051_dfifo_cack* outputs
 * @details these processes issue a pulse lasting 1 clock cycle to the
 *          a certain DFE0051 channel specified by index of DFE0051 macro and index of
 *          the channel
 * @param dmi index of DFE0051 macro
 * @param dci index of DFE0051 channel
 */
void DFE0051_FIFO::dfe0051_dfifo_cack_scth (const uint8_t& dmi, const uint8_t& dci) {
    while (1) {
        wait (dfe0051_dfifo_cack_scev[dmi][dci]);
        *mDFE0051_DFIFO_CACK[dmi][dci] = true;
        mWriteDFE0051_DFIFO_CACKEvent.notify();
        wait(getClkPeriod(pclk.basename()));
        *mDFE0051_DFIFO_CACK[dmi][dci] = false;
        mWriteDFE0051_DFIFO_CACKEvent.notify();
    }
}
//------------------------------------------------------------------------------
/**@brief thread process handle *dfe0051_ba_ce_irq* output
 * @details these processes issue a pulse lasting 1 clock cycle
 */
void DFE0051_FIFO::dfe0051_ba_ce_irq_scth () {
    while (1) {
        wait (dfe0051_ba_ce_irq_scev);
        mDFE0051_BA_CE_IRQ = true;
        mWriteDFE0051_BA_CE_IRQEvent.notify();
        wait(getClkPeriod(pclk.basename()));
        mDFE0051_BA_CE_IRQ = false;
        mWriteDFE0051_BA_CE_IRQEvent.notify();
    }
}
//------------------------------------------------------------------------------
/**@brief thread process handle *dfe0051_bb_ce_irq* output
 * @details these processes issue a pulse lasting 1 clock cycle
 */
void DFE0051_FIFO::dfe0051_bb_ce_irq_scth () {
    while (1) {
        wait (dfe0051_bb_ce_irq_scev);
        mDFE0051_BB_CE_IRQ = true;
        mWriteDFE0051_BB_CE_IRQEvent.notify();
        wait(getClkPeriod(pclk.basename()));
        mDFE0051_BB_CE_IRQ = false;
        mWriteDFE0051_BB_CE_IRQEvent.notify();
    }
}
//------------------------------------------------------------------------------
/**@brief thread process handle *dfe0051_berr_irq* output
 * @details these processes issue a pulse lasting 1 clock cycle
 */
void DFE0051_FIFO::dfe0051_berr_irq_scth () {
    while (1) {
        wait (dfe0051_berr_irq_scev);
        mDFE0051_BERR_IRQ = true;
        mWriteDFE0051_BERR_IRQEvent.notify();
        wait(getClkPeriod(pclk.basename()));
        mDFE0051_BERR_IRQ = false;
        mWriteDFE0051_BERR_IRQEvent.notify();
    }
}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *ba_di* signal
 * @param[in] fci FIFO channel A index
 * @return value of *ba_di* signal
 */
uint32_t DFE0051_FIFO::calc_ba_di(const uint8_t& fci) {
    uint8_t dmi  = (*(pFuncModel->DFBFACTLCH[fci]))["ADSL"];
    uint8_t dci  = (*(pFuncModel->DFBFACTLCH[fci]))["BFACH"];
    bool    AUNE = (*(pFuncModel->DFBFACCTL))["AUNE"];

    if ((dmi < DFE0051_MACRO_NUMB) &&  (dci < DFE0051_CHN_NUMB[dmi])) {
        dfe0051_do_asel    [fci] = dfe0051_do    [dmi][dci]->read();
        dfe0051_do_upd_asel[fci] = dfe0051_do_upd[dmi][dci]->read();
        ba_di          [fci] =  (  dfe0051_do_asel[fci]
                                & (uint32_t(!AUNE) | 0xFFFFFFFE)
                                )
                              | (  dfe0051_do_upd_asel[fci]
                                &  AUNE
                                );
        return (ba_di[fci]);
    } else {
        return (0);
    }
}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *ba_en* signal
 * @param[in] fci FIFO channel A index
 * @return true if both of AEN and CHEN of channel *fci* bit-fields are enabled.
 */
bool DFE0051_FIFO::calc_ba_en(const uint8_t& fci) {
    uint8_t AEN   = (*(pFuncModel->DFBFACCTL))["AEN"];
    uint8_t CHEN  = (*(pFuncModel->DFBFACTLCH[fci]))["CHEN"];
    return (AEN && CHEN);
}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *ba_full* signal
 * @param[in] fci FIFO channel A index
 * @return true if the FIFO channel *fci* of buffer A circuit is full.
 */
bool DFE0051_FIFO::calc_ba_full(const uint8_t& fci) {
    return (DFBA_DEPTH == pFFAs[fci].size());
}
//------------------------------------------------------------------------------
void DFE0051_FIFO::ba_wr_scth() {
    while (1) {
        wait(ba_wr_calc_scev);

        // sampling value of *dfe0051_tim_ctrig* input signal at the time of active clock edge
        if (1 == dfe0051_tim_ctrig.read()) {
            /// -# check write condition and issue write command
            // if *dfe0051_tim_ctrig* is active, for each FIFO channel *fci* of buffer A circuit
            for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci)
                // check if that FIFO channel *fci* is enable
                if (calc_ba_en(fci))
                    // check status *full* of the FIFO channel *fci*
                    if (calc_ba_full(fci)) {
                        // do not allow to write *ba_di* into the FIFO
                        ba_wr[fci]   = false;
                        calc_ba_cack(fci);

                        // assert corresponding overflow flag if the FIFO channel becomes overflow
                        // and issue an error (overflow) interrupt request
                        uint32_t currOVFA = (*pFuncModel->DFBFACST)["OVFA"];
                        uint32_t mask     = (1 << fci);
                        (*pFuncModel->DFBFACST)["OVFA"] = currOVFA | mask;
                        if (1 == (*pFuncModel->DFBFACCTL)["AIEE"])
                            dfe0051_berr_irq_scev.notify();
                    } else {
                        // allow to write data *ba_di* into the FIFO
                        ba_wr[fci]   = true;
                        // save data into the FIFO
                        ba_di_wr[fci] = ba_di[fci];
                        ba_ff_wr_scev[fci].notify(toNextSyncPoint(pclk.basename()));

                        // acknowledge the DFE0051 channel
                        calc_ba_cack(fci);
                        notify_dfe0051_dfifo_cack();
                    }

            /// -# check capture-end interrupt condition and issue interrupt
            do {
                bool all_empty = true;
                bool has_act_channel = false;
                for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci)
                    if (calc_ba_en(fci)) {
                        has_act_channel = true;
                        if (!pFFAs[fci].empty()) {
                            all_empty = false;
                            break;
                        }
                    }
                if (all_empty && has_act_channel && calc_ba_ceirq_en())
                    dfe0051_ba_ce_irq_scev.notify(toNextSyncPoint(pclk.basename(), sc_time_stamp()));
            } while (0);
        }
    } // end-while
}
//------------------------------------------------------------------------------
void DFE0051_FIFO::ba_ff_wr_scth(const uint8_t& fci) {
    while (1) {
        wait(ba_ff_wr_scev[fci]);
        pFFAs[fci].push(ba_di_wr[fci]);
    }
}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *bb_en* signal
  * @return true if BEN bit-field is enabled.
 */
bool DFE0051_FIFO::calc_bb_en() {
    uint8_t BEN = (*(pFuncModel->DFBFBCTL))["BEN"];
    return (bool)BEN;
}
bool DFE0051_FIFO::calc_bb_ceirq_en() {
    uint8_t OVFB = (*pFuncModel->DFBFBST) ["OVFB"];
    uint8_t BIEO = (*pFuncModel->DFBFBCTL)["BIEO"];
    return ((OVFB == 0) && (bool)BIEO);
}
bool DFE0051_FIFO::calc_ba_ceirq_en() {
    uint32_t OVFA = (*pFuncModel->DFBFACST) ["OVFA"];
    uint8_t  AIEO = (*pFuncModel->DFBFACCTL)["AIEO"];
    return ((OVFA == 0) && (bool)AIEO);
}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *bb_full* signal
 * @return true if the FIFO channel of buffer B circuit is full.
 */
bool DFE0051_FIFO::calc_bb_full() {
    return (DFBB_DEPTH == FFB.size());
}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *ba_cack* signal
 * @param[in] fci FIFO channel A index
 * @return true if the FIFO channel *fci* of buffer A circuit issues an ACK to the selected DFE0051 channel
 */
bool DFE0051_FIFO::calc_ba_cack(const uint8_t& fci) {
    ba_cack[fci] = ba_wr[fci] && (!dfe0051_do_upd_asel[fci]);
    return (ba_cack[fci]);
}

//------------------------------------------------------------------------------
/**@brief implement a combinational multiplexor selecting data from DFE0051 macros
 */
uint32_t DFE0051_FIFO::calc_bb_di() {
    uint8_t dmi  = (*(pFuncModel->DFBFBCTL))["BDSL"];
    uint8_t dci  = (*(pFuncModel->DFBFBCTL))["BFBCH"];
    if ((dmi < DFE0051_MACRO_NUMB) &&  (dci < DFE0051_CHN_NUMB[dmi])) {
        bb_di        = dfe0051_do [dmi][dci]->read();
        return (bb_di);
    } else
        return (0);
}
//------------------------------------------------------------------------------
void DFE0051_FIFO::bb_wr_scth() {
    while (1) {
        wait(bb_di_ctrig_scev);

        // sampling value of *dfe0051_do_ctrig* input signal at the time of active clock edge
        uint8_t dmi = (*(pFuncModel->DFBFBCTL))["BDSL"];
        uint8_t dci = (*(pFuncModel->DFBFBCTL))["BFBCH"];
        // if (1 == dfe0051_do_ctrig[dmi][dci]->read()) /*support Chan-san need, capture trigger only last 1 CLK_UHSB cycle, not CLK_HSB cycle, remove this condition*/
            // if *dfe0051_do_ctrig* is active
            if (calc_bb_en())
                if (calc_bb_full()) {
                    bb_cack = false;

                    // assert corresponding overflow flag if the FIFO channel becomes overflow
                    // and issue an error (overflow) interrupt request
                    (*pFuncModel->DFBFBST)["OVFB"] = 1;
                    if (1 == (*pFuncModel->DFBFBCTL)["BIEE"])
                        dfe0051_berr_irq_scev.notify();
                } else {
                    // allow to write data *ba_di* into the FIFO
                    FFB.push(bb_di);

                    // check and issue capture end interrupt
                    if (bb_state == FROM_EMPTY && FFB.size() == DFBB_DEPTH/2) {
                        bb_state = FROM_HALF_FULL;
                        if (calc_bb_ceirq_en())
                            dfe0051_bb_ce_irq_scev.notify();
                    }

                    // acknowledge the DFE0051 channel
                    bb_cack = true;
                    notify_dfe0051_dfifo_cack();
                }
    } // end-while
}
//------------------------------------------------------------------------------
/**@brief this function implements a de-multiplexor generate ack signals to
 *        DFE0051 channels by decoding settings of channels of buffer A circuit
 *        and buffer B circuit
 */
void DFE0051_FIFO::notify_dfe0051_dfifo_cack() {
    bool cack;
    uint8_t dmi_rgblk;
    uint8_t dci_rgblk;

    for (uint8_t dmi = 0; dmi < DFE0051_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE0051_CHN_NUMB[dmi]; ++dci) {
            cack = 0;

            dmi_rgblk = (*(pFuncModel->DFBFBCTL))["BDSL"];
            dci_rgblk = (*(pFuncModel->DFBFBCTL))["BFBCH"];
            if (dmi_rgblk == dmi && dci_rgblk == dci) {
                cack |= bb_cack;
            }

            for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci) {
                /// working
                dmi_rgblk = (*(pFuncModel->DFBFACTLCH[fci]))["ADSL"];
                dci_rgblk = (*(pFuncModel->DFBFACTLCH[fci]))["BFACH"];
                if (dmi_rgblk == dmi && dci_rgblk == dci) {
                    cack |= ba_cack[fci];
                }
            }

            if (cack) {
                dfe0051_dfifo_cack_scev[dmi][dci].notify();
            }
        }
}


#ifdef    DFE0051_FIFO_TEST
void DFE0051_FIFO::log_dfe0051_do_scmt (const uint8_t& dmi, const uint8_t& dci) {
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE0051_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE0051_FIFO]:"                  << dfe0051_do[dmi][dci]->basename()
        << " is changed to [" << std::hex << "0x" << uint32_t(dfe0051_do[dmi][dci]->read()) << "]"
        << endl;
    re_printf("info", oss.str().c_str());
}
void DFE0051_FIFO::log_dfe0051_do_upd_scmt    (const uint8_t& dmi, const uint8_t& dci) {
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE0051_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE0051_FIFO]:"               << dfe0051_do_upd[dmi][dci]->basename()
        << " is changed to [" << uint32_t(dfe0051_do_upd[dmi][dci]->read()) << "]"
        << endl;
    re_printf("info", oss.str().c_str());
}
void DFE0051_FIFO::log_dfe0051_do_ctrig_scmt  (const uint8_t& dmi, const uint8_t& dci) {
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE0051_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE0051_FIFO]:"               << dfe0051_do_ctrig[dmi][dci]->basename()
        << " is changed to [" << uint32_t(dfe0051_do_ctrig[dmi][dci]->read()) << "]"
        << endl;
    re_printf("info", oss.str().c_str());
}
void DFE0051_FIFO::log_dfe0051_dfifo_cack_scmt  (const uint8_t& dmi, const uint8_t& dci) {
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE0051_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE0051_FIFO]:"               << dfe0051_dfifo_cack[dmi][dci]->basename()
        << " is changed to [" << uint32_t(dfe0051_dfifo_cack[dmi][dci]->read()) << "]"
        << endl;
    re_printf("info", oss.str().c_str());
}
#endif // DFE0051_FIFO_TEST

//------------------------------------------------------------------------------
// PY_IF
//------------------------------------------------------------------------------
void DFE0051_FIFO::AssertReset     (const std::string reset_name
                               ,const double      start_time
                               ,const double      period)
{
    if (   start_time > 0
       &&  period     > 0
       &&  CrstCtrl::hasName(reset_name)
       && !CrstCtrl::isRstActive(reset_name)
    ) {
        reset_command_begin_scev.cancel();
        reset_command_end_scev  .cancel();
        reset_command_begin_scev.notify(start_time,        SC_NS);
        reset_command_end_scev  .notify(start_time+period, SC_NS);
        re_printf("info"
                 ,"The model will be reset (%s) for %f ns after %f ns.\n"
                 ,reset_name.c_str()
                 ,period
                 ,start_time);
    }
}
void DFE0051_FIFO::reset_command_begin_scmt() {
    EnableRstSrc("PRESETnCommand", true);
}
void DFE0051_FIFO::reset_command_end_scmt() {
    EnableRstSrc("PRESETnCommand", false);
}
void DFE0051_FIFO::SetCLKFreq      (const std::string     clock_name
                               ,const sc_dt::uint64   freq
                               ,const std::string     unit)
{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        uint64_t setFreq;
        string   setUnit("Hz");
        ConvertClockFreq (setFreq, setUnit, freq, unit);
        if (!CclkCtrl::setClkFreq(clock_name, setFreq)) {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}
void DFE0051_FIFO::GetCLKFreq      (const std::string     clock_name)
{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (CclkCtrl::hasName(word_vector[0])) {
            re_printf("info","%s frequency is %0.0f %s\n"
                            ,word_vector[0].c_str()
                            ,double(CclkCtrl::getClkFreq(word_vector[0]))
                            ," Hz");
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}
/**@brief Force value to register by the software via Python interface
 * @param reg_name name of the desired register
 * @param reg_value desired value to be written
 */
void DFE0051_FIFO::ForceRegister   (const std::string     reg_name
                               ,const unsigned int    reg_value)
{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            pFuncModel->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}
/**@brief Release forced value after forcing registers by the software
 *        via Python interface
 * @param reg_name name of the desired register
 */
void DFE0051_FIFO::ReleaseRegister (const std::string     reg_name)
{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            pFuncModel->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}
/**@brief Write value to registers by software by the software
 *        via Python interface
 * @param reg_name name of the desired register
 * @param reg_value desired value to be written
 */
void DFE0051_FIFO::WriteRegister   (const std::string     reg_name
                               ,const unsigned int    reg_value)
{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            pFuncModel->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}
/**@brief Read value of register by software by the software
 *        via Python interface
 * @param reg_name name of the desired register
 */
void DFE0051_FIFO::ReadRegister    (const std::string reg_name)
{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            pFuncModel->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}
/**@brief List all registers name by software by the software
 *        via Python interface
 */
void DFE0051_FIFO::ListRegister    (void)
{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    pFuncModel->RegisterHandler(cmd);
}

void DFE0051_FIFO::SetMessageLevel (const std::string msg_lv)
{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    std::string msg_ret = this->handleCommand(cmd);
    if (msg_ret != "") {
        SC_REPORT_INFO(this->basename(), msg_ret.c_str());
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    pFuncModel->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    pFuncModel->RegisterHandler(cmd);
}
void DFE0051_FIFO::DumpRegisterRW  (const std::string is_enable)
{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    pFuncModel->RegisterHandler(cmd);
}
//------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------
