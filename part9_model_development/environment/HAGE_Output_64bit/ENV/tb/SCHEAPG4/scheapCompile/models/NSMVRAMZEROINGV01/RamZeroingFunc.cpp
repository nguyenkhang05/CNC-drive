/*************************************************************************
 *
 * G0MevFunc.cpp
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

#include "RamZeroingFunc.h"

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
/// <returns>none</returns>
RamZeroingFunc::RamZeroingFunc (
  const char*      name,
  BusMasterIf<64>* busIf ):
    BusMasterFuncIf(),
    mModNameStr( name ),
    mpBusIf( busIf ),
    mStAddr( 0 ),
    mMaxAddr( 0 ),
    mAsAddr( 0 ),
    mSelin( false )
{
  dmsgrz( mModNameStr << ".RamZeroingFunc()" );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
RamZeroingFunc::~RamZeroingFunc ( void )
{
  dmsgrz( mModNameStr << ".~RamZeroingFunc()" );
}


// set function
/// <summary>set parameter function</summary>
/// <param name="stAddr">start address</param>
/// <param name="mxAddr">max address(size-1)</param>
/// <param name="asAddr"></param>
/// <param name="selin"></param>
/// <returns>none</returns>
void
RamZeroingFunc::setParameter (
  unsigned int stAddr,
  unsigned int mxAddr,
  unsigned int asAddr,
  bool         selin )
{
  dmsgrz( mModNameStr << ".setParameter( 0x" << stAddr << ", 0x" << mxAddr << ", 0x"
          << asAddr << " selin:" << selin << " )" );
  mStAddr  = stAddr;
  mMaxAddr = mxAddr;
  mAsAddr  = asAddr;
  mSelin   = selin;
}

/// <summary>write memory function</summary>
/// <param name="addr">write address</param>
/// <param name="data">write data</param>
/// <param name="byte">byte size</param>
/// <returns>none</returns>
void
RamZeroingFunc::writeMemValue (
  unsigned int           addr,
  unsigned long long int data,
  unsigned int           byte )
{
  dmsgrz( mModNameStr << ".writeMemValue( 0x"
          << addr << ", 0x" << data << ", 0x" << byte << " )" );

  unsigned char wdata[8];

  if ( 8 < byte ) {
     printWarningMessage( mModNameStr.c_str(), "writeMemValue",
                          "Byte size is invalid (not to be set over 8).");
  }
  else {
    TlmBasicPayload  trans;
    TlmG3mExtension* pG3mExt= new TlmG3mExtension;
    trans.set_extension( pG3mExt );
    trans.set_address( addr );
    trans.set_data_ptr( wdata );
    trans.set_data_length( byte );

    for ( unsigned int i = 0; i < byte; i++ ) {
      wdata[i] = (unsigned char)( (data>>(8*i))&0xff );
    }
    mpBusIf->write( trans, true ); // debug write

    trans.release_extension( pG3mExt );
  }
}

/// <summary>read memory function</summary>
/// <param name="addr">read address</param>
/// <param name="byte">byte size</param>
/// <returns>none</returns>
unsigned long long int
RamZeroingFunc::readMemValue (
  unsigned int addr,
  unsigned int byte )
{
  dmsgrz( mModNameStr << ".readMemValue( 0x" << addr << ", " << byte << " )" );

  unsigned long long int ret = 0;
  unsigned char          rdata[8];

  if ( 8 < byte ) {
     printWarningMessage( mModNameStr.c_str(), "writeMemValue",
                          "Byte size is invalid (not to be set over 8).");
  }
  else {
    TlmBasicPayload  trans;
    TlmG3mExtension* pG3mExt= new TlmG3mExtension;
    trans.set_extension( pG3mExt );
    trans.set_address( addr );
    trans.set_data_ptr( rdata );
    trans.set_data_length( byte );

    mpBusIf->read( trans, true ); // debug read

    trans.release_extension( pG3mExt );
    for ( unsigned int i = 0;  i < byte; i++ ) {
      ret |= (unsigned long long int)(rdata[i] << (8*i));
    }
  }

  return ret;
}


/// <summary>zeroing function</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
RamZeroingFunc::zeroing ( unsigned int mode )
{
  dmsgrz( mModNameStr << ".zeroing(0x" << mode << ")" );

  TlmBasicPayload trans;
  unsigned int    addr = 0;
  unsigned int    size = 0;

  if ( 1 == mode ) { // parted area
    if ( mSelin ) { // first area
      addr = mStAddr;
      size = mAsAddr+1;
    }
    else { // last area
      addr = mStAddr+mAsAddr;
      size = mMaxAddr-mAsAddr+1;
    }
  }
  else if ( 3 == mode ) { // all area
    addr = mStAddr;
    size = mMaxAddr+1;
  }
  else {
    // don't zeroing
  }
  dmsgrz( "  addr:0x" << addr << " size:0x" << size );

  if ( 0 != size ) {
    unsigned char* data = new unsigned char[size];
    for ( unsigned int i = 0; i < size; i++ ) {
      data[i] = 0;
    }
    TlmG3mExtension* pG3mExt= new TlmG3mExtension;
    trans.set_extension( pG3mExt );
    trans.set_address( addr );
    trans.set_data_ptr( data );
    trans.set_data_length( size );

    mpBusIf->write( trans, true ); // debug write

    trans.release_extension( pG3mExt );
    delete [] data;
  }
  else {
    // don't zeroing
  }
}
