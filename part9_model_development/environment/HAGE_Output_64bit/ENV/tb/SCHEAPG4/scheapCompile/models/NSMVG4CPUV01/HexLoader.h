/*
* Copyright(c) 2012-2013 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef HEXLOADER_H
#define HEXLOADER_H

#include "global.h"

class HexLoader
{
  
 public:

 protected:
  
 private:
  /// <summary>load the hex file to iss</summary>
  /// <param name="addr">address</param>
  /// <param name="data">data</param>
  /// <param name="peid">PE ID(PEID-1)</param>
  /// <returns>none</returns>
  virtual void LoadSrecBody (unsigned int addr, unsigned char data, unsigned int peid) = 0;

 public:
  HexLoader( void );
  ~HexLoader( void );
  void LoadSrec (const char* filename, unsigned int offset, unsigned int peid);
  
  
}; // end of HexLoader definition

#endif /* HEXLOADER_H */
