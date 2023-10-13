/**
 * @file test_intc_tregihvcfg.cpp
 *
 * @date Created on: 20170731
 * @author Author: YVo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for INTC1. (SAMPLE)
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


class TregIHVCFGAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregIHVCFGAccessTest() {}
  virtual ~TregIHVCFGAccessTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
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

  static const PhysAddr IHVCFG_OFFSET  = 0x0000002F0UL;
  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + IHVCFG_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_GetName) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();

  ASSERT_STREQ("IHVCFG", ihvcfg->GetName());

  delete ihvcfg;
}


TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_SetGetData) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->SetData(0xf);

  ASSERT_EQ(0xfU, ihvcfg->GetData());

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_SetGetIHVE) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->SetIHVE(0xFFFF);

  ASSERT_EQ(0x1U, ihvcfg->GetIHVE());

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_IsRegWriteEnable_1) {
  intc1->HardReset();
  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);

  ihvcfg->SetIHVE(0x1);

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_EQ(true, !ihvcfg->IsRegWriteEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_EQ(true, ihvcfg->IsRegWriteEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(1);
  ASSERT_EQ(true, !ihvcfg->IsRegWriteEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  ASSERT_EQ(true, !ihvcfg->IsRegWriteEnable(sideband));

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_IsRegWriteEnable_2) {
  intc1->HardReset();
  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);

  ihvcfg->SetIHVE(0x0);

  sideband.SetGM(0);        //don't care GM bit
  sideband.SetUM(1);
  ASSERT_EQ(true, ihvcfg->IsRegWriteEnable(sideband));

  sideband.SetGM(0);        //don't care GM bit
  sideband.SetUM(0);
  ASSERT_EQ(true, ihvcfg->IsRegWriteEnable(sideband));

  sideband.SetGM(1);        //don't care GM bit
  sideband.SetUM(1);
  ASSERT_EQ(true, ihvcfg->IsRegWriteEnable(sideband));

  sideband.SetGM(1);        //don't care GM bit
  sideband.SetUM(0);
  ASSERT_EQ(true, ihvcfg->IsRegWriteEnable(sideband));

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_IsRegReadEnable_1) {
  intc1->HardReset();
  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);

  ihvcfg->SetIHVE(0x1);

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_EQ(true, !ihvcfg->IsRegReadEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_EQ(true, ihvcfg->IsRegReadEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(1);
  ASSERT_EQ(true, !ihvcfg->IsRegReadEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  ASSERT_EQ(true, !ihvcfg->IsRegReadEnable(sideband));

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_IsRegReadEnable_2) {
  intc1->HardReset();
  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);

  ihvcfg->SetIHVE(0x0);

  sideband.SetGM(0);        //don't care GM bit
  sideband.SetUM(1);
  ASSERT_EQ(true, ihvcfg->IsRegReadEnable(sideband));

  sideband.SetGM(0);        //don't care GM bit
  sideband.SetUM(0);
  ASSERT_EQ(true, ihvcfg->IsRegReadEnable(sideband));

  sideband.SetGM(1);        //don't care GM bit
  sideband.SetUM(1);
  ASSERT_EQ(true, ihvcfg->IsRegReadEnable(sideband));

  sideband.SetGM(1);        //don't care GM bit
  sideband.SetUM(0);
  ASSERT_EQ(true, ihvcfg->IsRegReadEnable(sideband));

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_initialTest_outrange) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();

  ihvcfg->SetIHVE(0x1);
  ASSERT_THROW(ihvcfg->Write(sideband, base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
  ASSERT_THROW(ihvcfg->Write(sideband, base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

  ASSERT_THROW(ihvcfg->WriteBody(base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
  ASSERT_THROW(ihvcfg->WriteBody(base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_initialTest_32) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();

  ASSERT_NO_THROW(ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT));

  uint32_t expect_data = 0x1;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_initialTest_16) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();

  ASSERT_NO_THROW(ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT));

  uint32_t expect_data = 0x1;
  uint32_t index_16 = (base_addr) & 0x1;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  ASSERT_NO_THROW(ihvcfg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_16BIT));

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_initialTest_8) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();

  ASSERT_NO_THROW(ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT));

  uint32_t expect_data = 0x1;
  uint32_t index_8 = (base_addr) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);


  for (uint32_t i = 1; i <4; i++){
      ASSERT_NO_THROW(ihvcfg->Read(sideband, base_addr + i, (uint8_t*)data, SIZE_8BIT));

      expect_data = 0x0;
      index_8 = (base_addr + i) & 0x3U;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data, m_data8[index_8]);
  }

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest32_HV) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};

  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);

  uint32_t expect_data = 0x1;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest16_HV) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);

  uint8_t wdata[] = {0xff, 0xff};

  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x0U, syserr);

  uint32_t expect_data = 0x1;
  uint32_t index_16 = (base_addr) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  ihvcfg->Reset();
  uint8_t wdata1[] = {0xff, 0xff};
  syserr = ihvcfg->Write(sideband, base_addr+2, (uint8_t*)wdata1, SIZE_16BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest8_HV) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);

  uint8_t wdata[] = {0xff};
  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);

  uint32_t expect_data = 0x1;
  uint32_t index_8 = (base_addr) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  syserr = ihvcfg->Write(sideband, base_addr+1, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);

  syserr = ihvcfg->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 1) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  syserr = ihvcfg->Write(sideband, base_addr+2, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 2) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  syserr = ihvcfg->Write(sideband, base_addr+3, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr+3, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 3) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest128_SV) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);
  sideband.SetGM(1);
  sideband.SetUM(0);
  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};

  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_128BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest64_SV) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);
  sideband.SetGM(1);
  sideband.SetUM(0);
  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};

  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_64BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest32_SV) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);
  sideband.SetGM(1);
  sideband.SetUM(0);
  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};

  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest16_SV) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);

  uint8_t wdata[] = {0xff, 0xff};
  sideband.SetGM(1);
  sideband.SetUM(0);
  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  uint32_t index_16 = (base_addr) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  ihvcfg->Reset();
  uint8_t wdata1[] = {0xff, 0xff};
  syserr = ihvcfg->Write(sideband, base_addr+2, (uint8_t*)wdata1, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest8_SV) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);
  sideband.SetGM(1);
  sideband.SetUM(0);

  uint8_t wdata[] = {0xff};
  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  uint32_t index_8 = (base_addr) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  syserr = ihvcfg->Write(sideband, base_addr+1, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  syserr = ihvcfg->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 1) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  syserr = ihvcfg->Write(sideband, base_addr+2, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 2) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  syserr = ihvcfg->Write(sideband, base_addr+3, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr+3, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 3) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest128_UM) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);
  sideband.SetUM(1);
  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};

  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_128BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest64_UM) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);
  sideband.SetUM(1);
  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};

  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_64BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete ihvcfg;
}


TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest32_UM) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);
  sideband.SetUM(1);
  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};

  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest16_UM) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);

  uint8_t wdata[] = {0xff, 0xff};
  sideband.SetUM(1);
  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  uint32_t index_16 = (base_addr) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  ihvcfg->Reset();
  uint8_t wdata1[] = {0xff, 0xff};
  syserr = ihvcfg->Write(sideband, base_addr+2, (uint8_t*)wdata1, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest8_UM) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);
  sideband.SetUM(1);

  uint8_t wdata[] = {0xff};
  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  uint32_t index_8 = (base_addr) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  syserr = ihvcfg->Write(sideband, base_addr+1, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  syserr = ihvcfg->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 1) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  syserr = ihvcfg->Write(sideband, base_addr+2, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 2) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  syserr = ihvcfg->Write(sideband, base_addr+3, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = ihvcfg->Read(sideband, base_addr+3, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 3) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest32_IHVE0) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();

  ihvcfg->SetIHVE(0x0);
  sideband.SetUM(0);
  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};

  SysErrCause syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);

  ihvcfg->SetIHVE(0x0);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);

  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x0);
  sideband.SetUM(1);
  syserr = ihvcfg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);

  ihvcfg->SetIHVE(0x0);
  syserr = ihvcfg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest32_DB) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
  ASSERT_NO_THROW(ihvcfg->WriteBody(base_addr, (uint8_t*)wdata, SIZE_32BIT));
  ASSERT_NO_THROW(ihvcfg->ReadBody(base_addr, (uint8_t*)data, SIZE_32BIT));

  uint32_t expect_data = 0x1;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest16_DB) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);

  uint8_t wdata[] = {0xff, 0xff};
  ASSERT_NO_THROW(ihvcfg->WriteBody(base_addr, new uint8_t[2] {0xff, 0xff}, SIZE_16BIT));
  ASSERT_NO_THROW(ihvcfg->ReadBody(base_addr, (uint8_t*)data, SIZE_16BIT));

  uint32_t expect_data = 0x1;
  uint32_t index_16 = (base_addr) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  ihvcfg->Reset();
  ASSERT_NO_THROW(ihvcfg->WriteBody(base_addr+2, (uint8_t*)wdata, SIZE_16BIT));
  ASSERT_NO_THROW(ihvcfg->ReadBody(base_addr+2, (uint8_t*)data, SIZE_16BIT));

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete ihvcfg;
}

TEST_F(TregIHVCFGAccessTest, CheckIHVCFG_AccessTest8_DB) {
  intc1->HardReset();

  TRegIHVCFG *ihvcfg;
  ihvcfg = new TRegIHVCFG (reg_size, min_access_size, base_addr, intc1);
  ihvcfg->Reset();
  ihvcfg->SetIHVE(0x1);

  uint8_t wdata[] = {0xff};
  ASSERT_NO_THROW(ihvcfg->WriteBody(base_addr, (uint8_t*)wdata, SIZE_8BIT));
  ASSERT_NO_THROW(ihvcfg->ReadBody(base_addr, (uint8_t*)data, SIZE_8BIT));
  uint32_t expect_data = 0x1;
  uint32_t index_8 = (base_addr) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  ASSERT_NO_THROW(ihvcfg->WriteBody(base_addr+1, (uint8_t*)wdata, SIZE_8BIT));
  ASSERT_NO_THROW(ihvcfg->ReadBody(base_addr+1, (uint8_t*)data, SIZE_8BIT));
  expect_data = 0x0;
  index_8 = (base_addr + 1) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  ASSERT_NO_THROW(ihvcfg->WriteBody(base_addr+2, (uint8_t*)wdata, SIZE_8BIT));
  ASSERT_NO_THROW(ihvcfg->ReadBody(base_addr+2, (uint8_t*)data, SIZE_8BIT));
  expect_data = 0x0;
  index_8 = (base_addr + 2) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  ihvcfg->Reset();
  ASSERT_NO_THROW(ihvcfg->WriteBody(base_addr+3, (uint8_t*)wdata, SIZE_8BIT));
  ASSERT_NO_THROW(ihvcfg->ReadBody(base_addr+3, (uint8_t*)data, SIZE_8BIT));
  expect_data = 0x0;
  index_8 = (base_addr + 3) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);
  delete ihvcfg;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
