/*************************************************************************
 * $Id: G4_Intc1Func.h,v 1.9 2020/02/24 06:19:35 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_INTC1FUNC_H
#define  G4_INTC1FUNC_H

#include "BusSlaveFuncIf.h"
#include "OSCI2.h"
#include "intc1.h"

class NSMVINTC1V02;
class G4_Intc1FuncUsr;

class G4_Intc1Func : public CIntc1, public BusSlaveFuncIf
{
public:
  // added 4th argument by Yohshinaga for G4MH 2.0 (new cforestg4)
  // Please set the suitable core version (CORE_VER_10, CORE_VER_11, CORE_VER_20) from [G4CPU_PE_TYPE]
  G4_Intc1Func( unsigned int peNum, unsigned int baseAddr, NSMVINTC1V02* pIntc1, unsigned int core_ver=CORE_VER_10 );
  ~G4_Intc1Func( void );

  /* bus I/F functions */
  void read ( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug=0);
  void write( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug=0 );
  void setfunc( ADDRESS_TYPE size ) {}

  /* public functions */
  void            reset( void );
  IntReqInfoBase* getQueueReqInfo( uint32_t channel );
  unsigned int    getQueueSize( void );

private:
  /* variables */
  NSMVINTC1V02* mpIntc1;        // pointer to INTC1 module
  bool          mEicWriteFlag;  // flag of writing EIC register from Bus I/F
  bool          mEicUpdateFlag; // flag of updated EIC register by CIntc1
  bool          mEeicWriteFlag; // flag of writing EEIC register from Bus I/F
  bool          mEeicUpdateFlag;// flag of updated EEIC register by CIntc1
  unsigned int  mEicUpdateCh;   // channel of updated EIC register by CIntc1
  unsigned int  mEeicUpdateCh;  // channel of updated EEIC register by CIntc1
  bool          mEibdUpdateFlag;
  bool          mEibgUpdateFlag;
  unsigned int  mCoreVersion;   // core version
  unsigned int  mPeid;  
  G4_Intc1FuncUsr* mpIntc1FuncUsr; // pointer to user-function class

  /* function*/

  /* private functions to override */
  virtual void StartCyclicHandler( void );
  virtual void StopCyclicHandler( CIntc1* p_intc1 ){ /* do nothing */ }
  virtual void ReqFenmiToCpu( void (*resfunc)(CIntc1* intc1) );
  virtual void ReqFeintToCpu( uint32_t channel, bool bindmode, uint32_t gpid, uint32_t bge, void (*resfunc)(uint32_t channel, CIntc1* intc1) );
  virtual void ReqEiintToCpu(
                 uint32_t priority,
                 uint32_t channel, bool eitb, bool bindmode, uint32_t gpid, uint32_t bgpr,
                 void (*resfunc)(uint32_t channel, CIntc1* intc1) );
  virtual void CancelIntToCpu( uint32_t channel );
  virtual void CancelIntToCpu( uint32_t channel, bool dummy );
  virtual void CancelFenmiToCpu( void );
  virtual void CancelFeintToCpu( void );
  virtual void CancelFeintToCpu( bool dummy ); 
  virtual void CancelFeintToCpu( uint32_t channel, bool dummy ); 
  virtual void UpdateInterruptPort( uint32_t channel );
  virtual void InitPriorityInfo( void );
};

#endif // G4_INTC1FUNC_H
