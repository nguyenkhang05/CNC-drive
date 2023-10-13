/**
 * @file test_intc_tregimr0.cpp
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


TEST_F(TregIMR0AccessTest, CheckIMR_initialTest1) {
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

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest2) {
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

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest3) {
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

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest4) {
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

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest5) {
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

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest6) {
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

TEST_F(TregIMR0AccessTest, CheckIMR_initialTest7) {
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



int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
