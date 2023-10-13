/*************************************************************************
 *
 *  AXI2VPI.cpp
 *
 *  Copyright(c) 2015 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#include "AXI2VPI_core.h"
#include "AXI2VPI.h"
#include "CmErrMsg.h"  // for error message


using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">name of sc_module</param>
AXI2VPI::
AXI2VPI( sc_module_name moduleName ) :
  BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI>( moduleName ),
  BusBridgeSlaveBase<BUS_WIDTH_AXI,1>(),
  BusMasterBase<BUS_WIDTH_VPI,1>(),
  axiFreqPort( "axiFreqPort" ),
  vpiFreqPort( "vpiFreqPort" ),
  ts( (TlmTargetSocket<BUS_WIDTH_AXI> *)0 ),
  is( (TlmInitiatorSocket<BUS_WIDTH_VPI> *)0 )
{

#ifdef BUS_TST
  char fileName[1024];
  strcpy(fileName, moduleName);
  strcat(fileName, ".log");
  if( (fp=fopen(fileName,"w")) == NULL ){
    printf("\nAXI2VPI:UT test file open error!!\n\n");
    fflush(stdout);
  }
#endif //BUS_TST

  setSlaveResetPort( &resetPort ); // setting for reset port of slave I/F
  setSlaveFreqPort( &axiFreqPort ); // setting for frequency port of slave I/F
  setTargetSocket( "ts" ); // setting for target socket
  ts = tSocket[0]; //Alias for binding

  setMasterResetPortVpi( &resetPort ); // setting for reset port of master I/F
  setMasterFreqPortVpi( &vpiFreqPort ); // setting for frequency port of master I/F
  setInitiatorSocketVpi( "is" ); // setting for initiator socket
  is = iSocketVpi[0];     // Alias used for connection

  mpCore = new AXI2VPI_core( this, mBusSlaveIf[0], mBusMasterIfVpi[0] );
  
  mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mpCore );
  mBusSlaveIf[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusSlaveIf[0]->setBusProtocol( BUS_AXI );
  
  mBusMasterIfVpi[0]->setFuncModulePtr( (BusMasterFuncIf *)mpCore );
  mBusMasterIfVpi[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusMasterIfVpi[0]->setBusProtocol( BUS_VPI );

  mpCore->setBridgeFwLatency( 0 );
  mpCore->setBridgeBwLatency( 0 );
  mpCore->setTransSplitter( true );

}

/// <summary>destructor</summary>
AXI2VPI::
~AXI2VPI( void )
{
  delete mpCore;
}

