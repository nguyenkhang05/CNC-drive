/*************************************************************************
 *
 *  VCI2VCI32_core.cpp
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

#include "VCI2VCI32.h"
#include "VCI2VCI32_core.h"


/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
VCI2VCI32_core::
VCI2VCI32_core( BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32> *parent, 
	      BusBridgeSlaveIf<BUS_WIDTH_VCI>    *slaveIf,
	      BusMasterIf<BUS_WIDTH_VCI32>         *masterIf ):
  BusBridgeCoreLockBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
VCI2VCI32_core::~VCI2VCI32_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
VCI2VCI32_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransVci32( inTrans, outTrans );
}

/// <summary>setting function for VCI extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
VCI2VCI32_core::
setTransVci32( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmVciExtension *inVciExt  = (TlmVciExtension *)0;
  TlmVciExtension *outVciExt = new TlmVciExtension;
  inTrans.get_extension( inVciExt );
  if( inVciExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmVciExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    outVciExt->setRequestType( inVciExt->getRequestType() );
    outTrans.set_extension( outVciExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
VCI2VCI32_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmVciExtension *VciExt = (TlmVciExtension *)0;
  trans.get_extension( VciExt );
  trans.release_extension( VciExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32>::tstFp,
	  "VCI2VCI32_core::release_extension(VciExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

/// <summary>check if the transaction is Read-Modify-Write</summary>
/// <param name="trans">generic payload</param>
/// <returns>check result</returns>
bool 
VCI2VCI32_core::
checkIfRMW( TlmBasicPayload& trans )
{
  TlmVciExtension *pVciExtension = NULL;
  trans.get_extension( pVciExtension );

  if( !pVciExtension ){
    // error (not support TlmVciExtension)
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "Received transaction doesn't support the TlmVciExtension." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VCI32>::mParent->name(),
				 0, errorMessageBuf );
    return false;
  }

  TlmVciExtension::VciRequest_t requestType = pVciExtension->getRequestType();
  if( requestType == TlmVciExtension::CMD_BITOP_SET ||
      requestType == TlmVciExtension::CMD_BITOP_AND ||
      requestType == TlmVciExtension::CMD_BITOP_OR  ||
      requestType == TlmVciExtension::CMD_BITOP_XOR ||
      requestType == TlmVciExtension::CMD_CAXI      ){
    return true;
  }
  else{
    return false;
  }
}

