/**
 * @file test_ipir_cipir.cpp
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for CIPIR. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
// #include <memory>
#include <stdexcept>
#include "treg_ipir.h"
#include "ipir.h"
#include "forest_common.h"
#include "mock_proc_element.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;

class IpirRegAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
  IpirRegAccessTest() {}
  virtual ~IpirRegAccessTest() {}
  virtual void SetUp() {
    mock_pe =
        new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    ipir = new CIpir(&m_pes, IPIR_STANDARD);

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
  virtual void TearDown() { delete ipir; }

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  PeVec m_pes;
  CIpir *ipir;
  uint32_t pe_id = 0;

private:
};

/*!
 * @brief Check IPIR register read/write operation.
 */
TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_EN) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
        for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
            PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;
              {
                SCOPED_TRACE("Write");
                if (size == expect_size) {
                  ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
                } else {
                  // Expect throw if "Un-guaranteed Size Access".
                  ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                }
              }
              {
                SCOPED_TRACE("Read");
                if (size == expect_size) {
                  ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
                  for (auto i = 0; i < size; i++) {
                    ASSERT_EQ(wdata[i], rdata[i]);
                  }
                } else {
                  // Expect throw if "Un-guaranteed Size Access".
                  ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                }
                 }
           }
        }
    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_EN[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersEN, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_EN));

TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_FLG) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
            for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
                PhysAddr addr = TRegIpir::IPIFLG_OFFSET + channel * 0x20 + ipir_id * 0x100;
                  {
                    SCOPED_TRACE("Write");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                    }
                  }
                  {
                    SCOPED_TRACE("Read");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
                      for (auto i = 0; i < size; i++) {
                        ASSERT_EQ(0x0, rdata[i]);
                      }
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                    }
                  }
            }
        }

    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_FLG[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0003 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersFLG, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_FLG));


TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_FCLR) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
            for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
                PhysAddr addr = TRegIpir::IPIFCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
                  {
                    SCOPED_TRACE("Write");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                    }
                  }
                  {
                    SCOPED_TRACE("Read");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
                      for (auto i = 0; i < size; i++) {
                        ASSERT_EQ(0x0, rdata[i]);
                      }
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                    }
                  }
            }
        }

    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_FCLR[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0003 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersFCLR, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_FCLR));

TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_REQ) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
            for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
                PhysAddr addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + ipir_id * 0x100;
                  {
                    SCOPED_TRACE("Write");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                    }
                  }
                  {
                    SCOPED_TRACE("Read");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
                      for (auto i = 0; i < size; i++) {
                        ASSERT_EQ(wdata[i], rdata[i]);
                      }
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                    }
                  }
            }
        }

    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_REQ[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0003 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersREQ, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_REQ));

TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_RCLR) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
            for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
                PhysAddr addr = TRegIpir::IPIRCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
                  {
                    SCOPED_TRACE("Write");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                    }
                  }
                  {
                    SCOPED_TRACE("Read");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
                      for (auto i = 0; i < size; i++) {
                        ASSERT_EQ(0x0, rdata[i]);
                      }
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWrite(sideband, addr, wdata, size), std::range_error);
                    }
                  }
            }
        }

    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_RCLR[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0003 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersRCLR, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_RCLR));

/*!
 * @brief Check IPIR register read/write operation with debug interface.
  */

TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_EN_DBG) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
        for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
            PhysAddr addr = TRegIpir::IPIEN_OFFSET  + channel * 0x20 + ipir_id * 0x100;
              {
                SCOPED_TRACE("Write");
                if (size == expect_size) {
                  ASSERT_NO_THROW(ipir->TargetWriteDebug(addr, wdata, size));
                } else {
                  // Expect throw if "Un-guaranteed Size Access".
                  ASSERT_THROW(ipir->TargetWriteDebug( addr, wdata, size), std::range_error);
                }
              }
              {
                SCOPED_TRACE("Read");
                if (size == expect_size) {
                  ASSERT_NO_THROW(ipir->TargetReadDebug(addr, rdata, size));
                  for (auto i = 0; i < size; i++) {
                    ASSERT_EQ(wdata[i], rdata[i]);
                  }
                } else {
                  // Expect throw if "Un-guaranteed Size Access".
                  ASSERT_THROW(ipir->TargetWriteDebug(addr, wdata, size), std::range_error);
                }
                 }
           }
        }
    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_EN_DBG[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersEN_DBG, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_EN_DBG));

TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_FLG_DBG) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
            for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
                PhysAddr addr = TRegIpir::IPIFLG_OFFSET  + channel * 0x20 + ipir_id * 0x100;
                  {
                    SCOPED_TRACE("Write");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetWriteDebug(addr, wdata, size));
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWriteDebug(addr, wdata, size), std::range_error);
                    }
                  }
                  {
                    SCOPED_TRACE("Read");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetReadDebug(addr, rdata, size));
                      for (auto i = 0; i < size; i++) {
                        ASSERT_EQ(0x0, rdata[i]);
                      }
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWriteDebug(addr, wdata, size), std::range_error);
                    }
                  }
            }
        }

    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_FLG_DBG[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0003 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersFLG_DBG, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_FLG_DBG));


TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_FCLR_DBG) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
            for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
                PhysAddr addr = TRegIpir::IPIFCLR_OFFSET  + channel * 0x20 + ipir_id * 0x100;
                  {
                    SCOPED_TRACE("Write");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetWriteDebug(addr, wdata, size));
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWriteDebug(addr, wdata, size), std::range_error);
                    }
                  }
                  {
                    SCOPED_TRACE("Read");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetReadDebug(addr, rdata, size));
                      for (auto i = 0; i < size; i++) {
                        ASSERT_EQ(0x0, rdata[i]);
                      }
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWriteDebug(addr, wdata, size), std::range_error);
                    }
                  }
            }
        }

    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_FCLR_DBG[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0003 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersFCLR_DBG, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_FCLR_DBG));

TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_REQ_DBG) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
            for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
                PhysAddr addr = TRegIpir::IPIREQ_OFFSET  + channel * 0x20 + ipir_id * 0x100;
                  {
                    SCOPED_TRACE("Write");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetWriteDebug(addr, wdata, size));
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWriteDebug(addr, wdata, size), std::range_error);
                    }
                  }
                  {
                    SCOPED_TRACE("Read");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetReadDebug(addr, rdata, size));
                      for (auto i = 0; i < size; i++) {
                        ASSERT_EQ(wdata[i], rdata[i]);
                      }
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWriteDebug(addr, wdata, size), std::range_error);
                    }
                  }
            }
        }

    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_REQ_DBG[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0003 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersREQ_DBG, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_REQ_DBG));

TEST_P(IpirRegAccessTest, CheckIpirRegReadWrite_RCLR_DBG) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    ipir->HardReset();
    for (uint32_t  channel= 0 ; channel < 4; channel++){
            for (uint32_t  ipir_id= 0 ; ipir_id < 8; ipir_id++){
                PhysAddr addr = TRegIpir::IPIRCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
                  {
                    SCOPED_TRACE("Write");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetWriteDebug(addr, wdata, size));
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWriteDebug(addr, wdata, size), std::range_error);
                    }
                  }
                  {
                    SCOPED_TRACE("Read");
                    if (size == expect_size) {
                      ASSERT_NO_THROW(ipir->TargetReadDebug(addr, rdata, size));
                      for (auto i = 0; i < size; i++) {
                        ASSERT_EQ(0x0, rdata[i]);
                      }
                    } else {
                      // Expect throw if "Un-guaranteed Size Access".
                      ASSERT_THROW(ipir->TargetWriteDebug(addr, wdata, size), std::range_error);
                    }
                  }
            }
        }

    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_RCLR_DBG[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0003 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersRCLR_DBG, IpirRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable_RCLR_DBG));

