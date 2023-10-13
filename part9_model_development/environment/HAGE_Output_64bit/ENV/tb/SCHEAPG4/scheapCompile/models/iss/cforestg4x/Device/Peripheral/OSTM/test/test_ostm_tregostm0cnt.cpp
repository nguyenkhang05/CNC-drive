/**
 * @file test_cOstm.cpp
 *
 * @date Created on: 2016
  * @author Author: YVo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for Ostm. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <functional>
#include <string.h>
#include <vector>
#include <memory>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "gmock/gmock.h"
#include "./config.h"
#include "./mock_fsbus.h"
#include "./exp_info.h"
#include "os_timer.h"
#include "mock_sideband.h"
#include "./mock_proc_element.h"
#include "./mock_fsbus.h"
#include "./forest_common.h"
#include "./intc_api.h"
#include "./module_vc.h"
#include "./treg.h"

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

using ::testing::ExpectationSet;
using ::testing::Expectation;

class OstmRegAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
    OstmRegAccessTest() {}
  virtual ~OstmRegAccessTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
  }
  virtual void TearDown() {
      delete mock_fsbus;
      delete mock_pe;
      delete cintc2;

  };

public:
  MockFsBus* mock_fsbus = NULL;
  SideBand sideband;
  MockProcElement *mock_pe;
  MockCIntc1 *intc1 = NULL;
  MockCIntc2 *cintc2 = NULL;
  uint32_t pe_id = 0;
  uint32_t ostmch = 0;
  static const uint32_t MAX_OSTM_NUM = 7;
  OstmIO* m_ostm[MAX_OSTM_NUM];
  static const uint32_t INIT_OSTM_RATIO = 1;
  PhysAddr start_addr = 0xFFEC0004;
  PhysAddr base_addr = 0xFFEC0000;
  uint32_t intch = 25;
  uint8_t data[8] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };

  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_F(OstmRegAccessTest, CheckCNT_GetName) {
	m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, start_addr, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);
    OstmCore* m_ostm0_core = new OstmCore(pe_id, intc1, cintc2, intch, ostmch, mock_fsbus, m_ostm[ostmch], INIT_OSTM_RATIO);
    TRegOSTMnCNT* OSTM0CNT = new TRegOSTMnCNT(m_ostm0_core);

    ASSERT_STREQ("OSTMnCNT", OSTM0CNT->GetName());

    delete OSTM0CNT;
    delete m_ostm0_core;
    delete m_ostm[ostmch];
}

TEST_F(OstmRegAccessTest, CheckCNT_InitalValue) {
    for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
        pe_id = ostmch;
        PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
        m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);
        OstmCore* m_ostm0_core = new OstmCore(pe_id, intc1, cintc2, intch, ostmch, mock_fsbus, m_ostm[ostmch], INIT_OSTM_RATIO);
        TRegOSTMnCNT* OSTM0CNT = new TRegOSTMnCNT(m_ostm0_core);
        OSTM0CNT->Reset();
        uint32_t expect_data = 0xFFFFFFFF;

        ASSERT_EQ(expect_data, OSTM0CNT->GetVal());
        delete OSTM0CNT;
        delete m_ostm0_core;
        delete m_ostm[ostmch];
    }
}

/*!
 * @brief Check Intc1 register read/write operation.
 */
TEST_P(OstmRegAccessTest, CheckOstmRegReadWrite) {
  start_addr = std::get<0>(GetParam());
  int32_t temp = static_cast<int32_t>(std::get<1>(GetParam()));
  uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);

  ByteSize size = std::get<2>(GetParam());
  ByteSize expect_size = std::get<3>(GetParam());
  uint32_t expect = std::get<4>(GetParam());

  for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
      pe_id = ostmch;
      PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
      PhysAddr start_addr_pe = start_addr + 0x1000*ostmch;
      m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);
      m_ostm[ostmch]->HardReset();

      if (size != expect_size){
        // Expect throw if "Un-guaranteed Size Access".
        ASSERT_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe, wdata, size), std::range_error);
      }else{
        {
          SCOPED_TRACE("Write");
          if (size == expect_size) {
            ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe, wdata, size));
          }
        }
          {
          SCOPED_TRACE("Read");
          if (size == expect_size) {
              if (size == SIZE_8BIT){
              ASSERT_NO_THROW(m_ostm[ostmch]->TargetRead(sideband, start_addr_pe, rdata, size));
              uint32_t index_8 = (start_addr_pe) & 0x3;
              memcpy (&m_data8[index_8], rdata, sizeof(uint8_t));
              ASSERT_EQ(expect, m_data8[index_8]);
            }else if (size == SIZE_16BIT){
              ASSERT_NO_THROW(m_ostm[ostmch]->TargetRead(sideband, start_addr_pe, rdata, size));
              memcpy (&m_data16[0], rdata, sizeof(m_data16));
              ASSERT_EQ(expect, m_data16[0] );
            }else if (size == SIZE_32BIT){
              ASSERT_NO_THROW(m_ostm[ostmch]->TargetRead(sideband, start_addr_pe, rdata, size));
              memcpy (&m_data32, rdata, sizeof(m_data32));
              ASSERT_EQ(expect, m_data32);
            }
          }
        }
      }
  delete m_ostm[ostmch];
  }
}


