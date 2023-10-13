/*************************************************************************
 *
 * G4_G0MevFunc.cpp
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

#include <cstring>
#include "CmErrMsg.h"
#include "OSCI2.h"
#include "G4_G0MevFunc.h"
#include "g4_mev_def.h"

using namespace CmErrMsg;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream dmvfout;
#endif // DEBUG_MESSAGE_OUTPUT


/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
G4_G0MevReg::G4_G0MevReg ( const char* name ):
  GRegDataBase<GRegData32>( name )
{
  dmsgmv( GetName() << ".G4_G0MevReg()" );
  SetRegPtr( &mData );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_G0MevReg::~G4_G0MevReg ( void )
{
  dmsgmv( GetName() << ".~G4_G0MevReg()" );
}

/// <summary>reset function</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
G4_G0MevReg::Reset( void )
{
  dmsgmv( GetName() << ".Reset()" );
  unsigned char zeroData[4] = { 0, 0, 0, 0 };
  WriteBody( GetAddress(), zeroData, G_REG_SIZE_4BYTE );
}



/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
G4_G0MevFunc::G4_G0MevFunc( const char* name ):
  BusSlaveFuncIf()
{
  dmsgmv( name << ".G4_G0MevFunc()" );
  strcpy( mModName, name );
  char inst_name[128];
  sprintf( inst_name, "%s.G4_G0MEV", name );
  mpG4_G0MevReg = new G4_G0MevReg( inst_name );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_G0MevFunc::~G4_G0MevFunc( void )
{
  dmsgmv( mModName << ".~G4_G0MevFunc()" );
  delete mpG4_G0MevReg;
}


// public functions
///<summary>function to reset</summary>
///<param name="">none</param>
///<returns>none</returns>
void
G4_G0MevFunc::reset( void )
{
  dmsgmv( mModName << ".reset()" );
  mpG4_G0MevReg->Reset();
}

///<summary>get pointer to register</summary>
///<param name="">none</param>
///<returns>pointer to register</returns>
G4_G0MevReg*
G4_G0MevFunc::getRegPtr( void )
{
  dmsgmv( mModName << ".getRegPtr()" );
  return mpG4_G0MevReg;
}


///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_G0MevFunc::read (
  unsigned int     offsetAddress, 
  TlmBasicPayload& trans, 
  BusTime_t*       t,
  bool             debug )
{
  dmsgmv( mModName << ".read() t:" << *t << " addr:0x" << trans.get_address());
  unsigned char* pData = trans.get_data_ptr();
  unsigned int   size  = trans.get_data_length();

  for ( unsigned int i = 0; i < size; i++ ) {
    pData[i] = 0;
  }

  if ( debug ) {
    mpG4_G0MevReg->ReadBody( (unsigned int)trans.get_address(), pData, (G_REG_SIZE_T)size );
  }
  else {
    mpG4_G0MevReg->Read( (unsigned int)trans.get_address(), pData, (G_REG_SIZE_T)size );
  }

  dmsgmv( "  address:0x" << trans.get_address() );
  for ( unsigned int i = 0; i < size; i++ ) {
    dmsgmv( "  data:0x" << (int)pData[i] );
  }
}

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_G0MevFunc::write(
  unsigned int     offsetAddress, 
  TlmBasicPayload& trans, 
  BusTime_t*       t,
  bool             debug )
{
  dmsgmv( mModName << ".write() t:" << *t << " addr:0x" << trans.get_address());
  unsigned char* pData = trans.get_data_ptr();
  unsigned int   size  = trans.get_data_length();

  if ( debug ) {
    mpG4_G0MevReg->WriteBody( (unsigned int)trans.get_address(), pData, (G_REG_SIZE_T)size );
  }
  else {
    mpG4_G0MevReg->Write( (unsigned int)trans.get_address(), pData, (G_REG_SIZE_T)size );
  }

  for ( unsigned int i = 0; i < size; i++ ) {
    dmsgmv( "  data:0x" << (int)pData[i] );
  }
}