/*!
 * @brief Check reset operation
 */
// Case 1 TRegIPIEN
TEST_F(IpirRegAccessTest, CheckResetIpirReg_EN) {
  PhysAddr addr = 0x0800;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  uint8_t expect_value[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = 1;

  ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
  ipir->HardReset();
  ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(expect_value[i], rdata[i]);
    ASSERT_NE(wdata[i], rdata[i]);
  }
}
// Case 2 TRegIPIFLG
TEST_F(IpirRegAccessTest, CheckResetIpirReg_FLG) {
  PhysAddr addr = 0x0804;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  uint8_t expect_value[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = 1;

  ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
  ipir->HardReset();
  ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(expect_value[i], rdata[i]);
    ASSERT_NE(wdata[i], rdata[i]);
  }
}
// Case 3 TRegIPIFCLR
TEST_F(IpirRegAccessTest, CheckResetIpirReg_FCLR) {
  PhysAddr addr = 0x0808;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  uint8_t expect_value[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = 1;

  ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
  ipir->HardReset();
  ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(expect_value[i], rdata[i]);
    ASSERT_NE(wdata[i], rdata[i]);
  }
}
// Case 4 TRegIPIREQ
TEST_F(IpirRegAccessTest, CheckResetIpirReg_REQ) {
  PhysAddr addr = 0x0810;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  uint8_t expect_value[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = 1;

  ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
  ipir->HardReset();
  ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(expect_value[i], rdata[i]);
    ASSERT_NE(wdata[i], rdata[i]);
  }
}
// Case 5 TRegIPIRCLR
TEST_F(IpirRegAccessTest, CheckResetIpirReg_RCLR) {
  PhysAddr addr = 0x0814;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  uint8_t expect_value[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = 1;

  ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));
  ipir->HardReset();
  ASSERT_NO_THROW(ipir->TargetRead(sideband, addr, rdata, size));
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(expect_value[i], rdata[i]);
    ASSERT_NE(wdata[i], rdata[i]);
  }
}

TEST_F(IpirRegAccessTest, CheckGetIPIREQPtr) {
  ipir->HardReset();
  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + ipir_id * 0x100;
          uint8_t wdata[] = { 0x1A, 0, 0, 0, 0, 0, 0, 0 };
          ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, SIZE_8BIT));
          TRegIPIREQ* ipireq = ipir->GetIPIREQPtr(channel, ipir_id);

          ASSERT_EQ(0x1AU, ipireq->GetData());
      }
  }
}

TEST_F(IpirRegAccessTest, CheckGetIPIENPtr) {
  ipir->HardReset();
  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;
          uint8_t wdata[] = { 0x1A, 0, 0, 0, 0, 0, 0, 0 };
          ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, SIZE_8BIT));
          TRegIPIEN* ipien = ipir->GetIPIENPtr(channel, ipir_id);

          ASSERT_EQ(0x1AU, ipien->GetData());
      }
  }
}

TEST_F(IpirRegAccessTest, CheckGetIPIFLGPtr) {
  ipir->HardReset();
  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr addr = TRegIpir::IPIFLG_OFFSET + channel * 0x20 + ipir_id * 0x100;
          uint8_t wdata[] = { 0x1A, 0, 0, 0, 0, 0, 0, 0 };
          ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, SIZE_8BIT));
          TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);

          ASSERT_EQ(0x0U, ipiflg->GetData());
      }
  }
}

TEST_F(IpirRegAccessTest, CheckGetSet_IPIFLG) {
  ipir->HardReset();
  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          RegData data = 0x1A;
          ipir->Set_IPIFLG(channel, ipir_id,data);
          RegData get_data = ipir->Get_IPIFLG(channel, ipir_id);
          ASSERT_EQ(0x1AU, get_data);
      }
  }
}

