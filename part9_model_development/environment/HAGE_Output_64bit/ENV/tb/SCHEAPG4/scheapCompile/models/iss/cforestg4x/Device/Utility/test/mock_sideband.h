/**
 * @file mock_sideband.h
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for sideband.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include "gmock/gmock.h"
#include "sideband.h"

class MockSideBand : public SideBand {
public:
  MOCK_METHOD1(SetBitPos, void(uint32_t));
  MOCK_METHOD1(SetBitOp, void(uint32_t));
  MOCK_METHOD1(SetSecure, void(uint32_t));
  MOCK_METHOD1(SetDM, void(uint32_t));
  MOCK_METHOD1(SetUM, void(uint32_t));
  MOCK_METHOD1(SetBMID, void(uint32_t));
  MOCK_METHOD1(SetSPID, void(uint32_t));

  MOCK_METHOD0(GetBitPos, uint32_t());
  MOCK_METHOD0(GetBitOp, uint32_t());
  MOCK_METHOD0(GetSecure, uint32_t());
  MOCK_METHOD0(GetDM, uint32_t());
  MOCK_METHOD0(GetUM, uint32_t());
  MOCK_METHOD0(GetBMID, uint32_t());
  MOCK_METHOD0(GetSPID, uint32_t());
  MOCK_METHOD0(GetValue, uint32_t());

  MOCK_METHOD0(IsDebugAccess, bool());
  MOCK_METHOD0(DebugAccess, SideBand());
};
