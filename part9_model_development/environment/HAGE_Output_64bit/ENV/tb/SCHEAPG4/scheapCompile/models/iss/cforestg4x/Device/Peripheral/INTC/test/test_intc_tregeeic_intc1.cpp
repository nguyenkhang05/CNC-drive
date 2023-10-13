/**
 * @file test_intc_tregeeic_intc1.cpp
 *
 * @date Created on: 2016
 * @author Author: YVo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for INTC. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <string.h>
#include <vector>
#include <memory>
#include <map>
#include <stdexcept>
#include "./config.h"
#include "./fs_controler.h"
#include "./intc1.h"
#include "./exp_info.h"
#include "./fsbus.h"
#include "./proc_element.h"
#include "./forest.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"
#include "./forest_common.h"
#include "./intc_api.h"
#include "./module_vc.h"
#include "./treg.h"
#include "intc1.h"
#include "./intc2.h"

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

using ::testing::ExpectationSet;
using ::testing::Expectation;



class TregEEICAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregEEICAccessTest() {}
  virtual ~TregEEICAccessTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_F(TregEEICAccessTest, CheckEEIC_initial_HV) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));
      uint32_t expect_data = 0x0080000F;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);


      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));
      uint16_t expect_data_low = 0x000F;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data_low, m_data16[index_16]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));
      uint16_t expect_data_high = 0x0080;
      index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data_high, m_data16[index_16]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte0 = 0x0F;
      uint32_t index_8 = (base_addr + ch*4) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte0, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte1 = 0x00;
      index_8 = (base_addr + ch*4 + 1) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte1, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte2 = 0x80;
      index_8 = (base_addr + ch*4 + 2) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte2, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte3 = 0x00;
      index_8 = (base_addr + ch*4 + 3) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte3, m_data8[index_8]);

      delete eeic;
  }
}

TEST_F(TregEEICAccessTest, CheckEEIC_initial_SV_DIFF_GM) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));
      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);


      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));
      uint16_t expect_data_low = 0x0;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data_low, m_data16[index_16]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));
      uint16_t expect_data_high = 0x0;
      index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data_high, m_data16[index_16]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte0 = 0x0;
      uint32_t index_8 = (base_addr + ch*4) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte0, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte1 = 0x00;
      index_8 = (base_addr + ch*4 + 1) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte1, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte2 = 0x0;
      index_8 = (base_addr + ch*4 + 2) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte2, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte3 = 0x00;
      index_8 = (base_addr + ch*4 + 3) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte3, m_data8[index_8]);

      delete eeic;
  }
}

TEST_F(TregEEICAccessTest, CheckEEIC_initial_SV_DIFF_GPID) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));
      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);


      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));
      uint16_t expect_data_low = 0x0;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data_low, m_data16[index_16]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));
      uint16_t expect_data_high = 0x0;
      index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data_high, m_data16[index_16]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte0 = 0x0;
      uint32_t index_8 = (base_addr + ch*4) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte0, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte1 = 0x0;
      index_8 = (base_addr + ch*4 + 1) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte1, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte2 = 0x0;
      index_8 = (base_addr + ch*4 + 2) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte2, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte3 = 0x0;
      index_8 = (base_addr + ch*4 + 3) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte3, m_data8[index_8]);

      delete eeic;
  }
}

TEST_F(TregEEICAccessTest, CheckEEIC_initial_UM) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      sideband.SetUM(1);
      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));
      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);


      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));
      uint16_t expect_data_low = 0x0;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data_low, m_data16[index_16]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));
      uint16_t expect_data_high = 0x0;
      index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data_high, m_data16[index_16]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte0 = 0x0;
      uint32_t index_8 = (base_addr + ch*4) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte0, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte1 = 0x0;
      index_8 = (base_addr + ch*4 + 1) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte1, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte2 = 0x0;
      index_8 = (base_addr + ch*4 + 2) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte2, m_data8[index_8]);

      ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_byte3 = 0x0;
      index_8 = (base_addr + ch*4 + 3) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_byte3, m_data8[index_8]);

      delete eeic;
  }
}

TEST_F(TregEEICAccessTest, CheckEEIC_GetName) {
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eeic->Reset();

      ASSERT_STREQ("EEIC", eeic->GetName());

      delete eeic;
  }
}

TEST_F(TregEEICAccessTest, CheckEEIC_Access) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eeic->Reset();
      ASSERT_THROW(eeic->Write(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_64BIT), std::range_error);
      ASSERT_THROW(eeic->Write(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_128BIT), std::range_error);

      delete eeic;
  }
}

TEST_F(TregEEICAccessTest, CheckEEIC_AccessTest2) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eeic->Reset();
      ASSERT_THROW(eeic->WriteBody(base_addr + ch*2, (uint8_t*)data, SIZE_64BIT), std::range_error);
      ASSERT_THROW(eeic->WriteBody(base_addr + ch*2, (uint8_t*)data, SIZE_128BIT), std::range_error);

      delete eeic;
  }
}


class TregEEICAccessTest2_IHVE0
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
	TregEEICAccessTest2_IHVE0() {}
  virtual ~TregEEICAccessTest2_IHVE0() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEEICAccessTest2_IHVE0, CheckEEIC_Access_IHVE0) {
  uint32_t temp = std::get<0>(GetParam());
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  intc1->SetIHVEFromIHVCFG(0);

  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      sideband.SetUM(1);
      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      sideband.SetUM(0);
      syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      delete eeic;
  }
}

std::tuple<uint32_t, uint32_t> const WithParamInterface_e_IHVE0[] = {
  /* 0000 */ std::make_tuple(0x12345678, 0x10000078),
  /* 0001 */ std::make_tuple(0x0000ffff, 0x00080ff),
  /* 0002 */ std::make_tuple(0x00000000, 0x00000000),
  /* 0003 */ std::make_tuple(0x000010ff, 0x000000ff),
  /* 0004 */ std::make_tuple(0xff000000, 0x10000000),
  /* 0005 */ std::make_tuple(0xf000ffff, 0x100080ff),
  /* 0006 */ std::make_tuple(0xffffffff, 0x10c080ff),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEEICAccessTest2_IHVE0,
                        ::testing::ValuesIn(WithParamInterface_e_IHVE0));

class TregEEICAccessTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregEEICAccessTest2() {}
  virtual ~TregEEICAccessTest2() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;
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

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data32, rdata, sizeof(m_data32));
      ASSERT_EQ(expect, m_data32);

      delete eeic;
  }

  uint16_t temp_16 = std::get<0>(GetParam());
  uint16_t expect_16_low = std::get<1>(GetParam());

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata2, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16, rdata2, sizeof(m_data16));
      ASSERT_EQ(expect_16_low, m_data16[index_16]);

      delete eeic;
  }

  temp_16 = (temp >> 16) & 0xFFFF;
  uint16_t expect_16_high = (expect >> 16) & 0xFFFF;

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata3, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data16, rdata3, sizeof(m_data16));
      ASSERT_EQ(expect_16_high, m_data16[0]);

      delete eeic;
  }

  uint8_t temp_8_byte0 = std::get<0>(GetParam());
  uint8_t expect_8_byte0 = std::get<1>(GetParam());

  memcpy (wdata, &temp_8_byte0, sizeof(temp_8_byte0));
  uint8_t rdata4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
       TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata4, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        memcpy (&m_data8, rdata4, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte0, m_data8[0]);

        delete eeic;
  }

  uint32_t temp_8_byte1 = std::get<0>(GetParam());
  uint32_t expect_8_byte1 = std::get<1>(GetParam());
  temp_8_byte1 = (temp_8_byte1 >> 8) & 0xFFU;
  expect_8_byte1 = ((expect_8_byte1 >> 8)) & 0xFFU;

  memcpy (wdata, &temp_8_byte1, sizeof(temp_8_byte1));
  uint8_t rdata5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 1, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)rdata5, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data8, rdata5, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte1, m_data8[0]);

      delete eeic;
  }

  uint32_t temp_8_byte2 = std::get<0>(GetParam());
  uint32_t expect_8_byte2 = std::get<1>(GetParam());
  temp_8_byte2 = (temp_8_byte2 >> 16) & 0xFFU;
  expect_8_byte2 = ((expect_8_byte2 >> 16)) & 0xFFU;

  memcpy (wdata, &temp_8_byte2, sizeof(temp_8_byte2));
  uint8_t rdata6[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata6, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data8, rdata6, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte2, m_data8[0]);

      delete eeic;
    }

  uint32_t temp_8_byte3 = std::get<0>(GetParam());
  uint32_t expect_8_byte3 = std::get<1>(GetParam());
  temp_8_byte3 = (temp_8_byte3 >> 24) & 0xFFU;
  expect_8_byte3 = ((expect_8_byte3 >> 24)) & 0xFFU;
  memcpy (wdata, &temp_8_byte3, sizeof(temp_8_byte3));
  uint8_t rdata7[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 3, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)rdata7, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data8, rdata7, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte3, m_data8[0]);

      delete eeic;
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
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEEICAccessTest2,
                        ::testing::ValuesIn(WithParamInterface_e));


class TregEEICAccessTest2_SV
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregEEICAccessTest2_SV() {}
  virtual ~TregEEICAccessTest2_SV() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEEICAccessTest2_SV, CheckEEIC_Access_DIFF_GM) {
  uint32_t temp = std::get<0>(GetParam());
  uint32_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      memcpy (&m_data32, rdata, sizeof(m_data32));
      ASSERT_EQ(expect, m_data32);

      delete eeic;
  }

  uint16_t temp_16 = std::get<0>(GetParam());
  uint16_t expect_16_low = 0;

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata2, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16, rdata2, sizeof(m_data16));
      ASSERT_EQ(expect_16_low, m_data16[index_16]);

      delete eeic;
  }

  temp_16 = (temp >> 16) & 0xFFFF;
  uint16_t expect_16_high = 0;

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata3, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      memcpy (&m_data16, rdata3, sizeof(m_data16));
      ASSERT_EQ(expect_16_high, m_data16[0]);

      delete eeic;
  }

  uint8_t temp_8_byte0 = std::get<0>(GetParam());
  uint8_t expect_8_byte0 = 0;

  memcpy (wdata, &temp_8_byte0, sizeof(temp_8_byte0));
  uint8_t rdata4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
       TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata4, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        memcpy (&m_data8, rdata4, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte0, m_data8[0]);

        delete eeic;
  }

  uint32_t temp_8_byte1 = std::get<0>(GetParam());
  uint32_t expect_8_byte1 = 0;
  temp_8_byte1 = (temp_8_byte1 >> 8) & 0xFFU;

  memcpy (wdata, &temp_8_byte1, sizeof(temp_8_byte1));
  uint8_t rdata5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 1, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)rdata5, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      memcpy (&m_data8, rdata5, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte1, m_data8[0]);

      delete eeic;
  }

  uint32_t temp_8_byte2 = std::get<0>(GetParam());
  uint32_t expect_8_byte2 = 0;
  temp_8_byte2 = (temp_8_byte2 >> 16) & 0xFFU;

  memcpy (wdata, &temp_8_byte2, sizeof(temp_8_byte2));
  uint8_t rdata6[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata6, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      memcpy (&m_data8, rdata6, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte2, m_data8[0]);

      delete eeic;
    }

  uint32_t temp_8_byte3 = std::get<0>(GetParam());
  uint32_t expect_8_byte3 = 0;
  temp_8_byte3 = (temp_8_byte3 >> 24) & 0xFFU;
  memcpy (wdata, &temp_8_byte3, sizeof(temp_8_byte3));
  uint8_t rdata7[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4 + 3, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)rdata7, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      memcpy (&m_data8, rdata7, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte3, m_data8[0]);

      delete eeic;
  }
}

