// $Id: tlm_ini_if.h,v 1.25 2013/03/29 12:06:26 masudamk Exp $

/*************************************************************************
 *
 *  tlm_ini_if.h
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

#ifndef __TLM_INI_IF_H__
#define __TLM_INI_IF_H__

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/peq_with_get.h"
#include "tlm_utils/peq_with_cb_and_phase.h"
#include "tlm_if.h"
#include "tlm_if_command.h"
#include <cstdio>
#include <cstring>
#include <climits>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <sstream>

namespace vpcl
{

template <unsigned int BUSWIDTH, typename TYPES, unsigned int N>
class tlm_ini_thread;

/* Class of TLM Initiator Interface */
template <unsigned int BUSWIDTH = 64, typename TYPES = tlm::tlm_base_protocol_types, unsigned int N = 1>
class tlm_ini_if : public tlm::tlm_bw_transport_if<TYPES>, public vpcl::tlm_base_if
{
  /* Friend Class */
  friend class tlm_ini_thread<BUSWIDTH, TYPES, N>;

public:
  /* Member Data */
  tlm::tlm_initiator_socket<BUSWIDTH, TYPES, 1, sc_core::SC_ONE_OR_MORE_BOUND> m_ini_socket;

private:
  tlm_ini_thread<BUSWIDTH, TYPES, N> m_ini_thread;
  std::string m_name;
  tlm_ini_if_parameter m_param;
  tlm_if_command m_command;
  std::list<vpcl::dmi_memory> m_dmi_list;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_req_thread_peq;
  tlm_utils::peq_with_cb_and_phase<tlm_ini_if> m_resp_method_peq;
  tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_release_peq;

public:
  /* Constructor */
  tlm_ini_if(const char *name, unsigned int src_id = 0)
    : vpcl::tlm_base_if(std::string(name), "INITIATOR_SPEC")
    , m_ini_socket((std::string(name) + "_ini_socket").c_str())
    , m_ini_thread((std::string(name) + "_ini_thread").c_str(), *this)
    , m_name(name)
    , m_param(std::string(name), BUSWIDTH / 8)
    , m_command(std::string(name), BUSWIDTH / 8)
    , m_dmi_list()
    , m_req_thread_peq((std::string(name) + "_ini_req_thread_peq").c_str())
    , m_resp_method_peq(this, &tlm_ini_if::ini_resp_method)
    , m_release_peq((std::string(name) + "_ini_release_peq").c_str())
  {
    m_ini_socket.bind(*this);
    m_param.src_id = src_id;
    m_param.m_outstanding_configs[0];
  }

  /* Destructor */
  virtual ~tlm_ini_if()
  {
    /* do nothing */
  }

  bool ini_get_param(vpcl::tlm_ini_if_parameter *p_param) const
  {
    if (p_param != NULL) {
      *p_param = ini_get_param();
      return true;
    }
    else {
      return false;
    }
  }

  bool ini_set_param(vpcl::tlm_ini_if_parameter *p_param)
  {
    if (p_param != NULL) {
      ini_set_param(*p_param);
      return true;
    }
    else {
      return false;
    }
  }

  vpcl::tlm_ini_if_parameter ini_get_param() const
  {
    return m_param;
  }

  void ini_set_param(const vpcl::tlm_ini_if_parameter &param)
  {
    if ((param.name == m_name) && (param.bus_width == (BUSWIDTH / 8))) {
      m_param = param;
    }
    else {
      std::cout << "error : calling the method \"ini_set_param\" has failed, cannot change the parameters \"name\" and \"bus_width\"." << std::endl;
      sc_stop();
    }
  }

  void ini_init_param()
  {
    m_param.init(m_name, BUSWIDTH / 8);
  }

  unsigned int ini_get_num_outstanding(unsigned int id, bool *error = NULL)
  {
    if (m_param.m_outstanding_configs.find(id) == m_param.m_outstanding_configs.end()) {
      if (error != NULL) {
        *error = true;
      }
      return 0;
    }
    else {
      if (error != NULL) {
        *error = false;
      }
      return m_param.m_outstanding_configs[id].get_count();
    }
  }

  void ini_set_max_outstanding(unsigned int id, unsigned int max)
  {
    if ((max > 0) && (m_param.m_outstanding_configs[id].get_count() == 0) && m_param.m_outstanding_configs[id].queue_is_empty()) {
      m_param.m_outstanding_configs[id].set_max(max);
    }
    else {
      std::cout << "error : calling the method \"ini_set_max_outstanding\" has failed, the argument \"max\" is invalid or the outstanding configuration is in use." << std::endl;
      sc_stop();
    }
  }

  std::string ini_handle_command(const std::vector<std::string> &args)
  {
    return m_command.ini_handle_command(args, m_param);
  }

