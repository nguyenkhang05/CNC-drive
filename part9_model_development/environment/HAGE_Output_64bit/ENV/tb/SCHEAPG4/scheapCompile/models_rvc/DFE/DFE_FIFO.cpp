// ---------------------------------------------------------------------
// $Id: DFE_FIFO.cpp,v 1.2 2018/09/06 10:17:09 huepham Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "DFE_FIFO.h"
#include "DFE_FIFO_Utils.h"
#include <iomanip>
#include <string>
#include <sstream>
using std::ostringstream;
using std::cout;
using std::endl;
//------------------------------------------------------------------------------
/**@brief public constructor
 * @param[in] iname name of the DFE module
 * @param[in] iDFE_MACRO_NUMB number of the DFE macros, default value is 2
 * @param[in] iDFBA_CHN_NUM number of the FIFO channels of the buffer A circuit, default value is 6
 * @param[in] iDFBA_DEPTH number of stages (a.k.a depth) of a FIFO channel of the buffer A circuit, default value is 8
 * @param[in] iDFBB_DEPTH number of stages (a.k.a depth) of buffer B circuit, default value is 8
 * Detailed steps as follows:
 */
DFE_FIFO::DFE_FIFO(const sc_module_name& iname
                  ,const uint8_t&        iDFE_MACRO_NUMB
                  ,const uint8_t&        iDFBA_CHN_NUM
                  ,const uint8_t&        iDFBA_DEPTH
                  ,const uint8_t&        iDFBB_DEPTH
                  )