TEST_P(TregEEICAccessTest2_SV, CheckEEIC_Access_DIFF_GM_64bits) {
  uint32_t temp = std::get<0>(GetParam());
  uint32_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      memcpy (&m_data32, rdata, sizeof(m_data32));
      ASSERT_EQ(expect, m_data32);

      delete eeic;
  }
}

TEST_P(TregEEICAccessTest2_SV, CheckEEIC_Access_DIFF_GM_128bits) {
  uint32_t temp = std::get<0>(GetParam());
  uint32_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0 };

  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();
      SysErrCause syserr = eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      memcpy (&m_data32, rdata, sizeof(m_data32));
      ASSERT_EQ(expect, m_data32);

      delete eeic;
 }
}


std::tuple<uint32_t, uint32_t> const WithParamInterface_e_SV[] = {
  /* 0000 */ std::make_tuple(0x12345678, 0x10000038),
  /* 0001 */ std::make_tuple(0x0000ffff, 0x000803f),
  /* 0002 */ std::make_tuple(0x00000000, 0x00000000),
  /* 0003 */ std::make_tuple(0x000010ff, 0x0000003f),
  /* 0004 */ std::make_tuple(0xff000000, 0x10000000),
  /* 0005 */ std::make_tuple(0xf000ffff, 0x1000803f),
  /* 0006 */ std::make_tuple(0xffffffff, 0x10c0803f),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEEICAccessTest2_SV,
                        ::testing::ValuesIn(WithParamInterface_e_SV));

class TregEEICAccessTest2_SV_DIFF_GPID
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
	TregEEICAccessTest2_SV_DIFF_GPID() {}
  virtual ~TregEEICAccessTest2_SV_DIFF_GPID() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEEICAccessTest2_SV_DIFF_GPID, CheckEEIC_Access) {
  uint32_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  SysErrCause syserr = 1;

  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }

  uint16_t temp_16 = std::get<0>(GetParam());

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata2, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }

  temp_16 = (temp >> 16) & 0xFFFF;

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2 + 2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2 + 2, (uint8_t*)rdata3, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }

  uint8_t temp_8_byte0 = std::get<0>(GetParam());

  memcpy (wdata, &temp_8_byte0, sizeof(temp_8_byte0));
  uint8_t rdata4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
       TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata4, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        delete eeic;
  }

  uint32_t temp_8_byte1 = std::get<0>(GetParam());
  temp_8_byte1 = (temp_8_byte1 >> 8) & 0xFFU;

  memcpy (wdata, &temp_8_byte1, sizeof(temp_8_byte1));
  uint8_t rdata5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2 + 1, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2 + 1, (uint8_t*)rdata5, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }

  uint32_t temp_8_byte2 = std::get<0>(GetParam());
  uint32_t expect_8_byte2 = std::get<1>(GetParam());
  temp_8_byte2 = (temp_8_byte2 >> 16) & 0xFFU;
  expect_8_byte2 = ((expect_8_byte2 >> 16)) & 0xFFU;

  memcpy (wdata, &temp_8_byte2, sizeof(temp_8_byte2));
  uint8_t rdata6[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2 + 2, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2 + 2, (uint8_t*)rdata6, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
    }

  uint32_t temp_8_byte3 = std::get<0>(GetParam());
  uint32_t expect_8_byte3 = std::get<1>(GetParam());
  temp_8_byte3 = (temp_8_byte3 >> 24) & 0xFFU;
  expect_8_byte3 = ((expect_8_byte3 >> 24)) & 0xFFU;
  memcpy (wdata, &temp_8_byte3, sizeof(temp_8_byte3));
  uint8_t rdata7[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2 + 3, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2 + 3, (uint8_t*)rdata7, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }
}

TEST_P(TregEEICAccessTest2_SV_DIFF_GPID, CheckEEIC_Access_64bits) {
  uint32_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  SysErrCause syserr = 1;

  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }
}

TEST_P(TregEEICAccessTest2_SV_DIFF_GPID, CheckEEIC_Access_128bits) {
  uint32_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0 };
  SysErrCause syserr = 1;

  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }
}

