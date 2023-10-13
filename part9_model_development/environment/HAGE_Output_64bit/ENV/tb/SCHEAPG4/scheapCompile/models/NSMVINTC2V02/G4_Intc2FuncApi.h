/*************************************************************************
 * $Id: G4_Intc2FuncApi.h,v 1.3 2020/02/14 02:00:18 synguyen Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef G4_INTC2FUNCAPI_H
#define G4_INTC2FUNCAPI_H

#include "OSCI2.h"
#include "forest_common_api.h"

class G4_Intc2FuncApi
{
public:
  G4_Intc2FuncApi(void) {;}
  virtual ~G4_Intc2FuncApi(void) {;}
  virtual void ReqEiintToIntc1_callout   ( uint32_t peid, uint32_t channel ) = 0;
  virtual void CancelEiintToIntc1_callout( uint32_t peid, uint32_t channel ) = 0;
};
#endif //G4_INTC2FUNCAPI_H
