/*
* Copyright(c) 2016 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#include "CrgFunc.h"
#include "NSMVG4CPUV01.h"

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
CrgFunc::CrgFunc( const char* name ):
  BusSlaveFuncIf(),
  mpG4cpu(0)
{
  strcpy( mModName, name );
}

  
/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
CrgFunc::~CrgFunc(void)
{
}
  

///<summary>set memory size</summary>
///<param name="size">memory size</param>
///<returns>none</returns>
void CrgFunc::setfunc( ADDRESS_TYPE size )
{
}
  

///<summary>reset</summary>
///<returns>none</returns>
void CrgFunc::reset( void )
{
}
  

///<summary>set pointer of NSMVG4CPUV01's instance</summary>
///<param name="pG4cpu">pointer of NSMVG4CPUV01 module</param>
///<returns>none</returns>
void CrgFunc::setG4cpuPtr( NSMVG4CPUV01* pG4cpu )
{
  mpG4cpu = pG4cpu;
}


///<summary>TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void CrgFunc::read ( unsigned int offsetAddress, 
		     TlmBasicPayload& trans, 
		     BusTime_t* t, 
		     bool debug )
{
#ifdef TEST_GRD_DBG
  unsigned long addr = trans.get_address();
  printf("CrgFunc::read addr=0x%08x\n", (unsigned int)addr);
  fflush(stdout);
#endif
  mpG4cpu->CrgRegAcc( trans, true );
}
  

///<summary>TLM write function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void CrgFunc::write ( unsigned int offsetAddress, 
		     TlmBasicPayload& trans, 
		     BusTime_t* t, 
		     bool debug )
{
#ifdef TEST_GRD_DBG
  unsigned long addr = trans.get_address();
  printf("CrgFunc::write addr=0x%08x\n", (unsigned int)addr);
  fflush(stdout);
#endif
  mpG4cpu->CrgRegAcc( trans, false );
}