std::tuple<uint32_t, uint32_t> const WithParamInterface_e_SV_DIFF_GPID[] = {
  /* 0000 */ std::make_tuple(0x12345678, 0x0),
  /* 0001 */ std::make_tuple(0x0000ffff, 0x0),
  /* 0002 */ std::make_tuple(0x00000000, 0x0),
  /* 0003 */ std::make_tuple(0x000010ff, 0x0),
  /* 0004 */ std::make_tuple(0xff000000, 0x0),
  /* 0005 */ std::make_tuple(0xf000ffff, 0x0),
  /* 0006 */ std::make_tuple(0xffffffff, 0x0),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEEICAccessTest2_SV_DIFF_GPID,
                        ::testing::ValuesIn(WithParamInterface_e_SV_DIFF_GPID));


class TregEEICAccessTest2_UM
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
	TregEEICAccessTest2_UM() {}
  virtual ~TregEEICAccessTest2_UM() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_P(TregEEICAccessTest2_UM, CheckEEIC_Access) {
  uint32_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  SysErrCause syserr = 1;

  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }

  uint16_t temp_16 = std::get<0>(GetParam());

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata2, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }

  temp_16 = (temp >> 16) & 0xFFFF;

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2 + 2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2 + 2, (uint8_t*)rdata3, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }

  uint8_t temp_8_byte0 = std::get<0>(GetParam());

  memcpy (wdata, &temp_8_byte0, sizeof(temp_8_byte0));
  uint8_t rdata4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
       TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata4, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        delete eeic;
  }

  uint32_t temp_8_byte1 = std::get<0>(GetParam());
  temp_8_byte1 = (temp_8_byte1 >> 8) & 0xFFU;

  memcpy (wdata, &temp_8_byte1, sizeof(temp_8_byte1));
  uint8_t rdata5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2 + 1, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2 + 1, (uint8_t*)rdata5, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }

  uint32_t temp_8_byte2 = std::get<0>(GetParam());
  uint32_t expect_8_byte2 = std::get<1>(GetParam());
  temp_8_byte2 = (temp_8_byte2 >> 16) & 0xFFU;
  expect_8_byte2 = ((expect_8_byte2 >> 16)) & 0xFFU;

  memcpy (wdata, &temp_8_byte2, sizeof(temp_8_byte2));
  uint8_t rdata6[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2 + 2, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2 + 2, (uint8_t*)rdata6, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
    }

  uint32_t temp_8_byte3 = std::get<0>(GetParam());
  uint32_t expect_8_byte3 = std::get<1>(GetParam());
  temp_8_byte3 = (temp_8_byte3 >> 24) & 0xFFU;
  expect_8_byte3 = ((expect_8_byte3 >> 24)) & 0xFFU;
  memcpy (wdata, &temp_8_byte3, sizeof(temp_8_byte3));
  uint8_t rdata7[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2 + 3, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2 + 3, (uint8_t*)rdata7, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }
}

TEST_P(TregEEICAccessTest2_UM, CheckEEIC_Access_64bits) {
  uint32_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  SysErrCause syserr = 1;

  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }
}

TEST_P(TregEEICAccessTest2_UM, CheckEEIC_Access_128bits) {
  uint32_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0 };
  SysErrCause syserr = 1;

  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      syserr = eeic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eeic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eeic;
  }
}

std::tuple<uint32_t, uint32_t> const WithParamInterface_e_UM[] = {
  /* 0000 */ std::make_tuple(0x12345678, 0x0),
  /* 0001 */ std::make_tuple(0x0000ffff, 0x0),
  /* 0002 */ std::make_tuple(0x00000000, 0x0),
  /* 0003 */ std::make_tuple(0x000010ff, 0x0),
  /* 0004 */ std::make_tuple(0xff000000, 0x0),
  /* 0005 */ std::make_tuple(0xf000ffff, 0x0),
  /* 0006 */ std::make_tuple(0xffffffff, 0x0),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEEICAccessTest2_UM,
                        ::testing::ValuesIn(WithParamInterface_e_UM));

class TregEEICAccessTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregEEICAccessTest3() {}
  virtual ~TregEEICAccessTest3() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);

    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEEICAccessTest3, CheckEEIC_Access_DB) {
  uint32_t temp = std::get<0>(GetParam());
  uint32_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
  //for (uint32_t ch = 0; ch < 1; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT));
      ASSERT_NO_THROW(eeic->ReadBody(base_addr + ch*4, (uint8_t*)rdata, SIZE_32BIT));

      memcpy (&m_data32, rdata, sizeof(m_data32));
      ASSERT_EQ(expect, m_data32);

      delete eeic;
  }

  uint16_t temp_16 = std::get<0>(GetParam());
  uint16_t expect_16_low = std::get<1>(GetParam());

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(eeic->ReadBody(base_addr + ch*4, (uint8_t*)rdata2, SIZE_16BIT));

      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16, rdata2, sizeof(m_data16));
      ASSERT_EQ(expect_16_low, m_data16[index_16]);

      delete eeic;
  }

  temp_16 = (temp >> 16) & 0xFFFF;
  uint16_t expect_16_high = (expect >> 16) & 0xFFFF;

  memcpy (wdata, &temp_16, sizeof(temp_16));
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(eeic->ReadBody(base_addr + ch*4 + 2, (uint8_t*)rdata3, SIZE_16BIT));

      memcpy (&m_data16, rdata3, sizeof(m_data16));
      ASSERT_EQ(expect_16_high, m_data16[0]);

      delete eeic;
  }

  uint8_t temp_8_byte0 = std::get<0>(GetParam());
  uint8_t expect_8_byte0 = std::get<1>(GetParam());

  memcpy (wdata, &temp_8_byte0, sizeof(temp_8_byte0));
  uint8_t rdata4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4, (uint8_t*)wdata, SIZE_8BIT));
        ASSERT_NO_THROW(eeic->ReadBody(base_addr + ch*4, (uint8_t*)rdata4, SIZE_8BIT));

        memcpy (&m_data8, rdata4, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte0, m_data8[0]);

        delete eeic;
  }

  uint32_t temp_8_byte1 = std::get<0>(GetParam());
  uint32_t expect_8_byte1 = std::get<1>(GetParam());
  temp_8_byte1 = (temp_8_byte1 >> 8) & 0xFFU;
  expect_8_byte1 = ((expect_8_byte1 >> 8)) & 0xFFU;

  memcpy (wdata, &temp_8_byte1, sizeof(temp_8_byte1));
  uint8_t rdata5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4 + 1, (uint8_t*)wdata, SIZE_8BIT));
      ASSERT_NO_THROW(eeic->ReadBody(base_addr + ch*4 + 1, (uint8_t*)rdata5, SIZE_8BIT));

      memcpy (&m_data8, rdata5, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte1, m_data8[0]);

      delete eeic;
  }

  uint32_t temp_8_byte2 = std::get<0>(GetParam());
  uint32_t expect_8_byte2 = std::get<1>(GetParam());
  temp_8_byte2 = (temp_8_byte2 >> 16) & 0xFFU;
  expect_8_byte2 = ((expect_8_byte2 >> 16)) & 0xFFU;

  memcpy (wdata, &temp_8_byte2, sizeof(temp_8_byte2));
  uint8_t rdata6[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
      TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_8BIT));
      ASSERT_NO_THROW(eeic->ReadBody(base_addr + ch*4 + 2, (uint8_t*)rdata6, SIZE_8BIT));

      memcpy (&m_data8, rdata6, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte2, m_data8[0]);

      delete eeic;
    }

  uint32_t temp_8_byte3 = std::get<0>(GetParam());
  uint32_t expect_8_byte3 = std::get<1>(GetParam());
  temp_8_byte3 = (temp_8_byte3 >> 24) & 0xFFU;
  expect_8_byte3 = ((expect_8_byte3 >> 24)) & 0xFFU;
  memcpy (wdata, &temp_8_byte3, sizeof(temp_8_byte3));
  uint8_t rdata7[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
      eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
      eeic->Reset();

      ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4 + 3, (uint8_t*)wdata, SIZE_8BIT));
      ASSERT_NO_THROW(eeic->ReadBody(base_addr + ch*4 + 3, (uint8_t*)rdata7, SIZE_8BIT));

      memcpy (&m_data8, rdata7, sizeof(m_data8));
      ASSERT_EQ(expect_8_byte3, m_data8[0]);

      delete eeic;
  }
}

std::tuple<uint32_t, uint32_t> const WithParamInterfaceDB_e[] = {
  /* 0000 */ std::make_tuple(0x12345678, 0x10000038),
  /* 0001 */ std::make_tuple(0x0000ffff, 0x000803f),
  /* 0002 */ std::make_tuple(0x00000000, 0x00000000),
  /* 0003 */ std::make_tuple(0x000010ff, 0x0000003f),
  /* 0004 */ std::make_tuple(0xff000000, 0x90000000),
  /* 0005 */ std::make_tuple(0xf000ffff, 0x9000803f),
  /* 0006 */ std::make_tuple(0xffffffff, 0x90c0803f),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access_DB, TregEEICAccessTest3,
                        ::testing::ValuesIn(WithParamInterfaceDB_e));



class TregEEICAccessTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregEEICAccessTest4() {}
  virtual ~TregEEICAccessTest4() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;

  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


