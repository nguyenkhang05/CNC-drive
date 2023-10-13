/*
* Copyright(c) 2012 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __DMYSLV32_H__
#define __DMYSLV32_H__

#include "BusSlaveBase.h"


class DmyFnc32: public BusSlaveFuncIf {
public:
  ///<summary>constructor</summary>
  ///<param name="none"></param>
  ///<returns>none</returns>
  DmyFnc32( void ): BusSlaveFuncIf() { }

  ///<summary>Destructor</summary>
  ///<param name="none"></param>
  ///<returns>none</returns>
  ~DmyFnc32( void ) { }

  ///<summary>dummy read function</summary>
  ///<param name="offsetAddress">offset address</param>
  ///<param name="trans">TLM payload</param>
  ///<param name="t">pointer of loacal time variable</param>
  ///<param name="debug">debug access or not</param>
  ///<returns>none</returns>
  void read ( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug=0 ) {}

  ///<summary>dummy write function</summary>
  ///<param name="offsetAddress">offset address</param>
  ///<param name="trans">TLM payload</param>
  ///<param name="t">pointer of loacal time variable</param>
  ///<param name="debug">debug access or not</param>
  ///<returns>none</returns>
  void write( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug=0 ) {}

  ///<summary>dummy function to set the size of slave area</summary>
  ///<param name="size">size of slave area</param>
  ///<returns>none</returns>
  void setfunc( ADDRESS_TYPE size ) {}
};


class DmySlv32: public sc_module, public BusSlaveBase< 32, 1 >
{
public:
  sc_in<bool>          resetPort; // reset port
  sc_in<sc_dt::uint64> freqPort;  // clock frequency port
  TlmTargetSocket<32>* ts;        // pointer of TLM target socket
  DmyFnc32*            mpFunc;    // pointer of dummy function class

  ///<summary>constructor</summary>
  ///<param name="name">module name</param>
  ///<returns>none</returns>
  DmySlv32( sc_module_name name ):
    sc_module( name ),
    BusSlaveBase<32,1>()
  {
    setSlaveResetPort32( &resetPort );
    setSlaveFreqPort32( &freqPort );
    setTargetSocket32( "ts" );

    ts = tSocket32[0]; 
    mpFunc = new DmyFnc32();
    mBusSlaveIf32[0]->setFuncModulePtr( mpFunc );
    mBusSlaveIf32[0]->setReadLatency  ( 0 );
    mBusSlaveIf32[0]->setWriteLatency ( 0 );
  }

  ///<summary>Destructor</summary>
  ///<param name="none"></param>
  ///<returns>none</returns>
  ~DmySlv32(void){}
};

#endif // __DMYSLV32_H__
