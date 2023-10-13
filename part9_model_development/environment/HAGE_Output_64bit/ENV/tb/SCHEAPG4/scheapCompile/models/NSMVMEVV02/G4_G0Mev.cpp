/*************************************************************************
 *
 * G4_G0Mev.cpp
 *
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include <cstdio>
#include "CmErrMsg.h"
#include "G4_G0Mev.h"
#include "G4_G0MevFunc.h"
#include "g4_mev_def.h"

using namespace std;
using namespace CmErrMsg;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream dmvfout;
#endif // DEBUG_MESSAGE_OUTPUT


/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
G4_G0Mev::G4_G0Mev( sc_module_name name ):
    sc_module( name ),
    BusSlaveBase<BUS_WIDTH_VCI32, 1>(),
    resetz( "resetz" ),
    clk( "clk" ),
    mpModName( 0 ),
    mpG4_G0MevFunc( 0 )
{
  mpModName = this->name();
  dmsgmv( mpModName << ".G4_G0Mev()" );
  char inst_name[128];

  sprintf( inst_name, "%s.G4_G0MevFunc", mpModName );
  mpG4_G0MevFunc = new G4_G0MevFunc( inst_name );

  setSlaveResetPort32( &resetz );
  setSlaveFreqPort32( &clk );
  setTargetSocket32( "ts" );
  ts = tSocket32[0];

  mBusSlaveIf32[0]->setFuncModulePtr( mpG4_G0MevFunc );
  mBusSlaveIf32[0]->setBusProtocol( BUS_APB );
  mBusSlaveIf32[0]->setTransNmbLmt( 1 );
  mBusSlaveIf32[0]->setReadLatency( 0 );
  mBusSlaveIf32[0]->setWriteLatency( 0 );
  mBusSlaveIf32[0]->setEnableToCheckDataSize();
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_G0Mev::~G4_G0Mev( void )
{
  dmsgmv( mpModName << ".~G4_G0Mev()" );
  delete mpG4_G0MevFunc;
}


/// <summary>set address of register</summary>
/// <param name="addr">address</param>
/// <returns>none</returns>
void
G4_G0Mev::setRegAddr( unsigned int addr )
{
  dmsgmv( mpModName << ".setRegAddr() addr:" << addr );
  (mpG4_G0MevFunc->getRegPtr())->SetAddress( addr );
}


/// <summary>set access size of register</summary>
/// <param name="size">access size</param>
/// <returns>none</returns>
void
G4_G0Mev::setRegAccSize( unsigned int size )
{
  dmsgmv( mpModName << ".setRegAccSize() size:" << size );
  switch ( size ) {
  case 1: (mpG4_G0MevFunc->getRegPtr())->SetAccessSize(G_REG_SIZE_1BYTE); break;
  case 2: (mpG4_G0MevFunc->getRegPtr())->SetAccessSize(G_REG_SIZE_2BYTE); break;
  case 4: (mpG4_G0MevFunc->getRegPtr())->SetAccessSize(G_REG_SIZE_4BYTE); break;
  default:
    sprintf( mErrMsg, "Invalid access size(%d).", size );
    printWarningMessage( mpModName, "setRegAccSize", mErrMsg );
    break;
  }
}

/// <summary>clear access size of register</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
G4_G0Mev::clrRegAccSize( void )
{
  dmsgmv( mpModName << ".clrRegAccSize()" );
  (mpG4_G0MevFunc->getRegPtr())->ClearAccessSize();
}


// private functions
/// <summary>reset function</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
G4_G0Mev::resetFunc( void )
{
  dmsgmv( mpModName << ".resetFunc()" );
  mpG4_G0MevFunc->reset();
}