//EICT =1 --> EIOV is masked, EIRF is masked
TEST_P(TregEEICAccessTest4, CheckEEIC_Access_State) {
    uint32_t temp = std::get<0>(GetParam());
    uint32_t expect = std::get<1>(GetParam());

    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    sideband.SetDM(0);
    for (uint32_t ch = 0; ch < 1; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(0);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata, SIZE_32BIT));

        memcpy (&m_data32, rdata, sizeof(m_data32));
        ASSERT_EQ(expect, m_data32);
        delete eeic;
    }

    uint16_t temp_16 = std::get<0>(GetParam());
    uint16_t expect_16_low = std::get<1>(GetParam());


    memcpy (wdata, &temp_16, sizeof(temp_16));
    uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(0);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_16BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata2, SIZE_16BIT));

        uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
        memcpy (&m_data16, rdata2, sizeof(m_data16));
        ASSERT_EQ(expect_16_low, m_data16[index_16]);

        delete eeic;
    }

    temp_16 = (temp >> 16) & 0xFFFF;
    uint16_t expect_16_high = (expect >> 16) & 0xFFFF;

    memcpy (wdata, &temp_16, sizeof(temp_16));
    uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(0);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_16BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata3, SIZE_16BIT));

        memcpy (&m_data16, rdata3, sizeof(m_data16));
        ASSERT_EQ(expect_16_high, m_data16[0]);
        delete eeic;
      }

    uint8_t temp_8_byte0 = std::get<0>(GetParam());
    uint8_t expect_8_byte0 = std::get<1>(GetParam());

    memcpy (wdata, &temp_8_byte0, sizeof(temp_8_byte0));
    uint8_t rdata4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
       TRegEEIC *eeic;
       eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
       eeic->Reset();

       eeic->SetEICT(1);
       eeic->SetEIRF(0);

       ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_8BIT));
       ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata4, SIZE_8BIT));

       memcpy (&m_data8, rdata4, sizeof(m_data8));
       ASSERT_EQ(expect_8_byte0, m_data8[0]);
       delete eeic;
    }

    uint32_t temp_8_byte1 = std::get<0>(GetParam());
    uint32_t expect_8_byte1 = std::get<1>(GetParam());
    temp_8_byte1 = (temp_8_byte1 >> 8) & 0xFFU;
    expect_8_byte1 = ((expect_8_byte1 >> 8)) & 0xFFU;

    memcpy (wdata, &temp_8_byte1, sizeof(temp_8_byte1));
    uint8_t rdata5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(0);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4 + 1, (uint8_t*)wdata, SIZE_8BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)rdata5, SIZE_8BIT));

        memcpy (&m_data8, rdata5, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte1, m_data8[0]);
        delete eeic;
    }

    uint32_t temp_8_byte2 = std::get<0>(GetParam());
    uint32_t expect_8_byte2 = std::get<1>(GetParam());
    temp_8_byte2 = (temp_8_byte2 >> 16) & 0xFFU;
    expect_8_byte2 = ((expect_8_byte2 >> 16)) & 0xFFU;

    memcpy (wdata, &temp_8_byte2, sizeof(temp_8_byte2));
    uint8_t rdata6[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(0);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_8BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata6, SIZE_8BIT));

        memcpy (&m_data8, rdata6, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte2, m_data8[0]);
        delete eeic;
    }

    uint32_t temp_8_byte3 = std::get<0>(GetParam());
    uint32_t expect_8_byte3 = std::get<1>(GetParam());
    temp_8_byte3 = (temp_8_byte3 >> 24) & 0xFFU;
    expect_8_byte3 = ((expect_8_byte3 >> 24)) & 0xFFU;
    memcpy (wdata, &temp_8_byte3, sizeof(temp_8_byte3));
    uint8_t rdata7[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(0);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4 + 3, (uint8_t*)wdata, SIZE_8BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)rdata7, SIZE_8BIT));

        memcpy (&m_data8, rdata7, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte3, m_data8[0]);

        delete eeic;
    }
}
std::tuple<uint32_t, uint32_t> const WithParamInterface2_e[] = {
  /* 0000 */ std::make_tuple(0x12345678, 0x80000038),
  /* 0001 */ std::make_tuple(0x0000ffff, 0x8000003f),
  /* 0002 */ std::make_tuple(0x00000000, 0x80000000),
  /* 0003 */ std::make_tuple(0x000010ff, 0x8000003f),
  /* 0004 */ std::make_tuple(0xff000000, 0x80000000),
  /* 0005 */ std::make_tuple(0xf000ffff, 0x8000003f),
  /* 0006 */ std::make_tuple(0xffffffff, 0x80c0003f),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access_State, TregEEICAccessTest4,
                        ::testing::ValuesIn(WithParamInterface2_e));

class TregEEICAccessTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregEEICAccessTest5() {}
  virtual ~TregEEICAccessTest5() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

