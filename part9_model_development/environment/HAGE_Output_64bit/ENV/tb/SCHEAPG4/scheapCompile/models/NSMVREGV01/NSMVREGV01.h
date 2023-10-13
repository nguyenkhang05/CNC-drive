/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  NSMVREGV01_H
#define  NSMVREGV01_H

#include "ATLTSLAVE32.h"

class NSMVREGV01: public ATLTSLAVE32
{
public:
  /* constructor */
  SC_HAS_PROCESS( NSMVREGV01 );
  NSMVREGV01( sc_module_name name, const char* pCfgFileName );

  /* destructor */
  virtual ~NSMVREGV01( void );

private:
  /* processes */
  void ResetProc( void );
};

#endif // NSMVREGV01_H
