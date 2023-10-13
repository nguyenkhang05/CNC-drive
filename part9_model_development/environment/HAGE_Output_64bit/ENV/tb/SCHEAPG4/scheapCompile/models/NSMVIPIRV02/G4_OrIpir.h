/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_OR_IPIR_H
#define  G4_OR_IPIR_H

#include "systemc.h"
#include "global.h"
#include "G4_intc_def.h"

template <class T, int N>
class OrUnit;

class G4_OrIpir
{
public:
  sc_in<bool>*  in_pe_ch_bit[PE_MAX_NUM][INTC_IPIR_CH_NUM][PE_MAX_NUM]; // EIINT request port from IPIR
  sc_in<bool>*  in_pe_ch_ext[PE_MAX_NUM][INTC_IPIR_CH_NUM]; // EIINT request port from peripheral
  sc_out<bool>* out_pe_ch[PE_MAX_NUM][INTC_IPIR_CH_NUM]; // EIINT request port to INTC1

  /* constructor */
  G4_OrIpir( const char* name, const char* pCfgFileName );

  /* destructor */
  virtual ~G4_OrIpir( void );

private:
  /* private variables */
  const char*  mpModName; // module name
  unsigned int mPeNum;    // number of PEs

  OrUnit<bool, PE_MAX_NUM+1>* mpOrUnit[PE_MAX_NUM][INTC_IPIR_CH_NUM];  // OR logic circuit

  /* private functions */
  void readCfgFile( const char *filename );
};

#endif // G4_OR_IPIR_H

