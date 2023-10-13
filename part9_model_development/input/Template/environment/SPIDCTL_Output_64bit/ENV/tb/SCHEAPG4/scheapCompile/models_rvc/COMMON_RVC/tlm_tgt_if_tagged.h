// $Id: tlm_tgt_if_tagged.h,v 1.9 2010/11/15 06:49:44 masudamk Exp $
// =============================================================
//  file name : tlm_tgt_if_tagged.h
// =============================================================

#ifndef __TLM_TGT_IF_TAGGED_H__
#define __TLM_TGT_IF_TAGGED_H__

#include <cstdarg>
#include <systemc.h>
#include <tlm.h>
#include "tlm_utils/peq_with_get.h"

#include "tlm_if.h"
#include "tlm_if_command.h"

#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <list>

namespace vpcl
{

template <unsigned int BUSWIDTH, unsigned int N_TGT>
class tlm_tgt_if_base;

template <unsigned int BUSWIDTH, unsigned int N_TGT>
class tlm_tgt_tagged_thread;

template <unsigned int BUSWIDTH = 64, unsigned int N_TGT = 1>
class tlm_tgt_if_tagged : public tlm::tlm_fw_transport_if<>
{
public:
  // tlm target thread
  tlm_tgt_tagged_thread<BUSWIDTH, N_TGT> * tlm_tgt_t;

  tlm_tgt_if_base<BUSWIDTH, N_TGT> * m_core;
  unsigned int m_socket_id;
                                                                                
public:
  tlm_tgt_if_tagged(const char * name, tlm_tgt_if_base<BUSWIDTH, N_TGT> * core, unsigned int socket_id)
    : m_core(core)
    , m_socket_id(socket_id)
    , m_name(name)
    , m_param((std::string)name, BUSWIDTH/8)
    , m_command((std::string)name, BUSWIDTH/8)
    , m_req_rx_peq(((std::string)name + "_req_rx_peq").c_str())
    , m_resp_tx_peq(((std::string)name + "_resp_tx_peq").c_str())
    , m_buf_use(0)
  {
    tlm_tgt_t = new tlm_tgt_tagged_thread<BUSWIDTH, N_TGT>(((std::string)name + "_tgt_thread").c_str(), this);
    if (tlm_tgt_t == NULL) {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"tlm_tgt_tagged_thread\" Can't Create tlm_tgt_tagged_thread Instance.\n", tgt_get_name());
      sc_stop();
    }

  }

  virtual ~tlm_tgt_if_tagged() {
    delete tlm_tgt_t;
  }

  // tlm_tgt_if_tagged api function (called by system or derived class)
  const char *tgt_get_name()
  {
    return m_name.c_str();
  }

  bool tgt_set_param(vpcl::tlm_if_tgt_parameter *param)
  {
    if (param != NULL) {
      m_param = *param;
      return(true);
    }
    else {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_set_param\" get trans NULL pointer.\n", tgt_get_name());
      return(false);
    }
  }
  
  bool tgt_get_param(vpcl::tlm_if_tgt_parameter *param)
  {
    if (param != NULL) {
      *param = m_param;
      return(true);
    }
    else {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_get_param\" get trans NULL pointer.\n", tgt_get_name());
      return(false);
    }
  }

  std::string tgt_source_file(const char *src_fname)
  {
    if (src_fname == NULL) {
      return ("[" + (std::string)tgt_get_name() + "]  ERROR : source file name \"NULL\" is invalid.\n");
    }
    FILE *src_fp;
    errno_t err;

    err = fopen_s(&src_fp, src_fname, "r");
    if (err != 0) {
      return ("[" + (std::string)tgt_get_name() + "]  ERROR : cannot open source file \"" + (std::string)src_fname + "\".\n");
    }
    std::string completed_cnt = "";

    while (1) {
      char c_line[1024];
      memset(c_line, 0x00, sizeof(c_line));
      if (fgets(c_line, 1024, src_fp) == NULL) {
        break;
      }

      std::string str_line(c_line);
      std::istringstream iss_line(str_line);
      std::vector<std::string> cmd_line;

      while (1) {
        std::string str_word;
        iss_line >> std::skipws >> str_word;
        if (str_word.size() == 0) {
          break;
        }
        cmd_line.push_back(str_word);
      }

      if ((cmd_line.size() != 0) && (cmd_line[0] == tgt_get_name())) {
        cmd_line.erase(cmd_line.begin());
        completed_cnt += m_command.tgt_handle_command(cmd_line, m_param);
      }
    }

    fclose(src_fp);
    return completed_cnt;
  }

