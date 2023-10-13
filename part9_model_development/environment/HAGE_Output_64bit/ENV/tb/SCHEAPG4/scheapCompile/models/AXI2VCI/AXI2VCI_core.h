/*************************************************************************
 *
 *  AXI2VCI_core.h
 *
 *  Copyright(c) 2012 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#ifndef __AXI2VCI_CORE_H__
#define __AXI2VCI_CORE_H__

#include "global.h"
#include "BusBridgeCoreBase.h"

class AXI2VCI;

class AXI2VCI_core : 
public BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI>
{
 public:
  AXI2VCI_core( BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI> *parent, 
		BusBridgeSlaveIf<BUS_WIDTH_AXI>    *slaveIf,
		BusMasterIf<BUS_WIDTH_VCI>         *masterIf );

  ~AXI2VCI_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

 private:
  void setTransVci( TlmBasicPayload &inTrans, 
		    TlmBasicPayload &outTrans );
};


#endif // !__AXI2VCI_CORE_H__
