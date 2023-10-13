/*************************************************************************
 *
 * Swd0pcadFunc.cpp
 *
 * Copyright(c) 2013-2015 Renesas Electronics Corporation
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
#include "BusSlaveFuncIf.h"
#include "OSCI2.h"
#include "Swd0pcadFunc.h"
#include "NSMVSWD0PCADV01.h"
#include "NSMVG4CPUV01.h"

using namespace CmErrMsg;

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
Swd0pcadReg::Swd0pcadReg ( const char* name ):
  GRegDataBase<GRegData32>( name )
{
  SetRegPtr( &mData );
}


/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
Swd0pcadReg::~Swd0pcadReg( void )
{
}


///<summary>function to reset</summary>
///<param name="">none</param>
///<returns>none</returns>
void Swd0pcadReg::Reset( void )
{
  unsigned char data[4] = { 0xff, 0xff, 0xff, 0xff };
  WriteBody( GetAddress(), data, G_REG_SIZE_4BYTE );
}



/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
Swd0pcadFunc::Swd0pcadFunc( const char* name ):
  BusSlaveFuncIf(),
  mPeId(1)
{
  strcpy( mModName, name );
  char reg_name[512+10];
  sprintf( reg_name, "%s.SWD0PCAD", name );
  mpSwd0pcadReg[0] = new Swd0pcadReg( reg_name );
  mpSwd0pcadReg[0]->SetAddress( 0 );
  mpSwd0pcadReg[0]->SetAccessSize( G_REG_SIZE_1BYTE );
  mpSwd0pcadReg[0]->SetAccessSize( G_REG_SIZE_2BYTE );
  mpSwd0pcadReg[0]->SetAccessSize( G_REG_SIZE_4BYTE );
  mpSwd0pcadReg[0]->SetRegMask( 0xFFFFFFFF );
}


/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
Swd0pcadFunc::~Swd0pcadFunc( void )
{
  delete mpSwd0pcadReg[0];
}


// public functions
///<summary>function to reset</summary>
///<param name="">none</param>
///<returns>none</returns>
void Swd0pcadFunc::reset( void )
{
  mpSwd0pcadReg[0]->Reset();
}


///<summary>function to reset</summary>
///<param name="pG4cpu">pointer to NSMVG4CPUV01 module</param>
///<returns>none</returns>
void Swd0pcadFunc::setG4cpuPtr( NSMVG4CPUV01* pG4cpu )
{
  mpG4cpu = pG4cpu;
}


///<summary>function to read register</summary>
///<param name="addr">address of register</param>
///<param name="pData">pointer to read data</param>
///<param name="size">Byte size of read data</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void Swd0pcadFunc::readReg( unsigned int addr, unsigned char* pData, unsigned int size, bool debug )
{
  fflush(stdout);

  if ( debug ) {
    mpSwd0pcadReg[0]->ReadBody( addr, pData, (G_REG_SIZE_T)size );
  }
  else {
    mpSwd0pcadReg[0]->Read( addr, pData, (G_REG_SIZE_T)size );
  }
}


///<summary>function to write register</summary>
///<param name="addr">address of register</param>
///<param name="pData">pointer to read data</param>
///<param name="size">Byte size of write data</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void Swd0pcadFunc::writeReg( unsigned int addr, unsigned char* pData, unsigned int size, bool debug )
{
  if ( debug ) {
    mpSwd0pcadReg[0]->WriteBody( addr, pData, (G_REG_SIZE_T)size );
  }
  else {
    mpSwd0pcadReg[0]->Write( addr, pData, (G_REG_SIZE_T)size );
  }

  // update the value of G3MCPU's SWD0PCAD valuable
  unsigned char data[4];
  mpSwd0pcadReg[0]->ReadBody( addr, data, G_REG_SIZE_4BYTE );
  unsigned int val = 0;
  for(int i=0; i<4; i++){
    unsigned int tmp_val = (unsigned int)(data[i]);
    val = (tmp_val << i*8) | val;
  }

#ifdef TEST_SWDT_CPU
  printf("%s:%s:writeReg: call G3MCPU's SetSwd0pcadValue(0x%08x)\n",
	 sc_time_stamp().to_string().c_str(),
	 mModName,
	 val);
  fflush(stdout);
#endif
  mpG4cpu->SetSwd0pcadValue(mPeId, val);

}


///<summary>function to get the pointer to the register</summary>
///<param name="">none</param>
///<returns>pointer to the register</returns>
Swd0pcadReg* Swd0pcadFunc::getRegPtr( void )
{
  return mpSwd0pcadReg[0];
}


/// <summary>set PEID of related PE</summary>
/// <param name="peid">related PE's PEID</param>
///<returns>none</returns>
void Swd0pcadFunc::setPeId( unsigned int peid )
{
  mPeId = peid;
}


///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void Swd0pcadFunc::read ( unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug )
{
  unsigned char* pData = trans.get_data_ptr();
  unsigned int   size  = trans.get_data_length();

  for ( unsigned int i = 0; i < size; i++ ) {
    pData[i] = 0;
  }

  readReg( offsetAddress, pData, size, debug );
}


///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void Swd0pcadFunc::write( unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug )
{
  unsigned char* pData = trans.get_data_ptr();
  unsigned int   size  = trans.get_data_length();

  writeReg( offsetAddress, pData, size, debug );
}


