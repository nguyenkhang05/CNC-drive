/*************************************************************************
 *
 *  $Id: CorFunction.cpp,v 1.5 2010/11/04 06:08:21 e_arai Exp $
 *
 *  AHB2VCI_core.cpp
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

#include "AHB2VCI.h"
#include "AHB2VCI_core.h"

/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
AHB2VCI_core::
AHB2VCI_core( BusBridgeModuleBase<BUS_WIDTH_AHB,BUS_WIDTH_VCI> *parent, 
	      BusBridgeSlaveIf<BUS_WIDTH_AHB>    *slaveIf,
	      BusMasterIf<BUS_WIDTH_VCI>         *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_VCI>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
AHB2VCI_core::~AHB2VCI_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
AHB2VCI_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransVci( inTrans, outTrans );
}

/// <summary>setting function for VCI extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
AHB2VCI_core::
setTransVci( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmAhbExtension *inAhbExt  = (TlmAhbExtension *)0;
  TlmVciExtension *outVciExt = new TlmVciExtension;
  inTrans.get_extension( inAhbExt );
  if( inAhbExt == NULL ){
#if 0 // need to be sc_module if name() is used
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmVciExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_AHB,BUS_WIDTH_VCI>::mParent->name(), 
				 0,errorMessageBuf );
#endif// need to be sc_module if name() is used
  }
  else{
    // for Burst transfer information
    switch( inAhbExt->getBurstType() ){
    case TlmAhbExtension::BURST_FIXED :
      // Burst transfer should be decomposed to single transfer
      break;
    case TlmAhbExtension::BURST_INCR :
      // Burst transfer should be decomposed to single transfer
      break;
    case TlmAhbExtension::BURST_WRAP :
      // Burst transfer should be decomposed to single transfer
      break;
    }
    // Lock information is ignored in VCI
    // Cache information is ignored in VCI
    // Buffer information is ignored in VCI
    // Privilege information is ignored in VCI
    // DataOrOPcode information is ignored in VCI
    // Request type in VCI is always default.(means "don't care")
    outTrans.set_extension( outVciExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
AHB2VCI_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmVciExtension *VciExt = (TlmVciExtension *)0;
  trans.get_extension( VciExt );
  trans.release_extension( VciExt );
}

