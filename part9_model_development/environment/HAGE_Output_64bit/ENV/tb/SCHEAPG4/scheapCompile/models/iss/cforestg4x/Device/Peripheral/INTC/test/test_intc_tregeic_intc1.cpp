/**
 * @file test_intc_tregeic_intc1.cpp
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


TEST_F(TregEICAccessTest, CheckEIC_initial) {
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
      uint8_t expect_data_high = 0x00;

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



TEST_P(TregEICAccessTest2, CheckEIC_Access_1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

      ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata, SIZE_16BIT));

      uint32_t index_16 = ((base_addr + ch*2) >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);


      delete eic;
  }
}

TEST_P(TregEICAccessTest2, CheckEIC_Access_2) {
  uint8_t temp2 = std::get<0>(GetParam());
  uint8_t expect2 = std::get<1>(GetParam());

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch = 0; ch < TRegIntc1::N_EIC; ch ++ ){
      TRegEIC *eic;
      eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
      eic->Reset();

        ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2, (uint8_t*)wdata2, SIZE_8BIT));
        ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2, (uint8_t*)rdata2, SIZE_8BIT));

        uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
        ASSERT_EQ(expect2, m_rdata2);

        delete eic;
    }
}

TEST_P(TregEICAccessTest2, CheckEIC_Access_3) {
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

        ASSERT_NO_THROW(eic->Write(sideband, base_addr + ch*2 + 1, (uint8_t*)wdata3, SIZE_8BIT));
         ASSERT_NO_THROW(eic->Read(sideband, base_addr + ch*2 + 1, (uint8_t*)rdata3, SIZE_8BIT));

         uint8_t m_rdata3 = (*(reinterpret_cast<uint8_t*> (rdata3)));
         ASSERT_EQ(expect3, m_rdata3);

         delete eic;
      }
}

std::tuple<uint16_t,uint16_t> const WithParamInterface[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x10cf),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  // please write more test case.
};

INSTANTIATE_TEST_CASE_P(CheckEIC_Access, TregEICAccessTest2,
                        ::testing::ValuesIn(WithParamInterface));


class TregEICAccessTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregEICAccessTest3() {}
  virtual ~TregEICAccessTest3() {}
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

std::tuple<uint16_t,uint16_t> const WithParamInterface3[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x90cf),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x10ff, 0x10cf),
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
std::tuple<uint16_t,uint16_t> const WithParamInterface2[] = {
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

std::tuple<uint16_t,uint16_t> const WithParamInterface5[] = {
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
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x10);

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

TEST_F(TregEIC_Function, SetEIPTest) {
  uint32_t ch = 0;
  TRegEIC *eic;
  intc1->HardReset();
  eic = new TRegEIC (reg_size, min_access_size, base_addr+ch*2, ch, pe_id, intc1);
  eic->Reset();

  uint32_t pri_l;

  pri_l = 0x5;

  ASSERT_NO_THROW(eic->SetEIP(pri_l));
  ASSERT_EQ(0x05U, eic->GetEIP());


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
