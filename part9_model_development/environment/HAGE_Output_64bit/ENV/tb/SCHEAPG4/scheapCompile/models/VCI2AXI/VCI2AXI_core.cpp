/*************************************************************************
 *
 *  VCI2AXI_core.cpp
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

#include "VCI2AXI.h"
#include "VCI2AXI_core.h"


/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
VCI2AXI_core::
VCI2AXI_core( BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_AXI> *parent, 
			    BusBridgeSlaveIf<BUS_WIDTH_VCI>    *slaveIf,
			    BusMasterIf<BUS_WIDTH_AXI>         *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AXI>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
VCI2AXI_core::~VCI2AXI_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
VCI2AXI_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransAxi( inTrans, outTrans );
}

/// <summary>setting function for AXI extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
VCI2AXI_core::
setTransAxi( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmVciExtension *inVciExt  = (TlmVciExtension *)0;
  TlmAxiExtension *outAxiExt = new TlmAxiExtension;
  inTrans.get_extension( inVciExt );
  if( inVciExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmAxiExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AXI>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    switch( inVciExt->getRequestType() ){
    case TlmVciExtension::CMD_BITOP_SET :
    case TlmVciExtension::CMD_BITOP_AND :
    case TlmVciExtension::CMD_BITOP_OR  :
    case TlmVciExtension::CMD_BITOP_XOR :
    case TlmVciExtension::CMD_CAXI      :
    case TlmVciExtension::CMD_EXCL_LDLW :
    case TlmVciExtension::CMD_EXCL_STCW :
      outAxiExt->setBitOpType( TlmAxiExtension::BITOP_SET1 );
      break;
    default :
      outAxiExt->setBitOpType( TlmAxiExtension::BITOP_NON );
      break;
    }
    // All AXI extension members are always default value 
    // because they are not supported in the VP
    outTrans.set_extension( outAxiExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
VCI2AXI_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmAxiExtension *AxiExt = (TlmAxiExtension *)0;
  trans.get_extension( AxiExt );
  trans.release_extension( AxiExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AXI>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AXI>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AXI>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AXI>::tstFp,
	  "VCI2AXI_core::release_extension(AxiExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

