/*************************************************************************
 *
 *  VPI2APB_core.h
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

#ifndef __VPI2APB_CORE_H__
#define __VPI2APB_CORE_H__

#include "global.h"
#include "BusBridgeCoreLockBase.h"

class VPI2APB;

class VPI2APB_core : 
public BusBridgeCoreLockBase<BUS_WIDTH_VPI,BUS_WIDTH_APB>
{
 public:
  VPI2APB_core( BusBridgeModuleBase<BUS_WIDTH_VPI,BUS_WIDTH_APB> *parent, 
		BusBridgeSlaveIf<BUS_WIDTH_VPI>    *slaveIf,
		BusMasterIf<BUS_WIDTH_APB>         *masterIf );

  ~VPI2APB_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

  bool checkIfRMW( TlmBasicPayload& trans );
  
 private:
  void setTransApb( TlmBasicPayload &inTrans, 
		    TlmBasicPayload &outTrans );
};


#endif // !__VPI2APB_CORE_H__
