/*************************************************************************
 *
 *  G4_IsIfBase.cpp
 *
 * Copyright(c) 2016, 2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

/* $Id:$ */

#include "G4_IsIfBase.h"

/// <summary>constructor</summary>
G4_IsIfBase::G4_IsIfBase( void )
{
}


/// <summary>destructor</summary>
G4_IsIfBase::~G4_IsIfBase(void)
{
}


/// <summary>Get transaction read data(changed type from unsigned char* to unsigned long long)</summary>
/// <param name="trans">target transaction</param>
/// <returns>data value</returns>
unsigned long long G4_IsIfBase::getTransDataVal(TlmTransactionType &trans)
{
  unsigned long long val=0;
  unsigned int   size   = trans.get_data_length();
  unsigned char* data_p = trans.get_data_ptr();

  for(unsigned int i=0; i<size; i++){
    unsigned long long tmp_val= (unsigned long long)(*(data_p+i));
    val = (tmp_val << i*8) | val;
  }
  fflush(stdout);
  return(val);
}