TEST_F(IpirRegAccessTest, CheckGetSet_IPIREQ) {
  ipir->HardReset();
  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          RegData data = 0x1A;
          ipir->Set_IPIREQ(channel, ipir_id,data);
          RegData get_data = ipir->Get_IPIREQ(channel, ipir_id);
          ASSERT_EQ(0x1AU, get_data);
      }
  }
}

TEST_F(IpirRegAccessTest, CheckGetSet_IPIEN) {
  ipir->HardReset();
  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          RegData data = 0x1A;
          ipir->Set_IPIEN(channel, ipir_id,data);
          RegData get_data = ipir->Get_IPIEN(channel, ipir_id);
          ASSERT_EQ(0x1AU, get_data);
      }
  }
}


/*!
 * @brief Check IsValidPeId
 */
class IpirTest_IsValidPeId : public testing::Test {
protected:
  IpirTest_IsValidPeId() {}
  virtual ~IpirTest_IsValidPeId() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
private:
};

TEST_F(IpirTest_IsValidPeId, CheckIsValidPeIdFunc) {
  MockProcElement *mock_pe;
  PeVec m_pes;
  CIpir *ipir;
  bool result;
  uint32_t pe_id = 0;
  // Case 1 PeVec.size == 0
  m_pes.clear();
  ipir = new CIpir(&m_pes, IPIR_STANDARD);
  result = false;
  ASSERT_EQ(ipir->IsValidPeId(pe_id), result);
  delete ipir;
  m_pes.clear();

  // Case 2 PeVec.size == 1
  uint32_t init_id2[] = { 0 };
  std::map<uint32_t, bool> id2 = { { 0, true }, { 1, false }, { 2, false } }; // {pe_id, result}
  for (auto id : init_id2) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  ipir = new CIpir(&m_pes, IPIR_STANDARD);
  for (auto id : id2) {
    ASSERT_EQ(ipir->IsValidPeId(id.first), id.second);
  }
  delete ipir;
  m_pes.clear();

  // Case 3 PeVec.size == 2
  uint32_t init_id3[] = { 0, 1 };
  std::map<uint32_t, bool> id3 = { { 0, true }, { 1, true }, { 2, false } }; // {pe_id, result}
  for (auto id : init_id3) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  ipir = new CIpir(&m_pes, IPIR_STANDARD);
  for (auto id : id3) {
    ASSERT_EQ(ipir->IsValidPeId(id.first), id.second);
  }
  delete ipir;
  m_pes.clear();

  // Case 4 PeVec.size == 3
  uint32_t init_id4[] = { 0, 1, 2 };
  std::map<uint32_t, bool> id4 = { { 0, true }, { 1, true }, { 2, true }, {3, false} }; // {pe_id, result}
  for (auto id : init_id4) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  ipir = new CIpir(&m_pes, IPIR_STANDARD);
  for (auto id : id4) {
    ASSERT_EQ(ipir->IsValidPeId(id.first), id.second);
  }
  delete ipir;
  m_pes.clear();

  // Case 5 PeVec.size == 4
  uint32_t init_id5[] = { 0, 1, 2, 3 };
  std::map<uint32_t, bool> id5 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, false}  }; // {pe_id, result}
  for (auto id : init_id5) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  ipir = new CIpir(&m_pes, IPIR_STANDARD);
  for (auto id : id5) {
    ASSERT_EQ(ipir->IsValidPeId(id.first), id.second);
  }
  delete ipir;
  m_pes.clear();

  // Case 6 PeVec.size == 5
  uint32_t init_id6[] = { 0, 1, 2, 3, 4 };
  std::map<uint32_t, bool> id6 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, true}, {5, false}  }; // {pe_id, result}
  for (auto id : init_id6) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  ipir = new CIpir(&m_pes, IPIR_STANDARD);
  for (auto id : id6) {
    ASSERT_EQ(ipir->IsValidPeId(id.first), id.second);
  }
  delete ipir;
  m_pes.clear();

  // Case 7 PeVec.size == 6
  uint32_t init_id7[] = { 0, 1, 2, 3, 4, 5 };
  std::map<uint32_t, bool> id7 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, true}, {5, true}, {6, false}  }; // {pe_id, result}
  for (auto id : init_id7) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  ipir = new CIpir(&m_pes, IPIR_STANDARD);
  for (auto id : id7) {
    ASSERT_EQ(ipir->IsValidPeId(id.first), id.second);
  }
  delete ipir;
  m_pes.clear();

  // Case 8 PeVec.size == 7
  uint32_t init_id8[] = { 0, 1, 2, 3, 4, 5, 6 };
  std::map<uint32_t, bool> id8 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, true}, {5, true}, {6, true}, {7, false}}; // {pe_id, result}
  for (auto id : init_id8) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  ipir = new CIpir(&m_pes, IPIR_STANDARD);
  for (auto id : id8) {
    ASSERT_EQ(ipir->IsValidPeId(id.first), id.second);
  }
  delete ipir;
  m_pes.clear();

  // Case 9 PeVec.size == 8
  uint32_t init_id9[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
  std::map<uint32_t, bool> id9 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, true}, {5, true}, {6, true}, {7, true}, {8, false}}; // {pe_id, result}
  for (auto id : init_id9) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  ipir = new CIpir(&m_pes, IPIR_STANDARD);
  for (auto id : id9) {
    ASSERT_EQ(ipir->IsValidPeId(id.first), id.second);
  }
  delete ipir;
  m_pes.clear();
}

