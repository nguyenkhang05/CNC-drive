/*************************************************************************
 *
 *  AHB2AXI_core.cpp
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

#include "AHB2AXI.h"
#include "AHB2AXI_core.h"


/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
AHB2AXI_core::
AHB2AXI_core( BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI> *parent, 
			    BusBridgeSlaveIf<BUS_WIDTH_AHB>    *slaveIf,
			    BusMasterIf<BUS_WIDTH_AXI>         *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
AHB2AXI_core::~AHB2AXI_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
AHB2AXI_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransAxi( inTrans, outTrans );
}

/// <summary>setting function for AXI extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
AHB2AXI_core::
setTransAxi( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmAhbExtension *inAhbExt  = (TlmAhbExtension *)0;
  TlmAxiExtension *outAxiExt = new TlmAxiExtension;
  inTrans.get_extension( inAhbExt );
  if( inAhbExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmAxiExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    switch( inAhbExt->getBurstType() ){
    case TlmAhbExtension::BURST_FIXED :
      outAxiExt->setBurstType( TlmAxiExtension::BURST_FIXED );
      break;
    case TlmAhbExtension::BURST_INCR :
      outAxiExt->setBurstType( TlmAxiExtension::BURST_INCR );
      break;
    case TlmAhbExtension::BURST_WRAP :
      outAxiExt->setBurstType( TlmAxiExtension::BURST_WRAP );
      break;
    }
    outAxiExt->setCachable( inAhbExt->isCachable() );
    outAxiExt->setBufferable( inAhbExt->isBufferable() );
#if 0 // these members are not used for CC-Cube
    outAxiExt->setLock( inAhbExt->isLock() );
    outAxiExt->setSecure( 0 );
    outAxiExt->setTransId( 0 );
    outAxiExt->setBitOpType( TlmAxiExtension::BITOP_NON );
    outAxiExt->setBitOpPos( 0 );
#endif// these members are not used for CC-Cube
    outTrans.set_extension( outAxiExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
AHB2AXI_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmAxiExtension *AxiExt = (TlmAxiExtension *)0;
  trans.get_extension( AxiExt );
  trans.release_extension( AxiExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_AXI>::tstFp,
	  "AHB2AXI_core::release_extension(AxiExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

