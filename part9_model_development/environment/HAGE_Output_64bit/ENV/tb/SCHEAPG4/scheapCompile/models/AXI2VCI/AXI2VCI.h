/*************************************************************************
 *
 *  AXI2VCI.h
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

#ifndef AXI2VCI_H
#define AXI2VCI_H

#include "global.h"
#include "BusBridgeModuleBase.h"
#include "BusBridgeSlaveBase.h"
#include "BusMasterBase.h"

template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusBridgeSlaveBase;
template<unsigned int BUSWIDTH, unsigned int S_NUM> class BusMasterBase;
template<unsigned int BUSWIDTH> class TlmTargetSocket;
template<unsigned int BUSWIDTH> class TlmInitiatorSocket;

class AXI2VCI_core;

class AXI2VCI : 
public BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI>,
public BusBridgeSlaveBase<BUS_WIDTH_AXI,1>, 
public BusMasterBase<BUS_WIDTH_VCI,1>
{

 public:
  sc_in<uint64>          axiFreqPort;  //VCI bus frequensy port

  sc_in<uint64>          vciFreqPort;  //AHB bus frequency port

  TlmTargetSocket<BUS_WIDTH_AXI>    *ts;  //target socket

  TlmInitiatorSocket<BUS_WIDTH_VCI> *is;  //initiator socket

  AXI2VCI( sc_module_name moduleName );

  ~AXI2VCI( void );

 private:
#ifdef BUS_TST
  FILE *fp;
#endif //BUS_TST

};

#endif
