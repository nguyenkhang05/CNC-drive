/*
 * test_eiintreqinfo.cpp
 *
 * @date Created on: Dec 10, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for EiintReqInfo.
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

class EiintReqInfoTest : public testing::Test {
protected:
  EiintReqInfoTest() {}
  virtual ~EiintReqInfoTest() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);
    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));

    eic  = new TRegEIC (SIZE_16BIT, SIZE_8BIT, 0xFFF80040, 32, pe_id, cintc2);
    eeic  = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, 0xFFF80740, 32, pe_id, cintc2);
    TRegMask mask = TRegMask (TRegBND<EIBD>::INTC2_EIBD_MASK_G4MH20);
    eibd = new TRegEIBD(SIZE_32BIT, SIZE_8BIT, 0xFFF82040, mask, 32, pe_id, cintc2, INTC2_G4MH20);
    dbmk = new MockTRegDBMK (SIZE_16BIT, SIZE_8BIT, 0xFFFC0080, cintc2);

    eibg = new TRegEIBG (SIZE_32BIT, SIZE_8BIT, 0xFFF81FE0, TRegMask (TRegEIBG::EIBG_MASK_INTC2),0,cintc2);
    m_test_eiint = new EiintReqInfo(eic, eeic, eibd, dbmk, eibg);
  }
  virtual void TearDown() {
    delete m_test_eiint;
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
    delete eic,
    delete eeic,
    delete eibd;
    delete dbmk;
  }

public:
  uint32_t pe_id = 0;
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  TRegEIC* eic;
  TRegEEIC* eeic;
  TRegEIBD* eibd;
  MockTRegDBMK* dbmk;
  TRegEIBG* eibg;
  EiintReqInfo* m_test_eiint;

private:
};


/*!
 * @brief Check EiintReqInfoTest IsLevelDetect operation.
 */
TEST_F(EiintReqInfoTest, Check_IsLevelDetect) {
    SCOPED_TRACE("Check_IsLevelDetect");
    eic->SetEICT(1);
    bool is_lvl = m_test_eiint->IsLevelDetect();
    EXPECT_EQ(true, is_lvl);
    eic->SetEICT(0);
    is_lvl = m_test_eiint->IsLevelDetect();
    EXPECT_NE(true, is_lvl);
}


/*!
 * @brief Check EiintReqInfoTest GetMask operation.
 */
TEST_F(EiintReqInfoTest, Check_GetMask) {
    SCOPED_TRACE("Check_GetMask");
    uint32_t mask = 1;
    uint32_t un_mask = 0;
    EXPECT_CALL((*dbmk), SetEI(_)).Times(AtLeast(0));
    EXPECT_CALL((*dbmk), GetEI()).Times(AtLeast(0));

    eic->SetEIMK(0);
    dbmk->SetEI(0);
    uint32_t result = m_test_eiint->GetMask();
    EXPECT_EQ(un_mask, result);

    eic->SetEIMK(0);
    dbmk->SetEI(1);
    result = m_test_eiint->GetMask();
    EXPECT_EQ(un_mask, result);

    eic->SetEIMK(1);
    dbmk->SetEI(0);
    result = m_test_eiint->GetMask();
    EXPECT_EQ(mask, result);

    eic->SetEIMK(1);
    dbmk->SetEI(1);
    result = m_test_eiint->GetMask();
    EXPECT_EQ(mask, result);
}


/*!
 * @brief Check EiintReqInfoTest GetEITB operation.
 */
TEST_F(EiintReqInfoTest, Check_GetEITB) {
    SCOPED_TRACE("Check_GetEITB");
    eic->SetEITB(1);
    bool is_tbint = m_test_eiint->GetEITB();
    EXPECT_EQ(true, is_tbint);
    eic->SetEITB(0);
    is_tbint = m_test_eiint->GetEITB();
    EXPECT_NE(true, is_tbint);
}


/*!
 * @brief Check EiintReqInfoTest GetPriority operation.
 */
TEST_F(EiintReqInfoTest, Check_GetPriority) {
    SCOPED_TRACE("Check_GetPriority");
    for (int32_t i = 0; i < 16; i++) {
        ASSERT_NO_THROW(eic->SetEIP(i));
        ASSERT_NO_THROW(eeic->SetEIP(i));
        int32_t priority = m_test_eiint->GetPriority();
        ASSERT_EQ(i, priority);
    }
}


/*!
 * @brief Check EiintReqInfoTest GetCST operation.
 */
TEST_F(EiintReqInfoTest, Check_GetCST) {
    SCOPED_TRACE("Check_GetCST");
    eibd->SetCST(1);
    uint32_t is_bcas = m_test_eiint->GetCST();
    EXPECT_EQ(0x1U, is_bcas);
    eibd->SetCST(0);
    is_bcas = m_test_eiint->GetCST();
    EXPECT_EQ(0x0U, is_bcas);
}


/*!
 * @brief Check EiintReqInfoTest GetBCP operation.
 */
TEST_F(EiintReqInfoTest, Check_GetBCP) {
    SCOPED_TRACE("Check_GetBCP");
    eibd->SetBCP(0);
    uint32_t bcp = m_test_eiint->GetBCP();
    EXPECT_EQ(0x0U, bcp);
    eibd->SetBCP(1);
    bcp = m_test_eiint->GetBCP();
    EXPECT_EQ(0x1U, bcp);
    eibd->SetBCP(2);
    bcp = m_test_eiint->GetBCP();
    EXPECT_EQ(0x2U, bcp);
    eibd->SetBCP(3);
    bcp = m_test_eiint->GetBCP();
    EXPECT_EQ(0x3U, bcp);
}


/*!
 * @brief Check EiintReqInfoTest GetPEID operation.
 */
TEST_F(EiintReqInfoTest, Check_GetPEID) {
    SCOPED_TRACE("Check_GetPEID");
    eibd->SetPEID(0);
    uint32_t peid = m_test_eiint->GetPEID();
    EXPECT_EQ(0x0U, peid);
    eibd->SetPEID(15);
    peid = m_test_eiint->GetPEID();
    EXPECT_EQ(0x7U, peid);
}


/*!
 * @brief Check EiintReqInfoTest GetTcId operation.
 */
TEST_F(EiintReqInfoTest, Check_GetTcId) {
    SCOPED_TRACE("Check_GetTcId");
    ContextId tcid = m_test_eiint->GetTcId();
    EXPECT_EQ(NC, tcid);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

