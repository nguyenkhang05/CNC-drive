/*************************************************************************
 *
 *  VCI2APB_core.h
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

#ifndef __VCI2APB_CORE_H__
#define __VCI2APB_CORE_H__

#include "global.h"
#include "BusBridgeCoreLockBase.h"

class VCI2APB;

class VCI2APB_core : 
public BusBridgeCoreLockBase<BUS_WIDTH_VCI,BUS_WIDTH_APB>
{
 public:
  VCI2APB_core( BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_APB> *parent, 
		BusBridgeSlaveIf<BUS_WIDTH_VCI>    *slaveIf,
		BusMasterIf<BUS_WIDTH_APB>         *masterIf );

  ~VCI2APB_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

  bool checkIfRMW( TlmBasicPayload& trans );
  
 private:
  void setTransApb( TlmBasicPayload &inTrans, 
		    TlmBasicPayload &outTrans );
};


#endif // !__VCI2APB_CORE_H__
