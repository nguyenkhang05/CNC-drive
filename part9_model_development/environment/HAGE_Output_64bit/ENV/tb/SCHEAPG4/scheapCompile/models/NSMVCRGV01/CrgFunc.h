/*
* Copyright(c) 2016 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __CRG_FUNC_H__
#define __CRG_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "OSCI2.h"

class NSMVG4CPUV01;

class CrgFunc : public BusSlaveFuncIf
{
 public:
  CrgFunc( const char* name );
  
  ~CrgFunc( void );
  
  void read ( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug = false );
  void write( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug = false );

  void setfunc( ADDRESS_TYPE size );
  
  void reset( void );

  void setG4cpuPtr( NSMVG4CPUV01* pG4cpu );

 private:
  char           mModName[512]; // module name
  NSMVG4CPUV01* mpG4cpu;      // pointer to NSMVG4CPUV01 module
};

#endif // __CRG_FUNC_H__