: sc_module (iname)
, CclkCtrl(1)
, CrstCtrl(2)
// constants
, DFE_MACRO_NUMB (iDFE_MACRO_NUMB)
, DFE_CHN_NUMB   (NULL)
, DFBA_CHN_NUM   (iDFBA_CHN_NUM)
, DFBA_DEPTH     (iDFBA_DEPTH)
, DFBB_DEPTH     (iDFBB_DEPTH)
// initialize public interfaces: in/out/inout ports, sockets, events notifying the exterior, APIs
// clock, reset and slave bus interface
, pclk      ("CLK_HSB")
, presetn   ("PRESETn")
// ports interfacing with buffer A circuit
, dfe_tim_ctrig ("dfe_tim_ctrig")
// ports interfacing with DFE channels
, dfe_do_ctrig    (NULL)
, dfe_do          (NULL)
, dfe_do_upd      (NULL)
, dfe_dfifo_cack  (NULL)
// ports issues interrupt requests
, dfe_ba_ce_irq ("dfe_ba_ce_irq")
, dfe_bb_ce_irq ("dfe_bb_ce_irq")
, dfe_berr_irq  ("dfe_berr_irq")
// relationship of AgentController with Function Model
, pFuncModel        (NULL)
, pFFAs             (NULL)
// internal signals
, dfe_do_asel     (NULL)
, dfe_do_upd_asel (NULL)
, ba_di           (NULL)
, ba_wr           (NULL)
, ba_cack         (NULL)
, ba_di_wr        (NULL)
, bb_di           (0)
, bb_cack         (0)
, bb_state        (FROM_EMPTY)
{
    /// constants
    DFE_CHN_NUMB = new uint8_t[DFE_MACRO_NUMB];
    sc_assert(DFE_CHN_NUMB != NULL);
    DFE_CHN_NUMB[0] = 12;
    DFE_CHN_NUMB[1] =  4;

    CclkCtrl::setName(0, pclk   .basename());
    CrstCtrl::setName(0, presetn.basename());
    CrstCtrl::setName(1, "PRESETnCommand");

    //-------------------------------------
    /// -# Instantiate input/output signals
    ///     dfe_do, dfe_do_upd and dfe_dfifo_cack
    dfe_do_ctrig   = new sc_in<bool>**    [DFE_MACRO_NUMB];
    dfe_do         = new sc_in<uint32_t>**[DFE_MACRO_NUMB];
    dfe_do_upd     = new sc_in<bool>**    [DFE_MACRO_NUMB];
    dfe_dfifo_cack = new sc_out<bool>**   [DFE_MACRO_NUMB];
    sc_assert(dfe_do_ctrig   != NULL);
    sc_assert(dfe_do         != NULL);
    sc_assert(dfe_do_upd     != NULL);
    sc_assert(dfe_dfifo_cack != NULL);
    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi) {
        dfe_do_ctrig  [dmi] = new sc_in<bool>*    [DFE_CHN_NUMB[dmi]];
        dfe_do        [dmi] = new sc_in<uint32_t>*[DFE_CHN_NUMB[dmi]];
        dfe_do_upd    [dmi] = new sc_in<bool>*    [DFE_CHN_NUMB[dmi]];
        dfe_dfifo_cack[dmi] = new sc_out<bool>*   [DFE_CHN_NUMB[dmi]];
        sc_assert(dfe_do_ctrig  [dmi] != NULL);
        sc_assert(dfe_do        [dmi] != NULL);
        sc_assert(dfe_do_upd    [dmi] != NULL);
        sc_assert(dfe_dfifo_cack[dmi] != NULL);
        for (uint8_t dci = 0; dci < DFE_CHN_NUMB[dmi]; ++dci) {
            ostringstream ossName;

            ossName.clear();
            ossName.str("");
            ossName << "dfe" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_ctrig";
            dfe_do_ctrig[dmi][dci] = new sc_in<bool>(ossName.str().c_str());
            sc_assert(dfe_do_ctrig[dmi][dci] != NULL);


            ossName.clear();
            ossName.str("");
            ossName << "dfe" << (uint32_t)dmi << "_do" << (uint32_t)dci;
            dfe_do[dmi][dci] = new sc_in<uint32_t>(ossName.str().c_str());
            sc_assert(dfe_do[dmi][dci] != NULL);
            ossName.clear();
            ossName.str("");
            ossName << "dfe" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_upd";
            dfe_do_upd[dmi][dci] = new sc_in<bool>(ossName.str().c_str());
            sc_assert(dfe_do_upd[dmi][dci] != NULL);
            ossName.clear();
            ossName.str("");
            ossName << "dfe" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_cack";
            dfe_dfifo_cack[dmi][dci] = new sc_out<bool>(ossName.str().c_str());
            sc_assert(dfe_dfifo_cack[dmi][dci] != NULL);
        }
    }

    //-------------------------------------
    /// -# instantiate the function model
    pFuncModel = new DFE_FIFO_Func ((string)iname, this);
    sc_assert(pFuncModel != NULL);

    //-------------------------------------
    /// -# Instantiate internal signals, sub-modules, variables
    ///    channels of the buffer A circuit
    pFFAs = new queue<uint32_t>[DFBA_CHN_NUM];
    sc_assert(pFFAs != NULL);
    ///    dfe_do_asel
    dfe_do_asel = new uint32_t[DFBA_CHN_NUM];
    sc_assert(dfe_do_asel != NULL);
    ///    dfe_do_upd_asel
    dfe_do_upd_asel = new bool[DFBA_CHN_NUM];
    sc_assert(dfe_do_upd_asel != NULL);
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

    dfe_dfifo_cack_scev = new sc_event*[DFE_MACRO_NUMB];
    sc_assert(dfe_dfifo_cack_scev != NULL);
    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi) {
        dfe_dfifo_cack_scev[dmi] = new sc_event[DFE_CHN_NUMB[dmi]];
        sc_assert(dfe_dfifo_cack_scev[dmi] != NULL);
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
            sc_core::sc_spawn(sc_bind(&DFE_FIFO::ba_ff_wr_scth, this, fci)
                             ,ossName.str().c_str()
                             ,&opt);
        } while (0);
    }


    SC_METHOD(dfe_tim_ctrig_scmt);
        dont_initialize();
        sensitive << dfe_tim_ctrig;

    SC_THREAD(bb_wr_scth);
        //dont_initialize();

    SC_THREAD(dfe_ba_ce_irq_scth);
        //dont_initialize();

    SC_THREAD(dfe_bb_ce_irq_scth);
        //dont_initialize();

    SC_THREAD(dfe_berr_irq_scth);
        //dont_initialize();

    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE_CHN_NUMB[dmi]; ++dci) {
            ostringstream ossName;
            ossName.clear();
            ossName.str("");
            ossName << "dfe" << (uint32_t)dmi << "_dfifo" << (uint32_t)dci << "_cack_scev";
            sc_core::sc_spawn_options opt;
            //opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&DFE_FIFO::dfe_dfifo_cack_scth, this, dmi, dci)
                             ,ossName.str().c_str()
                             ,&opt);
        }

    SC_METHOD(reset_command_begin_scmt);
        dont_initialize();
        sensitive << reset_command_begin_scev;
    SC_METHOD(reset_command_end_scmt);
        dont_initialize();
        sensitive << reset_command_end_scev;
        
    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE_CHN_NUMB[dmi]; ++dci)
            dfe_dfifo_cack[dmi][dci]->initialize(false);
    dfe_ba_ce_irq.initialize(false);
    dfe_bb_ce_irq.initialize(false);
    dfe_berr_irq .initialize(false);
    /// -# initialize internal variables, signals, states
    Initialize();
}
//------------------------------------------------------------------------------
/**@brief public destructor
 * Detailed steps as follows:
 */
