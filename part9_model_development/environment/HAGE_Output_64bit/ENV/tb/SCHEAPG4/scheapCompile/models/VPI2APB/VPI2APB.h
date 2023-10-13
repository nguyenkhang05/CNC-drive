/*************************************************************************
 *
 *  VPI2APB.h
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

#ifndef VPI2APB_H
#define VPI2APB_H

#include "global.h"
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class VPI2APB_core;

class VPI2APB : 
public BusBridgeModuleBase<BUS_WIDTH_VPI,BUS_WIDTH_APB>,
public BusBridgeSlaveBase<BUS_WIDTH_VPI,1>, 
public BusMasterBase<BUS_WIDTH_APB,1>
{

 public:
  sc_in<uint64>          apbFreqPort;  //APB bus frequensy port

  sc_in<uint64>          vpiFreqPort;  //VPI bus frequency port

  TlmTargetSocket<BUS_WIDTH_VPI>    *ts;  //target socket

  TlmInitiatorSocket<BUS_WIDTH_APB> *is;  //initiator socket

  VPI2APB( sc_module_name moduleName );

  ~VPI2APB( void );

 private:
#ifdef BUS_TST
  FILE *fp;
#endif //BUS_TST

};

#endif
