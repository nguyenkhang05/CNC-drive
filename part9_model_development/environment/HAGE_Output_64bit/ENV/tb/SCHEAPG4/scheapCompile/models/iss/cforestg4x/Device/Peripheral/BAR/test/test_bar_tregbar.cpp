/**
 * @file test_bar_tregbar.cpp
 *
 * @date Created on: 2016
 * @author Author: Yvo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for TRegBar. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <memory>
#include <map>
#include <stdexcept>
#include "bar.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"
#include "mock_fsbus.h"

class TRegBARTest1
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize>> {
protected:
    TRegBARTest1() {}
  virtual ~TRegBARTest1() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);
    tregbar = new TRegBar(bar,&m_pes,mock_fsbus);
  }
  virtual void TearDown() { delete bar;
                            delete tregbar;}

public:
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;
  TRegBar* tregbar;

private:
};


/*!
 * @brief Check TRegBAR
 */
TEST_F(TRegBARTest1, CheckBRINITAddr) {
  for (uint32_t channel =0; channel <16; channel++){
      PhysAddr addr = tregbar->TrBRINITAddr(channel);
      ASSERT_EQ(addr,TRegBar::BRINIT_OFFSET + channel * 0x10);
  }
}

TEST_F(TRegBARTest1, CheckBRENAddr) {
  for (uint32_t channel =0; channel <16; channel++){
      PhysAddr addr = tregbar->TrBRENAddr(channel);
      ASSERT_EQ(addr,TRegBar::BREN_OFFSET + channel * 0x10);
  }
}

TEST_F(TRegBARTest1, CheckBRCHKAddr) {
  for (uint32_t bar_id = 0; bar_id < 8; bar_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr addr = tregbar->TrBRCHKAddr(channel, bar_id);
          ASSERT_EQ(addr,TRegBar::BRCHK_OFFSET + channel * 0x10 + bar_id * 0x100);
      }
  }
}

TEST_F(TRegBARTest1, CheckBRSYNAddr) {
  for (uint32_t bar_id = 0; bar_id < 8; bar_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr addr = tregbar->TrBRSYNAddr(channel, bar_id);
          ASSERT_EQ(addr,TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100);
      }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
