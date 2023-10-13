/**
 * @file test_intc_tregfnc.cpp
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


class TregFNCAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFNCAccessTest() {}
  virtual ~TregFNCAccessTest() {}
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

  static const PhysAddr FNC_OFFSET  = 0x00000000A0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FNC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_F(TregFNCAccessTest, CheckFNC_initial) {
  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  ASSERT_NO_THROW(fnc->Read(sideband, base_addr, (uint8_t*)data, SIZE_16BIT));
  uint16_t expect_data = 0x0;
  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], data, sizeof(m_data16));
  ASSERT_EQ(expect_data, m_data16[index_16]);

  ASSERT_NO_THROW(fnc->Read(sideband, base_addr, (uint8_t*)data, SIZE_8BIT));
  uint8_t expect_data1 = 0x0;
  uint8_t m_rdata1 = (*(reinterpret_cast<uint8_t*> (data)));
  ASSERT_EQ(expect_data1, m_rdata1);

  ASSERT_NO_THROW(fnc->Read(sideband, base_addr + 1 , (uint8_t*)data, SIZE_8BIT));
  uint8_t expect_data2 = 0x0;
  uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (data)));
  ASSERT_EQ(expect_data2, m_rdata2);

  delete fnc;
}


TEST_F(TregFNCAccessTest, CheckFNC_GetSetFNCT) {
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->SetFNCT(1);

  ASSERT_EQ(0x1U, fnc->GetFNCT());
  delete fnc;
}

TEST_F(TregFNCAccessTest, CheckFNC_GetSetFNRF) {
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();
  fnc->SetFNRF(1);

  ASSERT_EQ(0x1U, fnc->GetFNRF());
  delete fnc;
}


TEST_F(TregFNCAccessTest, CheckFNC_GetName) {
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  ASSERT_STREQ("FNC", fnc->GetName());

  delete fnc;
}

TEST_F(TregFNCAccessTest, CheckFNC_IsRegWriteEnableTest1) {
  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  ASSERT_EQ(true, fnc->IsRegWriteEnable(sideband));

  delete fnc;
}



TEST_F(TregFNCAccessTest, CheckFNC_AccessTest1) {
  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  ASSERT_THROW(fnc->Write(sideband, base_addr, (uint8_t*)data, SIZE_32BIT), std::range_error);
  ASSERT_THROW(fnc->Write(sideband, base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
  ASSERT_THROW(fnc->Write(sideband, base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

  ASSERT_THROW(fnc->WriteBody(base_addr, (uint8_t*)data, SIZE_32BIT), std::range_error);
  ASSERT_THROW(fnc->WriteBody(base_addr, (uint8_t*)data, SIZE_64BIT), std::range_error);
  ASSERT_THROW(fnc->WriteBody(base_addr, (uint8_t*)data, SIZE_128BIT), std::range_error);

  delete fnc;
}

class TregFNCAccessTest2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFNCAccessTest2() {}
  virtual ~TregFNCAccessTest2() {}
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

  static const PhysAddr FNC_OFFSET  = 0x00000000A0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FNC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregFNCAccessTest2, CheckFNC_AccessTest1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT));

  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
  ASSERT_EQ(expect, m_data16[index_16]);

  delete fnc;
}

std::tuple<uint16_t,uint16_t> const WithParamInterface[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x0000),
  /* 0001 */ std::make_tuple(0x1111, 0x0000),
  // please write more test case.
};

INSTANTIATE_TEST_CASE_P(CheckFNC_AccessTest1, TregFNCAccessTest2,
                        ::testing::ValuesIn(WithParamInterface));

class TregFNCAccessTest3
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFNCAccessTest3() {}
  virtual ~TregFNCAccessTest3() {}
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

  static const PhysAddr FNC_OFFSET  = 0x00000000A0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FNC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregFNCAccessTest3, CheckFNC_AccessTest1_1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT));

  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
  ASSERT_EQ(expect, m_data16[index_16]);

  delete fnc;
}

TEST_P(TregFNCAccessTest3, CheckFNC_AccessTest1_2) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT));

  uint8_t m_rdata1 = (*(reinterpret_cast<uint8_t*> (rdata1)));
  ASSERT_EQ(expect1, m_rdata1);

  delete fnc;
}

TEST_P(TregFNCAccessTest3, CheckFNC_AccessTest1_3) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT));

  uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
  ASSERT_EQ(expect2, m_rdata2);

  delete fnc;
}

