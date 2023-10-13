/*************************************************************************
 *
 *  AHB2VCI_core.h
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

#ifndef __AHB2VCI_CORE_H__
#define __AHB2VCI_CORE_H__

#include "global.h"
#include "BusBridgeCoreBase.h"

class AHB2VCI;

class AHB2VCI_core : 
public BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_VCI>
{
 public:
  AHB2VCI_core( BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_VCI> *parent, 
		BusBridgeSlaveIf<BUS_WIDTH_AHB>    *slaveIf,
		BusMasterIf<BUS_WIDTH_VCI>         *masterIf );

  ~AHB2VCI_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

 private:
  void setTransVci( TlmBasicPayload &inTrans, 
		    TlmBasicPayload &outTrans );
};


#endif // !__AHB2VCI_CORE_H__
