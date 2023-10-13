/*************************************************************************
 *
 *  VPI2APB.cpp
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

#include "VPI2APB_core.h"
#include "VPI2APB.h"
#include "CmErrMsg.h"  // for error message


using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">name of sc_module</param>
VPI2APB::
VPI2APB( sc_module_name moduleName ) :
  BusBridgeModuleBase<BUS_WIDTH_VPI,BUS_WIDTH_APB>( moduleName ),
  BusBridgeSlaveBase<BUS_WIDTH_VPI,1>(),
  BusMasterBase<BUS_WIDTH_APB,1>(),
  vpiFreqPort( "vpiFreqPort" ),
  apbFreqPort( "apbFreqPort" ),
  ts( (TlmTargetSocket<BUS_WIDTH_VPI> *)0 ),
  is( (TlmInitiatorSocket<BUS_WIDTH_APB> *)0 )
{

#ifdef BUS_TST
  char fileName[1024];
  strcpy(fileName, moduleName);
  strcat(fileName, ".log");
  if( (fp=fopen(fileName,"w")) == NULL ){
    printf("\nVPI2APB:UT test file open error!!\n\n");
    fflush(stdout);
  }
#endif //BUS_TST

  setSlaveResetPort( &resetPort ); // setting for reset port of slave I/F
  setSlaveFreqPort( &vpiFreqPort ); // setting for frequency port of slave I/F
  setTargetSocket( "ts" ); // setting for target socket
  ts = tSocket[0]; //Alias for binding

  setMasterResetPortApb( &resetPort ); // setting for reset port of master I/F
  setMasterFreqPortApb( &apbFreqPort ); // setting for frequency port of master I/F
  setInitiatorSocketApb( "is" ); // setting for initiator socket
  is = iSocketApb[0];     // Alias used for connection

  mpCore = new VPI2APB_core( this, mBusSlaveIf[0], mBusMasterIfApb[0] );
  
  mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mpCore );
  mBusSlaveIf[0]->setTransNmbLmt( 0xFFFFFFFF );
  mBusSlaveIf[0]->setBusProtocol( BUS_VPI );
  
  mBusMasterIfApb[0]->setFuncModulePtr( (BusMasterFuncIf *)mpCore );
  mBusMasterIfApb[0]->setTransNmbLmt( 1 );
  mBusMasterIfApb[0]->setBusProtocol( BUS_APB );

  mpCore->setBridgeFwLatency( 1 );//1 is default value. This is related to SG_X
  mpCore->setBridgeBwLatency( 1 );//1 is default value. This is related to SG_X

}

/// <summary>destructor</summary>
VPI2APB::
~VPI2APB( void )
{
  delete mpCore;
}

