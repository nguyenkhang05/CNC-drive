/*
* Copyright(c) 2014 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef _NSMVWTBUFV01__H__
#define _NSMVWTBUFV01__H__

#include "NSMVWTBUFV01.h"
#include "NSMVG4CPUV01.h"
#include "WtbufFunc.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pG3mcpu">pointer of NSMVG4CPUV01's instance</param>
/// <returns>none</returns>
NSMVWTBUFV01::NSMVWTBUFV01( sc_module_name name, NSMVG4CPUV01* pG3mcpu ):
  sc_module( name ),
  BusSlaveBase<BUS_WIDTH_VPI,1>(),
  mpWtbufFunc(0)
{
  setSlaveResetPort32( &resetz );
  setSlaveFreqPort32( &clk );
  setTargetSocket32( "ts" );
  
  ts = tSocket32[0]; //Alias for binding
  
  char inst_name[128];
  sprintf( inst_name, "%s.WtbufFunc", this->name() );
  mpWtbufFunc = new WtbufFunc( inst_name );
  mpWtbufFunc->setG3mcpuPtr( pG3mcpu );
  mBusSlaveIf32[0]->setFuncModulePtr( mpWtbufFunc );
  mBusSlaveIf32[0]->setBusProtocol( BUS_VPI );
  mBusSlaveIf32[0]->setReadLatency( 1 );
  mBusSlaveIf32[0]->setWriteLatency( 1 );

  // reset process
  SC_METHOD( ResetProc );
  sensitive << resetz.pos();
  dont_initialize();
}

/// <summary>destructor</summary>
/// <returns>none</returns>
NSMVWTBUFV01::~NSMVWTBUFV01(void)
{
  delete mpWtbufFunc;
}


/// <summary>reset process</summary>
/// <returns>none</returns>
void NSMVWTBUFV01::ResetProc( void )
{
#if defined(TEST_GRD_DBG) || defined(TEST_RESET_DBG)
  printf("%s:%s:RestProc invoked.\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name());
  fflush(stdout);
#endif
  if(resetz.read()){
    mpWtbufFunc->reset();
  }
}

#endif
