// $Id: tlm_if.h,v 1.21.2.1 2016/08/11 02:42:40 imotoas Exp $

/*************************************************************************
 *
 *  tlm_if.h
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

#ifndef __TLM_IF_H__
#define __TLM_IF_H__

#include "systemc.h"
#include "tlm.h"
#include <cstdio>
#include <cstdarg>
#include <map>
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <iomanip>

namespace vpcl
{

typedef enum tlm_if_access
{
  TLM_IF_LT_ACCESS,
  TLM_IF_AT_ACCESS
} tlm_if_access;

typedef enum tlm_if_phase /* unsupported */
{
  TLM_IF_SINGLE_PHASE,
  TLM_IF_MULTI_PHASE
} tlm_if_phase;

typedef enum tlm_if_msg_lv /* unsupported */
{
  TLM_IF_NO_OUT  = 0,
  TLM_IF_ERR_LV  = 1,
  TLM_IF_WAR_LV  = 2,
  TLM_IF_NML_LV  = 3,
  TLM_IF_DBGL_LV = 4,
  TLM_IF_DBGH_LV = 5
} tlm_if_msg_lv;

typedef enum tlmcc_exception_enum
{
  TLMCC_GENERIC_EXCEPTION,
  TLMCC_RESET_EXCEPTION
} tlmcc_exception_enum;

class tlm_ini_if_parameter
{
public:
  std::string name;
  unsigned int bus_width; /* byte */
  sc_time bus_clk;
  sc_time bus_gnt; /* unsupported */
  sc_time bus_rgnt; /* unsupported */
  unsigned int buf_size; /* unsupported */
  unsigned int src_id;
  unsigned int burst_width;
  unsigned int burst_length;
  vpcl::tlm_if_access access_mode;
  FILE *p_log_file;
  bool wr_log;
  bool rd_log;
  bool latency_log;
  vpcl::tlm_if_msg_lv msg_out_lvl; /* unsupported */
  bool endian_big;
  unsigned int dmi_size;
  tlm::tlm_sync_enum bw_resp_sync;
  sc_time wr_resp_latency;
  sc_time rd_resp_latency;
  bool info_displayed;
  class outstanding_config {
    unsigned int m_max;
    unsigned int m_count;
    std::list<tlm::tlm_generic_payload *> m_queue;
  public:
    outstanding_config() : m_max(UINT_MAX), m_count(0) {
    }
    void set_max(unsigned int max) {
      sc_assert(max > 0);
      m_max = max;
    }
    unsigned int get_count() const {
      return m_count;
    }
    bool count_is_max() const {
      return (m_count >= m_max);
    }
    void queue_push(tlm::tlm_generic_payload *p_trans) {
      sc_assert(p_trans != NULL);
      m_queue.push_back(p_trans);
    }
    void count_increment() {
      sc_assert(m_count < UINT_MAX);
      m_count++;
    }
    bool queue_is_empty() const {
      return m_queue.empty();
    }
    void count_decrement() {
      sc_assert(m_count > 0);
      m_count--;
    }
    tlm::tlm_generic_payload *queue_pop() {
      sc_assert(! m_queue.empty());
      tlm::tlm_generic_payload *p_trans = m_queue.front();
      m_queue.pop_front();
      return p_trans;
    }
  };
  std::map<unsigned int, outstanding_config> m_outstanding_configs;

  tlm_ini_if_parameter(std::string arg_name = "UNKNOWN", unsigned int arg_bus_width = 0)
    : name(arg_name)
    , bus_width(arg_bus_width) /* byte */
    , bus_clk(SC_ZERO_TIME)
    , bus_gnt(SC_ZERO_TIME) /* unsupported */
    , bus_rgnt(SC_ZERO_TIME) /* unsupported */
    , buf_size(1) /* unsupported */
    , src_id(0)
    , burst_width(arg_bus_width)
    , burst_length(0)
    , access_mode(vpcl::TLM_IF_LT_ACCESS)
    , p_log_file(stdout)
    , wr_log(false)
    , rd_log(false)
    , latency_log(false)
    , msg_out_lvl(vpcl::TLM_IF_ERR_LV) /* unsupported */
#ifdef IS_MODELED_ENDIAN_BIG
    , endian_big(true)
#else
    , endian_big(false)
#endif
    , dmi_size(2)
    , bw_resp_sync(tlm::TLM_COMPLETED)
    , wr_resp_latency(SC_ZERO_TIME)
    , rd_resp_latency(SC_ZERO_TIME)
    , info_displayed(false)
  {
    /* do nothing */
  }

  void init(std::string arg_name = "UNKNOWN", unsigned int arg_bus_width = 0)
  {
    name = arg_name;
    bus_width = arg_bus_width; /* byte */
    bus_clk = SC_ZERO_TIME;
    bus_gnt = SC_ZERO_TIME; /* unsupported */
    bus_rgnt = SC_ZERO_TIME;/* unsupported */
    buf_size = 1; /* unsupported */
    src_id = 0;
    burst_width = arg_bus_width;
    burst_length = 0;
    access_mode = vpcl::TLM_IF_LT_ACCESS;
    p_log_file = stdout;
    wr_log = false;
    rd_log = false;
    latency_log = false;
    msg_out_lvl = vpcl::TLM_IF_ERR_LV; /* unsupported */
#ifdef IS_MODELED_ENDIAN_BIG
    endian_big = true;
#else
    endian_big = false;
#endif
    dmi_size = 2;
    bw_resp_sync = tlm::TLM_COMPLETED;
    wr_resp_latency = SC_ZERO_TIME;
    rd_resp_latency = SC_ZERO_TIME;
    info_displayed = false;
  }
};
typedef tlm_ini_if_parameter tlm_if_ini_parameter;

