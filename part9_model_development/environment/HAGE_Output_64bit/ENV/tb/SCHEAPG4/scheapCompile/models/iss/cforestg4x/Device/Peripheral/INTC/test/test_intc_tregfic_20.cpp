/**
 * @file test_intc_tregfic_20.cpp
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


class TregFICAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest() {}
  virtual ~TregFICAccessTest() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  PhysAddr fibd_addr = 0xFFFC01C0;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_F(TregFICAccessTest, CheckFIC_initial) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      ASSERT_NO_THROW(fic->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT));
      uint16_t expect_data = 0x0;
      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], data, sizeof(m_data16));
      ASSERT_EQ(expect_data, m_data16[index_16]);
      delete fic;
  }

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
        TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
        fic->Reset();

        ASSERT_NO_THROW(fic->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT));
        uint8_t expect_data = 0x0;
        uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
        ASSERT_EQ(expect_data, m_rdata);
        delete fic;
    }

  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
         TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
         fic->Reset();

         ASSERT_NO_THROW(fic->Read(sideband, base_addr+1, (uint8_t*)data, SIZE_8BIT));
         uint8_t expect_data = 0x0;
         uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (data)));
         ASSERT_EQ(expect_data, m_rdata);
         delete fic;
  }
}

TEST_F(TregFICAccessTest, CheckFIC_GetName) {
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      ASSERT_STREQ("FIC", fic->GetName());

      delete fic;
  }
}

TEST_F(TregFICAccessTest, CheckFIC_GetSetFICT) {
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC *fic;
      fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->SetFICT(1);

      ASSERT_EQ(0x1U, fic->GetFICT());
      delete fic;
  }
}

TEST_F(TregFICAccessTest, CheckFIC_GetSetFIRF) {
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC *fic;
      fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();
      fic->SetFIRF(1);

      ASSERT_EQ(0x1U, fic->GetFIRF());
      delete fic;
  }
}

TEST_F(TregFICAccessTest, CheckFIC_GetSetFIOV) {
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC *fic;
      fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->SetFIOV(1);

      ASSERT_EQ(0x1U, fic->GetFIOV());
      delete fic;
  }
}


TEST_F(TregFICAccessTest, CheckFIC_IsRegWriteEnable) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      intc1->SetIHVEFromIHVCFG(1);
      sideband.SetGM(0);
      sideband.SetUM(0);
      sideband.SetDM(0);
      ASSERT_EQ(true, fic->IsRegWriteEnable(sideband));

      sideband.SetGM(0);
      sideband.SetUM(0);
      sideband.SetDM(1);
      ASSERT_EQ(true, fic->IsRegWriteEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(0);
      sideband.SetDM(0);
      ASSERT_EQ(true, !fic->IsRegWriteEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(0);
      sideband.SetDM(1);
      ASSERT_EQ(true, !fic->IsRegWriteEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(0);
      ASSERT_EQ(true, !fic->IsRegWriteEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(1);
      ASSERT_EQ(true, !fic->IsRegWriteEnable(sideband));

      sideband.SetUM(1);
      sideband.SetDM(0);
      ASSERT_EQ(true, !fic->IsRegWriteEnable(sideband));

      sideband.SetUM(1);
      sideband.SetDM(1);
      ASSERT_EQ(true, !fic->IsRegWriteEnable(sideband));

      intc1->SetIHVEFromIHVCFG(0);
      sideband.SetDM(0);
      sideband.SetUM(0);
      ASSERT_EQ(true, fic->IsRegWriteEnable(sideband));

      sideband.SetUM(1);
      ASSERT_EQ(true, fic->IsRegWriteEnable(sideband));

      sideband.SetDM(1);
      sideband.SetUM(0);
      ASSERT_EQ(true, fic->IsRegWriteEnable(sideband));

      sideband.SetUM(1);
      ASSERT_EQ(true, fic->IsRegWriteEnable(sideband));

      delete fic;
  }
}

TEST_F(TregFICAccessTest, CheckFIC_IsRegReadEnable) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      intc1->SetIHVEFromIHVCFG(1);
      sideband.SetGM(0);
      sideband.SetUM(0);
      sideband.SetGPID(0);
      ASSERT_EQ(true, fic->IsRegReadEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      ASSERT_EQ(true, !fic->IsRegReadEnable(sideband));

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      ASSERT_EQ(true, !fic->IsRegReadEnable(sideband));

      sideband.SetUM(1);
      ASSERT_EQ(true, !fic->IsRegReadEnable(sideband));

      intc1->SetIHVEFromIHVCFG(0);
      sideband.SetUM(0);
      ASSERT_EQ(true, fic->IsRegReadEnable(sideband));

      sideband.SetUM(1);
      ASSERT_EQ(true, fic->IsRegReadEnable(sideband));

      delete fic;
  }
}



TEST_F(TregFICAccessTest, CheckFIC_AccessTest1) {
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(0);
      sideband.SetDM(1);
      ASSERT_THROW(fic->Write(sideband, base_addr, (uint8_t*)data, SIZE_32BIT), std::range_error);
      ASSERT_THROW(fic->Write(sideband, base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
      ASSERT_THROW(fic->Write(sideband, base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

      ASSERT_THROW(fic->WriteBody(base_addr, (uint8_t*)data, SIZE_32BIT), std::range_error);
      ASSERT_THROW(fic->WriteBody(base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
      ASSERT_THROW(fic->WriteBody(base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

      delete fic;
  }
}

class TregFICAccessTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest2() {}
  virtual ~TregFICAccessTest2() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregFICAccessTest2, CheckFIC_AccessTest1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetDM(1);
      ASSERT_NO_THROW(fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT));

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);


      delete fic;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1020),
  /* 0001 */ std::make_tuple(0x1111, 0x1000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest2,
                        ::testing::ValuesIn(WithParamInterface));


