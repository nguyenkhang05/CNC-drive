/**
 * @file test_ipir_tregipien.cpp
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for TRegIPIENT. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "treg_ipir.h"
#include "ipir.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

// using ::testing::AtLeast;
// using ::testing::_;
// using ::testing::Return;

// ----------------------------------------
class TRegIPIENTest1 : public testing::Test {
protected:
  TRegIPIENTest1() {}
  virtual ~TRegIPIENTest1() {}
  virtual void SetUp() {
    m_treg_ipien = new TRegIPIEN(reg_size, min_access_size, reg_addr, channel, ipir_id);
  }
  virtual void TearDown() { delete m_treg_ipien; }

public:
  TRegIPIEN *m_treg_ipien;

private:
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  PhysAddr reg_addr = TRegIpir::IPIEN_OFFSET + 0 * 0x20 + 0 * 0x100;
  uint32_t channel = 1;
  uint32_t ipir_id = 0;
};

/*!
 * @brief Check GetName method.
 */
TEST_F(TRegIPIENTest1, Check_GetName) { ASSERT_STREQ("IPIEN", m_treg_ipien->GetName()); }

// ----------------------------------------
class TRegIPIENTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
  TRegIPIENTest2() {}
  virtual ~TRegIPIENTest2() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIEN *m_treg_ipien;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIEN register
 */
TEST_P(TRegIPIENTest2, Check_GetSetData) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipien = new TRegIPIEN(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipien->SetData(input_data);
          ASSERT_EQ(expect_data, m_treg_ipien->GetData());
          delete m_treg_ipien;
      }
  }
}

std::tuple<RegData, RegData> const GetSet_ParametersTable2[] = {
  /* 0000 */ std::make_tuple(0x1, 0x1),
  /* 0001 */ std::make_tuple(0x2, 0x2),
  /* 0002 */ std::make_tuple(0x3, 0x3),
  /* 0003 */ std::make_tuple(0x4, 0x4),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters2, TRegIPIENTest2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));

// ----------------------------------------
class TRegIPIENTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
  TRegIPIENTest3() {}
  virtual ~TRegIPIENTest3() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIEN *m_treg_ipien;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check Reset operation.
 */
TEST_P(TRegIPIENTest3, Check_Reset) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipien = new TRegIPIEN(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipien->SetData(input_data);
          m_treg_ipien->Reset();
          ASSERT_NE(input_data, m_treg_ipien->GetData());
          ASSERT_EQ(expect_data, m_treg_ipien->GetData());
          delete m_treg_ipien;
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
INSTANTIATE_TEST_CASE_P(TestWithParameters3, TRegIPIENTest3,
                        ::testing::ValuesIn(GetSet_ParametersTable3));

// ----------------------------------------
class TRegIPIENTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>> {
protected:
  TRegIPIENTest4() {}
  virtual ~TRegIPIENTest4() {}
  virtual void SetUp() {
  }
  virtual void TearDown() {}

public:
  MockSideBand sideband;
  uint8_t data[8] = { 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f };
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
};

/*!
 * @brief Check Write operation.
 */
TEST_P(TRegIPIENTest4, Check_Write) {
  // check following:
  //   if (IsRegWriteEnable (sideband) == false) {
  //     return SYSERR_NONE;
  // }
  {
    // Unreachable code ?: return SYSERR_NONE;
    // I don't know how to write test code.
  }

  // if (IsRegAccessSizeValid (size) == false) {
  //     PrintAccessSizeWarning ();
  //     return SYSERR_NONE;
  // }
  {

    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIEN *m_treg_ipien;
            m_treg_ipien = new TRegIPIEN(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipien->Reset();
                  if (e) { // expect true
                      m_treg_ipien->Write(sideband, addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipien->GetData());
                  } else { // expect false
                      ASSERT_THROW(m_treg_ipien->Write(sideband, addr, (uint8_t*)&data, size), std::range_error);
                  }
            delete m_treg_ipien;
        }
    }
  }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable4[] = {
  // In a range of size
  /* 0000 */ std::make_tuple(0x1f, SIZE_8BIT, true),
  // range over
  /* 0001 */ std::make_tuple(0x2f1f, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x3f2f1f, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x4f3f2f1f, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x1f, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters4, TRegIPIENTest4,
                        ::testing::ValuesIn(GetSet_ParametersTable4));

class TRegIPIENTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>> {
protected:
  TRegIPIENTest5() {}
  virtual ~TRegIPIENTest5() {}
  virtual void SetUp() {
  }
  virtual void TearDown() {}

public:
  MockSideBand sideband;
  uint8_t data[8] = { 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f };
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
};
TEST_P(TRegIPIENTest5, Check_Read) {
    {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIEN *m_treg_ipien;
            m_treg_ipien = new TRegIPIEN(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipien->Reset();
            m_treg_ipien->Read(sideband, addr, (uint8_t*)&data, size);
            ASSERT_EQ(expect_data,*data);

            m_treg_ipien->ReadBody(addr, (uint8_t*)&data, size);
            ASSERT_EQ(expect_data,*data);
    }
  }
 }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable5[] = {
  // In a range of size
  /* 0000 */ std::make_tuple(0x0, SIZE_8BIT, true),
};
INSTANTIATE_TEST_CASE_P(TestWithParameters5, TRegIPIENTest5,
                        ::testing::ValuesIn(GetSet_ParametersTable5));


class TRegIPIENTest6
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>> {
protected:
  TRegIPIENTest6() {}
  virtual ~TRegIPIENTest6() {}
  virtual void SetUp() {
  }
  virtual void TearDown() {}

public:
  MockSideBand sideband;
  uint8_t data[8] = { 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f };
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
};

/*!
 * @brief Check Write operation.
 */
TEST_P(TRegIPIENTest6, Check_Write) {
  // check following:
  //   if (IsRegWriteEnable (sideband) == false) {
  //     return SYSERR_NONE;
  // }
  {
    // Unreachable code ?: return SYSERR_NONE;
    // I don't know how to write test code.
  }

  // if (IsRegAccessSizeValid (size) == false) {
  //     PrintAccessSizeWarning ();
  //     return SYSERR_NONE;
  // }
  {

    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIEN *m_treg_ipien;
            m_treg_ipien = new TRegIPIEN(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipien->Reset();
                  if (e) { // expect true
                      m_treg_ipien->WriteBody(addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipien->GetData());
                  } else { // expect false
                      ASSERT_THROW(m_treg_ipien->WriteBody(addr, (uint8_t*)&data, size), std::range_error);
                  }
            delete m_treg_ipien;
        }
    }
  }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable6[] = {
  // In a range of size
  /* 0000 */ std::make_tuple(0x1f, SIZE_8BIT, true),
  // range over
  /* 0001 */ std::make_tuple(0x2f1f, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x3f2f1f, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x4f3f2f1f, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x1f, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters6, TRegIPIENTest6,
                        ::testing::ValuesIn(GetSet_ParametersTable6));

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