class tlm_tgt_if_parameter
{
public:
  std::string name;
  unsigned int bus_width; /* byte */
  sc_time bus_clk;
  sc_time bus_gnt; /* unsupported */
  sc_time bus_rgnt; /* unsupported */
  unsigned int buf_size; /* unsupported */
  sc_time wr_latency;
  sc_time rd_latency;
  vpcl::tlm_if_phase phase_mode; /* unsupported */
  FILE *p_log_file;
  bool wr_log;
  bool rd_log;
  vpcl::tlm_if_msg_lv msg_out_lvl; /* unsupported */
  tlm::tlm_phase fw_req_phase;
  sc_time wr_req_latency;
  sc_time rd_req_latency;
  bool info_displayed;

  tlm_tgt_if_parameter(std::string arg_name = "UNKNOWN", unsigned int arg_bus_width = 0)
    : name(arg_name)
    , bus_width(arg_bus_width) /* byte */
    , bus_clk(SC_ZERO_TIME)
    , bus_gnt(SC_ZERO_TIME) /* unsupported */
    , bus_rgnt(SC_ZERO_TIME) /* unsupported */
    , buf_size(1) /* unsupported */
    , wr_latency(SC_ZERO_TIME)
    , rd_latency(SC_ZERO_TIME)
    , phase_mode(vpcl::TLM_IF_SINGLE_PHASE) /* unsupported */
    , p_log_file(stdout)
    , wr_log(false)
    , rd_log(false)
    , msg_out_lvl(vpcl::TLM_IF_ERR_LV) /* unsupported */
    , fw_req_phase(tlm::END_RESP)
    , wr_req_latency(SC_ZERO_TIME)
    , rd_req_latency(SC_ZERO_TIME)
    , info_displayed(false)
  {
    /* do nothing */
  }

  void init(std::string arg_name = "UNKNOWN", unsigned int arg_bus_width = 0)
  {
    name = arg_name;
    bus_width = arg_bus_width; /* byte */
    bus_clk = SC_ZERO_TIME;
    bus_gnt = SC_ZERO_TIME; /* unsupported */
    bus_rgnt = SC_ZERO_TIME; /* unsupported */
    buf_size = 1; /* unsupported */
    wr_latency = SC_ZERO_TIME;
    rd_latency = SC_ZERO_TIME;
    phase_mode = vpcl::TLM_IF_SINGLE_PHASE; /* unsupported */
    p_log_file = stdout;
    wr_log = false;
    rd_log = false;
    msg_out_lvl = vpcl::TLM_IF_ERR_LV; /* unsupported */
    fw_req_phase = tlm::END_RESP;
    wr_req_latency = SC_ZERO_TIME;
    rd_req_latency = SC_ZERO_TIME;
    info_displayed = false;
  }
};
typedef tlm_tgt_if_parameter tlm_if_tgt_parameter;

class bus_extension
{
public:
  int pri;
  bool lck;
  bool r_lck;

  bus_extension()
    : pri(-1)
    , lck(false)
    , r_lck(false)
  {
    /* do nothing */
  }

  void init()
  {
    pri = -1;
    lck = false;
    r_lck = false;
  }
};

class tlm_if_latency_log
{
public:
  std::string name;
  sc_time time;

  tlm_if_latency_log(std::string iname, sc_time itime)
    : name(iname)
    , time(itime)
  {
    /* do nothing */
  }
};

class tlm_if_extension : public tlm::tlm_extension<tlm_if_extension>
{
public:
  bool ini_if_use;
  tlm_ini_if_parameter ini_if_param;
  bool tgt_if_use;
  tlm_tgt_if_parameter tgt_if_param;
  unsigned int trans_id;
  void *p_user_ext;
  bus_extension *p_bus_ext;
  std::vector<tlm_if_latency_log> latency_log_vector;
  unsigned int address_alignment;
  unsigned int data_length_alignment;
  unsigned int outstanding_id;

  tlm_if_extension()
    : ini_if_use(false)
    , ini_if_param()
    , tgt_if_use(false)
    , tgt_if_param()
    , trans_id(0)
    , p_user_ext(NULL)
    , p_bus_ext(NULL)
    , address_alignment(0)
    , data_length_alignment(0)
    , outstanding_id(0)
  {
    p_bus_ext = new vpcl::bus_extension();
  }

  virtual ~tlm_if_extension()
  {
    if (p_bus_ext != NULL) {
      delete p_bus_ext;
    }
  }

  /* this method is called from deep_copy_from method */
  tlm::tlm_extension_base *clone() const
  {
    tlm_if_extension *p_ext = new tlm_if_extension();
    if (p_ext != NULL) {
      p_ext->ini_if_use = ini_if_use;
      p_ext->ini_if_param = ini_if_param;
      p_ext->tgt_if_use = tgt_if_use;
      p_ext->tgt_if_param = tgt_if_param;
      p_ext->trans_id = trans_id;
      p_ext->p_user_ext = p_user_ext;
      if ((p_ext->p_bus_ext != NULL) && (p_bus_ext != NULL)) {
        *p_ext->p_bus_ext = *p_bus_ext;
      }
      p_ext->latency_log_vector = latency_log_vector;
      p_ext->address_alignment = address_alignment;
      p_ext->data_length_alignment = data_length_alignment;
      p_ext->outstanding_id = outstanding_id;
    }
    return static_cast<tlm::tlm_extension_base *>(p_ext);
  }