class TregFICAccessTest3_HV
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest3_HV() {}
  virtual ~TregFICAccessTest3_HV() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
private:
};

TEST_P(TregFICAccessTest3_HV, CheckFIC_AccessTest1_1_HV) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();
      sideband.SetDM(1);

      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_HV, CheckFIC_AccessTest1_2_HV) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();
        sideband.SetDM(1);

        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest3_HV, CheckFIC_AccessTest1_3_HV) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();
         sideband.SetDM(1);

         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2_HV[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1020),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x1000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest3_HV,
                        ::testing::ValuesIn(WithParamInterface2_HV));

class TregFICAccessTest3_HV_DM0
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest3_HV_DM0() {}
  virtual ~TregFICAccessTest3_HV_DM0() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
private:
};

TEST_P(TregFICAccessTest3_HV_DM0, CheckFIC_AccessTest1_1_HV_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();
      sideband.SetDM(0);

      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_HV_DM0, CheckFIC_AccessTest1_2_HV_DM0) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();
        sideband.SetDM(0);

        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest3_HV_DM0, CheckFIC_AccessTest1_3_HV_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();
         sideband.SetDM(0);

         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2_HV_DM0[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x0000),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x0000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest3_HV_DM0,
                        ::testing::ValuesIn(WithParamInterface2_HV_DM0));

class TregFICAccessTest3_SV
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest3_SV() {}
  virtual ~TregFICAccessTest3_SV() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
private:
};

TEST_P(TregFICAccessTest3_SV, CheckFIC_AccessTest1_1_SV_DIFF_GM) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV, CheckFIC_AccessTest1_1_SV_DIFF_GM_32bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV, CheckFIC_AccessTest1_1_SV_DIFF_GM_64bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV, CheckFIC_AccessTest1_1_SV_DIFF_GM_128bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV, CheckFIC_AccessTest1_2_SV_DIFF_GM) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = 0;

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();

        sideband.SetGM(1);
        sideband.SetUM(0);
        sideband.SetDM(1);
        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest3_SV, CheckFIC_AccessTest1_3_SV) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = 0;

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();

         sideband.SetGM(1);
         sideband.SetUM(0);
         sideband.SetDM(1);
         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2_SV[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1020),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x1000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest3_SV,
                        ::testing::ValuesIn(WithParamInterface2_SV));

