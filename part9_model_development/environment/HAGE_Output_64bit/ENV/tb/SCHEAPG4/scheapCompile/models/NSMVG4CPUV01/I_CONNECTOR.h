/*************************************************************************
 *
 *  I_CONNECTOR.h
 *
 *  Copyright(c) 2011-2017 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#ifndef I_CONNECTOR_H
#define I_CONNECTOR_H

#include <map>
#include "OSCI2.h"

class I_CONNECTOR_ELEMENT
{
 public:
  /// <summary>constructor</summary>
  /// <param name="phase">TLM phase</param>
  /// <param name="if_ptr">Master IP's I/F Pointer</param>
  I_CONNECTOR_ELEMENT(TlmPhase       &phase,
		      TlmBwTransportIf *if_ptr
		      ) {
    m_phase    = phase;
    m_if_ptr   = if_ptr;
  }

  /// <summary>destructor</summary>
  ~I_CONNECTOR_ELEMENT() {}

  /// <summary>get TLM phase</summary>
  /// <returns>TLM phase</returns>
  TlmPhase           &getPhase() {return m_phase;}

  /// <summary>get Master IP's I/F pointer</summary>
  /// <returns>Master IP's I/F pointer</returns>
  TlmBwTransportIf   *getIf_ptr() {return m_if_ptr;}

  TlmPhase           m_phase;       // TLM phase
  TlmBwTransportIf   *m_if_ptr;     // Master IP's I/F pointer
};

class I_CONNECTOR_QUEUE
{
 public:
  /// <summary>constructor</summary>
  I_CONNECTOR_QUEUE() {
  }

  /// <summary>destructor</summary>
  ~I_CONNECTOR_QUEUE() {
    m_map_trans.clear();
  }

  /// <summary>set transaction data</summary>
  /// <param name="trans">transaction</param>
  /// <param name="phase">TLM phase</param>
  /// <param name="if_ptr">Master IP's I/F Pointer</param>
  /// <returns>none</returns>
  void setTrans(TlmTransactionType& trans,
		TlmPhase            &phase,
		TlmBwTransportIf*   if_ptr
		) {
    I_CONNECTOR_ELEMENT *elmnt = new I_CONNECTOR_ELEMENT(phase, if_ptr);
    m_map_trans[&trans] = elmnt;
  }

  /// <summary>get TLM phase from specified transaction data</summary>
  /// <param name="trans">transaction</param>
  /// <returns>TLM phase</returns>
  TlmPhase &getPhase(TlmTransactionType& trans) {
    I_CONNECTOR_ELEMENT *elm = m_map_trans[&trans];
    return elm->getPhase();
  }


  /// <summary>get Master IP's I/F pointer from specified transaction data</summary>
  /// <param name="trans">transaction</param>
  /// <returns>Master IP's I/F pointer</returns>
  TlmBwTransportIf *getIf_ptr(TlmTransactionType& trans) {
    I_CONNECTOR_ELEMENT *elm = m_map_trans[&trans];
    return elm->getIf_ptr();
  }


  /// <summary>delete transaction data from transaction map</summary>
  /// <param name="trans">transaction</param>
  /// <returns>none</returns>
  void delTrans(TlmTransactionType& trans) {
    I_CONNECTOR_ELEMENT *elm = m_map_trans[&trans];
    delete elm;
    std::map<TlmTransactionType*, I_CONNECTOR_ELEMENT* >::iterator it = m_map_trans.find( &trans );
    m_map_trans.erase(it);
  }

  std::map<TlmTransactionType*, I_CONNECTOR_ELEMENT* > m_map_trans; // transaction map
};

template< unsigned int BUSWIDTH >
class I_CONNECTOR : 
public sc_module,
public virtual TlmFwTransportIf,
public virtual TlmBwTransportIf
{

 public:
  TlmInitiatorSocket<BUSWIDTH> is;  //initiator socket
  TlmTargetSocket<BUSWIDTH>    ts;  //target socket

  SC_HAS_PROCESS( I_CONNECTOR );   
 
  /// <summary>constructor</summary>
  /// <param name="moduleName">name of sc_module</param>
  I_CONNECTOR( sc_module_name moduleName );

  /// <summary>destructor</summary>
  ~I_CONNECTOR( void );

  /// <summary>transport API for forward pass of AT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="phase">TLM phase</param>
  /// <param name="t">time</param>
  /// <returns>synchronization information</returns>
  TlmSyncEnum nb_transport_fw( TlmTransactionType& trans,
			       TlmPhase& phase,
			       sc_time& t );

  /// <summary>transport API for backward pass of AT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="phase">TLM phase</param>
  /// <param name="t">time</param>
  /// <returns>synchronization information</returns>
  TlmSyncEnum nb_transport_bw( TlmTransactionType& trans,
			       TlmPhase& phase,
			       sc_time& t );

  /// <summary>transport API for debug access</summary>
  /// <param name="r">transaction</param>
  /// <returns>data number</returns>
  unsigned int transport_dbg( TlmTransactionType& r );

  /// <summary>Dummy function (DMI access is not supported)</summary>
  /// <param name="startRange">start range for DMI access</param>
  /// <param name="endRange">end rang for DMI access</param>
  /// <returns>none</returns>
  void invalidate_direct_mem_ptr( sc_dt::uint64 startRange,
				  sc_dt::uint64 endRange );

  /// <summary>Dummy function (DMI access is not supported)</summary>
  /// <param name="trans">transaction</param>
  /// <param name="dmiData">permissible access(read or(and) write)</param>
  /// <returns>accept or reject</returns>
  bool get_direct_mem_ptr( TlmTransactionType& trans, tlm::tlm_dmi&  dmiData );

  /// <summary>transport API for LT</summary>
  /// <param name="trans">transaction</param>
  /// <param name="t">time</param>
  /// <returns>none</returns>
  void b_transport(TlmTransactionType& trans, sc_time& t);

  I_CONNECTOR_QUEUE m_queue;           // transaction queue
  TlmTransactionType *m_trans;         // transaction
  sc_time            *m_t;             // time
  TlmPhase           *m_phase;         // TLM phase
  sc_pvector< TlmTransactionType* > m_trans_queue; // transaction queue for re-sending
};


template< unsigned int BUSWIDTH >
I_CONNECTOR<BUSWIDTH>::
I_CONNECTOR( sc_module_name moduleName ) :
  sc_module( moduleName ),
  is( "is" ),
  ts( "ts" ),
  m_queue()
{
  is( *this );
  ts( *this );
}


template< unsigned int BUSWIDTH >
I_CONNECTOR<BUSWIDTH>::
~I_CONNECTOR( void )
{
  m_trans_queue.erase_all();
}


template< unsigned int BUSWIDTH >
TlmSyncEnum 
I_CONNECTOR<BUSWIDTH>::
nb_transport_fw( TlmTransactionType& trans,
		 TlmPhase& phase,
		 sc_time& t )
{
  TlmG3mExtension *pG3mExtension = (TlmG3mExtension *)0;
  trans.get_extension( pG3mExtension );

  if( !pG3mExtension ){
    // error (not support TlmG3mExtension)
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
             "Received transaction doesn't support the TlmG3mExtension." );
    CmErrMsg::printErrorMessage( this->name(), 0, errorMessageBuf );
    return tlm::TLM_ACCEPTED;
  }

  TlmBwTransportIf* if_ptr = pG3mExtension->getIfPointer();

  m_queue.setTrans(trans,
		   phase,
		   if_ptr);

  pG3mExtension->setIfPointer(this);

#ifdef DWACC_TEST
  sc_dt::uint64  dbg_addr   = trans.get_address();
  unsigned int   dbg_size   = trans.get_data_length();
  unsigned char* dbg_data_p = trans.get_data_ptr();
  printf("%s:I_CONNECTOR::nb_transport_fw : address=0x%08x, size=%d, t=%s, ", 
	 sc_time_stamp().to_string().c_str(),
	 (unsigned int)dbg_addr,
	 dbg_size,
	 t.to_string().c_str());
  for(int i=0; i<dbg_size; i++){
    printf(" data[0x%08x]=0x%02x,", (unsigned int)dbg_addr+i, *(dbg_data_p+i));
  }
  printf("\n");
  fflush(stdout);
#endif
#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("I_CONNECTOR::nb_transport_fw: time=%s, module name=%s\n", sc_time_stamp().to_string().c_str(), name());
  fflush(stdout);
#endif

  return( is->nb_transport_fw( trans, phase, t ) );
}


template< unsigned int BUSWIDTH >
TlmSyncEnum 
I_CONNECTOR<BUSWIDTH>::
nb_transport_bw( TlmTransactionType& trans,
		 TlmPhase& phase,
		 sc_time& t )
{
  TlmG3mExtension *pG3mExtension = (TlmG3mExtension *)0;
  trans.get_extension( pG3mExtension );

#ifdef ERR_MSG_2_1_1_TEST
  if(pG3mExtension) trans.clear_extension( pG3mExtension );
  trans.get_extension( pG3mExtension );
#endif 

#ifdef DWACC_TEST
  sc_dt::uint64  dbg_addr   = trans.get_address();
  unsigned int   dbg_size   = trans.get_data_length();
  unsigned char* dbg_data_p = trans.get_data_ptr();
  printf("%s:I_CONNECTOR::nb_transport_bw : address=0x%08x, size=%d, t=%s, ", 
	 sc_time_stamp().to_string().c_str(),
	 (unsigned int)dbg_addr,
	 dbg_size,
	 t.to_string().c_str());
  for(int i=0; i<dbg_size; i++){
    printf(" data[0x%08x]=0x%02x,", (unsigned int)dbg_addr+i, *(dbg_data_p+i));
  }
  printf("\n");
  fflush(stdout);
#endif
#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("I_CONNECTOR::nb_transport_bw: time=%s, module name=%s\n", sc_time_stamp().to_string().c_str(), name());
  fflush(stdout);
#endif

  if( !pG3mExtension ){
    // error (not support TlmG3mExtension)
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "Received transaction doesn't support the TlmG3mExtension." );
    CmErrMsg::printErrorMessage( this->name(), 0, errorMessageBuf );
    return tlm::TLM_ACCEPTED;
  }

  pG3mExtension->setIfPointer( m_queue.getIf_ptr(trans) );
  m_queue.delTrans(trans);

  return( pG3mExtension->getIfPointer()->nb_transport_bw(trans, phase, t) );
}


template< unsigned int BUSWIDTH >
unsigned int
I_CONNECTOR<BUSWIDTH>::
transport_dbg( TlmTransactionType& r )
{
#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("I_CONNECTOR::transport_dbg: time=%s, module name=%s\n", sc_time_stamp().to_string().c_str(), name());
  fflush(stdout);
#endif
  return( is->transport_dbg( r ) );
}


template< unsigned int BUSWIDTH >
void 
I_CONNECTOR<BUSWIDTH>::
invalidate_direct_mem_ptr( sc_dt::uint64 startRange,
			   sc_dt::uint64 endRange )
{
}


template< unsigned int BUSWIDTH >
bool
I_CONNECTOR<BUSWIDTH>::
get_direct_mem_ptr( TlmTransactionType& trans, tlm::tlm_dmi&  dmiData )
{
  return false;
}


template< unsigned int BUSWIDTH >
void
I_CONNECTOR<BUSWIDTH>::
b_transport(TlmTransactionType& trans, sc_time& t)
{
#ifdef BUSIF_TEST_LOG
  printf("\n");
  printf("I_CONNECTOR::b_transport: time=%s, module name=%s\n", sc_time_stamp().to_string().c_str(), name());
  fflush(stdout);
#endif

#ifdef DWACC_TEST
  sc_dt::uint64  dbg_addr   = trans.get_address();
  unsigned int   dbg_size   = trans.get_data_length();
  unsigned char* dbg_data_p = trans.get_data_ptr();
  printf("%s:I_CONNECTOR::b_transport2 : address=0x%08x, size=%d, ", 
	 sc_time_stamp().to_string().c_str(),
	 (unsigned int)dbg_addr,
	 dbg_size);
  for(int i=0; i<dbg_size; i++){
    printf(" data[0x%08x]=0x%02x,", (unsigned int)dbg_addr+i, *(dbg_data_p+i));
  }
  printf("\n");
  fflush(stdout);
#endif
  return( is->b_transport( trans, t ) );
}

#endif
