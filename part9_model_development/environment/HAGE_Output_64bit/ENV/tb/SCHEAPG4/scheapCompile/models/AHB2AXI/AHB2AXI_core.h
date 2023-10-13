/*************************************************************************
 *
 *  AHB2AXI_core.h
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

#ifndef __AHB2AXI_CORE_H__
#define __AHB2AXI_CORE_H__

#include "global.h"
#include "BusBridgeCoreBase.h"

class AHB2AXI;

class AHB2AXI_core : 
public BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI>
{
 public:
  AHB2AXI_core( BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI> *parent, 
		BusBridgeSlaveIf<BUS_WIDTH_AHB>    *slaveIf,
		BusMasterIf<BUS_WIDTH_AXI>         *masterIf );

  ~AHB2AXI_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

 private:
  void setTransAxi( TlmBasicPayload &inTrans, 
		    TlmBasicPayload &outTrans );
};


#endif // !__AHB2AXI_CORE_H__