class TregFICAccessTest3_SV_DM0
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest3_SV_DM0() {}
  virtual ~TregFICAccessTest3_SV_DM0() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
private:
};

TEST_P(TregFICAccessTest3_SV_DM0, CheckFIC_AccessTest1_1_SV_DIFF_GM_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DM0, CheckFIC_AccessTest1_1_SV_DIFF_GM_32bits_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DM0, CheckFIC_AccessTest1_1_SV_DIFF_GM_64bits_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DM0, CheckFIC_AccessTest1_1_SV_DIFF_GM_128bits_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DM0, CheckFIC_AccessTest1_2_SV_DIFF_GM_DM0) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = 0;

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();

        sideband.SetGM(1);
        sideband.SetUM(0);
        sideband.SetDM(0);
        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest3_SV_DM0, CheckFIC_AccessTest1_3_SV_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = 0;

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();

         sideband.SetGM(1);
         sideband.SetUM(0);
         sideband.SetDM(0);
         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2_SV_DM0[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1020),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x1000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1_DM0, TregFICAccessTest3_SV_DM0,
                        ::testing::ValuesIn(WithParamInterface2_SV_DM0));

class TregFICAccessTest3_SV_DIFF_GPID
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest3_SV_DIFF_GPID() {}
  virtual ~TregFICAccessTest3_SV_DIFF_GPID() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
private:
};

TEST_P(TregFICAccessTest3_SV_DIFF_GPID, CheckFIC_AccessTest1_1_SV_DIFF_GPID) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DIFF_GPID, CheckFIC_AccessTest1_1_SV_DIFF_GPID_32bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DIFF_GPID, CheckFIC_AccessTest1_1_SV_DIFF_GPID_64bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DIFF_GPID, CheckFIC_AccessTest1_1_SV_DIFF_GPID_128bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0};

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}


TEST_P(TregFICAccessTest3_SV_DIFF_GPID, CheckFIC_AccessTest1_2_SV_DIFF_GPID) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = 0x0;

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();

        sideband.SetGM(1);
        sideband.SetUM(0);
        sideband.SetGPID(1);
        sideband.SetDM(1);
        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest3_SV_DIFF_GPID, CheckFIC_AccessTest1_3_SV_DIFF_GPID) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = 0x0;

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();

         sideband.SetGM(1);
         sideband.SetUM(0);
         sideband.SetGPID(1);
         sideband.SetDM(1);
         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2_SV_DIFF_GPID[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1020),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x1000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest3_SV_DIFF_GPID,
                        ::testing::ValuesIn(WithParamInterface2_SV_DIFF_GPID));

class TregFICAccessTest3_SV_DIFF_GPID_GM0
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest3_SV_DIFF_GPID_GM0() {}
  virtual ~TregFICAccessTest3_SV_DIFF_GPID_GM0() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
private:
};

TEST_P(TregFICAccessTest3_SV_DIFF_GPID_GM0, CheckFIC_AccessTest1_1_SV_DIFF_GPID_GM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DIFF_GPID_GM0, CheckFIC_AccessTest1_1_SV_DIFF_GPID_32bits_GM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DIFF_GPID_GM0, CheckFIC_AccessTest1_1_SV_DIFF_GPID_64bits_GM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_SV_DIFF_GPID_GM0, CheckFIC_AccessTest1_1_SV_DIFF_GPID_128bits_GM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0};

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(1);
      sideband.SetUM(0);
      sideband.SetGPID(1);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}


