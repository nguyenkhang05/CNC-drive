/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  NSMVRAMZEROINGV01_H
#define  NSMVRAMZEROINGV01_H

#include "systemc.h"
#include "BusMasterBase.h"

class RamZeroingFunc;

class NSMVRAMZEROINGV01:
  public sc_module,
  public BusMasterBase<64, 1>
{
public:
  /* ports for system */
  sc_in<uint64>       clk_ram;   // clock port
  sc_in<bool>         rst_rresz; // reset port
  sc_in<unsigned int> zero_mode; // mode port
  sc_out<bool>        zero_ack;  // ack port

  /* TLM ports */
  TlmInitiatorSocket<64>* is; // TLM initiator socket

  /* constructor */
  SC_HAS_PROCESS( NSMVRAMZEROINGV01 );
  NSMVRAMZEROINGV01( sc_module_name name, const char* pCfgFileName );

  /* destructor */
  virtual ~NSMVRAMZEROINGV01( void );


  void setParameter( unsigned int stAddr,
                     unsigned int mxAddr,
                     unsigned int asAddr,
                     bool         selin );
  void writeMemValue( unsigned int           addr,
                      unsigned long long int data,
                      unsigned int           byte );
  unsigned long long int readMemValue( unsigned int addr, unsigned int byte );

private:
  /* variables */
  const char*     mpModName;   // module name
  RamZeroingFunc* mpR0Func;    // pointer to RamZeroingFunc instance
  bool            mIsParamSet; // parameter-set flag

  /* processes */
  void ResetProc( void );
};

#endif // NSMVRAMZEROINGV01_H
