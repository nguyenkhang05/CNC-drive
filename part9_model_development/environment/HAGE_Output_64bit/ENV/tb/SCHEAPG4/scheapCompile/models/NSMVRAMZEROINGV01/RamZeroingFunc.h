/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  RAM_ZEROING_FUNC_H
#define  RAM_ZEROING_FUNC_H

#include <string>
#include "BusMasterFuncIf.h"
#include "BusMasterIf.h"

class RamZeroingFunc: public BusMasterFuncIf
{
public:
  /* constructor */
  RamZeroingFunc( const char* name, BusMasterIf<64>* busIf );

  /* destructor */
  virtual ~RamZeroingFunc( void );


  void setParameter( unsigned int stAddr,
                     unsigned int mxAddr,
                     unsigned int asAddr,
                     bool         selin );
  void writeMemValue( unsigned int           addr,
                      unsigned long long int data,
                      unsigned int           byte );
  unsigned long long int readMemValue( unsigned int addr, unsigned int byte );
  void zeroing( unsigned int mode );

private:
  /* private variables */
  BusMasterIf<64>* mpBusIf; // pointer to bus I/F
  std::string      mModNameStr; // module name
  unsigned int mStAddr; // start addres of RAM
  unsigned int mMaxAddr;// max address of RAM(relative address)
  unsigned int mAsAddr; // start or end address of zeroing at 'mode==1'(relative address)
  bool         mSelin;  // 0: mStAddr+mAsAddr~mStAddr+mMaxAddr  1: 0x0~mStAddr+mAsAddr
};

#endif // RAM_ZEROING_FUNC_H

