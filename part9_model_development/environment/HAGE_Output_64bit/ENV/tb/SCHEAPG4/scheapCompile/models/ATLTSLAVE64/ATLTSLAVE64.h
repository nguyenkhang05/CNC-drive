/*
* Copyright(c) 2012 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __ATLTSLAVE64_H__
#define __ATLTSLAVE64_H__

#include "BusSlaveBase.h"
#include "MemoryFunction64.h"
#include <cassert>

extern sc_time_unit glb_resolution_unit;

class ATLTSLAVE64 : public sc_module, public BusSlaveBase<64,1>
{

 public:
  sc_in<bool>   resetPort;
  sc_in<sc_dt::uint64> freqPort;

  MemoryFunction64 *mFunc;
  TlmTargetSocket<64> *ts;
  
  ATLTSLAVE64( sc_module_name name, 
	      sc_dt::uint64         rLatency,
	      sc_dt::uint64         wLatency ):
    sc_module( name ),
    BusSlaveBase<64,1>(),
    resetPort( "resetPort" ),
    freqPort( "freqPort" )
  {
    setSlaveResetPort64( &resetPort );
    setSlaveFreqPort64( &freqPort );
    setTargetSocket64( "ts" );

    ts = tSocket64[0]; //Alias for binding

#if defined(BUSIF_TST)
#if defined(PLTFRM_TST)
    mBusSlaveIf64[0]->setLogFileP( stdout );
    mFunc = new MemoryFunction64( stdout );
#else  //PLTFRM_TST
    FILE *tstFp;
    char debug_file_name[1024];
    strcpy(debug_file_name,this->name());
    strcat(debug_file_name, ".log");
    if( (tstFp=fopen(debug_file_name,"w")) == NULL ){
      printf("\n%s:UT test file open error!!\n\n", this->name());
      fflush(stdout);
    }
    mBusSlaveIf64[0]->setLogFileP( tstFp );
    mFunc = new MemoryFunction64( tstFp );
#endif //PLTFRM_TST
#else  //BUSIF_TST
    mFunc = new MemoryFunction64();
#endif //BUSIF_TST
    mBusSlaveIf64[0]->setFuncModulePtr( mFunc );

    mBusSlaveIf64[0]->setReadLatency( rLatency );
    mBusSlaveIf64[0]->setWriteLatency( wLatency );
  }

  ~ATLTSLAVE64(void){}

 private:
  
};

#endif // __ATLTSLAVE64_H__
