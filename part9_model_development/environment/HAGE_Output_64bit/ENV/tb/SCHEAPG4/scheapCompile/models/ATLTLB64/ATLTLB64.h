/*************************************************************************
 *
 *  ATLTLB64.h
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

#ifndef ATLTLB64_H
#define ATLTLB64_H

template<unsigned int BUSWIDTH> class OSCI2ARBTR;
template<unsigned int BUSWIDTH> class OSCI2DCDR;

class ATLTLB64 : 
public sc_module
{

 public:
  //Reset (active LOW)
  sc_in<bool>            resetPort;
  //frequency
  sc_in<sc_dt::uint64>   freqPort;
  //TLM2 initiator socket
  TlmInitiatorSocket<64> is;
  //TLM2 targeet socket
  TlmTargetSocket<64>    ts;

  ATLTLB64( sc_module_name moduleName,
	  const char     *pMapFileName );

  ~ATLTLB64( void );

 private:
  OSCI2ARBTR<64>   *ARBITER;
  OSCI2DCDR<64>    *DECODER;

#ifdef LB_TST
  FILE *fp;
#endif //LB_TST

};

#endif
