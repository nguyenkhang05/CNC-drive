/**
 * @file test_intc2_tregeeic_intc2.cpp
 *
 * @date Created on: Dec 23, 2017
 * @author Author: Y Vo <y.vo.wg@renesas.com>
 * @brief UNIT TEST for TRegEEIC.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "mock_intc1.h"
#include "intc2.h"
#include "forest_common.h"
#include "mock_proc_element.h"
#include "mock_sideband.h"
#include "mock_fsbus.h"
#include "gmock/gmock.h"

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;

class TRegEEICAccessPermission
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
	TRegEEICAccessPermission() {}
  virtual ~TRegEEICAccessPermission() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);
    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));

  }
  virtual void TearDown() {
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
  }

public:
  SideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  TRegEEIC* eeic;
  uint32_t pe_id = 0;
  PhysAddr eeic_base_addr = 0xFFF80000UL + 0x4000;
  uint32_t eeic_num = 2048;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_F(TRegEEICAccessPermission, IsRegWriteEnable) {
    uint32_t ch = 32;
    cintc2->HardReset();
    eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_base_addr + ch*4, ch, pe_id, cintc2);
    eeic->Reset();

    sideband.SetGM(0);
    sideband.SetUM(0);
    ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
    ASSERT_EQ(true, eeic->IsRegWriteEnable(sideband));

    sideband.SetGM(1);
    sideband.SetUM(0);
    sideband.SetGPID(0);
    ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
    ASSERT_EQ(true, eeic->IsRegWriteEnable(sideband));

    sideband.SetGM(1);
    sideband.SetUM(0);
    sideband.SetGPID(1);
    ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
    ASSERT_EQ(true, eeic->IsRegWriteEnable(sideband));

    sideband.SetGM(0);
    sideband.SetUM(1);
    ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
    ASSERT_EQ(true, eeic->IsRegWriteEnable(sideband));

    delete eeic;
}

TEST_F(TRegEEICAccessPermission, IsRegReadEnable) {
    uint32_t ch = 32;
    cintc2->HardReset();
    eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_base_addr + ch*4, ch, pe_id, cintc2);
    eeic->Reset();

    sideband.SetGM(0);
    sideband.SetUM(0);
    ASSERT_NO_THROW(eeic->IsRegReadEnable(sideband));
    ASSERT_EQ(true, eeic->IsRegReadEnable(sideband));

    sideband.SetGM(1);
    sideband.SetUM(0);
    sideband.SetGPID(0);
    ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
    ASSERT_EQ(true, eeic->IsRegWriteEnable(sideband));

    sideband.SetGM(1);
    sideband.SetUM(0);
    sideband.SetGPID(1);
    ASSERT_NO_THROW(eeic->IsRegReadEnable(sideband));
    ASSERT_EQ(true, eeic->IsRegReadEnable(sideband));

    sideband.SetGM(0);
    sideband.SetUM(1);
    ASSERT_NO_THROW(eeic->IsRegReadEnable(sideband));
    ASSERT_EQ(true, eeic->IsRegReadEnable(sideband));

    delete eeic;
}

class TRegEEICAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TRegEEICAccessTest() {}
  virtual ~TRegEEICAccessTest() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);

    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
  }

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  TRegEEIC* eeic;
  uint32_t pe_id = 0;
  PhysAddr eeic_base_addr = 0xFFF80000UL +0x4000 ;
  uint32_t eeic_num = 2048;
  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

/*!
 * @brief Check TRegEEIC Reset operation.
 */