DFE_FIFO::~DFE_FIFO() {
    // input/output signals
    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE_CHN_NUMB[dmi]; ++dci) {
            if (dfe_do_ctrig  [dmi][dci] != NULL) delete dfe_do_ctrig  [dmi][dci];
            if (dfe_do        [dmi][dci] != NULL) delete dfe_do        [dmi][dci];
            if (dfe_do_upd    [dmi][dci] != NULL) delete dfe_do_upd    [dmi][dci];
            if (dfe_dfifo_cack[dmi][dci] != NULL) delete dfe_dfifo_cack[dmi][dci];
        }
    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi) {
        if (dfe_do_ctrig  [dmi] != NULL) delete [] dfe_do_ctrig  [dmi];
        if (dfe_do        [dmi] != NULL) delete [] dfe_do        [dmi];
        if (dfe_do_upd    [dmi] != NULL) delete [] dfe_do_upd    [dmi];
        if (dfe_dfifo_cack[dmi] != NULL) delete [] dfe_dfifo_cack[dmi];
    }
    if (dfe_do_ctrig   != NULL) delete [] dfe_do_ctrig;
    if (dfe_do         != NULL) delete [] dfe_do;
    if (dfe_do_upd     != NULL) delete [] dfe_do_upd;
    if (dfe_dfifo_cack != NULL) delete [] dfe_dfifo_cack;

    // the function model
    if (pFuncModel != NULL) delete pFuncModel;

    // internal signals, sub-modules, variables
    if (pFFAs           != NULL) delete [] pFFAs;
    if (dfe_do_asel     != NULL) delete [] dfe_do_asel;
    if (dfe_do_upd_asel != NULL) delete [] dfe_do_upd_asel;
    if (ba_di           != NULL) delete [] ba_di;
    if (ba_wr           != NULL) delete [] ba_wr;
    if (ba_cack         != NULL) delete [] ba_cack;
    if (ba_di_wr        != NULL) delete [] ba_di_wr;

    // events
    if (ba_ff_wr_scev != NULL) delete [] ba_ff_wr_scev;
    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi)
        if (dfe_dfifo_cack_scev[dmi] != NULL) delete [] dfe_dfifo_cack_scev[dmi];
    if (dfe_dfifo_cack_scev != NULL) delete [] dfe_dfifo_cack_scev;

    // constants
    if (DFE_CHN_NUMB != NULL) delete[] DFE_CHN_NUMB;
}

void DFE_FIFO::end_of_elaboration() 
{//{{{
    do {
        ostringstream ossName;
        ossName.clear(); ossName.str("");
        ossName << "pclk_scmt";
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
            opt.set_sensitivity(&pclk);
            opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DFE_FIFO::pclk_scmt, this)
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
        sc_core::sc_spawn(sc_bind(&DFE_FIFO::presetn_scmt, this)
                         ,ossName.str().c_str()
                         ,&opt);
    } while (0);

    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE_CHN_NUMB[dmi]; ++dci) {
            ostringstream ossName;

            do {
                ossName.clear();
                ossName.str("");
                ossName << "dfe" << (uint32_t)dmi << "_do_upd" << (uint32_t)dci << "_scmt";
                    sc_core::sc_spawn_options opt;
                    opt.spawn_method();
                        opt.set_sensitivity(&dfe_do    [dmi][dci]->value_changed_event());
                        opt.set_sensitivity(&dfe_do_upd[dmi][dci]->value_changed_event());
                        opt.dont_initialize();
                    sc_core::sc_spawn(sc_bind(&DFE_FIFO::dfe_do_upd_scmt, this, dmi, dci)
                                     ,ossName.str().c_str()
                                     ,&opt);
            } while (0);

            do {
                ossName.clear();
                ossName.str("");
                ossName << "dfe" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_scmt";
                    sc_core::sc_spawn_options opt;
                    opt.spawn_method();
                        opt.set_sensitivity(&dfe_do[dmi][dci]->value_changed_event());
                        opt.dont_initialize();
                    sc_core::sc_spawn(sc_bind(&DFE_FIFO::dfe_do_scmt, this, dmi, dci)
                                     ,ossName.str().c_str()
                                     ,&opt);
            } while (0);

            do {
                ossName.clear();
                ossName.str("");
                ossName << "dfe" << (uint32_t)dmi << "_do" << (uint32_t)dci << "_ctrig_scmt";
                    sc_core::sc_spawn_options opt;
                    opt.spawn_method();
                    opt.set_sensitivity(&dfe_do_ctrig[dmi][dci]->posedge_event());
                        opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE_FIFO::dfe_do_ctrig_scmt, this, dmi, dci)
                                     ,ossName.str().c_str()
                                     ,&opt);
            } while (0);

            #ifdef DFE_FIFO_TEST
            do {
                ossName.clear(); ossName.str("");
                ossName << "log_" << dfe_do[dmi][dci]->basename();
                sc_core::sc_spawn_options opt;
                opt.spawn_method();
                    opt.set_sensitivity(dfe_do[dmi][dci]);
                    opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE_FIFO::log_dfe_do_scmt, this, dmi, dci)
                                 ,ossName.str().c_str()
                                 ,&opt);
            } while (0);
            do {
                ossName.clear(); ossName.str("");
                ossName << "log_" << dfe_do_upd[dmi][dci]->basename();
                sc_core::sc_spawn_options opt;
                opt.spawn_method();
                    opt.set_sensitivity(dfe_do_upd[dmi][dci]);
                    opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE_FIFO::log_dfe_do_upd_scmt, this, dmi, dci)
                                 ,ossName.str().c_str()
                                 ,&opt);
            } while (0);
            do {
                ossName.clear(); ossName.str("");
                ossName << "log_" << dfe_do_ctrig[dmi][dci]->basename();
                sc_core::sc_spawn_options opt;
                opt.spawn_method();
                    opt.set_sensitivity(dfe_do_ctrig[dmi][dci]);
                    opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE_FIFO::log_dfe_do_ctrig_scmt, this, dmi, dci)
                                 ,ossName.str().c_str()
                                 ,&opt);
            } while (0);
            do {
                ossName.clear(); ossName.str("");
                ossName << "log_" << dfe_dfifo_cack[dmi][dci]->basename();
                sc_core::sc_spawn_options opt;
                opt.spawn_method();
                    opt.set_sensitivity(dfe_dfifo_cack[dmi][dci]);
                    opt.dont_initialize();
                sc_core::sc_spawn(sc_bind(&DFE_FIFO::log_dfe_dfifo_cack_scmt, this, dmi, dci)
                                 ,ossName.str().c_str()
                                 ,&opt);
            } while (0);
            #endif // DFE_FIFO_TEST
        }
}//}}}