std::tuple<uint16_t, uint16_t> const WithParamInterface2[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x1000),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x1000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFNC_AccessTest1, TregFNCAccessTest3,
                        ::testing::ValuesIn(WithParamInterface2));


class TregFNCAccessTest4
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFNCAccessTest4() {}
  virtual ~TregFNCAccessTest4() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x10);

    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(AtLeast(0));
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

  static const PhysAddr FNC_OFFSET  = 0x00000000A0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FNC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregFNCAccessTest4, CheckFNC_AccessTest1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  ASSERT_NO_THROW(fnc->WriteBody(base_addr, (uint8_t*)wdata, SIZE_16BIT));
  ASSERT_NO_THROW(fnc->ReadBody(base_addr, (uint8_t*)rdata, SIZE_16BIT));

  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
  ASSERT_EQ(expect, m_data16[index_16]);

  delete fnc;
}


std::tuple<uint16_t, uint16_t> const WithParamInterface3[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x8000),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x9000, 0x8000),
  /* 0002 */ std::make_tuple(0x10ff, 0x1000),
  // please write more test case.
};


TEST_F(TregFNCAccessTest4, CheckFNC_SetFNRFTes1) {
  uint32_t data_input = 0xffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();
  bool flag = false;
  uint32_t ch = 0x1000U; //FENMI

  sideband.SetDM(1);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, wdata, SIZE_16BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);

  delete fnc;
}

TEST_F(TregFNCAccessTest4, CheckFNC_SetFNRFTes2) {
  uint32_t data_input = 0xffff;
  uint32_t data_input_2 = 0x0000;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
  uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_input_2);
  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();
  bool flag = false;
  uint32_t ch = 0x1000U; //FENMI

  sideband.SetDM(1);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, wdata, SIZE_16BIT));

  typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
  IntReqMap::iterator ite = intc1->m_intc1_queue.find (ch);
  if (ite != intc1->m_intc1_queue.end()) {
      flag = true;
  }
  ASSERT_EQ(true, flag);

  flag = false;
  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, wdata_2, SIZE_16BIT));
  flag =  intc1->m_intc1_queue.empty();
  ASSERT_EQ(true, flag);

  delete fnc;
}


class TregFNCAccessTest5
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFNCAccessTest5() {}
  virtual ~TregFNCAccessTest5() {}
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

  static const PhysAddr FNC_OFFSET  = 0x00000000A0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FNC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregFNCAccessTest5, CheckFNC_Access_1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  fnc->SetFNCT(1);
  fnc->SetFNRF(0);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT));

  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
  ASSERT_EQ(expect, m_data16[index_16]);

  delete fnc;
}
  //------------------------------

TEST_P(TregFNCAccessTest5, CheckFNC_Access_2) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  fnc->SetFNCT(1);
  fnc->SetFNRF(0);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT));

  uint8_t m_rdata1 = (*(reinterpret_cast<uint8_t*> (rdata1)));
  ASSERT_EQ(expect1, m_rdata1);

  delete fnc;
}
  //--------------------------------

TEST_P(TregFNCAccessTest5, CheckFNC_Access_3) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  fnc->SetFNCT(1);
  fnc->SetFNRF(0);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT));

  uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
  ASSERT_EQ(expect2, m_rdata2);

  delete fnc;
}


std::tuple<uint16_t, uint16_t> const WithParamInterface5[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x8000),
  /* 0001 */ std::make_tuple(0x0000, 0x8000),
  /* 0002 */ std::make_tuple(0x9000, 0x8000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFNC_AccessTest1, TregFNCAccessTest5,
                        ::testing::ValuesIn(WithParamInterface5));

class TregFNCAccessTest6
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFNCAccessTest6() {}
  virtual ~TregFNCAccessTest6() {}
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

  static const PhysAddr FNC_OFFSET  = 0x00000000A0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FNC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_P(TregFNCAccessTest6, CheckFNC_Access_1) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  fnc->SetFNCT(1);
  fnc->SetFNRF(1);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, (uint8_t*)wdata, SIZE_16BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr, (uint8_t*)rdata, SIZE_16BIT));

  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
  ASSERT_EQ(expect, m_data16[index_16]);

  delete fnc;
}
  //------------------------------