public:
  // tlm_tgt_if_tagged api function (called by derived class)
  std::string tgt_handle_command(const std::vector<std::string> &args)
  {
    return m_command.tgt_handle_command(args, m_param);
  }

private:
  // instance name of derived class
  const std::string m_name;

  // target parameter
  tlm_if_tgt_parameter m_param;

  // handle command
  tlm_if_command  m_command;

  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm_if_nb_transaction tlm_if_nb_trans_type;

  std::queue<tlm_if_nb_trans_type *> mUnSentQueue;
  std::set<tlm_if_nb_trans_type *> mSentSet;
  std::map<tlm_if_nb_trans_type *, transaction_type *> m_tgt_map;

  tlm_utils::peq_with_get<transaction_type> m_req_rx_peq;
  tlm_utils::peq_with_get<transaction_type> m_resp_tx_peq;
  sc_core::sc_event m_end_resp_event;
  sc_core::sc_event m_resp_set_event;
  sc_core::sc_event m_resp_free_event;

  // Generic Payload Queue Number Managiment.
  unsigned int m_buf_use;

  // tlm2.0 api function (called by base class)
  void b_transport(tlm::tlm_generic_payload &trans, sc_time &t)
  {
    tgt_acc(trans, t);
//    t += trans.is_write() ? m_param.wr_latency : m_param.rd_latency;
    tgt_dump_log(trans, t);
  }

  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t)
  {
    tlm::tlm_sync_enum rtn_val = tlm::TLM_COMPLETED;

    if ( m_param.phase_mode == vpcl::TLM_IF_SINGLE_PHASE) {
      tgt_acc(trans, t);
//      t += trans.is_write() ? m_param.wr_latency : m_param.rd_latency;
      tgt_dump_log(trans);
      rtn_val = tlm::TLM_COMPLETED;
    } else if ( m_param.phase_mode == vpcl::TLM_IF_MULTI_PHASE) {
      if ( phase == tlm::BEGIN_REQ ) {
        t = sc_core::SC_ZERO_TIME;

        m_req_rx_peq.notify(trans, t);  // immidiate Wake Up tgt_req_thread.
        phase = tlm::END_REQ;
        // Set T1 here.
        t = calc_t1(&trans);

        rtn_val = tlm::TLM_UPDATED;
      } else if ( phase == tlm::END_REQ ) {
        t = sc_core::SC_ZERO_TIME;
        m_req_rx_peq.notify(trans, t);  // immidiate Wake Up tgt_req_thread.
        phase = tlm::END_REQ;
        // Set T1 here.
        t = calc_t1(&trans);

        rtn_val = tlm::TLM_UPDATED;
      } else if ( phase == tlm::BEGIN_RESP ) {
        m_end_resp_event.notify(sc_core::SC_ZERO_TIME);     // immidiate Wake Up tgt_resp_thread.
        phase = tlm::END_RESP;
        rtn_val = tlm::TLM_COMPLETED;
      } else if ( phase == tlm::END_RESP ) {
        m_end_resp_event.notify(sc_core::SC_ZERO_TIME);     // immidiate Wake Up tgt_resp_thread.
        phase = tlm::END_RESP;
        rtn_val = tlm::TLM_COMPLETED;
      } else {
        assert(0); sc_stop();
      }
    } else { // No support phase.
      assert(0); sc_stop();
    }
	return rtn_val;
  }

  bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data)
  {

    vpcl::tlm_if_dmi *dmi=new vpcl::tlm_if_dmi();
    bool rtn_val = false;

    // get dmi properties from derived class
    if (trans.is_write()) {
      m_core->tgt_wr_dmi(m_socket_id, (unsigned int) trans.get_address(), dmi);
    }
    else {
      m_core->tgt_rd_dmi(m_socket_id, (unsigned int) trans.get_address(), dmi);
    }

    // set dmi properties to 2nd argument (dmi_data)
    if (dmi->wr_allow && dmi->rd_allow) {
      dmi_data.allow_read_write();
    }
    else if (dmi->wr_allow && (! dmi->rd_allow)) {
      dmi_data.allow_write();
    }
    else if ((! dmi->wr_allow) && dmi->rd_allow) {
      dmi_data.allow_read();
    }
    else {
      dmi_data.allow_none();
    }
    dmi_data.set_start_address(dmi->start_addr);
    dmi_data.set_end_address(dmi->end_addr);
    dmi_data.set_dmi_ptr(dmi->p_data);
    dmi_data.set_write_latency(m_param.wr_latency);
    dmi_data.set_read_latency(m_param.rd_latency);

    if (trans.is_write()) {
      if (dmi_data.is_write_allowed()) rtn_val = true; 
    }
    else if (trans.is_read()) {
      if (dmi_data.is_read_allowed()) rtn_val = true;
    }
    delete dmi;
    return (rtn_val);
  }

  unsigned int transport_dbg(tlm::tlm_generic_payload &trans)
  {
    tgt_acc_dbg(trans);
    return trans.get_data_length();
  }

  // sub routine function (called by my own class)
  void tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t)
  {
    bool status = false;
    vpcl::tlm_if_extension *p_ext = NULL;
//    sc_time t=SC_ZERO_TIME;
    bool crt_ext_flg = false;

    trans.get_extension(p_ext);
    if (p_ext == NULL) {
      crt_ext_flg = true;
      tgt_create_ext( p_ext );
    }

    unsigned int    addr;
    unsigned char * data_ptr;
    unsigned int    len;

    if(byte_enable_range(trans, addr, data_ptr, len) == false) {
      trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
      return;
    }

    if (trans.is_write()) {
      t = m_param.wr_latency;
      status = m_core->tgt_wr(m_socket_id, addr, data_ptr, len, p_ext, &t);
    }
    else {
      t = m_param.rd_latency;
      status = m_core->tgt_rd(m_socket_id, addr, data_ptr, len, p_ext, &t);
    }

    tgt_set_ext(p_ext);
    trans.set_response_status(status ? tlm::TLM_OK_RESPONSE : tlm::TLM_GENERIC_ERROR_RESPONSE);
    if ( crt_ext_flg == true ) delete p_ext;
  }

  void tgt_acc_dbg(tlm::tlm_generic_payload &trans)
  {
    bool status = false;
    vpcl::tlm_if_extension *p_ext = NULL;
    bool crt_ext_flg = false;

    trans.get_extension(p_ext);
    if (p_ext == NULL) {
      crt_ext_flg = true;
      tgt_create_ext( p_ext );
    }

    unsigned int    addr     = (unsigned int) trans.get_address();
    unsigned char * data_ptr = trans.get_data_ptr();
    unsigned int    len      = trans.get_data_length();

    if (trans.is_write()) {
      status = m_core->tgt_wr_dbg(m_socket_id, addr, data_ptr, len, p_ext);
    }
    else {
      status = m_core->tgt_rd_dbg(m_socket_id, addr, data_ptr, len, p_ext);
    }

    tgt_set_ext(p_ext);
    trans.set_response_status(status ? tlm::TLM_OK_RESPONSE : tlm::TLM_GENERIC_ERROR_RESPONSE);
    if ( crt_ext_flg == true ) delete p_ext;
  }

  bool byte_enable_range(tlm::tlm_generic_payload &trans, unsigned int &addr, unsigned char * &data_ptr, unsigned int &len)
  {
    unsigned char * be = trans.get_byte_enable_ptr();
    if(be == NULL) {
      addr     = (unsigned int) trans.get_address();
      data_ptr = trans.get_data_ptr();
      len      = trans.get_data_length();
    }
    else {
      unsigned int data_len = trans.get_data_length();
      unsigned int be_len   = trans.get_byte_enable_length();
      int sbyte = -1;
      len = 0;
      for(unsigned int i=0; i<data_len; i++) {
        if(be[i%be_len] == TLM_BYTE_ENABLED) {
          if(sbyte == -1) sbyte = i;
          len++;
        }
        else {
          if(sbyte >= 0) break;
        }
      }
      if(sbyte < 0) {
        dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : Empty byte enables\n", tgt_get_name()); // error
        return false;
      }
      for(unsigned int i=sbyte+len; i<data_len; i++) {
        if(be[i%be_len] == TLM_BYTE_ENABLED) {
          dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : Invalid byte enables\n", tgt_get_name()); // error // error
          return false;
        }
      }
      addr     = (unsigned int) trans.get_address() + sbyte;
      data_ptr = trans.get_data_ptr() + sbyte;
    }
    return true;
  }

  void tgt_set_ext(vpcl::tlm_if_extension *p_ext)
  {
    if (p_ext != NULL) {
      p_ext->tgt_if_param = m_param;
      p_ext->tgt_if_use   = true;
    }
  }

  virtual void tgt_dump_log(tlm::tlm_generic_payload &trans, sc_time delay=SC_ZERO_TIME)
  {
    if (m_param.p_log_file != NULL) {
      if ((trans.is_write() && (m_param.wr_log == true)) || (trans.is_read() && (m_param.rd_log == true))) {
        vpcl::tlm_if_extension *p_ext = NULL;
        trans.get_extension(p_ext);
        unsigned int src = (p_ext != NULL) ? p_ext->ini_if_param.src_id : 0;
        sc_time pass_time = sc_time_stamp() + delay;

        fprintf(m_param.p_log_file, "%8s :  [%-8s]  I(%02X) <-T  ", pass_time.to_string().c_str(), tgt_get_name(), src);
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
    } // End of (m_param.p_log_file != NULL) 
  }

public:
  bool tgt_resp_tx(vpcl::tlm_if_nb_transaction *p_nb_trans, unsigned int *p_buf_rem)
  {
    sc_time resp_latency = SC_ZERO_TIME;

    if ( (p_nb_trans != NULL) && (p_buf_rem != NULL) ) {
      // Get trans from p_nb_trans.
      std::map<tlm_if_nb_trans_type *, transaction_type *>::iterator it = m_tgt_map.find(p_nb_trans); 
      assert(it != m_tgt_map.end());

      tlm::tlm_generic_payload *trans;
      trans = it->second;
      if (trans == NULL) {
        dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_rest_tx\" Can't Get Generic Payload Pointer.\n", tgt_get_name());
        sc_stop();
      }

      m_buf_use++;

      *p_buf_rem = m_param.buf_size - m_buf_use;

      resp_latency = p_nb_trans->resp_tx_latency;

      m_resp_tx_peq.notify(*trans, resp_latency);

      // Delete NB Transaction from UnSent Queue.
      m_tgt_map.erase(it);

      delete p_nb_trans;

      return true;
    }
    else {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_resp_tx\" get trans NULL pointer.\n", tgt_get_name());
      return false;
    }
  }

  void tgt_invalidate_dmi(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
  {
    (*m_core->m_tgt_socket[m_socket_id])->invalidate_direct_mem_ptr(start_range, end_range);
  }

public:
  void tgt_req_thread(void)
  {
    tlm::tlm_generic_payload *trans;
    vpcl::tlm_if_nb_transaction *p_nb_trans;
    bool status = false;

//    while (true) {
      wait(m_req_rx_peq.get_event());

      trans = NULL;
      while ((trans = m_req_rx_peq.get_next_transaction())!=0) {

        if (trans != NULL) {
          p_nb_trans = NULL;
          p_nb_trans = new vpcl::tlm_if_nb_transaction();

          if (p_nb_trans == NULL) {
            dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"tgt_req_thread\" Can't Create tlm_if_nb_transaction Instance.\n", tgt_get_name());
            sc_stop();
          }

          p_nb_trans->wr_cmd          = trans->get_command();
          p_nb_trans->addr            = (unsigned int)trans->get_address();
          p_nb_trans->p_data          = trans->get_data_ptr();
          p_nb_trans->size            = trans->get_data_length();
          vpcl::tlm_if_extension *p_ext = NULL;
          bool crt_ext_flg = false;

          trans->get_extension(p_ext);

          if (p_ext == NULL) {
            crt_ext_flg = true;
            tgt_create_ext( p_ext );
          }
          tgt_set_ext(p_ext);
          p_nb_trans->p_ext = p_ext;

          p_nb_trans->req_latency     =  calc_t1(trans);

          // Entry KAY:GenericPayLoad's Pointer Value:NB Transaction Pointer to MAP
          m_tgt_map.insert( std::map<tlm_if_nb_trans_type *, transaction_type *>::value_type( p_nb_trans, trans ) );

          status = m_core->tgt_req_rx(m_socket_id, p_nb_trans);

          trans->set_response_status(status ? tlm::TLM_OK_RESPONSE : tlm::TLM_GENERIC_ERROR_RESPONSE);

          if (crt_ext_flg == true) delete p_ext;

        } // trans != NULL?
        else {
          dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_req_thread\" get trans NULL pointer.\n", tgt_get_name());
          sc_stop();
        } // trans == NULL
      } // while ((trans = m_req_rx_peq.get_next_transaction())!=0) 
