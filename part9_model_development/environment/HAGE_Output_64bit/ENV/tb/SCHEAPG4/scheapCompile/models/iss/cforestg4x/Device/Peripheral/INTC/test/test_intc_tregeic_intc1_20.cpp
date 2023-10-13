/**
 * @file test_intc_tregeic_intc1_20.cpp
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


class TregEICAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEICAccessTest() {}
  virtual ~TregEICAccessTest() {}
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

  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_F(TregEICAccessTest, CheckEIC_initial_HV) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x008F;
      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_low = 0x8F;
      uint32_t index_8 = (base_addr + ch*2) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_low, m_data8[index_8]);

      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2 +1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data_high = 0x0;
      index_8 = (base_addr + ch*2 + 1) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_high, m_data8[index_8]);

      delete eic;
  }
}

TEST_F(TregEICAccessTest, CheckEIC_initial_SV_DIFF_GM) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_low = 0x0;
      uint32_t index_8 = (base_addr + ch*2) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_low, m_data8[index_8]);

      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2 +1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data_high = 0x0;
      index_8 = (base_addr + ch*2 + 1) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_high, m_data8[index_8]);

      delete eic;
  }
}

TEST_F(TregEICAccessTest, CheckEIC_initial_SV_DIFF_GPID) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_low = 0x0;
      uint32_t index_8 = (base_addr + ch*2) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_low, m_data8[index_8]);

      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2 +1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data_high = 0x0;
      index_8 = (base_addr + ch*2 + 1) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_high, m_data8[index_8]);

      delete eic;
  }
}

TEST_F(TregEICAccessTest, CheckEIC_initial_UM) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetUM(1);
      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_16BIT));

      uint16_t expect_data = 0x0;
      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);

      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_8BIT));
      uint8_t expect_data_low = 0x0;
      uint32_t index_8 = (base_addr + ch*2) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_low, m_data8[index_8]);

      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2 +1, (uint8_t*)data, SIZE_8BIT));

      uint8_t expect_data_high = 0x0;
      index_8 = (base_addr + ch*2 + 1) & 0x3;
      memcpy (&m_data8[index_8], data, sizeof(m_data8));
      ASSERT_EQ(expect_data_high, m_data8[index_8]);

      delete eic;
  }
}


TEST_F(TregEICAccessTest, CheckEIC_GetName) {
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      ASSERT_STREQ("EIC", eic->GetName());

      delete eic;
  }
}

TEST_F(TregEICAccessTest, CheckEIC_Access) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();
      ASSERT_THROW(eic->Write(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_32BIT), std::range_error);
      ASSERT_THROW(eic->Write(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_64BIT), std::range_error);
      ASSERT_THROW(eic->Write(sideband, base_addr + ch*2, (uint8_t*)data, SIZE_128BIT), std::range_error);

      delete eic;
  }
}

TEST_F(TregEICAccessTest, CheckEIC_AccessTest2) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();
      ASSERT_THROW(eic->WriteBody(base_addr + ch*2, (uint8_t*)data, SIZE_32BIT), std::range_error);
      ASSERT_THROW(eic->WriteBody(base_addr + ch*2, (uint8_t*)data, SIZE_64BIT), std::range_error);
      ASSERT_THROW(eic->WriteBody(base_addr + ch*2, (uint8_t*)data, SIZE_128BIT), std::range_error);

      delete eic;
  }
}


class TregEICAccessTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEICAccessTest2() {}
  virtual ~TregEICAccessTest2() {}
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

  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEICAccessTest2, CheckEIC_Access_1_HV) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);


      delete eic;
  }
}

TEST_P(TregEICAccessTest2, CheckEIC_Access_2_HV) {
  uint8_t temp2 = std::get<0>(GetParam());
  uint8_t expect2 = std::get<1>(GetParam());

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata2, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata2, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

        uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
        ASSERT_EQ(expect2, m_rdata2);

        delete eic;
    }
}

TEST_P(TregEICAccessTest2, CheckEIC_Access_3_HV) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp3 = (uint8_t (temp >> 8));
  uint8_t expect3 = (uint8_t (expect >>8));

  uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&temp3);
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2 + 1, (uint8_t*)wdata3, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2 + 1, (uint8_t*)rdata3, SIZE_8BIT);
      ASSERT_EQ(0x0U, syserr);

         uint8_t m_rdata3 = (*(reinterpret_cast<uint8_t*> (rdata3)));
         ASSERT_EQ(expect3, m_rdata3);

         delete eic;
      }
}

TEST_P(TregEICAccessTest2, CheckEIC_Access_IHVE0) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp3 = (uint8_t (temp >> 8));

  uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&temp3);
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->SetIHVEFromIHVCFG(0);

    for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetUM(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata3, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata3, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      sideband.SetUM(0);
      syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata3, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);
      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata3, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      delete eic;
      }
}


std::tuple<uint16_t, uint16_t> const WithParamInterface_HV[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x10ef),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x10ff, 0x10ef),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEICAccessTest2,
                        ::testing::ValuesIn(WithParamInterface_HV));

class TregEICAccessTest2_SV
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEICAccessTest2_SV() {}
  virtual ~TregEICAccessTest2_SV() {}
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

  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEICAccessTest2_SV, CheckEIC_Access_1_SV_DIFF_GM) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr =  eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);


      delete eic;
  }
}

TEST_P(TregEICAccessTest2_SV, CheckEIC_Access_2_SV_DIFF_GM) {
  uint8_t temp2 = std::get<0>(GetParam());
  uint8_t expect2 = 0;

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

        sideband.SetGM(1);
        sideband.SetUM(0);
        SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata2, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr =  eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata2, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
        ASSERT_EQ(expect2, m_rdata2);

        delete eic;
    }
}

TEST_P(TregEICAccessTest2_SV, CheckEIC_Access_3_SV_DIFF_GM) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp3 = (uint8_t (temp >> 8));
  uint8_t expect3 = 0;

  uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&temp3);
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2 + 1, (uint8_t*)wdata3, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2 + 1, (uint8_t*)rdata3, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      uint8_t m_rdata3 = (*(reinterpret_cast<uint8_t*> (rdata3)));
      ASSERT_EQ(expect3, m_rdata3);

      delete eic;
      }
}

TEST_P(TregEICAccessTest2_SV, CheckEIC_Access_1_SV_DIFF_GM_32bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr =  eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete eic;
  }
}

TEST_P(TregEICAccessTest2_SV, CheckEIC_Access_1_SV_DIFF_GM_64bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr =  eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete eic;
  }
}

TEST_P(TregEICAccessTest2_SV, CheckEIC_Access_1_SV_DIFF_GM_128bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0};

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr =  eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete eic;
  }
}


std::tuple<uint16_t, uint16_t> const WithParamInterface_SV[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1fef),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x10ff, 0x10ef),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEICAccessTest2_SV,
                        ::testing::ValuesIn(WithParamInterface_SV));

class TregEICAccessTest2_SV_DIFF_GPID
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEICAccessTest2_SV_DIFF_GPID() {}
  virtual ~TregEICAccessTest2_SV_DIFF_GPID() {}
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

  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEICAccessTest2_SV_DIFF_GPID, CheckEIC_Access_1_SV_DIFF_GPID) {
  uint16_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
  }
}

TEST_P(TregEICAccessTest2_SV_DIFF_GPID, CheckEIC_Access_2_SV_DIFF_GPID) {
  uint8_t temp2 = std::get<0>(GetParam());

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata2, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata2, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
    }
}

TEST_P(TregEICAccessTest2_SV_DIFF_GPID, CheckEIC_Access_3_SV_DIFF_GPID) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp3 = (uint8_t (temp >> 8));

  uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&temp3);
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2 + 1, (uint8_t*)wdata3, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2 + 1, (uint8_t*)rdata3, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);
      delete eic;
      }
}

TEST_P(TregEICAccessTest2_SV_DIFF_GPID, CheckEIC_Access_1_SV_DIFF_GPID_32bits) {
  uint16_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
  }
}

TEST_P(TregEICAccessTest2_SV_DIFF_GPID, CheckEIC_Access_1_SV_DIFF_GPID_64bits) {
  uint16_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
  }
}

TEST_P(TregEICAccessTest2_SV_DIFF_GPID, CheckEIC_Access_1_SV_DIFF_GPID_128bits) {
  uint16_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
  }
}



std::tuple<uint16_t, uint16_t> const WithParamInterface_SV_DIFF_GPID[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x0),
  /* 0001 */ std::make_tuple(0x0000, 0x0),
  /* 0002 */ std::make_tuple(0x10ff, 0x0),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEICAccessTest2_SV_DIFF_GPID,
                        ::testing::ValuesIn(WithParamInterface_SV_DIFF_GPID));

