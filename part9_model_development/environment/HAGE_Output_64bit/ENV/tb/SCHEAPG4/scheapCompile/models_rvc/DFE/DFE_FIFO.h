// ---------------------------------------------------------------------
// $Id: DFE_FIFO.h,v 1.2 2018/09/06 10:17:09 huepham Exp $
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
#ifndef DFE_FIFO_DFE_FIFO_H_
#define DFE_FIFO_DFE_FIFO_H_

#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <queue>
using std::queue;
#include <cmath>

#include "BusSlaveFuncIf.h"

/// forward declaration of classes in a relationship
class DFE_FIFO;
class DFE_FIFO_Func;

/// Utilities
#include "DFE_FIFO_Utils.h"

/// the functional model
#include "DFE_FIFO_Func.h"

#define DFE_FIFO_TEST

/**
 limitation 1:
 writing and reading the FIFO at the same time, and at that time the FIFO
 is in empty or overflow state

 limitation 2:
 write into BDSL, BFBCH at the time ctrig is issue
 */
/**@brief top-module of the DFE FIFO
 */
class DFE_FIFO: public sc_module
              , public DFE_FIFO_Utils::CclkCtrl
              , public DFE_FIFO_Utils::CrstCtrl
              , public DFE_FIFO_Utils::PythonIf
{
#include "DFE_FIFO_cmdif.h"
    /**@enum bb_state_t state of buffer B circuit, is used to determine whether
     *       capture-end interrupt is issued.
     */
    enum bb_state_t {FROM_EMPTY     = 0 ///< the nearest state of buffer B is EMPTY, and it has not changed into HALF_FULL yet, when it reaches HALF_FULL state, an interrupt should be issued
                    ,FROM_HALF_FULL = 1 ///< the nearest state of buffer B is HALF_FULL, and it has not changed into EMPTY yet, when it reaches HALF_FULL stage again, an interrupt wont be issued
                    };
    friend class DFE_FIFO_Func;
    const uint8_t  DFE_MACRO_NUMB; ///< number of DFE macros
    uint8_t*       DFE_CHN_NUMB;   ///< number of DFE channels in DFE macros
    const uint8_t  DFBA_CHN_NUM;   ///< number of FIFO channels incorporated in buffer A circuit
    const uint8_t  DFBA_DEPTH;     ///< number of stages (depth) of FIFO channels incorporated in buffer A circuit
    const uint8_t  DFBB_DEPTH;     ///< number of stages (depth) of FIFO channels incorporated in buffer A circuit
//------------------------------------------------------------------------------
// public interfaces:
//    -# in/out/inout ports,
//    -# sockets,
//    -# events notifying the exterior, APIs
//------------------------------------------------------------------------------
public:
    // clock, reset and slave bus interface
    sc_in<sc_dt::uint64>  pclk;      ///< PCLK clock
    sc_in<bool>           presetn;   ///< reset signal
    // ports interfacing with buffer A circuit
    sc_in<bool>         dfe_tim_ctrig; ///< timer-capture control input, all enabled FIFO channels of buffer circuit A will capture inputs from the selected DFE channels
    // ports interfacing with DFE channels
    sc_in<bool>***      dfe_do_ctrig;  ///< data output capture triggers from DFE channels
    sc_in<uint32_t>***  dfe_do;        ///< data outputs from DFE channels
    sc_in<bool>***      dfe_do_upd;    ///< data output updated indicators from DFE channels
    sc_out<bool>***     dfe_dfifo_cack;///< data captured acknowledgments from FIFO channels to DFE channels
    // ports issues interrupt requests
    sc_out<bool> dfe_ba_ce_irq; ///< capture-end interrupt-request from buffer A circuit
    sc_out<bool> dfe_bb_ce_irq; ///< capture-end interrupt-request from buffer B circuit
    sc_out<bool> dfe_berr_irq;  ///< overflow-error interrupt-request from buffer A and/or B circuit
    // command APIs
    // void EnableReset(const bool& isActive); /*this model supports reset as a port*/

    DFE_FIFO_Func* getBusSlaveFunc();
//------------------------------------------------------------------------------
// internal sub-modules, signals, variables
//------------------------------------------------------------------------------
private:
    // the internal sub-modules/sub-model
    DFE_FIFO_Func*   pFuncModel;     ///< instance of functional part
    queue<uint32_t>* pFFAs;          ///< instance of the queue as FIFO channels of buffer A circuit
    queue<uint32_t>  FFB;            ///< instance of the queue as a FIFO channel of buffer B circuit

    // internal signals
    uint32_t* dfe_do_asel;
    bool*     dfe_do_upd_asel;
    uint32_t* ba_di;
    bool*     ba_wr;
    bool*     ba_cack;
    uint32_t* ba_di_wr;

    uint32_t         bb_di;
    bool             bb_cack;
    bb_state_t       bb_state;

    uint32_t calc_ba_di  (const uint8_t& fci);
    bool     calc_ba_en  (const uint8_t& fci);
    bool     calc_ba_full(const uint8_t& fci);
    bool     calc_ba_cack(const uint8_t& fci);
    void     init_FFA    (const uint8_t& fci);
    uint32_t calc_bb_di  ();
    bool     calc_bb_en  ();
    bool     calc_bb_full();
    void     init_FFB    ();
    void     notify_dfe_dfifo_cack();

    bool     calc_bb_ceirq_en();
    bool     calc_ba_ceirq_en();
//------------------------------------------------------------------------------
// internal processes and associated events
//------------------------------------------------------------------------------
private:
    void pclk_scmt();
    void presetn_scmt();
    void dfe_tim_ctrig_scmt ();
    void dfe_do_scmt       (const uint8_t& dmi, const uint8_t& dci);
    void dfe_do_upd_scmt   (const uint8_t& dmi, const uint8_t& dci);
    void dfe_do_ctrig_scmt (const uint8_t& dmi, const uint8_t& dci);

#ifdef    DFE_FIFO_TEST
    void log_dfe_do_scmt        (const uint8_t& dmi, const uint8_t& dci);
    void log_dfe_do_upd_scmt    (const uint8_t& dmi, const uint8_t& dci);
    void log_dfe_do_ctrig_scmt  (const uint8_t& dmi, const uint8_t& dci);
    void log_dfe_dfifo_cack_scmt(const uint8_t& dmi, const uint8_t& dci);
#endif // DFE_FIFO_TEST


    sc_event ba_wr_calc_scev;
    void ba_wr_scth ();
    sc_event* ba_ff_wr_scev;
    void ba_ff_wr_scth (const uint8_t& fci);

    sc_event bb_di_ctrig_scev;
    void bb_wr_scth ();

    sc_event** dfe_dfifo_cack_scev;
    void dfe_dfifo_cack_scth (const uint8_t& dmi, const uint8_t& dci);

    sc_event dfe_ba_ce_irq_scev;
    void dfe_ba_ce_irq_scth ();
    sc_event dfe_bb_ce_irq_scev;
    void dfe_bb_ce_irq_scth ();
    sc_event dfe_berr_irq_scev;
    void dfe_berr_irq_scth ();


    void end_of_elaboration();
//------------------------------------------------------------------------------
// public interface
//------------------------------------------------------------------------------
public:
    SC_HAS_PROCESS (DFE_FIFO);
    DFE_FIFO(const sc_module_name& iname
            ,const uint8_t&        iDFE_MACRO_NUMB = 2
            ,const uint8_t&        iDFBA_CHN_NUM   = 6
            ,const uint8_t&        iDFBA_DEPTH     = 8
            ,const uint8_t&        iDFBB_DEPTH     = 8
            );
    virtual ~DFE_FIFO();
//------------------------------------------------------------------------------
// PythonIF
//    internal unit does NOT need implement Python interface
//------------------------------------------------------------------------------
public:
    void AssertReset     (const std::string reset_name
                         ,const double      start_time
                         ,const double      period);
    void SetCLKFreq      (const std::string     clock_name
                         ,const sc_dt::uint64   freq
                         ,const std::string     unit);
    void GetCLKFreq      (const std::string     clock_name);
    void ForceRegister   (const std::string     reg_name
                         ,const unsigned int    reg_value);
    void ReleaseRegister (const std::string     reg_name);
    void WriteRegister   (const std::string     reg_name
                         ,const unsigned int    reg_value);
    void ReadRegister    (const std::string reg_name);
    void ListRegister    (void);
    // void Help            (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW  (const std::string is_enable);
    void DumpStatusInfo ();                    ///< Dump the status information of model
private:
    sc_event reset_command_begin_scev;
    void     reset_command_begin_scmt();
    sc_event reset_command_end_scev;
    void     reset_command_end_scmt();
//------------------------------------------------------------------------------
// helper methods
//------------------------------------------------------------------------------
private:
    void Initialize ();
    void CancelEvent();
    void HandleReset (const bool& isRstAct);
    void HandleZeroClk (const uint8_t& clkIdx, const bool& isZeroClk);
};

#endif /* DFE_FIFO_DFE_FIFO_H_ */
