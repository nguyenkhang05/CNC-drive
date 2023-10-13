/**
 * @file test_treg_crgctrlreg_20.cpp
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

class TRegCrgCtrlReg;

// ----------------------------------------
class TRegCrgCtrlReg1 : public testing::Test {
protected:
    TRegCrgCtrlReg1() {}
    virtual ~TRegCrgCtrlReg1() {}
    virtual void SetUp() {
        GuardInternal * guard_internal= new GuardInternal(false, false,
                                                              0x13, 0x8, 0x8, 0x4, 0xA5A5A500, 0x0);
        m_treg_crgctrlreg   = new TRegCrgCtrlReg (base_crgctrlreg, base_crgreg_ivc, CORE_G4MH, CORE_VER_20, guard_internal);
    }
    virtual void TearDown() { delete m_treg_crgctrlreg; }

public:
    TRegCrgCtrlReg   *m_treg_crgctrlreg;
    MockSideBand sideband;

public:
    uint32_t base_crgctrlreg = 0xFFC6D000;
    const uint32_t base_crgreg_ivc = 0xFFC6E800;
    union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
    };
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegCrgCtrlReg1, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_treg_crgctrlreg->GetCrgKceProt());
    ASSERT_EQ(base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, m_treg_crgctrlreg->GetCrgKceProt()->GetRegAddr());
    ASSERT_STREQ("CRGKCEPROT", m_treg_crgctrlreg->GetCrgKceProt()->GetName());

    for (uint32_t chn = 0; chn < GuardInternal_Interface::NUM_CRG_CTRL_CHANNEL; chn++) {
        std::ostringstream crgprot_name;
        crgprot_name << "CRGPROT" << chn;
        ASSERT_NE(nullptr, m_treg_crgctrlreg->GetCrgProt(chn));
        ASSERT_STREQ(crgprot_name.str().c_str(), m_treg_crgctrlreg->GetCrgProt(chn)->GetName());
        ASSERT_EQ(base_crgctrlreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn, m_treg_crgctrlreg->GetCrgProt(chn)->GetRegAddr());

        std::ostringstream crgspid_name;
        crgspid_name << "CRGSPID" << chn;
        ASSERT_NE(nullptr, m_treg_crgctrlreg->GetCrgSpid(chn));
        ASSERT_STREQ(crgspid_name.str().c_str(), m_treg_crgctrlreg->GetCrgSpid(chn)->GetName());
        ASSERT_EQ(base_crgctrlreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn, m_treg_crgctrlreg->GetCrgSpid(chn)->GetRegAddr());

        std::ostringstream crgbad_name;
        crgbad_name << "CRGBAD" << chn;
        ASSERT_NE(nullptr, m_treg_crgctrlreg->GetCrgBad(chn));
        ASSERT_STREQ(crgbad_name.str().c_str(), m_treg_crgctrlreg->GetCrgBad(chn)->GetName());
        ASSERT_EQ(base_crgctrlreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn, m_treg_crgctrlreg->GetCrgBad(chn)->GetRegAddr());

        std::ostringstream crgadv_name;
        crgadv_name << "CRGADV" << chn;
        ASSERT_NE(nullptr, m_treg_crgctrlreg->GetCrgAdv(chn));
        ASSERT_STREQ(crgadv_name.str().c_str(), m_treg_crgctrlreg->GetCrgAdv(chn)->GetName());
        ASSERT_EQ(base_crgctrlreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn, m_treg_crgctrlreg->GetCrgAdv(chn)->GetRegAddr());
    }
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
