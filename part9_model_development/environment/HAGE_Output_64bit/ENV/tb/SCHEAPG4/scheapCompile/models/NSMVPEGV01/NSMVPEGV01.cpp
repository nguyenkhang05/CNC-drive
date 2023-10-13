/*
* Copyright(c) 2016-2017 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#include "NSMVPEGV01.h"
#include "NSMVG4CPUV01.h"
#include "PegFunc.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pG4cpu">pointer of NSMVG4CPUV01's instance</param>
/// <returns>none</returns>
NSMVPEGV01::NSMVPEGV01( sc_module_name name, NSMVG4CPUV01* pG4cpu ):
  sc_module( name ),
  BusSlaveBase<BUS_WIDTH_APB,1>(),
  BusMasterBase<BUS_WIDTH_APB,1>(),
  mpPegFunc(0)
{
  setSlaveResetPort32( &resetz );
  setSlaveFreqPort32( &clk );
  setTargetSocket32( "ts" );

  setMasterResetPort32( &resetz );
  setMasterFreqPort32( &clk );
  setInitiatorSocket32( "is" );
  
  ts = tSocket32[0]; //Alias for binding
  is = iSocket32[0]; //Alias for binding
  
  char inst_name[128];
  sprintf( inst_name, "%s.CramGrdFunc", this->name() );
  mpPegFunc = new PegFunc( inst_name, mBusMasterIf32[0] );
  mpPegFunc->setG4cpuPtr( pG4cpu );
  mBusSlaveIf32[0]->setFuncModulePtr( mpPegFunc );
  mBusSlaveIf32[0]->setBusProtocol( BUS_APB );
  mBusSlaveIf32[0]->setReadLatency( 1 );
  mBusSlaveIf32[0]->setWriteLatency( 1 );

  mBusMasterIf32[0]->setFuncModulePtr( mpPegFunc);
  mBusMasterIf32[0]->setBusProtocol( BUS_APB );
  mBusMasterIf32[0]->setTransNmbLmt( 1 );
  mBusMasterIf32[0]->set0Latency();
  mBusMasterIf32[0]->setTlmTransType( pG4cpu->GetSimMode() );

  // reset process
  SC_METHOD( ResetProc );
  sensitive << resetz.pos();
  dont_initialize();
}

/// <summary>destructor</summary>
/// <returns>none</returns>
NSMVPEGV01::~NSMVPEGV01(void)
{
  delete mpPegFunc;
}


/// <summary>reset process</summary>
/// <returns>none</returns>
void NSMVPEGV01::ResetProc( void )
{
#if defined(TEST_GRD_DBG) || defined(TEST_RESET_DBG)
  printf("%s:%s:RestProc invoked.\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name());
  fflush(stdout);
#endif
  if(resetz.read()){
    mpPegFunc->reset();
  }
}

