/**
 * @file test_intc_tregeibg.cpp
 *
 * @date Created on: 20170309
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


class TregEIBGAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregEIBGAccessTest() {}
  virtual ~TregEIBGAccessTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);

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

  static const PhysAddr EIBG_OFFSET  = 0x000000280UL;
  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + EIBG_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_F(TregEIBGAccessTest, CheckEIBG_GetName) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();

  ASSERT_STREQ("EIBG", eibg->GetName());

  delete eibg;
}


TEST_F(TregEIBGAccessTest, CheckEIBG_SetGetData) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->SetData(0xf);

  ASSERT_EQ(0xfU, eibg->GetData());

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_SetGetBGPR) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->SetBGPR(0xFFFF);

  ASSERT_EQ(0x3FU, eibg->GetBGPR());

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_IsRegWriteEnable) {
  intc1->HardReset();
  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  intc1->SetIHVEFromIHVCFG(1);

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_EQ(true, eibg->IsRegWriteEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  ASSERT_EQ(true, !eibg->IsRegWriteEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_EQ(true, !eibg->IsRegWriteEnable(sideband));

  intc1->SetIHVEFromIHVCFG(0);
  sideband.SetUM(0);
  ASSERT_EQ(true, !eibg->IsRegWriteEnable(sideband));

  sideband.SetUM(1);
  ASSERT_EQ(true, !eibg->IsRegWriteEnable(sideband));

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_IsRegReadEnable) {
  intc1->HardReset();
  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  intc1->SetIHVEFromIHVCFG(1);

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_EQ(true, eibg->IsRegReadEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  ASSERT_EQ(true, !eibg->IsRegReadEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_EQ(true, !eibg->IsRegWriteEnable(sideband));

  intc1->SetIHVEFromIHVCFG(0);
  sideband.SetUM(0);
  ASSERT_EQ(true, !eibg->IsRegReadEnable(sideband));

  sideband.SetUM(1);
  ASSERT_EQ(true, !eibg->IsRegReadEnable(sideband));

  delete eibg;
}



TEST_F(TregEIBGAccessTest, CheckEIBG_initialTest_outrange) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  sideband.SetDM(1);
  ASSERT_THROW(eibg->Write(sideband, base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
  ASSERT_THROW(eibg->Write(sideband, base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

  ASSERT_THROW(eibg->WriteBody(base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
  ASSERT_THROW(eibg->WriteBody(base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_initialTest_32) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  ASSERT_NO_THROW(eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT));

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_initialTest_16) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  ASSERT_NO_THROW(eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT));

  uint32_t expect_data = 0x0;
  uint32_t index_16 = (base_addr) & 0x1;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  ASSERT_NO_THROW(eibg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_16BIT));

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_initialTest_8) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  for (uint32_t i = 0; i <4; i++){
      ASSERT_NO_THROW(eibg->Read(sideband, base_addr + i, (uint8_t*)data, SIZE_8BIT));

      uint32_t expect_data = 0x0;
      uint32_t index_8 = (base_addr + i) & 0x3U;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data, m_data8[index_8]);
  }

  delete eibg;
}



TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest32_HV) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);

  uint32_t expect_data = 0x3f;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest16_HV) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff};

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x0U, syserr);

  uint32_t expect_data = 0x3f;
  uint32_t index_16 = (base_addr) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  eibg->Reset();
  uint8_t wdata1[] = {0xff, 0xff};
  syserr = eibg->Write(sideband, base_addr+2, (uint8_t*)wdata1, SIZE_16BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = eibg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest8_HV) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff};
  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);

  uint32_t expect_data = 0x3f;
  uint32_t index_8 = (base_addr) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  syserr = eibg->Write(sideband, base_addr+1, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = eibg->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 1) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  syserr = eibg->Write(sideband, base_addr+2, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = eibg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 2) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  syserr = eibg->Write(sideband, base_addr+3, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = eibg->Read(sideband, base_addr+3, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 3) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest128_SV) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
  sideband.SetGM(1);
  sideband.SetUM(0);

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_128BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest64_SV) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
  sideband.SetGM(1);
  sideband.SetUM(0);

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_64BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest32_SV) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
  sideband.SetGM(1);
  sideband.SetUM(0);

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest16_SV) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff};
  sideband.SetGM(1);
  sideband.SetUM(0);

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  uint32_t index_16 = (base_addr) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  eibg->Reset();
  uint8_t wdata1[] = {0xff, 0xff};
  syserr = eibg->Write(sideband, base_addr+2, (uint8_t*)wdata1, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest8_SV) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);
  sideband.SetGM(1);
  sideband.SetUM(0);

  uint8_t wdata[] = {0xff};
  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  uint32_t index_8 = (base_addr) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  syserr = eibg->Write(sideband, base_addr+1, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 1) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  syserr = eibg->Write(sideband, base_addr+2, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 2) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  syserr = eibg->Write(sideband, base_addr+3, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr+3, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 3) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest128_UM) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
  sideband.SetUM(1);

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_128BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest64_UM) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
  sideband.SetUM(1);

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_64BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest32_UM) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
  sideband.SetUM(1);

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest16_UM) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff};
  sideband.SetUM(1);

  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  uint32_t index_16 = (base_addr) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  eibg->Reset();
  uint8_t wdata1[] = {0xff, 0xff};
  syserr = eibg->Write(sideband, base_addr+2, (uint8_t*)wdata1, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_16BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest8_UM) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);
  sideband.SetUM(1);

  uint8_t wdata[] = {0xff};
  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  uint32_t expect_data = 0x0;
  uint32_t index_8 = (base_addr) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  syserr = eibg->Write(sideband, base_addr+1, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 1) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  syserr = eibg->Write(sideband, base_addr+2, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 2) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  syserr = eibg->Write(sideband, base_addr+3, (uint8_t*)wdata, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);
  syserr = eibg->Read(sideband, base_addr+3, (uint8_t*)data, SIZE_8BIT);
  ASSERT_EQ(0x19U, syserr);

  expect_data = 0x0;
  index_8 = (base_addr + 3) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest32_IHVE0) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(0);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
  sideband.SetUM(1);
  SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);

  uint32_t expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);

  sideband.SetUM(0);
  syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);
  syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT);
  ASSERT_EQ(0x0U, syserr);

  expect_data = 0x0;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);

  delete eibg;
}


TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest32_DB) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
  ASSERT_NO_THROW(eibg->WriteBody(base_addr, (uint8_t*)wdata, SIZE_32BIT));
  ASSERT_NO_THROW(eibg->ReadBody(base_addr, (uint8_t*)data, SIZE_32BIT));

  uint32_t expect_data = 0x3f;
  memcpy (&m_data32, data, sizeof(m_data32));
  ASSERT_EQ(expect_data, m_data32);
  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest16_DB) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff, 0xff};
  ASSERT_NO_THROW(eibg->WriteBody(base_addr, new uint8_t[2] {0xff, 0xff}, SIZE_16BIT));
  ASSERT_NO_THROW(eibg->ReadBody(base_addr, (uint8_t*)data, SIZE_16BIT));

  uint32_t expect_data = 0x3f;
  uint32_t index_16 = (base_addr) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  eibg->Reset();
  ASSERT_NO_THROW(eibg->WriteBody(base_addr+2, (uint8_t*)wdata, SIZE_16BIT));
  ASSERT_NO_THROW(eibg->ReadBody(base_addr+2, (uint8_t*)data, SIZE_16BIT));

  expect_data = 0x0;
  index_16 = (base_addr + 2) & 0x1U;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  delete eibg;
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest8_DB) {
  intc1->HardReset();

  TRegEIBG *eibg;
  eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC1),TRegIntc1::N_G4MH20_EIBG,intc1);
  eibg->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  uint8_t wdata[] = {0xff};
  ASSERT_NO_THROW(eibg->WriteBody(base_addr, (uint8_t*)wdata, SIZE_8BIT));
  ASSERT_NO_THROW(eibg->ReadBody(base_addr, (uint8_t*)data, SIZE_8BIT));
  uint32_t expect_data = 0x3f;
  uint32_t index_8 = (base_addr) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  ASSERT_NO_THROW(eibg->WriteBody(base_addr+1, (uint8_t*)wdata, SIZE_8BIT));
  ASSERT_NO_THROW(eibg->ReadBody(base_addr+1, (uint8_t*)data, SIZE_8BIT));
  expect_data = 0x0;
  index_8 = (base_addr + 1) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  ASSERT_NO_THROW(eibg->WriteBody(base_addr+2, (uint8_t*)wdata, SIZE_8BIT));
  ASSERT_NO_THROW(eibg->ReadBody(base_addr+2, (uint8_t*)data, SIZE_8BIT));
  expect_data = 0x0;
  index_8 = (base_addr + 2) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);

  eibg->Reset();
  ASSERT_NO_THROW(eibg->WriteBody(base_addr+3, (uint8_t*)wdata, SIZE_8BIT));
  ASSERT_NO_THROW(eibg->ReadBody(base_addr+3, (uint8_t*)data, SIZE_8BIT));
  expect_data = 0x0;
  index_8 = (base_addr + 3) & 0x3U;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data, m_data8[index_8]);
  delete eibg;
}



int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