class TregEICAccessTest2_UM
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEICAccessTest2_UM() {}
  virtual ~TregEICAccessTest2_UM() {}
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

  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEICAccessTest2_UM, CheckEIC_Access_1_UM) {
  uint16_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetUM(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
  }
}

TEST_P(TregEICAccessTest2_UM, CheckEIC_Access_2_UM) {
  uint8_t temp2 = std::get<0>(GetParam());

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetUM(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata2, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata2, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
    }
}

TEST_P(TregEICAccessTest2_UM, CheckEIC_Access_3_UM) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp3 = (uint8_t (temp >> 8));

  uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&temp3);
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetUM(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2 + 1, (uint8_t*)wdata3, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2 + 1, (uint8_t*)rdata3, SIZE_8BIT);
      ASSERT_EQ(0x19U, syserr);
      delete eic;
      }
}

TEST_P(TregEICAccessTest2_UM, CheckEIC_Access_1_UM_32bits) {
  uint16_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetUM(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
  }
}

TEST_P(TregEICAccessTest2_UM, CheckEIC_Access_1_UM_64bits) {
  uint16_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetUM(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
  }
}

TEST_P(TregEICAccessTest2_UM, CheckEIC_Access_1_UM_128bits) {
  uint16_t temp = std::get<0>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      sideband.SetUM(1);
      SysErrCause syserr = eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      delete eic;
  }
}


