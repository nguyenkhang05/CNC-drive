/*************************************************************************
 *
 * NSMVSWD0PCADV01.cpp
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

#include <cstdio>
#include "NSMVSWD0PCADV01.h"
#include "NSMVG4CPUV01.h"
#include "Swd0pcadFunc.h"
#include "Common.h"
#include "CmErrMsg.h"

using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
NSMVSWD0PCADV01::NSMVSWD0PCADV01( sc_module_name name, NSMVG4CPUV01* pG4cpu ):
    sc_module( name ),
    BusSlaveBase<BUS_WIDTH_APB, 1>(),
    BusTimeBase(),
    resetz( "resetz" ),
    clk( "clk" ),
    start_addr( "start_addr" ),
    mpSwd0pcadFunc( 0 )
{
  setSlaveResetPort32( &resetz );
  setSlaveFreqPort32( &clk );
  setTargetSocket32( "ts" );

  ts = tSocket32[0];

  char inst_name[128];
  sprintf( inst_name, "%s.Swd0pcadFunc", this->name() );
  mpSwd0pcadFunc = new Swd0pcadFunc( inst_name );
  mpSwd0pcadFunc->setG4cpuPtr( pG4cpu );

  mBusSlaveIf32[0]->setFuncModulePtr( mpSwd0pcadFunc );
  mBusSlaveIf32[0]->setBusProtocol( BUS_APB );
  mBusSlaveIf32[0]->setTransNmbLmt( 1 );
  mBusSlaveIf32[0]->setReadLatency( 1 );
  mBusSlaveIf32[0]->setWriteLatency( 1 );

  // initialize process
  SC_METHOD( InitProc );

  // start_addr port updat process
  SC_METHOD( StartAddrUpdateProc );
  sensitive << start_addr;
  dont_initialize();

  // reset process
  SC_METHOD( ResetProc );
  sensitive << resetz.pos();
  dont_initialize();

}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
NSMVSWD0PCADV01::~NSMVSWD0PCADV01( void )
{
  delete mpSwd0pcadFunc;
}


// set function
/// <summary>set access size of register</summary>
/// <param name="size">access size</param>
/// <returns>none</returns>
void NSMVSWD0PCADV01::setRegAccSize( unsigned int size )
{
  switch ( size ) {
  case 1: (mpSwd0pcadFunc->getRegPtr())->SetAccessSize(G_REG_SIZE_1BYTE); break;
  case 2: (mpSwd0pcadFunc->getRegPtr())->SetAccessSize(G_REG_SIZE_2BYTE); break;
  case 4: (mpSwd0pcadFunc->getRegPtr())->SetAccessSize(G_REG_SIZE_4BYTE); break;
  default:
    sprintf( mErrMsg, "Invalid access size(%d).", size );
    printErrorMessage( this->name(), "setRegAccSize", mErrMsg );
    break;
  }
}

/// <summary>clear access size of register</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void NSMVSWD0PCADV01::clrRegAccSize( void )
{
  (mpSwd0pcadFunc->getRegPtr())->ClearAccessSize();
}


// public functions
/// <summary>set PEID of related PE</summary>
/// <param name="peid">related PE's PEID</param>
/// <returns>none</returns>
void NSMVSWD0PCADV01::setPeId( unsigned int peid )
{
  mpSwd0pcadFunc->setPeId(peid);
}


// private functions
/// <summary>reset function</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void NSMVSWD0PCADV01::resetFunc( void )
{
  unsigned int addr = start_addr.read();
#ifdef TEST_SWDT_CPU
  printf("%s:%s:resetFunc read start_addr value=0x%08x\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name(),
	 addr);
  fflush(stdout);
#endif

  unsigned char data[4];

  for ( int i=0; i<4; i++ ) {
    data[i] = (unsigned char)((addr >> i*8) & 0xff);
  }

#ifdef TEST_SWDT_CPU
  printf("%s:%s:resetFunc call Swd0pcadFunc's writeReg(0, 0x%x 0x%x 0x%x 0x%x, 4, false)\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name(),
	 *data,
	 *(data+1),
	 *(data+2),
	 *(data+3));
  fflush(stdout);
#endif
  mpSwd0pcadFunc->writeReg(0, data, 4, true);
}


// processes
/// <summary>initialization process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void NSMVSWD0PCADV01::InitProc( void )
{
#ifdef TEST_SWDT_CPU
  printf("%s:%s:InitProc invoked.\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name());
  fflush(stdout);
#endif
  resetFunc();
}


/// <summary>initialization process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void NSMVSWD0PCADV01::StartAddrUpdateProc( void )
{
#ifdef TEST_SWDT_CPU
  printf("%s:%s:StartAddrUpdateProc invoked.\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name());
  fflush(stdout);
#endif
  resetFunc();
}


/// <summary>reset process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void NSMVSWD0PCADV01::ResetProc( void )
{
#ifdef TEST_SWDT_CPU
  printf("%s:%s:RestProc invoked.\n",
	 sc_time_stamp().to_string().c_str(),
	 this->name());
  fflush(stdout);
#endif
  if(resetz.read()){
#ifdef TEST_SWDT_CPU
    printf("%s:%s:RestProc call resetFunc().\n",
	   sc_time_stamp().to_string().c_str(),
	   this->name());
    fflush(stdout);
#endif
    resetFunc();
  }
}

