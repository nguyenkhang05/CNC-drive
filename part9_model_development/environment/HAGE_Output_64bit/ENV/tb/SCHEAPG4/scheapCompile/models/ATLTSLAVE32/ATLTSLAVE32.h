/*
* Copyright(c) 2012 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef __ATLTSLAVE32_H__
#define __ATLTSLAVE32_H__

#include "BusSlaveBase.h"
#include "MemoryFunction32.h"
#include <cassert>

extern sc_time_unit glb_resolution_unit;

class ATLTSLAVE32 : public sc_module, public BusSlaveBase<32,1>
{

 public:
  sc_in<bool>   resetPort;
  sc_in<sc_dt::uint64> freqPort;

  MemoryFunction32 *mFunc;
  TlmTargetSocket<32> *ts;
  
  ATLTSLAVE32( sc_module_name name, 
	      sc_dt::uint64         rLatency,
	      sc_dt::uint64         wLatency ):
    sc_module( name ),
    BusSlaveBase<32,1>(),
    resetPort( "resetPort" ),
    freqPort( "freqPort" )
  {
    setSlaveResetPort32( &resetPort );
    setSlaveFreqPort32( &freqPort );
    setTargetSocket32( "ts" );

    ts = tSocket32[0]; //Alias for binding

#if defined(BUSIF_TST)
#if defined(PLTFRM_TST)
    mBusSlaveIf32[0]->setLogFileP( stdout );
    mFunc = new MemoryFunction32( stdout );
#else  //PLTFRM_TST
    FILE *tstFp;
    char debug_file_name[1024];
    strcpy(debug_file_name,this->name());
    strcat(debug_file_name, ".log");
    if( (tstFp=fopen(debug_file_name,"w")) == NULL ){
      printf("\n%s:UT test file open error!!\n\n", this->name());
      fflush(stdout);
    }
    mBusSlaveIf32[0]->setLogFileP( tstFp );
    mFunc = new MemoryFunction32( tstFp);
#endif //PLTFRM_TST
#else  //BUSIF_TST
    mFunc = new MemoryFunction32();
#endif //BUSIF_TST
    mBusSlaveIf32[0]->setFuncModulePtr( mFunc );

    mBusSlaveIf32[0]->setReadLatency( rLatency );
    mBusSlaveIf32[0]->setWriteLatency( wLatency );
  }

  ~ATLTSLAVE32(void){ delete mFunc; }

 private:
  
};

#endif // __ATLTSLAVE32_H__