//    } // while (true) End
  }

  void tgt_create_ext( vpcl::tlm_if_extension *&p_ext )
  {
    vpcl::tlm_if_extension *dmy_ext;

    dmy_ext = new vpcl::tlm_if_extension();
    if (dmy_ext == NULL) {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"tgt_req_thread\" Can't Create tlm_if_extension Instance.\n", tgt_get_name());
      sc_stop();
    }
    p_ext = dmy_ext;
  }

  void tgt_resp_thread(void)
  {
    tlm::tlm_generic_payload *trans;
    tlm::tlm_phase phase;
    unsigned int l_buf_rem = 0;

//    while (true) {

      wait(m_resp_tx_peq.get_event());

      trans = NULL;
      while ((trans = m_resp_tx_peq.get_next_transaction())!=0) {
        if (trans != NULL) {
          // Output Transaction Log.
          tgt_dump_log(*trans);

          phase = tlm::BEGIN_RESP;
          sc_time t=SC_ZERO_TIME;

          switch((*m_core->m_tgt_socket[m_socket_id])->nb_transport_bw(*trans,phase,t)) {
          case tlm::TLM_COMPLETED: // Transaction finished
            wait(t);  // Wait t3
            break;
          case tlm::TLM_ACCEPTED: // 
            if (phase == tlm::BEGIN_RESP) {

              wait(m_end_resp_event);

            }
            else {
              assert(0); sc_stop();
            }
            break;
          case tlm::TLM_UPDATED:
            // Transaction not yet finished
            if (phase == tlm::END_RESP) {
              wait(t);  // Wait t3
              break;
            }
            else {
              assert(0); sc_stop();
            }
            break;
          default:
            assert(0); sc_stop();
          } // End of switch

          m_buf_use--;
          l_buf_rem = m_param.buf_size - m_buf_use;
          m_core->tgt_resp_free(m_socket_id, l_buf_rem);

        } // End of trans NULL check.
        else {
          dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_resp_thread\" get trans NULL pointer.\n", tgt_get_name());
          sc_stop();
        }
        trans = NULL;
      } // End of while ((trans = m_resp_tx_peq.get_next_transaction())!=0)
