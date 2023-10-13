/*************************************************************************
 *
 *  AHB2AXI.h
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

#ifndef AHB2AXI_H
#define AHB2AXI_H

#include "global.h"
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class AHB2AXI_core;

class AHB2AXI : 
public BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI>,
public BusBridgeSlaveBase<BUS_WIDTH_AHB,1>, 
public BusMasterBase<BUS_WIDTH_AXI,1>
{

 public:
  sc_in<uint64>          ahbFreqPort;  //AHB bus frequensy port

  sc_in<uint64>          axiFreqPort;  //AXI bus frequency port

  TlmTargetSocket<BUS_WIDTH_AHB>    *ts;  //target socket

  TlmInitiatorSocket<BUS_WIDTH_AXI> *is;  //initiator socket

  AHB2AXI( sc_module_name moduleName );

  ~AHB2AXI( void );

 private:
#ifdef BUS_TST
  FILE *fp;
#endif //BUS_TST

};

#endif