  /* this method is called from update_extensions_from method */
  void copy_from(const tlm::tlm_extension_base &ext)
  {
    ini_if_use = static_cast<tlm_if_extension const &>(ext).ini_if_use;
    ini_if_param = static_cast<tlm_if_extension const &>(ext).ini_if_param;
    tgt_if_use = static_cast<tlm_if_extension const &>(ext).tgt_if_use;
    tgt_if_param = static_cast<tlm_if_extension const &>(ext).tgt_if_param;
    trans_id = static_cast<tlm_if_extension const &>(ext).trans_id;
    p_user_ext = static_cast<tlm_if_extension const &>(ext).p_user_ext;
    if ((p_bus_ext != NULL) && (static_cast<tlm_if_extension const &>(ext).p_bus_ext != NULL)) {
      *p_bus_ext = *static_cast<tlm_if_extension const &>(ext).p_bus_ext;
    }
    latency_log_vector = static_cast<tlm_if_extension const &>(ext).latency_log_vector;
    address_alignment = static_cast<tlm_if_extension const &>(ext).address_alignment;
    data_length_alignment = static_cast<tlm_if_extension const &>(ext).data_length_alignment;
    outstanding_id = static_cast<tlm_if_extension const &>(ext).outstanding_id;
  }

  void init()
  {
    ini_if_use = false;
    ini_if_param.init();
    tgt_if_use = false;
    tgt_if_param.init();
    trans_id = 0;
    p_user_ext = NULL;
    if (p_bus_ext != NULL) {
      p_bus_ext->init();
    }
    latency_log_vector.clear();
    address_alignment = 0;
    data_length_alignment = 0;
    outstanding_id = 0;
  }
};

class dmi_memory
{
private:
  unsigned char              *m_dmi_ptr;
  sc_dt::uint64               m_dmi_start_address;
  sc_dt::uint64               m_dmi_end_address;
  tlm::tlm_dmi::dmi_access_e  m_dmi_granted_access;
  sc_core::sc_time            m_dmi_read_latency;
  sc_core::sc_time            m_dmi_write_latency;

public:
  dmi_memory()
  {
    initial_dmi_memory();
  }

  void operation(tlm::tlm_generic_payload &gp, sc_core::sc_time &delay)
  {
    sc_dt::uint64    gp_offset      = gp.get_address() - m_dmi_start_address;
    tlm::tlm_command gp_command     = gp.get_command();
    unsigned char   *gp_data_ptr    = gp.get_data_ptr();
    unsigned int     gp_data_length = gp.get_data_length();
    sc_assert(gp_data_ptr != NULL);

    // execute dmi access
    if (gp_command == tlm::TLM_READ_COMMAND) {
      memcpy(gp_data_ptr, m_dmi_ptr + gp_offset, gp_data_length);
      delay += m_dmi_read_latency;
    }
    else if (gp_command == tlm::TLM_WRITE_COMMAND) {
      memcpy(m_dmi_ptr + gp_offset, gp_data_ptr, gp_data_length);
      delay += m_dmi_write_latency;
    }
    else {
      /* do nothing */
    }
    gp.set_response_status(tlm::TLM_OK_RESPONSE);
  }

  bool invalidate_dmi(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
  {
    // check whether magnitude relation of dmi invalidated range is reversed or not
    if (start_range > end_range) {
      printf("ERROR : dmi_memory::invalidate_dmi() : magnitude relation of dmi invalidated range is reversed.\n");
      return false; // not invalidate
    }

    // check whether dmi invalidated range is overlapped dmi area or not
    if ((start_range < m_dmi_end_address) && (end_range > m_dmi_start_address)) {
      initial_dmi_memory();
      return true;
    }
    else {
      return false; // not invalidate
    }
  }

  void initial_dmi_memory()
  {
    m_dmi_ptr            = NULL;
    m_dmi_start_address  = 0;
    m_dmi_end_address    = 0xffffffffffffffffULL;
    m_dmi_granted_access = tlm::tlm_dmi::DMI_ACCESS_NONE;
    m_dmi_read_latency   = SC_ZERO_TIME;
    m_dmi_write_latency  = SC_ZERO_TIME;
  }

  void load_dmi_ptr(tlm::tlm_dmi &dmi_properties)
  {
    // copy dmi data from transaction to dmi memory
    m_dmi_ptr            = dmi_properties.get_dmi_ptr();
    m_dmi_start_address  = dmi_properties.get_start_address();
    m_dmi_end_address    = dmi_properties.get_end_address();
    m_dmi_granted_access = dmi_properties.get_granted_access();
    m_dmi_read_latency   = dmi_properties.get_read_latency();
    m_dmi_write_latency  = dmi_properties.get_write_latency();
  }

  bool is_address_dmi(tlm::tlm_generic_payload &gp)
  {
    unsigned char   *gp_data_ptr      = gp.get_data_ptr();
    sc_dt::uint64    gp_start_address = gp.get_address();
    sc_dt::uint64    gp_end_address   = gp_start_address + gp.get_data_length();
    tlm::tlm_command gp_command       = gp.get_command();

    // check whether data pointer of generic payload or dmi memory is validated or not
    if ((gp_data_ptr == NULL) || (m_dmi_ptr == NULL)) {
      return false;
    }

    // check whether transaction address is within dmi area or not
    if ((gp_start_address < m_dmi_start_address) || (gp_end_address > m_dmi_end_address)) {
      return false; // not grant
    }

    // check whether transaction command is dmi granted access or not
    if (gp_command == tlm::TLM_READ_COMMAND) {
      return (m_dmi_granted_access & tlm::tlm_dmi::DMI_ACCESS_READ);
    }
    else if (gp_command == tlm::TLM_WRITE_COMMAND) {
      return (m_dmi_granted_access & tlm::tlm_dmi::DMI_ACCESS_WRITE);
    }
    else {
      return false; // not grant
    }
  }
};

/* Class of TLM Base Interface */
class tlm_base_if : public tlm::tlm_mm_interface
{
  enum mem_spec_enum {INITIATOR_SPEC, TARGET_SPEC};

