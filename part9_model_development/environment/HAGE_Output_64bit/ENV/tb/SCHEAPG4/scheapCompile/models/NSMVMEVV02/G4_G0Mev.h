/*************************************************************************
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_G0MEV_H
#define  G4_G0MEV_H

#include "systemc.h"
#include "global.h"
#include "BusSlaveBase.h"

class G4_G0MevFunc;

class G4_G0Mev: public sc_module, public BusSlaveBase<BUS_WIDTH_VCI32, 1>
{
public:
  /* ports for system */
  sc_in<bool>   resetz; // reset port
  sc_in<uint64> clk;    // clock port

  TlmTargetSocket<BUS_WIDTH_VCI32>* ts; // APB BUS target socket

  /* constructor */
  SC_HAS_PROCESS( G4_G0Mev );
  G4_G0Mev( sc_module_name name );

  /* destructor */
  virtual ~G4_G0Mev( void );

  /* set functions */
  void setRegAddr( unsigned int addr );
  void setRegAccSize( unsigned int size );
  void clrRegAccSize( void );

  /* public functions */
  void resetFunc( void );

  /* public variables */
  const char* mpModName; // module name

private:
  /* private variables */
  G4_G0MevFunc* mpG4_G0MevFunc;  // pointer to MevFunc instance
  char       mErrMsg[128]; // for error message
};

#endif // G4_G0MEV_H
