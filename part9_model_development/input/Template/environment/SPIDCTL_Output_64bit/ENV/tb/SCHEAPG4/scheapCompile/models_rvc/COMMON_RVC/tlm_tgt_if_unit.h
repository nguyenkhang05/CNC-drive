// $Id$

/*************************************************************************
 *
 *  tlm_tgt_if_unit.h
 *
 * Copyright(c) 2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
*************************************************************************/

#ifndef __TLM_TGT_IF_UNIT_H__
#define __TLM_TGT_IF_UNIT_H__

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/peq_with_get.h"
#include "tlm_if.h"
#include "tlm_if_command.h"
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>

namespace vpcl
{

template <unsigned int BUSWIDTH, typename TYPES, unsigned int N>
class tlm_tgt_if;
template <unsigned int BUSWIDTH, typename TYPES, unsigned int N>
class tlm_tgt_thread;

/* Class of TLM Target Interface Unit */
template <unsigned int BUSWIDTH, typename TYPES, unsigned int N>
class tlm_tgt_if_unit : public tlm::tlm_fw_transport_if<TYPES>, public vpcl::tlm_base_if
{
  /* Friend Class */
  friend class tlm_tgt_if<BUSWIDTH, TYPES, N>;
  friend class tlm_tgt_thread<BUSWIDTH, TYPES, N>;

  /* Member Data */
  tlm_tgt_thread<BUSWIDTH, TYPES, N> m_tgt_thread;
  tlm_tgt_if<BUSWIDTH, TYPES, N> *m_core;
  unsigned int m_socket_id;
  tlm_tgt_if_parameter m_param;
  tlm_if_command m_command;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_req_thread_peq;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_resp_thread_peq;
  sc_event m_end_resp_event;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_resp_handle_peq;
  sc_event m_resp_free_event;
  sc_event m_enable_reset_event;
  bool m_is_reset;

public:
  /* Constructor */
  tlm_tgt_if_unit(const std::string &name, tlm_tgt_if<BUSWIDTH, TYPES, N> *p_core, unsigned int socket_id)
    : vpcl::tlm_base_if(name, "TARGET_SPEC")
    , m_tgt_thread((name + "_tgt_thread").c_str(), *this)
    , m_core(p_core)
    , m_socket_id(socket_id)
    , m_param(name, BUSWIDTH / 8)
    , m_command(name, BUSWIDTH / 8)
    , m_req_thread_peq((name + "tgt_req_thread_peq").c_str())
    , m_resp_thread_peq((name + "tgt_resp_thread_peq").c_str())
    , m_end_resp_event()
    , m_resp_handle_peq((name + "tgt_resp_handle_peq").c_str())
    , m_resp_free_event()
    , m_enable_reset_event()
    , m_is_reset(false)
  {
    /* do nothing */
  }

  /* Destructor */
  virtual ~tlm_tgt_if_unit()
  {
    /* do nothing */
  }

  bool tgt_get_param(vpcl::tlm_tgt_if_parameter *p_param) const
  {
    if (p_param != NULL) {
      *p_param = tgt_get_param();
      return true;
    }
    else {
      return false;
    }
  }

  bool tgt_set_param(vpcl::tlm_tgt_if_parameter *p_param)
  {
    if (p_param != NULL) {
      tgt_set_param(*p_param);
      return true;
    }
    else {
      return false;
    }
  }

  vpcl::tlm_tgt_if_parameter tgt_get_param() const
  {
    return m_param;
  }

  void tgt_set_param(const vpcl::tlm_tgt_if_parameter &param)
  {
    if ((param.name == m_param.name) && (param.bus_width == (BUSWIDTH / 8))) {
      m_param = param;
    }
    else {
      std::cout << "error : calling the method \"tgt_set_param\" has failed, cannot change the parameters \"name\" and \"bus_width\"." << std::endl;
    }
  }

  void tgt_init_param()
  {
    m_param.init(m_param.name, BUSWIDTH / 8);
  }

  std::string tgt_handle_command(const std::vector<std::string> &args)
  {
    return m_command.tgt_handle_command(args, m_param);
  }