DFE_FIFO_Func* DFE_FIFO::getBusSlaveFunc() 
{//{{{
    return pFuncModel;
}//}}}
//------------------------------------------------------------------------------
/**@brief method process monitors the clock input
 */
void DFE_FIFO::pclk_scmt() 
{//{{{
    #ifdef DFE_FIFO_TEST
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE_FIFO]:"      << pclk.basename() << " clk "
        << " is changed to [" << pclk.read() << "]"
        << endl;
    cout << oss.str();
    #endif // DFE_FIFO_TEST
    setClkFreq(pclk.basename(), pclk.read());
}//}}}
//------------------------------------------------------------------------------
/**@brief method process monitors the reset input
 */
void DFE_FIFO::presetn_scmt() 
{//{{{
    #ifdef DFE_FIFO_TEST
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE_FIFO]:"      << presetn.basename() << " reset "
        << " is changed to [" << uint32_t(presetn.read()) << "]"
        << endl;
    re_printf("info", oss.str().c_str());
    #endif // DFE_FIFO_TEST
    EnableRstSrc(presetn.basename(), (presetn.read() == false));
}//}}}
//------------------------------------------------------------------------------
/**@brief handle reset
 */
void DFE_FIFO::HandleReset(const bool& isAct) 
{//{{{
    pFuncModel->EnableReset(isAct);
    if (isAct) {
        CancelEvent();
        Initialize();
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief initialize outputs and internal variables, internal states,
 *        internal signals
 * @details
 * Detailed steps as follows:
 */
void DFE_FIFO::Initialize() 
{//{{{
    /// -# initialize the outputs
    //for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi)
        //for (uint8_t dci = 0; dci < DFE_CHN_NUMB[dmi]; ++dci)
            //dfe_dfifo_cack[dmi][dci]->initialize(false);
    //dfe_ba_ce_irq.initialize(false);
    //dfe_bb_ce_irq.initialize(false);
    //dfe_berr_irq .initialize(false);

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
        dfe_do_asel    [fci] = 0;
        dfe_do_upd_asel[fci] = false;
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
}//}}}
//------------------------------------------------------------------------------
/**@brief initialize contents and status indicators of a FIFO channel of buffer A circuit
 * @param[in] fci FIFO channel A index
 * @details
 * Detailed steps as follows:
 */
void DFE_FIFO::init_FFA  (const uint8_t& fci) 
{//{{{
    /// -# clear contents of the FIFO core, this also updates empty flag
    while (!pFFAs[fci].empty())
        pFFAs[fci].pop();
}//}}}
//------------------------------------------------------------------------------
/**@brief initialize contents and status indicators of a FIFO channel of buffer B circuit
 * @details
 * Detailed steps as follows:
 */
void DFE_FIFO::init_FFB () 
{//{{{
    /// -# clear contents of the FIFO core, this also updates empty flag
    while (!FFB.empty())
        FFB.pop();
}//}}}
//------------------------------------------------------------------------------
/**@brief cancel all scheduled events when reset is activated or when clock
 *        becomes gated
 */
void DFE_FIFO::CancelEvent() 
{//{{{
    /// -# cancel events in all submodules
    pFuncModel->CancelEvent();

    /// -# cancel all events of this module
    ba_wr_calc_scev.cancel();
    for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci)
        ba_ff_wr_scev[fci].cancel();

    bb_di_ctrig_scev.cancel();

    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE_CHN_NUMB[dmi]; ++dci)
            dfe_dfifo_cack_scev[dmi][dci].cancel();

    dfe_ba_ce_irq_scev.cancel();
    dfe_bb_ce_irq_scev.cancel();
    dfe_berr_irq_scev .cancel();

    for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci)
        ba_ff_wr_scev[fci].cancel();
}//}}}
//------------------------------------------------------------------------------
/**@brief handle reset
 */
