/*************************************************************************
 *
 *  AHB2AXI.cpp
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

#include "AHB2AXI_core.h"
#include "AHB2AXI.h"
#include "CmErrMsg.h"  // for error message


using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">name of sc_module</param>
AHB2AXI::
AHB2AXI( sc_module_name moduleName ) :
  BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI>( moduleName ),
  BusBridgeSlaveBase<BUS_WIDTH_AHB,1>(),
  BusMasterBase<BUS_WIDTH_AXI,1>(),
  ahbFreqPort( "ahbFreqPort" ),
  axiFreqPort( "axiFreqPort" ),
  ts( (TlmTargetSocket<BUS_WIDTH_AHB> *)0 ),
  is( (TlmInitiatorSocket<BUS_WIDTH_AXI> *)0 )
{

#ifdef BUS_TST
  char fileName[1024];
  strcpy(fileName, moduleName);
  strcat(fileName, ".log");
  if( (fp=fopen(fileName,"w")) == NULL ){
    printf("\nAHB2AXI:UT test file open error!!\n\n");
    fflush(stdout);
  }
#endif //BUS_TST

  setSlaveResetPort( &resetPort ); // setting for reset port of slave I/F
  setSlaveFreqPort( &ahbFreqPort ); // setting for frequency port of slave I/F
  setTargetSocket( "ts" ); // setting for target socket
  ts = tSocket[0]; //Alias for binding

  setMasterResetPort64( &resetPort ); // setting for reset port of master I/F
  setMasterFreqPort64( &axiFreqPort ); // setting for frequency port of master I/F
  setInitiatorSocket64( "is" ); // setting for initiator socket
  is = iSocket64[0];     // Alias used for connection

  mpCore = new AHB2AXI_core( this, mBusSlaveIf[0], mBusMasterIf64[0] );
  
  mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mpCore );
  mBusSlaveIf[0]->setTransNmbLmt( 1 );
  mBusSlaveIf[0]->setBusProtocol( BUS_AHB );
  
  mBusMasterIf64[0]->setFuncModulePtr( (BusMasterFuncIf *)mpCore );
  mBusMasterIf64[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusMasterIf64[0]->setBusProtocol( BUS_AXI );

  mpCore->setBridgeFwLatency( 0 );
  mpCore->setBridgeBwLatency( 0 );

}

/// <summary>destructor</summary>
AHB2AXI::
~AHB2AXI( void )
{
  delete mpCore;
}

