/*************************************************************************
 * $Id: G4_Intc1FuncApi.h,v 1.4 2020/02/14 02:00:14 synguyen Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_INTC1FUNCAPI_H
#define  G4_INTC1FUNCAPI_H

#include "OSCI2.h"
#include "forest_common_api.h"

class G4_Intc1FuncApi {
public:
  G4_Intc1FuncApi(void) {;}
  virtual ~G4_Intc1FuncApi(void) {;}
  virtual void read_precallout  ( TlmBasicPayload& trans, BusTime_t* t, bool debug ) = 0;
  virtual void read_postcallout ( TlmBasicPayload& trans, BusTime_t* t, bool debug ) = 0;
  virtual void write_precallout ( TlmBasicPayload& trans, BusTime_t* t, bool debug ) = 0;
  virtual void write_postcallout( TlmBasicPayload& trans, BusTime_t* t, bool debug ) = 0;
  
  virtual void ReqEiintToCpu_callout   ( uint32_t channel, bool bindmode, uint32_t gpid, uint32_t bgpr) = 0;
  virtual void ReqFenmiToCpu_callout   ( ) = 0;
  virtual void ReqFeintToCpu_callout   ( uint32_t channel, bool bindmode, uint32_t gpid, uint32_t bge ) = 0;
  virtual void CancelEiintToCpu_callout( uint32_t channel ) = 0;
  virtual void CancelFenmiToCpu_callout() = 0;
  virtual void CancelFeintToCpu_callout( void ) = 0;
};
#endif //G4_INTC1FUNCAPI_H
