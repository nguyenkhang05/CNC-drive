/*************************************************************************
 * Copyright(c) 2013-2015 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  NSMVSWD0PCADV01_H
#define  NSMVSWD0PCADV01_H

#include <list>
#include "systemc.h"
#include "global.h"
#include "BusSlaveBase.h"
#include "BusTimeBase.h"

class  Swd0pcadFunc;
class  NSMVG4CPUV01;

class NSMVSWD0PCADV01 : public sc_module, public BusSlaveBase<BUS_WIDTH_APB, 1>, public BusTimeBase
{

public:
  /* ports for system */
  sc_in<bool>   resetz; // reset port
  sc_in<uint64> clk;    // clock port

  /* ports for OR circuit */
  sc_in<unsigned int> start_addr; // input poort for Inital value from SCDS

  /* port for APB BUS */
  TlmTargetSocket<BUS_WIDTH_APB>* ts; // APB BUS target socket

  /* constructor */
  SC_HAS_PROCESS( NSMVSWD0PCADV01 );
  NSMVSWD0PCADV01( sc_module_name name, NSMVG4CPUV01* pG4cpu);

  /* destructor */
  virtual ~NSMVSWD0PCADV01( void );

  /* set function */
  void setRegAccSize( unsigned int size );
  void clrRegAccSize( void );
  void setPeId( unsigned int peid );

private:
  /* private variables */
  Swd0pcadFunc* mpSwd0pcadFunc; // pointer to Swd0pcadFunc instance
  char          mErrMsg[64];    // for error message

  /* private function */
  void   resetFunc( void );

  /* processes */
  void InitProc( void );
  void StartAddrUpdateProc( void );
  void ResetProc( void );
};

#endif // NSMVSWD0PCADV01_H