TEST_P(TregFICAccessTest3_SV_DIFF_GPID_GM0, CheckFIC_AccessTest1_2_SV_DIFF_GPID_GM0) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = 0x0;

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();

        sideband.SetGM(1);
        sideband.SetUM(0);
        sideband.SetGPID(1);
        sideband.SetDM(0);
        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest3_SV_DIFF_GPID_GM0, CheckFIC_AccessTest1_3_SV_DIFF_GPID_GM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = 0x0;

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();

         sideband.SetGM(1);
         sideband.SetUM(0);
         sideband.SetGPID(1);
         sideband.SetDM(0);
         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2_SV_DIFF_GPID_GM0[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1020),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x1000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1_GM0, TregFICAccessTest3_SV_DIFF_GPID_GM0,
                        ::testing::ValuesIn(WithParamInterface2_SV_DIFF_GPID_GM0));


class TregFICAccessTest3_UM
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
	TregFICAccessTest3_UM() {}
  virtual ~TregFICAccessTest3_UM() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
private:
};

TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_1_UM) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetUM(1);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_1_UM_32bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetUM(1);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_1_UM_32bits_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetUM(1);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_32BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_1_UM_64bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetUM(1);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_1_UM_64bits_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetUM(1);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_64BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_1_UM_128bits) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetUM(1);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_1_UM_128bits_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = 0x0;

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0};

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetUM(1);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_128BIT);
      ASSERT_EQ(0x19U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}


TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_2_UM) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = 0x0;

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();

        sideband.SetUM(1);
        sideband.SetDM(1);
        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_2_UM_DM0) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = 0x0;

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();

        sideband.SetUM(1);
        sideband.SetDM(0);
        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x19U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}


TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_3_UM) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = 0x0;

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();

         sideband.SetUM(1);
         sideband.SetDM(1);
         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}

TEST_P(TregFICAccessTest3_UM, CheckFIC_AccessTest1_3_UM_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = 0x0;

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();

         sideband.SetUM(1);
         sideband.SetDM(0);
         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x19U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2_UM[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1020),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x1000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest3_UM,
                        ::testing::ValuesIn(WithParamInterface2_UM));

class TregFICAccessTest3_IHVE0
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
	TregFICAccessTest3_IHVE0() {}
  virtual ~TregFICAccessTest3_IHVE0() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(0);
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
private:
};

TEST_P(TregFICAccessTest3_IHVE0, CheckFIC_AccessTest1_1_IHVE0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetUM(0);
      sideband.SetDM(1);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);


      sideband.SetUM(1);
      syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);


      delete fic;
  }
}

TEST_P(TregFICAccessTest3_IHVE0, CheckFIC_AccessTest1_2_IHVE0) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();

        sideband.SetUM(0);
        sideband.SetDM(1);
        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        sideband.SetUM(0);
        sideband.SetDM(1);
        syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest3_IHVE0, CheckFIC_AccessTest1_3_IHVE0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();

         sideband.SetUM(0);
         sideband.SetDM(1);
         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         sideband.SetUM(1);
         sideband.SetDM(1);
         syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2_IHVE0[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1020),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x1000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest3_IHVE0,
                        ::testing::ValuesIn(WithParamInterface2_IHVE0));


class TregFICAccessTest3_IHVE0_DM0
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
	TregFICAccessTest3_IHVE0_DM0() {}
  virtual ~TregFICAccessTest3_IHVE0_DM0() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(0);
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
private:
};

TEST_P(TregFICAccessTest3_IHVE0_DM0, CheckFIC_AccessTest1_1_IHVE0_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetUM(0);
      sideband.SetDM(0);
      SysErrCause syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);


      sideband.SetUM(1);
      syserr = fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      syserr = fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT);
      ASSERT_EQ(0x0U, syserr);

      index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);


      delete fic;
  }
}

