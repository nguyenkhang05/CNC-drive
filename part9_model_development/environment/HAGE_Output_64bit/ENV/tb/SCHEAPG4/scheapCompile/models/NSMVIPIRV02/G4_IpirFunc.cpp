/*************************************************************************
 *
 * G4_IpirFunc.cpp
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

#include <cstring>
#include "CmErrMsg.h"
#include "BusSlaveFuncIf.h"
#include "OSCI2.h"
#include "G4_IpirFunc.h"
#include "NSMVIPIRV02.h"

using namespace CmErrMsg;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT


G4_IpirReg::G4_IpirReg ( const char* name ):
  GRegDataBase<GRegData8>( name )
{
  SetRegPtr( &mData );
}

G4_IpirReg::~G4_IpirReg ( void )
{
}

void
G4_IpirReg::Reset( void )
{
  unsigned char zeroData[2] = { 0, 0 };
  WriteBody( GetAddress(), zeroData, G_REG_SIZE_1BYTE );
}



/// <summary>constructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_IpirFunc::G4_IpirFunc( const char* name ):
  BusSlaveFuncIf()
{
  strcpy( mModName, name );
  char inst_name[128];
  for ( unsigned int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
    sprintf( inst_name, "%s.IPI%dEN", name, i );
    mpIPIEN[i] = new G4_IpirReg( inst_name );
    mpIPIEN[i]->SetAddress( 0x800+0x20*i );
    mpIPIEN[i]->SetAccessSize( G_REG_SIZE_1BYTE );

    sprintf( inst_name, "%s.IPI%dFLG", name, i );
    mpIPIFLG[i] = new G4_IpirReg( inst_name );
    mpIPIFLG[i]->SetAddress( 0x804+0x20*i );
    mpIPIFLG[i]->SetAccessSize( G_REG_SIZE_1BYTE );

    sprintf( inst_name, "%s.IPI%dFCLR", name, i );
    mpIPIFCLR[i] = new G4_IpirReg( inst_name );
    mpIPIFCLR[i]->SetAddress( 0x808+0x20*i );
    mpIPIFCLR[i]->SetAccessSize( G_REG_SIZE_1BYTE );

    sprintf( inst_name, "%s.IPI%dREQ", name, i );
    mpIPIREQ[i] = new G4_IpirReg( inst_name );
    mpIPIREQ[i]->SetAddress( 0x810+0x20*i );
    mpIPIREQ[i]->SetAccessSize( G_REG_SIZE_1BYTE );

    sprintf( inst_name, "%s.IPI%dRCLR", name, i );
    mpIPIRCLR[i] = new G4_IpirReg( inst_name );
    mpIPIRCLR[i]->SetAddress( 0x814+0x20*i );
    mpIPIRCLR[i]->SetAccessSize( G_REG_SIZE_1BYTE );
  }
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_IpirFunc::~G4_IpirFunc( void )
{
  dmsgsc( mModName << ".~G4_IpirFunc()" );
  for ( int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
    delete mpIPIEN[i];
    delete mpIPIFLG[i];
    delete mpIPIFCLR[i];
    delete mpIPIREQ[i];
    delete mpIPIRCLR[i];
  }
}


// public functions
///<summary>function to reset</summary>
///<param name="">none</param>
///<returns>none</returns>
void
G4_IpirFunc::reset( void )
{
  dmsgsc( mModName << ".reset()" );
  for ( int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
    mpIPIEN[i]->Reset();
    mpIPIFLG[i]->Reset();
    mpIPIFCLR[i]->Reset();
    mpIPIREQ[i]->Reset();
    mpIPIRCLR[i]->Reset();
  }
}

///<summary>function to reset</summary>
///<param name="pIpir">pointer to IPIR module</param>
///<returns>none</returns>
void
G4_IpirFunc::setIpirPtr( NSMVIPIRV02* pIpir )
{
  dmsgsc( mModName << ".setIpirPtr()" );
  mpIpir = pIpir;
}


///<summary>function to read register</summary>
///<param name="addr">address of register</param>
///<param name="pData">pointer to read data</param>
///<param name="size">Byte size of read data</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_IpirFunc::readReg(
  unsigned int   addr,
  unsigned char* pData,
  unsigned int   size,
  bool           debug )
{
  dmsgsc( mModName << ".readReg() addr:0x" << addr );
  bool hit_flag = false;

  for ( int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
    if ( mpIPIEN[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIEN[i]->ReadBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIEN[i]->Read( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    } else if ( mpIPIFLG[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIFLG[i]->ReadBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIFLG[i]->Read( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    } else if ( mpIPIFCLR[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIFCLR[i]->ReadBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIFCLR[i]->Read( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    } else if ( mpIPIREQ[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIREQ[i]->ReadBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIREQ[i]->Read( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    } else if ( mpIPIRCLR[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIRCLR[i]->ReadBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIRCLR[i]->Read( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    }
  }

  if ( !hit_flag ) {
    sprintf( mErrMsg, "There is no register at 0x%x.", addr );
    printWarningMessage( mModName, "readReg", mErrMsg );
  }
  else {
    // do nothing
  }
}

///<summary>function to write register</summary>
///<param name="addr">address of register</param>
///<param name="pData">pointer to read data</param>
///<param name="size">Byte size of write data</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_IpirFunc::writeReg(
  unsigned int   addr,
  unsigned char* pData,
  unsigned int   size,
  bool           debug )
{
  dmsgsc( mModName << ".writeReg() addr:0x" << addr << " size:" << size );
  bool hit_flag = false;

  for ( int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
    if ( mpIPIEN[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIEN[i]->WriteBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIEN[i]->Write( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    } else if ( mpIPIFLG[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIFLG[i]->WriteBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIFLG[i]->Write( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    } else if ( mpIPIFCLR[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIFCLR[i]->WriteBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIFCLR[i]->Write( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    } else if ( mpIPIREQ[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIREQ[i]->WriteBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIREQ[i]->Write( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    } else if ( mpIPIRCLR[i]->IsSearchHit(addr) ) {
      dmsgsc("  ch:" << i);
      hit_flag = true;
      if ( debug ) {
        mpIPIRCLR[i]->WriteBody( addr, pData, (G_REG_SIZE_T)size );
      }
      else {
        mpIPIRCLR[i]->Write( addr, pData, (G_REG_SIZE_T)size );
      }
      break;
    }
  }

  if ( !hit_flag ) {
    sprintf( mErrMsg, "There is no register at 0x%x.", addr );
    printWarningMessage( mModName, "writeReg", mErrMsg );
  }
  else {
    // do nothing
  }
}

///<summary>function to read register</summary>
///<param name="addr">address of register</param>
///<param name="debug">debug access or not</param>
///<returns>read data</returns>
unsigned int
G4_IpirFunc::readReg8( unsigned int addr, bool debug )
{
  dmsgsc( mModName << ".readReg8()" );
  unsigned char rdata[(int)G_REG_SIZE_1BYTE];
  unsigned int  rdata8 = 0;

  readReg( addr, rdata, (int)G_REG_SIZE_1BYTE, debug );
  for ( int i = 0; i < (int)G_REG_SIZE_1BYTE; i++ ) {
    rdata8 |= ( (rdata[i] & 0xff) << i );
  }

  return rdata8;
}

///<summary>function to get the pointer to the register</summary>
///<param name="ch">channel of register</param>
///<returns>pointer to the register</returns>
G4_IpirReg*
G4_IpirFunc::getRegPtr(const char* reg_name,  unsigned int ch )
{
  dmsgsc( mModName << ".getRegPtr()" );
  G4_IpirReg* ptr = 0;

  if ( INTC_IPIR_CH_NUM > ch ) {
    if (reg_name == "IPIEN") {
      ptr = mpIPIEN[ch];
    } else if (reg_name == "IPIFLG") {
      ptr = mpIPIFLG[ch];
    } else if (reg_name == "IPIFCLR") {
      ptr = mpIPIFCLR[ch];
    } else if (reg_name == "IPIREQ") {
      ptr = mpIPIREQ[ch];
    } else if (reg_name == "IPIRCLR") {
      ptr = mpIPIRCLR[ch];
    }
  }
  else {
    sprintf( mErrMsg, "Invalid channel(%d).", ch );
    printErrorMessage( mModName, "getRegPtr", mErrMsg );
  }

  return ptr;
}


///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_IpirFunc::read (
  unsigned int     offsetAddress, 
  TlmBasicPayload& trans, 
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc( mModName << ".read() t:" << *t << " addr:0x" << trans.get_address());
  unsigned char* pData = trans.get_data_ptr();
  unsigned int   size  = trans.get_data_length();

  for ( unsigned int i = 0; i < size; i++ ) {
    pData[i] = 0;
  }

  readReg( offsetAddress, pData, size, debug );

  dmsgsc( "  address:0x" << trans.get_address() );
  for ( unsigned int i = 0; i < size; i++ ) {
    dmsgsc( "  data:0x" << (int)pData[i] );
  }
}

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_IpirFunc::write(
  unsigned int     offsetAddress, 
  TlmBasicPayload& trans, 
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc( mModName << ".write() t:" << *t << " addr:0x" << trans.get_address());
  unsigned char* pData = trans.get_data_ptr();
  unsigned int   size  = trans.get_data_length();

  if ( debug ) {
    writeReg( offsetAddress, pData, size, debug );
  }
  else {
    G4_IpirRegWriteInfo wInfo( offsetAddress, pData, size );
    mpIpir->setWriteInfo( &wInfo );
  }

  mpIpir->reqInt( t, debug );
  dmsgsc("  t:" << *t);

  for ( unsigned int i = 0; i < size; i++ ) {
    dmsgsc( "  data:0x" << (int)pData[i] );
  }
}


