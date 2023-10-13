/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G0MEV_FUNC_H
#define  G0MEV_FUNC_H

#include "BusSlaveFuncIf.h"
#include "g_reg.h"

class G0MevReg: public GRegDataBase<GRegData32> {
public:
  void Reset( void );

  /* constructor */
  G0MevReg( const char* name );

  /* destructor */
  virtual ~G0MevReg( void );

private:
  /* private variables */
  GRegData32 mData; // register
};

class G0MevFunc: public BusSlaveFuncIf
{
public:
  G0MevFunc( const char* name );
  virtual ~G0MevFunc( void );

  /* public functions */
  void reset( void );

  G0MevReg* getRegPtr( void ); /* for debug */

  /* bus I/F functions */
  void read ( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug = false );
  void write( unsigned int     offsetAddress, 
              TlmBasicPayload& trans, 
              BusTime_t*       t,
              bool             debug = false );
  void setfunc( ADDRESS_TYPE size ) {}

private:
  /* private variables */
  char      mModName[128]; // module name
  G0MevReg* mpG0MevReg;    // pointer to MEV register module
};

#endif // G0MEV_FUNC_H

