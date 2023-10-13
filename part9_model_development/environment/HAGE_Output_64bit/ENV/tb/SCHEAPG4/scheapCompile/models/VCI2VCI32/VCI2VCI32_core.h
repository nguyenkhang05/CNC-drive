/*************************************************************************
 *
 *  VCI2VCI32_core.h
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

#ifndef __VCI2VCI32_CORE_H__
#define __VCI2VCI32_CORE_H__

#include "global.h"
#include "BusBridgeCoreLockBase.h"

class VCI2VCI32;

class VCI2VCI32_core : 
public BusBridgeCoreLockBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32>
{
 public:
  VCI2VCI32_core( BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32> *parent, 
		  BusBridgeSlaveIf<BUS_WIDTH_VCI>   *slaveIf,
		  BusMasterIf<BUS_WIDTH_VCI32>      *masterIf );

  ~VCI2VCI32_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

  bool checkIfRMW( TlmBasicPayload& trans );
  
 private:
  void setTransVci32( TlmBasicPayload &inTrans, 
		      TlmBasicPayload &outTrans );
};


#endif // !__VCI2VCI32_CORE_H__