  struct mem_history {
    sc_time time_stamp;
    std::string call_method;
    tlm::tlm_phase call_phase;
    sc_time call_t;
    tlm::tlm_sync_enum return_sync;
    tlm::tlm_phase return_phase;
    sc_time return_t;

    mem_history(std::string arg_call_method, tlm::tlm_phase arg_call_phase, sc_time arg_call_t,
      tlm::tlm_sync_enum arg_return_sync, tlm::tlm_phase arg_return_phase, sc_time arg_return_t) {
        time_stamp = sc_time_stamp();
        call_method = arg_call_method;
        call_phase = arg_call_phase;
        call_t = arg_call_t;
        return_sync = arg_return_sync;
        return_phase = arg_return_phase;
        return_t = arg_return_t;
    }

    virtual ~mem_history() {
      /* do nothing */
    }
  };

  struct transport_info {
    sc_time time_stamp;
    sc_time t;
    bool is_stored;

    transport_info() {
      init();
    }

    virtual ~transport_info() {
      /* do nothing */
    }

    void init() {
      time_stamp = SC_ZERO_TIME;
      t = SC_ZERO_TIME;
      is_stored = false;
    }

    void store(sc_time arg_t = SC_ZERO_TIME) {
      time_stamp = sc_time_stamp();
      t = arg_t;
      is_stored = true;
    }
  };

  struct mem_payload {
    mem_spec_enum spec;
    tlm_base_if *p_if;
    tlm::tlm_generic_payload *p_trans;
    std::list<mem_history> history;
    bool resp_allowed;
    sc_time resp_allowed_time;
    sc_event resp_allowed_event;
    transport_info trans_info;
    sc_event end_req_event;
    sc_event resp_rx_event;
    tlm::tlm_generic_payload *p_origin;
    std::list<tlm::tlm_generic_payload *> p_clones;
    bool outstanding;

    mem_payload(mem_spec_enum arg_spec, tlm_base_if *p_arg_if) {
      spec = arg_spec;
      sc_assert(p_arg_if != NULL);
      p_if = p_arg_if;
      if (spec == INITIATOR_SPEC) {
        /* create transaction and extension object */
        p_trans = new tlm::tlm_generic_payload();
        p_if->ini_create_ext(*p_trans);
      }
      /* be necessarily TARGET_SPEC */
      else {
        p_trans = NULL;
      }
      init();
    }

    virtual ~mem_payload() {
      if ((spec == INITIATOR_SPEC) && (p_trans != NULL)) {
        /* destroy extension and transaction object */
        p_trans->free_all_extensions();
        delete p_trans;
      }
    }

    void init() {
      if ((spec == INITIATOR_SPEC) && (p_trans != NULL)) {
        /* initialize transaction and extension attributes */
        p_trans->set_command(tlm::TLM_IGNORE_COMMAND);
        p_trans->set_address(0ULL);
        p_trans->set_data_ptr(NULL);
        p_trans->set_data_length(0U);
        p_trans->set_byte_enable_ptr(NULL);
        p_trans->set_byte_enable_length(0U);
        p_trans->set_streaming_width(0U);
        p_trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        p_trans->set_dmi_allowed(false);
        p_trans->reset();
        p_trans->set_mm(NULL);
        p_if->ini_init_ext(*p_trans);
      }
      /* be necessarily TARGET_SPEC */
      else {
        p_trans = NULL;
      }
      history.clear();
      resp_allowed = true;
      resp_allowed_time = SC_ZERO_TIME;
      resp_allowed_event.cancel();
      trans_info.init();
      end_req_event.cancel();
      resp_rx_event.cancel();
      p_origin = NULL;
      p_clones.clear();
      outstanding = false;
    }
  };

  std::string m_name;
  mem_spec_enum m_spec;
  std::map<tlm::tlm_generic_payload *, mem_payload *> m_pool;
  std::queue<mem_payload *> m_re_pool;
  std::string m_file_name;
  unsigned int m_line_number;
  std::string m_function_name;
  std::string m_process_name;

public:
  tlm_base_if(const std::string name, const std::string spec_name) {
    m_name = name;
    if (spec_name == "INITIATOR_SPEC") {
      m_spec = INITIATOR_SPEC;
    }
    else if (spec_name == "TARGET_SPEC") {
      m_spec = TARGET_SPEC;
    }
    else {
      sc_assert(false);
    }
    m_file_name = "UNKNOWN";
    m_line_number = 0;
    m_function_name = "UNKNOWN";
    m_process_name = "UNKNOWN";
  }

  virtual ~tlm_base_if() {
    cancel_all();
  }

  void cancel_all() {
   for (std::map<tlm::tlm_generic_payload *, mem_payload *>::iterator it = m_pool.begin(); it != m_pool.end(); it++) {
      if (it->second != NULL) {
        delete it->second;
      }
    }
    m_pool.clear();
    while (! m_re_pool.empty()) {
      if (m_re_pool.front() != NULL) {
        delete m_re_pool.front();
      }
      m_re_pool.pop();
    }
    m_file_name = "UNKNOWN";
    m_line_number = 0;
    m_function_name = "UNKNOWN";
    m_process_name = "UNKNOWN";
  }

