/*************************************************************************
 *
 *  T_SELECTOR.h
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

#ifndef T_SELECTOR_H
#define T_SELECTOR_H

#include "global.h"
#include "OSCI2.h"

template<unsigned int BUSWIDTH, unsigned int LT_BUSWIDTH> class T_SELECTOR : 
public sc_module,
virtual public TlmFwTransportIf,
virtual public TlmBwTransportIf
{

 public:
  TlmTargetSocket<BUSWIDTH>       ts;  // target socket
  TlmInitiatorSocket<BUSWIDTH>    isa; // initaiator socket(for CA ISS)
  TlmInitiatorSocket<LT_BUSWIDTH> isl; // initaiator socket(for fast ISS)

  /// <summary>constructor</summary>
  /// <param name="moduleName">name of sc_module</param>
  T_SELECTOR( sc_module_name moduleName ):
    ts("ts"),
    isa("isa"),
    isl("isl"),
    mSimMode(SIM_MODE_CA)
  {
    ts(*this);
    isa(*this);
    isl(*this);
  }
    
  /// <summary>destructor</summary>
  ~T_SELECTOR( void ){}
    
  /// <summary>transport API for LT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="phase">TLM phase</param>
  /// <param name="t">time</param>
  /// <returns>none</returns>
  TlmSyncEnum nb_transport_fw( TlmTransactionType& trans,
			       TlmPhase& phase,
			       sc_core::sc_time& t )
  {
#ifdef EX_MASTER_TEST_LOG
    printf("%s:T_SELECTOR::nb_transport_fw: addr=0x%08x\n", 
	   sc_time_stamp().to_string().c_str(),
	   trans.get_address());
    fflush(stdout);
#endif
    return(isa->nb_transport_fw( trans, phase, t ));
  }
  
  /// <summary>transport API for LT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="t">time</param>
  /// <returns>none</returns>
  void b_transport( TlmTransactionType& trans, sc_core::sc_time& t )
  {
#ifdef EX_MASTER_TEST_LOG
    printf("%s:T_SELECTOR::b_transport: addr=0x%08x\n", 
	   sc_time_stamp().to_string().c_str(),
	   trans.get_address());
    fflush(stdout);
#endif
    return isl->b_transport( trans, t );
  }
  
  /// <summary>transport API for debug access</summary>
  /// <param name="r">transaction</param>
  /// <returns>data number</returns>
  unsigned int transport_dbg( TlmTransactionType& r )
  {
    if( mSimMode == SIM_MODE_CA ){
      return isa->transport_dbg( r );
    }
    else{
      return isl->transport_dbg( r );
    }
  }
  
  /// <summary>Dummy function (DMI access is not supported)</summary>
  /// <param name="trans">transaction</param>
  /// <param name="dmi_data">permissible access(read or(and) write)</param>
  /// <returns>accept or reject</returns>
  bool get_direct_mem_ptr( TlmTransactionType& trans,
			   tlm::tlm_dmi&  dmi_data)
  {
    return false;
  }

  /// <summary>Dummy function (DMI access is not supported)</summary>
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
    TlmG3mExtension *pG3mExtension = NULL;
    trans.get_extension( pG3mExtension );

#ifdef ERR_MSG_2_1_3_TEST
    if(pG3mExtension) trans.clear_extension( pG3mExtension );
    trans.get_extension( pG3mExtension );
#endif 
    
    if( !pG3mExtension ){
      // error (not support TlmG3mExtension)
      char errorMessageBuf[1024];
      sprintf( errorMessageBuf,
	       "Received transaction doesn't support the TlmG3mExtension." );
      CmErrMsg::printErrorMessage( this->name(), 0, errorMessageBuf );
      return tlm::TLM_ACCEPTED;
    }
    else{
      return(pG3mExtension->getIfPointer()->nb_transport_bw(trans, phase, t));
    }
  }
  
  /// <summary>Dummy function (DMI access is not supported)</summary>
  /// <param name="start_range">start range for DMI access</param>
  /// <param name="end_range">end rang for DMI access</param>
  /// <returns>none</returns>
  void invalidate_direct_mem_ptr( sc_dt::uint64 start_range,
				  sc_dt::uint64 end_range )
  {}

  /// <summary>set transport type</summary>
  /// <param name="mode">simulation mode</param>
  /// <returns>none</returns>
  void setTlmTransType( SIM_MODE_T mode ){
    mSimMode = mode;
  }

 private:
  SIM_MODE_T mSimMode; // simulation mode

};

#endif

