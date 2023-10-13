/*************************************************************************
 *
 *  APB2APB.h
 *
 *  Copyright(c) 2018 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#ifndef APB2APB_H
#define APB2APB_H

#include "global.h"
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class APB2APB_core;

class APB2APB : 
public BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB>,
public BusBridgeSlaveBase<BUS_WIDTH_APB,1>, 
public BusMasterBase<BUS_WIDTH_APB,1>
{

 public:
  sc_in<uint64>          apbSFreqPort;  //APB bus slave frequensy port

  sc_in<uint64>          apbMFreqPort;  //APB bus maser frequency port

  TlmTargetSocket<BUS_WIDTH_APB>    *ts;  //target socket

  TlmInitiatorSocket<BUS_WIDTH_APB> *is;  //initiator socket

  APB2APB( sc_module_name moduleName );

  ~APB2APB( void );

 private:
  APB2APB_core *mpAPB2APB_core;           //APB2APB core class pointer
#ifdef BUS_TST
  FILE *fp;
#endif //BUS_TST

};

#endif
