/*************************************************************************
 *
 *  VCI2VPI.cpp
 *
 *  Copyright(c) 2011 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#include "VCI2VPI_core.h"
#include "VCI2VPI.h"
#include "CmErrMsg.h"  // for error message


using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">name of sc_module</param>
VCI2VPI::
VCI2VPI( sc_module_name moduleName ) :
  BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_VPI>( moduleName ),
  BusBridgeSlaveBase<BUS_WIDTH_VCI,1>(),
  BusMasterBase<BUS_WIDTH_VPI,1>(),
  vciFreqPort( "vciFreqPort" ),
  vpiFreqPort( "vpiFreqPort" ),
  ts( (TlmTargetSocket<BUS_WIDTH_VCI> *)0 ),
  is( (TlmInitiatorSocket<BUS_WIDTH_VPI> *)0 )
{

#ifdef BUS_TST
  char fileName[1024];
  strcpy(fileName, moduleName);
  strcat(fileName, ".log");
  if( (fp=fopen(fileName,"w")) == NULL ){
    printf("\nVCI2VPI:UT test file open error!!\n\n");
    fflush(stdout);
  }
#endif //BUS_TST

  setSlaveResetPort( &resetPort ); // setting for reset port of slave I/F
  setSlaveFreqPort( &vciFreqPort ); // setting for frequency port of slave I/F
  setTargetSocket( "ts" ); // setting for target socket
  ts = tSocket[0]; //Alias for binding

  setMasterResetPortVpi( &resetPort ); // setting for reset port of master I/F
  setMasterFreqPortVpi( &vpiFreqPort ); // setting for frequency port of master I/F
  setInitiatorSocketVpi( "is" ); // setting for initiator socket
  is = iSocketVpi[0];     // Alias used for connection

  mpCore = new VCI2VPI_core( this, mBusSlaveIf[0], mBusMasterIfVpi[0] );
  
  mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mpCore );
  mBusSlaveIf[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusSlaveIf[0]->setBusProtocol( BUS_VCI );
  
  mBusMasterIfVpi[0]->setFuncModulePtr( (BusMasterFuncIf *)mpCore );
  mBusMasterIfVpi[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusMasterIfVpi[0]->setBusProtocol( BUS_VPI );

  mpCore->setBridgeFwLatency( 2 );
  mpCore->setBridgeBwLatency( 1 );
  mpCore->setTransSplitter( true );

}

/// <summary>destructor</summary>
VCI2VPI::
~VCI2VPI( void )
{
  delete mpCore;
}

