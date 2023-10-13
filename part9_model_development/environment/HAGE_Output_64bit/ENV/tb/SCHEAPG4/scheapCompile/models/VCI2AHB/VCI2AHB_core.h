/*************************************************************************
 *
 *  VCI2AHB_core.h
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

#ifndef __VCI2AHB_CORE_H__
#define __VCI2AHB_CORE_H__

#include "global.h"
#include "BusBridgeCoreBase.h"

class VCI2AHB;

class VCI2AHB_core : 
public BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB>
{
 public:
  VCI2AHB_core( BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB> *parent, 
		BusBridgeSlaveIf<BUS_WIDTH_VCI>    *slaveIf,
		BusMasterIf<BUS_WIDTH_AHB>         *masterIf );

  ~VCI2AHB_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

 private:
  void setTransAhb( TlmBasicPayload &inTrans, 
		    TlmBasicPayload &outTrans );
};


#endif // !__VCI2AHB_CORE_H__
