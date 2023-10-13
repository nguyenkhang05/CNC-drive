/*************************************************************************
 *
 *  AXI2VPI_core.cpp
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

#include "AXI2VPI.h"
#include "AXI2VPI_core.h"

/// <summary>constructor</summary>
/// <param name="parent">pointer of parent class</param>
/// <param name="slaveIf">pointer of bus slave I/F</param>
/// <param name="masterIf">pointer of bus master I/F</param>
AXI2VPI_core::
AXI2VPI_core( BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI> *parent, 
	      BusBridgeSlaveIf<BUS_WIDTH_AXI>                  *slaveIf,
	      BusMasterIf<BUS_WIDTH_VPI>                       *masterIf ):
  BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI>(parent,slaveIf,masterIf)
{}

/// <summary>destructor</summary>
AXI2VPI_core::~AXI2VPI_core( void ){}

/// <summary>setting function for other extended payload</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void
AXI2VPI_core::
setTransOthers( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans)
{
  setTransVpi( inTrans, outTrans );
}

/// <summary>setting function for VPI extension transaction</summary>
/// <param name="inTrans">input transaction </param>
/// <param name="outTrans">output transaction </param>
void 
AXI2VPI_core::
setTransVpi( TlmBasicPayload &inTrans, TlmBasicPayload &outTrans )
{
  TlmAxiExtension *inAxiExt  = (TlmAxiExtension *)0;
  TlmVpiExtension *outVpiExt = new TlmVpiExtension;
  inTrans.get_extension( inAxiExt );
  if( inAxiExt == NULL ){
    char errorMessageBuf[1024];
    sprintf( errorMessageBuf,
	     "There is no TlmAxiExtension payload." );
    CmErrMsg::printErrorMessage( BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI>::mParent->name(), 
				 0,errorMessageBuf );
  }
  else{
    switch( inAxiExt->getBitOpType() ){
    case TlmAxiExtension::BITOP_NON :
      outVpiExt->setRequestType( TlmVpiExtension::CMD_READ );
      break;
    default :
      outVpiExt->setRequestType( TlmVpiExtension::CMD_BITOP_SET );
      break;
    }
    outTrans.set_extension( outVpiExt );
  }
}

/// <summary>delete other extended payload</summary>
/// <param name="trans">transaction </param>
void 
AXI2VPI_core::
deleteTransOthers( TlmBasicPayload &trans )
{
  TlmVpiExtension *VpiExt = (TlmVpiExtension *)0;
  trans.get_extension( VpiExt );
  trans.release_extension( VpiExt );
#ifdef BUSIF_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI>::tstFp,
	  "%s:",sc_time_stamp().to_string().c_str());
#ifdef PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI>::tstFp,
	  "%s\n        ",
	  BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI>::mParent->name());
#endif //PLTFRM_TST
  fprintf(BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_VPI>::tstFp,
	  "AXI2VPI_core::release_extension(VpiExt) was called for 0x%x\n",
	  &trans);
#endif //BUSIF_TST
}