  void tgt_enable_reset(bool is_reset) {
    if (is_reset) {
      this->cancel_all();
      m_req_thread_peq.cancel_all();
      m_resp_thread_peq.cancel_all();
      m_end_resp_event.cancel();
      m_resp_handle_peq.cancel_all();
      m_resp_free_event.cancel();
      tgt_reset_dmi(0x0, 0xffffffffffffffffULL);

      m_enable_reset_event.notify();
    }
    else {
      // not cancel for untimed simulation
    }
    m_is_reset = is_reset;
  }

private:
  void b_transport(tlm::tlm_generic_payload &trans, sc_time &t)
  {
    if (m_is_reset) {
      trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
      return;
    }

    if (m_param.info_displayed) {
      this->acquire(&trans);
      this->store_b_transport_info(&trans, t);
    }

    /* check and set transaction attributes */
    tgt_check_gp_attribute(trans, false);

    /* callback method */
    t += trans.is_read() ? m_param.rd_latency : m_param.wr_latency;
    m_core->tgt_acc(m_socket_id, trans, t);

    /* matsuo legacy */
    tgt_dump_log(trans, t);

    if (m_param.info_displayed) {
      this->print_b_transport_info(&trans, t);
      this->release(&trans);
    }
  }

  unsigned int transport_dbg(tlm::tlm_generic_payload &trans)
  {
    if (m_param.info_displayed) {
      this->acquire(&trans);
      this->store_transport_dbg_info(&trans);
    }

    /* check and set transaction attributes */
    tgt_check_gp_attribute(trans, true);

    /* callback method */
    unsigned data_length = m_core->tgt_acc_dbg(m_socket_id, trans);

    if (m_param.info_displayed) {
      this->print_transport_dbg_info(&trans);
      this->release(&trans);
    }

    return data_length;
  }

  void tgt_check_gp_attribute(tlm::tlm_generic_payload &trans, bool debug)
  {
    /* in the case of transport interface */
    if ((! debug) &&
    (trans.get_data_ptr() != NULL) &&
    ((trans.get_data_length() > 0) || (trans.get_command() == tlm::TLM_IGNORE_COMMAND)) &&
    ((trans.get_byte_enable_length() > 0) || (trans.get_byte_enable_ptr() == NULL)) &&
    (trans.get_streaming_width() >= trans.get_data_length()) &&
    (! trans.is_dmi_allowed()) &&
    (trans.get_response_status() == tlm::TLM_INCOMPLETE_RESPONSE)) {
      /* do nothing */
    }

    /* in the case of debug transport interface */
    else if (debug &&
    (trans.get_data_ptr() != NULL) &&
    ((trans.get_data_length() > 0) || (trans.get_command() == tlm::TLM_IGNORE_COMMAND))) {
      /* do nothing */
    }

    else {
      gp_assert(false, &trans,
        "TRA : setting of transaction attributes violates tlm-2.0 rules.");
    }

    /* attach extension information */
    vpcl::tlm_if_extension *p_ext = trans.get_extension<vpcl::tlm_if_extension>();
    if (p_ext != NULL) {
      p_ext->tgt_if_use = true;
      p_ext->tgt_if_param = m_param;
    }
  }

  void tgt_acc_default(tlm::tlm_generic_payload &trans, sc_time &t)
  {
    /* get transaction attributes */
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = m_core->tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, false);
    if (! status) {
      trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
      return;
    }
    if (data_ptr == NULL) {
      std::cout << "fatal : incoming data pointer of generic payload is null." << std::endl;
      sc_stop();
    }

    /* callback method */
    if (command == tlm::TLM_READ_COMMAND) {
      status = m_core->tgt_rd((unsigned int) address, data_ptr, data_length, trans.get_extension<vpcl::tlm_if_extension>());
    }
    else if (command == tlm::TLM_WRITE_COMMAND) {
      status = m_core->tgt_wr((unsigned int) address, data_ptr, data_length, trans.get_extension<vpcl::tlm_if_extension>());
    }
    /* be necessarily TLM_IGNORE_COMMAND */
    else {
      status = true;
    }

    /* set response status attribute */
    trans.set_response_status(status ? tlm::TLM_OK_RESPONSE : tlm::TLM_GENERIC_ERROR_RESPONSE);
  }

  unsigned int tgt_acc_dbg_default(tlm::tlm_generic_payload &trans)
  {
    /* get transaction attributes */
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = m_core->tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, true);
    if (data_ptr == NULL) {
      std::cout << "fatal : incoming data pointer of generic payload is null." << std::endl;
      sc_stop();
    }

