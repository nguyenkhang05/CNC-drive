/*************************************************************************
 *
 * NSMVREGV01.cpp
 *
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include "NSMVREGV01.h"


/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pCfgFileName">configuration file name</param>
/// <returns>none</returns>
NSMVREGV01::NSMVREGV01(
  sc_module_name name,
  const char*    pCfgFileName ):
    ATLTSLAVE32( name, 1, 1 )
{
  // reset process
  SC_METHOD( ResetProc );
  sensitive << resetPort.neg();
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
NSMVREGV01::~NSMVREGV01( void )
{
  // do nothing
}


// processes
/// <summary>reset process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVREGV01::ResetProc( void )
{
  mFunc->reset();
}
