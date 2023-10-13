// $Id: tlm_tgt_if_base.h,v 1.4 2010/09/14 08:03:49 hanakish Exp $
// =============================================================
//  file name : tlm_tgt_if_base.h
// =============================================================

#ifndef __TLM_TGT_IF_BASE_H__
#define __TLM_TGT_IF_BASE_H__

#include <systemc.h>
#include <tlm.h>
#include "tlm_utils/peq_with_get.h"

#include "tlm_if.h"
#include "tlm_tgt_if_tagged.h"

#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <list>

#ifndef _WIN32
#include "re_security.h"
#endif

namespace vpcl
{

template <unsigned int BUSWIDTH = 32, unsigned int N_TGT = 1>
class tlm_tgt_if_base
{
public:
  tlm_tgt_if_base(sc_module_name name) : m_name(name)
  {
    for(unsigned int i=0; i<N_TGT; i++) {
      std::ostringstream sname;
      std::ostringstream ifname;
      sname << name << "_socket_" << i;
      ifname << name << "_if_" << i;

      m_tgt_socket[i] = new tlm::tlm_target_socket<BUSWIDTH>(sname.str().c_str());
      m_tgt_if    [i] = new vpcl::tlm_tgt_if_tagged<BUSWIDTH, N_TGT>(ifname.str().c_str(), this, i);

      m_tgt_socket[i]->bind(*m_tgt_if[i]);
    }
  }

  virtual ~tlm_tgt_if_base() {
  }

  std::string tgt_handle_command(const std::vector<std::string> &args)
  {
    for(unsigned int i=0; i<N_TGT; i++) {
      m_tgt_if[i]->tgt_handle_command(args);
    }
    return("");
  }

  tlm::tlm_target_socket<BUSWIDTH> * m_tgt_socket[N_TGT];
  vpcl::tlm_tgt_if_tagged<BUSWIDTH, N_TGT> * m_tgt_if[N_TGT];

private:
  const std::string m_name;
  tlm_if_msg_lv m_msg_out_lvl;

  const char *tgt_get_name()
  {
    return m_name.c_str();
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

public:
  virtual bool tgt_wr(unsigned int socket_id, unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext) = 0;
  virtual bool tgt_wr(unsigned int socket_id, unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext, sc_time *p_time) {
     if (p_time != NULL) {
//       *p_time = SC_ZERO_TIME;
     }
     else {
       dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_wr\" get p_time NULL pointer.\n", tgt_get_name());
     }
     return tgt_wr(socket_id, addr, p_data, size, p_ext);
  }
  virtual bool tgt_rd(unsigned int socket_id, unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext) = 0;
  virtual bool tgt_rd(unsigned int socket_id, unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext, sc_time *p_time) {
     if (p_time != NULL) {
//       *p_time = SC_ZERO_TIME;
     }
     else {
       dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_rd\" get p_time NULL pointer.\n", tgt_get_name());
     }
     return tgt_rd(socket_id, addr, p_data, size, p_ext);
  }
  virtual bool tgt_wr_dbg(unsigned int socket_id, unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext) = 0;
  virtual bool tgt_rd_dbg(unsigned int socket_id, unsigned int addr, unsigned char *p_data, unsigned int size, vpcl::tlm_if_extension *p_ext) = 0;
  virtual void tgt_wr_dmi(unsigned int socket_id, unsigned int addr, vpcl::tlm_if_dmi *p_dmi)
  {
    if (p_dmi != NULL) {
      p_dmi->wr_allow = false;
      p_dmi->rd_allow = false;
    }
    else {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_wr_dmi\" get trans NULL pointer.\n", tgt_get_name());
    }
  }
  virtual void tgt_rd_dmi(unsigned int socket_id, unsigned int addr, vpcl::tlm_if_dmi *p_dmi)
  {
    if (p_dmi != NULL) {
      p_dmi->wr_allow = false;
      p_dmi->rd_allow = false;
    }
    else {
      dbg_printf(TLM_IF_ERR_LV,"[%s]  ERROR : function \" tgt_rd_dmi\" get trans NULL pointer.\n", tgt_get_name());
    }
  }

  virtual bool tgt_req_rx(unsigned int socket_id, vpcl::tlm_if_nb_transaction *p_nb_trans) = 0;

  virtual void tgt_resp_free(unsigned int socket_id, unsigned int buf_rem)
  {
  }

}; // tlm_tgt_if_base end

}  // vpcl namespace end

#endif
