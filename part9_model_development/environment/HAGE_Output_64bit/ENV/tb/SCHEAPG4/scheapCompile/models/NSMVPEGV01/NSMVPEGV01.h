/*
* Copyright(c) 2016 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __NSMVPEGV01_H__
#define __NSMVPEGV01_H__

#include "systemc.h"
#include "global.h"
#include "BusSlaveBase.h"
#include "BusMasterBase.h"
#include "BusTimeBase.h"

class  NSMVG4CPUV01;
class  PegFunc;

class NSMVPEGV01 : public sc_module, public BusSlaveBase<BUS_WIDTH_APB,1>, public BusMasterBase<BUS_WIDTH_APB,1>, public BusTimeBase
{

 public:
  sc_in<bool>                     resetz; // reset port
  sc_in<sc_dt::uint64>            clk;    // clock port
  TlmTargetSocket<BUS_WIDTH_APB> *ts;     // target socket
  TlmInitiatorSocket<BUS_WIDTH_APB> *is;  // initiator socket
  
  SC_HAS_PROCESS( NSMVPEGV01 );
  NSMVPEGV01( sc_module_name name, NSMVG4CPUV01* pG4cpu ); 

  ~NSMVPEGV01(void);

 private:
  PegFunc    *mpPegFunc; // fuctional module (dummy of PE guard register)

  /* processes */
  void ResetProc( void );
};

#endif // __NSMVPEGV01_H__
