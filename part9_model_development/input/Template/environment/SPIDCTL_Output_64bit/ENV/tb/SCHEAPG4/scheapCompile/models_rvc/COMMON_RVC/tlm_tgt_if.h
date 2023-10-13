// $Id: tlm_tgt_if.h,v 1.28 2014/04/02 09:44:24 masudamk Exp $

/*************************************************************************
 *
 *  tlm_tgt_if.h
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

#ifndef __TLM_TGT_IF_H__
#define __TLM_TGT_IF_H__

#include "systemc.h"
#include "tlm.h"
#include "tlm_if.h"
#include "tlm_tgt_if_unit.h"
#include <vector>
#include <string>
#include <sstream>

#define SOCKET_ID 0 /* DEFAULT SOCKET ID FOR MULTIPLE SOCKET SUPPORT */

namespace vpcl
{

/* Class of TLM Target Interface */
template <unsigned int BUSWIDTH = 64, typename TYPES = tlm::tlm_base_protocol_types, unsigned int N = 1>
class tlm_tgt_if
{
  /* Friend Class */
  friend class tlm_tgt_if_unit<BUSWIDTH, TYPES, N>;

  /* Member Data */
  std::string m_name;
  unsigned int m_tmp_id;
  /* MULTIPLE SOCKET SUPPORT */
  vpcl::tlm_tgt_if_unit<BUSWIDTH, TYPES, N> *m_tgt_if_units[N];
public:
  tlm::tlm_target_socket<BUSWIDTH, TYPES, 1, sc_core::SC_ONE_OR_MORE_BOUND> *m_tgt_sockets[N];

  /* Constructor */
  tlm_tgt_if(const char *name)
    : m_name(name)
    , m_tmp_id(0)
  {
    /* MULTIPLE SOCKET SUPPORT */
    for (unsigned int i = 0; i < N; i++) {
      std::stringstream if_unit_name;
      std::stringstream socket_name;
      if (N == 1) {
        if_unit_name << m_name;
      }
      else {
        if_unit_name << m_name << "[" << i << "]";
      }
      socket_name << m_name << "_socket[" << i << "]";
      m_tgt_if_units[i] = new vpcl::tlm_tgt_if_unit<BUSWIDTH, TYPES, N>(if_unit_name.str(), this, i);
      m_tgt_sockets[i] = new tlm::tlm_target_socket<BUSWIDTH, TYPES, 1, sc_core::SC_ONE_OR_MORE_BOUND>(socket_name.str().c_str());
      m_tgt_sockets[i]->bind(*m_tgt_if_units[i]);
    }
  }

  /* Destructor */
  virtual ~tlm_tgt_if()
  {
    for (unsigned int i = 1; i < N; i++) {
      delete m_tgt_if_units[i];
      delete m_tgt_sockets[i];
    }
  }

  /* Parameter Setting Interface */
  bool tgt_get_param(vpcl::tlm_tgt_if_parameter *p_param) const
  {
    if (N > 1) {
//      std::cout << "warning : the method \"tgt_get_param\" has been called without the socket id." << std::endl;
    }
    return m_tgt_if_units[SOCKET_ID]->tgt_get_param(p_param);
  }

  bool tgt_set_param(vpcl::tlm_tgt_if_parameter *p_param)
  {
    if (N > 1) {
//      std::cout << "warning : the method \"tgt_set_param\" has been called without the socket id." << std::endl;
    }
    return m_tgt_if_units[SOCKET_ID]->tgt_set_param(p_param);
  }

  vpcl::tlm_tgt_if_parameter tgt_get_param() const
  {
    if (N > 1) {
//      std::cout << "warning : the method \"tgt_get_param\" has been called without the socket id." << std::endl;
    }
    return m_tgt_if_units[SOCKET_ID]->tgt_get_param();
  }
  /* MULTIPLE SOCKET SUPPORT */
  vpcl::tlm_tgt_if_parameter tgt_get_param(unsigned int id) const
  {
    return m_tgt_if_units[id]->tgt_get_param();
  }

  void tgt_set_param(const vpcl::tlm_tgt_if_parameter &param)
  {
    if (N > 1) {
//      std::cout << "warning : the method \"tgt_set_param\" has been called without the socket id." << std::endl;
    }
    m_tgt_if_units[SOCKET_ID]->tgt_set_param(param);
  }
  /* MULTIPLE SOCKET SUPPORT */
  void tgt_set_param(unsigned int id, const vpcl::tlm_tgt_if_parameter &param)
  {
    m_tgt_if_units[id]->tgt_set_param(param);
  }

  void tgt_init_param()
  {
    if (N > 1) {
//      std::cout << "warning : the method \"tgt_init_param\" has been called without the socket id." << std::endl;
    }
    m_tgt_if_units[SOCKET_ID]->tgt_init_param();
  }
  /* MULTIPLE SOCKET SUPPORT */
  void tgt_init_param(unsigned int id)
  {
    m_tgt_if_units[id]->tgt_init_param();
  }

  std::string tgt_handle_command(const std::vector<std::string> &args)
  {
    std::string message = "";
    std::vector<std::string> command_args(args);
    if ((command_args.size() > 1) && (command_args[0] == "tgt")) {
      char *endptr;
      unsigned int socket_id = strtoul(command_args[1].c_str(), &endptr, 0);
      if (*endptr == '\0') {
        if (socket_id < N) {
          command_args.erase(command_args.begin() + 1);
          message += m_tgt_if_units[socket_id]->tgt_handle_command(command_args);
        }
        /* error : specified socket id is invalid */
      }
      else {
        for (unsigned int i = 0; i < N; i++) {
          message += m_tgt_if_units[i]->tgt_handle_command(command_args);
        }
      }
    }
    return message;
  }

