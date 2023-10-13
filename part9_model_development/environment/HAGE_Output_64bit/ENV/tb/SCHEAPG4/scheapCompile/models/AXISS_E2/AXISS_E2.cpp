/*************************************************************************
 *
 *  AXISS_E2.cpp
 *
 *  Copyright(c) 2016 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#include "BACKWARD_DECODER.h"
#include "OSCI2DCDR.h"

#include "AXISS_E2.h"

using namespace CmErrMsg;


/// <summary>constructor</summary>
/// <param name="name">name of sc_module</param>
AXISS_E2::
AXISS_E2( sc_module_name moduleName,
     const char     *pMapFileName ) :
  sc_module( moduleName ),
  isx( "isx" ),
  tsx( "tsx" )
{
  
#ifdef BUS_TST
  char fileName[1024];
  strcpy(fileName, moduleName);
  strcat(fileName, ".log");
  if( (fp=fopen(fileName,"w")) == NULL ){
    printf("\ncommon_bus:UT test file open error!!\n\n");
    fflush(stdout);
  }
#endif //BUS_TST
  
  //----- instantiation
#ifdef BUS_TST
  BW_DECODER = 
    new BACKWARD_DECODER<BUS_WIDTH_AXI>( "BW_DECODER", fp );
  DECODER = 
    new OSCI2DCDR<BUS_WIDTH_AXI>( "DECODER", pMapFileName, fp );
#else //BUS_TST
  BW_DECODER = 
    new BACKWARD_DECODER<BUS_WIDTH_AXI>( "BW_DECODER" );
  DECODER = 
    new OSCI2DCDR<BUS_WIDTH_AXI>( "DECODER", pMapFileName );
#endif //BUS_TST
  
  //----- Bind for bus
  tsx(BW_DECODER->ts);
  BW_DECODER->is(DECODER->ts);
  DECODER->is(isx);

  //----- Bind for system
  
  //----- Setting

}

/// <summary>destructor</summary>
AXISS_E2::
~AXISS_E2( void )
{
  delete BW_DECODER;
  delete DECODER;
}