TEST_F(TRegEEICAccessTest, CheckEIC_Reset) {
    cintc2->HardReset();
    SCOPED_TRACE("CheckEEIC_Reset");
    for (uint32_t ch = 32; ch < eeic_num; ch++) {
        eeic = new TRegEEIC (reg_size, min_access_size, eeic_base_addr+ch*4, ch, pe_id, cintc2);
        eeic->Reset();

        ASSERT_NO_THROW(eeic->Read(sideband, eeic_base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));
        uint32_t expect_data = 0x0080000F;
        memcpy (&m_data32, data, sizeof(m_data32));
        ASSERT_EQ(expect_data, m_data32);


        ASSERT_NO_THROW(eeic->Read(sideband, eeic_base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));
        uint16_t expect_data_low = 0x000F;
        uint32_t index_16 = ((eeic_base_addr + ch*4) >> 1) & 0x1;
        memcpy (&m_data16[index_16], data, sizeof(m_data16));
        ASSERT_EQ(expect_data_low, m_data16[index_16]);

        ASSERT_NO_THROW(eeic->Read(sideband, eeic_base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));
        uint16_t expect_data_high = 0x0080;
        index_16 = ((eeic_base_addr + ch*4 + 2) >> 1) & 0x1;
        memcpy (&m_data16[index_16], data, sizeof(m_data16));
        ASSERT_EQ(expect_data_high, m_data16[index_16]);

        ASSERT_NO_THROW(eeic->Read(sideband, eeic_base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));
        uint8_t expect_data_byte0 = 0x0F;
        uint32_t index_8 = (eeic_base_addr + ch*4) & 0x3;
        memcpy (&m_data8[index_8], data, sizeof(m_data8));
        ASSERT_EQ(expect_data_byte0, m_data8[index_8]);

        ASSERT_NO_THROW(eeic->Read(sideband, eeic_base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));
        uint8_t expect_data_byte1 = 0x00;
        index_8 = (eeic_base_addr + ch*4 + 1) & 0x3;
        memcpy (&m_data8[index_8], data, sizeof(m_data8));
        ASSERT_EQ(expect_data_byte1, m_data8[index_8]);

        ASSERT_NO_THROW(eeic->Read(sideband, eeic_base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));
        uint8_t expect_data_byte2 = 0x80;
        index_8 = (eeic_base_addr + ch*4 + 2) & 0x3;
        memcpy (&m_data8[index_8], data, sizeof(m_data8));
        ASSERT_EQ(expect_data_byte2, m_data8[index_8]);

        ASSERT_NO_THROW(eeic->Read(sideband, eeic_base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));
        uint8_t expect_data_byte3 = 0x00;
        index_8 = (eeic_base_addr + ch*4 + 3) & 0x3;
        memcpy (&m_data8[index_8], data, sizeof(m_data8));
        ASSERT_EQ(expect_data_byte3, m_data8[index_8]);

        delete eeic;
    }
}


class TregEEICAccessTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, uint32_t>> {
protected:
	TregEEICAccessTest2() {}
  virtual ~TregEEICAccessTest2() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);

    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
  }

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  TRegEEIC* eeic;
  uint32_t pe_id = 0;
  PhysAddr base_addr = 0xFFF80000UL +0x4000 ;
  uint32_t eeic_num = 2048;
  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregEEICAccessTest2, CheckEEIC_Access) {
  uint32_t temp = std::get<0>(GetParam());
  uint32_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  cintc2->HardReset();
  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data32, rdata, sizeof(m_data32));
      ASSERT_EQ(expect, m_data32);

  }

  uint16_t temp_16 = std::get<0>(GetParam());
  uint16_t expect_16_low = std::get<1>(GetParam());

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  cintc2->HardReset();
  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata2, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16, rdata2, sizeof(m_data16));
      ASSERT_EQ(expect_16_low, m_data16[index_16]);

  }

  temp_16 = (temp >> 16) & 0xFFFF;
  uint16_t expect_16_high = (expect >> 16) & 0xFFFF;

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  cintc2->HardReset();
  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata3, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data16, rdata3, sizeof(m_data16));
      ASSERT_EQ(expect_16_high, m_data16[0]);

  }

  uint8_t temp_8_byte0 = std::get<0>(GetParam());
  uint8_t expect_8_byte0 = std::get<1>(GetParam());

  memcpy (wdata, &temp_8_byte0, sizeof(temp_8_byte0));
  uint8_t rdata4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  cintc2->HardReset();
  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
        eeic->Reset();

        SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata4, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        memcpy (&m_data8, rdata4, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte0, m_data8[0]);

  }

  uint32_t temp_8_byte1 = std::get<0>(GetParam());
  uint32_t expect_8_byte1 = std::get<1>(GetParam());
  temp_8_byte1 = (temp_8_byte1 >> 8) & 0xFFU;
  expect_8_byte1 = ((expect_8_byte1 >> 8)) & 0xFFU;

  memcpy (wdata, &temp_8_byte1, sizeof(temp_8_byte1));
  uint8_t rdata5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  cintc2->HardReset();
  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 1, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)rdata5, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data8, rdata5, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte1, m_data8[0]);

  }

  uint32_t temp_8_byte2 = std::get<0>(GetParam());
  uint32_t expect_8_byte2 = std::get<1>(GetParam());
  temp_8_byte2 = (temp_8_byte2 >> 16) & 0xFFU;
  expect_8_byte2 = ((expect_8_byte2 >> 16)) & 0xFFU;

  memcpy (wdata, &temp_8_byte2, sizeof(temp_8_byte2));
  uint8_t rdata6[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  cintc2->HardReset();
  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata6, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data8, rdata6, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte2, m_data8[0]);

    }

  uint32_t temp_8_byte3 = std::get<0>(GetParam());
  uint32_t expect_8_byte3 = std::get<1>(GetParam());
  temp_8_byte3 = (temp_8_byte3 >> 24) & 0xFFU;
  expect_8_byte3 = ((expect_8_byte3 >> 24)) & 0xFFU;
  memcpy (wdata, &temp_8_byte3, sizeof(temp_8_byte3));
  uint8_t rdata7[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  cintc2->HardReset();
  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 3, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)rdata7, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data8, rdata7, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte3, m_data8[0]);

  }
}

std::tuple<uint32_t, uint32_t> const WithParamInterface_e[] = {
  /* 0000 */ std::make_tuple(0x12345678, 0x10000038),
  /* 0001 */ std::make_tuple(0x0000ffff, 0x000803f),
  /* 0002 */ std::make_tuple(0x00000000, 0x00000000),
  /* 0003 */ std::make_tuple(0x000010ff, 0x0000003f),
  /* 0004 */ std::make_tuple(0xff000000, 0x10000000),
  /* 0005 */ std::make_tuple(0xf000ffff, 0x1000803f),
  /* 0006 */ std::make_tuple(0xffffffff, 0x10c0803f),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEEIC_Access, TregEEICAccessTest2,
                        ::testing::ValuesIn(WithParamInterface_e));


