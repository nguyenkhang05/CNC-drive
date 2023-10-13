/*
* Copyright(c) 2014 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#include "WtbufFunc.h"
#include "NSMVG4CPUV01.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
WtbufFunc::WtbufFunc( const char* name ):
  BusSlaveFuncIf(),
  mpG3mcpu(0)
{
  strcpy( mModName, name );
}

  
/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
WtbufFunc::~WtbufFunc(void)
{
}
  

///<summary>set memory size</summary>
///<param name="size">memory size</param>
///<returns>none</returns>
void WtbufFunc::setfunc( ADDRESS_TYPE size )
{
}
  

///<summary>reset</summary>
///<returns>none</returns>
void WtbufFunc::reset( void )
{
}
  

///<summary>set pointer of NSMVG4CPUV01's instance</summary>
///<param name="pG3mcpu">pointer of NSMVG4CPUV01 module</param>
///<returns>none</returns>
void WtbufFunc::setG3mcpuPtr( NSMVG4CPUV01* pG3mcpu )
{
  mpG3mcpu = pG3mcpu;
}


///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void WtbufFunc::read ( unsigned int offsetAddress, 
		       TlmBasicPayload& trans, 
		       BusTime_t* t, 
		       bool debug )
{
#ifdef TEST_GRD_DBG
  unsigned long addr = trans.get_address();
  printf("WtbufFunc::read addr=0x%08x\n", (unsigned int)addr);
  fflush(stdout);
#endif
  mpG3mcpu->WtbufRegAcc( trans, true );
}
  

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void WtbufFunc::write ( unsigned int offsetAddress, 
			TlmBasicPayload& trans, 
			BusTime_t* t, 
			bool debug )
{
#ifdef TEST_GRD_DBG
  unsigned long addr = trans.get_address();
  printf("WtbufFunc::write addr=0x%08x\n", (unsigned int)addr);
  fflush(stdout);
#endif
  mpG3mcpu->WtbufRegAcc( trans, false );
}

