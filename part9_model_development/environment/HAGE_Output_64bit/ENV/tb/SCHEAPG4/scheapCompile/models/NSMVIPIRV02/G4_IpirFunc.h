/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_IPIR_FUNC_H
#define  G4_IPIR_FUNC_H

#include "BusSlaveFuncIf.h"
#include "G4_intc_def.h"
#include "g_reg.h"

class NSMVIPIRV02;

struct G4_IpirRegWriteInfo
{
  unsigned int  mAddr;
  unsigned char mData[2];
  unsigned int  mSize;

  G4_IpirRegWriteInfo( void ):
      mAddr( 0 ),
      mSize( 0 )
  {
    for ( int i = 0; i < 2; i++ ) {
      mData[i] = 0;
    }
  }

  G4_IpirRegWriteInfo(
    unsigned int   addr,
    unsigned char* pData,
    unsigned int   size  ):
      mAddr( addr ),
      mSize( size )
  {
    for ( int i = 0; i < 2; i++ ) {
      mData[i] = pData[i];
    }
  }
};

class G4_IpirReg : public GRegDataBase<GRegData8> {
public:
  void Reset( void );

  G4_IpirReg( const char* name );
  virtual ~G4_IpirReg( void );

private:
  GRegData8 mData;
};

class G4_IpirFunc : public BusSlaveFuncIf
{
public:
  G4_IpirFunc( const char* name );
  virtual ~G4_IpirFunc( void );

  /* public functions */
  void reset( void );
  void setIpirPtr( NSMVIPIRV02* pIpir );

  void readReg ( unsigned int   addr,
                 unsigned char* pData,
                 unsigned int   size,
                 bool           debug );
  void writeReg( unsigned int   addr,
                 unsigned char* pData,
                 unsigned int   size,
                 bool           debug );

  unsigned int readReg8( unsigned int addr, bool debug );
  G4_IpirReg*     getRegPtr( const char* reg_name, unsigned int ch ); // for debug

  /* bus I/F functions */
  void read ( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug = false );
  void write( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug = false );
  void setfunc( ADDRESS_TYPE size ) {}

private:
  /* private variables */
  char         mModName[512]; // module name
  char         mErrMsg[64]; // for error message
  NSMVIPIRV02* mpIpir; // pointer to NSMVIPIRV02 module
  G4_IpirReg*     mpIPIEN[INTC_IPIR_CH_NUM]; // pointer to IPInEN register
  G4_IpirReg*     mpIPIFLG[INTC_IPIR_CH_NUM]; // pointer to IPInFLG register
  G4_IpirReg*     mpIPIFCLR[INTC_IPIR_CH_NUM]; // pointer to IPInFCLR register
  G4_IpirReg*     mpIPIREQ[INTC_IPIR_CH_NUM]; // pointer to IPInREQ register
  G4_IpirReg*     mpIPIRCLR[INTC_IPIR_CH_NUM]; // pointer to IPInRCLR register
};

#endif // G4_IPIR_FUNC_H

