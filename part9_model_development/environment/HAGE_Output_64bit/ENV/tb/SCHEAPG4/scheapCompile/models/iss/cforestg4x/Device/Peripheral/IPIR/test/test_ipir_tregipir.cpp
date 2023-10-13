/**
 * @file test_ipir_tregipir.cpp
 *
 * @date Created on: 2016
 * @author Author: Yvo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for TRegIPIR. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <memory>
#include <map>
#include <stdexcept>
#include "treg_ipir.h"
#include "ipir.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

class TRegIPIRTest1
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize>> {
protected:
    TRegIPIRTest1() {}
  virtual ~TRegIPIRTest1() {}
  virtual void SetUp() {
    mock_pe =
        new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    ipir = new CIpir(&m_pes, IPIR_STANDARD);
    tregipir = new TRegIpir(ipir,&m_pes);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);
  }
  virtual void TearDown() { delete ipir;
                            delete tregipir;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  PeVec m_pes;
  CIpir *ipir;
  uint32_t pe_id = 0;
  TRegIpir* tregipir;

private:
};


/*!
 * @brief Check TRegIPIR
 */
TEST_F(TRegIPIRTest1, CheckIPIENAddr_) {
  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr addr = tregipir->TrIPIENAddr(channel, ipir_id);
          ASSERT_EQ(addr,TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100);
      }
  }
}

TEST_F(TRegIPIRTest1, TrIPIFLGAddr) {
  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr addr = tregipir->TrIPIFLGAddr(channel, ipir_id);
          ASSERT_EQ(addr,TRegIpir::IPIFLG_OFFSET + channel * 0x20 + ipir_id * 0x100);
      }
  }
}

TEST_F(TRegIPIRTest1, TrIPIREQAddr) {
  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr addr = tregipir->TrIPIREQAddr(channel, ipir_id);
          ASSERT_EQ(addr,TRegIpir::IPIREQ_OFFSET + channel * 0x20 + ipir_id * 0x100);
      }
  }
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
