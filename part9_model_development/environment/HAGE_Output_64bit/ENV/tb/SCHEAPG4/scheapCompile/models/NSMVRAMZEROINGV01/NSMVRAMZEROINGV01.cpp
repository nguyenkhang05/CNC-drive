/*************************************************************************
 *
 * NSMVRAMZEROINGV01.cpp
 *
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include <string>
#include "NSMVRAMZEROINGV01.h"
#include "RamZeroingFunc.h"

using namespace std;
using namespace CmErrMsg;

// for debug output
#ifdef RAM_ZEROING_TEST
#  include <fstream>
   extern ofstream drzfout;
#  define dmsgrz(...) drzfout << hex << sc_time_stamp() << ": " << __VA_ARGS__ << endl;
#else  // not defined RAM_ZEROING_TEST
#  define dmsgrz(...)
#endif // RAM_ZEROING_TEST

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pCfgFileName">configuration file name</param>
/// <returns>none</returns>
NSMVRAMZEROINGV01::NSMVRAMZEROINGV01(
  sc_module_name name,
  const char*    pCfgFileName ) :
    sc_module( name ),
    BusMasterBase<64, 1>(),
    mpR0Func( 0 ),
    mIsParamSet( false )
{
  mpModName = this->name();
  dmsgrz( mpModName << ".NSMVRAMZEROINGV01()" );

  setMasterResetPort64( &rst_rresz );
  setMasterFreqPort64 ( &clk_ram );
  setInitiatorSocket64( "is" );
  is = iSocket64[0];

  string funcmod_name( name );
  funcmod_name.append( ".mpR0Func" );
  mpR0Func = new RamZeroingFunc( funcmod_name.c_str(), mBusMasterIf64[0] );
  mBusMasterIf64[0]->setFuncModulePtr( (BusMasterFuncIf *)mpR0Func );

  // reset process
  SC_METHOD( ResetProc );
  sensitive << rst_rresz;
  dont_initialize();
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
NSMVRAMZEROINGV01::~NSMVRAMZEROINGV01( void )
{
  dmsgrz( mpModName << ".~NSMVRAMZEROINGV01()" );

  delete mpR0Func;
  mpR0Func = 0;
}


/* API functions */
/// <summary>set parameter function</summary>
/// <param name="stAddr">start address</param>
/// <param name="mxAddr">max address(size-1)</param>
/// <param name="asAddr"></param>
/// <param name="selin"></param>
/// <returns>none</returns>
void
NSMVRAMZEROINGV01::setParameter(
  unsigned int stAddr,
  unsigned int mxAddr,
  unsigned int asAddr,
  bool         selin )
{
  dmsgrz( mpModName << ".setParameter( 0x" << stAddr << ", 0x" << mxAddr << " , 0x" << asAddr << ", " << selin << " )" );

  char msg[128];
  if ( mxAddr < asAddr ) {
    sprintf( msg, "ASADR(0x%x) is larger than MAXADR(0x%x). No parameters are set.", asAddr, mxAddr );
    printWarningMessage( this->name(), "setParameter", msg );
  }
  else {
    mpR0Func->setParameter( stAddr, mxAddr, asAddr, selin );
    mIsParamSet = true;
  }
}

/// <summary>write memory function</summary>
/// <param name="addr">write address</param>
/// <param name="data">write data</param>
/// <param name="byte">byte size</param>
/// <returns>none</returns>
void
NSMVRAMZEROINGV01::writeMemValue (
  unsigned int           addr,
  unsigned long long int data,
  unsigned int           byte )
{
  dmsgrz( mpModName << ".writeMemValue( 0x"
          << addr << ", 0x" << data << ", 0x" << byte << " )" );

  mpR0Func->writeMemValue( addr, data, byte );
}

/// <summary>read memory function</summary>
/// <param name="addr">read address</param>
/// <param name="byte">byte size</param>
/// <returns>none</returns>
unsigned long long int
NSMVRAMZEROINGV01::readMemValue (
  unsigned int addr,
  unsigned int byte )
{
  dmsgrz( mpModName << ".readMemValue( 0x" << addr << ", " << byte << " )" );

  return mpR0Func->readMemValue( addr, byte );
}


/* processes */
/// <summary>reset process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVRAMZEROINGV01::ResetProc( void )
{
  dmsgrz( mpModName << ".ResetProc() rst_rresz:" << rst_rresz.read() );

  if ( !rst_rresz.read() ) { // negative edge
    zero_ack.write( false );
  }
  else { // positive edge
    if ( !mIsParamSet ) { // zoroing is inactive
      // don't zeroing
    }
    else { // zeroing is active
      mpR0Func->zeroing( zero_mode.read() );
    }
    zero_ack.write( true );
  }
}