TEST_P(TregFNCAccessTest6, CheckFNC_Access_2) {
  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  fnc->SetFNCT(1);
  fnc->SetFNRF(1);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr, (uint8_t*)wdata1, SIZE_8BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr, (uint8_t*)rdata1, SIZE_8BIT));

  uint8_t m_rdata1 = (*(reinterpret_cast<uint8_t*> (rdata1)));
  ASSERT_EQ(expect1, m_rdata1);

  delete fnc;
}
  //--------------------------------

TEST_P(TregFNCAccessTest6, CheckFNC_Access_3) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());
  uint8_t temp2 = (uint8_t (temp >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  fnc->SetFNCT(1);
  fnc->SetFNRF(1);

  ASSERT_NO_THROW(fnc->Write(sideband, base_addr+1, (uint8_t*)wdata2, SIZE_8BIT));
  ASSERT_NO_THROW(fnc->Read(sideband, base_addr+1, (uint8_t*)rdata2, SIZE_8BIT));

  uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
  ASSERT_EQ(expect2, m_rdata2);

  delete fnc;
}

std::tuple<uint16_t, uint16_t> const WithParamInterface6[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x9000),
  /* 0001 */ std::make_tuple(0x0000, 0x9000),
  /* 0002 */ std::make_tuple(0x1000, 0x9000),
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(CheckFNC_Access, TregFNCAccessTest6,
                        ::testing::ValuesIn(WithParamInterface6));


class TregFNCAccessTest7
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregFNCAccessTest7() {}
  virtual ~TregFNCAccessTest7() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x10);

    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(AtLeast(0));
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

  static const PhysAddr FNC_OFFSET  = 0x00000000A0UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + FNC_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


TEST_P(TregFNCAccessTest7, CheckFNC_Access_DB) {
  uint16_t temp = std::get<0>(GetParam());
  uint16_t expect = std::get<1>(GetParam());

  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  intc1->HardReset();
  TRegFNC *fnc;
  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();
  sideband.SetDM(1);

  fnc->SetFNCT(1);
  fnc->SetFNRF(1);

  ASSERT_NO_THROW(fnc->WriteBody(base_addr, (uint8_t*)wdata, SIZE_16BIT));
  ASSERT_NO_THROW(fnc->ReadBody(base_addr, (uint8_t*)rdata, SIZE_16BIT));

  uint32_t index_16 = (base_addr >> 1) & 0x1;
  memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
  ASSERT_EQ(expect, m_data16[index_16]);

  delete fnc;
  //------------------------------

  uint8_t temp1 = std::get<0>(GetParam());
  uint8_t expect1 = std::get<1>(GetParam());

  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);
  uint8_t rdata1[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  fnc->SetFNCT(1);
  fnc->SetFNRF(1);

  ASSERT_NO_THROW(fnc->WriteBody(base_addr, (uint8_t*)wdata1, SIZE_8BIT));
  ASSERT_NO_THROW(fnc->ReadBody(base_addr, (uint8_t*)rdata1, SIZE_8BIT));

  uint8_t m_rdata1 = (*(reinterpret_cast<uint8_t*> (rdata1)));
  ASSERT_EQ(expect1, m_rdata1);

  delete fnc;
  //--------------------------------
  uint16_t temp0 = std::get<0>(GetParam());
  uint8_t temp2 = (uint8_t (temp0 >> 8));
  uint8_t expect2 = (uint8_t (expect >>8));

  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&temp2);
  uint8_t rdata2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  fnc = new TRegFNC (reg_size, min_access_size, base_addr, intc1);
  fnc->Reset();

  sideband.SetDM(1);

  fnc->SetFNCT(1);
  fnc->SetFNRF(1);

  ASSERT_NO_THROW(fnc->WriteBody(base_addr+1, (uint8_t*)wdata2, SIZE_8BIT));
  ASSERT_NO_THROW(fnc->ReadBody(base_addr+1, (uint8_t*)rdata2, SIZE_8BIT));

  uint8_t m_rdata2 = (*(reinterpret_cast<uint8_t*> (rdata2)));
  ASSERT_EQ(expect2, m_rdata2);

  delete fnc;
}

std::tuple<uint16_t,uint16_t> const WithParamInterface7[] = {
  /* 0000 */ std::make_tuple(0xffff, 0x9000),
  /* 0001 */ std::make_tuple(0x0000, 0x0000),
  /* 0002 */ std::make_tuple(0x1000, 0x1000),
  // please write more test case.
};

INSTANTIATE_TEST_CASE_P(CheckFNC_Access_DB, TregFNCAccessTest7,
                        ::testing::ValuesIn(WithParamInterface7));



int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
