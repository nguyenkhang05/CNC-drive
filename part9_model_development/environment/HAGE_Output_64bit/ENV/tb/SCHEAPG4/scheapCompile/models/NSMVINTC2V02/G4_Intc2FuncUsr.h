/*************************************************************************
 * $Id: G4_Intc2FuncUsr.h,v 1.3 2020/02/24 06:58:32 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_INTC2FUNCUSR_H
#define  G4_INTC2FUNCUSR_H

#include "G4_Intc2FuncApi.h"

class NSMVINTC2V02;
class G4_Intc2Wrapper;

class G4_Intc2FuncUsr : public G4_Intc2FuncApi
{
public:
  /* public functions */
  G4_Intc2FuncUsr( void );
  virtual ~G4_Intc2FuncUsr( void );
  void setWrapperPtr( NSMVINTC2V02* ptr );

  void read_precallout  ( TlmBasicPayload& trans, BusTime_t* t, bool debug );
  void read_postcallout ( TlmBasicPayload& trans, BusTime_t* t, bool debug );
  void write_precallout ( TlmBasicPayload& trans, BusTime_t* t, bool debug );
  void write_postcallout( TlmBasicPayload& trans, BusTime_t* t, bool debug );
  void ReqEiintToIntc1_callout   ( uint32_t peid, uint32_t channel );
  void CancelEiintToIntc1_callout( uint32_t peid, uint32_t channel );

private:
  /* variables */
  G4_Intc2Wrapper *mpWrapper; // pointer to INTC2 wrapper module
};

#endif // G4_INTC2FUNCUSR_H