  std::string ini_import_command_file(const std::string &file_name)
  {
    /* file opening */
    FILE *p_file = NULL;
    fopen_s(&p_file, file_name.c_str(), "r");
    if (p_file == NULL) {
      std::cout << "error : cannot open source file \"" << file_name << "\"." << std::endl;
      sc_stop();
    }
    /* for each line */
    std::string command_msg = "";
    while (true) {
      /* capture 1 line */
      char line[1024];
      memset(line, 0x00, sizeof(line));
      if (fgets(line, 1024, p_file) == NULL) {
        break;
      }
      /* fetch 1 command */
      std::istringstream line_stream((std::string) line);
      std::vector<std::string> command_args;
      while (true) {
        std::string command_arg;
        line_stream >> std::skipws >> command_arg;
        if (command_arg.size() == 0) {
          break;
        }
        command_args.push_back(command_arg);
      }
      /* decode and execute 1 command */
      if ((! command_args.empty()) && (command_args[0].substr(command_args[0].rfind(".") + 1) == m_name)) {
        command_args.erase(command_args.begin());
        command_msg += ini_handle_command(command_args);
      }
    }
    /* file closing */
    fclose(p_file);
    return command_msg;
  }

protected:
  tlm::tlm_generic_payload *ini_acquire_gp()
  {
    /* acquire transaction object */
    tlm::tlm_generic_payload *p_trans = this->acquire();
    return p_trans;
  }

  tlm::tlm_generic_payload *ini_acquire_gp(tlm::tlm_command command, sc_dt::uint64 address, unsigned char *data_ptr, unsigned int data_length)
  {
    /* acquire transaction object */
    tlm::tlm_generic_payload *p_trans = ini_acquire_gp();
    if (p_trans == NULL) {
      std::cout << "fatal : acquired transaction pointer \"p_trans\" is null." << std::endl;
      sc_stop();
    }

    /* set transaction attributes */
    p_trans->set_command(command);
    p_trans->set_address(address);
    p_trans->set_data_ptr(data_ptr);
    p_trans->set_data_length(data_length);
    p_trans->set_streaming_width(data_length);
    return p_trans;
  }

  bool ini_rd(unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext = NULL)
  {
    return ini_acc_sub(tlm::TLM_READ_COMMAND, static_cast<sc_dt::uint64>(addr), p_data, size, p_ext, false);
  }

  bool ini_wr(unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext = NULL)
  {
    return ini_acc_sub(tlm::TLM_WRITE_COMMAND, static_cast<sc_dt::uint64>(addr), p_data, size, p_ext, false);
  }

  bool ini_rd_dbg(unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext = NULL)
  {
    return ini_acc_sub(tlm::TLM_READ_COMMAND, static_cast<sc_dt::uint64>(addr), p_data, size, p_ext, true);
  }

  bool ini_wr_dbg(unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext = NULL)
  {
    return ini_acc_sub(tlm::TLM_WRITE_COMMAND, static_cast<sc_dt::uint64>(addr), p_data, size, p_ext, true);
  }

  void ini_acc(tlm::tlm_generic_payload &trans)
  {
    /* create aligned generic payload */
    unsigned int width = 0;
    unsigned int length = 0;
    if (trans.get_extension<vpcl::tlm_if_extension>() != NULL) {
      width = trans.get_extension<vpcl::tlm_if_extension>()->address_alignment;
      length = trans.get_extension<vpcl::tlm_if_extension>()->data_length_alignment;
    }
    tlm::tlm_generic_payload *p_trans = create_aligned_gp(trans, width, length);
    sc_assert(p_trans);

    /* in the case of loosely timed */
    if (m_param.access_mode == vpcl::TLM_IF_LT_ACCESS) {

      /* check and set transaction attributes */
      ini_check_gp_attribute(*p_trans, false);

      /* matsuo legacy */
      ini_dump_log(*p_trans);

      /* call method */
      sc_time t = SC_ZERO_TIME;
      if (m_param.dmi_size == 0) {
        if (m_param.info_displayed) {
          this->store_b_transport_info(p_trans, t);
        }
        m_ini_socket->b_transport(*p_trans, t);
        if (m_param.info_displayed) {
          this->print_b_transport_info(p_trans, t);
        }
      }
      else {
        ini_acc_dmi(*p_trans, t);
      }

      /* wait timing annotation */
      wait_time(t);
    }

    /* in the case of approximately timed */
    else {
      /* create clones of the original generic payload */
      std::list<tlm::tlm_generic_payload *> &p_clones = this->get_clones(p_trans);
      create_gp_clones(p_trans, width, length, p_clones);

      /* update transaction attributes of the original generic payload */
      p_trans->set_dmi_allowed(true);
      p_trans->set_response_status(tlm::TLM_OK_RESPONSE);

      /* transport all transactions */
      for (std::list<tlm::tlm_generic_payload *>::iterator it = p_clones.begin(); it != p_clones.end(); it++) {
        this->set_origin(*it, p_trans);
        ini_req_tx(**it);
      }

      /* wait until all transactions complete */
      while (! p_clones.empty()) {
        wait(this->get_resp_rx_event(p_trans));
      }
    }

    /* destroy aligned generic payload */
    destroy_aligned_gp(trans, p_trans);

    /* release transaction object at next delta cycle */
    sc_time delta_delay = SC_ZERO_TIME;
    m_release_peq.notify(trans, delta_delay);
  }

