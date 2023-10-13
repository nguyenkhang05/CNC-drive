/**
 * @file test_intc_tregdbmk.cpp
 *
 * @date Created on: 2016
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


class TregDBMKAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t,uint16_t>> {
protected:
    TregDBMKAccessTest() {}
  virtual ~TregDBMKAccessTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);

    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x10);
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

  static const PhysAddr DBMK_OFFSET  = 0x000000080UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + DBMK_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_F(TregDBMKAccessTest, CheckDBMK_GetName) {
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  ASSERT_STREQ("DBMK", dbmk->GetName());

  delete dbmk;
}


TEST_F(TregDBMKAccessTest, CheckDBMK_SetGetData) {
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->SetData(0xf);

  ASSERT_EQ(0xfU, dbmk->GetData());

  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_SetGetFN) {
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->SetFN(0x1);

  ASSERT_EQ(0x1U, dbmk->GetFN());

  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_SetGetFI) {
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->SetFI(0x1);

  ASSERT_EQ(0x1U, dbmk->GetFI());

  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_SetGetEI) {
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->SetEI(0x1);

  ASSERT_EQ(0x1U, dbmk->GetEI());

  delete dbmk;
}



TEST_F(TregDBMKAccessTest, CheckDBMK_IsRegWriteEnableTest1) {
  intc1->HardReset();
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->SetEI(0x1);

  sideband.SetDM(1);
  ASSERT_EQ(true, dbmk->IsRegWriteEnable(sideband));

  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_IsRegWriteEnableTest2) {
  intc1->HardReset();
  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->SetEI(0x1);

  sideband.SetDM(0);
  ASSERT_EQ(true, !dbmk->IsRegWriteEnable(sideband));

  delete dbmk;
}


TEST_F(TregDBMKAccessTest, CheckDBMK_initialTest1) {
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  sideband.SetDM(1);
  ASSERT_THROW(dbmk->Write(sideband, base_addr, (uint8_t*)data, SIZE_32BIT), std::range_error);
  ASSERT_THROW(dbmk->Write(sideband, base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
  ASSERT_THROW(dbmk->Write(sideband, base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

  ASSERT_THROW(dbmk->WriteBody(base_addr, (uint8_t*)data, SIZE_32BIT), std::range_error);
  ASSERT_THROW(dbmk->WriteBody(base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
  ASSERT_THROW(dbmk->WriteBody(base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_initialTest2) {
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  ASSERT_NO_THROW(dbmk->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT));

  uint16_t expect_data = 0x0;
  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  ASSERT_NO_THROW(dbmk->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT));

  uint8_t expect_data2 = 0x0;
  uint32_t index_8 = (base_addr+1) & 0x3;
  memcpy (&m_data8[index_8], data, sizeof(m_data8));
  ASSERT_EQ(expect_data2, m_data8[index_8]);

  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_AccessTest1) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  ASSERT_NO_THROW(dbmk->Write(sideband, base_addr, wdata, SIZE_16BIT));
  ASSERT_NO_THROW(dbmk->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT));

  uint16_t expect_data = 0x0;
  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);
  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_AccessTest2) {
  uint16_t data_input = 0xff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  ASSERT_NO_THROW(dbmk->Write(sideband, base_addr, wdata, SIZE_8BIT));
  ASSERT_NO_THROW(dbmk->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT));

  uint8_t expect_data = 0x0;
  uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
  ASSERT_EQ(expect_data, m_rdata);
  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_AccessTest3) {
  uint16_t data_input = 0xff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  ASSERT_NO_THROW(dbmk->Write(sideband, base_addr + 1, wdata, SIZE_8BIT));
  ASSERT_NO_THROW(dbmk->Read(sideband, base_addr + 1, (uint8_t*)data, SIZE_8BIT));

  uint8_t expect_data = 0x0;
  uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
  ASSERT_EQ(expect_data, m_rdata);
  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_AccessTest_DB1) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  sideband.SetDM(1);
  ASSERT_NO_THROW(dbmk->Write(sideband, base_addr, wdata, SIZE_16BIT));
  ASSERT_NO_THROW(dbmk->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT));

  uint16_t expect_data = 0x7;
  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);
  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_AccessTest_DB2) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  sideband.SetDM(1);
  ASSERT_NO_THROW(dbmk->Write(sideband, base_addr, wdata, SIZE_8BIT));
  ASSERT_NO_THROW(dbmk->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT));

  uint8_t expect_data = 0x7;
  uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
  ASSERT_EQ(expect_data, m_rdata);
  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_AccessTest_DB3) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  sideband.SetDM(1);
  ASSERT_NO_THROW(dbmk->Write(sideband, base_addr+1, wdata, SIZE_8BIT));
  ASSERT_NO_THROW(dbmk->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT));

  uint8_t expect_data = 0x0;
  uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
  ASSERT_EQ(expect_data, m_rdata);
  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_WriteBodyTest1) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  sideband.SetDM(1);
  ASSERT_NO_THROW(dbmk->WriteBody(base_addr, wdata, SIZE_16BIT));
  ASSERT_NO_THROW(dbmk->ReadBody(base_addr, (uint8_t*)data, SIZE_16BIT));

  uint16_t expect_data = 0x7;
  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);
  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_WriteBodyTest2) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  ASSERT_NO_THROW(dbmk->WriteBody(base_addr, wdata, SIZE_8BIT));
  ASSERT_NO_THROW(dbmk->ReadBody(base_addr, (uint8_t*)data, SIZE_8BIT));

  uint8_t expect_data = 0x7;
  uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
  ASSERT_EQ(expect_data, m_rdata);
  delete dbmk;
}

TEST_F(TregDBMKAccessTest, CheckDBMK_WriteBodyTest3) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  TRegDBMK *dbmk;
  dbmk = new TRegDBMK (reg_size, min_access_size, base_addr, intc1);
  dbmk->Reset();

  ASSERT_NO_THROW(dbmk->WriteBody(base_addr+1, wdata, SIZE_8BIT));
  ASSERT_NO_THROW(dbmk->ReadBody(base_addr+1, (uint8_t*)data, SIZE_8BIT));

  uint8_t expect_data = 0x0;
  uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
  ASSERT_EQ(expect_data, m_rdata);
  delete dbmk;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
