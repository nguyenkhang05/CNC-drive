/*************************************************************************
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_MEV_ADAPTER_H
#define  G4_MEV_ADAPTER_H

#include "systemc.h"
#include "OSCI2.h"
#include "global.h"

class G4_MevAdptr:
public sc_module,
public virtual TlmFwTransportIf,
public virtual TlmBwTransportIf
{
public:
  sc_in<uint64>                      clk; // clock port
  TlmInitiatorSocket<BUS_WIDTH_VCI32>* is;  // VCI32 BUS initiator socket
  TlmTargetSocket<BUS_WIDTH_VCI32>*    ts;  // VCI32 BUS target socket

  /* constructor */
  G4_MevAdptr( sc_module_name name );

  /* destructor */
  virtual ~G4_MevAdptr( void );

  /* set functions */
  void setPeNum       ( unsigned int peNum );
  void setClkDivision ( unsigned int peid, unsigned int clkDiv );
  void setReadLatency ( uint64 latency );
  void setWriteLatency( uint64 latency );

  /* override functions */
  TlmSyncEnum  nb_transport_fw( TlmTransactionType& trans,
                                TlmPhase&           phase,
                                sc_core::sc_time&   t );
  TlmSyncEnum  nb_transport_bw( TlmTransactionType& trans,
                                TlmPhase&           phase,
                                sc_core::sc_time&   t );
  void         b_transport    ( TlmTransactionType& trans,
                                sc_core::sc_time&   t );
  unsigned int transport_dbg  ( TlmTransactionType& trans );
  void         invalidate_direct_mem_ptr( uint64 startRange,
                                          uint64 endRange );
  bool         get_direct_mem_ptr( TlmTransactionType& trans,
                                   tlm::tlm_dmi&       dmiData );

private:
  /* private variables */
  const char*    mpModName;    // module name
  char           mErrMsg[128]; // for error message
  unsigned int   mPeNum;       // number of PE
  unsigned int** mpClkDiv;     // clock division
  uint64         mRLatency;    // read latency
  uint64         mWLatency;    // write latency

  /* private functions */
  sc_time getClkPeriod( void );
};

#endif // G4_MEV_ADAPTER_H
