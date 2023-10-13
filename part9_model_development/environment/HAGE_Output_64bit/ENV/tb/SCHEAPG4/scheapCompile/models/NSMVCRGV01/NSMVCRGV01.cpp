/*
* Copyright(c) 2016-2017 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#include "NSMVCRGV01.h"
#include "NSMVG4CPUV01.h"
#include "CrgFunc.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pG4cpu">pointer of NSMVG4CPUV01's instance</param>
/// <returns>none</returns>
NSMVCRGV01::NSMVCRGV01( sc_module_name name, NSMVG4CPUV01* pG4cpu ):
  sc_module( name ),
  BusSlaveBase<BUS_WIDTH_APB,1>(),
  mpCrgFunc(0)
{
  setSlaveResetPort32( &resetz );
  setSlaveFreqPort32( &clk );
  setTargetSocket32( "ts" );
  
  ts = tSocket32[0]; //Alias for binding
  
  char inst_name[128];
  sprintf( inst_name, "%s.CramGrdFunc", this->name() );
  mpCrgFunc = new CrgFunc( inst_name );
  mpCrgFunc->setG4cpuPtr( pG4cpu );
  mBusSlaveIf32[0]->setFuncModulePtr( mpCrgFunc );
  mBusSlaveIf32[0]->setBusProtocol( BUS_APB );
  mBusSlaveIf32[0]->setReadLatency( 1 );
  mBusSlaveIf32[0]->setWriteLatency( 1 );

  // reset process
  SC_METHOD( ResetProc );
  sensitive << resetz.pos();
  dont_initialize();
}

/// <summary>destructor</summary>
/// <returns>none</returns>
NSMVCRGV01::~NSMVCRGV01(void)
{
  delete mpCrgFunc;
}


/// <summary>reset process</summary>
/// <returns>none</returns>
void NSMVCRGV01::ResetProc( void )
{
#if defined(TEST_GRD_DBG) || defined(TEST_RESET_DBG)
  printf("%s:%s:RestProc invoked.\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name());
  fflush(stdout);
#endif
  if(resetz.read()){
    mpCrgFunc->reset();
  }
}

