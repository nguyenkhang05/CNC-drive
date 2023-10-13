/*************************************************************************
 *
 *  APB2APB_core.cpp
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

#include "APB2APB.h"
#include "APB2APB_core.h"


/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
APB2APB_core::
APB2APB_core( BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB> *parent, 
			    BusBridgeSlaveIf<BUS_WIDTH_APB>    *slaveIf,
			    BusMasterIf<BUS_WIDTH_APB>         *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
APB2APB_core::~APB2APB_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
APB2APB_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransApb( inTrans, outTrans );
}

/// <summary>setting function for APB extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
APB2APB_core::
setTransApb( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmApbExtension *inApbExt  = (TlmApbExtension *)0;
  TlmApbExtension *outApbExt = new TlmApbExtension;
  inTrans.get_extension( inApbExt );
  if( inApbExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmApbExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    outApbExt->setLock( inApbExt->isLock() );
    outApbExt->setExclusion( inApbExt->isExclusion() );
    outTrans.set_extension( outApbExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
APB2APB_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmApbExtension *ApbExt = (TlmApbExtension *)0;
  trans.get_extension( ApbExt );
  trans.release_extension( ApbExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::tstFp,
	  "APB2APB_core::release_extension(ApbExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

/// <summary>set memory size</summary>
/// <param name="size">memory size </param>
void
APB2APB_core::
setfunc( ADDRESS_TYPE size )
{
  if(!mSlaveIf->targetSocket.mPureTlmSocketList[0]){
    mMasterIf->initiatorSocket.mTargetSocketList[0]->setBaseAddressSize(mSlaveIf->targetSocket.getBaseAddress(),size);
  }
}
