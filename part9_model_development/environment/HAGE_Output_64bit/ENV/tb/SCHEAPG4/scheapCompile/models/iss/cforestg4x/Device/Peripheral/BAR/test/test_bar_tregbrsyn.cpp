/**
 * @file test_bar_tregbrsyn.cpp
 *
 * @date Created on: 2016
 * @author Author: YVo <y.vo.wg@renesas.com>
 * @brief UNIT TEST for TRegBRSYN. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "bar.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"
#include "mock_fsbus.h"

// using ::testing::AtLeast;
// using ::testing::_;
// using ::testing::Return;

// ----------------------------------------
class TRegBRSYNTest1 : public testing::Test {
protected:
    TRegBRSYNTest1() {}
  virtual ~TRegBRSYNTest1() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    m_treg_brsyn = new TRegBRSYN(reg_size, min_access_size, reg_addr,channel, bar_id,mock_fsbus);
  }
  virtual void TearDown() { delete m_treg_brsyn;
                            delete mock_fsbus;}

public:
  TRegBRSYN *m_treg_brsyn;

private:
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  uint32_t bar_id = 0;
  uint32_t channel = 0;
  PhysAddr reg_addr = TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100;
  MockFsBus* mock_fsbus;
};

/*!
 * @brief Check GetName method.
 */
TEST_F(TRegBRSYNTest1, Check_GetName) { ASSERT_STREQ("BRSYN", m_treg_brsyn->GetName()); }

