/*************************************************************************
 * $Id: G4_Intc1FuncUsr.h,v 1.4 2020/02/24 06:19:35 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_INTC1FUNCUSR_H
#define  G4_INTC1FUNCUSR_H

#include "G4_Intc1FuncApi.h"

class NSMVINTC1V02;
class G4_Intc1Wrapper;

class G4_Intc1FuncUsr : public G4_Intc1FuncApi
{
 public:
  /* public functions */
  G4_Intc1FuncUsr( void );
  virtual ~G4_Intc1FuncUsr( void );
  void setWrapperPtr( NSMVINTC1V02* ptr );

  void read_precallout  ( TlmBasicPayload& trans, BusTime_t* t, bool debug );
  void read_postcallout ( TlmBasicPayload& trans, BusTime_t* t, bool debug );
  void write_precallout ( TlmBasicPayload& trans, BusTime_t* t, bool debug );
  void write_postcallout( TlmBasicPayload& trans, BusTime_t* t, bool debug );
 
  void ReqEiintToCpu_callout   ( uint32_t channel, bool bindmode, uint32_t gpid, uint32_t bgpr);
  void ReqFenmiToCpu_callout   ();
  void ReqFeintToCpu_callout   ( uint32_t channel, bool bindmode, uint32_t gpid, uint32_t bge );
  void CancelEiintToCpu_callout( uint32_t channel );
  void CancelFenmiToCpu_callout();
  void CancelFeintToCpu_callout( void );
 private:
  /* variables */
  G4_Intc1Wrapper *mpWrapper; // pointer to INTC1 wrapper module
};

#endif // G4_INTC1FUNCUSR_H
