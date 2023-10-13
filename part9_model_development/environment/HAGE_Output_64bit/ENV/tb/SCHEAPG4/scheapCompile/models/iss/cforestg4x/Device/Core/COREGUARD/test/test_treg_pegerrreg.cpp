/**
 * @file test_treg_pegerrreg.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for PEG error register. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "treg_guard.h"
#include "peg.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

// using ::testing::AtLeast;
// using ::testing::_;
// using ::testing::Return;

class TRegPegErrReg;

// ----------------------------------------
class TRegPegErrReg1 : public testing::Test {
protected:
    TRegPegErrReg1() {}
    virtual ~TRegPegErrReg1() {}
    virtual void SetUp() {
        m_treg_pegerrreg   = new TRegPegErrReg (base_pegerrreg, CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_treg_pegerrreg; }

public:
    TRegPegErrReg   *m_treg_pegerrreg;
    MockSideBand sideband;

public:
    uint32_t base_pegerrreg = 0xFFC6C800;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegPegErrReg1, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_treg_pegerrreg->GetPegErrStat());
    ASSERT_EQ(base_pegerrreg + TRegPegErrReg::PEGERRSTAT_OFFSET, m_treg_pegerrreg->GetPegErrStat()->GetRegAddr());
    ASSERT_STREQ("PEGERRSTAT", m_treg_pegerrreg->GetPegErrStat()->GetName());

    ASSERT_NE(nullptr, m_treg_pegerrreg->GetPegErrClr());
    ASSERT_EQ(base_pegerrreg + TRegPegErrReg::PEGERRCLR_OFFSET, m_treg_pegerrreg->GetPegErrClr()->GetRegAddr());
    ASSERT_STREQ("PEGERRCLR", m_treg_pegerrreg->GetPegErrClr()->GetName());

    ASSERT_NE(nullptr, m_treg_pegerrreg->GetPegErrAddr());
    ASSERT_EQ(base_pegerrreg + TRegPegErrReg::PEGERRADDR_OFFSET, m_treg_pegerrreg->GetPegErrAddr()->GetRegAddr());
    ASSERT_STREQ("PEGERRADDR", m_treg_pegerrreg->GetPegErrAddr()->GetName());

    ASSERT_NE(nullptr, m_treg_pegerrreg->GetPegErrType());
    ASSERT_EQ(base_pegerrreg + TRegPegErrReg::PEGERRTYPE_OFFSET, m_treg_pegerrreg->GetPegErrType()->GetRegAddr());
    ASSERT_STREQ("PEGERRTYPE", m_treg_pegerrreg->GetPegErrType()->GetName());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
