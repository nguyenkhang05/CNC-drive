/*
* Copyright(c) 2016-2017 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __NSMVCRGV01_H__
#define __NSMVCRGV01_H__

#include "systemc.h"
#include "global.h"
#include "BusSlaveBase.h"
#include "BusTimeBase.h"

class  NSMVG4CPUV01;
class  CrgFunc;

class NSMVCRGV01 : public sc_module, public BusSlaveBase<BUS_WIDTH_APB,1>, public BusTimeBase
{

 public:
  sc_in<bool>                     resetz; // reset port
  sc_in<sc_dt::uint64>            clk;    // clock port
  TlmTargetSocket<BUS_WIDTH_APB> *ts;     // target socket
  
  SC_HAS_PROCESS( NSMVCRGV01 );
  NSMVCRGV01( sc_module_name name, NSMVG4CPUV01* pG4cpu ); 

  ~NSMVCRGV01(void);

 private:
  CrgFunc    *mpCrgFunc; // fuctional module (dummy of CRAM guard register)

  /* processes */
  void ResetProc( void );
};

#endif // __NSMVCRGV01_H__
