/*************************************************************************
 *
 *  AXI2AHB.cpp
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

#include "AXI2AHB_core.h"
#include "AXI2AHB.h"
#include "CmErrMsg.h"  // for error message


using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">name of sc_module</param>
AXI2AHB::
AXI2AHB( sc_module_name moduleName ) :
  BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_AHB>( moduleName ),
  BusBridgeSlaveBase<BUS_WIDTH_AXI,1>(),
  BusMasterBase<BUS_WIDTH_AHB,1>(),
  axiFreqPort( "axiFreqPort" ),
  ahbFreqPort( "ahbFreqPort" ),
  ts( (TlmTargetSocket<BUS_WIDTH_AXI> *)0 ),
  is( (TlmInitiatorSocket<BUS_WIDTH_AHB> *)0 )
{

#ifdef BUS_TST
  char fileName[1024];
  strcpy(fileName, moduleName);
  strcat(fileName, ".log");
  if( (fp=fopen(fileName,"w")) == NULL ){
    printf("\nAXI2AHB:UT test file open error!!\n\n");
    fflush(stdout);
  }
#endif //BUS_TST

  setSlaveResetPort( &resetPort ); // setting for reset port of slave I/F
  setSlaveFreqPort( &axiFreqPort ); // setting for frequency port of slave I/F
  setTargetSocket( "ts" ); // setting for target socket
  ts = tSocket[0]; //Alias for binding

  setMasterResetPortAhb( &resetPort ); // setting for reset port of master I/F
  setMasterFreqPortAhb( &ahbFreqPort ); // setting for frequency port of master I/F
  setInitiatorSocketAhb( "is" ); // setting for initiator socket
  is = iSocketAhb[0];     // Alias used for connection

  mpCore = new AXI2AHB_core( this, mBusSlaveIf[0], mBusMasterIfAhb[0] );
  
  mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mpCore );
  mBusSlaveIf[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusSlaveIf[0]->setBusProtocol( BUS_AXI );
  
  mBusMasterIfAhb[0]->setFuncModulePtr( (BusMasterFuncIf *)mpCore );
  mBusMasterIfAhb[0]->setTransNmbLmt( 1 );
  mBusMasterIfAhb[0]->setBusProtocol( BUS_AHB );

  mpCore->setBridgeFwLatency( 2 );
  mpCore->setBridgeBwLatency( 1 );

}

/// <summary>destructor</summary>
AXI2AHB::
~AXI2AHB( void )
{
  delete mpCore;
}