//EICT =1 --> EIOV is masked
TEST_P(TregEEICAccessTest5, CheckEEIC_Access_State) {
    uint32_t temp = std::get<0>(GetParam());
    uint32_t expect = std::get<1>(GetParam());

    uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
    uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    sideband.SetDM(0);
    //for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
    for (uint32_t ch = 0; ch < 1; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(1);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata, SIZE_32BIT));

        memcpy (&m_data32, rdata, sizeof(m_data32));
        ASSERT_EQ(expect, m_data32);
        delete eeic;
    }

    uint16_t temp_16 = std::get<0>(GetParam());
    uint16_t expect_16_low = std::get<1>(GetParam());


    memcpy (wdata, &temp_16, sizeof(temp_16));
    uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(1);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_16BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata2, SIZE_16BIT));

        uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
        memcpy (&m_data16, rdata2, sizeof(m_data16));
        ASSERT_EQ(expect_16_low, m_data16[index_16]);

        delete eeic;
    }

    temp_16 = (temp >> 16) & 0xFFFF;
    uint16_t expect_16_high = (expect >> 16) & 0xFFFF;

    memcpy (wdata, &temp_16, sizeof(temp_16));
    uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(1);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_16BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata3, SIZE_16BIT));

        //uint32_t index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
        memcpy (&m_data16, rdata3, sizeof(m_data16));
        ASSERT_EQ(expect_16_high, m_data16[0]);
        delete eeic;
      }

    uint8_t temp_8_byte0 = std::get<0>(GetParam());
    uint8_t expect_8_byte0 = std::get<1>(GetParam());

    memcpy (wdata, &temp_8_byte0, sizeof(temp_8_byte0));
    uint8_t rdata4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
       TRegEEIC *eeic;
       eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
       eeic->Reset();

       eeic->SetEICT(1);
       eeic->SetEIRF(1);

       ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_8BIT));
       ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4, (uint8_t*)rdata4, SIZE_8BIT));

       memcpy (&m_data8, rdata4, sizeof(m_data8));
       ASSERT_EQ(expect_8_byte0, m_data8[0]);
       delete eeic;
    }

    uint32_t temp_8_byte1 = std::get<0>(GetParam());
    uint32_t expect_8_byte1 = std::get<1>(GetParam());
    temp_8_byte1 = (temp_8_byte1 >> 8) & 0xFFU;
    expect_8_byte1 = ((expect_8_byte1 >> 8)) & 0xFFU;

    memcpy (wdata, &temp_8_byte1, sizeof(temp_8_byte1));
    uint8_t rdata5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(1);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4 + 1, (uint8_t*)wdata, SIZE_8BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)rdata5, SIZE_8BIT));

        memcpy (&m_data8, rdata5, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte1, m_data8[0]);
        delete eeic;
    }

    uint32_t temp_8_byte2 = std::get<0>(GetParam());
    uint32_t expect_8_byte2 = std::get<1>(GetParam());
    temp_8_byte2 = (temp_8_byte2 >> 16) & 0xFFU;
    expect_8_byte2 = ((expect_8_byte2 >> 16)) & 0xFFU;

    memcpy (wdata, &temp_8_byte2, sizeof(temp_8_byte2));
    uint8_t rdata6[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(1);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4 + 2, (uint8_t*)wdata, SIZE_8BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)rdata6, SIZE_8BIT));

        memcpy (&m_data8, rdata6, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte2, m_data8[0]);
        delete eeic;
    }

    uint32_t temp_8_byte3 = std::get<0>(GetParam());
    uint32_t expect_8_byte3 = std::get<1>(GetParam());
    temp_8_byte3 = (temp_8_byte3 >> 24) & 0xFFU;
    expect_8_byte3 = ((expect_8_byte3 >> 24)) & 0xFFU;
    memcpy (wdata, &temp_8_byte3, sizeof(temp_8_byte3));
    uint8_t rdata7[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    intc1->HardReset();
    for (uint32_t ch = 0; ch < TRegIntc1::N_EEIC; ch ++ ){
        TRegEEIC *eeic;
        eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
        eeic->Reset();

        eeic->SetEICT(1);
        eeic->SetEIRF(1);

        ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4 + 3, (uint8_t*)wdata, SIZE_8BIT));
        ASSERT_NO_THROW(eeic->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)rdata7, SIZE_8BIT));

        memcpy (&m_data8, rdata7, sizeof(m_data8));
        ASSERT_EQ(expect_8_byte3, m_data8[0]);

        delete eeic;
    }
}
std::tuple<uint32_t, uint32_t> const WithParamInterface3_e[] = {
  /* 0000 */ std::make_tuple(0x12345678, 0x90000038),
  /* 0001 */ std::make_tuple(0x0000ffff, 0x9000003f),
  /* 0002 */ std::make_tuple(0x00000000, 0x90000000),
  /* 0003 */ std::make_tuple(0x000010ff, 0x9000003f),
  /* 0004 */ std::make_tuple(0xff000000, 0x90000000),
  /* 0005 */ std::make_tuple(0xf000ffff, 0x9000003f),
  /* 0006 */ std::make_tuple(0xffffffff, 0x90c0003f),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access_State, TregEEICAccessTest5,
                        ::testing::ValuesIn(WithParamInterface3_e));

class TregEEIC_Function
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregEEIC_Function() {}
  virtual ~TregEEIC_Function() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);

    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                              delete mock_fsbus;
                              delete mock_pe;}

public:
  MockFsBus* mock_fsbus;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;
  SideBand sideband;

  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0200;
  PhysAddr eibd_addr = 0xFFFC0100;

private:
};

TEST_F(TregEEIC_Function, CheckSetEIRFTest1) {
  uint32_t ch = 0;
  uint32_t data_input = 0x10000000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();
  bool flag = false;

  ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4, wdata, SIZE_32BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);

  delete eeic;
}

TEST_F(TregEEIC_Function, CheckSetEIRFTest1b) {
  uint32_t ch = 0;
  uint32_t data_input = 0x10000000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();
  bool flag = false;

  ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, wdata, SIZE_32BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);

  delete eeic;
}


TEST_F(TregEEIC_Function, CheckSetEIRFTest2) {
  uint32_t ch = 0;
  uint32_t data_input = 0x10000000;
  uint32_t data_input_2 = 0x00000000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();
  bool flag = false;

  ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4, wdata, SIZE_32BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);

  ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4, wdata_2, SIZE_32BIT));
  ASSERT_EQ(true, intc1->m_intc1_queue.empty());

  delete eeic;
}

TEST_F(TregEEIC_Function, CheckSetEIRFTest2b) {
  uint32_t ch = 0;
  uint32_t data_input = 0x10000000;
  uint32_t data_input_2 = 0x00000000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();
  bool flag = false;

  ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, wdata, SIZE_32BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);

  ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, wdata_2, SIZE_32BIT));
  ASSERT_EQ(true, intc1->m_intc1_queue.empty());

  delete eeic;
}


