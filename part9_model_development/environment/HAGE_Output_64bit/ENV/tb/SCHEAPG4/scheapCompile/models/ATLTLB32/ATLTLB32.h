/*************************************************************************
 *
 *  ATLTLB32.h
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

#ifndef ATLTLB32_H
#define ATLTLB32_H

template<unsigned int BUSWIDTH> class OSCI2ARBTR;
template<unsigned int BUSWIDTH> class OSCI2DCDR;

class ATLTLB32 : 
public sc_module
{

 public:
  //Reset (active LOW)
  sc_in<bool>            resetPort;
  //frequency
  sc_in<sc_dt::uint64>   freqPort;
  //TLM2 initiator socket
  TlmInitiatorSocket<32> is;
  //TLM2 targeet socket
  TlmTargetSocket<32>    ts;

  ATLTLB32( sc_module_name moduleName,
	  const char     *pMapFileName );

  ~ATLTLB32( void );

 private:
  OSCI2ARBTR<32>   *ARBITER;
  OSCI2DCDR<32>    *DECODER;

#ifdef LB_TST
  FILE *fp;
#endif //LB_TST

};

#endif