  virtual void ini_create_ext(tlm::tlm_generic_payload &trans) {
    sc_assert(false);
  }

  virtual void ini_init_ext(tlm::tlm_generic_payload &trans) {
    sc_assert(false);
  }

  tlm::tlm_generic_payload *acquire(tlm::tlm_generic_payload *p_trans = NULL) {
    mem_payload *p_payload = NULL;
    if (m_spec == INITIATOR_SPEC) {
      sc_assert(p_trans == NULL);
      if (m_re_pool.empty()) {
        p_payload = new mem_payload(INITIATOR_SPEC, this);
      }
      else {
        p_payload = m_re_pool.front();
        m_re_pool.pop();
      }
      sc_assert((p_payload != NULL) && (p_payload->p_trans != NULL));
      sc_assert(m_pool.find(p_payload->p_trans) == m_pool.end());
      m_pool[p_payload->p_trans] = p_payload;
      p_payload->p_trans->set_mm(this);
      p_payload->p_trans->acquire();
    }
    /* be necessarily TARGET_SPEC */
    else {
      sc_assert(p_trans != NULL);
      if (m_re_pool.empty()) {
        p_payload = new mem_payload(TARGET_SPEC, this);
      }
      else {
        p_payload = m_re_pool.front();
        m_re_pool.pop();
      }
      sc_assert(p_payload != NULL);
      p_payload->p_trans = p_trans;
      sc_assert(m_pool.find(p_trans) == m_pool.end());
      m_pool[p_trans] = p_payload;
      if (p_trans->has_mm()) {
        p_trans->acquire();
      }
    }
    return p_payload->p_trans;
  }

  void release(tlm::tlm_generic_payload *p_trans, bool mm_flag=true) { // added mm_flag
    sc_assert((p_trans != NULL) && is_entried(p_trans));
    if (m_spec == INITIATOR_SPEC) { // ignore mm_flag when INITIATOR_SPEC
      p_trans->release();
    }
    /* be necessarily TARGET_SPEC */
    else {
      m_pool[p_trans]->init();
      m_re_pool.push(m_pool[p_trans]);
      m_pool.erase(p_trans);
      if (mm_flag && p_trans->has_mm()) {  // if mm_flag is false, p_trans can't be accessed.
        p_trans->release();
      }
    }
  }

  void free(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    m_pool[p_trans]->init();
    m_re_pool.push(m_pool[p_trans]);
    m_pool.erase(p_trans);
  }

  bool is_entried(tlm::tlm_generic_payload *p_trans) {
    sc_assert(p_trans != NULL);
    return ((m_pool.find(p_trans) != m_pool.end()) && (m_pool[p_trans] != NULL));
  }

  void set_history(tlm::tlm_generic_payload *p_trans, std::string call_method, tlm::tlm_phase call_phase, sc_time call_t,
    tlm::tlm_sync_enum return_sync, tlm::tlm_phase return_phase, sc_time return_t) {
    sc_assert(is_entried(p_trans));
    m_pool[p_trans]->history.push_back(mem_history(call_method, call_phase, call_t, return_sync, return_phase, return_t));
  }

  tlm::tlm_phase get_phase(tlm::tlm_generic_payload *p_trans) {
    if (! is_entried(p_trans) || m_pool[p_trans]->history.empty()) {
      return tlm::UNINITIALIZED_PHASE;
    }
    return m_pool[p_trans]->history.back().return_phase;
  }

  tlm::tlm_sync_enum get_sync(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    sc_assert(! m_pool[p_trans]->history.empty());
    return m_pool[p_trans]->history.back().return_sync;
  }

  std::string get_history_string(tlm::tlm_generic_payload *p_trans, std::string severity_level, bool history_ended) {
    sc_assert(p_trans != NULL);
    if (! is_entried(p_trans) || m_pool[p_trans]->history.empty()) {
      return "";
    }
    /* get transaction attributes */
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    get_gp_attribute(command, address, data_ptr, data_length, *p_trans, true);
    /* start */
    std::stringstream history_stream;
    history_stream << severity_level << "[" << std::setw(20) << std::right << to_string(m_pool[p_trans]->history.front().time_stamp) << "] (" << std::setw(10) << std::left << m_name << ") ";
    history_stream << "START            { " << to_string(command) << " | " << to_hex_string((unsigned int) address) << " | ";
    history_stream << data_length << " | " << to_hex_string(data_ptr, sc_min<unsigned int>(data_length, 32)) << ((data_length > 32) ? " ... }" : " }") << std::endl;
    /* phase */
    int phase_id = 1;
    for (std::list<mem_history>::iterator it = m_pool[p_trans]->history.begin(); it != m_pool[p_trans]->history.end(); it++) {
      history_stream << severity_level << "[" << std::setw(20) << std::right << to_string((*it).time_stamp) << "] (" << std::setw(10) << std::left << m_name << ") ";
      history_stream << "PHASE" << phase_id << "           { " << (*it).call_method << "/" << std::setw(10) << std::left << to_string((*it).call_phase) << " (" << std::setw(10) << std::right << (*it).call_t.to_string() << ")  ->  ";
      history_stream << std::setw(13) << std::left << to_string((*it).return_sync) << "/" << std::setw(10) << std::left << to_string((*it).return_phase) << " (" << std::setw(10) << std::right << (*it).return_t.to_string() << ") }" << endl;
      phase_id++;
    }
    /* end */
    if (history_ended) {
      history_stream << severity_level << "[" << std::setw(20) << std::right << to_string(sc_time_stamp()) << "] (" << std::setw(10) << std::left << m_name << ") ";
      history_stream << "END" << std::endl;
    }
    return history_stream.str();
  }