std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable[] = {
  /* 0000 */ std::make_tuple(0xFFEC0004 /* CNT */, 0xf3f2ffff, SIZE_16BIT, SIZE_16BIT, 0x0), //16bit access
  /* 0000 */ std::make_tuple(0xFFEC0006 /* CNT */, 0xf3f2ffff, SIZE_16BIT, SIZE_16BIT, 0x0), //16bit access
  /* 0000 */ std::make_tuple(0xFFEC0004 /* CNT */, 0xf3f2ffff, SIZE_8BIT, SIZE_8BIT, 0x0), //8bit access
  /* 0000 */ std::make_tuple(0xFFEC0004 /* CNT */, 0xf3f2ffff, SIZE_32BIT, SIZE_32BIT, 0xF3F2FFFF), //32bit access
  /* 0000 */ std::make_tuple(0xFFEC0005 /* CNT */, 0xf3f2ffff, SIZE_8BIT, SIZE_8BIT, 0x0), //8bit access
  /* 0000 */ std::make_tuple(0xFFEC0006 /* CNT */, 0xf3f2ffff, SIZE_8BIT, SIZE_8BIT, 0x0), //8bit access
  /* 0000 */ std::make_tuple(0xFFEC0007 /* CNT */, 0xf3f2ffff, SIZE_8BIT, SIZE_8BIT, 0x0), //8bit access
  // please write more more more test case.
};

INSTANTIATE_TEST_CASE_P(CheckOstmRegReadWrite, OstmRegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable));

class OstmFuncTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
	OstmFuncTest() {}
  virtual ~OstmFuncTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
  }
  virtual void TearDown() {
      delete mock_fsbus;
      delete mock_pe;
      delete cintc2;

  };

public:
  MockFsBus* mock_fsbus = NULL;
  SideBand sideband;
  MockProcElement *mock_pe;
  MockCIntc1 *intc1 = NULL;
  MockCIntc2 *cintc2 = NULL;
  uint32_t pe_id = 0;
  uint32_t ostmch = 0;
  static const uint32_t MAX_OSTM_NUM = 7;
  OstmIO* m_ostm[MAX_OSTM_NUM];
  static const uint32_t INIT_OSTM_RATIO = 1;
  PhysAddr start_addr = 0xFFEC0004;
  PhysAddr base_addr = 0xFFEC0000;
  uint32_t intch = 25;
  uint8_t data[8] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };

  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_F(OstmFuncTest, CheckOstmFunction_TE_1) {
  int32_t temp = 0xF;  //TS = 1
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);

  for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
      pe_id = ostmch;
      PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
      PhysAddr start_addr_pe = start_addr + 0x1000*ostmch;
      m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);
      m_ostm[ostmch]->HardReset();
      EXPECT_CALL((*mock_fsbus), AddCyclicHandler (m_ostm[ostmch]->GetOstmCore()->GetOstmIO())).Times(AtLeast(0));

      m_ostm[ostmch]->GetOstmCore()->SetOSTMnCNT(0xB);
      m_ostm[ostmch]->GetOstmCore()->SetOSTMnTE(0x1);
      ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe, wdata, SIZE_32BIT));

      ASSERT_EQ(0xBU, m_ostm[ostmch]->GetOstmCore()->GetOSTMnCNT());

      delete m_ostm[ostmch];
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