    /* callback method */
    if (command == tlm::TLM_READ_COMMAND) {
      status = m_core->tgt_rd_dbg((unsigned int) address, data_ptr, data_length, trans.get_extension<vpcl::tlm_if_extension>());
    }
    else if (command == tlm::TLM_WRITE_COMMAND) {
      status = m_core->tgt_wr_dbg((unsigned int) address, data_ptr, data_length, trans.get_extension<vpcl::tlm_if_extension>());
    }
    /* be necessarily TLM_IGNORE_COMMAND */
    else {
      status = true;
      data_length = 0;
    }

    /* set response status attribute and return value */
    if (status) {
      trans.set_response_status(tlm::TLM_OK_RESPONSE);
      return data_length;
    }
    else {
      trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
      return 0;
    }
  }

  bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data)
  {
    if (m_is_reset) {
      trans.set_dmi_allowed(false);
      trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
      return false;
    }

    /* callback method */
    return m_core->tgt_get_dmi(m_socket_id, trans, dmi_data);
  }

  void tgt_reset_dmi(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
  {
    (*m_core->m_tgt_sockets[m_socket_id])->invalidate_direct_mem_ptr(start_range, end_range);
  }

  void tgt_req_rx_default(tlm::tlm_generic_payload &trans, sc_time &t)
  {
    sc_time prev_t = t;

    /* callback method */
    m_core->tgt_acc(m_socket_id, trans, t);
    gp_assert(t >= prev_t, &trans,
      "TMG : returned timing annotation (%s) must be greater than or equal to called value (%s).", t.to_string().c_str(), prev_t.to_string().c_str());

    /* activate response handle */
    m_resp_handle_peq.notify(trans, t);
  }

  void tgt_resp_tx(tlm::tlm_generic_payload &trans)
  {
    /* check outgoing transaction attributes */
    gp_assert(trans.get_data_ptr() != NULL, &trans,
      "TRA : setting of transaction attributes violates tlm-2.0 rules.");

    gp_assert(this->is_entried(&trans), &trans,
      "OBJ : called transaction object (not entried) must be entried in memory manager.", &trans);
    gp_assert(this->get_phase(&trans) == tlm::END_REQ, &trans,
      "PHS : called phase transition (%s->BEGIN_REQ) must be END_REQ->BEGIN_RESP.", this->to_string(this->get_phase(&trans)).c_str());
    gp_assert(this->will_be_resp_allowed(&trans, SC_ZERO_TIME), &trans,
      "RSP : response transition timing (%s) must be greater than or equal to request ending time (%s).", sc_time_stamp().to_string().c_str(), this->get_resp_allowed_time(&trans).to_string().c_str());

    /* activate response thread */
    m_resp_thread_peq.notify(trans);
  }

  void tgt_resp_free_default(tlm::tlm_generic_payload &trans)
  {
    /* resume response handle */
    m_resp_free_event.notify();
  }

  void tgt_req_thread(void)
  {
    while (true) {
      tlm::tlm_generic_payload *p_trans = NULL;
      tlm::tlm_phase phase = tlm::UNINITIALIZED_PHASE;
      tlm::tlm_phase prev_phase = tlm::UNINITIALIZED_PHASE;
      sc_time t = SC_ZERO_TIME;
      sc_time prev_t = SC_ZERO_TIME;

      try {
        /* repeat while reset is disabled */
        while (true) {
          /* wait next transaction */
          wait(m_req_thread_peq.get_event() | m_enable_reset_event);
          if (m_is_reset) throw TLMCC_RESET_EXCEPTION;
          while ((p_trans = m_req_thread_peq.get_next_transaction()) != NULL) {

            /* wait a front part of request accept delay */
            wait_time_with_event(tgt_req_tp1(*p_trans), m_enable_reset_event);
            if (m_is_reset) throw TLMCC_RESET_EXCEPTION;

            t = prev_t = tgt_req_t1(*p_trans);

            /* callback method */
            m_core->tgt_req_rx(m_socket_id, *p_trans, prev_t);

            /* wait a back part of request accept delay with lock response */
            this->lock_resp_allowed(p_trans, t);
            wait_time_with_event(t, m_enable_reset_event);
            if (m_is_reset) throw TLMCC_RESET_EXCEPTION;
            this->unlock_resp_allowed(p_trans);

            /* end of request thread */
            p_trans = NULL;
          }
        }
      }

      catch (enum tlmcc_exception_enum ex) {
        if (ex != TLMCC_RESET_EXCEPTION) break;
      }
    }
  }

  void tgt_resp_thread(void)
  {
    while (true) {
      tlm::tlm_generic_payload *p_trans = NULL;
      tlm::tlm_sync_enum sync = tlm::TLM_ACCEPTED;
      tlm::tlm_phase phase = tlm::UNINITIALIZED_PHASE;
      tlm::tlm_phase prev_phase = tlm::UNINITIALIZED_PHASE;
      sc_time t = SC_ZERO_TIME;
      sc_time prev_t = SC_ZERO_TIME;
  
      try {
        /* repeat while reset is disabled */
        while (true) {
          /* wait next transaction */
          wait(m_resp_thread_peq.get_event() | m_enable_reset_event);
          if (m_is_reset) throw TLMCC_RESET_EXCEPTION;
          while ((p_trans = m_resp_thread_peq.get_next_transaction()) != NULL) {
            bool mm_flag = p_trans->has_mm();
            /* matsuo legacy */
            tgt_dump_log(*p_trans);
    
            /* wait end of request with no time consumption */
            while (! this->is_resp_allowed(p_trans)) {
              wait(this->get_resp_allowed_event(p_trans) | m_enable_reset_event);
              if (m_is_reset) throw TLMCC_RESET_EXCEPTION;
            }
    
            /* [c] : nb_transport_bw/BEGIN_RESP */
            phase = prev_phase = tlm::BEGIN_RESP;
            t = prev_t = SC_ZERO_TIME;
            sync = (*m_core->m_tgt_sockets[m_socket_id])->nb_transport_bw(*p_trans, phase, t);
            this->set_history(p_trans, "nb_transport_bw", prev_phase, prev_t, sync, phase, t);
    
            /* (c7) : TLM_ACCEPTED */
            if ((sync == tlm::TLM_ACCEPTED) && (phase == tlm::BEGIN_RESP)) {
              gp_assert(t == prev_t, p_trans,
                "TMG : returned timing annotation (%s) must be equal to called value (%s).", t.to_string().c_str(), prev_t.to_string().c_str());
    
              /* wait next phase transition */
              wait(m_end_resp_event | m_enable_reset_event);
              if (m_is_reset) throw TLMCC_RESET_EXCEPTION;
            }
    
            /* (c8) : TLM_UPDATED/END_RESP | (c9) : TLM_COMPLETED */
            else if (((sync == tlm::TLM_UPDATED) && (phase == tlm::END_RESP)) || (sync == tlm::TLM_COMPLETED)) {
              gp_assert(t >= prev_t, p_trans,
                "TMG : returned timing annotation (%s) must be greater than or equal to called value (%s).", t.to_string().c_str(), prev_t.to_string().c_str());
    
              /* wait response accept delay */
//              wait_time_with_event(t, m_enable_reset_event);
              if (m_is_reset) throw TLMCC_RESET_EXCEPTION;
            }
    
            /* invalid phase transition */
            else {
              gp_assert(false, p_trans,
                "PHS : returned phase transition (%s/%s) must be TLM_ACCEPTED/BEGIN_RESP, TLM_UPDATED/END_RESP, or TLM_COMPLETED.", this->to_string(sync).c_str(), this->to_string(phase).c_str());
            }
    
            /* callback method */
            m_core->tgt_resp_free(m_socket_id, *p_trans);
    
            /* display transaction history */
            if (m_param.info_displayed && mm_flag) {  // update
              this->print_history(p_trans);
            }
    
            /* release transaction object at current delta cycle */
            this->release(p_trans, mm_flag);  // update
    
            /* end of response thread */
            p_trans = NULL;
          }
        }
      }

      catch (enum tlmcc_exception_enum ex) {
        if (ex != TLMCC_RESET_EXCEPTION) break;
      }
    }
  }

  void tgt_resp_handle(void)
  {
    while (true) {
      tlm::tlm_generic_payload *p_trans = NULL;

      try {
        /* repeat while reset is disabled */
        while (true) {
          /* wait next transaction */
          wait(m_resp_handle_peq.get_event() | m_enable_reset_event);
          if (m_is_reset) throw TLMCC_RESET_EXCEPTION;
          while ((p_trans = m_resp_handle_peq.get_next_transaction()) != NULL) {

            /* begin response */
            tgt_resp_tx(*p_trans);

            /* wait end of response */
            wait(m_resp_free_event | m_enable_reset_event);
            if (m_is_reset) throw TLMCC_RESET_EXCEPTION;

            /* end of response handle */
            p_trans = NULL;
          }
        }
      }

      catch (enum tlmcc_exception_enum ex) {
        if (ex != TLMCC_RESET_EXCEPTION) break;
      }
    }
  }

  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t)
  {
    if (m_is_reset) {
      trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
      phase = tlm::END_RESP;
      return tlm::TLM_COMPLETED;
    }

    tlm::tlm_sync_enum sync = tlm::TLM_ACCEPTED;
    tlm::tlm_phase prev_phase = phase;
    sc_time prev_t = t;

    gp_assert((prev_phase == tlm::BEGIN_REQ) || this->is_entried(&trans), &trans,
      "PHS : called phase transition (START->%s) must be START->BEGIN_REQ.", this->to_string(prev_phase).c_str());

    /* [a] : nb_transport_fw/BEGIN_REQ */
    if (prev_phase == tlm::BEGIN_REQ && (! this->is_entried(&trans))) {

      /* check and set transaction attributes */
      tgt_check_gp_attribute(trans, false);

      /* acquire transaction object */
      this->acquire(&trans);

      /* with multi phase */
      if (m_param.fw_req_phase == tlm::END_REQ) {

        /* activate request thread */
        m_req_thread_peq.notify(trans, prev_t);

        /* (a2) : TLM_UPDATED/END_REQ */
        sync = tlm::TLM_UPDATED;
        phase = tlm::END_REQ;
        t += tgt_req_tp1(trans) + tgt_req_t1(trans);
        this->set_history(&trans, "nb_transport_fw", prev_phase, prev_t, sync, phase, t);
      }

      /* with single phase (early completion) */
      else if (m_param.fw_req_phase == tlm::END_RESP) {

        /* callback method */
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        t += tgt_req_tp1(trans) + tgt_req_t1(trans);
        m_core->tgt_acc(m_socket_id, trans, t);
        gp_assert(t >= (prev_t + tgt_req_tp1(trans) + tgt_req_t1(trans)), &trans,
          "TMG : returned timing annotation (%s) must be greater than or equal to called value (%s).", t.to_string().c_str(), (prev_t + tgt_req_tp1(trans) + tgt_req_t1(trans)).to_string().c_str());

        /* check outgoing transaction attribute */
        gp_assert(trans.get_data_ptr() != NULL, &trans,
          "TRA : setting of transaction attributes violates tlm-2.0 rules.");

        /* matsuo legacy */
        tgt_dump_log(trans, t);

        /* (a4) : TLM_COMPLETED */
        sync = tlm::TLM_COMPLETED;
        phase = tlm::END_RESP;
        this->set_history(&trans, "nb_transport_fw", prev_phase, prev_t, sync, phase, t);

        /* display transaction history */
        if (m_param.info_displayed) {
          this->print_history(&trans);
        }

        /* release transaction object at current delta cycle */
        this->release(&trans);
      }

      /* unsupported parameter setting */
      else {
        gp_assert(false, &trans,
          "PRM : parameter fw_req_phase (%s) must be END_REQ or END_RESP.", this->to_string(m_param.fw_req_phase).c_str());
      }
    }

    /* [d] : nb_transport_fw/END_RESP */
    else if (prev_phase == tlm::END_RESP) {
      gp_assert(this->get_phase(&trans) == tlm::BEGIN_RESP, &trans,
        "PHS : called phase transition (%s->END_RESP) must be BEGIN_RESP->END_RESP.", this->to_string(this->get_phase(&trans)).c_str());

      /* resume response thread */
      m_end_resp_event.notify(prev_t);

      /* (d11) : TLM_COMPLETED */
      sync = tlm::TLM_COMPLETED;
      this->set_history(&trans, "nb_transport_fw", prev_phase, prev_t, sync, phase, t);
    }

    /* invalid phase transition */
    else {
      gp_assert(false, &trans,
        "PHS : called phase transition (%s->%s) must be START->BEGIN_REQ or BEGIN_RESP->END_RESP.", this->to_string(this->get_phase(&trans)).c_str(), this->to_string(prev_phase).c_str());
    }

    return sync;
  }

  sc_time tgt_req_t1(tlm::tlm_generic_payload &trans)
  {
    sc_time t = SC_ZERO_TIME;

    if (trans.is_write()) {
      t = m_param.bus_clk * (double) ((trans.get_data_length() + (BUSWIDTH / 8) - 1) / (BUSWIDTH / 8));
    }
    else if (trans.is_read()) {
      t = m_param.bus_clk;
    }
    /* be necessarily TLM_IGNORE_COMMAND */
    else {
      /* do nothing */
    }
    return t;
  }

  sc_time tgt_req_tp1(tlm::tlm_generic_payload &trans)
  {
    sc_time t = SC_ZERO_TIME;

    if (trans.is_write()) {
      t = m_param.wr_req_latency;
    }
    else if (trans.is_read()) {
      t = m_param.rd_req_latency;
    }
    /* be necessarily TLM_IGNORE_COMMAND */
    else {
      /* do nothing */
    }
    return t;
  }

  void wait_time(sc_time t)
  {
    if (t != SC_ZERO_TIME) {
      wait(t);
    }
  }

  void wait_time_with_event(sc_time t, sc_event &e)
  {
    if (t != SC_ZERO_TIME) {
      wait(t, e);
    }
  }

  void tgt_dump_log(tlm::tlm_generic_payload &trans, sc_time delay=SC_ZERO_TIME)
  {
    if (m_param.p_log_file != NULL) {
      if ((trans.is_write() && (m_param.wr_log == true)) || (trans.is_read() && (m_param.rd_log == true))) {
        vpcl::tlm_if_extension *p_ext = NULL;
        trans.get_extension(p_ext);
        unsigned int src = (p_ext != NULL) ? p_ext->ini_if_param.src_id : 0;
        sc_time pass_time = sc_time_stamp() + delay;

        fprintf(m_param.p_log_file, "%8s :  [%-8s]  I(%02X) <-T  ", pass_time.to_string().c_str(), m_param.name.c_str(), src);
        fprintf(m_param.p_log_file, "%s  (%08X)  ", (trans.is_write() ? "WR" : "RD"), (unsigned int) trans.get_address());
        if ((trans.get_data_ptr() == NULL) || (trans.get_data_length() == 0)) {
          fprintf(m_param.p_log_file, "NO DATA");
        }
        else {
          for (unsigned int i = 0; (i < trans.get_data_length()) && (i < 128); i++) {
            fprintf(m_param.p_log_file, "%02X%s", *(trans.get_data_ptr() + i), (((i % 4) == 3) ? " " : ""));
          }
        }
        fprintf(m_param.p_log_file, "\n");
        fflush(m_param.p_log_file);
      }
    }
  }
};

