/*
 * @file test_tregintc2.cpp
 *
 * @date Created on: Dec 07, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for TRegIntc2.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

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

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;

class TRegIntc2Test : public testing::Test {
protected:
  TRegIntc2Test() {}
  virtual ~TRegIntc2Test() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);
    treg_intc2 = new TRegIntc2(cintc2);
    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete treg_intc2;
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
  }

public:
  uint32_t pe_id = 0;
  uint32_t channel = 32;
  TRegIntc2* treg_intc2;
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  PhysAddr EIC_OFFSET  = 0x00000000000UL;
  PhysAddr EIBD_OFFSET = 0x0000002000UL;
  PhysAddr IMR_OFFSET  = 0x0000001000UL;
  PhysAddr EEIC_OFFSET  = 0x0000004000UL;
  PhysAddr EIBG_OFFSET  = 0x0000001FE0UL;
  uint32_t eic_base_addr = 0xFFF80000;
  uint32_t eibd_base_addr = 0xFFF82000;

private:
};


/*!
 * @brief Check TRegIntc2 TrEIC operation.
 */
TEST_F(TRegIntc2Test, Check_TrEIC) {
    SCOPED_TRACE("Check_TrEIC");
    uint32_t addr = 0xFFF80040;
    EXPECT_NE(nullptr, treg_intc2->TrEIC(addr));
}


/*!
 * @brief Check TRegIntc2 TrEIBD operation.
 */
TEST_F(TRegIntc2Test, Check_TrEIBD) {
    SCOPED_TRACE("Check_TrEIBD");
    uint32_t addr = 0xFFF82080;
    EXPECT_NE(nullptr, treg_intc2->TrEIBD(addr));
}


/*!
 * @brief Check TRegIntc2 TrEICAddr operation.
 */
TEST_F(TRegIntc2Test, Check_TrEICAddr) {
    SCOPED_TRACE("Check_TrEICAddr");
    EXPECT_EQ(EIC_OFFSET+2*channel, treg_intc2->TrEICAddr(channel));
}


/*!
 * @brief Check TRegIntc2 TrEEICAddr operation.
 */
TEST_F(TRegIntc2Test, Check_TrEEICAddr) {
    SCOPED_TRACE("Check_TrEEICAddr");
    EXPECT_EQ(EEIC_OFFSET+4*channel, treg_intc2->TrEEICAddr(channel));
}

/*!
 * @brief Check TRegIntc2 TrEIBGAddr operation.
 */
TEST_F(TRegIntc2Test, Check_TrEIBGAddr) {
    SCOPED_TRACE("Check_TrEIBGAddr");
    for (uint32_t peid = 0; peid< TRegIntc2::N_G4MH20_EIBG; peid++){
        EXPECT_EQ(EIBG_OFFSET+4*peid, treg_intc2->TrEIBGAddr(peid));
    }
}

/*!
 * @brief Check TRegIntc2 TrEIBDAddr operation.
 */
TEST_F(TRegIntc2Test, Check_TrEIBDAddr) {
    SCOPED_TRACE("Check_TrEIBDAddr");
    EXPECT_EQ(EIBD_OFFSET+4*channel, treg_intc2->TrEIBDAddr(channel));
}

/*!
 * @brief Check TRegIntc2 InitTRegMap operation.
 */
TEST_F(TRegIntc2Test, Check_InitTRegMap) {
    SCOPED_TRACE("Check_InitTRegMap");
    cintc2->HardReset();
    PhysAddr eic_addr = EIC_OFFSET;
    uint32_t eic_num = 2048;
    for (uint32_t i = 32; i < eic_num; i++) {
        ASSERT_NE(nullptr, treg_intc2->TrEIC(eic_addr + i*2));
    }

    PhysAddr eibd_addr = EIBD_OFFSET;
    uint32_t eibd_num = 2048;
    for (uint32_t i = 32; i < eibd_num; i++) {
        ASSERT_NE(nullptr, treg_intc2->TrEIBD(eibd_addr + i*4));
    }
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