// Check Intc Access
class IpirIntcRequestTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t, uint32_t>> {
protected:
  IpirIntcRequestTest() {}
  virtual ~IpirIntcRequestTest() {}
  virtual void SetUp() {
    mock_pe =
    new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    ipir = new CIpir(&m_pes, IPIR_STANDARD);
  }
  virtual void TearDown() {
    delete ipir;
    delete m_pes[pe_id];
  } // clear mock object

public:
  uint32_t pe_id = 0;
  PeVec m_pes;
  CIpir *ipir;
  MockProcElement *mock_pe;

private:
};

/*!
 * @brief Check Send Intc Request.
 */
TEST_P(IpirIntcRequestTest, CheckSendIntRequest) {
  uint32_t channel = std::get<0>(GetParam());
  uint32_t ipir_id = std::get<1>(GetParam());
  uint32_t peid_request_int = std::get<2>(GetParam());
  EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel))
      .Times(1); // It expected that the channel X is called only once.
  ipir->SendIntRequest(channel, ipir_id, peid_request_int);
}

std::tuple<uint32_t, uint32_t, uint32_t> const SIR_ParametersTable[] = {
  /* 0000 */ std::make_tuple(0, 0, 0),
  /* 0001 */ std::make_tuple(1, 0, 0),
  /* 0002 */ std::make_tuple(2, 0, 0),
  /* 0003 */ std::make_tuple(3, 0, 0),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters2, IpirIntcRequestTest,
                        ::testing::ValuesIn(SIR_ParametersTable));

class IpirIntcRequestTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t, uint32_t>> {
protected:
  IpirIntcRequestTest2() {}
  virtual ~IpirIntcRequestTest2() {}
  virtual void SetUp() {
    mock_pe =
    new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    ipir = new CIpir(&m_pes, IPIR_STANDARD);
  }
  virtual void TearDown() {
    delete ipir;
    delete m_pes[pe_id];
  } // clear mock object

public:
  uint32_t pe_id = 0;
  PeVec m_pes;
  CIpir *ipir;
  MockProcElement *mock_pe;

private:
};


TEST_P(IpirIntcRequestTest2, CheckSendIntRequestTest2) {
  uint32_t channel = std::get<0>(GetParam());
  uint32_t ipir_id = std::get<1>(GetParam());
  uint32_t peid_request_int = std::get<2>(GetParam());
  EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel))
      .Times(0); // It expected that the channel X is called only once.
  ipir->SendIntRequest(channel, ipir_id, peid_request_int);
}

