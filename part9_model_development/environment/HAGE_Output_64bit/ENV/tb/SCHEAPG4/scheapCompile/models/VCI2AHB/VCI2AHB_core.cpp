/*************************************************************************
 *
 *  VCI2AHB_core.cpp
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

#include "VCI2AHB.h"
#include "VCI2AHB_core.h"

/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
VCI2AHB_core::
VCI2AHB_core( BusBridgeModuleBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB> *parent, 
	      BusBridgeSlaveIf<BUS_WIDTH_VCI>                  *slaveIf,
	      BusMasterIf<BUS_WIDTH_AHB>                       *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
VCI2AHB_core::~VCI2AHB_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
VCI2AHB_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransAhb( inTrans, outTrans );
}

/// <summary>setting function for AHB extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
VCI2AHB_core::
setTransAhb( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmVciExtension *inVciExt  = (TlmVciExtension *)0;
  TlmAhbExtension *outAhbExt = new TlmAhbExtension;
  inTrans.get_extension( inVciExt );
  if( inVciExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmVciExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    // All AHB extension members are always default value 
    // because they are not supported in VCI
    outTrans.set_extension( outAhbExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
VCI2AHB_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmAhbExtension *AhbExt = (TlmAhbExtension *)0;
  trans.get_extension( AhbExt );
  trans.release_extension( AhbExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_VCI,BUS_WIDTH_AHB>::tstFp,
	  "VCI2AHB_core::release_extension(AhbExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

