/**
 * @file test_treg_crgerrreg_20.cpp
 *
 * @date Created on: 2016
 * @author Author: Hoanh Ho <hoanh.ho.px@renesas.com>
 * @brief UNIT TEST for GKCERRADDR. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "treg_guard.h"
#include "crg.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

// using ::testing::AtLeast;
// using ::testing::_;
// using ::testing::Return;

class TRegCrgErrReg;

// ----------------------------------------
class TRegCrgErrReg2 : public testing::Test {
protected:
    TRegCrgErrReg2() {}
    virtual ~TRegCrgErrReg2() {}
    virtual void SetUp() {
        m_treg_crgerrreg   = new TRegCrgErrReg (base_crgerrreg, CORE_G4MH, CORE_VER_20);
    }
    virtual void TearDown() { delete m_treg_crgerrreg; }

public:
    TRegCrgErrReg   *m_treg_crgerrreg;
    MockSideBand sideband;

public:
    uint32_t base_crgerrreg = 0xFFC6D800;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegCrgErrReg2, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_treg_crgerrreg->GetCrgErrStat());
    ASSERT_EQ(base_crgerrreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, m_treg_crgerrreg->GetCrgErrStat()->GetRegAddr());
    ASSERT_STREQ("CRGOVFSTAT", m_treg_crgerrreg->GetCrgErrStat()->GetName());

    ASSERT_NE(nullptr, m_treg_crgerrreg->GetCrgErrClr());
    ASSERT_EQ(base_crgerrreg + TRegCrgErrReg::CRGERRCLR_OFFSET, m_treg_crgerrreg->GetCrgErrClr()->GetRegAddr());
    ASSERT_STREQ("CRGOVFCLR", m_treg_crgerrreg->GetCrgErrClr()->GetName());

    ASSERT_NE(nullptr, m_treg_crgerrreg->GetCrgErrAddr());
    ASSERT_EQ(base_crgerrreg + TRegCrgErrReg::CRGERRADDR_OFFSET, m_treg_crgerrreg->GetCrgErrAddr()->GetRegAddr());
    ASSERT_STREQ("CRGERRADDR", m_treg_crgerrreg->GetCrgErrAddr()->GetName());

    ASSERT_NE(nullptr, m_treg_crgerrreg->GetCrgErrType());
    ASSERT_EQ(base_crgerrreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, m_treg_crgerrreg->GetCrgErrType()->GetRegAddr());
    ASSERT_STREQ("CRGERRTYPE", m_treg_crgerrreg->GetCrgErrType()->GetName());

    ASSERT_NE(nullptr, m_treg_crgerrreg->GetCrgSpidErrClr());
    ASSERT_EQ(base_crgerrreg + TRegCrgErrReg::CRGSPIDERRCLR_OFFSET, m_treg_crgerrreg->GetCrgSpidErrClr()->GetRegAddr());
    ASSERT_STREQ("CRGSPIDERRCLR", m_treg_crgerrreg->GetCrgSpidErrClr()->GetName());

    ASSERT_NE(nullptr, m_treg_crgerrreg->GetCrgSpidErrStat());
    ASSERT_EQ(base_crgerrreg + TRegCrgErrReg::CRGSPIDERRSTAT_OFFSET, m_treg_crgerrreg->GetCrgSpidErrStat()->GetRegAddr());
    ASSERT_STREQ("CRGSPIDERRSTAT", m_treg_crgerrreg->GetCrgSpidErrStat()->GetName());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