// ----------------------------------------
class TRegBRSYNTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData>> {
protected:
    TRegBRSYNTest2() {}
  virtual ~TRegBRSYNTest2() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegBRSYN *m_treg_brsyn;
  MockFsBus* mock_fsbus;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIEN register
 */
TEST_P(TRegBRSYNTest2, Check_GetSetData) {
  RegData input_data = std::get<0>(GetParam());
  PhysAddr reg_addr;

  for (uint32_t channel =0; channel <16; channel++){
      for (uint32_t bar_id =0; bar_id <8; bar_id++){
          mock_fsbus = new MockFsBus ();
          reg_addr = TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100;
          m_treg_brsyn = new TRegBRSYN(reg_size, min_access_size, reg_addr,channel, bar_id,mock_fsbus);
          m_treg_brsyn->SetData(input_data);
          ASSERT_EQ((input_data & 0x1), m_treg_brsyn->GetData());
          delete m_treg_brsyn;
          delete mock_fsbus;
      }
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
INSTANTIATE_TEST_CASE_P(TestWithParameters2, TRegBRSYNTest2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));

/// ----------------------------------------
class TRegBRSYNTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData>> {
protected:
    TRegBRSYNTest3() {}
  virtual ~TRegBRSYNTest3() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegBRSYN *m_treg_brsyn;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  MockFsBus* mock_fsbus;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIEN register
 */
TEST_P(TRegBRSYNTest3, Check_Reset) {
  RegData input_data = std::get<0>(GetParam());
  PhysAddr reg_addr;

  for (uint32_t channel =0; channel <16; channel++){
      for (uint32_t bar_id =0; bar_id <8; bar_id++){
          mock_fsbus = new MockFsBus ();
          reg_addr = TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100;
          m_treg_brsyn = new TRegBRSYN(reg_size, min_access_size, reg_addr,channel, bar_id,mock_fsbus);
          m_treg_brsyn->SetData(input_data);
          m_treg_brsyn->Reset();
          ASSERT_EQ(0x0U, m_treg_brsyn->GetData());
          delete m_treg_brsyn;
          delete mock_fsbus;
      }
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
INSTANTIATE_TEST_CASE_P(TestWithParameters3, TRegBRSYNTest3,
                        ::testing::ValuesIn(GetSet_ParametersTable3));

// ----------------------------------------
class TRegBRSYNTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>> {
protected:
    TRegBRSYNTest4() {}
  virtual ~TRegBRSYNTest4() {}
  virtual void SetUp() {
  }
  virtual void TearDown() {}

public:
  MockSideBand sideband;
  uint8_t data[8] = { 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f };
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  MockFsBus* mock_fsbus;

private:
};

/*!
 * @brief Check Write operation.
 */
TEST_P(TRegBRSYNTest4, Check_Write) {
  {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t channel =0; channel <16; channel++){
        for (uint32_t bar_id =0; bar_id <8; bar_id++){
            mock_fsbus = new MockFsBus ();
            PhysAddr addr = TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100;
            TRegBRSYN *m_treg_brsyn;
            m_treg_brsyn = new TRegBRSYN(reg_size, min_access_size, addr,channel, bar_id,mock_fsbus);
            m_treg_brsyn->Reset();
              if (e) { // expect true
                  m_treg_brsyn->Write(sideband, addr, (uint8_t*)&data, size);
                  ASSERT_EQ(expect_data,   m_treg_brsyn->GetData());
              } else { // expect false
                  ASSERT_THROW(m_treg_brsyn->Write(sideband, addr, (uint8_t*)&data, size), std::range_error);
              }
              delete m_treg_brsyn;
              delete mock_fsbus;
        }
    }
  }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable4[] = {
  /* 0000 */ std::make_tuple(0x1, SIZE_8BIT, true),
  // range over
  /* 0001 */ std::make_tuple(0x2f1f, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x3f2f1f, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x4f3f2f1f, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x1f, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters4, TRegBRSYNTest4,
                        ::testing::ValuesIn(GetSet_ParametersTable4));


// ----------------------------------------
class TRegBRSYNTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>> {
protected:
    TRegBRSYNTest5() {}
  virtual ~TRegBRSYNTest5() {}
  virtual void SetUp() {
  }
  virtual void TearDown() {}

public:
  MockSideBand sideband;
  uint8_t data[8] = { 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f };
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  MockFsBus* mock_fsbus;

private:
};

/*!
 * @brief Check Write operation.
 */
TEST_P(TRegBRSYNTest5, Check_WriteBody) {
  {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t channel =0; channel <16; channel++){
          for (uint32_t bar_id =0; bar_id <8; bar_id++){
              mock_fsbus = new MockFsBus ();
              PhysAddr addr = TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100;
              TRegBRSYN *m_treg_brsyn;
              m_treg_brsyn = new TRegBRSYN(reg_size, min_access_size, addr,channel, bar_id,mock_fsbus);
              m_treg_brsyn->Reset();
              if (e) { // expect true
                  m_treg_brsyn->WriteBody(addr, (uint8_t*)&data, size);
                  ASSERT_EQ(expect_data,   m_treg_brsyn->GetData());
              } else { // expect false
                  ASSERT_THROW(m_treg_brsyn->WriteBody(addr, (uint8_t*)&data, size), std::range_error);
              }
              delete m_treg_brsyn;
              delete mock_fsbus;
          }
    }
  }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable5[] = {
  /* 0000 */ std::make_tuple(0x1, SIZE_8BIT, true),
  // range over
  /* 0001 */ std::make_tuple(0x2f1f, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x3f2f1f, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x4f3f2f1f, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x1f, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters5, TRegBRSYNTest5,
                        ::testing::ValuesIn(GetSet_ParametersTable5));

// ----------------------------------------
// ----------------------------------------
class TRegBRSYNTest6
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>> {
protected:
    TRegBRSYNTest6() {}
  virtual ~TRegBRSYNTest6() {}
  virtual void SetUp() {
  }
  virtual void TearDown() {}

public:
  MockSideBand sideband;
  uint8_t data[8] = { 0x1f, 0x2f, 0x3f, 0x4f, 0x5f, 0x6f, 0x7f, 0x8f };
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  MockFsBus* mock_fsbus;

private:
};

/*!
 * @brief Check Write operation.
 */
TEST_P(TRegBRSYNTest6, Check_Read) {
  {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t channel =0; channel <16; channel++){
        for (uint32_t bar_id =0; bar_id <8; bar_id++){
            mock_fsbus = new MockFsBus ();
            PhysAddr addr = TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100;
            TRegBRSYN *m_treg_brsyn;
            m_treg_brsyn = new TRegBRSYN(reg_size, min_access_size, addr,channel, bar_id,mock_fsbus);
            m_treg_brsyn->Reset();
            if (e) { // expect true
                m_treg_brsyn->Read(sideband,addr, (uint8_t*)&data, size);
                ASSERT_EQ(expect_data,   m_treg_brsyn->GetData());
                m_treg_brsyn->ReadBody(addr, (uint8_t*)&data, size);
                ASSERT_EQ(expect_data,   m_treg_brsyn->GetData());
            }
            delete m_treg_brsyn;
          delete mock_fsbus;
        }
    }
  }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable6[] = {
  // range over
  /* 0001 */ std::make_tuple(0x2f1f, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x3f2f1f, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x4f3f2f1f, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x1f, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters6, TRegBRSYNTest6,
                        ::testing::ValuesIn(GetSet_ParametersTable6));


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
