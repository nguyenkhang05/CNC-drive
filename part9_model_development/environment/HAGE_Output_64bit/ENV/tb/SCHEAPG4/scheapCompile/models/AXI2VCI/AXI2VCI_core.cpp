/*************************************************************************
 *
 *  AXI2VCI_core.cpp
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

#include "AXI2VCI.h"
#include "AXI2VCI_core.h"

/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
AXI2VCI_core::
AXI2VCI_core( BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI> *parent, 
	      BusBridgeSlaveIf<BUS_WIDTH_AXI>                  *slaveIf,
	      BusMasterIf<BUS_WIDTH_VCI>                       *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
AXI2VCI_core::~AXI2VCI_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
AXI2VCI_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransVci( inTrans, outTrans );
}

/// <summary>setting function for VCI extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
AXI2VCI_core::
setTransVci( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmAxiExtension *inAxiExt  = (TlmAxiExtension *)0;
  TlmVciExtension *outVciExt = new TlmVciExtension;
  inTrans.get_extension( inAxiExt );
  if( inAxiExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmAxiExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    // All AXI extension members are always default value 
    // because they are not supported in VCI
    outTrans.set_extension( outVciExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
AXI2VCI_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmVciExtension *VciExt = (TlmVciExtension *)0;
  trans.get_extension( VciExt );
  trans.release_extension( VciExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VCI>::tstFp,
	  "AXI2VCI_core::release_extension(VciExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