std::tuple<uint32_t, uint32_t, uint32_t> const SIR_ParametersTable3[] = {
  /* 0000 */ std::make_tuple(0, 1, 0),
  /* 0001 */ std::make_tuple(1, 1, 0),
  /* 0002 */ std::make_tuple(2, 1, 0),
  /* 0003 */ std::make_tuple(3, 1, 0),
  // please write more test case.
};

INSTANTIATE_TEST_CASE_P(TestWithParameters3, IpirIntcRequestTest2,
                        ::testing::ValuesIn(SIR_ParametersTable3));


/*!
 * @brief Check Cancel Intc Request.
 * Expected to reach CancelIntByPeripheral of mock.
 */
TEST(IpirIntcRequestTest2, CheckCancelIntRequest) {
  uint32_t pe_id = 0;
  PeVec m_pes;
  CIpir *ipir;
  MockProcElement *mock_pe;
  mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
  m_pes.push_back(mock_pe);
  ipir = new CIpir(&m_pes, IPIR_STANDARD);

  EXPECT_CALL(*mock_pe, CancelIntByPeripheral(_))
      .Times(4); // It expected that the channel X is called only once.
  ipir->CancelIntRequest(0, 0, 0); // check more case
  ipir->CancelIntRequest(1, 0, 0); // check more case
  ipir->CancelIntRequest(2, 0, 0); // check more case
  ipir->CancelIntRequest(3, 0, 0); // check more case

  delete ipir;
  delete mock_pe;
}

TEST(IpirIntcRequestTest2, CheckCancelIntRequestTest2) {
  uint32_t pe_id = 0;
  PeVec m_pes;
  CIpir *ipir;
  MockProcElement *mock_pe;
  mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
  m_pes.push_back(mock_pe);
  ipir = new CIpir(&m_pes, IPIR_STANDARD);

  EXPECT_CALL(*mock_pe, CancelIntByPeripheral(_))
      .Times(0); // It expected that the channel X is called only once.
  ipir->CancelIntRequest(0, 1, 0); // check more case
  ipir->CancelIntRequest(1, 1, 0); // check more case
  ipir->CancelIntRequest(2, 1, 0); // check more case
  ipir->CancelIntRequest(3, 1, 0); // check more case

  delete ipir;
  delete mock_pe;
}



class IpirFunctionTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
    IpirFunctionTest() {}
  virtual ~IpirFunctionTest() {}
  virtual void SetUp() {
    mock_pe =
        new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    ipir = new CIpir(&m_pes, IPIR_STANDARD);

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
                                          delete mock_pe;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  PeVec m_pes;
  CIpir *ipir;
  uint32_t pe_id = 0;

private:
};

/*!
 * @brief Check IPIR Function
 */
TEST_F(IpirFunctionTest, CheckIPIFLG_functionTest1) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    uint32_t temp = 0x2;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    ByteSize size = 1;

    PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;        //IPInENm[] = 0x2
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));

    EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel)).Times(1);
    uint8_t data[] = {0xff};
    addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPInREQ1[] = 0xff
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x2U, ipiflg->GetData());
}

TEST_F(IpirFunctionTest, CheckIPIFLG_functionTest2) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    uint32_t temp = 0x2;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    ByteSize size = 1;

    PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;        //IPInENm[] = 0x2
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));

    EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel)).Times(0);
    uint8_t data[] = {0x2};
    addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPInREQ1[] = 0x2
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x0U, ipiflg->GetData());
}

TEST_F(IpirFunctionTest, CheckIPIFLG_functionTest3) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    uint32_t temp = 0xFD;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    ByteSize size = 1;

    PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;        //IPInENm[] = 0xFD
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));

    EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel)).Times(0);
    uint8_t data[] = {0x2};
    addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPInREQ1[] = 0x2
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x0U, ipiflg->GetData());
}