  void print_history(tlm::tlm_generic_payload *p_trans) {
    std::cout << get_history_string(p_trans, "info(TLM)   ", true) << std::flush;
  }

  void lock_resp_allowed(tlm::tlm_generic_payload *p_trans, sc_time t) {
    sc_assert(is_entried(p_trans));
    m_pool[p_trans]->resp_allowed = false;
    m_pool[p_trans]->resp_allowed_time = sc_time_stamp() + t;
    m_pool[p_trans]->resp_allowed_event.cancel();
  }

  void unlock_resp_allowed(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    m_pool[p_trans]->resp_allowed = true;
    m_pool[p_trans]->resp_allowed_time = SC_ZERO_TIME;
    m_pool[p_trans]->resp_allowed_event.notify();
  }

  bool will_be_resp_allowed(tlm::tlm_generic_payload *p_trans, sc_time t) {
    sc_assert(is_entried(p_trans));
    return (bool) (m_pool[p_trans]->resp_allowed_time <= sc_time_stamp() + t);
  }

  bool is_resp_allowed(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    return m_pool[p_trans]->resp_allowed;
  }

  sc_time get_resp_allowed_time(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    return m_pool[p_trans]->resp_allowed_time;
  }

  sc_event &get_resp_allowed_event(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    return m_pool[p_trans]->resp_allowed_event;
  }

  void store_b_transport_info(tlm::tlm_generic_payload *p_trans, sc_time t) {
    sc_assert(is_entried(p_trans));
    sc_assert(! m_pool[p_trans]->trans_info.is_stored);
    m_pool[p_trans]->trans_info.store(t);
  }

  void print_b_transport_info(tlm::tlm_generic_payload *p_trans, sc_time t) {
    sc_assert(p_trans);
    sc_assert(is_entried(p_trans));
    sc_assert(m_pool[p_trans]->trans_info.is_stored);
    sc_time prev_time_stamp = m_pool[p_trans]->trans_info.time_stamp;
    sc_time prev_t = m_pool[p_trans]->trans_info.t;
    /* get transaction attributes */
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    get_gp_attribute(command, address, data_ptr, data_length, *p_trans, true);
    /* start */
    std::stringstream info_stream;
    info_stream << "info(TLM)   [" << std::setw(20) << std::right << to_string(prev_time_stamp) << "] (" << std::setw(10) << std::left << m_name << ") ";
    info_stream << "START            { " << to_string(command) << " | " << to_hex_string((unsigned int) address) << " | ";
    info_stream << data_length << " | " << to_hex_string(data_ptr, sc_min<unsigned int>(data_length, 32)) << ((data_length > 32) ? " ... }" : " }") << std::endl;
    /* end */
    info_stream << "info(TLM)   [" << std::setw(20) << std::right << to_string(sc_time_stamp()) << "] (" << std::setw(10) << std::left << m_name << ") ";
    info_stream << "END              { b_transport (" << std::setw(10) << std::right << prev_t.to_string() << ")  ->  ";
    info_stream << p_trans->get_response_string() << " (" << std::setw(10) << std::right << t.to_string() << ") }" << endl;
    /* print */
    m_pool[p_trans]->trans_info.init();
    std::cout << info_stream.str() << std::flush;
  }

  void store_transport_dbg_info(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    sc_assert(! m_pool[p_trans]->trans_info.is_stored);
    m_pool[p_trans]->trans_info.store();
  }

  void print_transport_dbg_info(tlm::tlm_generic_payload *p_trans) {
    sc_assert(p_trans);
    sc_assert(is_entried(p_trans));
    sc_assert(m_pool[p_trans]->trans_info.is_stored);
    sc_time prev_time_stamp = m_pool[p_trans]->trans_info.time_stamp;
    /* get transaction attributes */
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    get_gp_attribute(command, address, data_ptr, data_length, *p_trans, true);
    /* start */
    std::stringstream info_stream;
    info_stream << "info(TLM)   [" << std::setw(20) << std::right << to_string(prev_time_stamp) << "] (" << std::setw(10) << std::left << m_name << ") ";
    info_stream << "START            { " << to_string(command) << " | " << to_hex_string((unsigned int) address) << " | ";
    info_stream << data_length << " | " << to_hex_string(data_ptr, sc_min<unsigned int>(data_length, 32)) << ((data_length > 32) ? " ... }" : " }") << std::endl;
    /* end */
    info_stream << "info(TLM)   [" << std::setw(20) << std::right << to_string(sc_time_stamp()) << "] (" << std::setw(10) << std::left << m_name << ") ";
    info_stream << "END              { transport_dbg  ->  ";
    info_stream << p_trans->get_response_string() << " }" << endl;
    /* print */
    m_pool[p_trans]->trans_info.init();
    std::cout << info_stream.str() << std::flush;
  }

  sc_event &get_end_req_event(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    return m_pool[p_trans]->end_req_event;
  }

  sc_event &get_resp_rx_event(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    return m_pool[p_trans]->resp_rx_event;
  }

  tlm::tlm_generic_payload *get_origin(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    return m_pool[p_trans]->p_origin;
  }

