/**
 * @file test_mecnt.cpp
 *
 * @date Created on: 2015
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for mecnt.
 * @par Copyright: 2015 Renesas Electronics Corp.
 */
#include "mock_sideband.h"
#include "mecnt.h"

using ::testing::AtLeast;
using ::testing::_;

class MecntTest : public testing::Test,
                 public testing::WithParamInterface<
                     std::tuple<PhysAddr, int32_t, ByteSize, SysErrCause>> {
protected:
  MecntTest(){}
  virtual ~MecntTest() {}
  virtual void SetUp() {
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);
    mecnt = new Mecnt();
  }
  virtual void TearDown() { delete mecnt; }

public:
  MockSideBand sideband;
  Mecnt* mecnt;
private:
};

/*!
 * @brief Check MECNT register read/write operation.
 */
TEST_P(MecntTest, CheckMecntRegReadWrite) {
  PhysAddr addr = std::get<0>(GetParam());
  int32_t temp = static_cast<int32_t>(std::get<1>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = {0, 0, 0, 0, 0, 0, 0, 0};
  ByteSize size = std::get<2>(GetParam());
  SysErrCause result = std::get<3>(GetParam());

  ASSERT_EQ(mecnt->TargetWrite(sideband, addr, wdata, size), result);
  ASSERT_EQ(mecnt->TargetRead(sideband, addr, rdata, size), result);
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(wdata[i], rdata[i]);
  }
}

std::tuple<PhysAddr, int32_t, ByteSize, SysErrCause> const
    RW_ParametersTable[] = {
        /* 0000 */ std::make_tuple(0xc000, 0xf4f3f2f1, 4, SYSERR_NONE),
        /* 0001 */ std::make_tuple(0xc004, 0xf8f7f6f5, 2, SYSERR_NONE),
        /* 0002 */ std::make_tuple(0xc008, 0xfcfbfaf9, 1, SYSERR_NONE),
        // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters1, MecntTest,
                        ::testing::ValuesIn(RW_ParametersTable));

/*!
 * @brief Check MECNT register read/write operation with debug interface.
 */
TEST_P(MecntTest, CheckMecntRegReadWriteWithDebugIF) {
  PhysAddr addr = std::get<0>(GetParam());
  int32_t temp = static_cast<int32_t>(std::get<1>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = {0, 0, 0, 0, 0, 0, 0, 0};
  ByteSize size = std::get<2>(GetParam());
  SysErrCause result = std::get<3>(GetParam());

  ASSERT_EQ(mecnt->TargetWriteDebug(addr, wdata, size), result);
  ASSERT_EQ(mecnt->TargetReadDebug(addr, rdata, size), result);
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(wdata[i], rdata[i]);
  }
}

std::tuple<PhysAddr, int32_t, ByteSize, SysErrCause> const
    RWD_ParametersTable[] = {
        /* 0000 */ std::make_tuple(0xc000, 0xf4f3f2f1, 4, SYSERR_NONE),
        /* 0001 */ std::make_tuple(0xc004, 0xf8f7f6f5, 2, SYSERR_NONE),
        /* 0002 */ std::make_tuple(0xc008, 0xfcfbfaf9, 1, SYSERR_NONE),
        // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters2, MecntTest,
                        ::testing::ValuesIn(RWD_ParametersTable));

/*!
 * @brief Check reset operation
 */
TEST_F(MecntTest, CheckResetMecntReg) {
  PhysAddr addr = 0xd000;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = {1, 1, 1, 1, 1, 1, 1, 1};
  ByteSize size = 4;
  SysErrCause result = SYSERR_NONE;

  ASSERT_EQ(mecnt->TargetWriteDebug(addr, wdata, size), result);
  mecnt->HardReset();
  ASSERT_EQ(mecnt->TargetReadDebug(addr, rdata, size), result);
  for (auto i = 0; i < size; i++) {
    ASSERT_NE(wdata[i], rdata[i]);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
