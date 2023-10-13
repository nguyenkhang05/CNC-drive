/*************************************************************************
 *
 *  VCI2APB_core.cpp
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

#include "VCI2APB.h"
#include "VCI2APB_core.h"


/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
VCI2APB_core::
VCI2APB_core( BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_APB> *parent, 
	      BusBridgeSlaveIf<BUS_WIDTH_VCI>    *slaveIf,
	      BusMasterIf<BUS_WIDTH_APB>         *masterIf ):
  BusBridgeCoreLockBase<BUS_WIDTH_VCI,BUS_WIDTH_APB>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
VCI2APB_core::~VCI2APB_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
VCI2APB_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransApb( inTrans, outTrans );
}

/// <summary>setting function for APB extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
VCI2APB_core::
setTransApb( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmVciExtension *inVciExt  = (TlmVciExtension *)0;
  TlmApbExtension *outApbExt = new TlmApbExtension;
  inTrans.get_extension( inVciExt );
  if( inVciExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmVciExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_APB>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
#if 0  // Currently, lock and exclutsion are not used
    outApbExt->setLock( false );
    outApbExt->setExclusion( false );
#endif // Currently, lock and exclutsion are not used
    outTrans.set_extension( outApbExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
VCI2APB_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmApbExtension *ApbExt = (TlmApbExtension *)0;
  trans.get_extension( ApbExt );
  trans.release_extension( ApbExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_APB>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_APB>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_APB>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_APB>::tstFp,
	  "VCI2APB_core::release_extension(ApbExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

/// <summary>check if the transaction is Read-Modify-Write</summary>
/// <param name="trans">generic payload</param>
/// <returns>check result</returns>
bool 
VCI2APB_core::
checkIfRMW( TlmBasicPayload& trans )
{
  TlmVciExtension *pVciExtension = NULL;
  trans.get_extension( pVciExtension );

  if( !pVciExtension ){
    // error (not support TlmVciExtension)
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "Received transaction doesn't support the TlmVciExtension." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_APB>::mParent->name(),
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