  void set_origin(tlm::tlm_generic_payload *p_trans, tlm::tlm_generic_payload *p_origin) {
    sc_assert(is_entried(p_trans));
    m_pool[p_trans]->p_origin = p_origin;
  }

  std::list<tlm::tlm_generic_payload *> &get_clones(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    return m_pool[p_trans]->p_clones;
  }

  void set_outstanding(tlm::tlm_generic_payload *p_trans, bool outstanding) {
    sc_assert(is_entried(p_trans));
    m_pool[p_trans]->outstanding = outstanding;
  }

  bool is_outstanding(tlm::tlm_generic_payload *p_trans) {
    sc_assert(is_entried(p_trans));
    return m_pool[p_trans]->outstanding;
  }

  bool get_gp_attribute(tlm::tlm_command &command, sc_dt::uint64 &address, unsigned char *&data_ptr, unsigned int &data_length, const tlm::tlm_generic_payload &trans, bool is_debug = false) const {
    command = trans.get_command();
    /* in the case that byte enable attributes are not used */
    unsigned char *be_array = trans.get_byte_enable_ptr();
    if (is_debug || (be_array == NULL)) {
      address = trans.get_address();
      data_ptr = trans.get_data_ptr();
      data_length = trans.get_data_length();
    }
    /* in the case that byte enable attributes are used */
    else {
      unsigned int length = trans.get_data_length();
      unsigned int be_length = trans.get_byte_enable_length();
      if (be_length == 0) {
        return false;
      }
      int shift_size = -1;
      unsigned int enable_size = 0;
      for (unsigned int i = 0; i < length; i++) {
        if (be_array[i % be_length] == TLM_BYTE_ENABLED) {
          if (shift_size == -1) {
            shift_size = i;
          }
          enable_size++;
        }
        else {
          if (shift_size >= 0) {
            break;
          }
        }
      }
      if (shift_size == -1) {
        return false;
      }
      for (unsigned int i = shift_size + enable_size; i < length; i++) {
        if (be_array[i % be_length] == TLM_BYTE_ENABLED) {
          return false;
        }
      }
      address = trans.get_address() + shift_size;
      data_ptr = trans.get_data_ptr() + shift_size;
      data_length = enable_size;
    }
    return true;
  }

#ifdef TLMCC_DISABLE_ASSERT
#define gp_assert(...);
#else
#define gp_assert(...); \
  this->get_report_header(__FILE__, __LINE__, __FUNCTION__, sc_get_current_process_handle().name()); \
  this->assert_with_report(__VA_ARGS__);
#endif

  void get_report_header(std::string file_name, unsigned int line_number, std::string function_name, std::string process_name) {
#ifdef _WIN32
    m_file_name = file_name.substr(file_name.rfind("\\") + 1);
#else
    m_file_name = file_name.substr(file_name.rfind("/") + 1);
#endif
    m_line_number = line_number;
#ifdef _WIN32
    m_function_name = function_name.substr(function_name.rfind("::") + 2);
#else
    m_function_name = function_name;
#endif
    m_process_name = process_name.substr(process_name.rfind(".") + 1);
  }

  void assert_with_report(bool expr, tlm::tlm_generic_payload *p_trans, const char *argc, ...) {
    /* pass */
    if (expr) {
      return;
    }
    /* fail */
    char report_msg[1024] = "UNKNOWN";
    if (argc != NULL) {
      va_list list;
      va_start(list, argc);
      vsprintf(report_msg, argc, list);
      va_end(list);
    }
    std::stringstream report_stream;
    report_stream << "fatal(TLM)  [" << std::setw(20) << std::right << to_string(sc_time_stamp()) << "] (" << std::setw(10) << std::left << m_name << ") ";
#if 0
    report_stream << "STOP @           { " << m_file_name << " (" << m_line_number << ") : " << m_function_name << " }" << std::endl;
#else
    report_stream << "STOP @           { " << m_file_name << " : " << m_function_name << " }" << std::endl;
#endif
    report_stream << "fatal(TLM)  [" << std::setw(20) << std::right << to_string(sc_time_stamp()) << "] (" << std::setw(10) << std::left << m_name << ") ";
    report_stream << "REPORT           { " << report_msg << " }" << std::endl;
    if (p_trans != NULL) {
      std::cout << get_history_string(p_trans, "fatal(TLM)  ", false);
    }
    std::cout << report_stream.str() << std::flush;
    exit(1);
  }