std::tuple<uint16_t, uint16_t> const WithParamInterface_UM[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x0),
  /* 0001 */ std::make_tuple(0x0000, 0x0),
  /* 0002 */ std::make_tuple(0x10ff, 0x0),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEICAccessTest2_UM,
                        ::testing::ValuesIn(WithParamInterface_UM));

class TregEICAccessTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEICAccessTest3() {}
  virtual ~TregEICAccessTest3() {}
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

  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregEICAccessTest3, CheckEIC_Access_DB_1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      ASSERT_NO_THROW(eic->WriteBody(base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(eic->ReadBody(base_addr + ch*2, (uint8_t*)rdata, SIZE_16BIT));

      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete eic;
  }

  uint8_t temp2 = std::get<0>(GetParam());
  uint8_t expect2 = std::get<1>(GetParam());

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

        ASSERT_NO_THROW(eic->WriteBody(base_addr + ch*2, (uint8_t*)wdata2, SIZE_8BIT));
        ASSERT_NO_THROW(eic->ReadBody(base_addr + ch*2, (uint8_t*)rdata2, SIZE_8BIT));

        uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
        ASSERT_EQ(expect2, m_rdata2);

        delete eic;
  }
}

TEST_P(TregEICAccessTest3, CheckEIC_Access_DB_2) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp3 = (uint8_t (temp >> 8));
  uint8_t expect3 = (uint8_t (expect >>8));

  uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&temp3);
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

        ASSERT_NO_THROW(eic->WriteBody(base_addr + ch*2 + 1, (uint8_t*)wdata3, SIZE_8BIT));
         ASSERT_NO_THROW(eic->ReadBody(base_addr + ch*2 + 1, (uint8_t*)rdata3, SIZE_8BIT));

         uint8_t m_rdata3 = (*(reinterpret_cast<uint8_t*> (rdata3)));
         ASSERT_EQ(expect3, m_rdata3);

         delete eic;
    }
}

std::tuple<uint16_t, uint16_t> const WithParamInterface3[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x90ef),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x10ff, 0x10ef),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access_DB, TregEICAccessTest3,
                        ::testing::ValuesIn(WithParamInterface3));


class TregEICAccessTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEICAccessTest4() {}
  virtual ~TregEICAccessTest4() {}
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

  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEICAccessTest4, CheckEIC_Access_1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      eic->SetEICT(1);
      eic->SetEIRF(0);

      ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_16BIT));

      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete eic;
  }
}

TEST_P(TregEICAccessTest4, CheckEIC_Access_2) {
  uint8_t temp2 = std::get<0>(GetParam());
  uint8_t expect2 = std::get<1>(GetParam());

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      eic->SetEICT(1);
      eic->SetEIRF(0);

        ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata2, SIZE_8BIT));
        ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata2, SIZE_8BIT));

        uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
        ASSERT_EQ(expect2, m_rdata2);

        delete eic;
    }
}

TEST_P(TregEICAccessTest4, CheckEIC_Access_3) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp3 = (uint8_t (temp >> 8));
  uint8_t expect3 = (uint8_t (expect >>8));

  uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&temp3);
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      eic->SetEICT(1);
      eic->SetEIRF(0);

        ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2 + 1, (uint8_t*)wdata3, SIZE_8BIT));
         ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2 + 1, (uint8_t*)rdata3, SIZE_8BIT));

         uint8_t m_rdata3 = (*(reinterpret_cast<uint8_t*> (rdata3)));
         ASSERT_EQ(expect3, m_rdata3);

         delete eic;
    }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x80cf),
  /* 0001 */ std::make_tuple(0x1000, 0x8000),
  /* 0001 */ std::make_tuple(0x0000, 0x8000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEICAccessTest4,
                        ::testing::ValuesIn(WithParamInterface2));

class TregEICAccessTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEICAccessTest5() {}
  virtual ~TregEICAccessTest5() {}
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

  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};



TEST_P(TregEICAccessTest5, CheckEIC_Access_1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      eic->SetEICT(1);
      eic->SetEIRF(1);

      ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_16BIT));

      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete eic;
  }
}

TEST_P(TregEICAccessTest5, CheckEIC_Access_2) {
  uint8_t temp2 = std::get<0>(GetParam());
  uint8_t expect2 = std::get<1>(GetParam());

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      eic->SetEICT(1);
      eic->SetEIRF(1);

        ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata2, SIZE_8BIT));
        ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata2, SIZE_8BIT));

        uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
        ASSERT_EQ(expect2, m_rdata2);

        delete eic;
    }
}

TEST_P(TregEICAccessTest5, CheckEIC_Access_3) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp3 = (uint8_t (temp >> 8));
  uint8_t expect3 = (uint8_t (expect >>8));

  uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&temp3);
  uint8_t rdata3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      eic->SetEICT(1);
      eic->SetEIRF(1);

      ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2 + 1, (uint8_t*)wdata3, SIZE_8BIT));
      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2 + 1, (uint8_t*)rdata3, SIZE_8BIT));

      uint8_t m_rdata3 = (*(reinterpret_cast<uint8_t*> (rdata3)));
      ASSERT_EQ(expect3, m_rdata3);

      delete eic;
    }
}

std::tuple<uint16_t, uint16_t> const WithParamInterface5[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x90cf),
  /* 0001 */ std::make_tuple(0x0000, 0x9000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEICAccessTest5,
                        ::testing::ValuesIn(WithParamInterface5));
class TregEIC_Function
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEIC_Function() {}
  virtual ~TregEIC_Function() {}
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

  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000;
  PhysAddr eibd_addr = 0xFFFC0100;

private:
};

TEST_F(TregEIC_Function, CheckSetEIRFTest1) {
  uint32_t ch = 0;
  uint32_t data_input = 0x1000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();
  bool flag = false;

  ASSERT_NO_THROW(eic->WriteBody(base_addr + ch*2, wdata, SIZE_16BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);

  delete eic;
}

TEST_F(TregEIC_Function, CheckSetEIRFTest1b) {
  uint32_t ch = 0;
  uint32_t data_input = 0x1000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();
  bool flag = false;

  ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, wdata, SIZE_16BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);

  delete eic;
}