  std::string tgt_import_command_file(const std::string &file_name)
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
        command_msg += tgt_handle_command(command_args);
      }
    }
    /* file closing */
    fclose(p_file);
    return command_msg;
  }

protected:
  bool tgt_get_gp_attribute(tlm::tlm_command &command, sc_dt::uint64 &address, unsigned char *&data_ptr, unsigned int &data_length, const tlm::tlm_generic_payload &trans, bool is_debug = false) const
  {
    return m_tgt_if_units[SOCKET_ID]->get_gp_attribute(command, address, data_ptr, data_length, trans, is_debug);
  }

  void tgt_enable_reset(bool is_reset) {
    for (unsigned int i = 0; i < N; i++) {
      m_tgt_if_units[i]->tgt_enable_reset(is_reset);
    }
  }

private:
  /* Transport Interface (LT and AT) */
  virtual bool tgt_rd(unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext)
  {
    std::cout << "error : neither the virtual method \"tgt_rd\" nor \"tgt_acc\" is overridden." << std::endl;
    sc_stop();
    return false;
  }

  virtual bool tgt_wr(unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext)
  {
    std::cout << "error : neither the virtual method \"tgt_wr\" nor \"tgt_acc\" is overridden." << std::endl;
    sc_stop();
    return false;
  }

  virtual void tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t)
  {
    m_tgt_if_units[m_tmp_id]->tgt_acc_default(trans, t);
  }
  /* MULTIPLE SOCKET SUPPORT */
  virtual void tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
  {
    if (N > 1) {
//      std::cout << "warning : the virtual method \"tgt_acc\" is not overridden with the socket id." << std::endl;
    }
    m_tmp_id = id;
    tgt_acc(trans, t);
  }

  /* Transport Interface (AT) */
  virtual void tgt_req_rx(tlm::tlm_generic_payload &trans, sc_time &t)
  {
    m_tgt_if_units[m_tmp_id]->tgt_req_rx_default(trans, t);
  }
  /* MULTIPLE SOCKET SUPPORT */
  virtual void tgt_req_rx(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
  {
    m_tmp_id = id;
    tgt_req_rx(trans, t);
  }

protected:
  void tgt_resp_tx(tlm::tlm_generic_payload &trans)
  {
    if (N > 1) {
//      std::cout << "warning : the method \"tgt_resp_tx\" has been called without the socket id." << std::endl;
    }
    m_tgt_if_units[SOCKET_ID]->tgt_resp_tx(trans);
  }
  /* MULTIPLE SOCKET SUPPORT */
  void tgt_resp_tx(unsigned int id, tlm::tlm_generic_payload &trans)
  {
    m_tgt_if_units[id]->tgt_resp_tx(trans);
  }

private:
  virtual void tgt_resp_free(tlm::tlm_generic_payload &trans)
  {
    m_tgt_if_units[m_tmp_id]->tgt_resp_free_default(trans);
  }
  /* MULTIPLE SOCKET SUPPORT */
  virtual void tgt_resp_free(unsigned int id, tlm::tlm_generic_payload &trans)
  {
    m_tmp_id = id;
    tgt_resp_free(trans);
  }

  /* Direct Memory Interface */
  virtual bool tgt_get_dmi(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data)
  {
//    std::cout << "info : the virtual method \"tgt_get_dmi\" is not overridden." << std::endl;
    trans.set_dmi_allowed(false);
    return false;
  }
  /* MULTIPLE SOCKET SUPPORT */
  virtual bool tgt_get_dmi(unsigned int id, tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data)
  {
    m_tmp_id = id;
    return tgt_get_dmi(trans, dmi_data);
  }

protected:
  void tgt_reset_dmi(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
  {
    m_tgt_if_units[SOCKET_ID]->tgt_reset_dmi(start_range, end_range);
  }
  /* MULTIPLE SOCKET SUPPORT */
  void tgt_reset_dmi(unsigned int id, sc_dt::uint64 start_range, sc_dt::uint64 end_range)
  {
    if (N > 1) {
//      std::cout << "warning : the method \"tgt_reset_dmi\" has been called without the socket id." << std::endl;
    }
    m_tgt_if_units[m_tmp_id]->tgt_reset_dmi(start_range, end_range);
  }

private:
  /* Debug Transport Interface */
  virtual bool tgt_rd_dbg(unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext)
  {
    std::cout << "warning : neither the virtual method \"tgt_rd_dbg\" nor \"tgt_acc_dbg\" is overridden." << std::endl;
    return false;
  }

  virtual bool tgt_wr_dbg(unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext)
  {
    std::cout << "warning : neither the virtual method \"tgt_wr_dbg\" nor \"tgt_acc_dbg\" is overridden." << std::endl;
    return false;
  }

  virtual unsigned int tgt_acc_dbg(tlm::tlm_generic_payload &trans)
  {
    return m_tgt_if_units[m_tmp_id]->tgt_acc_dbg_default(trans);
  }
  /* MULTIPLE SOCKET SUPPORT */
  virtual unsigned int tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
  {
    if (N > 1) {
//      std::cout << "warning : the virtual method \"tgt_acc_dbg\" is not overridden with the socket id." << std::endl;
    }
    m_tmp_id = id;
    return tgt_acc_dbg(trans);
  }
};

}

#endif