TEST_F(IpirFunctionTest, CheckIPIFLG_functionTest4) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    uint32_t temp = 0xff;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    ByteSize size = 1;

    PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;        //IPInENm[] = 0xff
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));

    EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel)).Times(1);
    uint8_t data[] = {0xff};
    addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPInREQ1[] = 0xff
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x2U, ipiflg->GetData());
}



/*!
 * @brief Check IPIR Function
 */
TEST_F(IpirFunctionTest, CheckIPIFCLR_functionTest1) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    uint32_t temp = 0x2;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    ByteSize size = 1;

    PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;        //IPInENm[] = 0x2
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));

    EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel)).Times(1);
    uint8_t data[] = {0xff};
    addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPInREQ1[] = 0xff
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));


    EXPECT_CALL(*mock_pe, CancelIntByPeripheral(_)).Times(1);
    data[0] = {0xff};
    addr = TRegIpir::IPIFCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIREQ* ipireq = ipir->GetIPIREQPtr(channel, 1);
    ASSERT_EQ (0xFEU, ipireq->GetData());

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x0U, ipiflg->GetData());
}

TEST_F(IpirFunctionTest, CheckIPIFCLR_functionTest2) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    uint32_t temp = 0x2;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    ByteSize size = 1;

    PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;        //IPInENm[] = 0x2
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));

    EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel)).Times(1);
    uint8_t data[] = {0xff};
    addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPInREQ1[] = 0xff
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));


    EXPECT_CALL(*mock_pe, CancelIntByPeripheral(_)).Times(1);
    data[0] = {0x2};
    addr = TRegIpir::IPIFCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIREQ* ipireq = ipir->GetIPIREQPtr(channel, 1);
    ASSERT_EQ (0xFEU, ipireq->GetData());

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x0U, ipiflg->GetData());
}

TEST_F(IpirFunctionTest, CheckIPIFCLR_functionTest3) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    uint32_t temp = 0x2;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    ByteSize size = 1;

    PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;        //IPInENm[] = 0x2
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));

    EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel)).Times(1);
    uint8_t data[] = {0xff};
    addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPInREQ1[] = 0xff
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));


    EXPECT_CALL(*mock_pe, CancelIntByPeripheral(_)).Times(0);
    data[0] = {0xFD};
    addr = TRegIpir::IPIFCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIREQ* ipireq = ipir->GetIPIREQPtr(channel, 1);
    ASSERT_EQ (0xFFU, ipireq->GetData());

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x2U, ipiflg->GetData());
}


TEST_F(IpirFunctionTest, CheckIPIREQ_function) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    ByteSize size = 1;

    uint8_t data[] = {0xff};
    PhysAddr addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPInREQ1[] = 0xff
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x0U, ipiflg->GetData());
}


TEST_F(IpirFunctionTest, CheckIPIRCLR_functionTest1) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    uint32_t temp = 0x2;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    ByteSize size = 1;

    PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;        //IPI0EN0[] = 0x2
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));

    EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel)).Times(1);
    uint8_t data[] = {0xff};
    addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPI0REQ1[] = 0xff
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIREQ * ipireq = ipir->GetIPIREQPtr(channel, 1);
    ASSERT_EQ (0xFFU, ipireq->GetData());

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x2U, ipiflg->GetData());


    EXPECT_CALL(*mock_pe, CancelIntByPeripheral(_)).Times(1);
    data[0] = {0x1};
    addr = TRegIpir::IPIRCLR_OFFSET + channel * 0x20 + 1 * 0x100;
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    ipireq = ipir->GetIPIREQPtr(channel, 1);
    ASSERT_EQ (0xFEU, ipireq->GetData());

    ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x0U, ipiflg->GetData());
}