//    } // End of while (true)
  } // End of Function.

  void tgt_resp_handle(void)
  {
    vpcl::tlm_if_nb_transaction *p_nb_trans;
    unsigned int l_buf_rem = 0;

//    while(true) {

      wait(m_resp_set_event);

      while (!mUnSentQueue.empty()) {
        if (m_buf_use >= m_param.buf_size) { // m_param.buf_size is Max sent peq size?
          wait(m_resp_free_event);
        }
        else{
          // Get & Delete Transaction from UnsentQueue.
          p_nb_trans = mUnSentQueue.front(); 
          mUnSentQueue.pop();

          l_buf_rem = m_param.buf_size - m_buf_use;
          tgt_resp_tx(p_nb_trans, &l_buf_rem);
        }
      }
//    }
  }

private:
  sc_time calc_t1(tlm::tlm_generic_payload *trans)
  {
    sc_time t=SC_ZERO_TIME;
    vpcl::tlm_if_tgt_parameter tgt_param;

    if (trans->is_write()) {
      this->tgt_get_param(&tgt_param);

      int trans_num = trans->get_data_length() / tgt_param.bus_width;
      int surplus = trans->get_data_length() % tgt_param.bus_width;
      if ( surplus != 0 ) trans_num += 1;

      t = trans_num * m_param.bus_clk; //
    }
    else {
      t = m_param.bus_rgnt;
    }
    return t;
  }

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

}; // tlm_tgt_if_tagged end

