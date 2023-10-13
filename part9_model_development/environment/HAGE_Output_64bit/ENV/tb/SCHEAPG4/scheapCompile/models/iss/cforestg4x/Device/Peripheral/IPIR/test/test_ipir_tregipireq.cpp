/**
 * @file test_ipir_tregipireq.cpp
 *
 * @date Created on: 2016
 * @author Author: Yvo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for TRegIPIreq. (SAMPLE)
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
class TRegIPIREQTest1 : public testing::Test {
protected:
  TRegIPIREQTest1() {}
  virtual ~TRegIPIREQTest1() {}
  virtual void SetUp() {
    m_treg_ipireq = new TRegIPIREQ(reg_size, min_access_size, reg_addr, channel, ipir_id);
  }
  virtual void TearDown() { delete m_treg_ipireq; }

public:
  TRegIPIREQ *m_treg_ipireq;

private:
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;
  PhysAddr reg_addr = TRegIpir::IPIREQ_OFFSET + 0 * 0x20 + 0 * 0x100;
  uint32_t channel = 1;
  uint32_t ipir_id = 0;
};

/*!
 * @brief Check GetName method.
 */
TEST_F(TRegIPIREQTest1, Check_GetName) { ASSERT_STREQ("IPIREQ", m_treg_ipireq->GetName()); }

// ----------------------------------------
class TRegIPIREQTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
    TRegIPIREQTest2() {}
  virtual ~TRegIPIREQTest2() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIREQ *m_treg_ipireq;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIreq register
 */
TEST_P(TRegIPIREQTest2, Check_GetSetData) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipireq = new TRegIPIREQ(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipireq->Reset();
          m_treg_ipireq->SetData(input_data);
          ASSERT_EQ(expect_data, m_treg_ipireq->GetData());
          delete m_treg_ipireq;
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
INSTANTIATE_TEST_CASE_P(TestWithParameters2, TRegIPIREQTest2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));

// ----------------------------------------
class TRegIPIREQTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<RegData, RegData>> {
protected:
    TRegIPIREQTest3() {}
  virtual ~TRegIPIREQTest3() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
  TRegIPIREQ *m_treg_ipireq;
  uint32_t reg_size = SIZE_8BIT;
  uint32_t min_access_size = SIZE_8BIT;

private:
  // nothing
};

/*!
 * @brief Check TRegIPIreq register
 */
TEST_P(TRegIPIREQTest3, Check_Reset) {
  RegData input_data = std::get<0>(GetParam());
  RegData expect_data = std::get<1>(GetParam());

  for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
      for (uint32_t channel =0; channel <4; channel++){
          PhysAddr reg_addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + ipir_id * 0x100;

          m_treg_ipireq = new TRegIPIREQ(reg_size, min_access_size, reg_addr, channel, ipir_id);
          m_treg_ipireq->SetData(input_data);
          m_treg_ipireq->Reset();
          ASSERT_EQ(expect_data, m_treg_ipireq->GetData());
          delete m_treg_ipireq;
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
INSTANTIATE_TEST_CASE_P(TestWithParameters3, TRegIPIREQTest3,
                        ::testing::ValuesIn(GetSet_ParametersTable3));


// ----------------------------------------
class TRegIPIREQTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIREQTest4() {}
  virtual ~TRegIPIREQTest4() {}
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
 * @brief Check TRegIPIreq register
 */
TEST_P(TRegIPIREQTest4, Check_ReadData) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIREQ *m_treg_ipireq;
            m_treg_ipireq = new TRegIPIREQ(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipireq->Reset();

                  if (e) { // expect true
                      m_treg_ipireq->Read(sideband, addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipireq->GetData());
                      m_treg_ipireq->ReadBody(addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipireq->GetData());
                  }
            delete m_treg_ipireq;
        }
   }
}


std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable4[] = {
 /* 0000 */ std::make_tuple(0x0, SIZE_8BIT, true),
 // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters4, TRegIPIREQTest4,
                        ::testing::ValuesIn(GetSet_ParametersTable4));

class TRegIPIREQTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIREQTest5() {}
  virtual ~TRegIPIREQTest5() {}
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

TEST_P(TRegIPIREQTest5, Check_Write) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIREQ *m_treg_ipireq;
            m_treg_ipireq = new TRegIPIREQ(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipireq->Reset();
                  if (e) { // expect true
                      m_treg_ipireq->Write(sideband, addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipireq->GetData());
                  } else { // expect false
                      ASSERT_THROW(m_treg_ipireq->Write(sideband, addr, (uint8_t*)&data, size), std::range_error);
                  }
            delete m_treg_ipireq;
        }
   }
}


std::tuple<uint8_t, ByteSize, bool> const GetSet_ParametersTable5[] = {
  // range over
  /* 0001 */ std::make_tuple(0x0, SIZE_16BIT, false),
  /* 0002 */ std::make_tuple(0x0, SIZE_32BIT, false),
  /* 0003 */ std::make_tuple(0x0, SIZE_64BIT, false),
  /* 0004 */ std::make_tuple(0x0, SIZE_128BIT, false),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters5, TRegIPIREQTest5,
                        ::testing::ValuesIn(GetSet_ParametersTable5));

class TRegIPIREQTest6
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint8_t, ByteSize, bool>>  {
protected:
    TRegIPIREQTest6() {}
  virtual ~TRegIPIREQTest6() {}
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

TEST_P(TRegIPIREQTest6, Check_Write) {
    uint8_t  expect_data   = std::get<0>(GetParam());
    ByteSize size          = std::get<1>(GetParam());
    bool     e             = std::get<2>(GetParam());

    for (uint32_t ipir_id = 0; ipir_id < 8; ipir_id++){
        for (uint32_t channel =0; channel <4; channel++){
            PhysAddr addr = TRegIpir::IPIREQ_OFFSET + channel * 0x20 + ipir_id * 0x100;
            TRegIPIREQ *m_treg_ipireq;
            m_treg_ipireq = new TRegIPIREQ(reg_size, min_access_size, addr, channel, ipir_id);
            m_treg_ipireq->Reset();
                  if (e) { // expect true
                      m_treg_ipireq->WriteBody(addr, (uint8_t*)&data, size);
                      ASSERT_EQ(expect_data,   m_treg_ipireq->GetData());
                  } else { // expect false
                      ASSERT_THROW(m_treg_ipireq->WriteBody(addr, (uint8_t*)&data, size), std::range_error);
                  }
            delete m_treg_ipireq;
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
INSTANTIATE_TEST_CASE_P(TestWithParameters6, TRegIPIREQTest6,
                        ::testing::ValuesIn(GetSet_ParametersTable6));



int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