TEST_F(TregEIC_Function, CheckSetEIRFTest2) {
  uint32_t ch = 0;
  uint32_t data_input = 0x1000;
  uint32_t data_input_2 = 0x0000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();
  bool flag = false;

  ASSERT_NO_THROW(eic->WriteBody(base_addr + ch*2, wdata, SIZE_16BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);
  ASSERT_NO_THROW(eic->WriteBody(base_addr + ch*2, wdata_2, SIZE_16BIT));
  ASSERT_EQ(true, intc1->m_intc1_queue.empty());

  delete eic;
}

TEST_F(TregEIC_Function, CheckSetEIRFTest2b) {
  uint32_t ch = 0;
  uint32_t data_input = 0x1000;
  uint32_t data_input_2 = 0x0000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();
  bool flag = false;

  ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, wdata, SIZE_16BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);

  ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, wdata_2, SIZE_16BIT));
  ASSERT_EQ(true, intc1->m_intc1_queue.empty());

  delete eic;
}



TEST_F(TregEIC_Function, CheckSetEIMK) {
  uint32_t ch = 0;
  uint32_t data_input = 0x7fff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();

  EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0); //no sending cancel int to outside

  eic->SetEIMK(0);
  ASSERT_NO_THROW(eic->WriteBody(base_addr + ch*2, wdata, SIZE_16BIT));

  delete eic;
}

TEST_F(TregEIC_Function, CheckSetEIMK_b) {
  uint32_t ch = 0;
  uint32_t data_input = 0x7fff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();

  EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0); //sending cancel int to outside

  eic->SetEIMK(0);
  ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, wdata, SIZE_16BIT));

  delete eic;
}


TEST_F(TregEIC_Function, CheckGetBit) {
  uint32_t ch = 0;
  uint32_t data_input = 0xffff;
  uint32_t data_input_2 = 0x7fff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();


  ASSERT_NO_THROW(eic->WriteBody(base_addr + ch*2, wdata, SIZE_16BIT));

  ASSERT_EQ(0x1U, eic->GetEICT());
  ASSERT_EQ(0x1U, eic->GetEIMK());
  ASSERT_EQ(0xfU, eic->GetEIP());
  ASSERT_EQ(0x1U, eic->GetEIRF());
  ASSERT_EQ(0x1U, eic->GetEITB());

  ASSERT_NO_THROW(eic->WriteBody(base_addr + ch*2, wdata_2, SIZE_16BIT));

  ASSERT_EQ(0x0U, eic->GetEICT());
  ASSERT_EQ(0x1U, eic->GetEIMK());
  ASSERT_EQ(0xfU, eic->GetEIP());
  ASSERT_EQ(0x1U, eic->GetEIRF());
  ASSERT_EQ(0x1U, eic->GetEITB());

  delete eic;
}

TEST_F(TregEIC_Function, CheckGetBit_b) {
  uint32_t ch = 0;
  uint32_t data_input = 0xffff;
  uint32_t data_input_2 = 0x7fff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();


  ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, wdata, SIZE_16BIT));

  ASSERT_EQ(0x0U, eic->GetEICT());
  ASSERT_EQ(0x1U, eic->GetEIMK());
  ASSERT_EQ(0xfU, eic->GetEIP());
  ASSERT_EQ(0x1U, eic->GetEIRF());
  ASSERT_EQ(0x1U, eic->GetEITB());

  ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, wdata_2, SIZE_16BIT));

  ASSERT_EQ(0x0U, eic->GetEICT());
  ASSERT_EQ(0x1U, eic->GetEIMK());
  ASSERT_EQ(0xfU, eic->GetEIP());
  ASSERT_EQ(0x1U, eic->GetEIRF());
  ASSERT_EQ(0x1U, eic->GetEITB());

  delete eic;
}

TEST_F(TregEIC_Function, SetEIPTest) {
  uint32_t ch = 0;
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();

  uint32_t pri_l;

  pri_l = 0x5;

  ASSERT_NO_THROW(eic->SetEIP(pri_l));
  ASSERT_EQ(0x5U, eic->GetEIP());

  delete eic;
}

