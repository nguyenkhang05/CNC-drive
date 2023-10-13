/**
 * @file test_intc2_tregeibg.cpp
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
#include "mock_intc1.h"
#include "intc2.h"
#include "forest_common.h"
#include "mock_proc_element.h"
#include "mock_sideband.h"
#include "mock_fsbus.h"
#include "gmock/gmock.h"

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
    SideBand sideband;
    MockProcElement *mock_pe;
    MockFsBus *m_mock_fsbus;
    PeVec m_pes;
    CIntc2 *cintc2;
    TRegEEIC* eeic;
    uint32_t pe_id = 0;
    PhysAddr base_addr = 0xFFF80000UL +0x1FE0 ;
    uint32_t eibg_num = TRegIntc2::N_G4MH20_EIBG;
    ByteSize reg_size = SIZE_32BIT;
    ByteSize min_access_size = SIZE_8BIT;
    uint8_t data[16] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    union {
        uint32_t m_data32;
        uint16_t m_data16[2];
        uint8_t  m_data8[4];
    };

  private:
  };


TEST_F(TregEIBGAccessTest, CheckEIBG_GetName) {
  cintc2->HardReset();

  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);
      eibg->Reset();

      ASSERT_STREQ("EIBG", eibg->GetName());

      delete eibg;
  }
}


TEST_F(TregEIBGAccessTest, CheckEIBG_SetGetData) {
  cintc2->HardReset();

  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);
      eibg->SetData(0xf);

      ASSERT_EQ(0xfU, eibg->GetData());

      delete eibg;
  }
}

TEST_F(TregEIBGAccessTest, CheckEIBG_SetGetBGPR) {
  cintc2->HardReset();

  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);
      eibg->SetBGPR(0xFFFF);

      ASSERT_EQ(0x3FU, eibg->GetBGPR());

      delete eibg;
  }
}

TEST_F(TregEIBGAccessTest, CheckEIBG_IsRegWriteEnable) {
  cintc2->HardReset();
  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);

      sideband.SetGM(0);
      sideband.SetUM(0);
      ASSERT_EQ(true, eibg->IsRegWriteEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      ASSERT_EQ(true, eibg->IsRegWriteEnable(sideband));

      sideband.SetGM(0);
      sideband.SetUM(1);
      ASSERT_EQ(true, eibg->IsRegWriteEnable(sideband));

     delete eibg;
  }
}

TEST_F(TregEIBGAccessTest, CheckEIBG_IsRegReadEnable) {
  cintc2->HardReset();
  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);

      sideband.SetGM(0);
      sideband.SetUM(0);
      ASSERT_EQ(true, eibg->IsRegReadEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      ASSERT_EQ(true, eibg->IsRegReadEnable(sideband));

      sideband.SetGM(0);
      sideband.SetUM(1);
      ASSERT_EQ(true, eibg->IsRegWriteEnable(sideband));

      delete eibg;
  }
}



TEST_F(TregEIBGAccessTest, CheckEIBG_initialTest_outrange) {
  cintc2->HardReset();

  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);
      eibg->Reset();

      sideband.SetDM(1);
      ASSERT_THROW(eibg->Write(sideband, base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
      ASSERT_THROW(eibg->Write(sideband, base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

      ASSERT_THROW(eibg->WriteBody(base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
      ASSERT_THROW(eibg->WriteBody(base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);
      delete eibg;
  }
}

TEST_F(TregEIBGAccessTest, CheckEIBG_initialTest_32) {
  cintc2->HardReset();

  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);
      eibg->Reset();

      ASSERT_NO_THROW(eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete eibg;
  }
}

TEST_F(TregEIBGAccessTest, CheckEIBG_initialTest_16) {
  cintc2->HardReset();

  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);
      eibg->Reset();

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
}

TEST_F(TregEIBGAccessTest, CheckEIBG_initialTest_8) {
  cintc2->HardReset();

  for (uint32_t k = 0; k< eibg_num; k++){
      TRegEIBG *eibg;
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),k,cintc2);
      eibg->Reset();

      for (uint32_t i = 0; i <1; i++){
            ASSERT_NO_THROW(eibg->Read(sideband, base_addr + i, (uint8_t*)data, SIZE_8BIT));

            uint32_t expect_data = 0x0;
            memcpy (&m_data8[0], data, sizeof(m_data8));
            ASSERT_EQ(expect_data, m_data8[0]);

            ASSERT_NO_THROW(eibg->Read(sideband, base_addr + i, (uint8_t*)data, SIZE_8BIT));


        }
      delete eibg;
  }
}



TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest32_HV) {
  cintc2->HardReset();

  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);
      eibg->Reset();

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
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest16_HV) {
  cintc2->HardReset();

  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);
      eibg->Reset();

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
}

TEST_F(TregEIBGAccessTest, CheckEIBG_AccessTest8_HV) {
  cintc2->HardReset();

  TRegEIBG *eibg;
  for (uint32_t i = 0; i< eibg_num; i++){
      eibg = new TRegEIBG (reg_size, min_access_size, base_addr, TRegMask (TRegEIBG::EIBG_MASK_INTC2),i,cintc2);
      eibg->Reset();

      uint8_t wdata[] = {0xff};
      SysErrCause syserr = eibg->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = eibg->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t expect_data = 0x3f;
      uint32_t index_8 = (base_addr) & 0x3U;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data, m_data8[index_8]);

      memcpy (&m_data32, data, sizeof(m_data32));

      eibg->Reset();
      syserr = eibg->Write(sideband, base_addr+1, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = eibg->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      expect_data = 0x0;
      memcpy (&m_data8[0], data, sizeof(m_data8));
      ASSERT_EQ(expect_data, m_data8[0]);

      eibg->Reset();
      syserr = eibg->Write(sideband, base_addr+2, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = eibg->Read(sideband, base_addr+2, (uint8_t*)data, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      expect_data = 0x0;
      memcpy (&m_data8[0], data, sizeof(m_data8));
      ASSERT_EQ(expect_data, m_data8[0]);

      eibg->Reset();
      syserr = eibg->Write(sideband, base_addr+3, (uint8_t*)wdata, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = eibg->Read(sideband, base_addr+3, (uint8_t*)data, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      expect_data = 0x0;
      memcpy (&m_data8[0], data, sizeof(m_data8));
      ASSERT_EQ(expect_data, m_data8[0]);
      delete eibg;
  }
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
