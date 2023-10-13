/*************************************************************************
 *
 *  AHB2AHB_core.cpp
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

#include "AHB2AHB.h"
#include "AHB2AHB_core.h"


/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
AHB2AHB_core::
AHB2AHB_core( BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB> *parent, 
			    BusBridgeSlaveIf<BUS_WIDTH_AHB>    *slaveIf,
			    BusMasterIf<BUS_WIDTH_AHB>         *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
AHB2AHB_core::~AHB2AHB_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
AHB2AHB_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransAhb( inTrans, outTrans );
}

/// <summary>setting function for AHB extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
AHB2AHB_core::
setTransAhb( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmAhbExtension *inAhbExt  = (TlmAhbExtension *)0;
  TlmAhbExtension *outAhbExt = new TlmAhbExtension;
  inTrans.get_extension( inAhbExt );
  if( inAhbExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmAhbExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    outAhbExt->setBurstType( inAhbExt->getBurstType() );
    outAhbExt->setLock( inAhbExt->isLock() );
    outAhbExt->setCachable( inAhbExt->isCachable() );
    outAhbExt->setBufferable( inAhbExt->isBufferable() );
    outAhbExt->setPrivilege( inAhbExt->isPrivilege() );
    outAhbExt->setDataOrOp( inAhbExt->getDataOrOp() );
    outTrans.set_extension( outAhbExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
AHB2AHB_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmAhbExtension *AhbExt = (TlmAhbExtension *)0;
  trans.get_extension( AhbExt );
  trans.release_extension( AhbExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AHB>::tstFp,
	  "AHB2AHB_core::release_extension(AhbExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

/// <summary>set memory size</summary>
/// <param name="size">memory size </param>
void
AHB2AHB_core::
setfunc( ADDRESS_TYPE size )
{
  if(!mSlaveIf->targetSocket.mPureTlmSocketList[0]){
    mMasterIf->initiatorSocket.mTargetSocketList[0]->setBaseAddressSize(mSlaveIf->targetSocket.getBaseAddress(),size);
  }
}