  unsigned int ini_acc_dbg(tlm::tlm_generic_payload &trans)
  {
    /* check and set transaction attributes */
    ini_check_gp_attribute(trans, true);

    /* call method */
    if (m_param.info_displayed) {
      this->store_transport_dbg_info(&trans);
    }
    unsigned int data_length = m_ini_socket->transport_dbg(trans);
    if (m_param.info_displayed) {
      this->print_transport_dbg_info(&trans);
    }

    /* release transaction object at next delta cycle */
    sc_time delta_delay = SC_ZERO_TIME;
    m_release_peq.notify(trans, delta_delay);
    return data_length;
  }

  void ini_req_tx(tlm::tlm_generic_payload &trans)
  {
    gp_assert(this->is_entried(&trans), &trans,
      "OBJ : called transaction object (not entried) must be entried in memory manager.");

    /* control outstanding config */
    unsigned int id = get_outstanding_id(trans);
    if (m_param.m_outstanding_configs[id].count_is_max()) {
      m_param.m_outstanding_configs[id].queue_push(&trans);
    }
    else {
      sc_assert(m_param.m_outstanding_configs[id].queue_is_empty());
      m_param.m_outstanding_configs[id].count_increment();
      m_req_thread_peq.notify(trans);
      this->set_outstanding(&trans, true);
    }
  }

private:
  void ini_release_gp_thread()
  {
    tlm::tlm_generic_payload *p_trans = NULL;

    while (true) {
      /* wait next transaction */
      wait(m_release_peq.get_event());
      while ((p_trans = m_release_peq.get_next_transaction()) != NULL) {

        /* display transaction history */
        if (m_param.info_displayed) {
          this->print_history(p_trans);
        }

        /* control outstanding config */
        if (this->is_outstanding(p_trans)) {
          unsigned int id = get_outstanding_id(*p_trans);
          if (m_param.m_outstanding_configs[id].queue_is_empty()) {
            m_param.m_outstanding_configs[id].count_decrement();
          }
          else {
            sc_assert(m_param.m_outstanding_configs[id].count_is_max());
            tlm::tlm_generic_payload *p_next_trans = m_param.m_outstanding_configs[id].queue_pop();
            this->set_outstanding(p_next_trans, true);
            m_req_thread_peq.notify(*p_next_trans);
          }
        }

        /* release transaction object */
        this->release(p_trans);
        p_trans = NULL;
      }
    }
  }

  unsigned int get_outstanding_id(const tlm::tlm_generic_payload &trans) const
  {
    unsigned int id = (trans.get_extension<vpcl::tlm_if_extension>() == NULL) ?
      0 : trans.get_extension<vpcl::tlm_if_extension>()->outstanding_id;
    if (m_param.m_outstanding_configs.find(id) == m_param.m_outstanding_configs.end()) {
      std::cout << "error : calling the method \"get_outstanding_id\" has failed, the extension attribute \"outstanding_id\" is invalid." << std::endl;
      sc_stop();
    }
    return id;
  }

  virtual void ini_create_ext(tlm::tlm_generic_payload &trans)
  {
    /* create extension object */
    trans.set_extension(new vpcl::tlm_if_extension());
  }

  virtual void ini_init_ext(tlm::tlm_generic_payload &trans)
  {
    vpcl::tlm_if_extension *p_ext = trans.get_extension<vpcl::tlm_if_extension>();
    if (p_ext != NULL) {
      /* initialize extension attributes */
      p_ext->init();
    }
    else {
      /* create and initialize extension (fail safe) */
      trans.set_extension(new vpcl::tlm_if_extension());
      trans.get_extension<vpcl::tlm_if_extension>()->init();
    }
  }

  bool ini_acc_sub(tlm::tlm_command command, sc_dt::uint64 address, unsigned char *data_ptr, unsigned int data_length, vpcl::tlm_if_extension *p_ext, bool debug)
  {
    /* acquire transaction object and set transaction attributes */
    tlm::tlm_generic_payload *p_trans = ini_acquire_gp(command, address, data_ptr, data_length);
    if (p_trans == NULL) {
      std::cout << "fatal : acquired transaction pointer \"p_trans\" is null." << std::endl;
      sc_stop();
    }
    vpcl::tlm_if_extension *p_trans_ext = p_trans->get_extension<vpcl::tlm_if_extension>();
    if ((p_trans_ext != NULL) && (p_ext != NULL)) {
      p_trans_ext->copy_from(*p_ext);
    }

    /* call method */
    if (! debug) {
      ini_acc(*p_trans);
    }
    else {
      ini_acc_dbg(*p_trans);
    }

    /* get extension information */
    if ((p_ext != NULL) && (p_trans_ext != NULL)) {
      p_ext->copy_from(*p_trans_ext);
    }
    return p_trans->is_response_ok();
  }

  void ini_check_gp_attribute(tlm::tlm_generic_payload &trans, bool debug)
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

    /* in the case of big endian */
    if (m_param.endian_big) {
      sc_dt::uint64 address = trans.get_address();
      unsigned int data_length = trans.get_data_length();
      unsigned int bus_width = BUSWIDTH / 8;

      /* flip address to convert little to big endian */
      if ((data_length > 0) && (data_length < bus_width) && (((address % bus_width) + data_length) <= bus_width)) {
        trans.set_address(address ^ (bus_width - (data_length % bus_width)));
      }
    }