TEST_F(TregEIC_Function, SetEIPTest_EEIC) {
  uint32_t ch = 0;
  intc1->HardReset();
  PhysAddr eeic_base = 0xFFFC0200;

  uint32_t data_input = 0x100000ff;
  uint32_t data_input_2 = 0x1000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);

  intc1->InitPriorityInfo();
  intc1->m_intc1_queue.clear();

  intc1->TargetWrite(sideband, eeic_base + ch*4, wdata, SIZE_32BIT);
  intc1->CyclicHandler();
  ASSERT_EQ(0x3F, intc1->m_intc1_priority[-1]->m_priority_eiint);

  intc1->TargetWrite(sideband, base_addr + ch*2, wdata_2, SIZE_16BIT);
  intc1->CyclicHandler();
  ASSERT_EQ(0x30, intc1->m_intc1_priority[-1]->m_priority_eiint);

}


TEST_F(TregEIC_Function, IsRegWriteEnable) {
  uint32_t ch = 0;
  TRegEIC *eic;
  TRegEIBD *eibd;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eibd = new TRegEIBD (reg_size, min_access_size, eibd_addr+ch*4, TRegMask(TRegBND<EIBD>::INTC1_EIBD_MASK_G4MH20), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  eic->Reset();
  eibd->Reset();

  intc1->SetIHVEFromIHVCFG(1);

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_NO_THROW(eic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, eic->IsRegWriteEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(0);
  ASSERT_NO_THROW(eic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, !eic->IsRegWriteEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  ASSERT_NO_THROW(eic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, !eic->IsRegWriteEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(eic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, !eic->IsRegWriteEnable(sideband));

  intc1->SetIHVEFromIHVCFG(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(eic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, eic->IsRegWriteEnable(sideband));

  sideband.SetUM(0);
  ASSERT_NO_THROW(eic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, eic->IsRegWriteEnable(sideband));


  delete eic;
  delete eibd;
}

TEST_F(TregEIC_Function, IsRegReadEnable) {
  uint32_t ch = 0;
  TRegEIC *eic;
  TRegEIBD *eibd;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eibd = new TRegEIBD (reg_size, min_access_size, eibd_addr+ch*4, TRegMask(TRegBND<EIBD>::INTC1_EIBD_MASK_G4MH20), ch, 0, intc1, (Intc2ImplType)INTC2_STANDARD);
  eic->Reset();
  eibd->Reset();

  intc1->SetIHVEFromIHVCFG(1);

  sideband.SetGM(0);
  sideband.SetUM(0);
  ASSERT_NO_THROW(eic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, eic->IsRegReadEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(0);
  ASSERT_NO_THROW(eic->IsRegWriteEnable(sideband));
  ASSERT_EQ(true, !eic->IsRegWriteEnable(sideband));

  sideband.SetGM(1);
  sideband.SetUM(0);
  sideband.SetGPID(1);
  ASSERT_NO_THROW(eic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, !eic->IsRegReadEnable(sideband));

  sideband.SetGM(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(eic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, !eic->IsRegReadEnable(sideband));

  intc1->SetIHVEFromIHVCFG(0);
  sideband.SetUM(1);
  ASSERT_NO_THROW(eic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, eic->IsRegReadEnable(sideband));

  sideband.SetUM(0);
  ASSERT_NO_THROW(eic->IsRegReadEnable(sideband));
  ASSERT_EQ(true, eic->IsRegReadEnable(sideband));


  delete eic;
  delete eibd;
}

TEST_F(TregEIC_Function, GetSetEIOV) {
  uint32_t ch = 0;
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();

  ASSERT_NO_THROW(eic->SetEIOV(1));
  ASSERT_EQ(0x1U, eic->GetEIOV());

  delete eic;
}


TEST_F(TregEIC_Function, GetSetEICT) {
  uint32_t ch = 0;
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();

  ASSERT_NO_THROW(eic->SetEICT(1));
  ASSERT_EQ(0x1U, eic->GetEICT());

  delete eic;
}

TEST_F(TregEIC_Function, GetSetEIRF) {
  uint32_t ch = 0;
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();

  ASSERT_NO_THROW(eic->SetEIRF(1));
  ASSERT_EQ(0x1U, eic->GetEIRF());

  delete eic;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
