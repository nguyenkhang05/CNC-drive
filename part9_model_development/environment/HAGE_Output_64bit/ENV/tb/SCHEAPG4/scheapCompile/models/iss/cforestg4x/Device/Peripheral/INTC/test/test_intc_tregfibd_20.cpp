/**
 * @file test_intc_tregfibd_20.cpp
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


class TregFIBDAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFIBDAccessTest() {}
  virtual ~TregFIBDAccessTest() {}
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

  static const PhysAddr FIBD_OFFSET  = 0x00000001C0UL;
  ByteSize reg_size = SIZE_32BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIBD_OFFSET;
  uint8_t data[8] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest1_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest2_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);


      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest3_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4 +2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest4_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest5_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4+1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest6_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4+2, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest7_HV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4+3, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest1_SV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest2_SV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);


      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest3_SV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4 +2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest4_SV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest5_SV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4+1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest6_SV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4+2, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest7_SV) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4+3, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest1_UM) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest2_UM) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);


      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest3_UM) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4 +2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest4_UM) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest5_UM) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4+1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest6_UM) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4+2, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest7_UM) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->Read(sideband, base_addr + ch*4+3, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_GetName) {
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_STREQ("FIBD", fibd->GetName());

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_Access_IHVE1_HV) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      SysErrCause syserr = fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t expect_data = 0x8700;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_Access_IHVE1_SV) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      SysErrCause syserr = fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_Access_IHVE1_SV_64bits) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      SysErrCause syserr = fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_Access_IHVE1_SV_128bits) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  sideband.SetGM(1);
  sideband.SetUM(0);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      SysErrCause syserr = fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_Access_IHVE1_UM) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      SysErrCause syserr = fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_Access_IHVE1_UM_64bits) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      SysErrCause syserr = fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_Access_IHVE1_UM_128bits) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  sideband.SetUM(1);
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      SysErrCause syserr = fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);
      syserr = fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}


TEST_F(TregFIBDAccessTest, CheckFIBD_Access_IHVE0) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      intc1->SetIHVEFromIHVCFG(0);
      sideband.SetUM(0);
      SysErrCause syserr = fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      sideband.SetUM(1);
      syserr = fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = fibd->Read(sideband, base_addr + ch*4, (uint8_t*)data, SIZE_32BIT);
      ASSERT_EQ(0x0U, syserr);

      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);


      delete fibd;
  }
}



TEST_F(TregFIBDAccessTest, IsRegWriteEnable) {
  uint32_t ch = 0;
  TRegFIBD *fibd;
  intc1->HardReset();
  fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_NO_THROW(fibd->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, fibd->IsRegWriteEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(fibd->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, !fibd->IsRegWriteEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  ASSERT_NO_THROW(fibd->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, !fibd->IsRegWriteEnable(sideband));

  sideband.SetGM(0);
  intc1->SetIHVEFromIHVCFG(0);

  sideband.SetUM(0);
  ASSERT_NO_THROW(fibd->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, fibd->IsRegWriteEnable(sideband));

  sideband.SetUM(1);
  ASSERT_NO_THROW(fibd->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, fibd->IsRegWriteEnable(sideband));

  delete fibd;
}

TEST_F(TregFIBDAccessTest, IsRegReadEnable) {
  uint32_t ch = 0;
  TRegFIBD *fibd;
  intc1->HardReset();
  fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  fibd->Reset();
  intc1->SetIHVEFromIHVCFG(1);

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_NO_THROW(fibd->IsRegReadEnable(sideband));
  ASSERT_EQ(true, fibd->IsRegReadEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(fibd->IsRegReadEnable(sideband));
  ASSERT_EQ(true, !fibd->IsRegReadEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  ASSERT_NO_THROW(fibd->IsRegReadEnable(sideband));
  ASSERT_EQ(true, !fibd->IsRegReadEnable(sideband));

  sideband.SetGM(0);
  intc1->SetIHVEFromIHVCFG(0);

  sideband.SetUM(0);
  ASSERT_NO_THROW(fibd->IsRegReadEnable(sideband));
  ASSERT_EQ(true, fibd->IsRegReadEnable(sideband));

  sideband.SetUM(1);
  ASSERT_NO_THROW(fibd->IsRegReadEnable(sideband));
  ASSERT_EQ(true, fibd->IsRegReadEnable(sideband));

  delete fibd;
}


//---------------------------------------------------

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest1_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->ReadBody(base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));

      uint32_t expect_data = 0x0;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest2_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->ReadBody(base_addr + ch*4, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);


      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest3_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->ReadBody(base_addr + ch*4 + 2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*4 +2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest4_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->ReadBody(base_addr + ch*4, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest5_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->ReadBody(base_addr + ch*4+1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest6_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->ReadBody(base_addr + ch*4+2, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_initialTest7_DB) {
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->ReadBody(base_addr + ch*4+3, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data = 0x0;
      uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
      ASSERT_EQ(expect_data, m_rdata);

      delete fibd;
  }
}


TEST_F(TregFIBDAccessTest, CheckFIBD_Access_DB) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_IMR; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_NO_THROW(fibd->WriteBody(base_addr + ch*4, wdata, SIZE_32BIT));
      ASSERT_NO_THROW(fibd->ReadBody(base_addr + ch*4, (uint8_t*)data, SIZE_32BIT));
      uint32_t expect_data = 0x8700;
      memcpy (&m_data32, data, sizeof(m_data32));
      ASSERT_EQ(expect_data, m_data32);

      delete fibd;
  }
}

TEST_F(TregFIBDAccessTest, CheckFIBD_Access_Size) {
  uint32_t data_input = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIBD; ch ++ ){
      TRegFIBD *fibd;
      fibd = new TRegFIBD (reg_size, min_access_size, base_addr+ch*4, TRegMask(TRegBND<FIBD>::FBD_MASK), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
      fibd->Reset();

      ASSERT_THROW(fibd->WriteBody(base_addr + ch*4, wdata, SIZE_64BIT), std::range_error);
      ASSERT_THROW(fibd->WriteBody(base_addr + ch*4, wdata, SIZE_128BIT), std::range_error);

      ASSERT_THROW(fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_64BIT), std::range_error);
      ASSERT_THROW(fibd->Write(sideband, base_addr + ch*4, wdata, SIZE_128BIT), std::range_error);

      delete fibd;
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
