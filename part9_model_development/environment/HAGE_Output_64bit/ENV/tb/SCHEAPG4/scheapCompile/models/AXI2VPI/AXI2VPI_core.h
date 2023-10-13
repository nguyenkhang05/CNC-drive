/*************************************************************************
 *
 *  AXI2VCI_core.h
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

#ifndef __AXI2VPI_CORE_H__
#define __AXI2VPI_CORE_H__

#include "global.h"
#include "BusBridgeCoreBase.h"

class AXI2VPI;

class AXI2VPI_core : 
public BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI>
{
 public:
  AXI2VPI_core( BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI> *parent, 
		BusBridgeSlaveIf<BUS_WIDTH_AXI>    *slaveIf,
		BusMasterIf<BUS_WIDTH_VPI>         *masterIf );

  ~AXI2VPI_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

 private:
  void setTransVpi( TlmBasicPayload &inTrans, 
		    TlmBasicPayload &outTrans );
};


#endif // !__AXI2VPI_CORE_H__
