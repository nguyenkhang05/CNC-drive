/*************************************************************************
 *
 *  AXISS_E2.h
 *
 *  Copyright(c) 2013-2015 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#ifndef AXISS_E2_H
#define AXISS_E2_H

#include "global.h"

template<unsigned int> class BACKWARD_DECODER;
template<unsigned int> class OSCI2DCDR;

class AXISS_E2 : 
public sc_module
{

 public:
  //TLM2 initiator socket for AXI
  TlmInitiatorSocket<BUS_WIDTH_AXI> isx;
  //TLM2 targeet socket for AXI
  TlmTargetSocket<BUS_WIDTH_AXI>    tsx;

  //constructor
  AXISS_E2( sc_module_name moduleName,
	  const char     *pMapFileName );

  //destructor
  ~AXISS_E2( void );

 private:
  // bachward decoder for AXI
  BACKWARD_DECODER<BUS_WIDTH_AXI>  *BW_DECODER;
  // decoder for AXI
  OSCI2DCDR<BUS_WIDTH_AXI>         *DECODER;
  
  
#ifdef BUS_TST
  FILE *fp;
#endif //BUS_TST

};

#endif //AXISS_E2_H
