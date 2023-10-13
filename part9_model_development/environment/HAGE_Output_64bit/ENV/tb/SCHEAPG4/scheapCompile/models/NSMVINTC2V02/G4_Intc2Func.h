/*************************************************************************
 * $Id: G4_Intc2Func.h,v 1.4 2020/02/24 06:58:32 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_INTC2FUNC_H
#define  G4_INTC2FUNC_H

#include "intc2.h"
#include "BusSlaveFuncIf.h"
#include "OSCI2.h"

class PeidInfo;
class NSMVINTC2V02;
class G4_Intc2FuncUsr;

class G4_Intc2Func : public CIntc2, public BusSlaveFuncIf
{
public:
  /* constructor */
  G4_Intc2Func( std::vector<PeidInfo*>* pPeidInfo, unsigned int baseAddr, NSMVINTC2V02* pIntc2, Intc2ImplType intc2_type );
  /* destructor */
  ~G4_Intc2Func( void );

  /* bus I/F functions */
  void read ( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug=0 );
  void write( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug=0 );
  void setfunc( ADDRESS_TYPE size ) {}

  /* public functions */
  void         reset( void );
  unsigned int getQueueSize( void );

private:
  /* private variables */
  NSMVINTC2V02*           mpIntc2;        // pointer to INTC2 module
  std::vector<PeidInfo*>* mpPeidInfo;     // pointer to PeidInfo vector
  bool                    mEicWriteFlag;  // flag of writing EIC register from Bus I/F
  bool                    mEicUpdateFlag; // flag of updated EIC register by CIntc2
  unsigned int            mEicPeid;       // PEID of i2_g_eiint port to be binded
  ContextId               mEicTcid;       // TCID of i2_g_eiint port to be binded
  unsigned int            mEicUpdateCh;   // channel of updated EIC register by CIntc2
  bool                    mEeicWriteFlag;  // flag of writing EEIC register from Bus I/F
  bool                    mEeicUpdateFlag; // flag of updated EEIC register by CIntc2
  unsigned int            mEeicPeid;       // PEID of i2_g_eiint port to be binded
  ContextId               mEeicTcid;       // TCID of i2_g_eiint port to be binded
  unsigned int            mEeicUpdateCh;   // channel of updated EEIC register by CIntc2
  bool                    mEibdUpdateFlag;
  bool                    mEibgUpdateFlag;
  G4_Intc2FuncUsr*        mpIntc2FuncUsr; // pointer to user-function class

  /* Override functions */
  virtual void  StartCyclicHandler( void );
  virtual void  StopCyclicHandler( void ) { /* do nothing */ }
  virtual void  ReqIntToIntc1( uint32_t peid, uint32_t channel, IntReqInfoBase* reqinfo );
  virtual void  CancelIntToIntc1( uint32_t peid, uint32_t channel );
  virtual void  CancelIntByMask( uint32_t channel );
  virtual void  UpdateHighPriority( uint32_t peid );
  virtual void  UpdateHighPriorityOfIntc1( uint32_t peid, uint32_t channel );
  virtual bool  IsIntReqInQueueOfIntc1( uint32_t peid, uint32_t channel );
  virtual void  AddBcasIntInfo( uint32_t channel );
};

#endif // G4_INTC2FUNC_H

