/**
 * @file test_bar_tregbren.cpp
 *
 * @date Created on: 2016
 * @author Author: YVo <y.vo.wg@renesas.com>
 * @brief UNIT TEST for TRegBREN. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "bar.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

// using ::testing::AtLeast;
// using ::testing::_;
// using ::testing::Return;

// ----------------------------------------
class TRegBRENTest1 : public testing::Test {
protected:
    TRegBRENTest1() {}
  virtual ~TRegBRENTest1() {}
  virtual void SetUp() {
    m_treg_bren = new TRegBREN(reg_size, min_access_size, reg_addr, bar_id);
  }
  virtual void TearDown() { delete m_treg_bren; }

public:
  TRegBREN *m_treg_bren;

private:
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  PhysAddr reg_addr = TRegBar::BREN_OFFSET + 0 * 0x10;
  uint32_t bar_id = 0;
};

/*!
 * @brief Check GetName method.
 */
TEST_F(TRegBRENTest1, Check_GetName) { ASSERT_STREQ("BREN", m_treg_bren->GetName()); }

// ----------------------------------------
class TRegBRENTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData>> {
protected:
    TRegBRENTest2() {}
  virtual ~TRegBRENTest2() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegBREN *m_treg_bren;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIEN register
 */
TEST_P(TRegBRENTest2, Check_GetSetData) {
  RegData input_data = std::get<0>(GetParam());
  PhysAddr reg_addr;

  for (uint32_t channel =0; channel <16; channel++){
      reg_addr = TRegBar::BREN_OFFSET + channel * 0x10;
      m_treg_bren = new TRegBREN(reg_size, min_access_size, reg_addr, channel);
      m_treg_bren->SetData(input_data);
      ASSERT_EQ((input_data), m_treg_bren->GetData());
      delete m_treg_bren;
  }
}

std::tuple<RegData> const GetSet_ParametersTable2[] = {
  /* 0000 */ std::make_tuple(0x1),
  /* 0001 */ std::make_tuple(0x2),
  /* 0002 */ std::make_tuple(0x3),
  /* 0003 */ std::make_tuple(0x4),
  /* 0004 */ std::make_tuple(0x1A),
  /* 0005 */ std::make_tuple(0x2B),
  /* 0006 */ std::make_tuple(0x3C),
  /* 0007 */ std::make_tuple(0x4D),
  /* 0008 */ std::make_tuple(0xFF),
  /* 0009 */ std::make_tuple(0x0),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters2, TRegBRENTest2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));

/// ----------------------------------------
class TRegBRENTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData>> {
protected:
    TRegBRENTest3() {}
  virtual ~TRegBRENTest3() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegBREN *m_treg_bren;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIEN register
 */
TEST_P(TRegBRENTest3, Check_Reset) {
  RegData input_data = std::get<0>(GetParam());
  PhysAddr reg_addr;

  for (uint32_t channel =0; channel <16; channel++){
      reg_addr = TRegBar::BREN_OFFSET + channel * 0x10;
      m_treg_bren = new TRegBREN(reg_size, min_access_size, reg_addr, channel);
      m_treg_bren->SetData(input_data);
      m_treg_bren->Reset();
      ASSERT_EQ(0x0U, m_treg_bren->GetData());
      delete m_treg_bren;
  }
}

std::tuple<RegData> const GetSet_ParametersTable3[] = {
  /* 0000 */ std::make_tuple(0x1),
  /* 0001 */ std::make_tuple(0x2),
  /* 0002 */ std::make_tuple(0x3),
  /* 0003 */ std::make_tuple(0x4),
  /* 0004 */ std::make_tuple(0x1A),
  /* 0005 */ std::make_tuple(0x2B),
  /* 0006 */ std::make_tuple(0x3C),
  /* 0007 */ std::make_tuple(0x4D),
  /* 0008 */ std::make_tuple(0xFF),
  /* 0009 */ std::make_tuple(0x0),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters3, TRegBRENTest3,
                        ::testing::ValuesIn(GetSet_ParametersTable3));

// ----------------------------------------
class TRegBRENTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>> {
protected:
    TRegBRENTest4() {}
  virtual ~TRegBRENTest4() {}
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
TEST_P(TRegBRENTest4, Check_Write) {
  {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t channel =0; channel <16; channel++){
        PhysAddr addr = TRegBar::BREN_OFFSET + channel * 0x10;
        TRegBREN *m_treg_bren;
        m_treg_bren = new TRegBREN(reg_size, min_access_size, addr, channel);
        m_treg_bren->Reset();
              if (e) { // expect true
                  m_treg_bren->Write(sideband, addr, (uint8_t*)&data, size);
                  ASSERT_EQ(expect_data,   m_treg_bren->GetData());
              } else { // expect false
                  ASSERT_THROW(m_treg_bren->Write(sideband, addr, (uint8_t*)&data, size), std::range_error);
              }
        delete m_treg_bren;
    }
  }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable4[] = {
  // range over
  /* 0001 */ std::make_tuple(0x2f1f, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x3f2f1f, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x4f3f2f1f, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x1f, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters4, TRegBRENTest4,
                        ::testing::ValuesIn(GetSet_ParametersTable4));

// ----------------------------------------
class TRegBRENTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>> {
protected:
    TRegBRENTest5() {}
  virtual ~TRegBRENTest5() {}
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
TEST_P(TRegBRENTest5, Check_WriteBody) {
  {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t channel =0; channel <16; channel++){
        PhysAddr addr = TRegBar::BREN_OFFSET + channel * 0x10;
        TRegBREN *m_treg_bren;
        m_treg_bren = new TRegBREN(reg_size, min_access_size, addr, channel);
        m_treg_bren->Reset();
              if (e) { // expect true
                  m_treg_bren->WriteBody(addr, (uint8_t*)&data, size);
                  ASSERT_EQ(expect_data,   m_treg_bren->GetData());
              } else { // expect false
                  ASSERT_THROW(m_treg_bren->WriteBody(addr, (uint8_t*)&data, size), std::range_error);
              }
        delete m_treg_bren;
    }
  }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable5[] = {
  // range over
  /* 0001 */ std::make_tuple(0x2f1f, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x3f2f1f, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x4f3f2f1f, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x1f, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters5, TRegBRENTest5,
                        ::testing::ValuesIn(GetSet_ParametersTable5));

// ----------------------------------------
// ----------------------------------------
class TRegBRENTest6
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>> {
protected:
    TRegBRENTest6() {}
  virtual ~TRegBRENTest6() {}
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
TEST_P(TRegBRENTest6, Check_Read) {
  {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t channel =0; channel <16; channel++){
        PhysAddr addr = TRegBar::BREN_OFFSET + channel * 0x10;
        TRegBREN *m_treg_bren;
        m_treg_bren = new TRegBREN(reg_size, min_access_size, addr, channel);
        m_treg_bren->Reset();
        if (e) { // expect true
            m_treg_bren->Read(sideband,addr, (uint8_t*)&data, size);
              ASSERT_EQ(expect_data,   m_treg_bren->GetData());
            m_treg_bren->ReadBody(addr, (uint8_t*)&data, size);
            ASSERT_EQ(expect_data,   m_treg_bren->GetData());
        }
        delete m_treg_bren;
    }
  }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable6[] = {
  /* 0001 */ std::make_tuple(0x0, SIZE_8BIT, true),
  // range over
  /* 0001 */ std::make_tuple(0x2f1f, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x3f2f1f, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x4f3f2f1f, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x1f, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters6, TRegBRENTest6,
                        ::testing::ValuesIn(GetSet_ParametersTable6));



int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
