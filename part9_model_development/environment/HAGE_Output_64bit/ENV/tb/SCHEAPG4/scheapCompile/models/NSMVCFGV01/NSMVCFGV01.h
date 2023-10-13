/*
* Copyright(c) 2013 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __NSMVCFGV01_H__
#define __NSMVCFGV01_H__

#include "systemc.h"
#include "global.h"
#include "BusSlaveBase.h"
#include "BusTimeBase.h"

class  NSMVG4CPUV01;
class  CfgFunc;

class NSMVCFGV01 : public sc_module, public BusSlaveBase<BUS_WIDTH_VPI,1>, public BusTimeBase
{

 public:
  sc_in<bool>                     resetz; // reset port
  sc_in<sc_dt::uint64>            clk;    // clock port
  TlmTargetSocket<BUS_WIDTH_VPI> *ts;     // target socket
  
  SC_HAS_PROCESS( NSMVCFGV01 );
  NSMVCFGV01( sc_module_name name, NSMVG4CPUV01* pG3mcpu ); 

  ~NSMVCFGV01(void);

 private:
  CfgFunc    *mpCfgFunc; // fuctional module (dummy of CF guard register)

  /* processes */
  void ResetProc( void );
};

#endif // __NSMVCFGV01_H__
