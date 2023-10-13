/*************************************************************************
 *
 *  VCI2AXI_core.h
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

#ifndef __VCI2AXI_CORE_H__
#define __VCI2AXI_CORE_H__

#include "global.h"
#include "BusBridgeCoreBase.h"

class VCI2AXI;

class VCI2AXI_core : 
public BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AXI>
{
 public:
  VCI2AXI_core( BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_AXI> *parent, 
		BusBridgeSlaveIf<BUS_WIDTH_VCI>    *slaveIf,
		BusMasterIf<BUS_WIDTH_AXI>         *masterIf );

  ~VCI2AXI_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

 private:
  void setTransAxi( TlmBasicPayload &inTrans, 
		    TlmBasicPayload &outTrans );
};


#endif // !__VCI2AXI_CORE_H__
