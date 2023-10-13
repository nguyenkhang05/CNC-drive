/*************************************************************************
 *
 *  VCI2VPI_core.cpp
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

#include "VCI2VPI.h"
#include "VCI2VPI_core.h"

/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
VCI2VPI_core::
VCI2VPI_core( BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_VPI> *parent, 
	      BusBridgeSlaveIf<BUS_WIDTH_VCI>                  *slaveIf,
	      BusMasterIf<BUS_WIDTH_VPI>                       *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VPI>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
VCI2VPI_core::~VCI2VPI_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
VCI2VPI_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransVpi( inTrans, outTrans );
}

/// <summary>setting function for VPI extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
VCI2VPI_core::
setTransVpi( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmVciExtension *inVciExt  = (TlmVciExtension *)0;
  TlmVpiExtension *outVpiExt = new TlmVpiExtension;
  inTrans.get_extension( inVciExt );
  if( inVciExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmVciExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VPI>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    switch( inVciExt->getRequestType() ){
    case TlmVciExtension::CMD_READ:
      outVpiExt->setRequestType( TlmVpiExtension::CMD_READ );
      break;
    case TlmVciExtension::CMD_WRITE:
      outVpiExt->setRequestType( TlmVpiExtension::CMD_WRITE );
      break;
    case TlmVciExtension::CMD_BITOP_SET:
      outVpiExt->setRequestType( TlmVpiExtension::CMD_BITOP_SET );
      break;
    case TlmVciExtension::CMD_BITOP_AND:
      outVpiExt->setRequestType( TlmVpiExtension::CMD_BITOP_AND );
      break;
    case TlmVciExtension::CMD_BITOP_OR:
      outVpiExt->setRequestType( TlmVpiExtension::CMD_BITOP_OR );
      break;
    case TlmVciExtension::CMD_BITOP_XOR:
      outVpiExt->setRequestType( TlmVpiExtension::CMD_BITOP_XOR );
      break;
    case TlmVciExtension::CMD_SYNC_BUS:
      outVpiExt->setRequestType( TlmVpiExtension::CMD_SYNC_BUS );
      break;
    case TlmVciExtension::CMD_SYNC_MASTER:
      outVpiExt->setRequestType( TlmVpiExtension::CMD_SYNC_MASTER );
      break;
    case TlmVciExtension::CMD_CAXI:
    case TlmVciExtension::CMD_EXCL_LDLW:
    case TlmVciExtension::CMD_EXCL_STCW:
      outVpiExt->setRequestType( TlmVpiExtension::CMD_BITOP_SET );
      break;
    default:
      break;
    }
    outTrans.set_extension( outVpiExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
VCI2VPI_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmVpiExtension *VpiExt = (TlmVpiExtension *)0;
  trans.get_extension( VpiExt );
  trans.release_extension( VpiExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VPI>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VPI>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VPI>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_VPI>::tstFp,
	  "VCI2VPI_core::release_extension(VpiExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

