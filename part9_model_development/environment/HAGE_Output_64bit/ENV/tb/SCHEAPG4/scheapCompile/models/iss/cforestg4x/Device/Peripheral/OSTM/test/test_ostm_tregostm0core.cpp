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


class OstmFunctionTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
	OstmFunctionTest() {}
  virtual ~OstmFunctionTest() {}
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
  PhysAddr base_addr = 0xFFEC0000;
  PhysAddr start_addr_TS = 0xFFEC0014;
  PhysAddr start_addr_CTL = 0xFFEC0020;

  uint32_t intch = 25;
  uint8_t data[8] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };

  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

TEST_F(OstmFunctionTest, SetGetOSTM0CNT) {
  for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
      pe_id = ostmch;
      PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
      m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);
      m_ostm[ostmch]->HardReset();

      m_ostm[ostmch]->GetOstmCore()->SetOSTMnCNT(0x0A);
      ASSERT_EQ(0x0AU,m_ostm[ostmch]->GetOstmCore()->GetOSTMnCNT());

      delete m_ostm[ostmch];
  }
}

TEST_F(OstmFunctionTest, SetGetOSTM0TE) {
  for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
      pe_id = ostmch;
      PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
      m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);
      m_ostm[ostmch]->HardReset();

      m_ostm[ostmch]->GetOstmCore()->SetOSTMnTE(0x0A);
      ASSERT_EQ(0x0AU,m_ostm[ostmch]->GetOstmCore()->GetOSTMnTE());

      delete m_ostm[ostmch];
  }

}

TEST_F(OstmFunctionTest, SetGetOSTM0CTL) {
  for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
      pe_id = ostmch;
      PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
      m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);
      m_ostm[ostmch]->HardReset();

      m_ostm[ostmch]->GetOstmCore()->SetOSTMnCTL(0x0A);
      ASSERT_EQ(0xAU,m_ostm[ostmch]->GetOstmCore()->GetOSTMnCTL());

      delete m_ostm[ostmch];
  }

}

TEST_F(OstmFunctionTest, SetGetOSTM0CMP) {
  for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
      pe_id = ostmch;
      PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
      m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);
      m_ostm[ostmch]->HardReset();

      m_ostm[ostmch]->GetOstmCore()->SetOSTMnCMP(0x0A);
      ASSERT_EQ(0xAU,m_ostm[ostmch]->GetOstmCore()->GetOSTMnCMP());

      delete m_ostm[ostmch];
  }

}

TEST_F(OstmFunctionTest, CheckIssueInt) {
  int32_t temp = 0x1;  //TS = 1
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);

  int32_t temp1 = 0x87;  //int enable = 1
  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);

  for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
      pe_id = ostmch;
      PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
      PhysAddr start_addr_pe_TS = start_addr_TS + 0x1000*ostmch;
      PhysAddr start_addr_pe_CTL = start_addr_CTL + 0x1000*ostmch;
      m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);
      m_ostm[ostmch]->HardReset();
      EXPECT_CALL((*mock_fsbus), AddCyclicHandler (m_ostm[ostmch]->GetOstmCore()->GetOstmIO())).Times(AtLeast(0));

      ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe_CTL, wdata1, SIZE_8BIT));
      ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe_TS, wdata, SIZE_8BIT));

      ASSERT_EQ(0x1U,m_ostm[ostmch]->GetOstmCore()->GetMD0Status());
      ASSERT_EQ(0x87U,m_ostm[ostmch]->GetOstmCore()->GetOSTMnCTL());
      ASSERT_EQ(0x1U,m_ostm[ostmch]->GetOstmCore()->GetMD1Status());
      ASSERT_EQ(0x1U,m_ostm[ostmch]->GetOstmCore()->GetIntEnabledStatus());

      delete m_ostm[ostmch];
  }
}

