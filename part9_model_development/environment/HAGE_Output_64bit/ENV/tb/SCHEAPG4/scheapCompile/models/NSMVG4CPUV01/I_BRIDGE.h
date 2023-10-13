/*************************************************************************
 *
 *  I_BRIDGE.h
 *
 *  Copyright(c) 2012 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#ifndef I_BRIDGE_H
#define I_BRIDGE_H

#include "OSCI2.h"

template<unsigned int IS_BUSWIDTH, unsigned int TS_BUSWIDTH> class I_BRIDGE : 
public sc_module,
virtual public TlmFwTransportIf,
virtual public TlmBwTransportIf
{

 public:
  TlmInitiatorSocket<IS_BUSWIDTH> is; // initaiator socket
  TlmTargetSocket<TS_BUSWIDTH>    ts; // target socket

  /// <summary>constructor</summary>
  /// <param name="moduleName">name of sc_module</param>
  I_BRIDGE( sc_module_name moduleName ):
    is("is"),
    ts("ts")
  {
    is(*this);
    ts(*this);
  }
    
  /// <summary>destructor</summary>
  ~I_BRIDGE( void ){}
    
  /// <summary>transport API for forward pass of AT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="phase">TLM phase</param>
  /// <param name="t">time</param>
  /// <returns>synchronization information</returns>
  TlmSyncEnum nb_transport_fw( TlmTransactionType& trans,
			       TlmPhase& phase,
			       sc_core::sc_time& t )
  {
    is->nb_transport_fw( trans, phase, t );
    return tlm::TLM_UPDATED;
  }
  
  /// <summary>transport API for LT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="t">time</param>
  /// <returns>none</returns>
  void b_transport( TlmTransactionType& trans, sc_core::sc_time& t )
  {
    return is->b_transport( trans, t );
  }
  
  /// <summary>transport API for debug access</summary>
  /// <param name="r">transaction</param>
  /// <returns>data number</returns>
  unsigned int transport_dbg( TlmTransactionType& r )
  {
    return is->transport_dbg( r );
  }
  

  /// <summary>Dummy function --DMI access is not supported--</summary>
  /// <param name="trans">transaction</param>
  /// <param name="dmi_data">permissible access(read or(and) write)</param>
  /// <returns>accept or reject</returns>
  bool get_direct_mem_ptr( TlmTransactionType& trans,
			   tlm::tlm_dmi&  dmi_data)
  {
    return false;
  }

  /// <summary>Dummy function --DMI access is not supported--</summary>
  /// <returns>none</returns>
  void invalidate_dmi_method(){}

  /// <summary>transport API for backward pass of AT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="phase">TLM phase</param>
  /// <param name="t">time</param>
  /// <returns>synchronization information</returns>
  TlmSyncEnum nb_transport_bw( TlmTransactionType& trans,
			       TlmPhase& phase,
			       sc_core::sc_time& t )
  { 
    ts->nb_transport_bw( trans, phase, t );
    return tlm::TLM_COMPLETED;
  }
  

  /// <summary>Dummy function --DMI access is not supported--</summary>
  /// <param name="start_range">start range for DMI access</param>
  /// <param name="end_range">end range for DMI access</param>
  /// <returns>none</returns>
  void invalidate_direct_mem_ptr( sc_dt::uint64 start_range,
				  sc_dt::uint64 end_range )
  {}

 private:
  unsigned int    mPeId;      // for PE ID

};

#endif

