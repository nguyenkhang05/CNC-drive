/*
* Copyright(c) 2016-2017 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __PEG_FUNC_H__
#define __PEG_FUNC_H__

#include "BusMasterIf.h"
#include "BusSlaveFuncIf.h"
#include "BusMasterFuncIf.h"
#include "OSCI2.h"

class NSMVG4CPUV01;

class PegFunc : public BusSlaveFuncIf, public BusMasterFuncIf
{
 public:
  PegFunc( const char* name, BusMasterIf<BUS_WIDTH_APB> *masterIf );
  
  ~PegFunc( void );
  
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
  BusMasterIf<BUS_WIDTH_APB> *mMasterIf;     // Bus master I/F
  char                        mModName[512]; // module name
  NSMVG4CPUV01               *mpG4cpu;       // pointer to NSMVG4CPUV01 module
};

#endif // __PEG_FUNC_H__
