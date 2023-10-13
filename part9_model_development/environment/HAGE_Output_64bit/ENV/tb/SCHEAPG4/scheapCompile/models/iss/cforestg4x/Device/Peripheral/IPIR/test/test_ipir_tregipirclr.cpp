/**
 * @file test_ipir_tregipirclr.cpp
 *
 * @date Created on: 2016
 * @author Author: Yvo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for TRegIPIRCLR. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <memory>
#include <map>
#include <stdexcept>
#include "treg_ipir.h"
#include "ipir.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

// ----------------------------------------
class TRegIPIRCLRTest1 : public testing::Test {
protected:
  TRegIPIRCLRTest1() {}
  virtual ~TRegIPIRCLRTest1() {}
  virtual void SetUp() {
    m_treg_ipirclr = new TRegIPIRCLR(reg_size, min_access_size, reg_addr, channel, ipir_id);
  }
  virtual void TearDown() { delete m_treg_ipirclr; }

public:
  TRegIPIRCLR *m_treg_ipirclr;

private:
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  PhysAddr reg_addr = TRegIpir::IPIRCLR_OFFSET + 0 * 0x20 + 0 * 0x100;
  uint32_t channel = 1;
  uint32_t ipir_id = 0;
};

/*!
 * @brief Check GetName method.
 */
TEST_F(TRegIPIRCLRTest1, Check_GetName) { ASSERT_STREQ("IPIRCLR", m_treg_ipirclr->GetName()); }

// ----------------------------------------
class TRegIPIRCLRTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
    TRegIPIRCLRTest2() {}
  virtual ~TRegIPIRCLRTest2() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIRCLR *m_treg_ipirclr;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIrclr register
 */
TEST_P(TRegIPIRCLRTest2, Check_GetSetData) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIRCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipirclr = new TRegIPIRCLR(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipirclr->Reset();
          m_treg_ipirclr->SetData(input_data);
          ASSERT_EQ(expect_data, m_treg_ipirclr->GetData());
          delete m_treg_ipirclr;
      }
  }
}

std::tuple<RegData, RegData> const GetSet_ParametersTable2[] = {
  /* 0000 */ std::make_tuple(0x1, 0x1),
  /* 0001 */ std::make_tuple(0x2, 0x2),
  /* 0002 */ std::make_tuple(0x3, 0x3),
  /* 0003 */ std::make_tuple(0x4, 0x4),
  /* 0004 */ std::make_tuple(0x0, 0x0),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters2, TRegIPIRCLRTest2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));

// ----------------------------------------
class TRegIPIRCLRTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
    TRegIPIRCLRTest3() {}
  virtual ~TRegIPIRCLRTest3() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIRCLR *m_treg_ipirclr;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIrcLR register
 */
TEST_P(TRegIPIRCLRTest3, Check_Reset) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIRCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipirclr = new TRegIPIRCLR(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipirclr->SetData(input_data);
          m_treg_ipirclr->Reset();
          ASSERT_EQ(expect_data, m_treg_ipirclr->GetData());
          delete m_treg_ipirclr;
      }
  }
}

std::tuple<RegData, RegData> const GetSet_ParametersTable3[] = {
  /* 0000 */ std::make_tuple(0x1, 0x0),
  /* 0001 */ std::make_tuple(0x2, 0x0),
  /* 0002 */ std::make_tuple(0x3, 0x0),
  /* 0003 */ std::make_tuple(0x4, 0x0),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters3, TRegIPIRCLRTest3,
                        ::testing::ValuesIn(GetSet_ParametersTable3));


// ----------------------------------------
class TRegIPIRCLRTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIRCLRTest4() {}
  virtual ~TRegIPIRCLRTest4() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  MockSideBand sideband;
  uint8_t data[8] = { 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f };
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIrclr register
 */
TEST_P(TRegIPIRCLRTest4, Check_ReadData) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIRCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIRCLR *m_treg_ipirclr;
            m_treg_ipirclr = new TRegIPIRCLR(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipirclr->Reset();
                  if (e) { // expect true
                      m_treg_ipirclr->Read(sideband, addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipirclr->GetData());
                      m_treg_ipirclr->ReadBody(addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipirclr->GetData());
                  }
            delete m_treg_ipirclr;
        }
   }
}


std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable4[] = {
  /* 0000 */ std::make_tuple(0x0, SIZE_8BIT, true),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters4, TRegIPIRCLRTest4,
                        ::testing::ValuesIn(GetSet_ParametersTable4));

class TRegIPIRCLRTest6
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIRCLRTest6() {}
  virtual ~TRegIPIRCLRTest6() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  MockSideBand sideband;
  uint8_t data[8] = { 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f };
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};


TEST_P(TRegIPIRCLRTest6, Check_Write) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIRCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIRCLR *m_treg_ipirclr;
            m_treg_ipirclr = new TRegIPIRCLR(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipirclr->Reset();
                  if (e) { // expect true
                      m_treg_ipirclr->WriteBody(addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipirclr->GetData());
                  } else { // expect false
                      ASSERT_THROW(m_treg_ipirclr->WriteBody(addr, (uint8_t*)&data, size), std::range_error);
                  }
            delete m_treg_ipirclr;
        }
   }
}


std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable6[] = {
  // range over
  /* 0001 */ std::make_tuple(0x0, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x0, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x0, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x0, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters6, TRegIPIRCLRTest6,
                        ::testing::ValuesIn(GetSet_ParametersTable6));

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