  std::string to_string(sc_time t) {
    std::stringstream t_stream;
    double tu_value = 0;
    std::string tu_name = "";
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
      tu_value = 1000000000000000LL;
      tu_name = " fs";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
      tu_value = 1000000000000LL;
      tu_name = " ps";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
      tu_value = 1000000000LL;
      tu_name = " ns";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
      tu_value = 1000000LL;
      tu_name = " us";
    }
    else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
      tu_value = 1000LL;
      tu_name = " ms";
    }
    else {
      tu_value = 1LL;
      tu_name = " s";
    }
    t_stream << t.value() * (uint64) (sc_get_time_resolution().to_seconds() * tu_value) << tu_name;
    return t_stream.str();
  }

  std::string to_string(tlm::tlm_sync_enum sync) {
    std::string sync_name = "UNKNOWN";
    if (sync == tlm::TLM_ACCEPTED) {
      sync_name = "TLM_ACCEPTED";
    }
    else if (sync == tlm::TLM_UPDATED) {
      sync_name = "TLM_UPDATED";
    }
    /* be necessarily TLM_COMPLETED */
    else {
      sync_name = "TLM_COMPLETED";
    }
    return sync_name;
  }

  std::string to_string(tlm::tlm_phase phase) {
    std::string phase_name = "UNKNOWN";
    if (phase == tlm::UNINITIALIZED_PHASE) {
      phase_name = "UNINITIALIZED_PHASE";
    }
    else if (phase == tlm::BEGIN_REQ) {
      phase_name = "BEGIN_REQ";
    }
    else if (phase == tlm::END_REQ) {
      phase_name = "END_REQ";
    }
    else if (phase == tlm::BEGIN_RESP) {
      phase_name = "BEGIN_RESP";
    }
    /* be necessarily END_RESP */
    else {
      phase_name = "END_RESP";
    }
    return phase_name;
  }

  std::string to_string(tlm::tlm_command command) {
    std::string command_name = "UNKNOWN";
    if (command == tlm::TLM_READ_COMMAND) {
      command_name = "TLM_READ_COMMAND";
    }
    else if (tlm::TLM_WRITE_COMMAND) {
      command_name = "TLM_WRITE_COMMAND";
    }
    /* be necessarily TLM_IGNORE_COMMAND */
    else {
      command_name = "TLM_IGNORE_COMMAND";
    }
    return command_name;
  }

  std::string to_hex_string(sc_dt::uint64 value) {
    std::stringstream value_stream;
    value_stream << std::setw(8) << std::setfill('0') << std::hex << value;
    return value_stream.str();
  }

  std::string to_hex_string(unsigned char *data_ptr, unsigned int data_length) {
    if (data_ptr == NULL) {
      return "NULL";
    }
    std::stringstream data_stream;
    for (unsigned int i = 0; i < data_length; i++) {
      data_stream << (((i > 0) && ((i % 4) == 0)) ? " " : "") << std::setw(2) << std::setfill('0') << std::hex << (unsigned int) *(data_ptr + i);
    }
    return data_stream.str();
  }

  std::string to_formal_string(tlm::tlm_generic_payload *p_trans) {
    if (p_trans == NULL) {
      return "NULL";
    }
    return to_formal_string(p_trans->get_command(), p_trans->get_address(), p_trans->get_data_ptr(), p_trans->get_data_length());
  }

  std::string to_formal_string(tlm::tlm_command command, sc_dt::uint64 address, unsigned char *data_ptr, unsigned int data_length) {
    std::stringstream trans_stream;
    trans_stream << to_string(command) << " | " << to_hex_string(address) << " | ";
    trans_stream << data_length << " | " << to_hex_string(data_ptr, sc_min<unsigned int>(data_length, 32)) << ((data_length > 32) ? " ..." : "");
    return trans_stream.str();
  }

  sc_time to_time(double time_value) {
    if (time_value < 0) {
      time_value = 0;
    }
    return sc_get_time_resolution() * time_value;
  }

  tlm::tlm_sync_enum to_sync(std::string sync_name) {
    tlm::tlm_sync_enum sync = tlm::TLM_ACCEPTED;
    if (sync_name == "TLM_ACCEPTED") {
      sync = tlm::TLM_ACCEPTED;
    }
    else if (sync_name == "TLM_UPDATED") {
      sync = tlm::TLM_UPDATED;
    }
    else if (sync_name == "TLM_COMPLETED"){
      sync = tlm::TLM_COMPLETED;
    }
    /* invalid sync name */
    else {
      /* do nothing */
    }
    return sync;
  }

  tlm::tlm_phase to_phase(std::string phase_name) {
    tlm::tlm_phase phase = tlm::UNINITIALIZED_PHASE;
    if (phase_name == "UNINITIALIZED_PHASE") {
      phase = tlm::UNINITIALIZED_PHASE;
    }
    else if (phase_name == "BEGIN_REQ") {
      phase = tlm::BEGIN_REQ;
    }
    else if (phase_name == "END_REQ") {
      phase = tlm::END_REQ;
    }
    else if (phase_name == "BEGIN_RESP") {
      phase = tlm::BEGIN_RESP;
    }
    else if (phase_name == "END_RESP") {
      phase = tlm::END_RESP;
    }
    /* invalid phase name */
    else {
      /* do nothing */
    }
    return phase;
  }

  tlm::tlm_response_status to_status(std::string status_name) {
    tlm::tlm_response_status status = tlm::TLM_INCOMPLETE_RESPONSE;
    if (status_name == "TLM_OK_RESPONSE") {
      status = tlm::TLM_OK_RESPONSE;
    }
    else if (status_name == "TLM_INCOMPLETE_RESPONSE") {
      status = tlm::TLM_INCOMPLETE_RESPONSE;
    }
    else if (status_name == "TLM_GENERIC_ERROR_RESPONSE") {
      status = tlm::TLM_GENERIC_ERROR_RESPONSE;
    }
    else if (status_name == "TLM_ADDRESS_ERROR_RESPONSE") {
      status = tlm::TLM_ADDRESS_ERROR_RESPONSE;
    }
    else if (status_name == "TLM_COMMAND_ERROR_RESPONSE") {
      status = tlm::TLM_COMMAND_ERROR_RESPONSE;
    }
    else if (status_name == "TLM_BURST_ERROR_RESPONSE") {
      status = tlm::TLM_BURST_ERROR_RESPONSE;
    }
    else if (status_name == "TLM_BYTE_ENABLE_ERROR_RESPONSE") {
      status =tlm:: TLM_BYTE_ENABLE_ERROR_RESPONSE;
    }
    /* invalid status name */
    else {
      /* do nothing */
    }
    return status;
  }
};

}

#endif
