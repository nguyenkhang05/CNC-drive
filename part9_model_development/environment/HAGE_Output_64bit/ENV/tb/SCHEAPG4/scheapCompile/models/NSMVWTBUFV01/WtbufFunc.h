/*
* Copyright(c) 2014 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __WTBUF_FUNC_H__
#define __WTBUF_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "OSCI2.h"

class NSMVG4CPUV01;

class WtbufFunc : public BusSlaveFuncIf
{
 public:
  WtbufFunc( const char* name );
  
  ~WtbufFunc( void );
  
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

  void setG3mcpuPtr( NSMVG4CPUV01* pG3mcpu );

 private:
  char           mModName[512]; // module name
  NSMVG4CPUV01* mpG3mcpu;      // pointer to NSMVG4CPUV01 module
};

#endif // __WTBUF_FUNC_H__