TEST_P(TregFICAccessTest3_IHVE0_DM0, CheckFIC_AccessTest1_2_IHVE0_DM0) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();

        sideband.SetUM(0);
        sideband.SetDM(0);
        SysErrCause syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        uint32_t index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        sideband.SetUM(0);
        sideband.SetDM(0);
        syserr = fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        syserr = fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT);
        ASSERT_EQ(0x0U, syserr);

        index_16 = (base_addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
        ASSERT_EQ(expect1    , m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest3_IHVE0_DM0, CheckFIC_AccessTest1_3_IHVE0_DM0) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();

         sideband.SetUM(0);
         sideband.SetDM(0);
         SysErrCause syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         sideband.SetUM(1);
         sideband.SetDM(0);
         syserr = fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         syserr = fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT);
         ASSERT_EQ(0x0U, syserr);

         m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface2_IHVE0_DM0[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x0000),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x0000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest3_IHVE0_DM0,
                        ::testing::ValuesIn(WithParamInterface2_IHVE0_DM0));

class TregFICAccessTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest4() {}
  virtual ~TregFICAccessTest4() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(AtLeast(0));
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregFICAccessTest4, CheckFIC_AccessTest1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetDM(1);
      ASSERT_NO_THROW(fic->WriteBody(base_addr, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(fic->ReadBody(base_addr, (uint8_t*)rdata, SIZE_16BIT));

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}
std::tuple<uint16_t, uint16_t> const WithParamInterface3[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x9020),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x9000),
  /* 0003 */ std::make_tuple(0x10ff, 0x1020),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest4,
                        ::testing::ValuesIn(WithParamInterface3));

TEST_F(TregFICAccessTest4, CheckFIC_SetFIRFTes1) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();
      bool flag = false;
      uint32_t base_ch = 0x1001U; //FEINT
      sideband.SetGM(0);
      sideband.SetDM(1);
      intc1->SetIHVEFromIHVCFG(1);
      ASSERT_NO_THROW(fic->Write(sideband, base_addr, wdata, SIZE_16BIT));

      typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
      IntReqMap::iterator ite = intc1->m_intc1_queue.find (base_ch + ch);
      if (ite != intc1->m_intc1_queue.end()) {
          flag = true;
      }
      ASSERT_EQ(true, flag);

      delete fic;
  }
}

TEST_F(TregFICAccessTest4, CheckFIC_SetFIRFTes2) {
  uint32_t data_input = 0xffff;
  uint32_t data_input_2 = 0x0000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();
      bool flag = false;
      uint32_t base_ch = 0x1001U; //FEINT

      sideband.SetGM(0);
      sideband.SetDM(1);
      ASSERT_NO_THROW(fic->Write(sideband, base_addr, wdata, SIZE_16BIT));

      typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
      IntReqMap::iterator ite = intc1->m_intc1_queue.find (base_ch + ch);
      if (ite != intc1->m_intc1_queue.end()) {
          flag = true;
      }
      ASSERT_EQ(true, flag);

      flag=false;
      ASSERT_NO_THROW(fic->Write(sideband, base_addr, wdata_2, SIZE_16BIT));
      flag =  intc1->m_intc1_queue.empty();
      ASSERT_EQ(true, flag);

      delete fic;
  }
}


TEST_F(TregFICAccessTest4, CheckFIC_PriorityTest1) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  TRegFIC* fic_6 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+6*2, 6, intc1);
  fic_6->Reset();

  TRegFIC* fic_4 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+4*2, 4, intc1);
  fic_4->Reset();

  bool flag = false;
  uint32_t base_ch = 0x1001U; //FEINT

  sideband.SetGM(0);
  sideband.SetDM(1);

  ASSERT_NO_THROW(fic_6->Write(sideband, base_addr, wdata, SIZE_16BIT));
  ASSERT_NO_THROW(fic_4->Write(sideband, base_addr, wdata, SIZE_16BIT));
  intc1->CyclicHandler();

  typedef std::map<ContextId, IntPriorityInfo*> IntHighPriorityMap;
  IntHighPriorityMap::const_iterator it = intc1->m_intc1_priority.begin();

  if (it != intc1->m_intc1_priority.end()) {
      uint32_t channel = it->second->GetChannelFeint();

      if ((channel - base_ch) == 4){
          flag = true;
      }
  }

  ASSERT_EQ(true, flag);

  delete fic_6;
  delete fic_4;
}


class TregFICAccessTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest5() {}
  virtual ~TregFICAccessTest5() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregFICAccessTest5, CheckFIC_AccessTest1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(0);
      sideband.SetDM(1);
      fic->SetFICT(1);
      fic->SetFIRF(0);

      ASSERT_NO_THROW(fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT));

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);


      delete fic;
  }


  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
        TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
        fic1->Reset();

      sideband.SetGM(0);
        fic1->SetFICT(1);
        fic1->SetFIRF(0);
        ASSERT_NO_THROW(fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT));
        ASSERT_NO_THROW(fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT));

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
      ASSERT_EQ(expect1, m_data16[index_16]);

        delete fic1;
    }


  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
         TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
         fic2->Reset();

         sideband.SetGM(0);
         sideband.SetDM(1);
         fic2->SetFICT(1);
         fic2->SetFIRF(0);
         ASSERT_NO_THROW(fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT));
         ASSERT_NO_THROW(fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT));

         uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
         ASSERT_EQ(expect2, m_rdata2);

         delete fic2;
  }
}

std::tuple<uint16_t, uint16_t> const WithParamInterface5[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x8000),
  /* 0001 */ std::make_tuple(0x0000, 0x8000),
  /* 0002 */ std::make_tuple(0x9000, 0x8000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest5,
                        ::testing::ValuesIn(WithParamInterface5));

class TregFICAccessTest6
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFICAccessTest6() {}
  virtual ~TregFICAccessTest6() {}
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

  static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FIC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregFICAccessTest6, CheckFIC_AccessTest1_1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  for (uint32_t ch = 0; ch < TRegIntc1::N_FIC; ch++) {
      TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch*2, ch, intc1);
      fic->Reset();

      sideband.SetGM(0);
      sideband.SetDM(1);
      fic->SetFICT(1);
      fic->SetFIRF(1);

      ASSERT_NO_THROW(fic->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT));
      ASSERT_NO_THROW(fic->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT));

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
      ASSERT_EQ(expect, m_data16[index_16]);

      delete fic;
  }
}

TEST_P(TregFICAccessTest6, CheckFIC_AccessTest1_2) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch1 = 0; ch1 < TRegIntc1::N_FIC; ch1++) {
      TRegFIC* fic1 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch1*2, ch1, intc1);
      fic1->Reset();

      sideband.SetGM(0);
      sideband.SetDM(1);
      fic1->SetFICT(1);
      fic1->SetFIRF(1);
      ASSERT_NO_THROW(fic1->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT));
      ASSERT_NO_THROW(fic1->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT));

      uint32_t index_16 = (base_addr >> 1) & 0x1;
      memcpy (&m_data16[index_16], rdata1, sizeof(m_data16));
      ASSERT_EQ(expect1, m_data16[index_16]);

        delete fic1;
    }
}

TEST_P(TregFICAccessTest6, CheckFIC_AccessTest1_3) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  for (uint32_t ch2 = 0; ch2 < TRegIntc1::N_FIC; ch2++) {
       TRegFIC* fic2 = new TRegFIC (SIZE_16BIT, SIZE_8BIT, base_addr+ch2*2, ch2, intc1);
       fic2->Reset();

       sideband.SetGM(0);
       sideband.SetDM(1);
       fic2->SetFICT(1);
       fic2->SetFIRF(1);
       ASSERT_NO_THROW(fic2->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT));
       ASSERT_NO_THROW(fic2->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT));

       uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
       ASSERT_EQ(expect2, m_rdata2);

       delete fic2;
  }
}

std::tuple<uint16_t, uint16_t> const WithParamInterface6[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x9000),
  /* 0001 */ std::make_tuple(0x0000, 0x9000),
  /* 0002 */ std::make_tuple(0x9000, 0x9000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFIC_AccessTest1, TregFICAccessTest6,
                        ::testing::ValuesIn(WithParamInterface6));

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
