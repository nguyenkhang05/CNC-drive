/*************************************************************************
 *
 *  VCI2AHB.h
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

#ifndef VCI2AHB_H
#define VCI2AHB_H

#include "global.h"
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class VCI2AHB_core;

class VCI2AHB : 
public BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB>,
public BusBridgeSlaveBase<BUS_WIDTH_VCI,1>, 
public BusMasterBase<BUS_WIDTH_AHB,1>
{

 public:
  sc_in<uint64>          vciFreqPort;  //VCI bus frequensy port

  sc_in<uint64>          ahbFreqPort;  //AHB bus frequency port

  TlmTargetSocket<BUS_WIDTH_VCI>    *ts;  //target socket

  TlmInitiatorSocket<BUS_WIDTH_AHB> *is;  //initiator socket

  VCI2AHB( sc_module_name moduleName );

  ~VCI2AHB( void );

 private:
#ifdef BUS_TST
  FILE *fp;
#endif //BUS_TST

};

#endif