//class TRegEEICAccessTestDebug
//    : public testing::Test,
//      public testing::WithParamInterface<std::tuple<PhysAddr, uint32_t>> {
//protected:
//  TRegEEICAccessTestDebug() {}
//  virtual ~TRegEEICAccessTestDebug() {}
//  virtual void SetUp() {
//    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
//    m_pes.push_back(mock_pe);
//    m_mock_fsbus = new MockFsBus();
//    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);
//
//    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
//    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
//  }
//  virtual void TearDown() {
//    delete mock_pe;
//    delete m_mock_fsbus;
//    delete cintc2;
//  }
//
//public:
//  MockSideBand sideband;
//  MockProcElement *mock_pe;
//  MockFsBus *m_mock_fsbus;
//  PeVec m_pes;
//  CIntc2 *cintc2;
//  TRegEEIC* eeic;
//  uint32_t pe_id = 0;
//  PhysAddr base_addr = 0xFFF80000UL +0x4000 ;
//  uint32_t eeic_num = 2048;
//  ByteSize reg_size = SIZE_32BIT;
//  ByteSize min_access_size = SIZE_8BIT;
//  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
//  union {
//      uint32_t m_data32;
//      uint16_t m_data16[2];
//      uint8_t  m_data8[4];
//  };
//
//private:
//};
//
//
///*!
// * @brief Check TRegEEIC Read/Write in debug operation.
// */
//TEST_P(TRegEEICAccessTestDebug, CheckEEIC_Access) {
//  uint32_t temp = std::get<0>(GetParam());
//  uint32_t expect = std::get<1>(GetParam());
//
//  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
//  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//
//  cintc2->HardReset();
//  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
//      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
//      eeic->Reset();
//
//      SysErrCause syserr = eeic->WriteBody(base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      syserr = eeic->ReadBody(base_addr + ch*4, (uint8_t*)rdata, SIZE_32BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      memcpy (&m_data32, rdata, sizeof(m_data32));
//      ASSERT_EQ(expect, m_data32);
//
//  }
//
//  uint16_t temp_16 = std::get<0>(GetParam());
//  uint16_t expect_16_low = std::get<1>(GetParam());
//
//  memcpy (wdata, &temp_16, sizeof(temp_16));
//  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//
//  cintc2->HardReset();
//  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
//      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
//      eeic->Reset();
//
//      SysErrCause syserr = eeic->WriteBody(base_addr + ch*4, (uint8_t*)wdata, SIZE_16BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      syserr = eeic->ReadBody(base_addr + ch*4, (uint8_t*)rdata2, SIZE_16BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
//      memcpy (&m_data16, rdata2, sizeof(m_data16));
//      ASSERT_EQ(expect_16_low, m_data16[index_16]);
//
//  }
//
//  temp_16 = (temp >> 16) & 0xFFFF;
//  uint16_t expect_16_high = (expect >> 16) & 0xFFFF;
//
//  memcpy (wdata, &temp_16, sizeof(temp_16));
//  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//
//  cintc2->HardReset();
//  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
//      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
//      eeic->Reset();
//
//      SysErrCause syserr = eeic->WriteBody(base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_16BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      syserr = eeic->ReadBody(base_addr + ch*4 + 2, (uint8_t*)rdata3, SIZE_16BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      memcpy (&m_data16, rdata3, sizeof(m_data16));
//      ASSERT_EQ(expect_16_high, m_data16[0]);
//
//  }
//
//  uint8_t temp_8_byte0 = std::get<0>(GetParam());
//  uint8_t expect_8_byte0 = std::get<1>(GetParam());
//
//  memcpy (wdata, &temp_8_byte0, sizeof(temp_8_byte0));
//  uint8_t rdata4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//
//  cintc2->HardReset();
//  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
//        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
//        eeic->Reset();
//
//        SysErrCause syserr = eeic->WriteBody(base_addr + ch*4, (uint8_t*)wdata, SIZE_8BIT);
//        ASSERT_EQ(0x0U, syserr);
//
//        syserr = eeic->ReadBody(base_addr + ch*4, (uint8_t*)rdata4, SIZE_8BIT);
//        ASSERT_EQ(0x0U, syserr);
//
//        memcpy (&m_data8, rdata4, sizeof(m_data8));
//        ASSERT_EQ(expect_8_byte0, m_data8[0]);
//
//  }
//
//  uint32_t temp_8_byte1 = std::get<0>(GetParam());
//  uint32_t expect_8_byte1 = std::get<1>(GetParam());
//  temp_8_byte1 = (temp_8_byte1 >> 8) & 0xFFU;
//  expect_8_byte1 = ((expect_8_byte1 >> 8)) & 0xFFU;
//
//  memcpy (wdata, &temp_8_byte1, sizeof(temp_8_byte1));
//  uint8_t rdata5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//
//  cintc2->HardReset();
//  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
//      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
//      eeic->Reset();
//
//      SysErrCause syserr = eeic->WriteBody(base_addr + ch*4 + 1, (uint8_t*)wdata, SIZE_8BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      syserr = eeic->ReadBody(base_addr + ch*4 + 1, (uint8_t*)rdata5, SIZE_8BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      memcpy (&m_data8, rdata5, sizeof(m_data8));
//      ASSERT_EQ(expect_8_byte1, m_data8[0]);
//
//  }
//
//  uint32_t temp_8_byte2 = std::get<0>(GetParam());
//  uint32_t expect_8_byte2 = std::get<1>(GetParam());
//  temp_8_byte2 = (temp_8_byte2 >> 16) & 0xFFU;
//  expect_8_byte2 = ((expect_8_byte2 >> 16)) & 0xFFU;
//
//  memcpy (wdata, &temp_8_byte2, sizeof(temp_8_byte2));
//  uint8_t rdata6[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//
//  cintc2->HardReset();
//  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
//      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
//      eeic->Reset();
//
//      SysErrCause syserr = eeic->WriteBody(base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_8BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      syserr = eeic->ReadBody(base_addr + ch*4 + 2, (uint8_t*)rdata6, SIZE_8BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      memcpy (&m_data8, rdata6, sizeof(m_data8));
//      ASSERT_EQ(expect_8_byte2, m_data8[0]);
//
//    }
//
//  uint32_t temp_8_byte3 = std::get<0>(GetParam());
//  uint32_t expect_8_byte3 = std::get<1>(GetParam());
//  temp_8_byte3 = (temp_8_byte3 >> 24) & 0xFFU;
//  expect_8_byte3 = ((expect_8_byte3 >> 24)) & 0xFFU;
//  memcpy (wdata, &temp_8_byte3, sizeof(temp_8_byte3));
//  uint8_t rdata7[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//
//  cintc2->HardReset();
//  for (uint32_t ch = 32; ch < TRegIntc2::N_EEIC; ch ++ ){
//      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, cintc2);
//      eeic->Reset();
//
//      SysErrCause syserr = eeic->WriteBody(base_addr + ch*4 + 3, (uint8_t*)wdata, SIZE_8BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      syserr = eeic->ReadBody(base_addr + ch*4 + 3, (uint8_t*)rdata7, SIZE_8BIT);
//      ASSERT_EQ(0x0U, syserr);
//
//      memcpy (&m_data8, rdata7, sizeof(m_data8));
//      ASSERT_EQ(expect_8_byte3, m_data8[0]);
//
//  }
//}
//
//std::tuple<uint32_t, uint32_t> const WithParamInterface_e_db[] = {
//  /* 0000 */ std::make_tuple(0x12345678, 0x10000038),
//  /* 0001 */ std::make_tuple(0x0000ffff, 0x0000803f),
//  /* 0002 */ std::make_tuple(0x00000000, 0x00000000),
//  /* 0003 */ std::make_tuple(0x000010ff, 0x0000003f),
//  /* 0004 */ std::make_tuple(0xff000000, 0x90000000),
//  /* 0005 */ std::make_tuple(0xf000ffff, 0x9000803f),
//  /* 0006 */ std::make_tuple(0xffffffff, 0x90c0803f),
//  // please write more test case.
//};
//INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TRegEEICAccessTestDebug,
//                        ::testing::ValuesIn(WithParamInterface_e_db));
//
//
//class TRegEEICBitSetTest
//    : public testing::Test,
//      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
//protected:
//  TRegEEICBitSetTest() {}
//  virtual ~TRegEEICBitSetTest() {}
//  virtual void SetUp() {
//    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
//    m_pes.push_back(mock_pe);
//    m_mock_fsbus = new MockFsBus();
//    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);
//
//    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
//    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
//  }
//  virtual void TearDown() {
//    delete mock_pe;
//    delete m_mock_fsbus;
//    delete cintc2;
//  }
//
//public:
//  MockSideBand sideband;
//  MockProcElement *mock_pe;
//  MockFsBus *m_mock_fsbus;
//  PeVec m_pes;
//  CIntc2 *cintc2;
//  TRegEEIC* eeic;
//  TRegEIC* eic;
//  uint32_t pe_id = 0;
//  PhysAddr eic_base_addr = 0xFFF80000UL;
//  PhysAddr eeic_base_addr = 0xFFF80000UL + 0x4000;
//  uint32_t eeic_num = 2048;
//
//private:
//};
//
//
///*!
// * @brief Check TRegEEIC Get/Set EICT operation.
// */
//TEST_F(TRegEEICBitSetTest, CheckEEIC_EICT) {
//    uint32_t expect_data_low = 0x0;
//    uint32_t expect_data_hi = 0x1;
//    SCOPED_TRACE("CheckEEIC_EICT");
//    cintc2->HardReset();
//    eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_base_addr + 0x20*4, 0x20, pe_id, cintc2); //Channel 32
//    eeic->Reset();
//    ASSERT_NO_THROW(eeic->SetEICT(0));
//    ASSERT_NO_THROW(eeic->GetEICT());
//    ASSERT_EQ(expect_data_low, eeic->GetEICT());
//
//    ASSERT_NO_THROW(eeic->SetEICT(1));
//    ASSERT_NO_THROW(eeic->GetEICT());
//    ASSERT_EQ(expect_data_hi, eeic->GetEICT());
//    delete eeic;
//}
//
//
///*!
// * @brief Check TRegEEIC Get/Set EITB operation.
// */
//TEST_F(TRegEEICBitSetTest, CheckEEIC_EITB) {
//    uint32_t expect_data_low = 0x0;
//    uint32_t expect_data_hi = 0x1;
//    SCOPED_TRACE("CheckEEIC_EITB");
//    cintc2->HardReset();
//    eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_base_addr + 0x20*4, 0x20, pe_id, cintc2); //Channel 32
//    eeic->Reset();
//    ASSERT_NO_THROW(eeic->SetEITB(0));
//    ASSERT_NO_THROW(eeic->GetEITB());
//    ASSERT_EQ(expect_data_low, eeic->GetEITB());
//
//    ASSERT_NO_THROW(eeic->SetEITB(1));
//    ASSERT_NO_THROW(eeic->GetEITB());
//    ASSERT_EQ(expect_data_hi, eeic->GetEITB());
//    delete eeic;
//}
//
//
///*!
// * @brief Check TRegEEIC Get/Set EIRF operation.
// */
//TEST_F(TRegEEICBitSetTest, CheckEEIC_EIRF) {
//    uint32_t expect_data_low = 0x0;
//    uint32_t expect_data_hi = 0x1;
//    SCOPED_TRACE("CheckEIC_EIRF");
//    cintc2->HardReset();
//    eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_base_addr + 0x20*4, 0x20, pe_id, cintc2); //Channel 32
//    eeic->Reset();
//    ASSERT_NO_THROW(eeic->SetEIRF(0));
//    ASSERT_NO_THROW(eeic->GetEIRF());
//    ASSERT_EQ(expect_data_low, eeic->GetEIRF());
//
//    ASSERT_NO_THROW(eeic->SetEIRF(1));
//    ASSERT_NO_THROW(eeic->GetEIRF());
//    ASSERT_EQ(expect_data_hi, eeic->GetEIRF());
//    delete eeic;
//}
//
//
///*!
// * @brief Check TRegEEIC Get/Set EIMK operation.
// */
//TEST_F(TRegEEICBitSetTest, CheckEEIC_EIMK) {
//    uint32_t expect_data_low = 0x0;
//    uint32_t expect_data_hi = 0x1;
//    SCOPED_TRACE("CheckEIC_EIMK");
//    cintc2->HardReset();
//    eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_base_addr + 0x20*4, 0x20, pe_id, cintc2); //Channel 32
//    eeic->Reset();
//    ASSERT_NO_THROW(eeic->SetEIMK(0));
//    ASSERT_NO_THROW(eeic->GetEIMK());
//    ASSERT_EQ(expect_data_low, eeic->GetEIMK());
//
//    ASSERT_NO_THROW(eeic->SetEIMK(1));
//    ASSERT_NO_THROW(eeic->GetEIMK());
//    ASSERT_EQ(expect_data_hi, eeic->GetEIMK());
//    delete eeic;
//}
//
//
///*!
// * @brief Check TRegEEIC Get/Set EIP operation.
// */
//TEST_F(TRegEEICBitSetTest, CheckEIC_EIP) {
//    SCOPED_TRACE("CheckEIC_EIP");
//    cintc2->HardReset();
//    eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_base_addr + 0x20*4, 0x20, pe_id, cintc2); //Channel 32
//    eeic->Reset();
//    for (uint32_t i = 0; i < 16; i++) {
//        ASSERT_NO_THROW(eeic->SetEIP(i));
//        ASSERT_NO_THROW(eeic->GetEIP());
//        ASSERT_EQ(i, eeic->GetEIP());
//    }
//    delete eeic;
//}
///*!
// * @brief Check TRegEEIC GetName operation.
// */
//TEST_F(TRegEEICBitSetTest, Check_GetName) {
//    SCOPED_TRACE("Check_GetName");
//    cintc2->HardReset();
//    eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_base_addr + 0x20*4, 0x20, pe_id, cintc2); //Channel 32
//    eeic->Reset();
//    const char* name = eeic->GetName();
//    ASSERT_STREQ("EEIC", name);
//    delete eeic;
//}
//
//
//TEST_F(TRegEEICBitSetTest, SetEIPTest_EEIC) {
//  uint32_t ch = 32;
//  cintc2->HardReset();
//  PhysAddr eeic_base = eeic_base_addr;
//  PhysAddr base_addr = eic_base_addr;
//
//  uint32_t data_input = 0x100000ff;
//  uint32_t data_input_2 = 0x1000;
//  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
//  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
//
//  cintc2->m_intc2_queue.clear();
//
//  cintc2->TargetWrite(sideband, eeic_base + ch*4, wdata, SIZE_32BIT);
//  cintc2->CyclicHandler();
//  ASSERT_EQ(0x3F, cintc2->m_intc2_priority[0]->m_priority_eiint);
//
//  cintc2->TargetWrite(sideband, base_addr + ch*2, wdata_2, SIZE_16BIT);
//  cintc2->CyclicHandler();
//  ASSERT_EQ(0x30, cintc2->m_intc2_priority[0]->m_priority_eiint);
//
//}
//
//class TRegEEIC_TestReplaceEICWriteData
//    : public testing::Test,
//      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
//protected:
//  TRegEEIC_TestReplaceEICWriteData() {}
//  virtual ~TRegEEIC_TestReplaceEICWriteData() {}
//  virtual void SetUp() {
//    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
//    m_pes.push_back(mock_pe);
//    m_mock_fsbus = new MockFsBus();
//    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);
//
//    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
//    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
//    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
//  }
//  virtual void TearDown() {
//    delete mock_pe;
//    delete m_mock_fsbus;
//    delete cintc2;
//  }
//
//public:
//  MockSideBand sideband;
//  MockProcElement *mock_pe;
//  MockFsBus *m_mock_fsbus;
//  PeVec m_pes;
//  CIntc2 *cintc2;
//  TRegEEIC* eeic;
//  uint32_t pe_id = 0;
//  PhysAddr eeic_base_addr = 0xFFF80000UL +0x4000;
//  uint32_t eeic_num = 2048;
//
//private:
//};
//
//
///*!
// * @brief Check TRegEEIC ReplaceEICWriteData operation.
// */
//TEST_F(TRegEEIC_TestReplaceEICWriteData, CheckEIC_ReplaceEICWriteData) {
//    uint8_t  wdata[]  = {0xFF, 0xFF};
//    SCOPED_TRACE("CheckEIC_ReplaceEICWriteData");
//    eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_base_addr + 0x20*4, 0x20, pe_id, cintc2); //Channel 32
//    eeic->Reset();
//    ASSERT_NO_THROW(eeic->SetEIRF(1));
//    ASSERT_NO_THROW(eeic->SetEICT(0));
//    eeic->ReplaceEICWriteData(wdata, 0);
//    ASSERT_EQ(0x7FU, wdata[0]);
//    ASSERT_NO_THROW(eeic->SetEICT(1));
//    eeic->ReplaceEICWriteData(wdata, 1);
//    ASSERT_EQ(0xFFU, wdata[1]);
//    delete eeic;
//}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

