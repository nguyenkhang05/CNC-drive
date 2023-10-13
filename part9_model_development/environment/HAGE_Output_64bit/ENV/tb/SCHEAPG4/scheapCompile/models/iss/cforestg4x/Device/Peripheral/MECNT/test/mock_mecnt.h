/**
 * @file mock_mecnt.h
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for mecnt.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include "gmock/gmock.h"
#include "mecnt.h"

class MockTRegG0MEV : public TRegG0MEV {
public:
  MOCK_METHOD0(Reset, void());
  MOCK_CONST_METHOD0(GetName, const char *());
};

class MockTRegMecnt : public TRegMecnt {
public:
  MOCK_METHOD0(InitTRegMap, void());
};

class MockMecnt : public Mecnt {
public:
  MOCK_METHOD0(CyclicHandler, void());
  MOCK_METHOD4(TargetWrite,
               SysErrCause(SideBand, PhysAddr, uint8_t *, ByteSize));
  MOCK_METHOD4(TargetRead,
               SysErrCause(SideBand, PhysAddr, uint8_t *, ByteSize));
  MOCK_METHOD4(TargetFetch,
               SysErrCause(SideBand, PhysAddr, uint8_t *, ByteSize));
  MOCK_METHOD3(TargetWriteDebug, SysErrCause(PhysAddr, uint8_t *, ByteSize));
  MOCK_METHOD3(TargetReadDebug, SysErrCause(PhysAddr, uint8_t *, ByteSize));
  MOCK_METHOD0(HardReset, void());
};
