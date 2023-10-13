/*************************************************************************
 *
 *  VCI2VCI32.cpp
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

#include "VCI2VCI32_core.h"
#include "VCI2VCI32.h"
#include "CmErrMsg.h"  // for error message


using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">name of sc_module</param>
VCI2VCI32::
VCI2VCI32( sc_module_name moduleName ) :
  BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32>( moduleName ),
  BusBridgeSlaveBase<BUS_WIDTH_VCI,1>(),
  BusMasterBase<BUS_WIDTH_VCI32,1>(),
  vciFreqPort( "vciFreqPort" ),
  vci32FreqPort( "vci32FreqPort" ),
  ts( (TlmTargetSocket<BUS_WIDTH_VCI> *)0 ),
  is( (TlmInitiatorSocket<BUS_WIDTH_VCI32> *)0 )
{

#ifdef BUS_TST
  char fileName[1024];
  strcpy(fileName, moduleName);
  strcat(fileName, ".log");
  if( (fp=fopen(fileName,"w")) == NULL ){
    printf("\nVCI2VCI32:UT test file open error!!\n\n");
    fflush(stdout);
  }
#endif //BUS_TST

  setSlaveResetPort( &resetPort ); // setting for reset port of slave I/F
  setSlaveFreqPort( &vciFreqPort ); // setting for frequency port of slave I/F
  setTargetSocket( "ts" ); // setting for target socket
  ts = tSocket[0]; //Alias for binding

  setMasterResetPortVci32( &resetPort ); // setting for reset port of master I/F
  setMasterFreqPortVci32( &vci32FreqPort ); // setting for frequency port of master I/F
  setInitiatorSocketVci32( "is" ); // setting for initiator socket
  is = iSocketVci32[0];     // Alias used for connection

  mpCore = new VCI2VCI32_core( this, mBusSlaveIf[0], mBusMasterIfVci32[0] );
  
  mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mpCore );
  mBusSlaveIf[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusSlaveIf[0]->setBusProtocol( BUS_VCI );
  
  mBusMasterIfVci32[0]->setFuncModulePtr( (BusMasterFuncIf *)mpCore );
  mBusMasterIfVci32[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusMasterIfVci32[0]->setBusProtocol( BUS_VCI );

  mpCore->setBridgeFwLatency( 1 );
  mpCore->setBridgeBwLatency( 0 );
  mpCore->setTransSplitter( true );

}

/// <summary>destructor</summary>
VCI2VCI32::
~VCI2VCI32( void )
{
  delete mpCore;
}