TEST_F(TregEEIC_Function, CheckGetBit_db) {
  uint32_t ch = 0;
  uint32_t data_input = 0xffffffff;
  uint32_t data_input_2 = 0x7fffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();


  ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4, wdata, SIZE_32BIT));

  ASSERT_EQ(0x1U, eeic->GetEICT());
  ASSERT_EQ(0x1U, eeic->GetEIMK());
  ASSERT_EQ(0x3fU, eeic->GetEIP());
  ASSERT_EQ(0x1U, eeic->GetEIRF());
  ASSERT_EQ(0x1U, eeic->GetEITB());

  ASSERT_NO_THROW(eeic->WriteBody(base_addr + ch*4, wdata_2, SIZE_32BIT));

  ASSERT_EQ(0x0U, eeic->GetEICT());
  ASSERT_EQ(0x1U, eeic->GetEIMK());
  ASSERT_EQ(0x3fU, eeic->GetEIP());
  ASSERT_EQ(0x1U, eeic->GetEIRF());
  ASSERT_EQ(0x1U, eeic->GetEITB());

  delete eeic;
}

TEST_F(TregEEIC_Function, CheckGetBit) {
  uint32_t ch = 0;
  uint32_t data_input = 0xffffffff;
  uint32_t data_input_2 = 0x7fffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();


  ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, wdata, SIZE_32BIT));

  ASSERT_EQ(0x0U, eeic->GetEICT());
  ASSERT_EQ(0x1U, eeic->GetEIMK());
  ASSERT_EQ(0x3fU, eeic->GetEIP());
  ASSERT_EQ(0x1U, eeic->GetEIRF());
  ASSERT_EQ(0x1U, eeic->GetEITB());

  ASSERT_NO_THROW(eeic->Write(sideband, base_addr + ch*4, wdata_2, SIZE_32BIT));

  ASSERT_EQ(0x0U, eeic->GetEICT());
  ASSERT_EQ(0x1U, eeic->GetEIMK());
  ASSERT_EQ(0x3fU, eeic->GetEIP());
  ASSERT_EQ(0x1U, eeic->GetEIRF());
  ASSERT_EQ(0x1U, eeic->GetEITB());

  delete eeic;
}

TEST_F(TregEEIC_Function, GetSetEIOV) {
  uint32_t ch = 0;
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();

  ASSERT_NO_THROW(eeic->SetEIOV(1));
  ASSERT_EQ(0x1U, eeic->GetEIOV());

  delete eeic;
}

TEST_F(TregEEIC_Function, GetSetEICT) {
  uint32_t ch = 0;
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();

  ASSERT_NO_THROW(eeic->SetEICT(1));
  ASSERT_EQ(0x1U, eeic->GetEICT());

  delete eeic;
}

TEST_F(TregEEIC_Function, GetSetEIRF) {
  uint32_t ch = 0;
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();

  ASSERT_NO_THROW(eeic->SetEIRF(1));
  ASSERT_EQ(0x1U, eeic->GetEIRF());

  delete eeic;
}

TEST_F(TregEEIC_Function, SetEIPTest) {
  uint32_t ch = 0;
  TRegEEIC *eeic;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eeic->Reset();

  ASSERT_NO_THROW(eeic->SetEIP(0xffff));
  ASSERT_EQ(0x3fU, eeic->GetEIP());

  delete eeic;
}

TEST_F(TregEEIC_Function, IsRegWriteEnable) {
  uint32_t ch = 0;
  TRegEEIC *eeic;
  TRegEIBD *eibd;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eibd = new TRegEIBD (reg_size, min_access_size, eibd_addr+ch*4, TRegMask(TRegBND<EIBD>::INTC1_EIBD_MASK_G4MH20), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  eeic->Reset();
  eibd->Reset();

  intc1->SetIHVEFromIHVCFG(1);

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, eeic->IsRegWriteEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(0);
  ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, !eeic->IsRegWriteEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, !eeic->IsRegWriteEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, !eeic->IsRegWriteEnable(sideband));
  intc1->SetIHVEFromIHVCFG(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, eeic->IsRegWriteEnable(sideband));

  sideband.SetUM(0);
  ASSERT_NO_THROW(eeic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, eeic->IsRegWriteEnable(sideband));

  delete eeic;
  delete eibd;
}

TEST_F(TregEEIC_Function, IsRegReadEnable) {
  uint32_t ch = 0;
  TRegEEIC *eeic;
  TRegEIBD *eibd;
  intc1->HardReset();
  eeic = new TRegEEIC (reg_size, min_access_size, base_addr+ch*4, ch, pe_id, intc1);
  eibd = new TRegEIBD (reg_size, min_access_size, eibd_addr+ch*4, TRegMask(TRegBND<EIBD>::INTC1_EIBD_MASK_G4MH20), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  eeic->Reset();
  eibd->Reset();

  intc1->SetIHVEFromIHVCFG(1);

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_NO_THROW(eeic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, eeic->IsRegReadEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(0);
  ASSERT_NO_THROW(eeic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, !eeic->IsRegReadEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  ASSERT_NO_THROW(eeic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, !eeic->IsRegReadEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(eeic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, !eeic->IsRegReadEnable(sideband));
  intc1->SetIHVEFromIHVCFG(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(eeic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, eeic->IsRegReadEnable(sideband));

  sideband.SetUM(0);
  ASSERT_NO_THROW(eeic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, eeic->IsRegReadEnable(sideband));

  delete eeic;
  delete eibd;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