TEST_F(IpirFunctionTest, CheckIPIRCLR_functionTest2) {
    ipir->HardReset();
    uint32_t  channel= 0;
    uint32_t  ipir_id= 0;
    uint32_t temp = 0x2;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    ByteSize size = 1;

    PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;        //IPI0EN0[] = 0x2
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, wdata, size));

    EXPECT_CALL(*mock_pe, ReqIntByPeripheral(channel)).Times(1);
    uint8_t data[] = {0xff};
    addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + 1 * 0x100;    //IPI0REQ1[] = 0xff
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    TRegIPIREQ * ipireq = ipir->GetIPIREQPtr(channel, 1);
    ASSERT_EQ (0xFFU, ipireq->GetData());

    TRegIPIFLG* ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x2U, ipiflg->GetData());


    EXPECT_CALL(*mock_pe, CancelIntByPeripheral(_)).Times(0);
    data[0] = {0xFE};
    addr = TRegIpir::IPIRCLR_OFFSET + channel * 0x20 + 1 * 0x100;
    ASSERT_NO_THROW(ipir->TargetWrite(sideband, addr, data, size));

    ipireq = ipir->GetIPIREQPtr(channel, 1);
    ASSERT_EQ (0x1U, ipireq->GetData());

    ipiflg = ipir->GetIPIFLGPtr(channel, ipir_id);
    ASSERT_EQ (0x2U, ipiflg->GetData());
}

class ConvertSelfAddr
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
  ConvertSelfAddr() {}
  virtual ~ConvertSelfAddr() {}
  virtual void SetUp() {
    mock_pe =
        new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    ipir = new CIpir(&m_pes, IPIR_STANDARD);

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
                                          delete mock_pe;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  PeVec m_pes;
  CIpir *ipir;
  uint32_t pe_id = 0;

private:
};

//n: channel m: peid
//+0x0 + 0x20*n - IPInENS
//+0x4 + 0x20*n - IPInFLGS
//+0x8 + 0x20*n - IPInFCLRS
//+0x10 + 0x20*n - IPInREQS
//+0x14 + 0x20*n - IPInRCLRS
//+0x800 + 0x20*n + 0x100m- IPInENm
//+0x804 + 0x20*n + 0x100m- IPInFLGm
//+0x808 + 0x20*n + 0x100m- IPInFCLRm
//+0x810 + 0x20*n + 0x100m- IPInREQm
//+0x814 + 0x20*n + 0x100m- IPInRCLRm


TEST_F(ConvertSelfAddr, ConvertSelfAddr_Regs) {
  ipir->HardReset();
  for (uint32_t peid = 0; peid < 8; peid++ ){
    for (uint32_t channel =0; channel <4; channel++){
      //TranslateAddr(uint32_t peid, PhysAddr addr)
      ASSERT_EQ(0x800 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x20*channel)); //IPInENS
      ASSERT_EQ(0x804 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x4+ 0x20*channel)); //IPInFLGS
      ASSERT_EQ(0x808 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x8+ 0x20*channel)); //IPInFCLRS
      ASSERT_EQ(0x810 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x10+ 0x20*channel)); //IPInREQS
      ASSERT_EQ(0x814 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x14+ 0x20*channel)); //IPInRCLRS

      ASSERT_EQ(0x800 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x800 + 0x20*channel + 0x100*peid)); //IPInENm
      ASSERT_EQ(0x804 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x804 + 0x20*channel + 0x100*peid)); //IPInFLGm
      ASSERT_EQ(0x808 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x808 + 0x20*channel + 0x100*peid)); //IPInFCLRm
      ASSERT_EQ(0x810 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x810 + 0x20*channel + 0x100*peid)); //IPInREQm
      ASSERT_EQ(0x814 + 0x20*channel + 0x100*peid, ipir->ConvertSelfAddr(peid, 0x814 + 0x20*channel + 0x100*peid)); //IPInRCLRm
    }
  }
}

// Next case, write a TEST to reach a CancelReq from TargetWrite method.

// and, Check other method,
// GetIPIREQPtr, GetIPIENPtr, GetIPIFLGPtr, Set_IPIFLG, Get_IPIFLG,
// Set_IPIREQ, Get_IPIREQ, Set_IPIEN, Get_IPIEN....
// SendIntRequest.. etc...

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
