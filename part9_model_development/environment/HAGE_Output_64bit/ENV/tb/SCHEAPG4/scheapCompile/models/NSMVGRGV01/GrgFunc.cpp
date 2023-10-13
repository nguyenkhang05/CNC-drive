/*
* Copyright(c) 2013 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#include "GrgFunc.h"
#include "NSMVG4CPUV01.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
GrgFunc::GrgFunc( const char* name ):
  BusSlaveFuncIf(),
  mpG3mcpu(0)
{
  strcpy( mModName, name );
}

  
/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
GrgFunc::~GrgFunc(void)
{
}
  

///<summary>set memory size</summary>
///<param name="size">memory size</param>
///<returns>none</returns>
void GrgFunc::setfunc( ADDRESS_TYPE size )
{
}
  

///<summary>reset</summary>
///<returns>none</returns>
void GrgFunc::reset( void )
{
}
  

///<summary>set pointer of NSMVG4CPUV01's instance</summary>
///<param name="pG3mcpu">pointer of NSMVG4CPUV01 module</param>
///<returns>none</returns>
void GrgFunc::setG3mcpuPtr( NSMVG4CPUV01* pG3mcpu )
{
  mpG3mcpu = pG3mcpu;
}


///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void GrgFunc::read ( unsigned int offsetAddress, 
		     TlmBasicPayload& trans, 
		     BusTime_t* t, 
		     bool debug )
{
#ifdef TEST_GRD_DBG
  unsigned long addr = trans.get_address();
  printf("GrgFunc::read addr=0x%08x\n", (unsigned int)addr);
  fflush(stdout);
#endif
  mpG3mcpu->GrgRegAcc( trans, true );
}
  

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void GrgFunc::write ( unsigned int offsetAddress, 
		     TlmBasicPayload& trans, 
		     BusTime_t* t, 
		     bool debug )
{
#ifdef TEST_GRD_DBG
  unsigned long addr = trans.get_address();
  printf("GrgFunc::write addr=0x%08x\n", (unsigned int)addr);
  fflush(stdout);
#endif
  mpG3mcpu->GrgRegAcc( trans, false );
}

