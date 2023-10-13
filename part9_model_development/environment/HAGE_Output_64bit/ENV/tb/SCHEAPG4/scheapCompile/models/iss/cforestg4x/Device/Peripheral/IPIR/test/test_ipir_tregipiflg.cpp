/**
 * @file test_ipir_tregipiflg.cpp
 *
 * @date Created on: 2016
 * @author Author: Yvo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for TRegIPIFLG. (SAMPLE)
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
class TRegIPIFLGTest1 : public testing::Test {
protected:
  TRegIPIFLGTest1() {}
  virtual ~TRegIPIFLGTest1() {}
  virtual void SetUp() {
    m_treg_ipiflg = new TRegIPIFLG(reg_size, min_access_size, reg_addr, channel, ipir_id);
  }
  virtual void TearDown() { delete m_treg_ipiflg; }

public:
  TRegIPIFLG *m_treg_ipiflg;

private:
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  PhysAddr reg_addr = TRegIpir::IPIFLG_OFFSET + 0 * 0x20 + 0 * 0x100;
  uint32_t channel = 1;
  uint32_t ipir_id = 0;
};

/*!
 * @brief Check GetName method.
 */
TEST_F(TRegIPIFLGTest1, Check_GetName) { ASSERT_STREQ("IPIFLG", m_treg_ipiflg->GetName()); }

// ----------------------------------------
class TRegIPIFLGTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
    TRegIPIFLGTest2() {}
  virtual ~TRegIPIFLGTest2() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIFLG *m_treg_ipiflg;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIFLG register
 */
TEST_P(TRegIPIFLGTest2, Check_GetSetData) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIFLG_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipiflg = new TRegIPIFLG(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipiflg->Reset();
          m_treg_ipiflg->SetData(input_data);
          ASSERT_EQ(expect_data, m_treg_ipiflg->GetData());
          delete m_treg_ipiflg;
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
INSTANTIATE_TEST_CASE_P(TestWithParameters2, TRegIPIFLGTest2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));

// ----------------------------------------
class TRegIPIFLGTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
    TRegIPIFLGTest3() {}
  virtual ~TRegIPIFLGTest3() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIFLG *m_treg_ipiflg;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIFLG register
 */
TEST_P(TRegIPIFLGTest3, Check_Reset) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIFLG_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipiflg = new TRegIPIFLG(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipiflg->SetData(input_data);
          m_treg_ipiflg->Reset();
          ASSERT_EQ(expect_data, m_treg_ipiflg->GetData());
          delete m_treg_ipiflg;
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
INSTANTIATE_TEST_CASE_P(TestWithParameters3, TRegIPIFLGTest3,
                        ::testing::ValuesIn(GetSet_ParametersTable3));


// ----------------------------------------
class TRegIPIFLGTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIFLGTest4() {}
  virtual ~TRegIPIFLGTest4() {}
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
 * @brief Check TRegIPIFLG register
 */
TEST_P(TRegIPIFLGTest4, Check_Write) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIFLG_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIFLG *m_treg_ipiflg;
            m_treg_ipiflg = new TRegIPIFLG(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipiflg->Reset();
                  if (e) { // expect true
                      m_treg_ipiflg->Write(sideband, addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipiflg->GetData());
                  } else { // expect false
                      ASSERT_THROW(m_treg_ipiflg->Write(sideband, addr, (uint8_t*)&data, size), std::range_error);
                  }
            delete m_treg_ipiflg;
        }
   }
}


std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable4[] = {
  // In a range of size
  /* 0000 */ std::make_tuple(0x0, SIZE_8BIT, true),
  // range over
  /* 0001 */ std::make_tuple(0x0, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x0, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x0, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x0, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters4, TRegIPIFLGTest4,
                        ::testing::ValuesIn(GetSet_ParametersTable4));

class TRegIPIFLGTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIFLGTest5() {}
  virtual ~TRegIPIFLGTest5() {}
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


TEST_P(TRegIPIFLGTest5, Check_Read) {
    {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIFLG_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIFLG *m_treg_ipiflg;
            m_treg_ipiflg = new TRegIPIFLG(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipiflg->Reset();
            m_treg_ipiflg->Read(sideband, addr, (uint8_t*)&data, size);
            ASSERT_EQ(expect_data,*data);
            m_treg_ipiflg->ReadBody(addr, (uint8_t*)&data, size);
             ASSERT_EQ(expect_data,*data);
    }
  }
 }
}

std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable5[] = {
  // In a range of size
  /* 0000 */ std::make_tuple(0x0, SIZE_8BIT, true),
};
INSTANTIATE_TEST_CASE_P(TestWithParameters5, TRegIPIFLGTest5,
                        ::testing::ValuesIn(GetSet_ParametersTable5));

class TRegIPIFLGTest6
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIFLGTest6() {}
  virtual ~TRegIPIFLGTest6() {}
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
 * @brief Check TRegIPIFLG register
 */
TEST_P(TRegIPIFLGTest6, Check_Write) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIFLG_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIFLG *m_treg_ipiflg;
            m_treg_ipiflg = new TRegIPIFLG(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipiflg->Reset();
                  if (e) { // expect true
                      m_treg_ipiflg->WriteBody(addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipiflg->GetData());
                  } else { // expect false
                      ASSERT_THROW(m_treg_ipiflg->WriteBody(addr, (uint8_t*)&data, size), std::range_error);
                  }
            delete m_treg_ipiflg;
        }
   }
}


std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable6[] = {
  // In a range of size
  /* 0000 */ std::make_tuple(0x0, SIZE_8BIT, true),
  // range over
  /* 0001 */ std::make_tuple(0x0, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x0, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x0, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x0, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters6, TRegIPIFLGTest6,
                        ::testing::ValuesIn(GetSet_ParametersTable6));

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