template <unsigned int BUSWIDTH = 64, unsigned int N_TGT = 1>
class tlm_tgt_tagged_thread : public sc_module
{
public:
  //SC_HAS_PROCESS(tlm_tgt_tagged_thread<BUSWIDTH, N_TGT>);
  SC_HAS_PROCESS(tlm_tgt_tagged_thread);
  tlm_tgt_tagged_thread(sc_module_name name, tlm_tgt_if_tagged<BUSWIDTH, N_TGT>* inst_nm)
    : sc_module(name)
    , m_inst_nm(inst_nm)
  {
    SC_THREAD(call_tgt_req_thread);
    SC_THREAD(call_tgt_resp_thread);
    SC_THREAD(call_tgt_resp_handle);
  }

  ~tlm_tgt_tagged_thread() {}

private:
  tlm_tgt_if_tagged<BUSWIDTH, N_TGT>* m_inst_nm;

  void call_tgt_req_thread(void)
  {
    while(true) {
      m_inst_nm->tgt_req_thread();
    }
  }

  void call_tgt_resp_thread(void)
  {
    while(true) {
      m_inst_nm->tgt_resp_thread();
    }
  }

  void call_tgt_resp_handle(void)
  {
    while(true) {
      m_inst_nm->tgt_resp_handle();
    }
  }

};  // end of tlm_tgt_tagged_thread class

}  // vpcl namespace end

#endif
