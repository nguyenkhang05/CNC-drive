/*************************************************************************
 * Copyright(c) 2013-2015 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  SWD0PCAD_FUNC_H
#define  SWD0PCAD_FUNC_H

#include "BusSlaveFuncIf.h"
#include "g_reg.h"

class NSMVSWD0PCADV01;
class NSMVG4CPUV01;

class Swd0pcadReg : public GRegDataBase<GRegData32> {
public:
  void Reset( void );

  Swd0pcadReg( const char* name );
  virtual ~Swd0pcadReg( void );

private:
  GRegData32 mData;
};

class Swd0pcadFunc : public BusSlaveFuncIf
{
public:
  Swd0pcadFunc( const char* name );
  virtual ~Swd0pcadFunc( void );

  /* public functions */
  void reset( void );
  void setG4cpuPtr( NSMVG4CPUV01* pG4cpu );

  void readReg ( unsigned int   addr,
                 unsigned char* pData,
                 unsigned int   size,
                 bool           debug );
  void writeReg( unsigned int   addr,
                 unsigned char* pData,
                 unsigned int   size,
                 bool           debug );
  void setPeId( unsigned int peid );

  Swd0pcadReg* getRegPtr( void ); // for debug

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
  char              mModName[512];    // module name
  char              mErrMsg[64];      // for error message
  Swd0pcadReg*      mpSwd0pcadReg[1]; // pointer to REG register module
  NSMVG4CPUV01*     mpG4cpu;         // pointer to NSMVG4CPUV01 module
  unsigned int      mPeId;            // for related PE's PEID
};

#endif // SWD0PCAD_FUNC_H

