/*************************************************************************
 *
 *  APB2APB_core.h
 *
 *  Copyright(c) 2018 Renesas Electronics Corporation
 *  RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 *  This program must be used solely for the purpose for which
 *  it was furnished by Renesas Electronics Corporation. No part of this
 *  program may be reproduced or disclosed to others, in any
 *  form, without the prior written permission of Renesas Electronics
 *  Corporation.
 *
 *************************************************************************/

#ifndef __APB2APB_CORE_H__
#define __APB2APB_CORE_H__

#include "global.h"
#include "BusBridgeCoreBase.h"

class APB2APB;

class APB2APB_core : 
public BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>
{
 public:
  APB2APB_core( BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB> *parent, 
		BusBridgeSlaveIf<BUS_WIDTH_APB>    *slaveIf,
		BusMasterIf<BUS_WIDTH_APB>         *masterIf );

  ~APB2APB_core(void);

  void setTransOthers( TlmBasicPayload &inTrans, 
		       TlmBasicPayload &outTrans);
 
  void deleteTransOthers( TlmBasicPayload &trans );

  void setfunc( ADDRESS_TYPE size );

 private:
  void setTransApb( TlmBasicPayload &inTrans, 
		    TlmBasicPayload &outTrans );
};


#endif // !__APB2APB_CORE_H__
