/*************************************************************************
 *
 *  AXI2AHB_core.cpp
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

#include "AXI2AHB.h"
#include "AXI2AHB_core.h"

/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
AXI2AHB_core::
AXI2AHB_core( BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_AHB> *parent, 
			    BusBridgeSlaveIf<BUS_WIDTH_AXI>    *slaveIf,
			    BusMasterIf<BUS_WIDTH_AHB>         *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AHB>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
AXI2AHB_core::~AXI2AHB_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
AXI2AHB_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransAhb( inTrans, outTrans );
}

/// <summary>setting function for AHB extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
AXI2AHB_core::
setTransAhb( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmAxiExtension *inAxiExt  = (TlmAxiExtension *)0;
  TlmAhbExtension *outAhbExt = new TlmAhbExtension;
  inTrans.get_extension( inAxiExt );
  if( inAxiExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmAxiExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AHB>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    switch( inAxiExt->getBurstType() ){
    case TlmAxiExtension::BURST_FIXED :
      outAhbExt->setBurstType( TlmAhbExtension::BURST_FIXED );
      break;
    case TlmAxiExtension::BURST_INCR :
      outAhbExt->setBurstType( TlmAhbExtension::BURST_INCR );
      break;
    case TlmAxiExtension::BURST_WRAP :
      outAhbExt->setBurstType( TlmAhbExtension::BURST_WRAP );
      break;
    }
    outAhbExt->setLock( inAxiExt->isLock() );
    outAhbExt->setCachable( inAxiExt->isCachable() );
    outAhbExt->setBufferable( inAxiExt->isBufferable() );
    outAhbExt->setPrivilege( false );
    outAhbExt->setDataOrOp( TlmAhbExtension::ACC_DATA );
    outTrans.set_extension( outAhbExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
AXI2AHB_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmAhbExtension *AhbExt = (TlmAhbExtension *)0;
  trans.get_extension( AhbExt );
  trans.release_extension( AhbExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AHB>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AHB>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AHB>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AHB>::tstFp,
	  "AXI2AHB_core::release extension(AhbExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