/* Class of TLM Target Thread */
template <unsigned int BUSWIDTH, typename TYPES, unsigned int N>
class tlm_tgt_thread : public sc_module
{
  /* Member Data */
  tlm_tgt_if_unit<BUSWIDTH, TYPES, N> &m_tgt_if_unit;

public:
  SC_HAS_PROCESS(tlm_tgt_thread);

  /* Constructor */
  tlm_tgt_thread(sc_module_name name, tlm_tgt_if_unit<BUSWIDTH, TYPES, N> &tgt_if_unit)
    : sc_module(name)
    , m_tgt_if_unit(tgt_if_unit)
  {
    SC_THREAD(activate_tgt_req_thread);
    SC_THREAD(activate_tgt_resp_thread);
    SC_THREAD(activate_tgt_resp_handle);
  }

  /* Destructor */
  ~tlm_tgt_thread()
  {
    /* do nothing */
  }

private:
  /* Thread Activator */
  void activate_tgt_req_thread()
  {
    m_tgt_if_unit.tgt_req_thread();
  }

  void activate_tgt_resp_thread()
  {
    m_tgt_if_unit.tgt_resp_thread();
  }

  void activate_tgt_resp_handle()
  {
    m_tgt_if_unit.tgt_resp_handle();
  }
};

}

#endif
