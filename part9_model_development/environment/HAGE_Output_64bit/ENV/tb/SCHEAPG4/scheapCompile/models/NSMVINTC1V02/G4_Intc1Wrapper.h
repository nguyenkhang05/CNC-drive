/*************************************************************************
 * $Id: G4_Intc1Wrapper.h,v 1.3 2020/02/24 06:19:35 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_INTC1WRAPPER_H
#define  G4_INTC1WRAPPER_H

#include "systemc.h"
#include "NSMVINTC1V02.h"

class G4_Intc1Wrapper : public NSMVINTC1V02
{
public:
  /* public functions */
  G4_Intc1Wrapper( sc_module_name name, const char* pCfgFileName, unsigned int peid );
  virtual ~G4_Intc1Wrapper( void );
  void read ( unsigned int addr, unsigned char* data, unsigned int size );
  void write( unsigned int addr, unsigned char* data, unsigned int size );

  //------------ for user coding from here -------------------
  //void user_function( void );
  //------------ for user coding up to here ------------------
};

#endif // G4_INTC1WRAPPER_H
