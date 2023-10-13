/*************************************************************************
 *
 *  AXI2VPI.h
 *
 *  Copyright(c) 2015 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#ifndef AXI2VPI_H
#define AXI2VPI_H

#include "global.h"
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class AXI2VPI_core;

class AXI2VPI : 
public BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI>,
public BusBridgeSlaveBase<BUS_WIDTH_AXI,1>, 
public BusMasterBase<BUS_WIDTH_VPI,1>
{

 public:
  sc_in<uint64>          axiFreqPort;  //VPI bus frequensy port

  sc_in<uint64>          vpiFreqPort;  //AHB bus frequency port

  TlmTargetSocket<BUS_WIDTH_AXI>    *ts;  //target socket

  TlmInitiatorSocket<BUS_WIDTH_VPI> *is;  //initiator socket

  AXI2VPI( sc_module_name moduleName );

  ~AXI2VPI( void );

 private:
#ifdef BUS_TST
  FILE *fp;
#endif //BUS_TST

};

#endif
