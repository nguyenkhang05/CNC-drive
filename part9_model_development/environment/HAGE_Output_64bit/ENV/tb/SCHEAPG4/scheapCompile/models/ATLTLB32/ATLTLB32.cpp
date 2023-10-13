/*************************************************************************
 *
 *  ATLTLB32.cpp
 *
 *  Copyright(c) 2011 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#include "OSCI2ARBTR.h"
#include "OSCI2DCDR.h"

#include "ATLTLB32.h"

using namespace CmErrMsg;


ATLTLB32::
ATLTLB32( sc_module_name moduleName,
	const char     *pMapFileName
 ) :
  sc_module( moduleName ),
  resetPort( "resetPort" ),
  freqPort( "freqPort" ),
  is( "is" ),
  ts( "ts" )
{

#ifdef LB_TST
  char fileName[1024];
  strcpy(fileName, moduleName);
  strcat(fileName, ".log");
  if( (fp=fopen(fileName,"w")) == NULL ){
    printf("\nATLTLB32:UT test file open error!!\n\n");
    fflush(stdout);
  }
#endif //LB_TST

  //----- instantiation
#ifdef LB_TST
  ARBITER = new OSCI2ARBTR<32>( "ARBITER", fp );
  DECODER = new OSCI2DCDR<32>( "DECODER", pMapFileName, fp );
#else //LB_TST
  ARBITER = new OSCI2ARBTR<32>( "ARBITER" );
  DECODER = new OSCI2DCDR<32>( "DECODER", pMapFileName );
#endif //LB_TST

  //----- Bind
  ts(ARBITER->ts);
  ARBITER->is(DECODER->ts);
  DECODER->is(is);

  ARBITER->freqPort(freqPort);
  ARBITER->resetPort(resetPort);

}

ATLTLB32::
~ATLTLB32( void )
{
  delete ARBITER;
  delete DECODER;
}