void DFE_FIFO::HandleZeroClk (const uint8_t& clkIdx, const bool& isZeroClk) 
{//{{{
    if (isZeroClk) {
        pFuncModel->CancelEvent();
        CancelEvent();
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief method that handle dfe_do inputs
 */
void DFE_FIFO::dfe_do_scmt (const uint8_t& dmi, const uint8_t& dci) 
{//{{{
    uint8_t dmi_bb  = (*(pFuncModel->DFBFBCTL))["BDSL"];
    uint8_t dci_bb  = (*(pFuncModel->DFBFBCTL))["BFBCH"];
    if ((dmi == dmi_bb) && (dci == dci_bb))
        calc_bb_di();
}//}}}
//------------------------------------------------------------------------------
/**@brief method that handle dfe_do inputs
 */
void DFE_FIFO::dfe_do_upd_scmt (const uint8_t& dmi, const uint8_t& dci) 
{//{{{
    for (uint8_t fci = 0; fci < DFBA_CHN_NUM; ++fci)
        if (    (dmi == (*(pFuncModel->DFBFACTLCH[fci]))["ADSL"])
             && (dci == (*(pFuncModel->DFBFACTLCH[fci]))["BFACH"])
           )
        {
            calc_ba_di (fci);
        }
}//}}}
//------------------------------------------------------------------------------
/**@brief group of method processes handle *dfe_do_ctrig* input
 * @details basing on setting of buffer B circuit, this process notify
 * @param dmi DFE macro index
 * @param dci DFE channel index
 */
void DFE_FIFO::dfe_do_ctrig_scmt (const uint8_t& dmi, const uint8_t& dci) 
{//{{{
    uint8_t dmi_bb  = (*(pFuncModel->DFBFBCTL))["BDSL"];
    uint8_t dci_bb  = (*(pFuncModel->DFBFBCTL))["BFBCH"];
    if ((dmi == dmi_bb) && (dci == dci_bb))
        if (!isRstActive() && !isZeroClk(0))
            bb_di_ctrig_scev.notify(toNextSyncPoint(0));
}//}}}
//------------------------------------------------------------------------------
/**@brief method process handles *dfe_tim_ctrig* input
 * @details this monitors *dfe_tim_ctrig* input, issue event *ba_wr_calc_scev*
 *          to write-process
 */
void DFE_FIFO::dfe_tim_ctrig_scmt() 
{//{{{
    if (1 == dfe_tim_ctrig.read())
        if (!isRstActive() && !isZeroClk(pclk.basename()))
            ba_wr_calc_scev.notify(toNextSyncPoint(pclk.basename()));
}//}}}
//------------------------------------------------------------------------------
/**@brief group of thread processes handle *dfe_dfifo_cack* outputs
 * @details these processes issue a pulse lasting 1 clock cycle to the
 *          a certain DFE channel specified by index of DFE macro and index of
 *          the channel
 * @param dmi index of DFE macro
 * @param dci index of DFE channel
 */
void DFE_FIFO::dfe_dfifo_cack_scth (const uint8_t& dmi, const uint8_t& dci) 
{//{{{
    while (1) {
        wait (dfe_dfifo_cack_scev[dmi][dci]);
        dfe_dfifo_cack[dmi][dci]->write(true);
        wait(getClkPeriod(pclk.basename()));
        dfe_dfifo_cack[dmi][dci]->write(false);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief thread process handle *dfe_ba_ce_irq* output
 * @details these processes issue a pulse lasting 1 clock cycle
 */
void DFE_FIFO::dfe_ba_ce_irq_scth () 
{//{{{
    while (1) {
        wait (dfe_ba_ce_irq_scev);
        dfe_ba_ce_irq.write(true);
        wait(getClkPeriod(pclk.basename()));
        dfe_ba_ce_irq.write(false);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief thread process handle *dfe_bb_ce_irq* output
 * @details these processes issue a pulse lasting 1 clock cycle
 */
void DFE_FIFO::dfe_bb_ce_irq_scth () 
{//{{{
    while (1) {
        wait (dfe_bb_ce_irq_scev);
        dfe_bb_ce_irq.write(true);
        wait(getClkPeriod(pclk.basename()));
        dfe_bb_ce_irq.write(false);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief thread process handle *dfe_berr_irq* output
 * @details these processes issue a pulse lasting 1 clock cycle
 */
void DFE_FIFO::dfe_berr_irq_scth () 
{//{{{
    while (1) {
        wait (dfe_berr_irq_scev);
        dfe_berr_irq.write(true);
        wait(getClkPeriod(pclk.basename()));
        dfe_berr_irq.write(false);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *ba_di* signal
 * @param[in] fci FIFO channel A index
 * @return value of *ba_di* signal
 */
uint32_t DFE_FIFO::calc_ba_di(const uint8_t& fci) 
{//{{{
    uint8_t dmi  = (*(pFuncModel->DFBFACTLCH[fci]))["ADSL"];
    uint8_t dci  = (*(pFuncModel->DFBFACTLCH[fci]))["BFACH"];
    bool    AUNE = (*(pFuncModel->DFBFACCTL))["AUNE"];

    if ((dmi < DFE_MACRO_NUMB) &&  (dci < DFE_CHN_NUMB[dmi])) {
        dfe_do_asel    [fci] = dfe_do    [dmi][dci]->read();
        dfe_do_upd_asel[fci] = dfe_do_upd[dmi][dci]->read();
        ba_di          [fci] =  (  dfe_do_asel[fci]
                                & (uint32_t(!AUNE) | 0xFFFFFFFE)
                                )
                              | (  dfe_do_upd_asel[fci]
                                &  AUNE
                                );
        return (ba_di[fci]);
    } else {
        return (0);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *ba_en* signal
 * @param[in] fci FIFO channel A index
 * @return true if both of AEN and CHEN of channel *fci* bit-fields are enabled.
 */
bool DFE_FIFO::calc_ba_en(const uint8_t& fci) 
{//{{{
    uint8_t AEN   = (*(pFuncModel->DFBFACCTL))["AEN"];
    uint8_t CHEN  = (*(pFuncModel->DFBFACTLCH[fci]))["CHEN"];
    return (AEN && CHEN);
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *ba_full* signal
 * @param[in] fci FIFO channel A index
 * @return true if the FIFO channel *fci* of buffer A circuit is full.
 */
bool DFE_FIFO::calc_ba_full(const uint8_t& fci) 
{//{{{
    return (DFBA_DEPTH == pFFAs[fci].size());
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO::ba_wr_scth() 
{//{{{
    while (1) {
        wait(ba_wr_calc_scev);

        // sampling value of *dfe_tim_ctrig* input signal at the time of active clock edge
        if (1 == dfe_tim_ctrig.read()) {
            /// -# check write condition and issue write command
            // if *dfe_tim_ctrig* is active, for each FIFO channel *fci* of buffer A circuit
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
                        uint32_t updateOVFA = currOVFA | mask;
                        if (updateOVFA != currOVFA) {
                            (*pFuncModel->DFBFACST)["OVFA"] = updateOVFA;
                            if (1 == (*pFuncModel->DFBFACCTL)["AIEE"]) {
                                dfe_berr_irq_scev.notify();
                            }
                        }
                    } else {
                        // allow to write data *ba_di* into the FIFO
                        ba_wr[fci]   = true;
                        // save data into the FIFO
                        ba_di_wr[fci] = ba_di[fci];
                        ba_ff_wr_scev[fci].notify(toNextSyncPoint(pclk.basename()));

                        // acknowledge the DFE channel
                        calc_ba_cack(fci);
                        notify_dfe_dfifo_cack();
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
                    dfe_ba_ce_irq_scev.notify(toNextSyncPoint(pclk.basename(), sc_time_stamp()));
            } while (0);
        }
    } // end-while
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO::ba_ff_wr_scth(const uint8_t& fci) 
{//{{{
    while (1) {
        wait(ba_ff_wr_scev[fci]);
        pFFAs[fci].push(ba_di_wr[fci]);
    }
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *bb_en* signal
  * @return true if BEN bit-field is enabled.
 */
bool DFE_FIFO::calc_bb_en() 
{//{{{
    uint8_t BEN = (*(pFuncModel->DFBFBCTL))["BEN"];
    return (bool)BEN;
}//}}}
bool DFE_FIFO::calc_bb_ceirq_en() 
{//{{{
    uint8_t OVFB = (*pFuncModel->DFBFBST) ["OVFB"];
    uint8_t BIEO = (*pFuncModel->DFBFBCTL)["BIEO"];
    return ((OVFB == 0) && (bool)BIEO);
}//}}}
bool DFE_FIFO::calc_ba_ceirq_en() 
{//{{{
    uint32_t OVFA = (*pFuncModel->DFBFACST) ["OVFA"];
    uint8_t  AIEO = (*pFuncModel->DFBFACCTL)["AIEO"];
    return ((OVFA == 0) && (bool)AIEO);
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *bb_full* signal
 * @return true if the FIFO channel of buffer B circuit is full.
 */
bool DFE_FIFO::calc_bb_full() 
{//{{{
    return (DFBB_DEPTH == FFB.size());
}//}}}
//------------------------------------------------------------------------------
/**@brief implement a combinational circuit part that drive *ba_cack* signal
 * @param[in] fci FIFO channel A index
 * @return true if the FIFO channel *fci* of buffer A circuit issues an ACK to the selected DFE channel
 */
bool DFE_FIFO::calc_ba_cack(const uint8_t& fci) 
{//{{{
    ba_cack[fci] = ba_wr[fci] && (!dfe_do_upd_asel[fci]);
    return (ba_cack[fci]);
}//}}}

//------------------------------------------------------------------------------
/**@brief implement a combinational multiplexor selecting data from DFE macros
 */
uint32_t DFE_FIFO::calc_bb_di() 
{//{{{
    uint8_t dmi  = (*(pFuncModel->DFBFBCTL))["BDSL"];
    uint8_t dci  = (*(pFuncModel->DFBFBCTL))["BFBCH"];
    if ((dmi < DFE_MACRO_NUMB) &&  (dci < DFE_CHN_NUMB[dmi])) {
        bb_di        = dfe_do [dmi][dci]->read();
        return (bb_di);
    } else
        return (0);
}//}}}
//------------------------------------------------------------------------------
void DFE_FIFO::bb_wr_scth() 
{//{{{
    while (1) {
        wait(bb_di_ctrig_scev);

        // sampling value of *dfe_do_ctrig* input signal at the time of active clock edge
        uint8_t dmi = (*(pFuncModel->DFBFBCTL))["BDSL"];
        uint8_t dci = (*(pFuncModel->DFBFBCTL))["BFBCH"];
        // if (1 == dfe_do_ctrig[dmi][dci]->read()) /*support Chan-san need, capture trigger only last 1 CLK_UHSB cycle, not CLK_HSB cycle, remove this condition*/
            // if *dfe_do_ctrig* is active
            if (calc_bb_en())
                if (calc_bb_full()) {
                    bb_cack = false;

                    // assert corresponding overflow flag if the FIFO channel becomes overflow
                    // and issue an error (overflow) interrupt request
                    uint8_t currOVFB = (*pFuncModel->DFBFBST)["OVFB"];
                    if (currOVFB == 0) {
                        (*pFuncModel->DFBFBST)["OVFB"] = 1;
                        if (1 == (*pFuncModel->DFBFBCTL)["BIEE"]) {
                            dfe_berr_irq_scev.notify();
                        }
                    }
                } else {
                    // allow to write data *ba_di* into the FIFO
                    FFB.push(bb_di);

                    // check and issue capture end interrupt
                    if (bb_state == FROM_EMPTY && FFB.size() == DFBB_DEPTH/2) {
                        bb_state = FROM_HALF_FULL;
                        if (calc_bb_ceirq_en())
                            dfe_bb_ce_irq_scev.notify();
                    }

                    // acknowledge the DFE channel
                    bb_cack = true;
                    notify_dfe_dfifo_cack();
                }
    } // end-while
}//}}}
//------------------------------------------------------------------------------
/**@brief this function implements a de-multiplexor generate ack signals to
 *        DFE channels by decoding settings of channels of buffer A circuit
 *        and buffer B circuit
 */
void DFE_FIFO::notify_dfe_dfifo_cack() 
{//{{{
    bool cack;
    uint8_t dmi_rgblk;
    uint8_t dci_rgblk;

    for (uint8_t dmi = 0; dmi < DFE_MACRO_NUMB; ++dmi)
        for (uint8_t dci = 0; dci < DFE_CHN_NUMB[dmi]; ++dci) {
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
                dfe_dfifo_cack_scev[dmi][dci].notify();
            }
        }
}//}}}

#ifdef    DFE_FIFO_TEST
void DFE_FIFO::log_dfe_do_scmt (const uint8_t& dmi, const uint8_t& dci) 
{//{{{
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE_FIFO]:"                  << dfe_do[dmi][dci]->basename()
        << " is changed to [" << std::hex << "0x" << uint32_t(dfe_do[dmi][dci]->read()) << "]"
        << endl;
    cout << oss.str();
}//}}}
void DFE_FIFO::log_dfe_do_upd_scmt    (const uint8_t& dmi, const uint8_t& dci) 
{//{{{
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE_FIFO]:"               << dfe_do_upd[dmi][dci]->basename()
        << " is changed to [" << uint32_t(dfe_do_upd[dmi][dci]->read()) << "]"
        << endl;
    cout << oss.str();
}//}}}
void DFE_FIFO::log_dfe_do_ctrig_scmt  (const uint8_t& dmi, const uint8_t& dci) 
{//{{{
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE_FIFO]:"               << dfe_do_ctrig[dmi][dci]->basename()
        << " is changed to [" << uint32_t(dfe_do_ctrig[dmi][dci]->read()) << "]"
        << endl;
    cout << oss.str();
}//}}}
void DFE_FIFO::log_dfe_dfifo_cack_scmt  (const uint8_t& dmi, const uint8_t& dci) 
{//{{{
    ostringstream oss;
    oss.clear();oss.str("");
    oss << std::dec << std::fixed << std::setprecision(0) << "[" << DFE_FIFO_Utils::Time::toDouble(sc_time_stamp(), SC_PS) << " ps]";
    oss << "[DFE_FIFO]:"               << dfe_dfifo_cack[dmi][dci]->basename()
        << " is changed to [" << uint32_t(dfe_dfifo_cack[dmi][dci]->read()) << "]"
        << endl;
    cout << oss.str();
}//}}}
#endif // DFE_FIFO_TEST

//------------------------------------------------------------------------------
// PY_IF
//------------------------------------------------------------------------------
void DFE_FIFO::AssertReset     (const std::string reset_name
                               ,const double      start_time
                               ,const double      period)
{//{{{
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
}//}}}
void DFE_FIFO::reset_command_begin_scmt() 
{//{{{
    EnableRstSrc("PRESETnCommand", true);
}//}}}
void DFE_FIFO::reset_command_end_scmt() 
{//{{{
    EnableRstSrc("PRESETnCommand", false);
}//}}}
void DFE_FIFO::SetCLKFreq      (const std::string     clock_name
                               ,const sc_dt::uint64   freq
                               ,const std::string     unit)
{//{{{
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
}//}}}
void DFE_FIFO::GetCLKFreq      (const std::string     clock_name)
{//{{{
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
}//}}}
/**@brief Force value to register by the software via Python interface
 * @param reg_name name of the desired register
 * @param reg_value desired value to be written
 */
void DFE_FIFO::ForceRegister   (const std::string     reg_name
                               ,const unsigned int    reg_value)
{//{{{
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
}//}}}
/**@brief Release forced value after forcing registers by the software
 *        via Python interface
 * @param reg_name name of the desired register
 */
void DFE_FIFO::ReleaseRegister (const std::string     reg_name)
{//{{{
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
}//}}}
/**@brief Write value to registers by software by the software
 *        via Python interface
 * @param reg_name name of the desired register
 * @param reg_value desired value to be written
 */
void DFE_FIFO::WriteRegister   (const std::string     reg_name
                               ,const unsigned int    reg_value)
{//{{{
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
}//}}}
/**@brief Read value of register by software by the software
 *        via Python interface
 * @param reg_name name of the desired register
 */
void DFE_FIFO::ReadRegister    (const std::string reg_name)
{//{{{
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
}//}}}
/**@brief List all registers name by software by the software
 *        via Python interface
 */
void DFE_FIFO::ListRegister    (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    pFuncModel->RegisterHandler(cmd);
}//}}}

void DFE_FIFO::SetMessageLevel (const std::string msg_lv)
{//{{{
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
}//}}}
void DFE_FIFO::DumpRegisterRW  (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    pFuncModel->RegisterHandler(cmd);
}//}}}

void DFE_FIFO::DumpStatusInfo ()
{//{{{
    std::string type = "StatInfo";
    re_printf("info", "PROFILE(%s): DFE_FIFO: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    re_printf("info", "PROFILE(%s): DFE_FIFO:   DFBFACST bits: 0x%08X\n", type.c_str(), pFuncModel->GetDFBFACST());
    re_printf("info", "PROFILE(%s): DFE_FIFO:   DFBFBST bits: 0x%08X\n", type.c_str(), pFuncModel->GetDFBFBST());
}//}}}
// vim600: set foldmethod=marker :