TEST_F(OstmFunctionTest, CheckIssueInt1) {
  int32_t temp = 0x1;  //TS = 1
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);

  int32_t temp1 = 0x85;  //int enable = 1
  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);

  for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
      pe_id = ostmch;
      PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
      PhysAddr start_addr_pe_TS = start_addr_TS + 0x1000*ostmch;
      PhysAddr start_addr_pe_CTL = start_addr_CTL + 0x1000*ostmch;
      m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);

      OstmCore* m_ostm0_core = new OstmCore(pe_id, intc1, cintc2, intch, ostmch, mock_fsbus, m_ostm[ostmch], INIT_OSTM_RATIO);
              TRegOSTMnCMP* OSTM0CMP = new TRegOSTMnCMP(m_ostm0_core);

              OSTM0CMP->Reset();

      m_ostm[ostmch]->HardReset();
      EXPECT_CALL((*mock_fsbus), AddCyclicHandler (m_ostm[ostmch]->GetOstmCore()->GetOstmIO())).Times(AtLeast(0));

      ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe_CTL, wdata1, SIZE_8BIT));
      ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe_TS, wdata, SIZE_8BIT));

      ASSERT_EQ(0x1U,m_ostm[ostmch]->GetOstmCore()->GetMD0Status());
      ASSERT_EQ(0x85U,m_ostm[ostmch]->GetOstmCore()->GetOSTMnCTL());
      ASSERT_EQ(0x0U,m_ostm[ostmch]->GetOstmCore()->GetMD1Status());
      ASSERT_EQ(0x1U,m_ostm[ostmch]->GetOstmCore()->GetIntEnabledStatus());

      EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (m_ostm[ostmch]->GetOstmCore()->GetOstmIO())).Times(AtLeast(0));
      ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe_TS, wdata, SIZE_8BIT));


      delete m_ostm[ostmch];
  }
}

TEST_F(OstmFunctionTest, CheckIssueInt2) {
  int32_t temp = 0x1;  //TS = 1
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);

  int32_t temp1 = 0x87;  //int enable = 1
  uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&temp1);

  uint32_t data_input = 0xffffffff;
  uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&data_input);

  for (ostmch = 0; ostmch < MAX_OSTM_NUM; ostmch++){
      pe_id = ostmch;
      PhysAddr base_addr_pe = base_addr + 0x1000*ostmch;
      PhysAddr start_addr_pe_TS = start_addr_TS + 0x1000*ostmch;
      PhysAddr start_addr_pe_CTL = start_addr_CTL + 0x1000*ostmch;
      m_ostm[ostmch] = new OstmIO(pe_id, intc1, cintc2, base_addr_pe, intch, ostmch, mock_fsbus, INIT_OSTM_RATIO);

      OstmCore* m_ostm0_core = new OstmCore(pe_id, intc1, cintc2, intch, ostmch, mock_fsbus, m_ostm[ostmch], INIT_OSTM_RATIO);
      TRegOSTMnCMP* OSTM0CMP = new TRegOSTMnCMP(m_ostm0_core);

      OSTM0CMP->Reset();

      m_ostm[ostmch]->HardReset();
      EXPECT_CALL((*mock_fsbus), AddCyclicHandler (m_ostm[ostmch]->GetOstmCore()->GetOstmIO())).Times(AtLeast(0));



      ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe_CTL, wdata1, SIZE_8BIT));
      ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe_TS, wdata, SIZE_8BIT));
      OSTM0CMP->Write(sideband, base_addr_pe, wdata2, SIZE_32BIT);



      ASSERT_EQ(0x1U,m_ostm[ostmch]->GetOstmCore()->GetMD0Status());
      ASSERT_EQ(0x87U,m_ostm[ostmch]->GetOstmCore()->GetOSTMnCTL());
      ASSERT_EQ(0x1U,m_ostm[ostmch]->GetOstmCore()->GetMD1Status());
      ASSERT_EQ(0x1U,m_ostm[ostmch]->GetOstmCore()->GetIntEnabledStatus());

      EXPECT_CALL((*mock_fsbus), AddCyclicHandler (m_ostm[ostmch]->GetOstmCore()->GetOstmIO())).Times(AtLeast(0));

      ASSERT_NO_THROW(m_ostm[ostmch]->TargetWrite(sideband, start_addr_pe_TS, wdata, SIZE_8BIT));
      OSTM0CMP->Write(sideband, base_addr_pe, wdata2, SIZE_32BIT);

      delete m_ostm[ostmch];
      delete m_ostm0_core;
      delete OSTM0CMP;
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