    /* attach extension information */
    vpcl::tlm_if_extension *p_ext = trans.get_extension<vpcl::tlm_if_extension>();
    if (p_ext != NULL) {
      p_ext->ini_if_use = true;
      p_ext->ini_if_param = m_param;
      p_ext->tgt_if_use = false;
      p_ext->tgt_if_param.init();
    }
  }

  sc_dt::uint64 floor64(sc_dt::uint64 value, unsigned int width)
  {
    if (width == 0) {
      return value;
    }
    return (value / width) * width;
  }

  unsigned int ceil32(unsigned int value, unsigned int width)
  {
    if (width == 0) {
      return value;
    }
    return ((value + width -1) / width) * width;
  }

  tlm::tlm_generic_payload *create_aligned_gp(tlm::tlm_generic_payload &trans, unsigned int width, unsigned int length)
  {
    /* non-alignment case */
    if ((width == 0) || (length == 0) || (length < width) || ((length % width) != 0)) {
      return &trans;
    }

    /* get aligned attributes */
    sc_dt::uint64 address = floor64(trans.get_address(), width);
    unsigned int offset = trans.get_address() % width;
    unsigned int data_length = ceil32(trans.get_data_length() + offset, length);
    unsigned char *data_ptr = new unsigned char [data_length];
    sc_assert(data_ptr != NULL);
    memset(data_ptr, 0, data_length);
    memcpy(data_ptr + offset, trans.get_data_ptr(), trans.get_data_length());
    unsigned int byte_enable_length = data_length;
    unsigned char *byte_enable_ptr = new unsigned char [byte_enable_length];
    sc_assert(byte_enable_ptr != NULL);
    memset(byte_enable_ptr, 0, byte_enable_length);
    if (trans.get_byte_enable_ptr() == NULL) {
      memset(byte_enable_ptr + offset, 0xff, trans.get_data_length());
    }
    else {
      if (trans.get_data_length() <= trans.get_byte_enable_length()) {
        memcpy(byte_enable_ptr + offset, trans.get_byte_enable_ptr(), trans.get_data_length());
      }
      else {
        for (unsigned int i = 0; i < trans.get_data_length(); i++) {
          byte_enable_ptr[offset + i] = trans.get_byte_enable_ptr()[i % trans.get_byte_enable_length()];
        }
      }
    }
    unsigned int streaming_width = data_length;

    /* create aligned generic payload */
    tlm::tlm_generic_payload *p_trans = ini_acquire_gp();
    sc_assert(p_trans != NULL);
    p_trans->set_command(trans.get_command()); /* unchanged */
    p_trans->set_address(address);
    p_trans->set_data_ptr(data_ptr);
    p_trans->set_data_length(data_length);
    p_trans->set_byte_enable_ptr(byte_enable_ptr);
    p_trans->set_byte_enable_length(byte_enable_length);
    p_trans->set_streaming_width(streaming_width);
    p_trans->update_original_from(trans);      /* unchanged */
    p_trans->update_extensions_from(trans);    /* unchanged */

    return p_trans;
  }

  void create_gp_clones(tlm::tlm_generic_payload *p_trans, unsigned int width, unsigned int length, std::list<tlm::tlm_generic_payload *> &p_clones)
  {
    /* check error */
    sc_assert(p_trans != NULL);

    /* non-alignment case */
    if ((width == 0) || (length == 0) || (length < width) || ((length % width) != 0)) {
      length = p_trans->get_data_length();
    }

    /* create clones of generic payload */
    p_clones.clear();
    for (unsigned int i = 0; i < (p_trans->get_data_length() / length); i++) {
      tlm::tlm_generic_payload *p_clone = ini_acquire_gp();
      p_clone->set_command(p_trans->get_command()); /* unchanged */
      p_clone->set_address(p_trans->get_address() + (length * i));
      p_clone->set_data_ptr(p_trans->get_data_ptr() + (length * i));
      p_clone->set_data_length(length);
      p_clone->set_byte_enable_ptr(p_trans->get_byte_enable_ptr() + (length * i));
      p_clone->set_byte_enable_length(length);
      p_clone->set_streaming_width(length);
      p_clone->update_original_from(*p_trans);      /* unchanged */
      p_clone->update_extensions_from(*p_trans);    /* unchanged */
      p_clones.push_back(p_clone);
    }
  }

  void destroy_aligned_gp(tlm::tlm_generic_payload &trans, tlm::tlm_generic_payload *p_trans)
  {
    /* check error */
    sc_assert(p_trans != NULL);

    /* non-alignment case */
    if (p_trans == &trans) {
      return;
    }

    /* update attributes from aligned generic payload */
    if (p_trans->is_read()) {
      memcpy(trans.get_data_ptr(), p_trans->get_data_ptr() + (trans.get_address() - p_trans->get_address()), trans.get_data_length());
    }
    trans.update_original_from(*p_trans);
    trans.update_extensions_from(*p_trans);

    /* destroy aligned generic payload */
    m_release_peq.notify(*p_trans);
  }

  void ini_acc_dmi(tlm::tlm_generic_payload &trans, sc_time &t)
  {
    /* (0) check updating of dmi size */
    while (m_dmi_list.size() > m_param.dmi_size) {
      m_dmi_list.pop_back();
      dbg_printf(TLM_IF_DBGH_LV, "%9s : DM POP  %d\n", sc_time_stamp().to_string().c_str(), m_dmi_list.size());
    }

    /* (1) check permission of dmi access */
    vpcl::dmi_memory *p_dmi_mem = NULL;
    for (std::list<vpcl::dmi_memory>::iterator it = m_dmi_list.begin(); it != m_dmi_list.end(); it++) {
      if (it->is_address_dmi(trans)) {
        p_dmi_mem = &(*it);
        break;
      }
    }

    /* (2-a) call b_transport */
    if (p_dmi_mem == NULL) {
      sc_dt::uint64 tmp_addr = trans.get_address();
      if (m_param.info_displayed) {
        this->store_b_transport_info(&trans, t);
      }
      m_ini_socket->b_transport(trans, t);
      if (m_param.info_displayed) {
        this->print_b_transport_info(&trans, t);
      }
      if (m_param.msg_out_lvl >= TLM_IF_DBGL_LV) {
        printf("%9s : %2s (%08X)", sc_time_stamp().to_string().c_str(), trans.is_write() ? "BW" : "BR", (unsigned int) trans.get_address());
        for (unsigned int i = 0; (i < trans.get_data_length()) && (i < 64); i++) {
          printf("%s%02X", (i % 4) ? "" : " ", *(trans.get_data_ptr() + i));
        }
        printf("\n");
      }

      /* (3) call get_direct_mem_ptr */
      trans.set_address(tmp_addr);
      tlm::tlm_dmi dmi_data;
      bool dmi_enable = m_ini_socket->get_direct_mem_ptr(trans, dmi_data);

      /* (4) check dmi allowed or not with get_direct_mem_ptr */
      if (dmi_enable) {

        /* (5) save dmi properties to temporary buffer (m_dmi_list) */
        while (m_dmi_list.size() >= m_param.dmi_size) {
          m_dmi_list.pop_back();
          dbg_printf(TLM_IF_DBGH_LV, "%9s : DM POP  %d\n", sc_time_stamp().to_string().c_str(), m_dmi_list.size());
        }
        vpcl::dmi_memory dmi_mem;
        dmi_mem.load_dmi_ptr(dmi_data);
        m_dmi_list.push_front(dmi_mem);
        dbg_printf(TLM_IF_DBGH_LV, "%9s : DM PUSH %d\n", sc_time_stamp().to_string().c_str(), m_dmi_list.size());
      }
    }

    /* (2-b) execute dmi access */
    else {
      p_dmi_mem->operation(trans, t);
      if (m_param.msg_out_lvl >= TLM_IF_DBGL_LV) {
        printf("%9s : %2s (%08X)", sc_time_stamp().to_string().c_str(), trans.is_write() ? "DW" : "DR", (unsigned int) trans.get_address());
        for (unsigned int i = 0; (i < trans.get_data_length()) && (i < 64); i++) {
          printf("%s%02X", (i % 4) ? "" : " ", *(trans.get_data_ptr() + i));
        }
        printf("\n");
      }
    }
  }

  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
  {
    std::list<vpcl::dmi_memory>::iterator it = m_dmi_list.begin();
    while (it != m_dmi_list.end()) {
      if (it->invalidate_dmi(start_range, end_range)) {
        it = m_dmi_list.erase(it);
        dbg_printf(TLM_IF_DBGH_LV, "%9s : DM POP  %d\n", sc_time_stamp().to_string().c_str(), m_dmi_list.size());
      }
      else {
        it++;
      }
    }
  }

  virtual void ini_req_free(tlm::tlm_generic_payload &trans)
  {
    /* do nothing */
  }

  virtual void ini_resp_rx(tlm::tlm_generic_payload &trans, sc_time &t)
  {
    /* resume request method */
    tlm::tlm_generic_payload *p_origin = this->get_origin(&trans);
    sc_assert(p_origin != NULL);
    this->get_resp_rx_event(p_origin).notify(t);

    /* update transaction attributes of the original generic payload */
    if (! trans.is_dmi_allowed()) {
      p_origin->set_dmi_allowed(false);
    }
    if (trans.is_response_error()) {
      p_origin->set_response_status(trans.get_response_status());
    }
    p_origin->update_extensions_from(trans);

    this->get_clones(p_origin).remove(&trans);
    this->set_origin(&trans, NULL);
  }

  void ini_req_thread(void)
  {
    tlm::tlm_generic_payload *p_trans = NULL;
    tlm::tlm_sync_enum sync = tlm::TLM_ACCEPTED;
    tlm::tlm_phase phase = tlm::UNINITIALIZED_PHASE;
    tlm::tlm_phase prev_phase = tlm::UNINITIALIZED_PHASE;
    sc_time t = SC_ZERO_TIME;
    sc_time prev_t = SC_ZERO_TIME;

    while (true) {
      /* wait next transaction */
      wait(m_req_thread_peq.get_event());
      while ((p_trans = m_req_thread_peq.get_next_transaction()) != NULL) {

        /* check and set transaction attributes */
        ini_check_gp_attribute(*p_trans, false);

        /* matsuo legacy */
        ini_dump_log(*p_trans);
        ini_add_latency_log(*p_trans);

        /* [a] : nb_transport_fw/BEGIN_REQ */
        phase = prev_phase = tlm::BEGIN_REQ;
        t = prev_t = SC_ZERO_TIME;
        sync = m_ini_socket->nb_transport_fw(*p_trans, phase, t);
        this->set_history(p_trans, "nb_transport_fw", prev_phase, prev_t, sync, phase, t);

        /* (a1) : TLM_ACCEPTED */
        if ((sync == tlm::TLM_ACCEPTED) && (phase == tlm::BEGIN_REQ)) {
          gp_assert(t == prev_t, p_trans,
            "TMG : returned timing annotation (%s) must be equal to called value (%s).", t.to_string().c_str(), prev_t.to_string().c_str());

          /* wait next phase transition */
          wait(this->get_end_req_event(p_trans));
          if (this->get_phase(p_trans) == tlm::END_REQ) {
            if (! this->is_resp_allowed(p_trans)) {
              this->unlock_resp_allowed(p_trans);
            }
            else {
              /* activate end response method */
              phase = tlm::END_RESP;
              m_resp_method_peq.notify(*p_trans, phase);
            }
          }
          /* be necessarily END_RESP */
          else {
            /* activate end response method */
            phase = tlm::END_RESP;
            t = ini_resp_tp3(*p_trans);
            m_resp_method_peq.notify(*p_trans, phase, t);
          }
        }

        /* (a2) : TLM_UPDATED/END_REQ */
        else if ((sync == tlm::TLM_UPDATED) && (phase == tlm::END_REQ)) {
          gp_assert(t >= prev_t, p_trans,
            "TMG : returned timing annotation (%s) must be greater than or equal to called value (%s).", t.to_string().c_str(), prev_t.to_string().c_str());

          /* wait request accept delay with lock response */
          this->lock_resp_allowed(p_trans, t);
          wait_time(t);
          if (! this->is_resp_allowed(p_trans)) {
            this->unlock_resp_allowed(p_trans);
          }
          else {
            /* activate end response method */
            phase = tlm::END_RESP;
            m_resp_method_peq.notify(*p_trans, phase);
          }
        }

        /* (a3) : TLM_UPDATED/BEGIN_RESP */
        else if ((sync == tlm::TLM_UPDATED) && (phase == tlm::BEGIN_RESP)) {
          gp_assert(p_trans->get_response_status() == tlm::TLM_OK_RESPONSE, p_trans,
            "STS : returned response status (%s) must be TLM_OK_RESPONSE.", p_trans->get_response_string().c_str());
          gp_assert(t >= prev_t, p_trans,
            "TMG : returned timing annotation (%s) must be greater than or equal to called value (%s).", t.to_string().c_str(), prev_t.to_string().c_str());

          /* wait request accept delay */
          wait_time(t);

          /* activate begin response method */
          phase = tlm::BEGIN_RESP;
          m_resp_method_peq.notify(*p_trans, phase);
        }

        /* (a4) : TLM_COMPLETED */
        else if (sync == tlm::TLM_COMPLETED) {
          gp_assert(p_trans->get_response_status() == tlm::TLM_OK_RESPONSE, p_trans,
            "STS : returned response status (%s) must be TLM_OK_RESPONSE.", p_trans->get_response_string().c_str());
          gp_assert(t >= prev_t, p_trans,
            "TMG : returned timing annotation (%s) must be greater than or equal to called value (%s).", t.to_string().c_str(), prev_t.to_string().c_str());

          /* wait request accept delay */
          wait_time(t);

          /* activate response method */
          phase = tlm::END_RESP;
          t = ini_resp_tp3(*p_trans);
          m_resp_method_peq.notify(*p_trans, phase, t);
        }

        /* invalid phase transition */
        else {
          gp_assert(false, p_trans,
            "PHS : returned phase transition (%s/%s) must be TLM_ACCEPTED/BEGIN_REQ, TLM_UPDATED/END_REQ, TLM_UPDATED/BEGIN_RESP, or TLM_COMPLETED.", this->to_string(sync).c_str(), this->to_string(phase).c_str());
        }

        /* callback method */
        ini_req_free(*p_trans);

        /* end of request thread */
        p_trans = NULL;
      }
    }
  }

  void ini_resp_method(tlm::tlm_generic_payload &trans, const tlm::tlm_phase &resp_phase)
  {
    /* begin response method */
    if (resp_phase == tlm::BEGIN_RESP) {
      tlm::tlm_sync_enum sync = tlm::TLM_ACCEPTED;
      tlm::tlm_phase phase = tlm::UNINITIALIZED_PHASE;
      tlm::tlm_phase prev_phase = tlm::UNINITIALIZED_PHASE;
      sc_time t = SC_ZERO_TIME;
      sc_time prev_t = SC_ZERO_TIME;

      phase = prev_phase = tlm::END_RESP;
      t = prev_t = ini_resp_tp3(trans) + ini_resp_t3(trans);
      sync = m_ini_socket->nb_transport_fw(trans, phase, t);
      this->set_history(&trans, "nb_transport_fw", prev_phase, prev_t, sync, phase, t);
      gp_assert(trans.get_response_status() == tlm::TLM_OK_RESPONSE, &trans,
        "STS : returned response status (%s) must be TLM_OK_RESPONSE.", trans.get_response_string().c_str());

      /* (d10) : TLM_ACCEPTED */
      if ((sync == tlm::TLM_ACCEPTED) && (phase == tlm::END_RESP)) {
        gp_assert(t == prev_t, &trans,
          "TMG : returned timing annotation (%s) must be equal to called value (%s).", t.to_string().c_str(), prev_t.to_string().c_str());
      }

      /* (d11) : TLM_COMPLETED */
      else if (sync == tlm::TLM_COMPLETED) {
        gp_assert(t >= prev_t, &trans,
          "TMG : returned timing annotation (%s) must be greater than or equal to called value (%s).", t.to_string().c_str(), prev_t.to_string().c_str());
      }

      /* invalid phase transition */
      else {
        gp_assert(false, &trans,
          "PHS : returned phase transition (%s/%s) must be TLM_ACCEPTED/END_RESP or TLM_COMPLETED.", this->to_string(sync).c_str(), this->to_string(phase).c_str());
      }

      /* activate end response method */
      phase = tlm::END_RESP;
      t -= ini_resp_t3(trans);
      m_resp_method_peq.notify(trans, phase, t);
    }

    /* end response method */
    else if (resp_phase == tlm::END_RESP) {
      /* wait end of request with no time consumption */
      if (! this->is_resp_allowed(&trans)) {
        this->unlock_resp_allowed(&trans);
        return;
      }
      sc_time t3 = ini_resp_t3(trans);

      /* matsuo legacy */
      ini_add_latency_log(trans);

      /* callback method */
      ini_resp_rx(trans, t3);

      /* release transaction object */
      m_release_peq.notify(trans, t3);
    }

    /* in the case there are any errors due to the coding or specification  */
    else {
      sc_assert(false);
    }
  }

  tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t)
  {
    tlm::tlm_sync_enum sync = tlm::TLM_ACCEPTED;
    tlm::tlm_phase prev_phase = phase;
    sc_time prev_t = t;

    gp_assert(this->is_entried(&trans), &trans,
      "OBJ : called transaction object (not entried) must be entried in memory manager.");

    /* [b] : nb_transport_bw/END_REQ */
    if (prev_phase == tlm::END_REQ) {
      gp_assert(this->get_phase(&trans) == tlm::BEGIN_REQ, &trans,
        "PHS : called phase transition (%s->END_REQ) must be BEGIN_REQ->END_REQ.", this->to_string(this->get_phase(&trans)).c_str());

      /* resume request thread with lock response */
      this->lock_resp_allowed(&trans, prev_t);
      this->get_end_req_event(&trans).notify(prev_t);

      /* (b5) : TLM_ACCEPTED */
      sync = tlm::TLM_ACCEPTED;
      this->set_history(&trans, "nb_transport_bw", prev_phase, prev_t, sync, phase, t);
    }

    /* [c] : nb_transport_bw/BEGIN_RESP */
    else if (prev_phase == tlm::BEGIN_RESP) {
      gp_assert(trans.get_response_status() == tlm::TLM_OK_RESPONSE, &trans,
        "STS : called response status (%s) must be TLM_OK_RESPONSE.", trans.get_response_string().c_str());

      if (this->get_phase(&trans) == tlm::BEGIN_REQ) {
        /* resume request thread */
        this->get_end_req_event(&trans).notify(prev_t);
      }
      else if (this->get_phase(&trans) == tlm::END_REQ) {
        gp_assert(this->will_be_resp_allowed(&trans, prev_t), &trans,
          "RSP : response transition timing (%s) must be greater than or equal to request ending time (%s).", (sc_time_stamp() + prev_t).to_string().c_str(), this->get_resp_allowed_time(&trans).to_string().c_str());

        /* activate response thread */
        tlm::tlm_phase tmp_phase = tlm::END_RESP;
        sc_time tmp_t = prev_t + ini_resp_tp3(trans);
        m_resp_method_peq.notify(trans, tmp_phase, tmp_t);
      }
      else {
        gp_assert(false, &trans,
          "PHS : called phase transition (%s->BEGIN_RESP) must be BEGIN_REQ->BEGIN_RESP or END_REQ->BEGIN_RESP.", this->to_string(this->get_phase(&trans)).c_str());
      }

      /* (c8) : TLM_UPDATED/END_RESP | (c9) : TLM_COMPLETED */
      gp_assert(m_param.bw_resp_sync != tlm::TLM_ACCEPTED, &trans,
        "PRM : parameter bw_resp_sync (%s) must be TLM_UPDATED or TLM_COMPLETED.", this->to_string(m_param.bw_resp_sync).c_str());
      sync = m_param.bw_resp_sync;
      phase = tlm::END_RESP;
      t += ini_resp_tp3(trans) + ini_resp_t3(trans);
      this->set_history(&trans, "nb_transport_bw", prev_phase, prev_t, sync, phase, t);
    }

    /* invalid phase transition */
    else {
      gp_assert(false, &trans,
        "PHS : called phase transition (%s->%s) must be BEGIN_REQ->END_REQ, BEGIN_REQ->BEGIN_RESP, or END_REQ->BEGIN_RESP.", this->to_string(this->get_phase(&trans)).c_str(), this->to_string(prev_phase).c_str());
    }

    return sync;
  }

  sc_time ini_resp_t3(tlm::tlm_generic_payload &trans)
  {
    sc_time t = SC_ZERO_TIME;

    if (trans.is_write()) {
      t = m_param.bus_clk;
    }
    else if (trans.is_read()) {
      t = m_param.bus_clk * (double) ((trans.get_data_length() + (BUSWIDTH / 8) - 1) / (BUSWIDTH / 8));
    }
    /* be necessarily TLM_IGNORE_COMMAND */
    else {
      /* do nothing */
    }
    return t;
  }

  sc_time ini_resp_tp3(tlm::tlm_generic_payload &trans)
  {
    sc_time t = SC_ZERO_TIME;

    if (trans.is_write()) {
      t = m_param.wr_resp_latency;
    }
    else if (trans.is_read()) {
      t = m_param.rd_resp_latency;
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

  /* Matsuo Legacy */
  void ini_dump_log(tlm::tlm_generic_payload &trans)
  {
    if (m_param.p_log_file != NULL) {
      if ((trans.is_write() && (m_param.wr_log == true)) || (trans.is_read() && (m_param.rd_log == true))) {
        vpcl::tlm_if_extension *p_ext = NULL;
        trans.get_extension(p_ext);
        unsigned int src = (p_ext != NULL) ? p_ext->ini_if_param.src_id : 0;

        fprintf(m_param.p_log_file, "%8s :  [%-8s]  I(%02X)-> T  ", sc_time_stamp().to_string().c_str(), m_param.name.c_str(), src);
        fprintf(m_param.p_log_file, "%s  (%08X)  ", (trans.is_write() ? "WR" : "RD"), (unsigned int) trans.get_address());
        if ((trans.get_data_ptr() == NULL) || (trans.get_data_length() == 0)) {
          fprintf(m_param.p_log_file, "NO DATA");
        }
        else {
          for (unsigned int i = 0; (i < trans.get_data_length()) && (i < 128); i++) {
            if (trans.is_write()) {
              fprintf(m_param.p_log_file, "%02X%s", *(trans.get_data_ptr() + i), (((i % 4) == 3) ? " " : ""));
            }
            else {
              fprintf(m_param.p_log_file, "--%s", (((i % 4) == 3) ? " " : ""));
            }
          }
        }
        fprintf(m_param.p_log_file, "\n");
        fflush(m_param.p_log_file);
      }
    }
    else {
    }
  }

  /* Matsuo Legacy */
  void ini_add_latency_log(tlm::tlm_generic_payload &trans)
  {
    if (m_param.latency_log) {
      vpcl::tlm_if_extension *p_ext = trans.get_extension<vpcl::tlm_if_extension>();
      if (p_ext != NULL) {
        p_ext->latency_log_vector.push_back(vpcl::tlm_if_latency_log(m_name.c_str(), sc_time_stamp()));
      }
    }
  }

  /* Matsuo Legacy */
  int dbg_printf(int dbglevel, const char *format, ... )
  {
    va_list arg;
    int rc;

    if ( format == NULL ) {
      printf( "ERROR(TLM_COMMON_IF) : dbg_printf module get NULL pointer.\n" );
      sc_stop();
    }

    if (dbglevel <= m_param.msg_out_lvl) {
      va_start(arg, format);
      if ( arg == NULL ) {
        printf( "ERROR(TLM_COMMON_IF) : dbg_printf module get no argument.\n" );
        sc_stop();
      }
      rc = vprintf(format, arg);
      va_end(arg);
      return rc;
    }
    return 0;
  }
};

/* Class of TLM Initiator Thread */
template <unsigned int BUSWIDTH, typename TYPES, unsigned int N>
class tlm_ini_thread : public sc_module
{
  /* Member Data */
  tlm_ini_if<BUSWIDTH, TYPES, N> &m_ini_if;

public:
  SC_HAS_PROCESS(tlm_ini_thread);

  /* Constructor */
  tlm_ini_thread(sc_module_name name, tlm_ini_if<BUSWIDTH, TYPES, N> &ini_if)
    : sc_module(name)
    , m_ini_if(ini_if)
  {
    SC_THREAD(activate_ini_req_thread);
    SC_THREAD(activate_ini_release_gp_thread);
  }

  /* Destructor */
  ~tlm_ini_thread()
  {
    /* do nothing */
  }

private:
  /* Thread Activator */
  void activate_ini_req_thread()
  {
    m_ini_if.ini_req_thread();
  }

  void activate_ini_release_gp_thread()
  {
    m_ini_if.ini_release_gp_thread();
  }
};

}

#endif
