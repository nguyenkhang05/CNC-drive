/*
* Copyright(c) 2013 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef _NSMVGRGV01__H__
#define _NSMVGRGV01__H__

#include "NSMVGRGV01.h"
#include "NSMVG4CPUV01.h"
#include "GrgFunc.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pG3mcpu">pointer of NSMVG4CPUV01's instance</param>
/// <returns>none</returns>
NSMVGRGV01::NSMVGRGV01( sc_module_name name, NSMVG4CPUV01* pG3mcpu ):
  sc_module( name ),
  BusSlaveBase<BUS_WIDTH_VPI,1>(),
  mpGrgFunc(0)
{
  setSlaveResetPort32( &resetz );
  setSlaveFreqPort32( &clk );
  setTargetSocket32( "ts" );
  
  ts = tSocket32[0]; //Alias for binding
  
  char inst_name[128];
  sprintf( inst_name, "%s.GramGrdFunc", this->name() );
  mpGrgFunc = new GrgFunc( inst_name );
  mpGrgFunc->setG3mcpuPtr( pG3mcpu );
  mBusSlaveIf32[0]->setFuncModulePtr( mpGrgFunc );
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
NSMVGRGV01::~NSMVGRGV01(void)
{
  delete mpGrgFunc;
}


/// <summary>reset process</summary>
/// <returns>none</returns>
void NSMVGRGV01::ResetProc( void )
{
#if defined(TEST_GRD_DBG) || defined(TEST_RESET_DBG)
  printf("%s:%s:RestProc invoked.\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name());
  fflush(stdout);
#endif
  if(resetz.read()){
    mpGrgFunc->reset();
  }
}

#endif
