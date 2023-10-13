/*************************************************************************
 * $Id: G4_Intc2Wrapper.h,v 1.3 2020/02/24 06:58:32 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_INTC2WRAPPER_H
#define  G4_INTC2WRAPPER_H

#include "systemc.h"
#include "NSMVINTC2V02.h"

class G4_Intc2Wrapper : public NSMVINTC2V02
{
public:
  /* public functions */
  G4_Intc2Wrapper( sc_module_name name, const char* pCfgFileName );
  virtual ~G4_Intc2Wrapper( void );
  void read ( unsigned int addr, unsigned char* data, unsigned int size );
  void write( unsigned int addr, unsigned char* data, unsigned int size );

  //------------ for user coding from here -------------------
  //void user_function( void );
  //------------ for user coding up to here ------------------
};

#endif // G4_INTC2WRAPPER_H
