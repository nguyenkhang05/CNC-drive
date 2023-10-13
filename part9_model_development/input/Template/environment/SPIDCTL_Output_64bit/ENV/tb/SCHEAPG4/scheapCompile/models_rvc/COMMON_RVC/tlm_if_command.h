// $Id: tlm_if_command.h,v 1.9 2013/12/04 11:20:03 watanama Exp $

/*************************************************************************
 *
 *  tlm_if_command.h
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

#ifndef __TLM_IF_COMMAND_H__
#define __TLM_IF_COMMAND_H__

#include "systemc.h"
#include <cstring>
#include <list>
#include <vector>
#include <string>
#include <sstream>

#include "re_security.h"

namespace vpcl
{

class tlm_if_command
{
public:
  tlm_if_command(std::string name, unsigned int bus_width)
  : m_name(name)
  , m_bus_width(bus_width)  // byte
  {
    m_msg_out_lvl = TLM_IF_ERR_LV;
    m_access_mode = TLM_IF_LT_ACCESS;
    m_phase_mode  = TLM_IF_SINGLE_PHASE;
  }

  virtual ~tlm_if_command() {
  }


  // tlm_ini_if api function (called by derived class)
  std::string ini_handle_command(const std::vector<std::string> &args, tlm_ini_if_parameter &param)
  {
    m_msg_out_lvl = param.msg_out_lvl;
    m_access_mode = param.access_mode;

    std::string ng_header = "Error ("   + (std::string)get_name() + ") ";
    std::string in_header = "["    + (std::string)get_name() + "]  ";
    std::ostringstream msg;

    if (args[0] != "ini") {
      return "";
    }
    else if (args.size() == 1) {
      return (ng_header + "command is not specified\n");
    }

    if(args[1] == "set_param") {
      return handle_ini_set_param(args, param);
    }
    else if (args[1] == "get_param") {
      return handle_ini_get_param(args, param);
    }
    else if (args[1] == "init_param") {
      param.init(param.name, param.bus_width);
      return "";
    }
    else if (args[1] == "set_max_outstanding") {
      unsigned int id = str_to_src(args[2]);
      unsigned int max = str_to_src(args[3]);
      if ((max > 0) && (param.m_outstanding_configs[id].get_count() == 0) && param.m_outstanding_configs[id].queue_is_empty()) {
        param.m_outstanding_configs[id].set_max(max);
        return "";
      }
      else {
        return "error : calling the method \"ini_set_max_outstanding\" has failed, the argument \"max\" is invalid or the outstanding configuration is in use.";
      }
    }
    else if (args[1] == "help") {
      msg << (std::string)get_name() << " has the following initiator I/F commands.\n";
      msg << "\n";
      msg << "    Command                                 Description\n";
      msg << "    -----------------------------------------------------------------------------------------\n";
      msg << "    set_param <term> <value> : Set simulation information about access to target.\n";
      msg << "              <term>  : m_bus_clk | m_bus_gnt | m_bus_rgnt | m_buf_size\n";
      msg << "                       | m_src_id | m_burst_width | m_burst_length\n";
      msg << "                       | m_access_mode | m_p_log_file | m_wr_log | m_rd_log\n";
      msg << "                       | m_msg_out_lvl | m_endian_big | m_latency_log | m_dmi_size\n";
      msg << "                       | m_wr_resp_latency | m_rd_resp_latency | m_bw_resp_sync\n";
      msg << "                       | m_info_displayed\n";
      msg << "              <value> : Please see tlm_common_class spec sheet.\n";
      msg << "\n";
      msg << "    get_param <term>         : Get simulation information about access to target.\n";
      msg << "\n";
      msg << "    init_param               : Initialize simulation information.\n";
      msg << "\n";
      msg << "    set_max_outstanding <id> <max> : Set the maximum number of outstanding.\n";
      return msg.str();
    }
    else {
      msg << "[" << (std::string)get_name() << "]  ERROR : command name \"" << args[1] << "\" or argument size \"" << args.size() - 2 << "\" is invalid.\n";
      return msg.str();
    }
  }


  // tlm_tgt_if api function (called by derived class)
  std::string tgt_handle_command(const std::vector<std::string> &args, tlm_tgt_if_parameter &param)
  {
    m_msg_out_lvl = param.msg_out_lvl;
    m_phase_mode  = param.phase_mode;

    std::string ng_header = "Error ("   + (std::string)get_name() + ") ";
    std::string ok_header = "Success (" + (std::string)get_name() + ") ";
    std::string in_header = "["    + (std::string)get_name() + "]  ";

    std::ostringstream msg;

    if (args[0] != "tgt") {
      return "";
    }
    else if (args.size() == 1) {
      return (ng_header + "command is not specified\n");
    }

    if(args[1] == "set_param") {
      return handle_tgt_set_param(args, param);
    }
    else if (args[1] == "get_param") {
      return handle_tgt_get_param(args, param);
    }
    else if (args[1] == "init_param") {
      param.init(param.name, param.bus_width);
      return "";
    }
    else if (args[1] == "help") {
      msg << (std::string)get_name() << " has the following target I/F commands.\n";
      msg << "\n";
      msg << "    Command                                 Description\n";
      msg << "    -----------------------------------------------------------------------------------------\n";
      msg << "    set_param <term> <value> : Set simulation information about access to target.\n";
      msg << "              <term>  : m_bus_clk | m_bus_gnt | m_bus_rgnt | m_buf_size\n";
      msg << "                       | m_wr_latency | m_rd_latency | m_phase_mode\n";
      msg << "                       | m_p_log_file | m_wr_log | m_rd_log | m_msg_out_lvl\n";
      msg << "                       | m_wr_req_latency | m_rd_req_latency | m_fw_req_phase\n";
      msg << "                       | m_info_displayed\n";
      msg << "              <value> : Please see tlm_common_class spec sheet.\n";
      msg << "\n";
      msg << "    get_param <term>         : Get simulation information about access to target.\n";
      msg << "\n";
      msg << "    init_param               : Initialize simulation information.\n";
      return msg.str();
    }
    else {
      msg << "[" << (std::string)get_name() << "]  ERROR : command name \"" << args[1] << "\" or argument size \"" << args.size() - 2 << "\" is invalid.\n";
      return msg.str();
    }
  }

private:
  // instance name of derived class
  const std::string m_name;

  // bus_width
  unsigned int m_bus_width;

  // parameter
  vpcl::tlm_if_msg_lv  m_msg_out_lvl;
  vpcl::tlm_if_access  m_access_mode;
  vpcl::tlm_if_phase   m_phase_mode;


  std::string handle_ini_set_param(const std::vector<std::string> &args, tlm_ini_if_parameter &param)
  {
    std::ostringstream msg;

    int loop = args.size();

    for ( int i = 2; i < loop; i++ ) {
      std::list<std::string> strList = split(args[i], "=");
      if (strList.size() == 2) { 

        std::list<std::string>::iterator iter = strList.begin();

        while( iter != strList.end() ) // Get to end.
        {
          if (*iter == "m_name") {
            msg << "[" << (std::string)get_name() << "]  ERROR : Can't Set Core Instance Name.\n";
            return msg.str();
          }
          else if (*iter == "m_bus_width") {
            msg << "[" << (std::string)get_name() << "]  ERROR : Can't Set Bus Width.\n";
            return msg.str();
          }
          else if (*iter == "m_bus_clk") {
            int val = 0; std::string unit = "";
            unit = GetVarAndUnit(*(++iter), val);
            param.bus_clk          = to_sc_time(val, unit);
          }
          else if (*iter == "m_bus_gnt") {
            int val = 0; std::string unit = "";
            unit = GetVarAndUnit(*(++iter), val);
            param.bus_gnt          = to_sc_time(val, unit);
          }
          else if (*iter == "m_bus_rgnt") {
            int val = 0; std::string unit = "";
            unit = GetVarAndUnit(*(++iter), val);
            param.bus_rgnt        = to_sc_time(val,unit);
          }
          else if (*iter == "m_buf_size") {
            param.buf_size     = str_to_src(*(++iter));
          }
          else if (*iter == "m_src_id") {
            param.src_id       = str_to_src(*(++iter));
          }
          else if (*iter == "m_burst_width") {
            param.burst_width  = str_to_src(*(++iter));
          }
          else if (*iter == "m_burst_length") {
            param.burst_length = str_to_src(*(++iter));
          }
          else if (*iter == "m_access_mode") {
            param.access_mode  = str_to_access(*(++iter));
          }
          else if (*iter == "m_p_log_file") {
            param.p_log_file   = str_to_p_log_file(*(++iter));
          }
          else if (*iter == "m_wr_log") {
            param.wr_log       = str_to_log(*(++iter));
          }
          else if (*iter == "m_rd_log") {
            param.rd_log       = str_to_log(*(++iter));
          }
          else if (*iter == "m_msg_out_lvl") {
            param.msg_out_lvl  = str_to_msg_lvl(*(++iter));
          }
          else if (*iter == "m_endian_big") {
            param.endian_big   = str_to_bool(*(++iter));
          }
          else if (*iter == "m_latency_log") {
            param.latency_log  = str_to_bool(*(++iter));
          }
          else if (*iter == "m_dmi_size") {
            param.dmi_size     = str_to_src(*(++iter));
          }
          else if (*iter == "m_wr_resp_latency") {
            param.wr_resp_latency = str_to_time(*(++iter), param.bus_clk);
          }
          else if (*iter == "m_rd_resp_latency") {
            param.rd_resp_latency = str_to_time(*(++iter), param.bus_clk);
          }
          else if (*iter == "m_bw_resp_sync") {
            param.bw_resp_sync = str_to_sync(*(++iter));
          }
          else if (*iter == "m_info_displayed") {
            param.info_displayed = str_to_bool(*(++iter));
          }
          else {
            msg << "[" << (std::string)get_name() << "]  ERROR : set parameter name \"" << *iter << "\" is invalid.\n";
            return msg.str();
          }
          // Increment.
          iter++;
        }
      }
      else {
        if (strncmp((strList.begin())->c_str(),"#",1) == 0) { // Comment Proccess.
          break;
        }
        else {
          msg << "[" << (std::string)get_name() << "]  ERROR : set parameter name \"" << *(strList.begin()) << "\" is invalid format.\n";
        }
        return msg.str();
      }
    }

    return "";
  }

  std::string handle_ini_get_param(const std::vector<std::string> &args, tlm_ini_if_parameter &param)
  {
    std::string in_header = "["    + (std::string)get_name() + "]  ";
    std::ostringstream msg;

    int loop = args.size();
    for ( int i = 2; i < loop; i++ ) {
      if (args[i] == "m_name") {
        msg << in_header << "m_name = "   << param.name << " ";
      }
      else if (args[i] == "m_bus_width") {
        msg << in_header << "m_bus_width = "   << param.bus_width << " ";
      }
      else if (args[i] == "m_bus_clk") {
        msg << in_header << "m_bus_clk = "   << param.bus_clk.to_string() << " ";
      }
      else if (args[i] == "m_bus_gnt") {
        msg << in_header << "m_bus_gnt = "   << param.bus_gnt.to_string() << " ";
      }
      else if (args[i] == "m_bus_rgnt") {
        msg << in_header << "m_bus_rgnt = "   << param.bus_rgnt.to_string() << " ";
      }
      else if (args[i] == "m_buf_size") {
        msg << in_header << "m_buf_size = "   << param.buf_size << " ";
      }
      else if (args[i] == "m_src_id") {
        msg << in_header << "m_src_id = "   << param.src_id << " ";
      }
      else if (args[i] == "m_burst_width") {
        msg << in_header << "m_burst_width = "   << param.burst_width << " ";
      }
      else if (args[i] == "m_burst_length") {
        msg << in_header << "m_burst_length = "   << param.burst_length << " ";
      }
      else if (args[i] == "m_access_mode") {
        msg << in_header << "m_access_mode = "   << to_c_str(param.access_mode) << " ";
      }
      else if (args[i] == "m_p_log_file") {
        msg << in_header << "m_p_log_file = "   << to_c_str(param.p_log_file) << " ";
      }
      else if (args[i] == "m_wr_log") {
        msg << in_header << "m_wr_log = "   << to_c_str(param.wr_log) << " ";
      }
      else if (args[i] == "m_rd_log") {
        msg << in_header << "m_rd_log = "   << to_c_str(param.rd_log) << " ";
      }
      else if (args[i] == "m_msg_out_lvl") {
        msg << in_header << "m_msg_out_lvl = "   << to_c_str(param.msg_out_lvl) << " ";
      }
      else if (args[i] == "m_endian_big") {
        msg << in_header << "m_endian_big = "   << to_str(param.endian_big) << " ";
      }
      else if (args[i] == "m_latency_log") {
        msg << in_header << "m_latency_log = "   << to_str(param.latency_log) << " ";
      }
      else if (args[i] == "m_dmi_size") {
        msg << in_header << "m_dmi_size = "   << param.dmi_size << " ";
      }
      else if (args[i] == "m_wr_resp_latency") {
        msg << in_header << "m_wr_resp_latency = " << param.wr_resp_latency.to_string() << " ";
      }
      else if (args[i] == "m_rd_resp_latency") {
        msg << in_header << "m_rd_resp_latency = " << param.rd_resp_latency.to_string() << " ";
      }
      else if (args[i] == "m_bw_resp_sync") {
        msg << in_header << "m_bw_resp_sync = " << sync_to_str(param.bw_resp_sync) << " ";
      }
      else if (args[i] == "m_info_displayed") {
        msg << in_header << "m_info_displayed = " << to_str(param.info_displayed) << " ";
      }
      else if (strncmp(args[i].c_str(),"#",1) == 0) {
        break;
      }
      else {
        msg << "[" << (std::string)get_name() << "]  ERROR : get parameter name \"" << args[i] << "\" is invalid.\n";
      }
      msg << "\n";
    }
    return msg.str();
  }


  std::string handle_tgt_set_param(const std::vector<std::string> &args, tlm_tgt_if_parameter &param)
  {
    std::ostringstream msg;

    int loop = args.size();

    for ( int i = 2; i < loop; i++ ) {
      std::list<std::string> strList = split(args[i], "=");
      if (strList.size() == 2) {

        std::list<std::string>::iterator iter = strList.begin();

        while( iter != strList.end() ) { // Get to end.

          if (*iter == "m_name") {
            msg << "[" << (std::string)get_name() << "]  ERROR : Can't Set Core Instance Name.\n";
            return msg.str();
          }
          else if (*iter == "m_bus_width") {
            msg << "[" << (std::string)get_name() << "]  ERROR : Can't Set Bus Width.\n";
            return msg.str();
          }
          else if (*iter == "m_bus_clk") {
            int val = 0; std::string unit = "";
            unit = GetVarAndUnit(*(++iter), val);
            param.bus_clk      = to_sc_time(val, unit);
          }
          else if (*iter == "m_bus_gnt") {
            int val = 0; std::string unit = "";
            unit = GetVarAndUnit(*(++iter), val);
            param.bus_gnt      = to_sc_time(val, unit);
          }
          else if (*iter == "m_bus_rgnt") {
            int val = 0; std::string unit = "";
            unit = GetVarAndUnit(*(++iter), val);
            param.bus_rgnt     = to_sc_time(val,unit);
          }
          else if (*iter == "m_buf_size") {
            param.buf_size     = str_to_src(*(++iter));
          }
          else if (*iter == "m_wr_latency") {
            int val = 0; std::string unit = "";
            unit = GetVarAndUnit(*(++iter), val);
            param.wr_latency   = to_sc_time(val,unit); // args[i+2] == UNIT
          }
          else if (*iter == "m_rd_latency") {
            int val = 0; std::string unit = "";
            unit = GetVarAndUnit(*(++iter), val);
            param.rd_latency   = to_sc_time(val,unit); // args[i+2] == UNIT
          }
          else if (*iter == "m_phase_mode") {
            param.phase_mode   = str_to_phase_mode(*(++iter));
          }
          else if (*iter == "m_p_log_file") {
            param.p_log_file   = str_to_p_log_file(*(++iter));
          }
          else if (*iter == "m_wr_log") {
            param.wr_log       = str_to_log(*(++iter));
          }
          else if (*iter == "m_rd_log") {
            param.rd_log       = str_to_log(*(++iter));
          }
          else if (*iter == "m_msg_out_lvl") {
            param.msg_out_lvl  = str_to_msg_lvl(*(++iter));
          }
          else if (*iter == "m_wr_req_latency") {
            param.wr_req_latency = str_to_time(*(++iter), param.bus_clk);
          }
          else if (*iter == "m_rd_req_latency") {
            param.rd_req_latency = str_to_time(*(++iter), param.bus_clk);
          }
          else if (*iter == "m_fw_req_phase") {
            param.fw_req_phase = str_to_phase(*(++iter));
          }
          else if (*iter == "m_info_displayed") {
            param.info_displayed = str_to_bool(*(++iter));
          }
          else {
            msg << "[" << (std::string)get_name() << "]  ERROR : parameter name \"" << *iter << "\" is invalid.\n";
            return msg.str();
          }
          // Increment
          iter++;
        }  
      }
      else {
        if (strncmp((strList.begin())->c_str(),"#",1) == 0) { // Comment Proccess.
          break;
        }
        else {
          msg << "[" << (std::string)get_name() << "]  ERROR : set parameter name \"" << *(strList.begin()) << "\" is invalid format.\n";
        }
        return msg.str();
      }
    }

    return "";
  }

  std::string handle_tgt_get_param(const std::vector<std::string> &args, tlm_tgt_if_parameter &param)
  {
    std::string in_header = "["    + (std::string)get_name() + "]  ";

    std::ostringstream msg;

    int loop = args.size();
    for ( int i = 2; i < loop; i++ ) {
      if (args[i] == "m_name") {
        msg << in_header << "m_name = "   << param.name << " ";
      }
      else if (args[i] == "m_bus_width") {
        msg << in_header << "m_bus_width = "   << param.bus_width << " ";
      }
      else if (args[i] == "m_bus_clk") {
        msg << in_header << "m_bus_clk = "   << param.bus_clk.to_string() << " ";
      }
      else if (args[i] == "m_bus_gnt") {
        msg << in_header << "m_bus_gnt = "   << param.bus_gnt.to_string() << " ";
      }
      else if (args[i] == "m_bus_rgnt") {
        msg << in_header << "m_bus_rgnt = "   << param.bus_rgnt.to_string() << " ";
      }
      else if (args[i] == "m_buf_size") {
        msg << in_header << "m_buf_size = "   << param.buf_size << " ";
      }
      else if (args[i] == "m_wr_latency") {
        msg << in_header << "m_wr_latency = "   << param.wr_latency.to_string() << " ";
      }
      else if (args[i] == "m_rd_latency") {
        msg << in_header << "m_rd_latency = "   << param.rd_latency.to_string() << " ";
      }
      else if (args[i] == "m_phase_mode") {
        msg << in_header << "m_phase_mode = "   << to_c_str(param.phase_mode) << " ";
      }
      else if (args[i] == "m_p_log_file") {
        msg << in_header << "m_p_log_file = "   << to_c_str(param.p_log_file) << " ";
      }
      else if (args[i] == "m_wr_log") {
        msg << in_header << "m_wr_log = "   << to_c_str(param.wr_log) << " ";
      }
      else if (args[i] == "m_rd_log") {
        msg << in_header << "m_rd_log = "   << to_c_str(param.rd_log) << " ";
      }
      else if (args[i] == "m_msg_out_lvl") {
        msg << in_header << "m_msg_out_lvl = "   << to_c_str(param.msg_out_lvl) << " ";
      }
      else if (args[i] == "m_wr_req_latency") {
        msg << in_header << "m_wr_req_latency = " << param.wr_req_latency.to_string() << " ";
      }
      else if (args[i] == "m_rd_req_latency") {
        msg << in_header << "m_rd_req_latency = " << param.rd_req_latency.to_string() << " ";
      }
      else if (args[i] == "m_fw_req_phase") {
        msg << in_header << "m_fw_req_phase = " << phase_to_str(param.fw_req_phase) << " ";
      }
      else if (args[i] == "m_info_displayed") {
        msg << in_header << "m_info_displayed = " << to_str(param.info_displayed) << " ";
      }
      else if (strncmp(args[i].c_str(),"#",1) == 0) {
        break;
      }
      else {
        msg << "[" << (std::string)get_name() << "]  ERROR : get parameter name \"" << args[i] << "\" is invalid.\n";
      }
      msg << "\n";
    }
    return msg.str();
  }

  const char *get_name()
  {
    return m_name.c_str();
  }

  std::string GetVarAndUnit(std::string str, int &val) {
    std::list<std::string> varList = split(str, ",");
    std::list<std::string>::iterator varIter = varList.begin();
    if (varList.size() != 2) {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"GetVarAndUnit\" Bad Command Parameter Format = %s.\n", get_name(), varIter->c_str());
      return "";
    }
    else {
      val = str_to_src(*varIter);
      varIter++;
      return( *varIter );
    }
  }

  std::list<std::string> split(std::string str, std::string delim)
  {
    std::list<std::string> result;
    size_t cutAt;
    while( (cutAt = str.find_first_of(delim)) != std::string::npos)
    {
        if(cutAt > 0)
        {
            result.push_back(str.substr(0, cutAt));
        }
        str = str.substr(cutAt + 1);
    }
    if(str.length() > 0)
    {
        result.push_back(str);
    }
    return result;
  }

  FILE *str_to_p_log_file(const std::string &str)
  {
    FILE *pdump;
    errno_t err;

    if (str == "NULL") {
      return NULL;
    }
    else if (str == "stdout") {
      return stdout;
    }
    else {
      err = fopen_s(&pdump, str.c_str(), "a");
      if (err != 0) {
        dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"str_to_p_log_file\" can't open designate file.\n", get_name());
        return NULL;
      }
      return pdump;
    }
  }

  bool str_to_log(const std::string &str)
  {
    if (str == "HIDE" || str == "false") {
      return false;
    }
    else if (str == "DISPLAY" || str == "true") {
      return true;
    }
    else {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"str_to_log\" No Support Log Output Mode.\n", get_name());
      return false;
    }
  }

  const char *to_c_str(FILE *p_log_file)
  {
    if (p_log_file == stdout) {
      return "stdout";
    }
    else {
      return "file";
    }
  }

  const char *to_c_str(bool log)
  {
    if (log == false) {
      return "HIDE";
    }
    else {
      return "DISPLAY";
    }
  }

  const char *to_c_str(tlm_if_access access_mode)
  {
    if (access_mode == vpcl::TLM_IF_LT_ACCESS) {
      return "TLM_IF_LT_ACCESS";
    }
    else {
      return "TLM_IF_AT_ACCESS";
    }
  }

  const char *to_c_str(tlm_if_phase if_phase)
  {
    if (if_phase == vpcl::TLM_IF_SINGLE_PHASE) {
      return "TLM_IF_SINGLE_PHASE";
    }
    else {
      return "TLM_IF_MULTI_PHASE";
    }
  }

  const char *to_c_str(tlm_if_msg_lv msg_out_lvl)
  {
    if (msg_out_lvl == vpcl::TLM_IF_NO_OUT) {
      return "TLM_IF_NO_OUT";
    }
    else if (msg_out_lvl == vpcl::TLM_IF_ERR_LV) {
      return "TLM_IF_ERR_LV";
    }
    else if (msg_out_lvl == vpcl::TLM_IF_WAR_LV) {
      return "TLM_IF_WAR_LV";
    }
    else if (msg_out_lvl == vpcl::TLM_IF_NML_LV) {
      return "TLM_IF_NML_LV";
    }
    else if (msg_out_lvl == vpcl::TLM_IF_DBGL_LV) {
      return "TLM_IF_DBGL_LV";
    }
    else {
      return "TLM_IF_DBGH_LV";
    }
  }

  vpcl::tlm_if_msg_lv str_to_msg_lvl(const std::string &str)
  {
     if (str == "TLM_IF_NO_OUT") {
       return vpcl::TLM_IF_NO_OUT;
     }
     else if (str == "TLM_IF_ERR_LV") {
       return vpcl::TLM_IF_ERR_LV;
     }
     else if (str == "TLM_IF_WAR_LV") {
       return vpcl::TLM_IF_WAR_LV;
     }
     else if (str == "TLM_IF_NML_LV") {
       return vpcl::TLM_IF_NML_LV;
     }
     else if (str == "TLM_IF_DBGL_LV") {
       return vpcl::TLM_IF_DBGL_LV;
     }
     else if (str == "TLM_IF_DBGH_LV") {
       return vpcl::TLM_IF_DBGH_LV;
     }
     else {
       dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"src_to_msg_lvl\" No Support Message Level. Not Change Message Level.\n", get_name());
       return m_msg_out_lvl;
     }
  }

  unsigned int str_to_src(const std::string &str)
  {
    char *endp = NULL;
    if (str == "BUSWIDTH") {
      return m_bus_width;
    }
    else if (str == "UINT_MAX") {
      return UINT_MAX;
    }
    else {
      return (unsigned int) strtoul(str.c_str(), &endp, 0);
    }
  }

  sc_time to_sc_time(int val,std::string unit)
  {
    if ( unit == "SC_FS" ) {
      return sc_time(val, SC_FS);
    }
    else if ( unit == "SC_PS" ) {
      return sc_time(val, SC_PS);
    }
    else if ( unit == "SC_NS" ) {
      return sc_time(val, SC_NS);
    }
    else if ( unit == "SC_US" ) {
      return sc_time(val, SC_US);
    }
    else if ( unit == "SC_MS" ) {
      return sc_time(val, SC_MS);
    }
    else if ( unit == "SC_SEC" ) {
      return sc_time(val, SC_SEC);
    }
    else {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"to_sc_time\" No Support sc_time Unit = %s.\n", get_name(), unit.c_str());
      return sc_time(0, SC_FS);
    }
  }

  vpcl::tlm_if_access str_to_access(const std::string &str)
  {
     if (str == "TLM_IF_LT_ACCESS") {
       return vpcl::TLM_IF_LT_ACCESS;
     }
     else if (str == "TLM_IF_AT_ACCESS") {
       return vpcl::TLM_IF_AT_ACCESS;
     }
     else {
       dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"str_to_access\" No Support Access Type = %s. Not Change Access Type.\n", get_name(), str.c_str());
       return m_access_mode;
     }
  }

  vpcl::tlm_if_phase str_to_phase_mode(const std::string &str)
  {
     if (str == "TLM_IF_SINGLE_PHASE") {
       return vpcl::TLM_IF_SINGLE_PHASE;
     }
     else if (str == "TLM_IF_MULTI_PHASE") {
       return vpcl::TLM_IF_MULTI_PHASE;
     }
     else {
       dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"str_to_phase_mode\" No Support Phase Mode = %s. Phase Mode Must Be MULTI Or SINGLE. Phase Mode Not Change.\n", get_name(), str.c_str());
       return m_phase_mode;
     }
  }

  bool str_to_bool(const std::string &str)
  {
     if (str == "false") {
       return false;
     }
     else if (str == "true") {
       return true;
     }
     else {
       dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \"str_to_bool\" No Suppport Endian Big Type = %s. Endian Big is Boolean type. Not Change Endian Big Type.\n", get_name(), str.c_str());
       return m_access_mode;
     }
  }

  std::string to_str(bool endian_big)
  {
    if (endian_big == false) {
      return "false";
    }
    else {
      return "true";
    }
  }

  tlm::tlm_sync_enum str_to_sync(const std::string &str)
  {
    tlm::tlm_sync_enum sync = tlm::TLM_COMPLETED;

    if (str == "TLM_ACCEPTED") {
      sync = tlm::TLM_ACCEPTED;
    }
    else if (str == "TLM_UPDATED") {
      sync = tlm::TLM_UPDATED;
    }
    else if (str == "TLM_COMPLETED") {
      sync = tlm::TLM_COMPLETED;
    }
    else {
      dbg_printf(TLM_IF_ERR_LV, "tlm_if_command.h : str_to_sync : error : unsupported parameter setting %s \n", str.c_str());
    }
    return sync;
  }

  tlm::tlm_phase str_to_phase(const std::string &str)
  {
    tlm::tlm_phase phase = tlm::END_RESP;

    if (str == "BEGIN_REQ") {
      phase = tlm::BEGIN_REQ;
    }
    else if (str == "END_REQ") {
      phase = tlm::END_REQ;
    }
    else if (str == "BEGIN_RESP") {
      phase = tlm::BEGIN_RESP;
    }
    else if (str == "END_RESP") {
      phase = tlm::END_RESP;
    }
    else {
      dbg_printf(TLM_IF_ERR_LV, "tlm_if_command.h : str_to_phase : error : unsupported parameter setting %s \n", str.c_str());
    }
    return phase;
  }

  sc_time str_to_time(const std::string &str, sc_time clk)
  {
    sc_time time = SC_ZERO_TIME;
    int time_value = 0;
    std::string time_unit = "SC_PS";

    time_unit = GetVarAndUnit(str, time_value);
    if (time_unit == "CYC") {
      time = clk * time_value;
    }
    else if ((time_unit == "SC_PS") || (time_unit == "SC_NS") || (time_unit == "SC_US") || (time_unit == "SC_MS") || (time_unit == "SC_SEC")) {
      time = to_sc_time(time_value, time_unit);
    }
    else {
      dbg_printf(TLM_IF_ERR_LV, "tlm_if_command.h : str_to_time : error : unsupported parameter setting %s \n", str.c_str());
    }
    return time;
  }

  std::string sync_to_str(tlm::tlm_sync_enum sync)
  {
    std::string str = "UNKNOWN";

    if (sync == tlm::TLM_ACCEPTED) {
      str = "TLM_ACCEPTED";
    }
    else if (sync == tlm::TLM_UPDATED) {
      str = "TLM_UPDATED";
    }
    else if (sync == tlm::TLM_COMPLETED) {
      str = "TLM_COMPLETED";
    }
    else {
      dbg_printf(TLM_IF_ERR_LV, "tlm_if_command.h : sync_to_str : error : unknown parameter setting \n");
    }
    return str;
  }

  std::string phase_to_str(tlm::tlm_phase phase)
  {
    std::string str = "UNKNOWN";

    if (phase == tlm::BEGIN_REQ) {
      str = "BEGIN_REQ";
    }
    else if (phase == tlm::END_REQ) {
      str = "END_REQ";
    }
    else if (phase == tlm::BEGIN_RESP) {
      str = "BEGIN_RESP";
    }
    else if (phase == tlm::END_RESP) {
      str = "END_RESP";
    }
    else {
      dbg_printf(TLM_IF_ERR_LV, "tlm_if_command.h : phase_to_str : error : unknown parameter setting \n");
    }
    return str;
  }

  int dbg_printf(int dbglevel, const char *format, ... )
  {
    va_list arg;
    int rc;
                                                                                
    if ( format == NULL ) {
      printf( "ERROR(TLM_COMMON_IF) : dbg_printf module get NULL pointer.\n" );
      sc_stop();
    }
                                                                                
    if (dbglevel <= m_msg_out_lvl) {
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

}

#endif
