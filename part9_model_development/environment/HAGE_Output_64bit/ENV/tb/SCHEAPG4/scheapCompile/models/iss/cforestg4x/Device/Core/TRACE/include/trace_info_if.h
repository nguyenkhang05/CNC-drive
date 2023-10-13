/*
 * (c) 2011,2012,2013,2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once
#if __cplusplus >= 201103L
    // For C++11 supported compilers
    #include <cstdint>
#else // #if __cplusplus >= 201103L
    // For C99 and older compilers
    #include <stdint.h>
#endif // #if __cplusplus >= 201103L

class TraceCtrlIF {
public:
  virtual ~TraceCtrlIF() {};

  // enum RegCommand {
  //   REG_READ,
  //   REG_WRITE,
  // };
  // virtual void RecordSReg<REG_WRITE>(CfSReg cfsrid, RegData data) = 0;
  // virtual void RecordSReg<REG_READ>(CfSReg cfsrid, RegData data) = 0;

  virtual int32_t GetInstId(void) const = 0;
};
