/**
 * @file test_ipir_tregipifclr.cpp
 *
 * @date Created on: 2016
 * @author Author: Yvo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for TRegIPIFCLR. (SAMPLE)
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
class TRegIPIFCLRTest1 : public testing::Test {
protected:
  TRegIPIFCLRTest1() {}
  virtual ~TRegIPIFCLRTest1() {}
  virtual void SetUp() {
    m_treg_ipifclr = new TRegIPIFCLR(reg_size, min_access_size, reg_addr, channel, ipir_id);
  }
  virtual void TearDown() { delete m_treg_ipifclr; }

public:
  TRegIPIFCLR *m_treg_ipifclr;

private:
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  PhysAddr reg_addr = TRegIpir::IPIFCLR_OFFSET + 0 * 0x20 + 0 * 0x100;
  uint32_t channel = 1;
  uint32_t ipir_id = 0;
};

/*!
 * @brief Check GetName method.
 */
TEST_F(TRegIPIFCLRTest1, Check_GetName) { ASSERT_STREQ("IPIFCLR", m_treg_ipifclr->GetName()); }

// ----------------------------------------
class TRegIPIFCLRTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
    TRegIPIFCLRTest2() {}
  virtual ~TRegIPIFCLRTest2() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIFCLR *m_treg_ipifclr;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIfclr register
 */
TEST_P(TRegIPIFCLRTest2, Check_GetSetData) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIFCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipifclr = new TRegIPIFCLR(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipifclr->Reset();
          m_treg_ipifclr->SetData(input_data);
          ASSERT_EQ(expect_data, m_treg_ipifclr->GetData());
          delete m_treg_ipifclr;
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
INSTANTIATE_TEST_CASE_P(TestWithParameters2, TRegIPIFCLRTest2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));

// ----------------------------------------
class TRegIPIFCLRTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
    TRegIPIFCLRTest3() {}
  virtual ~TRegIPIFCLRTest3() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIFCLR *m_treg_ipifclr;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIFCLR register
 */
TEST_P(TRegIPIFCLRTest3, Check_Reset) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIFCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipifclr = new TRegIPIFCLR(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipifclr->SetData(input_data);
          m_treg_ipifclr->Reset();
          ASSERT_EQ(expect_data, m_treg_ipifclr->GetData());
          delete m_treg_ipifclr;
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
INSTANTIATE_TEST_CASE_P(TestWithParameters3, TRegIPIFCLRTest3,
                        ::testing::ValuesIn(GetSet_ParametersTable3));


// ----------------------------------------
class TRegIPIFCLRTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIFCLRTest4() {}
  virtual ~TRegIPIFCLRTest4() {}
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
 * @brief Check TRegIPIfclr register
 */
TEST_P(TRegIPIFCLRTest4, Check_ReadData) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIFCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIFCLR *m_treg_ipifclr;
            m_treg_ipifclr = new TRegIPIFCLR(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipifclr->Reset();
                  if (e) { // expect true
                      m_treg_ipifclr->Read(sideband, addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipifclr->GetData());
                      m_treg_ipifclr->ReadBody(addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipifclr->GetData());
                  }
            delete m_treg_ipifclr;
        }
   }
}


std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable4[] = {
  /* 0000 */ std::make_tuple(0x0, SIZE_8BIT, true),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters4, TRegIPIFCLRTest4,
                        ::testing::ValuesIn(GetSet_ParametersTable4));

class TRegIPIFCLRTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIFCLRTest5() {}
  virtual ~TRegIPIFCLRTest5() {}
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
 * @brief Check TRegIPIFCLR register
 */
TEST_P(TRegIPIFCLRTest5, Check_Write) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIFCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIFCLR *m_treg_ipifclr;
            m_treg_ipifclr = new TRegIPIFCLR(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipifclr->Reset();
                  if (e) { // expect true
                      m_treg_ipifclr->Write(sideband, addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipifclr->GetData());
                  } else { // expect false
                      ASSERT_THROW(m_treg_ipifclr->Write(sideband, addr, (uint8_t*)&data, size), std::range_error);
                  }
            delete m_treg_ipifclr;
        }
   }
}


std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable5[] = {
  // range over
  /* 0000 */ std::make_tuple(0x0, SIZE_16BIT, false),
  /* 0001 */ std::make_tuple(0x0, SIZE_32BIT, false),
  /* 0002 */ std::make_tuple(0x0, SIZE_64BIT, false),
  /* 0003 */ std::make_tuple(0x0, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters5, TRegIPIFCLRTest5,
                        ::testing::ValuesIn(GetSet_ParametersTable5));

class TRegIPIFCLRTest6
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIFCLRTest6() {}
  virtual ~TRegIPIFCLRTest6() {}
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
 * @brief Check TRegIPIFCLR register
 */
TEST_P(TRegIPIFCLRTest6, Check_Write) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIFCLR_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIFCLR *m_treg_ipifclr;
            m_treg_ipifclr = new TRegIPIFCLR(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipifclr->Reset();
                  if (e) { // expect true
                      m_treg_ipifclr->WriteBody(addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipifclr->GetData());
                  } else { // expect false
                      ASSERT_THROW(m_treg_ipifclr->WriteBody(addr, (uint8_t*)&data, size), std::range_error);
                  }
            delete m_treg_ipifclr;
        }
   }
}


std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable6[] = {
  // range over
  /* 0000 */ std::make_tuple(0x0, SIZE_16BIT, false),
  /* 0001 */ std::make_tuple(0x0, SIZE_32BIT, false),
  /* 0002 */ std::make_tuple(0x0, SIZE_64BIT, false),
  /* 0003 */ std::make_tuple(0x0, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters6, TRegIPIFCLRTest6,
                        ::testing::ValuesIn(GetSet_ParametersTable6));

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
