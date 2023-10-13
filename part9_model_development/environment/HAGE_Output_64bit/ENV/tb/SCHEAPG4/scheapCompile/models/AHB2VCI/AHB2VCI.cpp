/*************************************************************************
 *
 *  AHB2VCI.cpp
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

#include "AHB2VCI_core.h"
#include "AHB2VCI.h"
#include "CmErrMsg.h"  // for error message


using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">name of sc_module</param>
AHB2VCI::
AHB2VCI( sc_module_name moduleName ) :
  BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_VCI>( moduleName ),
  BusBridgeSlaveBase<BUS_WIDTH_AHB,1>(),
  BusMasterBase<BUS_WIDTH_VCI,1>(),
  ahbFreqPort( "ahbFreqPort" ),
  vciFreqPort( "vciFreqPort" ),
  ts( (TlmTargetSocket<BUS_WIDTH_AHB> *)0 ),
  is( (TlmInitiatorSocket<BUS_WIDTH_VCI> *)0 )
{

#ifdef LB_TST
  char fileName[1024];
  strcpy(fileName, moduleName);
  strcat(fileName, ".log");
  if( (fp=fopen(fileName,"w")) == NULL ){
    printf("\nAHB2VCI:UT test file open error!!\n\n");
    fflush(stdout);
  }
#endif //LB_TST

  setSlaveResetPort( &resetPort ); // setting for reset port of slave I/F
  setSlaveFreqPort( &ahbFreqPort ); // setting for frequency port of slave I/F
  setTargetSocket( "ts" ); // setting for target socket
  ts = tSocket[0]; //Alias for binding

  setMasterResetPortVci( &resetPort ); // setting for reset port of master I/F
  setMasterFreqPortVci( &vciFreqPort ); // setting for frequency port of master I/F
  setInitiatorSocketVci( "is" ); // setting for initiator socket
  is = iSocketVci[0];     // Alias used for connection

  mpCore = new AHB2VCI_core( this, mBusSlaveIf[0], mBusMasterIfVci[0] );
  
  mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mpCore );
  mBusSlaveIf[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusSlaveIf[0]->setBusProtocol( BUS_AHB );
  
  mBusMasterIfVci[0]->setFuncModulePtr( (BusMasterFuncIf *)mpCore );
  mBusMasterIfVci[0]->setTransNmbLmt( 1 );
  mBusMasterIfVci[0]->setBusProtocol( BUS_VCI );

}

/// <summary>destructor</summary>
AHB2VCI::
~AHB2VCI( void )
{
  delete mpCore;
}

