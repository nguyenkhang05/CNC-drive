/**
 * @file test_intc_tregimr0_20.cpp
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


class TregIMR0AccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, uint32_t>> {
protected:
    TregIMR0AccessTest() {}
  virtual ~TregIMR0AccessTest() {}
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

  static const PhysAddr IMR_OFFSET  = 0x00000000F0UL;
  ByteSize reg_size = SIZE_32BIT;
  ByteSize reg_size_eic = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + IMR_OFFSET;
  PhysAddr base_addr_eic = 0xFFFC0000;
  uint8_t data[8] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_F(TregIMR0AccessTest, CheckIMR_initialTest1_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0xFFFFFFFF;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest2_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0xFFFF;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest3_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0xFFFF;
      uint32_t index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest4_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest5_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest6_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest7_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}


TEST_F(TregIMR0AccessTest, CheckIMR_initialTest1_SV) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0xFFFFFFFF;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest2_SV) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0xFFFF;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest3_SV) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0xFFFF;
      uint32_t index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest4_SV) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest5_SV) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest6_SV) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}


TEST_F(TregIMR0AccessTest, CheckIMR_initialTest7_SV) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest1_SV_DIFF_GPID) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0xFFFFFFFF;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest2_SV_DIFF_GPID) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0xFFFF;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest3_SV_DIFF_GPID) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0xFFFF;
      uint32_t index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest4_SV_DIFF_GPID) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest5_SV_DIFF_GPID) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest6_SV_DIFF_GPID) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}


TEST_F(TregIMR0AccessTest, CheckIMR_initialTest7_SV_DIFF_GPID) {
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest1_UM) {
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest2_UM) {
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest3_UM) {
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest4_UM) {
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest5_UM) {
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest6_UM) {
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}


TEST_F(TregIMR0AccessTest, CheckIMR_initialTest7_UM) {
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Read(sideband, base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_GetName) {
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_STREQ("IMR", imr->GetName());

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access) {
  uint32_t data_input = 0xfffffffe;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint32_t data_input_2 = 0x00000001;
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->Write(sideband, base_addr + ch*4, wdata, SIZE_32BIT));

      TRegEIC* eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x0U, eic->GetEIMK());

      TRegEEIC* eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x0U, eeic->GetEIMK());

      eic = intc1->GetEICPtr (1);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (1);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      ASSERT_NO_THROW(imr->Write(sideband, base_addr + ch*4, wdata_2, SIZE_32BIT));

      for (uint32_t i = 31; i > 0; i-- ){
          eic = intc1->GetEICPtr (i);
          ASSERT_EQ(0x0U, eic->GetEIMK());

          eeic = intc1->GetEEICPtr (i);
          ASSERT_EQ(0x0U, eeic->GetEIMK());
      }

      eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access_GM1_GPID0) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      sideband.SetGM(1);
      uint8_t wdata[] = {0xfe, 0xff, 0xff, 0xff};
      ASSERT_NO_THROW(imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT));

      TRegEIC* eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      TRegEEIC* eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      eic = intc1->GetEICPtr (1);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (1);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      uint8_t wdata1[] = {0x01, 0x00, 0x00, 0x00};
      ASSERT_NO_THROW(imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata1, SIZE_32BIT));

      for (uint32_t i = 31; i > 0; i-- ){
          eic = intc1->GetEICPtr (i);
          ASSERT_EQ(0x1U, eic->GetEIMK());

          eeic = intc1->GetEEICPtr (i);
          ASSERT_EQ(0x1U, eeic->GetEIMK());
      }

      eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access_GM1_GPID1) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      sideband.SetGM(1);
      sideband.SetGPID(1);
      uint8_t wdata[] = {0xfe, 0xff, 0xff, 0xff};
      ASSERT_NO_THROW(imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT));

      TRegEIC* eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      TRegEEIC* eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      eic = intc1->GetEICPtr (1);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (1);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      uint8_t wdata1[] = {0xfe, 0xff, 0xff, 0xff};
      ASSERT_NO_THROW(imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata1, SIZE_32BIT));

      for (uint32_t i = 31; i > 0; i-- ){
          eic = intc1->GetEICPtr (i);
          ASSERT_EQ(0x1U, eic->GetEIMK());

          eeic = intc1->GetEEICPtr (i);
          ASSERT_EQ(0x1U, eeic->GetEIMK());
      }

      eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access_UM) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      sideband.SetUM(1);
      uint8_t wdata[] = {0xfe, 0xff, 0xff, 0xff};
      ASSERT_NO_THROW(imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT));

      TRegEIC* eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      TRegEEIC* eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      eic = intc1->GetEICPtr (1);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (1);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      uint8_t wdata1[] = {0xfe, 0xff, 0xff, 0xff};
      ASSERT_NO_THROW(imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata1, SIZE_32BIT));

      for (uint32_t i = 31; i > 0; i-- ){
          eic = intc1->GetEICPtr (i);
          ASSERT_EQ(0x1U, eic->GetEIMK());

          eeic = intc1->GetEEICPtr (i);
          ASSERT_EQ(0x1U, eeic->GetEIMK());
      }

      eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access_return) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);

      sideband.SetUM(1);
      uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
      SysErrCause syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);


      uint8_t wdata1[] = {0x0, 0x0, 0x0, 0x0};
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata1, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);


      sideband.SetUM(0);
      sideband.SetGM(1);
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);


      uint8_t wdata2[] = {0x0, 0x0, 0x0, 0x0};
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata2, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      sideband.SetUM(0);
      sideband.SetGM(1);
      sideband.SetGPID(1);
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      uint8_t wdata3[] = {0x0, 0x0, 0x0, 0x0};
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata3, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      sideband.SetUM(0);
      sideband.SetGM(0);
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      uint8_t wdata4[] = {0x0, 0x0, 0x0, 0x0};
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata4, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      intc1->SetIHVEFromIHVCFG(0);
      sideband.SetUM(1);
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      sideband.SetUM(0);
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);


      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access_return_64bits) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);

      sideband.SetUM(1);
      uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
      SysErrCause syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);


      uint8_t wdata1[] = {0x0, 0x0, 0x0, 0x0};
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata1, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access_return_128bits) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);

      sideband.SetUM(1);
      uint8_t wdata[] = {0xff, 0xff, 0xff, 0xff};
      SysErrCause syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);


      uint8_t wdata1[] = {0x0, 0x0, 0x0, 0x0};
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata1, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      delete imr;
  }
}


//-----------------------------------

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest1_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->ReadBody(base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0xFFFFFFFF;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest2_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->ReadBody(base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0xFFFF;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest3_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->ReadBody(base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0xFFFF;
      uint32_t index_16 = ((base_addr + ch*4 + 2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest4_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->ReadBody(base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest5_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->ReadBody(base_addr + ch*4 + 1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest6_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->ReadBody(base_addr + ch*4 + 2, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest7_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_NO_THROW(imr->ReadBody(base_addr + ch*4 + 3, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0xFF;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access_DB) {
  uint32_t data_input = 0xfffffffe;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint32_t data_input_2 = 0x00000001;
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();


      ASSERT_NO_THROW(imr->WriteBody(base_addr + ch*4, wdata, SIZE_32BIT));

      TRegEIC* eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x0U, eic->GetEIMK());

      TRegEEIC* eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x0U, eeic->GetEIMK());

      eic = intc1->GetEICPtr (1);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (1);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      ASSERT_NO_THROW(imr->WriteBody(base_addr + ch*4, wdata_2, SIZE_32BIT));

      for (uint32_t i = 31; i > 0; i-- ){
          eic = intc1->GetEICPtr (i);
          ASSERT_EQ(0x0U, eic->GetEIMK());

          eeic = intc1->GetEEICPtr (i);
          ASSERT_EQ(0x0U, eeic->GetEIMK());
      }

      eic = intc1->GetEICPtr (0);
      ASSERT_EQ(0x1U, eic->GetEIMK());

      eeic = intc1->GetEEICPtr (0);
      ASSERT_EQ(0x1U, eeic->GetEIMK());

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access_Size) {
  uint32_t data_input = 0xfffffffe;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint32_t data_input_2 = 0x00000000;
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      ASSERT_THROW(imr->Write(sideband, base_addr + ch*4, wdata, SIZE_64BIT), std::range_error);
      ASSERT_THROW(imr->Write(sideband, base_addr + ch*4, wdata, SIZE_128BIT), std::range_error);

      ASSERT_THROW(imr->WriteBody(base_addr + ch*4, wdata_2, SIZE_64BIT), std::range_error);
      ASSERT_THROW(imr->WriteBody(base_addr + ch*4, wdata_2, SIZE_128BIT), std::range_error);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, CheckIMR_Access_return_IHVE0) {
  intc1->HardReset();
  intc1->SetIHVEFromIHVCFG(0);

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      sideband.SetUM(1);
      uint8_t wdata[] = {0xfe, 0xff, 0xff, 0xff};
      SysErrCause syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      sideband.SetUM(0);
      syserr = imr->Write(sideband, base_addr + ch*4, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = imr->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, Check_IsRegWriteEnable) {
  intc1->HardReset();
  intc1->SetIHVEFromIHVCFG(0);

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      intc1->SetIHVEFromIHVCFG(1);

      sideband.SetGM(0);
      sideband.SetUM(0);
      ASSERT_NO_THROW(imr->IsRegWriteEnable(sideband));
      ASSERT_EQ(true, imr->IsRegWriteEnable(sideband));

      sideband.SetGM(0);
      sideband.SetUM(1);
      ASSERT_NO_THROW(imr->IsRegWriteEnable(sideband));
      ASSERT_EQ(true, !imr->IsRegWriteEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      ASSERT_NO_THROW(imr->IsRegWriteEnable(sideband));
      ASSERT_EQ(true, imr->IsRegWriteEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      ASSERT_NO_THROW(imr->IsRegWriteEnable(sideband));
      ASSERT_EQ(true, imr->IsRegWriteEnable(sideband));

      sideband.SetGM(0);
      sideband.SetUM(0);
      sideband.SetGPID(0);
      intc1->SetIHVEFromIHVCFG(0);

      sideband.SetUM(0);
      ASSERT_NO_THROW(imr->IsRegWriteEnable(sideband));
      ASSERT_EQ(true, imr->IsRegWriteEnable(sideband));

      sideband.SetUM(1);
      ASSERT_NO_THROW(imr->IsRegWriteEnable(sideband));
      ASSERT_EQ(true, imr->IsRegWriteEnable(sideband));

      delete imr;
  }
}

TEST_F(TregIMR0AccessTest, Check_IsRegReadEnable) {
  intc1->HardReset();
  intc1->SetIHVEFromIHVCFG(0);

  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegIMR *imr;
      imr = new TRegIMR (reg_size, min_access_size, base_addr+ch*4, ch, intc1);
      imr->Reset();

      intc1->SetIHVEFromIHVCFG(1);

      sideband.SetGM(0);
      sideband.SetUM(0);
      ASSERT_NO_THROW(imr->IsRegReadEnable(sideband));
      ASSERT_EQ(true, imr->IsRegReadEnable(sideband));

      sideband.SetGM(0);
      sideband.SetUM(1);
      ASSERT_NO_THROW(imr->IsRegReadEnable(sideband));
      ASSERT_EQ(true, !imr->IsRegReadEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      ASSERT_NO_THROW(imr->IsRegReadEnable(sideband));
      ASSERT_EQ(true, imr->IsRegReadEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      ASSERT_NO_THROW(imr->IsRegReadEnable(sideband));
      ASSERT_EQ(true, imr->IsRegReadEnable(sideband));

      sideband.SetGM(0);
      sideband.SetUM(0);
      sideband.SetGPID(0);
      intc1->SetIHVEFromIHVCFG(0);

      sideband.SetUM(0);
      ASSERT_NO_THROW(imr->IsRegReadEnable(sideband));
      ASSERT_EQ(true, imr->IsRegReadEnable(sideband));

      sideband.SetUM(1);
      ASSERT_NO_THROW(imr->IsRegReadEnable(sideband));
      ASSERT_EQ(true, imr->IsRegReadEnable(sideband));

      delete imr;
  }
}
int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
